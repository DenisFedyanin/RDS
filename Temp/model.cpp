//---------------------------------------------------------------------------
// Автоматически сформированный исходный текст для модели "IpuL57Mk2SimpleCalc3.mdl"
// Исходный файл модели: C:\Users\Денис\Documents\ДЛЯ РДС\BR-2\IpuL57Mk2SimpleCalc3.mdl
// Модуль автокомпиляции: Open Watcom C++ (локально)
//---------------------------------------------------------------------------
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define RDSBCPP_MODELNAME "IpuL57Mk2SimpleCalc3.mdl" // Имя модели для индикации

#include <RdsDef.h>
#define RDS_SERV_FUNC_BODY rdsbcppGetService
#include <RdsFunc.h>
#include <CommonBl.h>
#include <CommonAC.hpp>

//---------------------------------------------------------------------------
// Математические константы
//---------------------------------------------------------------------------
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif
#ifndef M_E
  #define M_E 2.71828182845904523536
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Макросы обработки исключений
//---------------------------------------------------------------------------
// Обработка исключений включена
#define RDSBCPP_EXCEPTIONS
// Оператор try
#define RDSBCPP_TRY try
// Оператор catch
#define RDSBCPP_CATCHALL catch(...)
//---------------------------------------------------------------------------

// Ошибка математических функций
double rdsbcppHugeDouble;

//---------------------------------------------------------------------------
// Перехват ошибок математики
//---------------------------------------------------------------------------
int matherr(struct _exception *a)
{ a->retval=(a->type==UNDERFLOW)?0.0:rdsbcppHugeDouble;
  return 1;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Глобальные описания пользователя
//---------------------------------------------------------------------------
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */



































// Версия модели
#define L57_MODELVERSION 3


#define L57MS_GLOBALS
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_GLOBALS

// Служебный класс для вычисления значения функции
class L57_Calc
{
  public:
    double x;
    double A;
    L57_RdsDoubleArray X;
    double i;
    // Функция вычисления (генерируемая)
    double _Calc_gen(void);
    // Функция вычисления (с перехватом исключений)
    double _Calc_main(void)
    { double __temp;
      RDSBCPP_TRY
        { __temp=_Calc_gen(); }
      RDSBCPP_CATCHALL
        { __temp=rdsbcppHugeDouble; }
      return __temp;
    };
};
// Функция вычисления (генерируемая)
double L57_Calc::_Calc_gen(void)
{
int n=X.Size();
double S=0;

for (int j=0; j<n; j++){
	S=S+X[j];
};

if (i<n) x=(1-A/1000000)*X[i]+A*S/n/1000000;

return x; // 3
; /* */
  return rdsbcppHugeDouble;
}
//----------------------------------------------------------------------

/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Объекты для функций блока
//---------------------------------------------------------------------------
// Функция "IPU.L57.UserFuncTransfer.IsFuncSource"
class rdsbcppFunction0G : public rdsbcppFunction // Глобальный
{ public:
    // Вызов у блока
    int Call(RDS_BHANDLE Block)
      {return rdsCallBlockFunction(Block,_Id,NULL);};
    // Вызов у блоков подсистемы
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,NULL,Flags);};
    rdsbcppFunction0G(void):rdsbcppFunction(){};
    ~rdsbcppFunction0G(){};
};
rdsbcppFunction0G L57_BlkFunc_IsFuncSource;
class rdsbcppFunction0L : public rdsbcppFunction // Локальный
{ public:
    // Вызов у блока
    int Call(RDS_BHANDLE Block)
      {return rdsCallBlockFunction(Block,_Id,NULL);};
    // Вызов у блоков подсистемы
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,NULL,Flags);};
    // Вызов у исполнителя функции
    int Call(void)
      { return _Link?rdsCallBlockFunction(_Link->Block,_Id,NULL):0;};
    // Перевод функций подписки/регистрации в public
    inline void RegisterProvider(void){_RegisterProvider();};
    inline void UnregisterProvider(void){_UnregisterProvider();};
    inline void SubscribeToProvider(void){_SubscribeToProvider();};
    inline void UnsubscribeFromProvider(void){_UnsubscribeFromProvider();};
    rdsbcppFunction0L(void):rdsbcppFunction(){};
    ~rdsbcppFunction0L(){};
};
// Функция "IPU.L57.UserFuncTransfer.AcceptFunction"
class rdsbcppFunction1G : public rdsbcppFunction // Глобальный
{ public:
    // Вызов у блока
    int Call(RDS_BHANDLE Block,L57_AcceptFunc_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // Вызов у блоков подсистемы
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_AcceptFunc_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    rdsbcppFunction1G(void):rdsbcppFunction(){};
    ~rdsbcppFunction1G(){};
};
rdsbcppFunction1G L57_BlkFunc_AcceptFunc;
class rdsbcppFunction1L : public rdsbcppFunction // Локальный
{ public:
    // Вызов у блока
    int Call(RDS_BHANDLE Block,L57_AcceptFunc_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // Вызов у блоков подсистемы
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_AcceptFunc_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    // Вызов у исполнителя функции
    int Call(L57_AcceptFunc_Param* param)
      { return _Link?rdsCallBlockFunction(_Link->Block,_Id,param):0;};
    // Перевод функций подписки/регистрации в public
    inline void RegisterProvider(void){_RegisterProvider();};
    inline void UnregisterProvider(void){_UnregisterProvider();};
    inline void SubscribeToProvider(void){_SubscribeToProvider();};
    inline void UnsubscribeFromProvider(void){_UnsubscribeFromProvider();};
    rdsbcppFunction1L(void):rdsbcppFunction(){};
    ~rdsbcppFunction1L(){};
};
// Функция "IPU.L57.UserFuncTransfer.AcceptParameter"
class rdsbcppFunction2G : public rdsbcppFunction // Глобальный
{ public:
    // Вызов у блока
    int Call(RDS_BHANDLE Block,L57_AcceptParam_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // Вызов у блоков подсистемы
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_AcceptParam_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    rdsbcppFunction2G(void):rdsbcppFunction(){};
    ~rdsbcppFunction2G(){};
};
rdsbcppFunction2G L57_BlkFunc_AcceptParam;
class rdsbcppFunction2L : public rdsbcppFunction // Локальный
{ public:
    // Вызов у блока
    int Call(RDS_BHANDLE Block,L57_AcceptParam_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // Вызов у блоков подсистемы
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_AcceptParam_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    // Вызов у исполнителя функции
    int Call(L57_AcceptParam_Param* param)
      { return _Link?rdsCallBlockFunction(_Link->Block,_Id,param):0;};
    // Перевод функций подписки/регистрации в public
    inline void RegisterProvider(void){_RegisterProvider();};
    inline void UnregisterProvider(void){_UnregisterProvider();};
    inline void SubscribeToProvider(void){_SubscribeToProvider();};
    inline void UnsubscribeFromProvider(void){_UnsubscribeFromProvider();};
    rdsbcppFunction2L(void):rdsbcppFunction(){};
    ~rdsbcppFunction2L(){};
};
// Функция "IPU.L57.UserFuncTransfer.IsParameterTransferredWithFunc"
class rdsbcppFunction3G : public rdsbcppFunction // Глобальный
{ public:
    // Вызов у блока
    int Call(RDS_BHANDLE Block,L57_CheckParamConn_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // Вызов у блоков подсистемы
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_CheckParamConn_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    rdsbcppFunction3G(void):rdsbcppFunction(){};
    ~rdsbcppFunction3G(){};
};
rdsbcppFunction3G L57_BlkFunc_CheckTransParam;
class rdsbcppFunction3L : public rdsbcppFunction // Локальный
{ public:
    // Вызов у блока
    int Call(RDS_BHANDLE Block,L57_CheckParamConn_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // Вызов у блоков подсистемы
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_CheckParamConn_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    // Вызов у исполнителя функции
    int Call(L57_CheckParamConn_Param* param)
      { return _Link?rdsCallBlockFunction(_Link->Block,_Id,param):0;};
    // Перевод функций подписки/регистрации в public
    inline void RegisterProvider(void){_RegisterProvider();};
    inline void UnregisterProvider(void){_UnregisterProvider();};
    inline void SubscribeToProvider(void){_SubscribeToProvider();};
    inline void UnsubscribeFromProvider(void){_UnsubscribeFromProvider();};
    rdsbcppFunction3L(void):rdsbcppFunction(){};
    ~rdsbcppFunction3L(){};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Точка входа DLL
//---------------------------------------------------------------------------
int WINAPI DllMain(HINSTANCE hinst,unsigned long reason,void *lpReserved)
{ if(reason==DLL_PROCESS_ATTACH)
    { if(!RDS_SERV_FUNC_BODY())
        { MessageBox(NULL,"Невозможно получить доступ к сервисным функциям RDS",RDSBCPP_MODELNAME,MB_OK | MB_ICONERROR);
          return 0;
        }
      else // Есть доступ к сервисным функциям
        { rdsGetHugeDouble(&rdsbcppHugeDouble);
          // Регистрация функций блока
          L57_BlkFunc_IsFuncSource.Register("IPU.L57.UserFuncTransfer.IsFuncSource");
          L57_BlkFunc_AcceptFunc.Register("IPU.L57.UserFuncTransfer.AcceptFunction");
          L57_BlkFunc_AcceptParam.Register("IPU.L57.UserFuncTransfer.AcceptParameter");
          L57_BlkFunc_CheckTransParam.Register("IPU.L57.UserFuncTransfer.IsParameterTransferredWithFunc");
        }
    }
  return 1;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Описания классов переменных блока
//---------------------------------------------------------------------------
// Переменная char ("C")
RDSBCPP_STATICPLAINCLASS(rdsbcstChar,char);
// Переменная double ("D")
RDSBCPP_STATICPLAINCLASS(rdsbcstDouble,double);
// Переменная char ("S")
RDSBCPP_STATICPLAINCLASS(rdsbcstSignal,char);
// Переменная short int ("H")
RDSBCPP_STATICPLAINCLASS(rdsbcstShort,short int);
// Массив double ("MD")
RDSBCPP_STATICPLAINARRCLASS(rdsbcstADouble,double);
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Класс блока
//---------------------------------------------------------------------------
class rdsbcppBlockClass
{ public:
    // Структура данных блока
    RDS_PBLOCKDATA rdsbcppBlockData;
    // Статические переменные
    rdsbcstSignal _Start;
    rdsbcstSignal _Ready;
    rdsbcstShort Aux_in;
    rdsbcstShort Aux_out;
    rdsbcstChar Func_in;
    rdsbcstChar Func_out;
    rdsbcstDouble Calc_out;
    rdsbcstSignal _allinval_dchg;
    rdsbcstSignal _X_mchg;
    rdsbcstDouble x;
    rdsbcstDouble A;
    rdsbcstADouble X;
    rdsbcstDouble i;

    // Заглушка ограничения доступа
    rdsbcppStaticStub rdsbcppStaticStubVar;

    // Объекты функций блоков (те же имена, что и у глобальных)
    rdsbcppFunction0L L57_BlkFunc_IsFuncSource;
    rdsbcppFunction1L L57_BlkFunc_AcceptFunc;
    rdsbcppFunction2L L57_BlkFunc_AcceptParam;
    rdsbcppFunction3L L57_BlkFunc_CheckTransParam;

    // Инициализация переменных блока
    void rdsbcppInitVars(void *base)
    {
      // Статические переменные
      _Start.InitNum(base,0,0);
      _Ready.InitNum(base,1,1);
      Aux_in.InitNum(base,2,2);
      Aux_out.InitNum(base,4,3);
      Func_in.InitNum(base,6,4);
      Func_out.InitNum(base,7,5);
      Calc_out.InitNum(base,8,6);
      _allinval_dchg.InitNum(base,16,7);
      _X_mchg.InitNum(base,17,8);
      x.InitNum(base,18,9);
      A.InitNum(base,26,10);
      X.InitNum(base,34,11);
      i.InitNum(base,42,12);
    };

    // Проверка существования динамических переменных
    BOOL rdsbcppDynVarsOk(void)
    { return TRUE; };

    // Функции реакции на события
    void rdsbcppModel(RDS_PINITIALCALCDATA InitialCalc);
    void rdsbcppSetupFunc(int &Result);
    void rdsbcppInit(void);
    void rdsbcppStartCalc(RDS_PSTARTSTOPDATA StartStopData);
    void rdsbcppCalcMode(void);
    void rdsbcppLoadText(char *LoadedText);
    void rdsbcppSaveText(void);
    void rdsbcppOther(int CallMode,void *ExtParam,int &Result);
    void L57_BlkFunc_IsFuncSourceEvent(RDS_PFUNCTIONCALLDATA FData,int &Result);
    void L57_BlkFunc_AcceptFuncEvent(L57_AcceptFunc_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result);
    void L57_BlkFunc_AcceptParamEvent(L57_AcceptParam_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result);
    void L57_BlkFunc_CheckTransParamEvent(L57_CheckParamConn_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result);
    void rdsbcppStaticVarsInit(void);
    void rdsbcppResetCalc(void);

    // Конструктор
    rdsbcppBlockClass(RDS_PBLOCKDATA data);
    // Деструктор
    ~rdsbcppBlockClass();
//---------------------------------------------------------------------------
// Описания пользователя в классе блока
//---------------------------------------------------------------------------
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_CLASS
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_CLASS

  // Объекты для передачи входов-параметров функции
  L57_FuncTransfer _Transfer;
  L57_FuncTransferParameter *x_trans;
  L57_FuncTransferParameter *A_trans;
  L57_FuncTransferParameter *X_trans;
  L57_FuncTransferParameter *i_trans;
  // Сброс
  void L57_Reset_Trans_Data(void)
  {
    x_trans=NULL;
    A_trans=NULL;
    X_trans=NULL;
    i_trans=NULL;
  };
  // Создание всех параметров передачи
  void L57_Create_Trans_Data(void);

  // Хранилища для проверки изменения переменных
  double x_old;
  double A_old;
  L57_DblMatrTempStorage X_old;
  double i_old;
  // Инициализация хранилищ значений
  void L57_Init_Old_Data(void)
  {
    x_old=rdsbcppHugeDouble;
    A_old=rdsbcppHugeDouble;
    i_old=rdsbcppHugeDouble;
    X_old.Clear();
  };

/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
//---------------------------------------------------------------------------

}; // class rdsbcppBlockClass
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Описания пользователя после класса блока
//---------------------------------------------------------------------------
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_AFTERCLASS
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_AFTERCLASS

// Экспорт текста функции для взаимодействия с другими блоками по связям
char *rdsbcppBlockClass::_FuncExportText="V A V 1\n"
"V i V 0\n"
"V X A\n"
"V x V 0\n"
"F\n"
"TСинус A*x\n"
"X\n"
"|int n=X.Size();\n"
"|double S=0;\n"
"|\n"
"|for (int j=0; j<n; j++){\n"
"|\tS=S+X[j];\n"
"|};\n"
"|\n"
"|if (i<n) x=(1-A/1000000)*X[i]+A*S/n/1000000;\n"
"|\n"
"|return x; // 3\n"
"}\n"
"#ORDER#\n"
"x\n"
"A\n"
"X\n"
"i";

// Занесение стационарных переменных в вычислитель функции
void rdsbcppBlockClass::L57InputsToCalc(void)
{
  _Calculator.x=x;
  _Calculator.A=A;
  _Calculator.X.Setup(X);
  _Calculator.i=i;
}
//-----------------------------------------------------------------------

// Создание всех параметров передачи
void rdsbcppBlockClass::L57_Create_Trans_Data(void)
{
  // Стационарных входов: 4
  _Transfer.ClearParameters();
  _Transfer.EnlargeParameters(4);

  x_trans=_Transfer.AddParameterRds("x",L57_VARSUFFIX_NONE,FALSE,x.Num(),&x);
  A_trans=_Transfer.AddParameterRds("A",L57_VARSUFFIX_NONE,FALSE,A.Num(),&A);
  X_trans=_Transfer.AddParameterRds("X",L57_VARSUFFIX_NONE,TRUE,X.Num(),&X);
  i_trans=_Transfer.AddParameterRds("i",L57_VARSUFFIX_NONE,FALSE,i.Num(),&i);

}
//-----------------------------------------------------------------------

// Проверка изменения переменных и передача параметров
void rdsbcppBlockClass::L57InputsChange(void)
{ BOOL _changes=FALSE;

  // Стационарных вещественных входов: 3
  // Служебных вещественных входов: 0
  // Итого вещественных: 3

  // Вещественные входы
  if(_allinval_dchg!=0)
    { _allinval_dchg=0;
      _changes=FALSE;

      if(x_old!=x)
        { _changes=TRUE;
          _Transfer.TransferParameter(rdsbcppBlockData->Block,x_trans,_Transfer.DestBlocks,TRUE);
        }
      if(A_old!=A)
        { _changes=TRUE;
          _Transfer.TransferParameter(rdsbcppBlockData->Block,A_trans,_Transfer.DestBlocks,TRUE);
        }
      if(i_old!=i)
        { _changes=TRUE;
          _Transfer.TransferParameter(rdsbcppBlockData->Block,i_trans,_Transfer.DestBlocks,TRUE);
        }

      if(_changes)
        { // Запоминание изменившихся
          x_old=x;
          A_old=A;
          i_old=i;
        }
    }

  // Матричные входы (матрицы, массивы, функции)
  if(X_old.Check(_X_mchg.GetPtr(),&X,_changes,FALSE))
    _Transfer.TransferParameter(rdsbcppBlockData->Block,X_trans,_Transfer.DestBlocks,TRUE);

}
//-----------------------------------------------------------------------

/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Функция блока
//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) int RDSCALL rdsbcppBlockEntryPoint(
  int CallMode,             // Режим вызова (сообщение RDS_BFM_*)
  RDS_PBLOCKDATA BlockData, // Данные блока
  LPVOID ExtParam           // Дополнительные данные (зависят от CallMode)
)
{ int result=RDS_BFR_DONE;  // Код возврата
  // Объект класса блока (хранится в личной области данных)
  rdsbcppBlockClass *data=(rdsbcppBlockClass*)(BlockData->BlockData);

  // Перехват ошибок математики - начало
  volatile unsigned int FPMask=_control87(0,0);
  _control87(MCW_EM,MCW_EM);

  switch(CallMode)
    { case RDS_BFM_INIT: // Инициализация блока
        BlockData->BlockData=(data=new rdsbcppBlockClass(BlockData));
        data->rdsbcppInit();
        BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
        break;

      case RDS_BFM_CLEANUP: // Очистка (вызывается перед удалением)
        delete data;
        break;

      case RDS_BFM_VARCHECK: // Проверка допустимости структуры переменных
        if(strcmp((char*)ExtParam,"{SSHHCCDSSDDMDD}")!=0)
          return RDS_BFR_BADVARSMSG;
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppStaticVarsInit();
        BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
        break;

      case RDS_BFM_FUNCTIONCALL: // Реакция на вызов функции блока
        if(((RDS_PFUNCTIONCALLDATA)ExtParam)->Function==L57_BlkFunc_IsFuncSource.Id())
          { // Функция "IPU.L57.UserFuncTransfer.IsFuncSource"
            data->rdsbcppInitVars(BlockData->VarData);
            data->L57_BlkFunc_IsFuncSourceEvent((RDS_PFUNCTIONCALLDATA)ExtParam,result);
            BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
          }
        else if(((RDS_PFUNCTIONCALLDATA)ExtParam)->Function==L57_BlkFunc_AcceptFunc.Id())
          { // Функция "IPU.L57.UserFuncTransfer.AcceptFunction"
            data->rdsbcppInitVars(BlockData->VarData);
            data->L57_BlkFunc_AcceptFuncEvent((L57_AcceptFunc_Param*)(((RDS_PFUNCTIONCALLDATA)ExtParam)->Data),(RDS_PFUNCTIONCALLDATA)ExtParam,result);
            BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
          }
        else if(((RDS_PFUNCTIONCALLDATA)ExtParam)->Function==L57_BlkFunc_AcceptParam.Id())
          { // Функция "IPU.L57.UserFuncTransfer.AcceptParameter"
            data->rdsbcppInitVars(BlockData->VarData);
            data->L57_BlkFunc_AcceptParamEvent((L57_AcceptParam_Param*)(((RDS_PFUNCTIONCALLDATA)ExtParam)->Data),(RDS_PFUNCTIONCALLDATA)ExtParam,result);
            BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
          }
        else if(((RDS_PFUNCTIONCALLDATA)ExtParam)->Function==L57_BlkFunc_CheckTransParam.Id())
          { // Функция "IPU.L57.UserFuncTransfer.IsParameterTransferredWithFunc"
            data->rdsbcppInitVars(BlockData->VarData);
            data->L57_BlkFunc_CheckTransParamEvent((L57_CheckParamConn_Param*)(((RDS_PFUNCTIONCALLDATA)ExtParam)->Data),(RDS_PFUNCTIONCALLDATA)ExtParam,result);
            BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
          }
        break;

      case RDS_BFM_MODEL: // Один такт моделирования
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppModel((RDS_PINITIALCALCDATA)ExtParam);
        BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
        break;

      case RDS_BFM_SETUP: // Вызов функции настройки
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppSetupFunc(result);
        BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
        break;

      case RDS_BFM_STARTCALC: // Запуск расчета
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppStartCalc((RDS_PSTARTSTOPDATA)ExtParam);
        BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
        break;

      case RDS_BFM_CALCMODE: // Вход в режим моделирования
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppCalcMode();
        BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
        break;

      case RDS_BFM_LOADTXT: // Загрузка данных блока в текстовом виде
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppLoadText((char*)ExtParam);
        BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
        break;

      case RDS_BFM_SAVETXT: // Запись данных блока в текстовом виде
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppSaveText();
        BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
        break;

      case RDS_BFM_RESETCALC: // Сброс расчета
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppResetCalc();
        BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
        break;

      default: // Прочие события
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppOther(CallMode,ExtParam,result);
        BlockData->BlockData=data; // Предохраняет от случайного изменения пользователем
    } // switch(CallMode)

  // Перехват ошибок математики - конец
  _clear87();
  _control87(FPMask,MCW_EM);

  return result;
}
//---------------------------------------------------------------------------

// Конструктор класса блока
rdsbcppBlockClass::rdsbcppBlockClass(RDS_PBLOCKDATA data)
{ // Сохранение адреса структуры данных блока
  rdsbcppBlockData=data;
  // Инициализация локальных объектов-функций по глобальным
  L57_BlkFunc_IsFuncSource.InitLocal(::L57_BlkFunc_IsFuncSource);
  L57_BlkFunc_AcceptFunc.InitLocal(::L57_BlkFunc_AcceptFunc);
  L57_BlkFunc_AcceptParam.InitLocal(::L57_BlkFunc_AcceptParam);
  L57_BlkFunc_CheckTransParam.InitLocal(::L57_BlkFunc_CheckTransParam);

}
//---------------------------------------------------------------------------

// Деструктор класса блока
rdsbcppBlockClass::~rdsbcppBlockClass()
{
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Функции реакции на события
//---------------------------------------------------------------------------
// Один такт моделирования
void rdsbcppBlockClass::rdsbcppModel(RDS_PINITIALCALCDATA InitialCalc)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_MODEL
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_MODEL

/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::rdsbcppModel
//---------------------------------------------------------------------------

// Вызов функции настройки
void rdsbcppBlockClass::rdsbcppSetupFunc(int &Result)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_SETUP
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_SETUP

/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::rdsbcppSetupFunc
//---------------------------------------------------------------------------

// Ограничение доступа к статическим переменным
#define _Start rdsbcppStaticStubVar
#define _Ready rdsbcppStaticStubVar
#define Aux_in rdsbcppStaticStubVar
#define Aux_out rdsbcppStaticStubVar
#define Func_in rdsbcppStaticStubVar
#define Func_out rdsbcppStaticStubVar
#define Calc_out rdsbcppStaticStubVar
#define _allinval_dchg rdsbcppStaticStubVar
#define _X_mchg rdsbcppStaticStubVar
#define x rdsbcppStaticStubVar
#define A rdsbcppStaticStubVar
#define X rdsbcppStaticStubVar
#define i rdsbcppStaticStubVar
// Инициализация данных блока
void rdsbcppBlockClass::rdsbcppInit(void)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_INIT
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_INIT


/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::rdsbcppInit
// Восстановление доступа к статическим переменным
#undef i
#undef X
#undef A
#undef x
#undef _X_mchg
#undef _allinval_dchg
#undef Calc_out
#undef Func_out
#undef Func_in
#undef Aux_out
#undef Aux_in
#undef _Ready
#undef _Start
//---------------------------------------------------------------------------

// Запуск расчета
void rdsbcppBlockClass::rdsbcppStartCalc(RDS_PSTARTSTOPDATA StartStopData)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_START
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_START

/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::rdsbcppStartCalc
//---------------------------------------------------------------------------

// Вход в режим моделирования
void rdsbcppBlockClass::rdsbcppCalcMode(void)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_CALCMODE
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_CALCMODE

/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::rdsbcppCalcMode
//---------------------------------------------------------------------------

// Загрузка данных блока в текстовом виде
void rdsbcppBlockClass::rdsbcppLoadText(char *LoadedText)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_LOADPARAMS
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_LOADPARAMS
/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::rdsbcppLoadText
//---------------------------------------------------------------------------

// Запись данных блока в текстовом виде
void rdsbcppBlockClass::rdsbcppSaveText(void)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_SAVEPARAMS
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_SAVEPARAMS
/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::rdsbcppSaveText
//---------------------------------------------------------------------------

// Прочие события
void rdsbcppBlockClass::rdsbcppOther(int CallMode,void *ExtParam,int &Result)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_OTHER
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_OTHER
/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::rdsbcppOther
//---------------------------------------------------------------------------

// Реакция на функцию блока "IPU.L57.UserFuncTransfer.IsFuncSource"
void rdsbcppBlockClass::L57_BlkFunc_IsFuncSourceEvent(RDS_PFUNCTIONCALLDATA FData,int &Result)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

Result=1;
/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::L57_BlkFunc_IsFuncSourceEvent
//---------------------------------------------------------------------------

// Реакция на функцию блока "IPU.L57.UserFuncTransfer.AcceptFunction"
void rdsbcppBlockClass::L57_BlkFunc_AcceptFuncEvent(L57_AcceptFunc_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_ACCEPTFUNC
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_ACCEPTFUNC

/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::L57_BlkFunc_AcceptFuncEvent
//---------------------------------------------------------------------------

// Реакция на функцию блока "IPU.L57.UserFuncTransfer.AcceptParameter"
void rdsbcppBlockClass::L57_BlkFunc_AcceptParamEvent(L57_AcceptParam_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_ACCEPTPARAM
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_ACCEPTPARAM

/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::L57_BlkFunc_AcceptParamEvent
//---------------------------------------------------------------------------

// Реакция на функцию блока "IPU.L57.UserFuncTransfer.IsParameterTransferredWithFunc"
void rdsbcppBlockClass::L57_BlkFunc_CheckTransParamEvent(L57_CheckParamConn_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_CHECKTRANSFUNC
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_CHECKTRANSFUNC

/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::L57_BlkFunc_CheckTransParamEvent
//---------------------------------------------------------------------------

// После успешного создания статических переменных
void rdsbcppBlockClass::rdsbcppStaticVarsInit(void)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_VARCHECK
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_VARCHECK

/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::rdsbcppStaticVarsInit
//---------------------------------------------------------------------------

// Сброс расчета
void rdsbcppBlockClass::rdsbcppResetCalc(void)
{
/* ВНИМАНИЕ! Эта модель не предназначена для редактирования пользователем,
ее текст генерируется автоматически */

#define L57MS_RESET
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_RESET


/* Служебный комментарий, предохраняющий от незавершенного комментария в тексте пользователя */
} // rdsbcppBlockClass::rdsbcppResetCalc
//---------------------------------------------------------------------------

