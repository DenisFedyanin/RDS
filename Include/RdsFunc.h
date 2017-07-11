//---------------------------------------------------------------------------
// Набор макросов для доступа к интерфейсным функциям RDS из DLL
//---------------------------------------------------------------------------
#ifndef RdsFuncH
#define RdsFuncH

#ifndef RdsDefH
  // Для нормальной компиляции необходимы описания из RdsDef.h
  #include "RdsDef.h"
#endif

#ifndef RdsCompH
  // Для нормальной компиляции необходимы описания из RdsComp.h
  #include "RdsComp.h"
#endif

//===================================================================================
// Набор глобальных указателей на интерфейсные функции и функция для получения этих
// указателей.
// Компилируется ТОЛЬКО если ПЕРЕД директивой #include "RdsFunc.h" есть описание вида
//     #define RDS_SERV_FUNC_BODY FuncName
// где FuncName - имя генерируемой функции получения указателей.
// Этот набор может быть включен ТОЛЬКО В ОДИН файл проекта, т.е. только в одном файле
// перед #include "RdsFunc.h" может находиться #define RDS_SERV_FUNC_BODY.
// Старый макрос RDS_INTFUNCACCESS нельзя использовать из-за ограничения на развернутый текст
// макроопределения в 4096 байт.
// Пример использования (в Borland C++ 5.5):
//    ------- Главный файл проекта -------
//    #define RDS_SERV_FUNC_BODY MyGetService
//    #include "RdsFunc.h"  // Здесь будет вставлено тело функции BOOL MyGetService(void)
//    ......
//    int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
//    {
//      if(reason==DLL_PROCESS_ATTACH)
//        {
//          if(!MyGetService())
//            MessageBox(NULL,"Ошибка доступа к сервисным функциям","DLL",MB_OK | MB_ICONERROR);
//        }
//      return 1;
//    }
//    ......
//    ------- Другие файлы проекта -------
//    #include "RdsFunc.h"
//    ......
// Перед RdsFunc.h также необходимо включить файл, отвечающий за описания типов и
// прототипы функций Windows API (в частности, типа BOOL и функции GetModuleHandle).
// Для Borland C++ подойдет "windows.h".
//---------------------------------------------------------------------------
//===================================================================================
#ifdef RDS_SERV_FUNC_BODY
  // Вспомогательный макрос, объявляющий переменную-указатель
  #define _RDS_AUX_FUNC(name,type) type name;

  // Описания переменных-указателей на функции
  #include "RdsFunc_.h"

  #undef _RDS_AUX_FUNC
// Функция получения указателей
BOOL RDS_SERV_FUNC_BODY(void)
  { HMODULE exe=GetModuleHandle(NULL);
    BOOL ok=TRUE;
    // Вспомогательный макрос, связывающий функцию с указателем
    #define _RDS_AUX_FUNC(name,type) ok=((name=(type)GetProcAddress(exe,#name))!=NULL)&&ok;

    // Присвоение указателей переменным
    #include "RdsFunc_.h"

    #undef _RDS_AUX_FUNC
    return ok;
  }
// конец #ifdef RDS_SERV_FUNC_BODY
#endif
//===================================================================================


//===================================================================================
// Описания указателей на функции для файлов, не использующих #define RDS_SERV_FUNC_BODY
// (с "extern")
//===================================================================================
#if !defined(RDS_NOEXTERNFUNCPTRS) && !defined(RDS_SERV_FUNC_BODY)
  // Вспомогательный макрос, объявляющий внешнюю переменную-указатель
  #define _RDS_AUX_FUNC(name,type) extern type name;

  // Описания с extern
  #include "RdsFunc_.h"

  #undef _RDS_AUX_FUNC
// конец #ifndef RDS_AUX_NOEXTERNFUNCPTRS
#endif
//===================================================================================


//===================================================================================
// Функции-синонимы (другие обозначения универсальных функций для улучшения
// читаемости исходных текстов)
//===================================================================================
#ifndef RDS_NOHOBJMACROS
  // Редактировать связь по вспомогательному объекту
  #define rdsCEEditConnection           rdsCEEditConnBus
  // Редактировать шину по вспомогательному объекту
  #define rdsCEEditBus                  rdsCEEditConnBus
  // Создать связь по вспомогательному объекту
  #define rdsCECreateConnection(o,p,e)  rdsCECreateConnBus((o),(p),RDS_CTCONNECTION,(e))
  // Создать шину по вспомогательному объекту
  #define rdsCECreateBus(o,p,e)         rdsCECreateConnBus((o),(p),RDS_CTBUS,(e))
  // Очистить объект-редактор связи
  #define rdsCEClearEditor(o)           rdsCommandObject((o),RDS_HCE_RESET)
  // Получить массив блоков из объекта-списка
  #define rdsBCLGetBlockArray(o,ps)     ((RDS_BHANDLE*)rdsGetObjectArray((o),RDS_HBCL_BLOCKARRAY,0,(ps)))
  // Получить массив связей из объекта-списка
  #define rdsBCLGetConnArray(o,ps)      ((RDS_CHANDLE*)rdsGetObjectArray((o),RDS_HBCL_CONNARRAY,0,(ps)))
  // Установить имя структуры для набора переменных
  #define rdsVSSetStructName(o,name)    rdsSetObjectStr((o),RDS_HVAR_SETTYPENAME,0,(name))
  // Получить имя структуры набора переменных
  #define rdsVSGetStructName(o)         rdsGetObjectStr((o),RDS_HVAR_GETTYPENAME,0)
  // Очистить объект-набор переменных
  #define rdsVSClearEditor(o)           rdsCommandObject((o),RDS_HVAR_RESET)
  // Удалить переменную из набора переменных
  #define rdsVSDeleteVar(o,n)           rdsCommandObjectEx((o),RDS_HVAR_DELVAR,(n),NULL)
  // Удалить автосоединение из набора переменных
  #define rdsVSDeleteAutoConn(o,n)      rdsCommandObjectEx((o),RDS_HVAR_DELAUTO,(n),NULL)
  // Получить имя главной переменной автосоединения с указанным номером в наборе переменных
  #define rdsVSGetAutoMain(o,n)         rdsGetObjectStr((o),RDS_HVAR_GETAUTOMAIN,(n))
  // Получить имя связанной переменной автосоединения с указанным номером в наборе переменных
  #define rdsVSGetAutoConn(o,n)         rdsGetObjectStr((o),RDS_HVAR_GETAUTOCONN,(n))
  // Получить число переменных в наборе переменных
  #define rdsVSGetFieldCount(o)         rdsGetObjectInt((o),RDS_HVAR_GETFIELDCOUNT,0)
  // Получить число автоматических соединений  в наборе переменных
  #define rdsVSGetAutoCount(o)          rdsGetObjectInt((o),RDS_HVAR_GETAUTOCOUNT,0)
  // Получить уровень набора переменных
  #define rdsVSGetStructRank(o)         rdsGetObjectInt((o),RDS_HVAR_GETVARRANK,0)
  // Очистить список переименования структур
  #define rdsVSClearTypeRenames(o)      rdsCommandObject((o),RDS_HVAR_CLEARTYPEREN)
  // Очистить список переименования переменных
  #define rdsVSClearVarRenames(o)       rdsCommandObject((o),RDS_HVAR_CLEARVARREN)
  // Установить текст для анализатора (rdsSTRCreateTextReader)
  #define rdsSTRSetTextToRead(obj,text) rdsSetObjectStr((obj),RDS_HSTR_SETTEXT,0,(text))
  // Считать целое число
  #define rdsSTRGetIntWord(obj,skiplf)  rdsGetObjectInt((o),RDS_HSTR_READINT,(skiplf))
  // Считать число double
  #define rdsSTRGetDoubleWord(obj,skiplf) rdsGetObjectDouble((o),RDS_HSTR_READDOUBLE,(skiplf))
  // Очистить текст обработчика INI-файлов (rdsINICreateTextHolder)
  #define rdsINIClearText(o)            rdsCommandObject((o),RDS_HINI_RESET)
  // Установить текст для обработчика INI-файлов
  #define rdsINISetText(obj,text)       rdsSetObjectStr((obj),RDS_HINI_SETTEXT,0,(text))
  // Загрузить файл для обработчика INI-файлов
  #define rdsINILoadFile(obj,filename)  rdsSetObjectStr((obj),RDS_HINI_LOADFILE,0,(filename))
  // Записать файл для обработчика INI-файлов
  #define rdsINISaveFile(obj,filename)  rdsSetObjectStr((obj),RDS_HINI_SAVEFILE,0,(filename))
  // Создать секцию для обработчика INI-файлов
  #define rdsINICreateSection(obj,section)     rdsSetObjectStr((obj),RDS_HINI_CREATESECTION,0,(section))
  // Удалить секцию для обработчика INI-файлов
  #define rdsINIDeleteSection(obj,section)     rdsSetObjectStr((obj),RDS_HINI_DELETESECTION,0,(section))
  // Записать логическое значение в текущую секцию обработчика INI-файлов
  #define rdsINIWriteBool(obj,key,val)  rdsINIWriteInt((obj),(key),(val)?1:0)
  // Считать логическое значение из текущей секции обработчика INI-файлов
  #define rdsINIReadBool(obj,key,defval)        (rdsINIReadInt((obj),(key),(defval)?1:0)!=0)
  // Удалить значение из текущей секции обработчика INI-файлов
  #define rdsINIDeleteValue(obj,key)            rdsSetObjectStr((obj),RDS_HINI_DELETEKEYLAST,0,(key))
  // Записать текст обработчика INI-файлов как данные блока в текстовом формате
  #define rdsINISaveBlockText(o,newln)          rdsCommandObjectEx((o),RDS_HINI_SAVEBLOCKTEXT,(newln)?1:0,NULL)
  // Передать форме целое число (для объекта rdsFORMCreate)
  #define rdsFORMSetInt(o,id,val)               rdsSetObjectInt((o),(id),RDS_FORMVAL_VALUE,(val))
  // Получить у формы целое число (для объекта rdsFORMCreate)
  #define rdsFORMGetInt(o,id)                   rdsGetObjectInt((o),(id),RDS_FORMVAL_VALUE)
  // Передать форме строку (для объекта rdsFORMCreate)
  #define rdsFORMSetString(o,id,str)            rdsSetObjectStr((o),(id),RDS_FORMVAL_VALUE,(str))
  // Получить у формы строку (для объекта rdsFORMCreate)
  #define rdsFORMGetString(o,id)                rdsGetObjectStr((o),(id),RDS_FORMVAL_VALUE)
  // Передать форме число double (для объекта rdsFORMCreate)
  #define rdsFORMSetDouble(o,id,val)            rdsSetObjectDouble((o),(id),RDS_FORMVAL_VALUE,(val))
  // Получить у формы число double (для объекта rdsFORMCreate)
  #define rdsFORMGetDouble(o,id)                rdsGetObjectDouble((o),(id),RDS_FORMVAL_VALUE)
  // Передать форме BOOL (для объекта rdsFORMCreate)
  #define rdsFORMSetBool(o,id,val)              rdsSetObjectInt((o),(id),RDS_FORMVAL_VALUE,(val)?1:0)
  // Получить у формы BOOL (для объекта rdsFORMCreate)
  #define rdsFORMGetBool(o,id)                  (rdsGetObjectInt((o),(id),RDS_FORMVAL_VALUE)!=0)
  // Установить список вариантов для поля ввода (для объекта rdsFORMCreate)
  #define rdsFORMSetComboList(o,id,list)        rdsSetObjectStr((o),(id),RDS_FORMVAL_LIST,(list))
  // Установить дополнительный флаг разрешения поля ввода (для объекта rdsFORMCreate)
  #define rdsFORMSetEnableCheck(o,id,enabled)   rdsSetObjectInt((o),(id),RDS_FORMVAL_CHECK,(enabled)?1:0)
  // Получить дополнительный флаг разрешения поля ввода (для объекта rdsFORMCreate)
  #define rdsFORMGetEnableCheck(o,id)           (rdsGetObjectInt((o),(id),RDS_FORMVAL_CHECK)!=0)
  // Разрешить/запретить весь орган управления
  #define rdsFORMEnableControl(o,id,enabled)    rdsSetObjectInt((o),(id),RDS_FORMVAL_ENABLED,(enabled)?1:0)
  // Установить высоту для многострочного поля ввода (для объекта rdsFORMCreate)
  #define rdsFORMSetMultilineHeight(o,id,height)   rdsSetObjectInt((o),(id),RDS_FORMVAL_MLHEIGHT,(height))
  // Очистить форму (для объекта rdsFORMCreate)
  #define rdsFORMClear(o)                       rdsCommandObject((o),RDS_FORM_CLEAR)
  // Показать форму (для объекта rdsFORMCreate)
  #define rdsFORMShowModal(o)                   rdsCommandObject((o),RDS_FORM_SHOWMODAL)
  // Показать или убрать ProgressBar (для объекта rdsPBARCreate)
  #define rdsPBARShow(o,show)                   rdsCommandObject((o),(show)?RDS_PBAR_SHOW:RDS_PBAR_HIDE)
  // Установить текущее значение ProgressBar (для объекта rdsPBARCreate)
  #define rdsPBARSetPos(o,pos)                  rdsSetObjectInt((o),RDS_PBAR_POSITION,0,(pos))
  // Изменить текущее значение ProgressBar (для объекта rdsPBARCreate)
  #define rdsPBARIncrement(o,inc)               rdsSetObjectInt((o),RDS_PBAR_ADDTOPOS,0,(inc))
  //  Записать новые параметры блока в undo (для объекта rdsBEUCreate)
  #define rdsBEUStore(o)                        rdsCommandObject((o),RDS_BEU_STORECHANGED)
#endif
//===================================================================================


#endif

