//===================================================================================
// Описания указателей на функции для включения в какие-либо структуры
// (без "extern", без тела функции)
// Пример:
// typedef struct {
//   #define RDSCTRL_SERV_FUNC_POINTERS
//   #include "RdsCtrl.h"
// } TFunctionPointers;
//
//===================================================================================
#ifdef RDSCTRL_SERV_FUNC_POINTERS
  // Вспомогательный макрос, объявляющий переменную-указатель
  #define _RDSCTRL_AUX_FUNC(name,type) type name;
  // Описания переменных-указателей на функции
  #include "RdsCtrl_.h"
  #undef _RDSCTRL_AUX_FUNC
  #undef RDSCTRL_SERV_FUNC_POINTERS
// Конец #ifdef RDSCTRL_SERV_FUNC_POINTERS
#endif
//===================================================================================


//===================================================================================
// Отдельное описания тела функции получения адресов экспортированных функций
//===================================================================================
#ifdef RDSCTRL_SERV_FUNC_GETPTR
// Функция получения указателей
BOOL RDSCTRL_SERV_FUNC_GETPTR(HMODULE dll)
  { BOOL ok=TRUE;
    // Вспомогательный макрос, связывающий функцию с указателем
    #define _RDSCTRL_AUX_FUNC(name,type) ok=((name=(type)GetProcAddress(dll,#name))!=NULL)&&ok;
    #include "RdsCtrl_.h"
    #undef _RDSCTRL_AUX_FUNC
    return ok;
  }
  #undef RDSCTRL_SERV_FUNC_GETPTR
// конец #ifdef RDSCTRL_SERV_FUNC_GETPTR
#endif
//===================================================================================
