//****************************************************************************
//  Copyright (c) 2008-2017  Daniel D Miller
//  common_funcs.cpp - common functions for Windows and other programs.
//  commonw.h - Contains Windows-specific and System-specific definitions
//  This module, which has been entirely compiled from public-domain sources, 
//  is itself declared in the public domain.
//  
//  Collected and Organized by:  Dan Miller
//****************************************************************************

//****************************************************************************
//  constants used by build_font() and EzCreateFont()
//****************************************************************************
#define EZ_ATTR_NORMAL        0
#define EZ_ATTR_BOLD          1
#define EZ_ATTR_ITALIC        2
#define EZ_ATTR_UNDERLINE     4
#define EZ_ATTR_STRIKEOUT     8

//  windows defs which are not present in MinGW headers
//  Used for PSCB_PRECREATE message
//lint -esym(751, LPDLGTEMPLATEEX)
//lint -esym(754, DLGTEMPLATEEX_s::dlgVer, DLGTEMPLATEEX_s::helpID, DLGTEMPLATEEX_s::exStyle)
//lint -esym(754, DLGTEMPLATEEX_s::cDlgItems, DLGTEMPLATEEX_s::x, DLGTEMPLATEEX_s::y)
//lint -esym(754, DLGTEMPLATEEX_s::cx, DLGTEMPLATEEX_s::cy)
//lint -esym(756, LPDLGTEMPLATEEX, DLGTEMPLATEEX)
//lint -esym(768, DLGTEMPLATEEX_s::dlgVer, DLGTEMPLATEEX_s::helpID, DLGTEMPLATEEX_s::exStyle)
//lint -esym(768, DLGTEMPLATEEX_s::cDlgItems, DLGTEMPLATEEX_s::x, DLGTEMPLATEEX_s::y)
//lint -esym(768, DLGTEMPLATEEX_s::cx, DLGTEMPLATEEX_s::cy)
//lint -esym(768, DLGTEMPLATEEX_s::signature, DLGTEMPLATEEX_s::style)
typedef struct DLGTEMPLATEEX_s {
        WORD dlgVer;
        WORD signature;
        DWORD helpID;
        DWORD exStyle;
        DWORD style;
        WORD cDlgItems;
        short x;
        short y;
        short cx;
        short cy;
        // etc..
} DLGTEMPLATEEX, *LPDLGTEMPLATEEX; 

// #define PSN_FIRST                (0U-200U)
#define  PSN_GETOBJECT               (-210)
#define  PSN_TRANSLATEACCELERATOR    (-212)
#define  PSN_QUERYINITIALFOCUS       (-213)

#define  LVN_HOTTRACK                (-121)

//*****************************************************************
//  lookup-table struct, used for Windows Message handlers
//*****************************************************************
//lint -esym(768, winproc_table_s::win_code, winproc_table_s::winproc_func)
//lint -esym(756, winproc_table_t)
typedef struct winproc_table_s {
   uint win_code ;
   bool (*winproc_func)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, LPVOID private_data) ;
} winproc_table_t ;

//****************************************************************************
HWND MyCreateUpDownControl(
      HWND hwndParent, uint ControlID, HINSTANCE ghinst, HWND hwndBuddy, 
      uint MaxValue, uint MinValue, uint InitValue) ;
HWND CBGetItemHandle(HWND hwndCBox);
BOOL ShellGetPath (HANDLE hDlg, TCHAR lpszPath[]);
int  WriteFileF(HANDLE hdl, const TCHAR *fmt, ...);
uint get_bottom_line(HWND hwnd, uint ctrl_id);
void Line(HDC hdc, uint x1, uint y1, uint x2, uint y2);
TCHAR *read_edit_control(HWND hwnd, TCHAR *msgstr);
bool select_text_file(HWND hDlgWnd, TCHAR *command_filename);
bool select_file(HWND hDlgWnd, TCHAR *command_filename, TCHAR *ext);
void copy_buffer_to_clipboard(TCHAR *cbbfr, unsigned blen);
int  GetScreenDPI(void);
BOOL CenterWindow (HWND hwnd);
void resize_window(HWND hwnd, int dx, int dy);

//  font-management functions
HFONT build_font(TCHAR *fname, unsigned fheight, unsigned flags);
HFONT EzCreateFont(HDC hdc, TCHAR * szFaceName, int iDeciPtHeight,
      int iDeciPtWidth, unsigned iAttributes, int textangle, BOOL fLogRes);

#ifdef UNICODE
int str_unicode_to_ascii(WCHAR *UnicodeStr, char *AsciiStr, uint AsciiLen);
int str_ascii_to_unicode(char *AsciiStr, WCHAR *UnicodeStr, uint UnicodeLen);
WCHAR *ascii2unicode(char *AsciiStr, uint AsciiLen);
WCHAR *ascii2unicode(char *AsciiStr);
char *unicode2ascii(WCHAR *UnicodeStr);
#endif

//****************************************************************************
//  12/16/16 - import commonly-used functions from system.cpp
//****************************************************************************

//****************************************************************************
//  system.cpp - encapsulate DLU-mapping computations and decisions.
//****************************************************************************

//  dialog dimension functions
uint get_screen_width(void);
uint get_screen_height(void);
bool cp_recalc_dlu_width (uint *psheet_dx);
bool cp_recalc_dlu_height(uint *psheet_dy);
void center_dialog_on_screen(HWND hDlg);

//  general system functions
void get_monitor_dimens(HWND hwnd);
bool are_normal_fonts_active(void);

