# der_libs\release.mak -- shared GitHub release plumbing, included by each
# project's top-level Makefile. Assumes CHANGELOG.md lives in the project
# root with "## [x.y]" version headers, and that "gh" (GitHub CLI) is
# authenticated for this repo. GitHub-only by design -- PrettyReMark stays
# on GitLab as a standalone exception (fork continuity) and is not part of
# this include.
#
# Usage in a project Makefile:
#   include der_libs\release.mak
#   ...
#   DIST_ZIP := $(BASE)V$(VERSION).zip
#   # RELEASE_ASSETS defaults to "./$(DIST_ZIP) ./CHANGELOG.md" below;
#   # override after the include only if a project ships something else.
#
#   dist:
#   	rm -f *.zip
#   	zip $(DIST_ZIP) $(BINX) readme.md LICENSE.txt CHANGELOG.md
#
# That's all a project needs to supply -- check-clean, notes, release,
# update, retag, re-release, and sha256 all come from here.

# Most recent "## [x.y]" header in CHANGELOG.md, e.g. "## [1.16]" -> 1.16
VERSION := $(shell grep -oE '\[[0-9]+\.[0-9]+\]' CHANGELOG.md | head -n 1 | tr -d '[]')
TAG := v$(VERSION)

# Default asset list for release/update. Recursively expanded (plain '?='),
# so it's safe to reference here even though DIST_ZIP is normally defined
# by the project Makefile *after* this include -- it's only resolved when
# release/update actually run, by which point DIST_ZIP is set.
RELEASE_ASSETS ?= ./$(DIST_ZIP) ./CHANGELOG.md

# This file is normally included near the top of a project Makefile,
# before that Makefile's own "all:" target is defined -- which means
# check-clean below, being the first target GNU Make sees, would otherwise
# silently become the default goal for a bare "make" with no arguments
# instead of "all". Restore the intended default explicitly. NOTE: '?='
# does NOT work for .DEFAULT_GOAL -- GNU Make pre-seeds it internally, so
# '?=' sees it as already set and silently skips the assignment; this
# explicit empty-check is the actual working equivalent (verified against
# GNU Make 4.3). A project can still override by setting .DEFAULT_GOAL
# itself before this include, if its primary target isn't named "all".
ifeq ($(.DEFAULT_GOAL),)
.DEFAULT_GOAL := all
endif

.PHONY: check-clean notes release update retag re-release sha256

# Blocks release/retag on an uncommitted working tree -- catches building
# from a tree that doesn't match what the tag is about to point at. Runs
# before the (slow) dist rebuild.
check-clean:
	@if [ -n "$$(git status --porcelain)" ]; then \
		echo "ERROR: uncommitted changes present -- commit before releasing."; \
		git status --short; \
		exit 1; \
	fi

# Slices this version's section out of CHANGELOG.md into temp_notes.md, for
# release/update to hand to gh via --notes-file.
notes:
	sed -n '/## \[$(VERSION)\]/,/## \[/p' CHANGELOG.md | sed '$$d' > temp_notes.md

release: check-clean dist notes
	@cmd /C "@echo Preparing GitHub release for $(TAG)..."
	gh release create $(TAG) $(RELEASE_ASSETS) --notes-file temp_notes.md
	rm temp_notes.md
	@cmd /C "@echo Release $(TAG) successfully uploaded to GitHub!"

# Notes first, then assets ("--clobber" makes the asset re-upload safe with
# no stale-link cleanup needed). "gh release edit" fails if the release
# doesn't exist yet -- unlike "release" this is NOT self-healing, so run
# "release" first for a tag's first publish.
update: dist notes
	@cmd /C "@echo Updating release $(TAG)..."
	gh release edit $(TAG) --notes-file temp_notes.md
	rm temp_notes.md
	gh release upload $(TAG) $(RELEASE_ASSETS) --clobber
	@cmd /C "@echo Release $(TAG) assets and notes successfully updated on GitHub!"

# Recovery for "released with pending changes left out of the tag":
# force-moves $(TAG) to HEAD and re-pushes, then "update" re-releases
# against it. Solo-repo only -- force-pushing a moved tag is unsafe if
# anyone else has already fetched it. Deliberately separate from "release",
# which lets gh create the tag itself (no explicit git tag), so a plain
# release never force-moves anything.
retag: check-clean
	@if git rev-parse $(TAG) >/dev/null 2>&1; then \
		echo "Retagging existing tag $(TAG)."; \
	else \
		echo "Note: $(TAG) doesn't exist yet -- this will be its first release."; \
	fi
	git tag -f $(TAG)
	git push origin $(TAG) --force

re-release: retag update
	@cmd /C "@echo Release $(TAG) retagged and re-released."

# Not called by anything above -- run by hand when a distribution channel
# asks for a published checksum to verify the release matches what you claim it is 
# (came up for PrettyReMark's "awesome-markdown-editors" list submission).
sha256:
	certutil -hashfile $(DIST_ZIP) SHA256

#************************************************************
# Optional Inno Setup installer support (purely additive).
#
# Nothing below touches the targets above, and a project that never
# mentions these names is completely unaffected. Target names are
# deliberately NOT "setup"/"install"/"single" (PrettyReMark's names) so
# they cannot collide with a same-named target in any existing project.
#
# The defaults below mirror PrettyReMark's layout:
#   Output/<BASE>V<VERSION>.setup.exe   built by iscc from <BASE>.iss
#   Output/<BASE>V<VERSION>.setup.zip   the exe zipped (what gets uploaded)
#
# CONTRACT with the .iss file: it must take /DMyAppVersion=x.y from the
# command line, and set OutputDir=Output and
#   OutputBaseFilename=<BASE>V{#MyAppVersion}.setup
# (iscc appends ".exe") so the file lands exactly at $(SETUP_EXE).
#
# Variables are only expanded when a recipe runs, so it does not matter
# that BASE is defined by the project Makefile *after* this include. But
# PREREQUISITES are expanded at parse time, which is why "installer" has no
# $(BIN) prerequisite here -- the project adds it with a recipe-less rule:
#   installer: $(BIN)
# The generic target names below (installer, installer-zip, install-silent)
# are chosen NOT to collide with legacy projects. A project that wants
# PrettyReMark's names simply aliases them in its own Makefile, e.g.
# (wbigcalc does exactly this when its USE_INNO = YES; with USE_INNO = NO it
# keeps a loose-files "dist" and empty "setup"/"install" instead):
#   setup:   installer
#   dist:    installer-zip
#   install: install-silent
# release/update below depend on "dist", so a project whose "dist" is
# installer-zip publishes the installer. It also sets, after the include:
#   RELEASE_ASSETS = ./$(SETUP_ZIP)
#   DIST_ZIP = $(SETUP_ZIP)       (so the plain "sha256" target below
#                                  checksums the installer zip)
#
# PRETTYREMARK (Makefile.prm.gh) -> GENERIC NAMES HERE
#
#   PRM command        Generic name here          Notes
#   -----------------  -------------------------  ---------------------------
#   make single        (plain "make")             Builds $(BIN). A C/C++ project
#                                                 has no separate "single"
#                                                 step; the project's recipe-
#                                                 less "installer: $(BIN)"
#                                                 rule runs it automatically.
#   make setup         installer                  Runs iscc -> $(SETUP_EXE)
#   make dist          installer-zip              installer + zip of the setup
#                                                 exe -> $(SETUP_ZIP)
#   make install       install-silent             Silent-installs $(SETUP_EXE)
#   make sha256        sha256-setup (or "sha256"  sha256-setup always hits
#                      if DIST_ZIP = $(SETUP_ZIP)) $(SETUP_ZIP)
#   make clean         (project Makefile)         Project's clean should also
#                                                 remove $(SETUP_DIR).
#   make release       release                    Same name, DIFFERENT rules --
#   make update        update                     see the notes just below.
#   make retag         retag                      Identical in effect.
#   make re-release    re-release                 Identical in effect.
#
# release/update differences from PRM:
#   - "release" here lets gh create the tag (no explicit git tag/push), and
#     passes no -R or --title. PRM's tags and pushes explicitly first.
#   - "update" here is NOT self-healing: "gh release edit" fails if the
#     release does not exist yet. Run "release" first for a version's first
#     publish; PRM's "update" creates a missing release itself.
#   - "update" here does not depend on check-clean; PRM's does ("release"
#     and "retag" here do).
#   - Assets uploaded are $(RELEASE_ASSETS), set by each project Makefile.
#     (Default above: $(DIST_ZIP) + CHANGELOG.md. PRM uploads only its setup
#     zip.)
ISCC      ?= iscc
ISS_FILE  ?= $(BASE).iss
SETUP_DIR ?= Output
SETUP_EXE ?= $(SETUP_DIR)/$(BASE)V$(VERSION).setup.exe
SETUP_ZIP ?= $(SETUP_DIR)/$(BASE)V$(VERSION).setup.zip

.PHONY: installer installer-zip install-silent sha256-setup

# PRM equivalent: "make setup" (which also ran "single" first; here the
# project's "installer: $(BIN)" rule does the build step).
# Wipes Output/ first so a stale exe/zip from an older VERSION can never be
# mistaken for the current one.
installer:
	rm -rf $(SETUP_DIR)
	$(ISCC) /DMyAppVersion=$(VERSION) /Q $(ISS_FILE)

# PRM equivalent: "make dist".
# "-j" junks the Output/ path so the zip contains just the setup exe.
installer-zip: installer
	zip -j $(SETUP_ZIP) $(SETUP_EXE)

# PRM equivalent: "make install".
# Silent install from the freshly built exe, for smoke-testing.
install-silent:
	$(SETUP_EXE) /SILENT /SUPPRESSMSGBOXES /NORESTART

# PRM equivalent: "make sha256". (Plain "sha256" above = portable zip.)
sha256-setup:
	certutil -hashfile $(SETUP_ZIP) SHA256
