//************************************************************************
//  claude_browsers.cpp - a collection of file/folder selection functions
//  Copyright (c) 2026  Derell Licht and ClaudeAI
//  
//  This module requires linking with uuid.lib (-uuid)
//************************************************************************
// Claude:
// Yes — IFileDialog is the modern (Vista+) replacement for both GetOpenFileName() 
// and SHBrowseForFolder(), and FOS_PICKFOLDERS is exactly the flag for 
// folder-picking mode. It's COM-based, so there's a bit more ceremony than 
// GetOpenFileName(), but it's much less awkward than SHBrowseForFolder()'s 
// callback-based API, and it gives you the modern Explorer-style 
// dialog (nav pane, address bar, etc.) instead of the old tree view.
//************************************************************************

#include <windows.h>
#include <cstdio>   //  sprintf, needed for double
#include <cstdlib>  //  _MAX_PATH
#include <string>
#include <shobjidl.h>   // IFileDialog, IShellItem

#include "common.h"
#include "commonw.h"
#include "claude_browsers.h"

/*  examples of filters for Claude Browsers

// static TCHAR szPalFilter[]  = TEXT ("Palette Files (*.PLT)\0*.plt\0")  \
//                               TEXT ("All Files (*.*)\0*.*\0\0") ;
// static TCHAR szExecFilter[] = TEXT ("Executable Files (*.EXE)\0*.exe\0")  \
//                               TEXT ("All Files (*.*)\0*.*\0\0") ;
// static TCHAR szDirFilter[]  = TEXT ("All Files (*.*)\0*.*\0\0") ;

const COMDLG_FILTERSPEC PalFilter[] = {
 { L"Palette Files (*.plt)", L"*.plt" },
 { L"All files (*.*)",       L"*.*"   }
};

const COMDLG_FILTERSPEC ExecFilter[] = {
 { L"Executable Files (*.exe)", L"*.exe" },
 { L"All files (*.*)",          L"*.*"   }
};

*/

//************************************************************************
// Derives a stable, reproducible GUID from a caller-supplied string label.
// NOT cryptographically random and NOT guaranteed globally unique -- it's
// only meant to give distinct call sites within one program distinct
// GUIDs for IFileDialog::SetClientGuid() persistence, so Windows can
// remember a separate "last folder used" per call site. The same label
// string always produces the same GUID (required, since the persisted
// registry entry is keyed on the GUID value and must stay stable across
// runs). Pick any short, readable label per call site, e.g. "OpenSource",
// "OpenConfig", "SaveConfig" -- uniqueness within your own program is all
// that matters.
//************************************************************************
static uint64_t Fnv1aHash64(const char* data, size_t len, uint64_t seed)
{
    uint64_t hash = seed;
    for (size_t i = 0; i < len; ++i) {
        hash ^= (unsigned char)data[i];
        hash *= 1099511628211ULL;
    }
    return hash;
}

GUID MakeStableGuidFromLabel(const char* label)
{
    size_t len = strlen(label);
    uint64_t h1 = Fnv1aHash64(label, len, 14695981039346656037ULL);
    uint64_t h2 = Fnv1aHash64(label, len, 0xC6A4A7935BD1E995ULL);

    GUID g;
    g.Data1 = (unsigned long)(h1 >> 32);
    g.Data2 = (unsigned short)(h1 >> 16);
    g.Data3 = (unsigned short)(h1);
    for (int i = 0; i < 8; ++i) {
        g.Data4[i] = (unsigned char)(h2 >> (8 * i));
    }
    return g;
}

//************************************************************************
// Opens the modern folder-picker dialog (IFileDialog + FOS_PICKFOLDERS).
// hwndOwner may be nullptr. On success, writes the chosen folder path into
// outPath and returns true. On cancel or failure, outPath is left untouched
// and the function returns false. Handles COM init/uninit internally, so
// it's safe to call even if the caller hasn't already called CoInitialize.
//************************************************************************
bool BrowseForFolder(HWND hwndOwner, std::wstring& outPath,const GUID* clientId)
{
    bool weInitializedCom = false;
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (hr == S_OK || hr == S_FALSE) {
        weInitializedCom = true;
    }
    else if (hr != RPC_E_CHANGED_MODE) {
        // Some other real failure initializing COM.
        return false;
    }

    bool result = false;
    IFileDialog* pfd = nullptr;

    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr)) {
        if (clientId != nullptr) {
            pfd->SetClientGuid(*clientId);
        }
        
        DWORD options = 0;
        hr = pfd->GetOptions(&options);
        if (SUCCEEDED(hr)) {
            hr = pfd->SetOptions(options | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
        }

        if (SUCCEEDED(hr)) {
            hr = pfd->Show(hwndOwner);
            if (SUCCEEDED(hr)) {
                IShellItem* psi = nullptr;
                hr = pfd->GetResult(&psi);
                if (SUCCEEDED(hr)) {
                    PWSTR pszPath = nullptr;
                    hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
                    if (SUCCEEDED(hr)) {
                        outPath = pszPath;
                        CoTaskMemFree(pszPath);
                        result = true;
                    }
                    psi->Release();
                }
            }
            // hr == HRESULT_FROM_WIN32(ERROR_CANCELLED) just means the user hit Cancel.
        }

        pfd->Release();
    }

    if (weInitializedCom) {
        CoUninitialize();
    }

    return result;
}

//************************************************************************
// Opens the modern file-picker dialog (IFileDialog, no FOS_PICKFOLDERS).
// hwndOwner may be nullptr. filters/filterCount describe the file-type
// dropdown (pass nullptr/0 for "all files"). On success, writes the chosen
// file path into outPath and returns true. On cancel or failure, outPath
// is left untouched and the function returns false. Handles COM init/
// uninit internally, same as BrowseForFolder().
//************************************************************************
bool BrowseForFile(HWND hwndOwner, std::wstring& outPath,
                    const COMDLG_FILTERSPEC* filters,
                    UINT filterCount,
                    const GUID* clientId)
{
    bool weInitializedCom = false;
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (hr == S_OK || hr == S_FALSE) {
        weInitializedCom = true;
    }
    else if (hr != RPC_E_CHANGED_MODE) {
        return false;
    }

    bool result = false;
    IFileDialog* pfd = nullptr;

    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER,
                           IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr)) {
        if (clientId != nullptr) {
            pfd->SetClientGuid(*clientId);
        }
        if (filters != nullptr && filterCount > 0) {
            pfd->SetFileTypes(filterCount, filters);
            pfd->SetFileTypeIndex(1); // 1-based; first filter selected by default
        }

        hr = pfd->Show(hwndOwner);
        if (SUCCEEDED(hr)) {
            IShellItem* psi = nullptr;
            hr = pfd->GetResult(&psi);
            if (SUCCEEDED(hr)) {
                PWSTR pszPath = nullptr;
                hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
                if (SUCCEEDED(hr)) {
                    outPath = pszPath;
                    CoTaskMemFree(pszPath);
                    result = true;
                }
                psi->Release();
            }
        }
        // hr == HRESULT_FROM_WIN32(ERROR_CANCELLED) just means the user hit Cancel.

        pfd->Release();
    }

    if (weInitializedCom) {
        CoUninitialize();
    }

    return result;
}

//************************************************************************
// Opens the modern save-file dialog (IFileDialog via CLSID_FileSaveDialog).
// hwndOwner may be nullptr. filters/filterCount describe the file-type
// dropdown (pass nullptr/0 for "all files"). defaultName, if non-null,
// pre-fills the filename edit box (e.g. L"untitled.cfg"). Unlike
// BrowseForFile(), the returned path need not already exist -- that's
// the point. On success, writes the path into outPath and returns true.
// On cancel or failure, outPath is left untouched and the function
// returns false. Handles COM init/uninit internally, same as the others.
//************************************************************************
bool BrowseForFileSave(HWND hwndOwner, std::wstring& outPath,
                        const COMDLG_FILTERSPEC* filters,
                        UINT filterCount,
                        const wchar_t* defaultName,
                        const GUID* clientId)
{
    bool weInitializedCom = false;
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (hr == S_OK || hr == S_FALSE) {
        weInitializedCom = true;
    }
    else if (hr != RPC_E_CHANGED_MODE) {
        return false;
    }

    bool result = false;
    IFileDialog* pfd = nullptr;

    hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr)) {
        if (clientId != nullptr) {
            pfd->SetClientGuid(*clientId);
        }
        
        if (filters != nullptr && filterCount > 0) {
            pfd->SetFileTypes(filterCount, filters);
            pfd->SetFileTypeIndex(1); // 1-based; first filter selected by default
        }

        if (defaultName != nullptr) {
            pfd->SetFileName(defaultName);
        }

        hr = pfd->Show(hwndOwner);
        if (SUCCEEDED(hr)) {
            IShellItem* psi = nullptr;
            hr = pfd->GetResult(&psi);
            if (SUCCEEDED(hr)) {
                PWSTR pszPath = nullptr;
                hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
                if (SUCCEEDED(hr)) {
                    outPath = pszPath;
                    CoTaskMemFree(pszPath);
                    result = true;
                }
                psi->Release();
            }
        }
        // hr == HRESULT_FROM_WIN32(ERROR_CANCELLED) just means the user hit Cancel.

        pfd->Release();
    }

    if (weInitializedCom) {
        CoUninitialize();
    }

    return result;
}

//**************************************************************************
//  Claude: convert the wchar_t file/folder names to char strings 
//**************************************************************************
std::string WideToNarrow(const std::wstring& w)
{
    if (w.empty()) return {};
    int len = WideCharToMultiByte(CP_ACP, 0, w.c_str(), (int)w.size(),
                                   nullptr, 0, nullptr, nullptr);
    std::string s(len, 0);
    WideCharToMultiByte(CP_ACP, 0, w.c_str(), (int)w.size(),
                         s.data(), len, nullptr, nullptr);
    return s;
}

//**************************************************************************
//  Claude: example usage for BrowseForFile()
//**************************************************************************
#define  HIDE_EXAMPLE_CODE

#ifndef  HIDE_EXAMPLE_CODE

//  setting up the BrowseForXXX() functions to remember previous folder...
static GUID guidComprocPath  = MakeStableGuidFromLabel("console_attr.ComprocPath");
static GUID guidPalettePath  = MakeStableGuidFromLabel("console_attr.PalettePath");
static GUID guidStartingPath = MakeStableGuidFromLabel("console_attr.StartingPath");

void file_search_usage_examples(HWND hwndMain)
{
   // return select_starting_directory(hwndMain);
   std::wstring folder;
   if (BrowseForFolder(hwndMain, folder, &guidStartingPath)) {
      std::string folderA = WideToNarrow(folder);
      //  do something with the selected string
      // strncpy(starting_path, folderA.c_str(), sizeof(starting_path)) ;
   }

   //  usage, no filters (all files):
   std::wstring file;
   if (BrowseForFile(hwndMain, file)) {
      // use file
   }
   
   // Usage, with a filter (e.g. your program's own config/data files):
   const COMDLG_FILTERSPEC cfilters[] = {
      { L"Config files (*.cfg)", L"*.cfg" },
      { L"All files (*.*)",      L"*.*"   }
   };

   std::wstring file2;
   if (BrowseForFile(hwndMain, file2, cfilters, ARRAYSIZE(cfilters), &guidComprocPath)) {
      // use file
   }
   
   // examples for 
   const COMDLG_FILTERSPEC sfilters[] = {
      { L"Config files (*.cfg)", L"*.cfg" },
      { L"All files (*.*)",      L"*.*"   }
   };

   std::wstring file3;
   if (BrowseForFileSave(hwndMain, file3, sfilters, ARRAYSIZE(sfilters), L"untitled.cfg", &guidPalettePath)) {
      // use file -- may not exist on disk yet
   }   
}
#endif

