//---------------------------------------------------------------------------

#ifndef MultiTableH
#define MultiTableH
//---------------------------------------------------------------------------
//#define MULTITABLE_DEBUG
//---------------------------------------------------------------------------
class TMultiTableSaver;
//---------------------------------------------------------------------------

// ��������� ��� ���������� ������ �������� �������
#define MTSRES_BADARR   0       // ��� ������� ��� � ������� ������������ �������
#define MTSRES_LEFT     1       // ������ ������������
#define MTSRES_RIGHT    2       // ������ �������������
#define MTSRES_MID      3       // ����� ����� ����������
//---------------------------------------------------------------------------
// ����������� �������. ������ �������� ������ ��������.
//---------------------------------------------------------------------------
// ������ �������
class TMultiTableIndex
{ public:
    double *Scale;      // ������� �����
    int ScaleCount;     // ����� �������� �����
    char *Name;         // ��� �����

    int Shift;          // ����� � ����� ������� ��� ���������� ����� �������

    int Index;          // ������������� ������� ������ (������ ������������:
                        // ������� ��������������� ��� �������, ����� ���������� �������
                        // �� ������ ������� �������� GetValuesByIndices)
    bool Fixed;         // ������ ������������ (������������ � ����������� �������� ��������)

    double Argument;    // �������� ��� ������������ �� ����� �������
    int InterLeft,InterRight; // ������� ���� �������� ��� ������������ ���������
    int InterSearchResult; // ��������� ������ �������� ��������

    // ������ ��� ��������������� ���������������� �������
    int InterShift;     // ����� � ������� ������� ��� ���������� ����� �������
    int InterIndex;     // ������������� ������� ������ (��� ��������� ��������: 0 � 1,
                        // ������ ������������, ������� ��������������� ��� �������,
                        // ����� ���������� ������� �������� GetInterValuesByIndices)

    // �������� ���
    void __fastcall Clear(bool clearname=true);
    // ������� ����� ��� �����
    void __fastcall AllocateScale(int n);

    // ����� �������� � ���������� ������� ��� ������������
    bool __fastcall SearchArg(void);

    // ����� ��������� ������
    int __fastcall FindClosestArg(double arg);

    // �������� ��� �������� (�������� ���� �������� ������������ �������)
    void __fastcall AcceptValName(char *dynstr);
    // �������� ��� �������� (�������� ���������� ������)
    void __fastcall StoreValName(char *statstr);
    // �������� ��� �������
    inline char *GetValName(void){return Name?Name:"";};

    TMultiTableIndex(void);
    ~TMultiTableIndex(){Clear(true);};
};
//---------------------------------------------------------------------------

// ���������� �������
class TMultiTable
{ private:
    int Dim;                    // ����������� (����� ����������)
    int ValCount;               // ����� �������� ��� ������� ������ ����������
    TMultiTableIndex *Index;    // ������ ����

    char **ValNames;            // ����� ��������
    int ValNamesCount;          // ����� ���� �������� (����� ���������� � �������,
                                // ���� ����� ����� ValCount)

    double *Data;               // ������
    int DataSize;               // ����� ������ ������ (��� ������� � ������ ������������/�������������)

    // ��������������� ������ ��� ������������
    // (������� �� ���� ������ ������ � �� ������� ������� ����� ��� �������)
    double *InterData;  // ����� ������
    int InterDataSize;  // ������ ������� (��� �������)

    int LastError;              // ��� ��������� ������
      #define MTERROR_OK                0       // OK
      #define MTERROR_BADDIM            1       // �� ������� ��� �������� ����������� �������
      #define MTERROR_ALREADYSET        2       // ������ ��� �����������
      #define MTERROR_BADARG            3       // ������ �������� (������������� ����� ��������� � �.�.)
      #define MTERROR_ZEROSCALE         4       // �� ����� �� ���� ��� ��������
      #define MTERROR_BADINDEX          5       // ������ �� ���������
      #define MTERROR_NODATA            6       // ��� ������� ������
      #define MTERROR_SEARCHERR         7       // ������ ������ ���������
      #define MTERROR_LOADERR           8       // ������ ��������
      #define MTERROR_INTERNAL          9       // ���������� ���������� ������

    // ���������� ������
    void __fastcall SetError(int e);

    // �������� �� ���� ������� �� ���� ������������
    void __fastcall ResetIndices(void);

    // �������� �� ���� ��������������� ������� �� ���� ������������
    void __fastcall ResetInterIndices(void);
    // ��������� ��������� ���������������� ������ (� ��������� �� ��������)
    bool __fastcall NextInterIndex(void);

    // ��������� - ���������������
    bool __fastcall LoadAux(TMultiTableSaver *s);

  public:
    int RestoredRecords;        // ����� ��������������� ��� �������� ��������
                                // (����� �� �����������, ������ ��� ���������� ������������)
    int RestoreMode;            // ����� �������������� ������� (��� ���������� ������������)
    #define TMTSCT_RM_UNKNOWN   -1      // �����������
    #define TMTSCT_RM_GEO       0       // ������������ ���� ��������
    #define TMTSCT_RM_LIN       1       // ������� �������� ������������ �������

    // �������� ������
    inline int GetLastError(void){return LastError;};
    // �������� ������
    inline void ResetError(void){LastError=MTERROR_OK;};

    // ������?
    inline bool IsEmpty(void){return Data==NULL || Dim==0 || ValCount==0;};

    // �������� �����������
    inline int GetDimension(void){return Dim;};
    // ���������� �����������
    bool __fastcall SetDimension(int dim);
    // ������
    inline TMultiTableIndex *GetIndexFAST(int i){return &(Index[i]);};
    TMultiTableIndex * __fastcall GetIndex(int i){return (i<0 || i>=Dim)?NULL:(&(Index[i]));};
    // ���������� ����� �������� � �����������
    inline void SetIndexSizeFAST(int dim,int count){Index[dim].AllocateScale(count);};
    bool __fastcall SetIndexSize(int dim,int count);
    // �������� ����� �������� � �����������
    inline int GetIndexSizeFAST(int dim){return Index[dim].ScaleCount;};
    // �������� ����������� � ������������ ������ � �����������
    bool __fastcall GetIndexRange(int dim,double &vmin,double &vmax);

    // �������� ��� �������� (�������� ���� �������� ������������ �������)
    void __fastcall AcceptValName(int n,char *dynstr);
    // �������� ��� �������� (�������� ���������� ������)
    void __fastcall StoreValName(int n,char *statstr);
    // �������� ��� �������
    char * __fastcall GetValName(int n);

    // ����� ������������ � ����������� ��������
    bool __fastcall GetValueRange(int nval,double &vmin,double &vmax);

    //---------- ������� �������� ----------
    // ���������� ������ �� ��������� �����������
    inline void SetDimensionIndexFAST(int dim,int index){Index[dim].Index=index;};
    bool __fastcall SetDimensionIndex(int dim,int index);
    // �������� ������������� ������ �� ��������� �����������
    inline int GetDimensionIndexFAST(int dim){return Index[dim].Index;};
    // �������� ������ ������ �� ValCount ����� �� ������������� ��������
    double * __fastcall GetValuesByIndices(void);
    // �������� ������ ������ �� ValCount ����� �� ��������� �������� � ������������
    // (��� ������������, ������������ �������� � �������� ������)
    double * __fastcall GetValuesByArgsAndErrors(double *arg,double *argerror);
    // �������� ������ ������ �� ValCount ����� �� ��������� ��������
    // (��� ������������, ������������ ��������� ������)
    double * __fastcall GetClosestValuesByArgs(double *arg,double *realarg,int maxcount=-1);
    //--------------------------------------

    //---------- ������� ��������� �� ���� �������� ----------
    // �������� ������� �� ����
    void __fastcall Loop_ResetIndices(void);
    // ����� ��������� ������ (������� - ������� ��������)
    bool __fastcall Loop_NextElement(void);
    //--------------------------------------------------------

    //------ ������� ��������� � �������� ���������������� ��������� ------
    // �������� ��������������� ���� ��������
    void __fastcall UnfixAllIndices(void);
    // ���������� ��������������� �������
    void __fastcall FixIndex(int dim,bool fix=true);
    // �������� ������� �� ���� (����� �������������)
    void __fastcall LoopFixed_ResetIndices(void);
    // ����� ��������� ������ (����� �������������, ������� - ������� ��������)
    bool __fastcall LoopFixed_NextElement(void);
    //---------------------------------------------------------------------


    //---------- ������� �������� �� ���������������� ������� ----------
    // ���������� ������ �� ��������� �����������
    inline void SetDimensionInterIndexFAST(int dim,int index){Index[dim].InterIndex=index;};
    // �������� ������������� ������ �� ��������� �����������
    inline int GetDimensionInterIndexFAST(int dim){return Index[dim].InterIndex;};
    // �������� �� ��������������� ������� ������ ������ �� ValCount
    // ����� �� ������������� �������� (������� �� ���������)
    double * __fastcall GetInterValuesByIndices(void);
    //------------------------------------------------------------------

    //---------- ������������ ------------
    // ����� ����� ������ �������������
    void __fastcall ResetInterpolation(void);
    // ��������� ���������� ��� ������������
    inline void SetArgumentFAST(int dim,double arg){Index[dim].Argument=arg;};
    // �������� ������ �� ValCount ���������������� �������� (��������� - � ��������)
    double * __fastcall Interpolate(void);
    //------------------------------------

    // �������� ����� ��������
    inline int GetValuesCount(void){return ValCount;};
    // ���������� ����� ��������
    bool __fastcall SetValuesCount(int vc);

    // ��������
    void __fastcall Clear(void);

    // ������� ����������� - ������� ����� ��� ������
    bool __fastcall AllocateData(void);
    // ��������� ������ ���������
    void __fastcall FillData(double val);


    // ����� � ������� �������� �������
    static void __fastcall SearchInArray(double *arr,int count,double x,
                                         int &ileft,int &iright,int &result);
    // �������� ������������
    static double __fastcall LinInterpol(double x1,double y1,double x2,double y2,double x);

    // ���������
    bool __fastcall Save(TMultiTableSaver *s);
    // ���������
    bool __fastcall Load(TMultiTableSaver *s);

    #ifdef MULTITABLE_DEBUG
      // ���������� ����������
      AnsiString __fastcall Debug(bool showinterp=false);
    #endif

    TMultiTable(void);
    ~TMultiTable(){Clear();};
};
//---------------------------------------------------------------------------
#endif
