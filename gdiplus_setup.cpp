#include <windows.h>
#include <tchar.h>

#include "gdi_plus.h"

/************************************************************************/
static ULONG_PTR gdiplusToken;

void init_gdiplus_data(void)
{
   GdiplusStartupInput gdiplusStartupInput;
   
   // Initialize GDI+.
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

/************************************************************************/
void release_gdiplus_data(void)
{
   GdiplusShutdown(gdiplusToken);
}

