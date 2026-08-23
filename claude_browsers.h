//************************************************************************
//  claude_browsers.cpp - a collection of file/folder selection functions
//  Copyright (c) 2026  Derell Licht and ClaudeAI
//  
//  This module requires linking with uuid.lib (-uuid)
//************************************************************************

//************************************************************************
// shobjidl.h is included here (in the header, not just the .cpp) because
// BrowseForFile()/BrowseForFileSave() take a COMDLG_FILTERSPEC* parameter,
// and callers building a filter array need the full struct definition, 
// not just a declaration. A forward-declare was considered and rejected:
// COMDLG_FILTERSPEC is a typedef over an internally-tagged (or possibly
// anonymous) struct, so "struct COMDLG_FILTERSPEC;" would silently declare
// an unrelated, incompatible type rather than referring to the real one --
// and the internal tag name isn't guaranteed stable across toolchains
// (mingw-w64 / TDM32 / Cygwin may not all match). Full include is the
// portable choice, at the cost of pulling shobjidl.h into any file that
// includes this header.
//
// Linking anything that uses these functions also requires -luuid --
// CLSID_FileOpenDialog / CLSID_FileSaveDialog / IID_IFileDialog are
// declared extern here but only *defined* in mingw-w64's libuuid.a.
// (The alternative, #define INITGUID before this include, defines them
// locally instead of linking them -- but do NOT do that in a shared
// header/library: if two .cpp files in the same link both end up with
// INITGUID active, you get duplicate-symbol errors.)
//************************************************************************
//  DL: this module also requires -luuid 
#include <shlobj.h>

// bool BrowseForFolder(HWND hwndOwner, std::wstring& outPath);
// bool BrowseForFile(HWND hwndOwner, std::wstring& outPath,
//                     const COMDLG_FILTERSPEC* filters = nullptr,
//                     UINT filterCount = 0);
// bool BrowseForFileSave(HWND hwndOwner, std::wstring& outPath,
//                         const COMDLG_FILTERSPEC* filters = nullptr,
//                         UINT filterCount = 0,
//                         const wchar_t* defaultName = nullptr);
// std::string WideToNarrow(const std::wstring& w);

bool BrowseForFolder(HWND hwndOwner, std::wstring& outPath,
                      const GUID* clientId = nullptr);
bool BrowseForFile(HWND hwndOwner, std::wstring& outPath,
                    const COMDLG_FILTERSPEC* filters = nullptr,
                    UINT filterCount = 0,
                    const GUID* clientId = nullptr);
bool BrowseForFileSave(HWND hwndOwner, std::wstring& outPath,
                        const COMDLG_FILTERSPEC* filters = nullptr,
                        UINT filterCount = 0,
                        const wchar_t* defaultName = nullptr,
                        const GUID* clientId = nullptr);
std::string WideToNarrow(const std::wstring& w);
GUID MakeStableGuidFromLabel(const char* label);

