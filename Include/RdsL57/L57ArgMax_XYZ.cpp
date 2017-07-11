//---------------------------------------------------------------------------
// ����������� ����� ������ ����������� �������������
// ������� ���� ���������� f(x,y,z)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ���������� ��������
//---------------------------------------------------------------------------
#ifdef L57MS_GLOBALS
#ifndef L57_MODELVERSION
  #define L57_MODELVERSION 0
#endif
#include <RdsL57/L57FuncXYZ.cpp>

// ��� ������� ������������ ����������� �������
#define L57FUNC_LIST ListFunc

// ��� ����� �������������� �������
#ifndef L57_FUNCINPUT
  #define L57_FUNCINPUT "FuncId"
#endif

#ifndef L57_3DFUNCOUT_XY
  #define L57_3DFUNCOUT_XY "F0xy"
  #define L57_3DFUNCOUT_YZ "F0yz"
  #define L57_3DFUNCOUT_XZ "F0xz"
#endif

//-----------------------------------------------------------------------

// �������� ������� ���������� - ��������� (��� ��������� ����������)
double _SelectedFunc(int id,const TL57TextFuncArg &arguments);
//-----------------------------------------------------------------------

// ������� ���������� � ���������� ����������
double SelectedFunc(int id,const TL57TextFuncArg &arguments)
{ double f=0.0;
  RDSBCPP_TRY
    { f=_SelectedFunc(id,arguments); }
  RDSBCPP_CATCHALL
    { f=rdsbcppHugeDouble; }
  return f;
}
//-----------------------------------------------------------------------

// ������� �������� ������ ���������� � ���������
class TMaxCandidate
{ public:
    TMaxCandidate *Next;
    int x_index,y_index,z_index; // ������� �������� ����� � �������� �������
    double x_init,y_init,z_init; // ��������� �������� ����� (����� ����������� ��� �������)
    double x,y,z;	// ������� �������� ����������
    double f; // �������� �������
    double dx,dy,dz; // ������� ����
    int cycles_in_df; // ����� ����� ������ ������ �����������
    int steps_down; // ���������� ��������� ���������� �����

    /*
    static int Count; // ������� ���������
    static void CheckMsg(int fixed=0); // �������� �� ������ �����������
    */

    // ��������� ������� ���������� ����
    static void StepDown(double &step,int init_index,int arr_count,
                         int &dx0,int &dx1,double &aux_step,BOOL *p_reset=NULL);
    // ��������� ������� ���������� ���� ��� ��������� ������
    static void StepDownStage4(double &step,int arr_count,
                               int &dx0,int &dx1,double &aux_step);

    TMaxCandidate(void)
      {Next=NULL;x_index=y_index=z_index=-1;
       x=y=z=f=x_init=y_init=z_init=dx=dy=dz=rdsbcppHugeDouble;
       cycles_in_df=0; steps_down=0;
       //Count++;
      };
    ~TMaxCandidate(){/*Count--;*/};
};
/*
int TMaxCandidate::Count=0;
*/
// ������� ������ ����������� � ���������
class TMaxList
{ public:
    TMaxCandidate *First;

    // ������?
    inline BOOL IsEmpty(void){return First==NULL;};
    // ��������
    void Clear(void);
    // ��������
    TMaxCandidate *Add(void);
    // ����������� �������
    TMaxCandidate *RemoveTop(void);
    // �������� �������
    void AddExisting(TMaxCandidate *f);
    // �������� ����� �����
    void AddNew(double x,double y,double z,double f);
    // ����������
    int Count(void);
    // ����� ���� ����� (��� ���������)
    double SumDeltas(void);
    // ���������� ���������� � ������ �������
    void Exchange(TMaxList &list);

    TMaxList(void){First=NULL;};
    ~TMaxList(){Clear();};
};
//-----------------------------------------------------------------------

/*
// �������� �� ������ ����������� ������� � ����������
void TMaxCandidate::CheckMsg(int fixed)
{
  if(Count>fixed)
    { rdsStopCalc();
      rdsMessageBox("���������� ������ - ������ ������","������������",
        MB_ICONERROR|MB_OK);
      Count=fixed;
    }
}
//---------------------------------------------------------------------------
*/

// ��������� ������� ���������� ����
// step - ������� ��� (�����������)
// init_index - ������ ����������, � ������� ��� ��������
// arr_count - ����� ��������� � �������
// dx0,dx1 - ������ � ����� �������� (-2, 0, 2)
// aux_step - ����� step, �� ������ �������� ������ ������� ����
// p_reset - ��������� �� ����������, ������������ � FALSE ���� �� �� ������� ������������ ���������
void TMaxCandidate::StepDown(double &step,int init_index,int arr_count,
                             int &dx0,int &dx1,double &aux_step,BOOL *p_reset)
{
  if(step==rdsbcppHugeDouble || arr_count==1)  // ������ �� ������� ��� ����������
    { dx0=dx1=0;
      aux_step=0.0;
      return;
    }
  step/=2;
  aux_step=step;

  if(init_index==0) // ����� �� ���������
    { dx0=0;
      if(p_reset) *p_reset=FALSE;
    }
  else
    dx0=-2;
  if(init_index==(arr_count-1)) // ������ �� ���������
    { dx1=0;
      if(p_reset) *p_reset=FALSE;
    }
  else
    dx1=2;
}
//---------------------------------------------------------------------------

// ��������� ������� ���������� ���� ��� ��������� ������
// step - ������� ��� (�����������)
// arr_count - ����� ��������� � �������
// dx0,dx1 - ������ � ����� �������� (-2, 0, 2)
// aux_step - ����� step, �� ������ �������� ������ ������� ����
void TMaxCandidate::StepDownStage4(double &step,int arr_count,
                             int &dx0,int &dx1,double &aux_step)
{
  if(step==rdsbcppHugeDouble || arr_count==1)  // ������ �� ������� ��� ����������
    { dx0=dx1=0;
      aux_step=0.0;
      return;
    }
  step/=2;
  aux_step=step;

  dx0=-2;
  dx1=2;
}
//---------------------------------------------------------------------------

// �������� ������� ������ ����������
void TMaxList::Clear(void)
{ TMaxCandidate *f,*fn;

  f=First;
  while(f)
    { fn=f->Next;
      delete f;
      f=fn;
    }
  First=NULL;
}
//---------------------------------------------------------------------------

// �������� ��������� � ������� ������
TMaxCandidate *TMaxList::Add(void)
{ TMaxCandidate *f=new TMaxCandidate();
  f->Next=First;
  First=f;
  return f;
}
//---------------------------------------------------------------------------

// �������� ����� �����
void TMaxList::AddNew(double x,double y,double z,double f)
{ TMaxCandidate *ef=new TMaxCandidate();
  ef->Next=First;
  First=ef;
  ef->x=x;
  ef->y=y;
  ef->z=z;
  ef->f=f;
}
//---------------------------------------------------------------------------

// ����������� �������� ��������� �� �������� ������
TMaxCandidate *TMaxList::RemoveTop(void)
{ TMaxCandidate *f=First;
  if(!f)
    return f;
  First=f->Next;
  f->Next=NULL;
  return f;
}
//---------------------------------------------------------------------------

// �������� �������� ���������
void TMaxList::AddExisting(TMaxCandidate *f)
{ f->Next=First;
  First=f;
}
//---------------------------------------------------------------------------

// ���������� ����������
int TMaxList::Count(void)
{ int n=0;
  for(TMaxCandidate *f=First;f!=NULL;f=f->Next)
    n++;
  return n;
}
//---------------------------------------------------------------------------

// ����� ���� ����� (��� ���������)
double TMaxList::SumDeltas(void)
{ double sum=0;
  for(TMaxCandidate *f=First;f!=NULL;f=f->Next)
    { if(f->dx!=rdsbcppHugeDouble) sum+=f->dx;
      if(f->dy!=rdsbcppHugeDouble) sum+=f->dy;
      if(f->dz!=rdsbcppHugeDouble) sum+=f->dz;
    }
  return sum;
}
//---------------------------------------------------------------------------

// ���������� ���������� � ������ �������
void TMaxList::Exchange(TMaxList &list)
{ TMaxCandidate *f=First;
  First=list.First;
  list.First=f;
}
//---------------------------------------------------------------------------

// ��������� ������� ����� � ������������ �����
class TDoubleArray
{ public:
    double *Dbl;
    //int *Int;
    int Count,Size;

    // ��������
    void Clear(void); 
    // �������
    void Allocate(int n,BOOL setcount=FALSE);
    // �������� ����������
    void SetCount(int cnt);
    
    // ����������� �� � ������� ����� � ��������� �� ��� ������ ���������
    BOOL CanInsertValue(double v,double delta=1e-38);
  
    TDoubleArray(void){Dbl=NULL;/*Int=NULL;*/Count=Size=0;};
    ~TDoubleArray(){Clear();};
};
//---------------------------------------------------------------------------

// ��������
void TDoubleArray::Clear(void)
{ //if(Int) delete[] Int;
  if(Dbl) delete[] Dbl;
  //Int=NULL; 
  Dbl=NULL;
  Count=Size=0;
}
//---------------------------------------------------------------------------

// �������� ����������
void TDoubleArray::SetCount(int cnt)
{
  if(cnt>=0 && cnt<Size)
    Count=cnt;
}
//---------------------------------------------------------------------------

// �������
void TDoubleArray::Allocate(int n,BOOL setcount)
{ Clear();
  if(n>0)  
    { //Int=new int[Size=n];
      Dbl=new double[Size=n];
      if(setcount)
        Count=Size;
    }
}
//---------------------------------------------------------------------------

// ����� �������������� ��������
// array        - ������
// count        - ����� �����
// itemshift    - ���������� ����� ��������� ������� � �������
// pLeft,pRight - ������������ ������� �������������� �����
// �������      - LININTER_*
#define LININTER_NONE     0       // ��� ������
#define LININTER_BELOW    1       // x ����� �������
#define LININTER_ABOVE    2       // x ������ �������
#define LININTER_BETWEEN  3       // x ������ �������
int FindNearestPoints(double *array,int count,int itemshift,double x,int *pLeft,int *pRight)
{ int left=0,right=count-1,mid,diap;
  double xleft,xright,xmid;

  if(count==1)
    { *pLeft=0;
      *pRight=0;
      return LININTER_NONE;
    }

  xleft=array[0];
  xright=array[(count-1)*itemshift];
  if(xleft==rdsbcppHugeDouble || xright==rdsbcppHugeDouble)
    { *pLeft=0;
      *pRight=0;
      return LININTER_NONE;
    }
  if(x<xleft)
    { *pLeft=0;
      *pRight=1;
      return LININTER_BELOW;
    }
  if(x>xright)
    { *pLeft=count-2;
      *pRight=count-1;
      return LININTER_ABOVE;
    }

  for(;;)
    switch(diap=right-left)
      { case 0:
        case 1:
          *pLeft=left;
          *pRight=right;
          return diap==0?LININTER_NONE:LININTER_BETWEEN;

        default: // ���� � ����� ����� ����� left � right
          mid=left+diap/2;
          xmid=array[mid*itemshift];
          if(xmid==rdsbcppHugeDouble)
            { *pLeft=0;
              *pRight=0;
              return LININTER_NONE;
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


// ����������� �� � ������� ����� � ��������� �� ��� ������ ���������
BOOL TDoubleArray::CanInsertValue(double v,double delta)
{ int left,right;
  if(Count<2)
    return FALSE;
  if(FindNearestPoints(Dbl,Count,1,v,&left,&right)!=LININTER_BETWEEN)
    return FALSE;
  if(fabs(v-Dbl[left])<=delta)
    return FALSE;
  if(fabs(v-Dbl[right])<=delta)
    return FALSE;
  return TRUE;
}
//---------------------------------------------------------------------------


// ����������� ����� ����� �� ��������� � ����
int CalcNPointsByDiap(double vmin,double vmax,double dv)
{ int n;
  if(vmin==rdsbcppHugeDouble)
    return -1;
  // �.�. ���� �������� ������� �� ����� �����?
  if(vmax==rdsbcppHugeDouble || dv==rdsbcppHugeDouble || dv<=0.0 || vmax<vmin+0.5*dv)
    return 1;
  RDSBCPP_TRY
    { double d=(vmax-vmin)/dv;
      n=1+(int)(d);
      if(n<2)
        n=-1;
    }
  RDSBCPP_CATCHALL
    { n=-1; }
  if(n>0)
    { // ��������� ��������� �����
      double last=vmin+(n-1)*dv;
      if(vmax-last>0.1*dv)
        n++;
    }
  return n;
}
//-----------------------------------------------------------------------

// �������� �� ��������� �� ������
inline double vInDiap(double vmin,double vmax,double dv,int i,int cnt)
{ return (i==0) ? vmin : ( (i==(cnt-1))?vmax:(vmin+i*dv) ); }
//-----------------------------------------------------------------------

// ����������� ������� ��� ��������� (� ��������� ������ �������)
BOOL SortMatrixField(BOOL databyrows,int Rows,int Cols,
                     double *MData,int XIndex,int excludeindex)
{


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

// ����������� ������� ��� ������� ����� ����������
BOOL SortMatrix(BOOL databyrows,int Rows,int Cols,double *MData,int XIndex)
{

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
#endif // #ifdef L57MS_GLOBALS
//-----------------------------------------------------------------------





//---------------------------------------------------------------------------
// ���������
//---------------------------------------------------------------------------
#ifdef L57MS_SETUP
TL57TextFile file;
TL57TextString str;
RDS_HOBJECT win;
BOOL res;
char caption[]="������������";

RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

if(model!=NULL && rdscompIsEditorOpen(model->Model))
  { rdsMessageBox("��� ��������� ����� ����� ���������� ������� �������� ��� ������. "
                  "����������, �������� ��������.",caption,MB_OK|MB_ICONWARNING);
    rdscompOpenBlockModelEditor(rdsbcppBlockData->Block);
    return;
  }

win=rdsFORMCreate(TRUE,-1,-1,caption);
if(!win)
  return;

CheckConnection();


// ������ ������ ������� �� �����
file.CanEditFuncId=L57_FUNCIDEDIT;
file.Setup_ReadFuncFile(rdsbcppBlockData->Block,model!=NULL,L57FUNC_LIST,win);

// ������� ������ �������
rdsFORMAddTab(win,1,"�������");
file.Setup_AddFuncSelectFields(win,NULL,1,1,SetupFuncId,model!=NULL,L57TF_ALLBUTTONS,NULL,
#if L57_MODELVERSION>=1
  5*24
#else
  4*24
#endif
);

// ������� ��������
rdsFORMAddTab(win,3,"�������");
SetupVar(win,3,100,"x",setup_xmin,setup_xmax,setup_dx,
         in_xdiap,in_dx,TRUE,TRUE);
SetupVar(win,3,200,"y",setup_ymin,setup_ymax,setup_dy,
         in_ydiap,in_dy,FALSE,TRUE);
SetupVar(win,3,300,"z",setup_zmin,setup_zmax,setup_dz,
         in_zdiap,in_dz,FALSE,FALSE);

// ������� ����� ����������
rdsFORMAddTab(win,6,"���������");

rdsFORMAddEdit(win,6,1008,RDS_FORMCTRL_EDIT,"����������� ������������ ����������:",80);
rdsSetObjectDouble(win,1008,RDS_FORMVAL_VALUE,setup_df_equal);

rdsFORMAddEdit(win,6,1000,RDS_FORMCTRL_CHECKBOX,"�������� ���������",80);
rdsSetObjectInt(win,1000,RDS_FORMVAL_VALUE,deep_search?1:0);

#if L57_MODELVERSION>=1
  rdsFORMAddEdit(win,6,1009,RDS_FORMCTRL_CHECKBOX,"������� ������ ����� �� \"�����\"",80);
  rdsSetObjectInt(win,1009,RDS_FORMVAL_VALUE,setup_no_plateau?1:0);
#endif

rdsFORMAddEdit(win,6,1003,RDS_FORMCTRL_CHECKBOX,"�������� �� �������� �����",80);
rdsSetObjectInt(win,1003,RDS_FORMVAL_VALUE,setup_end_steps?1:0);

rdsFORMAddEdit(win,6,1004,RDS_FORMCTRL_EDIT,"�������� ��� �� X:",80);
rdsSetObjectDouble(win,1004,RDS_FORMVAL_VALUE,setup_dx_min);

rdsFORMAddEdit(win,6,1005,RDS_FORMCTRL_EDIT,"�������� ��� �� Y:",80);
rdsSetObjectDouble(win,1005,RDS_FORMVAL_VALUE,setup_dy_min);

rdsFORMAddEdit(win,6,1006,RDS_FORMCTRL_EDIT,"�������� ��� �� Z:",80);
rdsSetObjectDouble(win,1006,RDS_FORMVAL_VALUE,setup_dz_min);

rdsFORMAddEdit(win,6,1001,RDS_FORMCTRL_CHECKBOX,"�������� �� ����������� �������",80);
rdsSetObjectInt(win,1001,RDS_FORMVAL_VALUE,search_by_df?1:0);

rdsFORMAddEdit(win,6,1002,RDS_FORMCTRL_EDIT,"����������� �������� �������:",80);
rdsSetObjectDouble(win,1002,RDS_FORMVAL_VALUE,setup_df);

rdsFORMAddEdit(win,6,1007,RDS_FORMCTRL_UPDOWN,"�������� � ������ �����������:",80);
rdsSetObjectInt(win,1007,RDS_FORMVAL_VALUE,setup_df_cycles);
rdsSetObjectInt(win,1007,RDS_FORMVAL_UPDOWNMIN,0);
rdsSetObjectInt(win,1007,RDS_FORMVAL_UPDOWNMAX,30000);
rdsSetObjectInt(win,1007,RDS_FORMVAL_UPDOWNINC,1);


#if L57_MODELVERSION>=2
  // ������� ��������� ������
  rdsFORMAddTab(win,8,"�������");

  rdsFORMAddEdit(win,8,2000,RDS_FORMCTRL_CHECKBOX,"�������� ������� XY: (x,y)*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2000,RDS_FORMVAL_VALUE,setup_ds_XY?1:0);

  rdsFORMAddEdit(win,8,2001,RDS_FORMCTRL_CHECKBOX,"�������� ������� XZ: (x,z)*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2001,RDS_FORMVAL_VALUE,setup_ds_XZ?1:0);

  rdsFORMAddEdit(win,8,2002,RDS_FORMCTRL_CHECKBOX,"�������� ������� YZ: (y,z)*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2002,RDS_FORMVAL_VALUE,setup_ds_YZ?1:0);

  rdsFORMAddEdit(win,8,2003,RDS_FORMCTRL_CHECKBOX,"�������� ������� X � Fx: x*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2003,RDS_FORMVAL_VALUE,setup_ds_X?1:0);

  rdsFORMAddEdit(win,8,2004,RDS_FORMCTRL_CHECKBOX,"�������� ������� Y � Fy: y*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2004,RDS_FORMVAL_VALUE,setup_ds_Y?1:0);

  rdsFORMAddEdit(win,8,2005,RDS_FORMCTRL_CHECKBOX,"�������� ������� Z � Fz: z*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2005,RDS_FORMVAL_VALUE,setup_ds_Z?1:0);

  rdsFORMAddEdit(win,8,2100,RDS_FORMCTRL_EDIT,"�������� ��� �� X:",80);
  rdsSetObjectDouble(win,2100,RDS_FORMVAL_VALUE,setup_ds_edx);

  rdsFORMAddEdit(win,8,2101,RDS_FORMCTRL_EDIT,"�������� ��� �� Y:",80);
  rdsSetObjectDouble(win,2101,RDS_FORMVAL_VALUE,setup_ds_edy);

  rdsFORMAddEdit(win,8,2102,RDS_FORMCTRL_EDIT,"�������� ��� �� Z:",80);
  rdsSetObjectDouble(win,2102,RDS_FORMVAL_VALUE,setup_ds_edz);
#endif

#if L57_MODELVERSION>=7
  // ������� ������ �����������
  rdsFORMAddTab(win,9,"�����������");

  rdsFORMAddEdit(win,9,3002,RDS_FORMCTRL_MULTILABEL|RDS_FORMFLAG_LINE,
                     "������ ���� ���������� �� ������� \"" L57_3DFUNCOUT_XY "\", \"" L57_3DFUNCOUT_YZ "\" � \""
                     L57_3DFUNCOUT_XZ "\":",0);
  rdsSetObjectInt(win,3002,RDS_FORMVAL_MLHEIGHT,20);
  rdsSetObjectInt(win,3002,RDS_FORMVAL_MLABELBEVEL,0);

  rdsFORMAddEdit(win,9,3000,RDS_FORMCTRL_CHECKBOX,"�������� ����� ������� ������� ������� ���������",80);
  rdsSetObjectInt(win,3000,RDS_FORMVAL_VALUE,setup_surf_everyrow?1:0);

  rdsFORMAddEdit(win,9,3001,RDS_FORMCTRL_EDIT|RDS_FORMFLAG_CHECK,"����������� �������� ������, ���.:",80);
  rdsSetObjectDouble(win,3001,RDS_FORMVAL_VALUE,setup_surf_delayval);
  rdsSetObjectInt(win,3001,RDS_FORMVAL_CHECK,setup_surf_bydelay?1:0);
#endif


// ��������� ����
res=rdsFORMShowModalServ(win,WinCallback);

if(res)
  { // ���� Num �������
    SetupFuncId=file.FuncNumByIndex(rdsGetObjectInt(win,1,RDS_FORMVAL_VALUE),SetupFuncId);

    // ��������� ����, ���� �� ���������
    if(model!=NULL)
      file.Setup_WriteFuncFile(rdsbcppBlockData->Block,L57_BAK);

     // ������� X
     ReadSetupVar(win,100,setup_xmin,setup_xmax,setup_dx,
                  in_xdiap,in_dx);
     // ������� Y
     ReadSetupVar(win,200,setup_ymin,setup_ymax,setup_dy,
                  in_ydiap,in_dy);
     // ������� Z
     ReadSetupVar(win,300,setup_zmin,setup_zmax,setup_dz,
                  in_zdiap,in_dz);

     // ������� ����� ����������
     deep_search=(rdsGetObjectInt(win,1000,RDS_FORMVAL_VALUE)!=0);
     setup_df=rdsGetObjectDouble(win,1002,RDS_FORMVAL_VALUE);

     search_by_df=(rdsGetObjectInt(win,1001,RDS_FORMVAL_VALUE)!=0);
     setup_end_steps=(rdsGetObjectInt(win,1003,RDS_FORMVAL_VALUE)!=0);
     setup_dx_min=rdsGetObjectDouble(win,1004,RDS_FORMVAL_VALUE);
     setup_dy_min=rdsGetObjectDouble(win,1005,RDS_FORMVAL_VALUE);
     setup_dz_min=rdsGetObjectDouble(win,1006,RDS_FORMVAL_VALUE);
     setup_df_cycles=rdsGetObjectInt(win,1007,RDS_FORMVAL_VALUE);
     setup_df_equal=rdsGetObjectDouble(win,1008,RDS_FORMVAL_VALUE);
     #if L57_MODELVERSION>=1
       setup_no_plateau=(rdsGetObjectInt(win,1009,RDS_FORMVAL_VALUE)!=0);
     #endif

    #if L57_MODELVERSION>=2
      setup_ds_XY=(rdsGetObjectInt(win,2000,RDS_FORMVAL_VALUE)!=0);
      setup_ds_XZ=(rdsGetObjectInt(win,2001,RDS_FORMVAL_VALUE)!=0);
      setup_ds_YZ=(rdsGetObjectInt(win,2002,RDS_FORMVAL_VALUE)!=0);
      setup_ds_X=(rdsGetObjectInt(win,2003,RDS_FORMVAL_VALUE)!=0);
      setup_ds_Y=(rdsGetObjectInt(win,2004,RDS_FORMVAL_VALUE)!=0);
      setup_ds_Z=(rdsGetObjectInt(win,2005,RDS_FORMVAL_VALUE)!=0);
      setup_ds_edx=rdsGetObjectDouble(win,2100,RDS_FORMVAL_VALUE);
      setup_ds_edy=rdsGetObjectDouble(win,2101,RDS_FORMVAL_VALUE);
      setup_ds_edz=rdsGetObjectDouble(win,2102,RDS_FORMVAL_VALUE);
    #endif

    #if L57_MODELVERSION>=7
      setup_surf_everyrow=(rdsGetObjectInt(win,3000,RDS_FORMVAL_VALUE)!=0);
      setup_surf_delayval=rdsGetObjectDouble(win,3001,RDS_FORMVAL_VALUE);
      setup_surf_bydelay=(rdsGetObjectInt(win,3001,RDS_FORMVAL_CHECK)!=0);
    #endif

     // ������� ������� ��� ��������� �����
     xdiap_chg=ydiap_chg=zdiap_chg=1;
     Start=1;
 }

rdsDeleteObject(win);
Result=res?1:0;

#endif // #ifdef L57MS_SETUP
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// �������� � ������
//---------------------------------------------------------------------------
#ifdef L57MS_CLASS
TL57TextFuncArg Arguments;
BOOL InputConn;
TMaxList LocalMax; // ��������� � ���������
TMaxList NextMax; // ��� ��������� �� ����� - �������������� �� ��������� ����
TMaxList FinalMax; // ������� ���������� ���������
TDoubleArray AddXArg,AddYArg,AddZArg; // ���������� ��������� ���������� ����������
int AddXBase,AddYBase,AddZBase; // ������� ������� ����������� ���������� � ��������
int RealFuncId; // ������������� ������� (�� �������� ��� � �����)

double Dx,Dy,Dz; // �������� ���� �������� �����
double _Xmin,_Xmax,_Ymin,_Ymax,_Zmin,_Zmax; // �������� ������� ����������
int xy_cnt; // x_cnt*y_cnt

#if L57_MODELVERSION>=5
  // ����� � ������� ���������� �������
  BOOL F0xyConn,F0yzConn,F0xzConn;
  // ������� �����-�� ������ � �������� ���������� �������
  BOOL F0xyData,F0yzData,F0xzData;

  // ��������� � ���������� �������� ������������
  BOOL SetupSurfaces(void);

  BOOL SurfacesArraySet;        // ������� ������������ ��������
  DWORD LastSurfaceOut_xy,      // ����� ���������� ���������� �������
        LastSurfaceOut_yz,
        LastSurfaceOut_xz;
  DWORD SurfaceOutDelayMs;      // �������� � ��
#endif

#if L57_MODELVERSION>=2
  // ��������� ������ 4
  int SubStage4;
  #define L57SS4_NONE     -1      // ���
  #define L57SS4_XY       0       // ��������� (x,y)* - ������� z
  #define L57SS4_XZ       1       // ��������� (x,z)* - ������� y
  #define L57SS4_YZ       2       // ��������� (y,z)* - ������� x
  #define L57SS4_X        3       // ��������� x* - ������� (y,z)
  #define L57SS4_Y        4       // ��������� y* - ������� (x,z)
  #define L57SS4_Z        5       // ��������� z* - ������� (x,y)
  TMaxCandidate Stage4Max; // ���������� �������� �� ������ 4
  void Stage4_StorePointToMatr(void); // ������ ���������� ����� � ������� �� Stage4Max
  BOOL Stage4_NextPoint(void); // ����� � Stage4Max ��������� �����
  void Stage4_StepDownAndInspect(void); // ��������� ��� � Stage4Max � ��������� �����
  // �������� �������� ����������
  BOOL Stage4_FixX,Stage4_FixY,Stage4_FixZ;
#endif

// �������� �� ������ �������
double stage0_progress,stage0_progmax;
double stage1_progress,stage1_progmax;
double stage2_progress,stage2_progmax;
double stage3_progress,stage3_progmax;
#if L57_MODELVERSION>=2
  double stage4_progress,stage4_progmax;
#endif

#if L57_MODELVERSION>=9
  // �����
  DWORD dwStartTime;
  // ��������� �����
  void ShowTime(void)
    {
      if(Working!=0)
        Time=((double)(GetTickCount()-dwStartTime))/1000.0;
    };
#endif


// ������������� ���������� ���������
inline void InitProgress(void)
{ stage0_progress=stage1_progress=stage2_progress=stage3_progress=0;
  stage0_progmax=stage1_progmax=stage2_progmax=stage3_progmax=1.0;

#if L57_MODELVERSION>=2
  stage4_progress=0.0;
  stage4_progmax=1.0;
#endif
}
// �������� ������� ����� � ����� �������������� �������
void CheckConnection(void)
{ InputConn=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_FUNCINPUT);
  #if L57_MODELVERSION>=5
    F0xyConn=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_3DFUNCOUT_XY);
    F0yzConn=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_3DFUNCOUT_YZ);
    F0xzConn=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_3DFUNCOUT_XZ);
  #endif
}
//-----------------------------------------------------------------------

// ������������ ����� ���������
void ProgressOut(void);
//---------------------------------------------------------------------------

// �������� �������� ������ X,Y,Z (���������)
inline double & XItem(int y_index,int z_index)
  { return X[y_index+1][z_index+1]; };
inline double & YItem(int x_index,int z_index)
  { return Y[x_index+1][z_index+1]; };
inline double & ZItem(int x_index,int y_index)
  { return Z[x_index+1][y_index+1]; };
// �������� ��������� ������ FX,FY,FZ (���������)
inline double & FxItem(int y_index,int z_index)
  { return Fx[y_index+1][z_index+1]; };
inline double & FyItem(int x_index,int z_index)
  { return Fy[x_index+1][z_index+1]; };
inline double & FzItem(int x_index,int y_index)
  { return Fz[x_index+1][y_index+1]; };
//---------------------------------------------------------------------------

// �������� ���������� �� ������
inline double &GetX(int ix)
  { return Y[ix+1][0]; }
inline double &GetY(int iy)
  { return X[iy+1][0]; }
inline double &GetZ(int iz)
  { return X[0][iz+1]; }
//---------------------------------------------------------------------------

// �������� ������� �� ��������������� ������� �� ������
inline double &FuncVal(int ix,int iy,int iz)
{ //return *(FVal.GetBuffer()+iz*xy_cnt+iy*x_cnt+ix); 
  return FVal[iz*xy_cnt+iy*x_cnt+ix];
}
//---------------------------------------------------------------------------

// ������ 1 - ����� ���������� � ���������
void DoStage1(BOOL init);
// ������ 2 - ��������� ����������
void DoStage2(BOOL init);
// ��� ������ 2 - ����������� �������� � �������
void StoreFinalMax(TMaxCandidate *fnew);
// �������� ����� � ������� ��������� (��� ����������� ���������(
void StoreFinalMaxSimple(double x,double y,double z,double f);
// ������ 3 - �������� ������
void DoStage3(BOOL init);
// ��� ������ 3 - ������� � ������� ����� ����� � �������� �� FinalMax
void InsertNewArgs(void);
#if L57_MODELVERSION>=2
  // ������ 4 - ��������� ������
  void DoStage4(BOOL init);
#endif
//---------------------------------------------------------------------------

// ������� ��������� ������
void ClearTempData(void)
{ FVal.Resize(0);
  LocalMax.Clear();
  NextMax.Clear();
  FinalMax.Clear();
  AddXArg.Clear();
  AddYArg.Clear();
  AddZArg.Clear();
}
//---------------------------------------------------------------------------

// ��������� �������� �������� ����� ������
void SetupOutputs(BOOL _err=FALSE);
// ������� ������ ���������� � �������� ������� AllMax
void FillAllMax(TMaxList &list);
// ����������� ������� ������� (������� ����� ����������)
BOOL SortLineMatrix(rdsbcppVarAncestor &M);
// ����������� ������� ������� ���� ����������
BOOL SortFieldMatrix(rdsbcppVarAncestor &M);
// ����� ��������� �����
BOOL GetNextPoint(void);
// ���������� �������� ������� �� xi,yi,zi
void ProcessValues_xiyizi(double *pf=NULL);

#endif // #ifdef L57MS_CLASS
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// ����� �������������
//---------------------------------------------------------------------------
#ifdef L57MS_CALCMODE
CheckConnection();
#endif // #ifdef L57MS_CALCMODE
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// �������� ����� ������
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS
// ������� ��������� ������ ���� ���������
void RDSCALL WinCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data)
{ TL57TextFile *file=(TL57TextFile*)rdsGetObjectPtr(win,0,NULL);
  int deep=rdsGetObjectInt(win,1000,RDS_FORMVAL_VALUE);
  int steps=deep && rdsGetObjectInt(win,1003,RDS_FORMVAL_VALUE),
      prec=deep && rdsGetObjectInt(win,1001,RDS_FORMVAL_VALUE);
  if(file)
    file->Setup_WindowCallback(win,data);
  rdsSetObjectInt(win,1001,RDS_FORMVAL_ENABLED,deep);
  rdsSetObjectInt(win,1003,RDS_FORMVAL_ENABLED,deep);
  //rdsSetObjectInt(win,1008,RDS_FORMVAL_ENABLED,deep);
  rdsSetObjectInt(win,1004,RDS_FORMVAL_ENABLED,steps);
  rdsSetObjectInt(win,1005,RDS_FORMVAL_ENABLED,steps);
  rdsSetObjectInt(win,1006,RDS_FORMVAL_ENABLED,steps);
  rdsSetObjectInt(win,1007,RDS_FORMVAL_ENABLED,prec);
  rdsSetObjectInt(win,1002,RDS_FORMVAL_ENABLED,prec);
  #if L57_MODELVERSION>=1
    rdsSetObjectInt(win,1009,RDS_FORMVAL_ENABLED,deep);
  #endif

  /*
  rdsFORMAddEdit(win,8,2000,RDS_FORMCTRL_CHECKBOX,"�������� ������� XY: (x,y)*=arg max f(x,y,z) ",80);
  rdsFORMAddEdit(win,8,2001,RDS_FORMCTRL_CHECKBOX,"�������� ������� XZ: (x,z)*=arg max f(x,y,z) ",80);
  rdsFORMAddEdit(win,8,2002,RDS_FORMCTRL_CHECKBOX,"�������� ������� YZ: (y,z)*=arg max f(x,y,z) ",80);
  rdsFORMAddEdit(win,8,2003,RDS_FORMCTRL_CHECKBOX,"�������� ������� X � Fx: x*=arg max f(x,y,z) ",80);
  rdsFORMAddEdit(win,8,2004,RDS_FORMCTRL_CHECKBOX,"�������� ������� Y � Fy: y*=arg max f(x,y,z) ",80);
  rdsFORMAddEdit(win,8,2005,RDS_FORMCTRL_CHECKBOX,"�������� ������� Z � Fz: z*=arg max f(x,y,z) ",80);
  */

#if L57_MODELVERSION>=2
  // ������� ��������� ������
  rdsSetObjectInt(win,2100,RDS_FORMVAL_ENABLED,         // ��� x
    rdsGetObjectInt(win,2000,RDS_FORMVAL_VALUE)||       // xy=F(z)
    rdsGetObjectInt(win,2001,RDS_FORMVAL_VALUE)||       // xz=F(y)
    rdsGetObjectInt(win,2003,RDS_FORMVAL_VALUE)         // x=F(y,z)
    );

  rdsSetObjectInt(win,2101,RDS_FORMVAL_ENABLED,         // ��� y
    rdsGetObjectInt(win,2000,RDS_FORMVAL_VALUE)||       // xy=F(z)
    rdsGetObjectInt(win,2002,RDS_FORMVAL_VALUE)||       // yz=F(x)
    rdsGetObjectInt(win,2004,RDS_FORMVAL_VALUE)         // y=F(x,z)
    );

  rdsSetObjectInt(win,2102,RDS_FORMVAL_ENABLED,         // ��� z
    rdsGetObjectInt(win,2001,RDS_FORMVAL_VALUE)||       // xz=F(y)
    rdsGetObjectInt(win,2002,RDS_FORMVAL_VALUE)||       // yz=F(x)
    rdsGetObjectInt(win,2005,RDS_FORMVAL_VALUE)         // z=F(x,y)
    );
#endif

}
//---------------------------------------------------

// ��������� ���� ����� ��� ��������� � �������� ����������
void SetupVar(RDS_HOBJECT win,int tab,int basefield,char *var,
               double vmin,double vmax,double dv,
               BOOL in_vdiap,BOOL in_dv,BOOL head,BOOL line)
{ int id=basefield;
  TL57TextString str;

  if(head)
    { rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_MULTILABEL|RDS_FORMFLAG_LINE,
                     "��������� ��������� ����� ������������� � "
                     "���������� �����, � �� ������ � ��� ������:",0);
      rdsSetObjectInt(win,id,RDS_FORMVAL_MLHEIGHT,34);
      rdsSetObjectInt(win,id,RDS_FORMVAL_MLABELBEVEL,0);
    }

  id++;
  str.Assign("����������� �������� ");
  str.AddString(var);
  str.AddString("min...");
  str.AddString(var);
  str.AddString("max:");
  rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_RANGEEDIT|RDS_FORMFLAG_CHECK,
                 str.c_str(),160);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_VALUE,vmin);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_RANGEMAX,vmax);
  rdsSetObjectInt(win,id,RDS_FORMVAL_CHECK,!in_vdiap);

  id++;
  str.Assign("����������� ��� d");
  str.AddString(var);
  str.AddString(":");
  rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_EDIT|RDS_FORMFLAG_CHECK|(line?RDS_FORMFLAG_LINE:0),
                 str.c_str(),80);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_VALUE,dv);
  rdsSetObjectInt(win,id,RDS_FORMVAL_CHECK,!in_dv);
}
//---------------------------------------------------

// ������� ���� ����� ��� ��������� � �������� ����������
void ReadSetupVar(RDS_HOBJECT win,int basefield,
                  double &vmin,double &vmax,double &dv,
                  BOOL &in_vdiap,BOOL &in_dv)
{ int id=basefield+1;

  vmin=rdsGetObjectDouble(win,id,RDS_FORMVAL_VALUE);
  vmax=rdsGetObjectDouble(win,id,RDS_FORMVAL_RANGEMAX);
  in_vdiap=(rdsGetObjectInt(win,id,RDS_FORMVAL_CHECK)==0);

  id++;
  dv=rdsGetObjectDouble(win,id,RDS_FORMVAL_VALUE);
  in_dv=(rdsGetObjectInt(win,id,RDS_FORMVAL_CHECK)==0);
}
//---------------------------------------------------

#if L57_MODELVERSION>=5
// ��������� � ���������� �������� ������������
BOOL rdsbcppBlockClass::SetupSurfaces(void)
{ BOOL rok=TRUE;

  SurfacesArraySet=TRUE;

  if(F0xyConn)
    rok=rok && F0xy.Resize(x_cnt+1,y_cnt+1);
  else
    F0xy.Resize(0,0);
  if(F0xzConn && rok)
    rok=rok && F0xz.Resize(x_cnt+1,z_cnt+1);
  else
    F0xz.Resize(0,0);
  if(F0yzConn && rok)
    rok=rok && F0yz.Resize(y_cnt+1,z_cnt+1);
  else
    F0yz.Resize(0,0);

  if(!rok)
    return FALSE;

  for(int i=0;i<F0xy.Size();i++)
    *(F0xy.GetBuffer()+i)=rdsbcppHugeDouble;
  for(int i=0;i<F0xz.Size();i++)
    *(F0xz.GetBuffer()+i)=rdsbcppHugeDouble;
  for(int i=0;i<F0yz.Size();i++)
    *(F0yz.GetBuffer()+i)=rdsbcppHugeDouble;

  for(int xi=0;xi<x_cnt;xi++)
    { double v=vInDiap(_Xmin,_Xmax,Dx,xi,x_cnt); //(xi==(x_cnt-1)?_Xmax:(_Xmin+xi*Dx));
      if(F0xyConn)
        F0xy[xi+1][0]=v;
      if(F0xzConn)
        F0xz[xi+1][0]=v;
    }

  for(int yi=0;yi<y_cnt;yi++)
    { double v=vInDiap(_Ymin,_Ymax,Dy,yi,y_cnt); //(yi==(y_cnt-1)?_Ymax:(_Ymin+yi*Dy));
      if(F0xyConn)
        F0xy[0][yi+1]=v;
      if(F0yzConn)
        F0yz[yi+1][0]=v;
    }

  for(int zi=0;zi<z_cnt;zi++)
    { double v=vInDiap(_Zmin,_Zmax,Dz,zi,z_cnt); //(zi==(z_cnt-1)?_Zmax:(_Zmin+zi*Dz));
      if(F0xzConn)
        F0xz[0][zi+1]=v;
      if(F0yzConn)
        F0yz[0][zi+1]=v;
    }

  return TRUE;
}
//---------------------------------------------------
#endif

// ��������� �������� �������� ����� ������
void rdsbcppBlockClass::SetupOutputs(BOOL _err)
{ double delta;
  BOOL fchg=FALSE;
  Error=_err?1:0;
  Complete=OutReady=Finished=1;
  #if L57_MODELVERSION>=9
    ShowTime();
  #endif
  Working=0;
  Progress=1.0;
  stage=0;
  #if L57_MODELVERSION>=5
    if(!F0xyData)
      F0xy.Resize(0,0);
    if(!F0yzData)
      F0yz.Resize(0,0);
    if(!F0xzData)
      F0xz.Resize(0,0);
    F0xy_ready=1;
    F0xz_ready=1;
    F0yz_ready=1;
  #endif
  // ������� ������� ��������� ����������
  FillAllMax(FinalMax);
  ClearTempData();
  // ������������� ������
  delta=Dx*1.1;
  if(Xmax_f==rdsbcppHugeDouble || fabs(Xmax_f-Xmax)>delta)
    { Xmax_f=Xmax; fchg=TRUE; }
  delta=Dy*1.1;
  if(Ymax_f==rdsbcppHugeDouble || fabs(Ymax_f-Ymax)>delta)
    { Ymax_f=Ymax; fchg=TRUE; }
  delta=Dz*1.1;
  if(Zmax_f==rdsbcppHugeDouble || fabs(Zmax_f-Zmax)>delta)
    { Zmax_f=Zmax; fchg=TRUE; }
  if(fchg)
    Fmax_f=Fmax;
}
//---------------------------------------------------------------------------

// ������� ������ ���������� � �������� ������� AllMax
void rdsbcppBlockClass::FillAllMax(TMaxList &list)
{ int n=list.Count();
  if(!n)
    { AllMax.Resize(1,4);
      AllMax[0][0]=Xmax;
      AllMax[0][1]=Ymax;
      AllMax[0][2]=Zmax;
      AllMax[0][3]=Fmax;
      return;
    }
  if(!AllMax.Resize(n,4))
    { AllMax.Resize(0,0);
      rdsStopCalc();
      rdsMessageBox("������� ������� ����� ���������� - �� ������� ������ ��� ������� �����������",
        rdsbcppBlockData->BlockName,MB_ICONERROR|MB_OK);
      return;
    }
  n=0;
  for(TMaxCandidate *f=list.First;f!=NULL;f=f->Next,n++)
    { AllMax[n][0]=f->x;
      AllMax[n][1]=f->y;
      AllMax[n][2]=f->z;
      AllMax[n][3]=f->f;
      if(f->f>Fmax)
        { Fmax=f->f;
          Xmax=f->x;
          Ymax=f->y;
          Zmax=f->z;
        }
    }
}
//---------------------------------------------------------------------------

// ������������ ����� ���������
void rdsbcppBlockClass::ProgressOut(void)
{ double p;
  int n;
  if(deep_search)
    { p=stage0_progress/stage0_progmax+ // �������� �����
        stage1_progress/stage1_progmax+ // ����� ����������
        stage2_progress/stage2_progmax+ // ��������� ����������
        stage3_progress/stage3_progmax; // �������� �����
      n=4;
    }
  else
   { p=stage0_progress/stage0_progmax;
     n=1;
   }
  #if L57_MODELVERSION>=2
    if(setup_ds_X || setup_ds_Y || setup_ds_Z ||
       setup_ds_XY || setup_ds_XZ || setup_ds_YZ)
      { p+=stage4_progress/stage4_progmax; // ��������� ������
        n++;
      }
  #endif
  Progress=p/n;
}
//---------------------------------------------------------------------------


// ������ 1 - ����� ���������� � ���������
void rdsbcppBlockClass::DoStage1(BOOL init)
{ double f;
  BOOL _finish=FALSE;
  if(init)
    { xi=yi=zi=0;
      return;
    }
  // ��������

  // ��������� ����� xi,yi,zi �� ��������� ��������
  f=FuncVal(xi,yi,zi);
  if(f!=rdsbcppHugeDouble)
    { BOOL low=TRUE;
      // ����������� �������
      for(int idx=-1;low && idx<=1;idx++)
        { int i_x=xi+idx;
          if(i_x<0 || i_x>=x_cnt)
            continue;
          for(int idy=-1;low && idy<=1;idy++)
            { int i_y=yi+idy;
              if(i_y<0 || i_y>=y_cnt)
                continue;
              for(int idz=-1;low && idz<=1;idz++)
                { double fn;
                  int i_z=zi+idz;
                  if(i_z<0 || i_z>=z_cnt)
                    continue;
                  if(idx==0 && idy==0 && idz==0)
                    continue;
                  fn=FuncVal(i_x,i_y,i_z);
                  if(fn==rdsbcppHugeDouble)
                    continue;
                  if(fn>f) // �� ��� ������ ����
                    low=FALSE;
                } // for(int idz=-1;...)
            } // for(int idy=-1;...)
        } // for(int idx=-1;...)
      if(low)
        { // ��� ������ ����� xi,yi,zi ��������� ����
          TMaxCandidate *c=LocalMax.Add();
          c->x_index=xi;
          c->y_index=yi;
          c->z_index=zi;
          c->f=f;
          c->x=c->x_init=GetX(xi);
          c->y=c->y_init=GetY(yi);
          c->z=c->z_init=GetZ(zi);
          c->dx=(x_cnt==1?rdsbcppHugeDouble:Dx);
          c->dy=(y_cnt==1?rdsbcppHugeDouble:Dy);
          c->dz=(z_cnt==1?rdsbcppHugeDouble:Dz);
        }
    } // if(f!=rdsbcppHugeDouble)

  // ����� ��������� �����
  zi+=1;
  stage1_progress+=1.0;
  if(zi>=z_cnt)
    { zi=0;
      yi+=1;
      if(yi>=y_cnt)
        { yi=0;
          xi+=1;
          if(xi>=x_cnt)
            _finish=TRUE;
        }
    }

  Ready=1;
  if(_finish) // ���������
    { stage1_progress=stage1_progmax;
      ProgressOut();
      stage=2;
      Start=1;
      // FillAllMax(LocalMax);
      // SetupOutputs();
      DoStage2(TRUE);
      return;
    }
  // ��� �� ���������
  Start=1;
  ProgressOut();
  Complete=OutReady=0;
}
//-----------------------------------------------------------------------

// ������ 2 - ��������� ����������
void rdsbcppBlockClass::DoStage2(BOOL init)
{ TMaxCandidate *f;
  BOOL cand_ready;
  int count=1000;

  if(init)
    { int total=LocalMax.Count();
      FVal.Resize(0);
      if(total==0) // ��� ���������� � ���������, ��� �������
        { stage2_progress=stage2_progmax=1.0;
          return;
        }
      // �������� ��������� - ����� ��������� ����� ���� ����������
      stage2_progmax=LocalMax.SumDeltas();
      // ������ �������� ����� ����� stage2_progmax-(����� �����)
      return;
    }

  for(;;) // �� ��������� count ��� ��������� �������
  {

  // ��������
  if(LocalMax.IsEmpty())
    { // ������� ��� �������� - ��������� � ����������
      if(NextMax.IsEmpty())
        { // ��� ��������� - ��������� � ������ 3
          stage2_progress=stage2_progmax=1.0;
          ProgressOut();
          stage=3;
          Start=Ready=1;
          DoStage3(TRUE);
          return;
        }
      LocalMax.Exchange(NextMax);
    }

  f=LocalMax.RemoveTop();
  // ������������ ��������� f

  // ���������, �� ����� �� �� ���
  cand_ready=TRUE;
  if(setup_end_steps) // ������� ����� ����� �� �������� �����
    { if(f->dx!=rdsbcppHugeDouble && f->dx>setup_dx_min)
        cand_ready=FALSE;
      else if(f->dy!=rdsbcppHugeDouble && f->dy>setup_dy_min)
        cand_ready=FALSE;
      else if(f->dz!=rdsbcppHugeDouble && f->dz>setup_dz_min)
        cand_ready=FALSE;
    }
  if(cand_ready && search_by_df) // ����� ����� � ������ �����������
    { if(f->cycles_in_df<setup_df_cycles) // ������� � ������
        cand_ready=FALSE;
    }

  if(cand_ready) // �������� �������
    StoreFinalMax(f);
  else // �������� ������
    { // ��������� ���� � ��� ����
      int idx_0,idx_1,idy_0,idy_1,idz_0,idz_1;
      double dx_aux,dy_aux,dz_aux;
      BOOL changed=FALSE,equal;
      double _xnew,_ynew,_znew,_fnew=f->f;
      #if L57_MODELVERSION>=1
        equal=setup_no_plateau && (f->steps_down==0);
      #else
        equal=FALSE;
      #endif
      f->StepDown(f->dx,f->x_index,x_cnt,idx_0,idx_1,dx_aux,&equal);
      f->StepDown(f->dy,f->y_index,y_cnt,idy_0,idy_1,dy_aux,&equal);
      f->StepDown(f->dz,f->z_index,z_cnt,idz_0,idz_1,dz_aux,&equal);
      f->steps_down++;
      // ����������� ����������� (x,y,z)
      for(int idx=idx_0;idx<=idx_1;idx++)
        { double x=f->x+dx_aux*idx;
          for(int idy=idy_0;idy<=idy_1;idy++)
            { double y=f->y+dy_aux*idy;
              for(int idz=idz_0;idz<=idz_1;idz++)
                { double z=f->z+dz_aux*idz,fval;
                  Arguments.xval=x;
                  Arguments.yval=y;
                  Arguments.zval=z;
                  fval=SelectedFunc(RealFuncId,Arguments);
                  if(fval==rdsbcppHugeDouble)
                    { equal=FALSE;
                      continue;
                    }
                  if(equal && fabs(fval-f->f)>1e-38) // �� ����������
                    equal=FALSE;
                  if(fval>_fnew)
                    { _xnew=x; _ynew=y; _znew=z; _fnew=fval; changed=TRUE;
                       equal=FALSE;
                    }
                } // for(int idz=idz_0;...)
            } // for(int idy=idy_0;...)
        } // for(int idx=idx_0;...)
      #if L57_MODELVERSION>=1
        if(setup_no_plateau && equal) // ��� ������ ����� ��������� - ���������� � �����������
          { delete f;
            break;
          }
      #endif
      if(changed) // ������ ����� ��������
        { if(_fnew-f->f<setup_df) // � ������
            f->cycles_in_df++;
          else
            f->cycles_in_df=0;
          f->x=_xnew;
          f->y=_ynew;
          f->z=_znew;
          f->f=_fnew;
        }
      else
        f->cycles_in_df++; // � ������
      // ��������� �� ��������� ���
      NextMax.AddExisting(f);
    } // else [ if(cand_ready) ]

    count--;
    if(count<=0)
      break;

  } // for(;;)

  // ���������������
  Start=Ready=1;
  stage2_progress=stage2_progmax-LocalMax.SumDeltas();
  ProgressOut();
  Complete=OutReady=0;
}
//-----------------------------------------------------------------------

// �������� ����� � ������� ��������� (��� ����������� ���������(
void rdsbcppBlockClass::StoreFinalMaxSimple(double x,double y,double z,double f)
{ // ���� �� ������ ��������
  if(!FinalMax.First)
    { FinalMax.AddNew(x,y,z,f);
      return;
    }
  // ���������� � ������ ���������
  if(fabs(FinalMax.First->f-f)<=setup_df_equal) // ������� �����
    { FinalMax.AddNew(x,y,z,f);
      return;
    }
  if(f>FinalMax.First->f) // ����� ������ ������
    { FinalMax.Clear();
      FinalMax.AddNew(x,y,z,f);
    }
  // ����� ������ ������ - ������ �� ������
}
//-----------------------------------------------------------------------

// ��� ������ 2 - ����������� �������� � �������
void rdsbcppBlockClass::StoreFinalMax(TMaxCandidate *fnew)
{ BOOL higher=FALSE,lower=FALSE;
  // ���������� � ����������
  for(TMaxCandidate *f=FinalMax.First;f!=NULL;f=f->Next)
    { if(fabs(fnew->f-f->f)<=setup_df_equal) // ������� �����
        continue;
      if(fnew->f>f->f) // ����� ������ ������
        { higher=TRUE;
          break;
        }
      // ����� ������ ������
      lower=TRUE;
      break;
    }

  if(lower)
    { // ����� �������� ������ ������ - �� ����������
      delete fnew;
      return;
    }
  if(higher)
    { // ����� �������� ������ ������ - ������� ������
      FinalMax.Clear();
    }
  FinalMax.AddExisting(fnew);
}
//-----------------------------------------------------------------------

// ��� ������ 3 - ������� � ������� ����� ����� � �������� �� FinalMax
void rdsbcppBlockClass::InsertNewArgs(void)
{ int cnt=FinalMax.Count();
  TDoubleArray oldx,oldy,oldz;

  // ������ ������� �������� ���������� (��� ��������)
  oldx.Allocate(x_cnt,TRUE);
  for(int i=0;i<x_cnt;i++)
    oldx.Dbl[i]=GetX(i);
  oldy.Allocate(y_cnt,TRUE);
  for(int i=0;i<y_cnt;i++)
    oldy.Dbl[i]=GetY(i);
  oldz.Allocate(z_cnt,TRUE);
  for(int i=0;i<z_cnt;i++)
    oldz.Dbl[i]=GetZ(i);

  // ������ ������� ���������� ����������
  // � ������ ������ ������ �������� ��������� ����� �������� ������� ���������
  AddXArg.Allocate(cnt);
  AddYArg.Allocate(cnt);
  AddZArg.Allocate(cnt);
  for(TMaxCandidate *f=FinalMax.First;f!=NULL;f=f->Next)
    { // x
      if(oldx.CanInsertValue(f->x))
        AddXArg.Dbl[AddXArg.Count++]=f->x;
      // y
      if(oldy.CanInsertValue(f->y))
        AddYArg.Dbl[AddYArg.Count++]=f->y;
      // z
      if(oldz.CanInsertValue(f->z))
        AddZArg.Dbl[AddZArg.Count++]=f->z;
    }

  // ������������� ������ � ����������� �����������
  AddXBase=x_cnt;
  AddYBase=y_cnt;
  AddZBase=z_cnt;
  x_cnt+=AddXArg.Count;
  y_cnt+=AddYArg.Count;
  z_cnt+=AddZArg.Count;
  X.Resize(y_cnt+1,z_cnt+1,TRUE);
  Y.Resize(x_cnt+1,z_cnt+1,TRUE);
  Z.Resize(x_cnt+1,y_cnt+1,TRUE);
  Fx.Resize(y_cnt+1,z_cnt+1,TRUE);
  Fy.Resize(x_cnt+1,z_cnt+1,TRUE);
  Fz.Resize(x_cnt+1,y_cnt+1,TRUE);
  XY.Resize(z_cnt,4,TRUE);
  XZ.Resize(y_cnt,4,TRUE);
  YZ.Resize(x_cnt,4,TRUE);
  // ���������� �������������� ��������� � ����� ������
  for(int xi=0;xi<AddXArg.Count;xi++)
    { double v=AddXArg.Dbl[xi];
      Y[xi+1+AddXBase][0]=v;
      Fy[xi+1+AddXBase][0]=v;
      Z[xi+1+AddXBase][0]=v;
      Fz[xi+1+AddXBase][0]=v;
      YZ[xi+AddXBase][0]=v;
    }
  for(int yi=0;yi<AddYArg.Count;yi++)
    { double v=AddYArg.Dbl[yi];
      X[yi+1+AddYBase][0]=v;
      Fx[yi+1+AddYBase][0]=v;
      Z[0][yi+1+AddYBase]=v;
      Fz[0][yi+1+AddYBase]=v;
      XZ[yi+AddYBase][0]=v;
    }
  for(int zi=0;zi<AddZArg.Count;zi++)
    { double v=AddZArg.Dbl[zi];
      X[0][zi+1+AddZBase]=v;
      Fx[0][zi+1+AddZBase]=v;
      Y[0][zi+1+AddZBase]=v;
      Fy[0][zi+1+AddZBase]=v;
      XY[zi+AddZBase][0]=v;
    }
}
//-----------------------------------------------------------------------

// ����������� ������� ������� (������� ����� ����������)
BOOL rdsbcppBlockClass::SortLineMatrix(rdsbcppVarAncestor &M)
{ RDS_ARRAYACCESSDATA Data;
  if(!M.GetAccessData(&Data))
    return FALSE;
  if((!Data.Exists) || Data.ItemSize!=sizeof(double) || rdsGetVarArrayElementType(M.GetVoidPtr())!=RDS_VARTYPE_DOUBLE)
    return FALSE;
  return SortMatrix(FALSE,Data.Rows,Data.Cols,(double*)(Data.Data),0);
}
//-----------------------------------------------------------------------

// ����������� ������� ������� ���� ����������
BOOL rdsbcppBlockClass::SortFieldMatrix(rdsbcppVarAncestor &M)
{ RDS_ARRAYACCESSDATA Data;
  if(!M.GetAccessData(&Data))
    return FALSE;
  if((!Data.Exists) || Data.ItemSize!=sizeof(double) || rdsGetVarArrayElementType(M.GetVoidPtr())!=RDS_VARTYPE_DOUBLE)
    return FALSE;
  return SortMatrixField(TRUE,Data.Rows,Data.Cols,(double*)(Data.Data),0,0) &&
         SortMatrixField(FALSE,Data.Rows,Data.Cols,(double*)(Data.Data),0,0);
}
//-----------------------------------------------------------------------

// ���������� �������� ������� �� xi,yi,zi
void rdsbcppBlockClass::ProcessValues_xiyizi(double *pf)
{ double x,y,z,f,old_f;

#define XY_f(zi) (XY[zi][3])
#define XZ_f(yi) (XZ[yi][3])
#define YZ_f(xi) (YZ[xi][3])

  //inline double &GetX(int ix) {return Y[ix+1][0]; }
  //inline double &GetY(int iy) { return X[iy+1][0]; }
  //inline double &GetZ(int iz) { return X[0][iz+1]; }

  x=Y[xi+1][0];
  y=X[yi+1][0];
  z=X[0][zi+1];
  Arguments.xval=x;
  Arguments.yval=y;
  Arguments.zval=z;
  f=SelectedFunc(RealFuncId,Arguments);
  if(pf) *pf=f;
  if(f!=rdsbcppHugeDouble)
    { // �������� f(*,y,z), �.�. X(y,z)
      old_f=FxItem(yi,zi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { FxItem(yi,zi)=f;
          XItem(yi,zi)=x;
        }
      // �������� f(x,*,z), �.�. Y(x,z)
      old_f=FyItem(xi,zi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { FyItem(xi,zi)=f;
          YItem(xi,zi)=y;
        }
      // �������� f(x,y,*), �.�. Z(x,y)
      old_f=FzItem(xi,yi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { FzItem(xi,yi)=f;
          ZItem(xi,yi)=z;
        }
      // ���������� ��������
      if(absFmax==rdsbcppHugeDouble || absFmax<f)
        { absFmax=Fmax=f;
          Xmax=x;
          Ymax=y;
          Zmax=z;
        }
      // �������� f(x,*,*), �.�. YZ(x)
      old_f=YZ_f(xi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { YZ_f(xi)=f;
          YZ[xi][1]=y;
          YZ[xi][2]=z;
        }
      // �������� f(*,y,*), �.�. XZ(y)
      old_f=XZ_f(yi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { XZ_f(yi)=f;
          XZ[yi][1]=x;
          XZ[yi][2]=z;
        }
      // �������� f(*,*,z), �.�. XY(z)
      old_f=XY_f(zi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { XY_f(zi)=f;
          XY[zi][1]=x;
          XY[zi][2]=y;
        }
    } // if(f!=rdsbcppHugeDouble)

#undef XY_f
#undef XZ_f
#undef YZ_f
}
//-----------------------------------------------------------------------


// ����� ��������� �����
BOOL rdsbcppBlockClass::GetNextPoint(void)
{ BOOL end=FALSE;
  zi+=1;
  if(zi>=z_cnt)
    { zi=0;
      yi+=1;
      if(yi>=y_cnt)
        { yi=0;
          xi+=1;
          if(xi>=x_cnt)
            end=TRUE;
        }
    }
  return end;
}
//-----------------------------------------------------------------------

// ������ 3 - �������� ������
void rdsbcppBlockClass::DoStage3(BOOL init)
{ BOOL _finished=FALSE;
  double x,y,z,f;
  if(init)
    { // ����� �������� �� ��� ������� ������ � �������,
      // ��������������� ���������� FinalMax
      InsertNewArgs();
      // �������� ���������
      stage3_progmax=((double)x_cnt)*((double)y_cnt)*((double)z_cnt);
      stage3_progress=0;
      xi=yi=zi=0;
      Start=1;
      return;
    }

  // ��������� ��������� � �� ��������� - �������
  // ����� xi,yi,zi
  while(!_finished)
    { if(xi<AddXBase && yi<AddYBase && zi<AddZBase)
        { // ��� ����� - �� ��� �����������
          stage3_progress+=1.0;
          _finished=GetNextPoint();
        }
      else // ��� ����� ����� �������
        break;
    }
  if(!_finished)
    { ProcessValues_xiyizi();
      stage3_progress+=1.0;
      _finished=GetNextPoint();
    }


  Ready=1;
  if(_finished)
    { // ���������� ������
      bool _err=FALSE;
      if(!SortLineMatrix(XY))
        _err=TRUE;
      if(!SortLineMatrix(XZ))
        _err=TRUE;
      if(!SortLineMatrix(YZ))
        _err=TRUE;
      if(!SortFieldMatrix(X))
        _err=TRUE;
      if(!SortFieldMatrix(Y))
        _err=TRUE;
      if(!SortFieldMatrix(Z))
        _err=TRUE;
      if(!SortFieldMatrix(Fx))
        _err=TRUE;
      if(!SortFieldMatrix(Fy))
        _err=TRUE;
      if(!SortFieldMatrix(Fz))
        _err=TRUE;

      #if L57_MODELVERSION>=2
        // ���� ������ ��������� ������
        if(setup_ds_X || setup_ds_Y || setup_ds_Z ||
           setup_ds_XY || setup_ds_XZ || setup_ds_YZ)
          { // ��������� � ������ 4 (��������� ��������� ������)
            stage3_progress=stage3_progmax=1.0;
            ProgressOut();
            stage=4;
            Start=Ready=1;
            DoStage4(TRUE);
            return;
          }
        else
          SetupOutputs(_err);
      #else
        SetupOutputs(_err);
      #endif
    }
  else
    { Start=1;
      ProgressOut();
      Complete=OutReady=0;
    }
}
//-----------------------------------------------------------------------

#if L57_MODELVERSION>=2
// ������ 4 - ��������� ������
void rdsbcppBlockClass::DoStage4(BOOL init)
{ int count=1000;
  BOOL cand_ready;

  if(init)
    {// ��������
      stage4_progmax=0;
      if(setup_ds_X)
        stage4_progmax+=y_cnt*z_cnt;
      if(setup_ds_Y)
        stage4_progmax+=x_cnt*z_cnt;
      if(setup_ds_Z)
        stage4_progmax+=x_cnt*y_cnt;
      if(setup_ds_XY)
        stage4_progmax+=z_cnt;
      if(setup_ds_XZ)
        stage4_progmax+=y_cnt;
      if(setup_ds_YZ)
        stage4_progmax+=x_cnt;
      stage4_progress=0;
      if(stage4_progmax<0.5)
        stage4_progmax=1.0;
      SubStage4=L57SS4_NONE;
      stage=4;
      Start=1;
      return;
    }
  // ��������

  for(;;) // �� ��������� count ��� ��������� �������
    { BOOL nextpoint=TRUE;
      if(SubStage4!=L57SS4_NONE)
        { // ���������
          nextpoint=FALSE;

          // ���������, �� ����� �� ������� ��������
          cand_ready=TRUE;
          if((!Stage4_FixX) && Stage4Max.dx!=rdsbcppHugeDouble && Stage4Max.dx>setup_ds_edx)
            cand_ready=FALSE;
          else if((!Stage4_FixY) && Stage4Max.dy!=rdsbcppHugeDouble && Stage4Max.dy>setup_ds_edy)
            cand_ready=FALSE;
          else if((!Stage4_FixZ) && Stage4Max.dz!=rdsbcppHugeDouble && Stage4Max.dz>setup_ds_edz)
            cand_ready=FALSE;
          if(cand_ready) // �������� �������
            { // ���������� � �������
              Stage4_StorePointToMatr();
              nextpoint=TRUE;
            }
          else // �������� ������
            Stage4_StepDownAndInspect();
        } // if(SubStage4!=L57SS4_NONE)

      if(nextpoint)
        { // ����� ��������� �����
          if(Stage4_NextPoint())
            { // ������� ��������
              Ready=1;
              SetupOutputs();
              return;
            }
          stage4_progress+=1.0;  
        }

      count--;
      if(count<=0)
        break;
    } // for(;;)

  // ���������������
  Start=Ready=1;
  ProgressOut();
  Complete=OutReady=0;

}
//-----------------------------------------------------------------------

// ��������� ��� � Stage4Max � ��������� �����
void rdsbcppBlockClass::Stage4_StepDownAndInspect(void)
{ int idx_0=0,idx_1=0,idy_0=0,idy_1=0,idz_0=0,idz_1=0;
  double dx_aux=0.0,dy_aux=0.0,dz_aux=0.0;
  double _xnew,_ynew,_znew,_fnew=Stage4Max.f;
  BOOL changed=FALSE;

  /*
  if(fabs(Stage4Max.x-0.2)<1e-12 || fabs(Stage4Max.x-0.3)<1e-12)
            changed=changed;
  */

  if(!Stage4_FixX)
    Stage4Max.StepDownStage4(Stage4Max.dx,x_cnt,idx_0,idx_1,dx_aux);
  if(!Stage4_FixY)
    Stage4Max.StepDownStage4(Stage4Max.dy,y_cnt,idy_0,idy_1,dy_aux);
  if(!Stage4_FixZ)
    Stage4Max.StepDownStage4(Stage4Max.dz,z_cnt,idz_0,idz_1,dz_aux);

  // ����������� ����������� (x,y,z)
  for(int idx=idx_0;idx<=idx_1;idx++)
    { double x=Stage4Max.x+dx_aux*idx;
      if(idx!=0 && (x<_Xmin || x>_Xmax))
        continue;
      for(int idy=idy_0;idy<=idy_1;idy++)
        { double y=Stage4Max.y+dy_aux*idy;
          if(idy!=0 && (y<_Ymin || y>_Ymax))
            continue;
          for(int idz=idz_0;idz<=idz_1;idz++)
            { double z=Stage4Max.z+dz_aux*idz,fval;
              if(idz!=0 && (z<_Zmin || z>_Zmax))
                continue;
              Arguments.xval=x;
              Arguments.yval=y;
              Arguments.zval=z;
              fval=SelectedFunc(RealFuncId,Arguments);
              if(fval==rdsbcppHugeDouble)
                continue;
              if(fval>_fnew)
                { _xnew=x; _ynew=y; _znew=z; _fnew=fval; changed=TRUE;
                }
            }
        }
    }
  if(changed) // ������ ����� ��������
    { Stage4Max.x=_xnew;
      Stage4Max.y=_ynew;
      Stage4Max.z=_znew;
      Stage4Max.f=_fnew;
    }
}
//-----------------------------------------------------------------------


// ������ ���������� ����� � ������� �� Stage4Max
void rdsbcppBlockClass::Stage4_StorePointToMatr(void)
{
  switch(SubStage4)
    { case L57SS4_XY: // ��������� (x,y)* - ������� z, ������� ������� XY
        XY[zi][1]=Stage4Max.x;
        XY[zi][2]=Stage4Max.y;
        XY[zi][3]=Stage4Max.f;
        break;

      case L57SS4_XZ: // ��������� (x,z)* - ������� y, ������� ������� XZ
        XZ[yi][1]=Stage4Max.x;
        XZ[yi][2]=Stage4Max.z;
        XZ[yi][3]=Stage4Max.f;
        break;

      case L57SS4_YZ: // ��������� (y,z)* - ������� x, ������� ������� YZ
        YZ[xi][1]=Stage4Max.y;
        YZ[xi][2]=Stage4Max.z;
        YZ[xi][3]=Stage4Max.f;
        break;

      case L57SS4_X: // ��������� x* - ������� (y,z), ������� ������� X,Fx
        XItem(yi,zi)=Stage4Max.x;
        FxItem(yi,zi)=Stage4Max.f;
        break;

      case L57SS4_Y: // ��������� y* - ������� (x,z), ������� ������� Y,Fy
        YItem(xi,zi)=Stage4Max.y;
        FyItem(xi,zi)=Stage4Max.f;
        break;

      case L57SS4_Z: // ��������� z* - ������� (x,y), ������� ������� Z,Fz
        ZItem(xi,yi)=Stage4Max.z;
        FzItem(xi,yi)=Stage4Max.f;
        break;
    } // switch(SubStage4)
}
//-----------------------------------------------------------------------

// ����� � Stage4Max ��������� �����
BOOL rdsbcppBlockClass::Stage4_NextPoint(void)
{ BOOL inc=TRUE;

  Stage4Max.dx=(x_cnt==1?rdsbcppHugeDouble:Dx);
  Stage4Max.dy=(y_cnt==1?rdsbcppHugeDouble:Dy);
  Stage4Max.dz=(z_cnt==1?rdsbcppHugeDouble:Dz);

  for(;;)
    { switch(SubStage4)
        { case L57SS4_NONE: // ���
            break;

          case L57SS4_XY: // ��������� (x,y)* - ������� z, ������� ������� XY
            if(!setup_ds_XY) // ��������� ���������
              break;
            if(inc)
              { zi++;
                if(zi>=z_cnt)
                  break;
              }
            // �������� � ������� ������
            Stage4_FixX=FALSE;
            Stage4_FixY=FALSE;
            Stage4_FixZ=TRUE;
            Stage4Max.x=XY[zi][1];
            Stage4Max.y=XY[zi][2];
            Stage4Max.z=XY[zi][0];
            Stage4Max.f=XY[zi][3];
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          case L57SS4_XZ: // ��������� (x,z)* - ������� y, ������� ������� XZ
            if(!setup_ds_XZ) // ��������� ���������
              break;
            if(inc)
              { yi++;
                if(yi>=y_cnt)
                  break;
              }
            // �������� � ������� ������
            Stage4_FixX=FALSE;
            Stage4_FixY=TRUE;
            Stage4_FixZ=FALSE;
            Stage4Max.x=XZ[yi][1];
            Stage4Max.y=XZ[yi][0];
            Stage4Max.z=XZ[yi][2];
            Stage4Max.f=XZ[yi][3];
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          case L57SS4_YZ: // ��������� (y,z)* - ������� x, ������� ������� YZ
            if(!setup_ds_YZ) // ��������� ���������
              break;
            if(inc)
              { xi++;
                if(xi>=x_cnt)
                  break;
              }
            // �������� � ������� ������
            Stage4_FixX=TRUE;
            Stage4_FixY=FALSE;
            Stage4_FixZ=FALSE;
            Stage4Max.x=YZ[xi][0];
            Stage4Max.y=YZ[xi][1];
            Stage4Max.z=YZ[xi][2];
            Stage4Max.f=YZ[xi][3];
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          case L57SS4_X: // ��������� x* - ������� (y,z), ������� ������� X,Fx
            if(!setup_ds_X) // ��������� ���������
              break;
            if(inc)
              { yi++;
                if(yi>=y_cnt)
                  { yi=0;
                    zi++;
                    if(zi>=z_cnt)
                      break;
                  }
              }
            // �������� � ������� ������
            Stage4_FixX=FALSE;
            Stage4_FixY=TRUE;
            Stage4_FixZ=TRUE;
            Stage4Max.x=XItem(yi,zi);
            Stage4Max.y=GetY(yi);
            Stage4Max.z=GetZ(zi);
            Stage4Max.f=FxItem(yi,zi);
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          case L57SS4_Y: // ��������� y* - ������� (x,z), ������� ������� Y,Fy
            if(!setup_ds_Y) // ��������� ���������
              break;
            if(inc)
              { xi++;
                if(xi>=x_cnt)
                  { xi=0;
                    zi++;
                    if(zi>=z_cnt)
                      break;
                  }
              }
            // �������� � ������� ������
            Stage4_FixX=TRUE;
            Stage4_FixY=FALSE;
            Stage4_FixZ=TRUE;
            Stage4Max.x=GetX(xi);
            Stage4Max.y=YItem(xi,zi);
            Stage4Max.z=GetZ(zi);
            Stage4Max.f=FyItem(xi,zi);
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          case L57SS4_Z: // ��������� z* - ������� (x,y), ������� ������� Z,Fz
            if(!setup_ds_Z) // ��������� ���������
              break;
            if(inc)
              { xi++;
                if(xi>=x_cnt)
                  { xi=0;
                    yi++;
                    if(yi>=y_cnt)
                      break;
                  }
              }
            // �������� � ������� ������
            Stage4_FixX=TRUE;
            Stage4_FixY=TRUE;
            Stage4_FixZ=FALSE;
            Stage4Max.x=GetX(xi);
            Stage4Max.y=GetY(yi);
            Stage4Max.z=ZItem(xi,yi);
            Stage4Max.f=FzItem(xi,yi);
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          default:
            rdsStopCalc();
            rdsMessageBox("���������� ������ ������������� - ����������� ����� ��������� ������",
              rdsbcppBlockData->BlockName,MB_ICONERROR|MB_OK);
            return TRUE;

        } // switch(SubStage4)
      // ���� �������� ������ ���� ����� ������� ���������
      inc=FALSE;
      SubStage4++;
      xi=yi=zi=0;
      if(SubStage4>L57SS4_Z) // ���
        return TRUE;

      // ������ �� ����, �� ��� � inc=FALSE
    } // for(;;)

}
//-----------------------------------------------------------------------



#endif // #if L57_MODELVERSION>=2


#endif // #ifdef L57MS_AFTERCLASS
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// ������ �������
//---------------------------------------------------------------------------
#ifdef L57MS_START
Start=1;
#endif // #ifdef L57MS_START
//-----------------------------------------------------------------------





//---------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------
#ifdef L57MS_MODEL

BOOL changed=FALSE;
//double x,y,z,f,old_f,maxval;
double f;

int _count=1000;


// ������� ��������� ������ ���� �������
Ready=0;

// �������� ���������� ������
if(enabled_chg)
  { BOOL enabled=TRUE;
    enabled_chg=0;
    for(int i=0;enabled && i<Enabled.Size();i++)
      if(!Enabled[i])
        enabled=FALSE;
    _enabled=enabled?1:0;
    if(_enabled)
      { if(!Working)
          Complete=1;
        Ready=1;
      }
  }
if(!_enabled)
  { Progress=0.0;
    Ready=1;
    Complete=0;
    return;
  }

// ������������� �������
if(InputConn)
  RealFuncId=FuncId;
else
  RealFuncId=SetupFuncId;
if(RealFuncId!=OldFuncId)
  { OldFuncId=RealFuncId;
    changed=TRUE;
  }

if(func_chg) // ���������� �������
  { BOOL FuncsChanged=FALSE;
    func_chg=0;
    int n=F.Size();
    // ���������, ����� ������
    if(n!=FS.Size())
      { FS.Resize(n,TRUE);
        FuncsChanged=TRUE;
      }
    for(int i=0;i<n;i++)
      { rdsbcstMDouble Fi=F[i],FSi=FS[i];
        if(TL57TextFuncFArray::IsBadMatrix(&Fi)) // ��� ����� ������ �� F[i]
          continue;
        // ������ ����, �� ����� ��?
        if(TL57TextFuncFArray::CompareMatr(&Fi,&FSi))
          { // ����� ��� ����
            Fi.Resize(2,2);
            TL57TextFuncFArray::SetBadMatrix(&Fi);
            continue;
          }
        // ����� ������
        FuncsChanged=TRUE;
        rdsExchangeVarArrays(Fi.GetPtr(),FSi.GetPtr());
        Fi.Resize(2,2);
        TL57TextFuncFArray::SetBadMatrix(&Fi);
      }
    if(FuncsChanged)
      { changed=TRUE;
        Arguments.Functions.Realloc(n);
        for(int i=0;i<n;i++)
          Arguments.Functions[i].Setup(&(FS[i]));
      }
  }

if(ArrayChanged)
  { // ��������� �������� ��������� �������
    if(A.Size()!=A_old.Size()) // ��������� ������
      changed=TRUE;
    else // ������ ��� ��
      for(int i=0;i<A.Size();i++)
        if(A[i]!=A_old[i])
          { changed=TRUE;
            break;
          } 
    if(changed)
      A_old=A;
  }

// ���������
if(A.IsEmpty())
  Arguments.Params.Setup(NULL);
else
  Arguments.Params.Setup(A.GetBuffer(),A.Size());

// ���������
if(in_xdiap) { _Xmin=xmin; _Xmax=xmax; }
else { _Xmin=setup_xmin; _Xmax=setup_xmax;}
Dx=in_dx?dx:setup_dx;
if(in_ydiap) { _Ymin=ymin; _Ymax=ymax; }
else { _Ymin=setup_ymin; _Ymax=setup_ymax;}
Dy=in_dy?dy:setup_dy;
if(in_zdiap) { _Zmin=zmin; _Zmax=zmax; }
else { _Zmin=setup_zmin; _Zmax=setup_zmax;}
Dz=in_dz?dz:setup_dz;

if(!changed)
  { // �� ���������� �� ���������
    if(xdiap_chg && (_Xmin!=old_xmin || _Xmax!=old_xmax || Dx!=old_dx))
      changed=TRUE;
    else if(ydiap_chg && (_Ymin!=old_ymin || _Ymax!=old_ymax || Dy!=old_dy))
      changed=TRUE;
    else if(zdiap_chg && (_Zmin!=old_zmin || _Zmax!=old_zmax || Dz!=old_dz))
      changed=TRUE;
  }


if(changed)
  { // ���-�� ����������
    BOOL rok;
    old_xmin=_Xmin; old_xmax=_Xmax; old_dx=Dx;
    old_ymin=_Ymin; old_ymax=_Ymax; old_dy=Dy;
    old_zmin=_Zmin; old_zmax=_Zmax; old_dz=Dz;
    // ���������� ������� ������������ ������
    xdiap_chg=ydiap_chg=zdiap_chg=0;
    ArrayChanged=0;
    stage=0; // �������� ����� � ������

    // ����������� ������
    x_cnt=CalcNPointsByDiap(_Xmin,_Xmax,Dx);
    y_cnt=CalcNPointsByDiap(_Ymin,_Ymax,Dy);
    z_cnt=CalcNPointsByDiap(_Zmin,_Zmax,Dz);
    if(x_cnt<1 || y_cnt<1 || z_cnt<1)
      { // ������
        X.Resize(0,0);
        Y.Resize(0,0);
        Z.Resize(0,0);
        XY.Resize(0,0);
        XZ.Resize(0,0);
        YZ.Resize(0,0);
        Fx.Resize(0,0);
        Fy.Resize(0,0);
        Fz.Resize(0,0);
        ClearTempData();
        Xmax=Ymax=Zmax=Fmax=
        Xmax_f=Ymax_f=Zmax_f=Fmax_f=rdsbcppHugeDouble;
        Complete=OutReady=Ready=Finished=1;
        AllMax.Resize(0,0);
        return;
      }
    if(x_cnt==1)
      { _Xmax=_Xmin; Dx=1.0; }
    if(y_cnt==1)
      { _Ymax=_Ymin; Dy=1.0; }
    if(z_cnt==1)
      { _Zmax=_Zmin; Dz=1.0; }

    #if L57_MODELVERSION>=9
      // ����� � ������ ������
      Started=1;
      dwStartTime=GetTickCount();
      Time=0.0;
    #endif

    // ����������� �������� - �������
    rok=X.Resize(y_cnt+1,z_cnt+1);
    rok=rok && Y.Resize(x_cnt+1,z_cnt+1);
    rok=rok && Z.Resize(x_cnt+1,y_cnt+1);
    rok=rok && Fx.Resize(y_cnt+1,z_cnt+1);
    rok=rok && Fy.Resize(x_cnt+1,z_cnt+1);
    rok=rok && Fz.Resize(x_cnt+1,y_cnt+1);
    rok=rok && XY.Resize(z_cnt,4);
    rok=rok && XZ.Resize(y_cnt,4);
    rok=rok && YZ.Resize(x_cnt,4);
    #if L57_MODELVERSION>=5
      /*
      if(F0xyConn)
        rok=rok && F0xy.Resize(x_cnt+1,y_cnt+1);
      else
        F0xy.Resize(0,0);
      if(F0xzConn)
        rok=rok && F0xz.Resize(x_cnt+1,z_cnt+1);
      else
        F0xz.Resize(0,0);
      if(F0yzConn)
        rok=rok && F0yz.Resize(y_cnt+1,z_cnt+1);
      else
        F0yz.Resize(0,0);
      */
      F0xy.Resize(0,0);
      F0xz.Resize(0,0);
      F0yz.Resize(0,0);
      SurfacesArraySet=FALSE;
    #endif
    if(deep_search)
      { xy_cnt=x_cnt*y_cnt;
        rok=rok && FVal.Resize(xy_cnt*z_cnt,FALSE);
      }
    else
      { xy_cnt=0;
        FVal.Resize(0,FALSE);
      }
    if(!rok)
      { rdsStopCalc();
        X.Resize(0,0);
        Y.Resize(0,0);
        Z.Resize(0,0);
        XY.Resize(0,0);
        XZ.Resize(0,0);
        YZ.Resize(0,0);
        Fx.Resize(0,0);
        Fy.Resize(0,0);
        Fz.Resize(0,0);
        #if L57_MODELVERSION>=5
          F0xy.Resize(0,0);
          F0xz.Resize(0,0);
          F0yz.Resize(0,0);
        #endif
        ClearTempData();
        rdsMessageBox("������� ������� ��������� ���� �� ����� ��� ���������� ����������, �� ������� ������ ��� "
                      "���������� ������ ��������� ������� ������ �������.",
                      rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
        return;
      }

    // ��������� �������� ������� ��������� ������
    for(int i=0;i<X.Size();i++)
      *(X.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<Y.Size();i++)
      *(Y.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<Z.Size();i++)
      *(Z.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<XY.Size();i++)
      *(XY.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<XZ.Size();i++)
      *(XZ.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<YZ.Size();i++)
      *(YZ.GetBuffer()+i)=rdsbcppHugeDouble;
    Xmax=Ymax=Zmax=rdsbcppHugeDouble;
    absFmax=Fmax=rdsbcppHugeDouble;

    // ��������� ��������� ������� ��������� ������
    // (��� ����� ��������� ��������� �������)
    for(int i=0;i<Fx.Size();i++)
      *(Fx.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<Fy.Size();i++)
      *(Fy.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<Fz.Size();i++)
      *(Fz.GetBuffer()+i)=rdsbcppHugeDouble;

    // ��������� ��������� ������ � ������� ������ ������� ����������
    // ���������� ��������� �������
    X[0][0]=rdsbcppHugeDouble;
    Y[0][0]=rdsbcppHugeDouble;
    Z[0][0]=rdsbcppHugeDouble;
    Fx[0][0]=rdsbcppHugeDouble;
    Fy[0][0]=rdsbcppHugeDouble;
    Fz[0][0]=rdsbcppHugeDouble;
    /*
    #if L57_MODELVERSION>=5
      for(int i=0;i<F0xy.Size();i++)
        *(F0xy.GetBuffer()+i)=rdsbcppHugeDouble;
      for(int i=0;i<F0xz.Size();i++)
        *(F0xz.GetBuffer()+i)=rdsbcppHugeDouble;
      for(int i=0;i<F0yz.Size();i++)
        *(F0yz.GetBuffer()+i)=rdsbcppHugeDouble;
    #endif
    */
    for(int xi=0;xi<x_cnt;xi++)
      { double v=vInDiap(_Xmin,_Xmax,Dx,xi,x_cnt); //(xi==(x_cnt-1)?_Xmax:(_Xmin+xi*Dx));
        Y[xi+1][0]=v;
        Fy[xi+1][0]=v;
        Z[xi+1][0]=v;
        Fz[xi+1][0]=v;
        YZ[xi][0]=v;
        /*
        #if L57_MODELVERSION>=5
          if(F0xyConn)
            F0xy[xi+1][0]=v;
          if(F0xzConn)
            F0xz[xi+1][0]=v;
        #endif
        */
      }
    for(int yi=0;yi<y_cnt;yi++)
      { double v=vInDiap(_Ymin,_Ymax,Dy,yi,y_cnt); //(yi==(y_cnt-1)?_Ymax:(_Ymin+yi*Dy));
        X[yi+1][0]=v;
        Fx[yi+1][0]=v;
        Z[0][yi+1]=v;
        Fz[0][yi+1]=v;
        XZ[yi][0]=v;
        /*
        #if L57_MODELVERSION>=5
          if(F0xyConn)
            F0xy[0][yi+1]=v;
          if(F0yzConn)
            F0yz[yi+1][0]=v;
        #endif
        */
      }
    for(int zi=0;zi<z_cnt;zi++)
      { double v=vInDiap(_Zmin,_Zmax,Dz,zi,z_cnt); //(zi==(z_cnt-1)?_Zmax:(_Zmin+zi*Dz));
        X[0][zi+1]=v;
        Fx[0][zi+1]=v;
        Y[0][zi+1]=v;
        Fy[0][zi+1]=v;
        XY[zi][0]=v;
        /*
        #if L57_MODELVERSION>=5
          if(F0xzConn)
            F0xz[0][zi+1]=v;
          if(F0yzConn)
            F0yz[0][zi+1]=v;
        #endif
        */
      }
    xi=yi=zi=0;
    InitProgress();
    Complete=OutReady=0;
    Working=1; // ���� ������
    #if L57_MODELVERSION>=5
      F0xy_ready=1; // ����� ������ ������� ���������� �� �����
      F0xz_ready=1;
      F0yz_ready=1;
      LastSurfaceOut_xy=GetTickCount();
      LastSurfaceOut_xz=LastSurfaceOut_yz=LastSurfaceOut_xy;
      #if L57_MODELVERSION>=7
        SurfaceOutDelayMs=(DWORD)(setup_surf_delayval*1000);
      #endif
      F0xyData=F0yzData=F0xzData=FALSE;
    #endif
    // ��������� ��������������� �������� � ����������
    LocalMax.Clear();
    NextMax.Clear();
    FinalMax.Clear();
    //TMaxCandidate::CheckMsg();
    AllMax.Resize(0,0);
    // �������� ��������� ������� � ������ (���� ����) ������ ��������? ����� �����
    stage0_progmax=stage1_progmax=((double)x_cnt)*((double)y_cnt)*((double)z_cnt);

    Ready=1;
    Start=1;
    ProgressOut();
    Complete=OutReady=0;
    return;
  } // if(changed)
else
  { // ������ �� ����������
    if(!Working) // � �� � �� ��������
      { Finished=Ready=1;
        return;
      }
  }

if(stage)
  { // �����-�� ��� ��������� ����������
    switch(stage)
      { case 1: // ����� ����������
          DoStage1(FALSE);
          break;
        case 2: // ��������� ����������
          DoStage2(FALSE);
          break;
        case 3: // �������� ������
          DoStage3(FALSE);
          break;
        #if L57_MODELVERSION>=2
          case 4: // ��������� ������
            DoStage4(FALSE);
            break;
        #endif
        default: // ������
          rdsStopCalc();
          rdsMessageBox("���������� ������ ������������� - ����������� ����� ������",
                        rdsbcppBlockData->BlockName,MB_ICONERROR|MB_OK);
      }
    #if L57_MODELVERSION>=9
      ShowTime();
    #endif
    return;
  }


// ������ ����

for(;;)
{

// ��������� ��������� � �� ��������� - �������
// ����� xi,yi,zi
ProcessValues_xiyizi(&f);
if(deep_search)
  { // ������ �� ��������� ������ ��������
    if(FuncVal(xi,yi,zi)!=rdsbcppHugeDouble)
      { rdsStopCalc();
        rdsMessageBox("���������� ������ ������������� - ��������� ������ ������",
          rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
        return;
      }
    FuncVal(xi,yi,zi)=f;
  }
else if(f!=rdsbcppHugeDouble)
  { // ��������� ������ ����������
    double x=Y[xi+1][0],
           y=X[yi+1][0],
           z=X[0][zi+1];
    StoreFinalMaxSimple(x,y,z,f);
  }
#if L57_MODELVERSION>=5
  if(!SurfacesArraySet)
    { if(!SetupSurfaces())
        { rdsStopCalc();
          rdsMessageBox("���������� ������ ������������� - �� ������� ������� ������� ������������",
            rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
          return;
        }
    }

  // ��������� ����� � ������� ��� ��������
  if(f!=rdsbcppHugeDouble)
    { if(F0xyConn && zi==0)
        { F0xyData=TRUE;
          F0xy[xi+1][yi+1]=f;
        }
      if(F0xzConn && yi==0)
        { F0xzData=TRUE;
          F0xz[xi+1][zi+1]=f;
        }
      if(F0yzConn && xi==0)
        { F0yzData=TRUE;
          F0yz[yi+1][zi+1]=f;
        }
    }

#endif

// ����� ��������� �����
stage0_progress+=1.0;
if(GetNextPoint())
  Working=0;

Ready=1;
if(Working) // ������� ������
  {
    #if L57_MODELVERSION>=7
      // �� ���� �� �������� �����������
      DWORD now=GetTickCount();
      // ���������: z -> y -> x
      if(F0xyConn)
        { // �� (x,y), z � ������ ���������
          if((setup_surf_everyrow && yi==0 && zi==0) ||
             (setup_surf_bydelay && (now-LastSurfaceOut_xy)>SurfaceOutDelayMs) )
            { LastSurfaceOut_xy=now;
              F0xy_ready=1;
              _count=0; // ��� ������ �� ����������� �����
            }
        }
      if(F0yzConn)
        { // �� (y,z), x � ������ ���������
          if((setup_surf_everyrow && zi==0 && xi==0) ||
             (setup_surf_bydelay && (now-LastSurfaceOut_yz)>SurfaceOutDelayMs) )
            { LastSurfaceOut_yz=now;
              F0yz_ready=1;
              _count=0; // ��� ������ �� ����������� �����
            }
        }
      if(F0xzConn)
        { // �� (x,z), y � ������ ���������
          if((setup_surf_everyrow && yi==0 && zi==0) ||
             (setup_surf_bydelay && (now-LastSurfaceOut_xz)>SurfaceOutDelayMs) )
            { LastSurfaceOut_xz=now;
              F0xz_ready=1;
              _count=0; // ��� ������ �� ����������� �����
            }
        }
    #endif
    Start=1;
    ProgressOut();
    Complete=OutReady=0;
  }
else // ������ �������
  { _count=0; // ��� ������ �� ����������� �����
    #if L57_MODELVERSION>=7
      F0xy_ready=1; // ����� ������� ������������ ���������� �� ����� - ��� ���������
      F0xz_ready=1;
      F0yz_ready=1;
    #endif
    if(deep_search) // ����������� ������ ������� ������ ������
      { Complete=OutReady=0;
        Working=1;
        Start=1;
        stage=1;
        DoStage1(TRUE); // �������������
        return;
      }
    // ������ ������� �� ����
    SetupOutputs();
  }

  _count--;
  if(_count<=0)
    break;

} // for(;;)

#if L57_MODELVERSION>=9
  ShowTime();
#endif

#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------
