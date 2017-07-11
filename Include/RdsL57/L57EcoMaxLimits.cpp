//---------------------------------------------------------------------------
// ����������� ����� ������ ����������� ��� �������� �������������
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ���������� ��������
//---------------------------------------------------------------------------
#ifdef L57MS_GLOBALS
#include <RdsL57/L57TextBasic.cpp>

#define L57_HELPUIFILE "Blocks\\L57.ui"
#define L57_HELPTOPIC  "L57_EcoMaxLim"

// ������� ������� ����� �������
int L57_HelpState=0;

class TL57TextFileEML : public TL57TextFileBasic
{ public:
    TL57TextString InputCode,OutputCode;

    // ��������� ������� ������
    void ParseHiddenSection(TL57TextFileLine *line);
    // �������� ������� ������
    BOOL WriteHiddenSection(void);
    // �������� �����-�� ������ � ������� ������ �� �����
    BOOL WriteSection(int id,int *pendid);

    TL57TextFileEML(void):TL57TextFileBasic(){};
    virtual ~TL57TextFileEML(){};
};
//---------------------------------------------------------------------------

// ��������� ������� ������
void TL57TextFileEML::ParseHiddenSection(TL57TextFileLine *line)
{ char *aux;

  InputCode.Clear();
  OutputCode.Clear();

  // ������ �������
  for(TL57TextFileLine *l=line;l!=NULL;l=l->Next)
    { char *s=l->Line;
      if(!s)
        continue;
      if((*s)=='$') // ����� ������ (������ ��������� ������)
        break;
      switch(*s)
        { case '1': // ����������� �� �����
            aux=rdsProcessText(s+1,RDS_PT_STRINGTOTEXT,NULL);
            if(aux)
              { if(InputCode.IsNotEmpty())
                  InputCode.AddString("\n");
                InputCode.AddString(aux);
                rdsFree(aux);
              }
            break;
          case '2': // ����������� �� ������
            aux=rdsProcessText(s+1,RDS_PT_STRINGTOTEXT,NULL);
            if(aux)
              { if(OutputCode.IsNotEmpty())
                  OutputCode.AddString("\n");
                OutputCode.AddString(aux);
                rdsFree(aux);
              }
            break;
        }

    }

}
//---------------------------------------------------------------------------

// �������� ������� ������
BOOL TL57TextFileEML::WriteHiddenSection(void)
{ TL57TextFileBasic splitter;
  BOOL ok=TRUE;
  char *aux;

  splitter.SplitText(InputCode.c_str());
  for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
    if(l->Line)
      { aux=rdsProcessText(l->Line,RDS_PT_TEXTTOSTRINGSL,NULL);
        if(aux)
          { ok=ok && WriteLine("1");
            ok=ok && WriteLine(aux,1);
            rdsFree(aux);
          }
      }

  splitter.SplitText(OutputCode.c_str());
  for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
    if(l->Line)
      { aux=rdsProcessText(l->Line,RDS_PT_TEXTTOSTRINGSL,NULL);
        if(aux)
          { ok=ok && WriteLine("2");
            ok=ok && WriteLine(aux,1);
            rdsFree(aux);
          }
      }

  return ok;
}
//---------------------------------------------------------------------------

// �������� �����-�� ������ � ������� ������ �� �����
BOOL TL57TextFileEML::WriteSection(int id,int *pendid)
{ TL57TextFileBasic splitter;
  BOOL ok=TRUE;

  switch(id)
    { case 1: // ������ ������� ����������� �� ������
        *pendid=2;
        splitter.SplitText(InputCode.c_str());
        for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
          if(l->Line)
            { ok=ok && WriteLine("  ");
              ok=ok && WriteLine(l->Line,1);
            }
        ok=ok && WriteLine("  ; /* */",1);
        break;

      case 3: // ������ ������� ����������� �� �������
        *pendid=4;
        splitter.SplitText(OutputCode.c_str());
        for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
          if(l->Line)
            { ok=ok && WriteLine("  ");
              ok=ok && WriteLine(l->Line,1);
            }
        ok=ok && WriteLine("  ; /* */",1);
        break;
    }
  return ok;
}
//---------------------------------------------------------------------------

// ������ �������� �������
class TL57TextEMLFuncArg
{ public:
    double _Crit;
    TL57TextDoubleArray X_Params;
    TL57TextDoubleArray A_Params;
    TL57TextFuncFArray Functions;

    TL57TextEMLFuncArg(void){_Crit=rdsbcppHugeDouble;};
};
//---------------------------------------------------------------------------

// �������� ������� ���������� �������� ����������� - ��������� (��� ��������� ����������)
int _InFunc(const TL57TextEMLFuncArg &arguments);
// �������� ������� ���������� ��������� ����������� - ��������� (��� ��������� ����������)
int _OutFunc(const TL57TextEMLFuncArg &arguments);
//-----------------------------------------------------------------------

// ������� ���������� �������� ����������� � ���������� ����������
int InFunc(const TL57TextEMLFuncArg &arguments)
{ int ok=1;
  RDSBCPP_TRY
    { ok=_InFunc(arguments); }
  RDSBCPP_CATCHALL
    { ok=0; }
  return ok;
}
//-----------------------------------------------------------------------

// ������� ���������� ��������� ����������� � ���������� ����������
int OutFunc(const TL57TextEMLFuncArg &arguments)
{ int ok=1;
  RDSBCPP_TRY
    { ok=_OutFunc(arguments); }
  RDSBCPP_CATCHALL
    { ok=0; }
  return ok;
}
//-----------------------------------------------------------------------

#endif // #ifdef L57MS_GLOBALS
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// ���������
//---------------------------------------------------------------------------
#ifdef L57MS_SETUP
TL57TextFileEML file;
RDS_HOBJECT win;
BOOL res;
char caption[]="����������� ��� �������� �������������";
TL57TextFileBasic_ServMarker markers[]=
  {{"L57_ECOMAXLIM_INPUT_BEGIN_MARKER", 1,-1},
   {"L57_ECOMAXLIM_INPUT_END_MARKER",   2,-1},
   {"L57_ECOMAXLIM_OUTPUT_BEGIN_MARKER",3,-1},
   {"L57_ECOMAXLIM_OUTPUT_END_MARKER",  4,-1},
   {NULL,0,-1}
  };

RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

if(model==NULL)
  { rdsMessageBox("��� ����� ��������� �������������� ���������� ������. ��������� ������� ����������� ��� "
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

#ifdef L57_HELPUIFILE
  rdsFORMHelpButton(win,RDS_FORMHELP_UI|RDS_FORMHELP_CHECK,L57_HELPUIFILE,L57_HELPTOPIC);
#endif

// ������ �������
file.ReadAndParseModel(rdsbcppBlockData->Block,markers,TRUE);

rdsFORMAddTab(win,1,"�� �����");
rdsFORMAddEdit(win,1,1,RDS_FORMCTRL_MULTILINE,
  "�������� ����� ������� �����������:",80);
rdsSetObjectInt(win,1,RDS_FORMVAL_MLRETURNS,1);
rdsFORMSetMultilineHeight(win,1,4*24);
rdsSetObjectStr(win,1,RDS_FORMVAL_VALUE,file.InputCode.c_str());

rdsFORMAddEdit(win,1,100,RDS_FORMCTRL_MULTILABEL,
"���������� ������ ��������(�) �������� ������������ ������� ���������� � ���������� ����� C. "
"������� ��������� - �������� ������� \"X[...]\". ���� ��������� ���������� ���������, ����� ���� "
"��������� ��������� �������� ���������� \"ok\", ���� ������� ��������� �������� ���������� return. "
"��� ������������� ��������� ���������� ������������� ��� ������������ ����.",0);
rdsSetObjectInt(win,100,RDS_FORMVAL_MLHEIGHT,100);
rdsSetObjectInt(win,100,RDS_FORMVAL_MLABELBEVEL,1);

rdsFORMAddTab(win,2,"�� ������");
rdsFORMAddEdit(win,2,2,RDS_FORMCTRL_MULTILINE,
  "�������� ����� ������� �����������:",80);
rdsSetObjectInt(win,2,RDS_FORMVAL_MLRETURNS,1);
rdsFORMSetMultilineHeight(win,2,4*24);
rdsSetObjectStr(win,2,RDS_FORMVAL_VALUE,file.OutputCode.c_str());

rdsFORMAddEdit(win,2,200,RDS_FORMCTRL_MULTILABEL,
"���������� ������ ��������(�) �������� ������������ �������� ��������������� �������� \"Crit\" � ���������� ����� C. "
"���� �������� ���������, ����� ���� "
"��������� ��������� �������� ���������� \"ok\", ���� ������� ��������� �������� ���������� return. "
"��� ������������� �������� ������������� ��� ������������ ����.",0);
rdsSetObjectInt(win,200,RDS_FORMVAL_MLHEIGHT,100);
rdsSetObjectInt(win,200,RDS_FORMVAL_MLABELBEVEL,1);

res=rdsFORMShowModalServ(win,NULL);

if(res)
  { char *in_code=rdsGetObjectStr(win,1,RDS_FORMVAL_VALUE);
    char *out_code=rdsGetObjectStr(win,2,RDS_FORMVAL_VALUE);
    file.InputCode.Assign(in_code);
    file.OutputCode.Assign(out_code);
    file.SaveFileToModel(rdsbcppBlockData->Block,L57_BAK);
  }

rdsDeleteObject(win);
Result=res?1:0;

#endif // #ifdef L57MS_SETUP
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// �������� � ������
//---------------------------------------------------------------------------
#ifdef L57MS_CLASS
TL57TextEMLFuncArg Arguments;

#endif // #ifdef L57MS_CLASS
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------
#ifdef L57MS_MODEL
// ������� ��������� ������ ���� �������
Ready=Crit_Out_Ready=crit_out_en=0;
y_enabled=0;
Crit_Out=rdsbcppHugeDouble; // ��� �������

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
      }
  }

// ���������
if(A.IsEmpty())
  Arguments.A_Params.Setup(NULL);
else
  Arguments.A_Params.Setup(A.GetBuffer(),A.Size());

if(X.IsEmpty())
  Arguments.X_Params.Setup(NULL);
else
  Arguments.X_Params.Setup(X.GetBuffer(),X.Size());

// ���������
if(x_chg)
  { // ��������� ����� ���������
    x_chg=0;
    Arguments._Crit=rdsbcppHugeDouble;
    if(InFunc(Arguments))
      { // OK
        Y=X;
        y_enabled=1;
        Ready=1;
      }
    else
      { // ������ ���������
        Crit_Out=rdsbcppHugeDouble;
        Crit_Out_Ready=crit_out_en=1;
        Ready=1;
      }
    /*
    if(CritInReady)
      { static int nn=10;
        nn--;
        if(!nn)
          rdsStopCalc();
        rdsMessageBox("InReady","",MB_OK);
      }
    */
    Crit_In_Ready=crit_in_chg=0;
    return;
  }

if(Crit_In_Ready)
  { // �������� ����� ��������
    Crit_In_Ready=crit_in_chg=0;
    Arguments._Crit=Crit_In;
    if(OutFunc(Arguments)) // OK
      Crit_Out=Crit_In;
    else // ������
      Crit_Out=rdsbcppHugeDouble;
    Crit_Out_Ready=crit_out_en=1;
    Ready=1;
  }

#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// ����� �������������
//---------------------------------------------------------------------------
#ifdef L57MS_CALCMODE

#endif // #ifdef L57MS_CALCMODE
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ �������
//---------------------------------------------------------------------------
#ifdef L57MS_START

#endif // #ifdef L57MS_START
//-----------------------------------------------------------------------

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

  #ifdef L57_HELPUIFILE
    switch(CallMode)
      { case RDS_BFM_HELP:
          if(L57_HelpUIFileExists(L57_HELPUIFILE,&L57_HelpState))
            { rdsShowHelp(L57_HELPUIFILE,L57_HELPTOPIC,FALSE);
              Result=1;
            }
          break;
      }
  #endif

#endif // #ifdef L57MS_OTHER
//---------------------------------------------------------------------------

