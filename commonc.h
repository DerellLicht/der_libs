//****************************************************************************
//  Copyright (c) 2008-2014  Daniel D Miller
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

//****************************************************************************
HWND MyCreateUpDownControl(
      HWND hwndParent, uint ControlID, HINSTANCE ghinst, HWND hwndBuddy, 
      uint MaxValue, uint MinValue, uint InitValue) ;
HWND CBGetItemHandle(HWND hwndCBox);
BOOL ShellGetPath (HANDLE hDlg, char lpszPath[]);
int WriteFileF(HANDLE hdl, const char *fmt, ...);
uint get_bottom_line(HWND hwnd, uint ctrl_id);
void Line(HDC hdc, uint x1, uint y1, uint x2, uint y2);
char *read_edit_control(HWND hwnd, char *msgstr);
bool select_text_file(HWND hDlgWnd, char *command_filename);
bool select_file(HWND hDlgWnd, char *command_filename, char *ext);
void copy_buffer_to_clipboard(char *cbbfr, unsigned blen);
int GetScreenDPI(void);

//  font-management functions
HFONT build_font(TCHAR *fname, unsigned fheight, unsigned flags);
HFONT EzCreateFont(HDC hdc, char * szFaceName, int iDeciPtHeight,
      int iDeciPtWidth, unsigned iAttributes, int textangle, BOOL fLogRes);

#ifdef UNICODE
int str_unicode_to_ascii(WCHAR *UnicodeStr, char *AsciiStr, uint AsciiLen);
int str_ascii_to_unicode(char *AsciiStr, WCHAR *UnicodeStr, uint UnicodeLen);
WCHAR *ascii2unicode(char *AsciiStr, uint AsciiLen);
WCHAR *ascii2unicode(char *AsciiStr);
char *unicode2ascii(WCHAR *UnicodeStr);
#endif

