//---------------------------------------------------------------------------
// ��������, ����������� ��� ������� �������������� RDS
//---------------------------------------------------------------------------

#ifndef RdsCompH
#define RdsCompH
//---------------------------------------------------------------------------
#ifndef RdsDefH
  // ��� ���������� ���������� ���������� �������� �� RdsDef.h
  #include "RdsDef.h"
#endif

#include <pshpack1.h> // ��������� ������� �������� ��������

// ��������� (������ ������)
#define RDS_COMPM_INIT          0 // ������������� ������
#define RDS_COMPM_CLEANUP       1 // ������� (���������� ����� ��������� ������)
#define RDS_COMPM_MODELINIT     2 // ������������� ������
#define RDS_COMPM_MODELCLEANUP  3 // ������� ������ (���������� ����� ����������� ������)
#define RDS_COMPM_SETUP         5 // ����� ������� ���������
#define RDS_COMPM_ATTACHBLOCK   6 // ���������� ����� � ������
#define RDS_COMPM_DETACHBLOCK   7 // ���������� ����� �� ������
#define RDS_COMPM_PREPARE       8 // ������������� � ���������� � ��������� ��
                                  // �������������
#define RDS_COMPM_COMPILE       9 // �������������
#define RDS_COMPM_STRUCTCHANGE 10 // ��������� �� ��������� ���������
#define RDS_COMPM_OPENEDITOR   11 // ������� ���� ��������� ������
#define RDS_COMPM_CANRENMODEL  12 // ����� �� ������������� ������ (���������� ����� ���������������)
#define RDS_COMPM_MODELRENAMED 13 // ������������� ������ (���������� ����� ��������������)
#define RDS_COMPM_GETOPTIONS   14 // �������� ����� �������������� �������
#define RDS_COMPM_EXECFUNCTION 15 // ��������� ���� �� ������� RDS_COMPFLAG_*
#define RDS_COMPM_SAVESYSTEM   16 // ���������� ����� ����������� �������
#define RDS_COMPM_CANATTACHBLK 17 // ���������� ����� ����������� ����� � ������
                                  // (����� ������ ����� ��� �� ����)
#define RDS_COMPM_SAVEBLOCK    18 // ���������� ����� ����������� ������� ����� � �������
                                  // (����� ��������������� ��� ������ ����� �����������)
#define RDS_COMPM_CLOSEALLWIN  19 // ������� ��� ����������� ����
#define RDS_COMPM_MODECHANGE   20 // ��������� ����� RDS (��������������, �������������, ������)

#define RDS_COMPM_AFTERSAVESYSTEM 21 // ���������� ����� ���������� ������� **** REV2 ****
#define RDS_COMPM_BEGINPASTE   22 // ���������� ������� ������ �� ������ ������ **** REV2 ****
#define RDS_COMPM_ENDPASTE     23 // ��������� ������� ������ �� ������ ������ **** REV2 ****
#define RDS_COMPM_AFTERLOADSYSTEM 24 // ���������� ����� �������� ������� **** REV2 ****
#define RDS_COMPM_MODELPOSTINIT   25 // �������������� ������������� ������ (������ RDS_COMPM_MODELINIT � ��������������� �����������) **** REV2 ****
#define RDS_COMPM_BLOCKDELETE  26 // �������� ����� �� ����� **** REV2 ****
#define RDS_COMPM_ISEDITOROPEN  27 // ������ �� �������� ������ **** REV2 ****
#define RDS_COMPM_BLOCKFROMFILE  28 // ���� ������ ��� �������� �� ���������� ��� ����� **** REV2 ****
#define RDS_COMPM_BLOCKLIBRARYOP 29 // �����-�� �������� � ������ � ���������� **** REV2 ****
#define RDS_COMPM_SAVETEMPGLOBAL 30 // �������� ���������� ������ � ������ ��� ���������� ��� ������ ������ ��� ��������� �������

// ������������ �������� ��������
#define RDS_COMPR_DONE          0 // ���������� ���������� (��� ���� �������)
#define RDS_COMPR_ERROR         1 // ������
#define RDS_COMPR_ERRORNOMSG    2 // ������, �� �������� ��������� (��� ��������� �������)

// ���������� ������������� ������ ��������������
typedef LPVOID RDS_COMPHANDLE;
// ���������� ������������� ������
typedef LPVOID RDS_MODELHANDLE;

// ������ ������ �������������� (�������� ��� ����� ������������� ������)
typedef struct
{ RDS_COMPHANDLE Module;// ������������� ������ (������ ������)
  LPVOID ModuleData;    // ����� ������� ������ ������ (������/������)
  LPSTR DllFullPath;    // ������ ���� � DLL ������ (������ ������)
  LPSTR DllFuncName;    // ��� ������� DLL ������ (������ ������)
  int NModels;          // ����� �������, ��������� � ������ ������� (������ ������)

  LPSTR DllFuncParam;   // ��� ������ ���������� ������ (������ ������) **** REV2 ****
} RDS_COMPMODULEDATA;
typedef RDS_COMPMODULEDATA *RDS_PCOMPMODULEDATA;
//---------------------------------------------------------------------------

// ������ ������ (�������� ��� ����� ������������� ������)
typedef struct
{ RDS_MODELHANDLE Model;// ������������� ������ (������ ������)
  LPSTR ModelName;      // ��� ������ (������ ������)
  LPSTR ModelNameUC;    // ��� ������ � ������� �������� (������ ������)
  LPVOID ModelData;     // ����� ������� ������ ������ (������/������)
  int NBlocks;          // ����� ������, ��������� � ������ ������� (������ ������)
  RDS_COMPHANDLE Module;// ������������� ������ ��������������, � �������� ���������
                        // ������ (������ ������)
  // ���������, ��������������� ������� ��������������
  LPSTR CompDllName;    // DLL ���������������� ������ (������ ������,
                        // ��������� ��������� �������� �� ����� ������ BFCM_COMP_PREPARE)
  LPSTR CompDllFunc;    // ��� ������� ���������������� ������ (������ ������,
                        // ��������� ��������� �������� �� ����� ������ BFCM_COMP_PREPARE)
  BOOL Valid;           // ������� ������������� �������������� (������/������, ���������������
                        // ������� �� ����� ������ BFCM_COMP_PREPARE)
  // �������������� ���������
  LPSTR AltModelName;   // �������������� ��� ������ (������ ������). �������� � ����� � �����
                        // �������������� ������� �������������� ��� ����� ����. ��� �����������
                        // ����� ������ ����� ���� ���������� ����� ������ �������� ������ ������.
                        // ��������������� �������� rdscompSetAltModelName.
  // ����� ���������������� ���� (�� �������������� � �� ����������������)
  int Tag;

  int ModelVersion;     // ������ ������ (��������������� �������) **** REV2 ****
  DWORD ModelVersionDate;// ���� ������ (��������������� �������) **** REV2 ****
} RDS_COMPMODELDATA;
typedef RDS_COMPMODELDATA *RDS_PCOMPMODELDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� RDS_COMPM_CANATTACHBLK
typedef struct
{ LPSTR ModelName;      // ��� ������ (������ ������)
  LPSTR ModelNameUC;    // ��� ������ � ������� �������� (������ ������)
  LPSTR AltModelName;   // �������������� ��� ������ (������ ������)

  RDS_BHANDLE Block;    // ���� (������ ������)

  int AttachReason;     // ������� ����������� ����� � ������ (RDS_COMP_AR_*) (������ ������)

  BOOL ChangeModel;     // ���������� ������ ������ ������ ���� (������)
                        // �� ��������� - FALSE. ��� ����������� ������ ��������� ������
                        // ������ ������� ���������� � TRUE � �������� ����� ��� ������ ���
                        // ������ ������� rdscompAttachDifferentModel

  LPSTR BlockFile;      // ��� ����� ����� ��� NULL (������ ������,
                        // ������ ��� AttachReason==RDS_COMP_AR_LOADFROMFILE ��� RDS_COMP_AR_LOADSYSTEM) **** REV2 ****
  BOOL LoadSingleBlock; // ���� �������� ������ ����� (����������) ������ ����� (������ ������,
                        // ������ ��� AttachReason==RDS_COMP_AR_LOADFROMFILE ��� RDS_COMP_AR_LOADSYSTEM) **** REV2 ****

} RDS_COMPCANATTACHBLKDATA;
typedef RDS_COMPCANATTACHBLKDATA *RDS_PCOMPCANATTACHBLKDATA;
// ������� ����������� ����� (��� AttachReason)
#define RDS_COMP_AR_UNKNOWN             0       // ���������� (���������� ������)
#define RDS_COMP_AR_LOADSYSTEM          1       // �������� ���� �������
#define RDS_COMP_AR_LOADCLIPBRD         2       // �������� ������ �� ������ ������
#define RDS_COMP_AR_LOADFROMFILE        3       // �������� ���������� ����� �� ����������
#define RDS_COMP_AR_LOADUNDO            4       // �������� ��-�� ������ ���������
#define RDS_COMP_AR_MANUALSET           5       // ����� ������ ������ � ���� ����������
                                                // ��� ��������� ��������
#define RDS_COMP_AR_RENAMEMODEL         6       // �������������� ������ (�.�. �����������
                                                // ���� ������ � ���������������)
#define RDS_COMP_AR_LOADPROG            7       // �������� ������ �� ������ ��������� ��������
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ����������� RDS_COMPM_ATTACHBLOCK � RDS_COMPM_DETACHBLOCK
typedef struct
{ RDS_PCOMPMODELDATA Model;     // ������
  RDS_BHANDLE Block;            // ����
  int AttachReason;             // ������� ����������� ����� � ������
                                // (RDS_COMP_AR_*, ������ ��� RDS_COMPM_ATTACHBLOCK)
} RDS_COMPBLOCKOPDATA;
typedef RDS_COMPBLOCKOPDATA *RDS_PCOMPBLOCKOPDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� RDS_COMPM_BLOCKDELETE **** REV2 ****
typedef struct
{ RDS_PCOMPMODELDATA Model;     // ������
  RDS_BHANDLE Block;            // ����
} RDS_COMPBLOCKDELDATA;
typedef RDS_COMPBLOCKDELDATA *RDS_PCOMPBLOCKDELDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� RDS_COMPM_MODELPOSTINIT **** REV2 ****
typedef struct
{ RDS_PCOMPMODELDATA Model;     // ������
  int AttachReason;             // ������� ����������� ������
                                // (RDS_COMP_AR_*)
  LPSTR BlockFile;              // ��� ����� ����� ��� NULL (������ ������,
                                // ������ ��� AttachReason==RDS_COMP_AR_LOADFROMFILE) **** REV2 ****
} RDS_COMPMPOSTINITDATA;
typedef RDS_COMPMPOSTINITDATA *RDS_PCOMPMPOSTINITDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� RDS_COMPM_OPENEDITOR � RDS_COMPM_ISEDITOROPEN **** REV2 ****
typedef struct
{ RDS_PCOMPMODELDATA Model;     // ������ ������
  RDS_BHANDLE Block;            // ����, ��� �������� ������ �������� (������ ��� RDS_COMPM_OPENEDITOR)
} RDS_OPENEDITORDATA;
typedef RDS_OPENEDITORDATA *RDS_POPENEDITORDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� RDS_COMPM_PREPARE
typedef struct
{ RDS_PCOMPMODELDATA Model;     // ������ ����������� � ���������������� ������
  BOOL Rebuild;                 // TRUE ���� �������� �� ����� (��� ��������������
                                // �������������� ���� ������� ���������� �� ��
                                // ���������)
} RDS_COMPPREPAREDATA;
typedef RDS_COMPPREPAREDATA *RDS_PCOMPPREPAREDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� RDS_COMPM_COMPILE
typedef struct
{ RDS_PCOMPMODELDATA *InvalidModels;    // ������, ������� ����� �������������
                                        // (������ ������� ��������)
  int IMCount;                          // ������ ������� InvalidModels
  BOOL Rebuild;                         // TRUE - �������������� ��������������
                                        // ���� �������
} RDS_COMPILEDATA;
typedef RDS_COMPILEDATA *RDS_PCOMPILEDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� RDS_COMPM_STRUCTCHANGE
typedef struct
{ LPSTR OldStructType;  // ��� ���� ��������� �� ���������
  LPSTR NewStructType;  // ��� ���� ��������� ����� ���������
  BOOL Renamed;         // ��������� ������������� (OldStructType �� ��������� � NewStructType)

} RDS_COMPSTRUCTCHGDATA;
typedef RDS_COMPSTRUCTCHGDATA *RDS_PCOMPSTRUCTCHGDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ����������� RDS_COMPM_CANRENMODEL � RDS_COMPM_MODELRENAMED
typedef struct
{ RDS_PCOMPMODELDATA Model;     // ������ ������, ������� ����� ������������� (��� RDS_COMPM_CANRENMODEL)
                                // ��� ��� ������������� (��� RDS_COMPM_MODELRENAMED)
  LPSTR OldModelName;           // ������ ��� ������
  LPSTR NewModelName;           // ����� ��� ������
  // �������
  BOOL AllowRename;             // �������������� ��������� (������� ��� RDS_COMPM_CANRENMODEL)

} RDS_COMPMODELRENAMEDATA;
typedef RDS_COMPMODELRENAMEDATA *RDS_PCOMPMODELRENAMEDATA;
//---------------------------------------------------------------------------

// �����, ������������ � ����� �� RDS_COMPM_GETOPTIONS
#define RDS_COMPFLAG_FUNCMODELUSERINPUT 1       // ����� ������� ��� ������ �������
#define RDS_COMPFLAG_FUNCMODELBROWSE    2       // �������� ������ "�����"
#define RDS_COMPFLAG_FUNCMODELCREATE    4       // �������� ������ "�������"
#define RDS_COMPFLAG_FUNCMODELSAVEAS    8       // �������� ������ "��������� ���..."

#define RDS_COMPFLAG_CANCHANGESTRUCT    16      // ������ ����� ������ ��������� � ������� ���
                                                // �� ��������� � RDS

// ���������, ������������ ������ � ���������� RDS_COMPM_EXECFUNCTION
typedef struct
{ int Function;         // ������� (RDS_COMPFLAG_FUNC*)
  LPSTR ModelName;      // ���������� ������ ������
  // ��������� ��������� ����� ���� �� ���������� ��� ��������� �������.
  RDS_HOBJECT BlockVars;// ���������� ����� (���� ����, ������ ��� Function==RDS_COMPFLAG_FUNCMODELCREATE)
  int BlockType;        // ��� ����� (RDS_BT* ��� RDS_TUNKNOWN, ��. RdsDef.h)
} RDS_COMPEXECFUNCDATA;
typedef RDS_COMPEXECFUNCDATA *RDS_PCOMPEXECFUNCDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ����������� RDS_COMPM_SAVESYSTEM
// � RDS_COMPM_AFTERSAVESYSTEM **** REV2 ****
typedef struct
{
  LPSTR FileName;       // ��� ������������ �����
  LPSTR OldFileName;    // ��� ����� �� ���������� ��� NULL ��� ������ ����������
                        // ��������� �������
} RDS_COMPSAVESYSTEMDATA;
typedef RDS_COMPSAVESYSTEMDATA *RDS_PCOMPSAVESYSTEMDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� RDS_COMPM_SAVEBLOCK
typedef struct
{ RDS_PCOMPMODELDATA Model;     // ������ ����������� ������
  RDS_BHANDLE Block;            // ����������� ����
  int SaveAction;               // ������� ���������� (RDS_LS_SAVE*)

  BOOL ChangeName;      // �������� ������ ��� ������ (������)
                        // �� ��������� - FALSE. ��� ������ ������ ��������� ������
                        // ������ ������� ���������� � TRUE � �������� ����� ��� ������ ���
                        // ������ ������� rdscompAttachDifferentModel

  LPSTR TargetFile;     // ������ ���� � �����, ���� ����������� ���� ��� ��� �����, ��� NULL **** REV2 ****
  BOOL SaveSingleBlock; // ���� ���������� ������ ����� (����������) � ��������� ���� **** REV2 ****
} RDS_COMPSAVEBLOCKDATA;
typedef RDS_COMPSAVEBLOCKDATA *RDS_PCOMPSAVEBLOCKDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� RDS_COMPM_BLOCKFROMFILE **** REV2 ****
typedef struct
{ RDS_BHANDLE Block;
} RDS_COMPBLOCKFROMFILEDATA;
typedef RDS_COMPBLOCKFROMFILEDATA *RDS_PCOMPBLOCKFROMFILEDATA;
//---------------------------------------------------------------------------

// ���������, ������������ ������ � ���������� RDS_COMPM_SAVETEMPGLOBAL **** REV2 ****
typedef struct
{ RDS_PCOMPMODELDATA Model;     // ������ ����������� ������
  int SaveAction;               // ������� ���������� (RDS_LS_SAVE*)
} RDS_COMPSAVETEMPGLOBALDATA;
typedef RDS_COMPSAVETEMPGLOBALDATA *RDS_PCOMPSAVETEMPGLOBALDATA;
//---------------------------------------------------------------------------

// ������� ������ ������ �������������� - ��� ���������
typedef int(RDSCALL *RDS_COMPENTRYPOINT)(int,RDS_PCOMPMODULEDATA,LPVOID);
//---------------------------------------------------------------------------

// �������� �������� � ����������� ������ **** REV2 ****
// ���������� ������ � ���������� RDS_COMPM_BLOCKLIBRARYOP ��� ����������� �������� rdscompLibraryOperation
typedef struct
{ DWORD servSize;       // ������ ���� ��������� � ������ (���������� ��������� ����� ������� �������)
  int Operation;        // �������� (RDS_LOP_*)
    #define RDS_LOP_NONE        0       // �������� � ����������� ������ �� ������������
    #define RDS_LOP_PASTE       1       // ������� ����� � ���������� �� ������ ������ ��� ����������� ������ � ����������
    #define RDS_LOP_SAVE        2       // ���������� ����� �� ������ � ����
    #define RDS_LOP_COPY        3       // ����������� ������������� �����
    #define RDS_LOP_RENAME      4       // �������������� ������������� �����
    #define RDS_LOP_DELETE      5       // �������� ������������� �����
  LPSTR BlockFilePath;  // ���� � ����� �����, � ������� ���������� ��������
  LPSTR SystemPath;     // ���� � ����� �����, � ������� ����������� ���� ��� RDS_LOP_PASTE
  LPSTR OldBlockFilePath;  // ���� � ��������� ����� ����� ��� RDS_LOP_COPY � RDS_LOP_RENAME
  // ��������� ���� - ������ ��� ������� �� ��������� RDS_COMPM_BLOCKLIBRARYOP
  int Status;          // ��������� (RDS_LOPST_*)
    #define RDS_LOPST_NONE      0       // �� ������� �� ���������, ���� ������ �� ���������
    #define RDS_LOPST_BEGIN     1       // ����� ���������, ���� ������ �� ���������
    #define RDS_LOPST_END       2       // ����� ��������, ���� ������ �� ���������
    #define RDS_LOPST_MODEL     3       // ����� ��� ������, ���� ������ ���������
  LPSTR ModelName;      // ��� ������
  LPSTR AltModelName;   // �������������� ��� ������
} RDS_COMPLIBRARYOPDATA;
typedef RDS_COMPLIBRARYOPDATA *RDS_PCOMPLIBRARYOPDATA;
//---------------------------------------------------------------------------


// ������� ������������ �� ���������
#include <poppack.h>

// ������������ ������� - ���� ����������
typedef void (RDSCALL *RDS_VMhSS)(RDS_MODELHANDLE,LPSTR,LPSTR);
typedef RDS_PCOMPMODELDATA (RDSCALL *RDS_MdChI)(RDS_COMPHANDLE,int);
typedef RDS_BHANDLE (RDSCALL *RDS_BhMhIBd)(RDS_MODELHANDLE,int,RDS_PBLOCKDESCRIPTION descr);
typedef BOOL (RDSCALL *RDS_BChSSB)(RDS_COMPHANDLE,LPSTR,LPSTR,BOOL);
typedef RDS_PCOMPMODELDATA (RDSCALL *RDS_MdChS)(RDS_COMPHANDLE,LPSTR);
typedef BOOL (RDSCALL *RDS_BMhB)(RDS_MODELHANDLE,BOOL);
typedef void (RDSCALL *RDS_VMhS)(RDS_MODELHANDLE,LPSTR);
typedef RDS_PCOMPMODELDATA (RDSCALL *RDS_MdBh)(RDS_BHANDLE);
typedef RDS_PCOMPMODULEDATA (RDSCALL *RDS_CdCh)(RDS_COMPHANDLE);
typedef BOOL (RDSCALL *RDS_BMh)(RDS_MODELHANDLE);
typedef BOOL (RDSCALL *RDS_BLop)(RDS_PCOMPLIBRARYOPDATA);
typedef void (RDSCALL *RDS_VMhB)(RDS_MODELHANDLE,BOOL);
//---------------------------------------------------------------------------
// ������� ��� ��������� ������� ������� � "RdsFunc.h" � "RdsFunc_.h"
//---------------------------------------------------------------------------
#endif
