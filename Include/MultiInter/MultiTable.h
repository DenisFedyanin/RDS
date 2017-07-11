//---------------------------------------------------------------------------

#ifndef MultiTableH
#define MultiTableH
//---------------------------------------------------------------------------
//#define MULTITABLE_DEBUG
//---------------------------------------------------------------------------
class TMultiTableSaver;
//---------------------------------------------------------------------------

// Константы для результата поиска делением пополам
#define MTSRES_BADARR   0       // Нет массива или в массиве единственный элемент
#define MTSRES_LEFT     1       // Меньше минимального
#define MTSRES_RIGHT    2       // Больше максимального
#define MTSRES_MID      3       // Между двумя элементами
//---------------------------------------------------------------------------
// Многомерная таблица. Данные хранятся единым массивом.
//---------------------------------------------------------------------------
// Индекс таблицы
class TMultiTableIndex
{ public:
    double *Scale;      // Отсчеты шкалы
    int ScaleCount;     // Число отсчетов шкалы
    char *Name;         // Имя шкалы

    int Shift;          // Сдвиг в общем массиве при приращении этого индекса

    int Index;          // Установленный снаружи индекс (просто запоминается:
                        // сначала устанавливаются все индексы, потом выбирается элемент
                        // из общего массива функцией GetValuesByIndices)
    bool Fixed;         // Индекс зафиксирован (используется в специальных функциях перебора)

    double Argument;    // Аргумент для интерполяции по этому индексу
    int InterLeft,InterRight; // Индексы двух отсчетов для интерполяции аргумента
    int InterSearchResult; // Результат поиска соседних отсчетов

    // Данные для вспомогательной интерполяционной таблицы
    int InterShift;     // Сдвиг в массиве таблицы при приращении этого индекса
    int InterIndex;     // Установленный снаружи индекс (два возможных значения: 0 и 1,
                        // просто запоминается, сначала устанавливаются все индексы,
                        // потом выбирается элемент функцией GetInterValuesByIndices)

    // Очистить все
    void __fastcall Clear(bool clearname=true);
    // Отвести место под шкалу
    void __fastcall AllocateScale(int n);

    // Найти соседние с аргументом отсчеты для интерполяции
    bool __fastcall SearchArg(void);

    // Найти ближайший отсчет
    int __fastcall FindClosestArg(double arg);

    // Добавить имя значения (забирает себе владение динамической строкой)
    void __fastcall AcceptValName(char *dynstr);
    // Добавить имя значения (копирует переданную строку)
    void __fastcall StoreValName(char *statstr);
    // Получить имя зачения
    inline char *GetValName(void){return Name?Name:"";};

    TMultiTableIndex(void);
    ~TMultiTableIndex(){Clear(true);};
};
//---------------------------------------------------------------------------

// Собственно таблица
class TMultiTable
{ private:
    int Dim;                    // Размерность (число аргументов)
    int ValCount;               // Число значений для каждого набора аргументов
    TMultiTableIndex *Index;    // Массив шкал

    char **ValNames;            // Имена значений
    int ValNamesCount;          // Число имен значений (может отводиться с запасом,
                                // чаще всего равно ValCount)

    double *Data;               // Данные
    int DataSize;               // Общий размер данных (для справки и поиска минимального/максимального)

    // Вспомогательные данные для интерполяции
    // (таблица на один индекс меньше и по каждому индексу всего два отсчета)
    double *InterData;  // Общий массив
    int InterDataSize;  // Размер массива (для справки)

    int LastError;              // Код последней ошибки
      #define MTERROR_OK                0       // OK
      #define MTERROR_BADDIM            1       // Не указана или неверная размерность таблицы
      #define MTERROR_ALREADYSET        2       // Данные уже установлены
      #define MTERROR_BADARG            3       // Плохой параметр (отрицательное число элементов и т.п.)
      #define MTERROR_ZEROSCALE         4       // По одной из шкал нет отсчетов
      #define MTERROR_BADINDEX          5       // Индекс за пределами
      #define MTERROR_NODATA            6       // Нет массива данных
      #define MTERROR_SEARCHERR         7       // Ошибка поиска аргумента
      #define MTERROR_LOADERR           8       // Ошибка загрузки
      #define MTERROR_INTERNAL          9       // Непонятная внутренняя ошибка

    // Установить ошибку
    void __fastcall SetError(int e);

    // Сбросить на ноль индексы во всех размерностях
    void __fastcall ResetIndices(void);

    // Сбросить на ноль интерполяционые индексы во всех размерностях
    void __fastcall ResetInterIndices(void);
    // Вычислить следующий интерполяционный индекс (с переносом по индексам)
    bool __fastcall NextInterIndex(void);

    // Загрузить - вспомогательная
    bool __fastcall LoadAux(TMultiTableSaver *s);

  public:
    int RestoredRecords;        // Число восстановленных при создании отсчетов
                                // (нигде не испоьзуется, только для информации пользователю)
    int RestoreMode;            // Режим восстановления записей (для информации пользователю)
    #define TMTSCT_RM_UNKNOWN   -1      // Неизвестный
    #define TMTSCT_RM_GEO       0       // Взвешиванием всех отсчетов
    #define TMTSCT_RM_LIN       1       // Среднее линейной интерполяции соседей

    // Получить ошибку
    inline int GetLastError(void){return LastError;};
    // Сбросить ошибку
    inline void ResetError(void){LastError=MTERROR_OK;};

    // Пустая?
    inline bool IsEmpty(void){return Data==NULL || Dim==0 || ValCount==0;};

    // Получить размерность
    inline int GetDimension(void){return Dim;};
    // Установить размерность
    bool __fastcall SetDimension(int dim);
    // Индекс
    inline TMultiTableIndex *GetIndexFAST(int i){return &(Index[i]);};
    TMultiTableIndex * __fastcall GetIndex(int i){return (i<0 || i>=Dim)?NULL:(&(Index[i]));};
    // Установить число отсчетов в размерности
    inline void SetIndexSizeFAST(int dim,int count){Index[dim].AllocateScale(count);};
    bool __fastcall SetIndexSize(int dim,int count);
    // Получить число отсчетов в размерности
    inline int GetIndexSizeFAST(int dim){return Index[dim].ScaleCount;};
    // Получить минимальный и максимальный отсчет в размерности
    bool __fastcall GetIndexRange(int dim,double &vmin,double &vmax);

    // Добавить имя значения (забирает себе владение динамической строкой)
    void __fastcall AcceptValName(int n,char *dynstr);
    // Добавить имя значения (копирует переданную строку)
    void __fastcall StoreValName(int n,char *statstr);
    // Получить имя зачения
    char * __fastcall GetValName(int n);

    // Найти максимальное и минимальное значение
    bool __fastcall GetValueRange(int nval,double &vmin,double &vmax);

    //---------- Выборка элемента ----------
    // Установить индекс по выбранной размерности
    inline void SetDimensionIndexFAST(int dim,int index){Index[dim].Index=index;};
    bool __fastcall SetDimensionIndex(int dim,int index);
    // Получить установленный индекс по выбранной размерности
    inline int GetDimensionIndexFAST(int dim){return Index[dim].Index;};
    // Получить начало группы из ValCount чисел по установленным индексам
    double * __fastcall GetValuesByIndices(void);
    // Получить начало группы из ValCount чисел по значениям отсчетов и погрешностям
    // (без интерполяции, возвращается попавший в диапазон отсчет)
    double * __fastcall GetValuesByArgsAndErrors(double *arg,double *argerror);
    // Получить начало группы из ValCount чисел по значениям отсчетов
    // (без интерполяции, возвращается ближайший отсчет)
    double * __fastcall GetClosestValuesByArgs(double *arg,double *realarg,int maxcount=-1);
    //--------------------------------------

    //---------- Перебор элементов по всем индексам ----------
    // Сбросить индексы на ноль
    void __fastcall Loop_ResetIndices(void);
    // Взять следующий индекс (возврат - перебор закончен)
    bool __fastcall Loop_NextElement(void);
    //--------------------------------------------------------

    //------ Перебор элементов с частично зафиксированными индексами ------
    // Сбросить фиксированность всех индексов
    void __fastcall UnfixAllIndices(void);
    // Установить фиксированность индекса
    void __fastcall FixIndex(int dim,bool fix=true);
    // Сбросить индексы на ноль (кроме фиксированных)
    void __fastcall LoopFixed_ResetIndices(void);
    // Взять следующий индекс (кроме фиксированных, возврат - перебор закончен)
    bool __fastcall LoopFixed_NextElement(void);
    //---------------------------------------------------------------------


    //---------- Выборка элемента из интерполяционной таблицы ----------
    // Установить индекс по выбранной размерности
    inline void SetDimensionInterIndexFAST(int dim,int index){Index[dim].InterIndex=index;};
    // Получить установленный индекс по выбранной размерности
    inline int GetDimensionInterIndexFAST(int dim){return Index[dim].InterIndex;};
    // Получить во вспомогательной таблице начало группы из ValCount
    // чисел по установленным индексам (нулевой не участвует)
    double * __fastcall GetInterValuesByIndices(void);
    //------------------------------------------------------------------

    //---------- Интерполяция ------------
    // Сброс перед первой интерполяцией
    void __fastcall ResetInterpolation(void);
    // Установка аргументов для интерполяции
    inline void SetArgumentFAST(int dim,double arg){Index[dim].Argument=arg;};
    // Получить массив из ValCount итерполированных значений (аргументы - в индексах)
    double * __fastcall Interpolate(void);
    //------------------------------------

    // Получить число значений
    inline int GetValuesCount(void){return ValCount;};
    // Установить число значений
    bool __fastcall SetValuesCount(int vc);

    // Очистить
    void __fastcall Clear(void);

    // Индексы установлены - отвести место под данные
    bool __fastcall AllocateData(void);
    // Заполнить данные значением
    void __fastcall FillData(double val);


    // Поиск в массиве делением пополам
    static void __fastcall SearchInArray(double *arr,int count,double x,
                                         int &ileft,int &iright,int &result);
    // Линейная интерполяция
    static double __fastcall LinInterpol(double x1,double y1,double x2,double y2,double x);

    // Сохранить
    bool __fastcall Save(TMultiTableSaver *s);
    // Загрузить
    bool __fastcall Load(TMultiTableSaver *s);

    #ifdef MULTITABLE_DEBUG
      // Отладочная распечатка
      AnsiString __fastcall Debug(bool showinterp=false);
    #endif

    TMultiTable(void);
    ~TMultiTable(){Clear();};
};
//---------------------------------------------------------------------------
#endif
