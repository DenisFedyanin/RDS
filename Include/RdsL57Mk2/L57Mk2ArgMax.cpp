// �������� ������������� �������� ��������
//---------------------------------------------------------------------------
// ����������� ����� ������ ����������� ������������� �������
// ������������� ����� ���������� (������ 2)
//---------------------------------------------------------------------------

// ��������� �������������� ����� ������ "���� ������": L57OutOfMemMsg(14);

//---------------------------------------------------------------------------
// ���������� ��������
//---------------------------------------------------------------------------
#ifdef L57MS_GLOBALS
#ifndef L57_MODELVERSION
  #define L57_MODELVERSION 0
#endif

// �������� ������ ���� ������
#define L57_LATESTMODELVERSION 10

#ifdef L57_LOG
  #define _LOGBLK (rdsbcppBlockData->Block)
#endif

// ����������� ��� �������� �� ���������
#define L57_SMALLDELTA  1e-30

// ��� ����� ������� ������
#define L57_MODELTEMPLATE "$RDSINCLUDE$\\RdsL57Mk2\\IpuL57Mk2ArgMax.mdl"

// ��������� ��������� ����� �����
#define L57_MSGCAPTION  "���������� ������������"

// ����� ������� � ������
#include <RdsL57Mk2/L57Mk2Basic.cpp>

// ��������� ������������� (_Stage)
#define L57STAGE_WAIT           0       // ��������
#define L57STAGE_PRETAB         1       // ��������������� ������ ������� �������� ��������� �������
#define L57STAGE_GRID           2       // ��������� �������
      #define L57SUBSTAGE_GRIDFILL   0       // ��������� �������
      #define L57SUBSTAGE_GRIDCAND   1       // ����� ����������
#define L57STAGE_PREC           3       // ��������� ���������� ����������
#define L57STAGE_TABAFTERCALCSIMP 4     // �������� ������� �������� ������
#define L57STAGE_TABAFTERCALCAM   5     // �������� �������� ������ arg max
#define L57STAGE_TABPRECCALC    6       // ��������� �������� ������ arg max
#define L57STAGE_FINAL          100     // ��������� ������������

// ����� ��������� ����������
#define L57PRECGRID_4X4         0       // �� ������� ��������� - �� ��� ����������� ���� � �������
#define L57PRECGRID_2X2         1       // �� ������� ��������� - �� ���� ����������� ��� � �������

// �������������� ����� ���� ��������
#define L57SETUPFIELD_NOAUTOSTRT        1       // �������� ����� ������ �� �������
#define L57SETUPFIELD_DEEPSEARCH        2       // �������� ���������
#define L57SETUPFIELD_CHUNKMB           3       // ����� ��������� ������
#define L57SETUPFIELD_TACTCOUNT         4       // ����� �������� � �����
#define L57SETUPFIELD_EQMAXDELTA        5       // ����������� ������������ ����������
                                                // ��� ��������� ����������:
#define L57SETUPFIELD_PRECENDSTEPS      6       //   �������� �� �������� �����
#define L57SETUPFIELD_PRECDF            7       //   �������� �� ����������� �������
#define L57SETUPFIELD_DFCYCLES          8       //   �������� � ������ �����������
#define L57SETUPFIELD_DFVALUE           9       //   �������� �����������
#define L57SETUPFIELD_PRECGRID          10      //   ����� ���������
#define L57SETUPFIELD_NOPLATEAU         11      // ������� ����� �� �����
#define L57SETUPFIELD_TABAFTERCALC      12      // �������� �������� ��������� �������
#define L57SETUPFIELD_TABPRECCALC       13      // ��������� �������� ��������� �������
                                                // ��� ��������� ������:
#define L57SETUPFIELD_TABPRECENDSTEPS   14      //   �������� �� �������� �����
#define L57SETUPFIELD_TABPRECGRID       15      //   ����� ���������
#define L57SETUPFIELD_TABPRECDF         16      //   �������� �� ����������� �������
#define L57SETUPFIELD_TABDFVALUE        17      //   �������� �����������
#define L57SETUPFIELD_TABDFCYCLES       18      //   �������� � ������ �����������
#define L57SETUPFIELD_DFFILTERVALUE     19      // �������� ���������� �������� �������
#define L57SETUPFIELD_TITLE             1000    // �������������� - ��������� �������
#define L57SETUPFIELD_CODE              1001    // �������������� - ����� �������
#define L57SETUPFIELD_PARAMS            1002    // �������������� - ������ ����������
#define L57SETUPFIELD_EDITBTN           1003    // ������ ������ ���� ��������������
#define L57SETUPFIELD_OUTTABBTN         1004    // ������ ������ ���� �������������� � �������� �������� ������
#define L57SETUPFIELD_TEMPLATE          2000    // ������������ - ������ ������
#define L57SETUPFIELD_MODEL             2001    // ������������ - ����� ������
#define L57SETUPFIELD_LOCKEDIT          2002    // ������������ - ������� ���������� ��������������
#define L57SETUPFIELD_ADDITIONALDATA    2003    // ������������ - �������������� ������
#define L57SETUPFIELD_BLOCKTOSHOW       2004    // ������������ - ����, ������� ����� �������� ��� ��������� �������� ����

// ������������
class L57_MaxCandidate;
//-----------------------------------------------------------------------

// ������� ������� ����� �������
int L57_HelpState=0;
// ������� ���������� �������������� �������� ���� ����� � ����������
BOOL L57_AllowTextRectEdit=FALSE;
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// ������ ��������������� ���������
//-----------------------------------------------------------------------
class L57_Parameter
{ private:
    // ������� ������ ����������
    BOOL AllocateArray(int count);

  public:
    // �������� ������
    double Min,Max;     // �������� �������� ��������
    double StartStep;   // ��������� ���
    double EndStep;     // �������� ��� (���� ����)
    double FuncVal;     // �������� ��� ������� ��������� ������� (���� ����)

    // ��������������� ������ ���������� � ������� ������
    double _Min,_Max,_Delta;    // ����������������� �������� ��������� � ����
    L57_DblArray Array;         // ������ �������� ���������
    //double *Array;              // ������ �������� ���������
    //int ArrayCount;             // ����� �������� � �������
    int _ArgIndex;              // ����� �������� �������� � �������� �������� � ������� Array
    int EvalChunkSize;          // ������ "�����" ������� �� ����� ��������� � ��������
                                // "����������" ������ � ����������� ������
    int *ChunkStart;            // ����� �������� ������ �����
    int *ChunkEnd;              // ������ �������� ����� �����
    int ChunkCount;             // ����� ������ � �������
    int _ChunkIndex;            // ����� �������� �����

    DWORD ChunkArrayShift;      // ����� � ����� ������� �������� �������� ����� ���
                                // ���������� ����� �������

    // �������������� ������ (��� ������ ������)
    int AuxIndex,AuxIndexMin,AuxIndexMax;

    double Current;             // ������� �������� ��� �������� � ������� ����������

    // ��������� �� ������� ����������, ����������� � ������ �������� ��������� ����� ������������
    L57_MaxCandidate **AddedMaxObjects;
    int AddedMaxObjectsCount;

    // �������� ������ ����������
    void ClearArrays(void);
    // ������� ������ ���������� �� �������� ������
    BOOL AllocateArray(void);
    // �������� �������� ����� ���������� � ����� �������
    BOOL AddArgs(int n,BOOL fillwitherror=FALSE);

    // ������� ������� ������ �� EvalChunkCount
    BOOL AllocateChunkArray(void);

    L57_Parameter(void)
      { Min=Max=StartStep=EndStep=rdsbcppHugeDouble;
        ChunkCount=EvalChunkSize=AddedMaxObjectsCount=0;
        ChunkStart=ChunkEnd=NULL; AddedMaxObjects=NULL;
      };
    ~L57_Parameter(){ClearArrays();};
};
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// ����� �������������� ����������
//-----------------------------------------------------------------------
// ������ ���� ��� ���������� ������ L57_PARAMSCOUNT!
class L57_Parameters
{ public:
    #ifdef L57_PARAMSCOUNT
      L57_Parameter P[L57_PARAMSCOUNT];
    #endif

    // ������ �������� ������� ��� �������� ��������������� �����
    double *ChunkValues;
    DWORD ChunkValuesSize;

    // �������� ��� ��������� ������
    void ClearTempData(void);

    // �������� ������ ��� �������� ��������������� �����
    void ClearChunkValuesArray(void);
    // ������� ������ ��� �������� ��������������� �����
    // �����. ����������
    BOOL CreateChunkValuesArray(void);
    // ��������� ������ �������� ��������������� ����� ��������� ������
    void ResetChunkValuesArray(void);
    // �������� ��������� �� �������� � ������� �� ������� �������� (_ArgIndex,_ChunkIndex)
    double *GetChunkValuePtrByParams(DWORD *pIndex=NULL);
    // �������� ��������� �� �������� � ������� �� ������� �������� + AuxIndex
    double *GetChunkValuePtrByParamsAndAux(DWORD *pIndex=NULL);
    // ��������� ������� ���������� ���� ���������� �� ������ ������
    void ResetCurrentChunk(void);
    // ��������� ��� � ������� ����� (�����. TRUE, ���� ������� �������� ����� ��������)
    BOOL NextStep_Chunk(void);
    // ��������� ����� (�����. TRUE ���� ������� ������ ��������)
    BOOL NextChunk(void);
    // ��������� ��� � ������ ������ ���������� (��� ������)
    BOOL NextStep(void);

    // ���������������� ��� ��������������� �������
    void InitAuxIndex(int IndMin,int IndMax);
    // ����� ��������� � ���������������� �������� �������� � ������� ��������?
    BOOL ValidMainAndAuxIndices(BOOL *pAllZero);
    // ������� ��� �� ��������������� ��������
    BOOL StepAuxIndex(void);

    L57_Parameters(void)
      { ChunkValues=NULL; ChunkValuesSize=0;
      };
    ~L57_Parameters()
      { ClearTempData(); };
};
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� �������� ������ ���������� � ��������� ��� ������� ����������
//---------------------------------------------------------------------------
class L57_MaxCandidate
{ public:
    L57_MaxCandidate *Next;

    #ifdef L57_PARAMSCOUNT
    double CurParams[L57_PARAMSCOUNT]; // ������� �������� ����������
    double Deltas[L57_PARAMSCOUNT];    // ������� ����
    #endif
    double f;                          // �������� �������
    int cycles_in_df;                  // ����� ����� ������ ������ �����������

    double Old_Val;     // ���������� �������� (������������ �������)

    BOOL Tag1,Tag2;     // ��������������� �����
    int IntTag1,IntTag2;

    static int GlobalCount; // ����� ����� ��������� ��������

    //BOOL Equal;         // ��� ������ ����������
    int StepsDown;      // ������� ���������� ����� �������

    L57_MaxCandidate(void)
      { Next=NULL;
        cycles_in_df=0;
        f=Old_Val=rdsbcppHugeDouble;
        //Equal=TRUE;
        StepsDown=0;
        GlobalCount++;
        Tag1=Tag2=FALSE;
        IntTag1=IntTag2=-1;
      };
    ~L57_MaxCandidate(){GlobalCount--;};

};
int L57_MaxCandidate::GlobalCount=0; // ����� ����� ��������� ��������
//---------------------------------------------------------------------------
// ������� ������ ����������� � ���������
class L57_MaxList
{ public:
    L57_MaxCandidate *First;

    // ������?
    inline BOOL IsEmpty(void){return First==NULL;};
    // ��������
    void Clear(void);
    // �������� ��������� � ������ �������� ������
    L57_MaxCandidate *AddFirst(void);
    // ����������� �������
    L57_MaxCandidate *RemoveTop(void);
    // �������� �������� ��������� � ������ �������� ������
    void AddExistingFirst(L57_MaxCandidate *f);
    // ����������
    int Count(void);
    // ���������� ���������� � ������ �������
    void Exchange(L57_MaxList &list);
    // ������� ��� ���� �������
    void RemoveSmall(double level);

    L57_MaxList(void){First=NULL;};
    ~L57_MaxList(){Clear();};
};
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
// ������ ��������������� ���������
//-----------------------------------------------------------------------
// �������� ������ ����������
void L57_Parameter::ClearArrays(void)
{
  Array.Clear();
  Array.PrevCount=0;
  if(ChunkStart)
    rdsFree(ChunkStart);
  if(ChunkEnd)
    rdsFree(ChunkEnd);
  ChunkCount=0;
  ChunkStart=ChunkEnd=NULL;

  if(AddedMaxObjects)
    rdsFree(AddedMaxObjects);
  AddedMaxObjects=NULL;
  AddedMaxObjectsCount=0;
}
//---------------------------------------------------------------------------

// ������� ������ ����������
BOOL L57_Parameter::AllocateArray(int count)
{ double d_size=((double)count)*sizeof(double);
  ClearArrays();
  if(count<=0)
    return TRUE;
  if(d_size-1>((double)(count*sizeof(double)))) // ������ - ������� ������� ������
    return FALSE;

  return Array.Allocate(Array.PrevCount=count);
}
//---------------------------------------------------------------------------

// ������� ������ ���������� �� �������� ������
BOOL L57_Parameter::AllocateArray(void)
{ int n=L57_CalcNPointsByDiap(Min,Max,StartStep);
  ClearArrays();

  if(n<1)
    { // ������ ��������� �����
      _Min=_Max=0.0;
      _Delta=1.0;
      if(!AllocateArray(1)) // ������ ������� - � ���������� int
        return FALSE;
      Array.Data[0]=0.0;
      return TRUE;
    }

  if(!AllocateArray(n)) // ������ ������� - � ���������� int
    return FALSE;

  if(n==1)
    { _Min=_Max=Min; _Delta=1.0; }
  else
    { _Min=Min; _Max=Max; _Delta=StartStep; }

  for(int xi=0;xi<n;xi++)
    { double v=L57_ValueInDiap(_Min,_Max,_Delta,xi,n);
      Array.Data[xi]=v;
    }
  return TRUE;
}
//-----------------------------------------------------------------------

// �������� �������� ����� ���������� � ����� �������
BOOL L57_Parameter::AddArgs(int n,BOOL fillwitherror)
{ int ArgCount=Array.Count;
  int new_count=ArgCount+n;
  L57_DblArray newArgs;

  if(n==0)
    return TRUE;
  if(n<0 || new_count<0)
    return FALSE;

  // ����������� ���
  if(!newArgs.Allocate(new_count))
    return FALSE;
  newArgs.FillDataFrom(Array);
  Array.Exchange(newArgs);
  Array.PrevCount=ArgCount;

  if(fillwitherror)
    for(int i=Array.PrevCount;i<new_count;i++)
      Array.Data[i]=rdsbcppHugeDouble;
  return TRUE;
}
//-----------------------------------------------------------------------

// ������� ������� ������ �� EvalChunkCount
BOOL L57_Parameter::AllocateChunkArray(void)
{ int first,last,index,final;
  int ArrayCount=Array.Count;
  if(ChunkStart)
    rdsFree(ChunkStart);
  if(ChunkEnd)
    rdsFree(ChunkEnd);
  ChunkCount=0;
  ChunkStart=ChunkEnd=NULL;

  if(EvalChunkSize<=0)
    return FALSE;

  // EvalChunkSize=3, ArrayCount=8
  // 01234567
  // vvvvvvvv
  // ^^^----- ����� 0: [0...2], �.�. [first...first+EvalChunkSize-1]
  //   ^^^--- ����� 1: [2...4], �.�. [first...first+EvalChunkSize-1]
  //     ^^^- ����� 2: [4...6], �.�. [first...first+EvalChunkSize-1]
  //       ^^ ����� 3: [6...7], �.�. [first...ArrayCount-1]

  // ������������ ����� ������
  final=ArrayCount-1;
  first=index=0;
  for(;;)
    { // ����� first...first+EvalChunkSize-1 ������������
      last=first+EvalChunkSize-1;
      index++;
      if(last>=final)
        break;
      first=last;
    }
  // � index - ����� ������

  RDSBCPP_TRY
    { ChunkStart=L57_NewInt(index); }
  RDSBCPP_CATCHALL
    { ChunkStart=NULL; }
  if(!ChunkStart)
    return FALSE;
  RDSBCPP_TRY
    { ChunkEnd=L57_NewInt(index); }
  RDSBCPP_CATCHALL
    { ChunkEnd=NULL; }
  if(!ChunkEnd)
    { rdsFree(ChunkStart);
      ChunkStart=NULL;
      return FALSE;
    }
  ChunkCount=index;

  first=index=0;
  for(;;)
    { // ����� first...first+EvalChunkSize-1 ������������
      last=first+EvalChunkSize-1;
      if(last>final)
        last=final;
      ChunkStart[index]=first;
      ChunkEnd[index]=last;
      index++;
      if(last==final)
        break;
      first=last;
    }
  return TRUE;
}
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------
// ����� �������������� ����������
//-----------------------------------------------------------------------

// ������� ��� �� ��������������� ��������
BOOL L57_Parameters::StepAuxIndex(void)
{ BOOL end=TRUE;
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->AuxIndex++;
      if(p->AuxIndex<=p->AuxIndexMax)
        { end=FALSE;
          break;
        }
      // ����� �� �����
      p->AuxIndex=p->AuxIndexMin;
    }
#endif    
  return end;
}
//---------------------------------------------------------------------------

// �������� ��������� �� �������� � ������� �� ������� �������� + AuxIndex
double *L57_Parameters::GetChunkValuePtrByParamsAndAux(DWORD *pIndex)
{ DWORD n=0;
#ifdef L57_PARAMSCOUNT
  if(pIndex) *pIndex=0xffffffff;
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      int ai=p->_ArgIndex+p->AuxIndex;
      n+=(ai-p->ChunkStart[p->_ChunkIndex])*p->ChunkArrayShift;
    }
  if(n>=ChunkValuesSize)
    return NULL;
#endif
  if(pIndex) *pIndex=n;
  return ChunkValues+n;
}
//---------------------------------------------------------------------------

// ���������������� ��� ��������������� �������
void L57_Parameters::InitAuxIndex(int IndMin,int IndMax)
{
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->AuxIndex=p->AuxIndexMin=IndMin;
      p->AuxIndexMax=IndMax;
    }
#endif
}
//---------------------------------------------------------------------------

// ����� ��������� � ���������������� �������� �������� � ������� ��������?
BOOL L57_Parameters::ValidMainAndAuxIndices(BOOL *pAllZero)
{ BOOL inside=TRUE;
  if(pAllZero) *pAllZero=TRUE;
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      int n;
      if(pAllZero!=NULL && p->AuxIndex!=0)
        *pAllZero=FALSE;
      n=p->_ArgIndex+p->AuxIndex;
      if(n<p->ChunkStart[p->_ChunkIndex] || n>p->ChunkEnd[p->_ChunkIndex])
        inside=FALSE;
    }
#endif    
  return inside;
}
//---------------------------------------------------------------------------

// ��������� ������� ���������� ���� ���������� �� ������ ������
void L57_Parameters::ResetCurrentChunk(void)
{
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->_ArgIndex=p->ChunkStart[p->_ChunkIndex];
    }
#endif    
}
//---------------------------------------------------------------------------


// �������� ��������� �� �������� � ������� �� ������� ��������
double *L57_Parameters::GetChunkValuePtrByParams(DWORD *pIndex)
{ DWORD n=0;
  if(pIndex) *pIndex=0xffffffff;
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      n+=(p->_ArgIndex-p->ChunkStart[p->_ChunkIndex])*p->ChunkArrayShift;
    }
  if(n>=ChunkValuesSize)
    return NULL;
#endif
  if(pIndex) *pIndex=n;
  return ChunkValues+n;
}
//---------------------------------------------------------------------------

// �������� ��� ��������� ������
void L57_Parameters::ClearTempData(void)
{
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    P[i].ClearArrays();
#endif
  ClearChunkValuesArray();
}
//---------------------------------------------------------------------------

// �������� ������ ��� �������� ��������������� �����
void L57_Parameters::ClearChunkValuesArray(void)
{ if(ChunkValues)
    { rdsFree(ChunkValues);
      ChunkValues=NULL;
    }
  ChunkValuesSize=0;
}
//---------------------------------------------------------------------------

// ������� ������ ��� �������� ��������������� �����
// �����. ����������
BOOL L57_Parameters::CreateChunkValuesArray(void)
{ BOOL ok=TRUE;
#ifdef L57_PARAMSCOUNT
  DWORD count=1;
  double d_count=1.0;

  // �������
  if(ChunkValues)
    rdsFree(ChunkValues);
  ChunkValues=NULL;
  ChunkValuesSize=0;

  // ������ ������������� �������
  for(int i=L57_PARAMSCOUNT-1;i>=0;i--)
    { L57_Parameter *p=P+i;
      int maxchunk=0;
      p->ChunkArrayShift=count;
      for(int j=0;j<p->ChunkCount;j++)
        { int n=p->ChunkEnd[j]-p->ChunkStart[j]+1;
          if(maxchunk<n)
            maxchunk=n;
        }
      if(!maxchunk)
        return FALSE;
      count*=maxchunk;
      d_count*=(double)maxchunk;
    }
  if(((double)count)<d_count) // ������������?
    return FALSE;

  RDSBCPP_TRY
    { ChunkValues=L57_NewDouble(ChunkValuesSize=count);
      if(!ChunkValues)
        ok=FALSE;
    }
  RDSBCPP_CATCHALL
    { ok=FALSE; ChunkValuesSize=0; ChunkValues=NULL; }
#endif    
  return ok;
}
//---------------------------------------------------------------------------

// ��������� ������ �������� ��������������� ����� ��������� ������
void L57_Parameters::ResetChunkValuesArray(void)
{
  for(DWORD i=0;i<ChunkValuesSize;i++)
    ChunkValues[i]=rdsbcppHugeDouble;
}
//---------------------------------------------------------------------------

// ��������� ��� � ������ ������ ���������� (��� ������)
BOOL L57_Parameters::NextStep(void)
{ BOOL changed=FALSE;

#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->_ArgIndex++;
      if(p->_ArgIndex<p->Array.Count) // ���� ��� �������� � ������� �����
        { changed=TRUE;
          break;
        }
      // �������� ���
      p->_ArgIndex=0;
    }
#endif
  return (!changed);
}
//---------------------------------------------------------------------------

// ��������� ��� � ������� ����� (�����. TRUE, ���� ������� �������� ����� ��������)
BOOL L57_Parameters::NextStep_Chunk(void)
{ BOOL changed=FALSE;

#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->_ArgIndex++;
      if(p->_ArgIndex<=p->ChunkEnd[p->_ChunkIndex]) // ���� ��� �������� � ������� �����
        { changed=TRUE;
          break;
        }
      // �������� ���
      p->_ArgIndex=p->ChunkStart[p->_ChunkIndex];
    }
#endif
  return (!changed);
}
//---------------------------------------------------------------------------

// ��������� ����� (�����. TRUE ���� ������� ������ ��������)
BOOL L57_Parameters::NextChunk(void)
{
#ifdef L57_PARAMSCOUNT
  BOOL changed=FALSE;
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->_ChunkIndex++;
      if(p->_ChunkIndex<p->ChunkCount) // ���� ��� ����� �� ����� ���������
        { changed=TRUE;
          break;
        }
      // ����� ���
      p->_ChunkIndex=0;
    }
  if(!changed)
    return TRUE;

  // ������������� ������� ���������� �� ������ ������
  ResetCurrentChunk();

#endif
  // ���������� � �������� ���������
  return FALSE;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������� �������� ������ ���������� � ��������� ��� ������� ����������
//---------------------------------------------------------------------------

// �������� ������� ������ ����������
void L57_MaxList::Clear(void)
{ L57_MaxCandidate *f,*fn;

  f=First;
  while(f)
    { fn=f->Next;
      delete f;
      f=fn;
    }
  First=NULL;
}
//---------------------------------------------------------------------------

// ������� ��� ���� �������
void L57_MaxList::RemoveSmall(double level)
{ L57_MaxCandidate *f,*prev=NULL,*f1;

  f=First;
  while(f)
    { if(f->f>=level) // ���������
        { prev=f;
          f=f->Next;
          continue;
        }
      // �������� �� ������ � �������
      f1=f->Next;
      if(prev) // ���� ����������
        prev->Next=f1;
      else // ��� �����������
        First=f1;
      delete f;
      f=f1;
    }
}
//---------------------------------------------------------------------------


// �������� ��������� � ������ �������� ������
L57_MaxCandidate *L57_MaxList::AddFirst(void)
{ L57_MaxCandidate *f;
  try { f=new L57_MaxCandidate(); }
  catch(...) { f=NULL;};
  if(!f)
    return NULL;
  AddExistingFirst(f);
  return f;
}
//---------------------------------------------------------------------------

// �������� �������� ��������� � ������ �������� ������
void L57_MaxList::AddExistingFirst(L57_MaxCandidate *f)
{
  if(First==NULL)
    First=f;
  else
    { f->Next=First;
      First=f;
    }
}
//---------------------------------------------------------------------------

// ����������� �������� ��������� �� �������� ������
L57_MaxCandidate *L57_MaxList::RemoveTop(void)
{ L57_MaxCandidate *f=First;
  if(!f)
    return f;
  First=f->Next;
  f->Next=NULL;
  return f;
}
//---------------------------------------------------------------------------

// ���������� ����������
int L57_MaxList::Count(void)
{ int n=0;
  for(L57_MaxCandidate *f=First;f!=NULL;f=f->Next)
    n++;
  return n;
}
//---------------------------------------------------------------------------

// ���������� ���������� � ������ �������
void L57_MaxList::Exchange(L57_MaxList &list)
{ L57_MaxCandidate *f=First;
  First=list.First;
  list.First=f;
}
//---------------------------------------------------------------------------

#endif // #ifdef L57MS_GLOBALS
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����� ������������ ����
//---------------------------------------------------------------------------
#ifdef L57MS_POPUP

  #ifdef L57_HELPUIFILE
    if(L57_HelpUIFileExists(L57_HELPUIFILE,&L57_HelpState))
      rdsbcppBlockData->Flags=RDS_SETFLAG(rdsbcppBlockData->Flags,RDS_BLOCKHASHELP,TRUE);
  #endif

#endif // #ifdef L57MS_POPUP
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������ �������
//---------------------------------------------------------------------------
#ifdef L57MS_OTHER

switch(CallMode)
  { case RDS_BFM_LEAVINGEDITMODE:
      #if L57_MODELVERSION>=2
        L57_SendSingleFunctionIfNoInput(rdsbcppBlockData->Block,Func_in.Num(),Func_out.Num(),FALSE,
                                        _CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str());
      #endif
      break;
    case RDS_BFM_BEFORESAVE:
    case RDS_BFM_AFTERLOAD:
      // ������� ��������� ����� ����� ������
      L57_ResFile_DeleteModelCopy(rdsbcppBlockData->Block);
      break;
    case RDS_BFM_UNLOADSYSTEM:
      // ����� ����������� �� ������ - ���� ���� ��������� ����� ������, ������, ���������� �� ����
      L57_ResFile_RestoreModelCopy(rdsbcppBlockData->Block);
      break;

    #ifdef L57_HELPUIFILE
      case RDS_BFM_HELP:
        if(L57_HelpUIFileExists(L57_HELPUIFILE,&L57_HelpState))
          { rdsShowHelp(L57_HELPUIFILE,L57_HELPTOPIC,FALSE);
            Result=1;
          }
        break;
    #endif

    #if L57_MODELVERSION>=10
      case RDS_BFM_TRANSLATE:
        { L57_String usertext;
          usertext.Assign(TR_text.c_str());
          //L57_SaveModelToUndo(rdsbcppBlockData->Block,TRUE);
          if(L57_ExecTool_Translate(rdsbcppBlockData->Block,
                                    L57_MODELTEMPLATE,
                                    TR_params,&usertext))
            { TR_text=usertext.c_str();
              //L57_CompileModel(rdsbcppBlockData->Block);
            }
        }
        break;
    #endif
  }

#endif // #ifdef L57MS_OTHER
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� ������ �������
//---------------------------------------------------------------------------
#ifdef L57MS_ACCEPTFUNC
  #if L57_MODELVERSION>=2
    L57_AcceptFunc_Param_Queue *queue=L57_GetQueueFromParam(Param);

    if(queue)
      { if(queue->TransferId==_Transfer.LastTransferId)
          { // ��� ���� ����� - ���������
            _AcceptRecursionError=TRUE;
            return;
          }
        else
          _Transfer.LastTransferId=queue->TransferId;
      }

    // �� ��������� �� ��� ������ �������
    if(_InsideAcceptFunction)
      { _AcceptRecursionError=TRUE;
        return;
      }
    // ��������
    _InsideAcceptFunction=TRUE;
    RDSBCPP_TRY
      { if(L57_GetSingleFunctionByConn(rdsbcppBlockData->Block,L57_MODELTEMPLATE,Param,
                                      _CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str(),
                                      &_CurFuncExportText))
          { // �������� ������
            char *text=_CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str();
            //rdsBlockMessageBox(rdsbcppBlockData->Block,text,"��������",MB_OK);
            L57_SendSingleFunction(rdsbcppBlockData->Block,Func_out.Num(),Param->Immediate,text,queue);
          }
        _InsideAcceptFunction=FALSE;
      }
    RDSBCPP_CATCHALL
      { _InsideAcceptFunction=FALSE; }

  #endif
#endif // #ifdef L57MS_ACCEPTFUNC


//---------------------------------------------------------------------------
// ������� ������ ���������
//---------------------------------------------------------------------------
#ifdef L57MS_ACCEPTPARAM

  #if L57_MODELVERSION>=3
    // �� ��������� �� ��� ������ �������
    if(_InsideAcceptParam)
      { _AcceptParamRecursionError=TRUE;
        return;
      }
    // ��������
    _InsideAcceptParam=TRUE;
    RDSBCPP_TRY
      { if(_Transfer.AcceptParam(Param,rdsbcppBlockData->Block)) // ���-�� ������� (���� ����� �������� � �� ���������)
          { Restart_cmd=_Start=1; // �������������
          }

        _InsideAcceptParam=FALSE;
      }
    RDSBCPP_CATCHALL
      { _InsideAcceptParam=FALSE; }
  #endif

#endif // #ifdef L57MS_ACCEPTPARAM


//---------------------------------------------------------------------------
// ������� �������� �������� ���������
//---------------------------------------------------------------------------
#ifdef L57MS_CHECKTRANSFUNC
  // �� ��������� �� ��� ������ �������
  if(_InsideAcceptParam)
    { _AcceptParamRecursionError=TRUE;
      //rdsMessageBox("error","",MB_OK);
      return;
    }
  _InsideAcceptParam=TRUE;
  RDSBCPP_TRY
    { // � ���� �� ������� ���������� ���� ������� �����? (����� ��� ��������)
      _Transfer.CheckInputConnections(rdsbcppBlockData->Block);
      // ���������
      _Transfer.CheckTransferParam(rdsbcppBlockData->Block,Param,Func_in.Num());
      _InsideAcceptParam=FALSE;
    }
  RDSBCPP_CATCHALL
    { _InsideAcceptParam=FALSE; }
#endif // #ifdef L57MS_CHECKTRANSFUNC

//---------------------------------------------------------------------------
// ����� ������� �������� �������� ���������
//---------------------------------------------------------------------------
#ifdef L57MS_CHECKTRANSFUNC2
  RDSBCPP_TRY
    { _Transfer.CheckTransferReaction(rdsbcppBlockData->Block,
                                      Func_out.Num(),
                                      Param);
    }
  RDSBCPP_CATCHALL
    {  }
#endif // #ifdef L57MS_CHECKTRANSFUNC2

//---------------------------------------------------------------------------
// ���������
//---------------------------------------------------------------------------
#ifdef L57MS_SETUP
  RDS_HOBJECT win;
  BOOL res;
  int tabid=0,fieldid;
  L57_String Func,Inputs,Numbered,Title;
  L57_String modeltext,templatetext,oldoptions,auxdata;

  RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

  #if L57_MODELVERSION>=2
    BOOL FuncByConn=L57_CheckFunctionSource(rdsbcppBlockData->Block,Func_in.Num());
  #endif


  if(model!=NULL && rdscompIsEditorOpen(model->Model))
    { rdsMessageBox("��� ��������� ����� ����� ���������� ������� �������� ��� ������. "
                    "����������, �������� ��������.",L57_MSGCAPTION,MB_OK|MB_ICONWARNING);
      rdscompOpenBlockModelEditor(rdsbcppBlockData->Block);
      return;
    }

  win=rdsFORMCreate(TRUE,-1,-1,L57_MSGCAPTION);
  if(!win)
    return;

  #ifdef L57_HELPUIFILE
    rdsFORMHelpButton(win,RDS_FORMHELP_UI|RDS_FORMHELP_CHECK,L57_HELPUIFILE,L57_HELPTOPIC);
  #endif


  // �������� ������
  if(!L57_LoadModelFile(rdsbcppBlockData->Block,modeltext))
    rdsMessageBox("������ �������� ������ ������ �����",L57_MSGCAPTION,MB_OK|MB_ICONERROR);
  if(!L57_ReadFileToMemory(L57_MODELTEMPLATE,templatetext))
    rdsMessageBox("������ �������� ������� ������ ����� (" L57_MODELTEMPLATE,L57_MSGCAPTION,MB_OK|MB_ICONERROR);

  // ������ �������� ������� �������
  L57_ExecTool_GetDynamicDescription(templatetext,modeltext,&Func,&Inputs,&Numbered,&Title);

  // ������� �������������� ������� � ����������
  rdsFORMAddTab(win,++tabid,"�������");

  // ��������� ����: ����� ������� � ����� ������
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TEMPLATE,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,templatetext.c_str());
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_MODEL,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,modeltext.c_str());

  #if L57_MODELVERSION>=3
    // ��������� ����: �������������� ������ � �������������� �����
    L57_CreateAdditionalText(&auxdata,rdsbcppBlockData->Block,Func_in.Num());
    rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_ADDITIONALDATA,RDS_FORMCTRL_NONVISUAL,"",0);
    rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,auxdata.c_str());
  #endif

  #if L57_MODELVERSION>=2
    // ��������� ���� - ���������� ��������������
    rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_LOCKEDIT,RDS_FORMCTRL_NONVISUAL,"",0);
    rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,FuncByConn?"0":"");
  #endif

  rdsFORMAddEdit(win,tabid,L57SETUPFIELD_TITLE,RDS_FORMCTRL_DISPLAY,"��������:",500);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_CODE,RDS_FORMCTRL_MULTILINE,
      FuncByConn?"������� (����������� �� �����):":"������� (������� \"�������� �������...\" ��� ��������������):",80);
  rdsFORMSetMultilineHeight(win,fieldid,6*24);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLREADONLY,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLRETURNS,1);
  //rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLBEVEL,1);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_PARAMS,RDS_FORMCTRL_MULTILINE,"",0);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLHEIGHT,80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLABELBEVEL,1);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_EDITBTN,RDS_FORMCTRL_BUTTON,"",80);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,
    FuncByConn?"�������� ���������� �����":"�������� ������� � ���������� �����");

  // ������� ����������
  rdsFORMAddTab(win,++tabid,"�����");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_NOAUTOSTRT,RDS_FORMCTRL_CHECKBOX,
                 "�������� ����� ������ �� ������� (������ Start_cmd)",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,noautostart_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_EQMAXDELTA,RDS_FORMCTRL_EDIT,
                 "����������� ������������ ����������:",80);
  rdsSetObjectDouble(win,fieldid,RDS_FORMVAL_VALUE,EqualMaxDelta_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DEEPSEARCH,RDS_FORMCTRL_CHECKBOX,"�������� ���������",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,deepsearch_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_NOPLATEAU,RDS_FORMCTRL_CHECKBOX,"������� ������ ����� �� \"�����\"",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,NoPlateau_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_PRECENDSTEPS,RDS_FORMCTRL_CHECKBOX,"�������� �� �������� �����",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,PrecToEndSteps_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_PRECGRID,RDS_FORMCTRL_COMBOLIST,"    ����� ���������:",200);
  rdsFORMSetComboList(win,fieldid,"4x4 ���������� ����\n2x2 ���������� ����");
  rdsFORMSetInt(win,fieldid,PrecGridSize_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_PRECDF,RDS_FORMCTRL_CHECKBOX,"�������� �� ����������� �������",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,PrecToDf_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DFVALUE,RDS_FORMCTRL_EDIT,
                 "    �������� ����������� �������:",80);
  rdsSetObjectDouble(win,fieldid,RDS_FORMVAL_VALUE,DfDelta_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DFCYCLES,RDS_FORMCTRL_UPDOWN,
                 "    �������� � ������ ����������� �������:",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNINC,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMIN,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMAX,100);
  rdsFORMSetInt(win,fieldid,DfCycles_setup);

  // ������� �������� ������
  rdsFORMAddTab(win,++tabid,"�������� �������");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_OUTTABBTN,RDS_FORMCTRL_BUTTON,"",80);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,"�������� �������� ��������� �������");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABAFTERCALC,RDS_FORMCTRL_CHECKBOX,
                 "��������� � �������� ������� ��������� �������� ����������",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,TabAfterCalc_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABPRECCALC,RDS_FORMCTRL_CHECKBOX,
                 "�������� �������� �������� ������ arg max",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,TabPrecCalc_setup?1:0);


  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABPRECENDSTEPS,RDS_FORMCTRL_CHECKBOX,"�������� �� �������� �����",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,Tab_PrecToEndSteps_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABPRECGRID,RDS_FORMCTRL_COMBOLIST,"    ����� ���������:",200);
  rdsFORMSetComboList(win,fieldid,"4x4 ���������� ����\n2x2 ���������� ����");
  rdsFORMSetInt(win,fieldid,Tab_PrecGridSize_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABPRECDF,RDS_FORMCTRL_CHECKBOX,"�������� �� ����������� �������",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,Tab_PrecToDf_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABDFVALUE,RDS_FORMCTRL_EDIT,
                 "    �������� ����������� �������:",80);
  rdsSetObjectDouble(win,fieldid,RDS_FORMVAL_VALUE,Tab_DfDelta_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABDFCYCLES,RDS_FORMCTRL_UPDOWN,
                 "    �������� � ������ ����������� �������:",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNINC,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMIN,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMAX,100);
  rdsFORMSetInt(win,fieldid,Tab_DfCycles_setup);



  // �������������� �������
  rdsFORMAddTab(win,++tabid,"�������������");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_CHUNKMB,RDS_FORMCTRL_UPDOWN,
                 "���������� ����� ��������� ������, ��:",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNINC,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMIN,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMAX,1000);
  rdsFORMSetInt(win,fieldid,(int)ChunkSizeMb_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TACTCOUNT,RDS_FORMCTRL_EDIT,
                 "����� �������� � ����� ����� �������:",80);
  rdsFORMSetInt(win,fieldid,tactcount_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DFFILTERVALUE,RDS_FORMCTRL_EDIT,
                 "���������� ��������� �������� ������� ��� ����������:",80);
  rdsSetObjectDouble(win,fieldid,RDS_FORMVAL_VALUE,DfFilter_setup);

  #if L57_MODELVERSION>=9
    // ������� �������� ����
    if(L57_AllowTextRectEdit)
      { tabid++;
        L57_TextRectEdit_Setup(rdsbcppBlockData->Block,win,TR_params,TR_text.c_str(),tabid);
      }
  #endif

  Setup_FillFuncDescr(win,Func,Inputs,Numbered,Title);

  Setup_GetOptionsString(win,oldoptions);

  // ��������� ����
  res=rdsFORMShowModalServ(win,Setup_WinCallback);

  if(res)
    { char *newmodeltext=rdsGetObjectStr(win,L57SETUPFIELD_MODEL,RDS_FORMVAL_VALUE);
      L57_String options;

      #if L57_MODELVERSION>=9
        if(L57_AllowTextRectEdit)
          { L57_String text;
            L57_TextRectEdit_Apply(rdsbcppBlockData->Block,win,TR_params,text
            #ifdef L57_TITLES
              ,L57_TITLES
            #endif
              );
            TR_text=text.c_str();
          }
        #ifdef RDS_LOCKTEXTRECT
          rdsbcppBlockData->Flags=RDS_SETFLAG(rdsbcppBlockData->Flags,
                                    RDS_LOCKTEXTRECT,
                                    L57_AllowTextRectEdit && TR_params>L57_BLOCKTEXT_NOCONTROL);
        #endif
      #endif

      Setup_GetOptionsString(win,options);
      if(modeltext.IsNotEqual(newmodeltext) || options.IsNotEqual(oldoptions))
        { L57_String texttosave;
          int ok=L57_ExecTool_SetOptions(templatetext.c_str(),newmodeltext,options.c_str(),&texttosave,&_CurFuncExportText);
          if(ok)
            ok=L57_SaveModelFile(rdsbcppBlockData->Block,texttosave.c_str());
          if(ok)
            { L57_CompileModel(rdsbcppBlockData->Block);
            }
          else
            rdsMessageBox("������ ������ ������ ������ �����",L57_MSGCAPTION,MB_OK|MB_ICONERROR);
        }
       noautostart_setup=(rdsGetObjectInt(win,L57SETUPFIELD_NOAUTOSTRT,RDS_FORMVAL_VALUE)!=0);
       deepsearch_setup=(rdsGetObjectInt(win,L57SETUPFIELD_DEEPSEARCH,RDS_FORMVAL_VALUE)!=0);
       ChunkSizeMb_setup=rdsFORMGetInt(win,L57SETUPFIELD_CHUNKMB);
       tactcount_setup=rdsGetObjectInt(win,L57SETUPFIELD_TACTCOUNT,RDS_FORMVAL_VALUE);
       rdsGetObjectDoubleP(win,L57SETUPFIELD_EQMAXDELTA,RDS_FORMVAL_VALUE,&EqualMaxDelta_setup);
       PrecToEndSteps_setup=(rdsGetObjectInt(win,L57SETUPFIELD_PRECENDSTEPS,RDS_FORMVAL_VALUE)!=0);
       PrecToDf_setup=(rdsGetObjectInt(win,L57SETUPFIELD_PRECDF,RDS_FORMVAL_VALUE)!=0);
       rdsGetObjectDoubleP(win,L57SETUPFIELD_DFVALUE,RDS_FORMVAL_VALUE,&DfDelta_setup);
       DfCycles_setup=rdsFORMGetInt(win,L57SETUPFIELD_DFCYCLES);
       PrecGridSize_setup=rdsFORMGetInt(win,L57SETUPFIELD_PRECGRID);
       NoPlateau_setup=(rdsGetObjectInt(win,L57SETUPFIELD_NOPLATEAU,RDS_FORMVAL_VALUE)!=0);
       TabAfterCalc_setup=(rdsGetObjectInt(win,L57SETUPFIELD_TABAFTERCALC,RDS_FORMVAL_VALUE)!=0);
       TabPrecCalc_setup=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECCALC,RDS_FORMVAL_VALUE)!=0);
       rdsGetObjectDoubleP(win,L57SETUPFIELD_DFFILTERVALUE,RDS_FORMVAL_VALUE,&DfFilter_setup);
       Tab_PrecToEndSteps_setup=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECENDSTEPS,RDS_FORMVAL_VALUE)!=0);
       Tab_PrecToDf_setup=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECDF,RDS_FORMVAL_VALUE)!=0);
       rdsGetObjectDoubleP(win,L57SETUPFIELD_TABDFVALUE,RDS_FORMVAL_VALUE,&Tab_DfDelta_setup);
       Tab_DfCycles_setup=rdsFORMGetInt(win,L57SETUPFIELD_TABDFCYCLES);
       Tab_PrecGridSize_setup=rdsFORMGetInt(win,L57SETUPFIELD_TABPRECGRID);

       if(!(PrecToEndSteps_setup||PrecToDf_setup))
         deepsearch_setup=FALSE;
       if(!(Tab_PrecToEndSteps_setup||Tab_PrecToDf_setup))
         TabPrecCalc_setup=FALSE;
       _InParamsChange=TRUE;
    }

  if(_InParamsChange && (rdsbcppBlockData->Flags & RDS_VARCHECKFAILED)==0) // � ����������� ��� � �������
    _Start=1;

  rdsDeleteObject(win);
  Result=res?1:0;

#endif // #ifdef L57MS_SETUP
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// �������� � ������
//---------------------------------------------------------------------------
#ifdef L57MS_CLASS

// ������� ������ ������� ��� �������������� � ������� ������� �� ������
static char *_FuncExportText;

// ������� ������ �������, ���������� ����� ���������� �������������� (����� ���������� ��
// _FuncExportText �� ��������������)
L57_String _CurFuncExportText;

// ����� �������� �������� ������ �������
BOOL _InsideAcceptFunction,_AcceptRecursionError,_TransferError;
// ����� �������� �������� ������ ���������
BOOL _InsideAcceptParam,_AcceptParamRecursionError;

// ��������� ������������ ����������
BOOL _InParamsChange;

// ����������� �������
L57_Calc _Calculator;

// ����� ��������������� ����������
L57_Parameters _Params;

// ������� �������� (��������)
double _GlobalMax;

// ������ ������� ����������
L57_MaxList _FinalMax;
// ������ ���������� � ���������
L57_MaxList _Candidates;

BOOL Has_TabAfterCalcSimp;      // ���� ������ ��������� ������� ������
BOOL Has_TabAfterCalcAM;        // ���� ������ ��������� ������ arg max
BOOL Has_TabPrecCalc;           // ���� ������ ��������� ������ arg max

BOOL _AddMaxToArgs;             // ���������� �������� ��������� ��������� ���������� � ������� �������� ���� ����������
                                // (�.�. ��� �� ���������)

// ��������
double _Progress_PreTabCur,_Progress_PreTabMax;   // �� ������ ���������� ������� �������� ������
double _Progress_GridCur,_Progress_GridMax;   // �� ������ ���������� �������� � ������ ����������
double _Progress_PrecCur,_Progress_PrecMax;   // �� ������ ��������� ���������� � ������ ����������
double _Progress_TabAfterCalcCurS,_Progress_TabAfterCalcMaxS;  // �� ������ ��������� ������� �������� ������
double _Progress_TabAfterCalcCurAM,_Progress_TabAfterCalcMaxAM;  // �� ������ ��������� �������� ������ arg max
double _Progress_TabPrecCalcCur,_Progress_TabPrecCalcMax;  // �� ������ ��������� �������� ������ arg max

#if L57_MODELVERSION>=2
  // �����
  DWORD _dwStartTime;
  // ��������� �����
  void _ShowTime(void)
    { Time_out=((double)(GetTickCount()-_dwStartTime))/1000.0; };
#endif

// ������� ������ � ��������� ���������
#if L57_ONEARG_SIMPLE_CNT!=0
  // ���������� ������� ��������� ������� �������� ������ F(x)
  BOOL _OneargSimpleF_fill(void);
#endif
#if L57_TWOARG_SIMPLE_CNT!=0
  // ���������� ������� ��������� ������� �������� ������ F(x,y)
  BOOL _TwoargSimpleF_fill(void);
#endif
#if L57_ONEARG_ARGMAX_CNT!=0
  // ���������� ������� ��������� �������� ������ argmax(x)
  BOOL _OneargArgMaxF_fill(void);
#endif
#if L57_TWOARG_ARGMAX_CNT!=0
  // ���������� ������� ��������� �������� ������ argmax(x)
  BOOL _TwoargArgMaxF_fill(void);
#endif


// ��� �������� ������� ��������� �������
// ������ ����������� ������� ������ (1) ��� ���� (2) ����������
int _PreTab_NArg;
// ������� ����������� ������� ������ ���������
int _PreTab_OneArg_Cur;
// ������� ����������� ������� ���� ����������
int _PreTab_TwoArg_Cur;

// ��� �������� ��������� ������� Arg Max
// ������ ����������� ������� ������ (1) ��� ���� (2) ����������
int _ArgMax_NArg;
// ������� ����������� ������� ������ ���������
int _ArgMax_OneArg_Cur;
// ������� ����������� ������� ���� ����������
int _ArgMax_TwoArg_Cur;
// ������� �������������� ����� ������� Arg Max ��� ���������
L57_MaxCandidate _ArgMax_TablePoint;
// ���������� ���� �����
int _ArgMax_TP_Index1,_ArgMax_TP_Index2;

// ���������� ���������� ���� ������� ��������� �������
void _SetAllSimpleTableFuncReady(char flag);
// �������� �������� ������� ���� ��������� �������
void _ClearAllTableFunc(void);
// �������� ��������� ����������
BOOL L57InputsChange(BOOL _force,BOOL *pChgSignalOn);
// ��������� ������������ ���������� � ����������� �������
void L57InputsToCalc(void);
// ��������� ������� �������� ���������� � ����������� �������
void L57ParamsToCalc(void);
// ������������� ������ ��������������� ����������
void L57InitParams(void);
// ������ ������������ (�����. ���������� �������������)
BOOL L57StartIterations(void);
// �������� ��� ��������� ������
void L57ClearTempData(void);
// �������� ��� �������� ������ (��� ������)
void L57ClearOutputData(void);
// �������� ��������� �������� ������
void L57SetErrorOutputData(void);
// �������� ������� �������� ������
void L57SetOutputData(void);
// ��������� "���� ������" � �������
void L57OutOfMemMsg(int n);
// ����� ���������
void Model_ShowProgress(void);
// �������� ������� ����� � ������ ������� ����������, ���� ����� (������������ ������ ��� ����������� ���������)
void Model_CurPointToAllMax_NoDeep(double f);
// �������� ������ �������� �� _Candidates � _FinalMax
void Model_FirstCandidateToFinal(void);
// ������� � ��������� ������
void Model_NextStage(BOOL substageonly=FALSE);
// ����� ��������� ������� �������
// ������� - ���� ��� ��� ���������
BOOL Model_NextSimpleTable(BOOL fromzero=TRUE);
// ����� ��������� ������� ArgMax
// ������� - ���� ��� ��� ���������
BOOL Model_NextArgMaxTable(BOOL fromzero=TRUE);
// ���� ������� - ������ ���������� ���������� ������� �������� ������
void Model_PreTab(void);
// ���� ������� - ������ ���������� ��������
void Model_GridStage(void);
// ���� ������� - ������ ���������� �������� - ��������� ����������� � ����������
void Model_GridStage_Fill(void);
// ���� ������� - ������ ���������� �������� - ��������� ������ ����������
void Model_GridStage_Cand(void);
// ���� ������� - ������ ��������� ���������� � ���������
void Model_PrecStage(void);
// ���� ������� - ������ ��������� ������� ������
void Model_TabAfterCalcSimp(void);
// ���� ������� - ������ ��������� ������ arg max
void Model_TabAfterCalcAM(void);
// ���� ������� - ������ ��������� ������
void Model_TabPrecCalc(void);
// �������� ��������� � ������� �������� ���������� ����������
BOOL Model_AddAllMaxToParameters(void);
#if L57_ONEARG_ARGMAX_CNT!=0
  // ��������� ����� ����� ���������� �������
  BOOL Model_TabPrecCalc_OneArgPoint(void);
#endif
#if L57_TWOARG_ARGMAX_CNT!=0
  // ��������� ����� ����� ��������� �������
  BOOL Model_TabPrecCalc_TwoArgPoint(void);
#endif
// ���������� ��������� �� AllMax_out � ��������� ������� (������������)
BOOL L57AllMaxToArrays(void);


#endif // #ifdef L57MS_CLASS
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// ���������� ����������
//---------------------------------------------------------------------------
#ifdef L57MS_SAVEPARAMS
  switch(rdsGetSystemInt(RDS_GSIUNDOACTION))
    { case RDS_UA_SETUPBLOCK: // ��������� �������� ����� (����� ���������� ������)
      #ifdef RDS_UA_TRANSLATE
      case RDS_UA_TRANSLATE: // ����������� �������� ������� (���� � ������)
      #endif
        L57_SaveModelToUndo(rdsbcppBlockData->Block);
        break;
    }
#endif // #ifdef L57MS_SAVEPARAMS
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// �������� ����������
//---------------------------------------------------------------------------
#ifdef L57MS_LOADPARAMS

#if L57_MODELVERSION>=2
  if(rdsGetSystemInt(RDS_GSISAVELOADACTION)==RDS_LS_LOADAUTOCOMP)
    { // ��� ����������� ������ ����� �������������� ������� �������� ������� �� ������
      L57_SendSingleFunction(rdsbcppBlockData->Block,Func_out.Num(),TRUE, // Immediate=TRUE !
                             _CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str(),NULL);
    }
#endif
L57_LoadModelFromUndo(rdsbcppBlockData->Block,LoadedText);
if(_InParamsChange && (rdsbcppBlockData->Flags & RDS_VARCHECKFAILED)==0) // � ����������� ��� � �������
  _Start=1;

  #ifdef L57_TITLES
    // ��������� �������� ����
    if(L57_AllowTextRectEdit)
      L57_ApplyFunctionToTextRect(rdsbcppBlockData->Block,TR_params,TR_text.c_str(),
        L57_TITLES
        );
    #ifdef RDS_LOCKTEXTRECT
      rdsbcppBlockData->Flags=RDS_SETFLAG(rdsbcppBlockData->Flags,
                                RDS_LOCKTEXTRECT,
                                L57_AllowTextRectEdit && TR_params>L57_BLOCKTEXT_NOCONTROL);
    #endif
  #endif
  
#endif // #ifdef L57MS_LOADPARAMS
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����� �������������
//---------------------------------------------------------------------------
#ifdef L57MS_CALCMODE
  _TransferError=FALSE;
  #if L57_MODELVERSION>=3
    // � ���� �� ������� ���������� ���� ������� �����?
    _Transfer.CheckInputConnections(rdsbcppBlockData->Block);
    // ��� �������� ������� �� ����� �����?
    _Transfer.ListFunctionDestinations(rdsbcppBlockData->Block,Func_out.Num());
  #endif
  if(_AcceptRecursionError)
    { _AcceptRecursionError=FALSE;
      _TransferError=TRUE;
      rdsBlockMessageBox(NULL,"������ ������ ������� �� �����: ����� �������� � ������",
                         L57_MSGCAPTION,MB_OK|MB_ICONWARNING);

    }

#endif // #ifdef L57MS_CALCMODE
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// ����� �������� ����������
//---------------------------------------------------------------------------
#ifdef L57MS_VARCHECK
  #if L57_MODELVERSION>=3
    L57_Create_Trans_Data();
  #endif
#endif // #ifdef L57MS_VARCHECK
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����� �������
//---------------------------------------------------------------------------
#ifdef L57MS_RESET
  #if L57_MODELVERSION>=3
    // ������������� ������������� ����������
    L57_Init_Fixed_Data();
    // ������������� ����������� ����������
    L57_Init_Old_Data();
  #endif
#endif // #ifdef L57MS_RESET
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// �������������� ���������
//---------------------------------------------------------------------------
#ifdef L57MS_DRAWADDITIONAL
  if(_TransferError)
    { int w,h;
      DWORD icon=RDS_STDICON_YELCIRCEXCLAM;
      if(rdsXGGetStdIconSize(icon,&w,&h))
        rdsXGDrawStdIcon(DrawData->Left+(DrawData->Width-w)/2,
                         DrawData->Top+(DrawData->Height-h)/2,
                         icon);
    }
#endif // #ifdef L57MS_DRAWADDITIONAL
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����������� ���������
//---------------------------------------------------------------------------
#ifdef L57MS_HINT
  if(_TransferError)
    { rdsSetHintText("������ ������ ������� �� �����: ����� �������� � ������");
      Show=1;
    }
#endif // #ifdef L57MS_HINT
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������ �������
//---------------------------------------------------------------------------
#ifdef L57MS_START
  if(rdsCalcProcessNeverStarted())
    _InParamsChange=TRUE;
  _Start=1;
#endif // #ifdef L57MS_START
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����������� ������ �����
//---------------------------------------------------------------------------
#ifdef L57MS_INIT
  #if L57_MODELVERSION<L57_LATESTMODELVERSION
    // ��������� � ����������� ����������
    L57_ExecTool_UpdatePossible();
  #endif
  // ����� ������� � ��������� ��� �������� ��������� ����������
  rdsbcppBlockData->Flags|=RDS_ALWAYSSETUP|RDS_ALWAYSLOADSAVE;
  // ��������� ������������ ���������� (��� ������ ������������)
  _InParamsChange=TRUE;
  // ����� ������ �������� ��������
  _InsideAcceptFunction=_AcceptRecursionError=_InsideAcceptParam=_AcceptParamRecursionError=_TransferError=FALSE;
  #if L57_MODELVERSION>=3
    // ������������� ������������� ����������
    L57_Init_Fixed_Data();
    // ����� ���������� ��������
    L57_Reset_Trans_Data();
    // ������������� ����������� ����������
    L57_Init_Old_Data();
  #endif
  #ifdef L57_TITLES
    L57_AllowTextRectEdit=L57_ExecTool_CanEditTextRect();
  #endif
#endif
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------
#ifdef L57MS_MODEL
  BOOL enabled=TRUE,dostart=FALSE,_somethingchanged=FALSE;

  // ������� ��������� ������ ���� �������
  _Ready=0;

  // �������� ���������� ������
  if(_Enabled_chg)
    { int size=Enabled_in.Size();
      _Enabled_chg=0;
      #if L57_MODELVERSION>=2
        _enabled_res=1;
      #endif
      for(int i=0;enabled && i<size;i++)
        if(!Enabled_in[i])
          { enabled=FALSE;
            #if L57_MODELVERSION>=2
              _enabled_res=0;
            #endif
          }
    }
  #if L57_MODELVERSION>=2
  else
    enabled=_enabled_res;
  #endif

  if(!enabled)
    return;

  // ������ ��������� - ���������� �� ������� ���������
  if(L57InputsChange(FALSE,&_somethingchanged))
    _InParamsChange=TRUE;

  if(Restart_cmd) // �������������� ����������
    dostart=TRUE;
  else // ��������������� ����������� ���
    { if(Start_cmd)
        _somethingchanged=TRUE;
      if(noautostart_setup) // �� ��������� ������������� ��� ���������� ������
        { dostart=(Start_cmd!=0) && _InParamsChange;
          Start_cmd=0;
        }
      else // ��������� ������������� ���� ���������� ������
        dostart=_InParamsChange;
    }

  if(dostart)
    { // ������ ������������
      _InParamsChange=FALSE;
      Restart_cmd=0;
      Start_cmd=0;
      if(!L57StartIterations())
        { _Stage=L57STAGE_WAIT;
          return;
        }
      #if L57_MODELVERSION>=2
        Started_out=1;
        _dwStartTime=GetTickCount();
      #endif
    }
  else
    { if(_Stage==L57STAGE_WAIT && _somethingchanged && (!noautostart_setup)) // �� ��������� ������������ � �� ��������
        {
          #if L57_MODELVERSION>=2
            Finished_out=1;
            Complete_out=1;
            Error_out=0;
            Time_out=0;
          #endif
          _Ready=1;
          return;
        }
    }

  switch(_Stage)
    { case L57STAGE_WAIT:
        return;
      case L57STAGE_PRETAB:
        Model_PreTab();
        break;
      case L57STAGE_GRID:
        Model_GridStage();
        break;
      case L57STAGE_PREC:
        Model_PrecStage();
        break;
      case L57STAGE_TABAFTERCALCSIMP:
        Model_TabAfterCalcSimp();
        break;
      case L57STAGE_TABAFTERCALCAM:
        Model_TabAfterCalcAM();
        break;
      case L57STAGE_TABPRECCALC:
        Model_TabPrecCalc();
        break;
      default:
        rdsStopCalc();
        L57ClearTempData();
        L57ClearOutputData();
        _Stage=L57STAGE_WAIT;
        rdsBlockMessageBox(NULL,"���������� ������ ������������� - ����������� ����� ������",
                      L57_MSGCAPTION,MB_OK|MB_ICONERROR);
        return;
    }
  Model_ShowProgress();
  _Ready=1;
  _Start=1;
  #if L57_MODELVERSION>=2
    _ShowTime();
  #endif
#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// �������� ����� ������
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS

// ��������� "���� ������" � �������
void rdsbcppBlockClass::L57OutOfMemMsg(int n)
{ L57_String msg,nstr;
  rdsStopCalc();
  L57ClearTempData();
  L57ClearOutputData();
  _Stage=L57STAGE_WAIT;
  nstr.AssignInt(n);
  msg.Assign("���� ������ ��� ������ ������������� (");
  msg.AddString(nstr);
  msg.AddString(")");
  rdsBlockMessageBox(NULL,msg.c_str(),L57_MSGCAPTION,MB_OK|MB_ICONERROR);
}
//---------------------------------------------------------------------------

// �������� ��� ��������� ������
void rdsbcppBlockClass::L57ClearTempData(void)
{
  _Params.ClearTempData();
  _Candidates.Clear();
  _FinalMax.Clear();
  #if L57_ONEARG_SIMPLE_CNT!=0
  for(int i=0;i<L57_ONEARG_SIMPLE_CNT;i++)
    _OneargSimpleF[i].Clear();
  #endif
  #if L57_TWOARG_SIMPLE_CNT!=0
  for(int i=0;i<L57_TWOARG_SIMPLE_CNT;i++)
    _TwoargSimpleF[i].Clear();
  #endif
  #if L57_ONEARG_ARGMAX_CNT!=0
  for(int i=0;i<L57_ONEARG_ARGMAX_CNT;i++)
    _OneargArgMaxF[i].Clear();
  #endif
  #if L57_TWOARG_ARGMAX_CNT!=0
  for(int i=0;i<L57_TWOARG_ARGMAX_CNT;i++)
    _TwoargArgMaxF[i].Clear();
  #endif
}
//---------------------------------------------------------------------------

// �������� ��� �������� ������ (��� ������)
void rdsbcppBlockClass::L57ClearOutputData(void)
{
  AllMax_out.Resize(0,0);
  L57AllMaxToArrays();
  _results_ok=0; // MaxReady
  _SetAllSimpleTableFuncReady(0);
  _ClearAllTableFunc();
  Progress_out=0.0;
  #if L57_MODELVERSION>=2
    Complete_out=0;
    Error_out=0;
  #endif
}
//---------------------------------------------------------------------------

// �������� ��������� �������� ������
void rdsbcppBlockClass::L57SetErrorOutputData(void)
{
  AllMax_out.Resize(0,0);
  L57AllMaxToArrays();
  _ClearAllTableFunc();
  _results_ok=1;
  _SetAllSimpleTableFuncReady(0);
  Progress_out=0.0;
  #if L57_MODELVERSION>=2
    Complete_out=Error_out=Finished_out=1;
  #endif
}
//---------------------------------------------------------------------------

// �������� ������� �������� ������
void rdsbcppBlockClass::L57SetOutputData(void)
{ BOOL ok=TRUE;
#ifdef L57_PARAMSCOUNT
  int n_max=_FinalMax.Count();
  #ifdef L57_LOG
    //L57_String s;
    L57_Log_Add(_LOGBLK,"  ������ AllMax_out");
  #endif
  //                   rows  cols
  if(AllMax_out.Resize(n_max,L57_PARAMSCOUNT+1))
    { double *arr=AllMax_out.GetBuffer();
      int shift=(L57_PARAMSCOUNT+1);
      for(L57_MaxCandidate *f=_FinalMax.First;f!=NULL;f=f->Next,arr+=shift)
        { memcpy(arr,f->CurParams,L57_PARAMSCOUNT*sizeof(double));
          arr[L57_PARAMSCOUNT]=f->f;
        }
      L57_SortMatrixColDesc(AllMax_out.GetBuffer(),AllMax_out.Rows(),AllMax_out.Cols(),L57_PARAMSCOUNT);
      if(!L57AllMaxToArrays())
        ok=FALSE;
    }
  else
    { AllMax_out.Resize(0,0);
      L57AllMaxToArrays();
      ok=FALSE;
    }



  #if L57_ONEARG_SIMPLE_CNT!=0
    // �������������� ������� ������� ����� ����������
    #ifdef L57_LOG
      L57_Log_Add(_LOGBLK,"  ������ F(x)");
    #endif
    for(int i=0;ok && i<L57_ONEARG_SIMPLE_CNT;i++)
      if(!_OneargSimpleF[i].SendToOutput(FALSE))
        ok=FALSE;
  #endif

  #if L57_TWOARG_SIMPLE_CNT!=0
    // �������������� ������� ������� ���� ����������
    #ifdef L57_LOG
      L57_Log_Add(_LOGBLK,"  ������ F(x,y)");
    #endif
    for(int i=0;ok && i<L57_TWOARG_SIMPLE_CNT;i++)
      if(!_TwoargSimpleF[i].SendToOutput(FALSE))
        ok=FALSE;
  #endif

  #if L57_ONEARG_ARGMAX_CNT!=0
    // �������������� ������� arg max ����� ����������
    #ifdef L57_LOG
      L57_Log_Add(_LOGBLK,"  ������ argmax(x)");
    #endif
    for(int i=0;ok && i<L57_ONEARG_ARGMAX_CNT;i++)
      if(!_OneargArgMaxF[i].SendToOutput(FALSE))
        ok=FALSE;
  #endif

  #if L57_TWOARG_ARGMAX_CNT!=0
    // �������������� ������� arg max ����� ����������
    #ifdef L57_LOG
      L57_Log_Add(_LOGBLK,"  ������ argmax(x,y)");
    #endif
    for(int i=0;ok && i<L57_TWOARG_ARGMAX_CNT;i++)
      if(!_TwoargArgMaxF[i].SendToOutput(FALSE))
        ok=FALSE;
  #endif

  if(!ok)
    L57OutOfMemMsg(5);
#endif // #ifdef L57_PARAMSCOUNT

  _results_ok=1;
  _SetAllSimpleTableFuncReady(1);
  Progress_out=1.0;
  #if L57_MODELVERSION>=2
    Complete_out=Finished_out=1;
    Error_out=ok?0:1;
  #endif
}
//---------------------------------------------------------------------------


// ������ ������������ (�����. ���������� �������������)
#ifdef L57_PARAMSCOUNT
BOOL rdsbcppBlockClass::L57StartIterations(void)
{ BOOL _badalloc=FALSE,_error=FALSE;
  int _pc=0;
  double chunk_bytes;

  _GlobalMax=rdsbcppHugeDouble;
  #if L57_MODELVERSION>=2
    Complete_out=Error_out=Finished_out=0;
  #endif

  #ifdef L57_LOG
    L57_Log_Clear(_LOGBLK);
    L57_Log_Add(_LOGBLK,"������ ������");
  #endif

  // ������� ����
  #if L57_ONEARG_SIMPLE_CNT!=0 || L57_TWOARG_SIMPLE_CNT!=0
    _Stage=L57STAGE_PRETAB;
  #else
    _Stage=L57STAGE_GRID;
    _SubStage=L57SUBSTAGE_GRIDFILL;
  #endif

  _PreTab_OneArg_Cur=_PreTab_TwoArg_Cur=-1;
  _PreTab_NArg=0;
  _ArgMax_OneArg_Cur=_ArgMax_TwoArg_Cur=-1;
  _ArgMax_NArg=0;

  // ������� �������������� ������
  #if L57_ONEARG_SIMPLE_CNT!=0 || L57_TWOARG_SIMPLE_CNT!=0
    // ���� ������ ��������� ������� ������
    Has_TabAfterCalcSimp=TabAfterCalc_setup && deepsearch_setup; // ��� ��������� ���������� ��� ������ ����� � �����, ������ ���������
  #else
    Has_TabAfterCalcSimp=FALSE;
  #endif
  #if L57_ONEARG_ARGMAX_CNT!=0 || L57_TWOARG_ARGMAX_CNT!=0
    // ���� ������ ��������� ������ arg max (������� ����� ���������� �������������� �� ����)
    Has_TabAfterCalcAM=TabAfterCalc_setup && deepsearch_setup; // ��� ��������� ���������� ��� ������ ����� � �����, ������ ���������
  #else
    Has_TabAfterCalcAM=FALSE;
  #endif
  #if L57_ONEARG_ARGMAX_CNT!=0 || L57_TWOARG_ARGMAX_CNT!=0
    // ���� ������ ��������� ������
    Has_TabPrecCalc=TabPrecCalc_setup;
  #else
    Has_TabPrecCalc=FALSE;
  #endif

  // ��������� ��������� ���������� ��������� � ������� ����������, ������ ���� ���� �����-�� ��������
  _AddMaxToArgs=Has_TabAfterCalcSimp||Has_TabAfterCalcAM;

  // ��������
  _Progress_PreTabCur=0.0;
  _Progress_PreTabMax=0.0;
  _Progress_GridCur=0.0;
  _Progress_GridMax=1.0;
  _Progress_PrecCur=0.0;
  _Progress_PrecMax=1.0;
  _Progress_TabAfterCalcCurS=0.0;
  _Progress_TabAfterCalcMaxS=Has_TabAfterCalcSimp?1.0:0.0;
  _Progress_TabAfterCalcCurAM=0.0;
  _Progress_TabAfterCalcMaxAM=Has_TabAfterCalcAM?1.0:0.0;
  _Progress_TabPrecCalcCur=0.0;
  _Progress_TabPrecCalcMax=Has_TabPrecCalc?1.0:0.0;

  L57InputsToCalc();

  L57ClearTempData();
  L57ClearOutputData();
  L57InitParams();

  // �������� ����������� ������������
  for(int i=0;(!_error) && i<L57_PARAMSCOUNT;i++,_pc++)
    { L57_Parameter *p=_Params.P+i;
      // �������� ��������� �������� ��� ����������� ����������
      if(p->StartStep==rdsbcppHugeDouble)
        p->StartStep=1.0;
      if(p->EndStep==rdsbcppHugeDouble)
        p->EndStep=p->StartStep;
      // ������ ���������� ��������
      if(p->Min==rdsbcppHugeDouble)
        { _error=TRUE;
          // �������� ���������?
        }
      else if(p->Max!=rdsbcppHugeDouble && p->Max<p->Min)
        { _error=TRUE;
          // �������� ���������?
        }
   }
  if(_error || _pc==0)
    { L57ClearTempData();
      L57SetErrorOutputData();
      return FALSE;
    }

  // ������� ������� ����������
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=_Params.P+i;
      if(p->AllocateArray())
        p->EvalChunkSize=p->Array.Count;
      else
        _badalloc=TRUE;
      p->_ArgIndex=p->_ChunkIndex=0;
    }
  if(_badalloc)
   { rdsStopCalc();
     L57ClearTempData();
     L57ClearOutputData();
     _Stage=L57STAGE_WAIT;
     rdsBlockMessageBox(NULL,"������� ������� ��������� ���� �� ����� ��� ���������� ����������, �� ������� ������ ��� "
                        "���������� ������.",
                        L57_MSGCAPTION,MB_OK|MB_ICONERROR);
     return FALSE;
   }

  // ��������� ������� ������� ����������� �������
  _Progress_PreTabMax=0.0;
  #if L57_ONEARG_SIMPLE_CNT!=0
  if(_OneargSimpleF_fill())
    { for(int i=0;i<L57_ONEARG_SIMPLE_CNT;i++)
        _Progress_PreTabMax+=_OneargSimpleF[i].ArgsArray->Count;
    }
  else
    _badalloc=TRUE;
  #endif

  #if L57_TWOARG_SIMPLE_CNT!=0
  if(_TwoargSimpleF_fill())
    { for(int i=0;i<L57_TWOARG_SIMPLE_CNT;i++)
        _Progress_PreTabMax+=_TwoargSimpleF[i].TotalArgs();
    }
  else
    _badalloc=TRUE;
  #endif

  #if L57_ONEARG_ARGMAX_CNT!=0
  if(!_OneargArgMaxF_fill())
    _badalloc=TRUE;
  #endif

  #if L57_TWOARG_ARGMAX_CNT!=0
  if(!_TwoargArgMaxF_fill())
    _badalloc=TRUE;
  #endif

  if(_badalloc)
   { rdsStopCalc();
     L57ClearTempData();
     L57ClearOutputData();
     _Stage=L57STAGE_WAIT;
     rdsBlockMessageBox(NULL,"������������ ������ ��� ������������ �������� ��������� �������.",
                        L57_MSGCAPTION,MB_OK|MB_ICONERROR);
     return FALSE;
   }

  // ��������� ��������� � ���������� �� ������ �����, ���������� �� ������
  if(ChunkSizeMb_setup<1.0)
    ChunkSizeMb_setup=1.0;
  chunk_bytes=ChunkSizeMb_setup*1024.0*1024.0; // � ������
  //                      chunk_bytes=20*sizeof(double);
  //                      chunk_bytes=1000*sizeof(double);
  //                      chunk_bytes=38000;
  if(deepsearch_setup)
    { // ����������� ������� "�����" ��������� �� ������� ���������
      // ������� � EvalChunkSize �������� ������ ����� ��������
      for(;;)
        { double total=1.0;
          int maxchunk_index,maxchunk=-1;
          // ���������� ������ ������ ��� ����� �������
          for(int i=0;i<L57_PARAMSCOUNT;i++)
            { L57_Parameter *p=_Params.P+i;
              total*=p->EvalChunkSize;
              if(p->EvalChunkSize>maxchunk)
                { maxchunk=p->EvalChunkSize;
                  maxchunk_index=i;
                }
            }
          total*=sizeof(double);
          // ����� ������ total ������
          if(total<=chunk_bytes) // ���������
            break;
          // ������������ ����� ����� - maxchunk � ��������� maxchunk_index
          if(maxchunk<=2) // �����-�� ������
            { L57OutOfMemMsg(1);
              return FALSE;
            }
          // ����� ����� ������� ����� �������
          total=ceil(((double)maxchunk)/2);
          _Params.P[maxchunk_index].EvalChunkSize=(int)total;
        } // for(;;)
    }
  // �� ������ ������ � EvalChunkCount ������� ��������� ������� ������ ����������� �� ������ �����
  // ������� �� ����� ���������

  // ������� ������� ������
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=_Params.P+i;
      double totalpoints=0;
      if(!p->AllocateChunkArray())
        { L57OutOfMemMsg(2);
          return FALSE;
        }
      for(int j=0;j<p->ChunkCount;j++)
        totalpoints+=(p->ChunkEnd[j]-p->ChunkStart[j]+1);
      _Progress_GridMax*=totalpoints;
    }
  if(deepsearch_setup)
    _Progress_GridMax*=2; // ������ ����� �������� �� ��� ����: ��� ���������� � ��� ������

  if(deepsearch_setup)
    { // ������� ������ ��� �������� ������� � ������� �����
      if(!_Params.CreateChunkValuesArray())
        { L57OutOfMemMsg(3);
          return FALSE;
        }
      _Params.ResetChunkValuesArray();
    }

  if(_badalloc)
   { L57ClearTempData();
     L57ClearOutputData();
     L57OutOfMemMsg(4);
     return FALSE;
   }

  return TRUE;
}
#else // #ifdef L57_PARAMSCOUNT
BOOL rdsbcppBlockClass::L57StartIterations(void)
{ rdsStopCalc();
  L57ClearTempData();
  L57ClearOutputData();
  _Stage=L57STAGE_WAIT;
  rdsBlockMessageBox(NULL,"�� ������ ��������������� ���������",
                     L57_MSGCAPTION,MB_OK|MB_ICONWARNING);
  return FALSE;
}
#endif
//---------------------------------------------------------------------------

// ������� ��������� ����� ������ �� ���� ����������� ����������
void Setup_GetOptionsString(RDS_HOBJECT win,L57_String &options)
{ BOOL searchendsteps=(rdsGetObjectInt(win,L57SETUPFIELD_DEEPSEARCH,RDS_FORMVAL_VALUE)!=0) &&
                      (rdsGetObjectInt(win,L57SETUPFIELD_PRECENDSTEPS,RDS_FORMVAL_VALUE)!=0);
  options.Clear();
  options.Assign("EndStep=");
  options.AddString(searchendsteps?"1":"0");
}
//---------------------------------------------------------------------------

// ������� ��������� ���������� �������� � ���� ���������
void Setup_FillFuncDescr(RDS_HOBJECT win,L57_String &Func,L57_String &Inputs,L57_String &Numbered,L57_String &Title)
{ L57_String str;
  // ������������ ������� ����������
  if(Numbered.IsNotEmpty())
    { str.Assign("��������������� ���������: ");
      str.AddString(Numbered);
      str.AddString(".");
    }
  if(Inputs.IsNotEmpty())
    { if(str.IsNotEmpty())
        str.AddString("\n");
      str.AddString("����� ����� (������������ ���������): ");
      str.AddString(Inputs);
      str.AddString(".");
    }

  rdsSetObjectStr(win,L57SETUPFIELD_TITLE,RDS_FORMVAL_VALUE,Title.c_str());
  rdsSetObjectStr(win,L57SETUPFIELD_CODE,RDS_FORMVAL_VALUE,Func.c_str());
  rdsSetObjectStr(win,L57SETUPFIELD_PARAMS,RDS_FORMVAL_VALUE,str.c_str());
}
//---------------------------------------------------------------------------

// ������� ��������� ������ ���� ���������
void RDSCALL Setup_WinCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data)
{ char *modeltext,*templatetext,*lock=NULL,*auxdata;
  L57_String Func,Inputs,Numbered,Title,newmodeltext,options;
  BOOL deepsearch,endsteps,df;
  BOOL tab_deepsearch,tab_endsteps,tab_df;
  int starttab=1,res;
  RDS_BHANDLE show;
  #if L57_MODELVERSION>=2
    // ��������� ���� - ���������� ��������������
    lock=rdsGetObjectStr(win,L57SETUPFIELD_LOCKEDIT,RDS_FORMVAL_VALUE);
  #endif

  #if L57_MODELVERSION>=3
    // ��������� ����: �������������� ������ � �������������� �����
    auxdata=rdsGetObjectStr(win,L57SETUPFIELD_ADDITIONALDATA,RDS_FORMVAL_VALUE);
  #else
    auxdata=NULL;
  #endif


  switch(data->Event)
    {
      #if L57_MODELVERSION>=3
      case RDS_FORMSERVEVENT_TCREOPEN:   // �������� ����� ���������� ��������
        templatetext=NULL;
      #endif
      case RDS_FORMSERVEVENT_CLICK:
        switch(data->CtrlId)
          { case L57SETUPFIELD_OUTTABBTN: // ����� �������������� ��������� �������
              starttab=2;
              break;
          }
        if(data->Event==RDS_FORMSERVEVENT_CLICK)
          templatetext=rdsGetObjectStr(win,L57SETUPFIELD_TEMPLATE,RDS_FORMVAL_VALUE);
        modeltext=rdsGetObjectStr(win,L57SETUPFIELD_MODEL,RDS_FORMVAL_VALUE);
        Setup_GetOptionsString(win,options);
        show=NULL;
        res=L57_ExecTool_Edit(templatetext,modeltext,options.c_str(),starttab,&newmodeltext,
                              &Func,&Inputs,&Numbered,&Title,lock,NULL,auxdata
                              #if L57_MODELVERSION>=3
                              ,&show
                              #endif
                              );
        if(res<0)
          { // ����� �����
            char *fullname;
            if(show==NULL) // ������ ����������
              break;
            // �������� ��������� ���� ��� ������
            fullname=rdsCreateFullBlockNameString(show,NULL);
            rdsSetObjectStr(win,L57SETUPFIELD_BLOCKTOSHOW,RDS_FORMVAL_VALUE,fullname);
            rdsFree(fullname);
            rdsCommandObjectEx(win,RDS_FORM_TEMPCLOSE,0,NULL);
            break;
          }
        if(!res)
          break;
        if(newmodeltext.IsEmpty())
          break;
        if(strcmp(newmodeltext.c_str(),modeltext)==0) // ��� ���������
          break;
        rdsSetObjectStr(win,L57SETUPFIELD_MODEL,RDS_FORMVAL_VALUE,newmodeltext.c_str());
        Setup_FillFuncDescr(win,Func,Inputs,Numbered,Title);
        break;

      case RDS_FORMSERVEVENT_CHANGE: // ��������� ����
        deepsearch=(rdsGetObjectInt(win,L57SETUPFIELD_DEEPSEARCH,RDS_FORMVAL_VALUE)!=0);
        endsteps=(rdsGetObjectInt(win,L57SETUPFIELD_PRECENDSTEPS,RDS_FORMVAL_VALUE)!=0);
        df=(rdsGetObjectInt(win,L57SETUPFIELD_PRECDF,RDS_FORMVAL_VALUE)!=0);
        rdsSetObjectInt(win,L57SETUPFIELD_NOPLATEAU,RDS_FORMVAL_ENABLED,deepsearch);
        rdsSetObjectInt(win,L57SETUPFIELD_PRECENDSTEPS,RDS_FORMVAL_ENABLED,deepsearch);
        rdsSetObjectInt(win,L57SETUPFIELD_PRECDF,RDS_FORMVAL_ENABLED,deepsearch);
        rdsSetObjectInt(win,L57SETUPFIELD_PRECGRID,RDS_FORMVAL_ENABLED,deepsearch && endsteps);
        rdsSetObjectInt(win,L57SETUPFIELD_DFCYCLES,RDS_FORMVAL_ENABLED,deepsearch && df);
        rdsSetObjectInt(win,L57SETUPFIELD_DFVALUE,RDS_FORMVAL_ENABLED,deepsearch && df);

        tab_deepsearch=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECCALC,RDS_FORMVAL_VALUE)!=0);
        tab_endsteps=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECENDSTEPS,RDS_FORMVAL_VALUE)!=0);
        tab_df=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECDF,RDS_FORMVAL_VALUE)!=0);
        rdsSetObjectInt(win,L57SETUPFIELD_TABPRECENDSTEPS,RDS_FORMVAL_ENABLED,tab_deepsearch);
        rdsSetObjectInt(win,L57SETUPFIELD_TABPRECDF,RDS_FORMVAL_ENABLED,tab_deepsearch);
        rdsSetObjectInt(win,L57SETUPFIELD_TABPRECGRID,RDS_FORMVAL_ENABLED,tab_deepsearch && tab_endsteps);
        rdsSetObjectInt(win,L57SETUPFIELD_TABDFCYCLES,RDS_FORMVAL_ENABLED,tab_deepsearch && tab_df);
        rdsSetObjectInt(win,L57SETUPFIELD_TABDFVALUE,RDS_FORMVAL_ENABLED,tab_deepsearch && tab_df);

        #if L57_MODELVERSION>=9
          if(L57_AllowTextRectEdit)
            L57_TextRectEdit_EnableControls(win);
        #endif
        break;

      #if L57_MODELVERSION>=3
      case RDS_FORMSERVEVENT_TCREACTION: // ������� ����� ���������� ��������
        show=rdsBlockByFullName(rdsGetObjectStr(win,L57SETUPFIELD_BLOCKTOSHOW,RDS_FORMVAL_VALUE),NULL);
        L57_SelectAndShowBlock(show);
        break;
      #endif
    }
}
//---------------------------------------------------------------------------

// ����� ��������� ������� �������
// ������� - ���� ��� ��� ���������
BOOL rdsbcppBlockClass::Model_NextSimpleTable(BOOL fromzero)
{
#ifdef L57_PARAMSCOUNT
  for(;;)
    switch(_PreTab_NArg)
      { case 1: // ������� ������ ���������
          _PreTab_OneArg_Cur++;
          if(_PreTab_OneArg_Cur<L57_ONEARG_SIMPLE_CNT)
            { // ����� �������� �� ������
              #if L57_ONEARG_SIMPLE_CNT!=0
                L57_Table1ArgFuncData *f=_OneargSimpleF+_PreTab_OneArg_Cur;
                f->_CurIndex=fromzero?0:f->ArgsArray->PrevCount;
                if(f->_CurIndex>=f->ArgsArray->Count)
                  continue;
                for(int i=0;i<L57_PARAMSCOUNT;i++)
                  { L57_Parameter *p=_Params.P+i;
                    p->Current=p->FuncVal;
                  }
              #endif
              return TRUE;
            }
          _PreTab_NArg=2;
          _PreTab_TwoArg_Cur=-1;
          break;
        case 2: // ������� ���� ����������
          _PreTab_TwoArg_Cur++;
          if(_PreTab_TwoArg_Cur<L57_TWOARG_SIMPLE_CNT)
            { // �������� ��� ������� �� ������
              #if L57_TWOARG_SIMPLE_CNT!=0
                L57_Table2ArgFuncData *f=_TwoargSimpleF+_PreTab_TwoArg_Cur;
                if(fromzero)
                  f->_CurIndex1=f->_CurIndex2=0;
                else
                  { if(f->Args2Array->PrevCount==f->Args2Array->Count) // ��� "������������ ������" ������
                      { f->_CurIndex1=f->Args1Array->PrevCount;
                        f->_CurIndex2=0;
                      }
                    else // ���� ������������ ������
                      { f->_CurIndex1=0;
                        f->_CurIndex2=f->Args2Array->PrevCount;
                      }
                  }
                if(f->_CurIndex1>=f->Args1Array->Count ||
                   f->_CurIndex2>=f->Args2Array->Count)
                  continue;
                for(int i=0;i<L57_PARAMSCOUNT;i++)
                  { L57_Parameter *p=_Params.P+i;
                    p->Current=p->FuncVal;
                  }
              #endif
              return TRUE;
            }
          return FALSE;
        default: // ����� ������
          _PreTab_NArg=1;
          _PreTab_OneArg_Cur=-1;
    }
#else // #ifdef L57_PARAMSCOUNT
  return FALSE;
#endif  
}
//---------------------------------------------------------------------------

// ����� ��������� ������� Arg Max
// ������� - ���� ��� ��� ���������
BOOL rdsbcppBlockClass::Model_NextArgMaxTable(BOOL fromzero)
{
#ifdef L57_PARAMSCOUNT
  for(;;)
    switch(_ArgMax_NArg)
      { case 1: // ������� ������ ���������
          _ArgMax_TP_Index1=_ArgMax_TP_Index2=-1;
          _ArgMax_OneArg_Cur++;
          if(_ArgMax_OneArg_Cur<L57_ONEARG_ARGMAX_CNT)
            { // ����� �������� �� ������
              #if L57_ONEARG_ARGMAX_CNT!=0
                L57_Table1ArgFuncData *f=_OneargArgMaxF+_ArgMax_OneArg_Cur;
                f->_CurIndex=fromzero?0:f->ArgsArray->PrevCount;
                if(f->_CurIndex>=f->ArgsArray->Count)
                  continue;
                for(int i=0;i<L57_PARAMSCOUNT;i++)
                  { L57_Parameter *p=_Params.P+i;
                    p->Current=p->FuncVal;
                  }
              #endif
              return TRUE;
            }
          _ArgMax_NArg=2;
          _ArgMax_TwoArg_Cur=-1;
          break;
        case 2: // ������� ���� ����������
          _ArgMax_TP_Index1=_ArgMax_TP_Index2=-1;
          _ArgMax_TwoArg_Cur++;
          if(_ArgMax_TwoArg_Cur<L57_TWOARG_ARGMAX_CNT)
            { // �������� ��� ������� �� ������
              #if L57_TWOARG_ARGMAX_CNT!=0
                L57_Table2ArgFuncData *f=_TwoargArgMaxF+_ArgMax_TwoArg_Cur;
                if(fromzero)
                  f->_CurIndex1=f->_CurIndex2=0;
                else
                  { if(f->Args2Array->PrevCount==f->Args2Array->Count) // ��� "������������ ������" ������
                      { f->_CurIndex1=f->Args1Array->PrevCount;
                        f->_CurIndex2=0;
                      }
                    else // ���� ������������ ������
                      { f->_CurIndex1=0;
                        f->_CurIndex2=f->Args2Array->PrevCount;
                      }
                  }
                if(f->_CurIndex1>=f->Args1Array->Count ||
                   f->_CurIndex2>=f->Args2Array->Count)
                  continue;
                for(int i=0;i<L57_PARAMSCOUNT;i++)
                  { L57_Parameter *p=_Params.P+i;
                    p->Current=p->FuncVal;
                  }
              #endif
              return TRUE;
            }
          return FALSE;
        default: // ����� ������
          _ArgMax_NArg=1;
          _ArgMax_OneArg_Cur=-1;
          _ArgMax_TP_Index1=_ArgMax_TP_Index2=-1;
    }
#else // #ifdef L57_PARAMSCOUNT
  return FALSE;
#endif
}
//---------------------------------------------------------------------------

// ���� ������� - ������ ���������� ���������� ������� �������� ������
void rdsbcppBlockClass::Model_PreTab(void)
{ L57_LoopBreak _check;
  if(_PreTab_NArg<1)
    { if(!Model_NextSimpleTable())
        { Model_NextStage();
          return;
        }
    }
  if(_PreTab_NArg<1)
    return;

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    { 
      // ������ ���������� ������
      #if L57_ONEARG_SIMPLE_CNT!=0
      if(_PreTab_NArg==1)
        { double val;
          // ��������� ����� _CurIndex ������� _PreTab_OneArg_Cur
          L57_Table1ArgFuncData *f=_OneargSimpleF+_PreTab_OneArg_Cur;
          L57_Parameter *p=_Params.P+f->ArgIndex;
          _Progress_PreTabCur++;
          // ������� �������� � ��������� (��������� ��� ��������)
          p->Current=p->Array.Data[f->_CurIndex];
          // ��������� �������
          L57ParamsToCalc();
          val=_Calculator._Calc_main();
          // ������� ������ � �������
          *(f->CurIndexBase())=val;
          // ����� ��������� ������
          f->_CurIndex++;
          if(f->_CurIndex>=f->ArgsArray->Count) // ������� ���������
            {
              if(!Model_NextSimpleTable())
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif // #if L57_ONEARG_SIMPLE_CNT!=0

      #if L57_TWOARG_SIMPLE_CNT!=0
      if(_PreTab_NArg==2)
        { double val;
          // ��������� ����� _CurIndex1,_CurIndex2 ������� _PreTab_TwoArg_Cur
          L57_Table2ArgFuncData *f=_TwoargSimpleF+_PreTab_TwoArg_Cur;
          L57_Parameter *p1=_Params.P+f->Arg1Index;
          L57_Parameter *p2=_Params.P+f->Arg2Index;
          _Progress_PreTabCur++;
          // ������� �������� � ��������� (��������� ��� ��������)
          p1->Current=p1->Array.Data[f->_CurIndex1];
          p2->Current=p2->Array.Data[f->_CurIndex2];
          // ��������� �������
          L57ParamsToCalc();
          val=_Calculator._Calc_main();
          // ������� ������ � �������
          *(f->CurIndexBase())=val;
          // ����� ��������� ������
          f->_CurIndex2++;
          if(f->_CurIndex2>=f->Args2Array->Count)
            { f->_CurIndex1++;
              f->_CurIndex2=0;
            }
          if(f->_CurIndex1>=f->Args1Array->Count) // ������� ���������
            { f->_CurIndex2=f->Args2Array->Count;
              if(!Model_NextSimpleTable())
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif
      if(_check.Check())
        break;
    } // for(int _iteration=0;...)

}
//---------------------------------------------------------------------------

// ���� ������� - ������ ��������� ������� ������
void rdsbcppBlockClass::Model_TabAfterCalcSimp(void)
{ L57_LoopBreak _check;

  if(_PreTab_NArg<1)
    { if(!Model_NextSimpleTable(FALSE))
        { Model_NextStage();
          return;
        }
      #ifdef L57_LOG
        L57_Log_Add(_LOGBLK,"  Model_TabAfterCalcSimp(): ������ ������");
      #endif
    }
  if(_PreTab_NArg<1)
    return;

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    {
      // ������ ���������� ������ (AddArgs ���������)
      #if L57_ONEARG_SIMPLE_CNT!=0
      if(_PreTab_NArg==1)
        { double val;
          L57_Table1ArgFuncData *f=_OneargSimpleF+_PreTab_OneArg_Cur;
          // ��������� ����� _CurIndex ������� _PreTab_OneArg_Cur
          #ifdef L57_LOG
            L57_String s;
            s.Assign("  ������� F(x): ");
            s.AddInt(_PreTab_OneArg_Cur);
            s.AddString(", ������ ");
            s.AddInt(f->_CurIndex);
            s.AddString(" �� ");
            s.AddInt(f->ArgsArray->Count);
            L57_Log_Add(_LOGBLK,s);
          #endif
          if(f->_CurIndex<f->ArgsArray->Count)
            { L57_Parameter *p=_Params.P+f->ArgIndex;
              _Progress_TabAfterCalcCurS++;
              // ������� �������� � ��������� (��������� ��� ��������)
              p->Current=f->ArgsArray->Data[f->_CurIndex];
              // ��������� �������
              L57ParamsToCalc();
              val=_Calculator._Calc_main();
              // ������� ������ � �������
              *(f->CurIndexBase())=val;
            }
          // ����� ��������� ������
          f->_CurIndex++;
          if(f->_CurIndex>=f->ArgsArray->Count) // ������� ���������
            {
              if(!Model_NextSimpleTable(FALSE))
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif // #if L57_ONEARG_SIMPLE_CNT!=0

      // ������ ��������� ������ (AddArgs ���������)
      #if L57_TWOARG_SIMPLE_CNT!=0
      if(_PreTab_NArg==2)
        { double val;
          // ��������� ����� _CurIndex1,_CurIndex2 ������� _PreTab_TwoArg_Cur
          L57_Table2ArgFuncData *f=_TwoargSimpleF+_PreTab_TwoArg_Cur;
          #ifdef L57_LOG
            L57_String s;
            s.Assign("  ������� F(x,y): ");
            s.AddInt(_PreTab_TwoArg_Cur);
            s.AddString(", ������� (");
            s.AddInt(f->_CurIndex1);
            s.AddString(",");
            s.AddInt(f->_CurIndex2);
            s.AddString(") �� (");
            s.AddInt(f->Args1Array->Count);
            s.AddString(",");
            s.AddInt(f->Args2Array->Count);
            s.AddString(")...");
            L57_Log_Add(_LOGBLK,s,FALSE);
          #endif
          if(f->_CurIndex1<f->Args1Array->Count && f->_CurIndex2<f->Args2Array->Count)
            { L57_Parameter *p1=_Params.P+f->Arg1Index;
              L57_Parameter *p2=_Params.P+f->Arg2Index;
              _Progress_PreTabCur++;
              // ������� �������� � ��������� (��������� ��� ��������)
              p1->Current=f->Args1Array->Data[f->_CurIndex1];
              p2->Current=f->Args2Array->Data[f->_CurIndex2];

              // ��������� �������
              L57ParamsToCalc();
              val=_Calculator._Calc_main();
              // ������� ������ � �������
              *(f->CurIndexBase())=val;
            }
          #ifdef L57_LOG
            L57_Log_Add(_LOGBLK,"OK");
          #endif
          // ����� ��������� ������
          f->_CurIndex2++;
          if(f->_CurIndex2>=f->Args2Array->Count)
            { f->_CurIndex1++;
              if(f->_CurIndex1<f->Args1Array->PrevCount) // "������ ������"
                f->_CurIndex2=f->Args2Array->PrevCount;
              else
                f->_CurIndex2=0;

            }
          if(f->_CurIndex1>=f->Args1Array->Count) // ������� ���������
            { f->_CurIndex2=f->Args2Array->Count;
              if(!Model_NextSimpleTable())
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif
      if(_check.Check())
        break;
    } // for(int _iteration=0;...)
  // ��������������� ��� �����������
  _Start=1;
}
//---------------------------------------------------------------------------

// ���� ������� - ������ ��������� ������ arg max
void rdsbcppBlockClass::Model_TabAfterCalcAM(void)
{
#ifdef L57_PARAMSCOUNT
  L57_LoopBreak _check;

  // �������� ������ ��� ��������� ������, � ���������� ������ �������������� ���������
  #if L57_TWOARG_ARGMAX_CNT==0
    Model_NextStage();
  #else

  if(!Has_TabAfterCalcAM) // ��� ���������
    { Model_NextStage();
      _Start=1;
      return;
    }

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    { double f;

      _Progress_TabAfterCalcCurAM+=1.0;

      // ������� �������� ����������
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          p->Current=p->Array.Data[p->_ArgIndex];
        } // for(int i=0;...)
      L57ParamsToCalc();

      // ����������
      f=_Calculator._Calc_main();

      if(f!=rdsbcppHugeDouble)
        {
          for(int i=0;i<L57_TWOARG_ARGMAX_CNT;i++)
            { L57_Table2ArgFuncData *func=_TwoargArgMaxF+i;
              L57_Parameter *this_p1=_Params.P+func->Arg1Index;
              L57_Parameter *this_p2=_Params.P+func->Arg2Index;
              if(func->Arg1Index<0 || func->Arg2Index<0 ||
                 this_p1->_ArgIndex<0 || this_p2->_ArgIndex<0 ||
                 this_p1->_ArgIndex>=this_p1->Array.Count ||
                 this_p2->_ArgIndex>=this_p2->Array.Count)
                   { L57_ONCEDEBUGMSG("������ �������")
                   }
              double *base=func->IndexBase(this_p1->_ArgIndex,this_p2->_ArgIndex);
              double *fval=base+L57_PARAMSCOUNT; // �������� �������
              if( (*fval)==rdsbcppHugeDouble || (*fval)<f )
                { // ����� �������� ��� ������� ����������
                  for(int j=0;j<L57_PARAMSCOUNT;j++)
                    base[j]=_Params.P[j].Current;
                  *fval=f;
                }
            }
        }


      // ��������� ���
      if(_Params.NextStep())
        { // ������� �������� ����� ��������
          Model_NextStage();
          return;
        }
      // ������� �������� ����� �� �������� - ����� �������� ���������� ��� ����������� � NextStep

      if(_check.Check())
        break;
    } // for(int _iteration=0;...)

  #endif
  // ��������������� ��� �����������
  _Start=1;
#endif
}
//---------------------------------------------------------------------------

// ���� ������� - ������ ���������� ��������
void rdsbcppBlockClass::Model_GridStage(void)
{
  switch(_SubStage)
    { case L57SUBSTAGE_GRIDFILL: // ����������� � ����������
        Model_GridStage_Fill();
        break;
      case L57SUBSTAGE_GRIDCAND: // ����� ����������
        Model_GridStage_Cand();
        break;
    }
}
//---------------------------------------------------------------------------

// �������� ������� ����� � ������ ������� ����������, ���� ����� (������������ ������ ��� ����������� ���������)
void rdsbcppBlockClass::Model_CurPointToAllMax_NoDeep(double f)
{
#ifdef L57_PARAMSCOUNT
  L57_MaxCandidate *c;
  int result;
  if(_GlobalMax==rdsbcppHugeDouble || f>_GlobalMax+EqualMaxDelta_setup)
    { // ����� �������� �������� ����� ��� ������
      _GlobalMax=f;
      _FinalMax.Clear();
      c=_FinalMax.AddFirst();
      c->f=f;
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        c->CurParams[i]=_Params.P[i].Current;
      return;
    }
  if(f<_GlobalMax-EqualMaxDelta_setup) // ����� ������ ������ ����������
    return;
  // ����� ������ � ���������� �������� �� _GlobalMax
  if(f>_GlobalMax)
    _GlobalMax=f;
  // ����������� ��������� �� ��������� ��������� �� _GlobalMax
  _FinalMax.RemoveSmall(_GlobalMax-EqualMaxDelta_setup);
  // ��������� �����
  c=_FinalMax.AddFirst();
  c->f=f;
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    c->CurParams[i]=_Params.P[i].Current;
#endif
}
//---------------------------------------------------------------------------

// �������� ������ �������� �� _Candidates � _FinalMax
void rdsbcppBlockClass::Model_FirstCandidateToFinal(void)
{
#ifdef L57_PARAMSCOUNT
  L57_MaxCandidate *c=_Candidates.RemoveTop();

  if(!c)
    return;

  if(_GlobalMax==rdsbcppHugeDouble || c->f>_GlobalMax+EqualMaxDelta_setup)
    { // ����� �������� �������� ����� ��� ������
      _GlobalMax=c->f;
      _FinalMax.Clear();
      _FinalMax.AddExistingFirst(c);
      return;
    }

  if(c->f<_GlobalMax-EqualMaxDelta_setup) // ����� ������ ������ ����������
    { delete c;
      return;
    }

  // ����� ������ � ���������� �������� �� _GlobalMax

  #ifdef L57_ENDSTEPSUSED
  // ���������, �� ����� �� ��� ��������� �� ��������� ����������
  for(L57_MaxCandidate *x=_FinalMax.First;x!=NULL;x=x->Next)
    { BOOL eq=TRUE;
      for(int i=0;eq && i<L57_PARAMSCOUNT;i++)
        if(fabs(x->CurParams[i]-c->CurParams[i])>_Params.P[i].EndStep)
          eq=FALSE;
      if(eq) // � �������� ��������� � ��� ��������� ���������� - �����������
        { delete c;
          return;
        }
    }
  #endif

  if(c->f>_GlobalMax)
    _GlobalMax=c->f;
  // ����������� ��������� �� ��������� ��������� �� _GlobalMax
  _FinalMax.RemoveSmall(_GlobalMax-EqualMaxDelta_setup);
  // ��������� �����
  _FinalMax.AddExistingFirst(c);
#endif  
}
//---------------------------------------------------------------------------

// �������� ��������� � ������� �������� ���������� ����������
BOOL rdsbcppBlockClass::Model_AddAllMaxToParameters(void)
{
#ifdef L57_PARAMSCOUNT
  if(!_AddMaxToArgs) // �� ����� ��������� ��� ��� ��� ���������
    return TRUE;
  _AddMaxToArgs=FALSE;

  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=_Params.P+i;
      int count=0;
      // �������� ����������
      #ifdef L57_ENDSTEPSUSED
        double delta=p->EndStep/4;
      #else
        double delta=L57_SMALLDELTA;
      #endif

      // ��������� ��������� � �������� p

      // ��������� - �� ��������� �� ������ �������� � ����� �� ����������
      for(L57_MaxCandidate *x=_FinalMax.First;x!=NULL;x=x->Next)
        { double thisparam=x->CurParams[i];
          x->Tag1=TRUE;
          x->IntTag1=-1;
          // �� ������ ��������� � ������� �����������
          for(L57_MaxCandidate *x1=x->Next;x1!=NULL;x1=x1->Next)
            if(fabs(thisparam-x1->CurParams[i])<delta) // �� ��������
              { x->Tag1=FALSE;
                break;
              }
          if(!x->Tag1)
            continue;
          // �� ������ ��������� � �����������
          for(int j=0;j<p->Array.Count;j++)
            if(fabs(thisparam-p->Array.Data[j])<delta) // �� ��������
              { x->Tag1=FALSE;
                break;
              }
          if(x->Tag1)
            { x->IntTag1=count; // ����� ������, ������� ����� ���������
              count++;
            }
        } // for(L57_MaxCandidate *x=_FinalMax.First;...)
      if(!count) // ������ �� ���������
        continue;

      // ����� �������� count �����
      if(!p->AddArgs(count))
        { L57OutOfMemMsg(9);
          return FALSE;
        }
      if(p->AddedMaxObjects)
        rdsFree(p->AddedMaxObjects);
      p->AddedMaxObjects=(L57_MaxCandidate**)rdsAllocate(count*sizeof(L57_MaxCandidate*));
      p->AddedMaxObjectsCount=count;
      if(!p->AddedMaxObjects)
        { L57OutOfMemMsg(12);
          return FALSE;
        }
      count=0;
      for(L57_MaxCandidate *x=_FinalMax.First;x!=NULL;x=x->Next)
        if(x->Tag1)
          { int index=p->Array.PrevCount+count;
            p->Array.Data[index]=x->CurParams[i];
            p->AddedMaxObjects[count]=x;
            count++;
          }

    } // for(int i=0;...)
    
#endif // #ifdef L57_PARAMSCOUNT
  return TRUE;
}
//---------------------------------------------------------------------------

// ������� � ��������� ������
void rdsbcppBlockClass::Model_NextStage(BOOL substageonly)
{
#ifdef L57_PARAMSCOUNT

  int nextstage=L57STAGE_FINAL,nextsubstage=0;
  int stage=_Stage,substage=_SubStage;

  switch(stage)
    { case L57STAGE_PRETAB: // ��������������� ������ ������� �������
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"����������� L57STAGE_PRETAB");
        #endif
        nextstage=L57STAGE_GRID;
        nextsubstage=L57SUBSTAGE_GRIDFILL;
        break;
      case L57STAGE_GRID: // ��������� �������
        if(!deepsearch_setup) // ��� ���������
          {
            #ifdef L57_LOG
              L57_Log_Add(_LOGBLK,"����������� L57STAGE_GRID");
            #endif
            if(Has_TabAfterCalcSimp)  // ���� ������ ��������� ������� ������
              { nextstage=L57STAGE_TABAFTERCALCSIMP;
                break;
              }
            if(Has_TabAfterCalcAM)  // ���� ������ ��������� ������ arg max
              { nextstage=L57STAGE_TABAFTERCALCAM;
                break;
              }
            // ��� ������ ��������� ������
            if(Has_TabPrecCalc)  // ���� ������ ��������� ������
              { nextstage=L57STAGE_TABPRECCALC;
                break;
              }
            // ��� ������ ��������� ������
            nextstage=L57STAGE_FINAL;
            break;
          }
        if(substage==L57SUBSTAGE_GRIDCAND) // ������� �� ��������� ������ ����������
          { if(substageonly)
              { // ��������� �� ��������� ������ ���������� ������� � ��������� ���������� �������
                nextstage=L57STAGE_GRID;
                nextsubstage=L57SUBSTAGE_GRIDFILL;
                #ifdef L57_LOG
                  L57_Log_Add(_LOGBLK,"����������� L57STAGE_GRID/L57SUBSTAGE_GRIDCAND");
                #endif
              }
            else
              { // ������ ������� �� ������ ���������� ��������
                nextstage=L57STAGE_PREC;
                #ifdef L57_LOG
                  L57_Log_Add(_LOGBLK,"����������� L57STAGE_GRID");
                #endif
              }
            break;
          }
        // ������� �� ��������� ���������� �������
        // ����� ������������� �� ��������� ������ ����������
        nextstage=L57STAGE_GRID;
        nextsubstage=L57SUBSTAGE_GRIDCAND;
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"����������� L57STAGE_GRID/L57SUBSTAGE_GRIDFILL");
        #endif
        break;

      case L57STAGE_PREC: // ��������� ����������
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"����������� L57STAGE_PREC");
        #endif
        if(Has_TabAfterCalcSimp)  // ���� ������ ��������� ������� ������
          { nextstage=L57STAGE_TABAFTERCALCSIMP;
            break;
          }
        // ��� ������ ��������� ������� ������

      case L57STAGE_TABAFTERCALCSIMP: // �������� ������� ������
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"����������� L57STAGE_TABAFTERCALCSIMP");
        #endif
        if(Has_TabAfterCalcAM)  // ���� ������ ��������� ������ arg max
          { nextstage=L57STAGE_TABAFTERCALCAM;
            break;
          }
        // ��� ������ ��������� ������ arg max


      case L57STAGE_TABAFTERCALCAM: // �������� ������ arg max
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"����������� L57STAGE_TABAFTERCALCAM");
        #endif
        if(Has_TabPrecCalc)  // ���� ������ ��������� ������
          { nextstage=L57STAGE_TABPRECCALC;
            break;
          }
        // ��� ������ ��������� ������
        nextstage=L57STAGE_FINAL;
        break;
    }

  if(nextstage==L57STAGE_FINAL)
    { // ������������ ���������
      #ifdef L57_LOG
        L57_Log_Add(_LOGBLK,"����� ������");
      #endif
      L57SetOutputData();
      #ifdef L57_LOG
        L57_Log_Add(_LOGBLK,"������ ��������");
      #endif
      _Stage=L57STAGE_WAIT;
      _SubStage=0;
      _Ready=1;
      return;
    }
  // ��������� �� ��������� ������
  _Stage=stage=nextstage;
  _SubStage=substage=nextsubstage;

  // ������������� ����� ������
  switch(stage)
    { case L57STAGE_GRID: // ��������� �������
        switch(substage)
          { case L57SUBSTAGE_GRIDCAND: // ������ ������ ����������
              #ifdef L57_LOG
                L57_Log_Add(_LOGBLK,"���������� L57STAGE_GRID/L57SUBSTAGE_GRIDCAND");
              #endif
              _Params.ResetCurrentChunk();
              break;
            case L57SUBSTAGE_GRIDFILL: // ������ ���������� ������� (����� ������ ���������� ���������� �����)
              #ifdef L57_LOG
                L57_Log_Add(_LOGBLK,"���������� L57STAGE_GRID/L57SUBSTAGE_GRIDFILL");
              #endif
              // ���������� ������� �� ������ �������� �����
              _Params.ResetCurrentChunk();
              _Params.ResetChunkValuesArray();
              break;
          }
        break;

      case L57STAGE_PREC: // ��������� ����������
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"���������� L57STAGE_PREC");
        #endif
        _Params.ClearChunkValuesArray();
        // �������� ��� ���� ������
        _Progress_PrecCur=0.0;
        _Progress_PrecMax=_Candidates.Count();
        if(_Progress_PrecMax<0.5) // �����-�� ������
          _Progress_PrecMax=1.0;
        break;

      case L57STAGE_TABAFTERCALCSIMP: // �������� ������� ������
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"���������� L57STAGE_TABAFTERCALCSIMP");
        #endif
        if(!Model_AddAllMaxToParameters())
          {
            #ifdef L57_LOG
              L57_Log_Add(_LOGBLK,"*** ������ Model_AddAllMaxToParameters()");
            #endif
            return;
          }
        _Progress_PrecCur=_Progress_PrecMax;
        _PreTab_OneArg_Cur=_PreTab_TwoArg_Cur=-1;
        _PreTab_NArg=0;
        _Progress_TabAfterCalcCurS=_Progress_TabAfterCalcMaxS=0.0;
        #if L57_ONEARG_SIMPLE_CNT!=0
          #ifdef L57_LOG
            L57_Log_Add(_LOGBLK,"  ���������� ����������� � F(x)");
          #endif
          for(int i=0;i<L57_ONEARG_SIMPLE_CNT;i++)
            { int n=_OneargSimpleF[i].AddResultsFromPrevCount();//Model_AddAllMaxToOneArg(_OneargSimpleF+i);
              if(n<0)
                { L57OutOfMemMsg(10);
                  return;
                }
              _Progress_TabAfterCalcMaxS+=n;
            }
        #endif
        #if L57_TWOARG_SIMPLE_CNT!=0
          #ifdef L57_LOG
            L57_Log_Add(_LOGBLK,"  ���������� ����������� � F(x,y)");
          #endif
          for(int i=0;i<L57_TWOARG_SIMPLE_CNT;i++)
            { L57_Table2ArgFuncData *f=_TwoargSimpleF+i;
              int total1,total2;
              if(!f->AddResultsFromPrevCount())
                { L57OutOfMemMsg(13);
                  return;
                }
              // ��������� �����
              total1=f->Args1Array->Count-f->Args1Array->PrevCount;
              // ��������� ��������
              total2=f->Args2Array->Count-f->Args2Array->PrevCount;
              _Progress_TabAfterCalcMaxS+=total1*f->Args2Array->Count+total2*(f->Args1Array->Count-total1);
            }
        #endif
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"  ���������� ����������� ���������");
        #endif
        break;

      case L57STAGE_TABAFTERCALCAM: // �������� ������ arg max
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"���������� L57STAGE_TABAFTERCALCAM");
        #endif
        if(!Model_AddAllMaxToParameters())
          {
            #ifdef L57_LOG
              L57_Log_Add(_LOGBLK,"*** ������ Model_AddAllMaxToParameters()");
            #endif
            return;
          }
        _Progress_PrecCur=_Progress_PrecMax;
        _Progress_TabAfterCalcCurS=_Progress_TabAfterCalcMaxS;
        _Progress_TabAfterCalcCurAM=_Progress_TabAfterCalcMaxAM=0.0;
        _ArgMax_OneArg_Cur=_ArgMax_TwoArg_Cur=-1;
        _ArgMax_NArg=1;
        #if L57_ONEARG_ARGMAX_CNT!=0
          for(int i=0;i<L57_ONEARG_ARGMAX_CNT;i++)
            { L57_Table1ArgFuncData *f=_OneargArgMaxF+i;
              L57_Parameter *p=_Params.P+f->ArgIndex;
              int n=f->AddResultsFromPrevCount();
              if(n<0)
                { L57OutOfMemMsg(11);
                  return;
                }
              if(n==0)
                continue;
              // ������ ��� ���������, ������� ��������� � �������, �������� � p->AddedMaxObjects
              // ������� ��� ��������� � �������� ���� ���������� � ������� - �� �� ����� ����������� � ��������
              for(int j=0;j<p->AddedMaxObjectsCount;j++)
                { L57_MaxCandidate *x=p->AddedMaxObjects[j];
                  int index=f->ArgsArray->PrevCount+j;
                  double *base=f->ResultBaseForArgN(index);
                  memcpy(base,x->CurParams,L57_PARAMSCOUNT*sizeof(double));
                  base[L57_PARAMSCOUNT]=x->f;
                }
            }
        #endif
        #if L57_TWOARG_ARGMAX_CNT!=0
          for(int i=0;i<L57_TWOARG_ARGMAX_CNT;i++)
            { L57_Table2ArgFuncData *f=_TwoargArgMaxF+i;
              if(!f->AddResultsFromPrevCount())
                { L57OutOfMemMsg(14);
                  return;
                }
            }
          // �������� ��������� -  ������ ����� ����� ������������ �����
          _Progress_TabAfterCalcMaxAM=1.0;
           for(int i=0;i<L57_PARAMSCOUNT;i++)
             { L57_Parameter *p=_Params.P+i;
               _Progress_TabAfterCalcMaxAM*=p->Array.Count;
               p->_ArgIndex=0;
             }
        #endif
        break;

      case L57STAGE_TABPRECCALC: // ��������� ������ arg max
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"���������� L57STAGE_TABPRECCALC");
        #endif
        _Progress_PrecCur=_Progress_PrecMax;
        _Progress_TabAfterCalcCurS=_Progress_TabAfterCalcMaxS;
        _Progress_TabAfterCalcCurAM=_Progress_TabAfterCalcMaxAM;
        _Progress_TabPrecCalcMax=_Progress_TabPrecCalcCur=0.0;
        _ArgMax_OneArg_Cur=_ArgMax_TwoArg_Cur=-1;
        _ArgMax_NArg=0;
        #if L57_ONEARG_ARGMAX_CNT!=0
          for(int i=0;i<L57_ONEARG_ARGMAX_CNT;i++)
            { L57_Table1ArgFuncData *f=_OneargArgMaxF+i;
              L57_Parameter *p=_Params.P+f->ArgIndex;
              _Progress_TabPrecCalcMax+=p->Array.PrevCount; // � ���������� ���������� ������ ������ �������
            }
        #endif
        #if L57_TWOARG_ARGMAX_CNT!=0
          for(int i=0;i<L57_TWOARG_ARGMAX_CNT;i++)
            { L57_Table2ArgFuncData *f=_TwoargArgMaxF+i;
              L57_Parameter *p1=_Params.P+f->Arg1Index,
                            *p2=_Params.P+f->Arg2Index;
              _Progress_TabPrecCalcMax+=p1->Array.Count*p2->Array.Count;
            }
        #endif
        break;
    }

  _Start=_Ready=1;

#else // #ifdef L57_PARAMSCOUNT
  L57SetOutputData();
  _Stage=L57STAGE_WAIT;
  _SubStage=0;
  _Ready=1;
#endif
}
//---------------------------------------------------------------------------

// ���� ������� - ������ ���������� �������� - ��������� ����������� � ����������
void rdsbcppBlockClass::Model_GridStage_Fill(void)
{
#ifdef L57_PARAMSCOUNT
  L57_LoopBreak _check;

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    { double f;

      _Progress_GridCur+=1.0;

      // ������� �������� ����������
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          p->Current=p->Array.Data[p->_ArgIndex];
        } // for(int i=0;...)
      L57ParamsToCalc();

      // ����������
      f=_Calculator._Calc_main();

      #if L57_ONEARG_ARGMAX_CNT!=0 || L57_TWOARG_ARGMAX_CNT!=0
      if(f!=rdsbcppHugeDouble)
        { // ��������� ������ ������ arg max
          #if L57_ONEARG_ARGMAX_CNT!=0
          for(int i=0;i<L57_ONEARG_ARGMAX_CNT;i++)
            { L57_Table1ArgFuncData *func=_OneargArgMaxF+i;
              L57_Parameter *this_p=_Params.P+func->ArgIndex;
              if(func->ArgIndex<0)
                { L57_ONCEDEBUGMSG("������ ������")
                }
              double *base=func->ResultBaseForArgN(this_p->_ArgIndex);
              double *fval=base+L57_PARAMSCOUNT; // �������� �������
              if( (*fval)==rdsbcppHugeDouble || (*fval)<f )
                { // ����� �������� ��� ������� ����������
                  for(int j=0;j<L57_PARAMSCOUNT;j++)
                    base[j]=_Params.P[j].Current;
                  *fval=f;
                }
            }
          #endif
          #if L57_TWOARG_ARGMAX_CNT!=0
          if(!Has_TabAfterCalcAM) // ���� ���� �������� ������ arg max, �� ��� ����� ������ ����������� ��� ���������, � �� �����
            for(int i=0;i<L57_TWOARG_ARGMAX_CNT;i++)
              { L57_Table2ArgFuncData *func=_TwoargArgMaxF+i;
                L57_Parameter *this_p1=_Params.P+func->Arg1Index;
                L57_Parameter *this_p2=_Params.P+func->Arg2Index;
                if(func->Arg1Index<0 || func->Arg2Index<0 ||
                   this_p1->_ArgIndex<0 || this_p2->_ArgIndex<0)
                     { L57_ONCEDEBUGMSG("������ �������")
                     }
                double *base=func->IndexBase(this_p1->_ArgIndex,this_p2->_ArgIndex);
                double *fval=base+L57_PARAMSCOUNT; // �������� �������
                if( (*fval)==rdsbcppHugeDouble || (*fval)<f )
                  { // ����� �������� ��� ������� ����������
                    for(int j=0;j<L57_PARAMSCOUNT;j++)
                      base[j]=_Params.P[j].Current;
                    *fval=f;
                  }
              }
          #endif
        }
      #endif

      if(deepsearch_setup)
        { // ��������� �������� � ������ ��� ������������ ������ ���������� � ��������� (��� ��������)
          double *valptr=_Params.GetChunkValuePtrByParams();
          if(valptr)
            *valptr=f;
        }
      else
        { // ���������� � ������ ����������
          Model_CurPointToAllMax_NoDeep(f);
        }

      // ��������� ��� � ������� �����
      if(_Params.NextStep_Chunk())
        { // ������� �������� ����� ��������
          Model_NextStage();
          return;
        }
      // ������� �������� ����� �� �������� - ����� �������� ���������� ��� ����������� � NextStep

      if(_check.Check())
        break;
    } // for(int _iteration=0;...)
  // ��������������� ��� �����������
  _Start=1;
#endif
}
//---------------------------------------------------------------------------

// ���� ������� - ������ ���������� �������� - ��������� ������ ����������
void rdsbcppBlockClass::Model_GridStage_Cand(void)
{
#ifdef L57_PARAMSCOUNT

  BOOL low,allequal;
  double thisval,*valptr;
  int _count=tactcount_setup;
  L57_LoopBreak _check;


  for(;;)
    {
      _Progress_GridCur+=1.0;

      //--------- �������� ----------
      // ������������ ����� p->Array.Data[p->_ArgIndex] - ��������� �� �� ��������� ��������� ��������
      valptr=_Params.GetChunkValuePtrByParams();
      if(!valptr)
        { rdsStopCalc();
          L57ClearTempData();
          L57ClearOutputData();
          _Stage=L57STAGE_WAIT  ;
          rdsBlockMessageBox(NULL,"���������� ������ ������������� - ��������� � ������������� ������ ������� �������� (1)",
                             L57_MSGCAPTION,MB_OK|MB_ICONERROR);
          return;
        }
      thisval=(*valptr);
      low=(thisval!=rdsbcppHugeDouble); // ���� - ��� ������ �� ����
      allequal=NoPlateau_setup; // ���� - ��� ������ �������� �����
      // ����������� �������
      _Params.InitAuxIndex(-1,1);
      while(low)
        { BOOL allzero,inside;
          inside=_Params.ValidMainAndAuxIndices(&allzero);
          if(inside && (!allzero))
            { valptr=_Params.GetChunkValuePtrByParamsAndAux();
              if(!valptr)
                { rdsStopCalc();
                  L57ClearTempData();
                  L57ClearOutputData();
                  _Stage=L57STAGE_WAIT;
                  rdsBlockMessageBox(NULL,"���������� ������ ������������� - ��������� � ������������� ������ ������� �������� (2)",
                                     L57_MSGCAPTION,MB_OK|MB_ICONERROR);

                  return;
                }
              if((*valptr)!=rdsbcppHugeDouble)
                { if((*valptr)>thisval)
                    { low=allequal=FALSE; // ���� ����� ����
                      break;
                    }
                  if(allequal && fabs((*valptr)-thisval)>L57_SMALLDELTA)
                    allequal=FALSE;
                }
              else
                allequal=FALSE;
            } // if(inside && (!allzero))
          // ��������� ���
          if(_Params.StepAuxIndex())
            break;
        }

      // allequal==TRUE ���� �������� ��������� "�����" � ��� ������ ����������

      if((!allequal) && low) // ��� - �������� (��� ������ �� ����)
        { // ��������� � ������ ����������
          L57_MaxCandidate *c=_Candidates.AddFirst();
          if(!c)
            { L57OutOfMemMsg(6);
              return;
            }
          c->f=thisval;
          c->cycles_in_df=0;
          for(int i=0;i<L57_PARAMSCOUNT;i++)
            { L57_Parameter *p=_Params.P+i;
              c->CurParams[i]=p->Array.Data[p->_ArgIndex];
              c->Deltas[i]=p->StartStep;
            }
        }
      //-----------------------------

      // ��������� ���
      if(_Params.NextStep_Chunk())
        { // ������� �������� ����� �������� - ����� ������������� � ��������� ���������� � ����� ��������� �����
          if(_Params.NextChunk())
            { // ������ ��� ���������� ��������� ����� - ��������� � ������ ��������� ����������
              _Progress_GridCur=_Progress_GridMax;
              Model_NextStage();
              return;
            }
          Model_NextStage(TRUE); // ������ ���������, � �� ����� ������
          return;
        }

      _count--;
      if(_count<=0 || _check.Check())
        break;
      // �������� � �����, ���� _count �� ��������� ����

    } // for(;;)
  // ������� �������� ����� �� �������� - ����� �������� ���������� ����������� � NextStep
  _Ready=_Start=1; // ���� ���� �������������
#endif  
}
//---------------------------------------------------------------------------

// ���� ������� - ������ ��������� ���������� � ���������
void rdsbcppBlockClass::Model_PrecStage(void)
{
#ifdef L57_PARAMSCOUNT
  L57_LoopBreak _check;

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    { BOOL _cand_ready;
      L57_MaxCandidate *c=_Candidates.First;
      double baseparams[L57_PARAMSCOUNT];
      if(!c) // ��������� ���������
        { // ��������� � ��������� ������
          _Progress_PrecCur=_Progress_PrecMax;
          Model_NextStage();
          return;
        }

      // ������������ ��������� c
      // ���������, �� ����� �� �� ���
      _cand_ready=TRUE;
      if(PrecToEndSteps_setup) // ������� ����� ����� �� �������� �����
        { for(int i=0;i<L57_PARAMSCOUNT;i++)
            { L57_Parameter *p=_Params.P+i;
              double dx=c->Deltas[i];
              if(dx!=rdsbcppHugeDouble && dx>p->EndStep)
                { _cand_ready=FALSE;
                  break;
                }
            }
        }
      if(_cand_ready && PrecToDf_setup) // ����� ����� � ������ �����������
        { if(c->cycles_in_df<DfCycles_setup) // ������� � ������
            _cand_ready=FALSE;
        }
      if(_cand_ready) // �������� �������
        { // ���������� ��� � ������������� � ��������� � ����������
          Model_FirstCandidateToFinal();
          _Progress_PrecCur++;
          continue;
        }

      // �������� c �� ����� - ��������� ��� ���� � ��� ����
      c->Old_Val=c->f;
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        c->Deltas[i]*=0.5;
      c->StepsDown++;
      memcpy(baseparams,c->CurParams,L57_PARAMSCOUNT*sizeof(double));
      //c->Equal=TRUE;

      // �������������
      switch(PrecGridSize_setup)
        { case L57PRECGRID_4X4: _Params.InitAuxIndex(-2,2); break;
          default:              _Params.InitAuxIndex(-1,1);
        }

      for(;;)
        { // ������� �������� ����������
          BOOL valueinside=TRUE,all_2=TRUE;
          double f;
          for(int i=0;valueinside && i<L57_PARAMSCOUNT;i++)
            { L57_Parameter *p=_Params.P+i;
              double v=baseparams[i]+c->Deltas[i]*p->AuxIndex;
              if(v<p->_Min || v>p->_Max)
                valueinside=FALSE;
              else
                { p->Current=v;
                  if(p->AuxIndex==1 || p->AuxIndex==-1) // �� � ������ �����
                    all_2=FALSE;
                }
            } // for(int i=0;...)
          if(!all_2) // �������� �� �������� � ������ �����
            { if(valueinside)
                { // ����������
                  L57ParamsToCalc();
                  f=_Calculator._Calc_main();
                }
              else
                f=rdsbcppHugeDouble;
              // ��������� �������� ������� ��� ������� ���������� baseparams + c->Deltas*p->AuxIndex
              if(f!=rdsbcppHugeDouble && f>c->f) // ����� ����� ��������
                { c->f=f;
                  for(int i=0;i<L57_PARAMSCOUNT;i++)
                    c->CurParams[i]=_Params.P[i].Current;
                } // if(f!=rdsbcppHugeDouble)
            } // if(!all_2)
          if(_Params.StepAuxIndex())
            break;
        } // for(;;)
      // ��������� ����������� c, ����� �������� � c->f � c->CurParams
      if(c->f==c->Old_Val) // � �������� ���������
       c->cycles_in_df++;
      else if(c->f!=rdsbcppHugeDouble)
        { double d=fabs(c->f-c->Old_Val);
          if(d<DfDelta_setup)
            c->cycles_in_df++; // � ������
          else
            c->cycles_in_df=0; // ��� ������
        }

      // ������ �� ���� - ���������� ��������� ����� ���������
      if(_check.Check())
        break;
    } // for(int _iteration=0;...)

  _Ready=_Start=1;
#endif  
}
//---------------------------------------------------------------------------

#if L57_ONEARG_ARGMAX_CNT!=0
// ��������� ����� ����� ���������� �������
// (��������� ����� _CurIndex ������� _ArgMax_OneArg_Cur)
// �������: TRUE - ����� ��������� ������, FALSE - ��������� ��������� ���� ����� (���� ���������)
BOOL rdsbcppBlockClass::Model_TabPrecCalc_OneArgPoint(void)
{
#ifdef L57_PARAMSCOUNT

  L57_MaxCandidate *c=&_ArgMax_TablePoint;
  L57_Table1ArgFuncData *f=_OneargArgMaxF+_ArgMax_OneArg_Cur;


  double baseparams[L57_PARAMSCOUNT];
  BOOL _cand_ready;

  if(_ArgMax_TP_Index1!=f->_CurIndex)
    { // ������ �������������� ����� ��������� - ������������� �������� �������
      double *base=f->ResultBaseForArgN(f->_CurIndex);
      double *fval=base+L57_PARAMSCOUNT; // �������� �������
      _ArgMax_TP_Index1=f->_CurIndex;
      _Progress_TabPrecCalcCur++;
      c->f=*fval;
      c->cycles_in_df=0;
      c->StepsDown=0;
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          if(i==f->ArgIndex) // ���� �������� �� �������
            { c->Deltas[i]=0.0;
              c->CurParams[i]=f->ArgsArray->Data[f->_CurIndex]; // �������� ���������
            }
          else
            { c->Deltas[i]=p->StartStep;
              c->CurParams[i]=base[i];
            }
        }
    } // if(_ArgMax_TP_Index1!=f->_CurIndex)

  // ������������ ��������� c
  // ���������, �� ����� �� �� ���
  _cand_ready=TRUE;
  if(Tab_PrecToEndSteps_setup) // ������� ����� ����� �� �������� �����
    { for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          double dx=c->Deltas[i];
          if(dx!=rdsbcppHugeDouble && dx>p->EndStep)
            { _cand_ready=FALSE;
              break;
            }
        }
    }
  if(_cand_ready && Tab_PrecToDf_setup) // ����� ����� � ������ �����������
    { if(c->cycles_in_df<Tab_DfCycles_setup) // ������� � ������
        _cand_ready=FALSE;
    }
  if(_cand_ready) // �������� �������
    { // ���������� ��� ������� � ������� � ��������� � ����������
      double *base=f->ResultBaseForArgN(f->_CurIndex);
      double *fval=base+L57_PARAMSCOUNT; // �������� �������
      memcpy(base,c->CurParams,L57_PARAMSCOUNT*sizeof(double));
      *fval=c->f;
      return TRUE;
    }

  // �������� c �� ����� - ��������� ��� ���� � ��� ����
  c->Old_Val=c->f;
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    c->Deltas[i]*=0.5;
  c->StepsDown++;
  memcpy(baseparams,c->CurParams,L57_PARAMSCOUNT*sizeof(double));

  // �������������
  switch(Tab_PrecGridSize_setup)
    { case L57PRECGRID_4X4: _Params.InitAuxIndex(-2,2); break;
      default:              _Params.InitAuxIndex(-1,1);
    }

  for(;;)
    { // ������� �������� ����������
      BOOL valueinside=TRUE,all_2=TRUE;
      double f;
      for(int i=0;valueinside && i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          double v=baseparams[i]+c->Deltas[i]*p->AuxIndex;
          if(v<p->_Min || v>p->_Max)
            valueinside=FALSE;
          else
            { p->Current=v;
              if(p->AuxIndex==1 || p->AuxIndex==-1) // �� � ������ �����
                all_2=FALSE;
            }
        } // for(int i=0;...)
      if(!all_2) // �������� �� �������� � ������ �����
        { if(valueinside)
            { // ����������
              L57ParamsToCalc();
              f=_Calculator._Calc_main();
            }
          else
            f=rdsbcppHugeDouble;
          // ��������� �������� ������� ��� ������� ���������� baseparams + c->Deltas*p->AuxIndex
          if(f!=rdsbcppHugeDouble && f>c->f) // ����� ����� ��������
            { c->f=f;
              for(int i=0;i<L57_PARAMSCOUNT;i++)
                c->CurParams[i]=_Params.P[i].Current;
            } // if(f!=rdsbcppHugeDouble)
        } // if(!all_2)
      if(_Params.StepAuxIndex())
        break;
    } // for(;;)
  // ��������� ����������� c, ����� �������� � c->f � c->CurParams
  if(c->f==c->Old_Val) // � �������� ���������
   c->cycles_in_df++;
  else if(c->f!=rdsbcppHugeDouble)
    { double d=fabs(c->f-c->Old_Val);
      if(d<Tab_DfDelta_setup)
        c->cycles_in_df++; // � ������
      else
        c->cycles_in_df=0; // ��� ������
    }

  // ������ �� ���� - ���������� ��������� ����� ���������

#endif
  return FALSE;
}
//---------------------------------------------------------------------------
#endif

#if defined(L57_PARAMSCOUNT) && L57_TWOARG_ARGMAX_CNT!=0
// ��������� ����� ����� ��������� �������
// (��������� ����� _CurIndex1,_CurIndex2 ������� _ArgMax_TwoArg_Cur)
// �������: TRUE - ����� ��������� ������, FALSE - ��������� ��������� ���� ����� (���� ���������)
BOOL rdsbcppBlockClass::Model_TabPrecCalc_TwoArgPoint(void)
{
  L57_MaxCandidate *c=&_ArgMax_TablePoint;
  L57_Table2ArgFuncData *f=_TwoargArgMaxF+_ArgMax_TwoArg_Cur;
  L57_Parameter *p1=_Params.P+f->Arg1Index;
  L57_Parameter *p2=_Params.P+f->Arg2Index;

  double baseparams[L57_PARAMSCOUNT];
  BOOL _cand_ready;

  if(_ArgMax_TP_Index1!=f->_CurIndex1 || _ArgMax_TP_Index2!=f->_CurIndex2)
    { // ������ �������������� ����� ��������� - ������������� �������� �������
      double *base=f->IndexBase(f->_CurIndex1,f->_CurIndex2);
      double *fval=base+L57_PARAMSCOUNT; // �������� �������
      _ArgMax_TP_Index1=f->_CurIndex1;
      _ArgMax_TP_Index2=f->_CurIndex2;
      _Progress_TabPrecCalcCur++;
      c->f=*fval;
      c->cycles_in_df=0;
      c->StepsDown=0;
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          if(i==f->Arg1Index) // ���� �������� �� �������
            { c->Deltas[i]=0.0;
              c->CurParams[i]=f->Args1Array->Data[f->_CurIndex1]; // �������� ���������
            }
          else if(i==f->Arg2Index) // ���� �������� �� �������
            { c->Deltas[i]=0.0;
              c->CurParams[i]=f->Args2Array->Data[f->_CurIndex2]; // �������� ���������
            }
          else
            { c->Deltas[i]=p->StartStep;
              c->CurParams[i]=base[i];
            }
        }
    } // if(_ArgMax_TP_Index*!=f->_CurIndex*)

  // ������������ ��������� c
  // ���������, �� ����� �� �� ���
  _cand_ready=TRUE;
  if(Tab_PrecToEndSteps_setup) // ������� ����� ����� �� �������� �����
    { for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          double dx=c->Deltas[i];
          if(dx!=rdsbcppHugeDouble && dx>p->EndStep)
            { _cand_ready=FALSE;
              break;
            }
        }
    }
  if(_cand_ready && Tab_PrecToDf_setup) // ����� ����� � ������ �����������
    { if(c->cycles_in_df<Tab_DfCycles_setup) // ������� � ������
        _cand_ready=FALSE;
    }
  if(_cand_ready) // �������� �������
    { // ���������� ��� ������� � ������� � ��������� � ����������
      double *base=f->IndexBase(f->_CurIndex1,f->_CurIndex2);
      double *fval=base+L57_PARAMSCOUNT; // �������� �������
      memcpy(base,c->CurParams,L57_PARAMSCOUNT*sizeof(double));
      *fval=c->f;
      return TRUE;
    }

  // �������� c �� ����� - ��������� ��� ���� � ��� ����
  c->Old_Val=c->f;
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    c->Deltas[i]*=0.5;
  c->StepsDown++;
  memcpy(baseparams,c->CurParams,L57_PARAMSCOUNT*sizeof(double));

  // �������������
  switch(Tab_PrecGridSize_setup)
    { case L57PRECGRID_4X4: _Params.InitAuxIndex(-2,2); break;
      default:              _Params.InitAuxIndex(-1,1);
    }

  for(;;)
    { // ������� �������� ����������
      BOOL valueinside=TRUE,all_2=TRUE;
      double f;
      for(int i=0;valueinside && i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          double v=baseparams[i]+c->Deltas[i]*p->AuxIndex;
          if(v<p->_Min || v>p->_Max)
            valueinside=FALSE;
          else
            { p->Current=v;
              if(p->AuxIndex==1 || p->AuxIndex==-1) // �� � ������ �����
                all_2=FALSE;
            }
        } // for(int i=0;...)
      if(!all_2) // �������� �� �������� � ������ �����
        { if(valueinside)
            { // ����������
              L57ParamsToCalc();
              f=_Calculator._Calc_main();
            }
          else
            f=rdsbcppHugeDouble;
          // ��������� �������� ������� ��� ������� ���������� baseparams + c->Deltas*p->AuxIndex
          if(f!=rdsbcppHugeDouble && f>c->f) // ����� ����� ��������
            { c->f=f;
              for(int i=0;i<L57_PARAMSCOUNT;i++)
                c->CurParams[i]=_Params.P[i].Current;
            } // if(f!=rdsbcppHugeDouble)
        } // if(!all_2)
      if(_Params.StepAuxIndex())
        break;
    } // for(;;)
  // ��������� ����������� c, ����� �������� � c->f � c->CurParams
  if(c->f==c->Old_Val) // � �������� ���������
   c->cycles_in_df++;
  else if(c->f!=rdsbcppHugeDouble)
    { double d=fabs(c->f-c->Old_Val);
      if(d<Tab_DfDelta_setup)
        c->cycles_in_df++; // � ������
      else
        c->cycles_in_df=0; // ��� ������
    }

  // ������ �� ���� - ���������� ��������� ����� ���������

  return FALSE;
}
//---------------------------------------------------------------------------
#endif

// ���� ������� - ������ ��������� ������
void rdsbcppBlockClass::Model_TabPrecCalc(void)
{ L57_LoopBreak _check;
  if(_ArgMax_NArg<1)
    { if(!Model_NextArgMaxTable())
        { Model_NextStage();
          return;
        }
    }
  if(_ArgMax_NArg<1)
    return;

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    {
      // ������ ���������� ������
      #if L57_ONEARG_ARGMAX_CNT!=0
      if(_ArgMax_NArg==1)
        { // ��������� ����� _CurIndex ������� _ArgMax_OneArg_Cur
          L57_Table1ArgFuncData *f=_OneargArgMaxF+_ArgMax_OneArg_Cur;
          if(!Model_TabPrecCalc_OneArgPoint())
            continue; // ��������� ��������� � ������������ ������

          // ����� ��������� ������
          f->_CurIndex++;
          if(f->_CurIndex>=f->ArgsArray->PrevCount) // ������� ��������� (����� ����� PrevCount �� ������� ���������)
            {
              if(!Model_NextArgMaxTable())
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif // #if L57_ONEARG_ARGMAX_CNT!=0

      #if L57_TWOARG_ARGMAX_CNT!=0
      if(_ArgMax_NArg==2)
        {
          // ��������� ����� _CurIndex1,_CurIndex2 ������� _ArgMax_TwoArg_Cur
          L57_Table2ArgFuncData *f=_TwoargArgMaxF+_ArgMax_TwoArg_Cur;
          if(!Model_TabPrecCalc_TwoArgPoint())
            continue; // ��������� ��������� � ������������ ������

          // ����� ��������� ������
          f->_CurIndex2++;
          if(f->_CurIndex2>=f->Args2Array->Count)
            { f->_CurIndex1++;
              f->_CurIndex2=0;
            }
          if(f->_CurIndex1>=f->Args1Array->Count) // ������� ���������
            { f->_CurIndex2=f->Args2Array->Count;
              if(!Model_NextArgMaxTable())
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif
      if(_check.Check())
        break;
    } // for(int _iteration=0;...)
}
//---------------------------------------------------------------------------


// ����� ���������
void rdsbcppBlockClass::Model_ShowProgress(void)
{ double n=0.0,fraction=0.0,cf;
  if(_Stage==L57STAGE_WAIT)
    return;

#define ADDTOPROGRESS(_cur,_max) \
  if(_max!=rdsbcppHugeDouble && _max!=0.0) \
    { \
      RDSBCPP_TRY \
        { cf=_cur/_max; } \
      RDSBCPP_CATCHALL \
        { cf=0.0; } \
      n++; fraction+=cf; \
    }

  ADDTOPROGRESS(_Progress_PreTabCur,_Progress_PreTabMax)
  ADDTOPROGRESS(_Progress_GridCur,_Progress_GridMax)
  ADDTOPROGRESS(_Progress_PrecCur,_Progress_PrecMax)
  ADDTOPROGRESS(_Progress_TabAfterCalcCurS,_Progress_TabAfterCalcMaxS)
  ADDTOPROGRESS(_Progress_TabAfterCalcCurAM,_Progress_TabAfterCalcMaxAM)
  ADDTOPROGRESS(_Progress_TabPrecCalcCur,_Progress_TabPrecCalcMax)

  if(n==0.0)
    fraction=0.0;
  else
    {
      RDSBCPP_TRY
        { fraction/=n; }
      RDSBCPP_CATCHALL
        { fraction=0.0; }
    }
  Progress_out=fraction;
#undef ADDTOPROGRESS
}
//---------------------------------------------------------------------------

#endif // #ifdef L57MS_AFTERCLASS
//---------------------------------------------------------------------------

