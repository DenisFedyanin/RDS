//---------------------------------------------------------------------------
// ����������� ����� ������ ����� ������ ���������� (������ 2)
//---------------------------------------------------------------------------

// ��������� �������������� ����� ������ "���� ������": L57OutOfMemMsg(0);

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
#define L57_LATESTMODELVERSION 12

// ��� ����� ������� ������
#define L57_MODELTEMPLATE "$RDSINCLUDE$\\RdsL57Mk2\\IpuL57Mk2Stab.mdl"

// ��������� ��������� ����� �����
#define L57_MSGCAPTION  "����� ����������"

// ����� ������� � ������
#include <RdsL57Mk2/L57Mk2Basic.cpp>

// ��������� ����� (_Stage)
#define L57STAGE_WAIT           0       // ��������
#define L57STAGE_CALC           1       // ������

// ������ ���������� �������� ��������� �������
#define L57TABFILL_REALLOC      0       // ��� ���������� ������� ��������� �� ������
#define L57TABFILL_PACK         1       // ��� ���������� ������� �������� ������ ������ ������
#define L57TABFILL_STOP         2       // ��� ���������� ������� ���������� ������
#define L57TABFILL_TAIL         3       // ��� ���������� ������� ������� ������, �������� ����� ��������
                                        // (��������� ������ "�����")

// �������������� ����� ���� ��������
#define L57SETUPFIELD_NOAUTOSTRT        1       // �������� ����� ������ �� �������
#define L57SETUPFIELD_TACTCOUNT         2       // ����� �������� � �����
#define L57SETUPFIELD_DELTACYCLES       3       // ����� �������� � ������ �����������
#define L57SETUPFIELD_DELTA             4       // �����������
#define L57SETUPFIELD_MAXITER           5       // ������������ ����� ��������
#define L57SETUPFIELD_TABMODE           6       // ����� ���������� ��������� �������
#define L57SETUPFIELD_TABINITCOUNT      7       // �������� ����� �������� � ��������� ��������
#define L57SETUPFIELD_TABSTEP           8       // �������� ��� ������ � ��������� �������� (����� ������������� � ��� ����)
#define L57SETUPFIELD_TITLE             1000    // �������������� - ��������� �������
#define L57SETUPFIELD_CODE              1001    // �������������� - ����� �������
#define L57SETUPFIELD_PARAMS            1002    // �������������� - ������ ����������
#define L57SETUPFIELD_EDITBTN           1003    // ������ ������ ���� ��������������
#define L57SETUPFIELD_OUTTABBTN         1004    // ������ ������ ���� �������������� � �������� �������� ������
#define L57SETUPFIELD_TEMPLATE          2000    // ������������ - ������ ������
#define L57SETUPFIELD_MODEL             2001    // ������������ - ����� ������
#define L57SETUPFIELD_LOCKFUNC          2002    // ������������ - ������� ���������� �������������� �� ��������
#define L57SETUPFIELD_ADDITIONALDATA    2003    // ������������ - �������������� ������
#define L57SETUPFIELD_BLOCKTOSHOW       2004    // ������������ - ����, ������� ����� �������� ��� ��������� �������� ����

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

  #if L57_MODELVERSION>=9
    if(rdsSystemInEditMode())
      rdsAdditionalContextMenuItemEx("�������� ��� �������",0,1,0);
  #endif

#endif // #ifdef L57MS_POPUP
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����� ������ ������������ ����
//---------------------------------------------------------------------------
#ifdef L57MS_MENUSELECT

  #if L57_MODELVERSION>=9
    if(rdsSystemInEditMode() &&
       rdsMessageBox("�������� ��� ������� ������ �����?",L57_MSGCAPTION,MB_ICONWARNING|MB_YESNO)==IDYES)
      { L57_ExecTool_ClearAll(rdsbcppBlockData->Block,L57_MODELTEMPLATE);
      }
  #endif

#endif // #ifdef L57MS_MENUSELECT
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������ �������
//---------------------------------------------------------------------------
#ifdef L57MS_OTHER

switch(CallMode)
  { case RDS_BFM_LEAVINGEDITMODE:
      if(!_FuncConnDataReady)
        { // � ������ ������ �� �������� - ������, ������� ������� �� ����� ����� ��� �� ���������
          // ��������, ����� ����� ������� ������������
          BOOL connected=L57_CheckFunctionSourceMulti(rdsbcppBlockData->Block,Func_in.Num(),
                                                      &_FuncConnectedInputs,&_FuncConnectedAll);
          _FuncConnDataReady=TRUE;
          if(!connected) // ������ �� ������������
            L57_SendOutputFunctions(rdsbcppBlockData->Block,Func_out.Num(),FALSE,
                                    _FuncExportTexts,L57_FUNCCOUNT,NULL/*&_CurFuncExportTexts*/,NULL/*queue*/);
        }
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

    #if L57_MODELVERSION>=11
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
    if(Param==NULL)
      return;

    if(!L57_CANACCESSFIELD(Param,AcceptIndex)) // �������� ������ ���������
      return;

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
      { if(!Param->Immediate) // ������������� ��� ������ �� ��������������, � �� ������ �����
          { // ��������, ����� ����� ������� ������������, ���� ��� ��� �� ��������
            if(!_FuncConnDataReady)
              { L57_CheckFunctionSourceMulti(rdsbcppBlockData->Block,Func_in.Num(),
                                             &_FuncConnectedInputs,&_FuncConnectedAll);
                _FuncConnDataReady=TRUE;
                // ���-�� ����� ������������, ����� �� �� ���� ����� ������
              }
          }
        if(Param->TextIndex==-1 && Param->AcceptIndex==-1)
          { // ������ ��� �������
            BOOL ok;
            // ���������� ���������
            _CurFuncExportTexts.Clear();
            _CurFuncExportTexts.CreateItem(Param->TextsCount-1);
            for(int i=0;i<Param->TextsCount;i++)
              _CurFuncExportTexts.ItemPtrFAST(i)->Assign(Param->Texts[i]);
            // ��������, ��� ��� ������
            _FuncConnectedInputs.Clear();
            // ��������� ���� ������� � ������
            if(L57_MustImportFunctions(&_CurFuncExportTexts,_FuncExportTexts,L57_FUNCCOUNT))
              ok=L57_ExecTool_ImportFunctions(rdsbcppBlockData->Block,L57_MODELTEMPLATE,
                                             _FuncExportTexts,L57_FUNCCOUNT,&_CurFuncExportTexts);
            else
              ok=TRUE;
            if(ok)
              { // �������� ������
                L57_SendOutputFunctions(rdsbcppBlockData->Block,Func_out.Num(),Param->Immediate,
                                        _FuncExportTexts,L57_FUNCCOUNT,&_CurFuncExportTexts,queue);
              }
          }
        else if(Param->TextIndex!=-1 && Param->AcceptIndex!=-1)
          { // ���� �� ��� -1, �� ��� ������ ���� ��������
            // ���������� ��������� �������
            _CurFuncExportTexts.CreateItem(Param->AcceptIndex);
            _CurFuncExportTexts.ItemPtrFAST(Param->AcceptIndex)->Assign(Param->Texts[Param->TextIndex]);
            if(Param->Immediate) // ���������� ����������
              { // ����� ������� Param->Texts[Param->TextIndex] � ������ ��� ������� Param->AcceptIndex
                if(L57_GetCertainFunctionByConn(rdsbcppBlockData->Block,L57_MODELTEMPLATE,Param->AcceptIndex,
                                                Param->TextIndex<L57_FUNCCOUNT?_FuncExportTexts[Param->TextIndex]:NULL,
                                                Param->Texts[Param->TextIndex]))
                  { // �������� ������ ���
                    L57_SendOutputFunctions(rdsbcppBlockData->Block,Func_out.Num(),Param->Immediate,
                                            _FuncExportTexts,L57_FUNCCOUNT,&_CurFuncExportTexts,queue);
                  }
              }
            else // ���������� ������ �� ���������� ����
              { // ������ ������� �� ���� Param->AcceptIndex - ������ �� ���� ��
                _FuncConnectedInputs.DeleteByValue(Param->AcceptIndex);
                if(_FuncConnectedInputs.Count==0) // ������ ��� �����
                  { // ��������� ���� ������� � ������
                    BOOL ok;
                    if(L57_MustImportFunctions(&_CurFuncExportTexts,_FuncExportTexts,L57_FUNCCOUNT))
                      ok=L57_ExecTool_ImportFunctions(rdsbcppBlockData->Block,L57_MODELTEMPLATE,
                                                      _FuncExportTexts,L57_FUNCCOUNT,&_CurFuncExportTexts);
                    else // ��� ��������, ��� ����
                      ok=TRUE;
                    if(ok)
                      { // �������� ������ ���
                        L57_SendOutputFunctions(rdsbcppBlockData->Block,Func_out.Num(),Param->Immediate,
                                                _FuncExportTexts,L57_FUNCCOUNT,&_CurFuncExportTexts,queue);
                      }
                    _CurFuncExportTexts.Clear();
                  } // if(_FuncConnectedInputs.Count==0)
              } // if(Param->Immediate)...else
          } // else if(Param->TextIndex!=-1 && Param->AcceptIndex!=-1)

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
  L57_String Func,Inputs,Numbered,Title,auxdata;
  L57_String modeltext,templatetext;
  char lockarray[L57_FUNCCOUNT+1];

  RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

  // ����������� ������������ ������ �������
  BOOL AllFuncByConn=FALSE,somethinglocked=FALSE;
  memset(lockarray,'+',L57_FUNCCOUNT);
  lockarray[L57_FUNCCOUNT]=0;
  L57_CheckFunctionSourceMulti(rdsbcppBlockData->Block,Func_in.Num(),
                               &_FuncConnectedInputs,&AllFuncByConn);
  if(AllFuncByConn)
    memset(lockarray,'-',L57_FUNCCOUNT);
  else
    for(int i=0;i<_FuncConnectedInputs.Count;i++)
      { int n=_FuncConnectedInputs.Items[i];
        if(n>=0 && n<L57_FUNCCOUNT)
          { lockarray[n]='-';
            somethinglocked=TRUE;
          }
      }

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

  // ������� �������������� ������� � ����������
  rdsFORMAddTab(win,++tabid,"�������");

  // ��������� ����: ����� ������� � ����� ������
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TEMPLATE,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,templatetext.c_str());
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_MODEL,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,modeltext.c_str());

  // ��������� ���� - ���������� �������������� �� ��������
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_LOCKFUNC,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,lockarray);

  #if L57_MODELVERSION>=3
    // ��������� ����: �������������� ������ � �������������� �����
    L57_CreateAdditionalText(&auxdata,rdsbcppBlockData->Block,Func_in.Num());
    rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_ADDITIONALDATA,RDS_FORMCTRL_NONVISUAL,"",0);
    rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,auxdata.c_str());
  #endif

  rdsFORMAddEdit(win,tabid,L57SETUPFIELD_TITLE,RDS_FORMCTRL_DISPLAY,"����� ������� � �����:",50);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_CODE,RDS_FORMCTRL_MULTILINE,
      somethinglocked?"������� (����������� �� ������):":"������� (������� \"�������� �������...\" ��� ��������������):",80);
  rdsFORMSetMultilineHeight(win,fieldid,6*24);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLREADONLY,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLRETURNS,1);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_PARAMS,RDS_FORMCTRL_MULTILINE,"",0);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLHEIGHT,80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLABELBEVEL,1);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_EDITBTN,RDS_FORMCTRL_BUTTON,"",80);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,
    somethinglocked?"������� � ���������� �����":"�������� ������� � ���������� �����");

  // ������� ����������
  rdsFORMAddTab(win,++tabid,"�����");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_NOAUTOSTRT,RDS_FORMCTRL_CHECKBOX,
                 "�������� ����� ������ �� ������� (������ Start_cmd)",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,noautostart_setup?1:0);


  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DELTA,RDS_FORMCTRL_EDIT,
                 "���������� ����������� ��������� ��������:",80);
  rdsSetObjectDouble(win,fieldid,RDS_FORMVAL_VALUE,Delta_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DELTACYCLES,RDS_FORMCTRL_UPDOWN,
                 "�������� � ������ �����������:",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNINC,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMIN,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMAX,1000);
  rdsFORMSetInt(win,fieldid,DeltaCycles_setup);


  // ������� �������� ������
  rdsFORMAddTab(win,++tabid,"�������� �������");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_OUTTABBTN,RDS_FORMCTRL_BUTTON,"",80);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,"�������� �������� ��������� �������");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABMODE,RDS_FORMCTRL_COMBOLIST,"����� ������:",400);
  rdsFORMSetComboList(win,fieldid,"������ ��� �����������\n"
                                  "��� ���������� ����������� ������ ������ ������\n"
                                  "��� ���������� ���������� ������\n"
                                  "��� ���������� ������� ������");
  rdsFORMSetInt(win,fieldid,TableFillMode_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABINITCOUNT,RDS_FORMCTRL_EDIT,
                 "����� �������� � ��������:",80);
  rdsFORMSetInt(win,fieldid,TableInitCount_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABSTEP,RDS_FORMCTRL_UPDOWN,
                 "��� ������ ������ (����� �������� � �������):",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNINC,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMIN,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMAX,1000);
  rdsFORMSetInt(win,fieldid,TableFillStep_setup);


  // �������������� �������
  rdsFORMAddTab(win,++tabid,"�������������");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TACTCOUNT,RDS_FORMCTRL_EDIT,
                 "����� �������� � ����� ����� �������:",80);
  rdsFORMSetInt(win,fieldid,tactcount_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_MAXITER,RDS_FORMCTRL_EDIT,
                 "����������� ���������� ����� ��������:",80);
  rdsFORMSetInt(win,fieldid,MaxIterations_setup);

  #if L57_MODELVERSION>=11
    // ������� �������� ����
    if(L57_AllowTextRectEdit)
      { tabid++;
        L57_TextRectEdit_Setup(rdsbcppBlockData->Block,win,TR_params,TR_text.c_str(),tabid);
      }
  #endif

  Setup_FillFuncDescr(win);

  // ��������� ����
  res=rdsFORMShowModalServ(win,Setup_WinCallback);

  if(res)
    { char *newmodeltext=rdsGetObjectStr(win,L57SETUPFIELD_MODEL,RDS_FORMVAL_VALUE);

      #if L57_MODELVERSION>=11
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

       TableFillMode_setup=rdsFORMGetInt(win,L57SETUPFIELD_TABMODE);
       TableInitCount_setup=rdsFORMGetInt(win,L57SETUPFIELD_TABINITCOUNT);
       TableFillStep_setup=rdsFORMGetInt(win,L57SETUPFIELD_TABSTEP);

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

#if L57_FUNCCOUNT!=0
// ������� ������� ������� ��� �������������� � ������� ������� �� ������
static char *_FuncExportTexts[L57_FUNCCOUNT];
#endif
// ������ ��������� ������ �������, � ������� ���� ���������
L57_IntArray _FuncConnectedInputs;
// ������� ������������� ����� �������� ������� �������
BOOL _FuncConnectedAll;
// ������� ����, ��� ������� ������ ������� ����������
BOOL _FuncConnDataReady;
// ������� ������ �������, ���������� ����� ���������� �������������� (����� ���������� ��
// _FuncExportTexts �� ��������������)
L57_StringArray _CurFuncExportTexts;

// ����� �������� �������� ������ �������
BOOL _InsideAcceptFunction,_AcceptRecursionError,_TransferError;
// ����� �������� �������� ������ ���������
BOOL _InsideAcceptParam,_AcceptParamRecursionError;

// ��������� ������ ����������
BOOL _InParamsChange;

int _CyclesInDelta; // �������� � ������ �����������

// �����
DWORD _dwStartTime;
// ��������� �����
void _ShowTime(void)
  { Time_out=((double)(GetTickCount()-_dwStartTime))/1000.0; };

int TableCount_current;         // ������ �������� ��������� ������� �� ������ ������
int TableFillStep_current;      // ��� ������ �������� ��������� ������� �� ������ ������

// ��������� ������ ������� ������� �� ������ ��������
int L57TableIndex(int iteration);
// ���������� ������������� ������� ������ � �������� ��� �������� ��������
// ������� - ������ ������ � ��������
int L57PrepareTableRow(int iteration);
// ����������� �������
BOOL L57ReallocAllTables(void);
// ��������� ��� �������
BOOL L57PackAllTables(void);
// ��������� ���� ������� (��������� �������� �������)
BOOL L57PackTable(rdsbcppVarAncestor *Matr);
// ������������ ������� - ����������� ��� �������
BOOL L57ReallocAllTables_gen(int newrowcount);
// ������������ ������� - ��������� ��� �������
BOOL L57PackAllTables_gen(void);
// ������������ ������� - �������� ��� �������
void L57ClearTabFunc_gen(void);
#if L57_GENTABLESCOUNT!=0
// ������������ ������� - �������� ������ �������� �� ��� �������
void L57WriteIteraionRow_gen(int iteration,int rowindex);
// ������������ ������� - �������� � ���� ������ "�����" � �������� � �����������, ���� ����
void L57FinalizeTables_gen(BOOL sort);
#endif
// �������� � ������� �����" � �������� � �����������, ���� ����
void L57FinalizeTable(rdsbcppVarAncestor *Matr,BOOL sort);
// ��������� "���� ������" � �������
void L57OutOfMemMsg(int n);
// �������� ��������� ����������
BOOL L57InputsChange(BOOL _force,BOOL *pChgSignalOn);
// ������ ������ (�����. ���������� �������������)
BOOL L57StartIterations(void);
// �����
void Model_Calc(void);
// �������� ��� �������� ������ (��� ������)
void L57ClearOutputData(void);
// �������� ��������� �������� ������
void L57SetErrorOutputData(void);
// �������� ������� �������� ������
void L57SetOutputData(void);
// ��������� ������������ ���������� � ��������� �������� � ����������� �������
void L57InputsToCalc(void);
// �������� (���������� TRUE, ���� ������������ ��������� � ������� �������� �� ������ ��������� � ����������)
#if L57_MODELVERSION>=1
  BOOL L57CalcIteration(BOOL &HugeValError);
#else
  BOOL L57CalcIteration(void);
#endif
// ����������� ������� �������� ������������ � �������� ������
void L57DataToMainOuts(void);
// ����������� ������� �������� ������������ �� ��������������� ������
void L57DataToAuxOuts(void);
// ������� ���� �������
void L57ClearOuts(void);
// �������� ���������� � ������� ��������� ������ �������
void L57ResetFuncInputData(void);

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
    L57_SendOutputFunctions(rdsbcppBlockData->Block,Func_out.Num(),FALSE,
                            _FuncExportTexts,L57_FUNCCOUNT,NULL/*&_CurFuncExportTexts*/,NULL/*queue*/);
  }
L57_LoadModelFromUndo(rdsbcppBlockData->Block,LoadedText);
if(_InParamsChange && (rdsbcppBlockData->Flags & RDS_VARCHECKFAILED)==0) // � ����������� ��� � �������
  _Start=1;

  #if defined(L57_TITLES) && L57_MODELVERSION>=11
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
  L57ResetFuncInputData();
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
// ����� ��������������
//---------------------------------------------------------------------------
#ifdef L57MS_EDITMODE
  L57ResetFuncInputData();
#endif // #ifdef L57MS_EDITMODE
//-----------------------------------------------------------------------

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
  // ��������� ������ ���������� (��� ������� ������)
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
  #if defined(L57_TITLES) && L57_MODELVERSION>=11
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
    { // ������ ������
      _InParamsChange=FALSE;
      Restart_cmd=0;
      Start_cmd=0;
      if(!L57StartIterations())
        return;
      Started_out=1;
      _dwStartTime=GetTickCount();
    }
  else
    { if(_Stage==L57STAGE_WAIT && _somethingchanged && (!noautostart_setup)) // �� ��������� ����� � �� ��������
        {
          Finished_out=1;
          Complete_out=1;
          //Error_out=0;
          //Time_out=0;
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
        rdsBlockMessageBox(NULL,"���������� ������ ����� - ����������� ����� ������",
                      L57_MSGCAPTION,MB_OK|MB_ICONERROR);
        return;
    }

  _Ready=1;
  _Start=1;
  _ShowTime();
#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------


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
// �������� ����� ������
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS

// ��������� ������ ������� ������� �� ������ ��������
int rdsbcppBlockClass::L57TableIndex(int iteration)
{ int _mod,_div,index;
  if(TableFillStep_current<=0)
    return -1;
  if(iteration==0)
    return 0;
  _div=iteration/TableFillStep_current;
  _mod=iteration%TableFillStep_current;
  if(_mod)
    index=_div+1;
  else
    index=_div;
  // ��� 5
  // iteration    _div    _mod    index
  //      0         -       -       0        ��������� ������
  //      1         0       1       1        _div+1
  //      2         0       2       1        _div+1
  //      3         0       3       1        _div+1
  //      4         0       4       1        _div+1
  //      5         1       0       1        _div
  //      6         1       1       2        _div+1
  //      7         1       2       2        _div+1
  // ....
  if(TableFillMode_setup==L57TABFILL_TAIL) // ��������� �����
    return index%TableCount_current;
  return index;
}
//---------------------------------------------------------------------------

// ���������� ������������� ������� ������ � �������� ��� �������� ��������
// ������� - ������ ������ � ��������
int rdsbcppBlockClass::L57PrepareTableRow(int iteration)
{ int index=L57TableIndex(iteration);

  if(TableFillStep_current<=0 || index<0)
    return -1;
  if(index>=MaxIterations_setup) // ������� ������ �� ������
    return -1;
  if(index<TableCount_current)
    return index;

  // ��������� ������ ������
  switch(TableFillMode_setup)
    { case L57TABFILL_REALLOC: // ����� ������������
        if(!L57ReallocAllTables())
          return -1;
        break;
      case L57TABFILL_PACK: // ����� ���������
        if(!L57PackAllTables())
          return -1;
        break;
      default:
        return -1;
    }
  // ��������� ����������
  index=L57TableIndex(iteration);
  if(index>=0 && TableFillStep_current>0 &&
     index<MaxIterations_setup && index<TableCount_current) // ������ ��� � �������
    return index;
  // �����-�� ������
  return -1;
}
//---------------------------------------------------------------------------

// ����������� �������
BOOL rdsbcppBlockClass::L57ReallocAllTables(void)
{ int currentsize=TableCount_current;
  if(!currentsize)
    currentsize=100;
  currentsize*=2;
  if(currentsize>MaxIterations_setup)
    currentsize=MaxIterations_setup;
  if(currentsize<=TableCount_current) // ����������
    return FALSE;

  // �������� ������������ �������
  if(L57ReallocAllTables_gen(currentsize))
    { TableCount_current=currentsize;
      return TRUE;
    }
  return FALSE;
}
//---------------------------------------------------------------------------

// ��������� ��� �������
BOOL rdsbcppBlockClass::L57PackAllTables(void)
{
  if(TableCount_current==0) // �����-�� ������
    return FALSE;
  if(!L57PackAllTables_gen())
    return FALSE;
  TableFillStep_current*=2;
  return TRUE;
}
//---------------------------------------------------------------------------

// ��������� ���� ������� (��������� �������� �������)
BOOL rdsbcppBlockClass::L57PackTable(rdsbcppVarAncestor *Matr)
{ RDS_ARRAYACCESSDATA AccData;
  int rowsize,index,total;
  double *base;
  if((!Matr->GetAccessData(&AccData)) || AccData.ItemSize!=sizeof(double)) // ������
    return FALSE;
  rowsize=AccData.Cols*sizeof(double);
  base=(double*)AccData.Data;
  total=AccData.Cols*AccData.Rows;

  index=1;
  for(int i=2;i<TableCount_current;i+=2,index++)
    memcpy(base+index*AccData.Cols,base+i*AccData.Cols,rowsize);
  // ������� � index - ��������� ��������� ������
  for(int i=index*AccData.Cols;i<total;i++)
    base[i]=rdsbcppHugeDouble;
  return TRUE;
}
//---------------------------------------------------------------------------

// �������� � ������� �����" � �������� � �����������, ���� ����
void rdsbcppBlockClass::L57FinalizeTable(rdsbcppVarAncestor *Matr,BOOL sort)
{ RDS_ARRAYACCESSDATA AccData;
  double *row;
  int errorindex=-1;

  if((!Matr->GetAccessData(&AccData)) || AccData.ItemSize!=sizeof(double)) // ������
    return;
  if(AccData.Cols==0 || AccData.Rows==0)
    return;
  row=(double*)AccData.Data;
  // ���� ������ �������� ������
  for(int i=0;i<AccData.Rows;i++,row+=AccData.Cols)
    if(*row==rdsbcppHugeDouble)
      { errorindex=i;
        break;
      }
  if(errorindex==0) // ��� ������� �� ������
    { rdsResizeVarArray(Matr->GetVoidPtr(),0,0,FALSE,NULL);
      return;
    }
  if(errorindex>0) // ���� ������
    { rdsResizeVarArray(Matr->GetVoidPtr(),errorindex,AccData.Cols,TRUE,NULL);
      if(!Matr->GetAccessData(&AccData)) // ������
        return;
    }
  if(sort)
    { // ���������� �� ������� �������
      L57_SortMatrixColAsc((double*)AccData.Data,AccData.Rows,AccData.Cols,0);
    }
}
//---------------------------------------------------------------------------


// ��������� "���� ������" � �������
void rdsbcppBlockClass::L57OutOfMemMsg(int n)
{ L57_String msg,nstr;
  rdsStopCalc();
  _Stage=L57STAGE_WAIT;
  L57ClearOutputData();
  nstr.AssignInt(n);
  msg.Assign("���� ������ ��� ������ (");
  msg.AddString(nstr);
  msg.AddString(")");
  rdsBlockMessageBox(NULL,msg.c_str(),L57_MSGCAPTION,MB_OK|MB_ICONERROR);
}
//---------------------------------------------------------------------------

// �������� ��� �������� ������ (��� ������)
void rdsbcppBlockClass::L57ClearOutputData(void)
{
  L57ClearOuts();
  L57ClearTabFunc_gen();
}
//---------------------------------------------------------------------------

// �������� ��������� �������� ������
void rdsbcppBlockClass::L57SetErrorOutputData(void)
{
  L57ClearOuts();
  //L57ClearTabFunc_gen();
  Steps_out=Iteration_out;

  _results_ok=1;
  _TabFuncReady=1;
  Complete_out=Finished_out=1;
  Error_out=1;
  _Ready=1;
}
//---------------------------------------------------------------------------

// �������� ������� �������� ������
void rdsbcppBlockClass::L57SetOutputData(void)
{
  L57DataToMainOuts();
  L57DataToAuxOuts();
  Steps_out=Iteration_out;

  #if L57_GENTABLESCOUNT!=0
    // �������� � ������ "�����", ����������� ���������� ������, � ����� ��������� ��
    // �� ����������� �������� (��� ������ � ������ ���������� ������)
    L57FinalizeTables_gen(TableFillMode_setup==L57TABFILL_TAIL);
  #endif

  _TabFuncReady=1;
  _results_ok=1;
  Complete_out=Finished_out=1;
  Error_out=0; //ok?0:1;
  _Ready=1;
}
//---------------------------------------------------------------------------


// ������ ������ (�����. ���������� �������������)
BOOL rdsbcppBlockClass::L57StartIterations(void)
{
  Complete_out=Error_out=Finished_out=0;
  _Stage=L57STAGE_CALC;
  L57InputsToCalc();
  _CyclesInDelta=0;

  L57ClearOutputData();
  Iteration_out=0;
  Steps_out=0;
  _TabFuncReady=0;
  TableCount_current=(TableFillMode_setup==L57TABFILL_REALLOC)?100:TableInitCount_setup;
  if(TableCount_current>MaxIterations_setup)
    TableCount_current=MaxIterations_setup;
  TableFillStep_current=TableFillStep_setup;
  if(L57ReallocAllTables_gen(TableCount_current))
    {
      #if L57_GENTABLESCOUNT!=0
        int rowindex=L57PrepareTableRow(0);
        if(rowindex<0) // �����-�� ������
          { TableFillStep_current=0;
            L57ClearTabFunc_gen();
          }
        else
          L57WriteIteraionRow_gen(0,rowindex);
      #endif
    }
  else // ������� �� �����������
    TableCount_current=TableFillStep_current=0;



  return TRUE;
}
//---------------------------------------------------------------------------

// �����
void rdsbcppBlockClass::Model_Calc(void)
{ int _iteration=Iteration_out;
  L57_LoopBreak _check;

  for(int _i=0;_i<tactcount_setup;_i++)
    { BOOL hugeval=FALSE;
      #if L57_MODELVERSION>=1
        BOOL _inside=L57CalcIteration(hugeval);
      #else
        BOOL _inside=L57CalcIteration();
      #endif
      _iteration++;

      #if L57_GENTABLESCOUNT!=0
        if( TableFillStep_current>0)
          { // ������ ������� �������� � �������
            int rowindex=L57PrepareTableRow(_iteration);
            if(rowindex>=0)
              L57WriteIteraionRow_gen(_iteration,rowindex);
          }
      #endif

      if(_inside)
        { _CyclesInDelta++;
          if(_CyclesInDelta>DeltaCycles_setup)
            { // ��� �������
              Iteration_out=_iteration;
              _Stage=L57STAGE_WAIT;
              L57SetOutputData();
              return;
            }
        }
      else
        _CyclesInDelta=0;

      if(_iteration>MaxIterations_setup || hugeval)
        { // ������� ����� �� �������� ��� ������ ����������
          Iteration_out=_iteration;
          _Stage=L57STAGE_WAIT;
          L57SetErrorOutputData();
          return;
        }

      if(_check.Check())
        break;
    }

  Iteration_out=_iteration;
  _Start=_Ready=1;
}
//---------------------------------------------------------------------------



// ������� ��������� ���������� �������� � ���� ���������
void Setup_FillFuncDescr(RDS_HOBJECT win)
{ char *modeltext,*templatetext;
  RDS_TOOLEXTCALLDATA data;
  LPSTR array[2];
  L57_String Inputs,Numbered,Functions,str;
  int count=0;

  templatetext=rdsGetObjectStr(win,L57SETUPFIELD_TEMPLATE,RDS_FORMVAL_VALUE);
  modeltext=rdsGetObjectStr(win,L57SETUPFIELD_MODEL,RDS_FORMVAL_VALUE);

  data.servSize=sizeof(data);
  data.Command=4;
  data.NInStrings=2;
  data.InStrings=array;
  array[0]=templatetext;
  array[1]=modeltext;
  data.NOutStrings=0;
  data.OutStrings=NULL;
  data.Result=0;
  rdsExecToolExt(RDS_ETF_S_BYDLL,"ProcessVarAndFunc","$DLL$\\L57.dll",&data);
  if(data.NOutStrings>=2 && data.OutStrings!=NULL && data.Result!=0)
    { Inputs.AssignRdsDynamicString(data.OutStrings[0]);
      data.OutStrings[0]=NULL;
      Numbered.AssignRdsDynamicString(data.OutStrings[1]);
      data.OutStrings[1]=NULL;
      for(int i=2;i<data.NOutStrings;i+=3)
        { Functions.AddString("[");
          Functions.AddInt(count);
          Functions.AddString("]:  ");
          Functions.AddString(data.OutStrings[i+1]);
          Functions.AddString(" = ");
          if(data.OutStrings[i]!=NULL && data.OutStrings[i][0]!=0)
            Functions.AddString(data.OutStrings[i]);
          else
            Functions.AddString("(��� ��������)");
          Functions.AddString("\n");
          count++;

        }
    }
  rdsClearExecToolExtResults(&data);

  // ������������ ������� ����������
  if(Numbered.IsNotEmpty())
    { str.Assign("���������� ����� �������: ");
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

  rdsSetObjectStr(win,L57SETUPFIELD_CODE,RDS_FORMVAL_VALUE,Functions.c_str());
  rdsSetObjectStr(win,L57SETUPFIELD_PARAMS,RDS_FORMVAL_VALUE,str.c_str());
  str.AssignInt(count);
  rdsSetObjectStr(win,L57SETUPFIELD_TITLE,RDS_FORMVAL_VALUE,str.c_str());
}
//---------------------------------------------------------------------------

// ������� ��������� ������ ���� ���������
void RDSCALL Setup_WinCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data)
{ char *modeltext,*templatetext,*lock=NULL,*auxdata;
  L57_String newmodeltext;
  int starttab=1,mode,res;
  RDS_BHANDLE show;

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
        // ��������� ���� - ���������� �������������� �� ��������
        lock=rdsGetObjectStr(win,L57SETUPFIELD_LOCKFUNC,RDS_FORMVAL_VALUE);
        res=L57_ExecTool_Edit(templatetext,modeltext,NULL/*options*/,starttab,&newmodeltext,
                              NULL,NULL,NULL,NULL,lock,NULL,auxdata
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
        Setup_FillFuncDescr(win);
        break;

      case RDS_FORMSERVEVENT_CHANGE: // ��������� ����
        mode=rdsFORMGetInt(win,L57SETUPFIELD_TABMODE);
        rdsSetObjectInt(win,L57SETUPFIELD_TABINITCOUNT,RDS_FORMVAL_ENABLED,mode!=0);
        #if L57_MODELVERSION>=11
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

// �������� ���������� � ������� ��������� ������ �������
void rdsbcppBlockClass::L57ResetFuncInputData(void)
{
  _FuncConnectedInputs.Clear();
  _FuncConnectedAll=FALSE;
  _FuncConnDataReady=FALSE;
}
//---------------------------------------------------------------------------


#endif // #ifdef L57MS_AFTERCLASS
//---------------------------------------------------------------------------

