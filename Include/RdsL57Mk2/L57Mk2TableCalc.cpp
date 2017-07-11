//---------------------------------------------------------------------------
// ����������� ����� ������ ���������� ������ �� �������� (������ 2)
//---------------------------------------------------------------------------

// ��������� �������������� ����� ������ "���� ������": L57OutOfMemMsg(0);

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

// ��� ����� ������� ������
#define L57_MODELTEMPLATE "$RDSINCLUDE$\\RdsL57Mk2\\IpuL57Mk2TableCalc.mdl"

// ��������� ��������� ����� �����
#define L57_MSGCAPTION  "������������ ������"

// ����� ������� � ������
#include <RdsL57Mk2/L57Mk2Basic.cpp>

// ��������� ����� (_Stage)
#define L57STAGE_WAIT           0       // ��������
#define L57STAGE_CALC           1       // ������

// �������������� ����� ���� ��������
#define L57SETUPFIELD_NOAUTOSTRT        1       // �������� ����� ������ �� �������
#define L57SETUPFIELD_TACTCOUNT         2       // ����� �������� � �����
#define L57SETUPFIELD_TITLE             1000    // �������������� - ��������� �������
#define L57SETUPFIELD_CODE              1001    // �������������� - ����� �������
#define L57SETUPFIELD_PARAMS            1002    // �������������� - ������ ����������
#define L57SETUPFIELD_EDITBTN           1003    // ������ ������ ���� ��������������
#define L57SETUPFIELD_TEMPLATE          2000    // ������������ - ������ ������
#define L57SETUPFIELD_MODEL             2001    // ������������ - ����� ������
#define L57SETUPFIELD_LOCKEDIT          2002    // ������������ - ������� ���������� ��������������
#define L57SETUPFIELD_ADDITIONALDATA    2003    // ������������ - �������������� ������
#define L57SETUPFIELD_BLOCKTOSHOW       2004    // ������������ - ����, ������� ����� �������� ��� ��������� �������� ����

// ������� ������� ����� �������
int L57_HelpState=0;
// ������� ���������� �������������� �������� ���� ����� � ����������
BOOL L57_AllowTextRectEdit=FALSE;

//-----------------------------------------------------------------------
// ������ ������������� ���������
//-----------------------------------------------------------------------
class L57_Parameter
{ private:
    // ������� ������ ����������
    BOOL AllocateArray(int count);

  public:
    // �������� ������
    double Min,Max;     // �������� �������� ��������
    double Step;        // ���

    // ��������������� ������ ���������� � ������� ������
    double _Min,_Max,_Delta;    // ����������������� �������� ��������� � ����
    L57_DblArray Array;         // ������ �������� ���������
    int _ArgIndex;              // ����� �������� �������� � �������� �������� � ������� Array

    double Current;             // ������� �������� ��� �������� � ������� ����������

    // �������� ������ ����������
    void ClearArrays(void);
    // ������� ������ ���������� �� �������� ������
    BOOL AllocateArray(void);

    L57_Parameter(void)
      { Min=Max=Step=rdsbcppHugeDouble;
      };
    ~L57_Parameter(){ClearArrays();};
};
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// ������ ������������� ���������
//-----------------------------------------------------------------------
// �������� ������ ����������
void L57_Parameter::ClearArrays(void)
{
  Array.Clear();
  Array.PrevCount=0;
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
{ int n=L57_CalcNPointsByDiap(Min,Max,Step);
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
    { _Min=Min; _Max=Max; _Delta=Step; }

  for(int xi=0;xi<n;xi++)
    { double v=L57_ValueInDiap(_Min,_Max,_Delta,xi,n);
      Array.Data[xi]=v;
    }
  return TRUE;
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// ����� ������������ ����������
//-----------------------------------------------------------------------
// ������ ���� ��� ���������� ������ L57_PARAMSCOUNT!
class L57_Parameters
{ public:
    #if L57_PARAMSCOUNT!=0
      L57_Parameter P[L57_PARAMSCOUNT];
    #endif

    // ��������� ��� � ������ ����������
    BOOL NextStep(void);

    L57_Parameters(void){};
    ~L57_Parameters(){ };
};
//-----------------------------------------------------------------------

// ��������� ��� � ������ ����������
BOOL L57_Parameters::NextStep(void)
{ BOOL changed=FALSE;

#if L57_PARAMSCOUNT!=0
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
// ����� �������� ����������
//---------------------------------------------------------------------------
#ifdef L57MS_VARCHECK
  #if L57_MODELVERSION>=3
    L57_Create_Trans_Data();
  #endif
#endif // #ifdef L57MS_VARCHECK
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������ �������
//---------------------------------------------------------------------------
#ifdef L57MS_OTHER

switch(CallMode)
  { case RDS_BFM_LEAVINGEDITMODE:
      L57_SendSingleFunctionIfNoInput(rdsbcppBlockData->Block,Func_in.Num(),Func_out.Num(),FALSE,
                                      _CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str());
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

#endif // #ifdef L57MS_ACCEPTFUNC


//---------------------------------------------------------------------------
// ���������
//---------------------------------------------------------------------------
#ifdef L57MS_SETUP
  RDS_HOBJECT win;
  BOOL res;
  int tabid=0,fieldid;
  L57_String Func,Inputs,Numbered,Title;
  L57_String modeltext,templatetext,auxdata;

  RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

  BOOL FuncByConn=L57_CheckFunctionSource(rdsbcppBlockData->Block,Func_in.Num());


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

  // ��������� ���� - ���������� ��������������
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_LOCKEDIT,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,FuncByConn?"0":"");

  #if L57_MODELVERSION>=3
    // ��������� ����: �������������� ������ � �������������� �����
    L57_CreateAdditionalText(&auxdata,rdsbcppBlockData->Block,Func_in.Num());
    rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_ADDITIONALDATA,RDS_FORMCTRL_NONVISUAL,"",0);
    rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,auxdata.c_str());
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
  rdsFORMAddTab(win,++tabid,"����������");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_NOAUTOSTRT,RDS_FORMCTRL_CHECKBOX,
                 "�������� ���������� ������ �� ������� (������ Start_cmd)",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,noautostart_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TACTCOUNT,RDS_FORMCTRL_EDIT,
                 "����� �������� � ����� ����� �������:",80);
  rdsFORMSetInt(win,fieldid,tactcount_setup);

  #if L57_MODELVERSION>=9
    // ������� �������� ����
    if(L57_AllowTextRectEdit)
      { tabid++;
        L57_TextRectEdit_Setup(rdsbcppBlockData->Block,win,TR_params,TR_text.c_str(),tabid);
      }
  #endif

  Setup_FillFuncDescr(win,Func,Inputs,Numbered,Title);

  // ��������� ����
  res=rdsFORMShowModalServ(win,Setup_WinCallback);

  if(res)
    { char *newmodeltext=rdsGetObjectStr(win,L57SETUPFIELD_MODEL,RDS_FORMVAL_VALUE);

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


      if(modeltext.IsNotEqual(newmodeltext))
        { BOOL ok=L57_SaveModelFile(rdsbcppBlockData->Block,newmodeltext);
          if(ok)
            { L57_CompileModel(rdsbcppBlockData->Block);
            }
          else
            rdsMessageBox("������ ������ ������ ������ �����",L57_MSGCAPTION,MB_OK|MB_ICONERROR);
        }
       noautostart_setup=(rdsGetObjectInt(win,L57SETUPFIELD_NOAUTOSTRT,RDS_FORMVAL_VALUE)!=0);
       tactcount_setup=rdsGetObjectInt(win,L57SETUPFIELD_TACTCOUNT,RDS_FORMVAL_VALUE);
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

// ��������� ������� ����������
BOOL _InParamsChange;

// ��������
double _Progress_Cur,_Progress_Max;   // �� ������ ���������� ������� �������� ������

// ����������� �������
L57_Calc _Calculator;

// ����� ������������ ����������
L57_Parameters _Params;

// �����
DWORD _dwStartTime;
// ��������� �����
void _ShowTime(void)
  { Time_out=((double)(GetTickCount()-_dwStartTime))/1000.0; };

// ��������� "���� ������" � �������
void L57OutOfMemMsg(int n);
// �������� ��� �������� ������ (��� ������)
void L57ClearOutputData(void);
// �������� ��������� ����������
// *pChgSignalOn ��������� ��� ������� ������� ��������� (���������� �� ������������ ���������)
BOOL L57InputsChange(BOOL _force,BOOL *pChgSignalOn);
// ������ ���������� (�����. ���������� �������������)
BOOL L57StartIterations(void);
// �������� ��������� �������� ������
void L57SetErrorOutputData(void);
// ����� ���������
void Model_ShowProgress(void);
// ��������� ������������ ���������� � ����������� �������
void L57InputsToCalc(void);
// �������� ������� �������� ������
void L57SetOutputData(void);
// ������������� ������ ������������ ����������
void L57InitParams(void);
// ��������� ������� �������� ���������� � ����������� �������
void L57ParamsToCalc(void);
// ����������
void Model_Calc(void);

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

  if(rdsGetSystemInt(RDS_GSISAVELOADACTION)==RDS_LS_LOADAUTOCOMP)
    { // ��� ����������� ������ ����� �������������� ������� �������� ������� �� ������
      L57_SendSingleFunction(rdsbcppBlockData->Block,Func_out.Num(),TRUE, // Immediate=TRUE !
                             _CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str(),NULL);
    }
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
  // ��������� ������� ���������� (��� ������� �������)
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
      _enabled_res=1;
      for(int i=0;enabled && i<size;i++)
        if(!Enabled_in[i])
          { enabled=FALSE;
            _enabled_res=0;
          }
    }
  else
    enabled=_enabled_res;

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
      else // ��������� ������������� ��� ���������� ������
        dostart=_InParamsChange;
    }

  if(dostart)
    { // ������ �������
      _InParamsChange=FALSE;
      Restart_cmd=0;
      Start_cmd=0;
      if(!L57StartIterations())
        return;
      Started_out=1;
      _dwStartTime=GetTickCount();
    }
  else
    { if(_Stage==L57STAGE_WAIT && _somethingchanged && (!noautostart_setup)) // �� ��������� ������ � �� ��������
        {
          Finished_out=1;
          Complete_out=1;
          Error_out=0;
          Time_out=0;
          _Ready=1;
          return;
        }
    }

  switch(_Stage)
    { case L57STAGE_WAIT:
        return;
      case L57STAGE_CALC:
        Model_Calc();
        break;
      default:
        rdsStopCalc();
        L57ClearOutputData();
        _Stage=L57STAGE_WAIT;
        rdsBlockMessageBox(NULL,"���������� ������ ����������� ������ - ����������� ����� ������",
                      L57_MSGCAPTION,MB_OK|MB_ICONERROR);
        return;
    }
  Model_ShowProgress();
  _Ready=1;
  _Start=1;
  _ShowTime();
#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// �������� ����� ������
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS

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

  ADDTOPROGRESS(_Progress_Cur,_Progress_Max)

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
// ������� ��������� ���������� �������� � ���� ���������
void Setup_FillFuncDescr(RDS_HOBJECT win,L57_String &Func,L57_String &Inputs,L57_String &Numbered,L57_String &Title)
{ L57_String str;
  // ������������ ������� ����������
  if(Numbered.IsNotEmpty())
    { str.Assign("������������ ���������: ");
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
  L57_String Func,Inputs,Numbered,Title,newmodeltext;
  int starttab=1,res;
  RDS_BHANDLE show;

  // ��������� ���� - ���������� ��������������
  lock=rdsGetObjectStr(win,L57SETUPFIELD_LOCKEDIT,RDS_FORMVAL_VALUE);

  #if L57_MODELVERSION>=3
    // ��������� ����: �������������� ������ � �������������� �����
    auxdata=rdsGetObjectStr(win,L57SETUPFIELD_ADDITIONALDATA,RDS_FORMVAL_VALUE);
  #else
    auxdata=NULL;
  #endif

  switch(data->Event)
    {
      case RDS_FORMSERVEVENT_CHANGE:
        #if L57_MODELVERSION>=9
          if(L57_AllowTextRectEdit)
            L57_TextRectEdit_EnableControls(win);
        #endif
        break;

      #if L57_MODELVERSION>=3
      case RDS_FORMSERVEVENT_TCREOPEN:   // �������� ����� ���������� ��������
        templatetext=NULL;
      #endif
      case RDS_FORMSERVEVENT_CLICK:
        if(data->Event==RDS_FORMSERVEVENT_CLICK)
          templatetext=rdsGetObjectStr(win,L57SETUPFIELD_TEMPLATE,RDS_FORMVAL_VALUE);
        modeltext=rdsGetObjectStr(win,L57SETUPFIELD_MODEL,RDS_FORMVAL_VALUE);
        res=L57_ExecTool_Edit(templatetext,modeltext,NULL,starttab,&newmodeltext,
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

      #if L57_MODELVERSION>=3
      case RDS_FORMSERVEVENT_TCREACTION: // ������� ����� ���������� ��������
        show=rdsBlockByFullName(rdsGetObjectStr(win,L57SETUPFIELD_BLOCKTOSHOW,RDS_FORMVAL_VALUE),NULL);
        L57_SelectAndShowBlock(show);
        break;
      #endif

    }
}
//---------------------------------------------------------------------------

// ��������� "���� ������" � �������
void rdsbcppBlockClass::L57OutOfMemMsg(int n)
{ L57_String msg,nstr;
  rdsStopCalc();
  L57ClearOutputData();
  _Stage=L57STAGE_WAIT;
  nstr.AssignInt(n);
  msg.Assign("���� ������ ��� ������ ������������� ������ (");
  msg.AddString(nstr);
  msg.AddString(")");
  rdsBlockMessageBox(NULL,msg.c_str(),L57_MSGCAPTION,MB_OK|MB_ICONERROR);
}
//---------------------------------------------------------------------------

// �������� ��� �������� ������ (��� ������)
void rdsbcppBlockClass::L57ClearOutputData(void)
{
  Table_out.Resize(0,0);
  #if L57_PARAMSCOUNT!=0
    for(int i=0;i<L57_PARAMSCOUNT;i++)
      _Params.P[i].ClearArrays();
  #endif
  _results_ok=0;
  Complete_out=0;
  Error_out=0;
}
//---------------------------------------------------------------------------

// �������� ��������� �������� ������
void rdsbcppBlockClass::L57SetErrorOutputData(void)
{
  Table_out.Resize(0,0);
  #if L57_PARAMSCOUNT!=0
    for(int i=0;i<L57_PARAMSCOUNT;i++)
      _Params.P[i].ClearArrays();
  #endif
  _results_ok=1;
  Progress_out=0.0;
  Complete_out=Error_out=Finished_out=1;
}
//---------------------------------------------------------------------------

// �������� ������� �������� ������
void rdsbcppBlockClass::L57SetOutputData(void)
{

  #if L57_PARAMSCOUNT!=0
    for(int i=0;i<L57_PARAMSCOUNT;i++)
      _Params.P[i].ClearArrays();
  #endif
  _results_ok=1;
  Progress_out=1.0;
  Complete_out=Finished_out=1;
  Error_out=0;
}
//---------------------------------------------------------------------------

// ������ ���������� (�����. ���������� �������������)
#if L57_PARAMSCOUNT!=0
BOOL rdsbcppBlockClass::L57StartIterations(void)
{ BOOL _badalloc=FALSE;

  _Progress_Cur=0.0;
  _Progress_Max=1.0;

  L57InputsToCalc();
  L57ClearOutputData();
  L57InitParams();

  Complete_out=Error_out=Finished_out=0;

  _Stage=L57STAGE_CALC;

  // ������� ������� ����������
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=_Params.P+i;
      if(!p->AllocateArray())
        _badalloc=TRUE;
      p->_ArgIndex=0;
    }

  // ��������� �������� �������
  #if L57_PARAMSCOUNT==1
    // ���� ��������
    if(!_badalloc)
      { L57_Parameter *_p1=_Params.P;
        int cnt1=_p1->Array.Count;
        if(Table_out.Resize(cnt1,2))
          { for(int i=0;i<cnt1;i++)
              Table_out[i][0]=_p1->Array.Data[i];
            _Progress_Max=cnt1;
          }
        else
          _badalloc=TRUE;
      }
  #else
    // ��� ���������
    if(!_badalloc)
      { L57_Parameter *_p1=_Params.P,
                      *_p2=_Params.P+1;
        int cnt1=_p1->Array.Count,
            cnt2=_p2->Array.Count;
        if(Table_out.Resize(cnt1+1,cnt2+1))
          { Table_out[0][0]=rdsbcppHugeDouble;
            for(int i=0;i<cnt1;i++)
              Table_out[i+1][0]=_p1->Array.Data[i];
            for(int i=0;i<cnt2;i++)
              Table_out[0][i+1]=_p2->Array.Data[i];
            _Progress_Max=((double)cnt1)*((double)cnt2);
          }
        else
          _badalloc=TRUE;
      }
  #endif

  if(_badalloc)
   { rdsStopCalc();
     L57ClearOutputData();
     _Stage=L57STAGE_WAIT;
     L57OutOfMemMsg(0);
     return FALSE;
   }
  return TRUE;
}
#else // #if L57_PARAMSCOUNT!=0
BOOL rdsbcppBlockClass::L57StartIterations(void)
{ _Stage=L57STAGE_WAIT;
  L57SetErrorOutputData();
  return TRUE;
}
#endif
//---------------------------------------------------------------------------

// ����������
void rdsbcppBlockClass::Model_Calc(void)
{

  #if L57_PARAMSCOUNT==0
    L57SetOutputData();
    _Stage=L57STAGE_WAIT;
    _Ready=1;
  #else
    L57_LoopBreak _check;
    for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
      { // ��������� ����� _ArgIndex ���� ����������
        double val;
        _Progress_Cur++;
        #if L57_PARAMSCOUNT==1
          // ���� ��������
          L57_Parameter *p1=_Params.P;
          p1->Current=p1->Array.Data[p1->_ArgIndex];
          // ��������� �������
          L57ParamsToCalc();
          val=_Calculator._Calc_main();
          // ������� ������ � �������
          Table_out[p1->_ArgIndex][1]=val;
        #else
          // ��� ���������
          L57_Parameter *p1=_Params.P,
                        *p2=_Params.P+1;
          p1->Current=p1->Array.Data[p1->_ArgIndex];
          p2->Current=p2->Array.Data[p2->_ArgIndex];
          // ��������� �������
          L57ParamsToCalc();
          val=_Calculator._Calc_main();
          // ������� ������ � �������
          Table_out[p1->_ArgIndex+1][p2->_ArgIndex+1]=val;
        #endif
        // ����� ��������� ������
        if(_Params.NextStep())
          { // ������� ��������
            L57SetOutputData();
            _Stage=L57STAGE_WAIT;
            _Ready=1;
            return;
          }
        if(_check.Check())
          break;
      }
  #endif
}
//---------------------------------------------------------------------------

#endif // #ifdef L57MS_AFTERCLASS
//---------------------------------------------------------------------------

