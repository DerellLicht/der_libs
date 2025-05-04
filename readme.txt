A cursory overview of der_libs
The derelict's common code
====================================================

1. common_funcs.cpp (common.h)
This is a selection of common functions that I use regularly
in day-to-day code.  The functions included here are intended
for console-based applications; The only Windows-oriented
header file that it includes is windows.h

2. common_win.cpp (commonw.h)
This is a selection of common functions that I use regularly
in day-to-day code.  The functions included here are intended
for Windows-based applications; the common-controls and
ShellAPI interfaces are represented, as well as Windows functions.

3. gdi_plus.cpp (gdi_plus.h), gdiplus_setup.cpp (gdiplus_setup.h)
This is a wrapper class around the GDI+ graphics library.
It is intended to simplify access to common GDI+ operations.

All the other included modules are C++ classes which encapsulate
various operations that I regularly use in WinAPI applications.
Most of theses modules include a text file which discusses the
purpose and use of the class.

Obtain the current version of this package:
git clone https://github.com/DerellLicht/der_libs.git


