//---------------------------------------------------------------------------
#ifndef MULTITABLE_NOVCL
  //#include <vcl.h>
  #pragma hdrstop
#endif
#include <math.h>

#include "MultiTable.h"
#include "MultiTableSaver.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ������ �������
//---------------------------------------------------------------------------
TMultiTableIndex::TMultiTableIndex(void)
{
  Scale=NULL;
  ScaleCount=Shift=InterShift=0;
  InterSearchResult=MTSRES_BADARR;
  Name=NULL;
  Fixed=false;
}
//---------------------------------------------------------------------------

// �������� ���
void __fastcall TMultiTableIndex::Clear(bool clearname)
{
  if(clearname)
    { if(Name)
        delete[] Name;
      Name=NULL;
    }
  if(Scale)
    delete[] Scale;
  Scale=NULL;
  ScaleCount=Shift=0;
  InterSearchResult=MTSRES_BADARR;
}
//---------------------------------------------------------------------------

// ������� ����� ��� �����
void __fastcall TMultiTableIndex::AllocateScale(int n)
{
  Clear(false);
  if(n)
    Scale=new double[ScaleCount=n];
}
//---------------------------------------------------------------------------

// ����� �������� � ���������� ������� ��� ������������
bool __fastcall TMultiTableIndex::SearchArg(void)
{
  TMultiTable::SearchInArray(Scale,ScaleCount,Argument,
                             InterLeft,InterRight,InterSearchResult);
  return InterSearchResult!=MTSRES_BADARR;
}
//---------------------------------------------------------------------------

// ����� ��������� ������
int __fastcall TMultiTableIndex::FindClosestArg(double arg)
{ int left=0,right=ScaleCount-1,result=MTSRES_BADARR;
  double rleft,rright;
  TMultiTable::SearchInArray(Scale,ScaleCount,arg,
                             left,right,result);
  if(result==MTSRES_BADARR)
    return ScaleCount?0:-1;
  rleft=fabs(Scale[left]-arg);
  rright=fabs(Scale[right]-arg);
  return (rleft<rright)?left:right;
}
//---------------------------------------------------------------------------

// �������� ��� �������� (�������� ���� �������� ������������ �������)
void __fastcall TMultiTableIndex::AcceptValName(char *dynstr)
{
  if(Name)
    delete[] Name;
  Name=dynstr;
}
//---------------------------------------------------------------------------

// �������� ��� �������� (�������� ���������� ������)
void __fastcall TMultiTableIndex::StoreValName(char *statstr)
{ char *copy;
  int l;
  if(statstr==NULL)
    { AcceptValName(NULL);
      return;
    }
  if(*statstr==0)
    { AcceptValName(NULL);
      return;
    }
  l=strlen(statstr);
  copy=new char[l+1];
  strcpy(copy,statstr);
  AcceptValName(copy);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ���������� �������
//---------------------------------------------------------------------------
TMultiTable::TMultiTable(void)
{
  Dim=0;
  ValCount=0;
  ValNamesCount=0;
  RestoredRecords=0;
  RestoreMode=TMTSCT_RM_UNKNOWN;
  Index=NULL;
  Data=InterData=NULL;
  LastError=MTERROR_OK;
  ValNames=NULL;
}
//---------------------------------------------------------------------------

// ���������� ������
void __fastcall TMultiTable::SetError(int e)
{ LastError=e;
}
//---------------------------------------------------------------------------


// ��������
void __fastcall TMultiTable::Clear(void)
{
  if(ValNames)
    { for(int i=0;i<ValNamesCount;i++)
        if(ValNames[i])
          delete[] ValNames[i];
      delete[] ValNames;
      ValNames=NULL;
    }
  Dim=ValCount=DataSize=ValNamesCount=0;
  RestoredRecords=0;
  RestoreMode=TMTSCT_RM_UNKNOWN;
  if(Index)
    delete[] Index;
  if(Data)
    delete[] Data;
  if(InterData)
    delete[] InterData;
  Index=NULL;
  Data=InterData=NULL;
  ResetError();
}
//---------------------------------------------------------------------------

// ���������� �����������
bool __fastcall TMultiTable::SetDimension(int dim)
{
  if(dim<=0)
    { SetError(MTERROR_BADARG);
      return false;
    }
  if(Data)
    { // ��� ����������� ������
      SetError(MTERROR_ALREADYSET);
      return false;
    }
  Dim=dim;
  Index=new TMultiTableIndex[Dim];
  ResetError();
  return true;
}
//---------------------------------------------------------------------------

// �������� ��� �������� (�������� ���� �������� ������������ �������)
void __fastcall TMultiTable::AcceptValName(int n,char *dynstr)
{
  if(n>=ValNamesCount || ValNames==NULL)
    { // �������������
      int newcount=n+1;
      char **na;
      if(newcount<ValCount)
        newcount=ValCount;
      na=new char*[newcount];
      if(ValNamesCount!=0 && ValNames!=NULL)
        memcpy(na,ValNames,ValNamesCount*sizeof(char*));
      for(int i=ValNamesCount;i<newcount;i++)
        na[i]=NULL;
      if(ValNames)
        delete[] ValNames;
      ValNames=na;
      ValNamesCount=newcount;
    }
  if(ValNames[n])
    delete[] ValNames[n];
  ValNames[n]=dynstr;
}
//---------------------------------------------------------------------------

// �������� ��� �������� (�������� ���������� ������)
void __fastcall TMultiTable::StoreValName(int n,char *statstr)
{ char *copy;
  int l;
  if(statstr==NULL)
    { AcceptValName(n,NULL);
      return;
    }
  if(*statstr==0)
    { AcceptValName(n,NULL);
      return;
    }
  l=strlen(statstr);
  copy=new char[l+1];
  strcpy(copy,statstr);
  AcceptValName(n,copy);
}
//---------------------------------------------------------------------------

// �������� ��� �������
char * __fastcall TMultiTable::GetValName(int n)
{
  if(n<0 || n>ValNamesCount || ValNames==NULL)
    return "";
  return (ValNames[n])?(ValNames[n]):"";
}
//---------------------------------------------------------------------------


// ���������� ����� ��������
bool __fastcall TMultiTable::SetValuesCount(int vc)
{
  if(vc<=0)
    { SetError(MTERROR_BADARG);
      return false;
    }
  if(Data)
    { // ��� ����������� ������
      SetError(MTERROR_ALREADYSET);
      return false;
    }
  ValCount=vc;
  ResetError();
  return true;
}
//---------------------------------------------------------------------------

// ������� ����������� - ������� ����� ��� ������
bool __fastcall TMultiTable::AllocateData(void)
{ int n=ValCount;
  if(Dim<=0 || ValCount<=0 || Index==NULL)
    { SetError(MTERROR_BADDIM);
      return false;
    }
  // �� ������ ��������� �������� - ValCount ��������

  if(Data)
    { delete[] Data;
      Data=NULL;
    }
  if(InterData)
    { delete[] InterData;
      InterData=NULL;
    }

  // ������������ ���� ������������
  for(int i=Dim-1;i>=0;i--)
    { TMultiTableIndex *ind=GetIndexFAST(i);
      ind->Shift=n;
      n*=ind->ScaleCount;
      ind->InterSearchResult=MTSRES_BADARR;
    }
  if(!n)
    { SetError(MTERROR_ZEROSCALE);
      return false;
    }
  Data=new double[DataSize=n];

  // ���������� �������� InterData - ��� ��� �������� (��������) �������
  // � ����� �� ��� ������� � ����������
  n=ValCount;
  Index[0].InterShift=0; // ������� ������ �� ���������
  for(int i=Dim-1;i>0;i--)
    { TMultiTableIndex *ind=GetIndexFAST(i);
      ind->InterShift=n;
      n*=2;
    }
  InterData=new double[InterDataSize=n];

  ResetError();
  return true;
}
//---------------------------------------------------------------------------

// ��������� ������ ���������
void __fastcall TMultiTable::FillData(double val)
{ if(Data)
    for(int i=0;i<DataSize;i++)
      Data[i]=val;
}
//---------------------------------------------------------------------------

// �������� ����������� � ������������ ������ � �����������
bool __fastcall TMultiTable::GetIndexRange(int dim,double &vmin,double &vmax)
{ TMultiTableIndex *ind;
  vmin=vmax=0.0;
  if(dim<0 || dim>=Dim || Index==NULL)
    { SetError(MTERROR_BADDIM);
      return false;
    }
  ind=Index+dim;
  if(ind->ScaleCount==0 || ind->Scale==NULL)
    { SetError(MTERROR_ZEROSCALE);
      return false;
    }
  vmin=ind->Scale[0];
  vmax=ind->Scale[ind->ScaleCount-1];
  ResetError();
  return true;
}
//---------------------------------------------------------------------------

// ����� ������������ � ����������� ��������
bool __fastcall TMultiTable::GetValueRange(int nval,double &vmin,double &vmax)
{ int n;
  vmin=vmax=0.0;

  if(nval<0 || nval>=ValCount)
    { SetError(MTERROR_BADINDEX);
      return false;
    }
  if(Data==NULL||DataSize==0)
    { SetError(MTERROR_NODATA);
      return false;
    }
  // � ������� ����� DataSize/ValCount �������� �� ValCount ����� � ������
  n=DataSize/ValCount;
  if(n*ValCount!=DataSize) // �����-�� ���������� ������
    { SetError(MTERROR_INTERNAL);
      return false;
    }
  for(int i=0;i<n;i++)
    { double *base=Data+i*ValCount+nval;
      if(i)
        { if(vmin>(*base)) vmin=(*base);
          if(vmax<(*base)) vmax=(*base);
        }
      else
        vmin=vmax=*base;
    }
  return true;
}
//---------------------------------------------------------------------------



// ���������� ����� �������� � �����������
bool __fastcall TMultiTable::SetIndexSize(int dim,int count)
{ //TMultiTableIndex *ind;
  if(dim<0 || dim>=Dim || Index==NULL)
    { SetError(MTERROR_BADDIM);
      return false;
    }
  if(Data)
    { // ��� ����������� ������
      SetError(MTERROR_ALREADYSET);
      return false;
    }
  Index[dim].AllocateScale(count);
  ResetError();
  return true;
}
//---------------------------------------------------------------------------

// ���������� ������ �� ��������� �����������
bool __fastcall TMultiTable::SetDimensionIndex(int dim,int index)
{ TMultiTableIndex *ind;
  if(!Data)
    { SetError(MTERROR_NODATA);
      return false;
    }
  if(dim<0 || dim>=Dim || Index==NULL)
    { SetError(MTERROR_BADDIM);
      return false;
    }
  ind=GetIndexFAST(dim);
  if(index<0 || index>=ind->ScaleCount)
    { SetError(MTERROR_BADINDEX);
      return false;
    }
  ind->Index=index;
  ResetError();
  return true;
}
//---------------------------------------------------------------------------

// �������� ������ ������ �� ValCount ����� �� ������������� ��������
double * __fastcall TMultiTable::GetValuesByIndices(void)
{ int n=0;
  if(!Data)
    { SetError(MTERROR_NODATA);
      return NULL;
    }
  for(int i=Dim-1;i>=0;i--)
    { TMultiTableIndex *ind=GetIndexFAST(i);
      n+=ind->Index*ind->Shift;
    }
  if(n<0 || n>=DataSize)
    { SetError(MTERROR_BADINDEX);
      return NULL;
    }
  ResetError();
  return Data+n;
}
//---------------------------------------------------------------------------

// �������� ������ ������ �� ValCount ����� �� ��������� �������� � ������������
// (��� ������������, ������������ �������� � �������� ������)
double * __fastcall TMultiTable::GetValuesByArgsAndErrors(double *arg,double *argerror)
{
  for(int i=0;i<Dim;i++)
    { TMultiTableIndex *ind=GetIndexFAST(i);
      // ���� � ind ������ arg[i]+-argerror[i]
      int ileft=0,iright=ind->ScaleCount-1,result=MTSRES_BADARR;
      SearchInArray(ind->Scale,ind->ScaleCount,arg[i],ileft,iright,result);
      switch(result)
        { case MTSRES_LEFT: // ������ ������������
            if(fabs(ind->Scale[0]-arg[i])>argerror[i])
              return NULL;
            ind->Index=0;
            break;
          case MTSRES_RIGHT: // ������ �������������
            if(fabs(ind->Scale[ind->ScaleCount-1]-arg[i])>argerror[i])
              return NULL;
            ind->Index=ind->ScaleCount-1;
            break;
          case MTSRES_MID: // ����� ����� ����������
            if(fabs(ind->Scale[ileft]-arg[i])<=argerror[i])
              ind->Index=ileft;
            else if(fabs(ind->Scale[iright]-arg[i])<=argerror[i])
              ind->Index=iright;
            else
              return NULL;
            break;
          default: // ��� ��������� ��� ������������ �������
            if(ind->ScaleCount==0)
              return NULL;
            if(fabs(ind->Scale[0]-arg[i])>argerror[i])
              return NULL;
        }
    }
  // ���� ������ ���� ���� - ������� �����������
  return GetValuesByIndices();
}
//---------------------------------------------------------------------------

// �������� ������ ������ �� ValCount ����� �� ��������� ��������
// (��� ������������, ������������ ��������� ������)
// � realarg ������������ ��������� ��������
// ���� maxcount>0, �� � arg � realarg ������������ ������ maxcount ��������,
// ��������� ��������� ������������ ����������� �� ������� �������
double * __fastcall TMultiTable::GetClosestValuesByArgs(double *arg,double *realarg,int maxcount)
{
  for(int i=0;i<Dim;i++)
    { TMultiTableIndex *ind=GetIndexFAST(i);
      if(maxcount>0 && i>=maxcount)
        { // ���� �������� �� ����������
          ind->Index=0;
          continue;
        }
      // ���� � ind ������ arg[i]+-argerror[i]
      int ileft=0,iright=ind->ScaleCount-1,result=MTSRES_BADARR;
      SearchInArray(ind->Scale,ind->ScaleCount,arg[i],ileft,iright,result);
      switch(result)
        { case MTSRES_LEFT: // ������ ������������
            ind->Index=0;
            break;
          case MTSRES_RIGHT: // ������ �������������
            ind->Index=ind->ScaleCount-1;
            break;
          case MTSRES_MID: // ����� ����� ����������
            if(fabs(ind->Scale[ileft]-arg[i])<fabs(ind->Scale[iright]-arg[i]))
              ind->Index=ileft;
            else
              ind->Index=iright;
            break;
          default: // ��� ��������� ��� ������������ �������
            if(ind->ScaleCount==0)
              return NULL;
            ind->Index=0;
        }
      if(realarg) realarg[i]=ind->Scale[ind->Index];
    }
  // ���� ������ ���� ���� - ������� �����������
  return GetValuesByIndices();
}
//---------------------------------------------------------------------------

// �������� �� ��������������� ������� ������ ������ �� ValCount
// ����� �� ������������� �������� (������� �� ���������)
double * __fastcall TMultiTable::GetInterValuesByIndices(void)
{ int n=0;
  if(!Data)
    { SetError(MTERROR_NODATA);
      return NULL;
    }
  for(int i=Dim-1;i>0;i--) // ������� ������ �� ���������
    { TMultiTableIndex *ind=GetIndexFAST(i);
      n+=ind->InterIndex*ind->InterShift;
    }
  if(n<0 || n>=InterDataSize)
    { SetError(MTERROR_BADINDEX);
      return NULL;
    }
  ResetError();
  return InterData+n;
}
//---------------------------------------------------------------------------

// ����� � ������� �������� ������� (static)
// arr    - ������ (��������������� �� �����������)
// count  - ��������� � �������
// x      - ������� �������� (��������)
// ileft  - ����: ����� ������ �������� ������, �����: ��������� ����� ������
// iright - ����: ������ ������ �������� ������, �����: ��������� ������ ������
// result - ����: ��������� �������� ������, �����: ����� ���������
void __fastcall TMultiTable::SearchInArray(double *arr,int count,double x,
                                           int &ileft,int &iright,int &result)
{ double l,r;
  int _ileft,_iright,maxind=count-1;

  if(arr==NULL || count<2)
    { result=MTSRES_BADARR;
      return;
    }

  // ���� ������� ����� ��� ������� - ���������, �����, ����� �� �����
  switch(result)
    { case MTSRES_LEFT: // ������� x ��� ����� ����� �������
        if(x<=arr[0]) // ������ �� ����������
          return;
        if(x<arr[1]) // ������ ����� ������ � ������ - ������� �� ��, ��������� ������
          { result=MTSRES_MID;
            return;
          }
        // ��������� ����� �����
        _ileft=0;
        _iright=maxind;
        break;

      case MTSRES_RIGHT: // ������� x ��� ������ ����� �������
        if(x>=arr[iright]) // ������ �� ����������
          return;
        if(x>=arr[ileft]) // ������ ����� ������������� � ��������� - ������� �� ��, ��������� ������
          { result=MTSRES_MID;
            return;
          }
        // ��������� ����� �����
        _ileft=0;
        _iright=maxind;
        break;

      case MTSRES_MID: // x ��� ����� ileft � iright
        l=arr[ileft];
        r=arr[iright];
        if(x>=l)
          { // ��� ��� ������ ileft
            if(x<=r) // � ����� iright - ��� ���������
              return;
            // ����� ����-�� ������ �� iright
            if(iright==maxind) // ������ ������ ���
              { // ��������� iright==maxind, ileft==maxind-1
                // �������� ������ ���������
                result=MTSRES_RIGHT;
                return;
              }
            // ������ ���� ��� ������� �������
            if(x<=arr[iright+1])
              { // ����� iright � ���������
                ileft=iright;
                iright++;
                result=MTSRES_MID;
                return;
              }
            // ����� ������ ������ - ����� ����� �����
            _ileft=iright+1; // �� ��� �����, ��� �� ������ iright+1
            _iright=maxind;
            break;
          } // if(x>=l)
        // x ����� ileft
        if(ileft==0) // ����� ������ ��� - �������� ������ ���������
          { result=MTSRES_LEFT;
            return;
          }
        // ����� ���� ���-��
        if(x>=arr[ileft-1])
          { // ����� ileft � ��� ������� �����
            iright=ileft;
            ileft--;
            result=MTSRES_MID;
            return;
          }
        // ����� ������ ����� - ����� ����� �����
        _ileft=0;
        _iright=ileft+1; // �� ��� �����, ��� x<arr[ileft-1]
        break;

      default:
        // ��������� ����� �����
        _ileft=0;
        _iright=maxind;
    }


  // ���� ����� ������, ������, ����� �����
  if(x<=arr[0]) // ����� �������
    { ileft=0;
      iright=1;
      result=MTSRES_LEFT;
      return;
    }
  if(x>=arr[maxind]) // ������ �������
    { ileft=maxind-1;
      iright=maxind;
      result=MTSRES_RIGHT;
      return;
    }
  if(_ileft==_iright) // �� ������ ������
    { _ileft=0; _iright=maxind; }

  // x ���-�� ������ ������� ����� _ileft � _iright
  for(;;)
    { int m;
      if(_iright==_ileft+1) // ������ �� ���� ��������
        break; // ����� ��������
      m=(_ileft+_iright)/2; // ��������
      if(x<arr[m]) // � ����� ��������
        _iright=m;
      else // � ������ ��������
        _ileft=m;
    }
  ileft=_ileft;
  iright=_iright;
  result=MTSRES_MID;
}
//---------------------------------------------------------------------------

// �������� ������������ (static)
double __fastcall TMultiTable::LinInterpol(double x1,double y1,double x2,double y2,double x)
{ double k;
  k=(y2-y1)/(x2-x1);
  return k*x+(y2-k*x2);
}
//---------------------------------------------------------------------------

// ����� ����� ������ �������������
void __fastcall TMultiTable::ResetInterpolation(void)
{ for(int i=0;i<Dim;i++)
    { TMultiTableIndex *ind=GetIndexFAST(i);
      ind->InterSearchResult=MTSRES_BADARR;
      ind->Argument=0.0;
    }
}
//---------------------------------------------------------------------------

// �������� �� ���� ��������������� ������� �� ���� ������������
void __fastcall TMultiTable::ResetInterIndices(void)
{
  for(int d=0;d<Dim;d++)
    Index[d].InterIndex=0;
}
//---------------------------------------------------------------------------

// ��������� ��������� ���������������� ������ (� ��������� �� ��������)
// ������� - ������� �� ��������
bool __fastcall TMultiTable::NextInterIndex(void)
{
  // ��������� ������
  for(int d=Dim-1;d>0;d--)
    { int index=Index[d].InterIndex+1;
      if(index<2)
        { Index[d].InterIndex=index;
          return true;
        }
      // ��������� �� ������� d - ���������� �� ���� � ����������� ���������
      Index[d].InterIndex=0;
      if(d==1) // �������� ������� (������� ��� ������������ �� ������������, ��������� ��������
        { Index[0].InterIndex=1; // �������������� �������
          return false;
        }
    }
  return true;
}
//---------------------------------------------------------------------------

// �������� �� ���� ������� �� ���� ������������
void __fastcall TMultiTable::ResetIndices(void)
{
  for(int d=0;d<Dim;d++)
    Index[d].Index=0;
}
//---------------------------------------------------------------------------


// �������� ��������������� ���� ��������
void __fastcall TMultiTable::UnfixAllIndices(void)
{
  for(int d=0;d<Dim;d++)
    Index[d].Fixed=false;
}
//---------------------------------------------------------------------------

// ���������� ��������������� �������
void __fastcall TMultiTable::FixIndex(int dim,bool fix)
{
  if(dim>=0 && dim<Dim)
    Index[dim].Fixed=fix;
}
//---------------------------------------------------------------------------

// �������� ������� �� ���� (����� �������������)
void __fastcall TMultiTable::LoopFixed_ResetIndices(void)
{
  for(int d=0;d<Dim;d++)
    { TMultiTableIndex *ind=GetIndexFAST(d);
      if(!ind->Fixed)
        ind->Index=0;
    }
}
//---------------------------------------------------------------------------

// ����� ��������� ������ (����� �������������, ������� - ������� ��������)
bool __fastcall TMultiTable::LoopFixed_NextElement(void)
{ bool finish=false;

  for(int d=Dim-1;d>=0;d--)
    { TMultiTableIndex *ti=GetIndexFAST(d);
      int index=ti->Index+1,minindex,maxindex;
      if(ti->Fixed)
        minindex=maxindex=ti->Index;
      else
        { minindex=0; maxindex=ti->ScaleCount; }

      if(index<maxindex)
        { ti->Index=index;
          break;
        }
      // ��������� �� ������� d - ���������� �� ���� � ����������� ���������
      ti->Index=minindex;
      if(d==0) // �������� �������
        { finish=true;
          break;
        }
    }
  return finish;
}
//---------------------------------------------------------------------------

// �������� ������ �� ValCount ���������������� �������� (��������� - � ��������)
#ifdef MULTITABLE_DEBUG
  //#define INTER_DEBUG
#endif
  #ifdef INTER_DEBUG
    // ��������
    void __fastcall SaveAnsiString(const AnsiString &filename,const AnsiString &s);
  #endif
double * __fastcall TMultiTable::Interpolate(void)
{ double x1,x2;
  #ifdef INTER_DEBUG
    AnsiString s=Debug()+"\r\n----------------------------------------------------------\r\n";
  #endif

  if(!Data)
    { SetError(MTERROR_NODATA);
      return NULL;
    }

  // ����� � �������� �������� � ���������� ��������
  for(int i=0;i<Dim;i++)
    if(!Index[i].SearchArg())
      { SetError(MTERROR_SEARCHERR);
        return NULL;
      }

  #ifdef INTER_DEBUG
    s+="�����\r\n";
    for(int i=0;i<Dim;i++)
      { s.cat_sprintf("%2d: ",i);
        for(int j=0;j<Index[i].ScaleCount;j++)
          s.cat_sprintf("%5.1lf[%2d]",Index[i].Scale[j],j);
        s+="\r\n";
      }
    s+="����� ����������\r\n";
    for(int i=0;i<Dim;i++)
      s.cat_sprintf("%2d: %5.1lf -> %5.1lf[%2d] ...%5.1lf[%2d] (%d)\r\n",
                    i,
                    Index[i].Argument,
                    Index[i].Scale[Index[i].InterLeft],Index[i].InterLeft,
                    Index[i].Scale[Index[i].InterRight],Index[i].InterRight,
                    Index[i].InterSearchResult);
  #endif

  // ������������ �� �������� (��������) ������� -
  // ������ ������� �� �������� ������� Data, �����������
  // ���������������� ������� InterData
  // Index[i].InterIndex ������������ �� ������ ������:
  // 0 - � ����� ������� ������� InterLeft, 1 - ������� InterRight
  #ifdef INTER_DEBUG
    s+="��������� ������������\r\n";
  #endif
  ResetInterIndices();
  // ��������� ����� � ������ �������� ��������� �������� �������
  x1=Index->Scale[Index->InterLeft];
  x2=Index->Scale[Index->InterRight];
  // ������� ���� ��������, ����� ��������
  for(;;)
    { double *valleft,*valright,*interp;
      // ��������� �� ����, ����� ��������, Index �� InterIndex
      for(int d=1;d<Dim;d++)
        { TMultiTableIndex *ind=GetIndexFAST(d);
          ind->Index=(ind->InterIndex)?ind->InterRight:ind->InterLeft;
        }
      // �������� ��� �����: (0,i1,i2,...) � (1,i1,i2,...)
      Index->Index=Index->InterLeft;
      valleft=GetValuesByIndices();
      Index->Index=Index->InterRight;
      valright=GetValuesByIndices();
      // ������� � ���������������� ������� ����� (i1,i2,...)
      interp=GetInterValuesByIndices();
      // ����� � ���������������� ������� ��������� ������������
      for(int i=0;i<ValCount;i++)
        interp[i]=LinInterpol(x1,valleft[i],x2,valright[i],Index->Argument);
      #ifdef INTER_DEBUG
        for(int aux=0;aux<2;aux++)
          { s.cat_sprintf(" (%4d",aux?Index->InterRight:Index->InterLeft);
            for(int d=1;d<Dim;d++)
              { TMultiTableIndex *ind=GetIndexFAST(d);
                s.cat_sprintf(",%4d",ind->Index);
              }
            s+=") : ";
            for(int i=0;i<ValCount;i++)
              s.cat_sprintf("%6.1lf ",aux?valright[i]:valleft[i]);
            s+="\r\n";
          }
        s+="���������: ";
        for(int i=0;i<ValCount;i++)
          s.cat_sprintf("%6.1lf ",interp[i]);
        s+="\r\n-----------------------------------\r\n";
      #endif
      // ����� ��������� ������
      if(!NextInterIndex())
        break;
    }
  #ifdef INTER_DEBUG
    s+="��������� ���������������� �������\r\n";
    ResetInterIndices();
    for(;;)
      { double *a=GetInterValuesByIndices();
        for(int d=0;d<Dim;d++)
          s.cat_sprintf("%4d",GetDimensionInterIndexFAST(d));
        s+=" : ";
        for(int i=0;i<ValCount;i++)
          s.cat_sprintf("%8.3lf",a[i]);
        s+="\r\n";
        if(!NextInterIndex())
          break;
      }
  #endif

  // ������ ����� ��������������� InterData ������ �� ��������
  for(int d=1;d<Dim;d++)
    { // d - ������, �� �������� ����� ���������������
      TMultiTableIndex *c_ind=GetIndexFAST(d);
      x1=c_ind->Scale[c_ind->InterLeft];
      x2=c_ind->Scale[c_ind->InterRight];

      ResetInterIndices();
      for(;;)
        { double *valleft,*valright;
          // �������� ��� �����: (...,0,i{d+1},i{d+2},...) � (...,1,i{d+1},i{d+2},...)
          c_ind->InterIndex=0;
          valleft=GetInterValuesByIndices();
          c_ind->InterIndex=1;
          valright=GetInterValuesByIndices();
          // ������������� � ����� ��������� � valleft
          for(int i=0;i<ValCount;i++)
            valleft[i]=LinInterpol(x1,valleft[i],x2,valright[i],c_ind->Argument);
          c_ind->InterIndex=0;
          NextInterIndex();
          if(c_ind->InterIndex) // ������� � ������� ������ - ��� ���������
            break;
        }

      #ifdef INTER_DEBUG
        s.cat_sprintf("���������������� ������� �� ������� %d\r\n",d);
        ResetInterIndices();
        for(;;)
          { double *a=GetInterValuesByIndices();
            for(int d=0;d<Dim;d++)
              s.cat_sprintf("%4d",GetDimensionInterIndexFAST(d));
            s+=" : ";
            for(int i=0;i<ValCount;i++)
              s.cat_sprintf("%8.3lf",a[i]);
            s+="\r\n";
            if(!NextInterIndex())
              break;
          }
      #endif

    }

  // ����� ���� ������ � ������ InterData ���������� ��������� ������������
  #ifdef INTER_DEBUG
    s+="��������� ������������:\r\n  ���������: (";
    for(int i=0;i<Dim;i++)
      s.cat_sprintf("%5.1lf",Index[i].Argument);
    s+=")\r\n  ����������: ";
    for(int i=0;i<ValCount;i++)
      s.cat_sprintf("%5.1lf ",InterData[i]);
    s+="\r\n";
  #endif

  #ifdef INTER_DEBUG
    SaveAnsiString("debug.txt",s);
  #endif
  return InterData;
}
//---------------------------------------------------------------------------


// �������� ������� �� ����
void __fastcall TMultiTable::Loop_ResetIndices(void)
{ for(int d=0;d<Dim;d++)
    SetDimensionIndexFAST(d,0);
}
//---------------------------------------------------------------------------

// ����� ��������� ������ (������� - ������� ��������)
bool __fastcall TMultiTable::Loop_NextElement(void)
{ bool finish=false;

  for(int d=Dim-1;d>=0;d--)
    { TMultiTableIndex *ti=GetIndexFAST(d);
      int index=ti->Index+1;
      if(index<ti->ScaleCount)
        { ti->Index=index;
          break;
        }
      // ��������� �� ������� d - ���������� �� ���� � ����������� ���������
      ti->Index=0;
      if(d==0) // �������� �������
        { finish=true;
          break;
        }
    }
  return finish;
}
//---------------------------------------------------------------------------

#ifdef MULTITABLE_DEBUG
// ���������� ����������
AnsiString __fastcall TMultiTable::Debug(bool showinterp)
{ AnsiString s,s1;
  bool finish;

  if(Data==NULL) return "��� ������";
  Loop_ResetIndices();
  for(;;)
    { double *a;
      s1="";
      // ������� ������� �������
      for(int d=0;d<Dim;d++)
        s1.cat_sprintf("%4d",GetDimensionIndexFAST(d));
      s1+=" : ";
      a=GetValuesByIndices();
      if(!a)
        s1+="������ GetValuesByIndices";
      else
        { for(int i=0;i<ValCount;i++)
            s1.cat_sprintf("%6.1lf",a[i]);
        }
      s+=s1+"\r\n";
      // ��������� ������
      if(Loop_NextElement())
        break;
      /*
      for(int d=Dim-1;d>=0;d--)
        { int index=Index[d].Index+1;
          if(index<Index[d].ScaleCount)
            { Index[d].Index=index;
              break;
            }
          // ��������� �� ������� d - ���������� �� ���� � ����������� ���������
          Index[d].Index=0;
          if(d==0) // �������� �������
            finish=true;
        }
      if(finish)
        break;
      */
    }

  if(!showinterp)
    return s;

  s+="-------------\r\n";

  // �� �� ��� ���������������� �������
  finish=false;
  for(int d=0;d<Dim;d++)
    SetDimensionInterIndexFAST(d,0);
  for(;;)
    { double *a;
      s1="";
      // ������� ������� �������
      for(int d=0;d<Dim;d++)
        s1.cat_sprintf("%4d",GetDimensionInterIndexFAST(d));
      s1+=" : ";
      a=GetInterValuesByIndices();
      if(!a)
        s1+="������ GetValuesByIndices";
      else
        { for(int i=0;i<ValCount;i++)
            s1.cat_sprintf("%8.3lf",a[i]);
        }
      s+=s1+"\r\n";
      // ��������� ������
      for(int d=Dim-1;d>=0;d--)
        { int index=Index[d].InterIndex+1;
          if(index<2)
            { Index[d].InterIndex=index;
              break;
            }
          // ��������� �� ������� d - ���������� �� ���� � ����������� ���������
          Index[d].InterIndex=0;
          if(d==0) // �������� �������
            finish=true;
        }
      if(finish)
        break;
    }


  return s;
}
//---------------------------------------------------------------------------
#endif

// ���� ��� ���������� � ��������
#define TMTTAG_START    0       // ������ ������
#define TMTTAG_PREFIX   "MultiTableData" // �������
#define TMTTAG_VER      1       // ������ �������
#define TMTTAG_DIM      2       // �����������
#define TMTTAG_INDEX    3       // ������ �����
#define TMTTAG_DATA     4       // ������ ������
#define TMTTAG_END      5       // �����
#define TMTTAG_VALNAMES 6       // ����� ��������
#define TMTTAG_RESTORED 7       // ������������� ��������
#define TMTTAG_RESTMODE 8       // ����� �������������� ��������


// ���������
bool __fastcall TMultiTable::Save(TMultiTableSaver *s)
{ char prefix[]=TMTTAG_PREFIX;
  BYTE tag;
  int n;
  #define WRITEVAR(v) if(!s->Write(&(v),sizeof(v))) return false;
  #define WRITETAG(b) tag=b; WRITEVAR(tag)

  WRITETAG(TMTTAG_START)
  if(!s->Write(prefix,strlen(prefix)))
    return false;

  // ������
  n=0;
  WRITETAG(TMTTAG_VER)
  WRITEVAR(n)

  // �����������
  WRITETAG(TMTTAG_DIM)
  WRITEVAR(Dim)
  WRITEVAR(ValCount)

  // �����
  for(int i=0;i<Dim;i++)
    { TMultiTableIndex *index=GetIndexFAST(i);
      WRITETAG(TMTTAG_INDEX)
      if(!s->WriteString(index->Name)) return false;
      WRITEVAR(index->ScaleCount)
      if(index->ScaleCount)
        { if(!s->Write(index->Scale,index->ScaleCount*sizeof(double)))
            return false;
        }
    }
  // ���� ������
  if(Data)
    { WRITETAG(TMTTAG_DATA)
      WRITEVAR(DataSize);
      if(!s->Write(Data,DataSize*sizeof(double)))
        return false;
    }

  // ����� ��������
  if(ValNames)
    { WRITETAG(TMTTAG_VALNAMES)
      WRITEVAR(ValNamesCount)
      for(int i=0;i<ValNamesCount;i++)
        if(!s->WriteString(ValNames[i]))
          return false;
    }

  // ������������� ��������
  WRITETAG(TMTTAG_RESTORED)
  WRITEVAR(RestoredRecords)

  // ��� ����������������� �������
  WRITETAG(TMTTAG_RESTMODE)
  WRITEVAR(RestoreMode)

  // �����
  WRITETAG(TMTTAG_END)

  return true;

  #undef WRITETAG
  #undef WRITEVAR
}
//---------------------------------------------------------------------------

// ���������
bool __fastcall TMultiTable::Load(TMultiTableSaver *s)
{ if(!LoadAux(s))
    { Clear();
      SetError(MTERROR_LOADERR);
      return false;
    }
  ResetError();
  return true;
}
//---------------------------------------------------------------------------

// ��������� - ���������������
bool __fastcall TMultiTable::LoadAux(TMultiTableSaver *s)
{ char prefix[]=TMTTAG_PREFIX;
  BYTE tag;
  char *aux;
  int plen=strlen(prefix),dim,vals;
  int lastindex=-1;
  TMultiTableIndex *index;

  Clear();

  #define READVAR(v) if(!s->Read(&(v),sizeof(v))) return false;

  READVAR(tag)
  if(tag!=TMTTAG_START)
    return false;

  aux=new char[plen];
  if(!s->Read(aux,plen))
    { delete[] aux;
      return false;
    }
  if(strncmp(aux,prefix,plen)!=0)
    { delete[] aux;
      return false;
    }
  delete[] aux;

  for(;;)
    { READVAR(tag);
      switch(tag)
        { case TMTTAG_VER: // ������ (���� �� ����������)
            READVAR(plen);
            break;

          case TMTTAG_END: // �����
            return true;

          case TMTTAG_DIM: // �����������
            READVAR(dim)
            READVAR(vals)
            SetDimension(dim);
            SetValuesCount(vals);
            break;

          case TMTTAG_INDEX: // ������ �����
            lastindex++;
            index=GetIndexFAST(lastindex);
            if(index->Name)
              { delete[] index->Name;
                index->Name=NULL;
              }
            if(!s->ReadString(&(index->Name))) return false;
            READVAR(plen)
            if(!plen)
              break;
            index->AllocateScale(plen);
            if(!s->Read(index->Scale,index->ScaleCount*sizeof(double)))
              return false;
            break;

          case TMTTAG_DATA: // ������ ������
            AllocateData();
            READVAR(plen)
            if(plen!=DataSize)
              return false;
            if(!s->Read(Data,DataSize*sizeof(double)))
              return false;
            break;

          case TMTTAG_VALNAMES: // ����� ��������
            READVAR(plen)
            if(plen)
              { for(int i=0;i<plen;i++)
                  { char *str;
                    if(!s->ReadString(&str))
                      return false;
                    AcceptValName(i,str);
                  }
              }
            break;

          case TMTTAG_RESTORED: // ������������� ��������
            READVAR(RestoredRecords)
            break;

          case TMTTAG_RESTMODE: // ��� ����������������� �������
            READVAR(RestoreMode)
            break;
        }
    }

  #undef READVAR
}
//---------------------------------------------------------------------------

