//********************************************************************
//  Copyright (c) 2025  Daniel D Miller
//  This class encapsulates the interface to the gdi_plus library.
//********************************************************************

#include <windows.h>

#include "common.h"
#include "gdi_plus.h"

//lint -esym(40, nullptr)

//lint -e1066  Symbol declared as "C" conflicts with itself

//********************************************************************
gdi_plus::gdi_plus(wchar_t const * const new_img_name) :
   img_name(NULL),
   // gbitmap(NULL),
   pbitmap(nullptr),
   hBitmap(nullptr),
   nWidth(0),
   nHeight(0),
   sprite_dx(0),
   sprite_dy(0),
   tiles_x(1),
   tiles_y(1)
{
   img_name = new wchar_t[wcslen(new_img_name)+1] ;
   wcscpy(img_name, new_img_name) ;
   
#ifdef _lint   
   pbitmap   = new Bitmap(new_img_name);
#else   
   pbitmap   = std::make_unique<Bitmap>(new_img_name);
#endif   
   nWidth    = pbitmap->GetWidth();
   nHeight   = pbitmap->GetHeight();
   sprite_dx = pbitmap->GetWidth();
   sprite_dy = pbitmap->GetHeight();
   Gdiplus::Color backgroundColor(66, 107, 107, 255); // White background
   Status status = pbitmap->GetHBITMAP(backgroundColor, &hBitmap);
   if (status != Ok) {
      syslog(L"GetHBITMAP error: %u\n", (uint) status);
   }
}

//********************************************************************
gdi_plus::gdi_plus(wchar_t const * const new_img_name, uint icons_per_column, uint icon_rows) :
   img_name(NULL),
   // gbitmap(NULL),
   pbitmap(nullptr),
   hBitmap(nullptr),
   nWidth(0),
   nHeight(0),
   sprite_dx(0),
   sprite_dy(0),
   tiles_x(icons_per_column),
   tiles_y(icon_rows)
{
   img_name = new wchar_t[wcslen(new_img_name) +1] ;
   wcscpy(img_name, new_img_name) ;
   
#ifdef _lint   
   pbitmap   = new Bitmap(new_img_name);
#else   
   pbitmap   = std::make_unique<Bitmap>(new_img_name);
#endif   
   nWidth  = pbitmap->GetWidth();
   nHeight = pbitmap->GetHeight();
   sprite_dx = nWidth / tiles_x ;
   sprite_dy = nHeight / tiles_y ;
   Gdiplus::Color backgroundColor(66, 107, 107, 255); // White background
   Status status = pbitmap->GetHBITMAP(backgroundColor, &hBitmap);
   if (status != Ok) {
      syslog(L"GetHBITMAP error: %u\n", (uint) status);
   }
   
   // syslog(L"open: %s, width: %u, height: %u, sprite size: %u x %u\n", 
   //    new_img_name, nWidth, nHeight, sprite_dx, sprite_dy) ;
}

//********************************************************************
gdi_plus::gdi_plus(wchar_t const * const new_img_name, uint icons_per_column, uint icon_rows, uint sprite_width, uint sprite_height) :
   img_name(NULL),
   // gbitmap(NULL),
   pbitmap(nullptr),
   hBitmap(nullptr),
   nWidth(0),
   nHeight(0),
   sprite_dx(sprite_width),
   sprite_dy(sprite_height),
   tiles_x(icons_per_column),
   tiles_y(icon_rows)
{
   img_name = new wchar_t[wcslen(new_img_name) +1] ;
   wcscpy(img_name, new_img_name) ;
   
#ifdef _lint   
   pbitmap   = new Bitmap(new_img_name);
#else   
   pbitmap   = std::make_unique<Bitmap>(new_img_name);
#endif   
   nWidth  = pbitmap->GetWidth();
   nHeight = pbitmap->GetHeight();
   // Convert Gdiplus::Bitmap to HBITMAP
   Gdiplus::Color backgroundColor(66, 107, 107, 255); // White background
   Status status = pbitmap->GetHBITMAP(backgroundColor, &hBitmap);
   if (status != Ok) {
      syslog(L"GetHBITMAP error: %u\n", (uint) status);
   }
}

//********************************************************************
gdi_plus::~gdi_plus()
{
   if (img_name != NULL) {
      delete [] img_name ;
   }
   DeleteObject(hBitmap);
   hBitmap = NULL ;
#ifdef _lint   
   delete pbitmap ;  //lint !e1551
   pbitmap = NULL ;
#endif   
}

//***********************************************************************************
//  originally, this function used gdiplus::clone() to access the sprites;
//  this turned out to be *horifically* slow; drawing the 1080 tiles in
//  tiles32.png took several seconds, while BitBlt() method is almost instantanious.
//  Thus, we still use gdiplus to open and access the files, 
//  but we use GDI::BitBlt() to access sprite elements.
//***********************************************************************************
void gdi_plus::copy_imagelist_item(HDC hdc, int sprite_col, int sprite_row, int xdest, int ydest)
{
   HDC hdcMem ;
   uint xsrc = sprite_col * sprite_dx ;   //lint !e737
   uint ysrc = sprite_row * sprite_dy ;   //lint !e737
   hdcMem = CreateCompatibleDC (hdc);
   if (hdcMem == NULL) {
      syslog(L"CreateCompatibleDC error: %u\n", (uint) GetLastError());
      return ;
   }
   HGDIOBJ gresult = SelectObject (hdcMem, hBitmap);
   // [141784] SelectObject error: object is not a region [0]
   if (gresult == NULL) {
      syslog(L"SelectObject error: object is not a region [%u]\n", (uint) GetLastError());
   }
   else if (gresult == HGDI_ERROR) {
      syslog(L"SelectObject error: GDE [%u]\n", (uint) GetLastError());
   }
   else {
      // syslog(L"BitBlt: dest: %ux%u, size: %ux%u, src: %ux%u\n", 
      //    xdest, ydest, sprite_dx, sprite_dy, hdcMem, xsrc, ysrc);
      BitBlt (hdc, xdest, ydest, sprite_dx, sprite_dy, hdcMem, xsrc, ysrc, SRCCOPY);
      DeleteDC (hdcMem);
   }
}  //lint !e818 !e1762

//********************************************************************
void gdi_plus::render_bitmap(HDC hdc, uint xdest, uint ydest)
{
   copy_imagelist_item(hdc, 0, 0, xdest, ydest);
}

//********************************************************************
void gdi_plus::render_bitmap(HDC hdc, uint xdest, uint ydest, uint sprite_col, uint sprite_row)
{
   copy_imagelist_item(hdc, sprite_col, sprite_row, xdest, ydest);
}

/************************************************************************/
//lint -esym(578, y0, y1)
void gdi_plus::DrawBox(HDC hdc, int x0, int y0, int dx, int dy, COLORREF rColor)
{
   ul2uc_t uconv ;
   uconv.ul = (uint) rColor ;
   uconv.uc[3] = 255 ;
   
   Graphics graphics(hdc);
   
   // Pen greenPen(Color::Green, 2.0); //lint !e747
   // Pen      pen(Color(255, 0, 0, 255), 2.0);
   // Pen Pen(Color(uconv.uc[3], uconv.uc[0], uconv.uc[1], uconv.uc[2] ), 2.0); //lint !e747
   // Pen pen(Color(uconv.uc[3], uconv.uc[0], uconv.uc[1], uconv.uc[2])); //lint !e747
   Pen pen(Color(uconv.uc[0], uconv.uc[1], uconv.uc[2])); //lint !e747
   graphics.DrawRectangle(&pen, x0, y0, dx, dy);
}  //lint !e1762

/************************************************************************/
//lint -esym(578, y0, y1)
void gdi_plus::DrawBoxFilled(HDC hdc, int xdest, int ydest, int dx, int dy, COLORREF rColor)
{
   ul2uc_t uconv ;
   uconv.ul = (uint) rColor ;
   uconv.uc[3] = 255 ;
   
   Graphics graphics(hdc);
   SolidBrush redBrush(Color(uconv.uc[3], uconv.uc[0], uconv.uc[1], uconv.uc[2]));            
   graphics.FillRectangle(&redBrush, xdest, ydest, dx, dy);
   
}  //lint !e1762

