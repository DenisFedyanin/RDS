//---------------------------------------------------------------------------
// Описания, необходимые для модулей автокомпиляции RDS
//---------------------------------------------------------------------------

#ifndef RdsCompH
#define RdsCompH
//---------------------------------------------------------------------------
#ifndef RdsDefH
  // Для нормальной компиляции необходимы описания из RdsDef.h
  #include "RdsDef.h"
#endif

#include <pshpack1.h> // Установка плотной упаковки структур

// Сообщения (режимы вызова)
#define RDS_COMPM_INIT          0 // Инициализация модуля
#define RDS_COMPM_CLEANUP       1 // Очистка (вызывается перед удалением модуля)
#define RDS_COMPM_MODELINIT     2 // Инициализация модели
#define RDS_COMPM_MODELCLEANUP  3 // Очистка модели (вызывается перед отключением модели)
#define RDS_COMPM_SETUP         5 // Вызов функции настройки
#define RDS_COMPM_ATTACHBLOCK   6 // Добавление блока к модели
#define RDS_COMPM_DETACHBLOCK   7 // Отключение блока от модели
#define RDS_COMPM_PREPARE       8 // Подготовиться к компиляции и проверить ее
                                  // необходимость
#define RDS_COMPM_COMPILE       9 // Компилировать
#define RDS_COMPM_STRUCTCHANGE 10 // Сообщение об изменении структуры
#define RDS_COMPM_OPENEDITOR   11 // Открыть окно редактора модели
#define RDS_COMPM_CANRENMODEL  12 // Можно ли переименовать модель (вызывается перед переименованием)
#define RDS_COMPM_MODELRENAMED 13 // Переименовать модель (вызывается после переименования)
#define RDS_COMPM_GETOPTIONS   14 // Получить флаги поддерживаемых функций
#define RDS_COMPM_EXECFUNCTION 15 // Выполнить одну из функций RDS_COMPFLAG_*
#define RDS_COMPM_SAVESYSTEM   16 // Вызывается перед сохранением системы
#define RDS_COMPM_CANATTACHBLK 17 // Вызывается перед добавлением блока к модели
                                  // (самой модели может еще не быть)
#define RDS_COMPM_SAVEBLOCK    18 // Вызывается перед сохранением каждого блока с моделью
                                  // (можно скорректировать имя модели перед сохранением)
#define RDS_COMPM_CLOSEALLWIN  19 // Закрыть все немодальные окна
#define RDS_COMPM_MODECHANGE   20 // Изменился режим RDS (редактирование, моделирование, расчет)

#define RDS_COMPM_AFTERSAVESYSTEM 21 // Вызывается после сохранения системы **** REV2 ****
#define RDS_COMPM_BEGINPASTE   22 // Начинается вставка блоков из буфера обмена **** REV2 ****
#define RDS_COMPM_ENDPASTE     23 // Закончена вставка блоков из буфера обмена **** REV2 ****
#define RDS_COMPM_AFTERLOADSYSTEM 24 // Вызывается после загрузки системы **** REV2 ****
#define RDS_COMPM_MODELPOSTINIT   25 // Альтернативная инициализация модели (аналог RDS_COMPM_MODELINIT с дополнительными параметрами) **** REV2 ****
#define RDS_COMPM_BLOCKDELETE  26 // Удаление блока из схемы **** REV2 ****
#define RDS_COMPM_ISEDITOROPEN  27 // Открыт ли редактор модели **** REV2 ****
#define RDS_COMPM_BLOCKFROMFILE  28 // Блок только что вставлен из библиотеки или файла **** REV2 ****
#define RDS_COMPM_BLOCKLIBRARYOP 29 // Какая-то операция с блоком в библиотеке **** REV2 ****
#define RDS_COMPM_SAVETEMPGLOBAL 30 // Записать глобальные данные в память при сохранении для буфера обмена или сервисной функции

// Возвращаемые функцией значения
#define RDS_COMPR_DONE          0 // Нормальное завершение (для всех режимов)
#define RDS_COMPR_ERROR         1 // Ошибка
#define RDS_COMPR_ERRORNOMSG    2 // Ошибка, не выводить сообщение (для некоторых режимов)

// Уникальный идентификатор модуля автокомпиляции
typedef LPVOID RDS_COMPHANDLE;
// Уникальный идентификатор модели
typedef LPVOID RDS_MODELHANDLE;

// Данные модуля автокомпиляции (хранятся все время существования модуля)
typedef struct
{ RDS_COMPHANDLE Module;// Идентификатор модуля (только чтение)
  LPVOID ModuleData;    // Адрес области данных модуля (чтение/запись)
  LPSTR DllFullPath;    // Полный путь к DLL модуля (только чтение)
  LPSTR DllFuncName;    // Имя функции DLL модуля (только чтение)
  int NModels;          // Число моделей, связанных с данным модулем (только чтение)

  LPSTR DllFuncParam;   // Имя набора параметров модуля (только чтение) **** REV2 ****
} RDS_COMPMODULEDATA;
typedef RDS_COMPMODULEDATA *RDS_PCOMPMODULEDATA;
//---------------------------------------------------------------------------

// Данные модели (хранятся все время существования модели)
typedef struct
{ RDS_MODELHANDLE Model;// Идентификатор модели (только чтение)
  LPSTR ModelName;      // Имя модели (только чтение)
  LPSTR ModelNameUC;    // Имя модели в верхнем регистре (только чтение)
  LPVOID ModelData;     // Адрес области данных модели (чтение/запись)
  int NBlocks;          // Число блоков, связанных с данной моделью (только чтение)
  RDS_COMPHANDLE Module;// Идентификатор модуля автокомпиляции, к которому относится
                        // модель (только чтение)
  // Параметры, устанавливаемые модулем автокомпиляции
  LPSTR CompDllName;    // DLL скомпилированной модели (только чтение,
                        // установка сервисной функцией во время вызова BFCM_COMP_PREPARE)
  LPSTR CompDllFunc;    // Имя функции скомпилированной модели (только чтение,
                        // установка сервисной функцией во время вызова BFCM_COMP_PREPARE)
  BOOL Valid;           // Признак необходимости перекомпиляции (чтение/запись, устанавливается
                        // модулем во время вызова BFCM_COMP_PREPARE)
  // Дополнительные параметры
  LPSTR AltModelName;   // Альтернативное имя модели (только чтение). Хранится в схеме и может
                        // использоваться модулем автокомпиляции для своих нужд. При подключении
                        // новой модели через окно параметров блока всегда содержит пустую строку.
                        // Устанавливается функцией rdscompSetAltModelName.
  // Целое пользовательское поле (не обрабатывается и не инициализируется)
  int Tag;

  int ModelVersion;     // Версия модели (устанавливается модулем) **** REV2 ****
  DWORD ModelVersionDate;// Дата модели (устанавливается модулем) **** REV2 ****
} RDS_COMPMODELDATA;
typedef RDS_COMPMODELDATA *RDS_PCOMPMODELDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением RDS_COMPM_CANATTACHBLK
typedef struct
{ LPSTR ModelName;      // Имя модели (только чтение)
  LPSTR ModelNameUC;    // Имя модели в верхнем регистре (только чтение)
  LPSTR AltModelName;   // Альтернативное имя модели (только чтение)

  RDS_BHANDLE Block;    // Блок (только чтение)

  int AttachReason;     // Причина подключения блока к модели (RDS_COMP_AR_*) (только чтение)

  BOOL ChangeModel;     // Подключить другую модель вместо этой (запись)
                        // По умолчанию - FALSE. Для подключения вместо указанной другой
                        // модели следует установить в TRUE и передать новое имя модели при
                        // помощи функции rdscompAttachDifferentModel

  LPSTR BlockFile;      // Имя файла блока или NULL (только чтение,
                        // только при AttachReason==RDS_COMP_AR_LOADFROMFILE или RDS_COMP_AR_LOADSYSTEM) **** REV2 ****
  BOOL LoadSingleBlock; // Идет загрузка одного блока (подсистемы) внутрь схемы (только чтение,
                        // только при AttachReason==RDS_COMP_AR_LOADFROMFILE или RDS_COMP_AR_LOADSYSTEM) **** REV2 ****

} RDS_COMPCANATTACHBLKDATA;
typedef RDS_COMPCANATTACHBLKDATA *RDS_PCOMPCANATTACHBLKDATA;
// Причины подключения блока (для AttachReason)
#define RDS_COMP_AR_UNKNOWN             0       // Неизвестно (внутренняя ошибка)
#define RDS_COMP_AR_LOADSYSTEM          1       // Загрузка всей системы
#define RDS_COMP_AR_LOADCLIPBRD         2       // Загрузка блоков из буфера обмена
#define RDS_COMP_AR_LOADFROMFILE        3       // Загрузка одиночного блока из библиотеки
#define RDS_COMP_AR_LOADUNDO            4       // Загрузка из-за отмены изменений
#define RDS_COMP_AR_MANUALSET           5       // Новая модель задана в окне параметров
                                                // или сервисной функцией
#define RDS_COMP_AR_RENAMEMODEL         6       // Переименование модели (т.е. подключение
                                                // всех блоков к переименованной)
#define RDS_COMP_AR_LOADPROG            7       // Загрузка блоков из памяти сервисной функцией
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщениями RDS_COMPM_ATTACHBLOCK и RDS_COMPM_DETACHBLOCK
typedef struct
{ RDS_PCOMPMODELDATA Model;     // Модель
  RDS_BHANDLE Block;            // Блок
  int AttachReason;             // Причина подключения блока к модели
                                // (RDS_COMP_AR_*, только для RDS_COMPM_ATTACHBLOCK)
} RDS_COMPBLOCKOPDATA;
typedef RDS_COMPBLOCKOPDATA *RDS_PCOMPBLOCKOPDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением RDS_COMPM_BLOCKDELETE **** REV2 ****
typedef struct
{ RDS_PCOMPMODELDATA Model;     // Модель
  RDS_BHANDLE Block;            // Блок
} RDS_COMPBLOCKDELDATA;
typedef RDS_COMPBLOCKDELDATA *RDS_PCOMPBLOCKDELDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением RDS_COMPM_MODELPOSTINIT **** REV2 ****
typedef struct
{ RDS_PCOMPMODELDATA Model;     // Модель
  int AttachReason;             // Причина подключения модели
                                // (RDS_COMP_AR_*)
  LPSTR BlockFile;              // Имя файла блока или NULL (только чтение,
                                // только при AttachReason==RDS_COMP_AR_LOADFROMFILE) **** REV2 ****
} RDS_COMPMPOSTINITDATA;
typedef RDS_COMPMPOSTINITDATA *RDS_PCOMPMPOSTINITDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением RDS_COMPM_OPENEDITOR и RDS_COMPM_ISEDITOROPEN **** REV2 ****
typedef struct
{ RDS_PCOMPMODELDATA Model;     // Данные модели
  RDS_BHANDLE Block;            // Блок, для которого вызван редактор (только для RDS_COMPM_OPENEDITOR)
} RDS_OPENEDITORDATA;
typedef RDS_OPENEDITORDATA *RDS_POPENEDITORDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением RDS_COMPM_PREPARE
typedef struct
{ RDS_PCOMPMODELDATA Model;     // Данные проверяемой и подготавливаемой модели
  BOOL Rebuild;                 // TRUE если проверка не нужна (при принудительной
                                // перекомпиляции всех моделей независимо от их
                                // состояния)
} RDS_COMPPREPAREDATA;
typedef RDS_COMPPREPAREDATA *RDS_PCOMPPREPAREDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением RDS_COMPM_COMPILE
typedef struct
{ RDS_PCOMPMODELDATA *InvalidModels;    // Модели, которые нужно компилировать
                                        // (массив адресов структур)
  int IMCount;                          // Размер массива InvalidModels
  BOOL Rebuild;                         // TRUE - принудительная перекомпиляция
                                        // всех моделей
} RDS_COMPILEDATA;
typedef RDS_COMPILEDATA *RDS_PCOMPILEDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением RDS_COMPM_STRUCTCHANGE
typedef struct
{ LPSTR OldStructType;  // Имя типа структуры до изменения
  LPSTR NewStructType;  // Имя типа структуры после изменения
  BOOL Renamed;         // Структура переименована (OldStructType не совпадает с NewStructType)

} RDS_COMPSTRUCTCHGDATA;
typedef RDS_COMPSTRUCTCHGDATA *RDS_PCOMPSTRUCTCHGDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщениями RDS_COMPM_CANRENMODEL и RDS_COMPM_MODELRENAMED
typedef struct
{ RDS_PCOMPMODELDATA Model;     // Данные модели, которая будет переименована (для RDS_COMPM_CANRENMODEL)
                                // или уже переименована (для RDS_COMPM_MODELRENAMED)
  LPSTR OldModelName;           // Старое имя модели
  LPSTR NewModelName;           // Новое имя модели
  // Возврат
  BOOL AllowRename;             // Переименование разрешено (возврат для RDS_COMPM_CANRENMODEL)

} RDS_COMPMODELRENAMEDATA;
typedef RDS_COMPMODELRENAMEDATA *RDS_PCOMPMODELRENAMEDATA;
//---------------------------------------------------------------------------

// Флаги, возвращаемые в ответ на RDS_COMPM_GETOPTIONS
#define RDS_COMPFLAG_FUNCMODELUSERINPUT 1       // Можно вводить имя модели вручную
#define RDS_COMPFLAG_FUNCMODELBROWSE    2       // Работает кнопка "Обзор"
#define RDS_COMPFLAG_FUNCMODELCREATE    4       // Работает кнопка "Создать"
#define RDS_COMPFLAG_FUNCMODELSAVEAS    8       // Работает кнопка "Сохранить как..."

#define RDS_COMPFLAG_CANCHANGESTRUCT    16      // Модуль может менять структуры в моделях при
                                                // из изменении в RDS

// Структура, передаваемая вместе с сообщением RDS_COMPM_EXECFUNCTION
typedef struct
{ int Function;         // Функция (RDS_COMPFLAG_FUNC*)
  LPSTR ModelName;      // Содержимое строки модели
  // Следующие параметры могут быть не определены для некоторых функций.
  RDS_HOBJECT BlockVars;// Переменные блока (если есть, только при Function==RDS_COMPFLAG_FUNCMODELCREATE)
  int BlockType;        // Тип блока (RDS_BT* или RDS_TUNKNOWN, см. RdsDef.h)
} RDS_COMPEXECFUNCDATA;
typedef RDS_COMPEXECFUNCDATA *RDS_PCOMPEXECFUNCDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщениями RDS_COMPM_SAVESYSTEM
// и RDS_COMPM_AFTERSAVESYSTEM **** REV2 ****
typedef struct
{
  LPSTR FileName;       // Имя сохраняемого файла
  LPSTR OldFileName;    // Имя файла до сохранения или NULL при первом сохранении
                        // созданной системы
} RDS_COMPSAVESYSTEMDATA;
typedef RDS_COMPSAVESYSTEMDATA *RDS_PCOMPSAVESYSTEMDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением RDS_COMPM_SAVEBLOCK
typedef struct
{ RDS_PCOMPMODELDATA Model;     // Данные сохраняемой модели
  RDS_BHANDLE Block;            // Сохраняемый блок
  int SaveAction;               // Причина сохранения (RDS_LS_SAVE*)

  BOOL ChangeName;      // Записать другое имя модели (запись)
                        // По умолчанию - FALSE. Для записи вместо указанной другой
                        // модели следует установить в TRUE и передать новое имя модели при
                        // помощи функции rdscompAttachDifferentModel

  LPSTR TargetFile;     // Полный путь к файлу, куда сохраняется блок или вся схема, или NULL **** REV2 ****
  BOOL SaveSingleBlock; // Идет сохранение одного блока (подсистемы) в отдельный файл **** REV2 ****
} RDS_COMPSAVEBLOCKDATA;
typedef RDS_COMPSAVEBLOCKDATA *RDS_PCOMPSAVEBLOCKDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением RDS_COMPM_BLOCKFROMFILE **** REV2 ****
typedef struct
{ RDS_BHANDLE Block;
} RDS_COMPBLOCKFROMFILEDATA;
typedef RDS_COMPBLOCKFROMFILEDATA *RDS_PCOMPBLOCKFROMFILEDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением RDS_COMPM_SAVETEMPGLOBAL **** REV2 ****
typedef struct
{ RDS_PCOMPMODELDATA Model;     // Данные сохраняемой модели
  int SaveAction;               // Причина сохранения (RDS_LS_SAVE*)
} RDS_COMPSAVETEMPGLOBALDATA;
typedef RDS_COMPSAVETEMPGLOBALDATA *RDS_PCOMPSAVETEMPGLOBALDATA;
//---------------------------------------------------------------------------

// Функция вызова модуля автокомпиляции - тип указателя
typedef int(RDSCALL *RDS_COMPENTRYPOINT)(int,RDS_PCOMPMODULEDATA,LPVOID);
//---------------------------------------------------------------------------

// Описание операции с библиотекой блоков **** REV2 ****
// Передается вмести с сообщением RDS_COMPM_BLOCKLIBRARYOP или заполняется функцией rdscompLibraryOperation
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом функций)
  int Operation;        // Операция (RDS_LOP_*)
    #define RDS_LOP_NONE        0       // Действий с библиотекой сейчас не производится
    #define RDS_LOP_PASTE       1       // Вставка блока в подсистему из буфера обмена для последующей записи в библиотеку
    #define RDS_LOP_SAVE        2       // Сохранение блока из памяти в файл
    #define RDS_LOP_COPY        3       // Копирование библиотечного блока
    #define RDS_LOP_RENAME      4       // Переименование библиотечного блока
    #define RDS_LOP_DELETE      5       // Удаление библиотечного блока
  LPSTR BlockFilePath;  // Путь к файлу блока, с которым проводится операция
  LPSTR SystemPath;     // Путь к файлу схемы, в которую вставляется блок для RDS_LOP_PASTE
  LPSTR OldBlockFilePath;  // Путь к исходному файлу блока для RDS_LOP_COPY и RDS_LOP_RENAME
  // Следующие поля - только при реакции на сообщение RDS_COMPM_BLOCKLIBRARYOP
  int Status;          // Состояние (RDS_LOPST_*)
    #define RDS_LOPST_NONE      0       // Не реакция на сообщение, поля модели не заполнены
    #define RDS_LOPST_BEGIN     1       // Перед операцией, поля модели не заполнены
    #define RDS_LOPST_END       2       // После операции, поля модели не заполнены
    #define RDS_LOPST_MODEL     3       // Вызов для модели, поля модели заполнены
  LPSTR ModelName;      // Имя модели
  LPSTR AltModelName;   // Альтернативное имя модели
} RDS_COMPLIBRARYOPDATA;
typedef RDS_COMPLIBRARYOPDATA *RDS_PCOMPLIBRARYOPDATA;
//---------------------------------------------------------------------------


// Возврат выравнивания по умолчанию
#include <poppack.h>

// Интерфейсные функции - типы указателей
typedef void (RDSCALL *RDS_VMhSS)(RDS_MODELHANDLE,LPSTR,LPSTR);
typedef RDS_PCOMPMODELDATA (RDSCALL *RDS_MdChI)(RDS_COMPHANDLE,int);
typedef RDS_BHANDLE (RDSCALL *RDS_BhMhIBd)(RDS_MODELHANDLE,int,RDS_PBLOCKDESCRIPTION descr);
typedef BOOL (RDSCALL *RDS_BChSSB)(RDS_COMPHANDLE,LPSTR,LPSTR,BOOL);
typedef RDS_PCOMPMODELDATA (RDSCALL *RDS_MdChS)(RDS_COMPHANDLE,LPSTR);
typedef BOOL (RDSCALL *RDS_BMhB)(RDS_MODELHANDLE,BOOL);
typedef void (RDSCALL *RDS_VMhS)(RDS_MODELHANDLE,LPSTR);
typedef RDS_PCOMPMODELDATA (RDSCALL *RDS_MdBh)(RDS_BHANDLE);
typedef RDS_PCOMPMODULEDATA (RDSCALL *RDS_CdCh)(RDS_COMPHANDLE);
typedef BOOL (RDSCALL *RDS_BMh)(RDS_MODELHANDLE);
typedef BOOL (RDSCALL *RDS_BLop)(RDS_PCOMPLIBRARYOPDATA);
typedef void (RDSCALL *RDS_VMhB)(RDS_MODELHANDLE,BOOL);
//---------------------------------------------------------------------------
// Макросы для сервисных функций описаны в "RdsFunc.h" и "RdsFunc_.h"
//---------------------------------------------------------------------------
#endif
