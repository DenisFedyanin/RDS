#ifndef RdsCtrlH
#define RdsCtrlH

#include <pshpack1.h> // ��������� ������� �������� ��������

#ifndef RDSCALL
  #define RDSCALL CALLBACK
#endif

//---------------------------------------------------------------------------
// ������� ��������� ������ ��� �������� ������
typedef void (RDSCALL *RDSCTRL_RETURNSTRING)(LPVOID,LPSTR);
//---------------------------------------------------------------------------

// ���� �������
#define RDSCTRLEVENT_CALCSTART  1       // ������ �������
#define RDSCTRLEVENT_CALCSTOP   2       // ��������� �������
#define RDSCTRLEVENT_CONNCLOSED 3       // ������ �����
#define RDSCTRLEVENT_BLOCKMSG   4       // ��������� �� �����
#define RDSCTRLEVENT_NEWFILE    5       // �������� ������ ����� ��� �������� �����
#define RDSCTRLEVENT_EDITMODE   6       // ����� ��������������
#define RDSCTRLEVENT_CALCMODE   7       // ����� �������������
#define RDSCTRLEVENT_SAVEFILE   8       // ���������� �����
#define RDSCTRLEVENT_LOADREQ    9       // ������ �� �������� �����
#define RDSCTRLEVENT_PROGRESS   10      // �������� �������� ��� ����������
//---------------------------------------------------------------------------

// ��������� �������� �������, ������������ � �������� �� ������� �����
// ��������� ���� (���������� ��� LParam)
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (��� ������������� ������)
  int Link;             // ����� ����������, ���������� �������
  int Event;            // ������� (RDSCTRLEVENT_*)
  LPVOID Data;          // �������������� ��������� (���� ����)
} RDSCTRL_MSGEVENTDATA;
typedef RDSCTRL_MSGEVENTDATA *RDSCTRL_PMSGEVENTDATA;
//---------------------------------------------------------------------------

// ��������� �������� ��������� �� �����. �� ����� ���������� ����� �������� Data
// ��������� RDSCTRL_MSGEVENTDATA ��� ������� ����� ������� ��������� ��� �����
// ������ �������� ����������� CallBack-�������.
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (��� ������������� ������)
  LPSTR BlockFullName;  // ������ ��� �����, ����������� ���������
  int IntMsg;           // ���������� ����� �����
  LPSTR StrMsg;         // ���������� ������
} RDSCTRL_BLOCKMSGDATA;
typedef RDSCTRL_BLOCKMSGDATA *RDSCTRL_PBLOCKMSGDATA;
//---------------------------------------------------------------------------

// ��������� �������� ������� RDSCTRLEVENT_NEWFILE. �� ����� ���������� ����� �������� Data
// ��������� RDSCTRL_MSGEVENTDATA ��� ������� ����� ������� ��������� ��� �����
// ������ �������� ����������� CallBack-�������.
#define RDSCTRLEVENT_NEWFILE_NEW        0       // ������� ����� �������
#define RDSCTRLEVENT_NEWFILE_TEMPLATE   1       // ������� �� �������
#define RDSCTRLEVENT_NEWFILE_LOAD       2       // ��������� �� �����
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (��� ������������� ������)
  int Reason;           // ������� ������ (RDSCTRLEVENT_NEWFILE_*)
  LPSTR FileName;       // ��� �����
} RDSCTRL_NEWFILEDATA;
typedef RDSCTRL_NEWFILEDATA *RDSCTRL_PNEWFILEDATA;
//---------------------------------------------------------------------------

// ��������� �������� ������� RDSCTRLEVENT_SAVEFILE. �� ����� ���������� ����� �������� Data
// ��������� RDSCTRL_MSGEVENTDATA ��� ������� ����� ������� ��������� ��� �����
// ������ �������� ����������� CallBack-�������.
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (��� ������������� ������)
  LPSTR FileName;       // ��� �����
} RDSCTRL_SAVEFILEDATA;
typedef RDSCTRL_SAVEFILEDATA *RDSCTRL_PSAVEFILEDATA;
//---------------------------------------------------------------------------

// ��������� �������� ������� RDSCTRLEVENT_PROGRESS. �� ����� ���������� ����� �������� Data
// ��������� RDSCTRL_MSGEVENTDATA ��� ������� ����� ������� ��������� ��� �����
// ������ �������� ����������� CallBack-�������.
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (��� ������������� ������)
  DWORD Current;        // ������� ��������
  DWORD Maximum;        // ������������ ��������
} RDSCTRL_PROGRESSDATA;
typedef RDSCTRL_PROGRESSDATA *RDSCTRL_PPROGRESSDATA;
//---------------------------------------------------------------------------

// ��������� �������� ����������� �� RDS ������ ����
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (��� ������������� ������)
  LPSTR Text;           // ����� ������ ����
  BOOL Enabled;         // ��������
  BOOL Visible;         // �����
  LPSTR BlockFullName;  // ������ ��� �����, � �������� ��������� �����
  int MenuFunc,MenuData;// ��������� ������ ����
  BOOL Checked;         // �������
  BOOL Divider;         // ��� �� ����� ����, � �����������
  BOOL HasKey;          // � ������ ���� � RDS ���� "������� �������"
  int Key;              // ��� �������
  DWORD KeyFlags;       // ����� ������� (RDS_M* � RDS_K* �� RdsDef.h)
} RDSCTRL_MENUITEM;
typedef RDSCTRL_MENUITEM *RDSCTRL_PMENUITEM;
//---------------------------------------------------------------------------

// ��������� ��� ������� rdsctrlSetViewportZoomRectEx
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (��� ������������� ������)
  int Left,Top;         // ����� ������� ���� ��������������
  int Width,Height;     // ������� ��������������
  DWORD Reserved;       // ��������������� ��� ������������ **** REV2 ****
  double MaxZoom;       // ����������� ���������� �������
  DWORD Flags;          // �����
  DWORD Reserved2;      // ��������������� ��� ������������ **** REV2 ****
  // ������������ ���������
  double Zoom;          // ������������ �������
  int ScrollX,ScrollY;  // ������������ �����
} RDSCTRL_ZOOMRECT;
typedef RDSCTRL_ZOOMRECT *RDSCTRL_PZOOMRECT;
//---------------------------------------------------------------------------

// ����� ��� rdsctrlSetViewportZoomRect � rdsctrlSetViewportZoomRectEx
#define RDSCTRL_ZOOMRECTFLAGS_100       1       // ����� ������� ���� �������������� � ��� �������
                                                // ���� � �������� 100%


// �������� ��� rdsctrlViewportMouse
#define RDSCTRL_MOUSEOP_DOWN    0       // �������
#define RDSCTRL_MOUSEOP_UP      1       // ����������
#define RDSCTRL_MOUSEOP_MOVE    2       // �����������
#define RDSCTRL_MOUSEOP_DBL     3       // ������� ������
// ����� ��� rdsctrlViewportMouse
#define RDSCTRL_MOUSEF_LEFT     1       // ����� ������
#define RDSCTRL_MOUSEF_RIGHT    2       // ������ ������
#define RDSCTRL_MOUSEF_MIDDLE   4       // ������� ������
#define RDSCTRL_MOUSEF_CTRL     8       // Ctrl
#define RDSCTRL_MOUSEF_ALT      16      // Alt
#define RDSCTRL_MOUSEF_SHIFT    32      // Shift
// �������� ��� rdsctrlViewportKeyboard
#define RDSCTRL_KEYOP_DOWN      0       // �������
#define RDSCTRL_KEYOP_UP        1       // ����������
// ����� ��� rdsctrlViewportKeyboard
#define RDSCTRL_KEYF_LEFT       RDSCTRL_MOUSEF_LEFT     // ����� ������
#define RDSCTRL_KEYF_RIGHT      RDSCTRL_MOUSEF_RIGHT    // ������ ������
#define RDSCTRL_KEYF_MIDDLE     RDSCTRL_MOUSEF_MIDDLE   // ������� ������
#define RDSCTRL_KEYF_CTRL       RDSCTRL_MOUSEF_CTRL     // Ctrl
#define RDSCTRL_KEYF_ALT        RDSCTRL_MOUSEF_ALT      // Alt
#define RDSCTRL_KEYF_SHIFT      RDSCTRL_MOUSEF_SHIFT    // Shift
//---------------------------------------------------------------------------

// ���� ��� rdsctrlReadBlockMenuItems
#define RDSCTRL_MENUTYPE_BLK    0       // ���� �����
#define RDSCTRL_MENUTYPE_SYS    1      // ���� ���������� ����� ���� ����������
#define RDSCTRL_MENUTYPE_MAIN   2      // ���� "�������|�������������"
//---------------------------------------------------------------------------

// ����� ��� rdsctrlLoadSystemFromMem
#define RDSCTRL_LSFM_SAVEPROMPT 1       // �������������� ������������� �������
#define RDSCTRL_LSFM_TRANSMODE  (2+4)   // ����� ������
  #define RDSCTRL_LSFM_TRANSMAP    0    // ���������� ������ ����� ����������� ������
  #define RDSCTRL_LSFM_TRANSFILE   2    // ���������� ������ ����� ��������� ����
//  #define RDSCTRL_LSFM_TRANSPIPE   4    // ���������� ������ ����� ����� (pipe) - �� �����������
//---------------------------------------------------------------------------

// ������ �������� ��� rdsctrlGetSystemContent
#define RDSCTRL_GSC_TRANSFILE   0       // ����� ��������� ����
#define RDSCTRL_GSC_TRANSPIPE   1       // ����� �����
//---------------------------------------------------------------------------

// ����� ��� rdsctrlFindOpSetProviders
#define RDSCTRL_FOSP_RECURSIVE  1       // ������ �� ��������� �����������
#define RDSCTRL_FOSP_SELF       2       // ��������� ��������� ���������� �� ��������� �������
//---------------------------------------------------------------------------

// ����� ��� rdsctrlSaveSystemTaggedEx/rdsctrlLoadSystemTaggedEx
#define RDSCTRL_TAGGED_SHAREDMEM        1       // ���������� ��� - �� ����, � ������� ������
#define RDSCTRL_TAGGED_DBLBUF           2       // ��� ���������� - ������������ �������������� �����
//---------------------------------------------------------------------------

// ��������� ��� ���������/��������� ����� ���������� RDS
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (��� ������������� ������)
  DWORD UIFlags;        // ��������� ����������������� ���������� (RDSCTRL_SET_*)
    #define RDSCTRL_SET_ENABLEUI        1       // ������������� ���������� ������
    #define RDSCTRL_SET_ENABLEEDIT      2       // �������� ����� ��������������
    #define RDSCTRL_SET_ENABLECALC      4       // �������� ����� �������������
    #define RDSCTRL_SET_ENABLERUN       8       // �������� ������ �������
    #define RDSCTRL_SET_ENABLEPARAMED   16      // ��������� �������������� ������ ���������� ������
    #define RDSCTRL_SET_ENABLEOPTIONS   32      // ��������� ��������� �������� RDS
    #define RDSCTRL_SET_ENABLESYSWIN    64      // ��������� �������� ���� ���������
    #define RDSCTRL_SET_ENABLEMAINWIN   128     // ���������� ������� ���� RDS
  BOOL SystemAutoSave;  // �������������� ��� ������

} RDSCTRL_SETTINGS;
typedef RDSCTRL_SETTINGS *RDSCTRL_PSETTINGS;
//---------------------------------------------------------------------------

// ������ ������ RDS
#define RDSCTRL_MODE_UNKNOWN    -1      // ���������� (������)
#define RDSCTRL_MODE_EDIT       0       // ����� ��������������
#define RDSCTRL_MODE_CALC       1       // ����� �������������
#define RDSCTRL_MODE_RUNNING    2       // ����� �������
//---------------------------------------------------------------------------

// ����� ��� rdsctrlExecTool (������ ��������� � ������������ � RdsDef.h)
// �����
#define RDSCTRL_ETF_M_EXECUTE       0       // ������� ������ ���������� � ������ RDS_TOOLM_EXECUTE (��� �� ����)
#define RDSCTRL_ETF_M_SETUP         1       // ������� ������ ���������� � ������ RDS_TOOLM_SETUP (���������)
#define RDSCTRL_ETF_M_CALL          2       // ������� ������ ���������� � ������ RDS_TOOLM_CALL (��� RDS_TOOLM_REMOTECALL) � ���������� RDS_PTOOLCALLDATA
// ������ ������ ������
#define RDSCTRL_ETF_S_BYDLL         0       // �� ����� ������� � ����� DLL
#define RDSCTRL_ETF_S_BYSERVNAME    0x100   // �� ����������� �����
// �����
#define RDSCTRL_ETF_MODEMASK        0xff    // �����
#define RDSCTRL_ETF_SEARCHMASK      0x300   // ������ ������
//---------------------------------------------------------------------------

// ���� CallBack-�������
typedef void (RDSCALL *RDSCTRL_CALLBACK)(int,int,LPVOID,LPVOID);
typedef void (RDSCALL *RDSCTRL_BMCALLBACK)(int,LPSTR,int,LPSTR,LPVOID);
//---------------------------------------------------------------------------

// ������� ������������ �� ���������
#include <poppack.h>

// ���� ���������������� �������
typedef int (RDSCALL *RDSCTRL_IV)(void);
typedef void (RDSCALL *RDSCTRL_VI)(int);
typedef BOOL (RDSCALL *RDSCTRL_BI)(int);
typedef void (RDSCALL *RDSCTRL_VS)(LPSTR);
typedef BOOL (RDSCALL *RDSCTRL_BISB)(int,LPSTR,BOOL);
typedef BOOL (RDSCALL *RDSCTRL_BIS)(int,LPSTR);
typedef int (RDSCALL *RDSCTRL_IISISSIpI)(int,LPSTR,int,LPSTR,LPSTR,int,int*);
typedef void (RDSCALL *RDSCTRL_VIS)(int,LPSTR);
typedef void (RDSCALL *RDSCTRL_VIB)(int,BOOL);
typedef BOOL (RDSCALL *RDSCTRL_BIB)(int,BOOL);
typedef void (RDSCALL *RDSCTRL_VII)(int,int);
typedef void (RDSCALL *RDSCTRL_VIIHwUWp)(int,int,HWND,UINT,WPARAM);
typedef void (RDSCALL *RDSCTRL_VIICbpV)(int,int,RDSCTRL_CALLBACK,LPVOID);
typedef void (RDSCALL *RDSCTRL_VICb1pV)(int,RDSCTRL_BMCALLBACK,LPVOID);
typedef void (RDSCALL *RDSCTRL_VIIS)(int,int,LPSTR);
typedef void (RDSCALL *RDSCTRL_VISBB)(int,LPSTR,BOOL,BOOL);
typedef int (RDSCALL *RDSCTRL_IIIHwSDw)(int,int,HWND,LPSTR,DWORD);
typedef void (RDSCALL *RDSCTRL_VIIDIIDw)(int,int,double,int,int,DWORD);
typedef void (RDSCALL *RDSCTRL_VIIDw)(int,int,DWORD);
typedef BOOL (RDSCALL *RDSCTRL_BIIDpIpI)(int,int,double,int*,int*);
typedef void (RDSCALL *RDSCTRL_VIIIIII)(int,int,int,int,int,int);
typedef BOOL (RDSCALL *RDSCTRL_BIIIIIDw)(int,int,int,int,int,DWORD);
typedef void (RDSCALL *RDSCTRL_VIIpDpIpI)(int,int,double*,int*,int*);
typedef int (RDSCALL *RDSCTRL_IISI)(int,LPSTR,int);
typedef RDSCTRL_PMENUITEM (RDSCALL *RDSCTRL_MiII)(int,int);
typedef void (RDSCALL *RDSCTRL_VRs)(RDSCTRL_RETURNSTRING);
typedef BOOL (RDSCALL *RDSCTRL_BIIIIpV)(int,int,int,int,LPVOID);
typedef void (RDSCALL *RDSCTRL_VISII)(int,LPSTR,int,int);
typedef int (RDSCALL *RDSCTRL_III)(int,int);
typedef BOOL (RDSCALL *RDSCTRL_BIIIIpVpIpIpIpIpIpI)(int,int,int,int,LPVOID,int*,int*,int*,int*,int*,int*);
typedef BOOL (RDSCALL *RDSCTRL_BIIpV)(int,int,LPVOID);
typedef BOOL (RDSCALL *RDSCTRL_BIpVDwDw)(int,LPVOID,DWORD,DWORD);
typedef int (RDSCALL *RDSCTRL_IISISpV)(int,LPSTR,int,LPSTR,LPVOID);
typedef BOOL (RDSCALL *RDSCTRL_BIOpt)(int,RDSCTRL_PSETTINGS);
typedef BOOL (RDSCALL *RDSCTRL_BISpVpIpIpIpI)(int,LPSTR,LPVOID,int*,int*,int*,int*);
typedef int (RDSCALL *RDSCTRL_IISBBDwpV)(int,LPSTR,BOOL,BOOL,DWORD,LPVOID);
typedef BOOL (RDSCALL *RDSCTRL_BIpVIRs)(int,LPVOID,int,RDSCTRL_RETURNSTRING);
typedef void (RDSCALL *RDSCTRL_VISSpV)(int,LPSTR,LPSTR,LPVOID);
typedef void (RDSCALL *RDSCTRL_VISSS)(int,LPSTR,LPSTR,LPSTR);
typedef void (RDSCALL *RDSCTRL_VSB)(LPSTR,BOOL);
typedef void (RDSCALL *RDSCTRL_VBB)(BOOL,BOOL);
typedef void (RDSCALL *RDSCTRL_VV)(void);
typedef void (RDSCALL *RDSCTRL_VIIIIIIDDw)(int,int,int,int,int,int,double,DWORD);
typedef BOOL (RDSCALL *RDSCTRL_BIIZr)(int,int,RDSCTRL_PZOOMRECT);
typedef void (RDSCALL *RDSCTRL_VIIDwpDpIpI)(int,int,DWORD,double*,int*,int*);
typedef void (RDSCALL *RDSCTRL_VISB)(int,LPSTR,BOOL);
typedef BOOL (RDSCALL *RDSCTRL_BISDw)(int,LPSTR,DWORD);
typedef void (RDSCALL *RDSCTRL_VIDw)(int,DWORD);
typedef int (RDSCALL *RDSCTRL_IISSDwpV)(int,LPSTR,LPSTR,DWORD,LPVOID);
typedef BOOL (RDSCALL *RDSCTRL_BISDwpDw)(int,LPSTR,DWORD,DWORD*);
typedef BOOL (RDSCALL *RDSCTRL_BISDwDw)(int,LPSTR,DWORD,DWORD);
typedef DWORD (RDSCALL *RDSCTRL_DwIS)(int,LPSTR);
typedef BOOL (RDSCALL *RDSCTRL_BIDwpV)(int,DWORD,LPVOID);
typedef int (RDSCALL *RDSCTRL_II)(int);
typedef BOOL (RDSCALL *RDSCTRL_BIDwpVpDw)(int,DWORD,LPVOID,DWORD*);
typedef int (RDSCALL *RDSCTRL_IIDw)(int,DWORD);
typedef int (RDSCALL *RDSCTRL_IIpDwpDwpV)(int,DWORD*,DWORD*,LPVOID);
typedef HWND (RDSCALL *RDSCTRL_HwIHwSIIIIDw)(int,HWND,LPSTR,int,int,int,int,DWORD);
typedef void (RDSCALL *RDSCTRL_VIHw)(int,HWND);
typedef DWORD (RDSCALL *RDSCTRL_DwIIIIpVpIpIpIpIpIpI)(int,int,int,int,LPVOID,int*,int*,int*,int*,int*,int*);
typedef void (RDSCALL *RDSCTRL_VISIIBt)(int,LPSTR,int,int,BYTE);
typedef BOOL (RDSCALL *RDSCTRL_BIDwSSIS)(int,DWORD,LPSTR,LPSTR,int,LPSTR);
//---------------------------------------------------------------------------

//===================================================================================
// ����� ���������� ���������� �� ������������ ������� � ������� ��� ��������� ����
// ���������� (��� ������������� ����������� ����������).
// ������������� ������ ���� ����� ���������� #include "RdsCtrl.h" ���� �������� ����
//     #define RDSCTRL_SERV_FUNC_BODY FuncName
// ��� FuncName - ��� ������������ ������� ��������� ����������.
// ���� ����� ����� ���� ������� ������ � ���� ���� �������, �.�. ������ � ����� �����
// ����� #include "RdsCtrl.h" ����� ���������� #define RDSCTRL_SERV_FUNC_BODY.
// �� ���� ��������� ������ ������� ��� ������� � ��������� �������� ����� #include "RdsCtrl.h"
// ������� �������� ��������
//     #define RDSCTRL_SERV_FUNC_EXTERNAL
// ������ ������ RDSCTRL_DYNFUNCACCESS ������ ������������ ��-�� ����������� �� ����������� �����
// ���������������� � 4096 ����.
// ������ ������������� (� Borland C++ 5.5):
//    ------- ������� ���� ������� -------
//    #define RDSCTRL_SERV_FUNC_BODY MyGetService
//    #include "RdsCtrl.h"  // ����� ����� ��������� ���� ������� BOOL MyGetService(HMODULE dll)
//    ......
//
//    DllModule=LoadLibrary("RdsCtrl.dll");
//    if(!MyGetService(DllModule))
//      MessageBox(NULL,"������ ������� � ��������� ��������","RdsCtrl.dll",MB_OK | MB_ICONERROR);
//    ......
//
//    ------- ������ ����� ������� -------
//    #define RDSCTRL_SERV_FUNC_EXTERNAL
//    #include "RdsFunc.h"  // ����� ����� ��������� �������� ������� ����������
//    ......
// ����� RdsCtrl.h ����� ���������� �������� ����, ���������� �� �������� ����� �
// ��������� ������� Windows API (� ���������, ���� BOOL � ������� GetModuleHandle).
// ��� Borland C++ �������� "windows.h".
// ��� ����������� �������� �������� ����� #include �� �����.
//---------------------------------------------------------------------------
//===================================================================================
#ifdef RDSCTRL_SERV_FUNC_BODY
  // ��������������� ������, ����������� ����������-���������
  #define _RDSCTRL_AUX_FUNC(name,type) type name;
  // �������� ����������-���������� �� �������
  #include "RdsCtrl_.h"
  #undef _RDSCTRL_AUX_FUNC
// ������� ��������� ����������
BOOL RDSCTRL_SERV_FUNC_BODY(HMODULE dll)
  { BOOL ok=TRUE;
    // ��������������� ������, ����������� ������� � ����������
    #define _RDSCTRL_AUX_FUNC(name,type) ok=((name=(type)GetProcAddress(dll,#name))!=NULL)&&ok;
    #include "RdsCtrl_.h"
    #undef _RDSCTRL_AUX_FUNC
    return ok;
  }
// ����� #ifdef RDSCTRL_SERV_FUNC_BODY
#endif
//===================================================================================

//===================================================================================
// �������� ���������� �� ������� ��� ������, �� ������������ #define RDS_SERV_FUNC_BODY
// (� "extern")
//===================================================================================
#if defined(RDSCTRL_SERV_FUNC_EXTERNAL) && !defined(RDSCTRL_SERV_FUNC_BODY)
  // ��������������� ������, ����������� ������� ����������-���������
  #define _RDSCTRL_AUX_FUNC(name,type) extern type name;
  #include "RdsCtrl_.h"
  #undef _RDSCTRL_AUX_FUNC
// ����� #if defined(RDSCTRL_SERV_FUNC_EXTERNAL) && !defined(RDSCTRL_SERV_FUNC_BODY)
#endif
//===================================================================================

// ����� #ifndef RdsCtrlH
#endif


