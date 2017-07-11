//---------------------------------------------------------------------------
// ����������� ����� ������ �������� ���������� ������� (������ 2)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ���������� ��������
//---------------------------------------------------------------------------
#ifdef L57MS_GLOBALS
#ifndef L57_MODELVERSION
  #define L57_MODELVERSION 0
#endif

#ifdef L57_LOG
  #define _LOGBLK (rdsbcppBlockData->Block)
#endif

// �������� ������ ���� ������
#define L57_LATESTMODELVERSION 10

// ��� ����� ������� ������
#define L57_MODELTEMPLATE "$RDSINCLUDE$\\RdsL57Mk2\\IpuL57Mk2SimpleCalc.mdl"

// ��������� ��������� ����� �����
#define L57_MSGCAPTION  "���������� �������"

// ����� ������� � ������
#include <RdsL57Mk2/L57Mk2Basic.cpp>

// ������� ������� ����� �������
int L57_HelpState=0;
// ������� ���������� �������������� �������� ���� ����� � ����������
BOOL L57_AllowTextRectEdit=FALSE;

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
          _Start=1;
        }
      _InsideAcceptFunction=FALSE;
    }
  RDSBCPP_CATCHALL
    { _InsideAcceptFunction=FALSE; }

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
      { 
        if(_Transfer.AcceptParam(Param,rdsbcppBlockData->Block)) // ���-�� ������� (���� ����� �������� � �� ���������)
          { _Start=1; // �������������
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
  /*
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
  */
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
  L57_String modeltext,templatetext,newmodeltext,auxdata;
  RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);
  BOOL FuncByConn=L57_CheckFunctionSource(rdsbcppBlockData->Block,Func_in.Num());
  BOOL res,first=TRUE;
  #if L57_MODELVERSION>=9
    L57_String usertext;
    usertext.Assign(TR_text.c_str());
  #endif

  if(model!=NULL && rdscompIsEditorOpen(model->Model))
    { rdsMessageBox("��� ��������� ����� ����� ���������� ������� �������� ��� ������. "
                    "����������, �������� ��������.",L57_MSGCAPTION,MB_OK|MB_ICONWARNING);
      rdscompOpenBlockModelEditor(rdsbcppBlockData->Block);
      return;
    }

  // �������� ������
  if(!L57_LoadModelFile(rdsbcppBlockData->Block,modeltext))
    rdsMessageBox("������ �������� ������ ������ �����",L57_MSGCAPTION,MB_OK|MB_ICONERROR);
  if(!L57_ReadFileToMemory(L57_MODELTEMPLATE,templatetext))
    rdsMessageBox("������ �������� ������� ������ ����� (" L57_MODELTEMPLATE,L57_MSGCAPTION,MB_OK|MB_ICONERROR);

  #if L57_MODELVERSION>=3
    // �������������� ������ � �������������� �����
    L57_CreateAdditionalText(&auxdata,rdsbcppBlockData->Block,Func_in.Num());
  #endif

  for(;;)
    { RDS_BHANDLE show=NULL;
      res=L57_ExecTool_Edit(first?templatetext.c_str():NULL,modeltext.c_str(),NULL,1,&newmodeltext,
                            NULL,NULL,NULL,NULL,FuncByConn?"0":"",NULL,auxdata.IsEmpty()?NULL:auxdata.c_str()
                            #if L57_MODELVERSION>=3
                            ,&show
                            #endif
                            #if L57_MODELVERSION>=9
                            ,&TR_params,&usertext
                            #endif
                            );
      if(res>=0) // ���������� ��������
        break;
      // ��������� ��������
      if(show==NULL) // ������ ����������
        { res=0;
          break;
        }
      L57_SelectAndShowBlock(show);
      first=FALSE;
    }

  if(res)
    {
      #if L57_MODELVERSION>=9
        TR_text=usertext.c_str();
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
      if((rdsbcppBlockData->Flags & RDS_VARCHECKFAILED)==0) // � ����������� ��� � �������
        _Start=1;
    }

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

// ����������� �������
L57_Calc _Calculator;

// ��������� ������������ ���������� � ����������� �������
void L57InputsToCalc(void);

  #if L57_MODELVERSION>=3
    void L57InputsChange(void);
  #endif

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
  if((rdsbcppBlockData->Flags & RDS_VARCHECKFAILED)==0) // � ����������� ��� � �������
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
  // ����� ������ �������� ��������
  _InsideAcceptFunction=_AcceptRecursionError=_InsideAcceptParam=_AcceptParamRecursionError=_TransferError=FALSE;
  #if L57_MODELVERSION>=3
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

  #if L57_MODELVERSION>=3
    L57InputsChange();
  #endif
  L57InputsToCalc();
  Calc_out=_Calculator._Calc_main();

#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// �������� ����� ������
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS

#endif // #ifdef L57MS_AFTERCLASS
//---------------------------------------------------------------------------

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
    // ������������� ����������� ����������
    L57_Init_Old_Data();
  #endif
#endif // #ifdef L57MS_RESET
//-----------------------------------------------------------------------

