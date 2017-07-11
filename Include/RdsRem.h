//---------------------------------------------------------------------------

#ifndef RdsRemH
#define RdsRemH
//---------------------------------------------------------------------------
// ��������� Windows ��� �������� ���� ������� � �����������
#define RDS_WM_REMOTECALL (WM_USER+2)
//---------------------------------------------------------------------------
// ������ ������� RDS_REMOTECALL_DATA::EventFlags (�� ����� ����� �������)
#define RDS_REMOTECALL_EVENTFLAGSSIZE   255
//---------------------------------------------------------------------------

#include <pshpack1.h> // ��������� ������� �������� ��������


// ������ � ����� ������ (��� ��������� ����� � �������� ����������)
// ������ ��������� ������������
typedef struct
{
  DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� �������)
  HWND ServerWindow;    // ������� ���� �������, ���������� RMWM_REMOTECALL ���
                        // ������ ������ (��������������� �������� ��� �������,
                        // ������������ � NULL ��� ����������)
  HWND ContrWindow;     // ������� ���� �����������, ���������� RMWM_REMOTECALL
                        // ��� ������������� ������� (��������������� ������������,
                        // ������������ � NULL ��� ��� ����������)
  DWORD ContrProcessId; // ������������� ��������-�����������   	
  // ����� ��� ������� (��� ������ � ������������) - ��������������� ������������
  HANDLE Command_read;  // ��������� ������ ������� �� �����������
  HANDLE Result_write;  // ������� ����������� ������� �����������
  HANDLE Event_write;   // �������� ������ �������
  // ����� ������������� ������� (1 - ���������, 0 - ���������) - ��������������� ������������
  unsigned char EventFlags[RDS_REMOTECALL_EVENTFLAGSSIZE];
} RDS_REMOTECALL_DATA;
typedef RDS_REMOTECALL_DATA *RDS_PREMOTECALL_DATA;
//---------------------------------------------------------------------------
// ������� �����������
#define RDS_RCMD_LOADSYSTEMFROMFILE    10       // ��������� ������� �� �����
#define RDS_RCMD_STARTCALCPROCESS      11       // ��������� ������
#define RDS_RCMD_STOPCALCPROCESS       12       // ���������� ������
#define RDS_RCMD_SETEDITMODE           13       // ������� � ����� ��������������
#define RDS_RCMD_SETCALCMODE           14       // ������� � ����� �������
#define RDS_RCMD_ENABLERUNINTERFACE    15       // ���������/��������� ������������ ������ �����
#define RDS_RCMD_SHOWBLOCK             16       // �������� ���� �� ������� �����
#define RDS_RCMD_CALLBLOCK             17       // ������� ������ ����� �� ������� �����
#define RDS_RCMD_SETEXITMODE           18       // ��������� ���������� ������
#define RDS_RCMD_SETCONTROLLERNAME     19       // ���������� ��� �����������
#define RDS_RCMD_SHOWMAINWINDOW        20       // ��������/�������� ������� ����
#define RDS_RCMD_MINIMIZEAPPLICATION   21       // �������� ����������
#define RDS_RCMD_RESTOREAPPLICATION    22       // ���������� ����������
#define RDS_RCMD_LEAVE                 23       // ��������� �����, ������� ���������� ��������
#define RDS_RCMD_SAVESYSTEMTOFILE      24       // �������� ������� � ����
#define RDS_RCMD_CLOSEALLEXCEPTROOT    25       // ������� ��� ���� ����� ��������
#define RDS_RCMD_SETSTRING             26       // ���������� ������ � ����� ���������������
#define RDS_RCMD_SETAUTOSAVE           27       // ��������/��������� ��������������
#define RDS_RCMD_SETMODFLAG            28       // ���������� ���� ������������ �����
#define RDS_RCMD_GETMODFLAG            29       // �������� ���� ������������ �����
#define RDS_RCMD_CLOSEAPPLICATION      30       // ��������� ����������
#define RDS_RCMD_ENABLEEDITMODE        31       // ��������� ����� ��������������
#define RDS_RCMD_ENABLECALCMODE        32       // ��������� ����� �������������
#define RDS_RCMD_ENABLERUN             33       // ��������� ������ �������
#define RDS_RCMD_ENABLEPROPEDIT        34       // ��������� �������������� ���������� ������
#define RDS_RCMD_ENABLEOPTIONS         35       // ��������� ��������� �������� RDS
#define RDS_RCMD_BRINGTOFRONT          36       // �������� RDS �� �������� ����
#define RDS_RCMD_ENABLEWINREFRESH      37       // ��������� ���������� ����
  // ����� ��� RDS_RCMD_ENABLEWINREFRESH
  #define RDS_RCMD_ENABLEWINREFRESH_EN  1       // ���������/���������
  #define RDS_RCMD_ENABLEWINREFRESH_RC  2       // � ������������ ��� ���
#define RDS_RCMD_CLOSEALLMODAL         38       // ������� ��� ��������� ����
#define RDS_RCMD_CHECKMODAL            39       // ��������� ������� �������� ��������� ����
#define RDS_RCMD_SETVIEWPORT           40       // ���������� ���� ������
#define RDS_RCMD_RELEASEVIEWPORT       41       // ������� ���� ������
#define RDS_RCMD_SETVPPARAMS           42       // ���������� ��������� ���� ������
#define RDS_RCMD_UPDATEVIEWPORT        43       // �������� ���� ������
  // ����� ��� RDS_RCMD_UPDATEVIEWPORT
  #define RDS_RCMD_UPDATEVIEWPORT_WAIT  1       // �������� � ����� ����������
#define RDS_RCMD_GETVPSYSAREA          44       // �������� ������� ������� ������� ���������� �� ���� ������
#define RDS_RCMD_SETVIEWPORTRECT       45       // ���������� ������� ������� ������
#define RDS_RCMD_ENABLEUI              46       // ��������� (���������) ���������������� ���������
#define RDS_RCMD_RESETCALC             47       // �������� ������
#define RDS_RCMD_MOUSEOP               48       // ������� ������� �� "����" � ������� ������
  // ����� ��� RDS_RCMD_MOUSEOP
  //  631
  //  4268421
  // 76543210
  // sssbbboo
  #define RDS_RCMD_MOUSEOP_OPERMASK     3       // ����� ��������
    #define RDS_RCMD_MOUSEOP_DOWN       0       // ������� ������
    #define RDS_RCMD_MOUSEOP_UP         1       // ���������� ������
    #define RDS_RCMD_MOUSEOP_MOVE       2       // �����������
    #define RDS_RCMD_MOUSEOP_DBL        3       // ������� ������
  #define RDS_RCMD_MOUSEOP_BTNMASK      (4+8+16)// ����� ������
    #define RDS_RCMD_MOUSEOP_LEFT       4       // ����� ������
    #define RDS_RCMD_MOUSEOP_RIGHT      8       // ������ ������
    #define RDS_RCMD_MOUSEOP_MIDDLE     16      // ������� ������
  #define RDS_RCMD_MOUSEOP_SHMASK       (32+64+128) // ����� ������
    #define RDS_RCMD_MOUSEOP_ALT        32      // Alt
    #define RDS_RCMD_MOUSEOP_CTRL       64      // Ctrl
    #define RDS_RCMD_MOUSEOP_SHIFT      128     // Shift
#define RDS_RCMD_GETVPPARAMS            49      // �������� ��������� ���� ������
#define RDS_RCMD_GETMENUITEMS           50      // �������� �������� ������������ ���� �����
  // �������� ��� RDS_RCMD_GETMENUITEMS
  #define RDS_RCMD_GETMENUITEMS_BLK     0       // ���� �����
  #define RDS_RCMD_GETMENUITEMS_SYS     1       // ���� ���������� ����� ���� ����������
  #define RDS_RCMD_GETMENUITEMS_MAIN    2       // ���� "�������|�������������"
  // ����� ��� ���������� ������� ����
  #define RDS_RCMD_GETMENUITEMSF_EN     1       // ��������
  #define RDS_RCMD_GETMENUITEMSF_VIS    2       // �����
  #define RDS_RCMD_GETMENUITEMSF_CHK    4       // �������
  #define RDS_RCMD_GETMENUITEMSF_DIV    8       // �����������
  #define RDS_RCMD_GETMENUITEMSF_KEY    16      // ���� �������
#define RDS_RCMD_VPBLOCKATPOS           51      // ���� � ��������� ����� ���� ������
#define RDS_RCMD_BLOCKMENUCLICK         52      // ����� ������ ���� �����
#define RDS_RCMD_GETVPMOUSELEVEL        53      // �������� ������� ������� ������ ���� �� "����"
#define RDS_RCMD_VPPOPUPHINT            54      // �������� ��������� ����������� ���������
#define RDS_RCMD_ENABLESYSWIN           55      // ��������� (���������) �������� ���� ���������
#define RDS_RCMD_VPGETBLOCKNAME         56      // �������� ��� ����������, ��������� � ����� ������
#define RDS_RCMD_LOADSYSTEMFROMMEM      57      // ��������� ������� �� ������
  // ����� ��� RDS_RCMD_LOADSYSTEMFROMMEM
  #define RDS_RCMD_LOADSYSFROMMEM_SP    1       // ���������� ������������� �����
  #define RDS_RCMD_LOADSYSFROMMEM_MODE  (2+4)   // ����� ������ ��������
    #define RDS_RCMD_LOADSYSMEM_MAPPING   0     // ����� ����������� ������� ������
    #define RDS_RCMD_LOADSYSMEM_PIPE      2     // ��������������� �� �����
    #define RDS_RCMD_LOADSYSMEM_RESERVED  4     // ��������������� �� �������
#define RDS_RCMD_SETOPTIONS             58      // ���������� ���������
#define RDS_RCMD_GETOPTIONS             59      // �������� ���������
#define RDS_RCMD_FINDBLOCK              60      // ����� ���� �� ������� �����
#define RDS_RCMD_LISTBLOCKS             61      // �������� ������ ������
#define RDS_RCMD_CLOSEALL               62      // ������� ��� ���� ���������
#define RDS_RCMD_SETREMOTESAVE          63      // ���������� ���������� ����� ��
#define RDS_RCMD_GETSYSTEMCONTENT       64      // �������� ����� ���� �������
  #define RDS_RCMD_GETSYSCONT_PIPE      1         // ��������������� �� �����
  #define RDS_RCMD_GETSYSCONT_FILE      2         // ����� ��������� ����
#define RDS_RCMD_SETBLOCKVAR            65      // ���������� �������� ���������� �����
#define RDS_RCMD_GETBLOCKVAR            66      // �������� �������� ���������� �����
#define RDS_RCMD_SETVPZOOMRECT          67      // ���������� ������������ ����� �� ��������������
  #define RDS_RCMD_SETVPZR_100           1        // ���� - ������� �������� � ������� �����������
#define RDS_RCMD_VPFIT                  68      // ���������� ������� ���, ����� ��� ����� ������
#define RDS_RCMD_SETTRACELEVEL          69      // ���������� ������� �����������
#define RDS_RCMD_SETTRACEFILE           70      // ���������� ���� ������� �����������
#define RDS_RCMD_CLEARSYSTEM            71      // �������� �������
#define RDS_RCMD_SAVETAGGED             72      // ��������� � �����������
#define RDS_RCMD_LOADTAGGED             73      // ��������� � �����������
#define RDS_RCMD_SETREMOTELOAD          74      // ���������� �������� ����� ��
#define RDS_RCMD_SETPROGRESSDELAY       75      // ���������� �������� ������� ���������
#define RDS_RCMD_FINDOPSETPROVIDERS     76      // ����� �����, �������������� ������������� ����� ������� ��
  #define RDS_RCMD_FINDOPSET_RECURSIVE    1       // ���� - � ���������� �������
  #define RDS_RCMD_FINDOPSET_SELF         2       // ���� - ��������� ���������� ����������
#define RDS_RCMD_DELETEXCHGSHAREDMEM    77      // ������� �������������� ����������� ������� ������ �������
#define RDS_RCMD_SAVETAGGEDEX           78      // ��������� � ����������� - ����������� (����������� ����������� �������)
#define RDS_RCMD_LOADTAGGEDEX           79      // ��������� � ����������� - ����������� (����������� ����������� �������)
  #define RDS_RCMD_TAGGED_SHAREDMEM       1       // ���� - �� ����, � ������� ������
  #define RDS_RCMD_TAGGED_DBLBUF          2       // ��� ���������� - ������������ �������������� �����
#define RDS_RCMD_EXTIDBYNAME            80      // �������� ������� ������������� (DWORD) ����� �� ������� �����
#define RDS_RCMD_NAMEBYEXTID            81      // �������� ������ ��� ����� �� �������� �������������� (DWORD)
#define RDS_RCMD_GETMODE                82      // �������� ����� ������
#define RDS_RCMD_SAVETAGGEDEXEX         83      // ��������� � ����������� - ����������� (����������� ����������� �������, ������������)
#define RDS_RCMD_CLOSESYSWIN            85      // ������� ���� ���������� (��������, �� ������������)
// 84, 86, 87 ������ �� ������������
#define RDS_RCMD_KEYOP                  88      // ������� ������� �� ���������� � ������� ������
  // ������� ��� RDS_RCMD_KEYOP
  #define RDS_RCMD_KEYOP_DOWN           0       // ������� ������
  #define RDS_RCMD_KEYOP_UP             1       // ���������� ������
  // ����� ��� RDS_RCMD_KEYOP
  //  631
  //  4268421
  // 76543210
  // sssbbb..
  #define RDS_RCMD_KEYOP_BTNMASK      (4+8+16)// ����� ������ ����
    #define RDS_RCMD_KEYOP_LEFT       4       // ����� ������
    #define RDS_RCMD_KEYOP_RIGHT      8       // ������ ������
    #define RDS_RCMD_KEYOP_MIDDLE     16      // ������� ������
  #define RDS_RCMD_KEYOP_SHMASK       (32+64+128) // ����� ����������� ������
    #define RDS_RCMD_KEYOP_ALT        32      // Alt
    #define RDS_RCMD_KEYOP_CTRL       64      // Ctrl
    #define RDS_RCMD_KEYOP_SHIFT      128     // Shift
#define RDS_RCMD_TOOL                 89      // ������� ������ ����������
//---------------------------------------------------------------------------
// ������� �������
#define RDS_REVENT_CMDRESULTREADY       0       // ��������� - ����� ��������� ��������� �������
#define RDS_REVENT_CALCSTART           10       // ������� ������� ������� �������������
#define RDS_REVENT_CALCSTOP            11       // ������� ������� ���������� �������������
#define RDS_REVENT_BLOCKMSG            12       // ��������� �� �����
#define RDS_REVENT_NEWFILE             13       // ��������� ����������� ���� (�������� ��� ������ �����)
  // �������� ��� RDS_REVENT_NEWFILE
  #define RDS_REVENT_NEWFILE_NEW        0       // ������� ����� �������
  #define RDS_REVENT_NEWFILE_TEMPLATE   1       // ������� �� �������
  #define RDS_REVENT_NEWFILE_LOAD       2       // ��������� �� �����
#define RDS_REVENT_EDITMODE            14       // ������������ ������� ����� ��������������
#define RDS_REVENT_CALCMODE            15       // ������������ ������� ����� �������������
#define RDS_REVENT_SAVEFILE            16       // ������������ ��������� ����� (����� ����������)
#define RDS_REVENT_LOADREQUEST         17       // ������������ ����������� �������� ����� (���� ��������� ������ ��������)
#define RDS_REVENT_PROGRESS            18       // �������� ��������/����������
//---------------------------------------------------------------------------
#define RDS_REMMAPPING_SIZE  (sizeof(RDS_REMOTECALL_DATA))  // ������ ����� ������
//---------------------------------------------------------------------------
// ������� ������������ �� ���������
#include <poppack.h>
//---------------------------------------------------------------------------
#endif
