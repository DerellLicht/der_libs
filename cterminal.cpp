//****************************************************************************
//  Copyright (c) 2008-2014  Daniel D Miller
//  cterminal.cpp - Terminal class, derived from VListView class
//
//  This module, which has been entirely compiled from public-domain sources, 
//  is itself declared in the public domain.
//
//  Produced and Directed by:  Dan Miller
//****************************************************************************
//  Other requirements for this object:
//  In order for the virtual-listview elements to get drawn, 
//  caller needs to handle WM_NOTIFY MESSAGES.
//  Sample code is shown here:
// 
// static bool sample_notify(HWND hwnd, monitor_object_p this_device, LPARAM lParam)
// {
//    int msg_code = (int) ((NMHDR FAR *) lParam)->code ;
//    switch (msg_code) {
// 
//    //**********************************************************
//    //  terminal listview notifications
//    //**********************************************************
//    case LVN_GETDISPINFO:
//       get_terminal_entry(this_device->ecterm, lParam) ;
//       return true;
// 
//    case NM_CUSTOMDRAW:
//       SetWindowLongA (hwnd, DWL_MSGRESULT, (LONG) TerminalCustomDraw (this_device->ecterm, lParam));
//       return true;
// 
//    default:
//       if (dbg_flags & DBG_WINMSGS)
//          syslog("ElCal WM_NOTIFY: [%d] %s\n", msg_code, lookup_winmsg_name(msg_code)) ;
//       return false;
//    }
// }  //lint !e715
// 
//    case WM_NOTIFY:   //  in message-handler procedure
//       return sample_notify(hwnd, this_device, lParam) ;
//****************************************************************************
#include <windows.h>
#include <stdio.h>   //  vsprintf, sprintf, which supports %f
#include <stdlib.h>  //  atoi()
#include <time.h>    //  _strdate, _strtime
#include <tchar.h>

#include "common.h"
#include "commonw.h"
#include "cterminal.h"  //  includes vlistview.h

//lint -esym(714, set_term_fgnd, set_term_attr_default, append, termout, scroll_terminal)
//lint -esym(759, set_term_fgnd, set_term_attr_default, append, termout, scroll_terminal)
//lint -esym(765, set_term_fgnd, set_term_attr_default, append, termout, scroll_terminal)
//lint -esym(714, save_terminal_contents, get_last_term_entry)
//lint -esym(759, save_terminal_contents, get_last_term_entry)
//lint -esym(765, save_terminal_contents, get_last_term_entry)

// Info 1714: Member function not referenced
//lint -esym(1714, CTerminal::scroll_terminal, CTerminal::get_last_term_entry, CTerminal::terminal_lview_subclass)
//lint -esym(1714, CTerminal::CTerminal, CTerminal::resize_terminal_window, CTerminal::copy_selected_rows)
//lint -esym(1714, CTerminal::get_term_rows, CTerminal::get_term_columns, CTerminal::get_term_char_dy)
//lint -esym(1714, CTerminal::save_terminal_contents, CTerminal::set_term_attr_default)
//lint -esym(1714, CTerminal::get_term_attr, CTerminal::TerminalCustomDraw)
//lint -esym(1714, CTerminal::get_terminal_entry, CTerminal::termadd, CTerminal::termupdate)
//lint -esym(1714, CTerminal::get_element_count, CTerminal::delete_list, CTerminal::reverse_list_entries)
//lint -esym(1714, CTerminal::resize_terminal_rows, CTerminal::rows2pixels)
//lint -esym(1714, CTerminal::resize_terminal_width)
//lint -esym(1714, CTerminal::resize_terminal_pixels, CTerminal::get_terminal_dimens)
//lint -esym(1714, CTerminal::copy_list_to_clipboard, CTerminal::put)

//lint -esym(18, _strdate, _strtime)
//lint -esym(1055, _strdate, _strtime, _wstrdate, _wstrtime)
//lint -esym(746, _wstrdate, _wstrtime)

// Info 1762: Member function 'CTerminal::termout(const char *, ...)' could be made const
//lint -esym(1762, CTerminal::termout)

//lint -esym(1579, CTerminal::tlv_top, CTerminal::tlv_tail)

//lint -esym(526, _wfopen, _wstrdate, _wstrtime)
//lint -esym(628, _wfopen, _wstrdate, _wstrtime)

//****************************************************************************
CTerminal::CTerminal(HWND hwndParent, uint ControlID, HINSTANCE local_g_hinst, 
      uint x0, uint y0, uint dx, uint dy, uint local_style_flags) :
   CVListView(hwndParent, ControlID, local_g_hinst, x0, y0, dx, dy, local_style_flags),
   tlv_top(NULL),
   tlv_tail(NULL),
   term_fgnd(0x7F7F7F),
   term_bgnd(0),
   curr_row(0),
   rows(24),
   cols(80)
{
}

//*****************************************************************************
// Warning 1551: Function may throw exception '...' in destructor
//lint -esym(1551, CTerminal::~CTerminal)
CTerminal::~CTerminal()
{
   delete_list() ;
}

//****************************************************************************
void CTerminal::delete_list(void)
{
   term_lview_item_p lvptr = tlv_top ;
   tlv_top = NULL ;
   tlv_tail = NULL ;
   curr_row = 0 ;
   while (lvptr != NULL) {
      term_lview_item_p lvkill = lvptr ;
      lvptr = lvptr->next ;

      delete[] lvkill->msg ;
      delete lvkill ;
   }
}

//******************************************************************
void CTerminal::set_terminal_dimens(void)
{
   cols = get_lview_dx() / (cxChar) ;
   rows = get_lview_dy() / (cyChar + 1) ;
   // terminal: cols=89, rows=22, dx=717, dy=386, cyChar=16, cxChar=8
   // syslog("terminal: cols=%u, rows=%u, dx=%u, dy=%u, cyChar=%u, cxChar=%u\n",
   //    cols, rows, get_lview_dx(), get_lview_dy(), cyChar, cxChar) ;
}

//****************************************************************************
void CTerminal::resize_terminal_window(uint x0, uint y0, uint dx, uint dy)
{
   resize(x0, y0, dx, dy);
   set_terminal_dimens() ;  //  do this *after* resize()
}

//****************************************************************************
void CTerminal::resize_terminal_rows(uint new_rows)
{
   uint dy = new_rows * (cyChar + 1) ;
   // resize(x0, y0, get_lview_dx(), dy);
   resize(get_lview_dx(), dy+3); //  dialog is actually drawn a few pixels too small for text
   set_terminal_dimens() ;  //  do this *after* resize()
   // termout("resize_rows: rows=%u vs %u, dy=%u vs %u, cyChar=%u", 
   //    new_rows, rows, get_lview_dy(), dy, (cyChar + 1)) ;
}

//****************************************************************************
void CTerminal::resize_terminal_width(uint new_dx)
{
   resize(new_dx, get_lview_dy()); //  dialog is actually drawn a few pixels too small for text
   set_terminal_dimens() ;  //  do this *after* resize()
   resize_column(new_dx-25) ; //  make this narrower than new_dx, to allow for scroll bar
}

//****************************************************************************
void CTerminal::resize_terminal_pixels(uint new_dx, uint new_dy)
{
   resize(new_dx, new_dy); //  dialog is actually drawn a few pixels too small for text
   set_terminal_dimens() ;  //  do this *after* resize()
   resize_column(new_dx-25) ; //  make this narrower than new_dx, to allow for scroll bar
}

//****************************************************************************
//  This function expects to receive a pointer to an array of four uints
//****************************************************************************
void CTerminal::get_terminal_dimens(uint *array_of_four) const
{
   *array_of_four++ = cols ;
   *array_of_four++ = rows ;
   *array_of_four++ = get_lview_dx() ;
   *array_of_four++ = get_lview_dy() ;
}

//****************************************************************************
void CTerminal::set_terminal_font(TCHAR * szFaceName, int iDeciPtHeight, unsigned iAttributes)
{
   set_listview_font(szFaceName, iDeciPtHeight, iAttributes) ;
   set_terminal_dimens() ;  //  do this *after* resize(), or change font
}

//****************************************************************************
WNDPROC CTerminal::terminal_lview_subclass(LONG TermSubclassProc)
{
   return lview_subclass(TermSubclassProc) ;
}

//****************************************************************************
void CTerminal::scroll_terminal(bool scroll_down)
{
   int pixels_dy = rows * cyChar ;
   scroll_listview((scroll_down) ? pixels_dy : -pixels_dy);
}

//*************************************************************************
//lint -esym(1714, CTerminal::clear_message_area)
void CTerminal::clear_message_area(void)
{
   curr_row = 0 ;
   clear_listview() ;
   delete_list() ;
}

//****************************************************************************
term_lview_item_p CTerminal::find_element(uint idx)
{
   term_lview_item_p lvptr ;
   for (lvptr = tlv_top; lvptr != NULL; lvptr = lvptr->next) {
      if (lvptr->idx == idx)
         return lvptr;
   }
   return NULL;
}

//****************************************************************************
//  this copies all lines which have been marked in the listview 
//  by Windows selection operations.
//****************************************************************************
void CTerminal::copy_elements_to_clipboard(void)
{
   uint blen = 0 ;
   //  first, see what size of buffer we need
   term_lview_item_p lvptr ;
   for (lvptr = tlv_top; lvptr != NULL; lvptr = lvptr->next) {
      if (lvptr->marked)
         blen += _tcslen(lvptr->msg) + 4 ;
   }
   if (blen == 0)
      return ;
   TCHAR *bfr = new TCHAR[blen] ;
   if (bfr == NULL)
      return ;
   //  loop back over the list and copy all elements into buffer
   int slen = 0 ;
   for (lvptr = tlv_top; lvptr != NULL; lvptr = lvptr->next) {
      if (lvptr->marked)
         slen += _stprintf(bfr+slen, _T("%s\r\n"), lvptr->msg) ;
   }

   //  finally, copy the buffer to clipboard
   copy_buffer_to_clipboard(bfr, blen) ;
   delete[] bfr ;
}

//****************************************************************************
//  this copies all lines in the virtual listview
//****************************************************************************
void CTerminal::copy_list_to_clipboard(void)
{
   uint blen = 0 ;
   //  first, see what size of buffer we need
   term_lview_item_p lvptr ;
   for (lvptr = tlv_top; lvptr != NULL; lvptr = lvptr->next) {
      // if (lvptr->marked)
         blen += _tcslen(lvptr->msg) + 4 ;
   }
   if (blen == 0)
      return ;
   TCHAR *bfr = new TCHAR[blen] ;
   if (bfr == NULL)
      return ;
   //  loop back over the list and copy all elements into buffer
   int slen = 0 ;
   for (lvptr = tlv_top; lvptr != NULL; lvptr = lvptr->next) {
      // if (lvptr->marked)
         slen += _stprintf(bfr+slen, _T("%s\r\n"), lvptr->msg) ;
   }

   //  finally, copy the buffer to clipboard
   copy_buffer_to_clipboard(bfr, blen) ;
   delete[] bfr ;
}

//****************************************************************************
void CTerminal::mark_element(uint idx)
{
   for (term_lview_item_p lvptr = tlv_top; lvptr != NULL; lvptr = lvptr->next) {
      if (lvptr->idx == idx) {
         lvptr->marked = true ;
         break;
      }
   }
}

//****************************************************************************
void CTerminal::clear_marked_elements(void)
{
   for (term_lview_item_p lvptr = tlv_top; lvptr != NULL; lvptr = lvptr->next) {
      lvptr->marked = false ;
   }
}

//********************************************************************
void CTerminal::reverse_list_entries(void)
{
   term_lview_item_p lvptr = tlv_top ;
   tlv_top = NULL ;
   // uint curr_row = termptr->curr_row - 1;
   while (LOOP_FOREVER) {
      if (lvptr == NULL)
         break;
      term_lview_item_p ltemp = lvptr ;
      lvptr = lvptr->next ;
      ltemp->next = NULL ;
      // ltemp->idx = curr_row-- ;
      if (tlv_top == 0) {
         tlv_top = ltemp ;
      } else {
         ltemp->next = tlv_top ;
         tlv_top = ltemp ;
      }
   }

   //**************************************************************
   //  now, go back and renumber the elements
   //**************************************************************
   curr_row = 0 ;
   for (lvptr = tlv_top ; lvptr != 0; lvptr = lvptr->next) {
      lvptr->idx = curr_row++ ;
   }
   // syslog("reverse list: %u elements swapped\n", curr_row) ;
}

//****************************************************************************
int CTerminal::copy_selected_rows(void)
{
   // termout("C pressed") ;
   uint elements_found = 0 ;
   // uint selcount = ListView_GetSelectedCount(this_port->hwndRxData) ;
   uint selcount = get_selected_count();
   int result = 0 ;
   int nCurItem = -1 ;
   while (LOOP_FOREVER) {
      // nCurItem = ListView_GetNextItem(this_port->hwndRxData, nCurItem, LVNI_SELECTED);
      nCurItem = get_next_listview_index(nCurItem) ;
      if (nCurItem < 0)
         break;
      // syslog("mark %d\n", nCurItem) ;
      mark_element(nCurItem) ;
      elements_found++ ;
   }
   if (elements_found == selcount) {
      copy_elements_to_clipboard() ;
      termout(_T("%u rows copied"), selcount) ;  //  with TabControl, this does not work...
      // syslog("%u rows copied", selcount) ;
      result = selcount ;
   } else {
      // errout("found %u of %u elements", elements_found, selcount) ;  //  oops... these depend upon CommPort !!
      // termout("found %u of %u elements", elements_found, selcount) ;
      result = -(int)elements_found ;
   }
   clear_marked_elements() ;
   return result;
}

//****************************************************************************
static void html_output(FILE *fd, COLORREF fgnd, COLORREF bgnd, TCHAR *str)
{
   //  12/04/13 DDM - Why did the <br> get added here?  the closing </span> tab already forces a newline
   // fprintf(fd, "<span style=\"color : #%06X; background-color : #%06X;\">%s</span><br>\n",
   _ftprintf(fd, _T("<span style=\"color : #%06X; background-color : #%06X;\">%s</span>\n"),
               swap_rgb(fgnd), swap_rgb(bgnd), str) ; //lint !e559
}

//****************************************************************************
int CTerminal::save_terminal_contents(TCHAR *outfile, file_type_e file_type)
{
   FILE *fd = NULL ;
   int lcount = 0 ;
   term_lview_item_p lvptr ;
   TCHAR msgstr[260] ;

   TCHAR dbuffer [9];
   TCHAR tbuffer [9];
   _tstrdate( dbuffer );
   _tstrtime( tbuffer );

   switch (file_type) {
   case FTYP_TEXT:
      fd = _tfopen(outfile, _T("a+t")) ; //lint !e64
      if (fd == NULL) {
         return -(int)GetLastError();
      }
      fseek(fd, 0, SEEK_END) ;
      _ftprintf(fd, _T("***********************************************************************\n")) ;
      _ftprintf(fd, _T("Date/Time of report: %s, %s\n"), dbuffer, tbuffer) ; //lint !e559
      for (lvptr = tlv_top; lvptr != NULL; lvptr = lvptr->next) {
         lcount++ ;
         _ftprintf(fd, _T("%s\n"), lvptr->msg) ; //lint !e559
      }
      break;

   case FTYP_HTML:
      if (file_exists(outfile)) {
         //  from January 1999 MSDN:
         //  When a file is opened with the "a" or "a+" access type, 
         //  all write operations occur at the end of the file. 
         //  The file pointer can be repositioned using fseek or rewind, 
         //  but is always moved back to the end of the file before any 
         //  write operation is carried out. 
         //  Thus, existing data cannot be overwritten.
         // syslog("appending to existing file\n") ;
         fd = _tfopen(outfile, _T("r+t")) ; //lint !e64
         if (fd == NULL) {
            return -(int)GetLastError();
         }
         fseek(fd, 0, SEEK_END) ;
         //  back up pointer to over-write closing HTML tags
         fseek(fd, (long) -16, SEEK_CUR) ; 
      } else {
         // syslog("writing to html file\n") ;
         fd = _tfopen(outfile, _T("wt")) ; //lint !e64
         if (fd == NULL) {
            return -(int)GetLastError();
         }
         _ftprintf(fd, _T("<html><head><title>%s</title>")
                     //  STYLE was not needed once I converted to using PRE tag.
                     // "<STYLE type='text/css'>\n"
                     // "* { font-family: Courier, monospace }\n"
                     // "</STYLE>\n"
                     _T("</head><body>\n"), outfile) ; //lint !e559
      }
      _ftprintf(fd, _T("<pre>\n")) ;
      html_output(fd, WIN_BGREEN, WIN_GREY, _T("***********************************************************************")) ;
      _stprintf(msgstr, _T("Date/Time of report: %s, %s"), dbuffer, tbuffer) ;
      html_output(fd, WIN_BGREEN, WIN_GREY, msgstr) ;
      for (lvptr = tlv_top; lvptr != NULL; lvptr = lvptr->next) {
         lcount++ ;
         // fprintf(fd, "%s\n", lvptr->msg) ;
         html_output(fd, lvptr->fgnd, lvptr->bgnd, lvptr->msg) ;
      }
      _ftprintf(fd, _T("</pre>\n")) ;
      _ftprintf(fd, _T("</body></html>\n")) ;
      break;

   // default:
   //    return -(int)ERROR_INVALID_DATA;
      // break;
   }

   if (fd != NULL) {
      fflush(fd) ;
      fclose(fd) ;
      // clear_message_area();   //  leave this up to parent code
      return lcount ;
   } 
   termout(_T("invalid file_type [%u]"), (uint) file_type) ;
   return -(int) ERROR_INVALID_DATA;
}

//****************************************************************************
void CTerminal::set_term_attr(COLORREF fgnd, COLORREF bgnd)
{
   term_fgnd = fgnd ;
   term_bgnd = bgnd ;
}

//****************************************************************************
void CTerminal::set_term_attr_default(void)
{
   term_fgnd = GetSysColor(COLOR_WINDOWTEXT) ;
   term_bgnd = GetSysColor(COLOR_WINDOW) ;
}

//****************************************************************************
// Info 1762: Member function 'CTerminal::get_term_attr(unsigned long *, unsigned long *)' could be made const
//  DDM: if I put const on this member function, I get compile errors from g++:
//    undefined reference to `CTerminal::get_term_attr(unsigned long*, unsigned long*)'
//lint -esym(1762, CTerminal::get_term_attr)
void CTerminal::get_term_attr(COLORREF *prev_fgnd, COLORREF *prev_bgnd)
{
   *prev_fgnd = term_fgnd ;
   *prev_bgnd = term_bgnd ;
}

//****************************************************************************
term_lview_item_p CTerminal::get_lview_element(TCHAR *lpBuf, COLORREF fgnd, COLORREF bgnd)
{
   // syslog("print %u byte string\n", _tcslen(lpBuf)) ;
   term_lview_item_p lvptr = new term_lview_item_t ;
   ZeroMemory((TCHAR *) lvptr, sizeof(term_lview_item_t)) ; //lint !e740
   lvptr->idx = curr_row++ ;
   lvptr->msg = new TCHAR[_tcslen(lpBuf)+1] ;
   _tcscpy(lvptr->msg, lpBuf) ;
   //  save these settings for screen redraw
   lvptr->fgnd = fgnd ;
   lvptr->bgnd = bgnd ;
   return lvptr;
}

//****************************************************************
// Info 1762: Member function 'CTerminal::TerminalCustomDraw(long)' could be made const
// DDM: except that this *isn't* const...  it changes lParam)
//lint -esym(1762, CTerminal::TerminalCustomDraw)
LRESULT CTerminal::TerminalCustomDraw (LPARAM lParam)
{
   //  remember which item number we are currently dealing with.
   LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW) lParam;

   switch (lplvcd->nmcd.dwDrawStage) {
   case CDDS_PREPAINT:       //Before the paint cycle begins
      // syslog("CDDS_PREPAINT\n") ;
      //request notifications for individual listview items
      return CDRF_NOTIFYITEMDRAW;
      // return (CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYITEMDRAW);
      // return CDRF_NOTIFYSUBITEMDRAW;

   case CDDS_ITEMPREPAINT:  //Before an item is drawn
      // syslog("CDDS_ITEMPREPAINT\n") ;
      return CDRF_NOTIFYSUBITEMDRAW;

   case CDDS_SUBITEM | CDDS_ITEMPREPAINT: //Before a subitem is drawn
      // {
      // if (lplvcd->nmcd.dwDrawStage == CDDS_SUBITEM)
      //    syslog("CDDS_SUBITEM\n") ;
      // else
      //    syslog("CDDS_ITEMPREPAINT\n") ;
      // }
      // lplvcd->clrText = GetSysColor(COLOR_WINDOWTEXT) ;
      // lplvcd->clrTextBk = GetSysColor(COLOR_WINDOW);
      lplvcd->clrText   = term_fgnd ;
      lplvcd->clrTextBk = term_bgnd ;
      return CDRF_DODEFAULT ;

   default:
      syslog(_T("Terminal: Unknown CDDS code %d\n"), lplvcd->nmcd.dwDrawStage) ;
      break;
   }
   return CDRF_DODEFAULT;
}

//****************************************************************************
void CTerminal::get_terminal_entry(LPARAM lParam)
{
   LV_DISPINFO *lpdi = (LV_DISPINFO *) lParam;
   static TCHAR szString[MAX_TERM_CHARS + 1];  //  there's where we're crashing on long strings!!

   //  item number is: lpdi->item.iItem 
   if (lpdi->item.mask & LVIF_TEXT) {
      term_lview_item_p lvptr = find_element(lpdi->item.iItem) ;
      if (lvptr == NULL) {
         wsprintf(szString, _T("listview element %d not found [%u total]"), 
            lpdi->item.iItem, curr_row);
         lpdi->item.pszText = szString ;
         set_term_attr(WIN_BCYAN, WIN_RED) ;
      } else {
         lpdi->item.pszText = lvptr->msg ;
         set_term_attr(lvptr->fgnd, lvptr->bgnd) ; //  set up for TerminalCustomDraw()
      }
   }
}

//****************************************************************************
TCHAR *CTerminal::get_last_term_entry(void)
{
   static TCHAR empty_msg[] = _T("No terminal tail entry found") ;
   if (tlv_tail == NULL) 
      return empty_msg;
   term_lview_item_p lvptr = tlv_tail ;
   return lvptr->msg ;
}

//****************************************************************************
void CTerminal::put(TCHAR *lpBuf)
{
   term_lview_item_p lvptr = get_lview_element(lpBuf, term_fgnd, term_bgnd) ;

   //  add to object list
   if (tlv_top == NULL)
      tlv_top = lvptr ;
   else
      tlv_tail->next = lvptr ;
   tlv_tail = lvptr ;

   if (lvptr->idx+1 != curr_row) {
      syslog(_T("math_error [L%u]: %d != %d\n"), __LINE__, lvptr->idx+1, curr_row) ;
   }
   if (end_of_page_active())
      listview_update(lvptr->idx) ;
}

//*************************************************************************************************
//  This function overcomes a race condition in the conventional set_term_attr() + put() sequence.  
//  In that sequence, set_term_attr() stores the attributes in class variables, which put() then reads.
//  However, there is a possibility (especially in a multi-threaded application) that some other code may 
//  call set_term_attr() before the initial put() can access the values - 
//  this results in the string being displayed in the wrong colors.
//  
//  This combined function stores the specified attribute directly into the terminal
//  struct which contains the string for put().
//  This avoids the dependence upon class-global data fields, and *should* provide more reliable rendering.
//*************************************************************************************************
void CTerminal::put(TCHAR *lpBuf, COLORREF fgnd, COLORREF bgnd)
{
   term_lview_item_p lvptr = get_lview_element(lpBuf, fgnd, bgnd) ;

   //  add to object list
   if (tlv_top == NULL)
      tlv_top = lvptr ;
   else
      tlv_tail->next = lvptr ;
   tlv_tail = lvptr ;

   if (lvptr->idx+1 != curr_row) {
      syslog(_T("math_error [L%u]: %d != %d\n"), __LINE__, lvptr->idx+1, curr_row) ;
   }
   if (end_of_page_active())
      listview_update(lvptr->idx) ;
}

//****************************************************************************
//  append the new string to the current line
//****************************************************************************
void CTerminal::append(TCHAR *lpBuf)
{
   if (tlv_tail == NULL) {
      put(lpBuf) ;   //  append
      return ;
   }
   term_lview_item_p lvptr = tlv_tail ;
   TCHAR *lptr = lvptr->msg ;
   uint llen = _tcslen(lptr) ;
   uint nlen = _tcslen(lpBuf) ;
   lvptr->msg = new TCHAR[llen+nlen+1] ;
   _stprintf(lvptr->msg, _T("%s%s"), lptr, lpBuf) ;
   delete lptr ;
   //  save these settings for screen redraw
   lvptr->fgnd = term_fgnd ;
   lvptr->bgnd = term_bgnd ;

   if (end_of_page_active())
      listview_update(lvptr->idx) ;
}

//****************************************************************************
//  replace current line with new string
//****************************************************************************
void CTerminal::replace(TCHAR *lpBuf)
{
   if (tlv_tail == NULL) {
      put(lpBuf) ;   //  replace
      return ;
   }
   term_lview_item_p lvptr = tlv_tail ;
   TCHAR *lptr = lvptr->msg ;
   // uint llen = _tcslen(lptr) ;
   uint nlen = _tcslen(lpBuf) ;
   lvptr->msg = new TCHAR[nlen+1] ;
   _stprintf(lvptr->msg, _T("%s"), lpBuf) ;
   delete lptr ;
   //  save these settings for screen redraw
   lvptr->fgnd = term_fgnd ;
   lvptr->bgnd = term_bgnd ;

   if (end_of_page_active())
      listview_update(lvptr->idx) ;
}

//********************************************************************
int CTerminal::termout(const TCHAR *fmt, ...)
{
   TCHAR consoleBuffer[MAX_TERM_CHARS + 1];
   va_list al; //lint !e522

   va_start(al, fmt);   //lint !e1055 !e530
   _vstprintf(consoleBuffer, fmt, al);   //lint !e64
   put(consoleBuffer);  //  termout
   va_end(al);
   return 1;
}

//********************************************************************
int CTerminal::termadd(const TCHAR *fmt, ...)
{
   TCHAR consoleBuffer[MAX_TERM_CHARS + 1];
   va_list al; //lint !e522

   va_start(al, fmt);   //lint !e1055 !e530
   _vstprintf(consoleBuffer, fmt, al);   //lint !e64
   append(consoleBuffer);
   va_end(al);
   return 1;
}

//********************************************************************
int CTerminal::termupdate(const TCHAR *fmt, ...)
{
   TCHAR consoleBuffer[MAX_TERM_CHARS + 1];
   va_list al; //lint !e522

   va_start(al, fmt);   //lint !e1055 !e530
   _vstprintf(consoleBuffer, fmt, al);   //lint !e64
   replace(consoleBuffer);
   va_end(al);
   return 1;
}

