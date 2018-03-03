//*****************************************************************************
//  Copyright (c) 2018  Daniel D Miller
//  WinAPI/Comctl32 trackbar class
//
//  This module, which has been entirely compiled from public-domain sources, 
//  is itself declared in the public domain.
//*****************************************************************************

#include <windows.h>
#include <commctrl.h>           //  link to comctl32.lib

#include "trackbar.h"

//*****************************************************************************
CTrackbar::CTrackbar(
   HWND hwndDlg,  // handle of dialog box (parent window) 
   unsigned in_x0,
   unsigned in_y0,
   unsigned in_dx,
   unsigned in_dy,
   unsigned in_iMin,     // minimum value in trackbar range 
   unsigned in_iMax,     // maximum value in trackbar range 
   unsigned in_iSelMin,  // minimum value in trackbar selection 
   unsigned in_iSelMax,  // maximum value in trackbar selection 
   HMENU in_idTrackBar
   ) :
   hwndTrack(NULL),
   hwndParent(hwndDlg),
   x0(in_x0),
   y0(in_y0),
   dx(in_dx),
   dy(in_dy),
   iMin(in_iMin),
   iMax(in_iMax),
   iSelMin(in_iSelMin),
   iSelMax(in_iSelMax),
   idTrackBar(in_idTrackBar),
   tcount(0)
{
   hwndTrack = CreateWindowEx (0,   // no extended styles 
      TRACKBAR_CLASS,           // class name 
      "Trackbar Control",       // title (caption) 
      WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE, // style 
      x0, y0,      // position 
      dx, dy,     // size 
      hwndParent, // parent window 
      idTrackBar, // control identifier 
      GetModuleHandle(NULL),  // instance 
      NULL        // no WM_CREATE parameter 
      );
}

//*****************************************************************************
CTrackbar::~CTrackbar()
{
//  Warning 1540: Pointer member neither freed nor zeroed by destructor
//lint -esym(1540, CTrackbar::hwndTrack, CTrackbar::hwndParent, CTrackbar::idTrackBar)
}

//*********************************************************************
void CTrackbar::update_trackbar(unsigned max_time, unsigned tick_spread)
{
   // min. & max. positions 
   HWND hwnd = hwndTrack ;
   SendMessage (hwnd, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG (0, max_time)); 
   // new page size ( distance that bar is moved by PgUp/PgDn )
   SendMessage (hwnd, TBM_SETPAGESIZE, 0, (LPARAM) tick_spread) ;
   SendMessage (hwnd, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG (0, max_time));
   SendMessage (hwnd, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) 0);
   SendMessage (hwnd, TBM_SETTICFREQ, (WPARAM) tick_spread, (LPARAM) 0);

   SetFocus (hwndTrack);
}

//*****************************************************************************
void CTrackbar::set_position(unsigned new_tcount)
{
   tcount = new_tcount ;
   SendMessage (hwndTrack, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) tcount);
}

//*****************************************************************************
unsigned CTrackbar::get_position (void) 
{
   tcount = SendMessage (hwndTrack, TBM_GETPOS, 0, 0);
   return tcount ;
}

