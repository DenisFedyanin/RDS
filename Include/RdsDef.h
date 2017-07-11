//---------------------------------------------------------------------------
// Описания, необходимые для моделей блоков RDS
//---------------------------------------------------------------------------
#ifndef RdsDefH
#define RdsDefH
//================ Описания, необходимые для совместимости ==================
#ifndef RDS_NO_PACK
  #include <pshpack1.h> // Установка плотной упаковки структур
#endif
#ifndef RDSCALL
  #define RDSCALL CALLBACK
#endif

//===================================================================================
// Версия RDS, на которую рассчитаны эти описания. Может использоваться для сравнения
// с номером версии, полученным из RDS, и вывода сообщения об ошибке.
// Пример:
// if(rdsServiceVersion!=NULL && (rdsServiceVersion()<RDS_FVERSION))
//   { // Сообщение об ошибке: старая версия RDS
//     // ....
//   }
//===================================================================================
// Преобразование версии в int
#define RDS_INTVERSION(high,low,build) (((high)<<24) | ((low)<<16) | (build))
// Преобразование даты версии в DWORD **** REV2 ****
#define RDS_DWORDVERDATE(y,mon,d,h,mnu,q) ( ((((y-2000))&0x3ff)<<22)|(((mon)&0xf)<<18)|(((d)&0x1f)<<13)|(((h)&0x1f)<<8)|(((mnu)&0x3f)<<2)|((q)&3) )
#define RDS_DWORDVERDATESHORT(y,mon,d) ( ((((y-2000))&0x3ff)<<22)|(((mon)&0xf)<<18)|(((d)&0x1f)<<13) )

#ifndef RDS_NOVERSIONDEFINES
  // Описания версии RDS
  #define RDS_FVERSIONHIGH      1
  #define RDS_FVERSIONLOW       0
  #define RDS_FVERSIONBUILD     491
  #define RDS_FVERSION          (RDS_INTVERSION(RDS_FVERSIONHIGH,RDS_FVERSIONLOW,RDS_FVERSIONBUILD))
  // Описания даты выпуска этой версии RDS **** REV2 ****
  #define RDS_FVERSIONYEAR      2017    // Год
  #define RDS_FVERSIONMONTH     5       // Месяц
  #define RDS_FVERSIONDAY       28      // День
  #define RDS_FVERSIONHOUR      0       // Час
  #define RDS_FVERSIONMIN       0       // Минута
  #define RDS_FVERSIONQUARTER   0       // Четверть минуты
  #define RDS_FVERSIONDATE      (RDS_DWORDVERDATE(RDS_FVERSIONYEAR,RDS_FVERSIONMONTH,RDS_FVERSIONDAY,RDS_FVERSIONHOUR,RDS_FVERSIONMIN,RDS_FVERSIONQUARTER))
#endif
//===================================================================================

//======================== Основные описания ================================
// Описания, необходимые для функций отклика блоков

//---------------------------------------------------------------------------
// Сообщения (режимы вызова)
#define RDS_BFM_INIT                     0 // Инициализация блока
#define RDS_BFM_CLEANUP                  1 // Очистка (вызывается перед удалением)
#define RDS_BFM_MODEL                    2 // Моделирование
#define RDS_BFM_RENAME                   3 // Сообщение о переименовании блока
#define RDS_BFM_VARCHECK                 4 // Проверка структуры переменных
#define RDS_BFM_SETUP                    5 // Вызов функции настройки
#define RDS_BFM_REMOTEMSG                6 // Сообщение от внешнего управления
#define RDS_BFM_RESIZE                   7 // Размер блока изменен
#define RDS_BFM_RESIZING                 8 // Размер блока изменяется пользователем
#define RDS_BFM_TIMER                    9 // Сработал таймер
#define RDS_BFM_STARTCALC               10 // Расчет запущен
#define RDS_BFM_STOPCALC                11 // Расчет остановлен
#define RDS_BFM_RESETCALC               12 // Расчет сброшен
#define RDS_BFM_WINREFRESH              13 // Необходимо обновить немодальное окно
#define RDS_BFM_CHECKFUNCSUPPORT        14 // Проверить поддержку глобальной функции блоком (устаревшая)
#define RDS_BFM_FUNCTIONCALL            15 // Вызов глобальной функции
#define RDS_BFM_EDITMODE                16 // Вход в режим редактирования
#define RDS_BFM_CALCMODE                17 // Вход в режим расчета
#define RDS_BFM_POPUPHINT               18 // Запрос текста всплывающей подсказки
#define RDS_BFM_MENUFUNCTION            19 // Вызов внутренней функции блока (реакция на пункт меню)
#define RDS_BFM_UNLOADSYSTEM            20 // Вся система будет выгружена
#define RDS_BFM_MOVED                   21 // Блок перемещен
#define RDS_BFM_MANUALDELETE            22 // Перед удалением блока (вызывается только при удалении блоков
                                           // из схемы ПОЛЬЗОВАТЕЛЕМ)
#define RDS_BFM_BEFORESAVE              23 // Перед сохранением всей схемы
#define RDS_BFM_AFTERSAVE               24 // После сохранения всей схемы
#define RDS_BFM_CONTEXTPOPUP            25 // Вызов контекстного меню блока
#define RDS_BFM_MANUALINSERT            26 // После добавления блока в схему из буфера
                                           // обмена или из файла
#define RDS_BFM_DYNVARCHANGE            27 // Изменение динамической переменной
#define RDS_BFM_PREMODEL                28 // Перед тактом моделирования (для установивших флаг RDS_CTRLCALC)
#define RDS_BFM_BLOCKPANEL              29 // Какое-либо уведомление от панели блока в подсистеме
#define RDS_BFM_AFTERLOAD               30 // После загрузки всей схемы
#define RDS_BFM_CONTENTCHANGE           31 // Реакция на изменения в подсистеме (при подписке) **** REV2 ****
#define RDS_BFM_LEAVINGEDITMODE         32 // Перед выходом из режима редактирования **** REV2 ****
#define RDS_BFM_HELP                    33 // Запрос справки к блоку **** REV2 ****
#define RDS_BFM_SYSWORKSIZECHANGE       34 // Изменение размера рабочего поля подсистемы **** REV2 ****
#define RDS_BFM_LAYERCONFIGCHANGE       35 // Изменение конфигурации слоев подсистемы **** REV2 ****
#define RDS_BFM_SHAREDMEMCHANGE         36 // Изменение разделяемой переменной **** REV2 ****
#define RDS_BFM_TRANSLATE               37 // Вызов для перевода текстов **** REV2 ****

// Загрузка и запись личных данных блока
#define RDS_BFM_LOADBIN                 50 // Загрузка в двоичном виде
#define RDS_BFM_LOADTXT                 51 // Загрузка в текстовом виде
#define RDS_BFM_SAVEBIN                 52 // Запись в двоичном виде
#define RDS_BFM_SAVETXT                 53 // Запись в текстовом виде

// Загрузка и запись текущего состояния блока
#define RDS_BFM_LOADSTATE               60 // Загрузка состояния в двоичном виде
#define RDS_BFM_SAVESTATE               61 // Запись состояния в двоичном виде

// Реакция на "мышь" на изображении блока
#define RDS_BFM_MOUSEDOWN              100 // Реакция на нажатие кнопки "мыши" в режиме расчета
#define RDS_BFM_MOUSEUP                101 // Реакция на отпускание кнопки "мыши" в режиме расчета
#define RDS_BFM_MOUSEMOVE              102 // Реакция на перемещение "мыши" в режиме расчета
#define RDS_BFM_MOUSEDBLCLICK          103 // Реакция на двойной щелчок в режиме расчета
#define RDS_BFM_MOUSESELECT            104 // Возможность выбора блока щелчком "мыши" в режиме редактирования

// Реакция на клавиатуру
#define RDS_BFM_KEYDOWN                120 // Реакция на нажатие клавиши в режиме расчета
#define RDS_BFM_KEYUP                  121 // Реакция на отпускание клавиши в режиме расчета

#define RDS_BFM_DRAW                   150 // Рисование изображения блока
#define RDS_BFM_DRAWADDITIONAL         151 // Рисование дополнительных элементов

// Сообщения, обрабатываемые подсистемой, если блоки не среагировали на "мышь"
#define RDS_BFM_WINDOWMOUSEDOWN        200 // Реакция на нажатие кнопки "мыши" в режиме расчета
#define RDS_BFM_WINDOWMOUSEUP          201 // Реакция на отпускание кнопки "мыши" в режиме расчета
#define RDS_BFM_WINDOWMOUSEMOVE        202 // Реакция на перемещение "мыши" в режиме расчета
#define RDS_BFM_WINDOWMOUSEDBLCLICK    203 // Реакция на двойной щелчок в режиме расчета

// Сообщения, обрабатываемые подсистемой, если блоки не среагировали на клавиатуру
#define RDS_BFM_WINDOWKEYDOWN          220 // Реакция на нажатие клавиши в режиме расчета
#define RDS_BFM_WINDOWKEYUP            221 // Реакция на отпускание клавиши в режиме расчета

// Реакция на действия с окном подсистемы (вызывается у подсистемы и всех блоков в ее окне)
#define RDS_BFM_WINDOWOPERATION        230 // Открытие и закрытие окна подсистемы

// Реакции на сетевые события
#define RDS_BFM_NETCONNECT             300 // Соединение установлено
#define RDS_BFM_NETDISCONNECT          301 // Соединение разорвано
#define RDS_BFM_NETDATARECEIVED        302 // Получены данные
#define RDS_BFM_NETERROR               303 // Ошибка
#define RDS_BFM_NETDATAACCEPTED        304 // Данные блока получены сервером

// Реакции на запрос настроек параметров связи
#define RDS_BFM_SUPPORTSCONNSETUP      400 // Блок поддерживает настройку своей связи **** REV2 ****
#define RDS_BFM_CONNSETUP              401 // Вызов настройки связи **** REV2 ****
#define RDS_BFM_CONNHINT               402 // Запрос всплывающей подсказки связи **** REV2 ****
//---------------------------------------------------------------------------

// Возвращаемые функцией значения
#define RDS_BFR_DONE            0 // Нормальное завершение (для всех режимов)
#define RDS_BFR_ERROR           1 // Ошибка (для некоторых режимов)
#define RDS_BFR_NOTPROCESSED    1 // Отказ от обработки (для некоторых режимов), ==RDS_BFR_ERROR
#define RDS_BFR_STOP            1 // Прекращение дальнейшей обработки (для некоторых режимов), ==RDS_BFR_ERROR
#define RDS_BFR_BADVARSMSG      2 // Ошибка при вызове RDS_BFM_VARCHECK, но сообщение об ошибке
                                  // должна вывести основная программа.
#define RDS_BFR_MODIFIED        1 // Параметры изменены (для RDS_BFM_SETUP)
#define RDS_BFR_SHOWMENU        2 // При реакции на нажатие правой кнопки мыши - нажатие обработано,
                                  // нужно показать контекстное меню
#define RDS_BFR_RETRY           1 // Повтор операции (для RDS_BFM_LEAVINGEDITMODE)
#define RDS_BFR_ABORT           2 // Отмена операции (для RDS_BFM_LEAVINGEDITMODE)
//---------------------------------------------------------------------------

// Флаги мыши и клавиатуры
#define RDS_MLEFTBUTTON         1
#define RDS_MRIGHTBUTTON        2
#define RDS_MMIDDLEBUTTON       4
#define RDS_KSHIFT              8
#define RDS_KALT               16
#define RDS_KCTRL              32
#define RDS_MOUSEFLAGS         (RDS_MLEFTBUTTON|RDS_MRIGHTBUTTON|RDS_MMIDDLEBUTTON)
#define RDS_KBDFLAGS           (RDS_KSHIFT|RDS_KALT|RDS_KCTRL)
//---------------------------------------------------------------------------

// Уникальный идентификатор блока
typedef LPVOID RDS_BHANDLE;
// Уникальный идентификатор связи
typedef LPVOID RDS_CHANDLE;
// Уникальный идентификатор программируемого таймера
typedef LPVOID RDS_TIMERID;
// Уникальный идентификатор пункта меню "Система"
typedef LPVOID RDS_MENUITEM;
// Уникальный идентификатор переменной блока
typedef LPVOID RDS_VHANDLE;
// Уникальный идентификатор вспомогательного объекта
typedef LPVOID RDS_HOBJECT;
// Идентификатор сетевой станции (соединения)
typedef LPVOID RDS_NETSTATION;
// Идентификатор блока на сетевом соединении
typedef LPVOID RDS_NETBLOCK;
//---------------------------------------------------------------------------
// short int одним словом (для совместимости)
typedef short int RDS_SHORT;
//---------------------------------------------------------------------------
// Флаги блока (RDS_BLOCKDATA::Flags)
#define RDS_NEEDSDLLREDRAW    1  // Требуется перерисовка после завершения функции
                                 // отклика (игнорируется при вызове модели блока,
                                 // который не рисует себя сам, чтение/запись)
#define RDS_VARCHECKFAILED    2  // Предыдущий вызов RDS_BFM_VARCHECK вернул
                                 // флаг ошибки (только чтение)
#define RDS_MOUSECAPTURE      4  // Блок захватил "мышь" (чтение/запись)
#define RDS_NOWINREFRESH      8  // Перерисовка немодальных окон блока временно
                                 // запрещена (чтение/запись, управляется интерфейсной
                                 // функцией EnableWindowRefresh)
#define RDS_WINREFRESHWAITING 16 // Сразу после разрешения перерисовки немодальных окон
                                 // интерфейсной функцией EnableWindowRefresh необходимо
                                 // перерисовать одно из окон (чтение/запись)
#define RDS_DISABLED          32 // Блок не реагирует на действия пользователя - он не
                                 // может его выделить, отредактировать или удалить.
                                 // Блок изображается, но не реагирует на "мышь" и клавиатуру.
#define RDS_CTRLCALC          64 // Блок должен быть вызван перед каждым тактом расчета
                                 // в режиме RDS_BFM_PREMODEL
#define RDS_NOSUSPEND        128 // Вызовы блока в режимах RDS_BFM_MODEL и RDS_BFM_PREMODEL
                                 // не могут быть остановлены сервисной функцией
                                 // rdsSuspendCalc **** REV2 ****
#define RDS_SUSPENDED        256 // Была вызвана сервисная функция rdsSuspendCalc для остановки
                                 // вызовов этого блока и его соседей в режимах RDS_BFM_MODEL и
                                 // RDS_BFM_PREMODEL. При этом вызовы будут все равно продолжаться
                                 // несмотря на это, если взведен флаг RDS_NOSUSPEND **** REV2 ****
#define RDS_ALWAYSSETUP      512 // Вход в настройку блока разрешен, даже если проверка переменных
                                 // не прошла (установлен RDS_VARCHECKFAILED) **** REV2 ****
#define RDS_ALWAYSLOADSAVE  1024 // Сохранение и загрузка параметров блока разрешены, даже если проверка переменных
                                 // не прошла (установлен RDS_VARCHECKFAILED) **** REV2 ****
#define RDS_BLOCKHASHELP    2048 // У блока есть пункт "справка" в контекстном меню **** REV2 ****
#define RDS_INITCALC        4096 // Блок вызывается для инициализационного расчета при первом
                                 // выходе из редактирования при приходе данных от других блоков **** REV2 ****
#define RDS_INITCALCFIRST 0x2000 // Блок вызывается для инициализационного расчета при первом
                                 // выходе из редактирования в самом начале независимо от готовности **** REV2 ****
#define RDS_LOCKTEXTRECT  0x4000 // Изменение параметров арямоугольника с текстом во внешнем виде блока производится в
                                 // настройках самого блока (предупреждение пользователю в окне параметров, также
                                 // блокирует перевод текста на прямоугольнике) **** REV2 ****
//---------------------------------------------------------------------------
// Флаг-синоним для старых версий моделей
#define RDS_NEEDSREDRAW         RDS_NEEDSDLLREDRAW
//---------------------------------------------------------------------------

// Данные блока
typedef struct
{ LPVOID VarData;       // Адрес дерева данных блока (только чтение)
  LPVOID BlockData;     // Адрес области данных блока (чтение/запись)
  RDS_BHANDLE Block;    // Идентификатор блока (только чтение)
  LPSTR BlockName;      // Имя блока (только чтение)
  RDS_BHANDLE Parent;   // Идентификатор окружающей подсистемы (только чтение)
  DWORD Flags;          // Флаги (см. выше)
  // Размеры базового (заданного в режиме редактирования) описывающего прямоугольника.
  // Эти данные имеют смысл только если блок рисует себя функцией отклика.
  // Поскольку положение верхнего левого угла прямоугольника (т.е. точки привязки при
  // рисовании функцией) может быть связано с переменными, конкретное вычисленное
  // значение его координат передается в RDS_DRAWDATA и RDS_MOUSEDATA при соответствующих
  // вызовах.
  int Width,Height;
  // Целое пользовательское поле (не обрабатывается и не инициализируется)
  int Tag;
} RDS_BLOCKDATA;
typedef RDS_BLOCKDATA *RDS_PBLOCKDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая ТОЛЬКО при инициализационном расчете в режиме RDS_BFM_MODEL **** REV2 ****
// (при обычном расчете передается NULL)
typedef struct
{ BOOL FirstInChain;    // Этот блок начинает передачу из-за флага RDS_INITCALCFIRST

} RDS_INITIALCALCDATA;
typedef RDS_INITIALCALCDATA *RDS_PINITIALCALCDATA;
//---------------------------------------------------------------------------


// Структура, передаваемая вместе с мышиными сообщениями
// (RDS_BFM_MOUSE* и RDS_BFM_WINDOWMOUSE*)
typedef struct
{ int x,y;              // Координаты "мыши"
  int BlockX,BlockY;    // Координаты точки привязки блока (с учетом связи с переменными)
                        // (только для сообщений RDS_BFM_MOUSE*)
  int Left,Top;         // Верхний левый угол зоны блока (по последнему рисованию)
                        // (только для сообщений RDS_BFM_MOUSE*)
  int Width,Height;     // Размеры зоны блока (по последнему рисованию)
                        // (только для сообщений RDS_BFM_MOUSE*)
  int IntZoom;          // Масштаб в % (целый)
  DWORD Button;         // Кнопка - причина вызова (один из трех флагов "мыши" RDS_M*)
  DWORD Shift;          // Флаги мыши и клавиатуры (RDS_M*, RDS_K*)
  DWORD Reserved;       // Зарезервировано для выравнивания **** REV2 ****
  double DoubleZoom;    // Масштабный к-т (в долях единицы)

  int MouseEvent;       // Причина вызова - RDS_BFM_MOUSEDOWN, RDS_BFM_MOUSEUP,
                        // RDS_BFM_MOUSEMOVE или RDS_BFM_MOUSEDBLCLICK
                        // (при вызове с сообщением RDS_BFM_WINDOWMOUSE* сюда записывается
                        // соответствующая константа RDS_BFM_MOUSE*)
  int Viewport;         // Номер порта вывода, через которое пришло сообщение, или -1,
                        // если оно пришло от обычного окна подсистемы
} RDS_MOUSEDATA;
typedef RDS_MOUSEDATA *RDS_PMOUSEDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с клавиатурными сообщениями
// (RDS_BFM_KEY* и RDS_BFM_WINDOWKEY*)
typedef struct
{ int KeyCode;          // Код клавиши (виртуальный, см. WinAPI)
  BOOL Repeat;          // Для RDS_BFM_KEYDOWN: нажатие сгенерировано автоповтором
  int  RepeatCount;     // Для RDS_BFM_KEYDOWN при Repeat==TRUE: число повторенных
                        // с момента предыдущего вызова (обычно 1)
  DWORD Shift;          // Флаги клавиатуры (RDS_M*, RDS_K*)
  int KeyEvent;         // Причина вызова - RDS_BFM_KEYDOWN или RDS_BFM_KEYUP
                        // (при вызове с сообщением RDS_BFM_WINDOWKEY* сюда записывается
                        // соответствующая константа RDS_BFM_KEY*)
  //------- Возвращаемые параметры -------
  BOOL Handled;         // Устанавливается в TRUE, если остальные блоки (и системное
                        // меню в случае RDS_BFM_KEYDOWN) не должны реагировать на эту клавишу
  int Viewport;         // Номер порта вывода, через которое пришло сообщение, или -1,
                        // если оно пришло от обычного окна подсистемы
} RDS_KEYDATA;
typedef RDS_KEYDATA *RDS_PKEYDATA;
//---------------------------------------------------------------------------

// Действия, вызвавшие загрузку или запись данных блока
// (возвращается rdsGetSystemInt(RDS_GSISAVELOADACTION))
#define RDS_LS_ERROR            0       // В данный момент не производится загрузка или запись
#define RDS_LS_LOADROOT         1       // Загрузка корневого блока при загрузке системы
#define RDS_LS_SAVEROOT         2       // Запись корневого блока при записи системы
#define RDS_LS_LOADCONTENT      3       // Загрузка блока в составе загружаемой подсистемы
#define RDS_LS_SAVECONTENT      4       // Запись блока в составе записываемой подсистемы
#define RDS_LS_LOADCLIPBRD      5       // Загрузка блока при вставке из буфера обмена
#define RDS_LS_SAVECLIPBRD      6       // Запись блока при копировании в буфера обмена
#define RDS_LS_LOADFROMFILE     7       // Загрузка одиночного блока с диска или из библиотеки
#define RDS_LS_SAVETOFILE       8       // Запись одиночного блока на диск или в библиотеку
#define RDS_LS_LOADUNDO         9       // Загрузка блока из-за отмены изменений
#define RDS_LS_SAVEUNDO         10      // Запись блока в буфер для отмены изменений
#define RDS_LS_LOADAUTOCOMP     11      // Загрузка данных блока после автокомпиляции модели
#define RDS_LS_SAVEAUTOCOMP     12      // Запись данных блока перед автокомпиляцией модели
#define RDS_LS_LOADTAGGED       13      // Загрузка блока в спец.формате с тегами
#define RDS_LS_SAVETAGGED       14      // Запись блока в спец.формате с тегами
#define RDS_LS_LOADPROG         15      // Загрузка блока из-за вызова какой-либо сервисной функции **** REV2 ****
#define RDS_LS_SAVEPROG         16      // Запись блока из-за вызова какой-либо сервисной функции **** REV2 ****
//---------------------------------------------------------------------------

// Операция, вызвавшая загрузку или запись данных блока при сохранении/загрузке при отмене изменений
// (т.е. при rdsGetSystemInt(RDS_GSISAVELOADACTION)==RDS_LS_LOADUNDO или RDS_LS_SAVEUNDO
// (возвращается rdsGetSystemInt(RDS_GSIUNDOACTION))  **** REV2 ****
#define RDS_UA_ERROR            0       // В данный момент не производится загрузка или запись для отмены операции
#define RDS_UA_ADDBLOCK         1       // Добавление блока
#define RDS_UA_DELBLOCK         2       // Удаление блока
#define RDS_UA_EDITBLOCK        3       // Изменение различных параметров блока (вызова настройки не было)
#define RDS_UA_SETUPBLOCK       4       // Вызов настройки блока
#define RDS_UA_PROGUNDO         5       // Создание точки отмены для блока сервисной функцией
#define RDS_UA_TRANSLATE        6       // Реакция на перевод текстов
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщениями о рисовании (RDS_BFM_DRAW,RDS_BFM_DRAWADDITIONAL)
typedef struct
{ HDC dc;               // Где рисовать (контекст, см. WinAPI)
  BOOL CalcMode;        // TRUE - режим расчета, FALSE - режим редактирования
  int BlockX,BlockY;    // Координаты точки привязки блока (с учетом связи с переменными)
  double DoubleZoom;    // Масштабный к-т (в долях единицы)

  // Данные реального описывающего прямоугольника - заполняются процедурой рисования
  // для дальнейшего использования. Все координаты - абсолютные на рабочем поле с учетом
  // масштаба. Данные используются ТОЛЬКО в режиме расчета, в режиме редактирования
  // размеры прямоугольника берутся из RDS_BLOCKDATA::Width и RDS_BLOCKDATA::Height,
  // а верхний левый угол совпадает с точкой привязки.
  BOOL RectValid;       // Если процедура заполнила следующие далее данные, она должна
                        // установить эту переменную в TRUE. Иначе данные будут
                        // проигнорированы, и прямоугольник заполнится по RDS_BLOCKDATA.
  int Left,Top;         // Верхний левый угол зоны блока
  int Width,Height;     // Размеры зоны блока
  //---------

  RECT *VisibleRect;    // Реально отображаемый в окне прямоугольник (весь, только чтение)
  BOOL FullDraw;        // TRUE - нужно перерисовать весь блок, FALSE - можно перерисовать
                        // только изменившиеся с прошлого вызова части (при упрощенном
                        // режиме рисования)
} RDS_DRAWDATA;
typedef RDS_DRAWDATA *RDS_PDRAWDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщениями об изменении размера блока
// (RDS_BFM_RESIZE, RDS_BFM_RESIZING)
typedef struct
{ BOOL HorzResize;        // Идет изменение горизонтального размера
  BOOL VertResize;        // Идет изменение вертикального размера
  // HorzResize и VertResize устанавливаются в зависимости от того,
  // за какую из восьми меток масштабирования "тянет" пользователь.
  // При изменении размера через параметры блока обе эти переменные
  // равны FALSE.
  int newWidth,newHeight; // Новые значения ширины и высоты
  // Данные о сетке редактора
  int GridDx,GridDy;    // Размеры
  BOOL SnapToGrid;      // Включена ли привязка
} RDS_RESIZEDATA;
typedef RDS_RESIZEDATA *RDS_PRESIZEDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением о перемещении блока
// (RDS_BFM_MOVED)
typedef struct
{ int MoveReason;       // Причина перемещения (RDS_MR_*)
  int OldX,OldY;        // Старые координаты точки привязки
  int NewX,NewY;        // Новые координаты точки привязки
} RDS_MOVEDATA;
typedef RDS_MOVEDATA *RDS_PMOVEDATA;
// Причины перемещения
#define RDS_MR_SET      0       // Непосредственная установка координат (вставка из
                                // буфера обмена, сервисная функция и т.п)
#define RDS_MR_DRAG     1       // Перетаскивание в окне подсистемы
#define RDS_MR_KEYBOARD 2       // Перемещение с клавиатуры в окне подсистемы
#define RDS_MR_UNDOREDO 3       // Отмена или повтор действий пользователя по
                                // перемещению 
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщениями об запуске и остановке расчета
// (RDS_BFM_STARTCALC, RDS_BFM_STOPCALC)
// Все параметры - только чтение
typedef struct
{ BOOL FirstStart;      // TRUE  - расчет запущен с самого начала
                        // FALSE - расчет продолжен
  BOOL Loop;            // TRUE  - запуск неперерывного расчета
                        // FALSE - запуск на один такт
} RDS_STARTSTOPDATA;
typedef RDS_STARTSTOPDATA *RDS_PSTARTSTOPDATA;
//---------------------------------------------------------------------------

// Структура параметров вызова специальной (глобальной) функции блока
// (RDS_BFM_FUNCTIONCALL)
typedef struct
{ int Function;         // Номер функции
  LPVOID Data;          // Данные функции (не обрабатываются системой)

  int Reserved;         // Зарезервировано для дальнейшего использования
  RDS_BHANDLE Caller;   // Вызвавший блок
  BOOL Broadcast;       // TRUE - широковещательное сообщение, FALSE - личное
  int BroadcastCnt;     // При Broadcast==TRUE - счетчик с 0, указывающий на порядковый
                        // номер данного блока среди получивших сообщение
  BOOL Stop;            // При Broadcast==TRUE и соотв. параметрах функции rdsBroadcastFunctionCallsEx
                        // может быть установлена в блоком TRUE, после чего широковещательный вызов
                        // будет немедленно прекращен
  BOOL Delayed;         // TRUE - отложенный вызов, FALSE - прямой
  // Для отложенного вызова
  DWORD DataBufSize;    // Размер буфера, адрес которого передан через Data
} RDS_FUNCTIONCALLDATA;
typedef RDS_FUNCTIONCALLDATA *RDS_PFUNCTIONCALLDATA;
//---------------------------------------------------------------------------
// Сервисные макросы для приведения типа параметров функции
#define RDS_FUNCPARAMCAST(datastruct,type) ((type*)(((RDS_PFUNCTIONCALLDATA)(datastruct))->Data))
#define RDS_FUNCPARAMPVOID(datastruct) (((RDS_PFUNCTIONCALLDATA)(datastruct))->Data)
//---------------------------------------------------------------------------

// Структура параметров вызова внутренней функции блока (реакции на пункт
// меню, созданный самим блоком)
// (RDS_BFM_MENUFUNCTION)
typedef struct
{ int Function;         // Номер внутренней функции
  int MenuData;         // Данные функции (не обрабатываются системой)

} RDS_MENUFUNCDATA;
typedef RDS_MENUFUNCDATA *RDS_PMENUFUNCDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением о всплывающей подсказке
// (RDS_BFM_POPUPHINT)
typedef struct
{ int x,y;              // Координаты "мыши"
  int BlockX,BlockY;    // Координаты точки привязки блока (с учетом связи с переменными)
  // Описывающий прямоугольник блока
  int Left,Top;         // Верхний левый угол зоны блока (по последнему рисованию)
  int Width,Height;     // Размеры зоны блока (по последнему рисованию)
  //------- Возвращаемые параметры -------
  int HZLeft,HZTop,     // Размеры зоны действия подсказки (по умолчанию -
      HZWidth,HZHeight; // весь прямоугольник блока)
  int ReshowTimeout;    // Интервал (мс), через который нужно заново вывести подсказку
                        // (по умолчанию - 0, т.е. не выводить больше до покидания зоны HZ*)
  int HideTimeout;      // Интервал (мс), через который нужно погасить подсказку
                        // (по умолчанию - стандартное значение для Windows)
  //--------------------------------------
  // Масштаб (на всякий случай)
  int IntZoom;          // Масштаб в % (целый)
  DWORD Reserved;       // Зарезервировано для выравнивания **** REV2 ****
  double DoubleZoom;    // Масштабный к-т (в долях единицы)

} RDS_POPUPHINTDATA;
typedef RDS_POPUPHINTDATA *RDS_PPOPUPHINTDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением от дистанционного управления
// (RDS_BFM_REMOTEMSG)
typedef struct
{ LPSTR String;         // Переданная строка
  int   Value;          // Переданное число
  LPSTR ControllerName; // Имя программы дистанционного управления (если она
                        // его сообщила)
} RDS_REMOTEMSGDATA;
typedef RDS_REMOTEMSGDATA *RDS_PREMOTEMSGDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением о необходимости обновить окна
// (RDS_BFM_WINREFRESH)
typedef struct
{ RDS_TIMERID Timer;    // Таймер, вызвавший обновление, или NULL, если обновление
                        // вызвано сервисной функцией (только чтение)
  DWORD Reserved;       // Зарезервировано для выравнивания **** REV2 ****
  //------- Возвращаемый параметр -------
  double Delay;         // Время обновления окна, мс (запись, исходное -1.0)
} RDS_WINREFRESHDATA;
typedef RDS_WINREFRESHDATA *RDS_PWINREFRESHDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением о действиях с окном подсистемы
// (RDS_BFM_WINDOWOPERATION)
typedef struct
{ int Operation;        // Операция с окном
  HWND Handle;          // Handle окна
  BOOL EditMode;        // Включен режим редактирования
  BOOL Running;         // Идет моделирование (только при EditMode==FALSE)
  BOOL OwnWindow;       // Операция с окном данной подсистемы (TRUE) или родительской (FALSE)
} RDS_WINOPERATIONDATA;
typedef RDS_WINOPERATIONDATA *RDS_PWINOPERATIONDATA;
//---------------------------------------------------------------------------
// Операции с окном для RDS_WINOPERATIONDATA::Operation
#define RDS_SWO_OPEN    0       // Открытие окна
#define RDS_SWO_CLOSE   1       // Закрытие окна
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением об открытии контекстного меню
// (RDS_BFM_CONTEXTPOPUP)
// Все параметры - только чтение
typedef struct
{ BOOL EditMode;        // Включен режим редактирования
  BOOL FreeSpace;       // Меню открывается не для блока, а для свободного места подсистемы
                        // (только для подсистем)
} RDS_CONTEXTPOPUPDATA;
typedef RDS_CONTEXTPOPUPDATA *RDS_PCONTEXTPOPUPDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением о добавлении блока пользователем
// (RDS_BFM_MANUALINSERT)
typedef struct
{ int Reason;           // Добавление блока из буфера обмена (RDS_LS_LOADCLIPBRD) или
                        // из файла (RDS_LS_LOADFROMFILE)
  BOOL Single;          // Добавляется один блок (TRUE) или несколько (FALSE)
} RDS_MANUALINSERTDATA;
typedef RDS_MANUALINSERTDATA *RDS_PMANUALINSERTDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением об удалении блока пользователем
// (RDS_BFM_MANUALDELETE)
typedef struct
{ BOOL Single;          // Удаляется один блок (TRUE) или несколько (FALSE)
  BOOL WithSys;         // Удаляется внутри удаляемой подсистемы (TRUE) или
                        // самостоятельно (в составе группы выделенных блоков
                        // или один)
} RDS_MANUALDELETEDATA;
typedef RDS_MANUALDELETEDATA *RDS_PMANUALDELETEDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением об установлении или разрыве сетевого соединения
// (RDS_BFM_NETCONNECT, RDS_BFM_NETDISCONNECT)
typedef struct
{ int ConnId;           // Идентификатор соединения
  LPSTR Host;           // Адрес сервера (только для клиента)
  int Port;             // Порт
  LPSTR Channel;        // Имя канала
  BOOL ByServer;        // Для клиента при RDS_BFM_NETDISCONNECT - TRUE если соединение разорвано сервером

} RDS_NETCONNDATA;
typedef RDS_NETCONNDATA *RDS_PNETCONNDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением о принятых по сети данных
// (RDS_BFM_NETDATARECEIVED)
typedef struct
{ int ConnId;           // Идентификатор соединения
  LPSTR Host;           // Адрес сервера (только для клиента)
  int Port;             // Порт
  LPSTR Channel;        // Имя канала

  int Id;               // Принятое целое число
  LPSTR Str;            // Принятая строка (не может быть NULL)
  LPVOID Buffer;        // Принятый буфер или NULL
  DWORD BufferSize;     // Размер принятого буфера

  RDS_NETSTATION SenderStation; // Идентификатор машины-передатчика
  RDS_NETBLOCK SenderBlock;     // Идентификатор блока на этой машине
} RDS_NETRECEIVEDDATA;
typedef RDS_NETRECEIVEDDATA *RDS_PNETRECEIVEDDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением о сетевой ошибке
// (RDS_BFM_NETERROR)
typedef struct
{ int ConnId;           // Идентификатор соединения
  LPSTR Host;           // Адрес сервера (только для клиента)
  int Port;             // Порт
  LPSTR Channel;        // Имя канала

  int ErrorCode;        // Код ошибки (RDS_NETERR_*)

  RDS_NETSTATION Station; // Идентификатор машины (для соответствующих ошибок)
  RDS_NETBLOCK Block;     // Идентификатор блока (для соответствующих ошибок)
} RDS_NETERRORDATA;
typedef RDS_NETERRORDATA *RDS_PNETERRORDATA;
// Коды ошибок
#define RDS_NETERR_NOBLOCK      1       // Нет блока, которому были переданы данные
#define RDS_NETERR_GENERAL      2       // Неизвестная ошибка
#define RDS_NETERR_SEND         3       // Ошибка при передаче
#define RDS_NETERR_RECEIVE      4       // Ошибка при приеме
#define RDS_NETERR_DISCONNECT   5       // Ошибка при попытке разорвать связь
#define RDS_NETERR_ACCEPT       6       // Ошибка при установке соединения с клиентом
#define RDS_NETERR_CLIENTCONN   7       // Ошибка при установке связи (только для сервера)
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением о получении данных сервером
// (RDS_BFM_NETDATAACCEPTED)
typedef struct
{ int ConnId;           // Идентификатор соединения
  LPSTR Host;           // Адрес сервера (только для клиента)
  int Port;             // Порт
  LPSTR Channel;        // Имя канала

  int Id;               // Id отправленных данных
} RDS_NETACCEPTDATA;
typedef RDS_NETACCEPTDATA *RDS_PNETACCEPTDATA;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщениями о настройке присоединенной связи **** REV2 ****
// (RDS_BFM_SUPPORTSCONNSETUP, RDS_BFM_CONNSETUP и RDS_BFM_CONNHINT)
typedef struct
{ RDS_CHANDLE Conn;     // Идентификатор связи
  BOOL Output;          // TRUE - связь подключена к выходу блока, FALSE - ко входу
  LPSTR VarName;        // Полное имя подключенной переменной
  int VarType;          // Односимвольный тип переменной (RDS_VARTYPE_*)
  int VarNum;           // Номер переменной в блоке

  // Только для RDS_BFM_CONNHINT (возвращаемые параметры)
  int ReshowTimeout;    // Интервал (мс), через который нужно заново вывести подсказку
                        // (по умолчанию - 0, т.е. не выводить больше до покидания зоны HZ*)
  int HideTimeout;      // Интервал (мс), через который нужно погасить подсказку
                        // (по умолчанию - стандартное значение для Windows)
} RDS_CONNSETUPDATA;
typedef RDS_CONNSETUPDATA *RDS_PCONNSETUPDATA;
//---------------------------------------------------------------------------


// Структура описания панели блока в окне подсистемы
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  RDS_HOBJECT Object;   // Объект, связанный с этой панелью
  RDS_BHANDLE Block;    // Блок-владелец панели
  int PLeft,PTop;       // Левый верхний угол панели в системных координатах (масштаб 100%)
  int PWidth,PHeight;   // Размер панели в системных координатах (масштаб 100%)
  int Order;            // Номер панели в данном блоке
  BOOL Visible;         // Видимость панели
  HWND Handle;          // Handle оконного объекта панели или NULL
  int Width,Height;     // Размеры оконного объекта панели (при Handle!=NULL)
  BOOL Border;          // Наличие рамки вокруг оконного объекта
  BOOL CloseButton;     // Наличие кнопки закрытия панели
  BOOL Scalable;        // Панель меняет размер при изменении масштаба системы
  BOOL Sizeable;        // Размер панели может изменяться пользователем
  BOOL Moveable;        // Положение панели может изменяться пользователем
  BOOL CaptionBar;      // Есть полоса заголовка окна (за которую можно перетаскивать)
  LPSTR Caption;        // Текущий заголовок (изменять в этой структуре нельзя!)
} RDS_PANDESCRIPTION;
typedef RDS_PANDESCRIPTION *RDS_PPANDESCRIPTION;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением от панели блока
// (RDS_BFM_BLOCKPANEL)
typedef struct
{ int Operation;                // Действие с панелью (одна из констант RDS_PANOP_*)
  RDS_PPANDESCRIPTION Panel;    // Описание панели (указатель на структуру, существующую
                                // только в момент вызова сообщения)
} RDS_PANOPERATION;
typedef RDS_PANOPERATION *RDS_PPANOPERATION;
// Действия
#define RDS_PANOP_CREATE        0       // Создание оконного объекта для панели
#define RDS_PANOP_DESTROY       1       // Уничтожение оконного объекта для панели
#define RDS_PANOP_RESIZED       2       // Размер панели изменен
#define RDS_PANOP_MOVED         3       // Панель перемещена пользователем
#define RDS_PANOP_PAINT         4       // Необходимо перерисовать панель
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением об изменениях в подсистеме **** REV2 ****
// (RDS_BFM_CONTENTCHANGE)
typedef struct
{ RDS_BHANDLE Changed;  // Подсистема (возможно, вложенная), в которой произошли изменения
  BOOL Deep;            // Изменения не в самой подсистеме, а во вложенной (Changed!=Parent)
} RDS_CONTENTCHANGE;
typedef RDS_CONTENTCHANGE *RDS_PCONTENTCHANGE;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением об изменении размеров рабочего поля подсистемы **** REV2 ****
// (RDS_BFM_SYSWORKSIZECHANGE)
typedef struct
{ RDS_BHANDLE Sys;        // Подсистема
  int OldWidth,OldHeight; // Старые размеры
  int NewWidth,NewHeight; // Старые размеры
} RDS_SYSWORKWIDTHCHANGE;
typedef RDS_SYSWORKWIDTHCHANGE *RDS_PSYSWORKWIDTHCHANGE;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением об изменении конфигурации слоев подсистемы **** REV2 ****
// (RDS_BFM_LAYERCONFIGCHANGE)
typedef struct
{ RDS_BHANDLE Sys;      // Подсистема
  BOOL OwnConfig;       // TRUE  - реакция самой подсистемы на изменение конфигурации в ней
                        // FALSE - реакция блока на изменение конфигурации в родительской подсистеме
  LPSTR ConfigName;     // Имя установленной конфигурации
  int ConfigNum;        // Номер установленной конфигурации
} RDS_LAYERCONFIGCHANGE;
typedef RDS_LAYERCONFIGCHANGE *RDS_PLAYERCONFIGCHANGE;
//---------------------------------------------------------------------------

// Структура, передаваемая вместе с сообщением о записи или загрузке файла переводов **** REV2 ****
// (RDS_BFM_TRANSLATE)
typedef struct
{ BOOL Save;       // TRUE  - идет запись файла переводов
                   // FALSE - идет загрузка переводов
} RDS_TRANSLATEDATA;
typedef RDS_TRANSLATEDATA *RDS_PTRANSLATEDATA;
//---------------------------------------------------------------------------

// Функция вызова блока - тип указателя
typedef int(RDSCALL *RDS_BLOCKENTRYPOINT)(int,RDS_PBLOCKDATA,LPVOID);
//---------------------------------------------------------------------------

//======================= Вспомогательные описания ============================
// Описания, необходимые для вызова некоторых интерфейсных функций

// Указатель на COLORREF
typedef COLORREF *RDS_PCOLORREF;
//---------------------------------------------------------------------------

// Типы блоков и связей, используемые интерфейсными функциями
#define RDS_TUNKNOWN            0       // Непонятно что (при ошибке в функции)
// Блоки
#define RDS_BTSYSTEM            1       // Подсистема
#define RDS_BTSIMPLEBLOCK       2       // Блок с моделью DLL (простой)
#define RDS_BTINPUTBLOCK        4       // Внешний вход
#define RDS_BTOUTPUTBLOCK       8       // Внешний выход
#define RDS_BTBUSPORT           64      // Ввод шины
#define RDS_BTDLLBLOCK          RDS_BTSIMPLEBLOCK // Синоним (для совместимости)
// Связи
#define RDS_CTCONNECTION        16      // Простая связь
#define RDS_CTBUS               32      // Шина
//---------------------------------------------------------------------------
// Все типы блоков
#define RDS_BTALLTYPES  (RDS_BTSYSTEM | RDS_BTSIMPLEBLOCK | RDS_BTINPUTBLOCK | RDS_BTOUTPUTBLOCK | RDS_BTBUSPORT)
// Все типы связей
#define RDS_CTALLTYPES  (RDS_CTCONNECTION | RDS_CTBUS)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Инициализация поля servSize в структурах, передаваемых в RDS для заполнения
#define RDS_INITSERVSIZE(s)     s.servSize=sizeof(s)
//---------------------------------------------------------------------------

// Структура описания блока, заполняемая функцией rdsGetBlockDescription
// Запрещается изменять строки, адреса которых содержатся в структуре
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  RDS_BHANDLE Block;    // Идентификатор блока
  int BlockType;        // Тип блока (см. выше)
  LPSTR BlockName;      // Имя блока
  RDS_BHANDLE Parent;   // Идентификатор окружающей подсистемы
  LPSTR ParentName;     // Имя подсистемы
  LPSTR BlockComment;   // Комментарий блока
  int LayerId;          // Идентификатор слоя
  LPSTR DllFile;        // Имя модуля DLL или NULL
  LPSTR DllFunc;        // Имя функции DLL или NULL
  HINSTANCE Module;     // Загруженный модуль DLL или NULL
  LPSTR ParentVar;      // Имя переменной подсистемы, которая соответствует этому
                        // блоку (только для внешнего входа и выхода) или NULL
  BOOL Selected;        // Блок выделен в редакторе (только в режиме редактирования)
  int NumberOfVars;     // Число переменных блока
  DWORD Flags;          // Флаги описания (RDS_BDF_*)
  DWORD ExtId;          // Внешний уникальный идентификатор (для приложений, управляющих
                        // RDS снаружи)
  int NamePos;          // Положение имени блока (RDS_BDNP_*)
  int NameDx,NameDy;    // Смещение левого верхнего угла имени от точки привязки блока
                        // (в масштабе 100%)
  int AltNameAlignment; // Выравнивание текста, выводимого вместо имени (если он есть):
                        // -1 (влево), 0 (по центру) или 1 (вправо)

} RDS_BLOCKDESCRIPTION;
typedef RDS_BLOCKDESCRIPTION *RDS_PBLOCKDESCRIPTION;
// Флаги описания
#define RDS_BDF_RUNEVERYCYCLE   1       // Запуск каждый такт
#define RDS_BDF_NAMEOFF         2       // Запрещение вывода имени блока
#define RDS_BDF_SHOWMAINPOINT   4       // Показывать в редакторе точку привязки
#define RDS_BDF_SELFDRAW        8       // Рисование производится функцией DLL
#define RDS_BDF_MOUSEEVENTS     0x10    // Функция блока получает сообщения от "мыши"
#define RDS_BDF_FREEMOUSEMOVE   0x20    // Функция блока получает сообщения о перемещении "мыши"
                                        // с отпущенными кнопками
#define RDS_BDF_ALLOWRESIZE     0x40    // Разрешение масштабирования "мышью"
#define RDS_BDF_LOCKWIDTH       0x80    // Фиксировать ширину при масштабировании (при RDS_BDF_ALLOWRESIZE)
#define RDS_BDF_LOCKHEIGHT      0x100   // Фиксировать высоту при масштабировании (при RDS_BDF_ALLOWRESIZE)
#define RDS_BDF_SETUPFUNC       0x200   // Есть функция настройки
#define RDS_BDF_SETUPBYDCLICK   0x400   // Функция настройки по двойному щелчку
#define RDS_BDF_POPUPHINT       0x800   // Есть реакция на Hint
#define RDS_BDF_KBDEVENTS       0x1000  // Получает сообщения от клавиатуры
#define RDS_BDF_HASPICTURE      0x2000  // Есть картинка
#define RDS_BDF_TEXTRECT        0x4000  // Блок изображается прямоугольником с текстом
#define RDS_BDF_ALLOWMOVE       0x8000  // Блок может перемещаться пользователем **** REV2 ****
// Положение имени блока
#define RDS_BDNP_BELOW          0       // Под блоком
#define RDS_BDNP_ABOVE          1       // Над блоком
#define RDS_BDNP_CUSTOM         2       // Произвольное (перемещено пользователем вручную)
//---------------------------------------------------------------------------

// Структура описания размеров описывающего прямоугольника блока
// и точки привязки (заполняется функцией rdsGetBlockDimensions)
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  int BlockX,BlockY;    // Положение точки привязки на рабочем поле

  int Left,Top;         // Верхний левый угол зоны блока на рабочем поле
  int Width,Height;     // Размеры зоны блока на рабочем поле
} RDS_BLOCKDIMENSIONS;
typedef RDS_BLOCKDIMENSIONS *RDS_PBLOCKDIMENSIONS;
//---------------------------------------------------------------------------
// Флаги для rdsGetBlockDimensionsEx
#define RDS_GBD_NONE            0       // Нет флагов (для лучшей читаемости программы)
#define RDS_GBD_USEZOOM         1       // Использовать масштаб родительской подсистемы
#define RDS_GBD_USEVARS         2       // В режимах моделирования и расчета учитывать связь с переменными
//---------------------------------------------------------------------------

// Структура описания связи, заполняемая функцией rdsGetConnDescription
// Запрещается изменять строки, адреса которых содержатся в структуре
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  RDS_CHANDLE Conn;     // Идентификатор связи
  int ConnType;         // Тип связи (см. выше)
  RDS_BHANDLE Parent;   // Идентификатор окружающей подсистемы
  LPSTR ParentName;     // Имя подсистемы
  BOOL Active;          // Состояние связи (включена/отключена)
  int LayerId;          // Идентификатор слоя
  int NumPoints;        // Число точек в связи
  LPSTR BusName;        // Имя (только для шины)
  int NumChannels;      // Число каналов (только для шины)
  int NumLines;         // Число линий, соединяющих точки на изображении связи
  DWORD ExtId;          // Внешний уникальный идентификатор (для приложений, управляющих
                        // RDS снаружи)
  BOOL Selected;        // Связь выделена в редакторе (только в режиме редактирования) **** REV2 ****
  LPSTR Text;           // Дополнительный текст или NULL, если его нет **** REV2 ****
  BOOL ShowText;        // Показывать дополнительный текст рядом со связью **** REV2 ****
  int TextX,TextY;      // Абсолютные координаты центра дополнительного текста, если он есть **** REV2 ****
  int TextAlignment;    // Выравнивание текста (-1 - влево, 0 - по центру, 1 - вправо) **** REV2 ****
  BOOL TextTranslate;   // Необходимость перевода текста **** REV2 ****
} RDS_CONNDESCRIPTION;
typedef RDS_CONNDESCRIPTION *RDS_PCONNDESCRIPTION;
//---------------------------------------------------------------------------

// Структура описания внешнего вида связи, используемая функциями
// rdsGetConnAppearance, rdsSetConnAppearance
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  COLORREF LineColor;   // Цвет линии
  int LineWidth;        // Толщина линии
  int LineStyle;        // Стиль линии (WinAPI)
  int ArrowLength;      // Длина стрелки (0...255)
  int ArrowWidth;       // Выступ стрелки (0...255)
  int DotSize;          // Диаметр точки соединения
} RDS_CONNAPPEARANCE;
typedef RDS_CONNAPPEARANCE *RDS_PCONNAPPEARANCE;
//---------------------------------------------------------------------------

// Типы точек связи, используемые интерфейсными функциями
#define RDS_PTINTERNAL          1       // Внутренняя точка
#define RDS_PTBLOCK             2       // Соединение с блоком
#define RDS_PTBUS               4       // Соединение с шиной
#define RDS_PTBUSTOBLOCK        8       // Соединение шины с блоком
// Состояния точек
#define RDS_PSNORMAL            0       // Точка в порядке
#define RDS_PSBADVAR            1       // Нет такой переменной
#define RDS_PSBADTYPE           2       // Несоответствие типов
// Структура описания точки связи, заполняемая функцией rdsGetPointDescription
// Запрещается изменять строки, адреса которых содержатся в структуре
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  int PointType;        // Тип точки (см. выше)
  BOOL Source;          // TRUE - источник данных, FALSE - получатель
                        // (не используется для RDS_PTINTERNAL)
  int Status;           // Состояние (RDS_PS******)
  LPSTR ObjectName;     // Имя блока или шины (кроме RDS_PTINTERNAL)
  LPSTR VarName;        // Имя переменной (кроме RDS_PTINTERNAL)
  RDS_BHANDLE Block;    // Присоединенный блок для PointType==RDS_PTBLOCK или NULL
  RDS_CHANDLE Bus;      // Присоединенная шина для PointType==RDS_PTBUS или NULL

  RDS_CHANDLE Owner;    // Связь-владелец точки
  int x,y;              // Координаты точки (для RDS_PTBLOCK - относительные)
  int PointNum;         // Номер точки в связи

  char VarType;         // Односимвольный тип переменной (RDS_VARTYPE_*, кроме RDS_PTINTERNAL) **** REV2 ****
  BYTE Res1,Res2,Res3;  // Зарезервировано для выравнивания **** REV2 ****
  int VarNum;           // Номер переменной в блоке (только для RDS_PTBLOCK) или -1 **** REV2 ****
} RDS_POINTDESCRIPTION;
typedef RDS_POINTDESCRIPTION *RDS_PPOINTDESCRIPTION;
//---------------------------------------------------------------------------

// Типы линий связи, используемые интерфейсными функциями
#define RDS_LNLINE      1       // Прямая линия
#define RDS_LNBEZIER    2       // Кривая
// Структура описания линии связи, заполняемая функцией GetLineDescription
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  int LineType;         // Тип линии (RDS_LN*)
  int nPoint1,nPoint2;  // Номера соединяемых точек

  int x1,y1;            // Абсолютные координаты точки nPoint1
  int x2,y2;            // Абсолютные координаты точки nPoint2

  int dx1,dy1;          // Смещения управляющей точки для nPoint1 при LineType==RDS_LNBEZIER
  int dx2,dy2;          // Смещения управляющей точки для nPoint2 при LineType==RDS_LNBEZIER

  RDS_CHANDLE Owner;    // Связь-владелец линии
} RDS_LINEDESCRIPTION;
typedef RDS_LINEDESCRIPTION *RDS_PLINEDESCRIPTION;
//---------------------------------------------------------------------------

// Флаги переменных блока, используемые интерфейсными функциями
#define RDS_VARFLAG_INPUT       1       // Вход (не употребляется вместе с RDS_VARFALG_OUTPUT)
#define RDS_VARFLAG_OUTPUT      2       // Выход (не употребляется вместе с RDS_VARFALG_INPUT)
#define RDS_VARFLAG_RUN         4       // При срабатывании входа запускается модель
#define RDS_VARFLAG_MENU        8       // Присутствует в меню присоединения входов/выходов
#define RDS_VARFLAG_SHOWNAME    16      // Вывод имени разрешен по умолчанию
#define RDS_VARFLAG_ONEINDEX    32      // Одноиндексный массив (только для массивов)
// Дополнительные флаги - не описывают переменные, а используются при их создании
// интерфейсными функциями
#define RDS_VARFLAG_EXT_CHGNAME 0x10000 // Изменить имя создаваемой переменной, если такая
                                        // уже есть

// Структура описания переменной блока
// Запрещается изменять строки, адреса которых содержатся в структуре
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  char Type;            // Односимвольный тип переменной (RDS_VARTYPE_*)
  BYTE Res1,Res2,Res3;  // Зарезервировано для выравнивания **** REV2 ****
  DWORD Flags;          // Набор битовых флагов (RDS_VARFLAG_*)
  LPSTR Name;           // Имя переменной
  LPSTR StructType;     // Имя типа (только для структур, для остальных - NULL)
  int StructFields;     // Число полей (только для структур)
  int DataSize;         // Размер блока данных
  RDS_VHANDLE Var;      // Уникальный идентификатор
  int Rank;             // Уровень переменной
  LPSTR Remark;         // Комментарий переменной (пустая строка, если его нет) **** REV2 ****
} RDS_VARDESCRIPTION;
typedef RDS_VARDESCRIPTION *RDS_PVARDESCRIPTION;
//---------------------------------------------------------------------------

// Структура описания параметров редактора подсистемы, заполняемая функцией
// rdsGetEditorParameters
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  int GridDx,GridDy;    // Шаг сетки по горизонтали и вертикали
  BOOL SnapToGrid;      // Включена ли привязка к сетке
  BOOL DisplayGrid;     // Изображается ли сетка
  COLORREF GridColor;   // Цвет точек сетки

  BOOL Visible;         // Окно редактора открыто
  int WinLeft,WinTop,WinWidth,WinHeight; // Размеры и положение окна
  int WorkWidth,WorkHeight;   // Размеры рабочей области
  int ScrollX,ScrollY;  // Позиция ScrollBar'ов
  DWORD Reserved;       // Зарезервировано для выравнивания **** REV2 ****
  double Zoom;          // Масштаб

  COLORREF MainPointColor; // Цвет точек привязки блоков
  BOOL ShowBlockNames;  // Показывать имена блоков
  BOOL ShowVarNames;    // Показывать имена переменных

  int NumLayers;        // Число слоев
  int CurLayerNum;      // Номер текущего слоя (0...NumLayers-1)
  int CurLayerId;       // Id текущего слоя
  int NumConfigs;       // Чило конфигураций слоев
  int CurConfig;        // Номер текущей конфигурации (0...NumConfigs-1)

  BOOL PrintZoneActive; // Зона печати включена
  BOOL DisplayPrintZone;// Зона печати отображается
  int PZLeft,PZTop,PZWidth,PZHeight; // Размеры и положение зоны печати

  BOOL WinMaximized;    // Окно развернуто на весь экран
  BOOL WinMinimized;    // Окно свернуто

  DWORD RefreshDelay;   // Задержка автоматического обновления окна в режиме расчета,
                        // или 0, если обновление отключено

  COLORREF BlockNameColor;      // Цвет имен блоков
  int BlockNameDistance;        // Расстояние от блока до имени по умолчанию

  COLORREF BackgroundColor;     // Цвет фона окна
  BOOL DefBackground;           // В качестве цвета фона окна выбран цвет окна Windows

  DWORD WindowReactions;        // Битовые флаги реакции окна на "мышь" и клавиатуру, не перехваченных блоками:
                                // RDS_BDF_MOUSEEVENTS, RDS_BDF_FREEMOUSEMOVE, RDS_BDF_KBDEVENTS
  BOOL Wallpaper;       // Включены обои
  BOOL WallpaperTile;   // Обоями заполнено все рабочее поле
  int WallpaperWidth,WallpaperHeight; // Размеры обоев

  BOOL Dashboard;       // В системе только неподвижные неперекрывающиеся блоки (оптимизация обновления)

} RDS_EDITORPARAMETERS;
typedef RDS_EDITORPARAMETERS *RDS_PEDITORPARAMETERS;
//---------------------------------------------------------------------------

// Константы для функции rdsGetEditorFont
#define RDS_GEF_BLOCKNAME       0       // Шрифт имен блоков
#define RDS_GEF_VARNAME         1       // Шрифт имен переменных

// Структура описания панелей редактора подсистемы, используемая функциями
// rdsGetEditorToolBars и rdsSetEditorToolBars
// Эти вызовы устарели, вместо них следует использовать
// rdsGetEditorWindowFlags и rdsSetEditorWindowFlags
typedef struct
{ BOOL LayersBar;       // Видимость панели слоя и конфигурации
  BOOL ZoomBar;         // Видимость панели масштаба
  BOOL DisplayBar;      // Видимость панели изображения (сетка и т.п)
  BOOL PrintBar;        // Видимость панели печати
  BOOL StatusBar;       // Видимость строки состояния
} RDS_EDITORTOOLBARS;
typedef RDS_EDITORTOOLBARS *RDS_PEDITORTOOLBARS;
//---------------------------------------------------------------------------

// Режимы работы программируемых таймеров
#define RDS_TIMERM_DELETE       0       // Удалить таймер после срабатывания
#define RDS_TIMERM_STOP         1       // Таймер останавливается, но не удаляется
#define RDS_TIMERM_LOOP         2       // Таймер автоматически взводится заново
// Способы срабатывания таймеров
#define RDS_TIMERS_SIGNAL    0x10       // При срабатывании взводится сигнал запуска
#define RDS_TIMERS_TIMER     0x20       // При срабатывании вызвать блок с RDS_BFM_TIMER
#define RDS_TIMERS_WINREF    0x30       // При срабатывании вызвать блок с RDS_BFM_WINREFRESH
#define RDS_TIMERS_SYSTIMER  0x40       // При срабатывании вызвать блок с RDS_BFM_TIMER
                                        // даже в том случае, если процесс моделирования
                                        // остановлен или включен режим редактирования
// Флаги режимов
#define RDS_TIMERF_FIXFREQ   0x100      // Для RDS_BFM_WINREFRESH: не применять подстройку
                                        // частоты к этому таймеру
// Маски таймеров
#define RDS_TIMERMASK_M      0x0f       // Режим работы
#define RDS_TIMERMASK_S      0xf0       // Способ срабатывания
#define RDS_TIMERMASK_F    0xff00       // Флаги

// Структура параметров программируемого таймера
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  DWORD Delay;          // Задержка таймера (мс)
  DWORD StartTime;      // Время начала отсчета (мс)
  BOOL On;              // Таймер включен
  DWORD Mode;           // Режим (RDS_TIMERM_* | RDS_TIMERS_* | RDS_TIMERF_*)
} RDS_TIMERDESCRIPTION;
typedef RDS_TIMERDESCRIPTION *RDS_PTIMERDESCRIPTION;
//---------------------------------------------------------------------------

// Псевдоидентификаторы блоков для функций работы с динамическими переменными
#define RDS_DVSELF      0       // Переменная в собственном блоке
#define RDS_DVPARENT    1       // Переменная в родительской подсистеме
#define RDS_DVROOT      2       // Переменная в главной подсистеме
// Структура параметров подписки на динамическую переменную
typedef struct
{ LPVOID Data;          // Адрес области данных переменной или NULL
  LPSTR  VarName;       // Имя переменной (только чтение)
  LPSTR  VarType;       // Тип переменной (только чтение)
  RDS_BHANDLE Provider; // Блок, которому принадлежит данная переменная
  LPVOID UID;           // Уникальный идентификатор переменной в блоке Provider (служебная,
                        // изменять нельзя)
  RDS_VHANDLE Var;      // Идентификатор переменной для использования в сервисных функциях
} RDS_DYNVARLINK;
typedef RDS_DYNVARLINK *RDS_PDYNVARLINK;
//---------------------------------------------------------------------------

// Структура параметров подписки на функцию блока
typedef struct
{ RDS_BHANDLE Block;    // Блок-провайдер
  int FuncId;           // Функция
} RDS_FUNCPROVIDERLINK;
typedef RDS_FUNCPROVIDERLINK *RDS_PFUNCPROVIDERLINK;
// Макрос: удачность подписки
#define RDS_FUNCPROVIDERLINK_SUCCESS(pLink) ( ((pLink)!=NULL) && (((RDS_PFUNCPROVIDERLINK)(pLink))->Block!=NULL) )
//---------------------------------------------------------------------------

// Псевдоидентификаторы блоков для функций работы с разделяемой памятью **** REV2 ****
#define RDS_SMSELF      0       // Переменная в собственном блоке
#define RDS_SMPARENT    1       // Переменная в родительской подсистеме
#define RDS_SMROOT      2       // Переменная в главной подсистеме
// Структура параметров разделяемой блоками области памяти **** REV2 ****
typedef struct
{ BOOL RdsVar;          // TRUE - переменная RDS, FALSE - просто область памяти
  LPVOID Data;          // Указатель на область памяти ИЛИ адрес области данных переменной RDS
  LPSTR Name;           // Имя области памяти (только чтение)
  LPSTR VarType;        // Тип переменной RDS (для переменных RDS, только чтение)
  RDS_VHANDLE Var;      // Идентификатор переменной для использования в сервисных функциях
  DWORD Size;           // Размер области памяти (только для простых областей памяти)
  RDS_BHANDLE Block;    // Блок, в котором создана область памяти/переменная

} RDS_SHAREDMEMPARAMS;
typedef RDS_SHAREDMEMPARAMS *RDS_PSHAREDMEMPARAMS;
//---------------------------------------------------------------------------

// Структура описания шрифта (используется в некоторых функциях)
#define RDS_SERVFONTPARAMSNAMESIZE      256     // Размер массива под имя шрифта
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  char Name[RDS_SERVFONTPARAMSNAMESIZE];        // Имя шрифта
  int CharSet;          // Набор символов
  int Height;           // Высота (в пикселах)
  int Size;             // Высота (в типографских точках)
  BOOL SizePriority;    // Только при установке шрифта по этой структуре:
                        // TRUE - использовать Size, FALSE - использовать Height
  COLORREF Color;       // Цвет шрифта
  BOOL Bold;            // Жирный
  BOOL Italic;          // Курсив
  BOOL Underline;       // Подчеркнутый
  BOOL StrikeOut;       // Зачеркнутый
} RDS_SERVFONTPARAMS;
typedef RDS_SERVFONTPARAMS *RDS_PSERVFONTPARAMS;
//---------------------------------------------------------------------------

// Маски для графических сервисных функций
#define RDS_GFALL               0    // Все параметры (при этом для шрифта используется RDS_GFSIZE)
#define RDS_GFSTYLE             1    // Стиль линии или заливки
#define RDS_GFWIDTH             2    // Толщина линии
#define RDS_GFCOLOR             4    // Цвет (линии, шрифта, заливки)
#define RDS_GFMODE              8    // Режим рисования линии (WinAPI SetROP2)
// Далее - только для шрифта
#define RDS_GFNAME             16    // Имя шрифта
#define RDS_GFSIZE             32    // Размер шрифта в усл.ед (points)
#define RDS_GFHEIGHT           64    // Расмер шрифта в точках
#define RDS_GFCHARSET         128    // Набор символов
#define RDS_GFESCAPEMENT      256    // Угол поворота (в градусах)
#define RDS_GFBOLD           1024    // Полужирный
#define RDS_GFITALIC         2048    // Курсив
#define RDS_GFUNDERLINE      4096    // Подчеркнутый
#define RDS_GFSTRIKEOUT      8192    // Зачеркнутый
#define RDS_GFESCAPEMENT10  16384    // Угол поворота (в десятых долях градуса) **** REV2 ****
#define RDS_GFFONTSTYLES     (1024+2048+4096+8192) // Все стили
#define RDS_GFFONTALLHEIGHT  (4+16+64+128+256+1024+2048+4096+8192) // Все параметры и высота
                                                                   // GF_HEIGHT
#define RDS_GFFONTALLHE10    (4+16+64+128+16384+1024+2048+4096+8192) // Все параметры, высота
                                                                   // GF_HEIGHT и угол в десятых градуса **** REV2 ****
#define RDS_GFFONTBASIC      (4+16+64+128+1024+2048+4096+8192) // Все стили и высота без поворота

// Стили заливки для графических сервисных функций
#define RDS_GFS_EMPTY           0    // Нет заливки
#define RDS_GFS_SOLID           1    // Сплошная
#define RDS_GFS_BDIAGONAL       2    // 45-degree downward left-to-right hatch
#define RDS_GFS_CROSS           3    // Horizontal and vertical crosshatch
#define RDS_GFS_DIAGCROSS       4    // 45-degree crosshatch
#define RDS_GFS_FDIAGONAL       5    // 45-degree upward left-to-right hatch
#define RDS_GFS_HORIZONTAL      6    // Horizontal hatch
#define RDS_GFS_VERTICAL        7    // Vertical hatch

// Константы, используемые функцией rdsAltConnAppearanceOp
#define RDS_CAOSET              0    // Установить параметры альтернативного вида связи
#define RDS_CAOCOUNT            1    // Вернуть число альтернативных видов
#define RDS_CAOPREALLOCATE      2    // Отвести массив заданного размера
#define RDS_CAOGET              3    // Получить параметры альтернативного вида
#define RDS_CAOSETCURRENT       4    // Установить текущий вид по альтернативному
#define RDS_CAODELETE           5    // Удалить альтернативный вид
#define RDS_CAORESTORE          6    // Восстановить вид, заданный исходно   

// Константы, используемые функцией rdsGetSystemPath
#define RDS_GSPSYSTEMFILE       0    // Загруженный файл системы (с путем, если схема не
                                     // только что создана)
#define RDS_GSPAPPPATH          1    // Путь к EXE-файлу RDS без имени файла
#define RDS_GSPDLLPATH          2    // Директория стандартных Dll
#define RDS_GSPAPPEXE           3    // Путь к EXE-файлу RDS с именем файла
#define RDS_GSPTEMPLATEPATH     4    // Путь к шаблонам схем
#define RDS_GSPBLOCKLIBPATH     5    // Путь к библиотеке блоков
#define RDS_GSPBLOCKPANELPATH   6    // Путь к панели блоков
#define RDS_GSPTEMPPATH         7    // Путь к временной директории
#define RDS_GSPINCLUDEPATH      8    // Путь к директории заголовочных файлов
#define RDS_GSPSYSTEMFULLPATH   9    // Загруженный файл схемы с путем. Если пути еще нет
                                     // (только что создана), возвращается пустая строка
#define RDS_GSPINIPATH         10    // Путь к директории ini-файлов (обычно совпадает с RDS_GSPAPPPATH)
#define RDS_GSPDEFAULTHOST     11    // Имя сервера по умолчанию для сетевых соединений
#define RDS_GSPMODELSPATH      12    // Директория для автокомпилируемых моделей
#define RDS_GSPBAKFILEEXT      13    // Расширение файлов резервных копий схем
#define RDS_GSPDOCPATH         14    // Директория документации **** REV2 ****
#define RDS_GSPSYSTEMFULLDIR   15    // Директория загруженного файла схемы или пустая строка **** REV2 ****

// Константы, используемые функцией rdsGetSystemInt
#define RDS_GSICMDPARAMCOUNT    0    // Число необработанных параметров командной строки
#define RDS_GSIINSTSTRUCTCOUNT  1    // Число структур в общем списке
#define RDS_GSISAVELOADACTION   2    // Действие, вызвавшее загрузку или запись блока
                                     // (одна из констант RDS_LS_*)
#define RDS_GSIMODIFIED         3    // 1 - система изменена, 0 - не изменена
#define RDS_GSITICKPARITY       4    // 0 - четный такт, 1 - нечетный
#define RDS_GSIDEFAULTPORT      5    // Порт по умолчанию для сетевых соединений
#define RDS_GSIUNDOSIZE         6    // Число шагов undo (0 - выключено)
#define RDS_GSIUIENABLED        7    // Разрешен ли пользовательский интерфейс (0 - весь RDS спрятан,
                                     // работа через окно вывода в другом приложении)
#define RDS_GSISTOPPING         8    // 1 - в данный момент останавливается расчет, иначе - 0
#define RDS_GSITRACELEVEL       9    // Уровень ведения отладочного журнала (0 - отключен)
#define RDS_GSIBAKFILESCOUNT   10    // Число создаваемых при сохранении схемы резервных копий
#define RDS_GSIMODE            11    // Текущий режим RDS (одна из констант RDS_CM_*) **** REV2 ****
#define RDS_GSIMAINHELP        12    // Наличие главного файла справки RDS (1 - да,0 - нет) **** REV2 ****
#define RDS_GSIINVALIDPATHS    13    // При rdsGetSystemInt(RDS_GSISAVELOADACTION)==RDS_LS_LOADPROG -
                                     // следует ли игнорировать или как-то корректировать пути в параметрах блоков - например, если
                                     // данные для загрузки пришли по сети (1-игнорировать,0-нет) **** REV2 ****
#define RDS_GSIUNDOACTION      14    // действие undo, вызвавшее загрузку или сохранение блока (RDS_UA_*) **** REV2 ****

// Режим RDS, возвращаемый rdsGetSystemInt(RDS_GSIMODE)  **** REV2 ****
#define RDS_CM_EDIT     0       // Режим редактирования
#define RDS_CM_CALC     1       // Режим моделирования
#define RDS_CM_RUN      2       // Режим расчета

// Константы, используемые функцией rdsSetSystemInt
#define RDS_SSIFASTTEXTSAVE     0    // Быстрое сохранение в текстовом виде (без доп.обработки)
                                     // Вызывается в реакции RDS_BFM_SAVETXT, потом автоматически сбрасывается)
#define RDS_SSIWAITCURSOR       1    // Установить вид курсора "песочные часы" или вернуть нормальный
#define RDS_SSILEAVEEDITSTATUS  2    // Установить необходимость повторить/отменить выход из режима редактирования **** REV2 ****
#define RDS_SSICANCELUNDOSTORE  3    // Не записывать операцию, выполняющуюся сейчас, в undo **** REV2 ****

// Константы, используемые функцией rdsBlockStateOp
#define RDS_BSOSAVE             0    // Запомнить состояние
#define RDS_BSOCOUNT            1    // Вернуть число сохраненных состояний
#define RDS_BSORESTORE          2    // Восстановить состояние
#define RDS_BSODELETE           3    // Удалить состояние

// Типы потоков, возвращаемые функцией rdsCallerThreadType
#define RDS_THREADMAIN  0       // Главный поток
#define RDS_THREADAUX   1       // Вспомогательный поток моделирования

// Сервисные макросы для установки и проверки DWORD-флагов
#define RDS_GETFLAG(storage,mask) (((storage) & (mask))!=0)
#define RDS_SETFLAG(storage,mask,value) ((storage) = (value)?((storage) | (mask)):((DWORD)((storage) & (~(mask)))))
//---------------------------------------------------------------------------

// Сервисные макросы для работы с переменными-массивами
// Проверить существование массива
//   baseaddr - базовый адрес массива (void*)
#define RDS_ARRAYEXISTS(baseaddr) (*((void**)(baseaddr))!=NULL)
// Число строк (первый индекс) массива (массив должен существовать)
#define RDS_ARRAYROWS(baseaddr)  (*(*((int**)(baseaddr))))
// Число столбцов (второй индекс) массива (массив должен существовать)
#define RDS_ARRAYCOLS(baseaddr)  ( *( *((int**)(baseaddr))+1 ) )
// Начало области данных массива (массив должен существовать)
// Область данных: M[r,c]
//                 r0c0   r0c1   r0c2   ... r0cN   r1c0   r1c1   ...
//                 M[0,0] M[0,1] M[0,2] ... M[0,N] M[1,0] M[1,1] ...
#define RDS_ARRAYDATA(baseaddr)  ( *((int**)(baseaddr))+2 )
// Структура для быстрого доступа к массиву
typedef struct
{ BOOL Exists;          // TRUE - массив существует (т.е. не 0 x 0)
  int Rows,Cols;        // Размерность массива
  int ItemSize;         // Размер элемента массива
  LPVOID Data;          // Начало области данных массива
} RDS_ARRAYACCESSDATA;
typedef RDS_ARRAYACCESSDATA *RDS_PARRAYACCESSDATA;
// Адрес элемента массива по структуре доступа (массив должен существовать)
//   acc - RDS_PARRAYACCESSDATA массива
//   r,c - первый и второй индексы
#define RDS_ARRAYITEMADDR(acc,r,c) ( ((char*)((acc)->Data)) + ((r)*((acc)->Cols)+(c))*((acc)->ItemSize) )
// То же для одномерного массива
#define RDS_ARRAYITEMPLAINADDR(acc,n) ( ((char*)((acc)->Data)) + (n)*((acc)->ItemSize) )
// Элемент массива по структуре доступа
// type - тип элемента массива (например, double)
#define RDS_ARRAYITEM(type,acc,r,c) ( *((type *)RDS_ARRAYITEMADDR(acc,r,c)) )
//---------------------------------------------------------------------------

// Флаги для функции rdsEnumConnectedBlocks
#define RDS_BEN_INPUTS          1       // Просматривать входы блока
#define RDS_BEN_OUTPUTS         2       // Просматривать выходы блока
#define RDS_BEN_TRACELINKS      4       // Прослеживать связи внутрь и наружу подсистем до простых
                                        // блоков
#define RDS_BEN_USEDISABLED     8       // Если связь отключена, все равно обрабатывать ее **** REV2 ****
//---------------------------------------------------------------------------

// Флаги, используемые функциями rdsBroadcastFunctionCallsEx, rdsBroadcastFuncCallsDelayed
#define RDS_BCALL_SUBSYSTEMS    1    // Вызывать все вложенные блоки всех вложенных подсистем
#define RDS_BCALL_CHECKSUPPORT  2    // Вызывать функцию только если выполнится RDS_BFM_CHECKFUNCSUPPORT
#define RDS_BCALL_ALLOWSTOP     4    // Разрешить блоку прекратить дальнейшие вызовы
#define RDS_BCALL_FIRST         8    // При отложенном вызове помещается в начало очереди
#define RDS_BCALL_LAST          0    // При отложенном вызове помещается в конец очереди (введен для
                                     // улучшения читаемости, можно не указывать)
//---------------------------------------------------------------------------

// Флаги для функции rdsSetPointPosition
#define RDS_SPP_RELATIVE        1       // Задаются координаты относительно точки привязки
#define RDS_SPP_REFRESH         2       // Обновить окно после установки координат точки
//---------------------------------------------------------------------------

// Однобайтовые обозначения типов переменных (из них формируется строка для RDS_BFM_VARCHECK)
// Простые
#define RDS_VARTYPE_SIGNAL      'S'     // Сигнал (1 байт - char)
#define RDS_VARTYPE_LOGICAL     'L'     // Логическая (1 байт - char)
#define RDS_VARTYPE_CHAR        'C'     // Целый (1 байт - char)
#define RDS_VARTYPE_SHORT       'H'     // Целый (2 байта - short int)
#define RDS_VARTYPE_INT         'I'     // Целый (4 байта - int)
#define RDS_VARTYPE_FLOAT       'F'     // С плавающей точкой (4 байта - float)
#define RDS_VARTYPE_DOUBLE      'D'     // С плавающей точкой (8 байт - double)
// Сложные
#define RDS_VARTYPE_STRING      'A'     // Строка
#define RDS_VARTYPE_RUNTIME     'V'     // Изменяемый тип
#define RDS_VARTYPE_ARRAY       'M'     // Массив/матрица
#define RDS_VARTYPE_STRUCT      '{'     // Структура
#define RDS_VARTYPE_STRUCTEND   '}'     // Конец структуры
//---------------------------------------------------------------------------

// Флаги для функции rdsRegisterMenuItem, rdsChangeMenuItem
#define RDS_MENU_SHORTCUT       1       // Есть "горячая клавиша"
#define RDS_MENU_UNIQUECAPTION  2       // Не регистрировать, если уже есть пункт меню
                                        // с таким названием
#define RDS_MENU_CHECKED        4       // Есть "галочка"
#define RDS_MENU_DISABLED       8       // Пункт меню запрещен
#define RDS_MENU_HIDDEN        16       // Пункт меню невидим
#define RDS_MENU_DIVIDER       32       // Разделитель вместо пункта меню (только для контекстного,
                                        // не употребляется с другими флагами)
#define RDS_MENU_SEARCH        64       // Вызывается по Ctrl+F вместо стандартного поиска **** REV2 ****
//---------------------------------------------------------------------------

// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsCECreateEditor (объект для редактирования связи или шины)
#define RDS_HCE_RESET           0       // Очистить редактор
// Коды ошибок функции rdsCECreateConnBus
#define RDS_HCE_ERR_OK          0       // Нет ошибок
#define RDS_HCE_ERR_BADOBJECT   1       // Параметр функции не является объектом rdsCECreateEditor
#define RDS_HCE_ERR_INVBLKBUS   2       // Один из блоков или одна из шин или связей отсутствует
#define RDS_HCE_ERR_BADLINE     3       // Одна из линий ссылается на несуществующую точку
#define RDS_HCE_ERR_ALLOC       4       // Ошибка отведения памяти
//---------------------------------------------------------------------------

// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsBCLCreateList (список блоков и связей)
// Команды получения массивов (функция rdsGetObjectArray, макросы rdsBCLGetBlockArray и rdsBCLGetConnArray)
#define RDS_HBCL_BLOCKARRAY     0       // Операция с массивом блоков
#define RDS_HBCL_CONNARRAY      1       // Операция с массивом связей и шин
// Команды установки и получения целых и логических параметров (функции rdsSetObjectInt, rdsGetObjectInt)
#define RDS_HBCL_AUTODELETE     0       // Автоматически заносить в элемент массива
                                        // NULL, если блок или связь были удалены
#define RDS_HBCL_BLOCKCOUNT     1       // Число блоков в списке (только получение)
#define RDS_HBCL_CONNCOUNT      2       // Число связей в списке (только получение)
// Команды объекту
#define RDS_HBCL_CLEAR          3       // Очистить список
// Флаги вызова функции групповой установки
#define RDS_GS_DISABLEVARCHANGE         1       // Запрет изменения переменных блока
#define RDS_GS_DISABLEVARVALUES         2       // Запрет присвоения переменным
#define RDS_GS_DISABLEEDITORPARAMS      4       // Запрет изменения параметров редактора
#define RDS_GS_DISABLEREMARKS           8       // Запрет изменения комментариев блоков
#define RDS_GS_DISABLELAYERCHANGE      16       // Запрет изменения слоя
#define RDS_GS_DISABLEDBLCLICK         32       // Запрет изменения реакции на двойной щелчок
#define RDS_GS_DISABLEPICTURE          64       // Запрет изменения картинки и ее параметров
#define RDS_GS_DISABLEDRAWTYPE        128       // Запрет изменения типа рисования (DLL/картинка)
#define RDS_GS_DISABLEBLOCKAPPEARANCE 256       // Запрет изменения параметров внешнего вида
                                                // (изображение имени, точки привязки и т.п.)
#define RDS_GS_DISABLESIZING          512       // Запрет изменения параметров масштабирования
#define RDS_GS_DISABLECONNAPPEARANCE 1024       // Запрет изменения внешнего вида связи
#define RDS_GS_DISABLECONNSTATE      2048       // Запрет включения/выключения связи
#define RDS_GS_DISABLEBUSPACK        4096       // Запрет удаления пустых каналов шин
#define RDS_GS_DISABLEDLLFUN         8192       // Запрет изменения функции отклика
#define RDS_GS_DISABLEDLLOPTIONS    16384       // Запрет изменения флагов реакции функции (всех)
#define RDS_GS_DISABLEAUTOCOMP      32768       // Запрет изменения параметров автокомпиляции
#define RDS_GS_DISABLEMOVING        65536       // Запрет изменения параметров перемещения **** REV2 ****
#define RDS_GS_DISABLEEXTCONNPARAMS 0x20000     // Запрет изменения дополнительных параметров связи **** REV2 ****
#define RDS_GS_DISABLECLASSES       0x40000     // Запрет изменения классов **** REV2 ****
//---------------------------------------------------------------------------

// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsVSCreateEditor (набор переменных)
// Флаги редактора
#define RDS_HVAR_FSTRUCT        1       // Разрешено использование структур
#define RDS_HVAR_FARRAYS        2       // Разрешено использование массивов и матриц
#define RDS_HVAR_F1INDEX        4       // Разрешено использование массивов с одним индексом
#define RDS_HVAR_FRUNTIME       8       // Разрешено использование изменяемых типов
#define RDS_HVAR_FARROFSTRUCT  16       // Разрешено использование массивов структур
#define RDS_HVAR_FINT          32       // Разрешено использование int
#define RDS_HVAR_FDOUBLE       64       // Разрешено использование double
#define RDS_HVAR_FCHAR        128       // Разрешено использование char
#define RDS_HVAR_FSHORT       256       // Разрешено использование short int
#define RDS_HVAR_FFLOAT       512       // Разрешено использование float
#define RDS_HVAR_FLOGICAL    1024       // Разрешено использование однобайтовых логических
#define RDS_HVAR_FSIGNAL     2048       // Разрешено использование сигналов
#define RDS_HVAR_FSTRING     4096       // Разрешено использование строк
#define RDS_HVAR_FNOSTRUCTNAME 8192     // Для редактирования структуры - не показывать имя
#define RDS_HVAR_FNOOFFSET  16384       // Не показывать колонку смещения
#define RDS_HVAR_FNOREMARKS 32768       // Запретить комментарии переменных **** REV2 ****
#define RDS_HVAR_FALLPLAIN   (32+64+128+256+512+1024+2048) // Все простые типы
#define RDS_HVAR_FALLPLAINNS (32+64+128+256+512+1024) // Все простые типы без сигналов
#define RDS_HVAR_FALL        (1+2+4+8+16+RDS_HVAR_FALLPLAIN+4096) // Все типы
#define RDS_HVAR_FALLNS      (1+2+4+8+16+RDS_HVAR_FALLPLAINNS+4096) // Все типы без сигналов
// Результаты
#define RDS_HVAR_ROK            0       // Операция выполнена успешно
#define RDS_HVAR_ROKRENAMED     1       // Операция выполнена успешно, но структура переименована
#define RDS_HVAR_RNOTYPENAME    2       // Для операции требуется, чтобы у структуры
                                        // было имя типа
#define RDS_HVAR_RVARCHECKERR   3       // Блок не может работать с такой структурой
#define RDS_HVAR_REMPTYVARSET   4       // Набор переменных пуст
#define RDS_HVAR_RBADBLOCKTYPE  5       // Недопустимый тип блока
#define RDS_HVAR_RNOBLKSIGNALS  6       // Набор переменных не может быть назначен блоку т.к.
                                        // в нем нет двух сигналов в начале
// Команды (возврат целого числа)
#define RDS_HVAR_GETFIELDCOUNT  0       // Получить число полей структуры
#define RDS_HVAR_GETAUTOCOUNT   1       // Получить число автоматических соединений
#define RDS_HVAR_GETVARRANK     2       // Получить уровень переменной
#define RDS_HVAR_GETVARFLAGS    12      // Получить флаги переменной
// Команды (возврат строки)
#define RDS_HVAR_GETTYPENAME    3       // Получить строку типа структуры
#define RDS_HVAR_GETAUTOMAIN    4       // Получить имя главной переменной автосоединения
                                        // с указанным номером
#define RDS_HVAR_GETAUTOCONN    5       // Получить имя связанной переменной автосоединения
                                        // с указанным номером
#define RDS_HVAR_GETTYPESTRING  13      // Получить строку типа всей структуры                                        
#define RDS_HVAR_GETREMARK      14      // Получить комментарий переменной с указанным номером **** REV2 ****
// Команды (установка целого числа)
#define RDS_HVAR_SETVARFLAGS    RDS_HVAR_GETVARFLAGS    // Установить флаги переменной
// Команды (установка строки)
#define RDS_HVAR_SETTYPENAME    6       // Установить строку типа структуры
#define RDS_HVAR_SETREMARK      15      // Установить комментарий переменной с указанным номером **** REV2 ****
// Команды (без возврата)
#define RDS_HVAR_RESET          7       // Очистить редактор
#define RDS_HVAR_DELVAR         8       // Удалить переменную
#define RDS_HVAR_DELAUTO        9       // Удалить автосоединение
#define RDS_HVAR_CLEARTYPEREN   10      // Очистить список переименований типов
#define RDS_HVAR_CLEARENAMES    RDS_HVAR_CLEARTYPEREN      // Синоним
#define RDS_HVAR_CLEARVARREN    11      // Очистить список переименований переменных
//---------------------------------------------------------------------------

// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsSTRCreateTextReader (простой анализ слов текста)
#define RDS_HSTR_DEFUNKNOWNWORD         -1      // Идентификатор неопознанного слова по умолчанию
#define RDS_HSTR_DEFENDOFLINE           -2      // Идентификатор конца строки по умолчанию
#define RDS_HSTR_DEFENDOFTEXT           -3      // Идентификатор конца текста по умолчанию
// Команды установки и получения целых и логических параметров (функции rdsSetObjectInt, rdsGetObjectInt)
#define RDS_HSTR_IGNORECASE     0       // Учитывать (1) или не учитывать (0) регистр при анализе
#define RDS_HSTR_UNKNOWNID      1       // Идентификатор неопознанного слова
#define RDS_HSTR_ENDOFLINEID    2       // Идентификатор конца строки
#define RDS_HSTR_ENDOFTEXTID    3       // Идентификатор конца текста
#define RDS_HSTR_READINT        7       // Считать из текста целое число (только получение)
// Команды установки строки
#define RDS_HSTR_SETTEXT        4       // Установить текст для анализа
// Команды получения строки
#define RDS_HSTR_GETLASTWORD    5       // Получить последнее считанное слово
#define RDS_HSTR_GETRESTOFTEXT  6       // Получить указатель на остаток текста после чтения
// Командв плучения вещественных чисел
#define RDS_HSTR_READDOUBLE     8       // Считать из текста число double

// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsINICreateTextHolder (разбор данных формата INI-файла)
// Команды
#define RDS_HINI_RESET          0       // Очистка текста
#define RDS_HINI_GETLASTERROR   4       // Получить флаг ошибки последней операции
#define RDS_HINI_SAVEBLOCKTEXT  7       // Записать как текстовый формат блока
// Команды установки строки
#define RDS_HINI_SETTEXT        1       // Установить текст
#define RDS_HINI_LOADFILE       2       // Загрузить файл
#define RDS_HINI_SAVEFILE       3       // Записать файл
#define RDS_HINI_CREATESECTION  5       // Создать секцию
#define RDS_HINI_DELETESECTION  6       // Удалить секцию
#define RDS_HINI_DELETEKEYLAST  8       // Удалить значение в последней открытой секции

// Результат возврата некоторых функций, действие которых может быть отложено
#define RDS_FRESULT_ERROR       0       // Ошибка
#define RDS_FRESULT_OK          1       // Функция выполнена
#define RDS_FRESULT_DELAYED     2       // Функция отложена

// Флаги, используемые функциями rdsGetEditorWindowFlags и rdsSetEditorWindowFlags
#define RDS_EWF_STATUSBAR       1       // Есть строка состояния
#define RDS_EWF_LAYERSTOOLBAR   2       // Панель инструментов - слои
#define RDS_EWF_ZOOMTOOLBAR     4       // Панель инструментов - масштаб
#define RDS_EWF_DISPLAYTOOLBAR  8       // Панель инструментов - изображение
#define RDS_EWF_CALCTOOLBAR    16       // Панель инструментов - расчет и режимы
#define RDS_EWF_PRINTTOOLBAR   32       // Панель инструментов - печать
#define RDS_EWF_NOSCROLLBARS   64       // Нет полос прокрутки **** REV2 ****
#define RDS_EWF_WINMOUSE      128       // Окно реагирует на "мышь" после блоков **** REV2 ****
#define RDS_EWF_WINFREEMOUSE  256       // Окно реагирует на перемещения "мыши" **** REV2 ****
                                        // с отпущенными кнопками (после блоков)
#define RDS_EWF_WINKEYBOARD   512       // Окно реагирует на клавиатуру после блоков **** REV2 ****
// Маски и наборы флагов для тех же функций
#define RDS_EWF_ALLBARS        (1+2+4+8+16+32)  // Все панели и строка состояния

// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsFORMCreate (объект для создания модального окна)
// Команды
#define RDS_FORM_CLEAR          0       // Очистить
#define RDS_FORM_SHOWMODAL      1       // Показать
#define RDS_FORM_INVALIDATE     2       // Перерисовать
#define RDS_FORM_CLOSE          3       // Закрыть (как "OK" или "Отмена") **** REV2 ****
#define RDS_FORM_TEMPCLOSE      4       // Закрыть, выполнить реакцию и открыть снова **** REV2 ****
// Типы полей ввода (младший байт DWORD, передаваемого в rdsFORMAddEdit)
#define RDS_FORMCTRL_EDIT       0       // Простое поле ввода
#define RDS_FORMCTRL_COMBOEDIT  1       // Поле ввода со списком вариантов
#define RDS_FORMCTRL_COMBOLIST  2       // Список вариантов без возможности редактирования
#define RDS_FORMCTRL_CHECKBOX   3       // Флаг (не может сочетаться с RDS_FORMFLAG_CHECK)
#define RDS_FORMCTRL_LABEL      4       // Надпись без возможности редактирования
#define RDS_FORMCTRL_COLOR      5       // Кнопка выбора цвета
#define RDS_FORMCTRL_OPENDIALOG 6       // Поле ввода для выбора файла (открытие, с диалогом)
#define RDS_FORMCTRL_SAVEDIALOG 7       // Поле ввода для выбора файла (сохранение, с диалогом)
#define RDS_FORMCTRL_UPDOWN     8       // Поле ввода со стрелками
#define RDS_FORMCTRL_DISPLAY    9       // Индикация значения без возможности редактирования
#define RDS_FORMCTRL_MULTILINE  10      // Поле ввода из нескольких строк
#define RDS_FORMCTRL_LISTANDEDIT 11     // Выпадающий список и поле ввода
#define RDS_FORMCTRL_RANGEEDIT  12      // Два поля ввода, разделенных "..."
#define RDS_FORMCTRL_NONVISUAL  13      // Просто хранимые данные (не отображаются в окне)
#define RDS_FORMCTRL_PAINTBOX   14      // Область, рисуемая служебной функцией
#define RDS_FORMCTRL_RADIOBUTTON 15     // Флаг, исключающий другие (не может сочетаться с RDS_FORMFLAG_CHECK)
#define RDS_FORMCTRL_BUTTON     16      // Кнопка
#define RDS_FORMCTRL_FONTSELECT 17      // Кнопка выбора шрифта
#define RDS_FORMCTRL_HOTKEY     18      // Поле ввода для сочетания клавиш
#define RDS_FORMCTRL_DIRDIALOG  19      // Поле ввода для выбора папки (с диалогом)
#define RDS_FORMCTRL_MULTILABEL 20      // Многострочная надпись без возможности редактирования **** REV2 ****
#define RDS_FORMCTRL_TABLE      21      // Таблица **** REV2 ****
// Флаги полей (старшая часть DWORD, передаваемого в rdsFORMAddEdit)
#define RDS_FORMFLAG_LINE       0x100   // Рельефная линия под полем
#define RDS_FORMFLAG_CHECK      0x200   // С разрешающим/запрещающим флагом
#define RDS_FORMFLAG_DISABLED   0x400   // Поле ввода запрещено целиком (вместе с флагом, если он есть)
#define RDS_FORMFLAG_LRIGHT     0x800   // Метка поля ввода выровнена по правому краю
#define RDS_FORMFLAG_LCENTER    0x1000  // Метка поля ввода выровнена по центру
#define RDS_FORMFLAG_CHECKRADIO 0x2000  // Разрешающий/запрещающий флаг - RadioButton (при RDS_FORMFLAG_CHECK)
// Типы передаваемых и получаемых значений для функций rdsSetObject* и rdsGetObject* или
// заменяющих их макросов rdsFORMSet* и rdsFORMGet*.
#define RDS_FORMVAL_VALUE       0       // Значение органа управления (установка и чтение)
#define RDS_FORMVAL_LIST        1       // Список вариантов для RDS_FORMCTRL_COMBOEDIT и
                                        // RDS_FORMCTRL_COMBOLIST или список фильтров для
                                        // RDS_FORMCTRL_OPENDIALOG/RDS_FORMCTRL_SAVEDIALOG (только установка)
#define RDS_FORMVAL_CHECK       2       // Значение разрешающего/запрещающего флага (установка и чтение)
#define RDS_FORMVAL_UPDOWNINC   3       // Изменение для RDS_FORMCTRL_UPDOWN (установка и чтение)
#define RDS_FORMVAL_UPDOWNMIN   4       // Минимум для RDS_FORMCTRL_UPDOWN (установка и чтение)
#define RDS_FORMVAL_UPDOWNMAX   5       // Максимум для RDS_FORMCTRL_UPDOWN (установка и чтение)
#define RDS_FORMVAL_MLHEIGHT    6       // Высота для RDS_FORMCTRL_MULTILINE и RDS_FORMCTRL_MULTILABEL **** REV2 **** (только установка)
#define RDS_FORMVAL_ENABLED     7       // Общая разрешенность органа управления (установка и чтение)
                                        //-------- Для RDS_FORMCTRL_LISTANDEDIT ----------
#define RDS_FORMVAL_AUXLISTITEM 8       // Номер пункта вспомогательного списка для RDS_FORMCTRL_LISTANDEDIT(установка и чтение)
#define RDS_FORMVAL_AUXLISTWIDTH 9      // Ширина вспомогательного списка для RDS_FORMCTRL_LISTANDEDIT(только установка)
#define RDS_FORMVAL_2NDEDITENABLED 10   // Разрешенность второго поля ввода для RDS_FORMCTRL_LISTANDEDIT
#define RDS_FORMVAL_ITEMINDEX   11      // Номер выбранного пункта для RDS_FORMCTRL_COMBOLIST (даже если запрашивается строка,
                                        // возвращается символьное представление номера, а не название выбранного пункта)
                                        //-------- Для RDS_FORMCTRL_PAINTBOX ----------
#define RDS_FORMVAL_PBHEIGHT    RDS_FORMVAL_MLHEIGHT       // Высота для RDS_FORMCTRL_PAINTBOX (только установка)
#define RDS_FORMVAL_PBBEVEL     RDS_FORMVAL_AUXLISTWIDTH   // Наличие рамки для RDS_FORMCTRL_PAINTBOX (только установка)
                                        //-------- Для RDS_FORMCTRL_RANGEEDIT ----------
#define RDS_FORMVAL_RANGEMAX    RDS_FORMVAL_UPDOWNMAX  // Максимум для RDS_FORMCTRL_RANGEEDIT (установка и чтение)
                                        //-------- Для RDS_FORMCTRL_HOTKEY ----------
#define RDS_FORMVAL_HKSHIFTS    RDS_FORMVAL_MLHEIGHT    // Флаги Ctrl,Shift,Alt
                                        //-------- RDS_FORMCTRL_MULTILINE ---------
#define RDS_FORMVAL_MLRETURNS   RDS_FORMVAL_AUXLISTITEM // Разрешение переводов строк в поле
#define RDS_FORMVAL_MLREADONLY  RDS_FORMVAL_AUXLISTWIDTH // Только чтение **** REV2 ****
#define RDS_FORMVAL_MLBEVEL     RDS_FORMVAL_2NDEDITENABLED // Вид рамки (0,1,2 - только установка) **** REV2 ****
                                        //--------  RDS_FORMCTRL_MULTILABEL ---------  **** REV2 ****
#define RDS_FORMVAL_MLABELBEVEL  RDS_FORMVAL_AUXLISTWIDTH   // Наличие рамки для RDS_FORMCTRL_MULTILABEL (только установка)  **** REV2 ****
                                        //--------  Файловые диалоги ---------  **** REV2 ****
#define RDS_FORMVAL_DEFPATH     12      // Путь по умолчанию для файловых диалогов **** REV2 ****
                                        //--------  RDS_FORMCTRL_TABLE ---------  **** REV2 ****
                                        // (rdsSetObjectInt и, кое-где, rdsSetObjectStr)
#define RDS_FORMVAL_TABLEHEIGHT  13     // Высота всей таблицы (только установка)
#define RDS_FORMVAL_TBLBTN_ADD   14     // Видимость (rdsSetObjectInt) или подсказка (rdsSetObjectStr) кнопки "добавить" (только установка)
#define RDS_FORMVAL_TBLBTN_UP    15     // Видимость или подсказка кнопки "вверх" (видимость автоматически включает и "вниз", только установка)
#define RDS_FORMVAL_TBLBTN_DOWN  16     // Видимость или подсказка кнопки "вниз" (видимость автоматически включает и "вверх", только установка)
#define RDS_FORMVAL_TBLBTN_PROP  17     // Видимость или подсказка кнопки "свойства" (только установка)
#define RDS_FORMVAL_TBLBTN_DEL   18     // Видимость или подсказка кнопки "удалить" (только установка)
#define RDS_FORMVAL_TBLHEADERS   19     // Наличие/отсутствие строки заголовка (установка/чтение)
#define RDS_FORMVAL_TBLFIXEDCOL  20     // Наличие/отсутствие фиксированного нулевого столбца (установка/чтение)
#define RDS_FORMVAL_TBLHDRHEIGHT 21     // Высота строки заголовка (в точках экрана, установка/чтение)
#define RDS_FORMVAL_TBLLINEHEIGHT 22    // Высота строк данных (в точках экрана, установка/чтение)
#define RDS_FORMVAL_TBLCOUNT     23     // Число строк (только чтение)
#define RDS_FORMVAL_TBLDELLINE   24     // Удалить строку (только установка)
#define RDS_FORMVAL_TBLEMPTYTEXT 25     // Текст, выводимый для пустой таблицы вместо сетки
#define RDS_FORMVAL_TBLREDRAW    26     // Перерисовать таблицу (независимо от значения)
#define RDS_FORMVAL_TBLINSLINE   27     // Вставить строку (только установка)

// События для универсальной служебной функции модального окна
#define RDS_FORMSERVEVENT_CHANGE        0       // Изменение одного или нескольких полей ввода
#define RDS_FORMSERVEVENT_DRAW          1       // Рисование (панели или ячейки таблицы)
#define RDS_FORMSERVEVENT_CLICK         2       // Нажатие кнопки
#define RDS_FORMSERVEVENT_HELP          3       // Нажатие кнопки справки  **** REV2 ****
#define RDS_FORMSERVEVENT_TCREACTION    4       // Реакция после временного закрытия  **** REV2 ****
#define RDS_FORMSERVEVENT_TCREOPEN      5       // Открытие после временного закрытия  **** REV2 ****
#define RDS_FORMSERVEVENT_TBLBUTTON     6       // Нажатие кнопки таблицы  **** REV2 ****
#define RDS_FORMSERVEVENT_TBLBTNENABLE  7       // Проверка разрешенности кнопок добавления и удаления в таблице **** REV2 ****
// Флаги для rdsFORMHelpButton  **** REV2 ****
#define RDS_FORMHELP_UI         1       // Справка через файл UI (сброшен - указывается сам файл справки)
#define RDS_FORMHELP_EVENT      2       // Создавать событие в функции обратного вызова вместо показа справки
#define RDS_FORMHELP_CHECK      4       // Проверить существование файла справки (если не через событие)
// Структура для универсальной служебной функции модального окна
typedef struct
{ int Event;            // Событие (RDS_FORMSERVEVENT_*)
  int CtrlId;           // Идентификатор органа управления (если требуется для данного события)

  // Для RDS_FORMSERVEVENT_DRAW
  HDC dc;               // Где рисовать (контекст, см. WinAPI)
  int Left,Top;         // Верхний левый угол зоны рисования
  int Width,Height;     // Размеры зоны рисования

  // Дополнительно для рисования таблицы  **** REV2 ****
  int TableRow,TableCol;// Ячейка таблицы, если идет рисование таблицы
                        // (для строки заголовка TableRow=-1), TableRow также используется в реакциях на кнопки таблицы
  int ColId;            // Идентификатор столбца таблицы (по заголовку, только при рисовании таблицы)
  LPSTR CellValue;      // Значение ячейки таблицы (только при рисовании таблицы)
  COLORREF BGColor,FGColor; // Цвета фона и текста текущей ячейки таблицы (только при рисовании таблицы)
  int CellAlignment;    // Выравнивание ячейки таблицы
  BOOL DefaultTableDraw;  // Устанавливается в реакции в TRUE, если нужно нарисовать ячейку таблицы по умолчанию

  // Для RDS_FORMSERVEVENT_TBLBUTTON и RDS_FORMSERVEVENT_TBLBTNENABLE **** REV2 ****
  // Для указания строки таблицы, если нужно, используется TableRow
  int Button;           // Кнопка (RDS_FORMVAL_TBLBTN_*)
  int ButtonResult;     // Результат (зависит от кнопки)
    #define RDS_FORMSERVRESULT_OK       1       // Нормальное завершение
    #define RDS_FORMSERVRESULT_NO       0       // Отмена действия
  int IndexAfterButton; // Новый индекс строки, который нужно установить после реакции, или -1
} RDS_FORMSERVFUNCDATA;
typedef RDS_FORMSERVFUNCDATA *RDS_PFORMSERVFUNCDATA;
//---------------------------------------------------------------------------

// Структура для функции rdsBlockOrConnByExtId
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  BOOL Found;           // Что-то найдено
  RDS_BHANDLE Block;    // Найденный блок
  RDS_CHANDLE Conn;     // Найденная связь или шина
  int Type;             // Тип найденного объекта (RDS_BT*,RDS_CT* или RDS_TUNKNOWN)
} RDS_FINDBYEXTIDDATA;
typedef RDS_FINDBYEXTIDDATA *RDS_PFINDBYEXTIDDATA;
//---------------------------------------------------------------------------

// Константы для функции rdsProcessText
#define RDS_PT_TEXTTOSTRING     0       // Преобразование спецсимволов в символическое представление
#define RDS_PT_STRINGTOTEXT     1       // Обратная к RDS_PT_TEXTTOSTRING
#define RDS_PT_VARTYPETEXT      2       // Текстовое описание типа переменной по первой букве строки (RDS_VARTYPE_*)
#define RDS_PT_VARTYPECHAR      3       // Cтрока из одного символа RDS_VARTYPE_* по строке описания типа переменной
// **** REV2 ****
#define RDS_PT_RDSRUSENCODE     4       // Русские символы - в латинские с подчеркиванием
#define RDS_PT_RDSRUSDECODE     5       // Латинские символы с подчеркиванием - в русские
#define RDS_PT_ENCODEURICOMP    6       // Аналог encodeURIComponent в JavaScript
#define RDS_PT_DECODEURICOMP    7       // Аналог decodeURIComponent в JavaScript
#define RDS_PT_TEXTTOSTRINGSL   8       // Преобразование спецсимволов в символическое представление, замена '/' на '0x2f'
//---------------------------------------------------------------------------

// Константы для функции rdsShowBlockPanelTab
#define RDS_BLOCKPANELOP_HIDE           0       // Спрятать вкладку
#define RDS_BLOCKPANELOP_SHOW           1       // Показать вкладку
#define RDS_BLOCKPANELOP_HIDEEXCEPT     2       // Спрятать все, кроме указанной
#define RDS_BLOCKPANELOP_SHOWALL        3       // Показать все
#define RDS_BLOCKPANELOP_SELECT         4       // Сделать текущей
//---------------------------------------------------------------------------

// Константы для функции rdsSetBlockAltNameText
#define RDS_ALTBLKNAME_LEFT             -1      // Выравнивание влево
#define RDS_ALTBLKNAME_CENTER           0       // Выравнивание по центру
#define RDS_ALTBLKNAME_RIGHT            1       // Выравнивание вправо
//---------------------------------------------------------------------------

// Константы для функции rdsExecuteCommand
#define RDS_SYSCMD_STARTCALC            0       // Запустить расчет
#define RDS_SYSCMD_LOADFILE             1       // Загрузить файл
#define RDS_SYSCMD_SAVEFILE             2       // Записать файл
#define RDS_SYSCMD_LOADTEMPLATE         3       // Загрузить шаблон
#define RDS_SYSCMD_MESSAGEBOX           4       // Вывести окно с сообщением
#define RDS_SYSCMD_EDITMODE             5       // Перейти в режим редактирования
#define RDS_SYSCMD_CALCMODE             6       // Перейти в режим моделирования
#define RDS_SYSCMD_RESETCALC            7       // Сбросить расчет для всей схемы
#define RDS_SYSCMD_REFRESH              8       // Обновить окна для указанной подсистемы **** REV2 ****
#define RDS_SYSCMD_COMPILE              9       // Компилировать модель или все модели **** REV2 ****
#define RDS_SYSCMD_DELAYEDTOOLCALL      10      // Отложенный вызов модуля расширения без параметра (дублируется rdsExecToolDelayed) **** REV2 ****
// Флаги для команд RDS_SYSCMD_LOADFILE и RDS_SYSCMD_LOADTEMPLATE
#define RDS_SYSCMDF_KEEPQUEUE           1       // Не очищать очередь команд после загрузки **** REV2 ****
#define RDS_SYSCMDF_KEEPTOOLS           2       // Не выгружать модули расширения перед загрузкой **** REV2 ****
// Флаг для команды RDS_SYSCMD_REFRESH
#define RDS_SYSCMDF_RECURSE             1       // Обновить все вложенные окна **** REV2 ****
// Флаг для команды RDS_SYSCMD_COMPILE
#define RDS_SYSCMDF_REBUILD             1       // Принудительно компилировать **** REV2 ****
//---------------------------------------------------------------------------

// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsPBARCreate (ProgressBar)
// Команды
#define RDS_PBAR_RESET                  0       // Сбросить позицию в 0
#define RDS_PBAR_SHOW                   1       // Показать окно
#define RDS_PBAR_HIDE                   2       // Убрать окно
// Команды установки и получения целых параметров (функции rdsSetObjectInt, rdsGetObjectInt)
#define RDS_PBAR_MAX                    0       // Максимальное значение
#define RDS_PBAR_POSITION               1       // Текущее значение
#define RDS_PBAR_ADDTOPOS               2       // Добавить значение к текущему (только установка)
// Команды установки строки
#define RDS_PBAR_SETCAPTION             3       // Установить текст заголовка
//---------------------------------------------------------------------------

// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsBEUCreate (undo изменения параметров блока)
// Команды
#define RDS_BEU_STORECHANGED            0       // Запомнить изменившиеся параметры
#define RDS_BEU_JOINNEXT                1       // Объединить со следующей операцией  **** REV2 ****

// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsCEUCreate (undo изменения параметров связи)
// Команды
#define RDS_CEU_STORECHANGED            0       // Запомнить изменившиеся параметры  **** REV2 ****
#define RDS_CEU_JOINNEXT                1       // Объединить со следующей операцией  **** REV2 ****

// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsPANCreate (панель в окне подсистемы)
// Флаги rdsPANCreate
#define RDS_PAN_F_BORDER        1       // У панели есть рамка
#define RDS_PAN_F_SCALABLE      2       // Панель меняет размер согласно масштабу подсистемы
#define RDS_PAN_F_SIZEABLE      4       // Пользователь может изменять размер панели (только вместе с RDS_PAN_F_BORDER)
#define RDS_PAN_F_MOVEABLE      8       // Пользователь может перемещать панель (только вместе с RDS_PAN_F_BORDER)
#define RDS_PAN_F_NOBUTTON     16       // Нет кнопки закрытия панели (только вместе с RDS_PAN_F_BORDER)
#define RDS_PAN_F_HIDDEN       32       // Панель создается скрытой
#define RDS_PAN_F_CAPTION      64       // Есть заголовок панели (только вместе с RDS_PAN_F_BORDER)
#define RDS_PAN_F_PAINTMSG    128       // Блок должен получать сообщение о необходимости перерисовать панель
// Команды установки и получения целых параметров (функции rdsSetObjectInt, rdsGetObjectInt)
#define RDS_PAN_VISIBLE         0       // Видимость панели
#define RDS_PAN_WIDTH           1       // Внешняя ширина панели
#define RDS_PAN_HEIGHT          2       // Внешняя высота панели
#define RDS_PAN_LEFT            3       // Горизонтальная координата левой стороны панели
#define RDS_PAN_TOP             4       // Вертикальная координата верхней стороны панели
#define RDS_PAN_CLIENTWIDTH     5       // Внутренняя ширина панели
#define RDS_PAN_CLIENTHEIGHT    6       // Внутренняя высота панели
#define RDS_PAN_MINCLWIDTH      7       // Минимальная внутренняя ширина панели
#define RDS_PAN_MAXCLWIDTH      8       // Максимальная внутренняя ширина панели
#define RDS_PAN_MINCLHEIGHT     9       // Минимальная внутренняя высота панели
#define RDS_PAN_MAXCLHEIGHT     10      // Максимальная внутренняя высота панели
#define RDS_PAN_FLAGS           11      // Флаги RDS_PAN_F_*
// Команды установки и получения строк
#define RDS_PAN_CAPTION         12      // Заголовок панели

// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsCSVCreate (объект для работы с CSV)
// Команды
#define RDS_CSV_CLEAR           0       // Очистить все данные
#define RDS_CSV_CLOSEFILE       1       // Закрыть открытый файл
#define RDS_CSV_STRFROMFILE     2       // Считать очередную строку из файла
#define RDS_CSV_STRTOFILE       3       // Записать строку в файл
#define RDS_CSV_REWINDFILE      4       // Снова начать чтение строк с начала файла **** REV2 ****
// Команды установки и получения целых параметров (функции rdsSetObjectInt, rdsGetObjectInt)
#define RDS_CSV_LINECOUNT       4       // Число строк (только чтение)
#define RDS_CSV_LINECOLUMNS     5       // Число столбцов в конкретной строке (только чтение)
#define RDS_CSV_MAXCOLUMNS      6       // Максимальное число столбцов по всем строкам (только чтение)
#define RDS_CSV_QUOTECHAR       7       // Символ кавычки (чтение, установка)
#define RDS_CSV_DELIMITERCHAR   8       // Символ-разделитель (чтение, установка)
#define RDS_CSV_FILEISOPEN      9       // Файл открыт (1 - да, только чтение)
#define RDS_CSV_FILEERROR       16      // Ошибка в файловой функции (1 - да, только чтение)
// Команды установки и получения строк
#define RDS_CSV_LINE            10      // Строка (чтение дин.копии, установка)
#define RDS_CSV_TEXT            11      // Весь текст (чтение дин.копии, установка)
#define RDS_CSV_OPENFILEREAD    12      // Открыть файл для чтения (только установка)
#define RDS_CSV_OPENFILEWRITE   13      // Открыть файл для записи (только установка)
#define RDS_CSV_LOADFROMFILE    14      // Загрузить из файла весь текст (только установка)
#define RDS_CSV_SAVETOFILE      15      // Записать в файл весь текст (только установка)


// Идентификаторы иконок для функций rdsXGDrawStdIcon и rdsXGGetStdIconSize
#define RDS_STDICON_GREENSQUARE         0       // Маленький зеленый квадрат
#define RDS_STDICON_YELLOWSQUARE        1       // Маленький желтый квадрат
#define RDS_STDICON_REDSQUARE           2       // Маленький красный квадрат
#define RDS_STDICON_YELLOWGEAR          7       // Желтая шестеренка
#define RDS_STDICON_RUN                 8       // Символ "play" (треугольник вправо)
#define RDS_STDICON_REDGEAR             9       // Красная шестеренка
#define RDS_STDICON_SYSTEM             10       // Маленький значок подсистемы
#define RDS_STDICON_BLOCK              11       // Маленький значок блока
#define RDS_STDICON_EYE                15       // Глаз (как в редакторе слоев)
#define RDS_STDICON_PENCIL             16       // Карандаш (как в редакторе слоев)
#define RDS_STDICON_REDTRIEXCLAM       23       // "!" в красном треугольнике
#define RDS_STDICON_YELLOWQUESTION     24       // "?" в желтом круге
#define RDS_STDICON_DISABLEDCONN       25       // "кирпич" (как на выключенных связях)
#define RDS_STDICON_REDCIRCEXCLAM      26       // "!" в красном круге
#define RDS_STDICON_YELCIRCEXCLAM      27       // "!" в желтом круге
#define RDS_STDICON_STOP               (0x20000|24) // Символ "stop" (черный квадрат)
#define RDS_STDICON_ZOOM               39       // Лупа                            **** REV2 **** начиная с этого
#define RDS_STDICON_NOZOOM             40       // Лупа с красным крестом
#define RDS_STDICON_ZOOMIN             41       // Лупа с плюсом
#define RDS_STDICON_ZOOMOUT            42       // Лупа с минусом
#define RDS_STDICON_TDOWN              43       // Треугольник вниз
#define RDS_STDICON_TUP                44       // Треугольник вверх
#define RDS_STDICON_TRIGHT             45       // Треугольник вправо
#define RDS_STDICON_TLEFT              46       // Треугольник влево
#define RDS_STDICON_PLUS               47       // Плюс
#define RDS_STDICON_MINUS              48       // Минус
#define RDS_STDICON_DELETE             49       // Знак удаления
#define RDS_STDICON_LEFT               50       // Тонкая стрелка влево
#define RDS_STDICON_RIGHT              51       // Тонкая стрелка вправо
#define RDS_STDICON_UP                 52       // Тонкая стрелка вверх
#define RDS_STDICON_DOWN               53       // Тонкая стрелка вниз
#define RDS_STDICON_HZOOMIN            54       // "+" в круге с горизонтальной двунаправленной стрелкой
#define RDS_STDICON_HZOOMOUT           55       // "-" в круге с горизонтальной двунаправленной стрелкой
#define RDS_STDICON_VZOOMIN            56       // "+" в круге с вертикальной двунаправленной стрелкой
#define RDS_STDICON_VZOOMOUT           57       // "-" в круге с вертикальной двунаправленной стрелкой
#define RDS_STDICON_HSIZE              58       // Две горизонтальные размерные стрелки
#define RDS_STDICON_VSIZE              59       // Две вертикальные размерные стрелки
#define RDS_STDICON_HVSIZE             60       // Прямоугольник с двумя размерными стрелками
//---------------------------------------------------------------------------

// Флаги для функций rdsNetBroadcastData и rdsNetSendData
#define RDS_NETSEND_UPDATE      1       // Можно заменить старые неотправленные данные новыми
#define RDS_NETSEND_NOWAIT      2       // Не ждать ответа сервера, передавать следующие
                                        // данные немедленно
#define RDS_NETSEND_SERVREPLY   4       // Получив данные, сервер должен сообщить об этом
                                        // пославшему блоку (RDS_BFM_NETDATAACCEPTED)
#define RDS_NETSEND_UDP         8       // Передать данные по протоколу UDP, если это разрешено
                                        // в настройках RDS
//---------------------------------------------------------------------------

// Флаги для функции rdsVarArrayIndexCheck
#define RDS_VAIC_SINGLE         1       // Проверять не индексы по отдельности, а сравнивать
                                        // произведение индексов с общим размером массива
                                        // Влияет также на текст сообщения ("массив" вместо "матрица")
#define RDS_VAIC_MESSAGE        2       // Выводить сообщение об ошибке
#define RDS_VAIC_SINGLEMSG      4       // Выводить только самое первое сообщение об ошибке
                                        // (при сброшенном RDS_VAIC_MESSAGE игнорируется)
#define RDS_VAIC_STOPCALC       8       // Останавливать расчет при ошибке
//---------------------------------------------------------------------------

// Байты тегов для специального формата сохранения
// 87654321
// .tttnnnn
// tt: 0 - служебные, 0x10 - блоки, 0x20 - связи
// Служебные
#define RDS_SFTAG_EOF           0       // Конец данных
#define RDS_SFTAG_CONNSTYLES    1       // Стили связей и шин
#define RDS_SFTAG_TYPES         2       // Структуры
#define RDS_SFTAG_DATATYPE      0xff    // Тип данных (за ним идет DWORD 0)
// Блоки
#define RDS_SFTAG_SIMPLEBLOCK   0x10    // Простой блок
#define RDS_SFTAG_SYSTEM        0x11    // Подсистема
#define RDS_SFTAG_INPUTBLOCK    0x12    // Внешний вход
#define RDS_SFTAG_OUTPUTBLOCK   0x13    // Внешний выход
#define RDS_SFTAG_BUSPORT       0x14    // Порт шины
#define RDS_SFTAG_ROOT          0x15    // Корневая подсистема
// Связи
#define RDS_SFTAG_CONNECTION    0x20    // Связь
#define RDS_SFTAG_BUS           0x21    // Шина
//---------------------------------------------------------------------------

// Константы для функции rdsSetLayerPosition (тип операции)
#define RDS_SLP_TOP     0       // На самый верх
#define RDS_SLP_BOTTOM  1       // В самый низ
#define RDS_SLP_BEFORE  2       // Перед указанным слоем
#define RDS_SLP_AFTER   3       // После указанного слоя
//---------------------------------------------------------------------------

// Флаги для rdsStringReplace
#define RDS_SRF_STDPATHS        1       // Заменять символы стандартных путей ($DLL$ и т.д)
                                        // на сами пути
#define RDS_SRF_IGNORECASE      2       // Игнорировать регистр при поиске подстрок
//---------------------------------------------------------------------------

// Флаги для rdsCallFileDialog
#define RDS_CFD_OPEN            0       // Диалог открытия (константа введена
                                        // просто для удобства)
#define RDS_CFD_SAVE            1       // Диалог сохраненния
#define RDS_CFD_CREATEPROMPT    2       // Подтверждать создание файла при выборе
                                        // несуществующего в диалоге сохранения)
#define RDS_CFD_OVERWRITEPROMPT 4       // Подтверждать замену файла при выборе
                                        // существующего в диалоге сохранения)
#define RDS_CFD_MUSTEXIST       8       // Файл должен существовать (для диалога открытия)
#define RDS_CFD_ABSPATH         16      // Возвращать абсолютный, а не относительный путь
//---------------------------------------------------------------------------

// Константы для rdsTransformFileName
#define RDS_TFN_GETPATH         0       // Выделить из имени файла путь с "\"
#define RDS_TFN_GETPATHNOBS     1       // Выделить из имени файла путь без "\"
#define RDS_TFN_GETEXT          2       // Выделить из имени файла расширение
#define RDS_TFN_GETNAME         3       // Выделить из имени файла само имя без пути
#define RDS_TFN_CHANGEEXT       4       // Заменить расширение на другое
#define RDS_TFN_INCLUDEPATHBS   5       // Добавить к пути "\", если его там нет
#define RDS_TFN_EXCLUDEPATHBS   6       // Убрать из конца пути "\", если он там есть
//---------------------------------------------------------------------------

// Константы для модулей преобразования BMP в другие форматы **** REV2 ****
// Режимы вызова
#define RDS_BMPCONV_SETUP       0       // Настройки и "о программе"
#define RDS_BMPCONV_SAVEFILE    1       // Записать файл
//---------------------------------------------------------------------------

// Структура-параметр функции rdsSaveSystemBitmap **** REV2 ****
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  RDS_BHANDLE Block;    // Подсистема
  DWORD Flags;          // Флаги (RDS_SSBF_*)
    #define RDS_SSBF_ZOOMED     1       // Left,Top,Width и Height - в масштабе Zoom, а не 100%
  int Left,Top;         // Начало сохраняемой области в масштабе 100% или Zoom (зависит от RDS_SSBF_ZOOMCOORDS)
  int Width,Height;     // Размеры сохраняемой области в масштабе 100% или Zoom (зависит от RDS_SSBF_ZOOMCOORDS)
  int PicWidth,PicHeight;// Максимальные размеры сохраняемой картинки
  LPSTR FileName;       // Шаблон имени файла с путем (можно использовать символьные константы путей и RDS_SSBC_*)
    #define RDS_SSBC_NX         "$NX$"  // Номер картинки по горизонтали
    #define RDS_SSBC_4NX        "$4NX$" // То же с ведущими нулями, всегда 4 символа
    #define RDS_SSBC_NY         "$NY$"  // Номер картинки по вертикали
    #define RDS_SSBC_4NY        "$4NY$" // То же с ведущими нулями, всегда 4 символа
    #define RDS_SSBC_NN         "$NN$"  // Возрастающий порядковый номер картинки (из левого верхнего угла сначала вправо)
    #define RDS_SSBC_4NN        "$4NN$" // То же с ведущими нулями, всегда 4 символа
  LPSTR ConvertExt;     // Расширение модуля преобразования картинки или NULL для BMP
  DWORD Reserved;       // Зарезервировано для выравнивания **** REV2 ****
  double Zoom;          // Масштаб
} RDS_SAVESYSBMPDATA;
typedef RDS_SAVESYSBMPDATA *RDS_PSAVESYSBMPDATA;
//---------------------------------------------------------------------------

// Структура описания параметров внешнего вида блока "прямоугольник с текстом" **** REV2 ****
// Используется в rdsSetBlockTextRectParams, rdsGetBlockTextRectParams
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  BOOL Exists;          // Блок имеет внешний вид в виде прямоугольника с текстом
  LPSTR Text;           // Текст
  int HAlign;           // Выравнивание текста (-1 - влево, 0 - по центру, 1 - вправо)
  int VAlign;           // Выравнивание текста (-1 - вверх, 0 - по центру, 1 - вниз)
  BOOL BorderFlag;      // Наличие рамки
  int BorderWidth;      // Толщина линии рамки
  BOOL ScaleBorder;     // Масштабирование рамки
  COLORREF Border;      // Цвет рамки
  BOOL FillFlag;        // Наличие заполнения
  COLORREF Fill;        // Цвет заполнения
  BOOL Translate;       // Требуется перевод
  // Шрифт передается/получается в отдельной структуре RDS_SERVFONTPARAMS
} RDS_BLOCKTEXTRECTDATA;
typedef RDS_BLOCKTEXTRECTDATA *RDS_PBLOCKTEXTRECTDATA;
//---------------------------------------------------------------------------

// Флаги для rdsSetBlockTextRectParams **** REV2 ****
#define RDS_SBTRPREFRESH        1       // Обновить окно
//---------------------------------------------------------------------------

// Флаги для rdsSCEUSaveBlock и rdsSCEUSaveConn **** REV2 ****
#define RDS_SCEUFLAG_FULL       1       // Запись всех данных блока (не только параметров)
#define RDS_SCEUFLAG_BEFORE     2       // Запись данных объекта перед операцией
#define RDS_SCEUFLAG_AFTER      0       // Запись данных объекта после операции (псевдо-флаг для ясности)
// Константы для вспомогательного объекта rdsSCEUCreate **** REV2 ****
// Команды
#define RDS_SCEU_CLEAR          0       // Очистить все данные
#define RDS_SCEU_STORE          1       // Записать в undo
#define RDS_SCEU_STOREDELAYED   2       // Записать в undo после следующей операции
#define RDS_SCEU_JOINPREV       3       // Объединить с предыдущей
//---------------------------------------------------------------------------

// Константы и описания для модулей расширения **** REV2 ****
// Режимы вызова
#define RDS_TOOLM_INIT          0       // Инициализация
#define RDS_TOOLM_CLEANUP       1       // Очистка
#define RDS_TOOLM_SETUP         2       // Вызов настроек
#define RDS_TOOLM_EXECUTE       3       // Вызов самого расширения
#define RDS_TOOLM_MODECHANGE    4       // Изменение режима RDS
#define RDS_TOOLM_TIMER         5       // Срабатывание таймера
#define RDS_TOOLM_BEFORESAVE    6       // Перед сохранением схемы
#define RDS_TOOLM_AFTERSAVE     7       // После сохранения схемы
#define RDS_TOOLM_UNLOADSYSTEM  8       // Вся система сейчас будет выгружена
#define RDS_TOOLM_AFTERLOAD     9       // После загрузки всей схемы
#define RDS_TOOLM_CALL          10      // Вызов из модели блока или другого модуля
#define RDS_TOOLM_REMOTECALL    11      // Вызов по ДУ
#define RDS_TOOLM_EXTCALL       12      // Расширенный вызов из модели блока или другого модуля
#define RDS_TOOLM_DELAYEDCALL   13      // Отложенный вызов из модели блока или другого модуля
//---------------------------------------------------------------------------
// Результаты **** REV2 ****
#define RDS_TOOLR_DONE          0       // Выполнено (можно выгружать модуль)
#define RDS_TOOLR_STAY          1       // Модуль должен остаться в памяти
//---------------------------------------------------------------------------
// Данные модуля расширения **** REV2 ****
typedef struct
{ LPVOID ModuleData;    // Адрес области данных модуля (чтение/запись)
  int RDSMode;          // Текущий режим RDS (константа RDS_CM_*)
  LPSTR ServiceName;    // Внутреннее имя (только чтение)
  // Целое пользовательское поле (не обрабатывается и не инициализируется)
  int Tag;
} RDS_TOOLMODULEDATA;
typedef RDS_TOOLMODULEDATA *RDS_PTOOLMODULEDATA;
//---------------------------------------------------------------------------
// Параметры вызова модуля расширения **** REV2 ****
// Используются в режимах RDS_TOOLM_CALL и RDS_TOOLM_REMOTECALL
typedef struct
{ LPSTR String;         // Переданная строка
  int Value;            // Переданное число
} RDS_TOOLCALLDATA;
typedef RDS_TOOLCALLDATA *RDS_PTOOLCALLDATA;
//---------------------------------------------------------------------------
// Параметры расширенного вызова модуля расширения **** REV2 ****
// Используются в режиме RDS_TOOLM_EXTCALL
typedef struct
{ DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  int Command;          // Переданное число
  int IntParam;         // Дополнительное переданное число
  int IntValue;         // Дополнительное переданное число
  int NInStrings;       // Количество входных строк
  LPSTR *InStrings;     // Массив входных строк (поддерживаются и освобождаются вызвавшим)

  int NOutStrings;      // Число выходных строк
  LPSTR *OutStrings;    // Массив выходных строк (отводится модулем при помощи rdsAllocate, каждая строка в массиве
                        // тоже отводится модулем при помощи rdsAllocate, все освобождаются ВЫЗВАВШИМ при помощи
                        // rdsFree)
  int Result;           // Возвращаемый результат

  RDS_BHANDLE Block;    // Блок, для которого вызывается операция, или NULL (как входной параметр, заполняется
                        // вызвавшим, может служить и выходным)

} RDS_TOOLEXTCALLDATA;
typedef RDS_TOOLEXTCALLDATA *RDS_PTOOLEXTCALLDATA;
//---------------------------------------------------------------------------
// Флаги и константы функции rdsExecTool **** REV2 ****
//   3         2         1
//  10987654321098765432109876543210
//  ......................SSMMMMMMMM
// M - режим
// S - способ поиска модуля
// Режим
#define RDS_ETF_M_EXECUTE       0       // Вызвать модуль расширения в режиме RDS_TOOLM_EXECUTE (как из меню)
#define RDS_ETF_M_SETUP         1       // Вызвать модуль расширения в режиме RDS_TOOLM_SETUP (настройка)
#define RDS_ETF_M_CALL          2       // Вызвать модуль расширения в режиме RDS_TOOLM_CALL (или RDS_TOOLM_REMOTECALL) с параметром RDS_PTOOLCALLDATA
// Способ поиска модуля (используется и для rdsExecToolExt)
#define RDS_ETF_S_BYDLL         0       // По имени функции и имени DLL
#define RDS_ETF_S_BYSERVNAME    0x100   // По внутреннему имени
// Маски
#define RDS_ETF_MODEMASK        0xff    // Режим
#define RDS_ETF_SEARCHMASK      0x300   // Способ поиска
//---------------------------------------------------------------------------
// Флаги функции rdsVSCompare **** REV2 ****
#define RDS_VARCOMP_ALL         0       // Все параметры
#define RDS_VARCOMP_VARTYPES    1       // Типы переменных
#define RDS_VARCOMP_VARNAMES    2       // Имена переменных
#define RDS_VARCOMP_VARDEFAULTS 4       // Значения переменных по умолчанию
#define RDS_VARCOMP_VARIO       8       // Роли переменных в блоке (входы/выходы/внутренние)
#define RDS_VARCOMP_VARDISP     16      // Флаги отображения переменных (меню, отображение имени)
#define RDS_VARCOMP_VARAUTORUN  32      // Флаги автозапуска модели для входов
#define RDS_VARCOMP_VARREM      64      // Комментарии переменных
#define RDS_VARCOMP_AUTOCONN    128     // Связанные сигналы и логические переменные
//---------------------------------------------------------------------------
// Коды возврата функции справки **** REV2 ****
#define RDS_HELP_OK             0       // Все в порядке
#define RDS_HELP_NOUIFILE       1       // Файл UI (со списком тем) не найден
#define RDS_HELP_READERROR      2       // Ошибка чтения файла
#define RDS_HELP_NOTOPIC        3       // Тема не найдена
#define RDS_HELP_NOHELPFILE     4       // Файл самой справки (по ссылке из UI) не найден
#define RDS_HELP_NOEXE          5       // Для файла справки не найдена программа-обработчик
#define RDS_HELP_UNKNOWN        6       // Прочие ошибки
//---------------------------------------------------------------------------
// Флаги функции rdsEnumDynVars **** REV2 ****
#define RDS_EDV_SEARCH          1       // С поиском вверх по иерархии подсистем
//---------------------------------------------------------------------------
// Флаги дополнительных параметров связи **** REV2 ****
#define RDS_ACP_HINT            1       // Выводится во всплывающей подсказке
#define RDS_ACP_NOTRANSLATE     2       // Не требуется перевод
//---------------------------------------------------------------------------
// Флаги функции rdsCopySelectedToMem **** REV2 ****
#define RDS_CSTM_STATE          1       // Копировать вместе с состояниями блоков
#define RDS_CSTM_AUTOCOMP       2       // Копировать вместе с глобальными данными автокомпиляции
#define RDS_CSTM_LAYERS         4       // Копировать вместе со слоями
#define RDS_CSTM_CLASSES        8       // Копировать вместе с классами
#define RDS_CSTM_KEEPEXTERNAL  16       // Запрет замены точек связей, соединенных с не выделенными объектами, на внутренние 
//---------------------------------------------------------------------------
// Флаги функции rdsPasteFromMem **** REV2 ****
#define RDS_PFM_UPDATEWIN       1       // Обновить окно
#define RDS_PFM_CONNDATA        2       // Принять внешние данные по связям после вставки
#define RDS_PFM_STATE           4       // Загрузить вместе с состояниями блоков
#define RDS_PFM_CREATELAYERS    8       // Если в заружаемых данных есть слои, создавать отсутствующие
#define RDS_PFM_INVALIDPATHS    16      // Не следует полагатьcя на пути в загружаемых данных (данные с другой машины)
//---------------------------------------------------------------------------
// Константы функции rdsExclusiveFileOp **** REV2 ****
#define RDS_EFO_USE     1       // Пометить файл как используемый
#define RDS_EFO_UNUSE   2       // Пометить файл как не используемый
#define RDS_EFO_FORGET  3       // Забыть о существовании файла
//---------------------------------------------------------------------------
// Флаги функции rdsPasteFromMem **** REV2 ****
#define RDS_ICF_NOCALL          1       // Не вызывать модель самого блока (начать с передачи его выходов)
#define RDS_ICF_ALLBLOCKS       2       // Вызвать инициализационный расчет для всех блоков
#define RDS_ICF_UPDATEWIN       4       // Обновить затронутые окна после операции (только в режимах редактирования и моделирования)
//---------------------------------------------------------------------------
// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsINTCreate (объект для работы с интервалами положительных целых чисел)  **** REV2 ****
// Команды
#define RDS_INT_CLEAR           0       // Очистить все данные
// Команды установки и получения целых параметров (функции rdsSetObjectInt, rdsGetObjectInt)
#define RDS_INT_ALLOWZERO       1       // Разрешение работы с нулем
#define RDS_INT_VALUE           2       // Установить значение или получить свободное
//---------------------------------------------------------------------------
// Флаги функции rdsLockDrawUpdates **** REV2 ****
#define RDS_LDUF_LOCK           1       // Заблокировать
#define RDS_LDUF_UNLOCK         2       // Разблокировать
//---------------------------------------------------------------------------
// Флаги функции rdsGBLAddBlock **** REV2 ****
#define RDS_GBL_ADD     0       // Добавить блок (младший бит - 0)
#define RDS_GBL_DELETE  1       // Удалить блок (младший бит - 1)
//---------------------------------------------------------------------------
// Константы для взаимодействия со вспомогательным объектом, созданным функцией
// rdsGBLCreate (объект для работы с глобальным списком блоков)
// Команды установки и получения целых параметров (функции rdsSetObjectInt, rdsGetObjectInt)
#define RDS_GBL_GETCOUNT        0       // Число блоков (только чтение)
//---------------------------------------------------------------------------
// Константы функций rdsSetBlockOrder и rdsSetConnOrder **** REV2 ****
#define RDS_BLOCKORDER_FIRST    0       // Сделать первым в подсистеме
#define RDS_BLOCKORDER_LAST     1       // Сделать последним в подсистеме
#define RDS_BLOCKORDER_AFTER    2       // Поставить после заданного
#define RDS_BLOCKORDER_BEFORE   3       // Поставить перед заданным
//---------------------------------------------------------------------------
// Флаги функции rdsSetConnText **** REV2 ****
#define RDS_SCTF_TEXT           1       // Установить/очистить текст
#define RDS_SCTF_ALIGNMENT      2       // Установить выравнивание текста
#define RDS_SCTF_SHOW           4       // Установить показ текста
#define RDS_SCTF_COORDS         8       // Установить координаты центра текста
#define RDS_SCTF_TRANSLATE      16      // Установить необходимость перевода
//---------------------------------------------------------------------------
// Константы типа текста для переводов **** REV2 ****
#define RDS_TRANSLML_SINGLE     0       // Однострочный текст
#define RDS_TRANSLML_LF         1       // Многострочный текст, разделитель "\n"
#define RDS_TRANSLML_CRLF       2       // Многострочный текст, разделитель "\r\n"
// Диапазон разрешенных целых иденификаторов переводимых параметров для внутренних нужд модулей автокомпиляции **** REV2 ****
#define RDS_TRANSLACPID_LOW     -199999 // Начало диапазона (включительно)
#define RDS_TRANSLACPID_HIGH    -100000 // Конец диапазона (включительно)
//---------------------------------------------------------------------------
// Флаги функции rdsFORMTBLAddHeader **** REV2 ****
// Маска выравнивания текста
#define RDS_FORMTBLF_ALIGN      3
  #define RDS_FORMTBLF_LEFT     0       // Влево
  #define RDS_FORMTBLF_CENTER   1       // По центру
  #define RDS_FORMTBLF_RIGHT    2       // Вправо
// Маска вырванивания заголовка
#define RDS_FORMTBLF_HDRALIGN   12
  #define RDS_FORMTBLF_HDRLEFT   0       // Влево
  #define RDS_FORMTBLF_HDRCENTER 4       // По центру
  #define RDS_FORMTBLF_HDRRIGHT  8       // Вправо
// Пользовательское рисование текста
#define RDS_FORMTBLF_DRAW       16
// Пользовательское рисование заголовка
#define RDS_FORMTBLF_HDRDRAW    32
//---------------------------------------------------------------------------

// Возврат выравнивания по умолчанию
#ifndef RDS_NO_PACK
  #include <poppack.h>
#endif
//---------------------------------------------------------------------------

// Интерфейсные функции - типы указателей
typedef void (RDSCALL *RDS_VS)(LPSTR);
typedef int (RDSCALL *RDS_IV)(void);
typedef HWND (RDSCALL *RDS_HwV)(void);
typedef RDS_BHANDLE (RDSCALL *RDS_BhV)(void);
typedef int (RDSCALL *RDS_IBhBd)(RDS_BHANDLE,RDS_PBLOCKDESCRIPTION);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhIBd)(RDS_BHANDLE,int,RDS_PBLOCKDESCRIPTION);
typedef int (RDSCALL *RDS_IChCd)(RDS_CHANDLE,RDS_PCONNDESCRIPTION);
typedef RDS_CHANDLE (RDSCALL *RDS_ChBhICd)(RDS_BHANDLE,int,RDS_PCONNDESCRIPTION);
typedef RDS_CHANDLE (RDSCALL *RDS_ChChICd)(RDS_CHANDLE,int,RDS_PCONNDESCRIPTION);
typedef int (RDSCALL *RDS_IChIPd)(RDS_CHANDLE,int,RDS_PPOINTDESCRIPTION);
typedef LPSTR (RDSCALL *RDS_SI)(int);
typedef LPSTR (RDSCALL *RDS_SV)(void);
typedef LPSTR (RDSCALL *RDS_SBhI)(RDS_BHANDLE,int);
typedef int (RDSCALL *RDS_IIS)(int,LPSTR);
typedef int (RDSCALL *RDS_II)(int);
typedef void (RDSCALL *RDS_VIIICrI)(int,int,int,COLORREF,int);
typedef void (RDSCALL *RDS_VIICr)(int,int,COLORREF);
typedef void (RDSCALL *RDS_VIIII)(int,int,int,int);
typedef void (RDSCALL *RDS_VIIIIII)(int,int,int,int,int,int);
typedef void (RDSCALL *RDS_VIIIIIIII)(int,int,int,int,int,int,int,int);
typedef void (RDSCALL *RDS_VISICrIIBBBB)(int,LPSTR,int,COLORREF,int,int,BOOL,BOOL,BOOL,BOOL);
typedef void (RDSCALL *RDS_VpLfCr)(LOGFONT*,COLORREF);
typedef void (RDSCALL *RDS_VIIS)(int,int,LPSTR);
typedef void (RDSCALL *RDS_VII)(int,int);
typedef void (RDSCALL *RDS_VIISpR)(int,int,LPSTR,RECT*);
typedef RDS_TIMERID (RDSCALL *RDS_ThThDwDwB)(RDS_TIMERID,DWORD,DWORD,BOOL);
typedef BOOL (RDSCALL *RDS_BpVI)(LPVOID,int);
typedef void (RDSCALL *RDS_VpVI)(LPVOID,int);
typedef BOOL (RDSCALL *RDS_BV)(void);
typedef void (RDSCALL *RDS_VSB)(LPSTR,BOOL);
typedef void (RDSCALL *RDS_VSpIpI)(LPSTR,int*,int*);
typedef RDS_PCOLORREF (RDSCALL *RDS_pCrV)(void);
typedef BOOL (RDSCALL *RDS_BpCr)(RDS_PCOLORREF);
typedef BOOL (RDSCALL *RDS_BBhEd)(RDS_BHANDLE,RDS_PEDITORPARAMETERS);
typedef BOOL (RDSCALL *RDS_BThTd)(RDS_TIMERID,RDS_PTIMERDESCRIPTION);
typedef void (RDSCALL *RDS_VTh)(RDS_TIMERID);
typedef void (RDSCALL *RDS_VThDw)(RDS_TIMERID,DWORD);
typedef LPSTR (RDSCALL *RDS_SSpSpCB)(LPSTR,LPSTR*,char*,BOOL);
typedef void (RDSCALL *RDS_VIB)(int,BOOL);
typedef void (RDSCALL *RDS_VSI)(LPSTR,int);
typedef void (RDSCALL *RDS_VSS)(LPSTR,LPSTR);
typedef void (RDSCALL *RDS_VSCrB)(LPSTR,COLORREF,BOOL);
typedef BOOL (RDSCALL *RDS_BSpSSIpIpCrpIpIpBpBpBpB)(LPSTR,LPSTR*,LPSTR,int,int*,
                                                      COLORREF*,int*,int*,
                                                      BOOL*,BOOL*,BOOL*,BOOL*);
typedef COLORREF (RDSCALL *RDS_CrSpS)(LPSTR,LPSTR*);
typedef void (RDSCALL *RDS_VpR)(RECT*);
typedef void (RDSCALL *RDS_VIIDDDB)(int,int,double,double,double,BOOL);
typedef void (RDSCALL *RDS_VpPI)(POINT *,int);
typedef int (RDSCALL *RDS_IBhS)(RDS_BHANDLE,LPSTR);
typedef int (RDSCALL *RDS_IBhIpV)(RDS_BHANDLE,int,LPVOID);
typedef int (RDSCALL *RDS_IBhSpVB)(RDS_BHANDLE,LPSTR,LPVOID,BOOL);
typedef RDS_CHANDLE (RDSCALL *RDS_ChBhChBBPd)(RDS_BHANDLE,RDS_CHANDLE,BOOL,BOOL,RDS_PPOINTDESCRIPTION);
typedef RDS_CHANDLE (RDSCALL *RDS_ChBhChPd)(RDS_BHANDLE,RDS_CHANDLE,RDS_PPOINTDESCRIPTION);
typedef void (RDSCALL *RDS_VChCa)(RDS_CHANDLE,RDS_PCONNAPPEARANCE);
typedef BOOL (RDSCALL *RDS_BSCa)(LPSTR,RDS_PCONNAPPEARANCE);
typedef int (RDSCALL *RDS_IBhSpV)(RDS_BHANDLE,LPSTR,LPVOID);
typedef LPVOID (RDSCALL *RDS_pVISSBS)(int,LPSTR,LPSTR,BOOL,LPSTR);
typedef BOOL (RDSCALL *RDS_BIS)(int,LPSTR);
typedef RDS_PDYNVARLINK (RDSCALL *RDS_DvISSB)(int,LPSTR,LPSTR,BOOL);
typedef void (RDSCALL *RDS_VDv)(RDS_PDYNVARLINK);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhSBd)(RDS_BHANDLE,LPSTR,RDS_PBLOCKDESCRIPTION);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBd)(RDS_PBLOCKDESCRIPTION);
typedef RDS_PDYNVARLINK (RDSCALL *RDS_DvISSBS)(int,LPSTR,LPSTR,BOOL,LPSTR);
typedef void (RDSCALL *RDS_VBhB)(RDS_BHANDLE,BOOL);
typedef void (RDSCALL *RDS_VBhBB)(RDS_BHANDLE,BOOL,BOOL);
typedef int (RDSCALL *RDS_IBhIB)(RDS_BHANDLE,int,BOOL);
typedef void (RDSCALL *RDS_VBhS)(RDS_BHANDLE,LPSTR);
typedef void (RDSCALL *RDS_VB)(BOOL);
typedef void (RDSCALL *RDS_VBh)(RDS_BHANDLE);
typedef int (RDSCALL *RDS_IChIICa)(RDS_CHANDLE,int,int,RDS_PCONNAPPEARANCE);
typedef BOOL (RDSCALL *RDS_BBhI)(RDS_BHANDLE,int);
typedef BOOL (RDSCALL *RDS_BBhS)(RDS_BHANDLE,LPSTR);
typedef void (RDSCALL *RDS_VBhEt)(RDS_BHANDLE,RDS_PEDITORTOOLBARS);
typedef void (RDSCALL *RDS_VV)(void);
typedef LPVOID (RDSCALL *RDS_pVpVpIpI)(LPVOID,int*,int*);
typedef BOOL (RDSCALL *RDS_BpVAd)(LPVOID,RDS_PARRAYACCESSDATA);
typedef BOOL (RDSCALL *RDS_BpVIIBAd)(LPVOID,int,int,BOOL,RDS_PARRAYACCESSDATA);
typedef BOOL (RDSCALL *RDS_BBhpV)(RDS_BHANDLE,LPVOID);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhIBCbpV)(RDS_BHANDLE,int,BOOL,RDS_BBhpV,LPVOID);
typedef int (RDSCALL *RDS_IIIB)(int,int,BOOL);
typedef BOOL (RDSCALL *RDS_BBhpB)(RDS_BHANDLE,BOOL*);
typedef int (RDSCALL *RDS_IBhIBCb2)(RDS_BHANDLE,int,BOOL,RDS_BBhpB);
typedef BOOL (RDSCALL *RDS_BICb2)(int,RDS_BBhpB);
typedef BOOL (RDSCALL *RDS_BBhIIpIpI)(RDS_BHANDLE,int,int,int*,int*);
typedef int (RDSCALL *RDS_IpV)(LPVOID);
typedef int (RDSCALL *RDS_ICb4pV)(RDS_IpV,LPVOID);
typedef int (RDSCALL *RDS_IBhIpVDw)(RDS_BHANDLE,int,LPVOID,DWORD);
typedef int (RDSCALL *RDS_IS)(LPSTR);
typedef void (RDSCALL *RDS_VBhIpVB)(RDS_BHANDLE,int,LPVOID,BOOL);
typedef BOOL (RDSCALL *RDS_BhPdPdpV)(RDS_PPOINTDESCRIPTION,RDS_PPOINTDESCRIPTION,LPVOID);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhDwCb1pV)(RDS_BHANDLE,DWORD,RDS_BhPdPdpV,LPVOID);
typedef void (RDSCALL *RDS_VpVII)(LPVOID,int,int);
typedef int (RDSCALL *RDS_ISpSpVI)(LPSTR,LPSTR*,LPVOID,int);
typedef LPSTR (RDSCALL *RDS_SIB)(int,BOOL);
typedef int (RDSCALL *RDS_ISB)(LPSTR,BOOL);
typedef RDS_MENUITEM (RDSCALL *RDS_MhSDwIDwII)(LPSTR,BOOL,int,DWORD,int,int);
typedef void (RDSCALL *RDS_VMhBB)(RDS_MENUITEM,BOOL,BOOL);
typedef void (RDSCALL *RDS_VMh)(RDS_MENUITEM);
typedef LPVOID (RDSCALL *RDS_pVDw)(DWORD);
typedef void (RDSCALL *RDS_VpV)(LPVOID);
typedef LPSTR (RDSCALL *RDS_SBhpI)(RDS_BHANDLE,int*);
typedef RDS_BHANDLE (RDSCALL *RDS_BhSBd)(LPSTR,RDS_PBLOCKDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BBh)(RDS_BHANDLE);
typedef void (RDSCALL *RDS_VCh)(RDS_CHANDLE);
typedef RDS_BHANDLE (RDSCALL *RDS_BhSBhIIBd)(LPSTR,RDS_BHANDLE,int,int,RDS_PBLOCKDESCRIPTION);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhBhIIBd)(RDS_BHANDLE,RDS_BHANDLE,int,int,RDS_PBLOCKDESCRIPTION);
typedef RDS_VHANDLE (RDSCALL *RDS_VhBhiVd)(RDS_BHANDLE,int,RDS_PVARDESCRIPTION);
typedef RDS_VHANDLE (RDSCALL *RDS_VhBhSBVd)(RDS_BHANDLE,LPSTR,BOOL,RDS_PVARDESCRIPTION);
typedef RDS_HOBJECT (RDSCALL *RDS_HoV)(void);
typedef RDS_HOBJECT (RDSCALL *RDS_HoB)(BOOL);
typedef void (RDSCALL *RDS_VHo)(RDS_HOBJECT);
typedef RDS_CHANDLE (RDSCALL *RDS_ChHoBhIpI)(RDS_HOBJECT,RDS_BHANDLE,int,int*);
typedef BOOL (RDSCALL *RDS_BHoChI)(RDS_HOBJECT,RDS_CHANDLE,int);
typedef int (RDSCALL *RDS_IHoII)(RDS_HOBJECT,int,int);
typedef int (RDSCALL *RDS_IHoBhSIIB)(RDS_HOBJECT,RDS_BHANDLE,LPSTR,int,int,BOOL);
typedef int (RDSCALL *RDS_IHoChSBIIB)(RDS_HOBJECT,RDS_CHANDLE,LPSTR,BOOL,int,int,BOOL);
typedef int (RDSCALL *RDS_IHoIIIIII)(RDS_HOBJECT,int,int,int,int,int,int);
typedef BOOL (RDSCALL *RDS_BBhBrB)(RDS_BHANDLE,RDS_PBLOCKDIMENSIONS,BOOL);
typedef RDS_HOBJECT (RDSCALL *RDS_HoBhDwB)(RDS_BHANDLE,DWORD,BOOL);
typedef LPVOID (RDSCALL *RDS_pVHoIIpI)(RDS_HOBJECT,int,int,int*);
typedef void (RDSCALL *RDS_VBhII)(RDS_BHANDLE,int,int);
typedef void (RDSCALL *RDS_VBhSS)(RDS_BHANDLE,LPSTR,LPSTR);
typedef BOOL (RDSCALL *RDS_BChILdPdPd)(RDS_CHANDLE,int,RDS_PLINEDESCRIPTION,
                                       RDS_PPOINTDESCRIPTION,RDS_PPOINTDESCRIPTION);
typedef int (RDSCALL *RDS_IChIILdpI)(RDS_CHANDLE,int,int,RDS_PLINEDESCRIPTION,int*);
typedef void (RDSCALL *RDS_VBhIIIIB)(RDS_BHANDLE,int,int,int,int,BOOL);
typedef BOOL (RDSCALL *RDS_BBhIIII)(RDS_BHANDLE,int,int,int,int);
typedef BOOL (RDSCALL *RDS_BBhIII)(RDS_BHANDLE,int,int,int);
typedef BOOL (RDSCALL *RDS_BDv)(RDS_PDYNVARLINK);
typedef BOOL (RDSCALL *RDS_BpVpV)(LPVOID,LPVOID);
typedef BOOL (RDSCALL *RDS_BHopI)(RDS_HOBJECT,int*);
typedef int (RDSCALL *RDS_IHoISCSDwIS)(RDS_HOBJECT,int,LPSTR,char,LPSTR,DWORD,int,LPSTR);
typedef BOOL (RDSCALL *RDS_VHoIIS)(RDS_HOBJECT,int,int,LPSTR);
typedef BOOL (RDSCALL *RDS_BHoBhpI)(RDS_HOBJECT,RDS_BHANDLE,int*);
typedef int (RDSCALL *RDS_IHoSS)(RDS_HOBJECT,LPSTR,LPSTR);
typedef BOOL (RDSCALL *RDS_BHoIIpI)(RDS_HOBJECT,int,int,int*);
typedef BOOL (RDSCALL *RDS_BHoI)(RDS_HOBJECT,int);
typedef RDS_VHANDLE (RDSCALL *RDS_VhVhiVd)(RDS_VHANDLE,int,RDS_PVARDESCRIPTION);
typedef RDS_VHANDLE (RDSCALL *RDS_VhSVd)(LPSTR,RDS_PVARDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BHoIVd)(RDS_HOBJECT,int,RDS_PVARDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BHoBDwIS)(RDS_HOBJECT,BOOL,DWORD,int,LPSTR);
typedef LPSTR (RDSCALL *RDS_SHoII)(RDS_HOBJECT,int,int);
typedef void (RDSCALL *RDS_VHoIII)(RDS_HOBJECT,int,int,int);
typedef LPSTR (RDSCALL *RDS_SVhBIpI)(RDS_VHANDLE,BOOL,int,int*);
typedef int (RDSCALL *RDS_IHoIS)(RDS_HOBJECT,int,LPSTR);
typedef BOOL (RDSCALL *RDS_BHoS)(RDS_HOBJECT,LPSTR);
typedef BOOL (RDSCALL *RDS_BHwSHbHiS)(HWND,LPSTR,HBITMAP,HINSTANCE,LPSTR);
typedef void (RDSCALL *RDS_VHw)(HWND);
typedef LPSTR (RDSCALL *RDS_SHoS)(RDS_HOBJECT,LPSTR);
typedef RDS_VHANDLE (RDSCALL *RDS_VhIVd)(int,RDS_PVARDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BVhS)(RDS_VHANDLE,LPSTR);
typedef LPSTR (RDSCALL *RDS_SSSpI)(LPSTR,LPSTR,int*);
typedef void (RDSCALL *RDS_VHwS)(HWND,LPSTR);
typedef int (RDSCALL *RDS_IHoBhB)(RDS_HOBJECT,RDS_BHANDLE,BOOL);
typedef int (RDSCALL *RDS_IHoChB)(RDS_HOBJECT,RDS_CHANDLE,BOOL);
typedef void (RDSCALL *RDS_VHoDwSB)(RDS_HOBJECT,DWORD,LPSTR,BOOL);
typedef int (RDSCALL *RDS_ISSI)(LPSTR,LPSTR,int);
typedef int (RDSCALL *RDS_IHoSI)(RDS_HOBJECT,LPSTR,int);
typedef BOOL (RDSCALL *RDS_BHopSII)(RDS_HOBJECT,LPSTR*,int,int);
typedef int (RDSCALL *RDS_IHopSpSpCB)(RDS_HOBJECT,LPSTR*,LPSTR*,char*,BOOL);
typedef void (RDSCALL *RDS_VSD)(LPSTR,double);
typedef int (RDSCALL *RDS_ISpS)(LPSTR,LPSTR*);
typedef BOOL (RDSCALL *RDS_BBhIS)(RDS_BHANDLE,int,LPSTR);
typedef LPSTR (RDSCALL *RDS_SBhIpI)(RDS_BHANDLE,int,int*);
typedef BOOL (RDSCALL *RDS_BB)(BOOL);
typedef void (RDSCALL *RDS_VBhPv)(RDS_BHANDLE,LPVOID);
typedef BOOL (RDSCALL *RDS_BChIIIDw)(RDS_CHANDLE,int,int,int,DWORD);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhBd)(RDS_BHANDLE,RDS_PBLOCKDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BBhSBd)(RDS_BHANDLE,LPSTR,RDS_PBLOCKDESCRIPTION);
typedef LPSTR (RDSCALL *RDS_SHoIpI)(RDS_HOBJECT,int,int*);
typedef void (RDSCALL *RDS_VBhIII)(RDS_BHANDLE,int,int,int);
typedef BOOL (RDSCALL *RDS_BMhSDwIDwII)(RDS_MENUITEM,LPSTR,DWORD,int,DWORD,int,int);
typedef void (RDSCALL *RDS_VHoIID)(RDS_HOBJECT,int,int,double);
typedef double (RDSCALL *RDS_DHoII)(RDS_HOBJECT,int,int);
typedef void (RDSCALL *RDS_VHoSI)(RDS_HOBJECT,LPSTR,int);
typedef void (RDSCALL *RDS_VHoSD)(RDS_HOBJECT,LPSTR,double);
typedef double (RDSCALL *RDS_DHoSD)(RDS_HOBJECT,LPSTR,double);
typedef void (RDSCALL *RDS_VHoSS)(RDS_HOBJECT,LPSTR,LPSTR);
typedef LPSTR (RDSCALL *RDS_SHoSSpI)(RDS_HOBJECT,LPSTR,LPSTR,int*);
typedef int (RDSCALL *RDS_IBhSSSS)(RDS_BHANDLE,LPSTR,LPSTR,LPSTR,LPSTR);
typedef int (RDSCALL *RDS_IBh)(RDS_BHANDLE);
typedef DWORD (RDSCALL *RDS_DwBh)(RDS_BHANDLE);
typedef void (RDSCALL *RDS_VBhDwDw)(RDS_BHANDLE,DWORD,DWORD);
typedef RDS_BHANDLE (RDSCALL *RDS_BhBhIDwCb1pV)(RDS_BHANDLE,int,DWORD,RDS_BhPdPdpV,LPVOID);
typedef RDS_HOBJECT (RDSCALL *RDS_HoBIIS)(BOOL,int,int,LPSTR);
typedef void (RDSCALL *RDS_VHoIS)(RDS_HOBJECT,int,LPSTR);
typedef BOOL (RDSCALL *RDS_VHoIIDwSI)(RDS_HOBJECT,int,int,DWORD,LPSTR,int);
typedef LPSTR (RDSCALL *RDS_SSIpI)(LPSTR,int,int*);
typedef LPSTR (RDSCALL *RDS_SSSB)(LPSTR,LPSTR,BOOL);
typedef BOOL (RDSCALL *RDS_BSI)(LPSTR,int);
typedef int (RDSCALL *RDS_IIB)(int,BOOL);
typedef BOOL (RDSCALL *RDS_BI)(int);
typedef RDS_PFUNCPROVIDERLINK (RDSCALL *RDS_BFl)(int);
typedef void (RDSCALL *RDS_VI)(int);
typedef BOOL (RDSCALL *RDS_BHoCb3)(RDS_HOBJECT,RDS_VHo);
typedef BOOL (RDSCALL *RDS_BpD)(double*);
typedef BOOL (RDSCALL *RDS_BBhIDwDw)(RDS_BHANDLE,int,DWORD,DWORD);
typedef void (RDSCALL *RDS_VIS)(int,LPSTR);
typedef void (RDSCALL *RDS_VBhSI)(RDS_BHANDLE,LPSTR,int);
typedef void (RDSCALL *RDS_VIDwSS)(int,DWORD,LPSTR,LPSTR);
typedef RDS_MENUITEM (RDSCALL *RDS_MhSII)(LPSTR,int,int);
typedef void (RDSCALL *RDS_VBhSB)(RDS_BHANDLE,LPSTR,BOOL);
typedef RDS_HOBJECT (RDSCALL *RDS_HoIS)(int,LPSTR);
typedef LPSTR (RDSCALL *RDS_SSSSI)(LPSTR,LPSTR,LPSTR,int);
typedef BOOL (RDSCALL *RDS_BCb4pVI)(RDS_IpV,LPVOID,int*);
typedef LPVOID (RDSCALL *RDS_pVpVpS)(LPVOID,LPSTR*);
typedef void (RDSCALL *RDS_VSII)(LPSTR,int,int);
typedef LPVOID (RDSCALL *RDS_pVpVS)(LPVOID,LPSTR);
typedef HINSTANCE (RDSCALL *RDS_HiV)(void);
typedef void (RDSCALL *RDS_VSBII)(LPSTR,BOOL,int,int);
typedef BOOL (RDSCALL *RDS_BBhDvpV)(RDS_BHANDLE,RDS_PDYNVARLINK,LPVOID);
typedef RDS_BHANDLE (RDSCALL *RDS_BhDvCb5pV)(RDS_PDYNVARLINK,RDS_BBhDvpV,LPVOID);
typedef BOOL (RDSCALL *RDS_BSSB)(LPSTR,LPSTR,BOOL);
typedef void (RDSCALL *RDS_VHoIIpD)(RDS_HOBJECT,int,int,double*);
typedef void (RDSCALL *RDS_VHoSDpD)(RDS_HOBJECT,LPSTR,double,double*);
typedef int (RDSCALL *RDS_ISISB)(LPSTR,int,LPSTR,BOOL);
typedef int (RDSCALL *RDS_IISB)(int,LPSTR,BOOL);
typedef BOOL (RDSCALL *RDS_BIDwISpVDw)(int,DWORD,int,LPSTR,LPVOID,DWORD);
typedef BOOL (RDSCALL *RDS_BIDwISpVDwNsNb)(int,DWORD,int,LPSTR,LPVOID,DWORD,RDS_NETSTATION,RDS_NETBLOCK);
typedef LPVOID (RDSCALL *RDS_pVBhIpI)(RDS_BHANDLE,int,int*);
typedef BOOL (RDSCALL *RDS_BBhIpVI)(RDS_BHANDLE,int,LPVOID,int);
typedef void (RDSCALL *RDS_VIIDw)(int,int,DWORD);
typedef BOOL (RDSCALL *RDS_BDwpIpI)(DWORD,int*,int*);
typedef int (RDSCALL *RDS_IBhSSI)(RDS_BHANDLE,LPSTR,LPSTR,int);
typedef void (RDSCALL *RDS_VHoFsfd)(RDS_HOBJECT,RDS_PFORMSERVFUNCDATA);
typedef BOOL (RDSCALL *RDS_BHoCb6)(RDS_HOBJECT,RDS_VHoFsfd);
typedef void (RDSCALL *RDS_VBhIpVDw)(RDS_BHANDLE,int,LPVOID,DWORD);
typedef void (RDSCALL *RDS_VBhIpVDwDw)(RDS_BHANDLE,int,LPVOID,DWORD,DWORD);
typedef void (RDSCALL *RDS_VHoII)(RDS_HOBJECT,int,int);
typedef BOOL (RDSCALL *RDS_BBhB)(RDS_BHANDLE,BOOL);
typedef RDS_HOBJECT (RDSCALL *RDS_HoBhS)(RDS_BHANDLE,LPSTR);
typedef RDS_HOBJECT (RDSCALL *RDS_HoBh)(RDS_BHANDLE);
typedef LPSTR (RDSCALL *RDS_SDwS)(DWORD,LPSTR);
typedef LPSTR (RDSCALL *RDS_SVh)(RDS_VHANDLE);
typedef int (RDSCALL *RDS_IHoISSDwS)(RDS_HOBJECT,int,LPSTR,LPSTR,DWORD,LPSTR);
typedef BOOL (RDSCALL *RDS_BVhpVVhpV)(RDS_VHANDLE,LPVOID,RDS_VHANDLE,LPVOID);
typedef LPVOID (RDSCALL *RDS_pVBhIBpI)(RDS_BHANDLE,int,BOOL,int*);
typedef void (RDSCALL *RDS_VHoBhB)(RDS_HOBJECT,RDS_BHANDLE,BOOL);
typedef int (RDSCALL *RDS_ISpI)(LPSTR,int*);
typedef LPSTR (RDSCALL *RDS_SIIpI)(int,int,int*);
typedef void (RDSCALL *RDS_VSpD)(LPSTR,double*);
typedef LPSTR (RDSCALL *RDS_SDIpI)(double,int,int*);
typedef LPSTR (RDSCALL *RDS_SIII)(int,int,int);
typedef LPSTR (RDSCALL *RDS_SII)(int,int);
typedef BOOL (RDSCALL *RDS_BBhSS)(RDS_BHANDLE,LPSTR,LPSTR);
typedef int (RDSCALL *RDS_IMd)(RDS_PMOUSEDATA);
typedef void (RDSCALL *RDS_VBhBIIII)(RDS_BHANDLE,BOOL,int,int,int,int);
typedef BOOL (RDSCALL *RDS_BpVIIDwSS)(LPVOID,int,int,DWORD,LPSTR,LPSTR);
typedef BOOL (RDSCALL *RDS_BBhIIpBpBpB)(RDS_BHANDLE,int,int,BOOL*,BOOL*,BOOL*);
typedef BOOL (RDSCALL *RDS_BBhIIBBB)(RDS_BHANDLE,int,int,BOOL,BOOL,BOOL);
typedef void (RDSCALL *RDS_VBhIIII)(RDS_BHANDLE,int,int,int,int);
typedef BOOL (RDSCALL *RDS_BChBrB)(RDS_CHANDLE,RDS_PBLOCKDIMENSIONS,BOOL);
typedef BOOL (RDSCALL *RDS_BChI)(RDS_CHANDLE Conn,int LayerId);
typedef BOOL (RDSCALL *RDS_BBhDwFbei)(RDS_BHANDLE,DWORD,RDS_PFINDBYEXTIDDATA);
typedef RDS_HOBJECT (RDSCALL *RDS_HoIIIIIIS)(int,int,int,int,int,int,LPSTR);
typedef BOOL (RDSCALL *RDS_BHoPnd)(RDS_HOBJECT,RDS_PPANDESCRIPTION);
typedef BOOL (RDSCALL *RDS_BBhIpLfDwpI)(RDS_BHANDLE,int,LOGFONT*,DWORD,int*);
typedef void (RDSCALL *RDS_VpFsD)(RDS_PSERVFONTPARAMS,double);
typedef BOOL (RDSCALL *RDS_BSpSpFs)(LPSTR,LPSTR*,RDS_PSERVFONTPARAMS);
typedef LPSTR (RDSCALL *RDS_SpFspI)(RDS_PSERVFONTPARAMS,int*);
typedef void (RDSCALL *RDS_VpSSB)(LPSTR*,LPSTR,BOOL);
typedef void (RDSCALL *RDS_VMhDw)(RDS_MENUITEM,DWORD);
typedef void (RDSCALL *RDS_VSDwII)(LPSTR,DWORD,int,int);
typedef RDS_MENUITEM (RDSCALL *RDS_MhSDwII)(LPSTR,DWORD,int,int);
typedef BOOL (RDSCALL *RDS_BBhBrDw)(RDS_BHANDLE,RDS_PBLOCKDIMENSIONS,DWORD);
typedef void (RDSCALL *RDS_VHoIDwDw)(RDS_HOBJECT,int,DWORD,DWORD);
typedef LPSTR (RDSCALL *RDS_SBhS)(RDS_BHANDLE,LPSTR);
typedef LPSTR (RDSCALL *RDS_SBh)(RDS_BHANDLE);
typedef LPSTR (RDSCALL *RDS_SSpSpSIDw)(LPSTR,LPSTR*,LPSTR*,int,DWORD);
typedef LPSTR (RDSCALL *RDS_SS)(LPSTR);
typedef LPSTR (RDSCALL *RDS_SSDwSSS)(LPSTR,DWORD,LPSTR,LPSTR,LPSTR);
typedef LPSTR (RDSCALL *RDS_SSDwSpI)(LPSTR,DWORD,LPSTR,int*);
typedef LPSTR (RDSCALL *RDS_SIS)(int,LPSTR);
typedef int (RDSCALL *RDS_IBhII)(RDS_BHANDLE,int,int);
typedef BOOL (RDSCALL *RDS_BHoSCSI)(RDS_HOBJECT,LPSTR,char,LPSTR,int);
typedef BOOL (RDSCALL *RDS_BpBipBtS)(int,BITMAPINFO*,BYTE*,LPSTR);
typedef int (RDSCALL *RDS_IIEdpV)(int,RDS_PTOOLMODULEDATA,LPVOID);
typedef void (RDSCALL *RDS_VDw)(DWORD);
typedef int (RDSCALL *RDS_IDwSSIS)(DWORD,LPSTR,LPSTR,int,LPSTR);
typedef BOOL (RDSCALL *RDS_BS1d)(RDS_PSAVESYSBMPDATA);
typedef BOOL (RDSCALL *RDS_BIDwBr)(int,DWORD,RDS_PBLOCKDIMENSIONS);
typedef DWORD (RDSCALL *RDS_DwV)(void);
typedef void (RDSCALL *RDS_VIDw)(int,DWORD);
typedef BOOL (RDSCALL *RDS_BHoHoDw)(RDS_HOBJECT,RDS_HOBJECT,DWORD);
typedef void (RDSCALL *RDS_VIIB)(int,int,BOOL);
typedef BOOL (RDSCALL *RDS_BpVII)(LPVOID,int);
typedef BOOL (RDSCALL *RDS_BII)(int,int);
typedef BOOL (RDSCALL *RDS_BBhII)(RDS_BHANDLE,int,int);
typedef int (RDSCALL *RDS_ISSB)(LPSTR,LPSTR,BOOL);
typedef int (RDSCALL *RDS_ICh)(RDS_CHANDLE);
typedef int (RDSCALL *RDS_IChIpSpIpIpDpS)(RDS_CHANDLE,int,LPSTR*,int*,int*,double*,LPSTR*);
typedef int (RDSCALL *RDS_IChIpIpDpS)(RDS_CHANDLE,int,int*,double*,LPSTR*);
typedef BOOL (RDSCALL *RDS_BChIIIDS)(RDS_CHANDLE,int,int,int,double,LPSTR);
typedef BOOL (RDSCALL *RDS_BSBhBhSpV)(LPSTR,RDS_BHANDLE,RDS_BHANDLE,LPSTR,LPVOID);
typedef int (RDSCALL *RDS_IBhDwCb7pV)(RDS_BHANDLE,DWORD,RDS_BSBhBhSpV,LPVOID);
typedef int (RDSCALL *RDS_IChIpSpIpIpDpSpDw)(RDS_CHANDLE,int,LPSTR*,int*,int*,double*,LPSTR*,DWORD*);
typedef int (RDSCALL *RDS_IChIpIpDpSpDw)(RDS_CHANDLE,int,int*,double*,LPSTR*,DWORD*);
typedef BOOL (RDSCALL *RDS_BChIIIDSDw)(RDS_CHANDLE,int,int,int,double,LPSTR,DWORD);
typedef void (RDSCALL *RDS_VHoISpV)(RDS_HOBJECT,int,LPSTR,LPVOID);
typedef LPVOID (RDSCALL *RDS_pVHoIpS)(RDS_HOBJECT,int,LPSTR*);
typedef LPVOID (RDSCALL *RDS_pVBhpIDw)(RDS_BHANDLE,int*,DWORD);
typedef void (RDSCALL *RDS_VChBB)(RDS_CHANDLE,BOOL,BOOL);
typedef BOOL (RDSCALL *RDS_BBhpVIDw)(RDS_BHANDLE,LPVOID,int,DWORD);
typedef RDS_HOBJECT (RDSCALL *RDS_HoCh)(RDS_CHANDLE);
typedef void (RDSCALL *RDS_VSDw)(LPSTR,DWORD);
typedef void (RDSCALL *RDS_VSSISpVDw)(LPSTR,LPSTR,int,LPSTR,LPVOID,DWORD);
typedef BOOL (RDSCALL *RDS_BSSpIpSppVpDw)(LPSTR,LPSTR,int*,LPSTR*,LPVOID*,DWORD*);
typedef int (RDSCALL *RDS_IDwSSS2)(DWORD,LPSTR,LPSTR,RDS_PTOOLEXTCALLDATA);
typedef void (RDSCALL *RDS_VS2)(RDS_PTOOLEXTCALLDATA);
typedef BOOL (RDSCALL *RDS_BBhpIpI)(RDS_BHANDLE,int*,int*);
typedef void (RDSCALL *RDS_VHoDwSS)(RDS_HOBJECT,DWORD,LPSTR,LPSTR);
typedef int (RDSCALL *RDS_IBhIpI)(RDS_BHANDLE,int,int*);
typedef int * (RDSCALL *RDS_pIS)(LPSTR);
typedef void (RDSCALL *RDS_VBhDw)(RDS_BHANDLE,DWORD);
typedef BOOL (RDSCALL *RDS_BSIS)(LPSTR,int,LPSTR);
typedef BOOL (RDSCALL *RDS_BBhpBtrpFs)(RDS_BHANDLE,RDS_PBLOCKTEXTRECTDATA,RDS_PSERVFONTPARAMS);
typedef BOOL (RDSCALL *RDS_BBhpBtrpFsDw)(RDS_BHANDLE,RDS_PBLOCKTEXTRECTDATA,RDS_PSERVFONTPARAMS,DWORD);
typedef void (RDSCALL *RDS_VHoBhDw)(RDS_HOBJECT,RDS_BHANDLE,DWORD);
typedef void (RDSCALL *RDS_VHoChDw)(RDS_HOBJECT,RDS_CHANDLE,DWORD);
typedef void (RDSCALL *RDS_VDwSSS)(DWORD,LPSTR,LPSTR,LPSTR);
typedef BOOL (RDSCALL *RDS_BBhDw)(RDS_BHANDLE,DWORD);
typedef void (RDSCALL *RDS_VIIHb)(int,int,HBITMAP);
typedef BOOL (RDSCALL *RDS_BBhpIpIpIpIpIpI)(RDS_BHANDLE,int*,int*,int*,int*,int*,int*);
typedef RDS_HOBJECT (RDSCALL *RDS_HoS)(LPSTR);
typedef RDS_BHANDLE (RDSCALL *RDS_BhHoI)(RDS_HOBJECT,int,BOOL*);
typedef int (RDSCALL *RDS_IHoIpVDw)(RDS_HOBJECT,int,LPVOID,DWORD);
typedef void (RDSCALL *RDS_VDcIIDw)(HDC,int,int,DWORD);
typedef BOOL (RDSCALL *RDS_BBhSDwS)(RDS_BHANDLE,LPSTR,DWORD,LPSTR);
typedef RDS_BHANDLE (RDSCALL *RDS_BhIBh)(RDS_BHANDLE,int,RDS_BHANDLE);
typedef RDS_CHANDLE (RDSCALL *RDS_ChICh)(RDS_CHANDLE,int,RDS_CHANDLE);
typedef BOOL (RDSCALL *RDS_BChDwSIBII)(RDS_CHANDLE,DWORD,LPSTR,int,BOOL,int,int);
typedef BOOL (RDSCALL *RDS_BSSpSpI)(LPSTR,LPSTR,LPSTR*,int*);
typedef LPVOID (RDSCALL *RDS_pVBhpDw)(RDS_BHANDLE,DWORD*);
typedef BOOL (RDSCALL *RDS_BBhpVDw)(RDS_BHANDLE,LPVOID,DWORD);
typedef RDS_PSHAREDMEMPARAMS (RDSCALL *RDS_SmISDw)(int,LPSTR,DWORD);
typedef RDS_PSHAREDMEMPARAMS (RDSCALL *RDS_SmISS)(int,LPSTR,LPSTR);
typedef void (RDSCALL *RDS_VSm)(RDS_PSHAREDMEMPARAMS);
typedef BOOL (RDSCALL *RDS_BBhSmpV)(RDS_BHANDLE,RDS_PSHAREDMEMPARAMS,LPVOID);
typedef RDS_BHANDLE (RDSCALL *RDS_BhSmCb8pV)(RDS_PSHAREDMEMPARAMS,RDS_BBhSmpV,LPVOID);
typedef LPSTR (RDSCALL *RDS_SSIISpI)(LPSTR,int,int,LPSTR,int*);
typedef BOOL (RDSCALL *RDS_BChDwSIBIIB)(RDS_CHANDLE,DWORD,LPSTR,int,BOOL,int,int,BOOL);
typedef DWORD (RDSCALL *RDS_DwBhI)(RDS_BHANDLE,int);
typedef int (RDSCALL *RDS_IBhDw)(RDS_BHANDLE,DWORD);
typedef void (RDSCALL *RDS_VHoIIIS)(RDS_HOBJECT,int,int,int,LPSTR);
typedef LPSTR (RDSCALL *RDS_SHoIIIpB)(RDS_HOBJECT,int,int,int,BOOL*);
typedef void (RDSCALL *RDS_VHoIISDwI)(RDS_HOBJECT,int,int,LPSTR,DWORD,int);
typedef void (RDSCALL *RDS_VHoIIII)(RDS_HOBJECT,int,int,int,int);
typedef int (RDSCALL *RDS_IHoIIIpB)(RDS_HOBJECT,int,int,int,BOOL*);
typedef void (RDSCALL *RDS_VHoIIID)(RDS_HOBJECT,int,int,int,double);
typedef double (RDSCALL *RDS_DHoIIIpB)(RDS_HOBJECT,int,int,int,BOOL*);
typedef BOOL (RDSCALL *RDS_BHoIIIpD)(RDS_HOBJECT,int,int,int,double*);
typedef BOOL (RDSCALL *RDS_BHoIII)(RDS_HOBJECT,int,int,int);
//---------------------------------------------------------------------------

// Описания переменных-указателей на сервисные функции находится в
// "RdsFunc.h" и вспомогательном файле "RdsFunc_.h"
//---------------------------------------------------------------------------
#endif
