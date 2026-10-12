//********************************************************************
//  Copyright (c) 2025  Derell Licht
//  This class encapsulates the interface to the gdi_plus library.
//********************************************************************

#include <windows.h>

#include "common.h"
#include "gdi_plus.h"

//********************************************************************
gdi_plus::gdi_plus(wchar_t const * const new_img_name) :
   pbitmap(std::make_unique<Bitmap>(new_img_name)),
   hBitmap(nullptr),
   nWidth(pbitmap->GetWidth()),
   nHeight(pbitmap->GetHeight()),
   sprite_dx(pbitmap->GetWidth()),
   sprite_dy(pbitmap->GetHeight()),
   tiles_x(1),
   tiles_y(1)
{
   Gdiplus::Color backgroundColor(66, 107, 107, 255); // White background
   Status status = pbitmap->GetHBITMAP(backgroundColor, &hBitmap);
   if (status != Ok) {
      syslog(L"GetHBITMAP error: %u\n", (uint) status);
   }
}

//********************************************************************
gdi_plus::gdi_plus(wchar_t const * const new_img_name, uint icons_per_column, uint icon_rows) :
   pbitmap(std::make_unique<Bitmap>(new_img_name)),
   hBitmap(nullptr),
   nWidth(pbitmap->GetWidth()),
   nHeight(pbitmap->GetHeight()),
   sprite_dx(nWidth / icons_per_column),
   sprite_dy(nHeight / icon_rows),
   tiles_x(icons_per_column),
   tiles_y(icon_rows)
{
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
   pbitmap(std::make_unique<Bitmap>(new_img_name)),
   hBitmap(nullptr),
   nWidth(pbitmap->GetWidth()),
   nHeight(pbitmap->GetHeight()),
   sprite_dx(sprite_width),
   sprite_dy(sprite_height),
   tiles_x(icons_per_column),
   tiles_y(icon_rows)
{
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
   end_batch();   //  bitmap cannot be deleted while selected into a DC
   DeleteObject(hBitmap);
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
      syslog(L"SelectObject error: GLE [%u]\n", (uint) GetLastError());
   }
   else {
      // syslog(L"BitBlt: dest: %ux%u, size: %ux%u, src: %ux%u\n", 
      //    xdest, ydest, sprite_dx, sprite_dy, hdcMem, xsrc, ysrc);
      BitBlt (hdc, xdest, ydest, sprite_dx, sprite_dy, hdcMem, xsrc, ysrc, SRCCOPY);
   }
   DeleteDC (hdcMem);
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
void gdi_plus::DrawBox(HDC hdc, int x0, int y0, int dx, int dy, COLORREF rColor)  //  NOLINT(readability-convert-member-functions-to-static)
{
   ul2uc_t uconv {};
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
void gdi_plus::DrawBoxFilled(HDC hdc, int xdest, int ydest, int dx, int dy, COLORREF rColor)  //  NOLINT(readability-convert-member-functions-to-static)
{
   ul2uc_t uconv {};
   uconv.ul = (uint) rColor ;
   uconv.uc[3] = 255 ;
   
   Graphics graphics(hdc);
   SolidBrush redBrush(Color(uconv.uc[3], uconv.uc[0], uconv.uc[1], uconv.uc[2]));            
   graphics.FillRectangle(&redBrush, xdest, ydest, dx, dy);
   
}  //lint !e1762

//********************************************************************
//  Pure-GDI+ sprite path, for timing against the BitBlt path.
//  Set the Graphics up once per paint pass, then draw all sprites with it.
//********************************************************************
//  Apply the settings that keep DrawImage on its fast 1:1 copy path.
//  No scaling, so interpolation is irrelevant; half-pixel offset avoids
//  GDI+ resampling the edges.  Default SourceOver compositing is kept
//  so per-pixel alpha still blends correctly.
void gdi_plus::prepare_graphics(Gdiplus::Graphics &graphics)
{
   graphics.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
   graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
   graphics.SetSmoothingMode(Gdiplus::SmoothingModeNone);
}

//********************************************************************
//  Lazily build the premultiplied-ARGB copy of the sprite sheet.
//  DrawImage only takes its fast path from a PARGB source; the loaded
//  PNG is non-premultiplied, which costs a conversion on every draw.
//  Returns false (and logs) if the copy could not be made.
bool gdi_plus::build_pargb(void)
{
   if (ppargb != nullptr) {
      return true;
   }
   auto copy = std::make_unique<Gdiplus::Bitmap>(nWidth, nHeight, PixelFormat32bppPARGB);
   Gdiplus::Graphics g(copy.get());
   g.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);  //  exact pixel copy
   // explicit width/height: avoids DPI-based scaling of the source
   Status status = g.DrawImage(pbitmap.get(), 0, 0, (INT) nWidth, (INT) nHeight);
   if (status != Ok) {
      syslog(L"PARGB build error: %u\n", (uint) status);
      return false;
   }
   ppargb = std::move(copy);
   return true;
}

//********************************************************************
//  Draw one sprite via DrawImage with a source rectangle; no Clone().
//  Dest size == source size (1:1).  Call prepare_graphics() first.
void gdi_plus::draw_sprite_gp(Gdiplus::Graphics &graphics, uint xdest, uint ydest, uint sprite_col, uint sprite_row)
{
   if (!build_pargb()) {
      return;
   }
   graphics.DrawImage(ppargb.get(),
      Gdiplus::Rect((INT) xdest, (INT) ydest, (INT) sprite_dx, (INT) sprite_dy),
      (INT) (sprite_col * sprite_dx), (INT) (sprite_row * sprite_dy),
      (INT) sprite_dx, (INT) sprite_dy,
      Gdiplus::UnitPixel);
}

//********************************************************************
//  Batched BitBlt path: same copy as copy_imagelist_item(), but the
//  memory DC is created and the bitmap selected once per batch instead
//  of once per sprite.
//********************************************************************
//  Open a batch: create a memory DC compatible with hdc and select the
//  sprite sheet into it.  Any batch already open is closed first.
//  Returns false (and logs) on failure.
bool gdi_plus::begin_batch(HDC hdc)
{
   end_batch();
   hdcBatch = CreateCompatibleDC(hdc);
   if (hdcBatch == NULL) {
      syslog(L"CreateCompatibleDC error: %u\n", (uint) GetLastError());
      return false;
   }
   hOldBatch = SelectObject(hdcBatch, hBitmap);
   if (hOldBatch == NULL || hOldBatch == HGDI_ERROR) {
      syslog(L"SelectObject error: GLE [%u]\n", (uint) GetLastError());
      DeleteDC(hdcBatch);
      hdcBatch = NULL;
      hOldBatch = NULL;
      return false;
   }
   return true;
}

//********************************************************************
//  Copy one sprite using the batch DC; does nothing if no batch is open.
void gdi_plus::batch_sprite(HDC hdc, uint xdest, uint ydest, uint sprite_col, uint sprite_row)
{
   if (hdcBatch == NULL) {
      return;
   }
   BitBlt(hdc, (int) xdest, (int) ydest, (int) sprite_dx, (int) sprite_dy,
      hdcBatch, (int) (sprite_col * sprite_dx), (int) (sprite_row * sprite_dy), SRCCOPY);
}

//********************************************************************
//  Close the batch: deselect the sprite sheet and free the memory DC.
//  Safe to call when no batch is open.
void gdi_plus::end_batch(void)
{
   if (hdcBatch != NULL) {
      SelectObject(hdcBatch, hOldBatch);
      DeleteDC(hdcBatch);
      hdcBatch = NULL;
      hOldBatch = NULL;
   }
}

