//**************************************************************************
//  Copyright (c) 2011-2025  Daniel D Miller
//  This code was removed from common_funcs.cpp because it added 15KB
//  to the output file size, and was rarely needed.
//
//  This module, which has been entirely compiled from public-domain sources, 
//  is itself declared in the public domain.
//**************************************************************************
#include <windows.h>
#include <cstdio>
#include <tchar.h>

//**************************************************************************
typedef struct win_msgs_s {
   unsigned msg_num ;
   TCHAR msg_name[41] ;
} win_msgs_t ;

#define  WM_NOT_FOUND   100000

static win_msgs_t win_msgs[] = {
{ 0, _T("WM_NULL") },
{ 1, _T("WM_CREATE") },
{ 2, _T("WM_DESTROY") },
{ 3, _T("WM_MOVE") },
{ 5, _T("WM_SIZE") },
{ 6, _T("WM_ACTIVATE") },
{ 7, _T("WM_SETFOCUS") },
{ 8, _T("WM_KILLFOCUS") },
{ 10, _T("WM_ENABLE") },
{ 11, _T("WM_SETREDRAW") },
{ 12, _T("WM_SETTEXT") },
{ 13, _T("WM_GETTEXT") },
{ 14, _T("WM_GETTEXTLENGTH") },
{ 15, _T("WM_PAINT") },
{ 16, _T("WM_CLOSE") },
{ 17, _T("WM_QUERYENDSESSION") },
{ 18, _T("WM_QUIT") },
{ 19, _T("WM_QUERYOPEN") },
{ 20, _T("WM_ERASEBKGND") },
{ 21, _T("WM_SYSCOLORCHANGE") },
{ 22, _T("WM_ENDSESSION") },
{ 24, _T("WM_SHOWWINDOW") },
{ 26, _T("WM_SETTINGCHANGE") },
{ 26, _T("WM_WININICHANGE") },
{ 27, _T("WM_DEVMODECHANGE") },
{ 28, _T("WM_ACTIVATEAPP") },
{ 29, _T("WM_FONTCHANGE") },
{ 30, _T("WM_TIMECHANGE") },
{ 31, _T("WM_CANCELMODE") },
{ 32, _T("WM_SETCURSOR") },
{ 33, _T("WM_MOUSEACTIVATE") },
{ 34, _T("WM_CHILDACTIVATE") },
{ 35, _T("WM_QUEUESYNC") },
{ 36, _T("WM_GETMINMAXINFO") },
{ 38, _T("WM_PAINTICON") },
{ 39, _T("WM_ICONERASEBKGND") },
{ 40, _T("WM_NEXTDLGCTL") },
{ 42, _T("WM_SPOOLERSTATUS") },
{ 43, _T("WM_DRAWITEM") },
{ 44, _T("WM_MEASUREITEM") },
{ 45, _T("WM_DELETEITEM") },
{ 46, _T("WM_VKEYTOITEM") },
{ 47, _T("WM_CHARTOITEM") },
{ 48, _T("WM_SETFONT") },
{ 49, _T("WM_GETFONT") },
{ 50, _T("WM_SETHOTKEY") },
{ 51, _T("WM_GETHOTKEY") },
{ 55, _T("WM_QUERYDRAGICON") },
{ 57, _T("WM_COMPAREITEM") },
{ 61, _T("WM_GETOBJECT") },
{ 65, _T("WM_COMPACTING") },
{ 68, _T("WM_COMMNOTIFY") },
{ 70, _T("WM_WINDOWPOSCHANGING") },
{ 71, _T("WM_WINDOWPOSCHANGED") },
{ 72, _T("WM_POWER") },
{ 74, _T("WM_COPYDATA") },
{ 75, _T("WM_CANCELJOURNAL") },
{ 78, _T("WM_NOTIFY") },
{ 80, _T("WM_INPUTLANGCHANGEREQUEST") },
{ 81, _T("WM_INPUTLANGCHANGE") },
{ 82, _T("WM_TCARD") },
{ 83, _T("WM_HELP") },
{ 84, _T("WM_USERCHANGED") },
{ 85, _T("WM_NOTIFYFORMAT") },
{ 123, _T("WM_CONTEXTMENU") },
{ 124, _T("WM_STYLECHANGING") },
{ 125, _T("WM_STYLECHANGED") },
{ 126, _T("WM_DISPLAYCHANGE") },
{ 127, _T("WM_GETICON") },
{ 128, _T("WM_SETICON") },
{ 129, _T("WM_NCCREATE") },
{ 130, _T("WM_NCDESTROY") },
{ 131, _T("WM_NCCALCSIZE") },
{ 132, _T("WM_NCHITTEST") },
{ 133, _T("WM_NCPAINT") },
{ 134, _T("WM_NCACTIVATE") },
{ 135, _T("WM_GETDLGCODE") },
{ 136, _T("WM_SYNCPAINT ") },     // 0x88
//  from http://blog.airesoft.co.uk/2009/11/wm_messages/
// #define WM_SYNCTASK 0x0089
// // <undefined> 0x008a
// #define WM_KLUDGEMINRECT 0x008b
// #define WM_LPKDRAWSWITCHWND 0x008c
// // <undefined> 0x008d
// // <undefined> 0x008e
// // <undefined> 0x008f
// #define WM_UAHDESTROYWINDOW 0x0090
// #define WM_UAHDRAWMENU 0x0091
// #define WM_UAHDRAWMENUITEM 0x0092
// #define WM_UAHINITMENU 0x0093
// #define WM_UAHMEASUREMENUITEM 0x0094
// #define WM_UAHNCPAINTMENUPOPUP 0x0095
{ 137, _T("WM_SYNCTASK") },               // 0x0089
{ 139, _T("WM_KLUDGEMINRECT") },          // 0x008b
{ 140, _T("WM_LPKDRAWSWITCHWND") },       // 0x008c
{ 144, _T("WM_UAHDESTROYWINDOW") },       // 0x0090
{ 145, _T("WM_UAHDRAWMENU") },            // 0x0091
{ 146, _T("WM_UAHDRAWMENUITEM") },        // 0x0092
{ 147, _T("WM_UAHINITMENU") },            // 0x0093
{ 148, _T("WM_UAHMEASUREMENUITEM") },     // 0x0094
{ 149, _T("WM_UAHNCPAINTMENUPOPUP") },    // 0x0095
{ 160, _T("WM_NCMOUSEMOVE") },    // 0xA0
{ 161, _T("WM_NCLBUTTONDOWN") },
{ 162, _T("WM_NCLBUTTONUP") },
{ 163, _T("WM_NCLBUTTONDBLCLK") },
{ 164, _T("WM_NCRBUTTONDOWN") },
{ 165, _T("WM_NCRBUTTONUP") },
{ 166, _T("WM_NCRBUTTONDBLCLK") },
{ 167, _T("WM_NCMBUTTONDOWN") },
{ 168, _T("WM_NCMBUTTONUP") },
{ 169, _T("WM_NCMBUTTONDBLCLK") },
{ 171, _T("WM_NCXBUTTONDOWN") },
{ 172, _T("WM_NCXBUTTONUP") },
{ 173, _T("WM_NCXBUTTONDBLCLK") },
// http://social.msdn.microsoft.com/Forums/en-US/vbgeneral/thread/49e84457-42fe-42d3-ac93-4749b67ab9a2
// Wednesday, November 17, 2010 1:43 AM
// Tom Shelton, 8,570 Points
// Apparently, 0x00AE and 0x00AF are undocumented windows messages related to themes:
// WM_NCUAHDRAWCAPTION = 0x00AE
// WM_NCUAHDRAWFRAME = 0x00AF
{ 174, _T("WM_NCUAHDRAWCAPTION") },  // WM_NCUAHDRAWCAPTION = 0x00AE
{ 175, _T("WM_NCUAHDRAWFRAME") },    // WM_NCUAHDRAWFRAME = 0x00AF
{ 177, _T("EM_SETSEL") },
{ 255, _T("WM_INPUT") },
{ 256, _T("WM_KEYDOWN") },
{ 256, _T("WM_KEYFIRST") },
{ 257, _T("WM_KEYUP") },
{ 258, _T("WM_CHAR") },
{ 259, _T("WM_DEADCHAR") },
{ 260, _T("WM_SYSKEYDOWN") },
{ 261, _T("WM_SYSKEYUP") },
{ 262, _T("WM_SYSCHAR") },
{ 263, _T("WM_SYSDEADCHAR") },
{ 264, _T("WM_KEYLAST") },
{ 265, _T("WM_UNICHAR") },
{ 269, _T("WM_IME_STARTCOMPOSITION") },
{ 270, _T("WM_IME_ENDCOMPOSITION") },
{ 271, _T("WM_IME_KEYLAST") },
{ 272, _T("WM_INITDIALOG") },
{ 273, _T("WM_COMMAND") },
{ 274, _T("WM_SYSCOMMAND") },
{ 275, _T("WM_TIMER") },
{ 276, _T("WM_HSCROLL") },
{ 277, _T("WM_VSCROLL") },
{ 278, _T("WM_INITMENU") },
{ 279, _T("WM_INITMENUPOPUP") },
// #if(WINVER >= 0x0601)
{ 281, _T("WM_GESTURE") }, //                     0x0119
{ 282, _T("WM_GESTURENOTIFY") }, //                0x011A

{ 287, _T("WM_MENUSELECT") },
{ 288, _T("WM_MENUCHAR") },
{ 289, _T("WM_ENTERIDLE") },
{ 290, _T("WM_MENURBUTTONUP") },
{ 291, _T("WM_MENUDRAG") },
{ 292, _T("WM_MENUGETOBJECT") },
{ 293, _T("WM_UNINITMENUPOPUP") },
{ 294, _T("WM_MENUCOMMAND") },
{ 295, _T("WM_CHANGEUISTATE") },
{ 296, _T("WM_UPDATEUISTATE") },
{ 297, _T("WM_QUERYUISTATE") },
{ 306, _T("WM_CTLCOLORMSGBOX") },
{ 307, _T("WM_CTLCOLOREDIT") },
{ 308, _T("WM_CTLCOLORLISTBOX") },
{ 309, _T("WM_CTLCOLORBTN") },
{ 310, _T("WM_CTLCOLORDLG") },
{ 311, _T("WM_CTLCOLORSCROLLBAR") },
{ 312, _T("WM_CTLCOLORSTATIC") },

/*
 * Combo Box messages
 */
{ 320, _T("CB_GETEDITSEL") },  //               0x0140
{ 321, _T("CB_LIMITTEXT") },  //                0x0141
{ 322, _T("CB_SETEDITSEL") },  //               0x0142
{ 323, _T("CB_ADDSTRING") },  //                0x0143
{ 324, _T("CB_DELETESTRING") },  //             0x0144
{ 325, _T("CB_DIR") },  //                      0x0145
{ 326, _T("CB_GETCOUNT") },  //                 0x0146
{ 327, _T("CB_GETCURSEL") },  //                0x0147
{ 328, _T("CB_GETLBTEXT") },  //                0x0148
{ 329, _T("CB_GETLBTEXTLEN") },  //             0x0149
{ 330, _T("CB_INSERTSTRING") },  //             0x014A
{ 331, _T("CB_RESETCONTENT") },  //             0x014B
{ 332, _T("CB_FINDSTRING") },  //               0x014C
{ 333, _T("CB_SELECTSTRING") },  //             0x014D
{ 334, _T("CB_SETCURSEL") },  //                0x014E
{ 335, _T("CB_SHOWDROPDOWN") },  //             0x014F
{ 336, _T("CB_GETITEMDATA") },  //              0x0150
{ 337, _T("CB_SETITEMDATA") },  //              0x0151
{ 338, _T("CB_GETDROPPEDCONTROLRECT") },  //    0x0152
{ 339, _T("CB_SETITEMHEIGHT") },  //            0x0153
{ 340, _T("CB_GETITEMHEIGHT") },  //            0x0154
{ 341, _T("CB_SETEXTENDEDUI") },  //            0x0155
{ 342, _T("CB_GETEXTENDEDUI") },  //            0x0156
{ 343, _T("CB_GETDROPPEDSTATE") },  //          0x0157
{ 344, _T("CB_FINDSTRINGEXACT") },  //          0x0158
{ 345, _T("CB_SETLOCALE") },  //                0x0159
{ 346, _T("CB_GETLOCALE") },  //                0x015A
{ 347, _T("CB_GETTOPINDEX") },  //              0x015b
{ 348, _T("CB_SETTOPINDEX") },  //              0x015c
{ 349, _T("CB_GETHORIZONTALEXTENT") },  //      0x015d
{ 350, _T("CB_SETHORIZONTALEXTENT") },  //      0x015e
{ 351, _T("CB_GETDROPPEDWIDTH") },  //          0x015f
{ 352, _T("CB_SETDROPPEDWIDTH") },  //          0x0160
{ 353, _T("CB_INITSTORAGE") },  //              0x0161
{ 354, _T("CB_MULTIPLEADDSTRING") },  //        0x0163
{ 355, _T("CB_GETCOMBOBOXINFO") },  //          0x0164

// { 512, _T("WM_MOUSEFIRST") },
{ 512, _T("WM_MOUSEMOVE") },
{ 513, _T("WM_LBUTTONDOWN") },
{ 514, _T("WM_LBUTTONUP") },
{ 515, _T("WM_LBUTTONDBLCLK") },
{ 516, _T("WM_RBUTTONDOWN") },
{ 517, _T("WM_RBUTTONUP") },
{ 518, _T("WM_RBUTTONDBLCLK") },
{ 519, _T("WM_MBUTTONDOWN") },
{ 520, _T("WM_MBUTTONUP") },
{ 521, _T("WM_MBUTTONDBLCLK") },
{ 522, _T("WM_MOUSEWHEEL") },
{ 523, _T("WM_XBUTTONDOWN") },
{ 524, _T("WM_XBUTTONUP") },
{ 525, _T("WM_MOUSELAST") },
{ 528, _T("WM_PARENTNOTIFY") },
{ 529, _T("WM_ENTERMENULOOP") },
{ 530, _T("WM_EXITMENULOOP") },
{ 531, _T("WM_NEXTMENU") },
{ 532, _T("WM_SIZING") },
{ 533, _T("WM_CAPTURECHANGED") },
{ 534, _T("WM_MOVING") },
{ 536, _T("WM_POWERBROADCAST") },
{ 537, _T("WM_DEVICECHANGE") },
{ 544, _T("WM_MDICREATE") },
{ 545, _T("WM_MDIDESTROY") },
{ 546, _T("WM_MDIACTIVATE") },
{ 547, _T("WM_MDIRESTORE") },
{ 548, _T("WM_MDINEXT") },
{ 549, _T("WM_MDIMAXIMIZE") },
{ 550, _T("WM_MDITILE") },
{ 551, _T("WM_MDICASCADE") },
{ 552, _T("WM_MDIICONARRANGE") },
{ 553, _T("WM_MDIGETACTIVE") },
{ 560, _T("WM_MDISETMENU") },
{ 561, _T("WM_ENTERSIZEMOVE") },
{ 562, _T("WM_EXITSIZEMOVE") },
{ 563, _T("WM_DROPFILES") },
{ 564, _T("WM_MDIREFRESHMENU") },
// http://msdn.microsoft.com/en-us/library/windows/desktop/hh454922(v=vs.85).aspx
{ 568, _T("WM_POINTERDEVICECHANGE") },  //       0X238
{ 569, _T("WM_POINTERDEVICEINRANGE") },  //     0X239
// { WM_NOT_FOUND, _T("WM_NOT_FOUND") } };
// 
// static win_msgs_t win_msgsx[] = {

// #if (_WIN32_WINNT >= 0x0601)
{ 576, _T("WM_TOUCHMOVE") },
{ 577, _T("WM_TOUCHDOWN") },
{ 578, _T("WM_TOUCHUP") },
{ 588, _T("WM_POINTERCAPTURECHANGED") },  //           0x024C

{ 641, _T("WM_IME_SETCONTEXT") },
{ 642, _T("WM_IME_NOTIFY") },
{ 643, _T("WM_IME_CONTROL") },
{ 644, _T("WM_IME_COMPOSITIONFULL") },
{ 645, _T("WM_IME_SELECT") },
{ 646, _T("WM_IME_CHAR") },
{ 648, _T("WM_IME_REQUEST") },
{ 656, _T("WM_IME_KEYDOWN") },
{ 657, _T("WM_IME_KEYUP") },
{ 672, _T("WM_NCMOUSEHOVER") },
{ 673, _T("WM_MOUSEHOVER") },
{ 674, _T("WM_NCMOUSELEAVE") },
{ 675, _T("WM_MOUSELEAVE") },
{ 689, _T("WM_WTSSESSION_CHANGE") },
{ 704, _T("WM_TABLET_FIRST") },
{ 735, _T("WM_TABLET_LAST") },
{ 768, _T("WM_CUT") },
{ 769, _T("WM_COPY") },
{ 770, _T("WM_PASTE") },
{ 771, _T("WM_CLEAR") },
{ 772, _T("WM_UNDO") },
{ 773, _T("WM_RENDERFORMAT") },
{ 774, _T("WM_RENDERALLFORMATS") },
{ 775, _T("WM_DESTROYCLIPBOARD") },
{ 776, _T("WM_DRAWCLIPBOARD") },
{ 777, _T("WM_PAINTCLIPBOARD") },
{ 778, _T("WM_VSCROLLCLIPBOARD") },
{ 779, _T("WM_SIZECLIPBOARD") },
{ 780, _T("WM_ASKCBFORMATNAME") },
{ 781, _T("WM_CHANGECBCHAIN") },
{ 782, _T("WM_HSCROLLCLIPBOARD") },
{ 783, _T("WM_QUERYNEWPALETTE") },
{ 784, _T("WM_PALETTEISCHANGING") },
{ 785, _T("WM_PALETTECHANGED") },
{ 786, _T("WM_HOTKEY") },
{ 791, _T("WM_PRINT") },
{ 792, _T("WM_PRINTCLIENT") },
{ 793, _T("WM_APPCOMMAND") },
{ 794, _T("WM_THEMECHANGED") },
// http://sda-framework.googlecode.com/svn-history/r37/trunk/sdaUnits/sdaMessages.pas
// WM_DWMCOMPOSITIONCHANGED          = $031E;
// WM_DWMNCRENDERINGCHANGED          = $031F;
// WM_DWMCOLORIZATIONCOLORCHANGED    = $0320;
// WM_DWMWINDOWMAXIMIZEDCHANGE       = $0321;
// WM_DWMSENDICONICTHUMBNAIL         = $0323;
// WM_DWMSENDICONICLIVEPREVIEWBITMAP = $0326;
// WM_GETTITLEBARINFOEX              = $033F;
{ 798, _T("WM_DWMCOMPOSITIONCHANGED") },         // 0x031E;
{ 799, _T("WM_DWMNCRENDERINGCHANGED") },         // 0x031F;
{ 800, _T("WM_DWMCOLORIZATIONCOLORCHANGED") },   // 0x0320;
{ 801, _T("WM_DWMWINDOWMAXIMIZEDCHANGE") },      // 0x0321;
{ 803, _T("WM_DWMSENDICONICTHUMBNAIL") },        // 0x0323;
{ 806, _T("WM_DWMSENDICONICLIVEPREVIEWBITMAP") },// 0x0326;
{ 831, _T("WM_DWMSENDICONICLIVEPREVIEWBITMAP") },// 0x033F;

{ 856, _T("WM_HANDHELDFIRST") },
{ 863, _T("WM_HANDHELDLAST") },
{ 864, _T("WM_AFXFIRST") },
{ 895, _T("WM_AFXLAST") },
{ 896, _T("WM_PENWINFIRST") },
{ 911, _T("WM_PENWINLAST") },

// http://sda-framework.googlecode.com/svn-history/r37/trunk/sdaUnits/sdaMessages.pas
// { 831, "WM_DDE_FIRST    " },   // $03E0;
{ 992, _T("WM_DDE_INITIATE ") },   // WM_DDE_FIRST + 0;
{ 993, _T("WM_DDE_TERMINATE") },   // WM_DDE_FIRST + 1;
{ 994, _T("WM_DDE_ADVISE   ") },   // WM_DDE_FIRST + 2;
{ 995, _T("WM_DDE_UNADVISE ") },   // WM_DDE_FIRST + 3;
{ 996, _T("WM_DDE_ACK      ") },   // WM_DDE_FIRST + 4;
{ 997, _T("WM_DDE_DATA     ") },   // WM_DDE_FIRST + 5;
{ 998, _T("WM_DDE_REQUEST  ") },   // WM_DDE_FIRST + 6;
{ 999, _T("WM_DDE_POKE     ") },   // WM_DDE_FIRST + 7;
{ 1000, _T("WM_DDE_EXECUTE  ") },  // WM_DDE_FIRST + 8;

{ 1024, _T("DM_GETDEFID") },
{ 1025, _T("DM_SETDEFID") },
{ 1026, _T("DM_REPOSITION") },

//  PSM mesages
{ 1125, _T("PSM_SETCURSEL") },
{ 1126, _T("PSM_REMOVEPAGE") },
{ 1127, _T("PSM_ADDPAGE") },  // { 1127, _T("WM_DO_COMM_TASK") },
{ 1128, _T("PSM_CHANGED") },  // { 1128, _T("WM_COMM_TASK_DONE") },
{ 1129, _T("PSM_RESTARTWINDOWS") },   
{ 1130, _T("PSM_REBOOTSYSTEM") },     
{ 1131, _T("PSM_CANCELTOCLOSE") },    
{ 1132, _T("PSM_QUERYSIBLINGS") },    
{ 1133, _T("PSM_UNCHANGED") },        
{ 1134, _T("PSM_APPLY") },            
{ 1135, _T("PSM_SETTITLEA") },        
{ 1136, _T("PSM_SETWIZBUTTONS") },    
{ 1137, _T("PSM_PRESSBUTTON") },         //  WM_USER + 113
{ 1138, _T("PSM_SETCURSELID") },         
{ 1139, _T("PSM_SETFINISHTEXTA") },      
{ 1140, _T("PSM_GETTABCONTROL") },       
{ 1141, _T("PSM_ISDIALOGMESSAGE") },     
{ 1142, _T("PSM_GETCURRENTPAGEHWND") },  
{ 1144, _T("PSM_SETTITLEW") },
{ 1145, _T("PSM_SETFINISHTEXTW") },      

{ 4096, _T("LVM_GETBKCOLOR") },
{ 4097, _T("LVM_SETBKCOLOR") },
{ 4098, _T("LVM_GETIMAGELIST") },
{ 4099, _T("LVM_SETIMAGELIST") },
{ 4100, _T("LVM_GETITEMCOUNT") },
{ 4101, _T("LVM_GETITEMA") },
{ 4102, _T("LVM_SETITEMA") },
{ 4103, _T("LVM_INSERTITEMA") },
{ 4104, _T("LVM_DELETEITEM") },
{ 4105, _T("LVM_DELETEALLITEMS") },
{ 4106, _T("LVM_GETCALLBACKMASK") },
{ 4107, _T("LVM_SETCALLBACKMASK") },
{ 4108, _T("LVM_GETNEXTITEM") },
{ 4109, _T("LVM_FINDITEMA") },
{ 4110, _T("LVM_GETITEMRECT") },
{ 4111, _T("LVM_SETITEMPOSITION") },
{ 4112, _T("LVM_GETITEMPOSITION") },
{ 4113, _T("LVM_GETSTRINGWIDTHA") },
{ 4114, _T("LVM_HITTEST") },
{ 4115, _T("LVM_ENSUREVISIBLE") },
{ 4116, _T("LVM_SCROLL") },
{ 4117, _T("LVM_REDRAWITEMS") },
{ 4118, _T("LVM_ARRANGE") },
{ 4119, _T("LVM_EDITLABELA") },
{ 4120, _T("LVM_GETEDITCONTROL") },
{ 4121, _T("LVM_GETCOLUMNA") },
{ 4122, _T("LVM_SETCOLUMNA") },
{ 4123, _T("LVM_INSERTCOLUMNA") },
{ 4124, _T("LVM_DELETECOLUMN") },
{ 4125, _T("LVM_GETCOLUMNWIDTH") },
{ 4126, _T("LVM_SETCOLUMNWIDTH") },
{ 4127, _T("LVM_GETHEADER") },
{ 4129, _T("LVM_CREATEDRAGIMAGE") },
{ 4130, _T("LVM_GETVIEWRECT") },
{ 4131, _T("LVM_GETTEXTCOLOR") },
{ 4132, _T("LVM_SETTEXTCOLOR") },
{ 4133, _T("LVM_GETTEXTBKCOLOR") },
{ 4134, _T("LVM_SETTEXTBKCOLOR") },
{ 4135, _T("LVM_GETTOPINDEX") },
{ 4136, _T("LVM_GETCOUNTPERPAGE") },
{ 4137, _T("LVM_GETORIGIN") },
{ 4137, _T("LVM_GETORIGIN") },
{ 4138, _T("LVM_UPDATE") },
{ 4139, _T("LVM_SETITEMSTATE") },
{ 4140, _T("LVM_GETITEMSTATE") },
{ 4141, _T("LVM_GETITEMTEXTA") },
{ 4142, _T("LVM_SETITEMTEXTA") },
{ 4143, _T("LVM_SETITEMCOUNT") },
{ 4144, _T("LVM_SORTITEMS") },
{ 4145, _T("LVM_SETITEMPOSITION32") },
{ 4146, _T("LVM_GETSELECTEDCOUNT") },
{ 4147, _T("LVM_GETITEMSPACING") },
{ 4148, _T("LVM_GETISEARCHSTRINGA") },
{ 4149, _T("LVM_SETICONSPACING") },
{ 4150, _T("LVM_SETEXTENDEDLISTVIEWSTYLE") },
{ 4151, _T("LVM_GETEXTENDEDLISTVIEWSTYLE") },
{ 4152, _T("LVM_GETSUBITEMRECT") },
{ 4153, _T("LVM_SUBITEMHITTEST") },
{ 4154, _T("LVM_SETCOLUMNORDERARRAY") },
{ 4155, _T("LVM_GETCOLUMNORDERARRAY") },
{ 4156, _T("LVM_SETHOTITEM") },
{ 4157, _T("LVM_GETHOTITEM") },
{ 4158, _T("LVM_SETHOTCURSOR") },
{ 4159, _T("LVM_GETHOTCURSOR") },
{ 4160, _T("LVM_APPROXIMATEVIEWRECT") },
{ 4161, _T("LVM_SETWORKAREAS") },
{ 4162, _T("LVM_GETSELECTIONMARK") },
{ 4163, _T("LVM_SETSELECTIONMARK") },
{ 4164, _T("LVM_SETBKIMAGEA") },
{ 4165, _T("LVM_GETBKIMAGEA") },
{ 4166, _T("LVM_GETWORKAREAS") },
{ 4167, _T("LVM_SETHOVERTIME") },
{ 4168, _T("LVM_GETHOVERTIME") },
{ 4169, _T("LVM_GETNUMBEROFWORKAREAS") },
{ 4170, _T("LVM_SETTOOLTIPS") },
{ 4171, _T("LVM_GETITEMW") },
{ 4172, _T("LVM_SETITEMW") },
{ 4173, _T("LVM_INSERTITEMW") },
{ 4174, _T("LVM_GETTOOLTIPS") },
{ 4177, _T("LVM_SORTITEMSEX") },
{ 4179, _T("LVM_FINDITEMW") },
{ 4183, _T("LVM_GETSTRINGWIDTHW") },
{ 4191, _T("LVM_GETCOLUMNW") },
{ 4192, _T("LVM_SETCOLUMNW") },
{ 4193, _T("LVM_INSERTCOLUMNW") },
{ 4211, _T("LVM_GETITEMTEXTW") },
{ 4212, _T("LVM_SETITEMTEXTW") },
{ 4213, _T("LVM_GETISEARCHSTRINGW") },
{ 4214, _T("LVM_EDITLABELW") },
{ 4234, _T("LVM_SETBKIMAGEW") },
{ 4235, _T("LVM_GETBKIMAGEW") },
{ 4236, _T("LVM_SETSELECTEDCOLUMN") },
{ 4238, _T("LVM_SETVIEW") },
{ 4239, _T("LVM_GETVIEW") },
{ 4241, _T("LVM_INSERTGROUP") },
{ 4243, _T("LVM_SETGROUPINFO") },
{ 4245, _T("LVM_GETGROUPINFO") },
{ 4246, _T("LVM_REMOVEGROUP") },
{ 4247, _T("LVM_MOVEGROUP") },
{ 4251, _T("LVM_SETGROUPMETRICS") },
{ 4252, _T("LVM_GETGROUPMETRICS") },
{ 4253, _T("LVM_ENABLEGROUPVIEW") },
{ 4254, _T("LVM_SORTGROUPS") },
{ 4255, _T("LVM_INSERTGROUPSORTED") },
{ 4256, _T("LVM_REMOVEALLGROUPS") },
{ 4257, _T("LVM_HASGROUP") },
{ 4258, _T("LVM_SETTILEVIEWINFO") },
{ 4259, _T("LVM_GETTILEVIEWINFO") },
{ 4260, _T("LVM_SETTILEINFO") },
{ 4261, _T("LVM_GETTILEINFO") },
{ 4262, _T("LVM_SETINSERTMARK") },
{ 4263, _T("LVM_GETINSERTMARK") },
{ 4264, _T("LVM_INSERTMARKHITTEST") },
{ 4265, _T("LVM_GETINSERTMARKRECT") },
{ 4266, _T("LVM_SETINSERTMARKCOLOR") },
{ 4267, _T("LVM_GETINSERTMARKCOLOR") },
{ 4269, _T("LVM_SETINFOTIP") },
{ 4270, _T("LVM_GETSELECTEDCOLUMN") },
{ 4271, _T("LVM_ISGROUPVIEWENABLED") },
{ 4272, _T("LVM_GETOUTLINECOLOR") },
{ 4273, _T("LVM_SETOUTLINECOLOR") },
{ 4275, _T("LVM_CANCELEDITLABEL") },
{ 4277, _T("LVM_MAPIDTOINDEX") },
{ 4352, _T("TVM_INSERTITEMA") },
{ 4353, _T("TVM_DELETEITEM") },
{ 4354, _T("TVM_EXPAND") },
{ 4356, _T("TVM_GETITEMRECT") },
{ 4357, _T("TVM_GETCOUNT") },
{ 4358, _T("TVM_GETINDENT") },
{ 4359, _T("TVM_SETINDENT") },
{ 4360, _T("TVM_GETIMAGELIST") },
{ 4361, _T("TVM_SETIMAGELIST") },
{ 4362, _T("TVM_GETNEXTITEM") },
{ 4363, _T("TVM_SELECTITEM") },
{ 4364, _T("TVM_GETITEMA") },
{ 4365, _T("TVM_SETITEMA") },
{ 4366, _T("TVM_EDITLABELA") },
{ 4367, _T("TVM_GETEDITCONTROL") },
{ 4368, _T("TVM_GETVISIBLECOUNT") },
{ 4369, _T("TVM_HITTEST") },
{ 4370, _T("TVM_CREATEDRAGIMAGE") },
{ 4371, _T("TVM_SORTCHILDREN") },
{ 4372, _T("TVM_ENSUREVISIBLE") },
{ 4373, _T("TVM_SORTCHILDRENCB") },
{ 4374, _T("TVM_ENDEDITLABELNOW") },
{ 4375, _T("TVM_GETISEARCHSTRINGA") },
{ 4376, _T("TVM_SETTOOLTIPS") },
{ 4377, _T("TVM_GETTOOLTIPS") },
{ 4378, _T("TVM_SETINSERTMARK") },
{ 4379, _T("TVM_SETITEMHEIGHT") },
{ 4380, _T("TVM_GETITEMHEIGHT") },
{ 4381, _T("TVM_SETBKCOLOR") },
{ 4382, _T("TVM_SETTEXTCOLOR") },
{ 4383, _T("TVM_GETBKCOLOR") },
{ 4384, _T("TVM_GETTEXTCOLOR") },
{ 4385, _T("TVM_SETSCROLLTIME") },
{ 4386, _T("TVM_GETSCROLLTIME") },
{ 4389, _T("TVM_SETINSERTMARKCOLOR") },
{ 4390, _T("TVM_GETINSERTMARKCOLOR") },
{ 4391, _T("TVM_GETITEMSTATE") },
{ 4392, _T("TVM_SETLINECOLOR") },
{ 4393, _T("TVM_GETLINECOLOR") },
{ 4402, _T("TVM_INSERTITEMW") },
{ 4414, _T("TVM_GETITEMW") },
{ 4415, _T("TVM_SETITEMW") },
{ 4416, _T("TVM_GETISEARCHSTRINGW") },
{ 4417, _T("TVM_EDITLABELW") },
{ 4608, _T("HDM_GETITEMCOUNT") },
{ 4609, _T("HDM_INSERTITEMA") },
{ 4610, _T("HDM_DELETEITEM") },
{ 4611, _T("HDM_GETITEMA") },
{ 4612, _T("HDM_SETITEMA") },
{ 4613, _T("HDM_LAYOUT") },
{ 4614, _T("HDM_HITTEST") },
{ 4615, _T("HDM_GETITEMRECT") },
{ 4616, _T("HDM_SETIMAGELIST") },
{ 4617, _T("HDM_GETIMAGELIST") },
{ 4618, _T("HDM_INSERTITEMW") },
{ 4619, _T("HDM_GETITEMW") },
{ 4620, _T("HDM_SETITEMW") },
{ 4623, _T("HDM_ORDERTOINDEX") },
{ 4624, _T("HDM_CREATEDRAGIMAGE") },
{ 4625, _T("HDM_GETORDERARRAY") },
{ 4626, _T("HDM_SETORDERARRAY") },
{ 4627, _T("HDM_SETHOTDIVIDER") },
{ 4866, _T("TCM_GETIMAGELIST") },
{ 4867, _T("TCM_SETIMAGELIST") },
{ 4868, _T("TCM_GETITEMCOUNT") },
{ 4869, _T("TCM_GETITEMA") },
{ 4870, _T("TCM_SETITEMA") },
{ 4871, _T("TCM_INSERTITEMA") },
{ 4872, _T("TCM_DELETEITEM") },
{ 4873, _T("TCM_DELETEALLITEMS") },
{ 4874, _T("TCM_GETITEMRECT") },
{ 4875, _T("TCM_GETCURSEL") },
{ 4876, _T("TCM_SETCURSEL") },
{ 4877, _T("TCM_HITTEST") },
{ 4878, _T("TCM_SETITEMEXTRA") },
{ 4904, _T("TCM_ADJUSTRECT") },
{ 4905, _T("TCM_SETITEMSIZE") },
{ 4906, _T("TCM_REMOVEIMAGE") },
{ 4907, _T("TCM_SETPADDING") },
{ 4908, _T("TCM_GETROWCOUNT") },
{ 4909, _T("TCM_GETTOOLTIPS") },
{ 4910, _T("TCM_SETTOOLTIPS") },
{ 4911, _T("TCM_GETCURFOCUS") },
{ 4912, _T("TCM_SETCURFOCUS") },
{ 4913, _T("TCM_SETMINTABWIDTH") },
{ 4914, _T("TCM_DESELECTALL") },
{ 4915, _T("TCM_HIGHLIGHTITEM") },
{ 4916, _T("TCM_SETEXTENDEDSTYLE") },
{ 4917, _T("TCM_GETEXTENDEDSTYLE") },
{ 4924, _T("TCM_GETITEMW") },
{ 4925, _T("TCM_SETITEMW") },
{ 4926, _T("TCM_INSERTITEMW") },
{ 5121, _T("PGM_SETCHILD") },
{ 5122, _T("PGM_RECALCSIZE") },
{ 5123, _T("PGM_FORWARDMOUSE") },
{ 5124, _T("PGM_SETBKCOLOR") },
{ 5125, _T("PGM_GETBKCOLOR") },
{ 5126, _T("PGM_SETBORDER") },
{ 5127, _T("PGM_GETBORDER") },
{ 5128, _T("PGM_SETPOS") },
{ 5129, _T("PGM_GETPOS") },
{ 5130, _T("PGM_SETBUTTONSIZE") },
{ 5131, _T("PGM_GETBUTTONSIZE") },
{ 5132, _T("PGM_GETBUTTONSTATE") },

{ 32768, _T("WM_APP") },
{ WM_NOT_FOUND, _T("WM_NOT_FOUND") } };


// #define NM_FIRST  0
// #define NM_LAST   ((UINT)-99)
// #define LVN_FIRST ((UINT)-100)
// #define LVN_LAST  ((UINT)-199)
// #define HDN_FIRST ((UINT)-300)
// #define HDN_LAST  ((UINT)-399)
// #define TVN_FIRST ((UINT)-400)
// #define TVN_LAST  ((UINT)-499)
// #define TTN_FIRST ((UINT)-520)
// #define TTN_LAST  ((UINT)-549)
// #define TCN_FIRST ((UINT)-550)
// #define TCN_LAST  ((UINT)-580)
// #ifndef CDN_FIRST
// #define CDN_FIRST ((UINT)-601) /* also in commdlg.h */
// #define CDN_LAST  ((UINT)-699)
// #endif
// #define TBN_FIRST ((UINT)-700)
// #define TBN_LAST  ((UINT)-720)
// #define UDN_FIRST ((UINT)-721)
// #define UDN_LAST  ((UINT)-740)
// #if (_WIN32_IE >= 0x0300)
// #define RBN_FIRST ((UINT)-831)
// #define RBN_LAST  ((UINT)-859)
// #define MCN_FIRST ((UINT)-750)
// #define MCN_LAST  ((UINT)-759)
// #define DTN_FIRST ((UINT)-760)
// #define DTN_LAST  ((UINT)-799)
// #define CBEN_FIRST   ((UINT)-800)
// #define CBEN_LAST ((UINT)-830)
// #endif /* _WIN32_IE */
// #if (_WIN32_IE >= 0x0400)
// #define IPN_FIRST ((UINT)-860)
// #define IPN_LAST  ((UINT)-879)
// #define IPN_FIELDCHANGED   (IPN_FIRST-0)
// #define SBN_FIRST ((UINT)-880U)
// #define SBN_LAST  ((UINT)-899U)
// #define PGN_FIRST ((UINT)-900U)
// #define PGN_LAST  ((UINT)-950U)
// #define PGN_SCROLL   (PGN_FIRST-1)
// #define PGN_CALCSIZE (PGN_FIRST-2)

// #define TTN_FIRST ((UINT)-520)

static win_msgs_t win_neg_msgs[] = {
{ 0, _T("WM_NULL") },

{ 1,  _T("NM_OUTOFMEMORY") },
{ 2,  _T("NM_CLICK") },
{ 3,  _T("NM_DBLCLK") },          
{ 4,  _T("NM_RETURN") },          
{ 5,  _T("NM_RCLICK") },          
{ 6,  _T("NM_RDBLCLK") },         
{ 7,  _T("NM_SETFOCUS") },        
{ 8,  _T("NM_KILLFOCUS") },       
{ 12, _T("NM_CUSTOMDRAW") },      
{ 13, _T("NM_HOVER") },           
{ 14, _T("NM_NCHITTEST") },       
{ 15, _T("NM_KEYDOWN") },         
{ 16, _T("NM_RELEASEDCAPTURE") }, 
{ 17, _T("NM_SETCURSOR") },       
{ 18, _T("NM_CHAR") },            
{ 19, _T("NM_TOOLTIPSCREATED") }, 

{ 100, _T("LVN_ITEMCHANGING") },     
{ 101, _T("LVN_ITEMCHANGED") },      
{ 102, _T("LVN_INSERTITEM") },       
{ 103, _T("LVN_DELETEITEM") },       
{ 104, _T("LVN_DELETEALLITEMS") },   
{ 105, _T("LVN_BEGINLABELEDITA") },  
{ 106, _T("LVN_ENDLABELEDITA") },    
{ 108, _T("LVN_COLUMNCLICK") },      
{ 109, _T("LVN_BEGINDRAG") },        
{ 111, _T("LVN_BEGINRDRAG") },       
{ 113, _T("LVN_ODCACHEHINT") },      
{ 114, _T("LVN_ITEMACTIVATE") },     
{ 115, _T("LVN_ODSTATECHANGED") },   

{ 121, _T("LVN_HOTTRACK") },

{ 150, _T("LVN_GETDISPINFOA") },      
{ 151, _T("LVN_SETDISPINFOA") },
{ 152, _T("LVN_ODFINDITEMA") },
{ 155, _T("LVN_KEYDOWN") },
{ 156, _T("LVN_MARQUEEBEGIN") },
{ 157, _T("LVN_GETINFOTIPA") },
{ 158, _T("LVN_GETINFOTIPW") },

{ 175, _T("LVN_BEGINLABELEDITW") },   
{ 176, _T("LVN_ENDLABELEDITW") },     
{ 177, _T("LVN_GETDISPINFOW") },
{ 178, _T("LVN_SETDISPINFOW") },      
{ 179, _T("LVN_ODFINDITEMW") },       

//  prsht.h
{ 200, _T("PSN_SETACTIVE") },              
{ 201, _T("PSN_KILLACTIVE") },             
{ 202, _T("PSN_APPLY") },                  
{ 203, _T("PSN_RESET") },                  
{ 205, _T("PSN_HELP") },                   
{ 206, _T("PSN_WIZBACK") },                
{ 207, _T("PSN_WIZNEXT") },                
{ 208, _T("PSN_WIZFINISH") },              
{ 209, _T("PSN_QUERYCANCEL") },            
{ 210, _T("PSN_GETOBJECT") },              
{ 212, _T("PSN_TRANSLATEACCELERATOR") },   
{ 213, _T("PSN_QUERYINITIALFOCUS") },      

// #define HDN_FIRST ((UINT)-300)
// #define HDN_LAST  ((UINT)-399)
{ 300, _T("HDN_ITEMCHANGINGA") },     
{ 301, _T("HDN_ITEMCHANGEDA") },      
{ 302, _T("HDN_ITEMCLICKA") },        
{ 303, _T("HDN_ITEMDBLCLICKA") },     
{ 305, _T("HDN_DIVIDERDBLCLICKA") },  
{ 306, _T("HDN_BEGINTRACKA") },       
{ 307, _T("HDN_ENDTRACKA") },         
{ 308, _T("HDN_TRACKA") },            
{ 309, _T("HDN_GETDISPINFOA") },      
{ 310, _T("HDN_BEGINDRAG") },         
{ 311, _T("HDN_ENDDRAG") },           
{ 320, _T("HDN_ITEMCHANGINGW") },     
{ 321, _T("HDN_ITEMCHANGEDW") },      
{ 322, _T("HDN_ITEMCLICKW") },        
{ 323, _T("HDN_ITEMDBLCLICKW") },     
{ 325, _T("HDN_DIVIDERDBLCLICKW") },  
{ 326, _T("HDN_BEGINTRACKW") },       
{ 327, _T("HDN_ENDTRACKW") },         
{ 328, _T("HDN_TRACKW") },            
{ 329, _T("HDN_GETDISPINFOW") },      

{ 401, _T("TVN_SELCHANGINGA") },
{ 402, _T("TVN_SELCHANGEDA") },
{ 403, _T("TVN_GETDISPINFOA") },
{ 404, _T("TVN_SETDISPINFOA") },
{ 405, _T("TVN_ITEMEXPANDINGA") },
{ 406, _T("TVN_ITEMEXPANDEDA") },
{ 407, _T("TVN_BEGINDRAGA") },
{ 408, _T("TVN_BEGINRDRAGA") },
{ 409, _T("TVN_DELETEITEMA") },
{ 410, _T("TVN_BEGINLABELEDITA") },
{ 411, _T("TVN_ENDLABELEDITA") },
{ 412, _T("TVN_KEYDOWN") },
{ 413, _T("TVN_GETINFOTIPA") },
{ 414, _T("TVN_GETINFOTIPW") },
{ 415, _T("TVN_SINGLEEXPAND") },
{ 450, _T("TVN_SELCHANGINGW") },
{ 451, _T("TVN_SELCHANGEDW") },
{ 452, _T("TVN_GETDISPINFOW") },
{ 453, _T("TVN_SETDISPINFOW") },
{ 454, _T("TVN_ITEMEXPANDINGW") },
{ 455, _T("TVN_ITEMEXPANDEDW") },
{ 456, _T("TVN_BEGINDRAGW") },
{ 457, _T("TVN_BEGINRDRAGW") },
{ 458, _T("TVN_DELETEITEMW") },
{ 459, _T("TVN_BEGINLABELEDITW") },
{ 460, _T("TVN_ENDLABELEDITW") },

{ 520, _T("TTN_GETDISPINFOA") },      
{ 521, _T("TTN_SHOW") },      
{ 522, _T("TTN_POP") },      
{ 530, _T("TTN_GETDISPINFOW") },      

{ 550, _T("TCN_KEYDOWN") },      
{ 551, _T("TCN_SELCHANGE") },      
{ 552, _T("TCN_SELCHANGING") },      

{ 700, _T("TBN_GETBUTTONINFOA") },
{ 701, _T("TBN_BEGINDRAG") },
{ 702, _T("TBN_ENDDRAG") },
{ 703, _T("TBN_BEGINADJUST") },
{ 704, _T("TBN_ENDADJUST") },
{ 705, _T("TBN_RESET") },
{ 706, _T("TBN_QUERYINSERT") },
{ 707, _T("TBN_QUERYDELETE") },
{ 708, _T("TBN_TOOLBARCHANGE") },
{ 709, _T("TBN_CUSTHELP") },
{ 710, _T("TBN_DROPDOWN") },
{ 713, _T("TBN_HOTITEMCHANGE") },
{ 714, _T("TBN_DRAGOUT") },
{ 715, _T("TBN_DELETINGBUTTON") },
{ 716, _T("TBN_GETDISPINFOA") },
{ 717, _T("TBN_GETDISPINFOW") },
{ 718, _T("TBN_GETINFOTIPA") },
{ 719, _T("TBN_GETINFOTIPW") },
{ 720, _T("TBN_GETBUTTONINFOW") },
{ 721, _T("TBN_RESTORE") },
{ 722, _T("TBN_SAVE") },
{ 722, _T("UDN_DELTAPOS") },
{ 723, _T("TBN_INITCUSTOMIZE") },
{ 800, _T("CBEN_GETDISPINFOA") },
{ 801, _T("CBEN_INSERTITEM") },
{ 802, _T("CBEN_DELETEITEM") },
{ 804, _T("CBEN_BEGINEDIT") },
{ 805, _T("CBEN_ENDEDITA") },
{ 806, _T("CBEN_ENDEDITW") },
{ 807, _T("CBEN_GETDISPINFOW") },
{ 808, _T("CBEN_DRAGBEGINA") },
{ 809, _T("CBEN_DRAGBEGINW") },
{ 831, _T("RBN_HEIGHTCHANGE") },
{ 832, _T("RBN_GETOBJECT") },
{ 833, _T("RBN_LAYOUTCHANGED") },
{ 834, _T("RBN_AUTOSIZE") },
{ 835, _T("RBN_BEGINDRAG") },
{ 836, _T("RBN_ENDDRAG") },
{ 837, _T("RBN_DELETINGBAND") },
{ 838, _T("RBN_DELETEDBAND") },
{ 839, _T("RBN_CHILDSIZE") },
{ 860, _T("IPN_FIELDCHANGED") },
{ 880, _T("SBN_SIMPLEMODECHANGE") },
{ 901, _T("PGN_SCROLL") },
{ 902, _T("PGN_CALCSIZE") },

{ WM_NOT_FOUND, _T("WM_NOT_FOUND") } };

//*********************************************************************
//lint -esym(714, lookup_winmsg_name)
//lint -esym(759, lookup_winmsg_name)
//lint -esym(765, lookup_winmsg_name)
TCHAR *lookup_winmsg_name(int msg_code)
{
   static TCHAR msgstr[81] ;
   unsigned j ;
   bool is_negative = false ;
   if (msg_code >= 0) {
      if (msg_code >= WM_APP) {
         _stprintf(msgstr, _T("WM_APP + %u"), msg_code - WM_APP) ;
         return msgstr;
      }
      for (j=0; win_msgs[j].msg_num != WM_NOT_FOUND; j++) {
         if (win_msgs[j].msg_num == (unsigned) msg_code)
            return win_msgs[j].msg_name ;
      }
   } else {
      msg_code = -msg_code ;
      is_negative = true ;
      for (j=0; win_neg_msgs[j].msg_num != WM_NOT_FOUND; j++) {
         if (win_neg_msgs[j].msg_num == (unsigned) msg_code)
            return win_neg_msgs[j].msg_name ;
      }
   }
   if (is_negative)
      _stprintf(msgstr, _T("Msg not found [-%d]"), msg_code) ;
   else
      _stprintf(msgstr, _T("Msg not found [%d]"), msg_code) ;
   return msgstr;
}

