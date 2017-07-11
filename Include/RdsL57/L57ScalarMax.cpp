//---------------------------------------------------------------------------
// ����������� ����� ������ ���������� �������������
// � �������� "��������" � ���������� ������������
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ���������� ��������
//---------------------------------------------------------------------------
#ifdef L57MS_GLOBALS
#ifndef L57_MODELVERSION
  #define L57_MODELVERSION 0
#endif
#include <RdsL57/L57TextBasic.cpp>

// ���������
#define TSMFSTAGE_WAIT    0       // ����� �� �������
#define TSMFSTAGE_EXT     1       // ������ ������� �������
#define TSMFSTAGE_GRID    2       // ��������� �������
#define TSMFSTAGE_PREC    3       // ��������� ���������� ����������
#define TSMFSTAGE_FINAL   4       // ��������� ������ � ������������
//---------------------------------------------------------------------------

// ������ ����� ����� (��� ��������)
class TIntArray
{ public:
    int *Arr;
    int Count,Size;

    // ��������
    void Clear(void);
    // �������
    void Allocate(int n,BOOL setcount=FALSE);
    // �������� ����������
    void SetCount(int cnt);
    // �������� ����
    void Zero(void);
    // ��������� ��� ��������
    BOOL NextStep(int icount,int step);
    // ��������� ��� �� ��������� ���������
    BOOL AllEqual(int n);

    // ������� ����� ����� �������� �� ����� ���������� (�����������)
    static int Steps(int icount,int step);
    // ��������� ������ (�����������)
    static BOOL NextIndex(int icount,int step,int &index);

    TIntArray(void){Arr=NULL;Count=Size=0;};
    ~TIntArray(){Clear();};
};
// ��������
void TIntArray::Clear(void)
{ if(Arr) delete[] Arr;
  Arr=NULL;
  Count=Size=0;
}
// �������� ����
void TIntArray::Zero(void)
{ if(Arr!=NULL)
    memset(Arr,0,Size*sizeof(int));
}
// ��������� ��� �� ��������� ���������
BOOL TIntArray::AllEqual(int n)
{ for(int i=0;i<Count;i++)
    if(Arr[i]!=n)
      return FALSE;
  return TRUE;
}
// �������� ����������
void TIntArray::SetCount(int cnt)
{
  if(cnt>=0 && cnt<Size)
    Count=cnt;
}
// �������
void TIntArray::Allocate(int n,BOOL setcount)
{ Clear();
  if(n>0)
    { Arr=new int[Size=n];
      if(setcount)
        Count=Size;
    }
}
// ��������� ��� ��������
BOOL TIntArray::NextStep(int icount,int step)
{ BOOL changed=FALSE;
  int last=icount-1;
  if(!Arr) return FALSE;

  for(int i=0;i<Count;i++)
    { if(Arr[i]==last) // ��������� �������
        Arr[i]=0;
      else // �� ��������� �������
        { Arr[i]+=step;
          changed=TRUE;
          if(Arr[i]>=icount)
            Arr[i]=last;
          break;
        }
    }
  return changed;
}
// ������� ����� ����� �������� �� ����� ���������� (�����������)
int TIntArray::Steps(int icount,int step)
{ int n;
  if(step<=1 || icount<2)
    return icount;

  // icount=10, step=5: icount/step=2, icount%step=0
  // 0123456789
  // +    +   +       3 = icount/step+1

  // icount=11, step=5: icount/step=2, icount%step=1
  // 01234567890
  // +    +    +      3 = icount/step+1

  // icount=12, step=5: icount/step=2, icount%step=2
  // 012345678901
  // +    +    ++     4 = icount/step+1

  // icount=13, step=5: icount/step=2, icount%step=3
  // 0123456789012
  // +    +    + +    4 = icount/step+2

  // icount=15, step=5: icount/step=3, icount%step=0
  // 012345678901234
  // +    +    +   +  4 = icount/step+1

  // icount=16, step=5: icount/step=3, icount%step=1
  // 0123456789012345
  // +    +    +    +  4 = icount/step+1

  // icount=17, step=5: icount/step=3, icount%step=2
  // 01234567890123456
  // +    +    +    ++  5 = icount/step+2

  n=icount/step;
  if(icount%step<=1) // ������� ��� ������� ��� � �������� 1
    return n+1;
  return n+2;
}
// ��������� ������ (�����������)
BOOL TIntArray::NextIndex(int icount,int step,int &index)
{ int last=icount-1;
  if(index==last) // ���������
    return FALSE;
  index+=step;
  if(index>=icount)
    index=last;
  return TRUE;
}
//---------------------------------------------------------------------------

// ������� �������� ������ ���������� � ���������
class TMaxCandidate
{ public:
    TMaxCandidate *Next;
    TIntArray Indices;  // ������� ���������
    double f;           // �������� �������
    int IndStep;        // ��� �� ��������
    int steps_down;     // ���������� ��������� ���������� �����
    BOOL CheckPlateau;  // ���������� �������� �� �����
    BOOL EqualNeighbours;// ��� ������ ����� (��� �������� �� �����)

    static int Count; // ������� ���������
    static void CheckMsg(int fixed=0); // �������� �� ������ �����������

    TMaxCandidate(void)
      {Next=NULL;
       f=rdsbcppHugeDouble;
       steps_down=0;
       IndStep=1;
       CheckPlateau=EqualNeighbours=FALSE;
       Count++;
      };
    ~TMaxCandidate(){Count--;};
};
//---------------------------------------------------------------------------
int TMaxCandidate::Count=0;
// �������� �� ������ ����������� ������� � ����������
void TMaxCandidate::CheckMsg(int fixed)
{
  if(Count>fixed)
    { rdsStopCalc();
      rdsMessageBox("���������� ������ - ������ ������","��������� ������������",
        MB_ICONERROR|MB_OK);
      Count=fixed;
    }
}
//---------------------------------------------------------------------------

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
    // ����������
    int Count(void);
    // ���������� ���������� � ������ �������
    void Exchange(TMaxList &list);

    TMaxList(void){First=NULL;};
    ~TMaxList(){Clear();};
};
//-----------------------------------------------------------------------

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

// ���������� ���������� � ������ �������
void TMaxList::Exchange(TMaxList &list)
{ TMaxCandidate *f=First;
  First=list.First;
  list.First=f;
}
//---------------------------------------------------------------------------


// ����� ��������� ������ ������
class TL57TextFileSMF : public TL57TextFileBasic
{ public:
    TL57TextString Code;

    // ��������� ������� ������
    void ParseHiddenSection(TL57TextFileLine *line);
    // �������� ������� ������
    BOOL WriteHiddenSection(void);
    // �������� ������ � ������� ������ �� �����
    BOOL WriteSection(int /*id*/,int *pendid);

    TL57TextFileSMF(void):TL57TextFileBasic(){};
    virtual ~TL57TextFileSMF(){};
};
//---------------------------------------------------------------------------

// ��������� ������� ������
void TL57TextFileSMF::ParseHiddenSection(TL57TextFileLine *line)
{ char *aux;

  Code.Clear();

  // ������ �������
  for(TL57TextFileLine *l=line;l!=NULL;l=l->Next)
    { char *s=l->Line;
      if(!s)
        continue;
      if((*s)=='$') // ����� ������ (������ ��������� ������)
        break;
      switch(*s)
        { case '1': // ������� �������� � ��������� ������������
            aux=rdsProcessText(s+1,RDS_PT_STRINGTOTEXT,NULL);
            if(aux)
              { if(Code.IsNotEmpty())
                  Code.AddString("\n");
                Code.AddString(aux);
                rdsFree(aux);
              }
            break;
        }

    }

}
//---------------------------------------------------------------------------

// �������� ������� ������
BOOL TL57TextFileSMF::WriteHiddenSection(void)
{ TL57TextFileBasic splitter;
  BOOL ok=TRUE;
  char *aux;

  splitter.SplitText(Code.c_str());
  for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
    if(l->Line)
      { aux=rdsProcessText(l->Line,RDS_PT_TEXTTOSTRINGSL,NULL);
        if(aux)
          { ok=ok && WriteLine("1");
            ok=ok && WriteLine(aux,1);
            rdsFree(aux);
          }
      }

  return ok;
}
//---------------------------------------------------------------------------

// �������� ������ � ������� ������ �� �����
BOOL TL57TextFileSMF::WriteSection(int /*id*/,int *pendid)
{ TL57TextFileBasic splitter;
  BOOL ok=TRUE;

  *pendid=2;
  splitter.SplitText(Code.c_str());
  for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
    if(l->Line)
      { ok=ok && WriteLine("  ");
        ok=ok && WriteLine(l->Line,1);
      }
   ok=ok && WriteLine("  ; /* */",1);

   return ok;
}
//---------------------------------------------------------------------------

// ������ �������� �������
class TL57TextSMFFuncArg
{ public:
    double _Crit;
    int Count;
    TL57TextDoubleArray X_Params;
    TL57TextDoubleArray Y_Params;
    TL57TextDoubleArray A_Params;
    TL57TextFuncFArray Functions;

    TL57TextSMFFuncArg(void){_Crit=rdsbcppHugeDouble;Count=0;};
};
//---------------------------------------------------------------------------

// �������� ������� ���������� ��������
double _CritFunc(const TL57TextSMFFuncArg &arguments);
//-----------------------------------------------------------------------

// ������� ���������� �������� � ���������� ����������
double CritFunc(const TL57TextSMFFuncArg &arguments)
{ double crit=rdsbcppHugeDouble;

  RDSBCPP_TRY
    { crit=_CritFunc(arguments); }
  RDSBCPP_CATCHALL
    { crit=rdsbcppHugeDouble; }
  return crit;
}
//-----------------------------------------------------------------------

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

#endif // #ifdef L57MS_GLOBALS
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// ���������
//---------------------------------------------------------------------------
#ifdef L57MS_SETUP
TL57TextFileSMF file;
RDS_HOBJECT win;
BOOL res;
char caption[]="��������� ������������";
TL57TextFileBasic_ServMarker markers[]=
  {{"L57_SCALARMAX_FUNC_BEGIN_MARKER", 1,-1},
   {"L57_SCALARMAX_FUNC_END_MARKER", 2,-1},
   {NULL,0,-1}
  };

RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

if(model==NULL)
  { rdsMessageBox("��� ����� ��������� �������������� ���������� ������. ��������� ������� ��� "
                  "���� ����������.",caption,MB_OK|MB_ICONWARNING);
    return;
  }

if(rdscompIsEditorOpen(model->Model))
  { rdsMessageBox("��� ��������� ����� ����� ���������� ������� �������� ��� ������. "
                  "����������, �������� ��������.",caption,MB_OK|MB_ICONWARNING);
    rdscompOpenBlockModelEditor(rdsbcppBlockData->Block);
    return;
  }

win=rdsFORMCreate(TRUE,600,-1,caption);
if(!win)
  return;

// ������ �������
file.ReadAndParseModel(rdsbcppBlockData->Block,markers,TRUE);

rdsFORMAddTab(win,1,"�������");
rdsFORMAddEdit(win,1,1,RDS_FORMCTRL_MULTILINE,
  "�������� ����� ��������������� �������:",80);
rdsSetObjectInt(win,1,RDS_FORMVAL_MLRETURNS,1);
rdsFORMSetMultilineHeight(win,1,4*24);
rdsSetObjectStr(win,1,RDS_FORMVAL_VALUE,file.Code.c_str());

rdsFORMAddEdit(win,1,100,RDS_FORMCTRL_MULTILABEL,
"���������� ������ ��������(�) ���������� ��������������� ������� � ���������� ����� C. "
"������� ��������� ������� - ����� ������� \"N\", ����� ������� \"X[0]\"...\"X[N-1]\", "
"������ ������� \"Y[0]\"...\"Y[N-1]\". ����������� �������� ������� ���� ������������� ���������� \"f\","
"���� ������������ ���������� return. ���� ��������� ���������� �����������, ����� ���� "
"��������� \"f\" �������� \"rdsbcppHugeDouble\", ���� ������� ��� �� �������� ���������� return.",0);
rdsSetObjectInt(win,100,RDS_FORMVAL_MLHEIGHT,110);
rdsSetObjectInt(win,100,RDS_FORMVAL_MLABELBEVEL,1);

rdsFORMAddTab(win,2,"���������");
rdsFORMAddEdit(win,2,1000,RDS_FORMCTRL_CHECKBOX,"������ ������ �� ������� \"Start\"",80);
rdsSetObjectInt(win,1000,RDS_FORMVAL_VALUE,setup_startbycmd?1:0);

rdsFORMAddEdit(win,2,1001,RDS_FORMCTRL_CHECKBOX,"������� ������ ����� �� \"�����\"",80);
rdsSetObjectInt(win,1001,RDS_FORMVAL_VALUE,setup_no_plateau?1:0);

#if L57_MODELVERSION>=3
  rdsFORMAddEdit(win,2,1006,RDS_FORMCTRL_CHECKBOX,"��������� ������ - � ������������ ����������",80);
  rdsSetObjectInt(win,1006,RDS_FORMVAL_VALUE,setup_finalrun?1:0);
#endif


rdsFORMAddEdit(win,2,1002,RDS_FORMCTRL_EDIT
#if L57_MODELVERSION>=1
  |RDS_FORMFLAG_LINE
#endif
,"����������� ������������ ����������:",80);
rdsSetObjectDouble(win,1002,RDS_FORMVAL_VALUE,setup_df_equal);

#if L57_MODELVERSION>=1
rdsFORMAddEdit(win,2,1003,RDS_FORMCTRL_LABEL,"�������� ���������� ������� �������:",80);
rdsFORMAddEdit(win,2,1004,RDS_FORMCTRL_UPDOWN|RDS_FORMFLAG_CHECK|RDS_FORMFLAG_CHECKRADIO,"�������� ����� ������:",80);
rdsSetObjectInt(win,1004,RDS_FORMVAL_VALUE,setup_waitcycles);
rdsSetObjectInt(win,1004,RDS_FORMVAL_UPDOWNMIN,0);
rdsSetObjectInt(win,1004,RDS_FORMVAL_UPDOWNMAX,30000);
rdsSetObjectInt(win,1004,RDS_FORMVAL_UPDOWNINC,1);

rdsFORMAddEdit(win,2,1005,RDS_FORMCTRL_RADIOBUTTON,"���������� ����� ������ �������������",80);
rdsSetObjectInt(win,1005,RDS_FORMVAL_VALUE,setup_autowait?1:0);
#endif

#if L57_MODELVERSION>=4
  // ������� ������ �����������
  rdsFORMAddTab(win,3,"�����������");

  rdsFORMAddEdit(win,3,3002,RDS_FORMCTRL_MULTILABEL|RDS_FORMFLAG_LINE,
                     "������ ���� ���������� �� ������ \"Surface\":",0);
  rdsSetObjectInt(win,3002,RDS_FORMVAL_MLHEIGHT,20);
  rdsSetObjectInt(win,3002,RDS_FORMVAL_MLABELBEVEL,0);

  rdsFORMAddEdit(win,3,3000,RDS_FORMCTRL_CHECKBOX,"�������� ����� ������� ������� ������� ���������",80);
  rdsSetObjectInt(win,3000,RDS_FORMVAL_VALUE,setup_surf_everyrow?1:0);

  rdsFORMAddEdit(win,3,3001,RDS_FORMCTRL_EDIT|RDS_FORMFLAG_CHECK,"����������� �������� ������, ���.:",80);
  rdsSetObjectDouble(win,3001,RDS_FORMVAL_VALUE,setup_surf_delayval);
  rdsSetObjectInt(win,3001,RDS_FORMVAL_CHECK,setup_surf_bydelay?1:0);
#endif

res=rdsFORMShowModalServ(win,NULL);

if(res)
  { char *in_code=rdsGetObjectStr(win,1,RDS_FORMVAL_VALUE);
    file.Code.Assign(in_code);
    file.SaveFileToModel(rdsbcppBlockData->Block,L57_BAK);

    setup_startbycmd=(rdsGetObjectInt(win,1000,RDS_FORMVAL_VALUE)!=0);
    setup_no_plateau=(rdsGetObjectInt(win,1001,RDS_FORMVAL_VALUE)!=0);
    setup_df_equal=rdsGetObjectDouble(win,1002,RDS_FORMVAL_VALUE);
    
    #if L57_MODELVERSION>=1
      setup_autowait=(rdsGetObjectInt(win,1005,RDS_FORMVAL_VALUE)!=0);
      setup_waitcycles=rdsGetObjectInt(win,1004,RDS_FORMVAL_VALUE);
    #endif

    #if L57_MODELVERSION>=3
      setup_finalrun=(rdsGetObjectInt(win,1006,RDS_FORMVAL_VALUE)!=0);
    #endif

    #if L57_MODELVERSION>=4
      setup_surf_everyrow=(rdsGetObjectInt(win,3000,RDS_FORMVAL_VALUE)!=0);
      setup_surf_delayval=rdsGetObjectDouble(win,3001,RDS_FORMVAL_VALUE);
      setup_surf_bydelay=(rdsGetObjectInt(win,3001,RDS_FORMVAL_CHECK)!=0);
    #endif
  }

rdsDeleteObject(win);
Result=res?1:0;

#endif // #ifdef L57MS_SETUP
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------
#ifdef L57MS_INIT
ResetProgress();
TotalBlocks=0;
#endif // #ifdef L57MS_INIT
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// �������� � ������
//---------------------------------------------------------------------------
#ifdef L57MS_CLASS
TL57TextSMFFuncArg Arguments; // ��������� ����������������� �������

int TotalBlocks;        // ����� ������� ������ � �����

TIntArray Indices;      // ������� ��� ���������� �������� (������ TSMFSTAGE_GRID)
int IndStep;            // ������� ��� �� ��������
TMaxList Candidates;    // ������ ���������� � ���������
TMaxList FinalMax;      // ������ ��������� ����������

// �������� �� �������
double progress1_max,progress1_cur;
double progress2_max,progress2_cur;
double progress3_max,progress3_cur;

#if L57_MODELVERSION>=3
  void SetPArray(void); // ��������� ���������� �������� � P[]
  void FinalRunStage(BOOL init); // ��������� ������
#endif

#if L57_MODELVERSION>=4
  // ������� ����� � ������ ����������� (����������� �������������)
  BOOL SurfaceConn;
  // ����� �� ������� �����������
  BOOL CanDrawSurface;

  DWORD LastSurfaceOut;        // ����� ���������� ���������� ������ �����������
  DWORD SurfaceOutDelayMs;     // �������� � ��

  // ���������
  int SurfaceArgument1,SurfaceArgument2; // ������� ����������

  // ��������� � ���������� ������� ������������
  BOOL SetupSurfaceArray(int indstep);
#endif

#if L57_MODELVERSION>=5
  // �����
  DWORD dwStartTime;
  // ��������� �����
  void ShowTime(void)
    {
      if(Stage!=TSMFSTAGE_WAIT)
        Time=((double)(GetTickCount()-dwStartTime))/1000.0;
    };
#endif

// ��������� �������� �������� ��������� ������� ���������� ��������
inline int CountDownInit(void)
{
  #if L57_MODELVERSION>=1
    return setup_autowait?(TotalBlocks+5):(setup_waitcycles+5);
  #else
    return 10; /*CritReadyAuto?(TotalBlocks+5):(CritReadyCycles+5);*/
  #endif
};

// ����� ������� �������� ����������
void Reset_X_Ready(void)
{ for(int i=0;i<X_Ready.Size();i++)
    X_Ready[i]=0;
  X_chg=0;
};
// ���������, ������ �� ������ �������
BOOL __fastcall IsXReady(void);

inline void ResetProgress(void)
{ progress1_max=progress2_max=progress3_max=1.0;
  progress1_cur=progress2_cur=progress3_cur=0.0;
};

// �������� �� ������� (� ����������)
double ParByIndex(int index);
// �������� ������ �� ������� ���������
double ValueByIndex(int par_index,int ag_index,double &par);


// ������������� ��������
BOOL StartIterations(void);
// ��������� ������ �� ������
void SetErrorOutput(void);
// ��������� ������
void SetOutput(BOOL setmatr=TRUE);
// ��������� ������ - ��������� (������ �������� �������, ��� ����������)
void SetOutputServ(void);
// ����� �������
int RealNAgents(void);
// ���� ������� ������ ������ �������
void DoExtStage(void);
// �������� ��������
void ShowProgress(void);
// ���� ������� ������ ��������
void DoGridStage(void);
// ����������� �������� � ������� (�������� ��� �������� �� ����������)
void StoreFinalMax(TMaxCandidate *fnew);
// ��������� ������ ��������� �� ������ ���������
// �����. TRUE, ���� � ���� ���������� - ���
BOOL PrecStageProcess(TMaxCandidate *f,TIntArray &AuxIndices,TIntArray &AuxIndices2,const int &n_ag);
// ���� ������� ������ ���������
void DoPrecStage(void);

#endif // #ifdef L57MS_CLASS
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------
#ifdef L57MS_MODEL

BOOL enabled=TRUE;

// ������� ��������� ������ ���� �������
Ready=p_ready=Resp_ready=Finished=OutReady=0;
#if L57_MODELVERSION>=1
  p_go=0;
#endif

// �� ���� �� ������ ������?
if(CountingDownRestart && Stage!=TSMFSTAGE_WAIT)
{ RestartCountdown--;
  CountingDownCrit=0;
  if(RestartCountdown>0)
    { _Start=1;
      return;
    }
  // ���������
}

// �������� ���������� ������
Enabled_chg=0;
for(int i=0;enabled && i<Enabled.Size();i++)
  if(!Enabled[i])
    enabled=FALSE;
if(!enabled)
  return;

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
      { //test=GetTickCount();
        Arguments.Functions.Realloc(n);
        for(int i=0;i<n;i++)
          Arguments.Functions[i].Setup(&(FS[i]));
        Restart=1;
      }
  }

// ������ ��������
if(a_chg)
  { // ��������� �������� ��������� �������
    BOOL changed=FALSE;
    a_chg=0;
    if(A.Size()!=A_old.Size()) // ��������� ������
      changed=TRUE;
    else // ������ ��� ��
      for(int i=0;i<A.Size();i++)
        if(A[i]!=A_old[i])
          { changed=TRUE;
            break;
          }
    if(changed)
      { A_old=A;
        Restart=1;
      }
  }

if(A.IsEmpty())
  Arguments.A_Params.Setup(NULL);
else
  Arguments.A_Params.Setup(A.GetBuffer(),A.Size());

// ���������
if(diap_chg)
  { // ��������� ����� ���������
    diap_chg=0;
    if(pmin!=pmin_old || pmax!=pmax_old || dp_big!=dp1 || dp_small!=dp2 || N_Agents_old!=RealNAgents())
      { pmin_old=pmin;
        pmax_old=pmax;
        dp1=dp_big;
        dp2=dp_small;
        N_Agents_old=RealNAgents();
        Restart=1;
      }
  }

if(setup_startbycmd && Stage==TSMFSTAGE_WAIT)
  { // ������ �� �������
    if(!Start)
      return;
    Start=0;
  }
else
  Start=0; // Start ����� - �� ����������� ������ ������� ������, � ���������� ���� �������

if(Restart) // ����� �������������
  { Restart=0;
    CountingDownCrit=0;
    Reset_X_Ready();
    CountingDownRestart=1;
    RestartCountdown=CountDownInit();
    Stage=TSMFSTAGE_EXT; // �������� � ��������
    _Start=1;
    #if L57_MODELVERSION>=5
      // ����� � ������ ������
      Started=1;
      dwStartTime=GetTickCount();
      Time=0.0;
    #endif
    return;
  }

if(Stage==TSMFSTAGE_WAIT)
  { // ������������� �� ����, �� � �� ��������
    Finished=Ready=1;
    return;
  }

if(CountingDownRestart/*Restart*/) // ������ � �����������
  { Restart=0;
    CountingDownRestart=0;
    CountingDownCrit=0;
    if(!StartIterations())
      return;
    // ��� ����������������, ����������� ��������� ���������
    p_ready=Ready=1;
    Reset_X_Ready();
    #if L57_MODELVERSION>=1
      p_go=1;
    #endif
    return;
  }

switch(Stage)
  { case TSMFSTAGE_EXT:
      DoExtStage();
      break;
    case TSMFSTAGE_GRID:
      DoGridStage();
      break;
    case TSMFSTAGE_PREC:
      DoPrecStage();
      break;
    #if L57_MODELVERSION>=3
      case TSMFSTAGE_FINAL:
        FinalRunStage(FALSE);
        break;
    #endif
    default: // ������
      rdsStopCalc();
      rdsMessageBox("���������� ������ ���������� ������������� - ����������� ����� ������",
                    rdsbcppBlockData->BlockName,MB_ICONERROR|MB_OK);
      return;
  }

if(Complete==0 && Error==0)
  { ShowProgress();
    Ready=1;
  }
#if L57_MODELVERSION>=5
  ShowTime();
#endif

#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// ����� �������������
//---------------------------------------------------------------------------
#ifdef L57MS_CALCMODE

TotalBlocks=rdsCountBlocks(/*BlockData->Parent*/rdsGetRootSystem(NULL),RDS_BTSIMPLEBLOCK,TRUE);
#if L57_MODELVERSION>=4
  SurfaceConn=TL57TextFileBasic::CheckConnectionForBlockVar(rdsbcppBlockData->Block,"Surface");
#endif

#endif // #ifdef L57MS_CALCMODE
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ �������
//---------------------------------------------------------------------------
#ifdef L57MS_START
if(rdsCalcProcessNeverStarted())
  { ResetProgress();
    Progress=0.0;
  }
#endif // #ifdef L57MS_START
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// �������� ����� ������
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS

#if L57_MODELVERSION>=4
// ��������� � ���������� ������� ������������
BOOL rdsbcppBlockClass::SetupSurfaceArray(int indstep)
{
  if(CanDrawSurface)
    { int smallpoints=Resp.Rows();
      int bigpoints=TIntArray::Steps(smallpoints,indstep);
      int index,lastsmallpoint=smallpoints-1;
      if(!Surface.Resize(bigpoints+1,bigpoints+1))
        { rdsStopCalc();
          SetErrorOutput();
          rdsMessageBox("������ ��������� ������ ��� ������� �����������",
                        rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
          return FALSE;
        }
      for(int i=0;i<Surface.Size();i++)
        *(Surface.GetBuffer()+i)=rdsbcppHugeDouble;
      // ���������� ����
      index=0;
      for(;;)
        { double v;
          int n=(index==lastsmallpoint)?bigpoints:(index/indstep+1);
          if(n>=Surface.Cols())
            { rdsStopCalc();
              SetErrorOutput();
              rdsMessageBox("���������� ������ ��������� � ������� �����������",
                            rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
              return FALSE;
            }
          Surface[0][n]=v=Resp[index][0];
          Surface[n][0]=v;
          if(!TIntArray::NextIndex(smallpoints,indstep,index))
            break;
          n++;
        }
    }
  return TRUE;
}
//---------------------------------------------------
#endif

// ��������� ������ �� ������
void rdsbcppBlockClass::SetErrorOutput(void)
{
  //Resp.Resize(0,0);
  Resp_ready=1;
  Error=1;
  Complete=0;
  Finished=1;
  OutReady=1;
  Ready=1;
  #if L57_MODELVERSION>=5
    ShowTime();
  #endif
  Stage=TSMFSTAGE_WAIT;
  Progress=0.0;
  Candidates.Clear();
  FinalMax.Clear();
  AllMax.Resize(0,0);
  TMaxCandidate::CheckMsg();
  return;
}
//---------------------------------------------------------------------------

// ��������� ������ - ��������� (������ �������� �������, ��� ����������)
void rdsbcppBlockClass::SetOutputServ(void)
{ int nmax=FinalMax.Count(),
      n_ag=RealNAgents();
  //            rows cols
  AllMax.Resize(nmax,n_ag+1,FALSE);
  if(nmax!=0 && n_ag!=0)
    { int nn=0;
      for(TMaxCandidate *c=FinalMax.First;c!=NULL;c=c->Next)
        { AllMax[nn][n_ag]=c->f;
          for(int i=0;i<n_ag;i++)
            if(i<c->Indices.Count)
              AllMax[nn][i]=ParByIndex(c->Indices.Arr[i]);
          nn++;
        }
      #if L57_MODELVERSION>=2
        MaxSoFar.Resize(n_ag+1);
        for(int i=0;i<=n_ag;i++)
          MaxSoFar[i]=AllMax[0][i];
      #endif
    }
}
//---------------------------------------------------------------------------

// ��������� ������
void rdsbcppBlockClass::SetOutput(BOOL setmatr)
{ 
  Resp_ready=1;
  Error=0;
  Complete=1;
  Finished=1;
  OutReady=1;
  Ready=1;
  #if L57_MODELVERSION>=5
    ShowTime();
  #endif
  Stage=TSMFSTAGE_WAIT;
  Progress=1.0;

      /*
      // ���������� ����������
      TL57TextString s,s1;
      rdsStopCalc();
      int nn=0;
      for(TMaxCandidate *c=FinalMax.First;c!=NULL;c=c->Next)
        { s1.SetInt(nn++);
          s.AddString(s1.c_str());
          s.AddString(": [");
          for(int i=0;i<c->Indices.Count;i++)
            { if(i) s.AddString(", ");
              s1.SetDouble(ParByIndex(c->Indices.Arr[i]));
              s.AddString(s1.c_str());
            }
          s.AddString("]\n");
          if(nn>20)
            { s.AddString("...");
              break;
            }
        }
      rdsMessageBox(s.c_str(),"",MB_OK);
      */
  if(setmatr)
    SetOutputServ();

  Candidates.Clear();
  FinalMax.Clear();
  TMaxCandidate::CheckMsg();
  return;
}
//---------------------------------------------------------------------------

// ����� �������
int rdsbcppBlockClass::RealNAgents(void)
{ int n=X.Size();
  #if L57_MODELVERSION>=3
    int n1=P.Size();
    if(n1>n)
      n=n1;
  #endif

  if(n>N_Agents)
    return n;
  return N_Agents;
}
//---------------------------------------------------------------------------

#if L57_MODELVERSION>=3
  // ��������� ���������� �������� � P[]
  void rdsbcppBlockClass::SetPArray(void)
  { int np=P.Size();
    for(int i=0;i<np;i++)
      P[i]=p;
  }
#endif
//---------------------------------------------------------------------------

// ������������� ��������
BOOL rdsbcppBlockClass::StartIterations(void)
{ int p_cnt=CalcNPointsByDiap(pmin,pmax,dp_small);
  int n_ag=RealNAgents();
  BOOL rok;
  double _Xmin=pmin,_Xmax=pmax,Dx=dp_small;
  #if L57_MODELVERSION>=4
    int argcount=0;
  #endif

  ResetProgress();
  Progress=0.0;
  Candidates.Clear();
  FinalMax.Clear();
  Complete=Error=0;

  #if L57_MODELVERSION>=4
    Surface.Resize(0,0,FALSE);
    SurfaceReady=1;
    CanDrawSurface=FALSE;
    if(SurfaceConn)
      { SurfaceArgument1=SurfaceArgument2=-1;
        for(int i=0;i<FlagArg.Size() && i<n_ag && argcount<2;i++)
          if(FlagArg[i])
            { if(argcount==0) SurfaceArgument1=i;
              else SurfaceArgument2=i;
              argcount++;
            }
        CanDrawSurface=(argcount==2);
      }
    SurfaceOutDelayMs=(DWORD)(setup_surf_delayval*1000);
    LastSurfaceOut=GetTickCount();
  #endif

  if(p_cnt<1 || pmin==rdsbcppHugeDouble || n_ag<1)
    { SetErrorOutput();
      return FALSE;
    }
  if(p_cnt==1)
    { _Xmax=_Xmin; Dx=1.0; }

  //              rows  cols
  rok=Resp.Resize(p_cnt,n_ag+1,FALSE);

  #if L57_MODELVERSION>=2
    rok=rok && MaxSoFar.Resize(n_ag+1);
    for(int i=0;i<=n_ag;i++)
      MaxSoFar[i]=rdsbcppHugeDouble;
  #endif

  #if L57_MODELVERSION>=3
    if(P.Size()!=n_ag)
      rok=rok && P.Resize(n_ag,FALSE);
  #endif

  if(!rok)
    { rdsStopCalc();
      SetErrorOutput();
      rdsMessageBox("������� ������� ��������� ����, �� ������� ������ ��� "
                    "���������� ������ ������ �������.",
                    rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
      return FALSE;
    }

  for(int i=0;i<p_cnt;i++)
    { double v=vInDiap(_Xmin,_Xmax,Dx,i,p_cnt);
      Resp[i][0]=v;
    }
  progress1_cur=0.0;
  progress1_max=p_cnt;

  x_index=0;
  p=Resp[x_index][0];
  #if L57_MODELVERSION>=3
    SetPArray();
  #endif
  p_ready=Ready=1;
  Reset_X_Ready();
  #if L57_MODELVERSION>=1
    p_go=1;
  #endif
  Stage=TSMFSTAGE_EXT;

  return TRUE;
}
//---------------------------------------------------------------------------


// ���������, ������ �� ������ �������
BOOL __fastcall rdsbcppBlockClass::IsXReady(void)
{

  if(X_chg)
    { BOOL ready=TRUE;
      for(int i=0;ready && i<X_Ready.Size();i++)
        if(X_Ready[i]==0)
          ready=FALSE;

      if(ready) // ������ ������� ����������
        { //Reset_X_Ready();
          // ��������� �������� ������ ������
          XReadyCountDown=CountDownInit(); //TotalBlocks+5;
          CountingDownCrit=1;
          X_old=X;
          _Start=1;
          X_chg=0;
        }
      return FALSE;
    }

  if(CountingDownCrit) // �������� (�������) - ���������� �������� ��� ����������
    { BOOL valchanged=(X.Size()!=X_old.Size());
      if(!valchanged)
         { for(int i=0;i<X.Size();i++)
              if(X[i]!=X_old[i])
                 { valchanged=TRUE;
                   break;
                 }
         }
      if(valchanged) // ���� �����, �������� �������� ���������� - ������������� �������
        { XReadyCountDown=CountDownInit(); // TotalBlocks+5;
          _Start=1;
          X_old=X;
          return FALSE;
        }
      // �������� �������� �� �������� � ������� ��������
      XReadyCountDown--;
      if(XReadyCountDown>0)
        { _Start=1;
          return FALSE;
        }
      // �������� ������ ��������
      CountingDownCrit=0;
      Reset_X_Ready();
      return TRUE;
    }

/*
  if(X_chg)
    { BOOL ready=TRUE;
      for(int i=0;ready && i<X_Ready.Size();i++)
        if(X_Ready[i]==0)
          ready=FALSE;

      if(ready) // ������ ������� ����������
        { Reset_X_Ready();
          // ��������� �������� ������ ������
          XReadyCountDown=CountDownInit(); //TotalBlocks+5;
          CountingDownCrit=1;
          X_old=X;
          _Start=1;
          X_chg=0;
        }
      return FALSE;
    }
*/

  // �� ����������, �� ��������
  return FALSE;
}
//---------------------------------------------------------------------------

// �������� ��������
void rdsbcppBlockClass::ShowProgress(void)
{ double pp;
  int n=1;
  RDSBCPP_TRY
    { pp=progress1_cur/progress1_max;
      if(progress2_max!=rdsbcppHugeDouble)
        { pp+=progress2_cur/progress2_max;
          n++;
        }
      if(progress3_max!=rdsbcppHugeDouble)
        { pp+=progress3_cur/progress3_max;
          n++;
        }
    }
  RDSBCPP_CATCHALL
    { pp=0.0; }
  Progress=pp/n;
}
//---------------------------------------------------------------------------

// ���� ������� ������ ������ �������
void rdsbcppBlockClass::DoExtStage(void)
{ int n_ag,cols;
  #if L57_MODELVERSION>=2
    BOOL bad=false;
  #endif

  if(!IsXReady())
    return;
  progress1_cur+=1.0;

  // ����������
  if(x_index<0 || x_index>=Resp.Rows())
    { rdsStopCalc();
      SetErrorOutput();
      rdsMessageBox("���������� ������ ���������� �������������: �������� ������.",
                    rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
      return;
    }

  n_ag=RealNAgents();
  cols=X.Size();
  if(n_ag>cols)
    n_ag=cols;
  cols=Resp.Cols()-1;
  if(n_ag>cols)
    n_ag=cols;
  for(int i=0;i<n_ag;i++)
    Resp[x_index][i+1]=X[i];

  #if L57_MODELVERSION>=2
    // ������� �������� ��� ���������
    Arguments.Count=n_ag;
    if(XX.Size()!=n_ag)
      XX.Resize(n_ag);
    if(YY.Size()!=n_ag)
      YY.Resize(n_ag);
    if(XX.IsEmpty())
      Arguments.X_Params.Setup(NULL);
    else
      Arguments.X_Params.Setup(XX.GetBuffer(),XX.Size());
    if(YY.IsEmpty())
      Arguments.Y_Params.Setup(NULL);
    else
      Arguments.Y_Params.Setup(YY.GetBuffer(),YY.Size());

    for(int i=0;i<n_ag;i++)
      { double v=X[i];
        if(v==rdsbcppHugeDouble)
          { bad=TRUE;
            break;
          }
        XX[i]=p;
        YY[i]=v;
      }
    if(!bad)
      { double fff=CritFunc(Arguments);
        double cur=MaxSoFar[n_ag];
        if(fff!=rdsbcppHugeDouble && (cur==rdsbcppHugeDouble || fff>cur))
          { // ����� ��������
            MaxSoFar[n_ag]=fff;
            for(int i=0;i<n_ag;i++)
              MaxSoFar[i]=p;
          }
      }
  #endif // L57_MODELVERSION>=2

  // ��������� ������
  x_index++;
  if(x_index>=Resp.Rows())
    { // ��������� - ��������� � ������ �������� (TSMFSTAGE_GRID)
      Indices.Allocate(n_ag,TRUE);
      Indices.Zero();
      // ��������� ��� �� ��������
      if(dp_big==rdsbcppHugeDouble || dp_big<=dp_small || Resp.Rows()<2)
        IndStep=1; // ��� �������� ����
      else
        {
          RDSBCPP_TRY
            { IndStep=(int)floor(dp_big/dp_small); }
          RDSBCPP_CATCHALL
            { IndStep=1; }
        }

      Stage=TSMFSTAGE_GRID;
      progress1_cur=progress1_max;
      progress2_cur=0.0;
      RDSBCPP_TRY
        { progress2_max=pow(Resp.Rows(),n_ag);
        }
      RDSBCPP_CATCHALL
        { progress2_max=rdsbcppHugeDouble; }
      _Start=1;
      Ready=1;

      #if L57_MODELVERSION>=4
        if(CanDrawSurface)
          { // ������� ������� �����������
            if(!SetupSurfaceArray(IndStep))
              return;
            SurfaceReady=1; // ����� ��������� ������ �������
          }
      #endif

      //SetOutput();
      return;
    }
  p=Resp[x_index][0];
  #if L57_MODELVERSION>=3
    SetPArray();
  #endif
  p_ready=1;
  Reset_X_Ready();
  #if L57_MODELVERSION>=1
    p_go=1;
  #endif

  Ready=1;
}
//-----------------------------------------------------------------------

// �������� �� ������� (� ����������)
double rdsbcppBlockClass::ParByIndex(int index)
{
  if(index<0 || index>=Resp.Rows())
    return rdsbcppHugeDouble;
  return Resp[index][0];
}
//-----------------------------------------------------------------------

// �������� ������ �� ������� ���������
double rdsbcppBlockClass::ValueByIndex(int par_index,int ag_index,double &par)
{
  if(par_index<0 || par_index>=Resp.Rows())
    return rdsbcppHugeDouble;
  if(ag_index<0 || ag_index>=Resp.Cols()-1)
    return rdsbcppHugeDouble;
  par=Resp[par_index][0];
  return Resp[par_index][ag_index+1];
}
//-----------------------------------------------------------------------

// ���� ������� ������ ��������
void rdsbcppBlockClass::DoGridStage(void)
{ int _count=1000,n_ag,cols;
  int icount=Resp.Rows();
  TIntArray AuxIndices;
  BOOL finished=FALSE;


  n_ag=RealNAgents();
  cols=X.Size();
  if(n_ag>cols)
    n_ag=cols;
  cols=Resp.Cols()-1;
  if(n_ag>cols)
    n_ag=cols;
  if(n_ag>Indices.Count)
    n_ag=Indices.Count;
  Arguments.Count=n_ag;
  XX.Resize(n_ag);
  YY.Resize(n_ag);

  if(XX.IsEmpty())
    Arguments.X_Params.Setup(NULL);
  else
    Arguments.X_Params.Setup(XX.GetBuffer(),XX.Size());
  if(YY.IsEmpty())
    Arguments.Y_Params.Setup(NULL);
  else
    Arguments.Y_Params.Setup(YY.GetBuffer(),YY.Size());

  AuxIndices.Allocate(n_ag,TRUE);

  for(;;)
    { double fval,fn;
      BOOL bad;

      progress2_cur+=1.0;

      // ������������ ����� � ��������� Indices - ��������� �� �� ��������� ��������� ��������
      bad=FALSE;
      for(int i=0;i<n_ag;i++)
        { int parindex=Indices.Arr[i];
          double p;
          double v=ValueByIndex(parindex,i,p);
          if(v==rdsbcppHugeDouble)
            { bad=TRUE;
              break;
            }
          XX[i]=p;
          YY[i]=v;
        }
      /*
      double x0=(Arguments.X_Params)[0],
             x1=(Arguments.X_Params)[1];
      */
      fval=bad?rdsbcppHugeDouble:CritFunc(Arguments);

      #if L57_MODELVERSION>=4
        // �����������
        if(CanDrawSurface)
          { BOOL oktosave=TRUE;
            int last=icount-1;
            if(SurfaceArgument1>=n_ag || SurfaceArgument2>=n_ag)
              oktosave=FALSE;
            for(int i=0;oktosave && i<n_ag;i++)
              if(i!=SurfaceArgument1 && i!=SurfaceArgument2)
                { // �� "������" �������� ������ ���� ������ ���������
                  int parindex=Indices.Arr[i];
                  if(parindex!=0)
                    oktosave=FALSE;
                }
            if(oktosave) // ��������� � Surface
              { int parindex1=Indices.Arr[SurfaceArgument1], // �������
                    parindex2=Indices.Arr[SurfaceArgument2]; // ������
                int col=(parindex2==last)?(Surface.Cols()-1):(parindex2/IndStep+1),
                    row=(parindex1==last)?(Surface.Cols()-1):(parindex1/IndStep+1);
                DWORD now=GetTickCount();
                Surface[row][col]=fval;
                if((setup_surf_everyrow && parindex1==last) ||
                   (setup_surf_bydelay && (now-LastSurfaceOut)>SurfaceOutDelayMs) )
                  { SurfaceReady=1;
                    LastSurfaceOut=now;
                    _count=0; // ��� ������ �� ����������� �����
                  }
              }
          }
      #endif

      if(fval!=rdsbcppHugeDouble) // ����� ����� ���������������
        { BOOL highneighbour;
          #if L57_MODELVERSION>=2
            // ������� �������� ��� ���������
            double cur=MaxSoFar[n_ag];
            if(cur==rdsbcppHugeDouble || fval>cur)
              { // ����� ��������
                MaxSoFar[n_ag]=fval;
                for(int i=0;i<n_ag;i++)
                  MaxSoFar[i]=XX[i];
              }
          #endif

          // ����������� �������
          highneighbour=FALSE;
          AuxIndices.Zero();
          while(!highneighbour)
            { if(!AuxIndices.AllEqual(1)) // ��� ==1 - ��� ������� �����, � �� �����
                { bad=FALSE;
                  for(int i=0;i<n_ag;i++)
                    { double p;
                      int parindex=Indices.Arr[i]+(AuxIndices.Arr[i]-1)*IndStep;
                      double v=ValueByIndex(parindex,i,p);
                      if(v==rdsbcppHugeDouble)
                        { bad=TRUE;
                          break;
                        }
                      XX[i]=p;
                      YY[i]=v;
                    }
                  fn=bad?rdsbcppHugeDouble:CritFunc(Arguments);
                  if(fn!=rdsbcppHugeDouble && fn>fval)
                    highneighbour=TRUE;
                }
              if(!AuxIndices.NextStep(3,1))
                break;
            } // while(!highneighbour)
          if(!highneighbour)
            { // � ����� Indices ��� ������� ����
              TMaxCandidate *c=Candidates.Add();
              c->f=fval;
              c->Indices.Allocate(n_ag,TRUE);
              c->IndStep=IndStep/2;
              if(c->IndStep<1)
                c->IndStep=1;
              c->CheckPlateau=setup_no_plateau;
              for(int i=0;i<n_ag;i++)
                c->Indices.Arr[i]=Indices.Arr[i];
            }
        } // if(fval!=rdsbcppHugeDouble)
      if(!Indices.NextStep(icount,IndStep))
        { finished=TRUE;
          break;
        }
      _count--;
      if(_count<=0)
        break;
    } // for(;;)

  if(finished)
    { int total;
      // ��������� ������
      Ready=1;
      #if L57_MODELVERSION>=4
        SurfaceReady=1;
      #endif
      //Resp_ready=1; // �������!
      //SetOutput();

      /*
      // ���������� ����������
      TL57TextString s,s1;
      rdsStopCalc();
      int nn=0;
      for(TMaxCandidate *c=Candidates.First;c!=NULL;c=c->Next)
        { s1.SetInt(nn++);
          s.AddString(s1.c_str());
          s.AddString(": [");
          for(int i=0;i<c->Indices.Count;i++)
            { if(i) s.AddString(", ");
              s1.SetDouble(ParByIndex(c->Indices.Arr[i]));
              s.AddString(s1.c_str());
            }
          s.AddString("]\n");
          if(nn>20)
            { s.AddString("...");
              break;
            }
        }
      rdsMessageBox(s.c_str(),"",MB_OK);
      */

      Stage=TSMFSTAGE_PREC;
      progress2_cur=progress2_max;
      progress3_cur=0.0;
      progress3_max=total=Candidates.Count();
      if(total==0) // ��� ���������� � ���������, ��� �������
        progress3_max=rdsbcppHugeDouble;
      _Start=1;
      return;
    }

  // ������� ��� ���������
  Ready=_Start=1;
}
//-----------------------------------------------------------------------

// ����������� �������� � ������� (�������� ��� �������� �� ����������)
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

// ��������� ������ ��������� �� ������ ���������
// �����. TRUE, ���� � ���� ���������� - ���
BOOL rdsbcppBlockClass::PrecStageProcess(TMaxCandidate *f,TIntArray &AuxIndices,TIntArray &AuxIndices2,const int &n_ag)
{

  f->EqualNeighbours=f->CheckPlateau;
  f->CheckPlateau=FALSE;
  // ����������� �������
  AuxIndices.Zero();
  for(;;)
    { if(!AuxIndices.AllEqual(1)) // ��� ==1 - ��� ������� �����, � �� �����
        { double fn;
          BOOL bad=FALSE;
          for(int i=0;i<n_ag;i++)
            { int parindex=f->Indices.Arr[i]+(AuxIndices.Arr[i]-1)*f->IndStep;
              double p;
              double v=ValueByIndex(parindex,i,p);
              AuxIndices2.Arr[i]=parindex;
              if(v==rdsbcppHugeDouble)
                { bad=TRUE;
                  break;
                }
              XX[i]=p;
              YY[i]=v;
            }
          fn=bad?rdsbcppHugeDouble:CritFunc(Arguments);
          if(fn==rdsbcppHugeDouble)
            { // ��� ������ ������ - ����� �� �����
              f->EqualNeighbours=FALSE;
            }
          else
            {
              #if L57_MODELVERSION>=2
                // ������� �������� ��� ���������
                double cur=MaxSoFar[n_ag];
                if(cur==rdsbcppHugeDouble || fn>cur)
                  { // ����� ��������
                    MaxSoFar[n_ag]=fn;
                    for(int i=0;i<n_ag;i++)
                      MaxSoFar[i]=XX[i];
                  }
              #endif
              if(fn>f->f) // ����� ��������
                { f->EqualNeighbours=FALSE;
                  f->f=fn;
                  for(int i=0;i<n_ag;i++)
                    f->Indices.Arr[i]=AuxIndices2.Arr[i];
                }
              else if(f->EqualNeighbours && fabs(fn-f->f)>1e-38) // �� ����������
                f->EqualNeighbours=FALSE;
            }
        }
      if(!AuxIndices.NextStep(3,1))
        break;
    } // for(;;)

  if(f->IndStep==1)
    return TRUE;

  f->IndStep/=2;
  if(f->IndStep<1)
    f->IndStep=1;
  return FALSE;
}
//-----------------------------------------------------------------------

// ���� ������� ������ ���������
void rdsbcppBlockClass::DoPrecStage(void)
{ int _count=1000,n_ag,cols;
  //int icount=Resp.Rows();
  TIntArray AuxIndices,AuxIndices2;

  if(XX.IsEmpty())
    Arguments.X_Params.Setup(NULL);
  else
    Arguments.X_Params.Setup(XX.GetBuffer(),XX.Size());
  if(YY.IsEmpty())
    Arguments.Y_Params.Setup(NULL);
  else
    Arguments.Y_Params.Setup(YY.GetBuffer(),YY.Size());

  n_ag=RealNAgents();
  cols=XX.Size();
  if(n_ag>cols)
    n_ag=cols;
  cols=Resp.Cols()-1;
  if(n_ag>cols)
    n_ag=cols;
  if(n_ag>Indices.Count)
    n_ag=Indices.Count;
  Arguments.Count=n_ag;
  AuxIndices.Allocate(n_ag,TRUE);
  AuxIndices2.Allocate(n_ag,TRUE);

  for(;;)
    { // ����������� � ������ ���������� � Candidates
      TMaxCandidate *f=Candidates.First;
      BOOL end;
      if(!f)
        { // ���������
          #if L57_MODELVERSION>=3
            if(setup_finalrun && RealNAgents()!=0 && FinalMax.Count()!=0)
              { FinalRunStage(TRUE);
                return;
              }
          #endif
          SetOutput();
          return;
        }

      end=PrecStageProcess(f,AuxIndices,AuxIndices2,n_ag);
      //f->EqualNeighbours=FALSE;
      if(f->EqualNeighbours) // ��������� �������� �� ����� - �����������
        { progress3_cur+=1.0;
          Candidates.RemoveTop();
          delete f;
        }
      else if(end) // �������� f ��������� - ��������� � ����� ������
        { progress3_cur+=1.0;
          Candidates.RemoveTop();
          StoreFinalMax(f);
        }

      _count--;
      if(_count<=0)
        break;
    } // for(;;)

  // ������� ��� ���������
  Ready=_Start=1;
}
//-----------------------------------------------------------------------

#if L57_MODELVERSION>=3
// ���� ������� ���������� �������
void rdsbcppBlockClass::FinalRunStage(BOOL init)
{
  if(init)
    { int n_ag=RealNAgents();
      int cols=X.Size();
      if(n_ag>cols)
        n_ag=cols;
      cols=Resp.Cols()-1;
      if(n_ag>cols)
        n_ag=cols;
      if(n_ag>P.Size())
        n_ag=P.Size();

      SetOutputServ();

      if(n_ag>AllMax.Cols())
        n_ag=AllMax.Cols();
      for(int i=0;i<n_ag;i++)
        P[i]=AllMax[0][i];

      p_ready=Ready=1;
      Reset_X_Ready();
      p_go=1;
      Stage=TSMFSTAGE_FINAL;
      return;
    }

  // ���� ����������    
  if(!IsXReady())
    return;

  SetOutput(FALSE);
}
//-----------------------------------------------------------------------
#endif

#endif // #ifdef L57MS_AFTERCLASS
//-----------------------------------------------------------------------

