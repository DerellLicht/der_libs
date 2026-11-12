//  systray.cpp
//
//  Usage:
//  1. WM_INITDIALOG (or WM_CREATE):  load_tray_menu(), then attach_tray_icon()
//  2. Window procedure, before its main switch, for EVERY message:
//        if (respond_to_taskbar_created(message)) {
//           return TRUE;      // dialog procedure; use 'return 0' in a normal window procedure
//        }
//     This re-adds the icon after Explorer restarts; without it, the icon is lost
//     for good, though the program keeps running.
//  3. Tray messages (WM_USER): respond_to_tray_clicks()
//  4. WM_DESTROY:  release_systray_resource()
void load_tray_menu(WORD menuID);
void attach_tray_icon(HWND hwnd, TCHAR const * const szClassName, WORD iconID);
BOOL respond_to_tray_clicks(HWND hwnd, LPARAM lParam);
BOOL respond_to_taskbar_created(UINT message);
void release_systray_resource(void);

