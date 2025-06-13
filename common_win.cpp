//****************************************************************************
//  Copyright (c) 2008-2017  Daniel D Miller
//  common_win.cpp - common functions for Windows and other programs.
//  These functions will comprise all the functions which require linking
//  comctl32, comdlg32, gdi32, and other non-standard gcc/g++ libraries.
//  
//  This module, which has been entirely compiled from public-domain sources, 
//  is itself declared in the public domain.
//  
//  Collected and Organized by:  Dan Miller
//****************************************************************************

//lint -esym(18, _NOTIFYICONDATAA::szTip, _NOTIFYICONDATAW::szTip)
// #define  _WIN32_IE      0x0501
// #define  _WIN32_WINDOWS 0x0410
#include <windows.h>
#include <time.h>
#include <stdio.h>   //  vsprintf
#include <math.h>    //  fabs()
#include <limits.h>
#include <tchar.h>
#ifdef _lint
#include <stdlib.h>
#else
#include <tchar.h>
#include <commctrl.h>
#include <commdlg.h> //  GetOpenFileName()
#endif
#include <shlobj.h>  //  ShellGetPath()

#include "common.h"
#include "commonw.h"

//lint -e840  Use of nul character in a string literal is perfectly normal
//lint -e732  Loss of sign (arg. no. 6) (int to unsigned long)
//lint -e592  Non-literal format specifier used without arguments

//lint -e1065 Symbol declared as "C" conflicts with itself

//********************************************************************************
//  this function returns the appropriate child-window handle for a ComboBox,
//  so that the edit control can be sub-classed.
//  
//  GetComboBoxInfo() results:
//  hwndCombo - A handle to the combo box.
//  hwndItem  - A handle to the edit box.   <= subclass this element to catch WM_CHAR
//  hwndList  - A handle to the drop-down list.
// 
// typedef struct tagCOMBOBOXINFO {
//   DWORD cbSize;
//   RECT  rcItem;
//   RECT  rcButton;
//   DWORD stateButton;
//   HWND  hwndCombo;
//   HWND  hwndItem;
//   HWND  hwndList;
// } COMBOBOXINFO, *PCOMBOBOXINFO, *LPCOMBOBOXINFO;
//********************************************************************************
//lint -esym(714, CBGetItemHandle)
//lint -esym(759, CBGetItemHandle)
//lint -esym(765, CBGetItemHandle)
HWND CBGetItemHandle(HWND hwndCBox)
{
   COMBOBOXINFO cBoxInfo ;
   //  Find the ComboBox handle to sub-class for this function
   cBoxInfo.cbSize = sizeof(COMBOBOXINFO) ;
   GetComboBoxInfo(hwndCBox, &cBoxInfo);
   return cBoxInfo.hwndItem;
}

//****************************************************************************
//lint -esym(714, MyCreateUpDownControl)
//lint -esym(759, MyCreateUpDownControl)
//lint -esym(765, MyCreateUpDownControl)
HWND MyCreateUpDownControl(
      HWND hwndParent, uint ControlID, HINSTANCE ghinst, HWND hwndBuddy, 
      uint MaxValue, uint MinValue, uint InitValue) 
{
   // icex.dwICC = ICC_UPDOWN_CLASS;    // Set the Initialization Flag value.
   // InitCommonControlsEx(&icex);      // Initialize the Common Controls Library.

   HWND hwndControl = CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING,
                              UPDOWN_CLASS,
                              NULL,
                              WS_CHILDWINDOW | WS_VISIBLE
                              // | UDS_AUTOBUDDY 
                              | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK,
                              // Set to zero to automatically size to fit the buddy window.
                              0, 0, 0, 0,         
                              hwndParent,
                              (HMENU) ControlID,
                              ghinst,
                              NULL);

   // Sets the controls direction and range.
   SendMessage(hwndControl, UDM_SETRANGE, 0, MAKELPARAM(MaxValue, MinValue));    
   SendMessage(hwndControl, UDM_SETBUDDY, (WPARAM) hwndBuddy, 0);
   SendMessage(hwndControl, UDM_SETPOS, (WPARAM) 0, (LPARAM) InitValue);

   TCHAR msgstr[10] ;
   _stprintf(msgstr, _T("%u"), InitValue) ;
   SetWindowText(hwndBuddy, msgstr) ;

   return hwndControl ;
}

//***********************************************************************
//  from: http://www.codeguru.com/forum/showthread.php?t=128562
//  Vinoj Kumar posted this function on March 30th, 2001, 10:59 AM
//  
//  In the previous post, the poster asked if he had to "clean up" 
//  the LPITEMIDLIST variable. 
//  I did not know the answer.  Is that what you are doing with :
//  HGLOBAL hgMem;
//  hgMem = GlobalHandle(lpil);
//  GlobalFree(hgMem);
// 
//  Answer:  yes, it is
//  
//***********************************************************************
//  requires -lshell32
//***********************************************************************

//***********************************************************************
//lint -esym(714, ShellGetPath)
//lint -esym(759, ShellGetPath)
//lint -esym(765, ShellGetPath)
BOOL ShellGetPath(HANDLE hDlg, char lpszPath[])
{
   BOOL bRet;
   char szPath[MAX_PATH_LEN];
   LPITEMIDLIST lpil;
   HGLOBAL hgMem;
   BROWSEINFOA bi;

   bi.hwndOwner = (HWND) hDlg;
   bi.pidlRoot = NULL;
   bi.pszDisplayName = szPath;
   bi.lpszTitle = "Select Folder";
   // bi.ulFlags = BIF_RETURNONLYFSDIRS;
   //  The purpose of including BIF_DONTGOBELOWDOMAIN was to prevent access
   //  to network (i.e., URL) paths, which I thought would break fopen/fgets.
   //  However, those functions work *fine* with URL paths, so I didn't need to worry!!
   // bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE ;
   bi.ulFlags = BIF_NEWDIALOGSTYLE ;
   bi.lpfn = NULL;
   bi.lParam = 0L;
   bi.iImage = 0;
   lpil = SHBrowseForFolderA (&bi);
   if (lpil == NULL)
      return FALSE;
   bRet = SHGetPathFromIDListA (lpil, lpszPath);
   hgMem = GlobalHandle (lpil);
   GlobalFree (hgMem);
   return bRet;
}
//****************************************************************************
//lint -esym(714, copy_buffer_to_clipboard)
//lint -esym(759, copy_buffer_to_clipboard)
//lint -esym(765, copy_buffer_to_clipboard)
void copy_buffer_to_clipboard(TCHAR *cbbfr, unsigned blen)
{
   // test to see if we can open the clipboard first before
   // wasting any cycles with the memory allocation
   if (OpenClipboard(NULL)) {
      // Empty the Clipboard. This also has the effect
      // of allowing Windows to free the memory associated
      // with any data that is in the Clipboard
      EmptyClipboard ();

      // Ok. We have the Clipboard locked and it's empty. 
      // Now let's allocate the global memory for our data.

      // Here I'm simply using the GlobalAlloc function to 
      // allocate a block of data equal to the text in the
      // "to clipboard" edit control plus one character for the
      // terminating null character required when sending
      // ANSI text to the Clipboard.
      HGLOBAL hClipboardData = GlobalAlloc (GMEM_DDESHARE, blen + 1);

      // Calling GlobalLock returns to me a pointer to the data
      // associated with the handle returned from GlobalAlloc
      TCHAR *pchData = (TCHAR *) GlobalLock (hClipboardData);

      // At this point, all I need to do is use the standard 
      // C/C++ strcpy function to copy the data from the local 
      // variable to the global memory.
      _tcscpy (pchData, cbbfr);

      // Once done, I unlock the memory - remember you 
      // don't call GlobalFree because Windows will free the 
      // memory automatically when EmptyClipboard is next called. 
      GlobalUnlock (hClipboardData);

      // Now, set the Clipboard data by specifying that ANSI text 
      // is being used and passing the handle to the global memory.
      SetClipboardData (CF_TEXT, hClipboardData);

      // Finally, when finished I simply close the Clipboard
      // which has the effect of unlocking it so that other
      // applications can examine or modify its contents.
      CloseClipboard ();
   }
}  //lint !e429 !e818

//****************************************************************************
//  find screen coords of upper-left corner of CommPort dialog.
//  From this, hopefully, I can accurately find ctrl_top value.
//****************************************************************************
//lint -esym(714, get_bottom_line)
//lint -esym(759, get_bottom_line)
//lint -esym(765, get_bottom_line)
uint get_bottom_line(HWND hwnd, uint ctrl_id)
{
   POINT pt = { 0, 0 } ;
   ClientToScreen(hwnd, &pt) ;
   // syslog("upper-left corner is at %ld,%ld", pt.x, pt.y) ;
   //  this gives an X measure which is on outside of window frame,
   //  but Y measure is bottom of menu bar
   //  [20880] upper-left corner is at 1613,365

   RECT myRect ;
   GetWindowRect(GetDlgItem(hwnd, ctrl_id), &myRect) ;
   // syslog("bottom of DeviceDetection box: %u", (uint) myRect.bottom) ;
   // [20880] bottom of DeviceDetection box: 6617752
   return (uint) ((uint) myRect.bottom - (uint) pt.y) ;
}

//****************************************************************************
//lint -esym(714, WriteFileF)
//lint -esym(759, WriteFileF)
//lint -esym(765, WriteFileF)
int WriteFileF(HANDLE hdl, const TCHAR *fmt, ...)
{
   DWORD bytesWritten ;
   TCHAR consoleBuffer[260];
   va_list al; //lint !e522

   va_start(al, fmt);   //lint !e1055 !e530
   _vstprintf(consoleBuffer, fmt, al);   //lint !e64

   WriteFile(hdl, consoleBuffer, _tcslen(consoleBuffer), &bytesWritten, NULL) ;

   va_end(al);
   return 1;
}

//******************************************************************
//lint -esym(714, Line)
//lint -esym(759, Line)
//lint -esym(765, Line)
void Line(HDC hdc, uint x1, uint yy1, uint x2, uint y2)
{
   MoveToEx(hdc, x1, yy1, NULL) ;
   LineTo(hdc, x2, y2) ;
}

//*****************************************************************************
//  this function reads the edit-control contents into msgstr.
//  However, the returned pointer skips any leading spaces or tabs
//  in the data.  This corrects for my habit of putting leadings spaces
//  in front of edit-control data to improve readability.
//*****************************************************************************
//lint -esym(714, read_edit_control)
//lint -esym(759, read_edit_control)
//lint -esym(765, read_edit_control)
TCHAR *read_edit_control(HWND hwnd, TCHAR *msgstr)
{
   uint tempEditLength = GetWindowTextLength(hwnd);
   GetWindowText(hwnd, msgstr, tempEditLength + 1);
   msgstr[tempEditLength] = 0;
   return strip_leading_spaces(msgstr) ;
}

//*****************************************************************************
// static TCHAR szPalFilter[] = 
//    TEXT ("Text Files (*.TXT)\0*.txt\0")  
//    TEXT ("All Files (*.*)\0*.*\0\0") ;
static TCHAR const szPalFilter[] = 
   _T("Text Files (*.TXT)\0*.txt\0")  \
   _T("All Files (*.*)\0*.*\0\0") ;

//******************************************************************
//lint -esym(714, select_text_file)
//lint -esym(759, select_text_file)
//lint -esym(765, select_text_file)
bool select_text_file(HWND hDlgWnd, TCHAR *command_filename)
{
   // syslog("A handles=%d\n", get_handle_count());
   TCHAR szFile[MAX_PATH_LEN+1];       // buffer for file name
   TCHAR oldFile[MAX_PATH_LEN+1];       // buffer for file name
   TCHAR dirFile[MAX_PATH_LEN+1];       // buffer for file name

   // Initialize OPENFILENAME
   OPENFILENAME ofn;       // common dialog box structure
   ZeroMemory(&ofn, sizeof(ofn));
   ofn.lStructSize = sizeof(ofn);
   ofn.hwndOwner = hDlgWnd;
   ofn.lpstrFile = szFile;
   //
   // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
   // use the contents of szFile to initialize itself.
   //
   ofn.lpstrFile[0] = '\0';
   _tcscpy(dirFile, command_filename) ;
   TCHAR *strptr = _tcsrchr(dirFile, '\\') ;
   if (strptr != 0) {
      strptr++ ;  //  leave the backslash in place
      *strptr = 0 ;  //  strip off filename
      // OutputDebugStringA(dirFile) ;
   }
   ofn.lpstrInitialDir = dirFile ;
   ofn.nMaxFile = sizeof(szFile);
   ofn.lpstrFilter = szPalFilter ;
   ofn.nFilterIndex = 1;
   ofn.lpstrTitle = _T("select desired file") ;
   ofn.lpstrFileTitle = NULL ;
//    ofn.lpstrDefExt = TEXT ("txt") ;
   ofn.lpstrDefExt = _T("txt") ;
   // ofn.nMaxFileTitle = 0;
   // ofn.lpstrInitialDir = NULL;
   ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

   // Display the Open dialog box.
   // syslog("B handles=%d\n", get_handle_count());
   if (GetOpenFileName(&ofn)) {
   // syslog("C handles=%d\n", get_handle_count());
      _tcsncpy(oldFile, command_filename, sizeof(oldFile)) ;
      _tcsncpy(command_filename, ofn.lpstrFile, MAX_PATH_LEN) ;

      SetFocus(hDlgWnd) ;
      return true;
   }
   // syslog("D handles=%d\n", get_handle_count());
   return false;
}

//******************************************************************
//  Note that 'ext' should be *only* the extension, 
//  no dot, wildcard, or other text
//******************************************************************
// static char szGenFilter[] = 
//    "Text Files (*.TXT)\0*.txt\0"  
//    "All Files (*.*)\0*.*\0\0" ;

//lint -esym(714, select_file)
//lint -esym(759, select_file)
//lint -esym(765, select_file)
bool select_file(HWND hDlgWnd, TCHAR *command_filename, TCHAR *ext)
{
   TCHAR szGenFilter[80] ;
   TCHAR *gfptr = szGenFilter ;
   uint slen = 0 ;
   slen += (uint) _stprintf(gfptr+slen, _T("%s Files (*.%s)"), ext, ext) ;
   slen++ ; //  leave current NULL-term in place
   slen += (uint) _stprintf(gfptr+slen, _T("*.%s"), ext) ;
   slen++ ; //  leave current NULL-term in place
   slen += (uint) _stprintf(gfptr+slen, _T("All Files (*.*)")) ;
   slen++ ; //  leave current NULL-term in place
   slen += (uint) _stprintf(gfptr+slen, _T("*.*")) ;
   slen++ ; //  leave current NULL-term in place
   *(gfptr+slen) = 0 ;  //  add a terminating NULL-term

   // syslog("A handles=%d\n", get_handle_count());
   TCHAR szFile[PATH_MAX];       // buffer for file name
   TCHAR oldFile[PATH_MAX];       // buffer for file name
   TCHAR dirFile[PATH_MAX];       // buffer for file name

   // Initialize OPENFILENAME
   OPENFILENAME ofn;       // common dialog box structure
   ZeroMemory(&ofn, sizeof(ofn));
   ofn.lStructSize = sizeof(ofn);
   ofn.hwndOwner = hDlgWnd;
   ofn.lpstrFile = szFile;
   //
   // Set lpstrFile[0] to '\0' so that GetOpenFileName does not
   // use the contents of szFile to initialize itself.
   //
   ofn.lpstrFile[0] = '\0';
   _tcscpy(dirFile, command_filename) ;
   TCHAR *strptr = _tcsrchr(dirFile, '\\') ;
   if (strptr != 0) {
      strptr++ ;  //  leave the backslash in place
      *strptr = 0 ;  //  strip off filename
      // OutputDebugStringA(dirFile) ;
   }
   ofn.lpstrInitialDir = dirFile ;
   ofn.nMaxFile = sizeof(szFile);
   ofn.lpstrFilter = szGenFilter ;
   ofn.nFilterIndex = 1;
   ofn.lpstrTitle = _T("select desired file") ;
   ofn.lpstrFileTitle = NULL ;
//    ofn.lpstrDefExt = TEXT ("txt") ;
   ofn.lpstrDefExt = ext ;
   // ofn.nMaxFileTitle = 0;
   // ofn.lpstrInitialDir = NULL;
   ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

   // Display the Open dialog box.
   // syslog("B handles=%d\n", get_handle_count());
   if (GetOpenFileName(&ofn)) {
   // syslog("C handles=%d\n", get_handle_count());
      _tcsncpy(oldFile, command_filename, sizeof(oldFile)) ;
      _tcsncpy(command_filename, ofn.lpstrFile, PATH_MAX) ;

      SetFocus(hDlgWnd) ;
      return true;
   }
   // syslog("D handles=%d\n", get_handle_count());
   return false;
}

//*************************************************************************
//lint -esym(714, build_font)
//lint -esym(759, build_font)
//lint -esym(765, build_font)
HFONT build_font(TCHAR *fname, unsigned fheight, unsigned flags)
{
   int fbold      = (flags & EZ_ATTR_BOLD     ) ? TRUE : FALSE ;
   int fitalic    = (flags & EZ_ATTR_ITALIC   ) ? TRUE : FALSE ;
   int funderline = (flags & EZ_ATTR_UNDERLINE) ? TRUE : FALSE ;
   int fstrikeout = (flags & EZ_ATTR_STRIKEOUT) ? TRUE : FALSE ;
   HFONT hfont = CreateFont(
         //  46, 28, 215, 0,  //  height, width, escapement, orientation
         fheight, 0, 0, 0,
         (fbold == FALSE) ? FW_NORMAL : FW_BOLD,
         fitalic,
         funderline,
         fstrikeout,
         DEFAULT_CHARSET, 0, 0, 0, DEFAULT_PITCH, //  other stuff
         fname);
   return hfont;
}  //lint !e818

//*************************************************************************
//lint -esym(714, EzCreateFont)
//lint -esym(759, EzCreateFont)
//lint -esym(765, EzCreateFont)
HFONT EzCreateFont(HDC hdc, TCHAR * szFaceName, int iDeciPtHeight,
       int iDeciPtWidth, unsigned iAttributes, int textangle, BOOL fLogRes)
{
   FLOAT      cxDpi, cyDpi ;
   HFONT      hFont ;
   LOGFONT    lf ;
   POINT      pt ;
   TEXTMETRIC txtm ;

   SaveDC (hdc) ;

   SetGraphicsMode (hdc, GM_ADVANCED) ;
   ModifyWorldTransform (hdc, NULL, MWT_IDENTITY) ;
   SetViewportOrgEx (hdc, 0, 0, NULL) ;
   SetWindowOrgEx   (hdc, 0, 0, NULL) ;

   if (fLogRes) {
      cxDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSX) ;
      cyDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSY) ;
   }
   else {
      cxDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, HORZRES) /
                              GetDeviceCaps (hdc, HORZSIZE)) ;

      cyDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, VERTRES) /
                              GetDeviceCaps (hdc, VERTSIZE)) ;
   }

   pt.x = (long) (iDeciPtWidth  * cxDpi / 72) ;
   pt.y = (long) (iDeciPtHeight * cyDpi / 72) ;

   DPtoLP (hdc, &pt, 1) ;

   lf.lfHeight         = - (int) (fabs ((double) pt.y) / 10.0 + 0.5) ;
   lf.lfWidth          = 0 ;
   lf.lfEscapement     = textangle ;
   lf.lfOrientation    = textangle ;
   lf.lfWeight         = iAttributes & EZ_ATTR_BOLD      ? 700 : 0 ;
   lf.lfItalic         = iAttributes & EZ_ATTR_ITALIC    ?   1 : 0 ;
   lf.lfUnderline      = iAttributes & EZ_ATTR_UNDERLINE ?   1 : 0 ;
   lf.lfStrikeOut      = iAttributes & EZ_ATTR_STRIKEOUT ?   1 : 0 ;
   lf.lfCharSet        = 0 ;
   lf.lfOutPrecision   = 0 ;
   lf.lfClipPrecision  = 0 ;
   lf.lfQuality        = 0 ;
   lf.lfPitchAndFamily = 0 ;
   _tcscpy(lf.lfFaceName, szFaceName) ;
   hFont = CreateFontIndirect(&lf) ;

   if (iDeciPtWidth != 0) {
      hFont = (HFONT) SelectObject (hdc, hFont) ;

      GetTextMetrics (hdc, &txtm) ;

      DeleteObject (SelectObject (hdc, hFont)) ;

      lf.lfWidth = (int) (txtm.tmAveCharWidth *
                          fabs ((double) pt.x) / fabs ((double) pt.y) + 0.5) ;

      hFont = CreateFontIndirect(&lf) ;
   }
   RestoreDC (hdc, -1) ;
   return hFont ;
}  //lint !e818

//****************************************************************************
//  returns 96 for normal fonts, 120 for Large Fonts, 
//  and other values for custom settings.
//****************************************************************************
//lint -esym(714, GetScreenDPI)
//lint -esym(759, GetScreenDPI)
//lint -esym(765, GetScreenDPI)
int GetScreenDPI(void)
{
  HDC hdcScreen = GetDC(NULL);
  int iDPI = -1; // assume failure
  if (hdcScreen) {
    iDPI = GetDeviceCaps(hdcScreen, LOGPIXELSX);
    ReleaseDC(NULL, hdcScreen);
  }
  return iDPI;
}

//********************************************************************************************
//lint -esym(714, CenterWindow)
//lint -esym(759, CenterWindow)
//lint -esym(765, CenterWindow)
BOOL CenterWindow (HWND hwnd)
{
   HWND hwndParent;
   RECT rect, rectP;
   int width, height;
   int screenwidth, screenheight;
   int x, y;

   //make the window relative to its desktop
   hwndParent = GetDesktopWindow ();

   GetWindowRect (hwnd, &rect);
   GetWindowRect (hwndParent, &rectP);

   width = rect.right - rect.left;
   height = rect.bottom - rect.top;

   x = ((rectP.right - rectP.left) - width) / 2 + rectP.left;
   y = ((rectP.bottom - rectP.top) - height) / 2 + rectP.top;

   screenwidth = GetSystemMetrics (SM_CXSCREEN);
   screenheight = GetSystemMetrics (SM_CYSCREEN);

   //make sure that the dialog box never moves outside of
   //the screen
   if (x < 0)
      x = 0;
   if (y < 0)
      y = 0;
   if (x + width > screenwidth)
      x = screenwidth - width;
   if (y + height > screenheight)
      y = screenheight - height;

   MoveWindow (hwnd, x, y, width, height, FALSE);
   SetActiveWindow (hwnd);

   return TRUE;
}

//****************************************************************************
//  unfortunately, this resize operation sets the *outside* window area,
//  not the internal client area...
//****************************************************************************
//lint -esym(714, resize_window)
//lint -esym(759, resize_window)
//lint -esym(765, resize_window)
void resize_window(HWND hwnd, int dx, int dy)
{
   ShowWindow(hwnd, SW_HIDE) ;
   // SetWindowPos(hwnd, NULL, 0, 0, dx, dy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
   SetWindowPos(hwnd, NULL, 0, 0, dx, dy, SWP_NOMOVE);
   ShowWindow(hwnd, SW_SHOW) ;
}
#ifdef UNICODE
//****************************************************************************
//lint -esym(714, str_unicode_to_ascii)
//lint -esym(759, str_unicode_to_ascii)
//lint -esym(765, str_unicode_to_ascii)
int str_unicode_to_ascii(WCHAR *UnicodeStr, char *AsciiStr, uint AsciiLen)
{
   return WideCharToMultiByte(CP_ACP, 0, UnicodeStr, -1, AsciiStr, AsciiLen, NULL, NULL);
}

//****************************************************************************
//lint -esym(714, str_ascii_to_unicode)
//lint -esym(759, str_ascii_to_unicode)
//lint -esym(765, str_ascii_to_unicode)
int str_ascii_to_unicode(char *AsciiStr, WCHAR *UnicodeStr, uint AsciiLen)
{
   return MultiByteToWideChar(CP_ACP, 0, AsciiStr, -1, UnicodeStr, AsciiLen+1);
}

//****************************************************************************
//lint -esym(714, ascii2unicode)
//lint -esym(759, ascii2unicode)
//lint -esym(765, ascii2unicode)
WCHAR *ascii2unicode(char *AsciiStr, uint AsciiLen)
{
   static WCHAR UnicodeStr[MAX_UNICODE_LEN+1] ;
   if (AsciiLen > MAX_UNICODE_LEN)
       AsciiLen = MAX_UNICODE_LEN ;
   MultiByteToWideChar(CP_ACP, 0, AsciiStr, -1, UnicodeStr, AsciiLen+1);
   return UnicodeStr;
}

//****************************************************************************
//lint -esym(714, ascii2unicode)
//lint -esym(759, ascii2unicode)
//lint -esym(765, ascii2unicode)
WCHAR *ascii2unicode(char *AsciiStr)
{
   uint AsciiLen = strlen(AsciiStr) ;
   static WCHAR UnicodeStr[MAX_UNICODE_LEN+1] ;
   if (AsciiLen > MAX_UNICODE_LEN)
       AsciiLen = MAX_UNICODE_LEN ;
   MultiByteToWideChar(CP_ACP, 0, AsciiStr, -1, UnicodeStr, AsciiLen+1);
   return UnicodeStr;
}

//****************************************************************************
//lint -esym(714, unicode2ascii)
//lint -esym(759, unicode2ascii)
//lint -esym(765, unicode2ascii)
char *unicode2ascii(WCHAR *UnicodeStr)
{
   static char AsciiStr[MAX_UNICODE_LEN+1] ;
   WideCharToMultiByte(CP_ACP, 0, UnicodeStr, -1, AsciiStr, MAX_UNICODE_LEN, NULL, NULL);
   return AsciiStr ;
}
#endif

//****************************************************************************
//  12/16/16 - import commonly-used functions from system.cpp
//****************************************************************************
static const uint STD_DPI = 96 ;

// static int  curr_dpi = STD_DPI ;
static uint screen_width  = 0 ;
static uint screen_height = 0 ;

//lint -esym(714, get_monitor_dimens)
//lint -esym(752, get_monitor_dimens)
//lint -esym(759, get_monitor_dimens)
//lint -esym(765, get_monitor_dimens)
void get_monitor_dimens(HWND hwnd)
{
   HMONITOR currentMonitor;      // Handle to monitor where fullscreen should go
   MONITORINFO mi;               // Info of that monitor
   currentMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
   mi.cbSize = sizeof(MONITORINFO);
   if (GetMonitorInfo(currentMonitor, &mi) != FALSE) {
      screen_width  = mi.rcMonitor.right  - mi.rcMonitor.left ;
      screen_height = mi.rcMonitor.bottom - mi.rcMonitor.top ;
   }
   // curr_dpi = GetScreenDPI() ;
}

//lint -esym(714, get_screen_width)
//lint -esym(752, get_screen_width)
//lint -esym(759, get_screen_width)
//lint -esym(765, get_screen_width)
uint get_screen_width(void)
{
   return screen_width ;
}

//lint -esym(714, get_screen_height)
//lint -esym(752, get_screen_height)
//lint -esym(759, get_screen_height)
//lint -esym(765, get_screen_height)
uint get_screen_height(void)
{
   return screen_height ;
}

//********************************************************************************
//  This requires get_monitor_dimens() to be called first
//********************************************************************************
//lint -esym(714, center_dialog_on_screen)
//lint -esym(752, center_dialog_on_screen)
//lint -esym(759, center_dialog_on_screen)
//lint -esym(765, center_dialog_on_screen)
void center_dialog_on_screen(HWND hDlg)
{
   if (screen_width == 0  ||  screen_height == 0) {
      return ;
   }
   RECT rectAbout ;
   GetWindowRect(hDlg, &rectAbout);
   uint dxAbout = rectAbout.right  - rectAbout.left ;
   uint dyAbout = rectAbout.bottom - rectAbout.top ;
   uint xi = (screen_width  - dxAbout) / 2 ;
   uint yi = (screen_height - dyAbout) / 2 ; 
   SetWindowPos(hDlg, HWND_TOP, xi, yi, 0, 0, SWP_NOSIZE);   
}

//****************************************************************************
//lint -esym(714, are_normal_fonts_active)
//lint -esym(752, are_normal_fonts_active)
//lint -esym(759, are_normal_fonts_active)
//lint -esym(765, are_normal_fonts_active)
bool are_normal_fonts_active(void)
{
   uint curr_dpi = GetScreenDPI() ; //lint !e732 Loss of sign
   if (curr_dpi == 96)
      return true;
   return false;
}

//****************************************************************************
//  return true if recalculation was required, false otherwise
//****************************************************************************
//lint -esym(714, cp_recalc_dlu_width)
//lint -esym(752, cp_recalc_dlu_width)
//lint -esym(759, cp_recalc_dlu_width)
//lint -esym(765, cp_recalc_dlu_width)
bool cp_recalc_dlu_width(uint *psheet_dx)
{
   uint curr_dpi = GetScreenDPI() ; //lint !e732 Loss of sign
   if (curr_dpi == STD_DPI) 
      return false ;
   *psheet_dx = (*psheet_dx * curr_dpi) / STD_DPI ;
   return true ;
}

//****************************************************************************
//  return true if recalculation was required, false otherwise
//****************************************************************************
//lint -esym(714, cp_recalc_dlu_height)
//lint -esym(752, cp_recalc_dlu_height)
//lint -esym(759, cp_recalc_dlu_height)
//lint -esym(765, cp_recalc_dlu_height)
bool cp_recalc_dlu_height(uint *psheet_dy)
{
   uint curr_dpi = GetScreenDPI() ; //lint !e732 Loss of sign
   if (curr_dpi == STD_DPI) 
      return false ;
   *psheet_dy = (*psheet_dy * curr_dpi) / STD_DPI ;
   return true ;
}

