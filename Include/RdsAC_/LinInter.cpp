//---------------------------------------------------------------------------
// �������� ������������ �� ��������
//---------------------------------------------------------------------------
#ifndef RdsLinInterH
#define RdsLinInterH
// ������� CommonAC.hpp
#ifndef CommonACH
  #include <CommonAC.hpp>
#endif
//---------------------------------------------------------------------------
// ����� �������� ������������ ������� ��� ���������
class TRdsLinearInterpolation
{ protected:
    // ����������� ������ � �������
    // ��� (������� ��� ���������)
    int Type;
    // ����
    #define RDSLININTER_T_UNKNOWN       0       // ���������� (���������� ��������������)
    #define RDSLININTER_T_SINGLE        1       // �������
    #define RDSLININTER_T_FIELD         2       // ���������
    #define RDSLININTER_T_ERROR         3       // ������
    BOOL ByRows;                // TRUE - �����-������, FALSE - �����-�������
                                // ��� ��������: TRUE - ������ �������� � ������� �������, FALSE - � ������� ������
    BOOL InputMatrixSorted;     // ������� ������� �������������
    RDS_ARRAYACCESSDATA Data;   // ������ �������
    // �������������� ����� (������������ � ������� Setup)
    #define RDSLININTER_F_BYROWS        0x10     // ��. ByRows
    #define RDSLININTER_F_NOTSORTED     0x20     // �������� � InputMatrixSorted

    // ������ �������� �������������
    // ��������
    int LastSearchResult;       // ��������� �������� ������
    int lastXIndex,lastYIndex;  // ������� �������
    double LastXL,LastXR;       // ��������� ������

    // ��������
    int LastKSearchResult;      // ��������� �������� ������
    double LastKL,LastKR;       // ��������� �������� ���������

    // ��������� ������
   #define RDSLININTER_NONE     0       // ��� ������
   #define RDSLININTER_BELOW    1       // x ����� �������
   #define RDSLININTER_ABOVE    2       // x ������ �������
   #define RDSLININTER_BETWEEN  3       // x ������ �������

    // ��� ������� �������
    double LastK,LastB;         // ��������� ��������� ������

    // ��� ��������
    int LeftXIndex,RightXIndex,LeftKIndex,RightKIndex;


    // ����������� �������
    BOOL SortMatrix(BOOL databyrows,int Rows,int Cols,double *MData,int XIndex);
    // ����������� ������� ��� ��������� (� ��������� ������ �������)
    BOOL SortMatrixField(BOOL databyrows,int Rows,int Cols,
                         double *MData,int XIndex,int excludeindex);
    // ����������
    BOOL ModelSingle(double ext_x,double *py,int XIndex,int YIndex);
    BOOL ModelField(double ext_x1,int index_x1,double ext_x2,int index_x2,double *py);

    // �����
    inline void Reset(void)
      { InputMatrixSorted=FALSE; LastSearchResult=LastKSearchResult=RDSLININTER_NONE; lastXIndex=lastYIndex=-1;
        Type=RDSLININTER_T_UNKNOWN; };
    // ����� �������������� ��������
    // array        - ������
    // count        - ����� �����
    // itemshift    - ���������� ����� ��������� ������� � �������
    // pLeft,pRight - ������������ ������� �������������� �����
    // �������      - RDSLININTER_*
    static int FindNearestPoints(double *array,int count,int itemshift,double x,int *pLeft,int *pRight);
    // ����� �������������� �������� � ������� � ��������� ������
    // array        - ������
    // count        - ����� �����
    // itemshift    - ���������� ����� ��������� ������� � �������
    // exindex      - ������������ ������ (������ ����������� ��������� ������ �������, -1 �� �����������)
    // pLeft,pRight - ������������ ������� �������������� �����
    // �������      - RDSLININTER_*
    static int FindNearestPointsEx(double *array,int count,int itemshift,int exindex,double x,int *pLeft,int *pRight);
  public:
    // ��������� �� ������� (���������� ��� ��������� ������� ������� ��� ������ � ���)
    void Setup(rdsbcppVarAncestor &M,int flags=RDSLININTER_T_UNKNOWN);

    // ��������� �������� ������� ����� ����������
    //   XIndex - ������ ������ (�������) ���������
    //   YIndex - ������ ������ (�������) ��������
    //   x      - ��������
    double Calc_Fx(double x,int XIndex=0,int YIndex=1);
    // ��������� �������� ������� ���� ����������
    //   x1      - �������� 1
    //   x2      - �������� 2
    double Calc_Fx1x2(double x1,double x2);
    // ��������� �������� ������� ���� ����������
    //   x1      - �������� 1
    //   i2      - ������ � ������� ��������� 2
    double Calc_Fx1i2(double x1,int i2);
    // ��������� �������� ������� ���� ����������
    //   i1      - ������ � ������� ��������� 1
    //   x2      - �������� 2
    double Calc_Fi1x2(int i1,double x2);

    // ��������� �������� ������� ����� ���������� (���������� ������)
    inline double operator()(double x){ return Calc_Fx(x); };
    // ��������� �������� ������� ���� ����������  ���������� ������)
    inline double operator()(double x1,double x2){ return Calc_Fx1x2(x1,x2); };

    TRdsLinearInterpolation(void)
      { Reset(); };
};
//---------------------------------------------------------------------------

// ��������� �� �������
void TRdsLinearInterpolation::Setup(rdsbcppVarAncestor &M,int flags)
{ double *arr;
  int type=flags & 0xf;

  Reset();
  ByRows=((flags & RDSLININTER_F_BYROWS)!=0); //byrows;
  InputMatrixSorted=((flags & RDSLININTER_F_NOTSORTED)==0); //sorted;

  if(!M.GetAccessData(&Data))
    { Type=RDSLININTER_T_ERROR;
      return;
    }
  if((!Data.Exists) || Data.ItemSize!=sizeof(double))
    { Type=RDSLININTER_T_ERROR;
      return;                     
    }
  arr=(double*)(Data.Data);

  if(type==RDSLININTER_T_UNKNOWN)
    { if((*arr)==rdsbcppHugeDouble)
        Type=RDSLININTER_T_FIELD;
      else
        Type=RDSLININTER_T_SINGLE;
    }
  else
    Type=type;
}
//---------------------------------------------------------------------------

// ����������� ������� ��� ������� ����� ����������
BOOL TRdsLinearInterpolation::SortMatrix(BOOL databyrows,int Rows,int Cols,double *MData,int XIndex)
{

  InputMatrixSorted=TRUE;

  if(databyrows)
    { // ������� - ��������, �� ������������� �� ���
      BOOL sorted=TRUE;
      double *rowBaseX=MData+XIndex*Cols,*rowBaseY;
      double last=*rowBaseX,cur;
      if(last==rdsbcppHugeDouble)
        return FALSE;
      for(int i=1;sorted && i<Cols;i++)
        { cur=rowBaseX[i];
          if(cur==rdsbcppHugeDouble)
            return FALSE;
          if(cur<last)
            sorted=FALSE;
          else
            last=cur;
        }
      if(sorted) // ��� � �������
        return TRUE;
      // ����� ����������� �� ������ XIndex
      //rowBaseY=MData+YIndex*Cols;
      for(int i=0;i<Cols-1;i++)
        { // ����� ������������ �������� �� i �� Cols
          int minindex=i;
          last=rowBaseX[i];
          for(int j=i+1;j<Cols;j++)
            { cur=rowBaseX[j];
              if(cur<last)
                { last=cur;
                  minindex=j;
                }
            }
          if(i!=minindex)
            { // �������� ������� ������ i � minindex � ������ ������
              rowBaseY=MData;
              for(int j=0;j<Rows;j++,rowBaseY+=Cols)
                { cur=rowBaseY[minindex];
                  rowBaseY[minindex]=rowBaseY[i];
                  rowBaseY[i]=cur;
                }
            }
        } // for(int i=0;...)

    } // if(databyrows)
  else // ���������� �� ������� XIndex
    { // ������� - ��������, �� ������������� �� ���
      BOOL sorted=TRUE;
      double last=MData[XIndex],cur;
      if(last==rdsbcppHugeDouble)
        return FALSE;
      for(int i=1;sorted && i<Rows;i++)
        { cur=MData[XIndex+i*Cols];
          if(cur==rdsbcppHugeDouble)
            return FALSE;
          if(cur<last)
            sorted=FALSE;
          else
            last=cur;
        }
      if(sorted) // ��� � �������
        return TRUE;
      // ����� ����������� �� ������� XIndex
      for(int i=0;i<Rows-1;i++)
        { // ����� ������������ �������� �� i �� Rows
          int minindex=i;
          last=MData[XIndex+i*Cols];
          for(int j=i+1;j<Rows;j++)
            { cur=MData[XIndex+j*Cols];
              if(cur<last)
                { last=cur;
                  minindex=j;
                }
            }
          if(i!=minindex)
            { // �������� ������� ������ i � minindex � ������ �������
              double *cell_mi=MData+minindex*Cols,
                     *cell_i=MData+i*Cols;
              for(int j=0;j<Cols;j++)
                { cur=cell_i[j];
                  cell_i[j]=cell_mi[j];
                  cell_mi[j]=cur;
                }
            }
        } // for(int i=0;...)
    }

  return TRUE;
}
//---------------------------------------------------------------------------

// ����� �������������� ��������
// array        - ������
// count        - ����� �����
// itemshift    - ���������� ����� ��������� ������� � �������
// pLeft,pRight - ������������ ������� �������������� �����
// �������      - RDSLININTER_*
int TRdsLinearInterpolation::FindNearestPoints(double *array,int count,int itemshift,double x,int *pLeft,int *pRight)
{ int left=0,right=count-1,mid,diap;
  double xleft,xright,xmid;

  if(count==1)
    { *pLeft=0;
      *pRight=0;
      return RDSLININTER_NONE;
    }

  xleft=array[0];
  xright=array[(count-1)*itemshift];
  if(xleft==rdsbcppHugeDouble || xright==rdsbcppHugeDouble)
    { *pLeft=0;
      *pRight=0;
      return RDSLININTER_NONE;
    }
  if(x<xleft)
    { *pLeft=0;
      *pRight=1;
      return RDSLININTER_BELOW;
    }
  if(x>xright)
    { *pLeft=count-2;
      *pRight=count-1;
      return RDSLININTER_ABOVE;
    }

  for(;;)
    switch(diap=right-left)
      { case 0:
        case 1:
          *pLeft=left;
          *pRight=right;
          return diap==0?RDSLININTER_NONE:RDSLININTER_BETWEEN;

        default: // ���� � ����� ����� ����� left � right
          mid=left+diap/2;
          xmid=array[mid*itemshift];
          if(xmid==rdsbcppHugeDouble)
            { *pLeft=0;
              *pRight=0;
              return RDSLININTER_NONE;
            }
          if(x<xmid)
            { right=mid;
            }
          else
            { left=mid;
            }
      }

}
//---------------------------------------------------------------------------


// ������������� (�������)
// ext_x   - ���� ���������
// py      - ����� ��������
// XIndex  - ������ ������ (�������) ���������
// YIndex  - ������ ������ (�������) ��������
// ������� - ���������� (���� ��������)
BOOL TRdsLinearInterpolation::ModelSingle(double ext_x,double *py,int XIndex,int YIndex)
{ int Rows,Cols,leftindex,rightindex,itemshift;
  double *MData,x,y,*xarr,*yarr;
  BOOL ok;

  x=ext_x;

  if(!Data.Exists) // ��� �������
    return FALSE;

  if(x==rdsbcppHugeDouble)
    { *py=rdsbcppHugeDouble;
      return TRUE;
    }

  Rows=Data.Rows;
  Cols=Data.Cols;
  if(ByRows)
    { if(XIndex>=Rows || YIndex>=Rows || XIndex<0 || YIndex<0)
        return FALSE;
    }
  else
    { if(XIndex>=Cols || YIndex>=Cols || XIndex<0 || YIndex<0)
        return FALSE;
    }
  MData=(double*)(Data.Data);

  if(!InputMatrixSorted)
    { if(!SortMatrix(ByRows,Rows,Cols,MData,XIndex))
       { *py=rdsbcppHugeDouble;
         LastSearchResult=RDSLININTER_NONE;
         InputMatrixSorted=FALSE;
         return FALSE;
       }
      LastSearchResult=RDSLININTER_NONE; // ������ � ������� ����������
    }

  // ������� ������������� � � ��� ���� ������ (�������) XIndex � YIndex

  // �������� ����������� �������� �������������. ���� x ������� � ��� �� ���������,
  // ����� ����� ����������� �� �����.
  switch(LastSearchResult)
    { case RDSLININTER_BELOW:
        ok=(lastXIndex==XIndex && lastYIndex==YIndex && x<LastXL);
        break;
      case RDSLININTER_ABOVE:
        ok=(lastXIndex==XIndex && lastYIndex==YIndex && x>LastXR);
        break;
      case RDSLININTER_BETWEEN:
        ok=(lastXIndex==XIndex && lastYIndex==YIndex && x>LastXL && x<LastXR);
        break;
      default:
        ok=FALSE;
    }

  if(ok) // ����� ����� �� �����
    { *py=LastK*x+LastB;
      return TRUE;
    }
  lastXIndex=XIndex;
  lastYIndex=YIndex;

  // ����� ����� ������� ����� ������ (�������) XIndex, ����� �������� ���������� x
  if(ByRows)
    { LastSearchResult=FindNearestPoints(xarr=MData+XIndex*Cols,Cols,itemshift=1,x,&leftindex,&rightindex);
      yarr=MData+YIndex*Cols;
    }
  else
    { LastSearchResult=FindNearestPoints(xarr=MData+XIndex,Rows,itemshift=Cols,x,&leftindex,&rightindex);
      yarr=MData+YIndex;
    }

  if(leftindex==rightindex)
    y=yarr[leftindex*itemshift];
  else
    { double yleft,yright;
      LastXL=xarr[leftindex*itemshift],
      LastXR=xarr[rightindex*itemshift],
      yleft=yarr[leftindex*itemshift],
      yright=yarr[rightindex*itemshift];
      if(LastXL==rdsbcppHugeDouble || LastXR==rdsbcppHugeDouble ||
         yleft==rdsbcppHugeDouble || yright==rdsbcppHugeDouble)
        { y=rdsbcppHugeDouble;
          LastSearchResult=RDSLININTER_NONE;
        }
      else
        { double dx=LastXR-LastXL;
          try
            { if(dx==0.0)
                { y=yright;
                  LastK=0.0;
                  LastB=yright;
                }
              else
                { LastK=(yright-yleft)/dx;
                  LastB=yright-LastK*LastXR;
                  y=LastK*x+LastB;
                }
            }
          catch(...)
            { y=yright;
              LastK=0.0;
              LastB=yright;
            }
        }
    }

  *py=y;

  return TRUE;
}
//---------------------------------------------------------------------------

// ��������� �������� ������� ����� ����������
//   XIndex - ������ ������ (�������) ���������
//   YIndex - ������ ������ (�������) ��������
//   x      - ��������
double TRdsLinearInterpolation::Calc_Fx(double x,int XIndex,int YIndex)
{ double y;
  switch(Type)
    { case RDSLININTER_T_SINGLE: // �������
        if(!ModelSingle(x,&y,XIndex,YIndex))
          return rdsbcppHugeDouble;
        break;
      case RDSLININTER_T_FIELD: // ���������
        if(!ModelField(x,-1,0.0,1,&y))
          return rdsbcppHugeDouble;
        break;
      default:
        return rdsbcppHugeDouble;
    }
  return y;
}
//---------------------------------------------------------------------------

// ����������� ������� ��� ��������� (� ��������� ������ �������)
BOOL TRdsLinearInterpolation::SortMatrixField(BOOL databyrows,int Rows,int Cols,
                                              double *MData,int XIndex,int excludeindex)
{

  InputMatrixSorted=TRUE;

  if(databyrows)
    { // ������� - ��������, �� ������������� �� ���
      int start;
      BOOL sorted=TRUE;
      double *rowBaseX=MData+XIndex*Cols,*rowBaseY;
      double last,cur;
      if(excludeindex==0)
        { if(Cols<2) return TRUE; // ���������� ����������
          last=rowBaseX[1];
          start=2;
        }
      else
        { last=*rowBaseX;
          start=1;
        }
      if(last==rdsbcppHugeDouble)
        return FALSE;
      for(int i=start;sorted && i<Cols;i++)
        if(i!=excludeindex)
          { cur=rowBaseX[i];
            if(cur==rdsbcppHugeDouble)
              return FALSE;
            if(cur<last)
              sorted=FALSE;
            else
              last=cur;
          }
      if(sorted) // ��� � �������
        return TRUE;
      // ����� ����������� �� ������ XIndex
      //rowBaseY=MData+YIndex*Cols;
      for(int i=0;i<Cols-1;i++)
        if(i!=excludeindex)
          { // ����� ������������ �������� �� i �� Cols
            int minindex=i;
            last=rowBaseX[i];
            for(int j=i+1;j<Cols;j++)
              if(j!=excludeindex)
                { cur=rowBaseX[j];
                  if(cur<last)
                    { last=cur;
                      minindex=j;
                    }
                }
            if(i!=minindex)
              { // �������� ������� ������ i � minindex � ������ ������
                rowBaseY=MData;
                for(int j=0;j<Rows;j++,rowBaseY+=Cols)
                  { cur=rowBaseY[minindex];
                    rowBaseY[minindex]=rowBaseY[i];
                    rowBaseY[i]=cur;
                  }
              }
          } // for(int i=0;...)

    } // if(databyrows)
  else // ���������� �� ������� XIndex
    { // ������� - ��������, �� ������������� �� ���
      int start;
      BOOL sorted=TRUE;
      double last,cur;
      if(excludeindex==0)
        { if(Rows<2) return TRUE; // ���������� ����������
          last=MData[XIndex+Cols];
          start=2;
        }
      else
        { last=MData[XIndex];
          start=1;
        }
      if(last==rdsbcppHugeDouble)
        return FALSE;
      for(int i=start;sorted && i<Rows;i++)
        if(i!=excludeindex)
          { cur=MData[XIndex+i*Cols];
            if(cur==rdsbcppHugeDouble)
              return FALSE;
            if(cur<last)
              sorted=FALSE;
            else
              last=cur;
          }
      if(sorted) // ��� � �������
        return TRUE;
      // ����� ����������� �� ������� XIndex
      for(int i=0;i<Rows-1;i++)
        if(i!=excludeindex)
          { // ����� ������������ �������� �� i �� Rows
            int minindex=i;
            last=MData[XIndex+i*Cols];
            for(int j=i+1;j<Rows;j++)
              if(j!=excludeindex)
                { cur=MData[XIndex+j*Cols];
                  if(cur<last)
                    { last=cur;
                      minindex=j;
                    }
                }
            if(i!=minindex)
              { // �������� ������� ������ i � minindex � ������ �������
                double *cell_mi=MData+minindex*Cols,
                       *cell_i=MData+i*Cols;
                for(int j=0;j<Cols;j++)
                  { cur=cell_i[j];
                    cell_i[j]=cell_mi[j];
                    cell_mi[j]=cur;
                  }
              }
          } // for(int i=0;...)
    }
  return TRUE;
}
//---------------------------------------------------------------------------

// ����� �������������� �������� � ������� � ��������� ������
// array        - ������
// count        - ����� �����
// itemshift    - ���������� ����� ��������� ������� � �������
// exindex      - ������������ ������ (������ ����������� ��������� ������ �������, -1 �� �����������)
// pLeft,pRight - ������������ ������� �������������� �����
// �������      - RDSLININTER_*
int TRdsLinearInterpolation::FindNearestPointsEx(double *array,int count,int itemshift,int exindex,double x,int *pLeft,int *pRight)
{ int left=0,right=count,mid,diap,c_minus_1=count-1;
  double xleft,xright,xmid;

  if(exindex<0 || exindex>=count)
    return FindNearestPoints(array,count,itemshift,x,pLeft,pRight);

  if(exindex==0)
    { // �������� ������ �������� - �������� ������ ������� ������� �� ������ �����
      // � �������� ������� �� 1
      mid=FindNearestPoints(array+itemshift,c_minus_1,itemshift,x,pLeft,pRight);
      (*pLeft)++;
      (*pRight)++;
      return mid;
    }
  if(exindex==c_minus_1) // �������� ��������� �������� - �������� ������ ������� ��� �������� �������
    return FindNearestPoints(array,c_minus_1,itemshift,x,pLeft,pRight);

  // exindex ���-�� ������ ������� � �� ���� �� ������� �����
  // ����������, ��� ������ ������� �� ������ ����

  xleft=array[0];
  xright=array[c_minus_1*itemshift];
  if(xleft==rdsbcppHugeDouble || xright==rdsbcppHugeDouble)
    { *pLeft=0;
      *pRight=0;
      return RDSLININTER_NONE;
    }

  if(x<xleft)
    { *pLeft=0;
      *pRight=(exindex==1)?2:1;
      return RDSLININTER_BELOW;
    }
  if(x>xright)
    { int c_minus_2=count-2;
      *pLeft=(exindex==c_minus_2)?(count-3):c_minus_2;
      *pRight=c_minus_1;
      return RDSLININTER_ABOVE;
    }

  for(;;)
    { diap=right-left;
      switch(diap)
        { case 0:
          case 1:
            *pLeft=left;
            *pRight=right;
            return diap==0?RDSLININTER_NONE:RDSLININTER_BETWEEN;

          case 2: // ����� ���� ����� ����� left � right
            if(exindex==left+1) // ��� ����� - exindex
              { *pLeft=left;
                *pRight=right;
                return RDSLININTER_BETWEEN;
              }

          default: // ��� � ����� ����� ����� left � right,
                   // ��� ���� �����, �� �� exindex
            mid=left+diap/2;
            if(mid==exindex) // ������ � exindex - ������� ��������
              { mid--;
                if(mid==left) // ��������
                  mid+=2;
              }
            xmid=array[mid*itemshift];
            if(xmid==rdsbcppHugeDouble)
              { *pLeft=0;
                *pRight=0;
                return RDSLININTER_NONE;
              }
            if(x<xmid)
              { right=mid;
              }
            else
              { left=mid;
              }
        }
    }

}
//---------------------------------------------------------------------------

// ������������� (���������)
// ext_x1   - ���� ������� ���������
// index_x1 - ������ ������� ��������� (������ �����, ���� <0, ������� ext_x1)
// ext_x2   - ���� ������� ���������
// index_x2 - ������ ������� ��������� (������ �����, ���� <0, ������� ext_x2)
// py       - ����� ��������
// �������  - ���������� (���� ��������)
BOOL TRdsLinearInterpolation::ModelField(double ext_x1,int index_x1,double ext_x2,int index_x2,double *py)
{ int Rows,Cols,itemshift_x,itemshift_k;
  double *MData,x,y,k,*yarr_kl,*yarr_kr,dx;
  double y_kl_xl,y_kl_xr,y_kr_xl,y_kr_xr,yl,yr;
  BOOL x_ok,k_ok,x_by_index=FALSE,k_by_index=FALSE;

  #define ext_x ext_x2
  #define ext_k ext_x1
  #define index_x index_x2
  #define index_k index_x1
  #define XIndex 0

  x=ext_x;
  k=ext_k;

  if(!Data.Exists) // ��� �������
    return FALSE;

  Rows=Data.Rows;
  Cols=Data.Cols;
  if(Rows<2 || Cols<2)
    return FALSE;
  MData=(double*)(Data.Data);

  if(index_x<=0) // ������ ��������
    { if(x==rdsbcppHugeDouble)
        { *py=rdsbcppHugeDouble;
          return TRUE;
        }
    }
  else // ����� ������
    { if(index_x>=(ByRows?Cols:Rows))
        { *py=rdsbcppHugeDouble;
          return TRUE;
        }
      x_by_index=TRUE;
    }


  if(index_k<=0) // ������ ��������
    { if(k==rdsbcppHugeDouble)
        { *py=rdsbcppHugeDouble;
          return TRUE;
        }
    }
  else // ����� ������
    { if(index_k>=(ByRows?Rows:Cols))
        { *py=rdsbcppHugeDouble;
          return TRUE;
        }
      k_by_index=TRUE;
    }

  if(!InputMatrixSorted)
    { // K
      if(!SortMatrixField(!ByRows,Rows,Cols,MData,0,0))
        { *py=rdsbcppHugeDouble;
          LastKSearchResult=RDSLININTER_NONE;
          InputMatrixSorted=FALSE;
          return TRUE;
        }
      // x
      if(!SortMatrixField(ByRows,Rows,Cols,MData,0,0))
        { *py=rdsbcppHugeDouble;
          LastSearchResult=RDSLININTER_NONE;
          InputMatrixSorted=FALSE;
          return TRUE;
        }
      LastSearchResult=LastKSearchResult=RDSLININTER_NONE;
    }


  // �������� ����������� �������� �������������. ���� x ������� � ��� �� ���������,
  // ����� ����� ����������� �� �����.
  if(x_by_index)
    { LeftXIndex=RightXIndex=index_x;
      LastSearchResult=RDSLININTER_NONE;
      x_ok=TRUE;
    }
  else switch(LastSearchResult)
    { case RDSLININTER_BELOW:
        x_ok=x<LastXL;
        break;
      case RDSLININTER_ABOVE:
        x_ok=x>LastXR;
        break;
      case RDSLININTER_BETWEEN:
        x_ok=x>LastXL && x<LastXR;
        break;
      default:
        x_ok=FALSE;
    }

  // �������� ����������� �������� �������������. ���� k ������� � ��� �� ���������,
  // ����� ����� ����������� �� �����.
  if(k_by_index)
    { LeftKIndex=RightKIndex=index_k;
      LastKSearchResult=RDSLININTER_NONE;
      k_ok=TRUE;
    }
  else switch(LastKSearchResult)
    { case RDSLININTER_BELOW:
        k_ok=k<LastKL;
        break;
      case RDSLININTER_ABOVE:
        k_ok=k>LastKR;
        break;
      case RDSLININTER_BETWEEN:
        k_ok=k>LastKL && k<LastKR;
        break;
      default:
        k_ok=FALSE;
    }

  // ����� ����� ������� ����� ������ (�������) XIndex, ����� �������� ���������� x
  if(ByRows) // ������ � �������
    { itemshift_x=1;
      itemshift_k=Cols;
      if(!x_ok)
        { double *xarr;
          LastSearchResult=FindNearestPointsEx(xarr=MData+XIndex*Cols,Cols,
                                               itemshift_x,0,x,&LeftXIndex,&RightXIndex);
          LastXL=xarr[LeftXIndex*itemshift_x];
          LastXR=xarr[RightXIndex*itemshift_x];
        }
      if(!k_ok)
        { double *karr;
          LastKSearchResult=FindNearestPointsEx(karr=MData,Rows,
                                                itemshift_k,XIndex,k,&LeftKIndex,&RightKIndex);
          LastKL=karr[LeftKIndex*itemshift_k];
          LastKR=karr[RightKIndex*itemshift_k];
        }
      yarr_kl=MData+LeftKIndex*Cols;
      yarr_kr=MData+RightKIndex*Cols;
    }
  else
    { itemshift_x=Cols;
      itemshift_k=1;
      if(!x_ok)
        { double *xarr;
          LastSearchResult=FindNearestPointsEx(xarr=MData+XIndex,Rows,
                                          itemshift_x,0,x,&LeftXIndex,&RightXIndex);
          LastXL=xarr[LeftXIndex*itemshift_x];
          LastXR=xarr[RightXIndex*itemshift_x];
        }
      if(!k_ok)
        { double *karr;
          LastKSearchResult=FindNearestPointsEx(karr=MData+XIndex*Cols,Cols,
                                                itemshift_k,XIndex,k,&LeftKIndex,&RightKIndex);
          LastKL=karr[LeftKIndex*itemshift_k];
          LastKR=karr[RightKIndex*itemshift_k];
        }
      yarr_kl=MData+LeftKIndex;
      yarr_kr=MData+RightKIndex;
    }

  // ������� ������� � ��������
  y_kl_xl=yarr_kl[LeftXIndex*itemshift_x];
  y_kl_xr=yarr_kl[RightXIndex*itemshift_x];
  y_kr_xl=yarr_kr[LeftXIndex*itemshift_x];
  y_kr_xr=yarr_kr[RightXIndex*itemshift_x];


  if(y_kl_xl==rdsbcppHugeDouble || y_kl_xr==rdsbcppHugeDouble ||
     y_kr_xl==rdsbcppHugeDouble || y_kr_xr==rdsbcppHugeDouble)
    { *py=rdsbcppHugeDouble;
      LastSearchResult=RDSLININTER_NONE;
      return TRUE;
    }
  if(!k_by_index)
    { if(LastKL==rdsbcppHugeDouble || LastKR==rdsbcppHugeDouble)
        { *py=rdsbcppHugeDouble;
          LastSearchResult=RDSLININTER_NONE;
          return TRUE;
        }
    }

  // ������� �������������� ����� - ������������� y
  k_ok=TRUE;
  if(LeftKIndex==RightKIndex || LastKL==LastKR)
    { yl=y_kr_xl;
      yr=y_kr_xr;
    }
  else
    { double dk=LastKR-LastKL;
      try
        { yl=y_kl_xl+(k-LastKL)*(y_kr_xl-y_kl_xl)/dk;
          yr=y_kl_xr+(k-LastKL)*(y_kr_xr-y_kl_xr)/dk;
        }
      catch(...)
        { k_ok=FALSE; }
    }
  if(!k_ok)
    { yl=(y_kr_xl+y_kl_xl)/2;
      yr=(y_kr_xr+y_kl_xr)/2;
    }

  // ������� yl � yr ������, ��������������� �������� k
  try
    { if(LeftXIndex==RightXIndex || LastXR==LastXL)
        *py=yr;
      else
        { dx=LastXR-LastXL;
          LastK=(yr-yl)/dx,
          LastB=yr-LastK*LastXR;
          *py=LastK*x+LastB;
        }
    }
  catch(...)
    { *py=yr; }

  return TRUE;
  #undef ext_x
  #undef ext_k
  #undef XIndex
  #undef index_x
  #undef index_k
}
//---------------------------------------------------------------------------

// ��������� �������� ������� ���� ����������
//   x1      - �������� 1
//   x2      - �������� 2
double TRdsLinearInterpolation::Calc_Fx1x2(double x1,double x2)
{ double y;
  switch(Type)
    { case RDSLININTER_T_SINGLE: // �������
        if(!ModelSingle(x1,&y,0,1))
          return rdsbcppHugeDouble;
        break;
      case RDSLININTER_T_FIELD: // ���������
        if(!ModelField(x1,-1,x2,-1,&y))
          return rdsbcppHugeDouble;
        break;
      default:
        return rdsbcppHugeDouble;
    }
  return y;
}
//---------------------------------------------------------------------------

// ��������� �������� ������� ���� ����������
//   x1     - �������� 1
//   i2     - ������ � ������� ��������� 2
double TRdsLinearInterpolation::Calc_Fx1i2(double x1,int i2)
{ double y;
  switch(Type)
    { case RDSLININTER_T_SINGLE: // �������
        if(!ModelSingle(x1,&y,0,1))
          return rdsbcppHugeDouble;
        break;
      case RDSLININTER_T_FIELD: // ���������
        if(!ModelField(x1,-1,rdsbcppHugeDouble,i2,&y))
          return rdsbcppHugeDouble;
        break;
      default:
        return rdsbcppHugeDouble;
    }
  return y;
}
//---------------------------------------------------------------------------

// ��������� �������� ������� ���� ����������
//   i1      - ������ � ������� ��������� 1
//   x2      - �������� 2
double TRdsLinearInterpolation::Calc_Fi1x2(int i1,double x2)
{ double y;
  switch(Type)
    { case RDSLININTER_T_SINGLE: // �������
        return rdsbcppHugeDouble;
      case RDSLININTER_T_FIELD: // ���������
        if(!ModelField(rdsbcppHugeDouble,i1,x2,-1,&y))
          return rdsbcppHugeDouble;
        break;
      default:
        return rdsbcppHugeDouble;
    }
  return y;
}
//---------------------------------------------------------------------------



// ��� �������� ������ �� �����
#undef RDSLININTER_NONE
#undef RDSLININTER_BELOW
#undef RDSLININTER_ABOVE
#undef RDSLININTER_BETWEEN
#endif
