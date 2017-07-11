//---------------------------------------------------------------------------
// Дополнительные классы и функции для поддержки полуавтокомпилируемых блоков
// (общие функции работы с файлами, массивами и табличными функциями)
// Версия 2
//---------------------------------------------------------------------------
#ifndef L57Mk2BasicH
#define L57Mk2BasicH

#ifndef L57_NORDSCLASSES

#ifndef RdsLinInterH
  #include <RdsAC/LinInter.cpp>
#endif

#define L57_ONCEDEBUGMSG(text) static BOOL L57_msg=TRUE; if(L57_msg) { L57_msg=FALSE; rdsMessageBox(text,"Here",MB_OK); }

#endif // #ifndef L57_NORDSCLASSES

//---------------------------------------------------------------------------
class L57_FuncTransfer;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Глобальные целые переменные
//---------------------------------------------------------------------------
// Счетчик для формирования идентификатора текущей передачи текста или параметра функции
#define L57_GLOBALVAR_PARAMTRANSFERID "IPU.L57.UserFuncTransfer.ParamTransferId"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Функции блоков
//---------------------------------------------------------------------------
// Блок является источником пользовательской функции (без параметров)
#define L57_ISUSERFUNCSOURCENAME "IPU.L57.UserFuncTransfer.IsFuncSource"
//---------------------------------------------------------------------------
// Принять функцию
#define L57_ACCEPTUSERFUNCNAME  "IPU.L57.UserFuncTransfer.AcceptFunction"
// Функция обратного вызова для постановки вызова в очередь
typedef void (RDSCALL *L57_CALLFUNCLATERCB)(LPVOID,RDS_BHANDLE,LPVOID);
// Функция для постановки отложенного вызова в очередь и ее параметр
typedef struct
{ // Функция и объект
  L57_CALLFUNCLATERCB CallFunc;
  LPVOID CallObject;
  // Идентификатор передачи для проверки замыкания в кольца
  int TransferId;
} L57_AcceptFunc_Param_Queue;
// Структура параметров
typedef struct
{ DWORD servSize;
  BOOL Immediate;       // TRUE - вызов из-за редактирования функции в одном из блоков
                        // FALSE - вызов в процессе синхронизации при выходе из режима редактирования
  char **Texts;         // Тексты функций
  int TextsCount;       // Число текстов
  int TextIndex;        // Номер функции, которую нужно взять, или -1 для всех функций

  int AcceptIndex;      // Номер функции, которую нужно установить, или -1 для всех функций

  int DestVarNum;       // Номер переменной-функционального входа (если нужно)

  L57_AcceptFunc_Param_Queue *Queue; // Структура постановки вызова в очередь

} L57_AcceptFunc_Param;
//---------------------------------------------------------------------------
// Принять параметр
#define L57_ACCEPTUSERPARAMNAME "IPU.L57.UserFuncTransfer.AcceptParameter"
// Набор имен парамеров для регистрации через rdsRegisterUserName
#define L57_VARNAMESET          "IPU.L57.FuncParamNames"
// Условные идентификаторы суффиксов (все >=0!)
#define L57_VARSUFFIX_NONE      0       // нет
#define L57_VARSUFFIX_0         1       // "_0"
#define L57_VARSUFFIX_MIN       2       // "_min"
#define L57_VARSUFFIX_MAX       3       // "_max"
#define L57_VARSUFFIX_STEP      4       // "_step"
#define L57_VARSUFFIX_END       5       // "_end"
// Функция обратного вызова информационной передачи
typedef void (RDSCALL *L57_VARLISTCB)(LPVOID,int,int,BOOL,RDS_BHANDLE);
// Функция обратного вызова для постановки вызова в очередь
typedef void (RDSCALL *L57_CALLLATERCB)(LPVOID,RDS_BHANDLE,int);
// Структура параметров
typedef struct
{ DWORD servSize;
  int NameId;   // Идентификатор зарегистрированного имени параметра
  int SuffixId; // Условный идентификатор суффикса
  void *Data;   // Указатель на данные
  int Type;     // Тип передаваемой переменной (L57_APPTYPE_*)
    #define L57_APPTYPE_DOUBLE  0       // double (Data - double *)
    #define L57_APPTYPE_RDSMATR 1       // Матрица double RDS (Data - указатель на ячейку в дереве переменных)
    #define L57_APPTYPE_BLOCK   2       // Информационная передача (Data - NULL)
  int DestVarNum;       // Номер переменной-функционального входа, на который пришел параметр
  // Для информационной передачи
  RDS_BHANDLE SourceBlock;      // Блок-источник параметра
  RDS_BHANDLE DestinationBlock; // Блок, запросивший проверку передачи параметров к себе
  // Функция обратного вызова и ее параметр-объект для записи дошедших параметров
  LPVOID Object;
  L57_VARLISTCB InfFunc;
  // Функция для постановки отложенного вызова в очередь и ее параметр
  L57_CALLLATERCB CallFunc;
  LPVOID CallObject;
  // Идентификатор передачи для проверки замыкания в кольца
  int TransferId;
} L57_AcceptParam_Param;
//---------------------------------------------------------------------------
// Проверить, передается ли параметр по функциональной связи (УСТАРЕВШАЯ)
#define L57_CHECKPARAMNAMETRANS "IPU.L57.UserFuncTransfer.IsParameterTransferredWithFunc"
typedef struct
{ DWORD servSize;
  int FuncId;           // Идентификатор вызванной с этим параметром функции
  int ParamCount;       // Число параметров в массивах ниже
  int *NameIds;         // Идентификаторы имен
  int *SuffixIds;       // Идентификаторы суффиксов
  int *Flags;           // Флаги передачи (L57_PTR_*)
    #define L57_PTR_UNKNOWN     0       // Неизвестно
    #define L57_PTR_TRANS       1       // Передается
    #define L57_PTR_NOTRANS     2       // Не передается
  RDS_BHANDLE ThisBlock; // Для данного блока: если есть связь к параметру, флаг сбрасывается в L57_PTR_NOTRANS
                         // Для других блоков: если есть связь к параметру, флаг взводится в L57_PTR_TRANS
} L57_CheckParamConn_Param;
//---------------------------------------------------------------------------
// Проверить передачу параметров
#define L57_PREPARECHECKNAME "IPU.L57.UserFuncTransfer.CheckParamTransfer"
typedef struct
{ DWORD servSize;
  int Mode;     // Режим
    #define L57_CTM_PREPARE     0       // Подготовка (создание массивов)
    #define L57_CTM_CHECK       1       // Выполнить информационную передачу через L57_ACCEPTUSERPARAMNAME
  RDS_BHANDLE DestinationBlock; // Блок, запросивший проверку передачи параметров к себе

  // Функция обратного вызова и ее параметр-объект для записи дошедших параметров
  LPVOID Object;
  L57_VARLISTCB InfFunc;
} L57_CheckTranser_Param;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Метка порядка переменных в экспортированном тексте функции
#define L57_ORDERLABEL  "#ORDER#"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Вспомогательный класс для прерывания длинных циклов вычисления
//---------------------------------------------------------------------------
class L57_LoopBreak
{ public:
    DWORD Start;

    inline void Set(void){Start=GetTickCount();};
    inline BOOL Check(void){return (GetTickCount()-Start>2000);};

    L57_LoopBreak(void){Set();};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Операции с файлом
//---------------------------------------------------------------------------
class L57_File
{ protected:
    HANDLE Handle;

    int _WindowsWrite(const void *data,size_t *size);
    int _WindowsRead(void *data, size_t *size);
  public:
    void Close(void); // Закрыть файл
    int OpenWrite(char *path); // Открыть на запись
    int OpenRead(char *path); // Открыть на чтение
    int OpenAppend(char *path); // Открыть на дозапись
    int WindowsWrite(const void *data,size_t size); // Писать в файл
    int WindowsRead(void *data, size_t size); // Читать из файла
    int WindowsGetFileSizeShort(DWORD *psize,int *pTooLong=NULL); // Получить размер файла (короткий)

    // Считать короткий файл в динамически отведенный буфер (осв. rdsFree)
    char * ReadFileToMemory(char *path,DWORD *psize=NULL,int addzerobyte=1);
    // Записать область памяти в файл
    int WriteMemoryToFile(char *path,void *src,DWORD size);
    // Записать текст в файл
    int WriteTextToFile(char *path,char *src,DWORD len=0);


    L57_File(void){Handle=INVALID_HANDLE_VALUE;};
    ~L57_File(){Close();};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Строка
//---------------------------------------------------------------------------
class L57_String
{ private:
    char *Data;                 // Хранимая строка
    static char ZeroByte;       // Нулевой байт (для пустой строки)
  public:

    // Очистить данные
    inline void Clear(void){rdsFree(Data);Data=NULL;};
    // Внутренняя строка
    inline char *c_str(void)const{ZeroByte=0;return Data?Data:(&ZeroByte);};
    // Длина
    inline int Length(void)const{return Data?strlen(Data):0;};
    // Пустота
    inline int IsEmpty(void) const {return Data==NULL || (*Data)==0;};
    inline int IsNotEmpty(void) const {return !IsEmpty();};
    // Содержит только пробелы и переводы строк
    int IsBlank(void)const;
    inline int IsNotBlank(void){return !IsBlank();};
    // Присвоить
    void Assign(const char *s);
    void Assign(const char *s,int n);
    inline void Assign(const L57_String &s){Assign(s.c_str());};
    // Добавить к строке
    inline void AddString(char *s){rdsAddToDynStr(&Data,s,TRUE);};
    inline void AddString(const L57_String &s){AddString(s.c_str());};

    // Присвоить динамическую строку RDS
    inline void AssignRdsDynamicString(char *s){Clear();Data=s;};
    // Отсоединить динамическую строку RDS
    inline char *DetachRdsDynamicString(void){char *d=Data; Data=NULL; return d;};

    // Преобразование числа int в строку
    void AssignInt(int i);
    // Преобразование числа double в строку
    void AssignDouble(double d,int dec=-1);

    // Добавить int к строке
    void AddInt(int i);
    // Добавить double к строке
    void AddDouble(double d,int dec=-1);

    // Обменяться данными
    void Exchange(L57_String *pStr);

    // Совпадают ли строки
    inline int IsEqual(char *s)const{ZeroByte=0;return strcmp(s?s:(&ZeroByte),c_str())==0;};
    inline int IsEqual(const L57_String &s)const{return IsEqual(s.c_str());};
    inline int IsNotEqual(char *s)const{return !IsEqual(s);};
    inline int IsNotEqual(const L57_String &s)const{return !IsEqual(s);};

    // Операторы сравнения
    int operator==(const L57_String &s)const{return IsEqual(s);};
    int operator==(char *s)const{return IsEqual(s);};
    int operator!=(const L57_String &s)const{return !IsEqual(s);};
    int operator!=(char *s)const{return !IsEqual(s);};

    // Операторы присваивания
    L57_String & operator=(const L57_String &s){Assign(s);return *this;};
    L57_String & operator=(char *s){Assign(s);return *this;};

    // Операторы сложения
    L57_String operator+(const L57_String &s)const{return L57_String(Data,s.c_str());};
    L57_String operator+(char *s)const{return L57_String(Data,s);};

    // Операторы сложения с присваиванием
    L57_String & operator+=(const L57_String &s){AddString(s);return *this;};
    L57_String & operator+=(char *s){AddString(s);return *this;};

    // Конструкторы
    L57_String(void){Data=NULL;};
    L57_String(char *s){Data=rdsDynStrCopy(s);};
    L57_String(const L57_String &s){Data=rdsDynStrCopy(s.c_str());};
    L57_String(char *s1,char *s2){Data=rdsDynStrCat(s1,s2,TRUE);};


    // Деструктор
    ~L57_String(){Clear();};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Массив строк
//---------------------------------------------------------------------------
class L57_StringArray
{ private:
    L57_String **Data;  // Массив указателей на динамически создаваемые L57_String
    int _Count;         // Размер массива
  public:
    // Число строк в массиве
    inline int Count(void){return _Count;};
    // Строка из массива без проверки
    inline L57_String &ItemFAST(int n){return *(Data[n]); };
    inline L57_String *ItemPtrFAST(int n){return Data[n]; };

    // Очистить
    void Clear(void);
    // Создать элемент с заданным номером, если его нет
    void CreateItem(int n);

    // Установить размер
    inline void SetSize(int n){CreateItem(n-1);};

    // Сравнить
    BOOL IsEqualTo(L57_StringArray *pArray);
    BOOL IsEqualTo(char **array,int count);

    // Есть ли в списке непустые строки, отличающиеся от переданного
    BOOL HasDifferentNonEmpty(char **array,int count);

    // Найти строку
    int IndexOf(char *str);
    inline int IndexOf(L57_String &str){return IndexOf(str.c_str());};
    inline int IndexOf(L57_String *pstr){return pstr?IndexOf(pstr->c_str()):-1;};

    // Присвоить строку (разбивается по /n)
    void Assign(char *str);

    L57_StringArray(void){Data=NULL;_Count=0;};
    ~L57_StringArray(){Clear();};
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Массив целых чисел (владеет своими данными)
//---------------------------------------------------------------------------
class L57_IntArray
{ public:
    int *Items; // Элементы
    int Size;   // Размер
    int Step;   // Шаг увеличения размера
    int Count;  // Число

    // Добавить элемент
    void Add(int x);
    // Добавить, если такого еще не было
    void AddIfNew(int x);
    // Найти элемент
    int IndexOf(int x);
    // Очистить
    void Clear(void);
    // Отвести заданного размера
    void Init(int size,int initvalue);
    // Удалить элемент
    void DeleteByIndex(int index);
    // Удалить первое найденное значение
    void DeleteByValue(int value){DeleteByIndex(IndexOf(value));};

    // Записать в строку список через запятую
    void ToString(L57_String *pStr);

    // Установить элемент (расширив массив, если нужно)
    void SetElement(int index,int value);

    // Конструктор
    L57_IntArray(void){Items=NULL; Size=Count=0; Step=10;};
    // Деструктор
    ~L57_IntArray(){ rdsFree(Items); }
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Массив вещественных чисел (владеет своими данными)
//---------------------------------------------------------------------------
class L57_DblArray
{ public:
    double *Data;       // Данные
    int Count;          // Размер массива

    int PrevCount;      // Прошлый размер массива (устанавливается снаружи, в самом классе не обрабатывается)

    // Пустой?
    inline BOOL IsEmpty(void){return Data==NULL || Count==0;};
    // Не пустой?
    inline BOOL IsNotEmpty(void){return Data!=NULL && Count!=0;};

    // Очистить
    void Clear(void);
    // Отвести
    BOOL Allocate(int n);
    // Поменяться данными с другим массивом
    void Exchange(L57_DblArray &array);
    // Заполнить данными (или их частью) из другого массива
    void FillDataFrom(const L57_DblArray &array);

    L57_DblArray(void){Data=NULL;Count=0;};
    ~L57_DblArray(){Clear();};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Данные вещественной матрицы для проверки изменений
//---------------------------------------------------------------------------
class L57_DblMatrTempStorage
{ private:
    int Rows,Cols; // Размеры
    int Size;      // Общий размер
    double *Data;  // Массив данных

    BOOL Allocate(int rows,int cols);   // Отвести данные заданного размера
  public:

    // Очистить все
    inline void Clear(void){rdsFree(Data);Data=NULL;Rows=Cols=Size=0;};
    #ifndef L57_NORDSCLASSES
    // Сравнить с переменной блока
    BOOL CompareMatr(rdsbcppVarAncestor *matr,RDS_ARRAYACCESSDATA *pAccData=NULL);
    // При взведенном сигнале сравнить с переменной блока, если не совпадают
    // запомнить новые данные и взвести выходной флаг
    // Возврат - наличие изменений в данной матрице
    BOOL Check(char *insignal,rdsbcppVarAncestor *matr,BOOL &outchanges,BOOL forcechange=FALSE);
    #else
    // Сравнить с переменной блока
    BOOL CompareMatrAndCopy(void *matr);
    #endif

    L57_DblMatrTempStorage(void){Rows=Cols=Size=0;Data=NULL;};
    ~L57_DblMatrTempStorage(){Clear();};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Параметр функции или его вариант с суффиксом для приема параметров через связь функции
//---------------------------------------------------------------------------
class L57_FuncTransferParameter
{ public:
    int NameId;         // Идентификатор имени (регистрируется в RDS)
    int SuffixId;       // Идентификатор суффикса (L57_VARSUFFIX_*)
    BOOL Matrix;        // TRUE - матрица double, FALSE - число double

    int BlockVarNum;    // Номер входа блока для этого параметра (-1 - нет)
    BOOL HasConnection; // К входу этого параметра подключена связь
    RDS_BHANDLE ConnBlock; // Блок, от которого идет связь

    // Идентификаторы для замены при передаче
    int ChangedNameId;  // Выходной идентификатор имени
    BOOL ClearSuffix;   // Очистить суффикс при передаче

    L57_String SenderBlockNames; // Имена блоков, от которых приходит параметр (при проверке)

    // Куда записывать принятые параметры (и откуда брать передаваемые)
    #ifdef L57_NORDSCLASSES
      int ChangeSignalOffset;   // Смещение к сигналу изменения
      int VarOffset;            // Смещение к самой переменной
      // Данные для проверки изменений
      double OldValue;
      L57_DblMatrTempStorage OldMatrix;

      void ClearOldData(void)  // Очистить старые значения
        { OldValue=rdsbcppHugeDouble; OldMatrix.Clear(); };
    #else
      rdsbcppVarAncestor *pRdsVar;      // Статическая переменная автокомпилируемого блока (число или матрица)
      double *pPlainVar;                // Поле класса
    #endif

    // Копировать NameId и SuffixId в L57_AcceptParam_Param
    void CopyIds(L57_AcceptParam_Param *param);

    L57_FuncTransferParameter(void)
      { NameId=SuffixId=0; Matrix=HasConnection=FALSE; BlockVarNum=-1;
        ChangedNameId=-1; ClearSuffix=FALSE;
        #ifdef L57_NORDSCLASSES
          ChangeSignalOffset=VarOffset=0;
          OldValue=rdsbcppHugeDouble;
        #else
          pRdsVar=NULL;
          pPlainVar=NULL;
        #endif
        ConnBlock=NULL;
      };
    ~L57_FuncTransferParameter()
      { };
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Элемент связного списка вызовов функций блоков для передачи функций или их параметров
//---------------------------------------------------------------------------
class L57_TransferCallsList;
class L57_TransferCall
{ public:
    RDS_BHANDLE Block;  // Блок
    int DestVarNum;     // Номер переменной-функционального входа

    L57_TransferCall *Prev,*Next; // Соседи по списку
    L57_TransferCallsList *Parent;// Контейнер

    // Параметры при передаче функций
    BOOL Immediate;     // TRUE - вызов из-за редактирования функции в одном из блоков
                        // FALSE - вызов в процессе синхронизации при выходе из режима редактирования
    char **Texts;       // Тексты функций (динамический массив, элементы освобождаются rdsFree)
    int TextsCount;     // Число текстов
    int TextIndex;      // Номер функции, которую нужно взять, или -1 для всех функций
    int AcceptIndex;    // Номер функции, которую нужно установить, или -1 для всех функций

    void Extract(void); // Вырезать из контейнера
    void ConnectLast(L57_TransferCallsList *c); // Присоединить к новому контейнеру (в конец)

    // Копировать из параметра функции блока
    void CopyFrom(L57_AcceptFunc_Param *param);

    L57_TransferCall(void)
      { Block=NULL; DestVarNum=-1; Prev=Next=NULL; Parent=NULL; Texts=NULL; TextsCount=0;
      };
    ~L57_TransferCall();
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Связный список вызовов функций блоков для передачи функций или их параметров
//---------------------------------------------------------------------------
class L57_TransferCallsList
{ public:
    L57_TransferCall *First,*Last; // Список

    void Clear(void); // Очистить список
    void AddParamTransfer(RDS_BHANDLE block,int destvarnum); // Добавить вызов передачи параметров
    void AddFuncTransfer(RDS_BHANDLE block,L57_AcceptFunc_Param *param); // Добавить вызов передачи функции

    L57_TransferCallsList(void){First=Last=NULL;};
    ~L57_TransferCallsList(){Clear();};
};

//---------------------------------------------------------------------------
// Набор параметров функции, которые может принимать блок
//---------------------------------------------------------------------------
class L57_FuncTransfer
{ private:
    // Массив параметров
    L57_FuncTransferParameter **Params;
    int ParamsCount;    // Реальное число
    int ParamsSize;     // Отведенное число

    // Связный список вызовов функций блоков при передаче параметров (для разгрузки стека)
    L57_TransferCallsList Calls;

    // Указатель на глобальную переменную для генерации идентификаторов передачи параметра
    int *ParamTransferIdPtr;

    // Выполнить отложенные вызовы передачи параметров
    void DoDelayedTransferCalls(L57_AcceptParam_Param *param);
  public:
    // Набор регистрируемых имен параметров
    int varnames_set;
    // Идентификатор функции передачи параметров
    int transfer_func;
    // Идентификатор функции передачи функций
    int func_transfer_func;

    // Связный список вызовов функций блоков при передаче функций (для разгрузки стека)
    L57_TransferCallsList FuncCalls;

    // Идентификатор последней выполненной передачи параметра или функции
    int LastTransferId;

    // Получить очередной идентификатор для передачи параметра или функции
    int GenerateParamTransferId(void);

    // Список блоков-получателей
    RDS_HOBJECT DestBlocks;     // Список блоков
    L57_IntArray DestInputs;    // Список номеров входов

    // Число параметров
    inline int GetCount(void){return ParamsCount;};
    // Параметр по номеру без проверки
    inline L57_FuncTransferParameter *ParamFAST(int n){return Params[n];};

    // Очистить параметры
    void ClearParameters(void);
    // Увеличить размер массива параметров
    void EnlargeParameters(int newsize);

    // Добавить параметр (пустой)
    L57_FuncTransferParameter * AddParameter(void);

    // Добавить параметр
    L57_FuncTransferParameter * AddParameter(int nameid,int suffixid,BOOL matrix,int varnum);
    L57_FuncTransferParameter * AddParameter(char *name,int suffixid,BOOL matrix,int varnum);
    #ifdef L57_NORDSCLASSES
      L57_FuncTransferParameter * AddParameter(int nameid,int suffixid,BOOL matrix,int varnum,int signaloffset,int varoffset);
    #else
      L57_FuncTransferParameter * AddParameter(char *name,int suffixid,BOOL matrix,int varnum,double *plainvar);
      L57_FuncTransferParameter * AddParameterRds(char *name,int suffixid,BOOL matrix,int varnum,rdsbcppVarAncestor *rdsvar);
    #endif

    // Найти параметр
    L57_FuncTransferParameter * FindParameter(int nameid,int suffixid);

    #ifdef L57_NORDSCLASSES
      // Очистить старые значения
      void ClearOldData(void);
      // Проверить изменения и передать параметры
      void CheckInputsAndTransfer(void *vardata);
      // Сбросить флаги наличия соединений у всех параметров с заданным основным именем
      void ResetConnFlagsForName(char *name);
    #endif


    // Определить наличие связей у входных параметров
    void CheckInputConnections(RDS_BHANDLE block);
    // Составить список блоков на входных связях у входных параметров
    void ListInputConnections(RDS_BHANDLE block,LPVOID Object,L57_VARLISTCB Func);
    // Создать список блоков-получателей
    void ListFunctionDestinations(RDS_BHANDLE block,int outnum);
    // Сбросить имена блоков-источников
    void ResetSenderBlockNames(void);

    // Универсальная реакция на функцию проверки передачи параметров
    BOOL CheckTransferReaction(RDS_BHANDLE block,int funcoutputnum,L57_CheckTranser_Param *param);

    // Тестовая передача параметра блокам-получателям
    void TestTransferParameter(RDS_BHANDLE block,L57_FuncTransferParameter *p,L57_CheckTranser_Param *cparam);

    // Передать параметр блокам-получателям
    #ifndef L57_NORDSCLASSES
      void TransferParameter(RDS_BHANDLE block,L57_FuncTransferParameter *p,RDS_HOBJECT DestBlocks,BOOL checkconn=TRUE);
    #else
      void TransferParameter(void *varaddr,L57_FuncTransferParameter *p,RDS_HOBJECT DestBlocks,BOOL checkconn=TRUE);
    #endif

    // Переслать принятый параметр блокам-получателям, заменив в нем имя
    void RetransferParameter(L57_AcceptParam_Param* Param,int newnameid,int newsuffixid=-1);


    // Принять  параметр
    // Возврат - есть такой параметр и он изменился
    BOOL AcceptParam(L57_AcceptParam_Param* Param,RDS_BHANDLE thisblock=NULL);
    // Проверить, есть ли передача указанных параметров по связи здесь или ранее (УСТАРЕВШАЯ)
    void CheckTransferParam(RDS_BHANDLE Block,L57_CheckParamConn_Param* Param,int funcin_num);

    // Поставить в очередь вызов блока для передачи функции
    void QueueAcceptFuncCall(RDS_BHANDLE block,L57_AcceptFunc_Param *param);
    // Выполнить отложенные вызовы передачи функций
    void DoDelayedFuncTransferCalls(L57_AcceptFunc_Param_Queue *queue);

    L57_FuncTransfer(void);
    ~L57_FuncTransfer()
      { ClearParameters();
        rdsDeleteObject(DestBlocks);
      };
};
//---------------------------------------------------------------------------


#ifndef L57_NORDSCLASSES
// Массив double RDS (ссылка на внешние данные)
class L57_RdsDoubleArray
{ private:
    double *Array;
    int Count;
    double DefVal;
  public:
    inline void Setup(double *a,int cnt=0,double def=0.0){Array=a;Count=cnt;DefVal=def;};
    void Setup(rdsbcppVarAncestor &var,double def=0.0);
    inline int Size(void){return Count;};
    #ifdef RDSBCPP_INDEXCHECK
      inline double operator[](int i)const{ return (Array!=NULL && i>=0 && i<Count)?Array[i]:DefVal; };
      inline double operator[](double di)const{ int i=(int)di; return (Array!=NULL && i>=0 && i<Count)?Array[i]:DefVal; };
    #else
      inline double operator[](int i)const{ return Array[i]; };
      inline double operator[](double di)const{ return Array[(int)di]; };
    #endif

    L57_RdsDoubleArray(void){Array=NULL;Count=0;DefVal=0.0;};
};
//---------------------------------------------------------------------------

#ifdef RDSBCPP_INDEXCHECK
// Вспомогательный класс для матрицы double с проверкой индексов
class L57_RdsDoubleMatrix_aux
{ private:
    double *Array;
    int Count;
    double DefVal;
  public:
    inline double operator[](int c)const{ return (Array!=NULL && c>=0 && c<Count)?Array[c]:DefValue; };
    inline double operator[](double dc)const{int c=(int)dc; return (Array!=NULL && c>=0 && c<Count)?Array[c]:DefValue; };

    L57_DoubleMatrix_aux(double *a,int c,double def){Array=a;Count=c;DefVal=def;};
};
//---------------------------------------------------------------------------
#endif

// Матрица double RDS (ссылка на внешние данные)
class L57_RdsDoubleMatrix
{ private:
    double *Array;
    int _Rows,_Cols;
    double DefVal;
  public:
    inline void Setup(double *a,int rows=0,int cols=0,double def=0.0){Array=a;_Rows=rows;_Cols=cols;DefVal=def;};
    void Setup(rdsbcppVarAncestor &var,double def=0.0);
    inline int Rows(void){return _Rows;};
    inline int Cols(void){return _Cols;};
    #ifdef RDSBCPP_INDEXCHECK
      inline L57_RdsDoubleMatrix_aux operator[](int r)const
        { return L57_RdsDoubleMatrix_aux((Array!=NULL && r>=0 && r<_Rows && _Cols>0)?(Array+r*_Cols):NULL,
                                      _Cols,DefValue); };
      inline L57_RdsDoubleMatrix_aux operator[](double dr)const
        { int r=(int)dr; return L57_RdsDoubleMatrix_aux((Array!=NULL && r>=0 && r<_Rows && _Cols>0)?(Array+r*_Cols):NULL,
                                      _Cols,DefValue); };
    #else
      inline double * operator[](int r){return Array+r*_Cols;};
      inline double * operator[](double dr){return Array+((int)dr)*_Cols;};
    #endif
    L57_RdsDoubleMatrix(void){Array=NULL;_Rows=_Cols=0;DefVal=0.0;};
};
//---------------------------------------------------------------------------

// Интерполяция функции одной переменной
class L57_OneArgFunc
{ private:
    TRdsLinearInterpolation Engine;
  public:
    // Настройка
    inline void Setup(rdsbcppVarAncestor &M){ Engine.Setup(M,RDSLININTER_T_SINGLE); Engine.SetDefValue(0.0); };
    // Вычисление
    inline double operator()(double x){ return Engine.Calc_Fx(x,0,1); };
    inline double operator()(double x,int yindex){ return Engine.Calc_Fx(x,0,yindex); };
    // Вычисление с указанием столбца y
    inline double Calc(double x,int yindex=1){ return Engine.Calc_Fx(x,0,yindex); };

    L57_OneArgFunc(void){};
};
//---------------------------------------------------------------------------

// Интерполяция функции двух переменной
class L57_TwoArgFunc
{ private:
    TRdsLinearInterpolation Engine;
  public:
    // Настройка
    inline void Setup(rdsbcppVarAncestor &M){ Engine.Setup(M,RDSLININTER_T_FIELD); Engine.SetDefValue(0.0); };
    // Вычисление
    inline double operator()(double x1,double x2){ return Engine.Calc_Fx1x2(x1,x2); };

    L57_TwoArgFunc(void){};
};
//---------------------------------------------------------------------------

// Описание переноса данных из временного массива в выходную матрицу (элемент связного списка)
class L57_TableFiller
{ public:
    L57_TableFiller *Next;      // Следующий в списке
    rdsbcppVarAncestor *Matr;   // Матрица, в которую переносятся данные
    char *pReady;               // Логическая переменная готовности (если есть)

    int Count;                  // Число столбцов результатов в матрице одной переменной
    int *MatrCol;               // Массив индексов столбцов матрицы (если матрица - функция одной переменной)
    int *ResultN;               // Номера результатов во временном массиве для матрицы функции одной переменной

    int TwoArgResult;           // Номер результата во временном массиве для матрицы функции двух переменных
    int Transp;                 // Транспонировать матрицу при переносе (1) или нет (0) для матрицы функции двух переменных

    // Очистить все
    void Clear(void){rdsFree(MatrCol);rdsFree(ResultN);MatrCol=ResultN=NULL;Matr=NULL;pReady=NULL;TwoArgResult=-1;Count=0;};
    // Отвести массивы для матрицы функции одной переменной
    void AllocateArrays1Arg(int resultscount);

    L57_TableFiller(void){Next=NULL;Matr=NULL;MatrCol=ResultN=NULL;TwoArgResult=-1;Count=0;pReady=NULL;Transp=0;};
    ~L57_TableFiller(){Clear();};
};
//---------------------------------------------------------------------------

// Набор описаний переноса данных
class L57_TableFillProgram
{ public:
    L57_TableFiller *First;     // Начало связного списка

    // Очистить все
    void Clear(void);
    // Добавить перенос данных для матрицы функции одной переменной
    L57_TableFiller *Add1Arg(rdsbcppVarAncestor *matr,char *pReady,int resultscount);
    // Добавить перенос данных для матрицы функции двух переменных
    L57_TableFiller *Add2Arg(rdsbcppVarAncestor *matr,char *pReady,int resultnum,int transp=0);

    L57_TableFillProgram(void){First=NULL;};
    ~L57_TableFillProgram(){Clear();};
};
//---------------------------------------------------------------------------

// Формируемая табличная функция одной переменной (хранилище временных данных)
class L57_Table1ArgFuncData
{ protected:
    L57_DblArray _ArgsArray;// Массив отсчетов аргумента (собственный)
  public:
    L57_DblArray *ArgsArray;// Массив отсчетов аргумента (рабочий)
    int ArgIndex;       // Индекс аргумента (для внешних целей)

    int ResArgCount;    // Количество отсчетов аргумента в массиве результатов
    int NResults;       // Количество результатов
    double *Results;    // Массив результатов (всегда свой), записываются группами по NResults, размер NResults*ArgCount

    int _CurIndex;      // Индекс текущего отсчета аргумента

    BOOL OutputAllocError; // Произошла ошибка при переписи данных в выходную матрицу

    L57_TableFillProgram Output; // Программа переноса данных

    // Очистить все
    void Clear(void);
    // Отвести массивы результатов по ArgCount и NResults
    BOOL AllocateResultArrays(void);
    // База результата _CurIndex
    inline double *CurIndexBase(void){return Results+_CurIndex*NResults;};
    // База заданного результата
    inline double *ResultBaseForArgN(int argn){return Results+argn*NResults;};

    // Добавить результаты для добавленных в массив агрументов (добавленными считаются все, начиная с PrevCount)
    // Возврат - число добавленных точек или -1 при ошибке
    int AddResultsFromPrevCount(void);

    // Переписать результаты в матрицы от 0 до _CurIndex
    BOOL SendToOutput(BOOL tocurindex);

    L57_Table1ArgFuncData(void)
      { /*PrevArgCount=*/ResArgCount=NResults=0;ArgIndex=-1;
        Results=NULL;_CurIndex=0;OutputAllocError=FALSE;
        ArgsArray=&_ArgsArray;};
    ~L57_Table1ArgFuncData()
      { Clear(); };
};
//---------------------------------------------------------------------------

// Формируемая табличная функция двух переменных (хранилище временных данных)
class L57_Table2ArgFuncData
{ protected:
    L57_DblArray _Args1Array;// Массив отсчетов аргумента (собственный)
    L57_DblArray _Args2Array;// Массив отсчетов аргумента (собственный)
  public:
    L57_DblArray *Args1Array;// Массив отсчетов аргумента (рабочий)
    int Arg1Index;      // Индекс аргумента (для внешних целей)

    L57_DblArray *Args2Array;// Массив отсчетов аргумента (рабочий)
    int Arg2Index;      // Индекс аргумента (для внешних целей)

    int ResArg1Count;   // Количество отсчетов аргумента 1 в массиве результатов
    int ResArg2Count;   // Количество отсчетов аргумента 2 в массиве результатов
    int NResults;       // Количество результатов
    double **Results;   // Массив результатов (всегда свой)
                        // Элемент массива Results[i] - массив данных по второму аргументу
                        // при первом аргументе i. Данные в Results[i] идут группами по NResults,
                        // размер этого массива - NResults*Arg2Count. Размер Results - Arg1Count
                        // Результат n для [r,c]: Results[r][c*NResults+n]

    int _CurIndex1,     // Индексы текущего отсчета аргументов
        _CurIndex2;

    BOOL OutputAllocError; // Произошла ошибка при переписи данных в выходную матрицу

    L57_TableFillProgram Output; // Программа переноса данных

    // Произведение количеств индексов (всего отсчетов)
    inline int TotalArgs(void){return Args1Array->Count*Args2Array->Count;};

    // Очистить все
    void Clear(void);
    // Отвести массивы результатов по Arg*Count и NResults
    BOOL AllocateResultArrays(void);
    // База результата _CurIndex1,_CurIndex2
    inline double *CurIndexBase(void){return Results[_CurIndex1]+_CurIndex2*NResults;};
    // База результата index1,index2
    inline double *IndexBase(int index1,int index2){return Results[index1]+index2*NResults;};

    // Переписать результаты в матрицы от 0 до _CurIndex1,_CurIndex2
    BOOL SendToOutput(BOOL tocurindices);

    // Добавить результаты для добавленных в массив агрументов (добавленными считаются все, начиная с PrevCount)
    BOOL AddResultsFromPrevCount(void);

    L57_Table2ArgFuncData(void)
      { ResArg1Count=ResArg2Count=NResults=0;Arg1Index=Arg2Index=-1;
        Args1Array=&_Args1Array; Args2Array=&_Args2Array; Results=NULL;
        _CurIndex1=_CurIndex2=0;OutputAllocError=FALSE;};
    ~L57_Table2ArgFuncData()
      { Clear(); };
};
//---------------------------------------------------------------------------
#endif // #ifndef L57_NORDSCLASSES

//---------------------------------------------------------------------------
// Константы для управления внешним видом блоков
#define L57_BLOCKTEXT_NOSUPPORT         -1 // Служебная константа
#define L57_BLOCKTEXT_NOCONTROL         0 // Нет управления (как задано в параметрах блока, так и будет)
#define L57_BLOCKTEXT_BEFORE            1 // Имя функции добавляется перед пользовательским текстом, если не пустое
#define L57_BLOCKTEXT_AFTER             2 // Имя функции добавляется после пользовательского текста, если не пустое
#define L57_BLOCKTEXT_FIXED             3 // Имя функции не добавляется в пользовательский текст
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Общие функции
//---------------------------------------------------------------------------
// Вызвать L57.dll для получения динамических описаний текущей модели для нулевой функции
void L57_ExecTool_GetDynamicDescription(L57_String &templatetext,L57_String &modeltext,
                                        L57_String *pFunc,L57_String *pInputs,L57_String *pNumbered,L57_String *pTitle);
// Вызвать L57.dll для редактирования функций и переменных
int L57_ExecTool_Edit(char *templatetext,char *modeltext,char *params,int starttab,L57_String *pNewModelText,
                      L57_String *pFunc,L57_String *pInputs,L57_String *pNumbered,L57_String *pTitle,
                      char *lockedits=NULL,L57_String *pExport=NULL,char *additionaldata=NULL,
                      RDS_BHANDLE *pShowBlock=NULL,int *ptr_param=NULL,L57_String *ptr_text=NULL);
// Вызвать L57.dll для замены опций в модели
int L57_ExecTool_SetOptions(char *templatetext,char *modeltext,char *options,L57_String *pNewModelText,L57_String *pNewExport=NULL);
// Вызвать l57.dll для импорта функции в модель
BOOL L57_ExecTool_ImportFunction(RDS_BHANDLE block,char *templatefile,int funcnum,char *importtext);
// Вызвать L57.dll для перевода функций
int L57_ExecTool_Translate(RDS_BHANDLE block,char *templatefile,int textparam,L57_String *ptr_text);
// Создать дополнительную строку данных с именем блока и номером его функционального входа
void L57_CreateAdditionalText(L57_String *pText,RDS_BHANDLE block,int funcin_num);
// Получить полное имя файла модели блока
int L57_GetDynModelFileName(RDS_BHANDLE block,L57_String &name);
// Загрузить текст файла модели блока
int L57_LoadModelFile(RDS_BHANDLE block,L57_String &text);
// Записать текст файла модели блока
int L57_SaveModelFile(RDS_BHANDLE block,L57_String &text,BOOL saveres=TRUE);
int L57_SaveModelFile(RDS_BHANDLE block,char *text,BOOL saveres=TRUE);
// Загрузить текст файла в строку
int L57_ReadFileToMemory(char *path,L57_String &text);
// Перекомпилировать модель блока
void L57_CompileModel(RDS_BHANDLE block,BOOL rebuild=TRUE);
// Определение числа точек по диапазону и шагу
int L57_CalcNPointsByDiap(double vmin,double vmax,double dv);
// Подстройка диапазона и шага (интерпретация неправильных значений)
// Возвращает число точек в диапазоне
int L57_AdjustDiapAndStep(double &_vmin,double &_vmax,double &_dv,double vmin,double vmax,double dv);
// Значение из диапазона по номеру
inline double L57_ValueInDiap(double vmin,double vmax,double dv,int i,int cnt)
{ return (i==0) ? vmin : ( (i==(cnt-1))?vmax:(vmin+i*dv) ); }
// Отведение массива double через RDS
inline double *L57_NewDouble(int n){return (double*)rdsAllocate(n*sizeof(double));}
// Отведение массива int через RDS
inline int *L57_NewInt(int n){return (int*)rdsAllocate(n*sizeof(int));}
// Отведение массива double* через RDS
inline double **L57_NewDoublePtr(int n){return (double**)rdsAllocate(n*sizeof(double*));}
// Сортировать матрицу для функции одной переменной
BOOL L57_SortMatrix(double *MData,int Rows,int Cols,int SortCol=0);
// Сортировать матрицу для семейства по строке (с пропуском нулевого индекса)
BOOL L57_SortMatrixFieldByRow(double *MData,int Rows,int Cols,int SortRow=0);
// Сортировать матрицу для семейства по столбцу (с пропуском нулевого индекса)
BOOL L57_SortMatrixFieldByCol(double *MData,int Rows,int Cols,int SortCol=0);
// Сортировать матрицу по столбцу по убыванию
BOOL L57_SortMatrixColDesc(double *MData,int Rows,int Cols,int SortCol=0);
// Сортировать матрицу по столбцу по возрастанию
#define L57_SortMatrixColAsc L57_SortMatrix
// Записать модель блока при сохранении параметров в undo
void L57_SaveModelToUndo(RDS_BHANDLE block,BOOL force=FALSE);
// Загрузить модель блока при восстановлении параметров при undo
void L57_LoadModelFromUndo(RDS_BHANDLE block,char *text);
#ifdef L57_LOG
  // Получить полное имя файла журнала для модели блока
  int L57_Log_GetLogFileName(RDS_BHANDLE block,L57_String &name);
  // Очистить журнал модели блока
  void L57_Log_Clear(RDS_BHANDLE block);
  // Добавить строкув журнал модели блока
  void L57_Log_Add(RDS_BHANDLE block,const L57_String &text,BOOL crlf=TRUE);
  void L57_Log_Add(RDS_BHANDLE block,char *text,BOOL crlf=TRUE);
  // Добавить строку и целое число в журнал модели блока
  void L57_Log_AddInt(RDS_BHANDLE block,char *title,int v,BOOL crlf=TRUE);
  // Добавить строку и вещественное число в журнал модели блока
  void L57_Log_AddDouble(RDS_BHANDLE block,char *title,double v,BOOL crlf=TRUE);
#endif
// Проверить наличие источников функции у входа блока
// (используется только для блоков с единственной функцией)
BOOL L57_CheckFunctionSource(RDS_BHANDLE block,int inputnum);
// Проверить наличие источников функции у входа блока
// (используется для блоков с входом-массивом функций)
BOOL L57_CheckFunctionSourceMulti(RDS_BHANDLE block,int inputnum,L57_IntArray *pItems,BOOL *pArrayConnected);
// Послать функции по выходным связям
void L57_SendFunctionByConn(RDS_BHANDLE block,int outputnum,BOOL immediate,char **exporttexts,int funccount,L57_AcceptFunc_Param_Queue *queue);
// Послать функции по выходным связям (с массивом альтернативных текстов)
void L57_SendOutputFunctions(RDS_BHANDLE block,int outputnum,BOOL immediate,char **exporttexts,int funccount,L57_StringArray *exportalt,L57_AcceptFunc_Param_Queue *queue);
// Послать одну функцию по выходной связи, если нет входной связи для функции
// (используется только для блоков с единственной функцией)
void L57_SendSingleFunctionIfNoInput(RDS_BHANDLE block,int inputnum,int outputnum,BOOL immediate,char *exporttext);
// Послать одну функцию по выходной связи
// (используется только для блоков с единственной функцией)
void L57_SendSingleFunction(RDS_BHANDLE block,int outputnum,BOOL immediate,char *exporttext,L57_AcceptFunc_Param_Queue *queue);
// Принять одну (нулевую) функцию по входной связи
// (используется только для блоков с единственной функцией)
BOOL L57_GetSingleFunctionByConn(RDS_BHANDLE block,L57_AcceptFunc_Param *Param,char *curexporttext=NULL,L57_String *pNewExport=NULL);
// Принять в модель функцию с заданным номером
BOOL L57_GetCertainFunctionByConn(RDS_BHANDLE block,char *templatefile,int funcnum,
                                  char *curexporttext,char *newexporttext);
// Вызвать l57.dll для импорта набора функций в модель
BOOL L57_ExecTool_ImportFunctions(RDS_BHANDLE block,char *templatefile,char **exporttexts,int funccount,L57_StringArray *exportalt);
// Разбить текст экспорта функции на два массива: переменные и прочее
void L57_SplitExportText(char *text,L57_StringArray &vars,L57_StringArray &other);
// Сравнить пришедшую функцию и имеющуюся и вернуть необходимость замены (у имеющейся
// может быть больше переменных, это нормально)
BOOL L57_MustImportFunction(char *newfunc,char *oldfunc);
// Сравнить два списка экспорта функций и вернуть необходимость замены
BOOL L57_MustImportFunctions(L57_StringArray *newfuncs,L57_StringArray *oldfuncs);
BOOL L57_MustImportFunctions(L57_StringArray *newfuncs,char **oldfuncs,int oldfuncscount);
// Проверить возможность обращения к полю структуры
BOOL L57_CanAccessField(void *structbase,DWORD structsize,void *fieldbase,DWORD fieldsize);
// Макрос для проверки возможности доступа к полю структуры с servSize
#define L57_CANACCESSFIELD(pStruct,Field) L57_CanAccessField(pStruct,pStruct->servSize,&(pStruct->Field),sizeof(pStruct->Field))
// Сравнить два текста экспорта функций, игнорируя порядок переменных
BOOL L57_CompareExportTexts(char *text1,char *text2);
// Проверить наличие источника данных у входа блока
BOOL L57_CheckInputSource(RDS_BHANDLE block,int inputnum,RDS_BHANDLE *pConnBlock=NULL);
// Создать список источников данных у входа блока
BOOL L57_ListInputSources(RDS_BHANDLE block,L57_FuncTransferParameter *p,LPVOID Object,L57_VARLISTCB Func);
// Добавить получателей функции (функций) блока в список блоков
void L57_AppendFunctionDestList(RDS_BHANDLE block,int outputnum,RDS_HOBJECT list,L57_IntArray *inputs=NULL);
// Сравнить две матрицы double
BOOL L57_CompareDoubleMatr(void *VoidPtr1,void *VoidPtr2,bool *pError=NULL);
// Выделить и показать блок в окне
void L57_SelectAndShowBlock(RDS_BHANDLE block,BOOL waitmsg=TRUE);
// Запросить проверку передачи параметров в заданный блок
void L57_CheckTransferParameters(RDS_BHANDLE block,LPVOID Object,L57_VARLISTCB Func);
// Вызвать L57.dll и сообщить о возможности обновления модели
void L57_ExecTool_UpdatePossible(void);
// Сделать из имени файла имя резервной копии файла
BOOL L57_ResFile_MakeName(char *filename,L57_String &resname);
// Проверить существование файла
BOOL L57_FileExists(char *filename);
// Сделать копию файла модели блока перед первым изменением с момента загрузки схемы
void L57_ResFile_MakeCopy(char *filename);
// Удалить копию файла модели блока, сделанную перед первым изменением с момента загрузки схемы
void L57_ResFile_DeleteCopy(char *filename);
void L57_ResFile_DeleteModelCopy(RDS_BHANDLE block);
// Восстановить модель из копии, сделанной перед первым изменением с момента загрузки схемы
void L57_ResFile_RestoreCopy(char *filename);
void L57_ResFile_RestoreModelCopy(RDS_BHANDLE block);
// Получить очередь постановки вызовов из параметров функции передачи функции
L57_AcceptFunc_Param_Queue *L57_GetQueueFromParam(void *vparam);
// Проверить существование файла справки
BOOL L57_HelpUIFileExists(char *filename,int *pstate=NULL);
// Вызвать L57.dll для очистки всех функций и переменных
BOOL L57_ExecTool_ClearAll(RDS_BHANDLE block,char *templatefile);
// Вызвать L57.dll и сообщить о разрешении в глобальных настройках править внешний вид блока
BOOL L57_ExecTool_CanEditTextRect(void);
// Изменить внешний вид блока (прямоугольник с текстом) согласно принятой функции
void L57_ApplyFunctionToTextRect(RDS_BHANDLE block,int TR_params,char *TR_text,char *func_text,BOOL check=FALSE);
// Первое поле ввода внешнего вида блока с функциями
#define L57_BLOCKTEXT_FIELDBASE 10000
// Создать поля ввода для редактирования внешнего вида блока с функцией как прямоугольника с текстом
void L57_TextRectEdit_Setup(RDS_BHANDLE block,RDS_HOBJECT win,int TR_params,char *TR_text,int tab);
// Считать данные полей ввода для редактирования внешнего вида блока с функцией как прямоугольника с текстом
void L57_TextRectEdit_Apply(RDS_BHANDLE block,RDS_HOBJECT win,int &TR_params,L57_String &TR_text,char *functitles=NULL);
// Разрешение полей ввода для редактирования внешнего вида блока с функцией как прямоугольника с текстом
void L57_TextRectEdit_EnableControls(RDS_HOBJECT win);
//---------------------------------------------------------------------------

#endif // #ifndef L57Mk2BasicH

