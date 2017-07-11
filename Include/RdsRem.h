//---------------------------------------------------------------------------

#ifndef RdsRemH
#define RdsRemH
//---------------------------------------------------------------------------
// Сообщение Windows для главного окна сервера и контроллера
#define RDS_WM_REMOTECALL (WM_USER+2)
//---------------------------------------------------------------------------
// Размер массива RDS_REMOTECALL_DATA::EventFlags (не менее числа событий)
#define RDS_REMOTECALL_EVENTFLAGSSIZE   255
//---------------------------------------------------------------------------

#include <pshpack1.h> // Установка плотной упаковки структур


// Данные в общей памяти (для установки связи и проверки активности)
// Данные отводятся контроллером
typedef struct
{
  DWORD servSize;       // Размер этой структуры в байтах (необходимо присвоить перед вызовом)
  HWND ServerWindow;    // Главное окно сервера, получающее RMWM_REMOTECALL при
                        // подаче команд (устанавливается сервером при запуске,
                        // сбрасывается в NULL при завершении)
  HWND ContrWindow;     // Главное окно контроллера, получающее RMWM_REMOTECALL
                        // при возникновении событий (устанавливается контроллером,
                        // сбрасывается в NULL при его завершении)
  DWORD ContrProcessId; // Идентификатор процесса-контроллера   	
  // Трубы для сервера (для обмена с контроллером) - устанавливаются контроллером
  HANDLE Command_read;  // Получение данных команды от контроллера
  HANDLE Result_write;  // Возврат результатов команды контроллеру
  HANDLE Event_write;   // Передача данных события
  // Флаги разрешенности событий (1 - разрешено, 0 - запрещено) - устанавливаются контроллером
  unsigned char EventFlags[RDS_REMOTECALL_EVENTFLAGSSIZE];
} RDS_REMOTECALL_DATA;
typedef RDS_REMOTECALL_DATA *RDS_PREMOTECALL_DATA;
//---------------------------------------------------------------------------
// Команды контроллера
#define RDS_RCMD_LOADSYSTEMFROMFILE    10       // Загрузить систему из файла
#define RDS_RCMD_STARTCALCPROCESS      11       // Запустить расчет
#define RDS_RCMD_STOPCALCPROCESS       12       // Остановить расчет
#define RDS_RCMD_SETEDITMODE           13       // Перейти в режим редактирования
#define RDS_RCMD_SETCALCMODE           14       // Перейти в режим расчета
#define RDS_RCMD_ENABLERUNINTERFACE    15       // Разрешить/запретить пользователю менять режим
#define RDS_RCMD_SHOWBLOCK             16       // Показать блок по полному имени
#define RDS_RCMD_CALLBLOCK             17       // Вызвать отклик блока по полному имени
#define RDS_RCMD_SETEXITMODE           18       // Установка разрешения выхода
#define RDS_RCMD_SETCONTROLLERNAME     19       // Установить имя контроллера
#define RDS_RCMD_SHOWMAINWINDOW        20       // Показать/спрятать главное окно
#define RDS_RCMD_MINIMIZEAPPLICATION   21       // Свернуть приложение
#define RDS_RCMD_RESTOREAPPLICATION    22       // Развернуть приложение
#define RDS_RCMD_LEAVE                 23       // Отключить связь, оставив приложение активным
#define RDS_RCMD_SAVESYSTEMTOFILE      24       // Записать систему в файл
#define RDS_RCMD_CLOSEALLEXCEPTROOT    25       // Закрыть все окна кроме главного
#define RDS_RCMD_SETSTRING             26       // Установить строку с целым идентификатором
#define RDS_RCMD_SETAUTOSAVE           27       // Включить/выключить автосохранение
#define RDS_RCMD_SETMODFLAG            28       // Установить флаг измененности схемы
#define RDS_RCMD_GETMODFLAG            29       // Получить флаг измененности схемы
#define RDS_RCMD_CLOSEAPPLICATION      30       // Завершить приложение
#define RDS_RCMD_ENABLEEDITMODE        31       // Разрешить режим редактирования
#define RDS_RCMD_ENABLECALCMODE        32       // Разрешить режим моделирования
#define RDS_RCMD_ENABLERUN             33       // Разрешить запуск расчета
#define RDS_RCMD_ENABLEPROPEDIT        34       // Разрешить редактирование параметров блоков
#define RDS_RCMD_ENABLEOPTIONS         35       // Разрешить изменение настроек RDS
#define RDS_RCMD_BRINGTOFRONT          36       // Вытащить RDS на передний план
#define RDS_RCMD_ENABLEWINREFRESH      37       // Разрешить обновление окон
  // Флаги для RDS_RCMD_ENABLEWINREFRESH
  #define RDS_RCMD_ENABLEWINREFRESH_EN  1       // Разрешить/запретить
  #define RDS_RCMD_ENABLEWINREFRESH_RC  2       // С подсистемами или нет
#define RDS_RCMD_CLOSEALLMODAL         38       // Закрыть все модальные окна
#define RDS_RCMD_CHECKMODAL            39       // Проверить наличие открытых модальных окон
#define RDS_RCMD_SETVIEWPORT           40       // Установить окно вывода
#define RDS_RCMD_RELEASEVIEWPORT       41       // Удалить окно вывода
#define RDS_RCMD_SETVPPARAMS           42       // Установить параметры окна вывода
#define RDS_RCMD_UPDATEVIEWPORT        43       // Обновить окно вывода
  // Флаги для RDS_RCMD_UPDATEVIEWPORT
  #define RDS_RCMD_UPDATEVIEWPORT_WAIT  1       // Сообщить о конце обновления
#define RDS_RCMD_GETVPSYSAREA          44       // Получить размеры рабочей области подсистемы по окну вывода
#define RDS_RCMD_SETVIEWPORTRECT       45       // Установить размеры области вывода
#define RDS_RCMD_ENABLEUI              46       // Разрешить (запретить) пользовательский интерфейс
#define RDS_RCMD_RESETCALC             47       // Сбросить расчет
#define RDS_RCMD_MOUSEOP               48       // Вызвать реакцию на "мышь" в области вывода
  // Флаги для RDS_RCMD_MOUSEOP
  //  631
  //  4268421
  // 76543210
  // sssbbboo
  #define RDS_RCMD_MOUSEOP_OPERMASK     3       // Маска операций
    #define RDS_RCMD_MOUSEOP_DOWN       0       // Нажатие кнопки
    #define RDS_RCMD_MOUSEOP_UP         1       // Отпускание конпки
    #define RDS_RCMD_MOUSEOP_MOVE       2       // Перемещение
    #define RDS_RCMD_MOUSEOP_DBL        3       // Двойной щелчок
  #define RDS_RCMD_MOUSEOP_BTNMASK      (4+8+16)// Маска кнопок
    #define RDS_RCMD_MOUSEOP_LEFT       4       // Левая кнопка
    #define RDS_RCMD_MOUSEOP_RIGHT      8       // Правая кнопка
    #define RDS_RCMD_MOUSEOP_MIDDLE     16      // Средняя кнопка
  #define RDS_RCMD_MOUSEOP_SHMASK       (32+64+128) // Маска клавиш
    #define RDS_RCMD_MOUSEOP_ALT        32      // Alt
    #define RDS_RCMD_MOUSEOP_CTRL       64      // Ctrl
    #define RDS_RCMD_MOUSEOP_SHIFT      128     // Shift
#define RDS_RCMD_GETVPPARAMS            49      // Получить параметры окна вывода
#define RDS_RCMD_GETMENUITEMS           50      // Получить элементы контекстного меню блока
  // Параметр для RDS_RCMD_GETMENUITEMS
  #define RDS_RCMD_GETMENUITEMS_BLK     0       // Меню блока
  #define RDS_RCMD_GETMENUITEMS_SYS     1       // Меню свободного места окна подсистемы
  #define RDS_RCMD_GETMENUITEMS_MAIN    2       // Меню "Система|Дополнительно"
  // Флаги для получаемых пунктов меню
  #define RDS_RCMD_GETMENUITEMSF_EN     1       // Разрешен
  #define RDS_RCMD_GETMENUITEMSF_VIS    2       // Видим
  #define RDS_RCMD_GETMENUITEMSF_CHK    4       // Помечен
  #define RDS_RCMD_GETMENUITEMSF_DIV    8       // Разделитель
  #define RDS_RCMD_GETMENUITEMSF_KEY    16      // Есть клавиша
#define RDS_RCMD_VPBLOCKATPOS           51      // Блок в указанной точке окна вывода
#define RDS_RCMD_BLOCKMENUCLICK         52      // Выбор пункта меню блока
#define RDS_RCMD_GETVPMOUSELEVEL        53      // Получить уровень реакции блоков окна на "мышь"
#define RDS_RCMD_VPPOPUPHINT            54      // Получить параметры всплывающей подсказки
#define RDS_RCMD_ENABLESYSWIN           55      // Разрешить (запретить) открытие окон подсистем
#define RDS_RCMD_VPGETBLOCKNAME         56      // Получить имя подсистемы, связанной с окном вывода
#define RDS_RCMD_LOADSYSTEMFROMMEM      57      // Загрузить систему из памяти
  // Флаги для RDS_RCMD_LOADSYSTEMFROMMEM
  #define RDS_RCMD_LOADSYSFROMMEM_SP    1       // Записывать несохраненную схему
  #define RDS_RCMD_LOADSYSFROMMEM_MODE  (2+4)   // Маска режима передачи
    #define RDS_RCMD_LOADSYSMEM_MAPPING   0     // Через разделяемую область памяти
    #define RDS_RCMD_LOADSYSMEM_PIPE      2     // Непосредственно по трубе
    #define RDS_RCMD_LOADSYSMEM_RESERVED  4     // Зарезервировано на будущее
#define RDS_RCMD_SETOPTIONS             58      // Установить параметры
#define RDS_RCMD_GETOPTIONS             59      // Получить параметры
#define RDS_RCMD_FINDBLOCK              60      // Найти блок по полному имени
#define RDS_RCMD_LISTBLOCKS             61      // Получить список блоков
#define RDS_RCMD_CLOSEALL               62      // Закрыть все окна подсистем
#define RDS_RCMD_SETREMOTESAVE          63      // Установить сохранение через ДУ
#define RDS_RCMD_GETSYSTEMCONTENT       64      // Получить текст всей системы
  #define RDS_RCMD_GETSYSCONT_PIPE      1         // Непосредственно по трубе
  #define RDS_RCMD_GETSYSCONT_FILE      2         // Через временный файл
#define RDS_RCMD_SETBLOCKVAR            65      // Установить значение переменной блока
#define RDS_RCMD_GETBLOCKVAR            66      // Получить значение переменной блока
#define RDS_RCMD_SETVPZOOMRECT          67      // Установить отображаемую часть по прямоугольнику
  #define RDS_RCMD_SETVPZR_100           1        // Флаг - размеры переданы в схемных координатах
#define RDS_RCMD_VPFIT                  68      // Установить масштаб так, чтобы вся схема влезла
#define RDS_RCMD_SETTRACELEVEL          69      // Установить уровень трассировки
#define RDS_RCMD_SETTRACEFILE           70      // Установить файл журнала трассировки
#define RDS_RCMD_CLEARSYSTEM            71      // Очистить систему
#define RDS_RCMD_SAVETAGGED             72      // Сохранить в спецформате
#define RDS_RCMD_LOADTAGGED             73      // Загрузить в спецформате
#define RDS_RCMD_SETREMOTELOAD          74      // Установить загрузку через ДУ
#define RDS_RCMD_SETPROGRESSDELAY       75      // Установить задержку события прогресса
#define RDS_RCMD_FINDOPSETPROVIDERS     76      // Найти блоки, поддерживающие поименованный набор функций ДУ
  #define RDS_RCMD_FINDOPSET_RECURSIVE    1       // Флаг - с вложенными блоками
  #define RDS_RCMD_FINDOPSET_SELF         2       // Флаг - проверять переданную подсистему
#define RDS_RCMD_DELETEXCHGSHAREDMEM    77      // Удалить дополнительную разделяемую область обмена данными
#define RDS_RCMD_SAVETAGGEDEX           78      // Сохранить в спецформате - расширенная (допускается разделяемая область)
#define RDS_RCMD_LOADTAGGEDEX           79      // Сохранить в спецформате - расширенная (допускается разделяемая область)
  #define RDS_RCMD_TAGGED_SHAREDMEM       1       // Флаг - не файл, а область памяти
  #define RDS_RCMD_TAGGED_DBLBUF          2       // При сохранении - использовать дополнительный буфер
#define RDS_RCMD_EXTIDBYNAME            80      // Получить внешний идентификатор (DWORD) блока по полному имени
#define RDS_RCMD_NAMEBYEXTID            81      // Получить полное имя блока по внешнему идентификатору (DWORD)
#define RDS_RCMD_GETMODE                82      // Получить режим работы
#define RDS_RCMD_SAVETAGGEDEXEX         83      // Сохранить в спецформате - расширенная (допускается разделяемая область, автоназвание)
#define RDS_RCMD_CLOSESYSWIN            85      // Закрыть окно подсистемы (устарела, не используется)
// 84, 86, 87 больше не используются
#define RDS_RCMD_KEYOP                  88      // Вызвать реакцию на клавиатуру в области вывода
  // События для RDS_RCMD_KEYOP
  #define RDS_RCMD_KEYOP_DOWN           0       // Нажатие кнопки
  #define RDS_RCMD_KEYOP_UP             1       // Отпускание конпки
  // Флаги для RDS_RCMD_KEYOP
  //  631
  //  4268421
  // 76543210
  // sssbbb..
  #define RDS_RCMD_KEYOP_BTNMASK      (4+8+16)// Маска кнопок мыши
    #define RDS_RCMD_KEYOP_LEFT       4       // Левая кнопка
    #define RDS_RCMD_KEYOP_RIGHT      8       // Правая кнопка
    #define RDS_RCMD_KEYOP_MIDDLE     16      // Средняя кнопка
  #define RDS_RCMD_KEYOP_SHMASK       (32+64+128) // Маска специальных клавиш
    #define RDS_RCMD_KEYOP_ALT        32      // Alt
    #define RDS_RCMD_KEYOP_CTRL       64      // Ctrl
    #define RDS_RCMD_KEYOP_SHIFT      128     // Shift
#define RDS_RCMD_TOOL                 89      // Вызвать модуль расширения
//---------------------------------------------------------------------------
// События сервера
#define RDS_REVENT_CMDRESULTREADY       0       // Служебное - можно считывать результат команды
#define RDS_REVENT_CALCSTART           10       // Процесс расчета запущен пользователем
#define RDS_REVENT_CALCSTOP            11       // Процесс расчета остановлен пользователем
#define RDS_REVENT_BLOCKMSG            12       // Сообщение от блока
#define RDS_REVENT_NEWFILE             13       // Изменился загруженный файл (загружен или создан новый)
  // Параметр для RDS_REVENT_NEWFILE
  #define RDS_REVENT_NEWFILE_NEW        0       // Создана новая система
  #define RDS_REVENT_NEWFILE_TEMPLATE   1       // Создана по шаблону
  #define RDS_REVENT_NEWFILE_LOAD       2       // Загружена из файла
#define RDS_REVENT_EDITMODE            14       // Пользователь включил режим редактирования
#define RDS_REVENT_CALCMODE            15       // Пользователь включил режим моделирования
#define RDS_REVENT_SAVEFILE            16       // Пользователь сохраняет схему (после сохранения)
#define RDS_REVENT_LOADREQUEST         17       // Пользователь запрашивает загрузку схемы (если отключена прямая загрузка)
#define RDS_REVENT_PROGRESS            18       // Прогресс загрузки/сохранения
//---------------------------------------------------------------------------
#define RDS_REMMAPPING_SIZE  (sizeof(RDS_REMOTECALL_DATA))  // Размер общей памяти
//---------------------------------------------------------------------------
// Возврат выравнивания по умолчанию
#include <poppack.h>
//---------------------------------------------------------------------------
#endif
