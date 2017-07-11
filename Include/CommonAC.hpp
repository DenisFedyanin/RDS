//---------------------------------------------------------------------------
// �������� ������ �������������� BCpp55 � Common.dll
// (���������� - Borland C++ 5.5)
//---------------------------------------------------------------------------
#ifndef CommonACH
#define CommonACH

#ifndef RdsDefH
  // ��� ���������� ���������� ���������� �������� �� RdsDef.h
  #include "RdsDef.h"
#endif

#ifndef RdsFuncH
  // ��� ���������� ���������� ���������� �������� �� RdsFunc.h
  #include "RdsFunc.h"
#endif

//---------------------------------------------------------------------------
// �����-������ ���� ����������
//---------------------------------------------------------------------------
class rdsbcppVarAncestor
{ protected:
    int _Num;
  public:
    inline int Num(void){return _Num;};
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return FALSE;};
    virtual void * GetVoidPtr(void) { return NULL;};


    rdsbcppVarAncestor(void){_Num=-1;};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ��������� ������ ��� ������� � ������� ����������� ���������� �����
//---------------------------------------------------------------------------
#define RDSBCPP_STATICPLAINCLASS(classname,vartype) \
  class classname : public rdsbcppVarAncestor { protected: vartype *Ptr;  public: \
    inline void Init(void *base,int offset){Ptr=(vartype*)(((unsigned char*)base)+offset);}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    inline operator vartype() const {return *Ptr;}; \
    inline vartype * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline vartype operator=(classname &v){*Ptr=(vartype)v; return *Ptr; }; \
    inline vartype operator=(vartype v){*Ptr=v; return *Ptr; }; \
    inline vartype operator+=(vartype v){(*Ptr)+=v; return *Ptr; }; \
    inline vartype operator-=(vartype v){(*Ptr)-=v; return *Ptr; }; \
    inline vartype operator*=(vartype v){(*Ptr)*=v; return *Ptr; }; \
    inline vartype operator/=(vartype v){(*Ptr)/=v; return *Ptr; }; \
    inline vartype operator++(){++(*Ptr); return *Ptr; }; \
    inline vartype operator++(int){vartype tmp=*Ptr; (*Ptr)++; return tmp; }; \
    inline vartype operator--(){--(*Ptr); return *Ptr; }; \
    inline vartype operator--(int){vartype tmp=*Ptr; (*Ptr)--; return tmp; }; \
    classname(void):rdsbcppVarAncestor(){}; \
  }
/*
������ ����� ������ ��������� �� ����������� ���������� ����� ���� <vartype>,
����� ������� � ������ ���������� �������� �������� Init(<����>,<��������>).
���������� ��������� ��������� ������������ � �������������� � ���� <vartype>.
����������� �������� �������� ����� ����, �� ����������� �� ����������, ����������.
������:
    ������ ���������� ����� ����������� ������� "{SSD}". ��� ������� � �������
    ���������� (���� double) ���������� ������������ ��������� ��������:

    RDSBCPP_STATICPLAINCLASS(TAuxDoubleClass,double);  // �������� ����
    .....
    TAuxDoubleClass x;            // ������ ��� ������ �� ����������
    x.Init(BlockData->VarData,2); // BlockData - ������������ � �������
                                  // ������� ����� �������� ���� RDS_PBLOCKDATA
                                  // 2 - �������� � ������ (sizeof(char)+sizeof(char))
    ......
    x=2.0*x;                      // ������������� �������

� �������, ����� �������� ����������� (��������, � ��������� ������� ����
printf), ������� ������������ ����� ���������� ����:
    char buf[100];
    sprintf(buf,"%lf",(double)x);
������� ����� �������, ��� ��������� &x � ������ ������� ����� ����� ���
TAuxDoubleClass*, � �� double*. ����� �������� ��������� �� ����������,
�� ������� ��������� ������ x, ���������� ������������ ������� x.GetPtr().
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// �����-�������� ��� ������� ����������� ���������� ����� (������ �� ������)
//---------------------------------------------------------------------------
class rdsbcppStaticStub : public rdsbcppVarAncestor
{ protected:
    double Internal;
    BOOL ShowMsg;
    void Msg(void)
      { if(ShowMsg)
          rdsMessageBox("��� ������������� � ������� ����� ������ ���������� "
                        "� ����������� ����������",
          #ifdef RDSBCPP_MODELNAME
                        RDSBCPP_MODELNAME,
          #else
                        NULL
          #endif
                        MB_OK|MB_ICONERROR);
        ShowMsg=FALSE;
      };
  public:
    inline operator double() { Msg(); return 0.0;};
    inline void * GetPtr(void) { Msg(); return &Internal;};
    virtual void * GetVoidPtr(void) { Msg(); return &Internal;}; 
    inline double operator=(rdsbcppVarAncestor &v){ Msg(); return 0.0; };
    inline double operator=(double v){ Msg(); return 0.0; };
    inline double operator=(void *v){ Msg(); return 0.0; };
    inline double operator+=(double v){ Msg(); return 0.0; };
    inline double operator-=(double v){ Msg(); return 0.0; };
    inline double operator*=(double v){ Msg(); return 0.0; };
    inline double operator/=(double v){ Msg(); return 0.0; };
    inline double operator++(){ Msg(); return 0.0; };
    inline double operator++(int){ Msg(); return 0.0; };
    inline double operator--(){ Msg(); return 0.0; };
    inline double operator--(int){ Msg(); return 0.0; };
    inline rdsbcppStaticStub & operator[](int i){ Msg(); return *this; };
    inline int Cols(void){Msg(); return 0;};
    inline int Rows(void){Msg(); return 0;};
    inline int Size(void){Msg(); return 0;};
    inline BOOL IsEmpty(void){Msg(); return TRUE;};
    inline BOOL HasData(void){Msg(); return FALSE;};
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){Msg(); return FALSE;};
    inline void * GetBuffer(void){return &Internal;};
    inline BOOL Resize(int rows,int cols,BOOL keep){Msg(); return FALSE;};
    inline BOOL Resize(int size,BOOL keep){Msg(); return FALSE;};
    inline BOOL Resize(int size){Msg(); return FALSE;};
    inline rdsbcppStaticStub & Item(int r,int c){ Msg(); return *this; };
    rdsbcppStaticStub(void):rdsbcppVarAncestor(){Internal=0.0;ShowMsg=TRUE;};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ��������� ������ ��� ������� � ������������ �������������� ������� (�������)
// ������� ���������� �����
//---------------------------------------------------------------------------
/*
#ifdef RDSBCPP_INDEXCHECK
  #define RDSBCPP_INDEXCHECKOP(clname,vtype) \
    inline clname##_ operator[](int r) \
      { clname##_ el; el.Ptr=Ptr; el.r=r; return el; };
#else
  #define RDSBCPP_INDEXCHECKOP(clname,vtype) \
      inline vtype * operator[](int i){return ((vtype*)(RDS_ARRAYDATA(Ptr)))+i*(RDS_ARRAYCOLS(Ptr));};
#endif
*/

#ifndef RDSBCPP_INDEXCHECK

#define RDSBCPP_STATICPLAINMATRCLASS(classname,vartype) \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline vartype * operator[](int i){return ((vartype*)(RDS_ARRAYDATA(Ptr)))+i*(RDS_ARRAYCOLS(Ptr));}; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYCOLS(Ptr):0;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYROWS(Ptr):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline vartype * GetBuffer(void){return (vartype*)(RDS_ARRAYDATA(Ptr));}; \
    inline BOOL Resize(int rows,int cols,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,rows,cols,keep,NULL); } \
    inline vartype & Item(int r,int c) \
      { static vartype baddata; \
        if(!rdsVarArrayIndexCheck(Ptr,r,c,RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,NULL,NULL)) \
          { memset(&baddata,0,sizeof(vartype));  return baddata; } \
        return *(GetBuffer()+r*(RDS_ARRAYCOLS(Ptr))+c); }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; }; \
  }

#endif
/*
������ ����� ������ ��������� �� ����������� ���������� ����� ���� "������� <vartype>",
����� ������� � ������ ���������� �������� �������� Init(<����>,<��������>).
<vartype> ������ ���� ������� �����, �.�. char, int, short int, float ��� double.
� ������ ��� ������������� ���������� 'S', 'L', 'C', 'H', 'I', 'F' � 'D'.
��������� �������� [], ������������ ������ ������� ��� ������� �������, �.�. � ��������
������� X (i,j) ����� ���������� ��� ������ ���������� X[i][j] (�������� ������������
�������� �� ������������).
����� ���������� �������:
    int Rows(void)     - ����������� �� ������� ������� (����� �����)
    int Cols(void)     - ����������� �� ������� ������� (����� ��������)
    int Size(void)     - ����� ����� ��������� (Rows()*Cols())
    BOOL HasData(void) - ������ ���������� (�.�. �� 0x0)
    BOOL IsEmpty(void) - !HasData()
    BOOL GetAccessData(RDS_PARRAYACCESSDATA data)  - ��������� ���������
                                                     RDS_ARRAYACCESSDATA (��. RdsDef.h)
    BOOL Resize(int rows,int cols,BOOL keep=FALSE) - ������ ������ �������
                                                     (��. �-��� rdsResizeVarArray)
    <vartype> * GetBuffer(void) - ���������� ������ ����������� ������� ���������
                                  <vartype> ������� Size()
    static int DataSize(void)   - ������ ���������� � ������ (������ � ������� ������
                                  �������� ��� ������, �.�. 2*sizeof(void*)
    void * GetPtr(void)         - ���������� ����� ������ ���������� (������������ ���
                                  ������ Init()
������:
    ������ ���������� ����� ����������� ������� "{SSDMI}". ��� ������� � ���������
    ��������� (���� "MI", �.�. ������� int) ���������� ������������ ��������� ��������:

    RDSBCPP_STATICPLAINMATRCLASS(TAuxIntArrayClass,int);  // �������� ����
    .....
    TAuxIntArrayClass x;          // ������ ��� ������ �� ����������
    x.Init(BlockData->VarData,10);// BlockData - ������������ � �������
                                  // ������� ����� �������� ���� RDS_PBLOCKDATA
                                  // 10 - �������� � ������ (sizeof(char)+sizeof(char)+sizeof(double))
    ......
   for(int i=0;i<x.Rows();i++)           // ������������� �������
     for(int j=0;j<x.Cols();j++)
       x[i][j]=i+j;

�������� �������� ��������������� ���������� ����� ���� ��� ������� � ��������, �������
���� �������� ���������� �������� ��� ��������. ��� ����� ��������������� ����������
��������� �������� ������������. ������ ������������� ��������������� ����������
��. � �������� ���������� ������� ��� �������� ������� ����������
(RDSBCPP_STATICCOMPLMATRCLASS)
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ��������� ������ ��� ������� � ������������ �������������� �������
// ������� ���������� �����
//---------------------------------------------------------------------------
#ifndef RDSBCPP_INDEXCHECK

#define RDSBCPP_STATICPLAINARRCLASS(classname,vartype) \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?1:0;}; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int size,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,1,size,keep,NULL); } \
    inline vartype * GetBuffer(void){return (vartype*)(RDS_ARRAYDATA(Ptr));}; \
    inline vartype & Item(int n) \
      { static vartype baddata; \
        if(!rdsVarArrayIndexCheck(Ptr,n,1,RDS_VAIC_SINGLE|RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,NULL,NULL)) \
          { memset(&baddata,0,sizeof(vartype));  return baddata; } \
        return *(GetBuffer()+n); }; \
    inline vartype & operator[](int i){ return *(((vartype*)(RDS_ARRAYDATA(Ptr)))+i); }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; }; \
  }

#endif
/*
������ ����� ������ ��������� �� ����������� ���������� ����� ���� "������ <vartype>",
��� <vartype> - ������� ���. ���� ����� ���������� ������, ���������� � �������
RDSBCPP_STATICPLAINMATRCLASS �� ����������� ������ � ��������� - � �������, � �������
�� �������, ������ ������ ����. �������� [] ��������� �������� ���� <vartype>, � �
������� Resize ���������� ����� ������ ������� ������ ����� ����� � ����� ��������.
��������� ������������� � ������������� ������� (�������) � RDS ��������� �������� ���������,
��� ������ RDSBCPP_STATICPLAINMATRCLASS � RDSBCPP_STATICPLAINARRCLASS ����� ������������
��� ��� ��������, ��� � ��� ������.
������:
    ������ ���������� ����� ����������� ������� "{SSDMI}". ��� ������� � ���������
    ��������� (���� "MI", �.�. ������� int) ���������� ������������ ��������� ��������:

    RDSBCPP_STATICPLAINARRCLASS(TAuxIntArrayClass,int);  // �������� ����
    .....
    TAuxIntArrayClass x;          // ������ ��� ������ �� ����������
    x.Init(BlockData->VarData,10);// BlockData - ������������ � �������
                                  // ������� ����� �������� ���� RDS_PBLOCKDATA
                                  // 10 - �������� � ������ (sizeof(char)+sizeof(char)+sizeof(double))
    ......
    for(int i=0;i<x.Size();i++)    // ������������� �������
      x[i]=i;
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ��������� ������� ��� ������� � ������������ �������������� ������� (�������)
// ������� ���������� �����
//---------------------------------------------------------------------------
#ifndef RDSBCPP_INDEXCHECK

#define RDSBCPP_STATICCOMPLMATRCLASS(classname,elementclass) \
  class classname##_ { public: unsigned char *Base; \
    inline elementclass operator[](int i) \
      { elementclass el; \
        el.Init(Base+i*elementclass::DataSize(),0); \
        return el; \
      }; \
    }; \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline classname##_ operator[](int i) \
      { classname##_ el; \
        el.Base=((unsigned char*)RDS_ARRAYDATA(Ptr))+i*RDS_ARRAYCOLS(Ptr)*elementclass::DataSize(); \
        return el; \
      }; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYCOLS(Ptr):0;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYROWS(Ptr):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int rows,int cols,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,rows,cols,keep,NULL); } \
    inline elementclass Item(int r,int c) \
      { elementclass el; \
        classname##_ elc; \
        if(!rdsVarArrayIndexCheck(Ptr,r,c,RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,NULL,NULL)) \
          return el; \
        elc.Base=((unsigned char*)RDS_ARRAYDATA(Ptr))+r*RDS_ARRAYCOLS(Ptr)*elementclass::DataSize(); \
        return elc[c]; \
      }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; }; \
  }

#endif
/*
������ ����� ������ ��������� �� ����������� ���������� ����� ���� "������� ������� ��������".
������� ������ ����������� ������� <elementclass>. � �������� <elementclass> ����� ����
����������� ���� �� ��������� �������:
  - ������ ��� ������� ������� �������� (�����, ��������������� ���������
    RDSBCPP_STATICPLAINMATRCLASS ��� RDSBCPP_STATICPLAINARRCLASS)
  - ������ ��� ������� ������� �������� (�����, ��������������� ���������
    RDSBCPP_STATICCOMPLMATRCLASS ��� RDSBCPP_STATICCOMPLARRCLASS)
  - ���������-������� rdsbcppBasicStruct (��. ����)
� ������ <elementclass> ����������� ������ ���� ���������� ������� Init(<����>,<��������>) �
����������� ������� DataSize(void). �������� ������ <elementclass> ������ ��������������
������� ��������.
����� ����������-������� � ������ ���������� �������� �������� Init(<����>,<��������>).
��������� �������� [], ������������ ������ ���������������� ������, ��� �������� ����
��������� �������� [], ������������ ��������� (���������������) ������ ������ <elementclass>.
����� �������, � �������� ������� X (i,j) ����� ���������� ��� ������ ���������� X[i][j]
(�������� ������������ �������� �� ������������).
����� ���������� �������:
    int Rows(void)     - ����������� �� ������� ������� (����� �����)
    int Cols(void)     - ����������� �� ������� ������� (����� ��������)
    int Size(void)     - ����� ����� ��������� (Rows()*Cols())
    BOOL HasData(void) - ������ ���������� (�.�. �� 0x0)
    BOOL IsEmpty(void) - !HasData()
    BOOL GetAccessData(RDS_PARRAYACCESSDATA data)  - ��������� ���������
                                                     RDS_ARRAYACCESSDATA (��. RdsDef.h)
    BOOL Resize(int rows,int cols,BOOL keep=FALSE) - ������ ������ �������
                                                     (��. �-��� rdsResizeVarArray)
    static int DataSize(void)   - ������ ���������� � ������ (������ � ������� ������
                                  �������� ��� ������, �.�. 2*sizeof(void*)
    void * GetPtr(void)         - ���������� ����� ������ ���������� (������������ ���
                                  ������ Init()
������:
    ������ ���������� ����� ����������� ������� "{SSMMD}". ��� ������� � �������
    ��������� (���� "MMD", �.�. ������� �������� double) ���������� ������������
    ��������� ��������:

    RDSBCPP_STATICPLAINMATRCLASS(TAuxDoubleMatrClass,double);// �������� ���� MD,
                                                             // �.�. ������ double
    RDSBCPP_STATICCOMPLMATRCLASS(TAuxDoubleMatrMatrClass,    // �������� ���� MMD,
                                 TAuxDoubleMatrClass);       // �.�. ������ TAuxDoubleMatrClass
    .....
    TAuxDoubleMatrMatrClass x;    // ������ ��� ������ �� ����������
    x.Init(BlockData->VarData,2); // BlockData - ������������ � �������
                                  // ������� ����� �������� ���� RDS_PBLOCKDATA
                                  // 2 - �������� � ������ (sizeof(char)+sizeof(char))
    ......
    x.Resize(10,10);              // ������� ������� ������� x
    x[0][0].Resize(5,5);          // ������� ������� �������-�������� x(0,0)
    for(int i=0;i<5;i++)          // ��������� ��������
      for(int j=0;j<5;j++)
         x[0][0][i][j]=i+j;
    ......

����� ������������ ��������������� ���������� ��� ������� � ��������-���������:

    TAuxDoubleMatrClass temp;     // ��������������� ����������
    temp=x[1][2];                 // ������ temp ��������� �� ������-������� x(1,2)
    temp.Resize(10,5);            // �������� ������ x(1,2)
    for(int i=0;i<10;i++)         // ��������� �������� � x(1,2)
      for(int j=0;j<5;j++)
         temp[i][j]=0;

������� �������, ��� ���������� temp � ������ ������� ����� ������������
������ �� ��� ���, ���� ������ ������� x �� ����� �������:

    x.Resize(20,20,TRUE);         // ������ x ������� � ����������� ������, �� ������
                                  // ������������. ������ ��� ������ ��������� ����������
                                  // ���������������.
    temp[0][0]=100;               // ��������, ��������� ���������� ���� Access Violation

����� �������� ���� ��������, ������� ������ ��������� �������� temp ����� �������������
������� x:

    x.Resize(20,20,TRUE);         // ������ x �������, ������ ������������.
    temp=x[1][2];                 // ������ temp ��������� �� ������-������� x(1,2)
                                  // ��� � ����� ����� ������
    temp[0][0]=100;               // ��� � �������
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ��������� ������� ��� ������� � ������������ �������������� �������
// ������� ���������� �����
//---------------------------------------------------------------------------
#ifndef RDSBCPP_INDEXCHECK

#define RDSBCPP_STATICCOMPLARRCLASS(classname,elementclass) \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline elementclass operator[](int i) \
      { elementclass el; \
        el.Init(RDS_ARRAYDATA(Ptr),i*elementclass::DataSize()); \
        return el; \
      }; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?1:0;}; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int size,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,1,size,keep,NULL); } \
    inline elementclass Item(int n) \
      { elementclass el; \
        if(!rdsVarArrayIndexCheck(Ptr,n,1,RDS_VAIC_SINGLE|RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,NULL,NULL)) \
          return el; \
        el.Init(RDS_ARRAYDATA(Ptr),n*elementclass::DataSize()); \
        return el; \
      }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; }; \
  }

#endif
/*
������ ����� ������ ��������� �� ����������� ���������� ����� ���� "������ ������� ��������"
� ����������� ��������� ���������� RDSBCPP_STATICCOMPLMATRCLASS. �������:
  - �������� [] ����� ���������� ��������������� ������ ���� <elementclass>,
  - ������� Rows() � Cols() ���������� ������ ������ � 1 ��������������,
  - ������� Resize() ��������� ������ ���� ��������.
������:
    ������ ��� ��������, ��������� � ������� � RDSBCPP_STATICCOMPLMATRCLASS, ������ ������
    ������� ������ ������������ ������ ������.

    RDSBCPP_STATICPLAINMATRCLASS(TAuxDoubleMatrClass,double);// �������� ���� MD,
                                                             // �.�. ������ double
    RDSBCPP_STATICCOMPLARRCLASS(TAuxDoubleMatrArrClass,      // �������� ���� MMD,
                                 TAuxDoubleMatrClass);       // �.�. ������������� ������ TAuxDoubleMatrClass
    .....
    TAuxDoubleMatrArrClass x;     // ������ ��� ������ �� ����������
    x.Init(BlockData->VarData,2); // BlockData - ������������ � �������
                                  // ������� ����� �������� ���� RDS_PBLOCKDATA
                                  // 2 - �������� � ������ (sizeof(char)+sizeof(char))
    ......
    x.Resize(10);                 // ������� ������� ������� x
    x[0].Resize(5,5);             // ������� ������� �������-�������� x(0)
    for(int i=0;i<5;i++)          // ��������� ��������
      for(int j=0;j<5;j++)
         x[0][i][j]=i+j;
    ......
    TAuxDoubleMatrClass temp;     // ��������������� ����������
    temp=x[1];                    // ������ temp ��������� �� ������-������� x(1)
    temp.Resize(10,5);            // �������� ������ x(1,2)
    for(int i=0;i<10;i++)         // ��������� �������� � x(1,2)
      for(int j=0;j<5;j++)
        temp[i][j]=0;
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� ����� ��� ������� � ����������. ��� �������� ������ ������� �
// ���������� ������ ������������� �� ����.
//---------------------------------------------------------------------------
class rdsbcppBasicStruct : public rdsbcppVarAncestor
{ protected:
    void **Ptr;         // ������� ����� ���������
    BOOL Fixed;         // ������� ������������ ������� (�.�. ������������ ��
                        // ������ �����)

    // ������� ������������ ��� ����������������� ��������� ������������
    inline BOOL Assign(const rdsbcppBasicStruct &val)
      { if(!Fixed) // ������������ ���������
          { Ptr=val.GetPtr();
            InitMembers(*Ptr);
            return TRUE;
          }
        return FALSE;
      };

  public:
    // ������������� ����� ���������, ������ ���������������� � ������
    // ����������� ������
    virtual void InitMembers(void *base)=0;

    // ������������� ���������
    inline void Init(void *base,int offset)
      { Ptr=(void**)(((unsigned char*)base)+offset);
        Fixed=TRUE;
        InitMembers(*Ptr);
      };
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;};

    // ������ ���������� � ������ (����� ��������� ������
    // �������� ���� �����, �.�. sizeof(void*))
    static inline int DataSize(void){return sizeof(void*);};

    // ����� ���������� � ������
    inline void ** GetPtr(void) const {return Ptr;};
    virtual void * GetVoidPtr(void) { return Ptr;}; 

    // ���������������� �������� ������������ ������ ��������� ���:
    // TMyClass & operator=(const TMyClass &val)
    //   { if(Assign(val)) return *this; // ��������� �������� ��������� ����������
    //     <�������� �� ���������� �������� ����� ����������� ����������>
    //     return *this;
    //   };

    // �����������
    rdsbcppBasicStruct(void):rdsbcppVarAncestor(){ Fixed=FALSE; };
    // ���������� (������)
    virtual ~rdsbcppBasicStruct(){};
};
/*
������ ������� ����� ������ ��������� �� ����������� ���������� ����� ���� "���������".
��� ������� ���� ��������� ��������� ���� �����-������� �� ������ ������, ����������
������������ � �������� InitMembers.
������:
    ������ ���������� ����� ����������� ������� "{SS{DD}D}". ������ ���������� ����� - ���������
    "Complex" � ����� ������ Re � Im ���� double. ��������� ���������� - ������� ����������
    ���� double (������� ��� ������� ������������� �������). ��� ������� � ���������� �����
    ����� ������������ ��������� ��������:

    RDSBCPP_STATICPLAINCLASS(TAuxDoubleClass,double);  // �������� ���� ������ �� double
                                                       // (��������� ��� ����� ���������)
   // �������� ���������
   class TrdsComplex : public rdsbcppBasicStruct
     { public:
         TAuxDoubleClass Re,Im; // ����

        // ������������� ����� ��������� (������ ����������� rdsbcppBasicStruct::InitMembers)
        void InitMembers(void *base)
          { Re.Init(base,0);      // ���� Re - ������ � ���������
            Im.Init(base,8);      // ���� Im ������� �� Re, �.�. ����� sizeof(double) ����
          };

        // ���������������� �������� ������������
        TrdsComplex & operator=(const TrdsComplex &val)
          { if(Assign(val)) return *this; // ��������� �������� ��������� ����������
            // ���������� �������� ����������� ���������� (������������)
            Re=val.Re;
            Im=val.Im;
            return *this;
          };

        // �����������
        TrdsComplex(void):rdsbcppBasicStruct()
          { // ����� ����� ���� �����-������ ���������������� ��������, �� ������� �������,
            // ��� �� ������ ������ ������������ InitMembers ��� �� ����������, � ���� ���������
            // �� �� ��� �� ���������
          };
    }; // class TrdsComplex

    // ......
    TrdsComplex x;      // ���������� ��� ������ �� ���������
    TAuxDoubleClass y;  // ���������� ��� ������ �� 4-� ���������� �����
    x.Init(BlockData->VarData,2); // BlockData - ������������ � �������
                                  // ������� ����� �������� ���� RDS_PBLOCKDATA
                                  // 2 - �������� � ������ (sizeof(char)+sizeof(char))
    y.Init(BlockData->VarData,6); // sizeof(char)+sizeof(char)+sizeof(void*)
    // ......
    y=sqrt(x.Re*x.Re+x.Im*x.Im);  // ������������� ����� ��� ����������

����� ��������� ��������������� ���������� ��� ������ �� ��������� ���������.

    TrdsComplex aux;    // ���������� ��� ������ �� ���������
    aux=x;              // ������ aux ��������� �� x, � ����� �������� � ���
                        // �� ����� ���� ����� ������������� ��� x.

������������ ��������� ���������� ���������� �� ������������ ����������, �����������
�� ��������� �����. � ������ ������ (��. ������ ����) ������� rdsbcppBasicStruct::Assign
������ TRUE, � ��������� ���������� ����� ��������� �� �����������. �� ������ ������
Assign ������ FALSE, � ����� ��������� ���������������� ������������ ���� ����� �����
��������� ����� ������.

�� ������� ���������� � ����� ��������� ���������� �� ���������� �� ��������, ���������
���������� ��������� ���������� �� ��������� �� �� ����� ����������� ���������:
    TrdsComplex aux1;   // ���������� ��� ������ �� ���������
    aux1.Re=5.0;        // Access Violation, �.�. aux1 � aux1.Re �� ����������������
*/

// ������ ��� ����������������� ��������� ������������ ��������
#define RDSBCPP_STRUCTASSIGNMENTOP(classname,assignmentfunction) \
  classname & operator=(const classname &val) \
    { if(Assign(val)) return *this; \
      assignmentfunction(val); \
      return *this; \
    }
/*
���� ������ ����� ������������ ��� ��������������� ��������� ������������ � ������� ������
rdsbcppBasicStruct. � �������� ���������� ������� ������� ��� ������ �������
<classname> � ��� ������� ������������ <assignmentfunction>, ��������� �������������,
������� �������� ���� ����� ���� ��������� �������� ����� ���������, ���������� �
�������� ���������. � ����������� ���� ������� ��� TrdsComplex ���������������
��������� ������������ ����� ��������� ���:
   class TrdsComplex : public rdsbcppBasicStruct
     { public:
         .........
         // ������� ������������
         void AssignMemebers(const TrdsComplex &val)
           { Re=val.Re;
             Im=val.Im;
           }
         // ���������������� �������� ������������
         RDSBCPP_STRUCTASSIGNMENTOP(TrdsComplex,AssignMemebers);
         .........
     };
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ��������� ������ ��� ������� � ������� ������������ ���������� �����
//---------------------------------------------------------------------------
#define RDSBCPP_DYNAMICPLAINCLASS(classname,vartype,rdsvartypestring) \
  class classname : public rdsbcppVarAncestor { protected: RDS_PDYNVARLINK Link; public: \
    inline void DynInit(void) {}; \
    inline operator vartype() const {return *((vartype*)(Link->Data));}; \
    inline vartype * GetPtr(void) const {return (vartype*)(Link->Data);}; \
    virtual void * GetVoidPtr(void) { return (vartype*)(Link->Data);}; \
    inline RDS_PDYNVARLINK GetLink(void) const {return Link;}; \
    inline BOOL CheckLink(RDS_PDYNVARLINK chglnk) const {return Link==chglnk;}; \
    inline void NotifySubscribers(void) const {rdsNotifyDynVarSubscribers(Link);}; \
    inline vartype operator=(classname &v) \
      {vartype aux; *((vartype*)(Link->Data))=aux=(vartype)v; return aux; }; \
    inline vartype operator=(vartype v) \
      {*((vartype*)(Link->Data))=v; return v; }; \
    inline vartype operator+=(vartype v) \
      {vartype *aux=(vartype*)(Link->Data); (*aux)+=v; return *aux; }; \
    inline vartype operator-=(vartype v) \
      {vartype *aux=(vartype*)(Link->Data); (*aux)-=v; return *aux; }; \
    inline vartype operator*=(vartype v) \
      {vartype *aux=(vartype*)(Link->Data); (*aux)*=v; return *aux; }; \
    inline vartype operator/=(vartype v) \
      {vartype *aux=(vartype*)(Link->Data); (*aux)/=v; return *aux; }; \
    inline vartype operator++() \
      {++(*((vartype*)(Link->Data))); return *((vartype*)(Link->Data)); }; \
    inline vartype operator++(int) \
      {vartype tmp=*((vartype*)(Link->Data)); (*((vartype*)(Link->Data)))++; return tmp; }; \
    inline vartype operator--() \
      {--(*((vartype*)(Link->Data))); return *((vartype*)(Link->Data)); }; \
    inline vartype operator--(int) \
      {vartype tmp=*((vartype*)(Link->Data)); (*((vartype*)(Link->Data)))--; return tmp; }; \
    inline BOOL Exists(void) \
      {return ((Link!=NULL) && (Link->Data!=NULL)); }; \
    inline void Unsubscribe(void){rdsUnsubscribeFromDynamicVar(Link);Link=NULL;}; \
    inline BOOL Subscribe(int Block,char *Name,BOOL Search) \
      { Unsubscribe(); return (Link=rdsSubscribeToDynamicVar(Block,Name,rdsvartypestring,Search))!=NULL; }; \
    inline BOOL Delete(void) \
      { RDS_PDYNVARLINK tmp=Link; Link=NULL; return rdsDeleteDVByLink(tmp); }; \
    inline BOOL Create(int Block,char *Name,BOOL fixed) \
      { Delete(); return (Link=rdsCreateAndSubscribeDV(Block,Name,rdsvartypestring,fixed,NULL))!=NULL; }; \
    classname(void):rdsbcppVarAncestor(){ Link=NULL; }; \
  }
/*
������ ����� ������ ��������� �� ������������ ���������� ���� <vartype>. ���������� �����
������� ������ �������� ���� ���������� <rdsvartypestring>, ������������ � RDS (���
double - "D", ��� int - "I" � �.�., ��. ��� ��������� �������� � RdsDef.h).
���������� ��������� ��������� ������������ � �������������� � ���� <vartype>.
����� ����, ���������� ��������� �������:
  BOOL Exists(void) - ������������ ���������� ���������� (�������� ������ �������)
  BOOL Subscribe(int Block,char *Name,BOOL Search) - ����������� �� ���������� Name
                                                     � ����� Block � ������������ ������
                                                     (��. �-��� rdsSubscribeToDynamicVar)
  void Unsubscribe(void) - �������� �������� �� ������������ ����������
  BOOL Create(int Block,char *Name,BOOL fixed) - ������� ������������ ����������
                                                 (��. �-��� rdsCreateAndSubscribeDV)
  BOOL Delete(void) - ������� ������������ ����������.
  BOOL CheckLink(RDS_PDYNVARLINK chglnk) - �������� ��������� �� ���������� ���������
                                           ����� � ����������� (����� �������������� ���
                                           ������� �� ��������� ������������ ����������,
                                           ����� ���������� ��������� �� ������������)
  void NotifySubscribers(void) - ��������� ����������� �� ��������� ������������ ����������
                                 (��. �-��� rdsNotifyDynVarSubscribers)

������:
  ������ � ����������� ������������ ���������� ������� ��� ������������� �������:

    RDSBCPP_DYNAMICPLAINCLASS(TAuxDynDouble,double,"D"); // �������� ������
    // ......
    TAuxDynDouble time;                          // �������� ������� (���� �� ���������
                                                 // �� ����������)
    time.Subscribe(RDS_DVPARENT,"DynTime",TRUE); // ��������
    // ......
    if(time.Exists())                            // ���������� ����������
      { double v=time*2.0;                       // ������������� �������
        // ......
      }
    // ......
    time.Unsubscribe();                          // ����������� ��������

  �������� ������������ ����������:

    RDSBCPP_DYNAMICPLAINCLASS(TAuxDynDouble,double,"D"); // �������� ������
    // ......
    TAuxDynDouble test;                          // �������� ������� (���� �� ���������
                                                 // �� ����������)
    test.Create(RDS_DVSELF,"TestVar",TRUE);      // �������� ����������
    // ......
    if(test.Exists())                            // ���������� ����������
      { test=1234.56;                            // ������������� �������
        // ......
      }
    // ......
    test.Delete();                               // �������� ����������
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ��������� ������ ��� ������� � ������� ������������ ���������� �����
//---------------------------------------------------------------------------
#define RDSBCPP_DYNAMICCLASS(classname,varclass,rdsvartype) \
  class classname : public varclass \
  { protected: RDS_PDYNVARLINK Link; public: \
    inline BOOL Exists(void){return Link!=NULL && Link->Data!=NULL; }; \
    void DynInit(void) { if(Exists()) Init(Link->Data,0); }; \
    inline RDS_PDYNVARLINK GetLink(void) const {return Link;}; \
    inline BOOL CheckLink(RDS_PDYNVARLINK chglnk) const {return Link==chglnk;}; \
    inline void NotifySubscribers(void) const {rdsNotifyDynVarSubscribers(Link);}; \
    inline void Unsubscribe(void){rdsUnsubscribeFromDynamicVar(Link);Link=NULL;}; \
    inline BOOL Subscribe(int Block,char *Name,BOOL Search) \
      { Unsubscribe(); return (Link=rdsSubscribeToDynamicVar(Block,Name,rdsvartype,Search))!=NULL; }; \
    inline BOOL Delete(void) \
      { RDS_PDYNVARLINK tmp=Link; Link=NULL; return rdsDeleteDVByLink(tmp); }; \
    inline BOOL Create(int Block,char *Name,BOOL fixed) \
      { Delete(); return (Link=rdsCreateAndSubscribeDV(Block,Name,rdsvartype,fixed,NULL))!=NULL; }; \
    varclass & operator=(const varclass &val) \
       { this->varclass::operator=(val); \
         return *this; \
       }; \
    classname(void):varclass(){ Link=NULL; }; \
  }
/*
������ ����� ������ ��������� �� ������������ ����������, ��������������� ������ <varclass>.
���������� ����� ������� ������ �������� ���� ���������� <rdsvartypestring>, ������������ � RDS.
������������ ����� <classname> ����� �������� �������� ������ <varclass>, ������� ������
���� ��� ������ �����. � �������� <varclass> ����� ���� ����������� ���� �� ��������� �������:
  - ������ ��� ������� ������� �������� (�����, ��������������� ���������
    RDSBCPP_STATICPLAINMATRCLASS ��� RDSBCPP_STATICPLAINARRCLASS)
  - ������ ��� ������� ������� �������� (�����, ��������������� ���������
    RDSBCPP_STATICCOMPLMATRCLASS ��� RDSBCPP_STATICCOMPLARRCLASS)
  - ���������-������� rdsbcppBasicStruct
� ������������ ������ ���������� ������� Exists, Subscribe, Unsubscribe, Create � Delete,
����������� RDSBCPP_DYNAMICPLAINCLASS, � �������� ������������, ���������� ��������
������������ ������-������.

������:
  �������� �� ������������ ���������� ���� Complex ("{DD}") � ��������� �� ��������

    // �������� ���������
    class TrdsComplex : public rdsbcppBasicStruct
      { public:
          TAuxDoubleClass Re,Im; // ����

          // ������������� ����� ��������� (������ ����������� rdsbcppBasicStruct::InitMembers)
          void InitMembers(void *base)
            { Re.Init(base,0);      // ���� Re - ������ � ���������
              Im.Init(base,8);      // ���� Im ������� �� Re, �.�. ����� sizeof(double) ����
            };

          // ������� ������������
          void AssignMemebers(const TrdsComplex &val)
            { Re=val.Re;
              Im=val.Im;
            }
          // ���������������� �������� ������������
          RDSBCPP_STRUCTASSIGNMENTOP(TrdsComplex,AssignMemebers);
      }; // class TrdsComplex

    // ����� ��� ������ �� ������������ ����������
    RDSBCPP_DYNAMICCLASS(TrdsComplex_dyn,TrdsComplex,"{DD}");
    // ......
    TrdsComplex_dyn dynComp;            // �������� ����������
    // ......
    dynComp.Subscribe(RDS_DVPARENT,"TestCompl",FALSE); // �������� �� ����������
    // ......
    dynComp.DynInit();  // �������������
    if(dynComp.Exists())
      { double x=dynComp.Re*2.0;        // �������������
        // ......
      }
    // ......
    dynComp.Unsubscribe(); // ����������� ��������

�������� ������������ �������� ������ <varclass> �������� ������ <classname> � ��������:

    TrdsComplex aux;
    aux.Init(......);   // ������������� ��� ������ �� ����������� ���������� �����
    // ......
    dynComp=aux;        // ���������� ������������
    // ......
    aux=dynComp;        // ���� ���������� ������������
*/
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��������������� ����� ������
//---------------------------------------------------------------------------
#ifndef RDSBCPP_NOSTRINGCLASS
class rdsbcppString : public rdsbcppVarAncestor
{ protected:
    char *Data; // ������������ ��� ��������� ����������
    char **Ptr; // ������������ ��� ���������� �����
  public:

    // ������������� ��� ���������� �����
    void Init(void *base,int offset)
      { Ptr=(char**)(((unsigned char*)base)+offset);
        if(Data) rdsFree(Data);
        Data=NULL;              // ���������� ������ �� ���������
      };
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;};
    // ������ ���������� � ������ �����
    static inline int DataSize(void){return sizeof(char*);};
    // ����� ���������� � ������
    inline void * GetPtr(void) const {return Ptr;};
    virtual void * GetVoidPtr(void) { return Ptr;}; 

    char *c_str(void) const
    #ifdef RDSBCPP_DMWORKAROUND
      {return (char*)((*Ptr)?(*Ptr):"");};
    #else
      {return (*Ptr)?(*Ptr):"";};
    #endif

    BOOL IsEmpty(void)
      { return (*Ptr)==NULL; };

    int Length(void)
      { return (*Ptr)?strlen(*Ptr):0;};

    char & operator[](int i){ return (*Ptr)[i]; };

    // ��������� ��������
    rdsbcppString operator+(const rdsbcppString &val) const
      { return rdsbcppString(*Ptr,val.c_str()); };
    rdsbcppString operator+(char *str) const
      { return rdsbcppString(*Ptr,str); };

    // ��������� �������� � �������������
    rdsbcppString & operator+=(const rdsbcppString &val)
      { char *aux=rdsDynStrCat(*Ptr,val.c_str(),TRUE);
        if(*Ptr) rdsFree(*Ptr);
        *Ptr=aux;
        return *this;
      };
    rdsbcppString & operator+=(char *str)
      { char *aux=rdsDynStrCat(*Ptr,str,TRUE);
        if(*Ptr) rdsFree(*Ptr);
        *Ptr=aux;
        return *this;
      };

    // ��������� ������������
    rdsbcppString & operator=(const rdsbcppString &val)
       { if(*Ptr) rdsFree(*Ptr);
         *Ptr=rdsDynStrCat(val.c_str(),NULL,TRUE);
         return *this;
       };
    rdsbcppString & operator=(char *str)
       { if(*Ptr) rdsFree(*Ptr);
         *Ptr=rdsDynStrCat(str,NULL,TRUE);
         return *this;
       };

    // ������������
    rdsbcppString(void){Data=NULL;Ptr=&Data;};
    rdsbcppString(char *str) { Data=rdsDynStrCat(str,NULL,TRUE); Ptr=&Data;};
    rdsbcppString(char *str1,char *str2) { Data=rdsDynStrCat(str1,str2,TRUE); Ptr=&Data;};
    ~rdsbcppString(){if(Data) rdsFree(Data);};
};
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
// �����-������ ����������� ������� ��� ������� �����
//---------------------------------------------------------------------------
class rdsbcppFunction
{ protected:
    int _Id;    // ������������� ������� � RDS
    int _State; // 0 - ���������� ������,
                // 1 - ��������� (� ������ �����) ������, �� ����������� ���� ���� ������������ �������
                // 2 - ��������� ������, ���������� ���� ���� ������������ �������
    RDS_PFUNCPROVIDERLINK _Link; // ������ �� ��������� �������� �� ����������� �������

    // ���������� ������ ����� ������������ �������
    void _RegisterProvider(void)
      { if(_State==1)
          { rdsRegisterFuncProvider(_Id,FALSE);
            _State=2;
          }
      };
    // ������ ���������� ������ ����� ������������ �������
    void _UnregisterProvider(void)
      { if(_State==2) // ������� �����������
          { rdsUnregisterFuncProvider(_Id);
            _State=1;
          }
      };

    // ����������� �� ����������� �������
    void _SubscribeToProvider(void)
      { if(_Link==NULL && _State!=0) // ��������� ��� ���������� ��������
          _Link=rdsSubscribeToFuncProvider(_Id);
      };
    // ���������� �� ����������� �������
    void _UnsubscribeFromProvider(void)
      { if(_Link)
          { rdsUnsubscribeFromFuncProvider(_Id);
            _Link=NULL;
          }
      };
  public:
    // ������������� �������
    inline int Id(void){return _Id;};
    // ����������� �������
    inline void Register(char *name)
      { _Id=rdsRegisterFunction(name); };

    // ���� ������� ��������������� ��� ����������� �������
    inline BOOL IsProvider(void){return _State==2;};

    // ���� ������� �������� �� ����������� �������
    inline BOOL Subscribed(void){return _Link!=NULL && _Link->Block!=NULL;};
    // ����-����������� �������
    inline RDS_BHANDLE Provider(void){return _Link?(_Link->Block):NULL;};

    // ������� �������
    void Clear(void)
      { _UnregisterProvider(); // ������� �����������
        _UnsubscribeFromProvider(); // ������������ �� ����������� �������
      };
    // ������������� ���������� �� �����������
    inline void InitLocal(rdsbcppFunction &g){Clear();_Id=g.Id();_State=1;};

    // ����������� � ����������
    rdsbcppFunction(void){_Id=_State=0;_Link=NULL;};
    virtual ~rdsbcppFunction(){Clear();};
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ���������� ������� (�������, �������) � ��������� ��������
//---------------------------------------------------------------------------
#ifdef RDSBCPP_INDEXCHECK

#define RDSBCPP_STATICPLAINMATRCLASS(classname,vartype) \
  class classname##_ { public: void *Ptr; int r; LPSTR name; \
    inline vartype & operator[](int c) \
      { static vartype baddata; \
        if(!rdsVarArrayIndexCheck(Ptr,r,c,RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,name,NULL)) \
          { memset(&baddata,0,sizeof(vartype));  return baddata; } \
        return *(((vartype*)(RDS_ARRAYDATA(Ptr)))+r*(RDS_ARRAYCOLS(Ptr))+c); \
      }; \
    }; \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; LPSTR name; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitN(void *base,int offset,LPSTR vn){Ptr=((unsigned char*)base)+offset; Fixed=TRUE; name=vn;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    inline void InitNumN(void *base,int offset,LPSTR vn,int num){InitN(base,offset,vn);_Num=num;}; \
    inline void SetName(LPSTR n){name=n;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline classname##_ operator[](int r) { classname##_ el; el.Ptr=Ptr; el.r=r; el.name=name; return el; }; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYCOLS(Ptr):0;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYROWS(Ptr):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline vartype * GetBuffer(void){return (vartype*)(RDS_ARRAYDATA(Ptr));}; \
    inline BOOL Resize(int rows,int cols,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,rows,cols,keep,NULL); } \
    inline vartype & Item(int r,int c) \
      { static vartype baddata; \
        if(!rdsVarArrayIndexCheck(Ptr,r,c,RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,name,NULL)) \
          { memset(&baddata,0,sizeof(vartype));  return baddata; } \
        return *(GetBuffer()+r*(RDS_ARRAYCOLS(Ptr))+c); }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; name=NULL;}; \
  }

#define RDSBCPP_STATICPLAINARRCLASS(classname,vartype) \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; LPSTR name; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitN(void *base,int offset,LPSTR vn){Ptr=((unsigned char*)base)+offset; Fixed=TRUE; name=vn;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    inline void InitNumN(void *base,int offset,LPSTR vn,int num){InitN(base,offset,vn);_Num=num;}; \
    inline void SetName(LPSTR n){name=n;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?1:0;}; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int size,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,1,size,keep,NULL); } \
    inline vartype * GetBuffer(void){return (vartype*)(RDS_ARRAYDATA(Ptr));}; \
    inline vartype & Item(int n) \
      { static vartype baddata; \
        if(!rdsVarArrayIndexCheck(Ptr,n,1,RDS_VAIC_SINGLE|RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,name,NULL)) \
          { memset(&baddata,0,sizeof(vartype));  return baddata; } \
        return *(GetBuffer()+n); }; \
    inline vartype & operator[](int i){ return Item(i); }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; name=NULL;}; \
  }

#define RDSBCPP_STATICCOMPLMATRCLASS(classname,elementclass) \
  class classname##_ { public: void *Ptr; int r; LPSTR name; \
    inline elementclass operator[](int i) \
      { elementclass el; unsigned char *Base; \
        if(!rdsVarArrayIndexCheck(Ptr,r,i,RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,name,NULL)) \
          return el; \
        Base=((unsigned char*)RDS_ARRAYDATA(Ptr))+r*RDS_ARRAYCOLS(Ptr)*elementclass::DataSize(); \
        el.Init(Base+i*elementclass::DataSize(),0); \
        return el; \
      }; \
    }; \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; LPSTR name; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitN(void *base,int offset,LPSTR vn){Ptr=((unsigned char*)base)+offset; Fixed=TRUE; name=vn;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    inline void InitNumN(void *base,int offset,LPSTR vn,int num){InitN(base,offset,vn);_Num=num;}; \
    inline void SetName(LPSTR n){name=n;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline classname##_ operator[](int i) \
      { classname##_ el; el.Ptr=Ptr; el.r=i; el.name=name;\
        return el; \
      }; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYCOLS(Ptr):0;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYROWS(Ptr):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int rows,int cols,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,rows,cols,keep,NULL); } \
    inline elementclass Item(int r,int c) \
      { classname##_ elc; elc.Ptr=Ptr; elc.r=r; elc.name=name;\
        return elc[c]; \
      }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; name=NULL;}; \
  }

#define RDSBCPP_STATICCOMPLARRCLASS(classname,elementclass) \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; LPSTR name; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitN(void *base,int offset,LPSTR vn){Ptr=((unsigned char*)base)+offset; Fixed=TRUE; name=vn;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    inline void InitNumN(void *base,int offset,LPSTR vn,int num){InitN(base,offset,vn);_Num=num;}; \
    inline void SetName(LPSTR n){name=n;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?1:0;}; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int size,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,1,size,keep,NULL); } \
    inline elementclass Item(int n) \
      { elementclass el; \
        if(!rdsVarArrayIndexCheck(Ptr,n,1,RDS_VAIC_SINGLE|RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,NULL,NULL)) \
          return el; \
        el.Init(RDS_ARRAYDATA(Ptr),n*elementclass::DataSize()); \
        return el; \
      }; \
    inline elementclass operator[](int i) {return Item(i);};\
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; }; \
  }

#endif

#endif

