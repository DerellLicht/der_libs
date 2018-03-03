//*****************************************************************************
//  Copyright (c) 2012  Daniel D Miller
//  Thread class for encapsulating Windows thread handling
//
//  This module, which has been entirely compiled from public-domain sources, 
//  is itself declared in the public domain.
//*****************************************************************************

// wthread.h  21  Info 1712: default constructor not defined for class 'CTrackbar'
//lint -esym(1712, CTrackbar)

//lint -esym(1704, CTrackbar::CTrackbar) //(const CTrackbar &)' has private access specification

class CTrackbar {
private:   
   HWND hwndTrack;
   HWND hwndParent;
   unsigned x0;
   unsigned y0;
   unsigned dx;
   unsigned dy;
   unsigned iMin;     // minimum value in trackbar range 
   unsigned iMax;     // maximum value in trackbar range 
   unsigned iSelMin;  // minimum value in trackbar selection 
   unsigned iSelMax;  // maximum value in trackbar selection 
   HMENU idTrackBar;
   unsigned tcount ;

   CTrackbar &operator=(const CTrackbar &src) ;
   CTrackbar(const CTrackbar&);
   
public:   
   CTrackbar(
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
      );
   ~CTrackbar();
   void update_trackbar(unsigned max_time, unsigned tick_spread);
   void set_position(unsigned new_tcount);
   unsigned get_position (void);
} ;

