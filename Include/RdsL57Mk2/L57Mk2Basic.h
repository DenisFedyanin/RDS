//---------------------------------------------------------------------------
// �������������� ������ � ������� ��� ��������� ��������������������� ������
// (����� ������� ������ � �������, ��������� � ���������� ���������)
// ������ 2
//---------------------------------------------------------------------------
#ifndef L57Mk2BasicH
#define L57Mk2BasicH

#ifndef L57_NORDSCLASSES

#ifndef RdsLinInterH
  #include <RdsAC/LinInter.cpp>
#endif

#define L57_ONCEDEBUGMSG(text) static BOOL L57_msg=TRUE; if(L57_msg) { L57_msg=FALSE; rdsMessageBox(text,"Here",MB_OK); }

#endif // #ifndef L57_NORDSCLASSES

//---------------------------------------------------------------------------
class L57_FuncTransfer;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ���������� ����� ����������
//---------------------------------------------------------------------------
// ������� ��� ������������ �������������� ������� �������� ������ ��� ��������� �������
#define L57_GLOBALVAR_PARAMTRANSFERID "IPU.L57.UserFuncTransfer.ParamTransferId"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� ������
//---------------------------------------------------------------------------
// ���� �������� ���������� ���������������� ������� (��� ����������)
#define L57_ISUSERFUNCSOURCENAME "IPU.L57.UserFuncTransfer.IsFuncSource"
//---------------------------------------------------------------------------
// ������� �������
#define L57_ACCEPTUSERFUNCNAME  "IPU.L57.UserFuncTransfer.AcceptFunction"
// ������� ��������� ������ ��� ���������� ������ � �������
typedef void (RDSCALL *L57_CALLFUNCLATERCB)(LPVOID,RDS_BHANDLE,LPVOID);
// ������� ��� ���������� ����������� ������ � ������� � �� ��������
typedef struct
{ // ������� � ������
  L57_CALLFUNCLATERCB CallFunc;
  LPVOID CallObject;
  // ������������� �������� ��� �������� ��������� � ������
  int TransferId;
} L57_AcceptFunc_Param_Queue;
// ��������� ����������
typedef struct
{ DWORD servSize;
  BOOL Immediate;       // TRUE - ����� ��-�� �������������� ������� � ����� �� ������
                        // FALSE - ����� � �������� ������������� ��� ������ �� ������ ��������������
  char **Texts;         // ������ �������
  int TextsCount;       // ����� �������
  int TextIndex;        // ����� �������, ������� ����� �����, ��� -1 ��� ���� �������

  int AcceptIndex;      // ����� �������, ������� ����� ����������, ��� -1 ��� ���� �������

  int DestVarNum;       // ����� ����������-��������������� ����� (���� �����)

  L57_AcceptFunc_Param_Queue *Queue; // ��������� ���������� ������ � �������

} L57_AcceptFunc_Param;
//---------------------------------------------------------------------------
// ������� ��������
#define L57_ACCEPTUSERPARAMNAME "IPU.L57.UserFuncTransfer.AcceptParameter"
// ����� ���� ��������� ��� ����������� ����� rdsRegisterUserName
#define L57_VARNAMESET          "IPU.L57.FuncParamNames"
// �������� �������������� ��������� (��� >=0!)
#define L57_VARSUFFIX_NONE      0       // ���
#define L57_VARSUFFIX_0         1       // "_0"
#define L57_VARSUFFIX_MIN       2       // "_min"
#define L57_VARSUFFIX_MAX       3       // "_max"
#define L57_VARSUFFIX_STEP      4       // "_step"
#define L57_VARSUFFIX_END       5       // "_end"
// ������� ��������� ������ �������������� ��������
typedef void (RDSCALL *L57_VARLISTCB)(LPVOID,int,int,BOOL,RDS_BHANDLE);
// ������� ��������� ������ ��� ���������� ������ � �������
typedef void (RDSCALL *L57_CALLLATERCB)(LPVOID,RDS_BHANDLE,int);
// ��������� ����������
typedef struct
{ DWORD servSize;
  int NameId;   // ������������� ������������������� ����� ���������
  int SuffixId; // �������� ������������� ��������
  void *Data;   // ��������� �� ������
  int Type;     // ��� ������������ ���������� (L57_APPTYPE_*)
    #define L57_APPTYPE_DOUBLE  0       // double (Data - double *)
    #define L57_APPTYPE_RDSMATR 1       // ������� double RDS (Data - ��������� �� ������ � ������ ����������)
    #define L57_APPTYPE_BLOCK   2       // �������������� �������� (Data - NULL)
  int DestVarNum;       // ����� ����������-��������������� �����, �� ������� ������ ��������
  // ��� �������������� ��������
  RDS_BHANDLE SourceBlock;      // ����-�������� ���������
  RDS_BHANDLE DestinationBlock; // ����, ����������� �������� �������� ���������� � ����
  // ������� ��������� ������ � �� ��������-������ ��� ������ �������� ����������
  LPVOID Object;
  L57_VARLISTCB InfFunc;
  // ������� ��� ���������� ����������� ������ � ������� � �� ��������
  L57_CALLLATERCB CallFunc;
  LPVOID CallObject;
  // ������������� �������� ��� �������� ��������� � ������
  int TransferId;
} L57_AcceptParam_Param;
//---------------------------------------------------------------------------
// ���������, ���������� �� �������� �� �������������� ����� (����������)
#define L57_CHECKPARAMNAMETRANS "IPU.L57.UserFuncTransfer.IsParameterTransferredWithFunc"
typedef struct
{ DWORD servSize;
  int FuncId;           // ������������� ��������� � ���� ���������� �������
  int ParamCount;       // ����� ���������� � �������� ����
  int *NameIds;         // �������������� ����
  int *SuffixIds;       // �������������� ���������
  int *Flags;           // ����� �������� (L57_PTR_*)
    #define L57_PTR_UNKNOWN     0       // ����������
    #define L57_PTR_TRANS       1       // ����������
    #define L57_PTR_NOTRANS     2       // �� ����������
  RDS_BHANDLE ThisBlock; // ��� ������� �����: ���� ���� ����� � ���������, ���� ������������ � L57_PTR_NOTRANS
                         // ��� ������ ������: ���� ���� ����� � ���������, ���� ��������� � L57_PTR_TRANS
} L57_CheckParamConn_Param;
//---------------------------------------------------------------------------
// ��������� �������� ����������
#define L57_PREPARECHECKNAME "IPU.L57.UserFuncTransfer.CheckParamTransfer"
typedef struct
{ DWORD servSize;
  int Mode;     // �����
    #define L57_CTM_PREPARE     0       // ���������� (�������� ��������)
    #define L57_CTM_CHECK       1       // ��������� �������������� �������� ����� L57_ACCEPTUSERPARAMNAME
  RDS_BHANDLE DestinationBlock; // ����, ����������� �������� �������� ���������� � ����

  // ������� ��������� ������ � �� ��������-������ ��� ������ �������� ����������
  LPVOID Object;
  L57_VARLISTCB InfFunc;
} L57_CheckTranser_Param;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����� ������� ���������� � ���������������� ������ �������
#define L57_ORDERLABEL  "#ORDER#"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ��������������� ����� ��� ���������� ������� ������ ����������
//---------------------------------------------------------------------------
class L57_LoopBreak
{ public:
    DWORD Start;

    inline void Set(void){Start=GetTickCount();};
    inline BOOL Check(void){return (GetTickCount()-Start>2000);};

    L57_LoopBreak(void){Set();};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// �������� � ������
//---------------------------------------------------------------------------
class L57_File
{ protected:
    HANDLE Handle;

    int _WindowsWrite(const void *data,size_t *size);
    int _WindowsRead(void *data, size_t *size);
  public:
    void Close(void); // ������� ����
    int OpenWrite(char *path); // ������� �� ������
    int OpenRead(char *path); // ������� �� ������
    int OpenAppend(char *path); // ������� �� ��������
    int WindowsWrite(const void *data,size_t size); // ������ � ����
    int WindowsRead(void *data, size_t size); // ������ �� �����
    int WindowsGetFileSizeShort(DWORD *psize,int *pTooLong=NULL); // �������� ������ ����� (��������)

    // ������� �������� ���� � ����������� ���������� ����� (���. rdsFree)
    char * ReadFileToMemory(char *path,DWORD *psize=NULL,int addzerobyte=1);
    // �������� ������� ������ � ����
    int WriteMemoryToFile(char *path,void *src,DWORD size);
    // �������� ����� � ����
    int WriteTextToFile(char *path,char *src,DWORD len=0);


    L57_File(void){Handle=INVALID_HANDLE_VALUE;};
    ~L57_File(){Close();};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------
class L57_String
{ private:
    char *Data;                 // �������� ������
    static char ZeroByte;       // ������� ���� (��� ������ ������)
  public:

    // �������� ������
    inline void Clear(void){rdsFree(Data);Data=NULL;};
    // ���������� ������
    inline char *c_str(void)const{ZeroByte=0;return Data?Data:(&ZeroByte);};
    // �����
    inline int Length(void)const{return Data?strlen(Data):0;};
    // �������
    inline int IsEmpty(void) const {return Data==NULL || (*Data)==0;};
    inline int IsNotEmpty(void) const {return !IsEmpty();};
    // �������� ������ ������� � �������� �����
    int IsBlank(void)const;
    inline int IsNotBlank(void){return !IsBlank();};
    // ���������
    void Assign(const char *s);
    void Assign(const char *s,int n);
    inline void Assign(const L57_String &s){Assign(s.c_str());};
    // �������� � ������
    inline void AddString(char *s){rdsAddToDynStr(&Data,s,TRUE);};
    inline void AddString(const L57_String &s){AddString(s.c_str());};

    // ��������� ������������ ������ RDS
    inline void AssignRdsDynamicString(char *s){Clear();Data=s;};
    // ����������� ������������ ������ RDS
    inline char *DetachRdsDynamicString(void){char *d=Data; Data=NULL; return d;};

    // �������������� ����� int � ������
    void AssignInt(int i);
    // �������������� ����� double � ������
    void AssignDouble(double d,int dec=-1);

    // �������� int � ������
    void AddInt(int i);
    // �������� double � ������
    void AddDouble(double d,int dec=-1);

    // ���������� �������
    void Exchange(L57_String *pStr);

    // ��������� �� ������
    inline int IsEqual(char *s)const{ZeroByte=0;return strcmp(s?s:(&ZeroByte),c_str())==0;};
    inline int IsEqual(const L57_String &s)const{return IsEqual(s.c_str());};
    inline int IsNotEqual(char *s)const{return !IsEqual(s);};
    inline int IsNotEqual(const L57_String &s)const{return !IsEqual(s);};

    // ��������� ���������
    int operator==(const L57_String &s)const{return IsEqual(s);};
    int operator==(char *s)const{return IsEqual(s);};
    int operator!=(const L57_String &s)const{return !IsEqual(s);};
    int operator!=(char *s)const{return !IsEqual(s);};

    // ��������� ������������
    L57_String & operator=(const L57_String &s){Assign(s);return *this;};
    L57_String & operator=(char *s){Assign(s);return *this;};

    // ��������� ��������
    L57_String operator+(const L57_String &s)const{return L57_String(Data,s.c_str());};
    L57_String operator+(char *s)const{return L57_String(Data,s);};

    // ��������� �������� � �������������
    L57_String & operator+=(const L57_String &s){AddString(s);return *this;};
    L57_String & operator+=(char *s){AddString(s);return *this;};

    // ������������
    L57_String(void){Data=NULL;};
    L57_String(char *s){Data=rdsDynStrCopy(s);};
    L57_String(const L57_String &s){Data=rdsDynStrCopy(s.c_str());};
    L57_String(char *s1,char *s2){Data=rdsDynStrCat(s1,s2,TRUE);};


    // ����������
    ~L57_String(){Clear();};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������ �����
//---------------------------------------------------------------------------
class L57_StringArray
{ private:
    L57_String **Data;  // ������ ���������� �� ����������� ����������� L57_String
    int _Count;         // ������ �������
  public:
    // ����� ����� � �������
    inline int Count(void){return _Count;};
    // ������ �� ������� ��� ��������
    inline L57_String &ItemFAST(int n){return *(Data[n]); };
    inline L57_String *ItemPtrFAST(int n){return Data[n]; };

    // ��������
    void Clear(void);
    // ������� ������� � �������� �������, ���� ��� ���
    void CreateItem(int n);

    // ���������� ������
    inline void SetSize(int n){CreateItem(n-1);};

    // ��������
    BOOL IsEqualTo(L57_StringArray *pArray);
    BOOL IsEqualTo(char **array,int count);

    // ���� �� � ������ �������� ������, ������������ �� �����������
    BOOL HasDifferentNonEmpty(char **array,int count);

    // ����� ������
    int IndexOf(char *str);
    inline int IndexOf(L57_String &str){return IndexOf(str.c_str());};
    inline int IndexOf(L57_String *pstr){return pstr?IndexOf(pstr->c_str()):-1;};

    // ��������� ������ (����������� �� /n)
    void Assign(char *str);

    L57_StringArray(void){Data=NULL;_Count=0;};
    ~L57_StringArray(){Clear();};
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ ����� ����� (������� ������ �������)
//---------------------------------------------------------------------------
class L57_IntArray
{ public:
    int *Items; // ��������
    int Size;   // ������
    int Step;   // ��� ���������� �������
    int Count;  // �����

    // �������� �������
    void Add(int x);
    // ��������, ���� ������ ��� �� ����
    void AddIfNew(int x);
    // ����� �������
    int IndexOf(int x);
    // ��������
    void Clear(void);
    // ������� ��������� �������
    void Init(int size,int initvalue);
    // ������� �������
    void DeleteByIndex(int index);
    // ������� ������ ��������� ��������
    void DeleteByValue(int value){DeleteByIndex(IndexOf(value));};

    // �������� � ������ ������ ����� �������
    void ToString(L57_String *pStr);

    // ���������� ������� (�������� ������, ���� �����)
    void SetElement(int index,int value);

    // �����������
    L57_IntArray(void){Items=NULL; Size=Count=0; Step=10;};
    // ����������
    ~L57_IntArray(){ rdsFree(Items); }
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ ������������ ����� (������� ������ �������)
//---------------------------------------------------------------------------
class L57_DblArray
{ public:
    double *Data;       // ������
    int Count;          // ������ �������

    int PrevCount;      // ������� ������ ������� (��������������� �������, � ����� ������ �� ��������������)

    // ������?
    inline BOOL IsEmpty(void){return Data==NULL || Count==0;};
    // �� ������?
    inline BOOL IsNotEmpty(void){return Data!=NULL && Count!=0;};

    // ��������
    void Clear(void);
    // �������
    BOOL Allocate(int n);
    // ���������� ������� � ������ ��������
    void Exchange(L57_DblArray &array);
    // ��������� ������� (��� �� ������) �� ������� �������
    void FillDataFrom(const L57_DblArray &array);

    L57_DblArray(void){Data=NULL;Count=0;};
    ~L57_DblArray(){Clear();};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������ ������������ ������� ��� �������� ���������
//---------------------------------------------------------------------------
class L57_DblMatrTempStorage
{ private:
    int Rows,Cols; // �������
    int Size;      // ����� ������
    double *Data;  // ������ ������

    BOOL Allocate(int rows,int cols);   // ������� ������ ��������� �������
  public:

    // �������� ���
    inline void Clear(void){rdsFree(Data);Data=NULL;Rows=Cols=Size=0;};
    #ifndef L57_NORDSCLASSES
    // �������� � ���������� �����
    BOOL CompareMatr(rdsbcppVarAncestor *matr,RDS_ARRAYACCESSDATA *pAccData=NULL);
    // ��� ���������� ������� �������� � ���������� �����, ���� �� ���������
    // ��������� ����� ������ � ������� �������� ����
    // ������� - ������� ��������� � ������ �������
    BOOL Check(char *insignal,rdsbcppVarAncestor *matr,BOOL &outchanges,BOOL forcechange=FALSE);
    #else
    // �������� � ���������� �����
    BOOL CompareMatrAndCopy(void *matr);
    #endif

    L57_DblMatrTempStorage(void){Rows=Cols=Size=0;Data=NULL;};
    ~L57_DblMatrTempStorage(){Clear();};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// �������� ������� ��� ��� ������� � ��������� ��� ������ ���������� ����� ����� �������
//---------------------------------------------------------------------------
class L57_FuncTransferParameter
{ public:
    int NameId;         // ������������� ����� (�������������� � RDS)
    int SuffixId;       // ������������� �������� (L57_VARSUFFIX_*)
    BOOL Matrix;        // TRUE - ������� double, FALSE - ����� double

    int BlockVarNum;    // ����� ����� ����� ��� ����� ��������� (-1 - ���)
    BOOL HasConnection; // � ����� ����� ��������� ���������� �����
    RDS_BHANDLE ConnBlock; // ����, �� �������� ���� �����

    // �������������� ��� ������ ��� ��������
    int ChangedNameId;  // �������� ������������� �����
    BOOL ClearSuffix;   // �������� ������� ��� ��������

    L57_String SenderBlockNames; // ����� ������, �� ������� �������� �������� (��� ��������)

    // ���� ���������� �������� ��������� (� ������ ����� ������������)
    #ifdef L57_NORDSCLASSES
      int ChangeSignalOffset;   // �������� � ������� ���������
      int VarOffset;            // �������� � ����� ����������
      // ������ ��� �������� ���������
      double OldValue;
      L57_DblMatrTempStorage OldMatrix;

      void ClearOldData(void)  // �������� ������ ��������
        { OldValue=rdsbcppHugeDouble; OldMatrix.Clear(); };
    #else
      rdsbcppVarAncestor *pRdsVar;      // ����������� ���������� ������������������ ����� (����� ��� �������)
      double *pPlainVar;                // ���� ������
    #endif

    // ���������� NameId � SuffixId � L57_AcceptParam_Param
    void CopyIds(L57_AcceptParam_Param *param);

    L57_FuncTransferParameter(void)
      { NameId=SuffixId=0; Matrix=HasConnection=FALSE; BlockVarNum=-1;
        ChangedNameId=-1; ClearSuffix=FALSE;
        #ifdef L57_NORDSCLASSES
          ChangeSignalOffset=VarOffset=0;
          OldValue=rdsbcppHugeDouble;
        #else
          pRdsVar=NULL;
          pPlainVar=NULL;
        #endif
        ConnBlock=NULL;
      };
    ~L57_FuncTransferParameter()
      { };
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� �������� ������ ������� ������� ������ ��� �������� ������� ��� �� ����������
//---------------------------------------------------------------------------
class L57_TransferCallsList;
class L57_TransferCall
{ public:
    RDS_BHANDLE Block;  // ����
    int DestVarNum;     // ����� ����������-��������������� �����

    L57_TransferCall *Prev,*Next; // ������ �� ������
    L57_TransferCallsList *Parent;// ���������

    // ��������� ��� �������� �������
    BOOL Immediate;     // TRUE - ����� ��-�� �������������� ������� � ����� �� ������
                        // FALSE - ����� � �������� ������������� ��� ������ �� ������ ��������������
    char **Texts;       // ������ ������� (������������ ������, �������� ������������� rdsFree)
    int TextsCount;     // ����� �������
    int TextIndex;      // ����� �������, ������� ����� �����, ��� -1 ��� ���� �������
    int AcceptIndex;    // ����� �������, ������� ����� ����������, ��� -1 ��� ���� �������

    void Extract(void); // �������� �� ����������
    void ConnectLast(L57_TransferCallsList *c); // ������������ � ������ ���������� (� �����)

    // ���������� �� ��������� ������� �����
    void CopyFrom(L57_AcceptFunc_Param *param);

    L57_TransferCall(void)
      { Block=NULL; DestVarNum=-1; Prev=Next=NULL; Parent=NULL; Texts=NULL; TextsCount=0;
      };
    ~L57_TransferCall();
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� ������ ������� ������� ������ ��� �������� ������� ��� �� ����������
//---------------------------------------------------------------------------
class L57_TransferCallsList
{ public:
    L57_TransferCall *First,*Last; // ������

    void Clear(void); // �������� ������
    void AddParamTransfer(RDS_BHANDLE block,int destvarnum); // �������� ����� �������� ����������
    void AddFuncTransfer(RDS_BHANDLE block,L57_AcceptFunc_Param *param); // �������� ����� �������� �������

    L57_TransferCallsList(void){First=Last=NULL;};
    ~L57_TransferCallsList(){Clear();};
};

//---------------------------------------------------------------------------
// ����� ���������� �������, ������� ����� ��������� ����
//---------------------------------------------------------------------------
class L57_FuncTransfer
{ private:
    // ������ ����������
    L57_FuncTransferParameter **Params;
    int ParamsCount;    // �������� �����
    int ParamsSize;     // ���������� �����

    // ������� ������ ������� ������� ������ ��� �������� ���������� (��� ��������� �����)
    L57_TransferCallsList Calls;

    // ��������� �� ���������� ���������� ��� ��������� ��������������� �������� ���������
    int *ParamTransferIdPtr;

    // ��������� ���������� ������ �������� ����������
    void DoDelayedTransferCalls(L57_AcceptParam_Param *param);
  public:
    // ����� �������������� ���� ����������
    int varnames_set;
    // ������������� ������� �������� ����������
    int transfer_func;
    // ������������� ������� �������� �������
    int func_transfer_func;

    // ������� ������ ������� ������� ������ ��� �������� ������� (��� ��������� �����)
    L57_TransferCallsList FuncCalls;

    // ������������� ��������� ����������� �������� ��������� ��� �������
    int LastTransferId;

    // �������� ��������� ������������� ��� �������� ��������� ��� �������
    int GenerateParamTransferId(void);

    // ������ ������-�����������
    RDS_HOBJECT DestBlocks;     // ������ ������
    L57_IntArray DestInputs;    // ������ ������� ������

    // ����� ����������
    inline int GetCount(void){return ParamsCount;};
    // �������� �� ������ ��� ��������
    inline L57_FuncTransferParameter *ParamFAST(int n){return Params[n];};

    // �������� ���������
    void ClearParameters(void);
    // ��������� ������ ������� ����������
    void EnlargeParameters(int newsize);

    // �������� �������� (������)
    L57_FuncTransferParameter * AddParameter(void);

    // �������� ��������
    L57_FuncTransferParameter * AddParameter(int nameid,int suffixid,BOOL matrix,int varnum);
    L57_FuncTransferParameter * AddParameter(char *name,int suffixid,BOOL matrix,int varnum);
    #ifdef L57_NORDSCLASSES
      L57_FuncTransferParameter * AddParameter(int nameid,int suffixid,BOOL matrix,int varnum,int signaloffset,int varoffset);
    #else
      L57_FuncTransferParameter * AddParameter(char *name,int suffixid,BOOL matrix,int varnum,double *plainvar);
      L57_FuncTransferParameter * AddParameterRds(char *name,int suffixid,BOOL matrix,int varnum,rdsbcppVarAncestor *rdsvar);
    #endif

    // ����� ��������
    L57_FuncTransferParameter * FindParameter(int nameid,int suffixid);

    #ifdef L57_NORDSCLASSES
      // �������� ������ ��������
      void ClearOldData(void);
      // ��������� ��������� � �������� ���������
      void CheckInputsAndTransfer(void *vardata);
      // �������� ����� ������� ���������� � ���� ���������� � �������� �������� ������
      void ResetConnFlagsForName(char *name);
    #endif


    // ���������� ������� ������ � ������� ����������
    void CheckInputConnections(RDS_BHANDLE block);
    // ��������� ������ ������ �� ������� ������ � ������� ����������
    void ListInputConnections(RDS_BHANDLE block,LPVOID Object,L57_VARLISTCB Func);
    // ������� ������ ������-�����������
    void ListFunctionDestinations(RDS_BHANDLE block,int outnum);
    // �������� ����� ������-����������
    void ResetSenderBlockNames(void);

    // ������������� ������� �� ������� �������� �������� ����������
    BOOL CheckTransferReaction(RDS_BHANDLE block,int funcoutputnum,L57_CheckTranser_Param *param);

    // �������� �������� ��������� ������-�����������
    void TestTransferParameter(RDS_BHANDLE block,L57_FuncTransferParameter *p,L57_CheckTranser_Param *cparam);

    // �������� �������� ������-�����������
    #ifndef L57_NORDSCLASSES
      void TransferParameter(RDS_BHANDLE block,L57_FuncTransferParameter *p,RDS_HOBJECT DestBlocks,BOOL checkconn=TRUE);
    #else
      void TransferParameter(void *varaddr,L57_FuncTransferParameter *p,RDS_HOBJECT DestBlocks,BOOL checkconn=TRUE);
    #endif

    // ��������� �������� �������� ������-�����������, ������� � ��� ���
    void RetransferParameter(L57_AcceptParam_Param* Param,int newnameid,int newsuffixid=-1);


    // �������  ��������
    // ������� - ���� ����� �������� � �� ���������
    BOOL AcceptParam(L57_AcceptParam_Param* Param,RDS_BHANDLE thisblock=NULL);
    // ���������, ���� �� �������� ��������� ���������� �� ����� ����� ��� ����� (����������)
    void CheckTransferParam(RDS_BHANDLE Block,L57_CheckParamConn_Param* Param,int funcin_num);

    // ��������� � ������� ����� ����� ��� �������� �������
    void QueueAcceptFuncCall(RDS_BHANDLE block,L57_AcceptFunc_Param *param);
    // ��������� ���������� ������ �������� �������
    void DoDelayedFuncTransferCalls(L57_AcceptFunc_Param_Queue *queue);

    L57_FuncTransfer(void);
    ~L57_FuncTransfer()
      { ClearParameters();
        rdsDeleteObject(DestBlocks);
      };
};
//---------------------------------------------------------------------------


#ifndef L57_NORDSCLASSES
// ������ double RDS (������ �� ������� ������)
class L57_RdsDoubleArray
{ private:
    double *Array;
    int Count;
    double DefVal;
  public:
    inline void Setup(double *a,int cnt=0,double def=0.0){Array=a;Count=cnt;DefVal=def;};
    void Setup(rdsbcppVarAncestor &var,double def=0.0);
    inline int Size(void){return Count;};
    #ifdef RDSBCPP_INDEXCHECK
      inline double operator[](int i)const{ return (Array!=NULL && i>=0 && i<Count)?Array[i]:DefVal; };
      inline double operator[](double di)const{ int i=(int)di; return (Array!=NULL && i>=0 && i<Count)?Array[i]:DefVal; };
    #else
      inline double operator[](int i)const{ return Array[i]; };
      inline double operator[](double di)const{ return Array[(int)di]; };
    #endif

    L57_RdsDoubleArray(void){Array=NULL;Count=0;DefVal=0.0;};
};
//---------------------------------------------------------------------------

#ifdef RDSBCPP_INDEXCHECK
// ��������������� ����� ��� ������� double � ��������� ��������
class L57_RdsDoubleMatrix_aux
{ private:
    double *Array;
    int Count;
    double DefVal;
  public:
    inline double operator[](int c)const{ return (Array!=NULL && c>=0 && c<Count)?Array[c]:DefValue; };
    inline double operator[](double dc)const{int c=(int)dc; return (Array!=NULL && c>=0 && c<Count)?Array[c]:DefValue; };

    L57_DoubleMatrix_aux(double *a,int c,double def){Array=a;Count=c;DefVal=def;};
};
//---------------------------------------------------------------------------
#endif

// ������� double RDS (������ �� ������� ������)
class L57_RdsDoubleMatrix
{ private:
    double *Array;
    int _Rows,_Cols;
    double DefVal;
  public:
    inline void Setup(double *a,int rows=0,int cols=0,double def=0.0){Array=a;_Rows=rows;_Cols=cols;DefVal=def;};
    void Setup(rdsbcppVarAncestor &var,double def=0.0);
    inline int Rows(void){return _Rows;};
    inline int Cols(void){return _Cols;};
    #ifdef RDSBCPP_INDEXCHECK
      inline L57_RdsDoubleMatrix_aux operator[](int r)const
        { return L57_RdsDoubleMatrix_aux((Array!=NULL && r>=0 && r<_Rows && _Cols>0)?(Array+r*_Cols):NULL,
                                      _Cols,DefValue); };
      inline L57_RdsDoubleMatrix_aux operator[](double dr)const
        { int r=(int)dr; return L57_RdsDoubleMatrix_aux((Array!=NULL && r>=0 && r<_Rows && _Cols>0)?(Array+r*_Cols):NULL,
                                      _Cols,DefValue); };
    #else
      inline double * operator[](int r){return Array+r*_Cols;};
      inline double * operator[](double dr){return Array+((int)dr)*_Cols;};
    #endif
    L57_RdsDoubleMatrix(void){Array=NULL;_Rows=_Cols=0;DefVal=0.0;};
};
//---------------------------------------------------------------------------

// ������������ ������� ����� ����������
class L57_OneArgFunc
{ private:
    TRdsLinearInterpolation Engine;
  public:
    // ���������
    inline void Setup(rdsbcppVarAncestor &M){ Engine.Setup(M,RDSLININTER_T_SINGLE); Engine.SetDefValue(0.0); };
    // ����������
    inline double operator()(double x){ return Engine.Calc_Fx(x,0,1); };
    inline double operator()(double x,int yindex){ return Engine.Calc_Fx(x,0,yindex); };
    // ���������� � ��������� ������� y
    inline double Calc(double x,int yindex=1){ return Engine.Calc_Fx(x,0,yindex); };

    L57_OneArgFunc(void){};
};
//---------------------------------------------------------------------------

// ������������ ������� ���� ����������
class L57_TwoArgFunc
{ private:
    TRdsLinearInterpolation Engine;
  public:
    // ���������
    inline void Setup(rdsbcppVarAncestor &M){ Engine.Setup(M,RDSLININTER_T_FIELD); Engine.SetDefValue(0.0); };
    // ����������
    inline double operator()(double x1,double x2){ return Engine.Calc_Fx1x2(x1,x2); };

    L57_TwoArgFunc(void){};
};
//---------------------------------------------------------------------------

// �������� �������� ������ �� ���������� ������� � �������� ������� (������� �������� ������)
class L57_TableFiller
{ public:
    L57_TableFiller *Next;      // ��������� � ������
    rdsbcppVarAncestor *Matr;   // �������, � ������� ����������� ������
    char *pReady;               // ���������� ���������� ���������� (���� ����)

    int Count;                  // ����� �������� ����������� � ������� ����� ����������
    int *MatrCol;               // ������ �������� �������� ������� (���� ������� - ������� ����� ����������)
    int *ResultN;               // ������ ����������� �� ��������� ������� ��� ������� ������� ����� ����������

    int TwoArgResult;           // ����� ���������� �� ��������� ������� ��� ������� ������� ���� ����������
    int Transp;                 // ��������������� ������� ��� �������� (1) ��� ��� (0) ��� ������� ������� ���� ����������

    // �������� ���
    void Clear(void){rdsFree(MatrCol);rdsFree(ResultN);MatrCol=ResultN=NULL;Matr=NULL;pReady=NULL;TwoArgResult=-1;Count=0;};
    // ������� ������� ��� ������� ������� ����� ����������
    void AllocateArrays1Arg(int resultscount);

    L57_TableFiller(void){Next=NULL;Matr=NULL;MatrCol=ResultN=NULL;TwoArgResult=-1;Count=0;pReady=NULL;Transp=0;};
    ~L57_TableFiller(){Clear();};
};
//---------------------------------------------------------------------------

// ����� �������� �������� ������
class L57_TableFillProgram
{ public:
    L57_TableFiller *First;     // ������ �������� ������

    // �������� ���
    void Clear(void);
    // �������� ������� ������ ��� ������� ������� ����� ����������
    L57_TableFiller *Add1Arg(rdsbcppVarAncestor *matr,char *pReady,int resultscount);
    // �������� ������� ������ ��� ������� ������� ���� ����������
    L57_TableFiller *Add2Arg(rdsbcppVarAncestor *matr,char *pReady,int resultnum,int transp=0);

    L57_TableFillProgram(void){First=NULL;};
    ~L57_TableFillProgram(){Clear();};
};
//---------------------------------------------------------------------------

// ����������� ��������� ������� ����� ���������� (��������� ��������� ������)
class L57_Table1ArgFuncData
{ protected:
    L57_DblArray _ArgsArray;// ������ �������� ��������� (�����������)
  public:
    L57_DblArray *ArgsArray;// ������ �������� ��������� (�������)
    int ArgIndex;       // ������ ��������� (��� ������� �����)

    int ResArgCount;    // ���������� �������� ��������� � ������� �����������
    int NResults;       // ���������� �����������
    double *Results;    // ������ ����������� (������ ����), ������������ �������� �� NResults, ������ NResults*ArgCount

    int _CurIndex;      // ������ �������� ������� ���������

    BOOL OutputAllocError; // ��������� ������ ��� �������� ������ � �������� �������

    L57_TableFillProgram Output; // ��������� �������� ������

    // �������� ���
    void Clear(void);
    // ������� ������� ����������� �� ArgCount � NResults
    BOOL AllocateResultArrays(void);
    // ���� ���������� _CurIndex
    inline double *CurIndexBase(void){return Results+_CurIndex*NResults;};
    // ���� ��������� ����������
    inline double *ResultBaseForArgN(int argn){return Results+argn*NResults;};

    // �������� ���������� ��� ����������� � ������ ���������� (������������ ��������� ���, ������� � PrevCount)
    // ������� - ����� ����������� ����� ��� -1 ��� ������
    int AddResultsFromPrevCount(void);

    // ���������� ���������� � ������� �� 0 �� _CurIndex
    BOOL SendToOutput(BOOL tocurindex);

    L57_Table1ArgFuncData(void)
      { /*PrevArgCount=*/ResArgCount=NResults=0;ArgIndex=-1;
        Results=NULL;_CurIndex=0;OutputAllocError=FALSE;
        ArgsArray=&_ArgsArray;};
    ~L57_Table1ArgFuncData()
      { Clear(); };
};
//---------------------------------------------------------------------------

// ����������� ��������� ������� ���� ���������� (��������� ��������� ������)
class L57_Table2ArgFuncData
{ protected:
    L57_DblArray _Args1Array;// ������ �������� ��������� (�����������)
    L57_DblArray _Args2Array;// ������ �������� ��������� (�����������)
  public:
    L57_DblArray *Args1Array;// ������ �������� ��������� (�������)
    int Arg1Index;      // ������ ��������� (��� ������� �����)

    L57_DblArray *Args2Array;// ������ �������� ��������� (�������)
    int Arg2Index;      // ������ ��������� (��� ������� �����)

    int ResArg1Count;   // ���������� �������� ��������� 1 � ������� �����������
    int ResArg2Count;   // ���������� �������� ��������� 2 � ������� �����������
    int NResults;       // ���������� �����������
    double **Results;   // ������ ����������� (������ ����)
                        // ������� ������� Results[i] - ������ ������ �� ������� ���������
                        // ��� ������ ��������� i. ������ � Results[i] ���� �������� �� NResults,
                        // ������ ����� ������� - NResults*Arg2Count. ������ Results - Arg1Count
                        // ��������� n ��� [r,c]: Results[r][c*NResults+n]

    int _CurIndex1,     // ������� �������� ������� ����������
        _CurIndex2;

    BOOL OutputAllocError; // ��������� ������ ��� �������� ������ � �������� �������

    L57_TableFillProgram Output; // ��������� �������� ������

    // ������������ ��������� �������� (����� ��������)
    inline int TotalArgs(void){return Args1Array->Count*Args2Array->Count;};

    // �������� ���
    void Clear(void);
    // ������� ������� ����������� �� Arg*Count � NResults
    BOOL AllocateResultArrays(void);
    // ���� ���������� _CurIndex1,_CurIndex2
    inline double *CurIndexBase(void){return Results[_CurIndex1]+_CurIndex2*NResults;};
    // ���� ���������� index1,index2
    inline double *IndexBase(int index1,int index2){return Results[index1]+index2*NResults;};

    // ���������� ���������� � ������� �� 0 �� _CurIndex1,_CurIndex2
    BOOL SendToOutput(BOOL tocurindices);

    // �������� ���������� ��� ����������� � ������ ���������� (������������ ��������� ���, ������� � PrevCount)
    BOOL AddResultsFromPrevCount(void);

    L57_Table2ArgFuncData(void)
      { ResArg1Count=ResArg2Count=NResults=0;Arg1Index=Arg2Index=-1;
        Args1Array=&_Args1Array; Args2Array=&_Args2Array; Results=NULL;
        _CurIndex1=_CurIndex2=0;OutputAllocError=FALSE;};
    ~L57_Table2ArgFuncData()
      { Clear(); };
};
//---------------------------------------------------------------------------
#endif // #ifndef L57_NORDSCLASSES

//---------------------------------------------------------------------------
// ��������� ��� ���������� ������� ����� ������
#define L57_BLOCKTEXT_NOSUPPORT         -1 // ��������� ���������
#define L57_BLOCKTEXT_NOCONTROL         0 // ��� ���������� (��� ������ � ���������� �����, ��� � �����)
#define L57_BLOCKTEXT_BEFORE            1 // ��� ������� ����������� ����� ���������������� �������, ���� �� ������
#define L57_BLOCKTEXT_AFTER             2 // ��� ������� ����������� ����� ����������������� ������, ���� �� ������
#define L57_BLOCKTEXT_FIXED             3 // ��� ������� �� ����������� � ���������������� �����
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����� �������
//---------------------------------------------------------------------------
// ������� L57.dll ��� ��������� ������������ �������� ������� ������ ��� ������� �������
void L57_ExecTool_GetDynamicDescription(L57_String &templatetext,L57_String &modeltext,
                                        L57_String *pFunc,L57_String *pInputs,L57_String *pNumbered,L57_String *pTitle);
// ������� L57.dll ��� �������������� ������� � ����������
int L57_ExecTool_Edit(char *templatetext,char *modeltext,char *params,int starttab,L57_String *pNewModelText,
                      L57_String *pFunc,L57_String *pInputs,L57_String *pNumbered,L57_String *pTitle,
                      char *lockedits=NULL,L57_String *pExport=NULL,char *additionaldata=NULL,
                      RDS_BHANDLE *pShowBlock=NULL,int *ptr_param=NULL,L57_String *ptr_text=NULL);
// ������� L57.dll ��� ������ ����� � ������
int L57_ExecTool_SetOptions(char *templatetext,char *modeltext,char *options,L57_String *pNewModelText,L57_String *pNewExport=NULL);
// ������� l57.dll ��� ������� ������� � ������
BOOL L57_ExecTool_ImportFunction(RDS_BHANDLE block,char *templatefile,int funcnum,char *importtext);
// ������� L57.dll ��� �������� �������
int L57_ExecTool_Translate(RDS_BHANDLE block,char *templatefile,int textparam,L57_String *ptr_text);
// ������� �������������� ������ ������ � ������ ����� � ������� ��� ��������������� �����
void L57_CreateAdditionalText(L57_String *pText,RDS_BHANDLE block,int funcin_num);
// �������� ������ ��� ����� ������ �����
int L57_GetDynModelFileName(RDS_BHANDLE block,L57_String &name);
// ��������� ����� ����� ������ �����
int L57_LoadModelFile(RDS_BHANDLE block,L57_String &text);
// �������� ����� ����� ������ �����
int L57_SaveModelFile(RDS_BHANDLE block,L57_String &text,BOOL saveres=TRUE);
int L57_SaveModelFile(RDS_BHANDLE block,char *text,BOOL saveres=TRUE);
// ��������� ����� ����� � ������
int L57_ReadFileToMemory(char *path,L57_String &text);
// ����������������� ������ �����
void L57_CompileModel(RDS_BHANDLE block,BOOL rebuild=TRUE);
// ����������� ����� ����� �� ��������� � ����
int L57_CalcNPointsByDiap(double vmin,double vmax,double dv);
// ���������� ��������� � ���� (������������� ������������ ��������)
// ���������� ����� ����� � ���������
int L57_AdjustDiapAndStep(double &_vmin,double &_vmax,double &_dv,double vmin,double vmax,double dv);
// �������� �� ��������� �� ������
inline double L57_ValueInDiap(double vmin,double vmax,double dv,int i,int cnt)
{ return (i==0) ? vmin : ( (i==(cnt-1))?vmax:(vmin+i*dv) ); }
// ��������� ������� double ����� RDS
inline double *L57_NewDouble(int n){return (double*)rdsAllocate(n*sizeof(double));}
// ��������� ������� int ����� RDS
inline int *L57_NewInt(int n){return (int*)rdsAllocate(n*sizeof(int));}
// ��������� ������� double* ����� RDS
inline double **L57_NewDoublePtr(int n){return (double**)rdsAllocate(n*sizeof(double*));}
// ����������� ������� ��� ������� ����� ����������
BOOL L57_SortMatrix(double *MData,int Rows,int Cols,int SortCol=0);
// ����������� ������� ��� ��������� �� ������ (� ��������� �������� �������)
BOOL L57_SortMatrixFieldByRow(double *MData,int Rows,int Cols,int SortRow=0);
// ����������� ������� ��� ��������� �� ������� (� ��������� �������� �������)
BOOL L57_SortMatrixFieldByCol(double *MData,int Rows,int Cols,int SortCol=0);
// ����������� ������� �� ������� �� ��������
BOOL L57_SortMatrixColDesc(double *MData,int Rows,int Cols,int SortCol=0);
// ����������� ������� �� ������� �� �����������
#define L57_SortMatrixColAsc L57_SortMatrix
// �������� ������ ����� ��� ���������� ���������� � undo
void L57_SaveModelToUndo(RDS_BHANDLE block,BOOL force=FALSE);
// ��������� ������ ����� ��� �������������� ���������� ��� undo
void L57_LoadModelFromUndo(RDS_BHANDLE block,char *text);
#ifdef L57_LOG
  // �������� ������ ��� ����� ������� ��� ������ �����
  int L57_Log_GetLogFileName(RDS_BHANDLE block,L57_String &name);
  // �������� ������ ������ �����
  void L57_Log_Clear(RDS_BHANDLE block);
  // �������� ������� ������ ������ �����
  void L57_Log_Add(RDS_BHANDLE block,const L57_String &text,BOOL crlf=TRUE);
  void L57_Log_Add(RDS_BHANDLE block,char *text,BOOL crlf=TRUE);
  // �������� ������ � ����� ����� � ������ ������ �����
  void L57_Log_AddInt(RDS_BHANDLE block,char *title,int v,BOOL crlf=TRUE);
  // �������� ������ � ������������ ����� � ������ ������ �����
  void L57_Log_AddDouble(RDS_BHANDLE block,char *title,double v,BOOL crlf=TRUE);
#endif
// ��������� ������� ���������� ������� � ����� �����
// (������������ ������ ��� ������ � ������������ ��������)
BOOL L57_CheckFunctionSource(RDS_BHANDLE block,int inputnum);
// ��������� ������� ���������� ������� � ����� �����
// (������������ ��� ������ � ������-�������� �������)
BOOL L57_CheckFunctionSourceMulti(RDS_BHANDLE block,int inputnum,L57_IntArray *pItems,BOOL *pArrayConnected);
// ������� ������� �� �������� ������
void L57_SendFunctionByConn(RDS_BHANDLE block,int outputnum,BOOL immediate,char **exporttexts,int funccount,L57_AcceptFunc_Param_Queue *queue);
// ������� ������� �� �������� ������ (� �������� �������������� �������)
void L57_SendOutputFunctions(RDS_BHANDLE block,int outputnum,BOOL immediate,char **exporttexts,int funccount,L57_StringArray *exportalt,L57_AcceptFunc_Param_Queue *queue);
// ������� ���� ������� �� �������� �����, ���� ��� ������� ����� ��� �������
// (������������ ������ ��� ������ � ������������ ��������)
void L57_SendSingleFunctionIfNoInput(RDS_BHANDLE block,int inputnum,int outputnum,BOOL immediate,char *exporttext);
// ������� ���� ������� �� �������� �����
// (������������ ������ ��� ������ � ������������ ��������)
void L57_SendSingleFunction(RDS_BHANDLE block,int outputnum,BOOL immediate,char *exporttext,L57_AcceptFunc_Param_Queue *queue);
// ������� ���� (�������) ������� �� ������� �����
// (������������ ������ ��� ������ � ������������ ��������)
BOOL L57_GetSingleFunctionByConn(RDS_BHANDLE block,L57_AcceptFunc_Param *Param,char *curexporttext=NULL,L57_String *pNewExport=NULL);
// ������� � ������ ������� � �������� �������
BOOL L57_GetCertainFunctionByConn(RDS_BHANDLE block,char *templatefile,int funcnum,
                                  char *curexporttext,char *newexporttext);
// ������� l57.dll ��� ������� ������ ������� � ������
BOOL L57_ExecTool_ImportFunctions(RDS_BHANDLE block,char *templatefile,char **exporttexts,int funccount,L57_StringArray *exportalt);
// ������� ����� �������� ������� �� ��� �������: ���������� � ������
void L57_SplitExportText(char *text,L57_StringArray &vars,L57_StringArray &other);
// �������� ��������� ������� � ��������� � ������� ������������� ������ (� ���������
// ����� ���� ������ ����������, ��� ���������)
BOOL L57_MustImportFunction(char *newfunc,char *oldfunc);
// �������� ��� ������ �������� ������� � ������� ������������� ������
BOOL L57_MustImportFunctions(L57_StringArray *newfuncs,L57_StringArray *oldfuncs);
BOOL L57_MustImportFunctions(L57_StringArray *newfuncs,char **oldfuncs,int oldfuncscount);
// ��������� ����������� ��������� � ���� ���������
BOOL L57_CanAccessField(void *structbase,DWORD structsize,void *fieldbase,DWORD fieldsize);
// ������ ��� �������� ����������� ������� � ���� ��������� � servSize
#define L57_CANACCESSFIELD(pStruct,Field) L57_CanAccessField(pStruct,pStruct->servSize,&(pStruct->Field),sizeof(pStruct->Field))
// �������� ��� ������ �������� �������, ��������� ������� ����������
BOOL L57_CompareExportTexts(char *text1,char *text2);
// ��������� ������� ��������� ������ � ����� �����
BOOL L57_CheckInputSource(RDS_BHANDLE block,int inputnum,RDS_BHANDLE *pConnBlock=NULL);
// ������� ������ ���������� ������ � ����� �����
BOOL L57_ListInputSources(RDS_BHANDLE block,L57_FuncTransferParameter *p,LPVOID Object,L57_VARLISTCB Func);
// �������� ����������� ������� (�������) ����� � ������ ������
void L57_AppendFunctionDestList(RDS_BHANDLE block,int outputnum,RDS_HOBJECT list,L57_IntArray *inputs=NULL);
// �������� ��� ������� double
BOOL L57_CompareDoubleMatr(void *VoidPtr1,void *VoidPtr2,bool *pError=NULL);
// �������� � �������� ���� � ����
void L57_SelectAndShowBlock(RDS_BHANDLE block,BOOL waitmsg=TRUE);
// ��������� �������� �������� ���������� � �������� ����
void L57_CheckTransferParameters(RDS_BHANDLE block,LPVOID Object,L57_VARLISTCB Func);
// ������� L57.dll � �������� � ����������� ���������� ������
void L57_ExecTool_UpdatePossible(void);
// ������� �� ����� ����� ��� ��������� ����� �����
BOOL L57_ResFile_MakeName(char *filename,L57_String &resname);
// ��������� ������������� �����
BOOL L57_FileExists(char *filename);
// ������� ����� ����� ������ ����� ����� ������ ���������� � ������� �������� �����
void L57_ResFile_MakeCopy(char *filename);
// ������� ����� ����� ������ �����, ��������� ����� ������ ���������� � ������� �������� �����
void L57_ResFile_DeleteCopy(char *filename);
void L57_ResFile_DeleteModelCopy(RDS_BHANDLE block);
// ������������ ������ �� �����, ��������� ����� ������ ���������� � ������� �������� �����
void L57_ResFile_RestoreCopy(char *filename);
void L57_ResFile_RestoreModelCopy(RDS_BHANDLE block);
// �������� ������� ���������� ������� �� ���������� ������� �������� �������
L57_AcceptFunc_Param_Queue *L57_GetQueueFromParam(void *vparam);
// ��������� ������������� ����� �������
BOOL L57_HelpUIFileExists(char *filename,int *pstate=NULL);
// ������� L57.dll ��� ������� ���� ������� � ����������
BOOL L57_ExecTool_ClearAll(RDS_BHANDLE block,char *templatefile);
// ������� L57.dll � �������� � ���������� � ���������� ���������� ������� ������� ��� �����
BOOL L57_ExecTool_CanEditTextRect(void);
// �������� ������� ��� ����� (������������� � �������) �������� �������� �������
void L57_ApplyFunctionToTextRect(RDS_BHANDLE block,int TR_params,char *TR_text,char *func_text,BOOL check=FALSE);
// ������ ���� ����� �������� ���� ����� � ���������
#define L57_BLOCKTEXT_FIELDBASE 10000
// ������� ���� ����� ��� �������������� �������� ���� ����� � �������� ��� �������������� � �������
void L57_TextRectEdit_Setup(RDS_BHANDLE block,RDS_HOBJECT win,int TR_params,char *TR_text,int tab);
// ������� ������ ����� ����� ��� �������������� �������� ���� ����� � �������� ��� �������������� � �������
void L57_TextRectEdit_Apply(RDS_BHANDLE block,RDS_HOBJECT win,int &TR_params,L57_String &TR_text,char *functitles=NULL);
// ���������� ����� ����� ��� �������������� �������� ���� ����� � �������� ��� �������������� � �������
void L57_TextRectEdit_EnableControls(RDS_HOBJECT win);
//---------------------------------------------------------------------------

#endif // #ifndef L57Mk2BasicH

