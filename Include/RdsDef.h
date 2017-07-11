//---------------------------------------------------------------------------
// ��������, ����������� ��� ������� ������ RDS
//---------------------------------------------------------------------------
#ifndef RdsDefH
#define RdsDefH
//================ ��������, ����������� ��� ������������� ==================
#ifndef RDS_NO_PACK
  #include <pshpack1.h> // ��������� ������� �������� ��������
#endif
#ifndef RDSCALL
  #define RDSCALL CALLBACK
#endif

//===================================================================================
// ������ RDS, �� ������� ���������� ��� ��������. ����� �������������� ��� ���������
// � ������� ������, ���������� �� RDS, � ������ ��������� �� ������.
// ������:
// if(rdsServiceVersion!=NULL && (rdsServiceVersion()<RDS_FVERSION))
//   { // ��������� �� ������: ������ ������ RDS
//     // ....
//   }
//===================================================================================
// �������������� ������ � int
#define RDS_INTVERSION(high,low,build) (((high)<<24) | ((low)<<16) | (build))
// �������������� ���� ������ � DWORD **** REV2 ****
#define RDS_DWORDVERDATE(y,mon,d,h,mnu,q) ( ((((y-2000))&0x3ff)<<22)|(((mon)&0xf)<<18)|(((d)&0x1f)<<13)|(((h)&0x1f)<<8)|(((mnu)&0x3f)<<2)|((q)&3) )
#define RDS_DWORDVERDATESHORT(y,mon,d) ( ((((y-2000))&0x3ff)<<22)|(((mon)&0xf)<<18)|(((d)&0x1f)<<13) )

#ifndef RDS_NOVERSIONDEFINES
  // �������� ������ RDS
  #define RDS_FVERSIONHIGH      1
  #define RDS_FVERSIONLOW       0
  #define RDS_FVERSIONBUILD     491
  #define RDS_FVERSION          (RDS_INTVERSION(RDS_FVERSIONHIGH,RDS_FVERSIONLOW,RDS_FVERSIONBUILD))
  // �������� ���� ������� ���� ������ RDS **** REV2 ****
  #define RDS_FVERSIONYEAR      2017    // ���
  #define RDS_FVERSIONMONTH     5       // �����
  #define RDS_FVERSIONDAY       28      // ����
  #define RDS_FVERSIONHOUR      0       // ���
  #define RDS_FVERSIONMIN       0       // ������
  #define RDS_FVERSIONQUARTER   0       // �������� ������
  #define RDS_FVERSIONDATE      (RDS_DWORDVERDATE(RDS_FVERSIONYEAR,RDS_FVERSIONMONTH,RDS_FVERSIONDAY,RDS_FVERSIONHOUR,RDS_FVERSIONMIN,RDS_FVERSIONQUARTER))
#endif
//===================================================================================

//======================== �������� �������� ================================
// ��������, ����������� ��� ������� ������� ������

//---------------------------------------------------------------------------
// ��������� (������ ������)
#define RDS_BFM_INIT                     0 // ������������� �����
#define RDS_BFM_CLEANUP                  1 // ������� (���������� ����� ���������)
#define RDS_BFM_MODEL                    2 // �������������
#define RDS_BFM_RENAME                   3 // ��������� � �������������� �����
#define RDS_BFM_VARCHECK                 4 // �������� ��������� ����������
#define RDS_BFM_SETUP                    5 // ����� ������� ���������
#define RDS_BFM_REMOTEMSG                6 // ��������� �� �������� ����������
#define RDS_BFM_RESIZE                   7 // ������ ����� �������
#define RDS_BFM_RESIZING                 8 // ������ ����� ���������� �������������
#define RDS_BFM_TIMER                    9 // �������� ������
#define RDS_BFM_STARTCALC               10 // ������ �������
#define RDS_BFM_STOPCALC                11 // ������ ����������
#define RDS_BFM_RESETCALC               12 // ������ �������
#define RDS_BFM_WINREFRESH              13 // ���������� �������� ����������� ����
#define RDS_BFM_CHECKFUNCSUPPORT        14 // ��������� ��������� ���������� ������� ������ (����������)
#define RDS_BFM_FUNCTIONCALL            15 // ����� ���������� �������
#define RDS_BFM_EDITMODE                16 // ���� � ����� ��������������
#define RDS_BFM_CALCMODE                17 // ���� � ����� �������
#define RDS_BFM_POPUPHINT               18 // ������ ������ ����������� ���������
#define RDS_BFM_MENUFUNCTION            19 // ����� ���������� ������� ����� (������� �� ����� ����)
#define RDS_BFM_UNLOADSYSTEM            20 // ��� ������� ����� ���������
#define RDS_BFM_MOVED                   21 // ���� ���������
#define RDS_BFM_MANUALDELETE            22 // ����� ��������� ����� (���������� ������ ��� �������� ������
                                           // �� ����� �������������)
#define RDS_BFM_BEFORESAVE              23 // ����� ����������� ���� �����
#define RDS_BFM_AFTERSAVE               24 // ����� ���������� ���� �����
#define RDS_BFM_CONTEXTPOPUP            25 // ����� ������������ ���� �����
#define RDS_BFM_MANUALINSERT            26 // ����� ���������� ����� � ����� �� ������
                                           // ������ ��� �� �����
#define RDS_BFM_DYNVARCHANGE            27 // ��������� ������������ ����������
#define RDS_BFM_PREMODEL                28 // ����� ������ ������������� (��� ������������ ���� RDS_CTRLCALC)
#define RDS_BFM_BLOCKPANEL              29 // �����-���� ����������� �� ������ ����� � ����������
#define RDS_BFM_AFTERLOAD               30 // ����� �������� ���� �����
#define RDS_BFM_CONTENTCHANGE           31 // ������� �� ��������� � ���������� (��� ��������) **** REV2 ****
#define RDS_BFM_LEAVINGEDITMODE         32 // ����� ������� �� ������ �������������� **** REV2 ****
#define RDS_BFM_HELP                    33 // ������ ������� � ����� **** REV2 ****
#define RDS_BFM_SYSWORKSIZECHANGE       34 // ��������� ������� �������� ���� ���������� **** REV2 ****
#define RDS_BFM_LAYERCONFIGCHANGE       35 // ��������� ������������ ����� ���������� **** REV2 ****
#define RDS_BFM_SHAREDMEMCHANGE         36 // ��������� ����������� ���������� **** REV2 ****
#define RDS_BFM_TRANSLATE               37 // ����� ��� �������� ������� **** REV2 ****

// �������� � ������ ������ ������ �����
#define RDS_BFM_LOADBIN                 50 // �������� � �������� ����
#define RDS_BFM_LOADTXT                 51 // �������� � ��������� ����
#define RDS_BFM_SAVEBIN                 52 // ������ � �������� ����
#define RDS_BFM_SAVETXT                 53 // ������ � ��������� ����

// �������� � ������ �������� ��������� �����
#define RDS_BFM_LOADSTATE               60 // �������� ��������� � �������� ����
#define RDS_BFM_SAVESTATE               61 // ������ ��������� � �������� ����

// ������� �� "����" �� ����������� �����
#define RDS_BFM_MOUSEDOWN              100 // ������� �� ������� ������ "����" � ������ �������
#define RDS_BFM_MOUSEUP                101 // ������� �� ���������� ������ "����" � ������ �������
#define RDS_BFM_MOUSEMOVE              102 // ������� �� ����������� "����" � ������ �������
#define RDS_BFM_MOUSEDBLCLICK          103 // ������� �� ������� ������ � ������ �������
#define RDS_BFM_MOUSESELECT            104 // ����������� ������ ����� ������� "����" � ������ ��������������

// ������� �� ����������
#define RDS_BFM_KEYDOWN                120 // ������� �� ������� ������� � ������ �������
#define RDS_BFM_KEYUP                  121 // ������� �� ���������� ������� � ������ �������

#define RDS_BFM_DRAW                   150 // ��������� ����������� �����
#define RDS_BFM_DRAWADDITIONAL         151 // ��������� �������������� ���������

// ���������, �������������� �����������, ���� ����� �� ������������ �� "����"
#define RDS_BFM_WINDOWMOUSEDOWN        200 // ������� �� ������� ������ "����" � ������ �������
#define RDS_BFM_WINDOWMOUSEUP          201 // ������� �� ���������� ������ "����" � ������ �������
#define RDS_BFM_WINDOWMOUSEMOVE        202 // ������� �� ����������� "����" � ������ �������
#define RDS_BFM_WINDOWMOUSEDBLCLICK    203 // ������� �� ������� ������ � ������ �������

// ���������, �������������� �����������, ���� ����� �� ������������ �� ����������
#define RDS_BFM_WINDOWKEYDOWN          220 // ������� �� ������� ������� � ������ �������
#define RDS_BFM_WINDOWKEYUP            221 // ������� �� ���������� ������� � ������ �������

// ������� �� �������� � ����� ���������� (���������� � ���������� � ���� ������ � �� ����)
#define RDS_BFM_WINDOWOPERATION        230 // �������� � �������� ���� ����������

// ������� �� ������� �������
#define RDS_BFM_NETCONNECT             300 // ���������� �����������
#define RDS_BFM_NETDISCONNECT          301 // ���������� ���������
#define RDS_BFM_NETDATARECEIVED        302 // �������� ������
#define RDS_BFM_NETERROR               303 // ������
#define RDS_BFM_NETDATAACCEPTED        304 // ������ ����� �������� ��������

// ������� �� ������ �������� ���������� �����
#define RDS_BFM_SUPPORTSCONNSETUP      400 // ���� ������������ ��������� ����� ����� **** REV2 ****
#define RDS_BFM_CONNSETUP              401 // ����� ��������� ����� **** REV2 ****
#define RDS_BFM_CONNHINT               402 // ������ ����������� ��������� ����� **** REV2 ****
//---------------------------------------------------------------------------

// ������������ �������� ��������
#define RDS_BFR_DONE            0 // ���������� ���������� (��� ���� �������)
#define RDS_BFR_ERROR           1 // ������ (��� ��������� �������)
#define RDS_BFR_NOTPROCESSED    1 // ����� �� ��������� (��� ��������� �������), ==RDS_BFR_ERROR
#define RDS_BFR_STOP            1 // ����������� ���������� ��������� (��� ��������� �������), ==RDS_BFR_ERROR
#define RDS_BFR_BADVARSMSG      2 // ������ ��� ������ RDS_BFM_VARCHECK, �� ��������� �� ������
                                  // ������ ������� �������� ���������.
#define RDS_BFR_MODIFIED        1 // ��������� �������� (��� RDS_BFM_SETUP)
#define RDS_BFR_SHOWMENU        2 // ��� ������� �� ������� ������ ������ ���� - ������� ����������,
                                  // ����� �������� ����������� ����
#define RDS_BFR_RETRY           1 // ������ �������� (��� RDS_BFM_LEAVINGEDITMODE)
#define RDS_BFR_ABORT           2 // ������ �������� (��� RDS_BFM_LEAVINGEDITMODE)
//---------------------------------------------------------------------------

// ����� ���� � ����������
#define RDS_MLEFTBUTTON         1
#define RDS_MRIGHTBUTTON        2
#define RDS_MMIDDLEBUTTON       4
#define RDS_KSHIFT              8
#define RDS_KALT               16
#define RDS_KCTRL              32
#define RDS_MOUSEFLAGS         (RDS_MLEFTBUTTON|RDS_MRIGHTBUTTON|RDS_MMIDDLEBUTTON)
#define RDS_KBDFLAGS           (RDS_KSHIFT|RDS_KALT|RDS_KCTRL)
//---------------------------------------------------------------------------

// ���������� ������������� �����
typedef LPVOID RDS_BHANDLE;
// ���������� ������������� �����
typedef LPVOID RDS_CHANDLE;
// ���������� ������������� ���������������� �������
typedef LPVOID RDS_TIMERID;
// ���������� ������������� ������ ���� "�������"
typedef LPVOID RDS_MENUITEM;
// ���������� ������������� ���������� �����
typedef LPVOID RDS_VHANDLE;
// ���������� ������������� ���������������� �������
typedef LPVOID RDS_HOBJECT;
// ������������� ������� ������� (����������)
typedef LPVOID RDS_NETSTATION;
// ������������� ����� �� ������� ����������
typedef LPVOID RDS_NETBLOCK;
//---------------------------------------------------------------------------
// short int ����� ������ (��� �������������)
typedef short int RDS_SHORT;
//---------------------------------------------------------------------------
// ����� ����� (RDS_BLOCKDATA::Flags)
#define RDS_NEEDSDLLREDRAW    1  // ��������� ����������� ����� ���������� �������
                                 // ������� (������������ ��� ������ ������ �����,
                                 // ������� �� ������ ���� ���, ������/������)
#define RDS_VARCHECKFAILED    2  // ���������� ����� RDS_BFM_VARCHECK ������
                                 // ���� ������ (������ ������)
#define RDS_MOUSECAPTURE      4  // ���� �������� "����" (������/������)
#define RDS_NOWINREFRESH      8  // ����������� ����������� ���� ����� ��������
                                 // ��������� (������/������, ����������� ������������
                                 // �������� EnableWindowRefresh)
#define RDS_WINREFRESHWAITING 16 // ����� ����� ���������� ����������� ����������� ����
                                 // ������������ �������� EnableWindowRefresh ����������
                                 // ������������ ���� �� ���� (������/������)
#define RDS_DISABLED          32 // ���� �� ��������� �� �������� ������������ - �� ��
                                 // ����� ��� ��������, ��������������� ��� �������.
                                 // ���� ������������, �� �� ��������� �� "����" � ����������.
#define RDS_CTRLCALC          64 // ���� ������ ���� ������ ����� ������ ������ �������
                                 // � ������ RDS_BFM_PREMODEL
#define RDS_NOSUSPEND        128 // ������ ����� � ������� RDS_BFM_MODEL � RDS_BFM_PREMODEL
                                 // �� ����� ���� ����������� ��������� ��������
                                 // rdsSuspendCalc **** REV2 ****
#define RDS_SUSPENDED        256 // ���� ������� ��������� ������� rdsSuspendCalc ��� ���������
                                 // ������� ����� ����� � ��� ������� � ������� RDS_BFM_MODEL �
                                 // RDS_BFM_PREMODEL. ��� ���� ������ ����� ��� ����� ������������
                                 // �������� �� ���, ���� ������� ���� RDS_NOSUSPEND **** REV2 ****
#define RDS_ALWAYSSETUP      512 // ���� � ��������� ����� ��������, ���� ���� �������� ����������
                                 // �� ������ (���������� RDS_VARCHECKFAILED) **** REV2 ****
#define RDS_ALWAYSLOADSAVE  1024 // ���������� � �������� ���������� ����� ���������, ���� ���� �������� ����������
                                 // �� ������ (���������� RDS_VARCHECKFAILED) **** REV2 ****
#define RDS_BLOCKHASHELP    2048 // � ����� ���� ����� "�������" � ����������� ���� **** REV2 ****
#define RDS_INITCALC        4096 // ���� ���������� ��� ������������������ ������� ��� ������
                                 // ������ �� �������������� ��� ������� ������ �� ������ ������ **** REV2 ****
#define RDS_INITCALCFIRST 0x2000 // ���� ���������� ��� ������������������ ������� ��� ������
                                 // ������ �� �������������� � ����� ������ ���������� �� ���������� **** REV2 ****
#define RDS_LOCKTEXTRECT  0x4000 // ��������� ���������� �������������� � ������� �� ������� ���� ����� ������������ �
                                 // ���������� ������ ����� (�������������� ������������ � ���� ����������, �����
                                 // ��������� ������� ������ �� ��������������) **** REV2 ****
//---------------------------------------------------------------------------
// ����-������� ��� ������ ������ �������
#define RDS_NEEDSREDRAW         RDS_NEEDSDLLREDRAW
//---------------------------------------------------------------------------

// ������ �����
typedef struct
{ LPVOID VarData;       // ����� ������ ������ ����� (������ ������)
  LPVOID BlockData;     // ����� ������� ������ ����� (������/������)
  RDS_BHANDLE Block;    // ������������� ����� (������ ������)
  LPSTR BlockName;      // ��� ����� (������ ������)
  RDS_BHANDLE Parent;   // ������������� ���������� ���������� (������ ������)
  DWORD Flags;          // ����� (��. ����)
  // ������� �������� (��������� � ������ ��������������) ������������ ��������������.
  // ��� ������ ����� ����� ������ ���� ���� ������ ���� �������� �������.
  // ��������� ��������� �������� ������ ���� �������������� (�.�. ����� �������� ���
  // ��������� ��������) ����� ���� ������� � �����������, ���������� �����������
  // �������� ��� ��������� ���������� � RDS_DRAWDATA � RDS_MOUSEDATA ��� ���������������
  // �������.
  int Width,Height;
  // ����� ���������������� ���� (�� �������������� � �� ����������������)
  int Tag;
} RDS_BLOCKDATA;
typedef RDS_BLOCKDATA *RDS_PBLOCKDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ ��� ����������������� ������� � ������ RDS_BFM_MODEL **** REV2 ****
// (��� ������� ������� ���������� NULL)
typedef struct
{ BOOL FirstInChain;    // ���� ���� �������� �������� ��-�� ����� RDS_INITCALCFIRST

} RDS_INITIALCALCDATA;
typedef RDS_INITIALCALCDATA *RDS_PINITIALCALCDATA;
//---------------------------------------------------------------------------


// ���������, ������������ ������ � �������� �����������
// (RDS_BFM_MOUSE* � RDS_BFM_WINDOWMOUSE*)
typedef struct
{ int x,y;              // ���������� "����"
  int BlockX,BlockY;    // ���������� ����� �������� ����� (� ������ ����� � �����������)
                        // (������ ��� ��������� RDS_BFM_MOUSE*)
  int Left,Top;         // ������� ����� ���� ���� ����� (�� ���������� ���������)
                        // (������ ��� ��������� RDS_BFM_MOUSE*)
  int Width,Height;     // ������� ���� ����� (�� ���������� ���������)
                        // (������ ��� ��������� RDS_BFM_MOUSE*)
  int IntZoom;          // ������� � % (�����)
  DWORD Button;         // ������ - ������� ������ (���� �� ���� ������ "����" RDS_M*)
  DWORD Shift;          // ����� ���� � ���������� (RDS_M*, RDS_K*)
  DWORD Reserved;       // ��������������� ��� ������������ **** REV2 ****
  double DoubleZoom;    // ���������� �-� (� ����� �������)

  int MouseEvent;       // ������� ������ - RDS_BFM_MOUSEDOWN, RDS_BFM_MOUSEUP,
                        // RDS_BFM_MOUSEMOVE ��� RDS_BFM_MOUSEDBLCLICK
                        // (��� ������ � ���������� RDS_BFM_WINDOWMOUSE* ���� ������������
                        // ��������������� ��������� RDS_BFM_MOUSE*)
  int Viewport;         // ����� ����� ������, ����� ������� ������ ���������, ��� -1,
                        // ���� ��� ������ �� �������� ���� ����������
} RDS_MOUSEDATA;
typedef RDS_MOUSEDATA *RDS_PMOUSEDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ������������� �����������
// (RDS_BFM_KEY* � RDS_BFM_WINDOWKEY*)
typedef struct
{ int KeyCode;          // ��� ������� (�����������, ��. WinAPI)
  BOOL Repeat;          // ��� RDS_BFM_KEYDOWN: ������� ������������� ������������
  int  RepeatCount;     // ��� RDS_BFM_KEYDOWN ��� Repeat==TRUE: ����� �����������
                        // � ������� ����������� ������ (������ 1)
  DWORD Shift;          // ����� ���������� (RDS_M*, RDS_K*)
  int KeyEvent;         // ������� ������ - RDS_BFM_KEYDOWN ��� RDS_BFM_KEYUP
                        // (��� ������ � ���������� RDS_BFM_WINDOWKEY* ���� ������������
                        // ��������������� ��������� RDS_BFM_KEY*)
  //------- ������������ ��������� -------
  BOOL Handled;         // ��������������� � TRUE, ���� ��������� ����� (� ���������
                        // ���� � ������ RDS_BFM_KEYDOWN) �� ������ ����������� �� ��� �������
  int Viewport;         // ����� ����� ������, ����� ������� ������ ���������, ��� -1,
                        // ���� ��� ������ �� �������� ���� ����������
} RDS_KEYDATA;
typedef RDS_KEYDATA *RDS_PKEYDATA;
//---------------------------------------------------------------------------

// ��������, ��������� �������� ��� ������ ������ �����
// (������������ rdsGetSystemInt(RDS_GSISAVELOADACTION))
#define RDS_LS_ERROR            0       // � ������ ������ �� ������������ �������� ��� ������
#define RDS_LS_LOADROOT         1       // �������� ��������� ����� ��� �������� �������
#define RDS_LS_SAVEROOT         2       // ������ ��������� ����� ��� ������ �������
#define RDS_LS_LOADCONTENT      3       // �������� ����� � ������� ����������� ����������
#define RDS_LS_SAVECONTENT      4       // ������ ����� � ������� ������������ ����������
#define RDS_LS_LOADCLIPBRD      5       // �������� ����� ��� ������� �� ������ ������
#define RDS_LS_SAVECLIPBRD      6       // ������ ����� ��� ����������� � ������ ������
#define RDS_LS_LOADFROMFILE     7       // �������� ���������� ����� � ����� ��� �� ����������
#define RDS_LS_SAVETOFILE       8       // ������ ���������� ����� �� ���� ��� � ����������
#define RDS_LS_LOADUNDO         9       // �������� ����� ��-�� ������ ���������
#define RDS_LS_SAVEUNDO         10      // ������ ����� � ����� ��� ������ ���������
#define RDS_LS_LOADAUTOCOMP     11      // �������� ������ ����� ����� �������������� ������
#define RDS_LS_SAVEAUTOCOMP     12      // ������ ������ ����� ����� ��������������� ������
#define RDS_LS_LOADTAGGED       13      // �������� ����� � ����.������� � ������
#define RDS_LS_SAVETAGGED       14      // ������ ����� � ����.������� � ������
#define RDS_LS_LOADPROG         15      // �������� ����� ��-�� ������ �����-���� ��������� ������� **** REV2 ****
#define RDS_LS_SAVEPROG         16      // ������ ����� ��-�� ������ �����-���� ��������� ������� **** REV2 ****
//---------------------------------------------------------------------------

// ��������, ��������� �������� ��� ������ ������ ����� ��� ����������/�������� ��� ������ ���������
// (�.�. ��� rdsGetSystemInt(RDS_GSISAVELOADACTION)==RDS_LS_LOADUNDO ��� RDS_LS_SAVEUNDO
// (������������ rdsGetSystemInt(RDS_GSIUNDOACTION))  **** REV2 ****
#define RDS_UA_ERROR            0       // � ������ ������ �� ������������ �������� ��� ������ ��� ������ ��������
#define RDS_UA_ADDBLOCK         1       // ���������� �����
#define RDS_UA_DELBLOCK         2       // �������� �����
#define RDS_UA_EDITBLOCK        3       // ��������� ��������� ���������� ����� (������ ��������� �� ����)
#define RDS_UA_SETUPBLOCK       4       // ����� ��������� �����
#define RDS_UA_PROGUNDO         5       // �������� ����� ������ ��� ����� ��������� ��������
#define RDS_UA_TRANSLATE        6       // ������� �� ������� �������
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ����������� � ��������� (RDS_BFM_DRAW,RDS_BFM_DRAWADDITIONAL)
typedef struct
{ HDC dc;               // ��� �������� (��������, ��. WinAPI)
  BOOL CalcMode;        // TRUE - ����� �������, FALSE - ����� ��������������
  int BlockX,BlockY;    // ���������� ����� �������� ����� (� ������ ����� � �����������)
  double DoubleZoom;    // ���������� �-� (� ����� �������)

  // ������ ��������� ������������ �������������� - ����������� ���������� ���������
  // ��� ����������� �������������. ��� ���������� - ���������� �� ������� ���� � ������
  // ��������. ������ ������������ ������ � ������ �������, � ������ ��������������
  // ������� �������������� ������� �� RDS_BLOCKDATA::Width � RDS_BLOCKDATA::Height,
  // � ������� ����� ���� ��������� � ������ ��������.
  BOOL RectValid;       // ���� ��������� ��������� ��������� ����� ������, ��� ������
                        // ���������� ��� ���������� � TRUE. ����� ������ �����
                        // ���������������, � ������������� ���������� �� RDS_BLOCKDATA.
  int Left,Top;         // ������� ����� ���� ���� �����
  int Width,Height;     // ������� ���� �����
  //---------

  RECT *VisibleRect;    // ������� ������������ � ���� ������������� (����, ������ ������)
  BOOL FullDraw;        // TRUE - ����� ������������ ���� ����, FALSE - ����� ������������
                        // ������ ������������ � �������� ������ ����� (��� ����������
                        // ������ ���������)
} RDS_DRAWDATA;
typedef RDS_DRAWDATA *RDS_PDRAWDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ����������� �� ��������� ������� �����
// (RDS_BFM_RESIZE, RDS_BFM_RESIZING)
typedef struct
{ BOOL HorzResize;        // ���� ��������� ��������������� �������
  BOOL VertResize;        // ���� ��������� ������������� �������
  // HorzResize � VertResize ��������������� � ����������� �� ����,
  // �� ����� �� ������ ����� ��������������� "�����" ������������.
  // ��� ��������� ������� ����� ��������� ����� ��� ��� ����������
  // ����� FALSE.
  int newWidth,newHeight; // ����� �������� ������ � ������
  // ������ � ����� ���������
  int GridDx,GridDy;    // �������
  BOOL SnapToGrid;      // �������� �� ��������
} RDS_RESIZEDATA;
typedef RDS_RESIZEDATA *RDS_PRESIZEDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� � ����������� �����
// (RDS_BFM_MOVED)
typedef struct
{ int MoveReason;       // ������� ����������� (RDS_MR_*)
  int OldX,OldY;        // ������ ���������� ����� ��������
  int NewX,NewY;        // ����� ���������� ����� ��������
} RDS_MOVEDATA;
typedef RDS_MOVEDATA *RDS_PMOVEDATA;
// ������� �����������
#define RDS_MR_SET      0       // ���������������� ��������� ��������� (������� ��
                                // ������ ������, ��������� ������� � �.�)
#define RDS_MR_DRAG     1       // �������������� � ���� ����������
#define RDS_MR_KEYBOARD 2       // ����������� � ���������� � ���� ����������
#define RDS_MR_UNDOREDO 3       // ������ ��� ������ �������� ������������ ��
                                // ����������� 
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ����������� �� ������� � ��������� �������
// (RDS_BFM_STARTCALC, RDS_BFM_STOPCALC)
// ��� ��������� - ������ ������
typedef struct
{ BOOL FirstStart;      // TRUE  - ������ ������� � ������ ������
                        // FALSE - ������ ���������
  BOOL Loop;            // TRUE  - ������ ������������� �������
                        // FALSE - ������ �� ���� ����
} RDS_STARTSTOPDATA;
typedef RDS_STARTSTOPDATA *RDS_PSTARTSTOPDATA;
//---------------------------------------------------------------------------

// ��������� ���������� ������ ����������� (����������) ������� �����
// (RDS_BFM_FUNCTIONCALL)
typedef struct
{ int Function;         // ����� �������
  LPVOID Data;          // ������ ������� (�� �������������� ��������)

  int Reserved;         // ��������������� ��� ����������� �������������
  RDS_BHANDLE Caller;   // ��������� ����
  BOOL Broadcast;       // TRUE - ����������������� ���������, FALSE - ������
  int BroadcastCnt;     // ��� Broadcast==TRUE - ������� � 0, ����������� �� ����������
                        // ����� ������� ����� ����� ���������� ���������
  BOOL Stop;            // ��� Broadcast==TRUE � �����. ���������� ������� rdsBroadcastFunctionCallsEx
                        // ����� ���� ����������� � ������ TRUE, ����� ���� ����������������� �����
                        // ����� ���������� ���������
  BOOL Delayed;         // TRUE - ���������� �����, FALSE - ������
  // ��� ����������� ������
  DWORD DataBufSize;    // ������ ������, ����� �������� ������� ����� Data
} RDS_FUNCTIONCALLDATA;
typedef RDS_FUNCTIONCALLDATA *RDS_PFUNCTIONCALLDATA;
//---------------------------------------------------------------------------
// ��������� ������� ��� ���������� ���� ���������� �������
#define RDS_FUNCPARAMCAST(datastruct,type) ((type*)(((RDS_PFUNCTIONCALLDATA)(datastruct))->Data))
#define RDS_FUNCPARAMPVOID(datastruct) (((RDS_PFUNCTIONCALLDATA)(datastruct))->Data)
//---------------------------------------------------------------------------

// ��������� ���������� ������ ���������� ������� ����� (������� �� �����
// ����, ��������� ����� ������)
// (RDS_BFM_MENUFUNCTION)
typedef struct
{ int Function;         // ����� ���������� �������
  int MenuData;         // ������ ������� (�� �������������� ��������)

} RDS_MENUFUNCDATA;
typedef RDS_MENUFUNCDATA *RDS_PMENUFUNCDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� � ����������� ���������
// (RDS_BFM_POPUPHINT)
typedef struct
{ int x,y;              // ���������� "����"
  int BlockX,BlockY;    // ���������� ����� �������� ����� (� ������ ����� � �����������)
  // ����������� ������������� �����
  int Left,Top;         // ������� ����� ���� ���� ����� (�� ���������� ���������)
  int Width,Height;     // ������� ���� ����� (�� ���������� ���������)
  //------- ������������ ��������� -------
  int HZLeft,HZTop,     // ������� ���� �������� ��������� (�� ��������� -
      HZWidth,HZHeight; // ���� ������������� �����)
  int ReshowTimeout;    // �������� (��), ����� ������� ����� ������ ������� ���������
                        // (�� ��������� - 0, �.�. �� �������� ������ �� ��������� ���� HZ*)
  int HideTimeout;      // �������� (��), ����� ������� ����� �������� ���������
                        // (�� ��������� - ����������� �������� ��� Windows)
  //--------------------------------------
  // ������� (�� ������ ������)
  int IntZoom;          // ������� � % (�����)
  DWORD Reserved;       // ��������������� ��� ������������ **** REV2 ****
  double DoubleZoom;    // ���������� �-� (� ����� �������)

} RDS_POPUPHINTDATA;
typedef RDS_POPUPHINTDATA *RDS_PPOPUPHINTDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� �� �������������� ����������
// (RDS_BFM_REMOTEMSG)
typedef struct
{ LPSTR String;         // ���������� ������
  int   Value;          // ���������� �����
  LPSTR ControllerName; // ��� ��������� �������������� ���������� (���� ���
                        // ��� ��������)
} RDS_REMOTEMSGDATA;
typedef RDS_REMOTEMSGDATA *RDS_PREMOTEMSGDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� � ������������� �������� ����
// (RDS_BFM_WINREFRESH)
typedef struct
{ RDS_TIMERID Timer;    // ������, ��������� ����������, ��� NULL, ���� ����������
                        // ������� ��������� �������� (������ ������)
  DWORD Reserved;       // ��������������� ��� ������������ **** REV2 ****
  //------- ������������ �������� -------
  double Delay;         // ����� ���������� ����, �� (������, �������� -1.0)
} RDS_WINREFRESHDATA;
typedef RDS_WINREFRESHDATA *RDS_PWINREFRESHDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� � ��������� � ����� ����������
// (RDS_BFM_WINDOWOPERATION)
typedef struct
{ int Operation;        // �������� � �����
  HWND Handle;          // Handle ����
  BOOL EditMode;        // ������� ����� ��������������
  BOOL Running;         // ���� ������������� (������ ��� EditMode==FALSE)
  BOOL OwnWindow;       // �������� � ����� ������ ���������� (TRUE) ��� ������������ (FALSE)
} RDS_WINOPERATIONDATA;
typedef RDS_WINOPERATIONDATA *RDS_PWINOPERATIONDATA;
//---------------------------------------------------------------------------
// �������� � ����� ��� RDS_WINOPERATIONDATA::Operation
#define RDS_SWO_OPEN    0       // �������� ����
#define RDS_SWO_CLOSE   1       // �������� ����
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� �� �������� ������������ ����
// (RDS_BFM_CONTEXTPOPUP)
// ��� ��������� - ������ ������
typedef struct
{ BOOL EditMode;        // ������� ����� ��������������
  BOOL FreeSpace;       // ���� ����������� �� ��� �����, � ��� ���������� ����� ����������
                        // (������ ��� ���������)
} RDS_CONTEXTPOPUPDATA;
typedef RDS_CONTEXTPOPUPDATA *RDS_PCONTEXTPOPUPDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� � ���������� ����� �������������
// (RDS_BFM_MANUALINSERT)
typedef struct
{ int Reason;           // ���������� ����� �� ������ ������ (RDS_LS_LOADCLIPBRD) ���
                        // �� ����� (RDS_LS_LOADFROMFILE)
  BOOL Single;          // ����������� ���� ���� (TRUE) ��� ��������� (FALSE)
} RDS_MANUALINSERTDATA;
typedef RDS_MANUALINSERTDATA *RDS_PMANUALINSERTDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� �� �������� ����� �������������
// (RDS_BFM_MANUALDELETE)
typedef struct
{ BOOL Single;          // ��������� ���� ���� (TRUE) ��� ��������� (FALSE)
  BOOL WithSys;         // ��������� ������ ��������� ���������� (TRUE) ���
                        // �������������� (� ������� ������ ���������� ������
                        // ��� ����)
} RDS_MANUALDELETEDATA;
typedef RDS_MANUALDELETEDATA *RDS_PMANUALDELETEDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� �� ������������ ��� ������� �������� ����������
// (RDS_BFM_NETCONNECT, RDS_BFM_NETDISCONNECT)
typedef struct
{ int ConnId;           // ������������� ����������
  LPSTR Host;           // ����� ������� (������ ��� �������)
  int Port;             // ����
  LPSTR Channel;        // ��� ������
  BOOL ByServer;        // ��� ������� ��� RDS_BFM_NETDISCONNECT - TRUE ���� ���������� ��������� ��������

} RDS_NETCONNDATA;
typedef RDS_NETCONNDATA *RDS_PNETCONNDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� � �������� �� ���� ������
// (RDS_BFM_NETDATARECEIVED)
typedef struct
{ int ConnId;           // ������������� ����������
  LPSTR Host;           // ����� ������� (������ ��� �������)
  int Port;             // ����
  LPSTR Channel;        // ��� ������

  int Id;               // �������� ����� �����
  LPSTR Str;            // �������� ������ (�� ����� ���� NULL)
  LPVOID Buffer;        // �������� ����� ��� NULL
  DWORD BufferSize;     // ������ ��������� ������

  RDS_NETSTATION SenderStation; // ������������� ������-�����������
  RDS_NETBLOCK SenderBlock;     // ������������� ����� �� ���� ������
} RDS_NETRECEIVEDDATA;
typedef RDS_NETRECEIVEDDATA *RDS_PNETRECEIVEDDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� � ������� ������
// (RDS_BFM_NETERROR)
typedef struct
{ int ConnId;           // ������������� ����������
  LPSTR Host;           // ����� ������� (������ ��� �������)
  int Port;             // ����
  LPSTR Channel;        // ��� ������

  int ErrorCode;        // ��� ������ (RDS_NETERR_*)

  RDS_NETSTATION Station; // ������������� ������ (��� ��������������� ������)
  RDS_NETBLOCK Block;     // ������������� ����� (��� ��������������� ������)
} RDS_NETERRORDATA;
typedef RDS_NETERRORDATA *RDS_PNETERRORDATA;
// ���� ������
#define RDS_NETERR_NOBLOCK      1       // ��� �����, �������� ���� �������� ������
#define RDS_NETERR_GENERAL      2       // ����������� ������
#define RDS_NETERR_SEND         3       // ������ ��� ��������
#define RDS_NETERR_RECEIVE      4       // ������ ��� ������
#define RDS_NETERR_DISCONNECT   5       // ������ ��� ������� ��������� �����
#define RDS_NETERR_ACCEPT       6       // ������ ��� ��������� ���������� � ��������
#define RDS_NETERR_CLIENTCONN   7       // ������ ��� ��������� ����� (������ ��� �������)
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� � ��������� ������ ��������
// (RDS_BFM_NETDATAACCEPTED)
typedef struct
{ int ConnId;           // ������������� ����������
  LPSTR Host;           // ����� ������� (������ ��� �������)
  int Port;             // ����
  LPSTR Channel;        // ��� ������

  int Id;               // Id ������������ ������
} RDS_NETACCEPTDATA;
typedef RDS_NETACCEPTDATA *RDS_PNETACCEPTDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ����������� � ��������� �������������� ����� **** REV2 ****
// (RDS_BFM_SUPPORTSCONNSETUP, RDS_BFM_CONNSETUP � RDS_BFM_CONNHINT)
typedef struct
{ RDS_CHANDLE Conn;     // ������������� �����
  BOOL Output;          // TRUE - ����� ���������� � ������ �����, FALSE - �� �����
  LPSTR VarName;        // ������ ��� ������������ ����������
  int VarType;          // �������������� ��� ���������� (RDS_VARTYPE_*)
  int VarNum;           // ����� ���������� � �����

  // ������ ��� RDS_BFM_CONNHINT (������������ ���������)
  int ReshowTimeout;    // �������� (��), ����� ������� ����� ������ ������� ���������
                        // (�� ��������� - 0, �.�. �� �������� ������ �� ��������� ���� HZ*)
  int HideTimeout;      // �������� (��), ����� ������� ����� �������� ���������
                        // (�� ��������� - ����������� �������� ��� Windows)
} RDS_CONNSETUPDATA;
typedef RDS_CONNSETUPDATA *RDS_PCONNSETUPDATA;
//---------------------------------------------------------------------------


// ��������� �������� ������ ����� � ���� ����������
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  RDS_HOBJECT Object;   // ������, ��������� � ���� �������
  RDS_BHANDLE Block;    // ����-�������� ������
  int PLeft,PTop;       // ����� ������� ���� ������ � ��������� ����������� (������� 100%)
  int PWidth,PHeight;   // ������ ������ � ��������� ����������� (������� 100%)
  int Order;            // ����� ������ � ������ �����
  BOOL Visible;         // ��������� ������
  HWND Handle;          // Handle �������� ������� ������ ��� NULL
  int Width,Height;     // ������� �������� ������� ������ (��� Handle!=NULL)
  BOOL Border;          // ������� ����� ������ �������� �������
  BOOL CloseButton;     // ������� ������ �������� ������
  BOOL Scalable;        // ������ ������ ������ ��� ��������� �������� �������
  BOOL Sizeable;        // ������ ������ ����� ���������� �������������
  BOOL Moveable;        // ��������� ������ ����� ���������� �������������
  BOOL CaptionBar;      // ���� ������ ��������� ���� (�� ������� ����� �������������)
  LPSTR Caption;        // ������� ��������� (�������� � ���� ��������� ������!)
} RDS_PANDESCRIPTION;
typedef RDS_PANDESCRIPTION *RDS_PPANDESCRIPTION;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� �� ������ �����
// (RDS_BFM_BLOCKPANEL)
typedef struct
{ int Operation;                // �������� � ������� (���� �� �������� RDS_PANOP_*)
  RDS_PPANDESCRIPTION Panel;    // �������� ������ (��������� �� ���������, ������������
                                // ������ � ������ ������ ���������)
} RDS_PANOPERATION;
typedef RDS_PANOPERATION *RDS_PPANOPERATION;
// ��������
#define RDS_PANOP_CREATE        0       // �������� �������� ������� ��� ������
#define RDS_PANOP_DESTROY       1       // ����������� �������� ������� ��� ������
#define RDS_PANOP_RESIZED       2       // ������ ������ �������
#define RDS_PANOP_MOVED         3       // ������ ���������� �������������
#define RDS_PANOP_PAINT         4       // ���������� ������������ ������
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� �� ���������� � ���������� **** REV2 ****
// (RDS_BFM_CONTENTCHANGE)
typedef struct
{ RDS_BHANDLE Changed;  // ���������� (��������, ���������), � ������� ��������� ���������
  BOOL Deep;            // ��������� �� � ����� ����������, � �� ��������� (Changed!=Parent)
} RDS_CONTENTCHANGE;
typedef RDS_CONTENTCHANGE *RDS_PCONTENTCHANGE;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� �� ��������� �������� �������� ���� ���������� **** REV2 ****
// (RDS_BFM_SYSWORKSIZECHANGE)
typedef struct
{ RDS_BHANDLE Sys;        // ����������
  int OldWidth,OldHeight; // ������ �������
  int NewWidth,NewHeight; // ������ �������
} RDS_SYSWORKWIDTHCHANGE;
typedef RDS_SYSWORKWIDTHCHANGE *RDS_PSYSWORKWIDTHCHANGE;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� �� ��������� ������������ ����� ���������� **** REV2 ****
// (RDS_BFM_LAYERCONFIGCHANGE)
typedef struct
{ RDS_BHANDLE Sys;      // ����������
  BOOL OwnConfig;       // TRUE  - ������� ����� ���������� �� ��������� ������������ � ���
                        // FALSE - ������� ����� �� ��������� ������������ � ������������ ����������
  LPSTR ConfigName;     // ��� ������������� ������������
  int ConfigNum;        // ����� ������������� ������������
} RDS_LAYERCONFIGCHANGE;
typedef RDS_LAYERCONFIGCHANGE *RDS_PLAYERCONFIGCHANGE;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� � ������ ��� �������� ����� ��������� **** REV2 ****
// (RDS_BFM_TRANSLATE)
typedef struct
{ BOOL Save;       // TRUE  - ���� ������ ����� ���������
                   // FALSE - ���� �������� ���������
} RDS_TRANSLATEDATA;
typedef RDS_TRANSLATEDATA *RDS_PTRANSLATEDATA;
//---------------------------------------------------------------------------

// ������� ������ ����� - ��� ���������
typedef int(RDSCALL *RDS_BLOCKENTRYPOINT)(int,RDS_PBLOCKDATA,LPVOID);
//---------------------------------------------------------------------------

//======================= ��������������� �������� ============================
// ��������, ����������� ��� ������ ��������� ������������ �������

// ��������� �� COLORREF
typedef COLORREF *RDS_PCOLORREF;
//---------------------------------------------------------------------------

// ���� ������ � ������, ������������ ������������� ���������
#define RDS_TUNKNOWN            0       // ��������� ��� (��� ������ � �������)
// �����
#define RDS_BTSYSTEM            1       // ����������
#define RDS_BTSIMPLEBLOCK       2       // ���� � ������� DLL (�������)
#define RDS_BTINPUTBLOCK        4       // ������� ����
#define RDS_BTOUTPUTBLOCK       8       // ������� �����
#define RDS_BTBUSPORT           64      // ���� ����
#define RDS_BTDLLBLOCK          RDS_BTSIMPLEBLOCK // ������� (��� �������������)
// �����
#define RDS_CTCONNECTION        16      // ������� �����
#define RDS_CTBUS               32      // ����
//---------------------------------------------------------------------------
// ��� ���� ������
#define RDS_BTALLTYPES  (RDS_BTSYSTEM | RDS_BTSIMPLEBLOCK | RDS_BTINPUTBLOCK | RDS_BTOUTPUTBLOCK | RDS_BTBUSPORT)
// ��� ���� ������
#define RDS_CTALLTYPES  (RDS_CTCONNECTION | RDS_CTBUS)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������������� ���� servSize � ����������, ������������ � RDS ��� ����������
#define RDS_INITSERVSIZE(s)     s.servSize=sizeof(s)
//---------------------------------------------------------------------------

// ��������� �������� �����, ����������� �������� rdsGetBlockDescription
// ����������� �������� ������, ������ ������� ���������� � ���������
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  RDS_BHANDLE Block;    // ������������� �����
  int BlockType;        // ��� ����� (��. ����)
  LPSTR BlockName;      // ��� �����
  RDS_BHANDLE Parent;   // ������������� ���������� ����������
  LPSTR ParentName;     // ��� ����������
  LPSTR BlockComment;   // ����������� �����
  int LayerId;          // ������������� ����
  LPSTR DllFile;        // ��� ������ DLL ��� NULL
  LPSTR DllFunc;        // ��� ������� DLL ��� NULL
  HINSTANCE Module;     // ����������� ������ DLL ��� NULL
  LPSTR ParentVar;      // ��� ���������� ����������, ������� ������������� �����
                        // ����� (������ ��� �������� ����� � ������) ��� NULL
  BOOL Selected;        // ���� ������� � ��������� (������ � ������ ��������������)
  int NumberOfVars;     // ����� ���������� �����
  DWORD Flags;          // ����� �������� (RDS_BDF_*)
  DWORD ExtId;          // ������� ���������� ������������� (��� ����������, �����������
                        // RDS �������)
  int NamePos;          // ��������� ����� ����� (RDS_BDNP_*)
  int NameDx,NameDy;    // �������� ������ �������� ���� ����� �� ����� �������� �����
                        // (� �������� 100%)
  int AltNameAlignment; // ������������ ������, ���������� ������ ����� (���� �� ����):
                        // -1 (�����), 0 (�� ������) ��� 1 (������)

} RDS_BLOCKDESCRIPTION;
typedef RDS_BLOCKDESCRIPTION *RDS_PBLOCKDESCRIPTION;
// ����� ��������
#define RDS_BDF_RUNEVERYCYCLE   1       // ������ ������ ����
#define RDS_BDF_NAMEOFF         2       // ���������� ������ ����� �����
#define RDS_BDF_SHOWMAINPOINT   4       // ���������� � ��������� ����� ��������
#define RDS_BDF_SELFDRAW        8       // ��������� ������������ �������� DLL
#define RDS_BDF_MOUSEEVENTS     0x10    // ������� ����� �������� ��������� �� "����"
#define RDS_BDF_FREEMOUSEMOVE   0x20    // ������� ����� �������� ��������� � ����������� "����"
                                        // � ����������� ��������
#define RDS_BDF_ALLOWRESIZE     0x40    // ���������� ��������������� "�����"
#define RDS_BDF_LOCKWIDTH       0x80    // ����������� ������ ��� ��������������� (��� RDS_BDF_ALLOWRESIZE)
#define RDS_BDF_LOCKHEIGHT      0x100   // ����������� ������ ��� ��������������� (��� RDS_BDF_ALLOWRESIZE)
#define RDS_BDF_SETUPFUNC       0x200   // ���� ������� ���������
#define RDS_BDF_SETUPBYDCLICK   0x400   // ������� ��������� �� �������� ������
#define RDS_BDF_POPUPHINT       0x800   // ���� ������� �� Hint
#define RDS_BDF_KBDEVENTS       0x1000  // �������� ��������� �� ����������
#define RDS_BDF_HASPICTURE      0x2000  // ���� ��������
#define RDS_BDF_TEXTRECT        0x4000  // ���� ������������ ��������������� � �������
#define RDS_BDF_ALLOWMOVE       0x8000  // ���� ����� ������������ ������������� **** REV2 ****
// ��������� ����� �����
#define RDS_BDNP_BELOW          0       // ��� ������
#define RDS_BDNP_ABOVE          1       // ��� ������
#define RDS_BDNP_CUSTOM         2       // ������������ (���������� ������������� �������)
//---------------------------------------------------------------------------

// ��������� �������� �������� ������������ �������������� �����
// � ����� �������� (����������� �������� rdsGetBlockDimensions)
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  int BlockX,BlockY;    // ��������� ����� �������� �� ������� ����

  int Left,Top;         // ������� ����� ���� ���� ����� �� ������� ����
  int Width,Height;     // ������� ���� ����� �� ������� ����
} RDS_BLOCKDIMENSIONS;
typedef RDS_BLOCKDIMENSIONS *RDS_PBLOCKDIMENSIONS;
//---------------------------------------------------------------------------
// ����� ��� rdsGetBlockDimensionsEx
#define RDS_GBD_NONE            0       // ��� ������ (��� ������ ���������� ���������)
#define RDS_GBD_USEZOOM         1       // ������������ ������� ������������ ����������
#define RDS_GBD_USEVARS         2       // � ������� ������������� � ������� ��������� ����� � �����������
//---------------------------------------------------------------------------

// ��������� �������� �����, ����������� �������� rdsGetConnDescription
// ����������� �������� ������, ������ ������� ���������� � ���������
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  RDS_CHANDLE Conn;     // ������������� �����
  int ConnType;         // ��� ����� (��. ����)
  RDS_BHANDLE Parent;   // ������������� ���������� ����������
  LPSTR ParentName;     // ��� ����������
  BOOL Active;          // ��������� ����� (��������/���������)
  int LayerId;          // ������������� ����
  int NumPoints;        // ����� ����� � �����
  LPSTR BusName;        // ��� (������ ��� ����)
  int NumChannels;      // ����� ������� (������ ��� ����)
  int NumLines;         // ����� �����, ����������� ����� �� ����������� �����
  DWORD ExtId;          // ������� ���������� ������������� (��� ����������, �����������
                        // RDS �������)
  BOOL Selected;        // ����� �������� � ��������� (������ � ������ ��������������) **** REV2 ****
  LPSTR Text;           // �������������� ����� ��� NULL, ���� ��� ��� **** REV2 ****
  BOOL ShowText;        // ���������� �������������� ����� ����� �� ������ **** REV2 ****
  int TextX,TextY;      // ���������� ���������� ������ ��������������� ������, ���� �� ���� **** REV2 ****
  int TextAlignment;    // ������������ ������ (-1 - �����, 0 - �� ������, 1 - ������) **** REV2 ****
  BOOL TextTranslate;   // ������������� �������� ������ **** REV2 ****
} RDS_CONNDESCRIPTION;
typedef RDS_CONNDESCRIPTION *RDS_PCONNDESCRIPTION;
//---------------------------------------------------------------------------

// ��������� �������� �������� ���� �����, ������������ ���������
// rdsGetConnAppearance, rdsSetConnAppearance
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  COLORREF LineColor;   // ���� �����
  int LineWidth;        // ������� �����
  int LineStyle;        // ����� ����� (WinAPI)
  int ArrowLength;      // ����� ������� (0...255)
  int ArrowWidth;       // ������ ������� (0...255)
  int DotSize;          // ������� ����� ����������
} RDS_CONNAPPEARANCE;
typedef RDS_CONNAPPEARANCE *RDS_PCONNAPPEARANCE;
//---------------------------------------------------------------------------

// ���� ����� �����, ������������ ������������� ���������
#define RDS_PTINTERNAL          1       // ���������� �����
#define RDS_PTBLOCK             2       // ���������� � ������
#define RDS_PTBUS               4       // ���������� � �����
#define RDS_PTBUSTOBLOCK        8       // ���������� ���� � ������
// ��������� �����
#define RDS_PSNORMAL            0       // ����� � �������
#define RDS_PSBADVAR            1       // ��� ����� ����������
#define RDS_PSBADTYPE           2       // �������������� �����
// ��������� �������� ����� �����, ����������� �������� rdsGetPointDescription
// ����������� �������� ������, ������ ������� ���������� � ���������
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  int PointType;        // ��� ����� (��. ����)
  BOOL Source;          // TRUE - �������� ������, FALSE - ����������
                        // (�� ������������ ��� RDS_PTINTERNAL)
  int Status;           // ��������� (RDS_PS******)
  LPSTR ObjectName;     // ��� ����� ��� ���� (����� RDS_PTINTERNAL)
  LPSTR VarName;        // ��� ���������� (����� RDS_PTINTERNAL)
  RDS_BHANDLE Block;    // �������������� ���� ��� PointType==RDS_PTBLOCK ��� NULL
  RDS_CHANDLE Bus;      // �������������� ���� ��� PointType==RDS_PTBUS ��� NULL

  RDS_CHANDLE Owner;    // �����-�������� �����
  int x,y;              // ���������� ����� (��� RDS_PTBLOCK - �������������)
  int PointNum;         // ����� ����� � �����

  char VarType;         // �������������� ��� ���������� (RDS_VARTYPE_*, ����� RDS_PTINTERNAL) **** REV2 ****
  BYTE Res1,Res2,Res3;  // ��������������� ��� ������������ **** REV2 ****
  int VarNum;           // ����� ���������� � ����� (������ ��� RDS_PTBLOCK) ��� -1 **** REV2 ****
} RDS_POINTDESCRIPTION;
typedef RDS_POINTDESCRIPTION *RDS_PPOINTDESCRIPTION;
//---------------------------------------------------------------------------

// ���� ����� �����, ������������ ������������� ���������
#define RDS_LNLINE      1       // ������ �����
#define RDS_LNBEZIER    2       // ������
// ��������� �������� ����� �����, ����������� �������� GetLineDescription
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  int LineType;         // ��� ����� (RDS_LN*)
  int nPoint1,nPoint2;  // ������ ����������� �����

  int x1,y1;            // ���������� ���������� ����� nPoint1
  int x2,y2;            // ���������� ���������� ����� nPoint2

  int dx1,dy1;          // �������� ����������� ����� ��� nPoint1 ��� LineType==RDS_LNBEZIER
  int dx2,dy2;          // �������� ����������� ����� ��� nPoint2 ��� LineType==RDS_LNBEZIER

  RDS_CHANDLE Owner;    // �����-�������� �����
} RDS_LINEDESCRIPTION;
typedef RDS_LINEDESCRIPTION *RDS_PLINEDESCRIPTION;
//---------------------------------------------------------------------------

// ����� ���������� �����, ������������ ������������� ���������
#define RDS_VARFLAG_INPUT       1       // ���� (�� ������������� ������ � RDS_VARFALG_OUTPUT)
#define RDS_VARFLAG_OUTPUT      2       // ����� (�� ������������� ������ � RDS_VARFALG_INPUT)
#define RDS_VARFLAG_RUN         4       // ��� ������������ ����� ����������� ������
#define RDS_VARFLAG_MENU        8       // ������������ � ���� ������������� ������/�������
#define RDS_VARFLAG_SHOWNAME    16      // ����� ����� �������� �� ���������
#define RDS_VARFLAG_ONEINDEX    32      // ������������� ������ (������ ��� ��������)
// �������������� ����� - �� ��������� ����������, � ������������ ��� �� ��������
// ������������� ���������
#define RDS_VARFLAG_EXT_CHGNAME 0x10000 // �������� ��� ����������� ����������, ���� �����
                                        // ��� ����

// ��������� �������� ���������� �����
// ����������� �������� ������, ������ ������� ���������� � ���������
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  char Type;            // �������������� ��� ���������� (RDS_VARTYPE_*)
  BYTE Res1,Res2,Res3;  // ��������������� ��� ������������ **** REV2 ****
  DWORD Flags;          // ����� ������� ������ (RDS_VARFLAG_*)
  LPSTR Name;           // ��� ����������
  LPSTR StructType;     // ��� ���� (������ ��� ��������, ��� ��������� - NULL)
  int StructFields;     // ����� ����� (������ ��� ��������)
  int DataSize;         // ������ ����� ������
  RDS_VHANDLE Var;      // ���������� �������������
  int Rank;             // ������� ����������
  LPSTR Remark;         // ����������� ���������� (������ ������, ���� ��� ���) **** REV2 ****
} RDS_VARDESCRIPTION;
typedef RDS_VARDESCRIPTION *RDS_PVARDESCRIPTION;
//---------------------------------------------------------------------------

// ��������� �������� ���������� ��������� ����������, ����������� ��������
// rdsGetEditorParameters
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  int GridDx,GridDy;    // ��� ����� �� ����������� � ���������
  BOOL SnapToGrid;      // �������� �� �������� � �����
  BOOL DisplayGrid;     // ������������ �� �����
  COLORREF GridColor;   // ���� ����� �����

  BOOL Visible;         // ���� ��������� �������
  int WinLeft,WinTop,WinWidth,WinHeight; // ������� � ��������� ����
  int WorkWidth,WorkHeight;   // ������� ������� �������
  int ScrollX,ScrollY;  // ������� ScrollBar'��
  DWORD Reserved;       // ��������������� ��� ������������ **** REV2 ****
  double Zoom;          // �������

  COLORREF MainPointColor; // ���� ����� �������� ������
  BOOL ShowBlockNames;  // ���������� ����� ������
  BOOL ShowVarNames;    // ���������� ����� ����������

  int NumLayers;        // ����� �����
  int CurLayerNum;      // ����� �������� ���� (0...NumLayers-1)
  int CurLayerId;       // Id �������� ����
  int NumConfigs;       // ���� ������������ �����
  int CurConfig;        // ����� ������� ������������ (0...NumConfigs-1)

  BOOL PrintZoneActive; // ���� ������ ��������
  BOOL DisplayPrintZone;// ���� ������ ������������
  int PZLeft,PZTop,PZWidth,PZHeight; // ������� � ��������� ���� ������

  BOOL WinMaximized;    // ���� ���������� �� ���� �����
  BOOL WinMinimized;    // ���� ��������

  DWORD RefreshDelay;   // �������� ��������������� ���������� ���� � ������ �������,
                        // ��� 0, ���� ���������� ���������

  COLORREF BlockNameColor;      // ���� ���� ������
  int BlockNameDistance;        // ���������� �� ����� �� ����� �� ���������

  COLORREF BackgroundColor;     // ���� ���� ����
  BOOL DefBackground;           // � �������� ����� ���� ���� ������ ���� ���� Windows

  DWORD WindowReactions;        // ������� ����� ������� ���� �� "����" � ����������, �� ������������� �������:
                                // RDS_BDF_MOUSEEVENTS, RDS_BDF_FREEMOUSEMOVE, RDS_BDF_KBDEVENTS
  BOOL Wallpaper;       // �������� ����
  BOOL WallpaperTile;   // ������ ��������� ��� ������� ����
  int WallpaperWidth,WallpaperHeight; // ������� �����

  BOOL Dashboard;       // � ������� ������ ����������� ����������������� ����� (����������� ����������)

} RDS_EDITORPARAMETERS;
typedef RDS_EDITORPARAMETERS *RDS_PEDITORPARAMETERS;
//---------------------------------------------------------------------------

// ��������� ��� ������� rdsGetEditorFont
#define RDS_GEF_BLOCKNAME       0       // ����� ���� ������
#define RDS_GEF_VARNAME         1       // ����� ���� ����������

// ��������� �������� ������� ��������� ����������, ������������ ���������
// rdsGetEditorToolBars � rdsSetEditorToolBars
// ��� ������ ��������, ������ ��� ������� ������������
// rdsGetEditorWindowFlags � rdsSetEditorWindowFlags
typedef struct
{ BOOL LayersBar;       // ��������� ������ ���� � ������������
  BOOL ZoomBar;         // ��������� ������ ��������
  BOOL DisplayBar;      // ��������� ������ ����������� (����� � �.�)
  BOOL PrintBar;        // ��������� ������ ������
  BOOL StatusBar;       // ��������� ������ ���������
} RDS_EDITORTOOLBARS;
typedef RDS_EDITORTOOLBARS *RDS_PEDITORTOOLBARS;
//---------------------------------------------------------------------------

// ������ ������ ��������������� ��������
#define RDS_TIMERM_DELETE       0       // ������� ������ ����� ������������
#define RDS_TIMERM_STOP         1       // ������ ���������������, �� �� ���������
#define RDS_TIMERM_LOOP         2       // ������ ������������� ��������� ������
// ������� ������������ ��������
#define RDS_TIMERS_SIGNAL    0x10       // ��� ������������ ��������� ������ �������
#define RDS_TIMERS_TIMER     0x20       // ��� ������������ ������� ���� � RDS_BFM_TIMER
#define RDS_TIMERS_WINREF    0x30       // ��� ������������ ������� ���� � RDS_BFM_WINREFRESH
#define RDS_TIMERS_SYSTIMER  0x40       // ��� ������������ ������� ���� � RDS_BFM_TIMER
                                        // ���� � ��� ������, ���� ������� �������������
                                        // ���������� ��� ������� ����� ��������������
// ����� �������
#define RDS_TIMERF_FIXFREQ   0x100      // ��� RDS_BFM_WINREFRESH: �� ��������� ����������
                                        // ������� � ����� �������
// ����� ��������
#define RDS_TIMERMASK_M      0x0f       // ����� ������
#define RDS_TIMERMASK_S      0xf0       // ������ ������������
#define RDS_TIMERMASK_F    0xff00       // �����

// ��������� ���������� ���������������� �������
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  DWORD Delay;          // �������� ������� (��)
  DWORD StartTime;      // ����� ������ ������� (��)
  BOOL On;              // ������ �������
  DWORD Mode;           // ����� (RDS_TIMERM_* | RDS_TIMERS_* | RDS_TIMERF_*)
} RDS_TIMERDESCRIPTION;
typedef RDS_TIMERDESCRIPTION *RDS_PTIMERDESCRIPTION;
//---------------------------------------------------------------------------

// �������������������� ������ ��� ������� ������ � ������������� �����������
#define RDS_DVSELF      0       // ���������� � ����������� �����
#define RDS_DVPARENT    1       // ���������� � ������������ ����������
#define RDS_DVROOT      2       // ���������� � ������� ����������
// ��������� ���������� �������� �� ������������ ����������
typedef struct
{ LPVOID Data;          // ����� ������� ������ ���������� ��� NULL
  LPSTR  VarName;       // ��� ���������� (������ ������)
  LPSTR  VarType;       // ��� ���������� (������ ������)
  RDS_BHANDLE Provider; // ����, �������� ����������� ������ ����������
  LPVOID UID;           // ���������� ������������� ���������� � ����� Provider (���������,
                        // �������� ������)
  RDS_VHANDLE Var;      // ������������� ���������� ��� ������������� � ��������� ��������
} RDS_DYNVARLINK;
typedef RDS_DYNVARLINK *RDS_PDYNVARLINK;
//---------------------------------------------------------------------------

// ��������� ���������� �������� �� ������� �����
typedef struct
{ RDS_BHANDLE Block;    // ����-���������
  int FuncId;           // �������
} RDS_FUNCPROVIDERLINK;
typedef RDS_FUNCPROVIDERLINK *RDS_PFUNCPROVIDERLINK;
// ������: ��������� ��������
#define RDS_FUNCPROVIDERLINK_SUCCESS(pLink) ( ((pLink)!=NULL) && (((RDS_PFUNCPROVIDERLINK)(pLink))->Block!=NULL) )
//---------------------------------------------------------------------------

// �������������������� ������ ��� ������� ������ � ����������� ������� **** REV2 ****
#define RDS_SMSELF      0       // ���������� � ����������� �����
#define RDS_SMPARENT    1       // ���������� � ������������ ����������
#define RDS_SMROOT      2       // ���������� � ������� ����������
// ��������� ���������� ����������� ������� ������� ������ **** REV2 ****
typedef struct
{ BOOL RdsVar;          // TRUE - ���������� RDS, FALSE - ������ ������� ������
  LPVOID Data;          // ��������� �� ������� ������ ��� ����� ������� ������ ���������� RDS
  LPSTR Name;           // ��� ������� ������ (������ ������)
  LPSTR VarType;        // ��� ���������� RDS (��� ���������� RDS, ������ ������)
  RDS_VHANDLE Var;      // ������������� ���������� ��� ������������� � ��������� ��������
  DWORD Size;           // ������ ������� ������ (������ ��� ������� �������� ������)
  RDS_BHANDLE Block;    // ����, � ������� ������� ������� ������/����������

} RDS_SHAREDMEMPARAMS;
typedef RDS_SHAREDMEMPARAMS *RDS_PSHAREDMEMPARAMS;
//---------------------------------------------------------------------------

// ��������� �������� ������ (������������ � ��������� ��������)
#define RDS_SERVFONTPARAMSNAMESIZE      256     // ������ ������� ��� ��� ������
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  char Name[RDS_SERVFONTPARAMSNAMESIZE];        // ��� ������
  int CharSet;          // ����� ��������
  int Height;           // ������ (� ��������)
  int Size;             // ������ (� ������������ ������)
  BOOL SizePriority;    // ������ ��� ��������� ������ �� ���� ���������:
                        // TRUE - ������������ Size, FALSE - ������������ Height
  COLORREF Color;       // ���� ������
  BOOL Bold;            // ������
  BOOL Italic;          // ������
  BOOL Underline;       // ������������
  BOOL StrikeOut;       // �����������
} RDS_SERVFONTPARAMS;
typedef RDS_SERVFONTPARAMS *RDS_PSERVFONTPARAMS;
//---------------------------------------------------------------------------

// ����� ��� ����������� ��������� �������
#define RDS_GFALL               0    // ��� ��������� (��� ���� ��� ������ ������������ RDS_GFSIZE)
#define RDS_GFSTYLE             1    // ����� ����� ��� �������
#define RDS_GFWIDTH             2    // ������� �����
#define RDS_GFCOLOR             4    // ���� (�����, ������, �������)
#define RDS_GFMODE              8    // ����� ��������� ����� (WinAPI SetROP2)
// ����� - ������ ��� ������
#define RDS_GFNAME             16    // ��� ������
#define RDS_GFSIZE             32    // ������ ������ � ���.�� (points)
#define RDS_GFHEIGHT           64    // ������ ������ � ������
#define RDS_GFCHARSET         128    // ����� ��������
#define RDS_GFESCAPEMENT      256    // ���� �������� (� ��������)
#define RDS_GFBOLD           1024    // ����������
#define RDS_GFITALIC         2048    // ������
#define RDS_GFUNDERLINE      4096    // ������������
#define RDS_GFSTRIKEOUT      8192    // �����������
#define RDS_GFESCAPEMENT10  16384    // ���� �������� (� ������� ����� �������) **** REV2 ****
#define RDS_GFFONTSTYLES     (1024+2048+4096+8192) // ��� �����
#define RDS_GFFONTALLHEIGHT  (4+16+64+128+256+1024+2048+4096+8192) // ��� ��������� � ������
                                                                   // GF_HEIGHT
#define RDS_GFFONTALLHE10    (4+16+64+128+16384+1024+2048+4096+8192) // ��� ���������, ������
                                                                   // GF_HEIGHT � ���� � ������� ������� **** REV2 ****
#define RDS_GFFONTBASIC      (4+16+64+128+1024+2048+4096+8192) // ��� ����� � ������ ��� ��������

// ����� ������� ��� ����������� ��������� �������
#define RDS_GFS_EMPTY           0    // ��� �������
#define RDS_GFS_SOLID           1    // ��������
#define RDS_GFS_BDIAGONAL       2    // 45-degree downward left-to-right hatch
#define RDS_GFS_CROSS           3    // Horizontal and vertical crosshatch
#define RDS_GFS_DIAGCROSS       4    // 45-degree crosshatch
#define RDS_GFS_FDIAGONAL       5    // 45-degree upward left-to-right hatch
#define RDS_GFS_HORIZONTAL      6    // Horizontal hatch
#define RDS_GFS_VERTICAL        7    // Vertical hatch

// ���������, ������������ �������� rdsAltConnAppearanceOp
#define RDS_CAOSET              0    // ���������� ��������� ��������������� ���� �����
#define RDS_CAOCOUNT            1    // ������� ����� �������������� �����
#define RDS_CAOPREALLOCATE      2    // ������� ������ ��������� �������
#define RDS_CAOGET              3    // �������� ��������� ��������������� ����
#define RDS_CAOSETCURRENT       4    // ���������� ������� ��� �� ���������������
#define RDS_CAODELETE           5    // ������� �������������� ���
#define RDS_CAORESTORE          6    // ������������ ���, �������� �������   

// ���������, ������������ �������� rdsGetSystemPath
#define RDS_GSPSYSTEMFILE       0    // ����������� ���� ������� (� �����, ���� ����� ��
                                     // ������ ��� �������)
#define RDS_GSPAPPPATH          1    // ���� � EXE-����� RDS ��� ����� �����
#define RDS_GSPDLLPATH          2    // ���������� ����������� Dll
#define RDS_GSPAPPEXE           3    // ���� � EXE-����� RDS � ������ �����
#define RDS_GSPTEMPLATEPATH     4    // ���� � �������� ����
#define RDS_GSPBLOCKLIBPATH     5    // ���� � ���������� ������
#define RDS_GSPBLOCKPANELPATH   6    // ���� � ������ ������
#define RDS_GSPTEMPPATH         7    // ���� � ��������� ����������
#define RDS_GSPINCLUDEPATH      8    // ���� � ���������� ������������ ������
#define RDS_GSPSYSTEMFULLPATH   9    // ����������� ���� ����� � �����. ���� ���� ��� ���
                                     // (������ ��� �������), ������������ ������ ������
#define RDS_GSPINIPATH         10    // ���� � ���������� ini-������ (������ ��������� � RDS_GSPAPPPATH)
#define RDS_GSPDEFAULTHOST     11    // ��� ������� �� ��������� ��� ������� ����������
#define RDS_GSPMODELSPATH      12    // ���������� ��� ����������������� �������
#define RDS_GSPBAKFILEEXT      13    // ���������� ������ ��������� ����� ����
#define RDS_GSPDOCPATH         14    // ���������� ������������ **** REV2 ****
#define RDS_GSPSYSTEMFULLDIR   15    // ���������� ������������ ����� ����� ��� ������ ������ **** REV2 ****

// ���������, ������������ �������� rdsGetSystemInt
#define RDS_GSICMDPARAMCOUNT    0    // ����� �������������� ���������� ��������� ������
#define RDS_GSIINSTSTRUCTCOUNT  1    // ����� �������� � ����� ������
#define RDS_GSISAVELOADACTION   2    // ��������, ��������� �������� ��� ������ �����
                                     // (���� �� �������� RDS_LS_*)
#define RDS_GSIMODIFIED         3    // 1 - ������� ��������, 0 - �� ��������
#define RDS_GSITICKPARITY       4    // 0 - ������ ����, 1 - ��������
#define RDS_GSIDEFAULTPORT      5    // ���� �� ��������� ��� ������� ����������
#define RDS_GSIUNDOSIZE         6    // ����� ����� undo (0 - ���������)
#define RDS_GSIUIENABLED        7    // �������� �� ���������������� ��������� (0 - ���� RDS �������,
                                     // ������ ����� ���� ������ � ������ ����������)
#define RDS_GSISTOPPING         8    // 1 - � ������ ������ ��������������� ������, ����� - 0
#define RDS_GSITRACELEVEL       9    // ������� ������� ����������� ������� (0 - ��������)
#define RDS_GSIBAKFILESCOUNT   10    // ����� ����������� ��� ���������� ����� ��������� �����
#define RDS_GSIMODE            11    // ������� ����� RDS (���� �� �������� RDS_CM_*) **** REV2 ****
#define RDS_GSIMAINHELP        12    // ������� �������� ����� ������� RDS (1 - ��,0 - ���) **** REV2 ****
#define RDS_GSIINVALIDPATHS    13    // ��� rdsGetSystemInt(RDS_GSISAVELOADACTION)==RDS_LS_LOADPROG -
                                     // ������� �� ������������ ��� ���-�� �������������� ���� � ���������� ������ - ��������, ����
                                     // ������ ��� �������� ������ �� ���� (1-������������,0-���) **** REV2 ****
#define RDS_GSIUNDOACTION      14    // �������� undo, ��������� �������� ��� ���������� ����� (RDS_UA_*) **** REV2 ****

// ����� RDS, ������������ rdsGetSystemInt(RDS_GSIMODE)  **** REV2 ****
#define RDS_CM_EDIT     0       // ����� ��������������
#define RDS_CM_CALC     1       // ����� �������������
#define RDS_CM_RUN      2       // ����� �������

// ���������, ������������ �������� rdsSetSystemInt
#define RDS_SSIFASTTEXTSAVE     0    // ������� ���������� � ��������� ���� (��� ���.���������)
                                     // ���������� � ������� RDS_BFM_SAVETXT, ����� ������������� ������������)
#define RDS_SSIWAITCURSOR       1    // ���������� ��� ������� "�������� ����" ��� ������� ����������
#define RDS_SSILEAVEEDITSTATUS  2    // ���������� ������������� ���������/�������� ����� �� ������ �������������� **** REV2 ****
#define RDS_SSICANCELUNDOSTORE  3    // �� ���������� ��������, ������������� ������, � undo **** REV2 ****

// ���������, ������������ �������� rdsBlockStateOp
#define RDS_BSOSAVE             0    // ��������� ���������
#define RDS_BSOCOUNT            1    // ������� ����� ����������� ���������
#define RDS_BSORESTORE          2    // ������������ ���������
#define RDS_BSODELETE           3    // ������� ���������

// ���� �������, ������������ �������� rdsCallerThreadType
#define RDS_THREADMAIN  0       // ������� �����
#define RDS_THREADAUX   1       // ��������������� ����� �������������

// ��������� ������� ��� ��������� � �������� DWORD-������
#define RDS_GETFLAG(storage,mask) (((storage) & (mask))!=0)
#define RDS_SETFLAG(storage,mask,value) ((storage) = (value)?((storage) | (mask)):((DWORD)((storage) & (~(mask)))))
//---------------------------------------------------------------------------

// ��������� ������� ��� ������ � �����������-���������
// ��������� ������������� �������
//   baseaddr - ������� ����� ������� (void*)
#define RDS_ARRAYEXISTS(baseaddr) (*((void**)(baseaddr))!=NULL)
// ����� ����� (������ ������) ������� (������ ������ ������������)
#define RDS_ARRAYROWS(baseaddr)  (*(*((int**)(baseaddr))))
// ����� �������� (������ ������) ������� (������ ������ ������������)
#define RDS_ARRAYCOLS(baseaddr)  ( *( *((int**)(baseaddr))+1 ) )
// ������ ������� ������ ������� (������ ������ ������������)
// ������� ������: M[r,c]
//                 r0c0   r0c1   r0c2   ... r0cN   r1c0   r1c1   ...
//                 M[0,0] M[0,1] M[0,2] ... M[0,N] M[1,0] M[1,1] ...
#define RDS_ARRAYDATA(baseaddr)  ( *((int**)(baseaddr))+2 )
// ��������� ��� �������� ������� � �������
typedef struct
{ BOOL Exists;          // TRUE - ������ ���������� (�.�. �� 0 x 0)
  int Rows,Cols;        // ����������� �������
  int ItemSize;         // ������ �������� �������
  LPVOID Data;          // ������ ������� ������ �������
} RDS_ARRAYACCESSDATA;
typedef RDS_ARRAYACCESSDATA *RDS_PARRAYACCESSDATA;
// ����� �������� ������� �� ��������� ������� (������ ������ ������������)
//   acc - RDS_PARRAYACCESSDATA �������
//   r,c - ������ � ������ �������
#define RDS_ARRAYITEMADDR(acc,r,c) ( ((char*)((acc)->Data)) + ((r)*((acc)->Cols)+(c))*((acc)->ItemSize) )
// �� �� ��� ����������� �������
#define RDS_ARRAYITEMPLAINADDR(acc,n) ( ((char*)((acc)->Data)) + (n)*((acc)->ItemSize) )
// ������� ������� �� ��������� �������
// type - ��� �������� ������� (��������, double)
#define RDS_ARRAYITEM(type,acc,r,c) ( *((type *)RDS_ARRAYITEMADDR(acc,r,c)) )
//---------------------------------------------------------------------------

// ����� ��� ������� rdsEnumConnectedBlocks
#define RDS_BEN_INPUTS          1       // ������������� ����� �����
#define RDS_BEN_OUTPUTS         2       // ������������� ������ �����
#define RDS_BEN_TRACELINKS      4       // ������������ ����� ������ � ������ ��������� �� �������
                                        // ������
#define RDS_BEN_USEDISABLED     8       // ���� ����� ���������, ��� ����� ������������ �� **** REV2 ****
//---------------------------------------------------------------------------

// �����, ������������ ��������� rdsBroadcastFunctionCallsEx, rdsBroadcastFuncCallsDelayed
#define RDS_BCALL_SUBSYSTEMS    1    // �������� ��� ��������� ����� ���� ��������� ���������
#define RDS_BCALL_CHECKSUPPORT  2    // �������� ������� ������ ���� ���������� RDS_BFM_CHECKFUNCSUPPORT
#define RDS_BCALL_ALLOWSTOP     4    // ��������� ����� ���������� ���������� ������
#define RDS_BCALL_FIRST         8    // ��� ���������� ������ ���������� � ������ �������
#define RDS_BCALL_LAST          0    // ��� ���������� ������ ���������� � ����� ������� (������ ���
                                     // ��������� ����������, ����� �� ���������)
//---------------------------------------------------------------------------

// ����� ��� ������� rdsSetPointPosition
#define RDS_SPP_RELATIVE        1       // �������� ���������� ������������ ����� ��������
#define RDS_SPP_REFRESH         2       // �������� ���� ����� ��������� ��������� �����
//---------------------------------------------------------------------------

// ������������ ����������� ����� ���������� (�� ��� ����������� ������ ��� RDS_BFM_VARCHECK)
// �������
#define RDS_VARTYPE_SIGNAL      'S'     // ������ (1 ���� - char)
#define RDS_VARTYPE_LOGICAL     'L'     // ���������� (1 ���� - char)
#define RDS_VARTYPE_CHAR        'C'     // ����� (1 ���� - char)
#define RDS_VARTYPE_SHORT       'H'     // ����� (2 ����� - short int)
#define RDS_VARTYPE_INT         'I'     // ����� (4 ����� - int)
#define RDS_VARTYPE_FLOAT       'F'     // � ��������� ������ (4 ����� - float)
#define RDS_VARTYPE_DOUBLE      'D'     // � ��������� ������ (8 ���� - double)
// �������
#define RDS_VARTYPE_STRING      'A'     // ������
#define RDS_VARTYPE_RUNTIME     'V'     // ���������� ���
#define RDS_VARTYPE_ARRAY       'M'     // ������/�������
#define RDS_VARTYPE_STRUCT      '{'     // ���������
#define RDS_VARTYPE_STRUCTEND   '}'     // ����� ���������
//---------------------------------------------------------------------------

// ����� ��� ������� rdsRegisterMenuItem, rdsChangeMenuItem
#define RDS_MENU_SHORTCUT       1       // ���� "������� �������"
#define RDS_MENU_UNIQUECAPTION  2       // �� ��������������, ���� ��� ���� ����� ����
                                        // � ����� ���������
#define RDS_MENU_CHECKED        4       // ���� "�������"
#define RDS_MENU_DISABLED       8       // ����� ���� ��������
#define RDS_MENU_HIDDEN        16       // ����� ���� �������
#define RDS_MENU_DIVIDER       32       // ����������� ������ ������ ���� (������ ��� ������������,
                                        // �� ������������� � ������� �������)
#define RDS_MENU_SEARCH        64       // ���������� �� Ctrl+F ������ ������������ ������ **** REV2 ****
//---------------------------------------------------------------------------

// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsCECreateEditor (������ ��� �������������� ����� ��� ����)
#define RDS_HCE_RESET           0       // �������� ��������
// ���� ������ ������� rdsCECreateConnBus
#define RDS_HCE_ERR_OK          0       // ��� ������
#define RDS_HCE_ERR_BADOBJECT   1       // �������� ������� �� �������� �������� rdsCECreateEditor
#define RDS_HCE_ERR_INVBLKBUS   2       // ���� �� ������ ��� ���� �� ��� ��� ������ �����������
#define RDS_HCE_ERR_BADLINE     3       // ���� �� ����� ��������� �� �������������� �����
#define RDS_HCE_ERR_ALLOC       4       // ������ ��������� ������
//---------------------------------------------------------------------------

// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsBCLCreateList (������ ������ � ������)
// ������� ��������� �������� (������� rdsGetObjectArray, ������� rdsBCLGetBlockArray � rdsBCLGetConnArray)
#define RDS_HBCL_BLOCKARRAY     0       // �������� � �������� ������
#define RDS_HBCL_CONNARRAY      1       // �������� � �������� ������ � ���
// ������� ��������� � ��������� ����� � ���������� ���������� (������� rdsSetObjectInt, rdsGetObjectInt)
#define RDS_HBCL_AUTODELETE     0       // ������������� �������� � ������� �������
                                        // NULL, ���� ���� ��� ����� ���� �������
#define RDS_HBCL_BLOCKCOUNT     1       // ����� ������ � ������ (������ ���������)
#define RDS_HBCL_CONNCOUNT      2       // ����� ������ � ������ (������ ���������)
// ������� �������
#define RDS_HBCL_CLEAR          3       // �������� ������
// ����� ������ ������� ��������� ���������
#define RDS_GS_DISABLEVARCHANGE         1       // ������ ��������� ���������� �����
#define RDS_GS_DISABLEVARVALUES         2       // ������ ���������� ����������
#define RDS_GS_DISABLEEDITORPARAMS      4       // ������ ��������� ���������� ���������
#define RDS_GS_DISABLEREMARKS           8       // ������ ��������� ������������ ������
#define RDS_GS_DISABLELAYERCHANGE      16       // ������ ��������� ����
#define RDS_GS_DISABLEDBLCLICK         32       // ������ ��������� ������� �� ������� ������
#define RDS_GS_DISABLEPICTURE          64       // ������ ��������� �������� � �� ����������
#define RDS_GS_DISABLEDRAWTYPE        128       // ������ ��������� ���� ��������� (DLL/��������)
#define RDS_GS_DISABLEBLOCKAPPEARANCE 256       // ������ ��������� ���������� �������� ����
                                                // (����������� �����, ����� �������� � �.�.)
#define RDS_GS_DISABLESIZING          512       // ������ ��������� ���������� ���������������
#define RDS_GS_DISABLECONNAPPEARANCE 1024       // ������ ��������� �������� ���� �����
#define RDS_GS_DISABLECONNSTATE      2048       // ������ ���������/���������� �����
#define RDS_GS_DISABLEBUSPACK        4096       // ������ �������� ������ ������� ���
#define RDS_GS_DISABLEDLLFUN         8192       // ������ ��������� ������� �������
#define RDS_GS_DISABLEDLLOPTIONS    16384       // ������ ��������� ������ ������� ������� (����)
#define RDS_GS_DISABLEAUTOCOMP      32768       // ������ ��������� ���������� ��������������
#define RDS_GS_DISABLEMOVING        65536       // ������ ��������� ���������� ����������� **** REV2 ****
#define RDS_GS_DISABLEEXTCONNPARAMS 0x20000     // ������ ��������� �������������� ���������� ����� **** REV2 ****
#define RDS_GS_DISABLECLASSES       0x40000     // ������ ��������� ������� **** REV2 ****
//---------------------------------------------------------------------------

// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsVSCreateEditor (����� ����������)
// ����� ���������
#define RDS_HVAR_FSTRUCT        1       // ��������� ������������� ��������
#define RDS_HVAR_FARRAYS        2       // ��������� ������������� �������� � ������
#define RDS_HVAR_F1INDEX        4       // ��������� ������������� �������� � ����� ��������
#define RDS_HVAR_FRUNTIME       8       // ��������� ������������� ���������� �����
#define RDS_HVAR_FARROFSTRUCT  16       // ��������� ������������� �������� ��������
#define RDS_HVAR_FINT          32       // ��������� ������������� int
#define RDS_HVAR_FDOUBLE       64       // ��������� ������������� double
#define RDS_HVAR_FCHAR        128       // ��������� ������������� char
#define RDS_HVAR_FSHORT       256       // ��������� ������������� short int
#define RDS_HVAR_FFLOAT       512       // ��������� ������������� float
#define RDS_HVAR_FLOGICAL    1024       // ��������� ������������� ������������ ����������
#define RDS_HVAR_FSIGNAL     2048       // ��������� ������������� ��������
#define RDS_HVAR_FSTRING     4096       // ��������� ������������� �����
#define RDS_HVAR_FNOSTRUCTNAME 8192     // ��� �������������� ��������� - �� ���������� ���
#define RDS_HVAR_FNOOFFSET  16384       // �� ���������� ������� ��������
#define RDS_HVAR_FNOREMARKS 32768       // ��������� ����������� ���������� **** REV2 ****
#define RDS_HVAR_FALLPLAIN   (32+64+128+256+512+1024+2048) // ��� ������� ����
#define RDS_HVAR_FALLPLAINNS (32+64+128+256+512+1024) // ��� ������� ���� ��� ��������
#define RDS_HVAR_FALL        (1+2+4+8+16+RDS_HVAR_FALLPLAIN+4096) // ��� ����
#define RDS_HVAR_FALLNS      (1+2+4+8+16+RDS_HVAR_FALLPLAINNS+4096) // ��� ���� ��� ��������
// ����������
#define RDS_HVAR_ROK            0       // �������� ��������� �������
#define RDS_HVAR_ROKRENAMED     1       // �������� ��������� �������, �� ��������� �������������
#define RDS_HVAR_RNOTYPENAME    2       // ��� �������� ���������, ����� � ���������
                                        // ���� ��� ����
#define RDS_HVAR_RVARCHECKERR   3       // ���� �� ����� �������� � ����� ����������
#define RDS_HVAR_REMPTYVARSET   4       // ����� ���������� ����
#define RDS_HVAR_RBADBLOCKTYPE  5       // ������������ ��� �����
#define RDS_HVAR_RNOBLKSIGNALS  6       // ����� ���������� �� ����� ���� �������� ����� �.�.
                                        // � ��� ��� ���� �������� � ������
// ������� (������� ������ �����)
#define RDS_HVAR_GETFIELDCOUNT  0       // �������� ����� ����� ���������
#define RDS_HVAR_GETAUTOCOUNT   1       // �������� ����� �������������� ����������
#define RDS_HVAR_GETVARRANK     2       // �������� ������� ����������
#define RDS_HVAR_GETVARFLAGS    12      // �������� ����� ����������
// ������� (������� ������)
#define RDS_HVAR_GETTYPENAME    3       // �������� ������ ���� ���������
#define RDS_HVAR_GETAUTOMAIN    4       // �������� ��� ������� ���������� ��������������
                                        // � ��������� �������
#define RDS_HVAR_GETAUTOCONN    5       // �������� ��� ��������� ���������� ��������������
                                        // � ��������� �������
#define RDS_HVAR_GETTYPESTRING  13      // �������� ������ ���� ���� ���������                                        
#define RDS_HVAR_GETREMARK      14      // �������� ����������� ���������� � ��������� ������� **** REV2 ****
// ������� (��������� ������ �����)
#define RDS_HVAR_SETVARFLAGS    RDS_HVAR_GETVARFLAGS    // ���������� ����� ����������
// ������� (��������� ������)
#define RDS_HVAR_SETTYPENAME    6       // ���������� ������ ���� ���������
#define RDS_HVAR_SETREMARK      15      // ���������� ����������� ���������� � ��������� ������� **** REV2 ****
// ������� (��� ��������)
#define RDS_HVAR_RESET          7       // �������� ��������
#define RDS_HVAR_DELVAR         8       // ������� ����������
#define RDS_HVAR_DELAUTO        9       // ������� ��������������
#define RDS_HVAR_CLEARTYPEREN   10      // �������� ������ �������������� �����
#define RDS_HVAR_CLEARENAMES    RDS_HVAR_CLEARTYPEREN      // �������
#define RDS_HVAR_CLEARVARREN    11      // �������� ������ �������������� ����������
//---------------------------------------------------------------------------

// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsSTRCreateTextReader (������� ������ ���� ������)
#define RDS_HSTR_DEFUNKNOWNWORD         -1      // ������������� ������������� ����� �� ���������
#define RDS_HSTR_DEFENDOFLINE           -2      // ������������� ����� ������ �� ���������
#define RDS_HSTR_DEFENDOFTEXT           -3      // ������������� ����� ������ �� ���������
// ������� ��������� � ��������� ����� � ���������� ���������� (������� rdsSetObjectInt, rdsGetObjectInt)
#define RDS_HSTR_IGNORECASE     0       // ��������� (1) ��� �� ��������� (0) ������� ��� �������
#define RDS_HSTR_UNKNOWNID      1       // ������������� ������������� �����
#define RDS_HSTR_ENDOFLINEID    2       // ������������� ����� ������
#define RDS_HSTR_ENDOFTEXTID    3       // ������������� ����� ������
#define RDS_HSTR_READINT        7       // ������� �� ������ ����� ����� (������ ���������)
// ������� ��������� ������
#define RDS_HSTR_SETTEXT        4       // ���������� ����� ��� �������
// ������� ��������� ������
#define RDS_HSTR_GETLASTWORD    5       // �������� ��������� ��������� �����
#define RDS_HSTR_GETRESTOFTEXT  6       // �������� ��������� �� ������� ������ ����� ������
// ������� �������� ������������ �����
#define RDS_HSTR_READDOUBLE     8       // ������� �� ������ ����� double

// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsINICreateTextHolder (������ ������ ������� INI-�����)
// �������
#define RDS_HINI_RESET          0       // ������� ������
#define RDS_HINI_GETLASTERROR   4       // �������� ���� ������ ��������� ��������
#define RDS_HINI_SAVEBLOCKTEXT  7       // �������� ��� ��������� ������ �����
// ������� ��������� ������
#define RDS_HINI_SETTEXT        1       // ���������� �����
#define RDS_HINI_LOADFILE       2       // ��������� ����
#define RDS_HINI_SAVEFILE       3       // �������� ����
#define RDS_HINI_CREATESECTION  5       // ������� ������
#define RDS_HINI_DELETESECTION  6       // ������� ������
#define RDS_HINI_DELETEKEYLAST  8       // ������� �������� � ��������� �������� ������

// ��������� �������� ��������� �������, �������� ������� ����� ���� ��������
#define RDS_FRESULT_ERROR       0       // ������
#define RDS_FRESULT_OK          1       // ������� ���������
#define RDS_FRESULT_DELAYED     2       // ������� ��������

// �����, ������������ ��������� rdsGetEditorWindowFlags � rdsSetEditorWindowFlags
#define RDS_EWF_STATUSBAR       1       // ���� ������ ���������
#define RDS_EWF_LAYERSTOOLBAR   2       // ������ ������������ - ����
#define RDS_EWF_ZOOMTOOLBAR     4       // ������ ������������ - �������
#define RDS_EWF_DISPLAYTOOLBAR  8       // ������ ������������ - �����������
#define RDS_EWF_CALCTOOLBAR    16       // ������ ������������ - ������ � ������
#define RDS_EWF_PRINTTOOLBAR   32       // ������ ������������ - ������
#define RDS_EWF_NOSCROLLBARS   64       // ��� ����� ��������� **** REV2 ****
#define RDS_EWF_WINMOUSE      128       // ���� ��������� �� "����" ����� ������ **** REV2 ****
#define RDS_EWF_WINFREEMOUSE  256       // ���� ��������� �� ����������� "����" **** REV2 ****
                                        // � ����������� �������� (����� ������)
#define RDS_EWF_WINKEYBOARD   512       // ���� ��������� �� ���������� ����� ������ **** REV2 ****
// ����� � ������ ������ ��� ��� �� �������
#define RDS_EWF_ALLBARS        (1+2+4+8+16+32)  // ��� ������ � ������ ���������

// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsFORMCreate (������ ��� �������� ���������� ����)
// �������
#define RDS_FORM_CLEAR          0       // ��������
#define RDS_FORM_SHOWMODAL      1       // ��������
#define RDS_FORM_INVALIDATE     2       // ������������
#define RDS_FORM_CLOSE          3       // ������� (��� "OK" ��� "������") **** REV2 ****
#define RDS_FORM_TEMPCLOSE      4       // �������, ��������� ������� � ������� ����� **** REV2 ****
// ���� ����� ����� (������� ���� DWORD, ������������� � rdsFORMAddEdit)
#define RDS_FORMCTRL_EDIT       0       // ������� ���� �����
#define RDS_FORMCTRL_COMBOEDIT  1       // ���� ����� �� ������� ���������
#define RDS_FORMCTRL_COMBOLIST  2       // ������ ��������� ��� ����������� ��������������
#define RDS_FORMCTRL_CHECKBOX   3       // ���� (�� ����� ���������� � RDS_FORMFLAG_CHECK)
#define RDS_FORMCTRL_LABEL      4       // ������� ��� ����������� ��������������
#define RDS_FORMCTRL_COLOR      5       // ������ ������ �����
#define RDS_FORMCTRL_OPENDIALOG 6       // ���� ����� ��� ������ ����� (��������, � ��������)
#define RDS_FORMCTRL_SAVEDIALOG 7       // ���� ����� ��� ������ ����� (����������, � ��������)
#define RDS_FORMCTRL_UPDOWN     8       // ���� ����� �� ���������
#define RDS_FORMCTRL_DISPLAY    9       // ��������� �������� ��� ����������� ��������������
#define RDS_FORMCTRL_MULTILINE  10      // ���� ����� �� ���������� �����
#define RDS_FORMCTRL_LISTANDEDIT 11     // ���������� ������ � ���� �����
#define RDS_FORMCTRL_RANGEEDIT  12      // ��� ���� �����, ����������� "..."
#define RDS_FORMCTRL_NONVISUAL  13      // ������ �������� ������ (�� ������������ � ����)
#define RDS_FORMCTRL_PAINTBOX   14      // �������, �������� ��������� ��������
#define RDS_FORMCTRL_RADIOBUTTON 15     // ����, ����������� ������ (�� ����� ���������� � RDS_FORMFLAG_CHECK)
#define RDS_FORMCTRL_BUTTON     16      // ������
#define RDS_FORMCTRL_FONTSELECT 17      // ������ ������ ������
#define RDS_FORMCTRL_HOTKEY     18      // ���� ����� ��� ��������� ������
#define RDS_FORMCTRL_DIRDIALOG  19      // ���� ����� ��� ������ ����� (� ��������)
#define RDS_FORMCTRL_MULTILABEL 20      // ������������� ������� ��� ����������� �������������� **** REV2 ****
#define RDS_FORMCTRL_TABLE      21      // ������� **** REV2 ****
// ����� ����� (������� ����� DWORD, ������������� � rdsFORMAddEdit)
#define RDS_FORMFLAG_LINE       0x100   // ��������� ����� ��� �����
#define RDS_FORMFLAG_CHECK      0x200   // � �����������/����������� ������
#define RDS_FORMFLAG_DISABLED   0x400   // ���� ����� ��������� ������� (������ � ������, ���� �� ����)
#define RDS_FORMFLAG_LRIGHT     0x800   // ����� ���� ����� ��������� �� ������� ����
#define RDS_FORMFLAG_LCENTER    0x1000  // ����� ���� ����� ��������� �� ������
#define RDS_FORMFLAG_CHECKRADIO 0x2000  // �����������/����������� ���� - RadioButton (��� RDS_FORMFLAG_CHECK)
// ���� ������������ � ���������� �������� ��� ������� rdsSetObject* � rdsGetObject* ���
// ���������� �� �������� rdsFORMSet* � rdsFORMGet*.
#define RDS_FORMVAL_VALUE       0       // �������� ������ ���������� (��������� � ������)
#define RDS_FORMVAL_LIST        1       // ������ ��������� ��� RDS_FORMCTRL_COMBOEDIT �
                                        // RDS_FORMCTRL_COMBOLIST ��� ������ �������� ���
                                        // RDS_FORMCTRL_OPENDIALOG/RDS_FORMCTRL_SAVEDIALOG (������ ���������)
#define RDS_FORMVAL_CHECK       2       // �������� ������������/������������ ����� (��������� � ������)
#define RDS_FORMVAL_UPDOWNINC   3       // ��������� ��� RDS_FORMCTRL_UPDOWN (��������� � ������)
#define RDS_FORMVAL_UPDOWNMIN   4       // ������� ��� RDS_FORMCTRL_UPDOWN (��������� � ������)
#define RDS_FORMVAL_UPDOWNMAX   5       // �������� ��� RDS_FORMCTRL_UPDOWN (��������� � ������)
#define RDS_FORMVAL_MLHEIGHT    6       // ������ ��� RDS_FORMCTRL_MULTILINE � RDS_FORMCTRL_MULTILABEL **** REV2 **** (������ ���������)
#define RDS_FORMVAL_ENABLED     7       // ����� ������������� ������ ���������� (��������� � ������)
                                        //-------- ��� RDS_FORMCTRL_LISTANDEDIT ----------
#define RDS_FORMVAL_AUXLISTITEM 8       // ����� ������ ���������������� ������ ��� RDS_FORMCTRL_LISTANDEDIT(��������� � ������)
#define RDS_FORMVAL_AUXLISTWIDTH 9      // ������ ���������������� ������ ��� RDS_FORMCTRL_LISTANDEDIT(������ ���������)
#define RDS_FORMVAL_2NDEDITENABLED 10   // ������������� ������� ���� ����� ��� RDS_FORMCTRL_LISTANDEDIT
#define RDS_FORMVAL_ITEMINDEX   11      // ����� ���������� ������ ��� RDS_FORMCTRL_COMBOLIST (���� ���� ������������� ������,
                                        // ������������ ���������� ������������� ������, � �� �������� ���������� ������)
                                        //-------- ��� RDS_FORMCTRL_PAINTBOX ----------
#define RDS_FORMVAL_PBHEIGHT    RDS_FORMVAL_MLHEIGHT       // ������ ��� RDS_FORMCTRL_PAINTBOX (������ ���������)
#define RDS_FORMVAL_PBBEVEL     RDS_FORMVAL_AUXLISTWIDTH   // ������� ����� ��� RDS_FORMCTRL_PAINTBOX (������ ���������)
                                        //-------- ��� RDS_FORMCTRL_RANGEEDIT ----------
#define RDS_FORMVAL_RANGEMAX    RDS_FORMVAL_UPDOWNMAX  // �������� ��� RDS_FORMCTRL_RANGEEDIT (��������� � ������)
                                        //-------- ��� RDS_FORMCTRL_HOTKEY ----------
#define RDS_FORMVAL_HKSHIFTS    RDS_FORMVAL_MLHEIGHT    // ����� Ctrl,Shift,Alt
                                        //-------- RDS_FORMCTRL_MULTILINE ---------
#define RDS_FORMVAL_MLRETURNS   RDS_FORMVAL_AUXLISTITEM // ���������� ��������� ����� � ����
#define RDS_FORMVAL_MLREADONLY  RDS_FORMVAL_AUXLISTWIDTH // ������ ������ **** REV2 ****
#define RDS_FORMVAL_MLBEVEL     RDS_FORMVAL_2NDEDITENABLED // ��� ����� (0,1,2 - ������ ���������) **** REV2 ****
                                        //--------  RDS_FORMCTRL_MULTILABEL ---------  **** REV2 ****
#define RDS_FORMVAL_MLABELBEVEL  RDS_FORMVAL_AUXLISTWIDTH   // ������� ����� ��� RDS_FORMCTRL_MULTILABEL (������ ���������)  **** REV2 ****
                                        //--------  �������� ������� ---------  **** REV2 ****
#define RDS_FORMVAL_DEFPATH     12      // ���� �� ��������� ��� �������� �������� **** REV2 ****
                                        //--------  RDS_FORMCTRL_TABLE ---------  **** REV2 ****
                                        // (rdsSetObjectInt �, ���-���, rdsSetObjectStr)
#define RDS_FORMVAL_TABLEHEIGHT  13     // ������ ���� ������� (������ ���������)
#define RDS_FORMVAL_TBLBTN_ADD   14     // ��������� (rdsSetObjectInt) ��� ��������� (rdsSetObjectStr) ������ "��������" (������ ���������)
#define RDS_FORMVAL_TBLBTN_UP    15     // ��������� ��� ��������� ������ "�����" (��������� ������������� �������� � "����", ������ ���������)
#define RDS_FORMVAL_TBLBTN_DOWN  16     // ��������� ��� ��������� ������ "����" (��������� ������������� �������� � "�����", ������ ���������)
#define RDS_FORMVAL_TBLBTN_PROP  17     // ��������� ��� ��������� ������ "��������" (������ ���������)
#define RDS_FORMVAL_TBLBTN_DEL   18     // ��������� ��� ��������� ������ "�������" (������ ���������)
#define RDS_FORMVAL_TBLHEADERS   19     // �������/���������� ������ ��������� (���������/������)
#define RDS_FORMVAL_TBLFIXEDCOL  20     // �������/���������� �������������� �������� ������� (���������/������)
#define RDS_FORMVAL_TBLHDRHEIGHT 21     // ������ ������ ��������� (� ������ ������, ���������/������)
#define RDS_FORMVAL_TBLLINEHEIGHT 22    // ������ ����� ������ (� ������ ������, ���������/������)
#define RDS_FORMVAL_TBLCOUNT     23     // ����� ����� (������ ������)
#define RDS_FORMVAL_TBLDELLINE   24     // ������� ������ (������ ���������)
#define RDS_FORMVAL_TBLEMPTYTEXT 25     // �����, ��������� ��� ������ ������� ������ �����
#define RDS_FORMVAL_TBLREDRAW    26     // ������������ ������� (���������� �� ��������)
#define RDS_FORMVAL_TBLINSLINE   27     // �������� ������ (������ ���������)

// ������� ��� ������������� ��������� ������� ���������� ����
#define RDS_FORMSERVEVENT_CHANGE        0       // ��������� ������ ��� ���������� ����� �����
#define RDS_FORMSERVEVENT_DRAW          1       // ��������� (������ ��� ������ �������)
#define RDS_FORMSERVEVENT_CLICK         2       // ������� ������
#define RDS_FORMSERVEVENT_HELP          3       // ������� ������ �������  **** REV2 ****
#define RDS_FORMSERVEVENT_TCREACTION    4       // ������� ����� ���������� ��������  **** REV2 ****
#define RDS_FORMSERVEVENT_TCREOPEN      5       // �������� ����� ���������� ��������  **** REV2 ****
#define RDS_FORMSERVEVENT_TBLBUTTON     6       // ������� ������ �������  **** REV2 ****
#define RDS_FORMSERVEVENT_TBLBTNENABLE  7       // �������� ������������� ������ ���������� � �������� � ������� **** REV2 ****
// ����� ��� rdsFORMHelpButton  **** REV2 ****
#define RDS_FORMHELP_UI         1       // ������� ����� ���� UI (������� - ����������� ��� ���� �������)
#define RDS_FORMHELP_EVENT      2       // ��������� ������� � ������� ��������� ������ ������ ������ �������
#define RDS_FORMHELP_CHECK      4       // ��������� ������������� ����� ������� (���� �� ����� �������)
// ��������� ��� ������������� ��������� ������� ���������� ����
typedef struct
{ int Event;            // ������� (RDS_FORMSERVEVENT_*)
  int CtrlId;           // ������������� ������ ���������� (���� ��������� ��� ������� �������)

  // ��� RDS_FORMSERVEVENT_DRAW
  HDC dc;               // ��� �������� (��������, ��. WinAPI)
  int Left,Top;         // ������� ����� ���� ���� ���������
  int Width,Height;     // ������� ���� ���������

  // ������������� ��� ��������� �������  **** REV2 ****
  int TableRow,TableCol;// ������ �������, ���� ���� ��������� �������
                        // (��� ������ ��������� TableRow=-1), TableRow ����� ������������ � �������� �� ������ �������
  int ColId;            // ������������� ������� ������� (�� ���������, ������ ��� ��������� �������)
  LPSTR CellValue;      // �������� ������ ������� (������ ��� ��������� �������)
  COLORREF BGColor,FGColor; // ����� ���� � ������ ������� ������ ������� (������ ��� ��������� �������)
  int CellAlignment;    // ������������ ������ �������
  BOOL DefaultTableDraw;  // ��������������� � ������� � TRUE, ���� ����� ���������� ������ ������� �� ���������

  // ��� RDS_FORMSERVEVENT_TBLBUTTON � RDS_FORMSERVEVENT_TBLBTNENABLE **** REV2 ****
  // ��� �������� ������ �������, ���� �����, ������������ TableRow
  int Button;           // ������ (RDS_FORMVAL_TBLBTN_*)
  int ButtonResult;     // ��������� (������� �� ������)
    #define RDS_FORMSERVRESULT_OK       1       // ���������� ����������
    #define RDS_FORMSERVRESULT_NO       0       // ������ ��������
  int IndexAfterButton; // ����� ������ ������, ������� ����� ���������� ����� �������, ��� -1
} RDS_FORMSERVFUNCDATA;
typedef RDS_FORMSERVFUNCDATA *RDS_PFORMSERVFUNCDATA;
//---------------------------------------------------------------------------

// ��������� ��� ������� rdsBlockOrConnByExtId
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  BOOL Found;           // ���-�� �������
  RDS_BHANDLE Block;    // ��������� ����
  RDS_CHANDLE Conn;     // ��������� ����� ��� ����
  int Type;             // ��� ���������� ������� (RDS_BT*,RDS_CT* ��� RDS_TUNKNOWN)
} RDS_FINDBYEXTIDDATA;
typedef RDS_FINDBYEXTIDDATA *RDS_PFINDBYEXTIDDATA;
//---------------------------------------------------------------------------

// ��������� ��� ������� rdsProcessText
#define RDS_PT_TEXTTOSTRING     0       // �������������� ������������ � ������������� �������������
#define RDS_PT_STRINGTOTEXT     1       // �������� � RDS_PT_TEXTTOSTRING
#define RDS_PT_VARTYPETEXT      2       // ��������� �������� ���� ���������� �� ������ ����� ������ (RDS_VARTYPE_*)
#define RDS_PT_VARTYPECHAR      3       // C����� �� ������ ������� RDS_VARTYPE_* �� ������ �������� ���� ����������
// **** REV2 ****
#define RDS_PT_RDSRUSENCODE     4       // ������� ������� - � ��������� � ��������������
#define RDS_PT_RDSRUSDECODE     5       // ��������� ������� � �������������� - � �������
#define RDS_PT_ENCODEURICOMP    6       // ������ encodeURIComponent � JavaScript
#define RDS_PT_DECODEURICOMP    7       // ������ decodeURIComponent � JavaScript
#define RDS_PT_TEXTTOSTRINGSL   8       // �������������� ������������ � ������������� �������������, ������ '/' �� '0x2f'
//---------------------------------------------------------------------------

// ��������� ��� ������� rdsShowBlockPanelTab
#define RDS_BLOCKPANELOP_HIDE           0       // �������� �������
#define RDS_BLOCKPANELOP_SHOW           1       // �������� �������
#define RDS_BLOCKPANELOP_HIDEEXCEPT     2       // �������� ���, ����� ���������
#define RDS_BLOCKPANELOP_SHOWALL        3       // �������� ���
#define RDS_BLOCKPANELOP_SELECT         4       // ������� �������
//---------------------------------------------------------------------------

// ��������� ��� ������� rdsSetBlockAltNameText
#define RDS_ALTBLKNAME_LEFT             -1      // ������������ �����
#define RDS_ALTBLKNAME_CENTER           0       // ������������ �� ������
#define RDS_ALTBLKNAME_RIGHT            1       // ������������ ������
//---------------------------------------------------------------------------

// ��������� ��� ������� rdsExecuteCommand
#define RDS_SYSCMD_STARTCALC            0       // ��������� ������
#define RDS_SYSCMD_LOADFILE             1       // ��������� ����
#define RDS_SYSCMD_SAVEFILE             2       // �������� ����
#define RDS_SYSCMD_LOADTEMPLATE         3       // ��������� ������
#define RDS_SYSCMD_MESSAGEBOX           4       // ������� ���� � ����������
#define RDS_SYSCMD_EDITMODE             5       // ������� � ����� ��������������
#define RDS_SYSCMD_CALCMODE             6       // ������� � ����� �������������
#define RDS_SYSCMD_RESETCALC            7       // �������� ������ ��� ���� �����
#define RDS_SYSCMD_REFRESH              8       // �������� ���� ��� ��������� ���������� **** REV2 ****
#define RDS_SYSCMD_COMPILE              9       // ������������� ������ ��� ��� ������ **** REV2 ****
#define RDS_SYSCMD_DELAYEDTOOLCALL      10      // ���������� ����� ������ ���������� ��� ��������� (����������� rdsExecToolDelayed) **** REV2 ****
// ����� ��� ������ RDS_SYSCMD_LOADFILE � RDS_SYSCMD_LOADTEMPLATE
#define RDS_SYSCMDF_KEEPQUEUE           1       // �� ������� ������� ������ ����� �������� **** REV2 ****
#define RDS_SYSCMDF_KEEPTOOLS           2       // �� ��������� ������ ���������� ����� ��������� **** REV2 ****
// ���� ��� ������� RDS_SYSCMD_REFRESH
#define RDS_SYSCMDF_RECURSE             1       // �������� ��� ��������� ���� **** REV2 ****
// ���� ��� ������� RDS_SYSCMD_COMPILE
#define RDS_SYSCMDF_REBUILD             1       // ������������� ������������� **** REV2 ****
//---------------------------------------------------------------------------

// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsPBARCreate (ProgressBar)
// �������
#define RDS_PBAR_RESET                  0       // �������� ������� � 0
#define RDS_PBAR_SHOW                   1       // �������� ����
#define RDS_PBAR_HIDE                   2       // ������ ����
// ������� ��������� � ��������� ����� ���������� (������� rdsSetObjectInt, rdsGetObjectInt)
#define RDS_PBAR_MAX                    0       // ������������ ��������
#define RDS_PBAR_POSITION               1       // ������� ��������
#define RDS_PBAR_ADDTOPOS               2       // �������� �������� � �������� (������ ���������)
// ������� ��������� ������
#define RDS_PBAR_SETCAPTION             3       // ���������� ����� ���������
//---------------------------------------------------------------------------

// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsBEUCreate (undo ��������� ���������� �����)
// �������
#define RDS_BEU_STORECHANGED            0       // ��������� ������������ ���������
#define RDS_BEU_JOINNEXT                1       // ���������� �� ��������� ���������  **** REV2 ****

// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsCEUCreate (undo ��������� ���������� �����)
// �������
#define RDS_CEU_STORECHANGED            0       // ��������� ������������ ���������  **** REV2 ****
#define RDS_CEU_JOINNEXT                1       // ���������� �� ��������� ���������  **** REV2 ****

// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsPANCreate (������ � ���� ����������)
// ����� rdsPANCreate
#define RDS_PAN_F_BORDER        1       // � ������ ���� �����
#define RDS_PAN_F_SCALABLE      2       // ������ ������ ������ �������� �������� ����������
#define RDS_PAN_F_SIZEABLE      4       // ������������ ����� �������� ������ ������ (������ ������ � RDS_PAN_F_BORDER)
#define RDS_PAN_F_MOVEABLE      8       // ������������ ����� ���������� ������ (������ ������ � RDS_PAN_F_BORDER)
#define RDS_PAN_F_NOBUTTON     16       // ��� ������ �������� ������ (������ ������ � RDS_PAN_F_BORDER)
#define RDS_PAN_F_HIDDEN       32       // ������ ��������� �������
#define RDS_PAN_F_CAPTION      64       // ���� ��������� ������ (������ ������ � RDS_PAN_F_BORDER)
#define RDS_PAN_F_PAINTMSG    128       // ���� ������ �������� ��������� � ������������� ������������ ������
// ������� ��������� � ��������� ����� ���������� (������� rdsSetObjectInt, rdsGetObjectInt)
#define RDS_PAN_VISIBLE         0       // ��������� ������
#define RDS_PAN_WIDTH           1       // ������� ������ ������
#define RDS_PAN_HEIGHT          2       // ������� ������ ������
#define RDS_PAN_LEFT            3       // �������������� ���������� ����� ������� ������
#define RDS_PAN_TOP             4       // ������������ ���������� ������� ������� ������
#define RDS_PAN_CLIENTWIDTH     5       // ���������� ������ ������
#define RDS_PAN_CLIENTHEIGHT    6       // ���������� ������ ������
#define RDS_PAN_MINCLWIDTH      7       // ����������� ���������� ������ ������
#define RDS_PAN_MAXCLWIDTH      8       // ������������ ���������� ������ ������
#define RDS_PAN_MINCLHEIGHT     9       // ����������� ���������� ������ ������
#define RDS_PAN_MAXCLHEIGHT     10      // ������������ ���������� ������ ������
#define RDS_PAN_FLAGS           11      // ����� RDS_PAN_F_*
// ������� ��������� � ��������� �����
#define RDS_PAN_CAPTION         12      // ��������� ������

// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsCSVCreate (������ ��� ������ � CSV)
// �������
#define RDS_CSV_CLEAR           0       // �������� ��� ������
#define RDS_CSV_CLOSEFILE       1       // ������� �������� ����
#define RDS_CSV_STRFROMFILE     2       // ������� ��������� ������ �� �����
#define RDS_CSV_STRTOFILE       3       // �������� ������ � ����
#define RDS_CSV_REWINDFILE      4       // ����� ������ ������ ����� � ������ ����� **** REV2 ****
// ������� ��������� � ��������� ����� ���������� (������� rdsSetObjectInt, rdsGetObjectInt)
#define RDS_CSV_LINECOUNT       4       // ����� ����� (������ ������)
#define RDS_CSV_LINECOLUMNS     5       // ����� �������� � ���������� ������ (������ ������)
#define RDS_CSV_MAXCOLUMNS      6       // ������������ ����� �������� �� ���� ������� (������ ������)
#define RDS_CSV_QUOTECHAR       7       // ������ ������� (������, ���������)
#define RDS_CSV_DELIMITERCHAR   8       // ������-����������� (������, ���������)
#define RDS_CSV_FILEISOPEN      9       // ���� ������ (1 - ��, ������ ������)
#define RDS_CSV_FILEERROR       16      // ������ � �������� ������� (1 - ��, ������ ������)
// ������� ��������� � ��������� �����
#define RDS_CSV_LINE            10      // ������ (������ ���.�����, ���������)
#define RDS_CSV_TEXT            11      // ���� ����� (������ ���.�����, ���������)
#define RDS_CSV_OPENFILEREAD    12      // ������� ���� ��� ������ (������ ���������)
#define RDS_CSV_OPENFILEWRITE   13      // ������� ���� ��� ������ (������ ���������)
#define RDS_CSV_LOADFROMFILE    14      // ��������� �� ����� ���� ����� (������ ���������)
#define RDS_CSV_SAVETOFILE      15      // �������� � ���� ���� ����� (������ ���������)


// �������������� ������ ��� ������� rdsXGDrawStdIcon � rdsXGGetStdIconSize
#define RDS_STDICON_GREENSQUARE         0       // ��������� ������� �������
#define RDS_STDICON_YELLOWSQUARE        1       // ��������� ������ �������
#define RDS_STDICON_REDSQUARE           2       // ��������� ������� �������
#define RDS_STDICON_YELLOWGEAR          7       // ������ ����������
#define RDS_STDICON_RUN                 8       // ������ "play" (����������� ������)
#define RDS_STDICON_REDGEAR             9       // ������� ����������
#define RDS_STDICON_SYSTEM             10       // ��������� ������ ����������
#define RDS_STDICON_BLOCK              11       // ��������� ������ �����
#define RDS_STDICON_EYE                15       // ���� (��� � ��������� �����)
#define RDS_STDICON_PENCIL             16       // �������� (��� � ��������� �����)
#define RDS_STDICON_REDTRIEXCLAM       23       // "!" � ������� ������������
#define RDS_STDICON_YELLOWQUESTION     24       // "?" � ������ �����
#define RDS_STDICON_DISABLEDCONN       25       // "������" (��� �� ����������� ������)
#define RDS_STDICON_REDCIRCEXCLAM      26       // "!" � ������� �����
#define RDS_STDICON_YELCIRCEXCLAM      27       // "!" � ������ �����
#define RDS_STDICON_STOP               (0x20000|24) // ������ "stop" (������ �������)
#define RDS_STDICON_ZOOM               39       // ����                            **** REV2 **** ������� � �����
#define RDS_STDICON_NOZOOM             40       // ���� � ������� �������
#define RDS_STDICON_ZOOMIN             41       // ���� � ������
#define RDS_STDICON_ZOOMOUT            42       // ���� � �������
#define RDS_STDICON_TDOWN              43       // ����������� ����
#define RDS_STDICON_TUP                44       // ����������� �����
#define RDS_STDICON_TRIGHT             45       // ����������� ������
#define RDS_STDICON_TLEFT              46       // ����������� �����
#define RDS_STDICON_PLUS               47       // ����
#define RDS_STDICON_MINUS              48       // �����
#define RDS_STDICON_DELETE             49       // ���� ��������
#define RDS_STDICON_LEFT               50       // ������ ������� �����
#define RDS_STDICON_RIGHT              51       // ������ ������� ������
#define RDS_STDICON_UP                 52       // ������ ������� �����
#define RDS_STDICON_DOWN               53       // ������ ������� ����
#define RDS_STDICON_HZOOMIN            54       // "+" � ����� � �������������� ��������������� ��������
#define RDS_STDICON_HZOOMOUT           55       // "-" � ����� � �������������� ��������������� ��������
#define RDS_STDICON_VZOOMIN            56       // "+" � ����� � ������������ ��������������� ��������
#define RDS_STDICON_VZOOMOUT           57       // "-" � ����� � ������������ ��������������� ��������
#define RDS_STDICON_HSIZE              58       // ��� �������������� ��������� �������
#define RDS_STDICON_VSIZE              59       // ��� ������������ ��������� �������
#define RDS_STDICON_HVSIZE             60       // ������������� � ����� ���������� ���������
//---------------------------------------------------------------------------

// ����� ��� ������� rdsNetBroadcastData � rdsNetSendData
#define RDS_NETSEND_UPDATE      1       // ����� �������� ������ �������������� ������ ������
#define RDS_NETSEND_NOWAIT      2       // �� ����� ������ �������, ���������� ���������
                                        // ������ ����������
#define RDS_NETSEND_SERVREPLY   4       // ������� ������, ������ ������ �������� �� ����
                                        // ���������� ����� (RDS_BFM_NETDATAACCEPTED)
#define RDS_NETSEND_UDP         8       // �������� ������ �� ��������� UDP, ���� ��� ���������
                                        // � ���������� RDS
//---------------------------------------------------------------------------

// ����� ��� ������� rdsVarArrayIndexCheck
#define RDS_VAIC_SINGLE         1       // ��������� �� ������� �� �����������, � ����������
                                        // ������������ �������� � ����� �������� �������
                                        // ������ ����� �� ����� ��������� ("������" ������ "�������")
#define RDS_VAIC_MESSAGE        2       // �������� ��������� �� ������
#define RDS_VAIC_SINGLEMSG      4       // �������� ������ ����� ������ ��������� �� ������
                                        // (��� ���������� RDS_VAIC_MESSAGE ������������)
#define RDS_VAIC_STOPCALC       8       // ������������� ������ ��� ������
//---------------------------------------------------------------------------

// ����� ����� ��� ������������ ������� ����������
// 87654321
// .tttnnnn
// tt: 0 - ���������, 0x10 - �����, 0x20 - �����
// ���������
#define RDS_SFTAG_EOF           0       // ����� ������
#define RDS_SFTAG_CONNSTYLES    1       // ����� ������ � ���
#define RDS_SFTAG_TYPES         2       // ���������
#define RDS_SFTAG_DATATYPE      0xff    // ��� ������ (�� ��� ���� DWORD 0)
// �����
#define RDS_SFTAG_SIMPLEBLOCK   0x10    // ������� ����
#define RDS_SFTAG_SYSTEM        0x11    // ����������
#define RDS_SFTAG_INPUTBLOCK    0x12    // ������� ����
#define RDS_SFTAG_OUTPUTBLOCK   0x13    // ������� �����
#define RDS_SFTAG_BUSPORT       0x14    // ���� ����
#define RDS_SFTAG_ROOT          0x15    // �������� ����������
// �����
#define RDS_SFTAG_CONNECTION    0x20    // �����
#define RDS_SFTAG_BUS           0x21    // ����
//---------------------------------------------------------------------------

// ��������� ��� ������� rdsSetLayerPosition (��� ��������)
#define RDS_SLP_TOP     0       // �� ����� ����
#define RDS_SLP_BOTTOM  1       // � ����� ���
#define RDS_SLP_BEFORE  2       // ����� ��������� �����
#define RDS_SLP_AFTER   3       // ����� ���������� ����
//---------------------------------------------------------------------------

// ����� ��� rdsStringReplace
#define RDS_SRF_STDPATHS        1       // �������� ������� ����������� ����� ($DLL$ � �.�)
                                        // �� ���� ����
#define RDS_SRF_IGNORECASE      2       // ������������ ������� ��� ������ ��������
//---------------------------------------------------------------------------

// ����� ��� rdsCallFileDialog
#define RDS_CFD_OPEN            0       // ������ �������� (��������� �������
                                        // ������ ��� ��������)
#define RDS_CFD_SAVE            1       // ������ �����������
#define RDS_CFD_CREATEPROMPT    2       // ������������ �������� ����� ��� ������
                                        // ��������������� � ������� ����������)
#define RDS_CFD_OVERWRITEPROMPT 4       // ������������ ������ ����� ��� ������
                                        // ������������� � ������� ����������)
#define RDS_CFD_MUSTEXIST       8       // ���� ������ ������������ (��� ������� ��������)
#define RDS_CFD_ABSPATH         16      // ���������� ����������, � �� ������������� ����
//---------------------------------------------------------------------------

// ��������� ��� rdsTransformFileName
#define RDS_TFN_GETPATH         0       // �������� �� ����� ����� ���� � "\"
#define RDS_TFN_GETPATHNOBS     1       // �������� �� ����� ����� ���� ��� "\"
#define RDS_TFN_GETEXT          2       // �������� �� ����� ����� ����������
#define RDS_TFN_GETNAME         3       // �������� �� ����� ����� ���� ��� ��� ����
#define RDS_TFN_CHANGEEXT       4       // �������� ���������� �� ������
#define RDS_TFN_INCLUDEPATHBS   5       // �������� � ���� "\", ���� ��� ��� ���
#define RDS_TFN_EXCLUDEPATHBS   6       // ������ �� ����� ���� "\", ���� �� ��� ����
//---------------------------------------------------------------------------

// ��������� ��� ������� �������������� BMP � ������ ������� **** REV2 ****
// ������ ������
#define RDS_BMPCONV_SETUP       0       // ��������� � "� ���������"
#define RDS_BMPCONV_SAVEFILE    1       // �������� ����
//---------------------------------------------------------------------------

// ���������-�������� ������� rdsSaveSystemBitmap **** REV2 ****
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  RDS_BHANDLE Block;    // ����������
  DWORD Flags;          // ����� (RDS_SSBF_*)
    #define RDS_SSBF_ZOOMED     1       // Left,Top,Width � Height - � �������� Zoom, � �� 100%
  int Left,Top;         // ������ ����������� ������� � �������� 100% ��� Zoom (������� �� RDS_SSBF_ZOOMCOORDS)
  int Width,Height;     // ������� ����������� ������� � �������� 100% ��� Zoom (������� �� RDS_SSBF_ZOOMCOORDS)
  int PicWidth,PicHeight;// ������������ ������� ����������� ��������
  LPSTR FileName;       // ������ ����� ����� � ����� (����� ������������ ���������� ��������� ����� � RDS_SSBC_*)
    #define RDS_SSBC_NX         "$NX$"  // ����� �������� �� �����������
    #define RDS_SSBC_4NX        "$4NX$" // �� �� � �������� ������, ������ 4 �������
    #define RDS_SSBC_NY         "$NY$"  // ����� �������� �� ���������
    #define RDS_SSBC_4NY        "$4NY$" // �� �� � �������� ������, ������ 4 �������
    #define RDS_SSBC_NN         "$NN$"  // ������������ ���������� ����� �������� (�� ������ �������� ���� ������� ������)
    #define RDS_SSBC_4NN        "$4NN$" // �� �� � �������� ������, ������ 4 �������
  LPSTR ConvertExt;     // ���������� ������ �������������� �������� ��� NULL ��� BMP
  DWORD Reserved;       // ��������������� ��� ������������ **** REV2 ****
  double Zoom;          // �������
} RDS_SAVESYSBMPDATA;
typedef RDS_SAVESYSBMPDATA *RDS_PSAVESYSBMPDATA;
//---------------------------------------------------------------------------

// ��������� �������� ���������� �������� ���� ����� "������������� � �������" **** REV2 ****
// ������������ � rdsSetBlockTextRectParams, rdsGetBlockTextRectParams
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  BOOL Exists;          // ���� ����� ������� ��� � ���� �������������� � �������
  LPSTR Text;           // �����
  int HAlign;           // ������������ ������ (-1 - �����, 0 - �� ������, 1 - ������)
  int VAlign;           // ������������ ������ (-1 - �����, 0 - �� ������, 1 - ����)
  BOOL BorderFlag;      // ������� �����
  int BorderWidth;      // ������� ����� �����
  BOOL ScaleBorder;     // ��������������� �����
  COLORREF Border;      // ���� �����
  BOOL FillFlag;        // ������� ����������
  COLORREF Fill;        // ���� ����������
  BOOL Translate;       // ��������� �������
  // ����� ����������/���������� � ��������� ��������� RDS_SERVFONTPARAMS
} RDS_BLOCKTEXTRECTDATA;
typedef RDS_BLOCKTEXTRECTDATA *RDS_PBLOCKTEXTRECTDATA;
//---------------------------------------------------------------------------

// ����� ��� rdsSetBlockTextRectParams **** REV2 ****
#define RDS_SBTRPREFRESH        1       // �������� ����
//---------------------------------------------------------------------------

// ����� ��� rdsSCEUSaveBlock � rdsSCEUSaveConn **** REV2 ****
#define RDS_SCEUFLAG_FULL       1       // ������ ���� ������ ����� (�� ������ ����������)
#define RDS_SCEUFLAG_BEFORE     2       // ������ ������ ������� ����� ���������
#define RDS_SCEUFLAG_AFTER      0       // ������ ������ ������� ����� �������� (������-���� ��� �������)
// ��������� ��� ���������������� ������� rdsSCEUCreate **** REV2 ****
// �������
#define RDS_SCEU_CLEAR          0       // �������� ��� ������
#define RDS_SCEU_STORE          1       // �������� � undo
#define RDS_SCEU_STOREDELAYED   2       // �������� � undo ����� ��������� ��������
#define RDS_SCEU_JOINPREV       3       // ���������� � ����������
//---------------------------------------------------------------------------

// ��������� � �������� ��� ������� ���������� **** REV2 ****
// ������ ������
#define RDS_TOOLM_INIT          0       // �������������
#define RDS_TOOLM_CLEANUP       1       // �������
#define RDS_TOOLM_SETUP         2       // ����� ��������
#define RDS_TOOLM_EXECUTE       3       // ����� ������ ����������
#define RDS_TOOLM_MODECHANGE    4       // ��������� ������ RDS
#define RDS_TOOLM_TIMER         5       // ������������ �������
#define RDS_TOOLM_BEFORESAVE    6       // ����� ����������� �����
#define RDS_TOOLM_AFTERSAVE     7       // ����� ���������� �����
#define RDS_TOOLM_UNLOADSYSTEM  8       // ��� ������� ������ ����� ���������
#define RDS_TOOLM_AFTERLOAD     9       // ����� �������� ���� �����
#define RDS_TOOLM_CALL          10      // ����� �� ������ ����� ��� ������� ������
#define RDS_TOOLM_REMOTECALL    11      // ����� �� ��
#define RDS_TOOLM_EXTCALL       12      // ����������� ����� �� ������ ����� ��� ������� ������
#define RDS_TOOLM_DELAYEDCALL   13      // ���������� ����� �� ������ ����� ��� ������� ������
//---------------------------------------------------------------------------
// ���������� **** REV2 ****
#define RDS_TOOLR_DONE          0       // ��������� (����� ��������� ������)
#define RDS_TOOLR_STAY          1       // ������ ������ �������� � ������
//---------------------------------------------------------------------------
// ������ ������ ���������� **** REV2 ****
typedef struct
{ LPVOID ModuleData;    // ����� ������� ������ ������ (������/������)
  int RDSMode;          // ������� ����� RDS (��������� RDS_CM_*)
  LPSTR ServiceName;    // ���������� ��� (������ ������)
  // ����� ���������������� ���� (�� �������������� � �� ����������������)
  int Tag;
} RDS_TOOLMODULEDATA;
typedef RDS_TOOLMODULEDATA *RDS_PTOOLMODULEDATA;
//---------------------------------------------------------------------------
// ��������� ������ ������ ���������� **** REV2 ****
// ������������ � ������� RDS_TOOLM_CALL � RDS_TOOLM_REMOTECALL
typedef struct
{ LPSTR String;         // ���������� ������
  int Value;            // ���������� �����
} RDS_TOOLCALLDATA;
typedef RDS_TOOLCALLDATA *RDS_PTOOLCALLDATA;
//---------------------------------------------------------------------------
// ��������� ������������ ������ ������ ���������� **** REV2 ****
// ������������ � ������ RDS_TOOLM_EXTCALL
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  int Command;          // ���������� �����
  int IntParam;         // �������������� ���������� �����
  int IntValue;         // �������������� ���������� �����
  int NInStrings;       // ���������� ������� �����
  LPSTR *InStrings;     // ������ ������� ����� (�������������� � ������������� ���������)

  int NOutStrings;      // ����� �������� �����
  LPSTR *OutStrings;    // ������ �������� ����� (��������� ������� ��� ������ rdsAllocate, ������ ������ � �������
                        // ���� ��������� ������� ��� ������ rdsAllocate, ��� ������������� ��������� ��� ������
                        // rdsFree)
  int Result;           // ������������ ���������

  RDS_BHANDLE Block;    // ����, ��� �������� ���������� ��������, ��� NULL (��� ������� ��������, �����������
                        // ���������, ����� ������� � ��������)

} RDS_TOOLEXTCALLDATA;
typedef RDS_TOOLEXTCALLDATA *RDS_PTOOLEXTCALLDATA;
//---------------------------------------------------------------------------
// ����� � ��������� ������� rdsExecTool **** REV2 ****
//   3         2         1
//  10987654321098765432109876543210
//  ......................SSMMMMMMMM
// M - �����
// S - ������ ������ ������
// �����
#define RDS_ETF_M_EXECUTE       0       // ������� ������ ���������� � ������ RDS_TOOLM_EXECUTE (��� �� ����)
#define RDS_ETF_M_SETUP         1       // ������� ������ ���������� � ������ RDS_TOOLM_SETUP (���������)
#define RDS_ETF_M_CALL          2       // ������� ������ ���������� � ������ RDS_TOOLM_CALL (��� RDS_TOOLM_REMOTECALL) � ���������� RDS_PTOOLCALLDATA
// ������ ������ ������ (������������ � ��� rdsExecToolExt)
#define RDS_ETF_S_BYDLL         0       // �� ����� ������� � ����� DLL
#define RDS_ETF_S_BYSERVNAME    0x100   // �� ����������� �����
// �����
#define RDS_ETF_MODEMASK        0xff    // �����
#define RDS_ETF_SEARCHMASK      0x300   // ������ ������
//---------------------------------------------------------------------------
// ����� ������� rdsVSCompare **** REV2 ****
#define RDS_VARCOMP_ALL         0       // ��� ���������
#define RDS_VARCOMP_VARTYPES    1       // ���� ����������
#define RDS_VARCOMP_VARNAMES    2       // ����� ����������
#define RDS_VARCOMP_VARDEFAULTS 4       // �������� ���������� �� ���������
#define RDS_VARCOMP_VARIO       8       // ���� ���������� � ����� (�����/������/����������)
#define RDS_VARCOMP_VARDISP     16      // ����� ����������� ���������� (����, ����������� �����)
#define RDS_VARCOMP_VARAUTORUN  32      // ����� ����������� ������ ��� ������
#define RDS_VARCOMP_VARREM      64      // ����������� ����������
#define RDS_VARCOMP_AUTOCONN    128     // ��������� ������� � ���������� ����������
//---------------------------------------------------------------------------
// ���� �������� ������� ������� **** REV2 ****
#define RDS_HELP_OK             0       // ��� � �������
#define RDS_HELP_NOUIFILE       1       // ���� UI (�� ������� ���) �� ������
#define RDS_HELP_READERROR      2       // ������ ������ �����
#define RDS_HELP_NOTOPIC        3       // ���� �� �������
#define RDS_HELP_NOHELPFILE     4       // ���� ����� ������� (�� ������ �� UI) �� ������
#define RDS_HELP_NOEXE          5       // ��� ����� ������� �� ������� ���������-����������
#define RDS_HELP_UNKNOWN        6       // ������ ������
//---------------------------------------------------------------------------
// ����� ������� rdsEnumDynVars **** REV2 ****
#define RDS_EDV_SEARCH          1       // � ������� ����� �� �������� ���������
//---------------------------------------------------------------------------
// ����� �������������� ���������� ����� **** REV2 ****
#define RDS_ACP_HINT            1       // ��������� �� ����������� ���������
#define RDS_ACP_NOTRANSLATE     2       // �� ��������� �������
//---------------------------------------------------------------------------
// ����� ������� rdsCopySelectedToMem **** REV2 ****
#define RDS_CSTM_STATE          1       // ���������� ������ � ����������� ������
#define RDS_CSTM_AUTOCOMP       2       // ���������� ������ � ����������� ������� ��������������
#define RDS_CSTM_LAYERS         4       // ���������� ������ �� ������
#define RDS_CSTM_CLASSES        8       // ���������� ������ � ��������
#define RDS_CSTM_KEEPEXTERNAL  16       // ������ ������ ����� ������, ����������� � �� ����������� ���������, �� ���������� 
//---------------------------------------------------------------------------
// ����� ������� rdsPasteFromMem **** REV2 ****
#define RDS_PFM_UPDATEWIN       1       // �������� ����
#define RDS_PFM_CONNDATA        2       // ������� ������� ������ �� ������ ����� �������
#define RDS_PFM_STATE           4       // ��������� ������ � ����������� ������
#define RDS_PFM_CREATELAYERS    8       // ���� � ���������� ������ ���� ����, ��������� �������������
#define RDS_PFM_INVALIDPATHS    16      // �� ������� ��������c� �� ���� � ����������� ������ (������ � ������ ������)
//---------------------------------------------------------------------------
// ��������� ������� rdsExclusiveFileOp **** REV2 ****
#define RDS_EFO_USE     1       // �������� ���� ��� ������������
#define RDS_EFO_UNUSE   2       // �������� ���� ��� �� ������������
#define RDS_EFO_FORGET  3       // ������ � ������������� �����
//---------------------------------------------------------------------------
// ����� ������� rdsPasteFromMem **** REV2 ****
#define RDS_ICF_NOCALL          1       // �� �������� ������ ������ ����� (������ � �������� ��� �������)
#define RDS_ICF_ALLBLOCKS       2       // ������� ����������������� ������ ��� ���� ������
#define RDS_ICF_UPDATEWIN       4       // �������� ���������� ���� ����� �������� (������ � ������� �������������� � �������������)
//---------------------------------------------------------------------------
// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsINTCreate (������ ��� ������ � ����������� ������������� ����� �����)  **** REV2 ****
// �������
#define RDS_INT_CLEAR           0       // �������� ��� ������
// ������� ��������� � ��������� ����� ���������� (������� rdsSetObjectInt, rdsGetObjectInt)
#define RDS_INT_ALLOWZERO       1       // ���������� ������ � �����
#define RDS_INT_VALUE           2       // ���������� �������� ��� �������� ���������
//---------------------------------------------------------------------------
// ����� ������� rdsLockDrawUpdates **** REV2 ****
#define RDS_LDUF_LOCK           1       // �������������
#define RDS_LDUF_UNLOCK         2       // ��������������
//---------------------------------------------------------------------------
// ����� ������� rdsGBLAddBlock **** REV2 ****
#define RDS_GBL_ADD     0       // �������� ���� (������� ��� - 0)
#define RDS_GBL_DELETE  1       // ������� ���� (������� ��� - 1)
//---------------------------------------------------------------------------
// ��������� ��� �������������� �� ��������������� ��������, ��������� ��������
// rdsGBLCreate (������ ��� ������ � ���������� ������� ������)
// ������� ��������� � ��������� ����� ���������� (������� rdsSetObjectInt, rdsGetObjectInt)
#define RDS_GBL_GETCOUNT        0       // ����� ������ (������ ������)
//---------------------------------------------------------------------------
// ��������� ������� rdsSetBlockOrder � rdsSetConnOrder **** REV2 ****
#define RDS_BLOCKORDER_FIRST    0       // ������� ������ � ����������
#define RDS_BLOCKORDER_LAST     1       // ������� ��������� � ����������
#define RDS_BLOCKORDER_AFTER    2       // ��������� ����� ���������
#define RDS_BLOCKORDER_BEFORE   3       // ��������� ����� ��������
//---------------------------------------------------------------------------
// ����� ������� rdsSetConnText **** REV2 ****
#define RDS_SCTF_TEXT           1       // ����������/�������� �����
#define RDS_SCTF_ALIGNMENT      2       // ���������� ������������ ������
#define RDS_SCTF_SHOW           4       // ���������� ����� ������
#define RDS_SCTF_COORDS         8       // ���������� ���������� ������ ������
#define RDS_SCTF_TRANSLATE      16      // ���������� ������������� ��������
//---------------------------------------------------------------------------
// ��������� ���� ������ ��� ��������� **** REV2 ****
#define RDS_TRANSLML_SINGLE     0       // ������������ �����
#define RDS_TRANSLML_LF         1       // ������������� �����, ����������� "\n"
#define RDS_TRANSLML_CRLF       2       // ������������� �����, ����������� "\r\n"
// �������� ����������� ����� �������������� ����������� ���������� ��� ���������� ���� ������� �������������� **** REV2 ****
#define RDS_TRANSLACPID_LOW     -199999 // ������ ��������� (������������)
#define RDS_TRANSLACPID_HIGH    -100000 // ����� ��������� (������������)
//---------------------------------------------------------------------------
// ����� ������� rdsFORMTBLAddHeader **** REV2 ****
// ����� ������������ ������
#define RDS_FORMTBLF_ALIGN      3
  #define RDS_FORMTBLF_LEFT     0       // �����
  #define RDS_FORMTBLF_CENTER   1       // �� ������
  #define RDS_FORMTBLF_RIGHT    2       // ������
// ����� ������������ ���������
#define RDS_FORMTBLF_HDRALIGN   12
  #define RDS_FORMTBLF_HDRLEFT   0       // �����
  #define RDS_FORMTBLF_HDRCENTER 4       // �� ������
  #define RDS_FORMTBLF_HDRRIGHT  8       // ������
// ���������������� ��������� ������
#define RDS_FORMTBLF_DRAW       16
// ���������������� ��������� ���������
#define RDS_FORMTBLF_HDRDRAW    32
//---------------------------------------------------------------------------

// ������� ������������ �� ���������
#ifndef RDS_NO_PACK
  #include <poppack.h>
#endif
//---------------------------------------------------------------------------

// ������������ ������� - ���� ����������
typedef void (RDSCALL *RDS_VS)(LPSTR);
typedef int (RDSCALL *RDS_IV)(void);
typedef HWND (RDSCALL *RDS_HwV)(void);
typedef RDS_BHANDLE (RDSCALL *RDS_BhV)(void);
typedef int (RDSCALL *RDS_IBhBd)(RDS_BHANDLE,RDS_PBLOCKDESCRIPTION);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhIBd)(RDS_BHANDLE,int,RDS_PBLOCKDESCRIPTION);
typedef int (RDSCALL *RDS_IChCd)(RDS_CHANDLE,RDS_PCONNDESCRIPTION);
typedef RDS_CHANDLE (RDSCALL *RDS_ChBhICd)(RDS_BHANDLE,int,RDS_PCONNDESCRIPTION);
typedef RDS_CHANDLE (RDSCALL *RDS_ChChICd)(RDS_CHANDLE,int,RDS_PCONNDESCRIPTION);
typedef int (RDSCALL *RDS_IChIPd)(RDS_CHANDLE,int,RDS_PPOINTDESCRIPTION);
typedef LPSTR (RDSCALL *RDS_SI)(int);
typedef LPSTR (RDSCALL *RDS_SV)(void);
typedef LPSTR (RDSCALL *RDS_SBhI)(RDS_BHANDLE,int);
typedef int (RDSCALL *RDS_IIS)(int,LPSTR);
typedef int (RDSCALL *RDS_II)(int);
typedef void (RDSCALL *RDS_VIIICrI)(int,int,int,COLORREF,int);
typedef void (RDSCALL *RDS_VIICr)(int,int,COLORREF);
typedef void (RDSCALL *RDS_VIIII)(int,int,int,int);
typedef void (RDSCALL *RDS_VIIIIII)(int,int,int,int,int,int);
typedef void (RDSCALL *RDS_VIIIIIIII)(int,int,int,int,int,int,int,int);
typedef void (RDSCALL *RDS_VISICrIIBBBB)(int,LPSTR,int,COLORREF,int,int,BOOL,BOOL,BOOL,BOOL);
typedef void (RDSCALL *RDS_VpLfCr)(LOGFONT*,COLORREF);
typedef void (RDSCALL *RDS_VIIS)(int,int,LPSTR);
typedef void (RDSCALL *RDS_VII)(int,int);
typedef void (RDSCALL *RDS_VIISpR)(int,int,LPSTR,RECT*);
typedef RDS_TIMERID (RDSCALL *RDS_ThThDwDwB)(RDS_TIMERID,DWORD,DWORD,BOOL);
typedef BOOL (RDSCALL *RDS_BpVI)(LPVOID,int);
typedef void (RDSCALL *RDS_VpVI)(LPVOID,int);
typedef BOOL (RDSCALL *RDS_BV)(void);
typedef void (RDSCALL *RDS_VSB)(LPSTR,BOOL);
typedef void (RDSCALL *RDS_VSpIpI)(LPSTR,int*,int*);
typedef RDS_PCOLORREF (RDSCALL *RDS_pCrV)(void);
typedef BOOL (RDSCALL *RDS_BpCr)(RDS_PCOLORREF);
typedef BOOL (RDSCALL *RDS_BBhEd)(RDS_BHANDLE,RDS_PEDITORPARAMETERS);
typedef BOOL (RDSCALL *RDS_BThTd)(RDS_TIMERID,RDS_PTIMERDESCRIPTION);
typedef void (RDSCALL *RDS_VTh)(RDS_TIMERID);
typedef void (RDSCALL *RDS_VThDw)(RDS_TIMERID,DWORD);
typedef LPSTR (RDSCALL *RDS_SSpSpCB)(LPSTR,LPSTR*,char*,BOOL);
typedef void (RDSCALL *RDS_VIB)(int,BOOL);
typedef void (RDSCALL *RDS_VSI)(LPSTR,int);
typedef void (RDSCALL *RDS_VSS)(LPSTR,LPSTR);
typedef void (RDSCALL *RDS_VSCrB)(LPSTR,COLORREF,BOOL);
typedef BOOL (RDSCALL *RDS_BSpSSIpIpCrpIpIpBpBpBpB)(LPSTR,LPSTR*,LPSTR,int,int*,
                                                      COLORREF*,int*,int*,
                                                      BOOL*,BOOL*,BOOL*,BOOL*);
typedef COLORREF (RDSCALL *RDS_CrSpS)(LPSTR,LPSTR*);
typedef void (RDSCALL *RDS_VpR)(RECT*);
typedef void (RDSCALL *RDS_VIIDDDB)(int,int,double,double,double,BOOL);
typedef void (RDSCALL *RDS_VpPI)(POINT *,int);
typedef int (RDSCALL *RDS_IBhS)(RDS_BHANDLE,LPSTR);
typedef int (RDSCALL *RDS_IBhIpV)(RDS_BHANDLE,int,LPVOID);
typedef int (RDSCALL *RDS_IBhSpVB)(RDS_BHANDLE,LPSTR,LPVOID,BOOL);
typedef RDS_CHANDLE (RDSCALL *RDS_ChBhChBBPd)(RDS_BHANDLE,RDS_CHANDLE,BOOL,BOOL,RDS_PPOINTDESCRIPTION);
typedef RDS_CHANDLE (RDSCALL *RDS_ChBhChPd)(RDS_BHANDLE,RDS_CHANDLE,RDS_PPOINTDESCRIPTION);
typedef void (RDSCALL *RDS_VChCa)(RDS_CHANDLE,RDS_PCONNAPPEARANCE);
typedef BOOL (RDSCALL *RDS_BSCa)(LPSTR,RDS_PCONNAPPEARANCE);
typedef int (RDSCALL *RDS_IBhSpV)(RDS_BHANDLE,LPSTR,LPVOID);
typedef LPVOID (RDSCALL *RDS_pVISSBS)(int,LPSTR,LPSTR,BOOL,LPSTR);
typedef BOOL (RDSCALL *RDS_BIS)(int,LPSTR);
typedef RDS_PDYNVARLINK (RDSCALL *RDS_DvISSB)(int,LPSTR,LPSTR,BOOL);
typedef void (RDSCALL *RDS_VDv)(RDS_PDYNVARLINK);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhSBd)(RDS_BHANDLE,LPSTR,RDS_PBLOCKDESCRIPTION);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBd)(RDS_PBLOCKDESCRIPTION);
typedef RDS_PDYNVARLINK (RDSCALL *RDS_DvISSBS)(int,LPSTR,LPSTR,BOOL,LPSTR);
typedef void (RDSCALL *RDS_VBhB)(RDS_BHANDLE,BOOL);
typedef void (RDSCALL *RDS_VBhBB)(RDS_BHANDLE,BOOL,BOOL);
typedef int (RDSCALL *RDS_IBhIB)(RDS_BHANDLE,int,BOOL);
typedef void (RDSCALL *RDS_VBhS)(RDS_BHANDLE,LPSTR);
typedef void (RDSCALL *RDS_VB)(BOOL);
typedef void (RDSCALL *RDS_VBh)(RDS_BHANDLE);
typedef int (RDSCALL *RDS_IChIICa)(RDS_CHANDLE,int,int,RDS_PCONNAPPEARANCE);
typedef BOOL (RDSCALL *RDS_BBhI)(RDS_BHANDLE,int);
typedef BOOL (RDSCALL *RDS_BBhS)(RDS_BHANDLE,LPSTR);
typedef void (RDSCALL *RDS_VBhEt)(RDS_BHANDLE,RDS_PEDITORTOOLBARS);
typedef void (RDSCALL *RDS_VV)(void);
typedef LPVOID (RDSCALL *RDS_pVpVpIpI)(LPVOID,int*,int*);
typedef BOOL (RDSCALL *RDS_BpVAd)(LPVOID,RDS_PARRAYACCESSDATA);
typedef BOOL (RDSCALL *RDS_BpVIIBAd)(LPVOID,int,int,BOOL,RDS_PARRAYACCESSDATA);
typedef BOOL (RDSCALL *RDS_BBhpV)(RDS_BHANDLE,LPVOID);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhIBCbpV)(RDS_BHANDLE,int,BOOL,RDS_BBhpV,LPVOID);
typedef int (RDSCALL *RDS_IIIB)(int,int,BOOL);
typedef BOOL (RDSCALL *RDS_BBhpB)(RDS_BHANDLE,BOOL*);
typedef int (RDSCALL *RDS_IBhIBCb2)(RDS_BHANDLE,int,BOOL,RDS_BBhpB);
typedef BOOL (RDSCALL *RDS_BICb2)(int,RDS_BBhpB);
typedef BOOL (RDSCALL *RDS_BBhIIpIpI)(RDS_BHANDLE,int,int,int*,int*);
typedef int (RDSCALL *RDS_IpV)(LPVOID);
typedef int (RDSCALL *RDS_ICb4pV)(RDS_IpV,LPVOID);
typedef int (RDSCALL *RDS_IBhIpVDw)(RDS_BHANDLE,int,LPVOID,DWORD);
typedef int (RDSCALL *RDS_IS)(LPSTR);
typedef void (RDSCALL *RDS_VBhIpVB)(RDS_BHANDLE,int,LPVOID,BOOL);
typedef BOOL (RDSCALL *RDS_BhPdPdpV)(RDS_PPOINTDESCRIPTION,RDS_PPOINTDESCRIPTION,LPVOID);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhDwCb1pV)(RDS_BHANDLE,DWORD,RDS_BhPdPdpV,LPVOID);
typedef void (RDSCALL *RDS_VpVII)(LPVOID,int,int);
typedef int (RDSCALL *RDS_ISpSpVI)(LPSTR,LPSTR*,LPVOID,int);
typedef LPSTR (RDSCALL *RDS_SIB)(int,BOOL);
typedef int (RDSCALL *RDS_ISB)(LPSTR,BOOL);
typedef RDS_MENUITEM (RDSCALL *RDS_MhSDwIDwII)(LPSTR,BOOL,int,DWORD,int,int);
typedef void (RDSCALL *RDS_VMhBB)(RDS_MENUITEM,BOOL,BOOL);
typedef void (RDSCALL *RDS_VMh)(RDS_MENUITEM);
typedef LPVOID (RDSCALL *RDS_pVDw)(DWORD);
typedef void (RDSCALL *RDS_VpV)(LPVOID);
typedef LPSTR (RDSCALL *RDS_SBhpI)(RDS_BHANDLE,int*);
typedef RDS_BHANDLE (RDSCALL *RDS_BhSBd)(LPSTR,RDS_PBLOCKDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BBh)(RDS_BHANDLE);
typedef void (RDSCALL *RDS_VCh)(RDS_CHANDLE);
typedef RDS_BHANDLE (RDSCALL *RDS_BhSBhIIBd)(LPSTR,RDS_BHANDLE,int,int,RDS_PBLOCKDESCRIPTION);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhBhIIBd)(RDS_BHANDLE,RDS_BHANDLE,int,int,RDS_PBLOCKDESCRIPTION);
typedef RDS_VHANDLE (RDSCALL *RDS_VhBhiVd)(RDS_BHANDLE,int,RDS_PVARDESCRIPTION);
typedef RDS_VHANDLE (RDSCALL *RDS_VhBhSBVd)(RDS_BHANDLE,LPSTR,BOOL,RDS_PVARDESCRIPTION);
typedef RDS_HOBJECT (RDSCALL *RDS_HoV)(void);
typedef RDS_HOBJECT (RDSCALL *RDS_HoB)(BOOL);
typedef void (RDSCALL *RDS_VHo)(RDS_HOBJECT);
typedef RDS_CHANDLE (RDSCALL *RDS_ChHoBhIpI)(RDS_HOBJECT,RDS_BHANDLE,int,int*);
typedef BOOL (RDSCALL *RDS_BHoChI)(RDS_HOBJECT,RDS_CHANDLE,int);
typedef int (RDSCALL *RDS_IHoII)(RDS_HOBJECT,int,int);
typedef int (RDSCALL *RDS_IHoBhSIIB)(RDS_HOBJECT,RDS_BHANDLE,LPSTR,int,int,BOOL);
typedef int (RDSCALL *RDS_IHoChSBIIB)(RDS_HOBJECT,RDS_CHANDLE,LPSTR,BOOL,int,int,BOOL);
typedef int (RDSCALL *RDS_IHoIIIIII)(RDS_HOBJECT,int,int,int,int,int,int);
typedef BOOL (RDSCALL *RDS_BBhBrB)(RDS_BHANDLE,RDS_PBLOCKDIMENSIONS,BOOL);
typedef RDS_HOBJECT (RDSCALL *RDS_HoBhDwB)(RDS_BHANDLE,DWORD,BOOL);
typedef LPVOID (RDSCALL *RDS_pVHoIIpI)(RDS_HOBJECT,int,int,int*);
typedef void (RDSCALL *RDS_VBhII)(RDS_BHANDLE,int,int);
typedef void (RDSCALL *RDS_VBhSS)(RDS_BHANDLE,LPSTR,LPSTR);
typedef BOOL (RDSCALL *RDS_BChILdPdPd)(RDS_CHANDLE,int,RDS_PLINEDESCRIPTION,
                                       RDS_PPOINTDESCRIPTION,RDS_PPOINTDESCRIPTION);
typedef int (RDSCALL *RDS_IChIILdpI)(RDS_CHANDLE,int,int,RDS_PLINEDESCRIPTION,int*);
typedef void (RDSCALL *RDS_VBhIIIIB)(RDS_BHANDLE,int,int,int,int,BOOL);
typedef BOOL (RDSCALL *RDS_BBhIIII)(RDS_BHANDLE,int,int,int,int);
typedef BOOL (RDSCALL *RDS_BBhIII)(RDS_BHANDLE,int,int,int);
typedef BOOL (RDSCALL *RDS_BDv)(RDS_PDYNVARLINK);
typedef BOOL (RDSCALL *RDS_BpVpV)(LPVOID,LPVOID);
typedef BOOL (RDSCALL *RDS_BHopI)(RDS_HOBJECT,int*);
typedef int (RDSCALL *RDS_IHoISCSDwIS)(RDS_HOBJECT,int,LPSTR,char,LPSTR,DWORD,int,LPSTR);
typedef BOOL (RDSCALL *RDS_VHoIIS)(RDS_HOBJECT,int,int,LPSTR);
typedef BOOL (RDSCALL *RDS_BHoBhpI)(RDS_HOBJECT,RDS_BHANDLE,int*);
typedef int (RDSCALL *RDS_IHoSS)(RDS_HOBJECT,LPSTR,LPSTR);
typedef BOOL (RDSCALL *RDS_BHoIIpI)(RDS_HOBJECT,int,int,int*);
typedef BOOL (RDSCALL *RDS_BHoI)(RDS_HOBJECT,int);
typedef RDS_VHANDLE (RDSCALL *RDS_VhVhiVd)(RDS_VHANDLE,int,RDS_PVARDESCRIPTION);
typedef RDS_VHANDLE (RDSCALL *RDS_VhSVd)(LPSTR,RDS_PVARDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BHoIVd)(RDS_HOBJECT,int,RDS_PVARDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BHoBDwIS)(RDS_HOBJECT,BOOL,DWORD,int,LPSTR);
typedef LPSTR (RDSCALL *RDS_SHoII)(RDS_HOBJECT,int,int);
typedef void (RDSCALL *RDS_VHoIII)(RDS_HOBJECT,int,int,int);
typedef LPSTR (RDSCALL *RDS_SVhBIpI)(RDS_VHANDLE,BOOL,int,int*);
typedef int (RDSCALL *RDS_IHoIS)(RDS_HOBJECT,int,LPSTR);
typedef BOOL (RDSCALL *RDS_BHoS)(RDS_HOBJECT,LPSTR);
typedef BOOL (RDSCALL *RDS_BHwSHbHiS)(HWND,LPSTR,HBITMAP,HINSTANCE,LPSTR);
typedef void (RDSCALL *RDS_VHw)(HWND);
typedef LPSTR (RDSCALL *RDS_SHoS)(RDS_HOBJECT,LPSTR);
typedef RDS_VHANDLE (RDSCALL *RDS_VhIVd)(int,RDS_PVARDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BVhS)(RDS_VHANDLE,LPSTR);
typedef LPSTR (RDSCALL *RDS_SSSpI)(LPSTR,LPSTR,int*);
typedef void (RDSCALL *RDS_VHwS)(HWND,LPSTR);
typedef int (RDSCALL *RDS_IHoBhB)(RDS_HOBJECT,RDS_BHANDLE,BOOL);
typedef int (RDSCALL *RDS_IHoChB)(RDS_HOBJECT,RDS_CHANDLE,BOOL);
typedef void (RDSCALL *RDS_VHoDwSB)(RDS_HOBJECT,DWORD,LPSTR,BOOL);
typedef int (RDSCALL *RDS_ISSI)(LPSTR,LPSTR,int);
typedef int (RDSCALL *RDS_IHoSI)(RDS_HOBJECT,LPSTR,int);
typedef BOOL (RDSCALL *RDS_BHopSII)(RDS_HOBJECT,LPSTR*,int,int);
typedef int (RDSCALL *RDS_IHopSpSpCB)(RDS_HOBJECT,LPSTR*,LPSTR*,char*,BOOL);
typedef void (RDSCALL *RDS_VSD)(LPSTR,double);
typedef int (RDSCALL *RDS_ISpS)(LPSTR,LPSTR*);
typedef BOOL (RDSCALL *RDS_BBhIS)(RDS_BHANDLE,int,LPSTR);
typedef LPSTR (RDSCALL *RDS_SBhIpI)(RDS_BHANDLE,int,int*);
typedef BOOL (RDSCALL *RDS_BB)(BOOL);
typedef void (RDSCALL *RDS_VBhPv)(RDS_BHANDLE,LPVOID);
typedef BOOL (RDSCALL *RDS_BChIIIDw)(RDS_CHANDLE,int,int,int,DWORD);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhBd)(RDS_BHANDLE,RDS_PBLOCKDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BBhSBd)(RDS_BHANDLE,LPSTR,RDS_PBLOCKDESCRIPTION);
typedef LPSTR (RDSCALL *RDS_SHoIpI)(RDS_HOBJECT,int,int*);
typedef void (RDSCALL *RDS_VBhIII)(RDS_BHANDLE,int,int,int);
typedef BOOL (RDSCALL *RDS_BMhSDwIDwII)(RDS_MENUITEM,LPSTR,DWORD,int,DWORD,int,int);
typedef void (RDSCALL *RDS_VHoIID)(RDS_HOBJECT,int,int,double);
typedef double (RDSCALL *RDS_DHoII)(RDS_HOBJECT,int,int);
typedef void (RDSCALL *RDS_VHoSI)(RDS_HOBJECT,LPSTR,int);
typedef void (RDSCALL *RDS_VHoSD)(RDS_HOBJECT,LPSTR,double);
typedef double (RDSCALL *RDS_DHoSD)(RDS_HOBJECT,LPSTR,double);
typedef void (RDSCALL *RDS_VHoSS)(RDS_HOBJECT,LPSTR,LPSTR);
typedef LPSTR (RDSCALL *RDS_SHoSSpI)(RDS_HOBJECT,LPSTR,LPSTR,int*);
typedef int (RDSCALL *RDS_IBhSSSS)(RDS_BHANDLE,LPSTR,LPSTR,LPSTR,LPSTR);
typedef int (RDSCALL *RDS_IBh)(RDS_BHANDLE);
typedef DWORD (RDSCALL *RDS_DwBh)(RDS_BHANDLE);
typedef void (RDSCALL *RDS_VBhDwDw)(RDS_BHANDLE,DWORD,DWORD);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhIDwCb1pV)(RDS_BHANDLE,int,DWORD,RDS_BhPdPdpV,LPVOID);
typedef RDS_HOBJECT (RDSCALL *RDS_HoBIIS)(BOOL,int,int,LPSTR);
typedef void (RDSCALL *RDS_VHoIS)(RDS_HOBJECT,int,LPSTR);
typedef BOOL (RDSCALL *RDS_VHoIIDwSI)(RDS_HOBJECT,int,int,DWORD,LPSTR,int);
typedef LPSTR (RDSCALL *RDS_SSIpI)(LPSTR,int,int*);
typedef LPSTR (RDSCALL *RDS_SSSB)(LPSTR,LPSTR,BOOL);
typedef BOOL (RDSCALL *RDS_BSI)(LPSTR,int);
typedef int (RDSCALL *RDS_IIB)(int,BOOL);
typedef BOOL (RDSCALL *RDS_BI)(int);
typedef RDS_PFUNCPROVIDERLINK (RDSCALL *RDS_BFl)(int);
typedef void (RDSCALL *RDS_VI)(int);
typedef BOOL (RDSCALL *RDS_BHoCb3)(RDS_HOBJECT,RDS_VHo);
typedef BOOL (RDSCALL *RDS_BpD)(double*);
typedef BOOL (RDSCALL *RDS_BBhIDwDw)(RDS_BHANDLE,int,DWORD,DWORD);
typedef void (RDSCALL *RDS_VIS)(int,LPSTR);
typedef void (RDSCALL *RDS_VBhSI)(RDS_BHANDLE,LPSTR,int);
typedef void (RDSCALL *RDS_VIDwSS)(int,DWORD,LPSTR,LPSTR);
typedef RDS_MENUITEM (RDSCALL *RDS_MhSII)(LPSTR,int,int);
typedef void (RDSCALL *RDS_VBhSB)(RDS_BHANDLE,LPSTR,BOOL);
typedef RDS_HOBJECT (RDSCALL *RDS_HoIS)(int,LPSTR);
typedef LPSTR (RDSCALL *RDS_SSSSI)(LPSTR,LPSTR,LPSTR,int);
typedef BOOL (RDSCALL *RDS_BCb4pVI)(RDS_IpV,LPVOID,int*);
typedef LPVOID (RDSCALL *RDS_pVpVpS)(LPVOID,LPSTR*);
typedef void (RDSCALL *RDS_VSII)(LPSTR,int,int);
typedef LPVOID (RDSCALL *RDS_pVpVS)(LPVOID,LPSTR);
typedef HINSTANCE (RDSCALL *RDS_HiV)(void);
typedef void (RDSCALL *RDS_VSBII)(LPSTR,BOOL,int,int);
typedef BOOL (RDSCALL *RDS_BBhDvpV)(RDS_BHANDLE,RDS_PDYNVARLINK,LPVOID);
typedef RDS_BHANDLE (RDSCALL *RDS_BhDvCb5pV)(RDS_PDYNVARLINK,RDS_BBhDvpV,LPVOID);
typedef BOOL (RDSCALL *RDS_BSSB)(LPSTR,LPSTR,BOOL);
typedef void (RDSCALL *RDS_VHoIIpD)(RDS_HOBJECT,int,int,double*);
typedef void (RDSCALL *RDS_VHoSDpD)(RDS_HOBJECT,LPSTR,double,double*);
typedef int (RDSCALL *RDS_ISISB)(LPSTR,int,LPSTR,BOOL);
typedef int (RDSCALL *RDS_IISB)(int,LPSTR,BOOL);
typedef BOOL (RDSCALL *RDS_BIDwISpVDw)(int,DWORD,int,LPSTR,LPVOID,DWORD);
typedef BOOL (RDSCALL *RDS_BIDwISpVDwNsNb)(int,DWORD,int,LPSTR,LPVOID,DWORD,RDS_NETSTATION,RDS_NETBLOCK);
typedef LPVOID (RDSCALL *RDS_pVBhIpI)(RDS_BHANDLE,int,int*);
typedef BOOL (RDSCALL *RDS_BBhIpVI)(RDS_BHANDLE,int,LPVOID,int);
typedef void (RDSCALL *RDS_VIIDw)(int,int,DWORD);
typedef BOOL (RDSCALL *RDS_BDwpIpI)(DWORD,int*,int*);
typedef int (RDSCALL *RDS_IBhSSI)(RDS_BHANDLE,LPSTR,LPSTR,int);
typedef void (RDSCALL *RDS_VHoFsfd)(RDS_HOBJECT,RDS_PFORMSERVFUNCDATA);
typedef BOOL (RDSCALL *RDS_BHoCb6)(RDS_HOBJECT,RDS_VHoFsfd);
typedef void (RDSCALL *RDS_VBhIpVDw)(RDS_BHANDLE,int,LPVOID,DWORD);
typedef void (RDSCALL *RDS_VBhIpVDwDw)(RDS_BHANDLE,int,LPVOID,DWORD,DWORD);
typedef void (RDSCALL *RDS_VHoII)(RDS_HOBJECT,int,int);
typedef BOOL (RDSCALL *RDS_BBhB)(RDS_BHANDLE,BOOL);
typedef RDS_HOBJECT (RDSCALL *RDS_HoBhS)(RDS_BHANDLE,LPSTR);
typedef RDS_HOBJECT (RDSCALL *RDS_HoBh)(RDS_BHANDLE);
typedef LPSTR (RDSCALL *RDS_SDwS)(DWORD,LPSTR);
typedef LPSTR (RDSCALL *RDS_SVh)(RDS_VHANDLE);
typedef int (RDSCALL *RDS_IHoISSDwS)(RDS_HOBJECT,int,LPSTR,LPSTR,DWORD,LPSTR);
typedef BOOL (RDSCALL *RDS_BVhpVVhpV)(RDS_VHANDLE,LPVOID,RDS_VHANDLE,LPVOID);
typedef LPVOID (RDSCALL *RDS_pVBhIBpI)(RDS_BHANDLE,int,BOOL,int*);
typedef void (RDSCALL *RDS_VHoBhB)(RDS_HOBJECT,RDS_BHANDLE,BOOL);
typedef int (RDSCALL *RDS_ISpI)(LPSTR,int*);
typedef LPSTR (RDSCALL *RDS_SIIpI)(int,int,int*);
typedef void (RDSCALL *RDS_VSpD)(LPSTR,double*);
typedef LPSTR (RDSCALL *RDS_SDIpI)(double,int,int*);
typedef LPSTR (RDSCALL *RDS_SIII)(int,int,int);
typedef LPSTR (RDSCALL *RDS_SII)(int,int);
typedef BOOL (RDSCALL *RDS_BBhSS)(RDS_BHANDLE,LPSTR,LPSTR);
typedef int (RDSCALL *RDS_IMd)(RDS_PMOUSEDATA);
typedef void (RDSCALL *RDS_VBhBIIII)(RDS_BHANDLE,BOOL,int,int,int,int);
typedef BOOL (RDSCALL *RDS_BpVIIDwSS)(LPVOID,int,int,DWORD,LPSTR,LPSTR);
typedef BOOL (RDSCALL *RDS_BBhIIpBpBpB)(RDS_BHANDLE,int,int,BOOL*,BOOL*,BOOL*);
typedef BOOL (RDSCALL *RDS_BBhIIBBB)(RDS_BHANDLE,int,int,BOOL,BOOL,BOOL);
typedef void (RDSCALL *RDS_VBhIIII)(RDS_BHANDLE,int,int,int,int);
typedef BOOL (RDSCALL *RDS_BChBrB)(RDS_CHANDLE,RDS_PBLOCKDIMENSIONS,BOOL);
typedef BOOL (RDSCALL *RDS_BChI)(RDS_CHANDLE Conn,int LayerId);
typedef BOOL (RDSCALL *RDS_BBhDwFbei)(RDS_BHANDLE,DWORD,RDS_PFINDBYEXTIDDATA);
typedef RDS_HOBJECT (RDSCALL *RDS_HoIIIIIIS)(int,int,int,int,int,int,LPSTR);
typedef BOOL (RDSCALL *RDS_BHoPnd)(RDS_HOBJECT,RDS_PPANDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BBhIpLfDwpI)(RDS_BHANDLE,int,LOGFONT*,DWORD,int*);
typedef void (RDSCALL *RDS_VpFsD)(RDS_PSERVFONTPARAMS,double);
typedef BOOL (RDSCALL *RDS_BSpSpFs)(LPSTR,LPSTR*,RDS_PSERVFONTPARAMS);
typedef LPSTR (RDSCALL *RDS_SpFspI)(RDS_PSERVFONTPARAMS,int*);
typedef void (RDSCALL *RDS_VpSSB)(LPSTR*,LPSTR,BOOL);
typedef void (RDSCALL *RDS_VMhDw)(RDS_MENUITEM,DWORD);
typedef void (RDSCALL *RDS_VSDwII)(LPSTR,DWORD,int,int);
typedef RDS_MENUITEM (RDSCALL *RDS_MhSDwII)(LPSTR,DWORD,int,int);
typedef BOOL (RDSCALL *RDS_BBhBrDw)(RDS_BHANDLE,RDS_PBLOCKDIMENSIONS,DWORD);
typedef void (RDSCALL *RDS_VHoIDwDw)(RDS_HOBJECT,int,DWORD,DWORD);
typedef LPSTR (RDSCALL *RDS_SBhS)(RDS_BHANDLE,LPSTR);
typedef LPSTR (RDSCALL *RDS_SBh)(RDS_BHANDLE);
typedef LPSTR (RDSCALL *RDS_SSpSpSIDw)(LPSTR,LPSTR*,LPSTR*,int,DWORD);
typedef LPSTR (RDSCALL *RDS_SS)(LPSTR);
typedef LPSTR (RDSCALL *RDS_SSDwSSS)(LPSTR,DWORD,LPSTR,LPSTR,LPSTR);
typedef LPSTR (RDSCALL *RDS_SSDwSpI)(LPSTR,DWORD,LPSTR,int*);
typedef LPSTR (RDSCALL *RDS_SIS)(int,LPSTR);
typedef int (RDSCALL *RDS_IBhII)(RDS_BHANDLE,int,int);
typedef BOOL (RDSCALL *RDS_BHoSCSI)(RDS_HOBJECT,LPSTR,char,LPSTR,int);
typedef BOOL (RDSCALL *RDS_BpBipBtS)(int,BITMAPINFO*,BYTE*,LPSTR);
typedef int (RDSCALL *RDS_IIEdpV)(int,RDS_PTOOLMODULEDATA,LPVOID);
typedef void (RDSCALL *RDS_VDw)(DWORD);
typedef int (RDSCALL *RDS_IDwSSIS)(DWORD,LPSTR,LPSTR,int,LPSTR);
typedef BOOL (RDSCALL *RDS_BS1d)(RDS_PSAVESYSBMPDATA);
typedef BOOL (RDSCALL *RDS_BIDwBr)(int,DWORD,RDS_PBLOCKDIMENSIONS);
typedef DWORD (RDSCALL *RDS_DwV)(void);
typedef void (RDSCALL *RDS_VIDw)(int,DWORD);
typedef BOOL (RDSCALL *RDS_BHoHoDw)(RDS_HOBJECT,RDS_HOBJECT,DWORD);
typedef void (RDSCALL *RDS_VIIB)(int,int,BOOL);
typedef BOOL (RDSCALL *RDS_BpVII)(LPVOID,int);
typedef BOOL (RDSCALL *RDS_BII)(int,int);
typedef BOOL (RDSCALL *RDS_BBhII)(RDS_BHANDLE,int,int);
typedef int (RDSCALL *RDS_ISSB)(LPSTR,LPSTR,BOOL);
typedef int (RDSCALL *RDS_ICh)(RDS_CHANDLE);
typedef int (RDSCALL *RDS_IChIpSpIpIpDpS)(RDS_CHANDLE,int,LPSTR*,int*,int*,double*,LPSTR*);
typedef int (RDSCALL *RDS_IChIpIpDpS)(RDS_CHANDLE,int,int*,double*,LPSTR*);
typedef BOOL (RDSCALL *RDS_BChIIIDS)(RDS_CHANDLE,int,int,int,double,LPSTR);
typedef BOOL (RDSCALL *RDS_BSBhBhSpV)(LPSTR,RDS_BHANDLE,RDS_BHANDLE,LPSTR,LPVOID);
typedef int (RDSCALL *RDS_IBhDwCb7pV)(RDS_BHANDLE,DWORD,RDS_BSBhBhSpV,LPVOID);
typedef int (RDSCALL *RDS_IChIpSpIpIpDpSpDw)(RDS_CHANDLE,int,LPSTR*,int*,int*,double*,LPSTR*,DWORD*);
typedef int (RDSCALL *RDS_IChIpIpDpSpDw)(RDS_CHANDLE,int,int*,double*,LPSTR*,DWORD*);
typedef BOOL (RDSCALL *RDS_BChIIIDSDw)(RDS_CHANDLE,int,int,int,double,LPSTR,DWORD);
typedef void (RDSCALL *RDS_VHoISpV)(RDS_HOBJECT,int,LPSTR,LPVOID);
typedef LPVOID (RDSCALL *RDS_pVHoIpS)(RDS_HOBJECT,int,LPSTR*);
typedef LPVOID (RDSCALL *RDS_pVBhpIDw)(RDS_BHANDLE,int*,DWORD);
typedef void (RDSCALL *RDS_VChBB)(RDS_CHANDLE,BOOL,BOOL);
typedef BOOL (RDSCALL *RDS_BBhpVIDw)(RDS_BHANDLE,LPVOID,int,DWORD);
typedef RDS_HOBJECT (RDSCALL *RDS_HoCh)(RDS_CHANDLE);
typedef void (RDSCALL *RDS_VSDw)(LPSTR,DWORD);
typedef void (RDSCALL *RDS_VSSISpVDw)(LPSTR,LPSTR,int,LPSTR,LPVOID,DWORD);
typedef BOOL (RDSCALL *RDS_BSSpIpSppVpDw)(LPSTR,LPSTR,int*,LPSTR*,LPVOID*,DWORD*);
typedef int (RDSCALL *RDS_IDwSSS2)(DWORD,LPSTR,LPSTR,RDS_PTOOLEXTCALLDATA);
typedef void (RDSCALL *RDS_VS2)(RDS_PTOOLEXTCALLDATA);
typedef BOOL (RDSCALL *RDS_BBhpIpI)(RDS_BHANDLE,int*,int*);
typedef void (RDSCALL *RDS_VHoDwSS)(RDS_HOBJECT,DWORD,LPSTR,LPSTR);
typedef int (RDSCALL *RDS_IBhIpI)(RDS_BHANDLE,int,int*);
typedef int * (RDSCALL *RDS_pIS)(LPSTR);
typedef void (RDSCALL *RDS_VBhDw)(RDS_BHANDLE,DWORD);
typedef BOOL (RDSCALL *RDS_BSIS)(LPSTR,int,LPSTR);
typedef BOOL (RDSCALL *RDS_BBhpBtrpFs)(RDS_BHANDLE,RDS_PBLOCKTEXTRECTDATA,RDS_PSERVFONTPARAMS);
typedef BOOL (RDSCALL *RDS_BBhpBtrpFsDw)(RDS_BHANDLE,RDS_PBLOCKTEXTRECTDATA,RDS_PSERVFONTPARAMS,DWORD);
typedef void (RDSCALL *RDS_VHoBhDw)(RDS_HOBJECT,RDS_BHANDLE,DWORD);
typedef void (RDSCALL *RDS_VHoChDw)(RDS_HOBJECT,RDS_CHANDLE,DWORD);
typedef void (RDSCALL *RDS_VDwSSS)(DWORD,LPSTR,LPSTR,LPSTR);
typedef BOOL (RDSCALL *RDS_BBhDw)(RDS_BHANDLE,DWORD);
typedef void (RDSCALL *RDS_VIIHb)(int,int,HBITMAP);
typedef BOOL (RDSCALL *RDS_BBhpIpIpIpIpIpI)(RDS_BHANDLE,int*,int*,int*,int*,int*,int*);
typedef RDS_HOBJECT (RDSCALL *RDS_HoS)(LPSTR);
typedef RDS_BHANDLE (RDSCALL *RDS_BhHoI)(RDS_HOBJECT,int,BOOL*);
typedef int (RDSCALL *RDS_IHoIpVDw)(RDS_HOBJECT,int,LPVOID,DWORD);
typedef void (RDSCALL *RDS_VDcIIDw)(HDC,int,int,DWORD);
typedef BOOL (RDSCALL *RDS_BBhSDwS)(RDS_BHANDLE,LPSTR,DWORD,LPSTR);
typedef RDS_BHANDLE (RDSCALL *RDS_BhIBh)(RDS_BHANDLE,int,RDS_BHANDLE);
typedef RDS_CHANDLE (RDSCALL *RDS_ChICh)(RDS_CHANDLE,int,RDS_CHANDLE);
typedef BOOL (RDSCALL *RDS_BChDwSIBII)(RDS_CHANDLE,DWORD,LPSTR,int,BOOL,int,int);
typedef BOOL (RDSCALL *RDS_BSSpSpI)(LPSTR,LPSTR,LPSTR*,int*);
typedef LPVOID (RDSCALL *RDS_pVBhpDw)(RDS_BHANDLE,DWORD*);
typedef BOOL (RDSCALL *RDS_BBhpVDw)(RDS_BHANDLE,LPVOID,DWORD);
typedef RDS_PSHAREDMEMPARAMS (RDSCALL *RDS_SmISDw)(int,LPSTR,DWORD);
typedef RDS_PSHAREDMEMPARAMS (RDSCALL *RDS_SmISS)(int,LPSTR,LPSTR);
typedef void (RDSCALL *RDS_VSm)(RDS_PSHAREDMEMPARAMS);
typedef BOOL (RDSCALL *RDS_BBhSmpV)(RDS_BHANDLE,RDS_PSHAREDMEMPARAMS,LPVOID);
typedef RDS_BHANDLE (RDSCALL *RDS_BhSmCb8pV)(RDS_PSHAREDMEMPARAMS,RDS_BBhSmpV,LPVOID);
typedef LPSTR (RDSCALL *RDS_SSIISpI)(LPSTR,int,int,LPSTR,int*);
typedef BOOL (RDSCALL *RDS_BChDwSIBIIB)(RDS_CHANDLE,DWORD,LPSTR,int,BOOL,int,int,BOOL);
typedef DWORD (RDSCALL *RDS_DwBhI)(RDS_BHANDLE,int);
typedef int (RDSCALL *RDS_IBhDw)(RDS_BHANDLE,DWORD);
typedef void (RDSCALL *RDS_VHoIIIS)(RDS_HOBJECT,int,int,int,LPSTR);
typedef LPSTR (RDSCALL *RDS_SHoIIIpB)(RDS_HOBJECT,int,int,int,BOOL*);
typedef void (RDSCALL *RDS_VHoIISDwI)(RDS_HOBJECT,int,int,LPSTR,DWORD,int);
typedef void (RDSCALL *RDS_VHoIIII)(RDS_HOBJECT,int,int,int,int);
typedef int (RDSCALL *RDS_IHoIIIpB)(RDS_HOBJECT,int,int,int,BOOL*);
typedef void (RDSCALL *RDS_VHoIIID)(RDS_HOBJECT,int,int,int,double);
typedef double (RDSCALL *RDS_DHoIIIpB)(RDS_HOBJECT,int,int,int,BOOL*);
typedef BOOL (RDSCALL *RDS_BHoIIIpD)(RDS_HOBJECT,int,int,int,double*);
typedef BOOL (RDSCALL *RDS_BHoIII)(RDS_HOBJECT,int,int,int);
//---------------------------------------------------------------------------

// �������� ����������-���������� �� ��������� ������� ��������� �
// "RdsFunc.h" � ��������������� ����� "RdsFunc_.h"
//---------------------------------------------------------------------------
#endif
