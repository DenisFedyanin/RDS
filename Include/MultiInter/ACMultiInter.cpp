#define MULTITABLE_NOVCL
#define MTFL_TABLE
#define __fastcall
#define bool BOOL
#include "MultiTableFileLibrary.cpp"
#include "MultiTable.cpp"
#include "FileLibrary.cpp"
#include "MultiTableSaver.cpp"
#undef __fastcall
#undef bool
//---------------------------------------------------------------------------

class TMultiInterpolator
{ private:
    rdsbcppString LastLoadedFile; // ��������� ����������� ����
    FILETIME LastWrite; // ����� ��������� ������ � ��������� ����

    BOOL Error; // ������� ������

    double *LastResult; // ��������� ��������� ������������
    int LastResultCount; // ������ ������� �����������

    // �������� ��������� ���������
    void ClearResult(void);
  public:
    TMultiTable Table;  // ���������������� �������

    // ��������� ������� �� FileName
    BOOL LoadTable(char *FileName,BOOL check=TRUE);
    // �������� ������
    inline BOOL GetError(void){return Error;};

    // ���������� �������� � �������� �������
    void SetArg(int i,double x);

    // �������� �������� � �������� �������
    inline double GetVal(int i)
      { return (i<LastResultCount)?LastResult[i]:0.0;};

    // ��������������� (��������� ��� ��������� � Table)
    BOOL Interpolate(void);
    // ��������������� (4 ���������)
    BOOL Interpolate(double x0,double x1,double x2,double x3);


    TMultiInterpolator(void){Error=TRUE; LastResult=NULL; LastResultCount=0;};
    ~TMultiInterpolator();
};
//---------------------------------------------------------------------------

TMultiInterpolator::~TMultiInterpolator()
{
  if(LastResult)
    delete[] LastResult;
}
//---------------------------------------------------------------------------

// �������� ��������� ���������
void TMultiInterpolator::ClearResult(void)
{
  if(LastResult)
    memset(LastResult,0,sizeof(double)*LastResultCount);
}
//---------------------------------------------------------------------------

// ��������� ������� �� FileName
// check - ��������� �������� ������������� ��������
BOOL TMultiInterpolator::LoadTable(char *FileName,BOOL check)
{ BOOL ok;
  char *dynname;
  TFileLibrary lib;
  FILETIME ft;

  if(FileName==NULL || FileName[0]==0)
    { LastLoadedFile="";
      Error=TRUE;
      return FALSE;
    }

  if(LastLoadedFile.IsEmpty())
    check=FALSE;
  if(Table.IsEmpty())
    check=FALSE;

  // ��������� ���� � ��� �����
  dynname=rdsGetFullFilePath(FileName,NULL,NULL);
  if(!dynname)
    { LastLoadedFile="";
      Error=TRUE;
      return FALSE;
    }

  // ��������� ����
  if(!mtflLoadLibrary_Begin(&lib,dynname,&ft))
    { LastLoadedFile="";
      rdsFree(dynname);
      Error=TRUE;
      return FALSE;
    }
  rdsFree(dynname);

  if(check)
    { // ���� ����� � ��� ����� �� ����������, ������������ �� �����
      if(strcmp(LastLoadedFile.c_str(),FileName)==0 && CompareFileTime(&LastWrite,&ft)>=0)
        return TRUE;
    }
  // �������� ����������
  LastLoadedFile=FileName;
  memcpy(&LastWrite,&ft,sizeof(FILETIME));

  if(mtflLoadLibrary_HasResultTable(&lib))
    { ok=mtflLoadLibrary_End(&lib,&Table,NULL);
      if(ok)
        Error=Table.IsEmpty();
      else
        Error=FALSE;
    }
  else
    { ok=FALSE;
      Error=TRUE;
    }
  Table.ResetInterpolation();
  return ok;
}
//---------------------------------------------------------------------------

// ��������������� (��������� ��� ��������� � Table)
BOOL TMultiInterpolator::Interpolate(void)
{ double *result=Table.Interpolate();
  int vc;
  if(!result)
    { ClearResult();
      return FALSE;
    }
  vc=Table.GetValuesCount();
  if(vc>LastResultCount)
    { if(LastResult)
        delete[] LastResult;
      LastResult=new double[LastResultCount=vc];
    }
  memcpy(LastResult,result,vc*sizeof(double));
  return TRUE;
}
//---------------------------------------------------------------------------

// ���������� �������� � �������� �������
void TMultiInterpolator::SetArg(int i,double x)
{ if(i<Table.GetDimension())
    Table.SetArgumentFAST(i,x);
}
//---------------------------------------------------------------------------


// ��������������� (4 ���������)
BOOL TMultiInterpolator::Interpolate(double x0,double x1,double x2,double x3)
{ int d=Table.GetDimension();
  if(d<1)
    return FALSE;
  Table.SetArgumentFAST(0,x0);
  if(d>1)
    { Table.SetArgumentFAST(1,x1);
      if(d>2)
       { Table.SetArgumentFAST(2,x2);
         if(d>3)
           Table.SetArgumentFAST(3,x3);
       }
    }
  for(int i=4;i<d;i++)
    Table.SetArgumentFAST(i,0.0);
  return Interpolate();
}
//---------------------------------------------------------------------------

