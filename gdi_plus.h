//*******************************************************************
//  Copyright (c) 2025-2026  Derell Licht
//  This class encapsulates the interface to the LodePNG library.
//********************************************************************
//  WARNING about instantiating this class:
//  
//  It *CANNOT* be statically created like this:
//  The problem is that the gdiplus startup functions have not yet
//  been executed at this point, so the new() operations in this class will fail.
// static gdi_plus pngSprites(_T("tiles32.png"), 40, 26) ;
// static gdi_plus pngTiles  (_T("images.png"), 3, 1) ;
//  
//  Instead, the classes need to be created as pointers:
// static gdi_plus *pngSprites = NULL ;
// static gdi_plus *pngTiles = NULL;
//  Then, at runtime, after the gdiplus startup functions have been executed,
//  then these classes can be dynamically allocated:
//    pngSprites = new gdi_plus(_T("tiles32.png"), 40, 26) ;
//    pngTiles   = new gdi_plus(_T("images.png"), 3, 1) ;
//********************************************************************

// #include <string>
#include <memory>
#include <gdiplus.h>

using namespace Gdiplus;

typedef unsigned int  uint ;

//  our own name for the premultiplied-ARGB pixel format
//  (PixelFormat32bppPARGB is a macro from the GDI+ headers and cannot be renamed)
static constexpr PixelFormat PIXFMT_PRE_ARGB = PixelFormat32bppPARGB ;

//*****************************************************************
//  Drawing techniques being compared; 'G' cycles through them
enum class DrawMode : uint8_t {
   BitBlt,            //  GDI BitBlt, memory DC created per sprite
   BatchBlt,          //  GDI BitBlt, one memory DC per paint
   GdiPlus,           //  GDI+ DrawImage straight to the window
   GdiPlusOffscreen,  //  GDI+ DrawImage into a bitmap, then one blit
   Count              //  number of modes; keep last
};

//*****************************************************************
class gdi_plus {
private:   
   // std::wstring img_name {};
   std::unique_ptr<Bitmap> pbitmap {};
   HBITMAP hBitmap {};
   uint nWidth {};
   uint nHeight {};
   
   uint sprite_dx {};
   uint sprite_dy {};
   uint tiles_x {};
   uint tiles_y {};
   //  premultiplied-ARGB copy of the sheet, built on first DrawImage use
   std::unique_ptr<Bitmap> ppre_argb {};
   //  cached memory DC for the batched BitBlt path (see begin_batch)
   HDC hdcBatch {};
   HGDIOBJ hOldBatch {};

   //  private functions
   void copy_imagelist_item(HDC hdc, int xsrc, int ysrc, int xdest, int ydest);
   bool build_pre_argb(void);
   // void alloc_clone_elements(void);

public:   
   explicit gdi_plus(wchar_t const * const new_img_name) ;
   gdi_plus(wchar_t const * const new_img_name, uint icons_per_column, uint icon_rows);
   gdi_plus(wchar_t const * const new_img_name, uint icons_per_column, uint icon_rows, uint sprite_width, uint sprite_height);
   //  disable the default constructor
   //  not needed if any other constructors exist
   // gdi_plus() = delete;
   ~gdi_plus();
   //  disable the assignment operator and copy constructor
   gdi_plus &operator=(const gdi_plus &src) = delete;
   gdi_plus(const gdi_plus&) = delete;
   //  disable the assignment operator and move constructor
   gdi_plus &operator=(const gdi_plus &&src) = delete;
   gdi_plus(const gdi_plus&&) = delete;
   
   void render_bitmap(HDC hdc, uint xdest, uint ydest);
   void render_bitmap(HDC hdc, uint xdest, uint ydest, uint sprite_col, uint sprite_row);
   //  pure-GDI+ sprite path: call prepare_graphics() once per Graphics object
   // case DrawMode::GdiPlus:          paint_gdiplus(hdc);   break;
   // case DrawMode::GdiPlusOffscreen: paint_offscreen(hdc); break;
   static void prepare_graphics(Gdiplus::Graphics &graphics);
   void draw_sprite_gp(Gdiplus::Graphics &graphics, uint xdest, uint ydest, uint sprite_col, uint sprite_row);

   // case DrawMode::BatchBlt:
   //  batched BitBlt path: begin_batch(), many batch_sprite(), end_batch()
   bool begin_batch(HDC hdc);
   void batch_sprite(HDC hdc, uint xdest, uint ydest, uint sprite_col, uint sprite_row);
   void end_batch(void);
   
   void DrawBox(HDC hdc, int x0, int y0, int dx, int dy, COLORREF rColor);
   void DrawBoxFilled(HDC hdc, int x0, int y0, int dx, int dy, COLORREF rColor);
   [[nodiscard]] uint img_width(void) const
      { return nWidth ; } ;
   [[nodiscard]] uint img_height(void) const
      { return nHeight ; } ;
   [[nodiscard]] uint horiz_tiles(void) const
      { return tiles_x ; } ;
   [[nodiscard]] uint vert_tiles(void) const
      { return tiles_y ; } ;
   [[nodiscard]] uint get_sprite_dx(void) const
      { return sprite_dx ; } ;
   [[nodiscard]] uint get_sprite_dy(void) const
      { return sprite_dy ; } ;
} ;

