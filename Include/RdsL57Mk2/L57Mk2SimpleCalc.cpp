//---------------------------------------------------------------------------
// Стандартные части модели простого вычисления функции (версия 2)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Глобальные описания
//---------------------------------------------------------------------------
#ifdef L57MS_GLOBALS
#ifndef L57_MODELVERSION
  #define L57_MODELVERSION 0
#endif

#ifdef L57_LOG
  #define _LOGBLK (rdsbcppBlockData->Block)
#endif

// Новейшая версия этой модели
#define L57_LATESTMODELVERSION 10

// Имя файла шаблона модели
#define L57_MODELTEMPLATE "$RDSINCLUDE$\\RdsL57Mk2\\IpuL57Mk2SimpleCalc.mdl"

// Заголовок сообщений этого блока
#define L57_MSGCAPTION  "Вычисление функции"

// Общие функции и классы
#include <RdsL57Mk2/L57Mk2Basic.cpp>

// Признак наличия файла справки
int L57_HelpState=0;
// Признак разрешения редактирования внешнего вида блока в настройках
BOOL L57_AllowTextRectEdit=FALSE;

#endif // #ifdef L57MS_GLOBALS
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Вызов контекстного меню
//---------------------------------------------------------------------------
#ifdef L57MS_POPUP

  #ifdef L57_HELPUIFILE
    if(L57_HelpUIFileExists(L57_HELPUIFILE,&L57_HelpState))
      rdsbcppBlockData->Flags=RDS_SETFLAG(rdsbcppBlockData->Flags,RDS_BLOCKHASHELP,TRUE);
  #endif

#endif // #ifdef L57MS_POPUP
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Прочие события
//---------------------------------------------------------------------------
#ifdef L57MS_OTHER

switch(CallMode)
  { case RDS_BFM_LEAVINGEDITMODE:
      L57_SendSingleFunctionIfNoInput(rdsbcppBlockData->Block,Func_in.Num(),Func_out.Num(),FALSE,
                                      _CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str());
      break;
    case RDS_BFM_BEFORESAVE:
    case RDS_BFM_AFTERLOAD:
      // Удаляем резервную копию файла модели
      L57_ResFile_DeleteModelCopy(rdsbcppBlockData->Block);
      break;
    case RDS_BFM_UNLOADSYSTEM:
      // Схема выгружается из памяти - если есть резервная копия модели, значит, сохранения не было
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
// Команда приема функции
//---------------------------------------------------------------------------
#ifdef L57MS_ACCEPTFUNC
  L57_AcceptFunc_Param_Queue *queue=L57_GetQueueFromParam(Param);

  if(queue)
    { if(queue->TransferId==_Transfer.LastTransferId)
        { // Уже были здесь - замыкание
          _AcceptRecursionError=TRUE;
          return;
        }
      else
        _Transfer.LastTransferId=queue->TransferId;
    }

  // Не находимся ли уже внутри реакции
  if(_InsideAcceptFunction)
    { _AcceptRecursionError=TRUE;
      return;
    }
  // Получаем
  _InsideAcceptFunction=TRUE;
  RDSBCPP_TRY
    { if(L57_GetSingleFunctionByConn(rdsbcppBlockData->Block,L57_MODELTEMPLATE,Param,
                                    _CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str(),
                                    &_CurFuncExportText))
        { // Передаем дальше
          char *text=_CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str();
          //rdsBlockMessageBox(rdsbcppBlockData->Block,text,"Передача",MB_OK);
          L57_SendSingleFunction(rdsbcppBlockData->Block,Func_out.Num(),Param->Immediate,text,queue);
          _Start=1;
        }
      _InsideAcceptFunction=FALSE;
    }
  RDSBCPP_CATCHALL
    { _InsideAcceptFunction=FALSE; }

#endif // #ifdef L57MS_ACCEPTFUNC


//---------------------------------------------------------------------------
// Команда приема параметра
//---------------------------------------------------------------------------
#ifdef L57MS_ACCEPTPARAM

  #if L57_MODELVERSION>=3
    // Не находимся ли уже внутри реакции
    if(_InsideAcceptParam)
      { _AcceptParamRecursionError=TRUE;
        return;
      }
    // Получаем
    _InsideAcceptParam=TRUE;
    RDSBCPP_TRY
      { 
        if(_Transfer.AcceptParam(Param,rdsbcppBlockData->Block)) // Что-то принято (есть такой параметр и он изменился)
          { _Start=1; // Перезапускаем
          }

        _InsideAcceptParam=FALSE;
      }
    RDSBCPP_CATCHALL
      { _InsideAcceptParam=FALSE; }
  #endif

#endif // #ifdef L57MS_ACCEPTPARAM


//---------------------------------------------------------------------------
// Команда проверки передачи параметра
//---------------------------------------------------------------------------
#ifdef L57MS_CHECKTRANSFUNC
  /*
  // Не находимся ли уже внутри реакции
  if(_InsideAcceptParam)
    { _AcceptParamRecursionError=TRUE;
      //rdsMessageBox("error","",MB_OK);
      return;
    }
  _InsideAcceptParam=TRUE;
  RDSBCPP_TRY
    { // У кого из входных параметров есть рабочая связь? (нужно для проверки)
      _Transfer.CheckInputConnections(rdsbcppBlockData->Block);
      // Проверяем
      _Transfer.CheckTransferParam(rdsbcppBlockData->Block,Param,Func_in.Num());
      _InsideAcceptParam=FALSE;
    }
  RDSBCPP_CATCHALL
    { _InsideAcceptParam=FALSE; }
  */
#endif // #ifdef L57MS_CHECKTRANSFUNC

//---------------------------------------------------------------------------
// Новая команда проверки передачи параметра
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
// Настройки
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
    { rdsMessageBox("Для настройки этого блока необходимо закрыть редактор его модели. "
                    "Пожалуйста, закройте редактор.",L57_MSGCAPTION,MB_OK|MB_ICONWARNING);
      rdscompOpenBlockModelEditor(rdsbcppBlockData->Block);
      return;
    }

  // Загрузка данных
  if(!L57_LoadModelFile(rdsbcppBlockData->Block,modeltext))
    rdsMessageBox("Ошибка загрузки текста модели блока",L57_MSGCAPTION,MB_OK|MB_ICONERROR);
  if(!L57_ReadFileToMemory(L57_MODELTEMPLATE,templatetext))
    rdsMessageBox("Ошибка загрузки шаблона модели блока (" L57_MODELTEMPLATE,L57_MSGCAPTION,MB_OK|MB_ICONERROR);

  #if L57_MODELVERSION>=3
    // Дополнительные данные о функциональном входе
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
      if(res>=0) // Нормальное закрытие
        break;
      // Временное закрытие
      if(show==NULL) // Нечего показывать
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
            rdsMessageBox("Ошибка записи текста модели блока",L57_MSGCAPTION,MB_OK|MB_ICONERROR);
        }
      if((rdsbcppBlockData->Flags & RDS_VARCHECKFAILED)==0) // С переменными все в порядке
        _Start=1;
    }

  Result=res?1:0;
#endif // #ifdef L57MS_SETUP
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// Описания в классе
//---------------------------------------------------------------------------
#ifdef L57MS_CLASS

// Экспорт текста функции для взаимодействия с другими блоками по связям
static char *_FuncExportText;

// Экспорт текста функции, полученный после последнего редактирования (может отличаться от
// _FuncExportText до перекомпиляции)
L57_String _CurFuncExportText;

// Флаги проверки рекурсии приема функции
BOOL _InsideAcceptFunction,_AcceptRecursionError,_TransferError;
// Флаги проверки рекурсии приема параметра
BOOL _InsideAcceptParam,_AcceptParamRecursionError;

// Вычислитель функции
L57_Calc _Calculator;

// Занесение стационарных переменных в вычислитель функции
void L57InputsToCalc(void);

  #if L57_MODELVERSION>=3
    void L57InputsChange(void);
  #endif

#endif // #ifdef L57MS_CLASS
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// Сохранение параметров
//---------------------------------------------------------------------------
#ifdef L57MS_SAVEPARAMS
  switch(rdsGetSystemInt(RDS_GSIUNDOACTION))
    { case RDS_UA_SETUPBLOCK: // Изменение настроек блока (может измениться модель)
      #ifdef RDS_UA_TRANSLATE
      case RDS_UA_TRANSLATE: // Переводится название функции (тоже в модели)
      #endif
        L57_SaveModelToUndo(rdsbcppBlockData->Block);
        break;
    }
#endif // #ifdef L57MS_SAVEPARAMS
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// Загрузка параметров
//---------------------------------------------------------------------------
#ifdef L57MS_LOADPARAMS

  if(rdsGetSystemInt(RDS_GSISAVELOADACTION)==RDS_LS_LOADAUTOCOMP)
    { // При подключении модели после автокомпиляции пробуем передать функцию по связям
      L57_SendSingleFunction(rdsbcppBlockData->Block,Func_out.Num(),TRUE, // Immediate=TRUE !
                             _CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str(),NULL);
    }
  L57_LoadModelFromUndo(rdsbcppBlockData->Block,LoadedText);
  if((rdsbcppBlockData->Flags & RDS_VARCHECKFAILED)==0) // С переменными все в порядке
    _Start=1;
  #ifdef L57_TITLES
    // Установка внешнего вида
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
// Режим моделирования
//---------------------------------------------------------------------------
#ifdef L57MS_CALCMODE
  _TransferError=FALSE;
  #if L57_MODELVERSION>=3
    // У кого из входных параметров есть рабочая связь?
    _Transfer.CheckInputConnections(rdsbcppBlockData->Block);
    // Кто получает функции из этого блока?
    _Transfer.ListFunctionDestinations(rdsbcppBlockData->Block,Func_out.Num());
  #endif
  if(_AcceptRecursionError)
    { _AcceptRecursionError=FALSE;
      _TransferError=TRUE;
      rdsBlockMessageBox(NULL,"Ошибка приема функции по связи: блоки замкнуты в кольцо",
                         L57_MSGCAPTION,MB_OK|MB_ICONWARNING);

    }

#endif // #ifdef L57MS_CALCMODE
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// Дополнительное рисование
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
// Всплывающая подсказка
//---------------------------------------------------------------------------
#ifdef L57MS_HINT
  if(_TransferError)
    { rdsSetHintText("Ошибка приема функции по связи: блоки замкнуты в кольцо");
      Show=1;
    }
#endif // #ifdef L57MS_HINT
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Запуск расчета
//---------------------------------------------------------------------------
#ifdef L57MS_START
  _Start=1;
#endif // #ifdef L57MS_START
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Конструктор класса блока
//---------------------------------------------------------------------------
#ifdef L57MS_INIT
  #if L57_MODELVERSION<L57_LATESTMODELVERSION
    // Сообщение о возможности обновления
    L57_ExecTool_UpdatePossible();
  #endif
  // Можно входить в настройки при неверной структуре переменных
  rdsbcppBlockData->Flags|=RDS_ALWAYSSETUP|RDS_ALWAYSLOADSAVE;
  // Сброс флагов проверки рекурсии
  _InsideAcceptFunction=_AcceptRecursionError=_InsideAcceptParam=_AcceptParamRecursionError=_TransferError=FALSE;
  #if L57_MODELVERSION>=3
    // Сброс параметров передачи
    L57_Reset_Trans_Data();
    // Инициализация проверочных переменных
    L57_Init_Old_Data();
  #endif
  #ifdef L57_TITLES
    L57_AllowTextRectEdit=L57_ExecTool_CanEditTextRect();
  #endif
#endif
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Модель
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
// Описания после класса
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS

#endif // #ifdef L57MS_AFTERCLASS
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// После создания переменных
//---------------------------------------------------------------------------
#ifdef L57MS_VARCHECK
  #if L57_MODELVERSION>=3
    L57_Create_Trans_Data();
  #endif
#endif // #ifdef L57MS_VARCHECK
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// Сброс расчета
//---------------------------------------------------------------------------
#ifdef L57MS_RESET
  #if L57_MODELVERSION>=3
    // Инициализация проверочных переменных
    L57_Init_Old_Data();
  #endif
#endif // #ifdef L57MS_RESET
//-----------------------------------------------------------------------

