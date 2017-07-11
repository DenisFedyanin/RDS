//---------------------------------------------------------------------------
// Стандартные части модели построения таблиц по функциям (версия 2)
//---------------------------------------------------------------------------

// Последний использованный номер ошибки "мало памяти": L57OutOfMemMsg(0);

//---------------------------------------------------------------------------
// Глобальные описания
//---------------------------------------------------------------------------
#ifdef L57MS_GLOBALS
#ifndef L57_MODELVERSION
  #define L57_MODELVERSION 0
#endif

// Новейшая версия этой модели
#define L57_LATESTMODELVERSION 10

#ifdef L57_LOG
  #define _LOGBLK (rdsbcppBlockData->Block)
#endif

// Имя файла шаблона модели
#define L57_MODELTEMPLATE "$RDSINCLUDE$\\RdsL57Mk2\\IpuL57Mk2TableCalc.mdl"

// Заголовок сообщений этого блока
#define L57_MSGCAPTION  "Формирование таблиц"

// Общие функции и классы
#include <RdsL57Mk2/L57Mk2Basic.cpp>

// Состояния блока (_Stage)
#define L57STAGE_WAIT           0       // Ожидание
#define L57STAGE_CALC           1       // Расчет

// Идентификаторы полей окна настроек
#define L57SETUPFIELD_NOAUTOSTRT        1       // Начинать поиск только по команде
#define L57SETUPFIELD_TACTCOUNT         2       // Число итераций в такте
#define L57SETUPFIELD_TITLE             1000    // Информационное - заголовок функции
#define L57SETUPFIELD_CODE              1001    // Информационное - текст функции
#define L57SETUPFIELD_PARAMS            1002    // Информационное - список переменных
#define L57SETUPFIELD_EDITBTN           1003    // Кнопка вызова окна редактирования
#define L57SETUPFIELD_TEMPLATE          2000    // Невизуальное - шаблон модели
#define L57SETUPFIELD_MODEL             2001    // Невизуальное - текст модели
#define L57SETUPFIELD_LOCKEDIT          2002    // Невизуальное - признак блокировки редактирования
#define L57SETUPFIELD_ADDITIONALDATA    2003    // Невизуальное - дополнительные данные
#define L57SETUPFIELD_BLOCKTOSHOW       2004    // Невизуальное - блок, который нужно показать при временном закрытии окна

// Признак наличия файла справки
int L57_HelpState=0;
// Признак разрешения редактирования внешнего вида блока в настройках
BOOL L57_AllowTextRectEdit=FALSE;

//-----------------------------------------------------------------------
// Данные перебираемого параметра
//-----------------------------------------------------------------------
class L57_Parameter
{ private:
    // Отвести массив аргументов
    BOOL AllocateArray(int count);

  public:
    // Исходные данные
    double Min,Max;     // Исходный диапазон перебора
    double Step;        // Шаг

    // Сгенерированный массив аргументов и текущие данные
    double _Min,_Max,_Delta;    // Скорректированные значения диапазона и шага
    L57_DblArray Array;         // Массив отсчетов аргумента
    int _ArgIndex;              // Номер текущего значения в процессе перебора в массиве Array

    double Current;             // Текущее значение для передачи в функцию вычисления

    // Очистить массив аргументов
    void ClearArrays(void);
    // Отвести массив аргументов по исходным данным
    BOOL AllocateArray(void);

    L57_Parameter(void)
      { Min=Max=Step=rdsbcppHugeDouble;
      };
    ~L57_Parameter(){ClearArrays();};
};
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Данные перебираемого параметра
//-----------------------------------------------------------------------
// Очистить массив аргументов
void L57_Parameter::ClearArrays(void)
{
  Array.Clear();
  Array.PrevCount=0;
}
//---------------------------------------------------------------------------

// Отвести массив аргументов
BOOL L57_Parameter::AllocateArray(int count)
{ double d_size=((double)count)*sizeof(double);
  ClearArrays();
  if(count<=0)
    return TRUE;
  if(d_size-1>((double)(count*sizeof(double)))) // Ошибка - слишком большой массив
    return FALSE;

  return Array.Allocate(Array.PrevCount=count);
}
//---------------------------------------------------------------------------

// Отвести массив аргументов по исходным данным
BOOL L57_Parameter::AllocateArray(void)
{ int n=L57_CalcNPointsByDiap(Min,Max,Step);
  ClearArrays();

  if(n<1)
    { // Вводим фиктивную точку
      _Min=_Max=0.0;
      _Delta=1.0;
      if(!AllocateArray(1)) // Другая функция - с параметром int
        return FALSE;
      Array.Data[0]=0.0;
      return TRUE;
    }

  if(!AllocateArray(n)) // Другая функция - с параметром int
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
// Набор перебираемых параметров
//-----------------------------------------------------------------------
// Должен быть уже установлен макрос L57_PARAMSCOUNT!
class L57_Parameters
{ public:
    #if L57_PARAMSCOUNT!=0
      L57_Parameter P[L57_PARAMSCOUNT];
    #endif

    // Следующий шаг в наборе аргументов
    BOOL NextStep(void);

    L57_Parameters(void){};
    ~L57_Parameters(){ };
};
//-----------------------------------------------------------------------

// Следующий шаг в наборе аргументов
BOOL L57_Parameters::NextStep(void)
{ BOOL changed=FALSE;

#if L57_PARAMSCOUNT!=0
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->_ArgIndex++;
      if(p->_ArgIndex<p->Array.Count) // Есть еще значение в текущем куске
        { changed=TRUE;
          break;
        }
      // Значения нет
      p->_ArgIndex=0;
    }
#endif
  return (!changed);
}
//---------------------------------------------------------------------------


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
// После создания переменных
//---------------------------------------------------------------------------
#ifdef L57MS_VARCHECK
  #if L57_MODELVERSION>=3
    L57_Create_Trans_Data();
  #endif
#endif // #ifdef L57MS_VARCHECK
//-----------------------------------------------------------------------

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
        }
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
  L57_String Func,Inputs,Numbered,Title;
  L57_String modeltext,templatetext,auxdata;

  RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

  BOOL FuncByConn=L57_CheckFunctionSource(rdsbcppBlockData->Block,Func_in.Num());


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

  // Чтение описания нулевой функции
  L57_ExecTool_GetDynamicDescription(templatetext,modeltext,&Func,&Inputs,&Numbered,&Title);

  // Вкладка редактирования функции и переменных
  rdsFORMAddTab(win,++tabid,"Функция");

  // Служебные поля: текст шаблона и самой модели
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TEMPLATE,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,templatetext.c_str());
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_MODEL,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,modeltext.c_str());

  // Служебное поле - блокировка редактирования
  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_LOCKEDIT,RDS_FORMCTRL_NONVISUAL,"",0);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,FuncByConn?"0":"");

  #if L57_MODELVERSION>=3
    // Служебное поле: дополнительные данные о функциональном входе
    L57_CreateAdditionalText(&auxdata,rdsbcppBlockData->Block,Func_in.Num());
    rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_ADDITIONALDATA,RDS_FORMCTRL_NONVISUAL,"",0);
    rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,auxdata.c_str());
  #endif

  rdsFORMAddEdit(win,tabid,L57SETUPFIELD_TITLE,RDS_FORMCTRL_DISPLAY,"Название:",500);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_CODE,RDS_FORMCTRL_MULTILINE,
      FuncByConn?"Функция (принимается по связи):":"Функция (нажмите \"Изменить функцию...\" для редактирования):",80);
  rdsFORMSetMultilineHeight(win,fieldid,6*24);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLREADONLY,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLRETURNS,1);
  //rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLBEVEL,1);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_PARAMS,RDS_FORMCTRL_MULTILINE,"",0);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLHEIGHT,80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_MLABELBEVEL,1);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_EDITBTN,RDS_FORMCTRL_BUTTON,"",80);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,
    FuncByConn?"Изменить переменные блока":"Изменить функцию и переменные блока");

  // Вкладка параметров
  rdsFORMAddTab(win,++tabid,"Вычисления");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_NOAUTOSTRT,RDS_FORMCTRL_CHECKBOX,
                 "Начинать вычисления только по команде (сигнал Start_cmd)",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,noautostart_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TACTCOUNT,RDS_FORMCTRL_EDIT,
                 "Число итераций в одном такте расчета:",80);
  rdsFORMSetInt(win,fieldid,tactcount_setup);

  #if L57_MODELVERSION>=9
    // Вкладка внешнего вида
    if(L57_AllowTextRectEdit)
      { tabid++;
        L57_TextRectEdit_Setup(rdsbcppBlockData->Block,win,TR_params,TR_text.c_str(),tabid);
      }
  #endif

  Setup_FillFuncDescr(win,Func,Inputs,Numbered,Title);

  // Открываем окно
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
            rdsMessageBox("Ошибка записи текста модели блока",L57_MSGCAPTION,MB_OK|MB_ICONERROR);
        }
       noautostart_setup=(rdsGetObjectInt(win,L57SETUPFIELD_NOAUTOSTRT,RDS_FORMVAL_VALUE)!=0);
       tactcount_setup=rdsGetObjectInt(win,L57SETUPFIELD_TACTCOUNT,RDS_FORMVAL_VALUE);
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

// Экспорт текста функции для взаимодействия с другими блоками по связям
static char *_FuncExportText;

// Экспорт текста функции, полученный после последнего редактирования (может отличаться от
// _FuncExportText до перекомпиляции)
L57_String _CurFuncExportText;

// Флаги проверки рекурсии приема функции
BOOL _InsideAcceptFunction,_AcceptRecursionError,_TransferError;
// Флаги проверки рекурсии приема параметра
BOOL _InsideAcceptParam,_AcceptParamRecursionError;

// Параметры расчета изменились
BOOL _InParamsChange;

// Прогресс
double _Progress_Cur,_Progress_Max;   // На стадии первичного расчета выходных таблиц

// Вычислитель функции
L57_Calc _Calculator;

// Набор перебираемых параметров
L57_Parameters _Params;

// Время
DWORD _dwStartTime;
// Вычислить время
void _ShowTime(void)
  { Time_out=((double)(GetTickCount()-_dwStartTime))/1000.0; };

// Сообщение "мало памяти" с номером
void L57OutOfMemMsg(int n);
// Очистить все выходные данные (при ошибке)
void L57ClearOutputData(void);
// Проверка изменения переменных
// *pChgSignalOn взводится при наличии сигнала изменения (независимо от фактического изменения)
BOOL L57InputsChange(BOOL _force,BOOL *pChgSignalOn);
// Начало вычисления (возвр. успешность инициализации)
BOOL L57StartIterations(void);
// Записать ошибочные выходные данные
void L57SetErrorOutputData(void);
// Вывод прогресса
void Model_ShowProgress(void);
// Занесение стационарных переменных в вычислитель функции
void L57InputsToCalc(void);
// Записать готовые выходные данные
void L57SetOutputData(void);
// Инициализация набора перебираемых параметров
void L57InitParams(void);
// Занесение текущих значений параметров в вычислитель функции
void L57ParamsToCalc(void);
// Вычисление
void Model_Calc(void);

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
  if(_InParamsChange && (rdsbcppBlockData->Flags & RDS_VARCHECKFAILED)==0) // С переменными все в порядке
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
  // Параметры расчета изменились (для первого расчета)
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
  #ifdef L57_TITLES
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
    { // Запуск расчета
      _InParamsChange=FALSE;
      Restart_cmd=0;
      Start_cmd=0;
      if(!L57StartIterations())
        return;
      Started_out=1;
      _dwStartTime=GetTickCount();
    }
  else
    { if(_Stage==L57STAGE_WAIT && _somethingchanged && (!noautostart_setup)) // Не запускаем расчет и не работаем
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
        rdsBlockMessageBox(NULL,"Внутренняя ошибка вычислителя таблиц - неизвестный режим работы",
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
// Описания после класса
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS

// Вывод прогресса
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
// Функция занесения параметров описания в окно настройки
void Setup_FillFuncDescr(RDS_HOBJECT win,L57_String &Func,L57_String &Inputs,L57_String &Numbered,L57_String &Title)
{ L57_String str;
  // Формирование списков параметров
  if(Numbered.IsNotEmpty())
    { str.Assign("Перебираемые параметры: ");
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

  rdsSetObjectStr(win,L57SETUPFIELD_TITLE,RDS_FORMVAL_VALUE,Title.c_str());
  rdsSetObjectStr(win,L57SETUPFIELD_CODE,RDS_FORMVAL_VALUE,Func.c_str());
  rdsSetObjectStr(win,L57SETUPFIELD_PARAMS,RDS_FORMVAL_VALUE,str.c_str());
}
//---------------------------------------------------------------------------

// Функция обратного вызова окна настройки
void RDSCALL Setup_WinCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data)
{ char *modeltext,*templatetext,*lock=NULL,*auxdata;
  L57_String Func,Inputs,Numbered,Title,newmodeltext;
  int starttab=1,res;
  RDS_BHANDLE show;

  // Служебное поле - блокировка редактирования
  lock=rdsGetObjectStr(win,L57SETUPFIELD_LOCKEDIT,RDS_FORMVAL_VALUE);

  #if L57_MODELVERSION>=3
    // Служебное поле: дополнительные данные о функциональном входе
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
      case RDS_FORMSERVEVENT_TCREOPEN:   // Открытие после временного закрытия
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
        Setup_FillFuncDescr(win,Func,Inputs,Numbered,Title);
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

// Сообщение "мало памяти" с номером
void rdsbcppBlockClass::L57OutOfMemMsg(int n)
{ L57_String msg,nstr;
  rdsStopCalc();
  L57ClearOutputData();
  _Stage=L57STAGE_WAIT;
  nstr.AssignInt(n);
  msg.Assign("Мало памяти для работы формирователя таблиц (");
  msg.AddString(nstr);
  msg.AddString(")");
  rdsBlockMessageBox(NULL,msg.c_str(),L57_MSGCAPTION,MB_OK|MB_ICONERROR);
}
//---------------------------------------------------------------------------

// Очистить все выходные данные (при ошибке)
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

// Записать ошибочные выходные данные
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

// Записать готовые выходные данные
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

// Начало вычисления (возвр. успешность инициализации)
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

  // Отводим массивы аргументов
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=_Params.P+i;
      if(!p->AllocateArray())
        _badalloc=TRUE;
      p->_ArgIndex=0;
    }

  // Отведение выходной матрицы
  #if L57_PARAMSCOUNT==1
    // Один параметр
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
    // Два параметра
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

// Вычисление
void rdsbcppBlockClass::Model_Calc(void)
{

  #if L57_PARAMSCOUNT==0
    L57SetOutputData();
    _Stage=L57STAGE_WAIT;
    _Ready=1;
  #else
    L57_LoopBreak _check;
    for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
      { // Вычисляем точку _ArgIndex всех параметров
        double val;
        _Progress_Cur++;
        #if L57_PARAMSCOUNT==1
          // Один параметр
          L57_Parameter *p1=_Params.P;
          p1->Current=p1->Array.Data[p1->_ArgIndex];
          // Вычисляем функцию
          L57ParamsToCalc();
          val=_Calculator._Calc_main();
          // Заносим данные в таблицу
          Table_out[p1->_ArgIndex][1]=val;
        #else
          // Два параметра
          L57_Parameter *p1=_Params.P,
                        *p2=_Params.P+1;
          p1->Current=p1->Array.Data[p1->_ArgIndex];
          p2->Current=p2->Array.Data[p2->_ArgIndex];
          // Вычисляем функцию
          L57ParamsToCalc();
          val=_Calculator._Calc_main();
          // Заносим данные в таблицу
          Table_out[p1->_ArgIndex+1][p2->_ArgIndex+1]=val;
        #endif
        // Берем следующий индекс
        if(_Params.NextStep())
          { // Перебор закончен
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

