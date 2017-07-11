//---------------------------------------------------------------------------
// Стандартные части модели блока поиска равновесия (версия 2)
//---------------------------------------------------------------------------

// Последний использованный номер ошибки "мало памяти": L57OutOfMemMsg(0);

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
#define L57_LATESTMODELVERSION 12

// Имя файла шаблона модели
#define L57_MODELTEMPLATE "$RDSINCLUDE$\\RdsL57Mk2\\IpuL57Mk2Stab.mdl"

// Заголовок сообщений этого блока
#define L57_MSGCAPTION  "Поиск равновесия"

// Общие функции и классы
#include <RdsL57Mk2/L57Mk2Basic.cpp>

// Состояния блока (_Stage)
#define L57STAGE_WAIT           0       // Ожидание
#define L57STAGE_CALC           1       // Расчет

// Способ заполнения выходных табличных функций
#define L57TABFILL_REALLOC      0       // При заполнении таблицы увеличить ее размер
#define L57TABFILL_PACK         1       // При заполнении таблицы выкинуть каждый второй отсчет
#define L57TABFILL_STOP         2       // При заполнении таблицы прекратить запись
#define L57TABFILL_TAIL         3       // При заполнении таблицы стирать начало, сохраняя число отсчетов
                                        // (запишется только "хвост")

// Идентификаторы полей окна настроек
#define L57SETUPFIELD_NOAUTOSTRT        1       // Начинать поиск только по команде
#define L57SETUPFIELD_TACTCOUNT         2       // Число итераций в такте
#define L57SETUPFIELD_DELTACYCLES       3       // Число итераций в трубке погрешности
#define L57SETUPFIELD_DELTA             4       // Погрешность
#define L57SETUPFIELD_MAXITER           5       // Максимальное число итераций
#define L57SETUPFIELD_TABMODE           6       // Режим заполнения табличных функций
#define L57SETUPFIELD_TABINITCOUNT      7       // Исходное число отсчетов в табличных функциях
#define L57SETUPFIELD_TABSTEP           8       // Исходный шаг записи в табличных функциях (может увеличиваться в два раза)
#define L57SETUPFIELD_TITLE             1000    // Информационное - заголовок функции
#define L57SETUPFIELD_CODE              1001    // Информационное - текст функции
#define L57SETUPFIELD_PARAMS            1002    // Информационное - список переменных
#define L57SETUPFIELD_EDITBTN           1003    // Кнопка вызова окна редактирования
#define L57SETUPFIELD_OUTTABBTN         1004    // Кнопка вызова окна редактирования с вкладкой выходных таблиц
#define L57SETUPFIELD_TEMPLATE          2000    // Невизуальное - шаблон модели
#define L57SETUPFIELD_MODEL             2001    // Невизуальное - текст модели
#define L57SETUPFIELD_LOCKFUNC          2002    // Невизуальное - признак блокировки редактирования по функциям
#define L57SETUPFIELD_ADDITIONALDATA    2003    // Невизуальное - дополнительные данные
#define L57SETUPFIELD_BLOCKTOSHOW       2004    // Невизуальное - блок, который нужно показать при временном закрытии окна

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

  #if L57_MODELVERSION>=9
    if(rdsSystemInEditMode())
      rdsAdditionalContextMenuItemEx("Очистить все функции",0,1,0);
  #endif

#endif // #ifdef L57MS_POPUP
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Выбор пункта контекстного меню
//---------------------------------------------------------------------------
#ifdef L57MS_MENUSELECT

  #if L57_MODELVERSION>=9
    if(rdsSystemInEditMode() &&
       rdsMessageBox("Очистить все функции внутри блока?",L57_MSGCAPTION,MB_ICONWARNING|MB_YESNO)==IDYES)
      { L57_ExecTool_ClearAll(rdsbcppBlockData->Block,L57_MODELTEMPLATE);
      }
  #endif

#endif // #ifdef L57MS_MENUSELECT
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Прочие события
//---------------------------------------------------------------------------
#ifdef L57MS_OTHER

switch(CallMode)
  { case RDS_BFM_LEAVINGEDITMODE:
      if(!_FuncConnDataReady)
        { // О входах ничего не известно - значит, никакая функция по входу точно еще не приходила
          // Выясняем, какие входы функций присоединены
          BOOL connected=L57_CheckFunctionSourceMulti(rdsbcppBlockData->Block,Func_in.Num(),
                                                      &_FuncConnectedInputs,&_FuncConnectedAll);
          _FuncConnDataReady=TRUE;
          if(!connected) // Ничего не присоединено
            L57_SendOutputFunctions(rdsbcppBlockData->Block,Func_out.Num(),FALSE,
                                    _FuncExportTexts,L57_FUNCCOUNT,NULL/*&_CurFuncExportTexts*/,NULL/*queue*/);
        }
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
// Команда приема функции
//---------------------------------------------------------------------------
#ifdef L57MS_ACCEPTFUNC
    L57_AcceptFunc_Param_Queue *queue=L57_GetQueueFromParam(Param);
    if(Param==NULL)
      return;

    if(!L57_CANACCESSFIELD(Param,AcceptIndex)) // Неверный размер струкутры
      return;

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
      { if(!Param->Immediate) // Синхронизация при выходе из редактирования, а не прямой вызов
          { // Выясняем, какие входы функций присоединены, если это еще не известно
            if(!_FuncConnDataReady)
              { L57_CheckFunctionSourceMulti(rdsbcppBlockData->Block,Func_in.Num(),
                                             &_FuncConnectedInputs,&_FuncConnectedAll);
                _FuncConnDataReady=TRUE;
                // Что-то точно присоединено, иначе бы не было этого вызова
              }
          }
        if(Param->TextIndex==-1 && Param->AcceptIndex==-1)
          { // Пришли все функции
            BOOL ok;
            // Запоминаем пришедшие
            _CurFuncExportTexts.Clear();
            _CurFuncExportTexts.CreateItem(Param->TextsCount-1);
            for(int i=0;i<Param->TextsCount;i++)
              _CurFuncExportTexts.ItemPtrFAST(i)->Assign(Param->Texts[i]);
            // Помечаем, что все пришло
            _FuncConnectedInputs.Clear();
            // Занесение всех функций в модель
            if(L57_MustImportFunctions(&_CurFuncExportTexts,_FuncExportTexts,L57_FUNCCOUNT))
              ok=L57_ExecTool_ImportFunctions(rdsbcppBlockData->Block,L57_MODELTEMPLATE,
                                             _FuncExportTexts,L57_FUNCCOUNT,&_CurFuncExportTexts);
            else
              ok=TRUE;
            if(ok)
              { // Передаем дальше
                L57_SendOutputFunctions(rdsbcppBlockData->Block,Func_out.Num(),Param->Immediate,
                                        _FuncExportTexts,L57_FUNCCOUNT,&_CurFuncExportTexts,queue);
              }
          }
        else if(Param->TextIndex!=-1 && Param->AcceptIndex!=-1)
          { // Если не оба -1, то оба должны быть номерами
            // Запоминаем пришедшую функцию
            _CurFuncExportTexts.CreateItem(Param->AcceptIndex);
            _CurFuncExportTexts.ItemPtrFAST(Param->AcceptIndex)->Assign(Param->Texts[Param->TextIndex]);
            if(Param->Immediate) // Немедленно встраиваем
              { // Прием функции Param->Texts[Param->TextIndex] в модель под номером Param->AcceptIndex
                if(L57_GetCertainFunctionByConn(rdsbcppBlockData->Block,L57_MODELTEMPLATE,Param->AcceptIndex,
                                                Param->TextIndex<L57_FUNCCOUNT?_FuncExportTexts[Param->TextIndex]:NULL,
                                                Param->Texts[Param->TextIndex]))
                  { // Передаем дальше все
                    L57_SendOutputFunctions(rdsbcppBlockData->Block,Func_out.Num(),Param->Immediate,
                                            _FuncExportTexts,L57_FUNCCOUNT,&_CurFuncExportTexts,queue);
                  }
              }
            else // Встраиваем только по готовности всех
              { // Пришла функция на вход Param->AcceptIndex - больше не ждем ее
                _FuncConnectedInputs.DeleteByValue(Param->AcceptIndex);
                if(_FuncConnectedInputs.Count==0) // Пришли все входы
                  { // Занесение всех функций в модель
                    BOOL ok;
                    if(L57_MustImportFunctions(&_CurFuncExportTexts,_FuncExportTexts,L57_FUNCCOUNT))
                      ok=L57_ExecTool_ImportFunctions(rdsbcppBlockData->Block,L57_MODELTEMPLATE,
                                                      _FuncExportTexts,L57_FUNCCOUNT,&_CurFuncExportTexts);
                    else // Все осталось, как было
                      ok=TRUE;
                    if(ok)
                      { // Передаем дальше все
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
// Настройки
//---------------------------------------------------------------------------
#ifdef L57MS_SETUP
  RDS_HOBJECT win;
  BOOL res;
  int tabid=0,fieldid;
  L57_String Func,Inputs,Numbered,Title,auxdata;
  L57_String modeltext,templatetext;
  char lockarray[L57_FUNCCOUNT+1];

  RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

  // Определение подключенных связей функций
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
    { rdsMessageBox("Для настройки этого блока необходимо закрыть редактор его модели. "
                    "Пожалуйста, закройте редактор.",L57_MSGCAPTION,MB_OK|MB_ICONWARNING);
      rdscompOpenBlockModelEditor(rdsbcppBlockData->Block);
      return;
    }

  win=rdsFORMCreate(TRUE,-1,-1,L57_MSGCAPTION);
  if(!win)
    return;

  #ifdef L57_HELPUIFILE
    rdsFORMHelpButton(win,RDS_FORMHELP_UI|RDS_FORMHELP_CHECK,L57_HELPUIFILE,L57_HELPTOPIC);
  #endif

  // Загрузка данных
  if(!L57_LoadModelFile(rdsbcppBlockData->Block,modeltext))
    rdsMessageBox("Ошибка загрузки текста модели блока",L57_MSGCAPTION,MB_OK|MB_ICONERROR);
  if(!L57_ReadFileToMemory(L57_MODELTEMPLATE,templatetext))
    rdsMessageBox("Ошибка загрузки шаблона модели блока (" L57_MODELTEMPLATE,L57_MSGCAPTION,MB_OK|MB_ICONERROR);

  // Вкладка редактирования функции и переменных
  rdsFORMAddTab(win,++tabid,"Функции");

  // Служебные поля: текст шаблона и самой модели
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TEMPLATE,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,templatetext.c_str());
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_MODEL,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,modeltext.c_str());

  // Служебное поле - блокировка редактирования по функциям
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_LOCKFUNC,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,lockarray);

  #if L57_MODELVERSION>=3
    // Служебное поле: дополнительные данные о функциональном входе
    L57_CreateAdditionalText(&auxdata,rdsbcppBlockData->Block,Func_in.Num());
    rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_ADDITIONALDATA,RDS_FORMCTRL_NONVISUAL,"",0);
    rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,auxdata.c_str());
  #endif

  rdsFORMAddEdit(win,tabid,L57SETUPFIELD_TITLE,RDS_FORMCTRL_DISPLAY,"Число функций в блоке:",50);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_CODE,RDS_FORMCTRL_MULTILINE,
      somethinglocked?"Функции (принимаются по связям):":"Функции (нажмите \"Изменить функции...\" для редактирования):",80);
  rdsFORMSetMultilineHeight(win,fieldid,6*24);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLREADONLY,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLRETURNS,1);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_PARAMS,RDS_FORMCTRL_MULTILINE,"",0);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLHEIGHT,80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLABELBEVEL,1);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_EDITBTN,RDS_FORMCTRL_BUTTON,"",80);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,
    somethinglocked?"Функции и переменные блока":"Изменить функции и переменные блока");

  // Вкладка параметров
  rdsFORMAddTab(win,++tabid,"Поиск");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_NOAUTOSTRT,RDS_FORMCTRL_CHECKBOX,
                 "Начинать поиск только по команде (сигнал Start_cmd)",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,noautostart_setup?1:0);


  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DELTA,RDS_FORMCTRL_EDIT,
                 "Допустимая погрешность изменения значений:",80);
  rdsSetObjectDouble(win,fieldid,RDS_FORMVAL_VALUE,Delta_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DELTACYCLES,RDS_FORMCTRL_UPDOWN,
                 "Итераций в трубке погрешности:",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNINC,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMIN,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMAX,1000);
  rdsFORMSetInt(win,fieldid,DeltaCycles_setup);


  // Вкладка выходных таблиц
  rdsFORMAddTab(win,++tabid,"Выходные таблицы");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_OUTTABBTN,RDS_FORMCTRL_BUTTON,"",80);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,"Изменить выходные табличные функции");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABMODE,RDS_FORMCTRL_COMBOLIST,"Режим записи:",400);
  rdsFORMSetComboList(win,fieldid,"Писать без ограничений\n"
                                  "При заполнении выбрасывать каждый второй отсчет\n"
                                  "При заполнении прекратить запись\n"
                                  "При заполнении стирать начало");
  rdsFORMSetInt(win,fieldid,TableFillMode_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABINITCOUNT,RDS_FORMCTRL_EDIT,
                 "Число отсчетов в таблицах:",80);
  rdsFORMSetInt(win,fieldid,TableInitCount_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABSTEP,RDS_FORMCTRL_UPDOWN,
                 "Шаг записи таблиц (число итераций в отсчете):",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNINC,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMIN,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMAX,1000);
  rdsFORMSetInt(win,fieldid,TableFillStep_setup);


  // Дополнительная вкладка
  rdsFORMAddTab(win,++tabid,"Дополнительно");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TACTCOUNT,RDS_FORMCTRL_EDIT,
                 "Число итераций в одном такте расчета:",80);
  rdsFORMSetInt(win,fieldid,tactcount_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_MAXITER,RDS_FORMCTRL_EDIT,
                 "Максимально допустимое число итераций:",80);
  rdsFORMSetInt(win,fieldid,MaxIterations_setup);

  #if L57_MODELVERSION>=11
    // Вкладка внешнего вида
    if(L57_AllowTextRectEdit)
      { tabid++;
        L57_TextRectEdit_Setup(rdsbcppBlockData->Block,win,TR_params,TR_text.c_str(),tabid);
      }
  #endif

  Setup_FillFuncDescr(win);

  // Открываем окно
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
            rdsMessageBox("Ошибка записи текста модели блока",L57_MSGCAPTION,MB_OK|MB_ICONERROR);
        }
       noautostart_setup=(rdsGetObjectInt(win,L57SETUPFIELD_NOAUTOSTRT,RDS_FORMVAL_VALUE)!=0);
       tactcount_setup=rdsGetObjectInt(win,L57SETUPFIELD_TACTCOUNT,RDS_FORMVAL_VALUE);

       TableFillMode_setup=rdsFORMGetInt(win,L57SETUPFIELD_TABMODE);
       TableInitCount_setup=rdsFORMGetInt(win,L57SETUPFIELD_TABINITCOUNT);
       TableFillStep_setup=rdsFORMGetInt(win,L57SETUPFIELD_TABSTEP);

       _InParamsChange=TRUE;
    }

  if(_InParamsChange && (rdsbcppBlockData->Flags & RDS_VARCHECKFAILED)==0) // С переменными все в порядке
    _Start=1;

  rdsDeleteObject(win);
  Result=res?1:0;
#endif // #ifdef L57MS_SETUP
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// Описания в классе
//---------------------------------------------------------------------------
#ifdef L57MS_CLASS

#if L57_FUNCCOUNT!=0
// Экспорт текстов функций для взаимодействия с другими блоками по связям
static char *_FuncExportTexts[L57_FUNCCOUNT];
#endif
// Номера отдельных входов функций, у которых есть источники
L57_IntArray _FuncConnectedInputs;
// Признак присоединения всего входного массива функций
BOOL _FuncConnectedAll;
// Признак того, что наличие входов функций определено
BOOL _FuncConnDataReady;
// Экспорт текста функции, полученный после последнего редактирования (может отличаться от
// _FuncExportTexts до перекомпиляции)
L57_StringArray _CurFuncExportTexts;

// Флаги проверки рекурсии приема функции
BOOL _InsideAcceptFunction,_AcceptRecursionError,_TransferError;
// Флаги проверки рекурсии приема параметра
BOOL _InsideAcceptParam,_AcceptParamRecursionError;

// Параметры поиска изменились
BOOL _InParamsChange;

int _CyclesInDelta; // Итераций в трубке погрешности

// Время
DWORD _dwStartTime;
// Вычислить время
void _ShowTime(void)
  { Time_out=((double)(GetTickCount()-_dwStartTime))/1000.0; };

int TableCount_current;         // Размер выходных табличных функций на данный момент
int TableFillStep_current;      // Шаг записи выходных табличных функций на данный момент

// Вычислить индекс отсчета таблицы по номеру итерации
int L57TableIndex(int iteration);
// Обеспечить существование индекса строки в таблицах для заданной итерации
// Возврат - индекс строки в таблицах
int L57PrepareTableRow(int iteration);
// Переотвести таблицы
BOOL L57ReallocAllTables(void);
// Упаковать все таблицы
BOOL L57PackAllTables(void);
// Упаковать одну таблицу (выбросить нечетные отсчеты)
BOOL L57PackTable(rdsbcppVarAncestor *Matr);
// Генерируемая функция - переотвести все таблицы
BOOL L57ReallocAllTables_gen(int newrowcount);
// Генерируемая функция - упаковать все таблицы
BOOL L57PackAllTables_gen(void);
// Генерируемая функция - очистить все таблицы
void L57ClearTabFunc_gen(void);
#if L57_GENTABLESCOUNT!=0
// Генерируемая функция - записать строку итерации во все таблицы
void L57WriteIteraionRow_gen(int iteration,int rowindex);
// Генерируемая функция - отрезать у всех таблиц "хвост" с ошибками и сортировать, если надо
void L57FinalizeTables_gen(BOOL sort);
#endif
// Отрезать у таблицы хвост" с ошибками и сортировать, если надо
void L57FinalizeTable(rdsbcppVarAncestor *Matr,BOOL sort);
// Сообщение "мало памяти" с номером
void L57OutOfMemMsg(int n);
// Проверка изменения переменных
BOOL L57InputsChange(BOOL _force,BOOL *pChgSignalOn);
// Начало поиска (возвр. успешность инициализации)
BOOL L57StartIterations(void);
// Поиск
void Model_Calc(void);
// Очистить все выходные данные (при ошибке)
void L57ClearOutputData(void);
// Записать ошибочные выходные данные
void L57SetErrorOutputData(void);
// Записать готовые выходные данные
void L57SetOutputData(void);
// Занесение стационарных переменных и начальных значений в вычислители функции
void L57InputsToCalc(void);
// Итерация (возвращает TRUE, если максимальное изменение с прошлой итерации не больше заданного в настройках)
#if L57_MODELVERSION>=1
  BOOL L57CalcIteration(BOOL &HugeValError);
#else
  BOOL L57CalcIteration(void);
#endif
// Копирование текущих значений вычислителей в основные выходы
void L57DataToMainOuts(void);
// Копирование текущих значений вычислителей во вспомогательные выходы
void L57DataToAuxOuts(void);
// Очистка всех выходов
void L57ClearOuts(void);
// Сбросить информацию о наличии отдельных входов функций
void L57ResetFuncInputData(void);

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
    L57_SendOutputFunctions(rdsbcppBlockData->Block,Func_out.Num(),FALSE,
                            _FuncExportTexts,L57_FUNCCOUNT,NULL/*&_CurFuncExportTexts*/,NULL/*queue*/);
  }
L57_LoadModelFromUndo(rdsbcppBlockData->Block,LoadedText);
if(_InParamsChange && (rdsbcppBlockData->Flags & RDS_VARCHECKFAILED)==0) // С переменными все в порядке
  _Start=1;

  #if defined(L57_TITLES) && L57_MODELVERSION>=11
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
  L57ResetFuncInputData();
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
    // Инициализация фиксированных параметров
    L57_Init_Fixed_Data();
    // Инициализация проверочных переменных
    L57_Init_Old_Data();
  #endif
#endif // #ifdef L57MS_RESET
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// Режим редактирования
//---------------------------------------------------------------------------
#ifdef L57MS_EDITMODE
  L57ResetFuncInputData();
#endif // #ifdef L57MS_EDITMODE
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// Запуск расчета
//---------------------------------------------------------------------------
#ifdef L57MS_START
  if(rdsCalcProcessNeverStarted())
    _InParamsChange=TRUE;
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
  // Параметры поиска изменились (для первого поиска)
  _InParamsChange=TRUE;
  // Сброс флагов проверки рекурсии
  _InsideAcceptFunction=_AcceptRecursionError=_InsideAcceptParam=_AcceptParamRecursionError=_TransferError=FALSE;
  #if L57_MODELVERSION>=3
    // Инициализация фиксированных параметров
    L57_Init_Fixed_Data();
    // Сброс параметров передачи
    L57_Reset_Trans_Data();
    // Инициализация проверочных переменных
    L57_Init_Old_Data();
  #endif
  #if defined(L57_TITLES) && L57_MODELVERSION>=11
    L57_AllowTextRectEdit=L57_ExecTool_CanEditTextRect();
  #endif
#endif
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Модель
//---------------------------------------------------------------------------
#ifdef L57MS_MODEL
  BOOL enabled=TRUE,dostart=FALSE,_somethingchanged=FALSE;

  // Исходно запрещаем работу всех выходов
  _Ready=0;

  // Проверка разрешения работы
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

  // Работа разрешена - изменились ли входные параметры
  if(L57InputsChange(FALSE,&_somethingchanged))
    _InParamsChange=TRUE;

  if(Restart_cmd) // Принудительный перезапуск
    dostart=TRUE;
  else // Принудительного перезапуска нет
    { if(Start_cmd)
        _somethingchanged=TRUE;
      if(noautostart_setup) // Не запускать автоматически при изменениях входов
        { dostart=(Start_cmd!=0) && _InParamsChange;
          Start_cmd=0;
        }
      else // Запускать автоматически при изменениях входов
        dostart=_InParamsChange;
    }

  if(dostart)
    { // Запуск поиска
      _InParamsChange=FALSE;
      Restart_cmd=0;
      Start_cmd=0;
      if(!L57StartIterations())
        return;
      Started_out=1;
      _dwStartTime=GetTickCount();
    }
  else
    { if(_Stage==L57STAGE_WAIT && _somethingchanged && (!noautostart_setup)) // Не запускаем поиск и не работаем
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
        rdsBlockMessageBox(NULL,"Внутренняя ошибка блока - неизвестный режим работы",
                      L57_MSGCAPTION,MB_OK|MB_ICONERROR);
        return;
    }

  _Ready=1;
  _Start=1;
  _ShowTime();
#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------


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
      { if(_Transfer.AcceptParam(Param,rdsbcppBlockData->Block)) // Что-то принято (есть такой параметр и он изменился)
          { Restart_cmd=_Start=1; // Перезапускаем
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
// Описания после класса
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS

// Вычислить индекс отсчета таблицы по номеру итерации
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
  // Шаг 5
  // iteration    _div    _mod    index
  //      0         -       -       0        отдельный случай
  //      1         0       1       1        _div+1
  //      2         0       2       1        _div+1
  //      3         0       3       1        _div+1
  //      4         0       4       1        _div+1
  //      5         1       0       1        _div
  //      6         1       1       2        _div+1
  //      7         1       2       2        _div+1
  // ....
  if(TableFillMode_setup==L57TABFILL_TAIL) // Кольцевой буфер
    return index%TableCount_current;
  return index;
}
//---------------------------------------------------------------------------

// Обеспечить существование индекса строки в таблицах для заданной итерации
// Возврат - индекс строки в таблицах
int rdsbcppBlockClass::L57PrepareTableRow(int iteration)
{ int index=L57TableIndex(iteration);

  if(TableFillStep_current<=0 || index<0)
    return -1;
  if(index>=MaxIterations_setup) // Столько вообще не бывает
    return -1;
  if(index<TableCount_current)
    return index;

  // Превысили размер таблиц
  switch(TableFillMode_setup)
    { case L57TABFILL_REALLOC: // Будем переотводить
        if(!L57ReallocAllTables())
          return -1;
        break;
      case L57TABFILL_PACK: // Нужно упаковать
        if(!L57PackAllTables())
          return -1;
        break;
      default:
        return -1;
    }
  // Параметры изменились
  index=L57TableIndex(iteration);
  if(index>=0 && TableFillStep_current>0 &&
     index<MaxIterations_setup && index<TableCount_current) // Теперь все в порядке
    return index;
  // Какая-то ошибка
  return -1;
}
//---------------------------------------------------------------------------

// Переотвести таблицы
BOOL rdsbcppBlockClass::L57ReallocAllTables(void)
{ int currentsize=TableCount_current;
  if(!currentsize)
    currentsize=100;
  currentsize*=2;
  if(currentsize>MaxIterations_setup)
    currentsize=MaxIterations_setup;
  if(currentsize<=TableCount_current) // Невозможно
    return FALSE;

  // Вызываем генерируемую функцию
  if(L57ReallocAllTables_gen(currentsize))
    { TableCount_current=currentsize;
      return TRUE;
    }
  return FALSE;
}
//---------------------------------------------------------------------------

// Упаковать все таблицы
BOOL rdsbcppBlockClass::L57PackAllTables(void)
{
  if(TableCount_current==0) // Какая-то ошибка
    return FALSE;
  if(!L57PackAllTables_gen())
    return FALSE;
  TableFillStep_current*=2;
  return TRUE;
}
//---------------------------------------------------------------------------

// Упаковать одну таблицу (выбросить нечетные отсчеты)
BOOL rdsbcppBlockClass::L57PackTable(rdsbcppVarAncestor *Matr)
{ RDS_ARRAYACCESSDATA AccData;
  int rowsize,index,total;
  double *base;
  if((!Matr->GetAccessData(&AccData)) || AccData.ItemSize!=sizeof(double)) // Ошибка
    return FALSE;
  rowsize=AccData.Cols*sizeof(double);
  base=(double*)AccData.Data;
  total=AccData.Cols*AccData.Rows;

  index=1;
  for(int i=2;i<TableCount_current;i+=2,index++)
    memcpy(base+index*AccData.Cols,base+i*AccData.Cols,rowsize);
  // Начиная с index - заполняем значением ошибки
  for(int i=index*AccData.Cols;i<total;i++)
    base[i]=rdsbcppHugeDouble;
  return TRUE;
}
//---------------------------------------------------------------------------

// Отрезать у таблицы хвост" с ошибками и сортировать, если надо
void rdsbcppBlockClass::L57FinalizeTable(rdsbcppVarAncestor *Matr,BOOL sort)
{ RDS_ARRAYACCESSDATA AccData;
  double *row;
  int errorindex=-1;

  if((!Matr->GetAccessData(&AccData)) || AccData.ItemSize!=sizeof(double)) // Ошибка
    return;
  if(AccData.Cols==0 || AccData.Rows==0)
    return;
  row=(double*)AccData.Data;
  // Ищем первое значение ошибки
  for(int i=0;i<AccData.Rows;i++,row+=AccData.Cols)
    if(*row==rdsbcppHugeDouble)
      { errorindex=i;
        break;
      }
  if(errorindex==0) // Вся матрица из ошибок
    { rdsResizeVarArray(Matr->GetVoidPtr(),0,0,FALSE,NULL);
      return;
    }
  if(errorindex>0) // Есть ошибки
    { rdsResizeVarArray(Matr->GetVoidPtr(),errorindex,AccData.Cols,TRUE,NULL);
      if(!Matr->GetAccessData(&AccData)) // Ошибка
        return;
    }
  if(sort)
    { // Сортировка по нулевой колонке
      L57_SortMatrixColAsc((double*)AccData.Data,AccData.Rows,AccData.Cols,0);
    }
}
//---------------------------------------------------------------------------


// Сообщение "мало памяти" с номером
void rdsbcppBlockClass::L57OutOfMemMsg(int n)
{ L57_String msg,nstr;
  rdsStopCalc();
  _Stage=L57STAGE_WAIT;
  L57ClearOutputData();
  nstr.AssignInt(n);
  msg.Assign("Мало памяти для работы (");
  msg.AddString(nstr);
  msg.AddString(")");
  rdsBlockMessageBox(NULL,msg.c_str(),L57_MSGCAPTION,MB_OK|MB_ICONERROR);
}
//---------------------------------------------------------------------------

// Очистить все выходные данные (при ошибке)
void rdsbcppBlockClass::L57ClearOutputData(void)
{
  L57ClearOuts();
  L57ClearTabFunc_gen();
}
//---------------------------------------------------------------------------

// Записать ошибочные выходные данные
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

// Записать готовые выходные данные
void rdsbcppBlockClass::L57SetOutputData(void)
{
  L57DataToMainOuts();
  L57DataToAuxOuts();
  Steps_out=Iteration_out;

  #if L57_GENTABLESCOUNT!=0
    // Отрезаем у таблиц "хвост", заполненный значениями ошибки, а затем сортируем их
    // по возрастанию итерации (при записи в режиме кольцевого буфера)
    L57FinalizeTables_gen(TableFillMode_setup==L57TABFILL_TAIL);
  #endif

  _TabFuncReady=1;
  _results_ok=1;
  Complete_out=Finished_out=1;
  Error_out=0; //ok?0:1;
  _Ready=1;
}
//---------------------------------------------------------------------------


// Начало поиска (возвр. успешность инициализации)
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
        if(rowindex<0) // Какая-то ошибка
          { TableFillStep_current=0;
            L57ClearTabFunc_gen();
          }
        else
          L57WriteIteraionRow_gen(0,rowindex);
      #endif
    }
  else // Таблицы не формируются
    TableCount_current=TableFillStep_current=0;



  return TRUE;
}
//---------------------------------------------------------------------------

// Поиск
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
          { // Запись текущей итерации в таблицы
            int rowindex=L57PrepareTableRow(_iteration);
            if(rowindex>=0)
              L57WriteIteraionRow_gen(_iteration,rowindex);
          }
      #endif

      if(_inside)
        { _CyclesInDelta++;
          if(_CyclesInDelta>DeltaCycles_setup)
            { // Все сошлось
              Iteration_out=_iteration;
              _Stage=L57STAGE_WAIT;
              L57SetOutputData();
              return;
            }
        }
      else
        _CyclesInDelta=0;

      if(_iteration>MaxIterations_setup || hugeval)
        { // Слишком долго не сходится или ошибка вычислений
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



// Функция занесения параметров описания в окно настройки
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
            Functions.AddString("(без названия)");
          Functions.AddString("\n");
          count++;

        }
    }
  rdsClearExecToolExtResults(&data);

  // Формирование списков параметров
  if(Numbered.IsNotEmpty())
    { str.Assign("Переменные связи функций: ");
      str.AddString(Numbered);
      str.AddString(".");
    }
  if(Inputs.IsNotEmpty())
    { if(str.IsNotEmpty())
        str.AddString("\n");
      str.AddString("Входы блока (стационарные параметры): ");
      str.AddString(Inputs);
      str.AddString(".");
    }

  rdsSetObjectStr(win,L57SETUPFIELD_CODE,RDS_FORMVAL_VALUE,Functions.c_str());
  rdsSetObjectStr(win,L57SETUPFIELD_PARAMS,RDS_FORMVAL_VALUE,str.c_str());
  str.AssignInt(count);
  rdsSetObjectStr(win,L57SETUPFIELD_TITLE,RDS_FORMVAL_VALUE,str.c_str());
}
//---------------------------------------------------------------------------

// Функция обратного вызова окна настройки
void RDSCALL Setup_WinCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data)
{ char *modeltext,*templatetext,*lock=NULL,*auxdata;
  L57_String newmodeltext;
  int starttab=1,mode,res;
  RDS_BHANDLE show;

  #if L57_MODELVERSION>=3
    // Служебное поле: дополнительные данные о функциональном входе
    auxdata=rdsGetObjectStr(win,L57SETUPFIELD_ADDITIONALDATA,RDS_FORMVAL_VALUE);
  #else
    auxdata=NULL;
  #endif

  switch(data->Event)
    {
      #if L57_MODELVERSION>=3
      case RDS_FORMSERVEVENT_TCREOPEN:   // Открытие после временного закрытия
        templatetext=NULL;
      #endif
      case RDS_FORMSERVEVENT_CLICK:
        switch(data->CtrlId)
          { case L57SETUPFIELD_OUTTABBTN: // Вызов редактирования табличных функций
              starttab=2;
              break;
          }
        if(data->Event==RDS_FORMSERVEVENT_CLICK)
          templatetext=rdsGetObjectStr(win,L57SETUPFIELD_TEMPLATE,RDS_FORMVAL_VALUE);
        modeltext=rdsGetObjectStr(win,L57SETUPFIELD_MODEL,RDS_FORMVAL_VALUE);
        // Служебное поле - блокировка редактирования по функциям
        lock=rdsGetObjectStr(win,L57SETUPFIELD_LOCKFUNC,RDS_FORMVAL_VALUE);
        res=L57_ExecTool_Edit(templatetext,modeltext,NULL/*options*/,starttab,&newmodeltext,
                              NULL,NULL,NULL,NULL,lock,NULL,auxdata
                              #if L57_MODELVERSION>=3
                              ,&show
                              #endif
                              );
        if(res<0)
          { // Показ блока
            char *fullname;
            if(show==NULL) // Нечего показывать
              break;
            // Временно закрываем окно для показа
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
        if(strcmp(newmodeltext.c_str(),modeltext)==0) // Нет изменений
          break;
        rdsSetObjectStr(win,L57SETUPFIELD_MODEL,RDS_FORMVAL_VALUE,newmodeltext.c_str());
        Setup_FillFuncDescr(win);
        break;

      case RDS_FORMSERVEVENT_CHANGE: // Изменение поля
        mode=rdsFORMGetInt(win,L57SETUPFIELD_TABMODE);
        rdsSetObjectInt(win,L57SETUPFIELD_TABINITCOUNT,RDS_FORMVAL_ENABLED,mode!=0);
        #if L57_MODELVERSION>=11
          if(L57_AllowTextRectEdit)
            L57_TextRectEdit_EnableControls(win);
        #endif
        break;

      #if L57_MODELVERSION>=3
      case RDS_FORMSERVEVENT_TCREACTION: // Реакция после временного закрытия
        show=rdsBlockByFullName(rdsGetObjectStr(win,L57SETUPFIELD_BLOCKTOSHOW,RDS_FORMVAL_VALUE),NULL);
        L57_SelectAndShowBlock(show);
        break;
      #endif
    }
}
//---------------------------------------------------------------------------

// Сбросить информацию о наличии отдельных входов функций
void rdsbcppBlockClass::L57ResetFuncInputData(void)
{
  _FuncConnectedInputs.Clear();
  _FuncConnectedAll=FALSE;
  _FuncConnDataReady=FALSE;
}
//---------------------------------------------------------------------------


#endif // #ifdef L57MS_AFTERCLASS
//---------------------------------------------------------------------------

