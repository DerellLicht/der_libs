//****************************************************************************
//  Copyright (c) 1985-2025  Daniel D Miller
//  Wrapper functions for interface to CTerminal module
//
//  Written by:  Dan Miller
//****************************************************************************
#include <windows.h>
#include <stdio.h>   //  vsprintf, sprintf, which supports %f
#include <tchar.h>

#include "common.h"
#include "commonw.h"
#include "cterminal.h" 
#include "terminal.h"
#include "winmsgs.h"

//lint -esym(714, term_append, term_copy_selected_rows, term_clear_message_area)
//lint -esym(759, term_append, term_copy_selected_rows, term_clear_message_area)
//lint -esym(765, term_append, term_copy_selected_rows, term_clear_message_area)
//lint -esym(714, term_lview_subclass, term_set_debug, term_set_font)
//lint -esym(759, term_lview_subclass, term_set_debug, term_set_font)
//lint -esym(765, term_lview_subclass, term_set_debug, term_set_font)

static CTerminal *myTerminal = NULL;

static bool debug_enabled = false ;

//******************************************************************
void term_set_debug(bool enabled)
{
   debug_enabled = enabled ;
}

//******************************************************************
void term_set_attr(uint fgnd, uint bgnd)
{
   myTerminal->set_term_attr(fgnd, bgnd);
}

//******************************************************************
void term_put(TCHAR *term_str)
{
   myTerminal->put(term_str);
}

//lint -esym(714, term_append)
//lint -esym(759, term_append)
//lint -esym(765, term_append)
void term_append(TCHAR *term_str)
{
   myTerminal->append(term_str);
}

//lint -esym(714, term_replace)
//lint -esym(759, term_replace)
//lint -esym(765, term_replace)
void term_replace(TCHAR *term_str)
{
   myTerminal->replace(term_str);
}

uint term_get_columns(void) 
{
   return myTerminal->get_term_columns() ;
}

uint term_get_rows(void)
{
   return myTerminal->get_term_rows() ;
}

void term_clear_message_area(void)
{
   myTerminal->clear_message_area() ;
}

//******************************************************************
void term_copy_selected_rows(void)
{
   myTerminal->copy_selected_rows() ;
}

//******************************************************************
void term_set_font(TCHAR *font_name, uint fsize, uint attr)
{
   myTerminal->set_terminal_font(font_name, fsize, attr) ;
}

//******************************************************************
//  NOLINTBEGIN(clang-diagnostic-c++11-narrowing)
bool term_notify(HWND hwnd, LPARAM lParam)
{
   UINT msg_code = (int) ((NMHDR FAR *) lParam)->code ;
   if (debug_enabled) {
      switch (msg_code) {
      //  list messages to be ignored
      case LVN_GETDISPINFO:   //lint !e650  Constant '4294967146' out of range for operator 'case'
      case NM_CUSTOMDRAW:     //lint !e650  Constant '-12' out of range for operator 'case'
      case NM_KILLFOCUS:      //lint !e650  Constant '-8' out of range for operator 'case'
      case TTN_SHOW:          //lint !e650  Constant '4294967146' out of range for operator 'case'
      case TTN_POP:           //lint !e650  Constant '4294967146' out of range for operator 'case'
      case LVN_ODCACHEHINT:   //lint !e650  Constant '4294967146' out of range for operator 'case'
      case LVN_KEYDOWN:       //lint !e650  Constant '4294967146' out of range for operator 'case'
         break;
      default:
         syslog(_T("TNOT [%s]\n"), lookup_winmsg_name(msg_code)) ;
         break;
      }
   }

   switch (msg_code) {

   //**********************************************************
   //  terminal listview notifications
   //**********************************************************
   case LVN_GETDISPINFO:  //lint !e650
      // get_terminal_entry(term, lParam) ;
      myTerminal->get_terminal_entry(lParam) ;
      return true;

   case NM_CUSTOMDRAW:  //lint !e650  Constant '-12' out of range for operator 'case'
      SetWindowLongA (hwnd, DWL_MSGRESULT, (LONG) myTerminal->TerminalCustomDraw(lParam));
      return true;

   default:
      // if (dbg_flags & DBG_WINMSGS)
      //    syslog("WM_NOTIFY: [%d] %s\n", msg_code, lookup_winmsg_name(msg_code)) ;
      return false;
   }
}  //lint !e715
//  NOLINTEND(clang-diagnostic-c++11-narrowing)

//******************************************************************
// Subclass procedure for the Terminal Virtual ListView control
// http://msdn.microsoft.com/en-us/library/ms633570%28VS.85%29.aspx#subclassing_window
//******************************************************************
//  mon.support.cpp
// static WNDPROC wpOrigTermProc; 
// uint key_mask = 0 ;
// 
// static LRESULT APIENTRY TermSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
// {
//    if (uMsg == WM_GETDLGCODE) 
//       return DLGC_WANTALLKEYS; 
//  
//    // switch (uMsg) {
//    // }  //lint !e744
// 
//    return CallWindowProc(wpOrigTermProc, hwnd, uMsg, wParam, lParam); 
// }

//******************************************************************
// Subclass the terminal ListView 
//  this is typically set in WM_INITDIALOG
// wpOrigTermProc = (WNDPROC) SetWindowLongPtr(term->hwndSelf, GWL_WNDPROC, (LONG) TermSubclassProc); 
// wpOrigTermProc = (WNDPROC) myTerminal->terminal_lview_subclass((LONG) TermSubclassProc); 
//******************************************************************
WNDPROC term_lview_subclass(LONG TermSubclassProc)
{
   return myTerminal->terminal_lview_subclass((LONG) TermSubclassProc); 
}

//******************************************************************
void setup_terminal_window(HWND hwnd, uint StatusBarHeight, uint bottom_ref_control, uint terminal_control)
{
   RECT myRect ;
   // GetWindowRect(hwnd, &myRect) ;
   GetClientRect(hwnd, &myRect) ;
   uint dx = (myRect.right - myRect.left) ;
   uint dy = (myRect.bottom - myRect.top) ;

   uint ctrl_bottom = get_bottom_line(hwnd, bottom_ref_control) ;
   ctrl_bottom += 3 ;
   uint lvdy = dy - ctrl_bottom - StatusBarHeight ;

   myTerminal = new CTerminal(hwnd, terminal_control, (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), 
      0, ctrl_bottom, dx-1, lvdy,
      LVL_STY_VIRTUAL | LVL_STY_NO_HEADER | LVL_STY_PAGE_TO_END | LVL_STY_EX_GRIDLINES) ;
   myTerminal->set_terminal_font(_T("Courier New"), 100, EZ_ATTR_BOLD) ;    // NOLINT
   myTerminal->lview_assign_column_headers() ;
}

