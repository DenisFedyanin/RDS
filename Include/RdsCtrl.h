#ifndef RdsCtrlH
#define RdsCtrlH

#include <pshpack1.h> // Установка плотной упаковки структур

#ifndef RDSCALL
  #define RDSCALL CALLBACK
#endif

//---------------------------------------------------------------------------
// Функция обратного вызова для возврата строки
typedef void (RDSCALL *RDSCTRL_RETURNSTRING)(LPVOID,LPSTR);
//---------------------------------------------------------------------------

// Типы событий
#define RDSCTRLEVENT_CALCSTART  1       // Запуск расчета
#define RDSCTRLEVENT_CALCSTOP   2       // Остановка расчета
#define RDSCTRLEVENT_CONNCLOSED 3       // Разрыв связи
#define RDSCTRLEVENT_BLOCKMSG   4       // Сообщение от блока
#define RDSCTRLEVENT_NEWFILE    5       // Загрузка другой схемы или создание новой
#define RDSCTRLEVENT_EDITMODE   6       // Режим редактирования
#define RDSCTRLEVENT_CALCMODE   7       // Режим моделирования
#define RDSCTRLEVENT_SAVEFILE   8       // Сохранение схемы
#define RDSCTRLEVENT_LOADREQ    9       // Запрос на загрузку схемы
#define RDSCTRLEVENT_PROGRESS   10      // Прогресс загрузки или сохранения
//---------------------------------------------------------------------------

// Структура описания события, используемая в реакциях на события через
// сообщение окну (передается как LParam)
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (для совместимости версий)
  int Link;             // Номер соединения, вызвавшего событие
  int Event;            // Событие (RDSCTRLEVENT_*)
  LPVOID Data;          // Дополнительные параметры (если есть)
} RDSCTRL_MSGEVENTDATA;
typedef RDSCTRL_MSGEVENTDATA *RDSCTRL_PMSGEVENTDATA;
//---------------------------------------------------------------------------

// Структура описания сообщения от блока. Ее адрес передается через параметр Data
// структуры RDSCTRL_MSGEVENTDATA при реакции через оконные сообщения или через
// третий параметр стандартной CallBack-функции.
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (для совместимости версий)
  LPSTR BlockFullName;  // Полное имя блока, передавшего сообщение
  int IntMsg;           // Полученное целое число
  LPSTR StrMsg;         // Полученная строка
} RDSCTRL_BLOCKMSGDATA;
typedef RDSCTRL_BLOCKMSGDATA *RDSCTRL_PBLOCKMSGDATA;
//---------------------------------------------------------------------------

// Структура описания события RDSCTRLEVENT_NEWFILE. Ее адрес передается через параметр Data
// структуры RDSCTRL_MSGEVENTDATA при реакции через оконные сообщения или через
// третий параметр стандартной CallBack-функции.
#define RDSCTRLEVENT_NEWFILE_NEW        0       // Создана новая система
#define RDSCTRLEVENT_NEWFILE_TEMPLATE   1       // Создана по шаблону
#define RDSCTRLEVENT_NEWFILE_LOAD       2       // Загружена из файла
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (для совместимости версий)
  int Reason;           // Причина вызова (RDSCTRLEVENT_NEWFILE_*)
  LPSTR FileName;       // Имя файла
} RDSCTRL_NEWFILEDATA;
typedef RDSCTRL_NEWFILEDATA *RDSCTRL_PNEWFILEDATA;
//---------------------------------------------------------------------------

// Структура описания события RDSCTRLEVENT_SAVEFILE. Ее адрес передается через параметр Data
// структуры RDSCTRL_MSGEVENTDATA при реакции через оконные сообщения или через
// третий параметр стандартной CallBack-функции.
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (для совместимости версий)
  LPSTR FileName;       // Имя файла
} RDSCTRL_SAVEFILEDATA;
typedef RDSCTRL_SAVEFILEDATA *RDSCTRL_PSAVEFILEDATA;
//---------------------------------------------------------------------------

// Структура описания события RDSCTRLEVENT_PROGRESS. Ее адрес передается через параметр Data
// структуры RDSCTRL_MSGEVENTDATA при реакции через оконные сообщения или через
// третий параметр стандартной CallBack-функции.
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (для совместимости версий)
  DWORD Current;        // Текущее значение
  DWORD Maximum;        // Максимальное значение
} RDSCTRL_PROGRESSDATA;
typedef RDSCTRL_PROGRESSDATA *RDSCTRL_PPROGRESSDATA;
//---------------------------------------------------------------------------

// Структура описания полученного из RDS пункта меню
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (для совместимости версий)
  LPSTR Text;           // Текст пункта меню
  BOOL Enabled;         // Разрешен
  BOOL Visible;         // Видим
  LPSTR BlockFullName;  // Полное имя блока, к которому относится пункт
  int MenuFunc,MenuData;// Параметры пункта меню
  BOOL Checked;         // Помечен
  BOOL Divider;         // Это не пункт меню, а разделитель
  BOOL HasKey;          // У пункта меню в RDS есть "горячая клавиша"
  int Key;              // Код клавиши
  DWORD KeyFlags;       // Флаги клавиши (RDS_M* и RDS_K* из RdsDef.h)
} RDSCTRL_MENUITEM;
typedef RDSCTRL_MENUITEM *RDSCTRL_PMENUITEM;
//---------------------------------------------------------------------------

// Структура для функции rdsctrlSetViewportZoomRectEx
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (для совместимости версий)
  int Left,Top;         // Левый верхний угол прямоугольника
  int Width,Height;     // Размеры прямоугольника
  DWORD Reserved;       // Зарезервировано для выравнивания **** REV2 ****
  double MaxZoom;       // Максимально допустимый масштаб
  DWORD Flags;          // Флаги
  DWORD Reserved2;      // Зарезервировано для выравнивания **** REV2 ****
  // Возвращаемые параметры
  double Zoom;          // Получившийся масштаб
  int ScrollX,ScrollY;  // Получившийся сдвиг
} RDSCTRL_ZOOMRECT;
typedef RDSCTRL_ZOOMRECT *RDSCTRL_PZOOMRECT;
//---------------------------------------------------------------------------

// Флаги для rdsctrlSetViewportZoomRect и rdsctrlSetViewportZoomRectEx
#define RDSCTRL_ZOOMRECTFLAGS_100       1       // Левый верхний угол прямоугольника и его размеры
                                                // даны в масштабе 100%


// Операции для rdsctrlViewportMouse
#define RDSCTRL_MOUSEOP_DOWN    0       // Нажатие
#define RDSCTRL_MOUSEOP_UP      1       // Отпускание
#define RDSCTRL_MOUSEOP_MOVE    2       // Перемещение
#define RDSCTRL_MOUSEOP_DBL     3       // Двойной щелчок
// Флаги для rdsctrlViewportMouse
#define RDSCTRL_MOUSEF_LEFT     1       // Левая кнопка
#define RDSCTRL_MOUSEF_RIGHT    2       // Правая кнопка
#define RDSCTRL_MOUSEF_MIDDLE   4       // Средняя кнопка
#define RDSCTRL_MOUSEF_CTRL     8       // Ctrl
#define RDSCTRL_MOUSEF_ALT      16      // Alt
#define RDSCTRL_MOUSEF_SHIFT    32      // Shift
// Операции для rdsctrlViewportKeyboard
#define RDSCTRL_KEYOP_DOWN      0       // Нажатие
#define RDSCTRL_KEYOP_UP        1       // Отпускание
// Флаги для rdsctrlViewportKeyboard
#define RDSCTRL_KEYF_LEFT       RDSCTRL_MOUSEF_LEFT     // Левая кнопка
#define RDSCTRL_KEYF_RIGHT      RDSCTRL_MOUSEF_RIGHT    // Правая кнопка
#define RDSCTRL_KEYF_MIDDLE     RDSCTRL_MOUSEF_MIDDLE   // Средняя кнопка
#define RDSCTRL_KEYF_CTRL       RDSCTRL_MOUSEF_CTRL     // Ctrl
#define RDSCTRL_KEYF_ALT        RDSCTRL_MOUSEF_ALT      // Alt
#define RDSCTRL_KEYF_SHIFT      RDSCTRL_MOUSEF_SHIFT    // Shift
//---------------------------------------------------------------------------

// Типы для rdsctrlReadBlockMenuItems
#define RDSCTRL_MENUTYPE_BLK    0       // Меню блока
#define RDSCTRL_MENUTYPE_SYS    1      // Меню свободного места окна подсистемы
#define RDSCTRL_MENUTYPE_MAIN   2      // Меню "Система|Дополнительно"
//---------------------------------------------------------------------------

// Флаги для rdsctrlLoadSystemFromMem
#define RDSCTRL_LSFM_SAVEPROMPT 1       // Перезаписывать несохраненную систему
#define RDSCTRL_LSFM_TRANSMODE  (2+4)   // Маска режима
  #define RDSCTRL_LSFM_TRANSMAP    0    // Передавать данные через разделяемую память
  #define RDSCTRL_LSFM_TRANSFILE   2    // Передавать данные через временный файл
//  #define RDSCTRL_LSFM_TRANSPIPE   4    // Передавать данные через трубу (pipe) - не реализовано
//---------------------------------------------------------------------------

// Способ передачи для rdsctrlGetSystemContent
#define RDSCTRL_GSC_TRANSFILE   0       // Через временный файл
#define RDSCTRL_GSC_TRANSPIPE   1       // Через трубу
//---------------------------------------------------------------------------

// Флаги для rdsctrlFindOpSetProviders
#define RDSCTRL_FOSP_RECURSIVE  1       // Искать во вложенных подсистемах
#define RDSCTRL_FOSP_SELF       2       // Проверять указанную подсистему на поддержку функций
//---------------------------------------------------------------------------

// Флаги для rdsctrlSaveSystemTaggedEx/rdsctrlLoadSystemTaggedEx
#define RDSCTRL_TAGGED_SHAREDMEM        1       // Переданное имя - не файл, а область памяти
#define RDSCTRL_TAGGED_DBLBUF           2       // При сохранении - использовать дополнительный буфер
//---------------------------------------------------------------------------

// Структура для получения/установки общих параметров RDS
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (для совместимости версий)
  DWORD UIFlags;        // Настройки пользовательского интерфейса (RDSCTRL_SET_*)
    #define RDSCTRL_SET_ENABLEUI        1       // Разрешенность интерфейса вообще
    #define RDSCTRL_SET_ENABLEEDIT      2       // Разрешен режим редактирования
    #define RDSCTRL_SET_ENABLECALC      4       // Разрешен режим моделирования
    #define RDSCTRL_SET_ENABLERUN       8       // Разрешен запуск расчета
    #define RDSCTRL_SET_ENABLEPARAMED   16      // Разрешено редактирование важных параметров блоков
    #define RDSCTRL_SET_ENABLEOPTIONS   32      // Разрешено изменение настроек RDS
    #define RDSCTRL_SET_ENABLESYSWIN    64      // Разрешено открытие окон подсистем
    #define RDSCTRL_SET_ENABLEMAINWIN   128     // Показывать главное окно RDS
  BOOL SystemAutoSave;  // Автосохранение при выходе

} RDSCTRL_SETTINGS;
typedef RDSCTRL_SETTINGS *RDSCTRL_PSETTINGS;
//---------------------------------------------------------------------------

// Режимы работы RDS
#define RDSCTRL_MODE_UNKNOWN    -1      // Неизвестно (ошибка)
#define RDSCTRL_MODE_EDIT       0       // Режим редактирования
#define RDSCTRL_MODE_CALC       1       // Режим моделирования
#define RDSCTRL_MODE_RUNNING    2       // Режим расчета
//---------------------------------------------------------------------------

// Флаги для rdsctrlExecTool (ДОЛЖНЫ СОВПАДАТЬ с одноименными в RdsDef.h)
// Режим
#define RDSCTRL_ETF_M_EXECUTE       0       // Вызвать модуль расширения в режиме RDS_TOOLM_EXECUTE (как из меню)
#define RDSCTRL_ETF_M_SETUP         1       // Вызвать модуль расширения в режиме RDS_TOOLM_SETUP (настройка)
#define RDSCTRL_ETF_M_CALL          2       // Вызвать модуль расширения в режиме RDS_TOOLM_CALL (или RDS_TOOLM_REMOTECALL) с параметром RDS_PTOOLCALLDATA
// Способ поиска модуля
#define RDSCTRL_ETF_S_BYDLL         0       // По имени функции и имени DLL
#define RDSCTRL_ETF_S_BYSERVNAME    0x100   // По внутреннему имени
// Маски
#define RDSCTRL_ETF_MODEMASK        0xff    // Режим
#define RDSCTRL_ETF_SEARCHMASK      0x300   // Способ поиска
//---------------------------------------------------------------------------

// Типы CallBack-функций
typedef void (RDSCALL *RDSCTRL_CALLBACK)(int,int,LPVOID,LPVOID);
typedef void (RDSCALL *RDSCTRL_BMCALLBACK)(int,LPSTR,int,LPSTR,LPVOID);
//---------------------------------------------------------------------------

// Возврат выравнивания по умолчанию
#include <poppack.h>

// Типы экспортированных функций
typedef int (RDSCALL *RDSCTRL_IV)(void);
typedef void (RDSCALL *RDSCTRL_VI)(int);
typedef BOOL (RDSCALL *RDSCTRL_BI)(int);
typedef void (RDSCALL *RDSCTRL_VS)(LPSTR);
typedef BOOL (RDSCALL *RDSCTRL_BISB)(int,LPSTR,BOOL);
typedef BOOL (RDSCALL *RDSCTRL_BIS)(int,LPSTR);
typedef int (RDSCALL *RDSCTRL_IISISSIpI)(int,LPSTR,int,LPSTR,LPSTR,int,int*);
typedef void (RDSCALL *RDSCTRL_VIS)(int,LPSTR);
typedef void (RDSCALL *RDSCTRL_VIB)(int,BOOL);
typedef BOOL (RDSCALL *RDSCTRL_BIB)(int,BOOL);
typedef void (RDSCALL *RDSCTRL_VII)(int,int);
typedef void (RDSCALL *RDSCTRL_VIIHwUWp)(int,int,HWND,UINT,WPARAM);
typedef void (RDSCALL *RDSCTRL_VIICbpV)(int,int,RDSCTRL_CALLBACK,LPVOID);
typedef void (RDSCALL *RDSCTRL_VICb1pV)(int,RDSCTRL_BMCALLBACK,LPVOID);
typedef void (RDSCALL *RDSCTRL_VIIS)(int,int,LPSTR);
typedef void (RDSCALL *RDSCTRL_VISBB)(int,LPSTR,BOOL,BOOL);
typedef int (RDSCALL *RDSCTRL_IIIHwSDw)(int,int,HWND,LPSTR,DWORD);
typedef void (RDSCALL *RDSCTRL_VIIDIIDw)(int,int,double,int,int,DWORD);
typedef void (RDSCALL *RDSCTRL_VIIDw)(int,int,DWORD);
typedef BOOL (RDSCALL *RDSCTRL_BIIDpIpI)(int,int,double,int*,int*);
typedef void (RDSCALL *RDSCTRL_VIIIIII)(int,int,int,int,int,int);
typedef BOOL (RDSCALL *RDSCTRL_BIIIIIDw)(int,int,int,int,int,DWORD);
typedef void (RDSCALL *RDSCTRL_VIIpDpIpI)(int,int,double*,int*,int*);
typedef int (RDSCALL *RDSCTRL_IISI)(int,LPSTR,int);
typedef RDSCTRL_PMENUITEM (RDSCALL *RDSCTRL_MiII)(int,int);
typedef void (RDSCALL *RDSCTRL_VRs)(RDSCTRL_RETURNSTRING);
typedef BOOL (RDSCALL *RDSCTRL_BIIIIpV)(int,int,int,int,LPVOID);
typedef void (RDSCALL *RDSCTRL_VISII)(int,LPSTR,int,int);
typedef int (RDSCALL *RDSCTRL_III)(int,int);
typedef BOOL (RDSCALL *RDSCTRL_BIIIIpVpIpIpIpIpIpI)(int,int,int,int,LPVOID,int*,int*,int*,int*,int*,int*);
typedef BOOL (RDSCALL *RDSCTRL_BIIpV)(int,int,LPVOID);
typedef BOOL (RDSCALL *RDSCTRL_BIpVDwDw)(int,LPVOID,DWORD,DWORD);
typedef int (RDSCALL *RDSCTRL_IISISpV)(int,LPSTR,int,LPSTR,LPVOID);
typedef BOOL (RDSCALL *RDSCTRL_BIOpt)(int,RDSCTRL_PSETTINGS);
typedef BOOL (RDSCALL *RDSCTRL_BISpVpIpIpIpI)(int,LPSTR,LPVOID,int*,int*,int*,int*);
typedef int (RDSCALL *RDSCTRL_IISBBDwpV)(int,LPSTR,BOOL,BOOL,DWORD,LPVOID);
typedef BOOL (RDSCALL *RDSCTRL_BIpVIRs)(int,LPVOID,int,RDSCTRL_RETURNSTRING);
typedef void (RDSCALL *RDSCTRL_VISSpV)(int,LPSTR,LPSTR,LPVOID);
typedef void (RDSCALL *RDSCTRL_VISSS)(int,LPSTR,LPSTR,LPSTR);
typedef void (RDSCALL *RDSCTRL_VSB)(LPSTR,BOOL);
typedef void (RDSCALL *RDSCTRL_VBB)(BOOL,BOOL);
typedef void (RDSCALL *RDSCTRL_VV)(void);
typedef void (RDSCALL *RDSCTRL_VIIIIIIDDw)(int,int,int,int,int,int,double,DWORD);
typedef BOOL (RDSCALL *RDSCTRL_BIIZr)(int,int,RDSCTRL_PZOOMRECT);
typedef void (RDSCALL *RDSCTRL_VIIDwpDpIpI)(int,int,DWORD,double*,int*,int*);
typedef void (RDSCALL *RDSCTRL_VISB)(int,LPSTR,BOOL);
typedef BOOL (RDSCALL *RDSCTRL_BISDw)(int,LPSTR,DWORD);
typedef void (RDSCALL *RDSCTRL_VIDw)(int,DWORD);
typedef int (RDSCALL *RDSCTRL_IISSDwpV)(int,LPSTR,LPSTR,DWORD,LPVOID);
typedef BOOL (RDSCALL *RDSCTRL_BISDwpDw)(int,LPSTR,DWORD,DWORD*);
typedef BOOL (RDSCALL *RDSCTRL_BISDwDw)(int,LPSTR,DWORD,DWORD);
typedef DWORD (RDSCALL *RDSCTRL_DwIS)(int,LPSTR);
typedef BOOL (RDSCALL *RDSCTRL_BIDwpV)(int,DWORD,LPVOID);
typedef int (RDSCALL *RDSCTRL_II)(int);
typedef BOOL (RDSCALL *RDSCTRL_BIDwpVpDw)(int,DWORD,LPVOID,DWORD*);
typedef int (RDSCALL *RDSCTRL_IIDw)(int,DWORD);
typedef int (RDSCALL *RDSCTRL_IIpDwpDwpV)(int,DWORD*,DWORD*,LPVOID);
typedef HWND (RDSCALL *RDSCTRL_HwIHwSIIIIDw)(int,HWND,LPSTR,int,int,int,int,DWORD);
typedef void (RDSCALL *RDSCTRL_VIHw)(int,HWND);
typedef DWORD (RDSCALL *RDSCTRL_DwIIIIpVpIpIpIpIpIpI)(int,int,int,int,LPVOID,int*,int*,int*,int*,int*,int*);
typedef void (RDSCALL *RDSCTRL_VISIIBt)(int,LPSTR,int,int,BYTE);
typedef BOOL (RDSCALL *RDSCTRL_BIDwSSIS)(int,DWORD,LPSTR,LPSTR,int,LPSTR);
//---------------------------------------------------------------------------

//===================================================================================
// Набор глобальных указателей на интерфейсные функции и функция для получения этих
// указателей (для динамического подключения библиотеки).
// Компилируется ТОЛЬКО если ПЕРЕД директивой #include "RdsCtrl.h" есть описание вида
//     #define RDSCTRL_SERV_FUNC_BODY FuncName
// где FuncName - имя генерируемой функции получения указателей.
// Этот набор может быть включен ТОЛЬКО В ОДИН файл проекта, т.е. только в одном файле
// перед #include "RdsCtrl.h" может находиться #define RDSCTRL_SERV_FUNC_BODY.
// Во всех остальных файлах проекта для доступа к сервисным функциям перед #include "RdsCtrl.h"
// следует включить описание
//     #define RDSCTRL_SERV_FUNC_EXTERNAL
// Старый макрос RDSCTRL_DYNFUNCACCESS нельзя использовать из-за ограничения на развернутый текст
// макроопределения в 4096 байт.
// Пример использования (в Borland C++ 5.5):
//    ------- Главный файл проекта -------
//    #define RDSCTRL_SERV_FUNC_BODY MyGetService
//    #include "RdsCtrl.h"  // Здесь будет вставлено тело функции BOOL MyGetService(HMODULE dll)
//    ......
//
//    DllModule=LoadLibrary("RdsCtrl.dll");
//    if(!MyGetService(DllModule))
//      MessageBox(NULL,"Ошибка доступа к сервисным функциям","RdsCtrl.dll",MB_OK | MB_ICONERROR);
//    ......
//
//    ------- Другие файлы проекта -------
//    #define RDSCTRL_SERV_FUNC_EXTERNAL
//    #include "RdsFunc.h"  // Здесь будут вставлены описания внешних переменных
//    ......
// Перед RdsCtrl.h также необходимо включить файл, отвечающий за описания типов и
// прототипы функций Windows API (в частности, типа BOOL и функции GetModuleHandle).
// Для Borland C++ подойдет "windows.h".
// Для статической загрузки описания перед #include не нужны.
//---------------------------------------------------------------------------
//===================================================================================
#ifdef RDSCTRL_SERV_FUNC_BODY
  // Вспомогательный макрос, объявляющий переменную-указатель
  #define _RDSCTRL_AUX_FUNC(name,type) type name;
  // Описания переменных-указателей на функции
  #include "RdsCtrl_.h"
  #undef _RDSCTRL_AUX_FUNC
// Функция получения указателей
BOOL RDSCTRL_SERV_FUNC_BODY(HMODULE dll)
  { BOOL ok=TRUE;
    // Вспомогательный макрос, связывающий функцию с указателем
    #define _RDSCTRL_AUX_FUNC(name,type) ok=((name=(type)GetProcAddress(dll,#name))!=NULL)&&ok;
    #include "RdsCtrl_.h"
    #undef _RDSCTRL_AUX_FUNC
    return ok;
  }
// конец #ifdef RDSCTRL_SERV_FUNC_BODY
#endif
//===================================================================================

//===================================================================================
// Описания указателей на функции для файлов, не использующих #define RDS_SERV_FUNC_BODY
// (с "extern")
//===================================================================================
#if defined(RDSCTRL_SERV_FUNC_EXTERNAL) && !defined(RDSCTRL_SERV_FUNC_BODY)
  // Вспомогательный макрос, объявляющий внешнюю переменную-указатель
  #define _RDSCTRL_AUX_FUNC(name,type) extern type name;
  #include "RdsCtrl_.h"
  #undef _RDSCTRL_AUX_FUNC
// конец #if defined(RDSCTRL_SERV_FUNC_EXTERNAL) && !defined(RDSCTRL_SERV_FUNC_BODY)
#endif
//===================================================================================

// конец #ifndef RdsCtrlH
#endif


