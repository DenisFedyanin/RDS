// Добавить настраиваемую проверку индексов
//---------------------------------------------------------------------------
// Стандартные части модели внутреннего максимизатора функции
// произвольного числа переменных (версия 2)
//---------------------------------------------------------------------------

// Последний использованный номер ошибки "мало памяти": L57OutOfMemMsg(14);

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

// Погрешность для проверок по умолчанию
#define L57_SMALLDELTA  1e-30

// Имя файла шаблона модели
#define L57_MODELTEMPLATE "$RDSINCLUDE$\\RdsL57Mk2\\IpuL57Mk2ArgMax.mdl"

// Заголовок сообщений этого блока
#define L57_MSGCAPTION  "Внутренний максимизатор"

// Общие функции и классы
#include <RdsL57Mk2/L57Mk2Basic.cpp>

// Состояния максимизатора (_Stage)
#define L57STAGE_WAIT           0       // Ожидание
#define L57STAGE_PRETAB         1       // Предварительный расчет простых выходных табличных функций
#define L57STAGE_GRID           2       // Первичный перебор
      #define L57SUBSTAGE_GRIDFILL   0       // Первичный перебор
      #define L57SUBSTAGE_GRIDCAND   1       // Отбор кандидатов
#define L57STAGE_PREC           3       // Уточнение отобранных кандидатов
#define L57STAGE_TABAFTERCALCSIMP 4     // Дорасчет простых выходных таблиц
#define L57STAGE_TABAFTERCALCAM   5     // Дорасчет выходных таблиц arg max
#define L57STAGE_TABPRECCALC    6       // Уточнение выходных таблиц arg max
#define L57STAGE_FINAL          100     // Окончание максимизации

// Сетка уточнения максимумов
#define L57PRECGRID_4X4         0       // По каждому параметру - на два уменьшенных шага в стороны
#define L57PRECGRID_2X2         1       // По каждому параметру - на один уменьшенный шаг в стороны

// Идентификаторы полей окна настроек
#define L57SETUPFIELD_NOAUTOSTRT        1       // Начинать поиск только по команде
#define L57SETUPFIELD_DEEPSEARCH        2       // Уточнять максимумы
#define L57SETUPFIELD_CHUNKMB           3       // Объем временных данных
#define L57SETUPFIELD_TACTCOUNT         4       // Число итераций в такте
#define L57SETUPFIELD_EQMAXDELTA        5       // Погрешность одинаковости максимумов
                                                // Для уточнения максимумов:
#define L57SETUPFIELD_PRECENDSTEPS      6       //   Уточнять до конечных шагов
#define L57SETUPFIELD_PRECDF            7       //   Уточнять до погрешности функции
#define L57SETUPFIELD_DFCYCLES          8       //   Итераций в трубке погрешности
#define L57SETUPFIELD_DFVALUE           9       //   Значение погрешности
#define L57SETUPFIELD_PRECGRID          10      //   Сетка уточнения
#define L57SETUPFIELD_NOPLATEAU         11      // Убирать точки на плато
#define L57SETUPFIELD_TABAFTERCALC      12      // Дорасчет выходных табличных функций
#define L57SETUPFIELD_TABPRECCALC       13      // Уточнение выходных табличных функций
                                                // Для уточнения таблиц:
#define L57SETUPFIELD_TABPRECENDSTEPS   14      //   Уточнять до конечных шагов
#define L57SETUPFIELD_TABPRECGRID       15      //   Сетка уточнения
#define L57SETUPFIELD_TABPRECDF         16      //   Уточнять до погрешности функции
#define L57SETUPFIELD_TABDFVALUE        17      //   Значение погрешности
#define L57SETUPFIELD_TABDFCYCLES       18      //   Итераций в трубке погрешности
#define L57SETUPFIELD_DFFILTERVALUE     19      // Интервал фильтрации значения функции
#define L57SETUPFIELD_TITLE             1000    // Информационное - заголовок функции
#define L57SETUPFIELD_CODE              1001    // Информационное - текст функции
#define L57SETUPFIELD_PARAMS            1002    // Информационное - список переменных
#define L57SETUPFIELD_EDITBTN           1003    // Кнопка вызова окна редактирования
#define L57SETUPFIELD_OUTTABBTN         1004    // Кнопка вызова окна редактирования с вкладкой выходных таблиц
#define L57SETUPFIELD_TEMPLATE          2000    // Невизуальное - шаблон модели
#define L57SETUPFIELD_MODEL             2001    // Невизуальное - текст модели
#define L57SETUPFIELD_LOCKEDIT          2002    // Невизуальное - признак блокировки редактирования
#define L57SETUPFIELD_ADDITIONALDATA    2003    // Невизуальное - дополнительные данные
#define L57SETUPFIELD_BLOCKTOSHOW       2004    // Невизуальное - блок, который нужно показать при временном закрытии окна

// Предописания
class L57_MaxCandidate;
//-----------------------------------------------------------------------

// Признак наличия файла справки
int L57_HelpState=0;
// Признак разрешения редактирования внешнего вида блока в настройках
BOOL L57_AllowTextRectEdit=FALSE;
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Данные оптимизируемого параметра
//-----------------------------------------------------------------------
class L57_Parameter
{ private:
    // Отвести массив аргументов
    BOOL AllocateArray(int count);

  public:
    // Исходные данные
    double Min,Max;     // Исходный диапазон перебора
    double StartStep;   // Начальный шаг
    double EndStep;     // Конечный шаг (если есть)
    double FuncVal;     // Значение для простых табличных функций (если есть)

    // Сгенерированный массив аргументов и текущие данные
    double _Min,_Max,_Delta;    // Скорректированные значения диапазона и шага
    L57_DblArray Array;         // Массив отсчетов аргумента
    //double *Array;              // Массив отсчетов аргумента
    //int ArrayCount;             // Число отсчетов в массиве
    int _ArgIndex;              // Номер текущего значения в процессе перебора в массиве Array
    int EvalChunkSize;          // Размер "куска" массива по этому аргументу в процессе
                                // "вписывания" кусков в разрешенную память
    int *ChunkStart;            // Масив индексов начала куска
    int *ChunkEnd;              // Массив индексов конца куска
    int ChunkCount;             // Число кусков в массиве
    int _ChunkIndex;            // Номер текущего куска

    DWORD ChunkArrayShift;      // Сдвиг в общем массиве значений текущего куска при
                                // приращении этого индекса

    // Дополнительный индекс (для разных циклов)
    int AuxIndex,AuxIndexMin,AuxIndexMax;

    double Current;             // Текущее значение для передачи в функцию вычисления

    // Указатели на объекты максимумов, добавленных в массив отсчетов аргумента после максимизации
    L57_MaxCandidate **AddedMaxObjects;
    int AddedMaxObjectsCount;

    // Очистить массив аргументов
    void ClearArrays(void);
    // Отвести массив аргументов по исходным данным
    BOOL AllocateArray(void);
    // Добавить заданное число аргументов в конец массива
    BOOL AddArgs(int n,BOOL fillwitherror=FALSE);

    // Отвести массивы кусков по EvalChunkCount
    BOOL AllocateChunkArray(void);

    L57_Parameter(void)
      { Min=Max=StartStep=EndStep=rdsbcppHugeDouble;
        ChunkCount=EvalChunkSize=AddedMaxObjectsCount=0;
        ChunkStart=ChunkEnd=NULL; AddedMaxObjects=NULL;
      };
    ~L57_Parameter(){ClearArrays();};
};
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Набор оптимизируемых параметров
//-----------------------------------------------------------------------
// Должен быть уже установлен макрос L57_PARAMSCOUNT!
class L57_Parameters
{ public:
    #ifdef L57_PARAMSCOUNT
      L57_Parameter P[L57_PARAMSCOUNT];
    #endif

    // Массив значений функции для текущего обрабатываемого куска
    double *ChunkValues;
    DWORD ChunkValuesSize;

    // Очистить все временные данные
    void ClearTempData(void);

    // Очистить массив для значений обрабатываемого куска
    void ClearChunkValuesArray(void);
    // Отвести массив для значений обрабатываемого куска
    // Возвр. успешность
    BOOL CreateChunkValuesArray(void);
    // Заполнить массив значений обрабатываемого куска признаком ошибки
    void ResetChunkValuesArray(void);
    // Получить указатель на значение в массиве по текущим индексам (_ArgIndex,_ChunkIndex)
    double *GetChunkValuePtrByParams(DWORD *pIndex=NULL);
    // Получить указатель на значение в массиве по текущим индексам + AuxIndex
    double *GetChunkValuePtrByParamsAndAux(DWORD *pIndex=NULL);
    // Установка индеков аргументов всех параметров на начала кусков
    void ResetCurrentChunk(void);
    // Следующий шаг в текущем куске (возвр. TRUE, если перебор текущего куска закончен)
    BOOL NextStep_Chunk(void);
    // Следующий кусок (возвр. TRUE если перебор кусков закончен)
    BOOL NextChunk(void);
    // Следующий шаг в полном наборе аргументов (без кусков)
    BOOL NextStep(void);

    // Инициализировать все вспомогательные индексы
    void InitAuxIndex(int IndMin,int IndMax);
    // Сумма основного и вспомогательного индексов попадает в массивы значений?
    BOOL ValidMainAndAuxIndices(BOOL *pAllZero);
    // Сделать шаг по вспомогательным индексам
    BOOL StepAuxIndex(void);

    L57_Parameters(void)
      { ChunkValues=NULL; ChunkValuesSize=0;
      };
    ~L57_Parameters()
      { ClearTempData(); };
};
//-----------------------------------------------------------------------

//---------------------------------------------------------------------------
// Элемент связного списка кандидатов в максимумы или готовых максимумов
//---------------------------------------------------------------------------
class L57_MaxCandidate
{ public:
    L57_MaxCandidate *Next;

    #ifdef L57_PARAMSCOUNT
    double CurParams[L57_PARAMSCOUNT]; // Текущие значения аргументов
    double Deltas[L57_PARAMSCOUNT];    // Текущие шаги
    #endif
    double f;                          // Значение функции
    int cycles_in_df;                  // Число шагов внутри трубки погрешности

    double Old_Val;     // Предыдущее значения (используется снаружи)

    BOOL Tag1,Tag2;     // Вспомогательные флаги
    int IntTag1,IntTag2;

    static int GlobalCount; // Общее число созданных объектов

    //BOOL Equal;         // Все вокруг одинаковые
    int StepsDown;      // Сколько уменьшений шагов сделано

    L57_MaxCandidate(void)
      { Next=NULL;
        cycles_in_df=0;
        f=Old_Val=rdsbcppHugeDouble;
        //Equal=TRUE;
        StepsDown=0;
        GlobalCount++;
        Tag1=Tag2=FALSE;
        IntTag1=IntTag2=-1;
      };
    ~L57_MaxCandidate(){GlobalCount--;};

};
int L57_MaxCandidate::GlobalCount=0; // Общее число созданных объектов
//---------------------------------------------------------------------------
// Связный список кандидиатов в максимумы
class L57_MaxList
{ public:
    L57_MaxCandidate *First;

    // Пустой?
    inline BOOL IsEmpty(void){return First==NULL;};
    // Очистить
    void Clear(void);
    // Добавить кандидата в начало связного списка
    L57_MaxCandidate *AddFirst(void);
    // Отсоединить верхний
    L57_MaxCandidate *RemoveTop(void);
    // Вставить готового кандидата в начало связного списка
    void AddExistingFirst(L57_MaxCandidate *f);
    // Подсчитать
    int Count(void);
    // Поменяться содержимым с другим списком
    void Exchange(L57_MaxList &list);
    // Удалить все ниже границы
    void RemoveSmall(double level);

    L57_MaxList(void){First=NULL;};
    ~L57_MaxList(){Clear();};
};
//-----------------------------------------------------------------------




//-----------------------------------------------------------------------
// Данные оптимизируемого параметра
//-----------------------------------------------------------------------
// Очистить массив аргументов
void L57_Parameter::ClearArrays(void)
{
  Array.Clear();
  Array.PrevCount=0;
  if(ChunkStart)
    rdsFree(ChunkStart);
  if(ChunkEnd)
    rdsFree(ChunkEnd);
  ChunkCount=0;
  ChunkStart=ChunkEnd=NULL;

  if(AddedMaxObjects)
    rdsFree(AddedMaxObjects);
  AddedMaxObjects=NULL;
  AddedMaxObjectsCount=0;
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
{ int n=L57_CalcNPointsByDiap(Min,Max,StartStep);
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
    { _Min=Min; _Max=Max; _Delta=StartStep; }

  for(int xi=0;xi<n;xi++)
    { double v=L57_ValueInDiap(_Min,_Max,_Delta,xi,n);
      Array.Data[xi]=v;
    }
  return TRUE;
}
//-----------------------------------------------------------------------

// Добавить заданное число аргументов в конец массива
BOOL L57_Parameter::AddArgs(int n,BOOL fillwitherror)
{ int ArgCount=Array.Count;
  int new_count=ArgCount+n;
  L57_DblArray newArgs;

  if(n==0)
    return TRUE;
  if(n<0 || new_count<0)
    return FALSE;

  // Переотводим все
  if(!newArgs.Allocate(new_count))
    return FALSE;
  newArgs.FillDataFrom(Array);
  Array.Exchange(newArgs);
  Array.PrevCount=ArgCount;

  if(fillwitherror)
    for(int i=Array.PrevCount;i<new_count;i++)
      Array.Data[i]=rdsbcppHugeDouble;
  return TRUE;
}
//-----------------------------------------------------------------------

// Отвести массивы кусков по EvalChunkCount
BOOL L57_Parameter::AllocateChunkArray(void)
{ int first,last,index,final;
  int ArrayCount=Array.Count;
  if(ChunkStart)
    rdsFree(ChunkStart);
  if(ChunkEnd)
    rdsFree(ChunkEnd);
  ChunkCount=0;
  ChunkStart=ChunkEnd=NULL;

  if(EvalChunkSize<=0)
    return FALSE;

  // EvalChunkSize=3, ArrayCount=8
  // 01234567
  // vvvvvvvv
  // ^^^----- кусок 0: [0...2], т.е. [first...first+EvalChunkSize-1]
  //   ^^^--- кусок 1: [2...4], т.е. [first...first+EvalChunkSize-1]
  //     ^^^- кусок 2: [4...6], т.е. [first...first+EvalChunkSize-1]
  //       ^^ кусок 3: [6...7], т.е. [first...ArrayCount-1]

  // Подсчитываем число кусков
  final=ArrayCount-1;
  first=index=0;
  for(;;)
    { // Кусок first...first+EvalChunkSize-1 включительно
      last=first+EvalChunkSize-1;
      index++;
      if(last>=final)
        break;
      first=last;
    }
  // В index - число кусков

  RDSBCPP_TRY
    { ChunkStart=L57_NewInt(index); }
  RDSBCPP_CATCHALL
    { ChunkStart=NULL; }
  if(!ChunkStart)
    return FALSE;
  RDSBCPP_TRY
    { ChunkEnd=L57_NewInt(index); }
  RDSBCPP_CATCHALL
    { ChunkEnd=NULL; }
  if(!ChunkEnd)
    { rdsFree(ChunkStart);
      ChunkStart=NULL;
      return FALSE;
    }
  ChunkCount=index;

  first=index=0;
  for(;;)
    { // Кусок first...first+EvalChunkSize-1 включительно
      last=first+EvalChunkSize-1;
      if(last>final)
        last=final;
      ChunkStart[index]=first;
      ChunkEnd[index]=last;
      index++;
      if(last==final)
        break;
      first=last;
    }
  return TRUE;
}
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------
// Набор оптимизируемых параметров
//-----------------------------------------------------------------------

// Сделать шаг по вспомогательным индексам
BOOL L57_Parameters::StepAuxIndex(void)
{ BOOL end=TRUE;
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->AuxIndex++;
      if(p->AuxIndex<=p->AuxIndexMax)
        { end=FALSE;
          break;
        }
      // Дошли до конца
      p->AuxIndex=p->AuxIndexMin;
    }
#endif    
  return end;
}
//---------------------------------------------------------------------------

// Получить указатель на значение в массиве по текущим индексам + AuxIndex
double *L57_Parameters::GetChunkValuePtrByParamsAndAux(DWORD *pIndex)
{ DWORD n=0;
#ifdef L57_PARAMSCOUNT
  if(pIndex) *pIndex=0xffffffff;
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      int ai=p->_ArgIndex+p->AuxIndex;
      n+=(ai-p->ChunkStart[p->_ChunkIndex])*p->ChunkArrayShift;
    }
  if(n>=ChunkValuesSize)
    return NULL;
#endif
  if(pIndex) *pIndex=n;
  return ChunkValues+n;
}
//---------------------------------------------------------------------------

// Инициализировать все вспомогательные индексы
void L57_Parameters::InitAuxIndex(int IndMin,int IndMax)
{
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->AuxIndex=p->AuxIndexMin=IndMin;
      p->AuxIndexMax=IndMax;
    }
#endif
}
//---------------------------------------------------------------------------

// Сумма основного и вспомогательного индексов попадает в массивы значений?
BOOL L57_Parameters::ValidMainAndAuxIndices(BOOL *pAllZero)
{ BOOL inside=TRUE;
  if(pAllZero) *pAllZero=TRUE;
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      int n;
      if(pAllZero!=NULL && p->AuxIndex!=0)
        *pAllZero=FALSE;
      n=p->_ArgIndex+p->AuxIndex;
      if(n<p->ChunkStart[p->_ChunkIndex] || n>p->ChunkEnd[p->_ChunkIndex])
        inside=FALSE;
    }
#endif    
  return inside;
}
//---------------------------------------------------------------------------

// Установка индеков аргументов всех параметров на начала кусков
void L57_Parameters::ResetCurrentChunk(void)
{
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->_ArgIndex=p->ChunkStart[p->_ChunkIndex];
    }
#endif    
}
//---------------------------------------------------------------------------


// Получить указатель на значение в массиве по текущим индексам
double *L57_Parameters::GetChunkValuePtrByParams(DWORD *pIndex)
{ DWORD n=0;
  if(pIndex) *pIndex=0xffffffff;
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      n+=(p->_ArgIndex-p->ChunkStart[p->_ChunkIndex])*p->ChunkArrayShift;
    }
  if(n>=ChunkValuesSize)
    return NULL;
#endif
  if(pIndex) *pIndex=n;
  return ChunkValues+n;
}
//---------------------------------------------------------------------------

// Очистить все временные данные
void L57_Parameters::ClearTempData(void)
{
#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    P[i].ClearArrays();
#endif
  ClearChunkValuesArray();
}
//---------------------------------------------------------------------------

// Очистить массив для значений обрабатываемого куска
void L57_Parameters::ClearChunkValuesArray(void)
{ if(ChunkValues)
    { rdsFree(ChunkValues);
      ChunkValues=NULL;
    }
  ChunkValuesSize=0;
}
//---------------------------------------------------------------------------

// Отвести массив для значений обрабатываемого куска
// Возвр. успешность
BOOL L57_Parameters::CreateChunkValuesArray(void)
{ BOOL ok=TRUE;
#ifdef L57_PARAMSCOUNT
  DWORD count=1;
  double d_count=1.0;

  // Очистка
  if(ChunkValues)
    rdsFree(ChunkValues);
  ChunkValues=NULL;
  ChunkValuesSize=0;

  // Расчет максимального размера
  for(int i=L57_PARAMSCOUNT-1;i>=0;i--)
    { L57_Parameter *p=P+i;
      int maxchunk=0;
      p->ChunkArrayShift=count;
      for(int j=0;j<p->ChunkCount;j++)
        { int n=p->ChunkEnd[j]-p->ChunkStart[j]+1;
          if(maxchunk<n)
            maxchunk=n;
        }
      if(!maxchunk)
        return FALSE;
      count*=maxchunk;
      d_count*=(double)maxchunk;
    }
  if(((double)count)<d_count) // Переполнение?
    return FALSE;

  RDSBCPP_TRY
    { ChunkValues=L57_NewDouble(ChunkValuesSize=count);
      if(!ChunkValues)
        ok=FALSE;
    }
  RDSBCPP_CATCHALL
    { ok=FALSE; ChunkValuesSize=0; ChunkValues=NULL; }
#endif    
  return ok;
}
//---------------------------------------------------------------------------

// Заполнить массив значений обрабатываемого куска признаком ошибки
void L57_Parameters::ResetChunkValuesArray(void)
{
  for(DWORD i=0;i<ChunkValuesSize;i++)
    ChunkValues[i]=rdsbcppHugeDouble;
}
//---------------------------------------------------------------------------

// Следующий шаг в полном наборе аргументов (без кусков)
BOOL L57_Parameters::NextStep(void)
{ BOOL changed=FALSE;

#ifdef L57_PARAMSCOUNT
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

// Следующий шаг в текущем куске (возвр. TRUE, если перебор текущего куска закончен)
BOOL L57_Parameters::NextStep_Chunk(void)
{ BOOL changed=FALSE;

#ifdef L57_PARAMSCOUNT
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->_ArgIndex++;
      if(p->_ArgIndex<=p->ChunkEnd[p->_ChunkIndex]) // Есть еще значение в текущем куске
        { changed=TRUE;
          break;
        }
      // Значения нет
      p->_ArgIndex=p->ChunkStart[p->_ChunkIndex];
    }
#endif
  return (!changed);
}
//---------------------------------------------------------------------------

// Следующий кусок (возвр. TRUE если перебор кусков закончен)
BOOL L57_Parameters::NextChunk(void)
{
#ifdef L57_PARAMSCOUNT
  BOOL changed=FALSE;
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=P+i;
      p->_ChunkIndex++;
      if(p->_ChunkIndex<p->ChunkCount) // Есть еще кусок по этому параметру
        { changed=TRUE;
          break;
        }
      // Куска нет
      p->_ChunkIndex=0;
    }
  if(!changed)
    return TRUE;

  // Устанавливаем индексы аргументов на начала кусков
  ResetCurrentChunk();

#endif
  // Записываем в выходные параметры
  return FALSE;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Элемент связного списка кандидатов в максимумы или готовых максимумов
//---------------------------------------------------------------------------

// Очистить связный список кандидатов
void L57_MaxList::Clear(void)
{ L57_MaxCandidate *f,*fn;

  f=First;
  while(f)
    { fn=f->Next;
      delete f;
      f=fn;
    }
  First=NULL;
}
//---------------------------------------------------------------------------

// Удалить все ниже границы
void L57_MaxList::RemoveSmall(double level)
{ L57_MaxCandidate *f,*prev=NULL,*f1;

  f=First;
  while(f)
    { if(f->f>=level) // Оставляем
        { prev=f;
          f=f->Next;
          continue;
        }
      // Вырезаем из списка и удаляем
      f1=f->Next;
      if(prev) // Есть предыдущий
        prev->Next=f1;
      else // Нет предыдущего
        First=f1;
      delete f;
      f=f1;
    }
}
//---------------------------------------------------------------------------


// Добавить кандидата в начало связного списка
L57_MaxCandidate *L57_MaxList::AddFirst(void)
{ L57_MaxCandidate *f;
  try { f=new L57_MaxCandidate(); }
  catch(...) { f=NULL;};
  if(!f)
    return NULL;
  AddExistingFirst(f);
  return f;
}
//---------------------------------------------------------------------------

// Вставить готового кандидата в начало связного списка
void L57_MaxList::AddExistingFirst(L57_MaxCandidate *f)
{
  if(First==NULL)
    First=f;
  else
    { f->Next=First;
      First=f;
    }
}
//---------------------------------------------------------------------------

// Отсоединить верхнего кандидата от связного списка
L57_MaxCandidate *L57_MaxList::RemoveTop(void)
{ L57_MaxCandidate *f=First;
  if(!f)
    return f;
  First=f->Next;
  f->Next=NULL;
  return f;
}
//---------------------------------------------------------------------------

// Подсчитать кандидатов
int L57_MaxList::Count(void)
{ int n=0;
  for(L57_MaxCandidate *f=First;f!=NULL;f=f->Next)
    n++;
  return n;
}
//---------------------------------------------------------------------------

// Поменяться содержимым с другим списком
void L57_MaxList::Exchange(L57_MaxList &list)
{ L57_MaxCandidate *f=First;
  First=list.First;
  list.First=f;
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
// Прочие события
//---------------------------------------------------------------------------
#ifdef L57MS_OTHER

switch(CallMode)
  { case RDS_BFM_LEAVINGEDITMODE:
      #if L57_MODELVERSION>=2
        L57_SendSingleFunctionIfNoInput(rdsbcppBlockData->Block,Func_in.Num(),Func_out.Num(),FALSE,
                                        _CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str());
      #endif
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
  #if L57_MODELVERSION>=2
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

  #endif
#endif // #ifdef L57MS_ACCEPTFUNC


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
// Настройки
//---------------------------------------------------------------------------
#ifdef L57MS_SETUP
  RDS_HOBJECT win;
  BOOL res;
  int tabid=0,fieldid;
  L57_String Func,Inputs,Numbered,Title;
  L57_String modeltext,templatetext,oldoptions,auxdata;

  RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

  #if L57_MODELVERSION>=2
    BOOL FuncByConn=L57_CheckFunctionSource(rdsbcppBlockData->Block,Func_in.Num());
  #endif


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

  #if L57_MODELVERSION>=3
    // Служебное поле: дополнительные данные о функциональном входе
    L57_CreateAdditionalText(&auxdata,rdsbcppBlockData->Block,Func_in.Num());
    rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_ADDITIONALDATA,RDS_FORMCTRL_NONVISUAL,"",0);
    rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,auxdata.c_str());
  #endif

  #if L57_MODELVERSION>=2
    // Служебное поле - блокировка редактирования
    rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_LOCKEDIT,RDS_FORMCTRL_NONVISUAL,"",0);
    rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,FuncByConn?"0":"");
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
  rdsFORMAddTab(win,++tabid,"Поиск");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_NOAUTOSTRT,RDS_FORMCTRL_CHECKBOX,
                 "Начинать поиск только по команде (сигнал Start_cmd)",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,noautostart_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_EQMAXDELTA,RDS_FORMCTRL_EDIT,
                 "Погрешность одинаковости максимумов:",80);
  rdsSetObjectDouble(win,fieldid,RDS_FORMVAL_VALUE,EqualMaxDelta_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DEEPSEARCH,RDS_FORMCTRL_CHECKBOX,"Уточнять максимумы",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,deepsearch_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_NOPLATEAU,RDS_FORMCTRL_CHECKBOX,"Убирать лишние точки на \"плато\"",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,NoPlateau_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_PRECENDSTEPS,RDS_FORMCTRL_CHECKBOX,"Уточнять до конечных шагов",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,PrecToEndSteps_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_PRECGRID,RDS_FORMCTRL_COMBOLIST,"    Сетка уточнения:",200);
  rdsFORMSetComboList(win,fieldid,"4x4 половинных шага\n2x2 половинных шага");
  rdsFORMSetInt(win,fieldid,PrecGridSize_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_PRECDF,RDS_FORMCTRL_CHECKBOX,"Уточнять до погрешности функции",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,PrecToDf_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DFVALUE,RDS_FORMCTRL_EDIT,
                 "    Значение погрешности функции:",80);
  rdsSetObjectDouble(win,fieldid,RDS_FORMVAL_VALUE,DfDelta_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DFCYCLES,RDS_FORMCTRL_UPDOWN,
                 "    Итераций в трубке погрешности функции:",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNINC,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMIN,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMAX,100);
  rdsFORMSetInt(win,fieldid,DfCycles_setup);

  // Вкладка выходных таблиц
  rdsFORMAddTab(win,++tabid,"Выходные таблицы");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_OUTTABBTN,RDS_FORMCTRL_BUTTON,"",80);
  rdsSetObjectStr(win,fieldid,RDS_FORMVAL_VALUE,"Изменить выходные табличные функции");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABAFTERCALC,RDS_FORMCTRL_CHECKBOX,
                 "Добавлять в выходные таблицы найденные значения максимумов",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,TabAfterCalc_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABPRECCALC,RDS_FORMCTRL_CHECKBOX,
                 "Уточнять значения выходных таблиц arg max",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,TabPrecCalc_setup?1:0);


  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABPRECENDSTEPS,RDS_FORMCTRL_CHECKBOX,"Уточнять до конечных шагов",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,Tab_PrecToEndSteps_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABPRECGRID,RDS_FORMCTRL_COMBOLIST,"    Сетка уточнения:",200);
  rdsFORMSetComboList(win,fieldid,"4x4 половинных шага\n2x2 половинных шага");
  rdsFORMSetInt(win,fieldid,Tab_PrecGridSize_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABPRECDF,RDS_FORMCTRL_CHECKBOX,"Уточнять до погрешности функции",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_VALUE,Tab_PrecToDf_setup?1:0);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABDFVALUE,RDS_FORMCTRL_EDIT,
                 "    Значение погрешности функции:",80);
  rdsSetObjectDouble(win,fieldid,RDS_FORMVAL_VALUE,Tab_DfDelta_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TABDFCYCLES,RDS_FORMCTRL_UPDOWN,
                 "    Итераций в трубке погрешности функции:",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNINC,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMIN,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMAX,100);
  rdsFORMSetInt(win,fieldid,Tab_DfCycles_setup);



  // Дополнительная вкладка
  rdsFORMAddTab(win,++tabid,"Дополнительно");

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_CHUNKMB,RDS_FORMCTRL_UPDOWN,
                 "Допустимый объем временных данных, Мб:",80);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNINC,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMIN,1);
  rdsSetObjectInt(win,fieldid,RDS_FORMVAL_UPDOWNMAX,1000);
  rdsFORMSetInt(win,fieldid,(int)ChunkSizeMb_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_TACTCOUNT,RDS_FORMCTRL_EDIT,
                 "Число итераций в одном такте расчета:",80);
  rdsFORMSetInt(win,fieldid,tactcount_setup);

  rdsFORMAddEdit(win,tabid,fieldid=L57SETUPFIELD_DFFILTERVALUE,RDS_FORMCTRL_EDIT,
                 "Допустимое изменение значения функции при фильтрации:",80);
  rdsSetObjectDouble(win,fieldid,RDS_FORMVAL_VALUE,DfFilter_setup);

  #if L57_MODELVERSION>=9
    // Вкладка внешнего вида
    if(L57_AllowTextRectEdit)
      { tabid++;
        L57_TextRectEdit_Setup(rdsbcppBlockData->Block,win,TR_params,TR_text.c_str(),tabid);
      }
  #endif

  Setup_FillFuncDescr(win,Func,Inputs,Numbered,Title);

  Setup_GetOptionsString(win,oldoptions);

  // Открываем окно
  res=rdsFORMShowModalServ(win,Setup_WinCallback);

  if(res)
    { char *newmodeltext=rdsGetObjectStr(win,L57SETUPFIELD_MODEL,RDS_FORMVAL_VALUE);
      L57_String options;

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

      Setup_GetOptionsString(win,options);
      if(modeltext.IsNotEqual(newmodeltext) || options.IsNotEqual(oldoptions))
        { L57_String texttosave;
          int ok=L57_ExecTool_SetOptions(templatetext.c_str(),newmodeltext,options.c_str(),&texttosave,&_CurFuncExportText);
          if(ok)
            ok=L57_SaveModelFile(rdsbcppBlockData->Block,texttosave.c_str());
          if(ok)
            { L57_CompileModel(rdsbcppBlockData->Block);
            }
          else
            rdsMessageBox("Ошибка записи текста модели блока",L57_MSGCAPTION,MB_OK|MB_ICONERROR);
        }
       noautostart_setup=(rdsGetObjectInt(win,L57SETUPFIELD_NOAUTOSTRT,RDS_FORMVAL_VALUE)!=0);
       deepsearch_setup=(rdsGetObjectInt(win,L57SETUPFIELD_DEEPSEARCH,RDS_FORMVAL_VALUE)!=0);
       ChunkSizeMb_setup=rdsFORMGetInt(win,L57SETUPFIELD_CHUNKMB);
       tactcount_setup=rdsGetObjectInt(win,L57SETUPFIELD_TACTCOUNT,RDS_FORMVAL_VALUE);
       rdsGetObjectDoubleP(win,L57SETUPFIELD_EQMAXDELTA,RDS_FORMVAL_VALUE,&EqualMaxDelta_setup);
       PrecToEndSteps_setup=(rdsGetObjectInt(win,L57SETUPFIELD_PRECENDSTEPS,RDS_FORMVAL_VALUE)!=0);
       PrecToDf_setup=(rdsGetObjectInt(win,L57SETUPFIELD_PRECDF,RDS_FORMVAL_VALUE)!=0);
       rdsGetObjectDoubleP(win,L57SETUPFIELD_DFVALUE,RDS_FORMVAL_VALUE,&DfDelta_setup);
       DfCycles_setup=rdsFORMGetInt(win,L57SETUPFIELD_DFCYCLES);
       PrecGridSize_setup=rdsFORMGetInt(win,L57SETUPFIELD_PRECGRID);
       NoPlateau_setup=(rdsGetObjectInt(win,L57SETUPFIELD_NOPLATEAU,RDS_FORMVAL_VALUE)!=0);
       TabAfterCalc_setup=(rdsGetObjectInt(win,L57SETUPFIELD_TABAFTERCALC,RDS_FORMVAL_VALUE)!=0);
       TabPrecCalc_setup=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECCALC,RDS_FORMVAL_VALUE)!=0);
       rdsGetObjectDoubleP(win,L57SETUPFIELD_DFFILTERVALUE,RDS_FORMVAL_VALUE,&DfFilter_setup);
       Tab_PrecToEndSteps_setup=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECENDSTEPS,RDS_FORMVAL_VALUE)!=0);
       Tab_PrecToDf_setup=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECDF,RDS_FORMVAL_VALUE)!=0);
       rdsGetObjectDoubleP(win,L57SETUPFIELD_TABDFVALUE,RDS_FORMVAL_VALUE,&Tab_DfDelta_setup);
       Tab_DfCycles_setup=rdsFORMGetInt(win,L57SETUPFIELD_TABDFCYCLES);
       Tab_PrecGridSize_setup=rdsFORMGetInt(win,L57SETUPFIELD_TABPRECGRID);

       if(!(PrecToEndSteps_setup||PrecToDf_setup))
         deepsearch_setup=FALSE;
       if(!(Tab_PrecToEndSteps_setup||Tab_PrecToDf_setup))
         TabPrecCalc_setup=FALSE;
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

// Параметры максимизации изменились
BOOL _InParamsChange;

// Вычислитель функции
L57_Calc _Calculator;

// Набор максимизируемых параметров
L57_Parameters _Params;

// Текущий максимум (значение)
double _GlobalMax;

// Список готовых максимумов
L57_MaxList _FinalMax;
// Список кандидатов в максимумы
L57_MaxList _Candidates;

BOOL Has_TabAfterCalcSimp;      // Есть стадия дорасчета простых таблиц
BOOL Has_TabAfterCalcAM;        // Есть стадия дорасчета таблиц arg max
BOOL Has_TabPrecCalc;           // Есть стадия уточнения таблиц arg max

BOOL _AddMaxToArgs;             // Необходимо добавить аргументы найденных максимумов в массивы отсчетов всех параметров
                                // (т.е. еще не добавлены)

// Прогресс
double _Progress_PreTabCur,_Progress_PreTabMax;   // На стадии первичного расчета выходных таблиц
double _Progress_GridCur,_Progress_GridMax;   // На стадии первичного перебора и отбора кандидатов
double _Progress_PrecCur,_Progress_PrecMax;   // На стадии уточнения кандидатов и записи максимумов
double _Progress_TabAfterCalcCurS,_Progress_TabAfterCalcMaxS;  // На стадии дорасчета простых выходных таблиц
double _Progress_TabAfterCalcCurAM,_Progress_TabAfterCalcMaxAM;  // На стадии дорасчета выходных таблиц arg max
double _Progress_TabPrecCalcCur,_Progress_TabPrecCalcMax;  // На стадии уточнения выходных таблиц arg max

#if L57_MODELVERSION>=2
  // Время
  DWORD _dwStartTime;
  // Вычислить время
  void _ShowTime(void)
    { Time_out=((double)(GetTickCount()-_dwStartTime))/1000.0; };
#endif

// Функции работы с выходными таблицами
#if L57_ONEARG_SIMPLE_CNT!=0
  // Заполнение массива генерации простых выходных таблиц F(x)
  BOOL _OneargSimpleF_fill(void);
#endif
#if L57_TWOARG_SIMPLE_CNT!=0
  // Заполнение массива генерации простых выходных таблиц F(x,y)
  BOOL _TwoargSimpleF_fill(void);
#endif
#if L57_ONEARG_ARGMAX_CNT!=0
  // Заполнение массива генерации выходных таблиц argmax(x)
  BOOL _OneargArgMaxF_fill(void);
#endif
#if L57_TWOARG_ARGMAX_CNT!=0
  // Заполнение массива генерации выходных таблиц argmax(x)
  BOOL _TwoargArgMaxF_fill(void);
#endif


// Для перебора простых табличных функций
// Сейчас формируются таблицы одного (1) или двух (2) аргументов
int _PreTab_NArg;
// Текущая формируемая таблица одного аргумента
int _PreTab_OneArg_Cur;
// Текущая формируемая таблица двух аргументов
int _PreTab_TwoArg_Cur;

// Для перебора табличных функций Arg Max
// Сейчас формируются таблицы одного (1) или двух (2) аргументов
int _ArgMax_NArg;
// Текущая формируемая таблица одного аргумента
int _ArgMax_OneArg_Cur;
// Текущая формируемая таблица двух аргументов
int _ArgMax_TwoArg_Cur;
// Текущая обрабатываемая точка функции Arg Max при уточнении
L57_MaxCandidate _ArgMax_TablePoint;
// Координаты этой точки
int _ArgMax_TP_Index1,_ArgMax_TP_Index2;

// Установить готовность всех простых табличных функций
void _SetAllSimpleTableFuncReady(char flag);
// Очистить выходные матрицы всех табличных функций
void _ClearAllTableFunc(void);
// Проверка изменения переменных
BOOL L57InputsChange(BOOL _force,BOOL *pChgSignalOn);
// Занесение стационарных переменных в вычислитель функции
void L57InputsToCalc(void);
// Занесение текущих значений параметров в вычислитель функции
void L57ParamsToCalc(void);
// Инициализации набора максимизируемых параметров
void L57InitParams(void);
// Начало максимизации (возвр. успешность инициализации)
BOOL L57StartIterations(void);
// Очистить все временные данные
void L57ClearTempData(void);
// Очистить все выходные данные (при ошибке)
void L57ClearOutputData(void);
// Записать ошибочные выходные данные
void L57SetErrorOutputData(void);
// Записать готовые выходные данные
void L57SetOutputData(void);
// Сообщение "мало памяти" с номером
void L57OutOfMemMsg(int n);
// Вывод прогресса
void Model_ShowProgress(void);
// Добавить текущую точку к списку готовых максимумов, если нужно (используется только при отключенном уточнении)
void Model_CurPointToAllMax_NoDeep(double f);
// Добавить первый максимум из _Candidates в _FinalMax
void Model_FirstCandidateToFinal(void);
// Переход к следующей стадии
void Model_NextStage(BOOL substageonly=FALSE);
// Взять следующую простую таблицу
// Возврат - есть еще что заполнять
BOOL Model_NextSimpleTable(BOOL fromzero=TRUE);
// Взять следующую таблицу ArgMax
// Возврат - есть еще что заполнять
BOOL Model_NextArgMaxTable(BOOL fromzero=TRUE);
// Такт расчета - стадия первичного построения простых выходных таблиц
void Model_PreTab(void);
// Такт расчета - стадия первичного перебора
void Model_GridStage(void);
// Такт расчета - стадия первичного перебора - подстадия запоминания и накопления
void Model_GridStage_Fill(void);
// Такт расчета - стадия первичного перебора - подстадия отбора кандидатов
void Model_GridStage_Cand(void);
// Такт расчета - стадия уточнения кандидатов в максимумы
void Model_PrecStage(void);
// Такт расчета - стадия дорасчета простых таблиц
void Model_TabAfterCalcSimp(void);
// Такт расчета - стадия дорасчета таблиц arg max
void Model_TabAfterCalcAM(void);
// Такт расчета - стадия уточнения таблиц
void Model_TabPrecCalc(void);
// Добавить максимумы в массивы отсчетов аргументов параметров
BOOL Model_AddAllMaxToParameters(void);
#if L57_ONEARG_ARGMAX_CNT!=0
  // Уточнение одной точки одномерной таблицы
  BOOL Model_TabPrecCalc_OneArgPoint(void);
#endif
#if L57_TWOARG_ARGMAX_CNT!=0
  // Уточнение одной точки двумерной таблицы
  BOOL Model_TabPrecCalc_TwoArgPoint(void);
#endif
// Переписать максимумы из AllMax_out в отдельные массивы (генерируется)
BOOL L57AllMaxToArrays(void);


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

#if L57_MODELVERSION>=2
  if(rdsGetSystemInt(RDS_GSISAVELOADACTION)==RDS_LS_LOADAUTOCOMP)
    { // При подключении модели после автокомпиляции пробуем передать функцию по связям
      L57_SendSingleFunction(rdsbcppBlockData->Block,Func_out.Num(),TRUE, // Immediate=TRUE !
                             _CurFuncExportText.IsEmpty()?_FuncExportText:_CurFuncExportText.c_str(),NULL);
    }
#endif
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
// После создания переменных
//---------------------------------------------------------------------------
#ifdef L57MS_VARCHECK
  #if L57_MODELVERSION>=3
    L57_Create_Trans_Data();
  #endif
#endif // #ifdef L57MS_VARCHECK
//-----------------------------------------------------------------------

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
  // Параметры максимизации изменились (для первой максимизации)
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
      #if L57_MODELVERSION>=2
        _enabled_res=1;
      #endif
      for(int i=0;enabled && i<size;i++)
        if(!Enabled_in[i])
          { enabled=FALSE;
            #if L57_MODELVERSION>=2
              _enabled_res=0;
            #endif
          }
    }
  #if L57_MODELVERSION>=2
  else
    enabled=_enabled_res;
  #endif

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
      else // Запускать автоматически прии изменениях входов
        dostart=_InParamsChange;
    }

  if(dostart)
    { // Запуск максимизации
      _InParamsChange=FALSE;
      Restart_cmd=0;
      Start_cmd=0;
      if(!L57StartIterations())
        { _Stage=L57STAGE_WAIT;
          return;
        }
      #if L57_MODELVERSION>=2
        Started_out=1;
        _dwStartTime=GetTickCount();
      #endif
    }
  else
    { if(_Stage==L57STAGE_WAIT && _somethingchanged && (!noautostart_setup)) // Не запускаем максимизацию и не работаем
        {
          #if L57_MODELVERSION>=2
            Finished_out=1;
            Complete_out=1;
            Error_out=0;
            Time_out=0;
          #endif
          _Ready=1;
          return;
        }
    }

  switch(_Stage)
    { case L57STAGE_WAIT:
        return;
      case L57STAGE_PRETAB:
        Model_PreTab();
        break;
      case L57STAGE_GRID:
        Model_GridStage();
        break;
      case L57STAGE_PREC:
        Model_PrecStage();
        break;
      case L57STAGE_TABAFTERCALCSIMP:
        Model_TabAfterCalcSimp();
        break;
      case L57STAGE_TABAFTERCALCAM:
        Model_TabAfterCalcAM();
        break;
      case L57STAGE_TABPRECCALC:
        Model_TabPrecCalc();
        break;
      default:
        rdsStopCalc();
        L57ClearTempData();
        L57ClearOutputData();
        _Stage=L57STAGE_WAIT;
        rdsBlockMessageBox(NULL,"Внутренняя ошибка максимизатора - неизвестный режим работы",
                      L57_MSGCAPTION,MB_OK|MB_ICONERROR);
        return;
    }
  Model_ShowProgress();
  _Ready=1;
  _Start=1;
  #if L57_MODELVERSION>=2
    _ShowTime();
  #endif
#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// Описания после класса
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS

// Сообщение "мало памяти" с номером
void rdsbcppBlockClass::L57OutOfMemMsg(int n)
{ L57_String msg,nstr;
  rdsStopCalc();
  L57ClearTempData();
  L57ClearOutputData();
  _Stage=L57STAGE_WAIT;
  nstr.AssignInt(n);
  msg.Assign("Мало памяти для работы максимизатора (");
  msg.AddString(nstr);
  msg.AddString(")");
  rdsBlockMessageBox(NULL,msg.c_str(),L57_MSGCAPTION,MB_OK|MB_ICONERROR);
}
//---------------------------------------------------------------------------

// Очистить все временные данные
void rdsbcppBlockClass::L57ClearTempData(void)
{
  _Params.ClearTempData();
  _Candidates.Clear();
  _FinalMax.Clear();
  #if L57_ONEARG_SIMPLE_CNT!=0
  for(int i=0;i<L57_ONEARG_SIMPLE_CNT;i++)
    _OneargSimpleF[i].Clear();
  #endif
  #if L57_TWOARG_SIMPLE_CNT!=0
  for(int i=0;i<L57_TWOARG_SIMPLE_CNT;i++)
    _TwoargSimpleF[i].Clear();
  #endif
  #if L57_ONEARG_ARGMAX_CNT!=0
  for(int i=0;i<L57_ONEARG_ARGMAX_CNT;i++)
    _OneargArgMaxF[i].Clear();
  #endif
  #if L57_TWOARG_ARGMAX_CNT!=0
  for(int i=0;i<L57_TWOARG_ARGMAX_CNT;i++)
    _TwoargArgMaxF[i].Clear();
  #endif
}
//---------------------------------------------------------------------------

// Очистить все выходные данные (при ошибке)
void rdsbcppBlockClass::L57ClearOutputData(void)
{
  AllMax_out.Resize(0,0);
  L57AllMaxToArrays();
  _results_ok=0; // MaxReady
  _SetAllSimpleTableFuncReady(0);
  _ClearAllTableFunc();
  Progress_out=0.0;
  #if L57_MODELVERSION>=2
    Complete_out=0;
    Error_out=0;
  #endif
}
//---------------------------------------------------------------------------

// Записать ошибочные выходные данные
void rdsbcppBlockClass::L57SetErrorOutputData(void)
{
  AllMax_out.Resize(0,0);
  L57AllMaxToArrays();
  _ClearAllTableFunc();
  _results_ok=1;
  _SetAllSimpleTableFuncReady(0);
  Progress_out=0.0;
  #if L57_MODELVERSION>=2
    Complete_out=Error_out=Finished_out=1;
  #endif
}
//---------------------------------------------------------------------------

// Записать готовые выходные данные
void rdsbcppBlockClass::L57SetOutputData(void)
{ BOOL ok=TRUE;
#ifdef L57_PARAMSCOUNT
  int n_max=_FinalMax.Count();
  #ifdef L57_LOG
    //L57_String s;
    L57_Log_Add(_LOGBLK,"  Запись AllMax_out");
  #endif
  //                   rows  cols
  if(AllMax_out.Resize(n_max,L57_PARAMSCOUNT+1))
    { double *arr=AllMax_out.GetBuffer();
      int shift=(L57_PARAMSCOUNT+1);
      for(L57_MaxCandidate *f=_FinalMax.First;f!=NULL;f=f->Next,arr+=shift)
        { memcpy(arr,f->CurParams,L57_PARAMSCOUNT*sizeof(double));
          arr[L57_PARAMSCOUNT]=f->f;
        }
      L57_SortMatrixColDesc(AllMax_out.GetBuffer(),AllMax_out.Rows(),AllMax_out.Cols(),L57_PARAMSCOUNT);
      if(!L57AllMaxToArrays())
        ok=FALSE;
    }
  else
    { AllMax_out.Resize(0,0);
      L57AllMaxToArrays();
      ok=FALSE;
    }



  #if L57_ONEARG_SIMPLE_CNT!=0
    // Сформированные простые функции одной переменной
    #ifdef L57_LOG
      L57_Log_Add(_LOGBLK,"  Запись F(x)");
    #endif
    for(int i=0;ok && i<L57_ONEARG_SIMPLE_CNT;i++)
      if(!_OneargSimpleF[i].SendToOutput(FALSE))
        ok=FALSE;
  #endif

  #if L57_TWOARG_SIMPLE_CNT!=0
    // Сформированные простые функции двух переменной
    #ifdef L57_LOG
      L57_Log_Add(_LOGBLK,"  Запись F(x,y)");
    #endif
    for(int i=0;ok && i<L57_TWOARG_SIMPLE_CNT;i++)
      if(!_TwoargSimpleF[i].SendToOutput(FALSE))
        ok=FALSE;
  #endif

  #if L57_ONEARG_ARGMAX_CNT!=0
    // Сформированные функции arg max одной переменной
    #ifdef L57_LOG
      L57_Log_Add(_LOGBLK,"  Запись argmax(x)");
    #endif
    for(int i=0;ok && i<L57_ONEARG_ARGMAX_CNT;i++)
      if(!_OneargArgMaxF[i].SendToOutput(FALSE))
        ok=FALSE;
  #endif

  #if L57_TWOARG_ARGMAX_CNT!=0
    // Сформированные функции arg max одной переменной
    #ifdef L57_LOG
      L57_Log_Add(_LOGBLK,"  Запись argmax(x,y)");
    #endif
    for(int i=0;ok && i<L57_TWOARG_ARGMAX_CNT;i++)
      if(!_TwoargArgMaxF[i].SendToOutput(FALSE))
        ok=FALSE;
  #endif

  if(!ok)
    L57OutOfMemMsg(5);
#endif // #ifdef L57_PARAMSCOUNT

  _results_ok=1;
  _SetAllSimpleTableFuncReady(1);
  Progress_out=1.0;
  #if L57_MODELVERSION>=2
    Complete_out=Finished_out=1;
    Error_out=ok?0:1;
  #endif
}
//---------------------------------------------------------------------------


// Начало максимизации (возвр. успешность инициализации)
#ifdef L57_PARAMSCOUNT
BOOL rdsbcppBlockClass::L57StartIterations(void)
{ BOOL _badalloc=FALSE,_error=FALSE;
  int _pc=0;
  double chunk_bytes;

  _GlobalMax=rdsbcppHugeDouble;
  #if L57_MODELVERSION>=2
    Complete_out=Error_out=Finished_out=0;
  #endif

  #ifdef L57_LOG
    L57_Log_Clear(_LOGBLK);
    L57_Log_Add(_LOGBLK,"Начало работы");
  #endif

  // Текущий этап
  #if L57_ONEARG_SIMPLE_CNT!=0 || L57_TWOARG_SIMPLE_CNT!=0
    _Stage=L57STAGE_PRETAB;
  #else
    _Stage=L57STAGE_GRID;
    _SubStage=L57SUBSTAGE_GRIDFILL;
  #endif

  _PreTab_OneArg_Cur=_PreTab_TwoArg_Cur=-1;
  _PreTab_NArg=0;
  _ArgMax_OneArg_Cur=_ArgMax_TwoArg_Cur=-1;
  _ArgMax_NArg=0;

  // Наличие дополнительных стадий
  #if L57_ONEARG_SIMPLE_CNT!=0 || L57_TWOARG_SIMPLE_CNT!=0
    // Есть стадия дорасчета простых таблиц
    Has_TabAfterCalcSimp=TabAfterCalc_setup && deepsearch_setup; // Без уточнения максимумов они всегда будут в сетке, нечего добавлять
  #else
    Has_TabAfterCalcSimp=FALSE;
  #endif
  #if L57_ONEARG_ARGMAX_CNT!=0 || L57_TWOARG_ARGMAX_CNT!=0
    // Есть стадия дорасчета таблиц arg max (таблицы одной переменной дорассчитывать не надо)
    Has_TabAfterCalcAM=TabAfterCalc_setup && deepsearch_setup; // Без уточнения максимумов они всегда будут в сетке, нечего добавлять
  #else
    Has_TabAfterCalcAM=FALSE;
  #endif
  #if L57_ONEARG_ARGMAX_CNT!=0 || L57_TWOARG_ARGMAX_CNT!=0
    // Есть стадия уточнения таблиц
    Has_TabPrecCalc=TabPrecCalc_setup;
  #else
    Has_TabPrecCalc=FALSE;
  #endif

  // Аргументы найденных максимумов добавляем в массивы параметров, только если есть какой-то дорасчет
  _AddMaxToArgs=Has_TabAfterCalcSimp||Has_TabAfterCalcAM;

  // Прогресс
  _Progress_PreTabCur=0.0;
  _Progress_PreTabMax=0.0;
  _Progress_GridCur=0.0;
  _Progress_GridMax=1.0;
  _Progress_PrecCur=0.0;
  _Progress_PrecMax=1.0;
  _Progress_TabAfterCalcCurS=0.0;
  _Progress_TabAfterCalcMaxS=Has_TabAfterCalcSimp?1.0:0.0;
  _Progress_TabAfterCalcCurAM=0.0;
  _Progress_TabAfterCalcMaxAM=Has_TabAfterCalcAM?1.0:0.0;
  _Progress_TabPrecCalcCur=0.0;
  _Progress_TabPrecCalcMax=Has_TabPrecCalc?1.0:0.0;

  L57InputsToCalc();

  L57ClearTempData();
  L57ClearOutputData();
  L57InitParams();

  // Проверка возможности максимизации
  for(int i=0;(!_error) && i<L57_PARAMSCOUNT;i++,_pc++)
    { L57_Parameter *p=_Params.P+i;
      // Заменить некоторые значения для возможности вычисления
      if(p->StartStep==rdsbcppHugeDouble)
        p->StartStep=1.0;
      if(p->EndStep==rdsbcppHugeDouble)
        p->EndStep=p->StartStep;
      // Теперь собственно проверка
      if(p->Min==rdsbcppHugeDouble)
        { _error=TRUE;
          // Добавить сообщение?
        }
      else if(p->Max!=rdsbcppHugeDouble && p->Max<p->Min)
        { _error=TRUE;
          // Добавить сообщение?
        }
   }
  if(_error || _pc==0)
    { L57ClearTempData();
      L57SetErrorOutputData();
      return FALSE;
    }

  // Отводим массивы аргументов
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=_Params.P+i;
      if(p->AllocateArray())
        p->EvalChunkSize=p->Array.Count;
      else
        _badalloc=TRUE;
      p->_ArgIndex=p->_ChunkIndex=0;
    }
  if(_badalloc)
   { rdsStopCalc();
     L57ClearTempData();
     L57ClearOutputData();
     _Stage=L57STAGE_WAIT;
     rdsBlockMessageBox(NULL,"Выбраны слишком маленькие шаги по одной или нескольким переменным, не хватает памяти для "
                        "размещения данных.",
                        L57_MSGCAPTION,MB_OK|MB_ICONERROR);
     return FALSE;
   }

  // Заполняем массивы наборов формируемых функций
  _Progress_PreTabMax=0.0;
  #if L57_ONEARG_SIMPLE_CNT!=0
  if(_OneargSimpleF_fill())
    { for(int i=0;i<L57_ONEARG_SIMPLE_CNT;i++)
        _Progress_PreTabMax+=_OneargSimpleF[i].ArgsArray->Count;
    }
  else
    _badalloc=TRUE;
  #endif

  #if L57_TWOARG_SIMPLE_CNT!=0
  if(_TwoargSimpleF_fill())
    { for(int i=0;i<L57_TWOARG_SIMPLE_CNT;i++)
        _Progress_PreTabMax+=_TwoargSimpleF[i].TotalArgs();
    }
  else
    _badalloc=TRUE;
  #endif

  #if L57_ONEARG_ARGMAX_CNT!=0
  if(!_OneargArgMaxF_fill())
    _badalloc=TRUE;
  #endif

  #if L57_TWOARG_ARGMAX_CNT!=0
  if(!_TwoargArgMaxF_fill())
    _badalloc=TRUE;
  #endif

  if(_badalloc)
   { rdsStopCalc();
     L57ClearTempData();
     L57ClearOutputData();
     _Stage=L57STAGE_WAIT;
     rdsBlockMessageBox(NULL,"Недостаточно памяти для формирования выходных табличных функций.",
                        L57_MSGCAPTION,MB_OK|MB_ICONERROR);
     return FALSE;
   }

  // Разбиваем диапазоны в параметрах на мелкие куски, допустимые по памяти
  if(ChunkSizeMb_setup<1.0)
    ChunkSizeMb_setup=1.0;
  chunk_bytes=ChunkSizeMb_setup*1024.0*1024.0; // В байтах
  //                      chunk_bytes=20*sizeof(double);
  //                      chunk_bytes=1000*sizeof(double);
  //                      chunk_bytes=38000;
  if(deepsearch_setup)
    { // Определение размера "куска" диапазона по каждому параметру
      // Исходно в EvalChunkSize записано полное число отсчетов
      for(;;)
        { double total=1.0;
          int maxchunk_index,maxchunk=-1;
          // Вычисление объема памяти под кусок массива
          for(int i=0;i<L57_PARAMSCOUNT;i++)
            { L57_Parameter *p=_Params.P+i;
              total*=p->EvalChunkSize;
              if(p->EvalChunkSize>maxchunk)
                { maxchunk=p->EvalChunkSize;
                  maxchunk_index=i;
                }
            }
          total*=sizeof(double);
          // Кусок займет total байтов
          if(total<=chunk_bytes) // Уложились
            break;
          // Максимальная длина куска - maxchunk в параметре maxchunk_index
          if(maxchunk<=2) // Какая-то ошибка
            { L57OutOfMemMsg(1);
              return FALSE;
            }
          // Делим самый большой кусок пополам
          total=ceil(((double)maxchunk)/2);
          _Params.P[maxchunk_index].EvalChunkSize=(int)total;
        } // for(;;)
    }
  // На данный момент в EvalChunkCount каждого параметра записан размер допустимого по памяти куска
  // массива по этому параметру

  // Отводим массивы кусков
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=_Params.P+i;
      double totalpoints=0;
      if(!p->AllocateChunkArray())
        { L57OutOfMemMsg(2);
          return FALSE;
        }
      for(int j=0;j<p->ChunkCount;j++)
        totalpoints+=(p->ChunkEnd[j]-p->ChunkStart[j]+1);
      _Progress_GridMax*=totalpoints;
    }
  if(deepsearch_setup)
    _Progress_GridMax*=2; // Каждую точку проходим по два раза: при вычислении и при отборе

  if(deepsearch_setup)
    { // Отводим массив для значений функции в текущем куске
      if(!_Params.CreateChunkValuesArray())
        { L57OutOfMemMsg(3);
          return FALSE;
        }
      _Params.ResetChunkValuesArray();
    }

  if(_badalloc)
   { L57ClearTempData();
     L57ClearOutputData();
     L57OutOfMemMsg(4);
     return FALSE;
   }

  return TRUE;
}
#else // #ifdef L57_PARAMSCOUNT
BOOL rdsbcppBlockClass::L57StartIterations(void)
{ rdsStopCalc();
  L57ClearTempData();
  L57ClearOutputData();
  _Stage=L57STAGE_WAIT;
  rdsBlockMessageBox(NULL,"Не заданы максимизируемые параметры",
                     L57_MSGCAPTION,MB_OK|MB_ICONWARNING);
  return FALSE;
}
#endif
//---------------------------------------------------------------------------

// Функция получения опций модели из окна настроечных параметров
void Setup_GetOptionsString(RDS_HOBJECT win,L57_String &options)
{ BOOL searchendsteps=(rdsGetObjectInt(win,L57SETUPFIELD_DEEPSEARCH,RDS_FORMVAL_VALUE)!=0) &&
                      (rdsGetObjectInt(win,L57SETUPFIELD_PRECENDSTEPS,RDS_FORMVAL_VALUE)!=0);
  options.Clear();
  options.Assign("EndStep=");
  options.AddString(searchendsteps?"1":"0");
}
//---------------------------------------------------------------------------

// Функция занесения параметров описания в окно настройки
void Setup_FillFuncDescr(RDS_HOBJECT win,L57_String &Func,L57_String &Inputs,L57_String &Numbered,L57_String &Title)
{ L57_String str;
  // Формирование списков параметров
  if(Numbered.IsNotEmpty())
    { str.Assign("Максимизируемые параметры: ");
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
  L57_String Func,Inputs,Numbered,Title,newmodeltext,options;
  BOOL deepsearch,endsteps,df;
  BOOL tab_deepsearch,tab_endsteps,tab_df;
  int starttab=1,res;
  RDS_BHANDLE show;
  #if L57_MODELVERSION>=2
    // Служебное поле - блокировка редактирования
    lock=rdsGetObjectStr(win,L57SETUPFIELD_LOCKEDIT,RDS_FORMVAL_VALUE);
  #endif

  #if L57_MODELVERSION>=3
    // Служебное поле: дополнительные данные о функциональном входе
    auxdata=rdsGetObjectStr(win,L57SETUPFIELD_ADDITIONALDATA,RDS_FORMVAL_VALUE);
  #else
    auxdata=NULL;
  #endif


  switch(data->Event)
    {
      #if L57_MODELVERSION>=3
      case RDS_FORMSERVEVENT_TCREOPEN:   // Открытие после временного закрытия
        templatetext=NULL;
      #endif
      case RDS_FORMSERVEVENT_CLICK:
        switch(data->CtrlId)
          { case L57SETUPFIELD_OUTTABBTN: // Вызов редактирования табличных функций
              starttab=2;
              break;
          }
        if(data->Event==RDS_FORMSERVEVENT_CLICK)
          templatetext=rdsGetObjectStr(win,L57SETUPFIELD_TEMPLATE,RDS_FORMVAL_VALUE);
        modeltext=rdsGetObjectStr(win,L57SETUPFIELD_MODEL,RDS_FORMVAL_VALUE);
        Setup_GetOptionsString(win,options);
        show=NULL;
        res=L57_ExecTool_Edit(templatetext,modeltext,options.c_str(),starttab,&newmodeltext,
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

      case RDS_FORMSERVEVENT_CHANGE: // Изменение поля
        deepsearch=(rdsGetObjectInt(win,L57SETUPFIELD_DEEPSEARCH,RDS_FORMVAL_VALUE)!=0);
        endsteps=(rdsGetObjectInt(win,L57SETUPFIELD_PRECENDSTEPS,RDS_FORMVAL_VALUE)!=0);
        df=(rdsGetObjectInt(win,L57SETUPFIELD_PRECDF,RDS_FORMVAL_VALUE)!=0);
        rdsSetObjectInt(win,L57SETUPFIELD_NOPLATEAU,RDS_FORMVAL_ENABLED,deepsearch);
        rdsSetObjectInt(win,L57SETUPFIELD_PRECENDSTEPS,RDS_FORMVAL_ENABLED,deepsearch);
        rdsSetObjectInt(win,L57SETUPFIELD_PRECDF,RDS_FORMVAL_ENABLED,deepsearch);
        rdsSetObjectInt(win,L57SETUPFIELD_PRECGRID,RDS_FORMVAL_ENABLED,deepsearch && endsteps);
        rdsSetObjectInt(win,L57SETUPFIELD_DFCYCLES,RDS_FORMVAL_ENABLED,deepsearch && df);
        rdsSetObjectInt(win,L57SETUPFIELD_DFVALUE,RDS_FORMVAL_ENABLED,deepsearch && df);

        tab_deepsearch=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECCALC,RDS_FORMVAL_VALUE)!=0);
        tab_endsteps=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECENDSTEPS,RDS_FORMVAL_VALUE)!=0);
        tab_df=(rdsGetObjectInt(win,L57SETUPFIELD_TABPRECDF,RDS_FORMVAL_VALUE)!=0);
        rdsSetObjectInt(win,L57SETUPFIELD_TABPRECENDSTEPS,RDS_FORMVAL_ENABLED,tab_deepsearch);
        rdsSetObjectInt(win,L57SETUPFIELD_TABPRECDF,RDS_FORMVAL_ENABLED,tab_deepsearch);
        rdsSetObjectInt(win,L57SETUPFIELD_TABPRECGRID,RDS_FORMVAL_ENABLED,tab_deepsearch && tab_endsteps);
        rdsSetObjectInt(win,L57SETUPFIELD_TABDFCYCLES,RDS_FORMVAL_ENABLED,tab_deepsearch && tab_df);
        rdsSetObjectInt(win,L57SETUPFIELD_TABDFVALUE,RDS_FORMVAL_ENABLED,tab_deepsearch && tab_df);

        #if L57_MODELVERSION>=9
          if(L57_AllowTextRectEdit)
            L57_TextRectEdit_EnableControls(win);
        #endif
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

// Взять следующую простую таблицу
// Возврат - есть еще что заполнять
BOOL rdsbcppBlockClass::Model_NextSimpleTable(BOOL fromzero)
{
#ifdef L57_PARAMSCOUNT
  for(;;)
    switch(_PreTab_NArg)
      { case 1: // Таблицы одного аргумента
          _PreTab_OneArg_Cur++;
          if(_PreTab_OneArg_Cur<L57_ONEARG_SIMPLE_CNT)
            { // Сброс индексов на начало
              #if L57_ONEARG_SIMPLE_CNT!=0
                L57_Table1ArgFuncData *f=_OneargSimpleF+_PreTab_OneArg_Cur;
                f->_CurIndex=fromzero?0:f->ArgsArray->PrevCount;
                if(f->_CurIndex>=f->ArgsArray->Count)
                  continue;
                for(int i=0;i<L57_PARAMSCOUNT;i++)
                  { L57_Parameter *p=_Params.P+i;
                    p->Current=p->FuncVal;
                  }
              #endif
              return TRUE;
            }
          _PreTab_NArg=2;
          _PreTab_TwoArg_Cur=-1;
          break;
        case 2: // Таблицы двух аргументов
          _PreTab_TwoArg_Cur++;
          if(_PreTab_TwoArg_Cur<L57_TWOARG_SIMPLE_CNT)
            { // Сбросить все индексы на начало
              #if L57_TWOARG_SIMPLE_CNT!=0
                L57_Table2ArgFuncData *f=_TwoargSimpleF+_PreTab_TwoArg_Cur;
                if(fromzero)
                  f->_CurIndex1=f->_CurIndex2=0;
                else
                  { if(f->Args2Array->PrevCount==f->Args2Array->Count) // Нет "вертикальной стойки" уголка
                      { f->_CurIndex1=f->Args1Array->PrevCount;
                        f->_CurIndex2=0;
                      }
                    else // Есть вертикальная стойка
                      { f->_CurIndex1=0;
                        f->_CurIndex2=f->Args2Array->PrevCount;
                      }
                  }
                if(f->_CurIndex1>=f->Args1Array->Count ||
                   f->_CurIndex2>=f->Args2Array->Count)
                  continue;
                for(int i=0;i<L57_PARAMSCOUNT;i++)
                  { L57_Parameter *p=_Params.P+i;
                    p->Current=p->FuncVal;
                  }
              #endif
              return TRUE;
            }
          return FALSE;
        default: // Самое начало
          _PreTab_NArg=1;
          _PreTab_OneArg_Cur=-1;
    }
#else // #ifdef L57_PARAMSCOUNT
  return FALSE;
#endif  
}
//---------------------------------------------------------------------------

// Взять следующую таблицу Arg Max
// Возврат - есть еще что заполнять
BOOL rdsbcppBlockClass::Model_NextArgMaxTable(BOOL fromzero)
{
#ifdef L57_PARAMSCOUNT
  for(;;)
    switch(_ArgMax_NArg)
      { case 1: // Таблицы одного аргумента
          _ArgMax_TP_Index1=_ArgMax_TP_Index2=-1;
          _ArgMax_OneArg_Cur++;
          if(_ArgMax_OneArg_Cur<L57_ONEARG_ARGMAX_CNT)
            { // Сброс индексов на начало
              #if L57_ONEARG_ARGMAX_CNT!=0
                L57_Table1ArgFuncData *f=_OneargArgMaxF+_ArgMax_OneArg_Cur;
                f->_CurIndex=fromzero?0:f->ArgsArray->PrevCount;
                if(f->_CurIndex>=f->ArgsArray->Count)
                  continue;
                for(int i=0;i<L57_PARAMSCOUNT;i++)
                  { L57_Parameter *p=_Params.P+i;
                    p->Current=p->FuncVal;
                  }
              #endif
              return TRUE;
            }
          _ArgMax_NArg=2;
          _ArgMax_TwoArg_Cur=-1;
          break;
        case 2: // Таблицы двух аргументов
          _ArgMax_TP_Index1=_ArgMax_TP_Index2=-1;
          _ArgMax_TwoArg_Cur++;
          if(_ArgMax_TwoArg_Cur<L57_TWOARG_ARGMAX_CNT)
            { // Сбросить все индексы на начало
              #if L57_TWOARG_ARGMAX_CNT!=0
                L57_Table2ArgFuncData *f=_TwoargArgMaxF+_ArgMax_TwoArg_Cur;
                if(fromzero)
                  f->_CurIndex1=f->_CurIndex2=0;
                else
                  { if(f->Args2Array->PrevCount==f->Args2Array->Count) // Нет "вертикальной стойки" уголка
                      { f->_CurIndex1=f->Args1Array->PrevCount;
                        f->_CurIndex2=0;
                      }
                    else // Есть вертикальная стойка
                      { f->_CurIndex1=0;
                        f->_CurIndex2=f->Args2Array->PrevCount;
                      }
                  }
                if(f->_CurIndex1>=f->Args1Array->Count ||
                   f->_CurIndex2>=f->Args2Array->Count)
                  continue;
                for(int i=0;i<L57_PARAMSCOUNT;i++)
                  { L57_Parameter *p=_Params.P+i;
                    p->Current=p->FuncVal;
                  }
              #endif
              return TRUE;
            }
          return FALSE;
        default: // Самое начало
          _ArgMax_NArg=1;
          _ArgMax_OneArg_Cur=-1;
          _ArgMax_TP_Index1=_ArgMax_TP_Index2=-1;
    }
#else // #ifdef L57_PARAMSCOUNT
  return FALSE;
#endif
}
//---------------------------------------------------------------------------

// Такт расчета - стадия первичного построения простых выходных таблиц
void rdsbcppBlockClass::Model_PreTab(void)
{ L57_LoopBreak _check;
  if(_PreTab_NArg<1)
    { if(!Model_NextSimpleTable())
        { Model_NextStage();
          return;
        }
    }
  if(_PreTab_NArg<1)
    return;

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    { 
      // Расчет одномерных таблиц
      #if L57_ONEARG_SIMPLE_CNT!=0
      if(_PreTab_NArg==1)
        { double val;
          // Вычисляем точку _CurIndex таблицы _PreTab_OneArg_Cur
          L57_Table1ArgFuncData *f=_OneargSimpleF+_PreTab_OneArg_Cur;
          L57_Parameter *p=_Params.P+f->ArgIndex;
          _Progress_PreTabCur++;
          // Заносим значение в параметры (остальные уже занесены)
          p->Current=p->Array.Data[f->_CurIndex];
          // Вычисляем функцию
          L57ParamsToCalc();
          val=_Calculator._Calc_main();
          // Заносим данные в таблицу
          *(f->CurIndexBase())=val;
          // Берем следующий индекс
          f->_CurIndex++;
          if(f->_CurIndex>=f->ArgsArray->Count) // Таблица кончилась
            {
              if(!Model_NextSimpleTable())
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif // #if L57_ONEARG_SIMPLE_CNT!=0

      #if L57_TWOARG_SIMPLE_CNT!=0
      if(_PreTab_NArg==2)
        { double val;
          // Вычисляем точку _CurIndex1,_CurIndex2 таблицы _PreTab_TwoArg_Cur
          L57_Table2ArgFuncData *f=_TwoargSimpleF+_PreTab_TwoArg_Cur;
          L57_Parameter *p1=_Params.P+f->Arg1Index;
          L57_Parameter *p2=_Params.P+f->Arg2Index;
          _Progress_PreTabCur++;
          // Заносим значение в параметры (остальные уже занесены)
          p1->Current=p1->Array.Data[f->_CurIndex1];
          p2->Current=p2->Array.Data[f->_CurIndex2];
          // Вычисляем функцию
          L57ParamsToCalc();
          val=_Calculator._Calc_main();
          // Заносим данные в таблицу
          *(f->CurIndexBase())=val;
          // Берем следующий индекс
          f->_CurIndex2++;
          if(f->_CurIndex2>=f->Args2Array->Count)
            { f->_CurIndex1++;
              f->_CurIndex2=0;
            }
          if(f->_CurIndex1>=f->Args1Array->Count) // Таблица кончилась
            { f->_CurIndex2=f->Args2Array->Count;
              if(!Model_NextSimpleTable())
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif
      if(_check.Check())
        break;
    } // for(int _iteration=0;...)

}
//---------------------------------------------------------------------------

// Такт расчета - стадия дорасчета простых таблиц
void rdsbcppBlockClass::Model_TabAfterCalcSimp(void)
{ L57_LoopBreak _check;

  if(_PreTab_NArg<1)
    { if(!Model_NextSimpleTable(FALSE))
        { Model_NextStage();
          return;
        }
      #ifdef L57_LOG
        L57_Log_Add(_LOGBLK,"  Model_TabAfterCalcSimp(): Начало работы");
      #endif
    }
  if(_PreTab_NArg<1)
    return;

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    {
      // Расчет одномерных таблиц (AddArgs выполнено)
      #if L57_ONEARG_SIMPLE_CNT!=0
      if(_PreTab_NArg==1)
        { double val;
          L57_Table1ArgFuncData *f=_OneargSimpleF+_PreTab_OneArg_Cur;
          // Вычисляем точку _CurIndex таблицы _PreTab_OneArg_Cur
          #ifdef L57_LOG
            L57_String s;
            s.Assign("  Таблица F(x): ");
            s.AddInt(_PreTab_OneArg_Cur);
            s.AddString(", индекс ");
            s.AddInt(f->_CurIndex);
            s.AddString(" из ");
            s.AddInt(f->ArgsArray->Count);
            L57_Log_Add(_LOGBLK,s);
          #endif
          if(f->_CurIndex<f->ArgsArray->Count)
            { L57_Parameter *p=_Params.P+f->ArgIndex;
              _Progress_TabAfterCalcCurS++;
              // Заносим значение в параметры (остальные уже занесены)
              p->Current=f->ArgsArray->Data[f->_CurIndex];
              // Вычисляем функцию
              L57ParamsToCalc();
              val=_Calculator._Calc_main();
              // Заносим данные в таблицу
              *(f->CurIndexBase())=val;
            }
          // Берем следующий индекс
          f->_CurIndex++;
          if(f->_CurIndex>=f->ArgsArray->Count) // Таблица кончилась
            {
              if(!Model_NextSimpleTable(FALSE))
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif // #if L57_ONEARG_SIMPLE_CNT!=0

      // Расчет двумерных таблиц (AddArgs выполнено)
      #if L57_TWOARG_SIMPLE_CNT!=0
      if(_PreTab_NArg==2)
        { double val;
          // Вычисляем точку _CurIndex1,_CurIndex2 таблицы _PreTab_TwoArg_Cur
          L57_Table2ArgFuncData *f=_TwoargSimpleF+_PreTab_TwoArg_Cur;
          #ifdef L57_LOG
            L57_String s;
            s.Assign("  Таблица F(x,y): ");
            s.AddInt(_PreTab_TwoArg_Cur);
            s.AddString(", индексы (");
            s.AddInt(f->_CurIndex1);
            s.AddString(",");
            s.AddInt(f->_CurIndex2);
            s.AddString(") из (");
            s.AddInt(f->Args1Array->Count);
            s.AddString(",");
            s.AddInt(f->Args2Array->Count);
            s.AddString(")...");
            L57_Log_Add(_LOGBLK,s,FALSE);
          #endif
          if(f->_CurIndex1<f->Args1Array->Count && f->_CurIndex2<f->Args2Array->Count)
            { L57_Parameter *p1=_Params.P+f->Arg1Index;
              L57_Parameter *p2=_Params.P+f->Arg2Index;
              _Progress_PreTabCur++;
              // Заносим значение в параметры (остальные уже занесены)
              p1->Current=f->Args1Array->Data[f->_CurIndex1];
              p2->Current=f->Args2Array->Data[f->_CurIndex2];

              // Вычисляем функцию
              L57ParamsToCalc();
              val=_Calculator._Calc_main();
              // Заносим данные в таблицу
              *(f->CurIndexBase())=val;
            }
          #ifdef L57_LOG
            L57_Log_Add(_LOGBLK,"OK");
          #endif
          // Берем следующий индекс
          f->_CurIndex2++;
          if(f->_CurIndex2>=f->Args2Array->Count)
            { f->_CurIndex1++;
              if(f->_CurIndex1<f->Args1Array->PrevCount) // "стойка уголка"
                f->_CurIndex2=f->Args2Array->PrevCount;
              else
                f->_CurIndex2=0;

            }
          if(f->_CurIndex1>=f->Args1Array->Count) // Таблица кончилась
            { f->_CurIndex2=f->Args2Array->Count;
              if(!Model_NextSimpleTable())
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif
      if(_check.Check())
        break;
    } // for(int _iteration=0;...)
  // Перезапускаемся для продолжения
  _Start=1;
}
//---------------------------------------------------------------------------

// Такт расчета - стадия дорасчета таблиц arg max
void rdsbcppBlockClass::Model_TabAfterCalcAM(void)
{
#ifdef L57_PARAMSCOUNT
  L57_LoopBreak _check;

  // Дорасчет только для двумерных таблиц, в одномерные просто переписываются максимумы
  #if L57_TWOARG_ARGMAX_CNT==0
    Model_NextStage();
  #else

  if(!Has_TabAfterCalcAM) // Нет дорасчета
    { Model_NextStage();
      _Start=1;
      return;
    }

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    { double f;

      _Progress_TabAfterCalcCurAM+=1.0;

      // Текущие значения параметров
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          p->Current=p->Array.Data[p->_ArgIndex];
        } // for(int i=0;...)
      L57ParamsToCalc();

      // Вычисление
      f=_Calculator._Calc_main();

      if(f!=rdsbcppHugeDouble)
        {
          for(int i=0;i<L57_TWOARG_ARGMAX_CNT;i++)
            { L57_Table2ArgFuncData *func=_TwoargArgMaxF+i;
              L57_Parameter *this_p1=_Params.P+func->Arg1Index;
              L57_Parameter *this_p2=_Params.P+func->Arg2Index;
              if(func->Arg1Index<0 || func->Arg2Index<0 ||
                 this_p1->_ArgIndex<0 || this_p2->_ArgIndex<0 ||
                 this_p1->_ArgIndex>=this_p1->Array.Count ||
                 this_p2->_ArgIndex>=this_p2->Array.Count)
                   { L57_ONCEDEBUGMSG("Плохие индексы")
                   }
              double *base=func->IndexBase(this_p1->_ArgIndex,this_p2->_ArgIndex);
              double *fval=base+L57_PARAMSCOUNT; // Значение функции
              if( (*fval)==rdsbcppHugeDouble || (*fval)<f )
                { // Новый максимум для текущих параметров
                  for(int j=0;j<L57_PARAMSCOUNT;j++)
                    base[j]=_Params.P[j].Current;
                  *fval=f;
                }
            }
        }


      // Следующий шаг
      if(_Params.NextStep())
        { // Перебор текущего куска закончен
          Model_NextStage();
          return;
        }
      // Перебор текущего куска не закончен - новые значения параметров уже установлены в NextStep

      if(_check.Check())
        break;
    } // for(int _iteration=0;...)

  #endif
  // Перезапускаемся для продолжения
  _Start=1;
#endif
}
//---------------------------------------------------------------------------

// Такт расчета - стадия первичного перебора
void rdsbcppBlockClass::Model_GridStage(void)
{
  switch(_SubStage)
    { case L57SUBSTAGE_GRIDFILL: // Запоминание и накопление
        Model_GridStage_Fill();
        break;
      case L57SUBSTAGE_GRIDCAND: // Отбор кандидатов
        Model_GridStage_Cand();
        break;
    }
}
//---------------------------------------------------------------------------

// Добавить текущую точку к списку готовых максимумов, если нужно (используется только при отключенном уточнении)
void rdsbcppBlockClass::Model_CurPointToAllMax_NoDeep(double f)
{
#ifdef L57_PARAMSCOUNT
  L57_MaxCandidate *c;
  int result;
  if(_GlobalMax==rdsbcppHugeDouble || f>_GlobalMax+EqualMaxDelta_setup)
    { // Новый максимум заменяет собой все старые
      _GlobalMax=f;
      _FinalMax.Clear();
      c=_FinalMax.AddFirst();
      c->f=f;
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        c->CurParams[i]=_Params.P[i].Current;
      return;
    }
  if(f<_GlobalMax-EqualMaxDelta_setup) // Точка сильно меньше максимумов
    return;
  // Точка попала в допустимый диапазон от _GlobalMax
  if(f>_GlobalMax)
    _GlobalMax=f;
  // Выбрасываем максимумы за пределами диапазона от _GlobalMax
  _FinalMax.RemoveSmall(_GlobalMax-EqualMaxDelta_setup);
  // Добавляем новый
  c=_FinalMax.AddFirst();
  c->f=f;
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    c->CurParams[i]=_Params.P[i].Current;
#endif
}
//---------------------------------------------------------------------------

// Добавить первый максимум из _Candidates в _FinalMax
void rdsbcppBlockClass::Model_FirstCandidateToFinal(void)
{
#ifdef L57_PARAMSCOUNT
  L57_MaxCandidate *c=_Candidates.RemoveTop();

  if(!c)
    return;

  if(_GlobalMax==rdsbcppHugeDouble || c->f>_GlobalMax+EqualMaxDelta_setup)
    { // Новый максимум заменяет собой все старые
      _GlobalMax=c->f;
      _FinalMax.Clear();
      _FinalMax.AddExistingFirst(c);
      return;
    }

  if(c->f<_GlobalMax-EqualMaxDelta_setup) // Точка сильно меньше максимумов
    { delete c;
      return;
    }

  // Точка попала в допустимый диапазон от _GlobalMax

  #ifdef L57_ENDSTEPSUSED
  // Проверяем, не равна ли она остальным по значениям параметров
  for(L57_MaxCandidate *x=_FinalMax.First;x!=NULL;x=x->Next)
    { BOOL eq=TRUE;
      for(int i=0;eq && i<L57_PARAMSCOUNT;i++)
        if(fabs(x->CurParams[i]-c->CurParams[i])>_Params.P[i].EndStep)
          eq=FALSE;
      if(eq) // В точности совпадает с уже найденным максимумом - выбрасываем
        { delete c;
          return;
        }
    }
  #endif

  if(c->f>_GlobalMax)
    _GlobalMax=c->f;
  // Выбрасываем максимумы за пределами диапазона от _GlobalMax
  _FinalMax.RemoveSmall(_GlobalMax-EqualMaxDelta_setup);
  // Добавляем новый
  _FinalMax.AddExistingFirst(c);
#endif  
}
//---------------------------------------------------------------------------

// Добавить максимумы в массивы отсчетов аргументов параметров
BOOL rdsbcppBlockClass::Model_AddAllMaxToParameters(void)
{
#ifdef L57_PARAMSCOUNT
  if(!_AddMaxToArgs) // Не нужно добавлять или все уже добавлено
    return TRUE;
  _AddMaxToArgs=FALSE;

  for(int i=0;i<L57_PARAMSCOUNT;i++)
    { L57_Parameter *p=_Params.P+i;
      int count=0;
      // Точность совпадения
      #ifdef L57_ENDSTEPSUSED
        double delta=p->EndStep/4;
      #else
        double delta=L57_SMALLDELTA;
      #endif

      // Добавляем максимумы в параметр p

      // Проверяем - не совпадает ли каждый максимум с одним из аргументов
      for(L57_MaxCandidate *x=_FinalMax.First;x!=NULL;x=x->Next)
        { double thisparam=x->CurParams[i];
          x->Tag1=TRUE;
          x->IntTag1=-1;
          // Не должен совпадать с другими максимумами
          for(L57_MaxCandidate *x1=x->Next;x1!=NULL;x1=x1->Next)
            if(fabs(thisparam-x1->CurParams[i])<delta) // Не подходит
              { x->Tag1=FALSE;
                break;
              }
          if(!x->Tag1)
            continue;
          // Не должен совпадать с аргументами
          for(int j=0;j<p->Array.Count;j++)
            if(fabs(thisparam-p->Array.Data[j])<delta) // Не подходит
              { x->Tag1=FALSE;
                break;
              }
          if(x->Tag1)
            { x->IntTag1=count; // Номер строки, которая будет добавлена
              count++;
            }
        } // for(L57_MaxCandidate *x=_FinalMax.First;...)
      if(!count) // Ничего не добавлено
        continue;

      // Нужно добавить count точек
      if(!p->AddArgs(count))
        { L57OutOfMemMsg(9);
          return FALSE;
        }
      if(p->AddedMaxObjects)
        rdsFree(p->AddedMaxObjects);
      p->AddedMaxObjects=(L57_MaxCandidate**)rdsAllocate(count*sizeof(L57_MaxCandidate*));
      p->AddedMaxObjectsCount=count;
      if(!p->AddedMaxObjects)
        { L57OutOfMemMsg(12);
          return FALSE;
        }
      count=0;
      for(L57_MaxCandidate *x=_FinalMax.First;x!=NULL;x=x->Next)
        if(x->Tag1)
          { int index=p->Array.PrevCount+count;
            p->Array.Data[index]=x->CurParams[i];
            p->AddedMaxObjects[count]=x;
            count++;
          }

    } // for(int i=0;...)
    
#endif // #ifdef L57_PARAMSCOUNT
  return TRUE;
}
//---------------------------------------------------------------------------

// Переход к следующей стадии
void rdsbcppBlockClass::Model_NextStage(BOOL substageonly)
{
#ifdef L57_PARAMSCOUNT

  int nextstage=L57STAGE_FINAL,nextsubstage=0;
  int stage=_Stage,substage=_SubStage;

  switch(stage)
    { case L57STAGE_PRETAB: // Предварительный расчет простых функций
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"Закончилась L57STAGE_PRETAB");
        #endif
        nextstage=L57STAGE_GRID;
        nextsubstage=L57SUBSTAGE_GRIDFILL;
        break;
      case L57STAGE_GRID: // Первичный перебор
        if(!deepsearch_setup) // Нет уточнения
          {
            #ifdef L57_LOG
              L57_Log_Add(_LOGBLK,"Закончилась L57STAGE_GRID");
            #endif
            if(Has_TabAfterCalcSimp)  // Есть стадия дорасчета простых таблиц
              { nextstage=L57STAGE_TABAFTERCALCSIMP;
                break;
              }
            if(Has_TabAfterCalcAM)  // Есть стадия дорасчета таблиц arg max
              { nextstage=L57STAGE_TABAFTERCALCAM;
                break;
              }
            // Нет стадии дорасчета таблиц
            if(Has_TabPrecCalc)  // Есть стадия уточнения таблиц
              { nextstage=L57STAGE_TABPRECCALC;
                break;
              }
            // Нет стадии уточнения таблиц
            nextstage=L57STAGE_FINAL;
            break;
          }
        if(substage==L57SUBSTAGE_GRIDCAND) // Выходим из подстадии отбора кандидатов
          { if(substageonly)
              { // Переходим из подстадии отбора кандидатов обратно в подстадию заполнения массива
                nextstage=L57STAGE_GRID;
                nextsubstage=L57SUBSTAGE_GRIDFILL;
                #ifdef L57_LOG
                  L57_Log_Add(_LOGBLK,"Закончилась L57STAGE_GRID/L57SUBSTAGE_GRIDCAND");
                #endif
              }
            else
              { // Вообще выходим из стадии первичного перебора
                nextstage=L57STAGE_PREC;
                #ifdef L57_LOG
                  L57_Log_Add(_LOGBLK,"Закончилась L57STAGE_GRID");
                #endif
              }
            break;
          }
        // Выходим из подстадии заполнения массива
        // Нужно переключиться на подстадию отбора кандидатов
        nextstage=L57STAGE_GRID;
        nextsubstage=L57SUBSTAGE_GRIDCAND;
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"Закончилась L57STAGE_GRID/L57SUBSTAGE_GRIDFILL");
        #endif
        break;

      case L57STAGE_PREC: // Уточнение кандидатов
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"Закончилась L57STAGE_PREC");
        #endif
        if(Has_TabAfterCalcSimp)  // Есть стадия дорасчета простых таблиц
          { nextstage=L57STAGE_TABAFTERCALCSIMP;
            break;
          }
        // Нет стадии дорасчета простых таблиц

      case L57STAGE_TABAFTERCALCSIMP: // Дорасчет простых таблиц
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"Закончилась L57STAGE_TABAFTERCALCSIMP");
        #endif
        if(Has_TabAfterCalcAM)  // Есть стадия дорасчета таблиц arg max
          { nextstage=L57STAGE_TABAFTERCALCAM;
            break;
          }
        // Нет стадии дорасчета таблиц arg max


      case L57STAGE_TABAFTERCALCAM: // Дорасчет таблиц arg max
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"Закончилась L57STAGE_TABAFTERCALCAM");
        #endif
        if(Has_TabPrecCalc)  // Есть стадия уточнения таблиц
          { nextstage=L57STAGE_TABPRECCALC;
            break;
          }
        // Нет стадии уточнения таблиц
        nextstage=L57STAGE_FINAL;
        break;
    }

  if(nextstage==L57STAGE_FINAL)
    { // Максимизация завершена
      #ifdef L57_LOG
        L57_Log_Add(_LOGBLK,"Конец работы");
      #endif
      L57SetOutputData();
      #ifdef L57_LOG
        L57_Log_Add(_LOGBLK,"Выходы записаны");
      #endif
      _Stage=L57STAGE_WAIT;
      _SubStage=0;
      _Ready=1;
      return;
    }
  // Переходим на следующую стадию
  _Stage=stage=nextstage;
  _SubStage=substage=nextsubstage;

  // Инициализация новой стадии
  switch(stage)
    { case L57STAGE_GRID: // Первичный перебор
        switch(substage)
          { case L57SUBSTAGE_GRIDCAND: // Стадия отбора кандидатов
              #ifdef L57_LOG
                L57_Log_Add(_LOGBLK,"Начинается L57STAGE_GRID/L57SUBSTAGE_GRIDCAND");
              #endif
              _Params.ResetCurrentChunk();
              break;
            case L57SUBSTAGE_GRIDFILL: // Стадия заполнения массива (после отбора кандидатов очередного куска)
              #ifdef L57_LOG
                L57_Log_Add(_LOGBLK,"Начинается L57STAGE_GRID/L57SUBSTAGE_GRIDFILL");
              #endif
              // Сбрасываем индексы на начало текущего куска
              _Params.ResetCurrentChunk();
              _Params.ResetChunkValuesArray();
              break;
          }
        break;

      case L57STAGE_PREC: // Уточнение кандидатов
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"Начинается L57STAGE_PREC");
        #endif
        _Params.ClearChunkValuesArray();
        // Прогресс для этой стадии
        _Progress_PrecCur=0.0;
        _Progress_PrecMax=_Candidates.Count();
        if(_Progress_PrecMax<0.5) // Какая-то ошибка
          _Progress_PrecMax=1.0;
        break;

      case L57STAGE_TABAFTERCALCSIMP: // Дорасчет простых таблиц
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"Начинается L57STAGE_TABAFTERCALCSIMP");
        #endif
        if(!Model_AddAllMaxToParameters())
          {
            #ifdef L57_LOG
              L57_Log_Add(_LOGBLK,"*** Ошибка Model_AddAllMaxToParameters()");
            #endif
            return;
          }
        _Progress_PrecCur=_Progress_PrecMax;
        _PreTab_OneArg_Cur=_PreTab_TwoArg_Cur=-1;
        _PreTab_NArg=0;
        _Progress_TabAfterCalcCurS=_Progress_TabAfterCalcMaxS=0.0;
        #if L57_ONEARG_SIMPLE_CNT!=0
          #ifdef L57_LOG
            L57_Log_Add(_LOGBLK,"  Добавление результатов в F(x)");
          #endif
          for(int i=0;i<L57_ONEARG_SIMPLE_CNT;i++)
            { int n=_OneargSimpleF[i].AddResultsFromPrevCount();//Model_AddAllMaxToOneArg(_OneargSimpleF+i);
              if(n<0)
                { L57OutOfMemMsg(10);
                  return;
                }
              _Progress_TabAfterCalcMaxS+=n;
            }
        #endif
        #if L57_TWOARG_SIMPLE_CNT!=0
          #ifdef L57_LOG
            L57_Log_Add(_LOGBLK,"  Добавление результатов в F(x,y)");
          #endif
          for(int i=0;i<L57_TWOARG_SIMPLE_CNT;i++)
            { L57_Table2ArgFuncData *f=_TwoargSimpleF+i;
              int total1,total2;
              if(!f->AddResultsFromPrevCount())
                { L57OutOfMemMsg(13);
                  return;
                }
              // Добавлено строк
              total1=f->Args1Array->Count-f->Args1Array->PrevCount;
              // Добавлено столбцов
              total2=f->Args2Array->Count-f->Args2Array->PrevCount;
              _Progress_TabAfterCalcMaxS+=total1*f->Args2Array->Count+total2*(f->Args1Array->Count-total1);
            }
        #endif
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"  Добавление результатов закончено");
        #endif
        break;

      case L57STAGE_TABAFTERCALCAM: // Дорасчет таблиц arg max
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"Начинается L57STAGE_TABAFTERCALCAM");
        #endif
        if(!Model_AddAllMaxToParameters())
          {
            #ifdef L57_LOG
              L57_Log_Add(_LOGBLK,"*** Ошибка Model_AddAllMaxToParameters()");
            #endif
            return;
          }
        _Progress_PrecCur=_Progress_PrecMax;
        _Progress_TabAfterCalcCurS=_Progress_TabAfterCalcMaxS;
        _Progress_TabAfterCalcCurAM=_Progress_TabAfterCalcMaxAM=0.0;
        _ArgMax_OneArg_Cur=_ArgMax_TwoArg_Cur=-1;
        _ArgMax_NArg=1;
        #if L57_ONEARG_ARGMAX_CNT!=0
          for(int i=0;i<L57_ONEARG_ARGMAX_CNT;i++)
            { L57_Table1ArgFuncData *f=_OneargArgMaxF+i;
              L57_Parameter *p=_Params.P+f->ArgIndex;
              int n=f->AddResultsFromPrevCount();
              if(n<0)
                { L57OutOfMemMsg(11);
                  return;
                }
              if(n==0)
                continue;
              // Сейчас все максимумы, которые добавлены в отсчеты, записаны в p->AddedMaxObjects
              // Заносим все параметры и значения этих максимумов в таблицу - их не нужно довычислять и уточнять
              for(int j=0;j<p->AddedMaxObjectsCount;j++)
                { L57_MaxCandidate *x=p->AddedMaxObjects[j];
                  int index=f->ArgsArray->PrevCount+j;
                  double *base=f->ResultBaseForArgN(index);
                  memcpy(base,x->CurParams,L57_PARAMSCOUNT*sizeof(double));
                  base[L57_PARAMSCOUNT]=x->f;
                }
            }
        #endif
        #if L57_TWOARG_ARGMAX_CNT!=0
          for(int i=0;i<L57_TWOARG_ARGMAX_CNT;i++)
            { L57_Table2ArgFuncData *f=_TwoargArgMaxF+i;
              if(!f->AddResultsFromPrevCount())
                { L57OutOfMemMsg(14);
                  return;
                }
            }
          // Максимум прогресса -  просто общее число перебираемых точек
          _Progress_TabAfterCalcMaxAM=1.0;
           for(int i=0;i<L57_PARAMSCOUNT;i++)
             { L57_Parameter *p=_Params.P+i;
               _Progress_TabAfterCalcMaxAM*=p->Array.Count;
               p->_ArgIndex=0;
             }
        #endif
        break;

      case L57STAGE_TABPRECCALC: // Уточнение таблиц arg max
        #ifdef L57_LOG
          L57_Log_Add(_LOGBLK,"Начинается L57STAGE_TABPRECCALC");
        #endif
        _Progress_PrecCur=_Progress_PrecMax;
        _Progress_TabAfterCalcCurS=_Progress_TabAfterCalcMaxS;
        _Progress_TabAfterCalcCurAM=_Progress_TabAfterCalcMaxAM;
        _Progress_TabPrecCalcMax=_Progress_TabPrecCalcCur=0.0;
        _ArgMax_OneArg_Cur=_ArgMax_TwoArg_Cur=-1;
        _ArgMax_NArg=0;
        #if L57_ONEARG_ARGMAX_CNT!=0
          for(int i=0;i<L57_ONEARG_ARGMAX_CNT;i++)
            { L57_Table1ArgFuncData *f=_OneargArgMaxF+i;
              L57_Parameter *p=_Params.P+f->ArgIndex;
              _Progress_TabPrecCalcMax+=p->Array.PrevCount; // У одномерных уточняется только начало таблицы
            }
        #endif
        #if L57_TWOARG_ARGMAX_CNT!=0
          for(int i=0;i<L57_TWOARG_ARGMAX_CNT;i++)
            { L57_Table2ArgFuncData *f=_TwoargArgMaxF+i;
              L57_Parameter *p1=_Params.P+f->Arg1Index,
                            *p2=_Params.P+f->Arg2Index;
              _Progress_TabPrecCalcMax+=p1->Array.Count*p2->Array.Count;
            }
        #endif
        break;
    }

  _Start=_Ready=1;

#else // #ifdef L57_PARAMSCOUNT
  L57SetOutputData();
  _Stage=L57STAGE_WAIT;
  _SubStage=0;
  _Ready=1;
#endif
}
//---------------------------------------------------------------------------

// Такт расчета - стадия первичного перебора - подстадия запоминания и накопления
void rdsbcppBlockClass::Model_GridStage_Fill(void)
{
#ifdef L57_PARAMSCOUNT
  L57_LoopBreak _check;

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    { double f;

      _Progress_GridCur+=1.0;

      // Текущие значения параметров
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          p->Current=p->Array.Data[p->_ArgIndex];
        } // for(int i=0;...)
      L57ParamsToCalc();

      // Вычисление
      f=_Calculator._Calc_main();

      #if L57_ONEARG_ARGMAX_CNT!=0 || L57_TWOARG_ARGMAX_CNT!=0
      if(f!=rdsbcppHugeDouble)
        { // Первичный расчет таблиц arg max
          #if L57_ONEARG_ARGMAX_CNT!=0
          for(int i=0;i<L57_ONEARG_ARGMAX_CNT;i++)
            { L57_Table1ArgFuncData *func=_OneargArgMaxF+i;
              L57_Parameter *this_p=_Params.P+func->ArgIndex;
              if(func->ArgIndex<0)
                { L57_ONCEDEBUGMSG("Плохой индекс")
                }
              double *base=func->ResultBaseForArgN(this_p->_ArgIndex);
              double *fval=base+L57_PARAMSCOUNT; // Значение функции
              if( (*fval)==rdsbcppHugeDouble || (*fval)<f )
                { // Новый максимум для текущих параметров
                  for(int j=0;j<L57_PARAMSCOUNT;j++)
                    base[j]=_Params.P[j].Current;
                  *fval=f;
                }
            }
          #endif
          #if L57_TWOARG_ARGMAX_CNT!=0
          if(!Has_TabAfterCalcAM) // Если есть дорасчет таблиц arg max, то все точки таблиц вычисляются при дорасчете, а не здесь
            for(int i=0;i<L57_TWOARG_ARGMAX_CNT;i++)
              { L57_Table2ArgFuncData *func=_TwoargArgMaxF+i;
                L57_Parameter *this_p1=_Params.P+func->Arg1Index;
                L57_Parameter *this_p2=_Params.P+func->Arg2Index;
                if(func->Arg1Index<0 || func->Arg2Index<0 ||
                   this_p1->_ArgIndex<0 || this_p2->_ArgIndex<0)
                     { L57_ONCEDEBUGMSG("Плохие индексы")
                     }
                double *base=func->IndexBase(this_p1->_ArgIndex,this_p2->_ArgIndex);
                double *fval=base+L57_PARAMSCOUNT; // Значение функции
                if( (*fval)==rdsbcppHugeDouble || (*fval)<f )
                  { // Новый максимум для текущих параметров
                    for(int j=0;j<L57_PARAMSCOUNT;j++)
                      base[j]=_Params.P[j].Current;
                    *fval=f;
                  }
              }
          #endif
        }
      #endif

      if(deepsearch_setup)
        { // Занесение значения в массив для последующего поиска кандидатов в максимумы (без проверок)
          double *valptr=_Params.GetChunkValuePtrByParams();
          if(valptr)
            *valptr=f;
        }
      else
        { // Добавление в список максимумов
          Model_CurPointToAllMax_NoDeep(f);
        }

      // Следующий шаг в текущем куске
      if(_Params.NextStep_Chunk())
        { // Перебор текущего куска закончен
          Model_NextStage();
          return;
        }
      // Перебор текущего куска не закончен - новые значения параметров уже установлены в NextStep

      if(_check.Check())
        break;
    } // for(int _iteration=0;...)
  // Перезапускаемся для продолжения
  _Start=1;
#endif
}
//---------------------------------------------------------------------------

// Такт расчета - стадия первичного перебора - подстадия отбора кандидатов
void rdsbcppBlockClass::Model_GridStage_Cand(void)
{
#ifdef L57_PARAMSCOUNT

  BOOL low,allequal;
  double thisval,*valptr;
  int _count=tactcount_setup;
  L57_LoopBreak _check;


  for(;;)
    {
      _Progress_GridCur+=1.0;

      //--------- Действия ----------
      // Обрабатываем точку p->Array.Data[p->_ArgIndex] - проверяем ее на возможный локальный максимум
      valptr=_Params.GetChunkValuePtrByParams();
      if(!valptr)
        { rdsStopCalc();
          L57ClearTempData();
          L57ClearOutputData();
          _Stage=L57STAGE_WAIT  ;
          rdsBlockMessageBox(NULL,"Внутренняя ошибка максимизатора - обращение к отсутствующей ячейке массива значений (1)",
                             L57_MSGCAPTION,MB_OK|MB_ICONERROR);
          return;
        }
      thisval=(*valptr);
      low=(thisval!=rdsbcppHugeDouble); // Флаг - все соседи не выше
      allequal=NoPlateau_setup; // Флаг - все соседи примерно равны
      // Осматриваем соседей
      _Params.InitAuxIndex(-1,1);
      while(low)
        { BOOL allzero,inside;
          inside=_Params.ValidMainAndAuxIndices(&allzero);
          if(inside && (!allzero))
            { valptr=_Params.GetChunkValuePtrByParamsAndAux();
              if(!valptr)
                { rdsStopCalc();
                  L57ClearTempData();
                  L57ClearOutputData();
                  _Stage=L57STAGE_WAIT;
                  rdsBlockMessageBox(NULL,"Внутренняя ошибка максимизатора - обращение к отсутствующей ячейке массива значений (2)",
                                     L57_MSGCAPTION,MB_OK|MB_ICONERROR);

                  return;
                }
              if((*valptr)!=rdsbcppHugeDouble)
                { if((*valptr)>thisval)
                    { low=allequal=FALSE; // Есть сосед выше
                      break;
                    }
                  if(allequal && fabs((*valptr)-thisval)>L57_SMALLDELTA)
                    allequal=FALSE;
                }
              else
                allequal=FALSE;
            } // if(inside && (!allzero))
          // Следующий шаг
          if(_Params.StepAuxIndex())
            break;
        }

      // allequal==TRUE если включено отсечение "плато" и все соседи одинаковые

      if((!allequal) && low) // Это - кандидат (все соседи не выше)
        { // Добавляем в список кандидатов
          L57_MaxCandidate *c=_Candidates.AddFirst();
          if(!c)
            { L57OutOfMemMsg(6);
              return;
            }
          c->f=thisval;
          c->cycles_in_df=0;
          for(int i=0;i<L57_PARAMSCOUNT;i++)
            { L57_Parameter *p=_Params.P+i;
              c->CurParams[i]=p->Array.Data[p->_ArgIndex];
              c->Deltas[i]=p->StartStep;
            }
        }
      //-----------------------------

      // Следующий шаг
      if(_Params.NextStep_Chunk())
        { // Перебор текущего куска закончен - нужно переключиться в подстадию накопления и взять следующий кусок
          if(_Params.NextChunk())
            { // Только что обработали последний кусок - переходим к стадии уточнения кандидатов
              _Progress_GridCur=_Progress_GridMax;
              Model_NextStage();
              return;
            }
          Model_NextStage(TRUE); // Другая подстадия, а не целая стадия
          return;
        }

      _count--;
      if(_count<=0 || _check.Check())
        break;
      // Остаемся в цикле, пока _count не достигнет нуля

    } // for(;;)
  // Перебор текущего куска не закончен - новые значения параметров установлены в NextStep
  _Ready=_Start=1; // Сами себя перезапускаем
#endif  
}
//---------------------------------------------------------------------------

// Такт расчета - стадия уточнения кандидатов в максимумы
void rdsbcppBlockClass::Model_PrecStage(void)
{
#ifdef L57_PARAMSCOUNT
  L57_LoopBreak _check;

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    { BOOL _cand_ready;
      L57_MaxCandidate *c=_Candidates.First;
      double baseparams[L57_PARAMSCOUNT];
      if(!c) // Кандидаты кончились
        { // Переходим к следующей стадии
          _Progress_PrecCur=_Progress_PrecMax;
          Model_NextStage();
          return;
        }

      // Обрабатываем кандидата c
      // Проверяем, не готов ли он уже
      _cand_ready=TRUE;
      if(PrecToEndSteps_setup) // Сначала нужно дойти до конечных шагов
        { for(int i=0;i<L57_PARAMSCOUNT;i++)
            { L57_Parameter *p=_Params.P+i;
              double dx=c->Deltas[i];
              if(dx!=rdsbcppHugeDouble && dx>p->EndStep)
                { _cand_ready=FALSE;
                  break;
                }
            }
        }
      if(_cand_ready && PrecToDf_setup) // Нужно войти в трубку погрешности
        { if(c->cycles_in_df<DfCycles_setup) // Недавно в трубке
            _cand_ready=FALSE;
        }
      if(_cand_ready) // Максимум уточнен
        { // Записываем его в окончательные и переходим к следующему
          Model_FirstCandidateToFinal();
          _Progress_PrecCur++;
          continue;
        }

      // Кандидат c не готов - уменьшаем его шаги в два раза
      c->Old_Val=c->f;
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        c->Deltas[i]*=0.5;
      c->StepsDown++;
      memcpy(baseparams,c->CurParams,L57_PARAMSCOUNT*sizeof(double));
      //c->Equal=TRUE;

      // Осматриваемся
      switch(PrecGridSize_setup)
        { case L57PRECGRID_4X4: _Params.InitAuxIndex(-2,2); break;
          default:              _Params.InitAuxIndex(-1,1);
        }

      for(;;)
        { // Текущие значения параметров
          BOOL valueinside=TRUE,all_2=TRUE;
          double f;
          for(int i=0;valueinside && i<L57_PARAMSCOUNT;i++)
            { L57_Parameter *p=_Params.P+i;
              double v=baseparams[i]+c->Deltas[i]*p->AuxIndex;
              if(v<p->_Min || v>p->_Max)
                valueinside=FALSE;
              else
                { p->Current=v;
                  if(p->AuxIndex==1 || p->AuxIndex==-1) // Не в старой сетке
                    all_2=FALSE;
                }
            } // for(int i=0;...)
          if(!all_2) // Значение не попадает в старую сетку
            { if(valueinside)
                { // Вычисление
                  L57ParamsToCalc();
                  f=_Calculator._Calc_main();
                }
              else
                f=rdsbcppHugeDouble;
              // Вычислено значение функции при текущих параметрах baseparams + c->Deltas*p->AuxIndex
              if(f!=rdsbcppHugeDouble && f>c->f) // Нашли новый максимум
                { c->f=f;
                  for(int i=0;i<L57_PARAMSCOUNT;i++)
                    c->CurParams[i]=_Params.P[i].Current;
                } // if(f!=rdsbcppHugeDouble)
            } // if(!all_2)
          if(_Params.StepAuxIndex())
            break;
        } // for(;;)
      // Осмотрели окрестности c, новый максимум в c->f и c->CurParams
      if(c->f==c->Old_Val) // В точности совпадает
       c->cycles_in_df++;
      else if(c->f!=rdsbcppHugeDouble)
        { double d=fabs(c->f-c->Old_Val);
          if(d<DfDelta_setup)
            c->cycles_in_df++; // В трубке
          else
            c->cycles_in_df=0; // Вне трубки
        }

      // Уходим на цикл - продолжаем обработку этого кандидата
      if(_check.Check())
        break;
    } // for(int _iteration=0;...)

  _Ready=_Start=1;
#endif  
}
//---------------------------------------------------------------------------

#if L57_ONEARG_ARGMAX_CNT!=0
// Уточнение одной точки одномерной таблицы
// (Вычисляем точку _CurIndex таблицы _ArgMax_OneArg_Cur)
// Возврат: TRUE - берем следующий индекс, FALSE - повторяем обработку этой точки (шаги уменьшены)
BOOL rdsbcppBlockClass::Model_TabPrecCalc_OneArgPoint(void)
{
#ifdef L57_PARAMSCOUNT

  L57_MaxCandidate *c=&_ArgMax_TablePoint;
  L57_Table1ArgFuncData *f=_OneargArgMaxF+_ArgMax_OneArg_Cur;


  double baseparams[L57_PARAMSCOUNT];
  BOOL _cand_ready;

  if(_ArgMax_TP_Index1!=f->_CurIndex)
    { // Индекс обрабатываемой точки изменился - инициализация рабочего объекта
      double *base=f->ResultBaseForArgN(f->_CurIndex);
      double *fval=base+L57_PARAMSCOUNT; // Значение функции
      _ArgMax_TP_Index1=f->_CurIndex;
      _Progress_TabPrecCalcCur++;
      c->f=*fval;
      c->cycles_in_df=0;
      c->StepsDown=0;
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          if(i==f->ArgIndex) // Этот параметр не трогаем
            { c->Deltas[i]=0.0;
              c->CurParams[i]=f->ArgsArray->Data[f->_CurIndex]; // Значение аргумента
            }
          else
            { c->Deltas[i]=p->StartStep;
              c->CurParams[i]=base[i];
            }
        }
    } // if(_ArgMax_TP_Index1!=f->_CurIndex)

  // Обрабатываем кандидата c
  // Проверяем, не готов ли он уже
  _cand_ready=TRUE;
  if(Tab_PrecToEndSteps_setup) // Сначала нужно дойти до конечных шагов
    { for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          double dx=c->Deltas[i];
          if(dx!=rdsbcppHugeDouble && dx>p->EndStep)
            { _cand_ready=FALSE;
              break;
            }
        }
    }
  if(_cand_ready && Tab_PrecToDf_setup) // Нужно войти в трубку погрешности
    { if(c->cycles_in_df<Tab_DfCycles_setup) // Недавно в трубке
        _cand_ready=FALSE;
    }
  if(_cand_ready) // Максимум уточнен
    { // Записываем его обратно в таблицу и переходим к следующему
      double *base=f->ResultBaseForArgN(f->_CurIndex);
      double *fval=base+L57_PARAMSCOUNT; // Значение функции
      memcpy(base,c->CurParams,L57_PARAMSCOUNT*sizeof(double));
      *fval=c->f;
      return TRUE;
    }

  // Максимум c не готов - уменьшаем его шаги в два раза
  c->Old_Val=c->f;
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    c->Deltas[i]*=0.5;
  c->StepsDown++;
  memcpy(baseparams,c->CurParams,L57_PARAMSCOUNT*sizeof(double));

  // Осматриваемся
  switch(Tab_PrecGridSize_setup)
    { case L57PRECGRID_4X4: _Params.InitAuxIndex(-2,2); break;
      default:              _Params.InitAuxIndex(-1,1);
    }

  for(;;)
    { // Текущие значения параметров
      BOOL valueinside=TRUE,all_2=TRUE;
      double f;
      for(int i=0;valueinside && i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          double v=baseparams[i]+c->Deltas[i]*p->AuxIndex;
          if(v<p->_Min || v>p->_Max)
            valueinside=FALSE;
          else
            { p->Current=v;
              if(p->AuxIndex==1 || p->AuxIndex==-1) // Не в старой сетке
                all_2=FALSE;
            }
        } // for(int i=0;...)
      if(!all_2) // Значение не попадает в старую сетку
        { if(valueinside)
            { // Вычисление
              L57ParamsToCalc();
              f=_Calculator._Calc_main();
            }
          else
            f=rdsbcppHugeDouble;
          // Вычислено значение функции при текущих параметрах baseparams + c->Deltas*p->AuxIndex
          if(f!=rdsbcppHugeDouble && f>c->f) // Нашли новый максимум
            { c->f=f;
              for(int i=0;i<L57_PARAMSCOUNT;i++)
                c->CurParams[i]=_Params.P[i].Current;
            } // if(f!=rdsbcppHugeDouble)
        } // if(!all_2)
      if(_Params.StepAuxIndex())
        break;
    } // for(;;)
  // Осмотрели окрестности c, новый максимум в c->f и c->CurParams
  if(c->f==c->Old_Val) // В точности совпадает
   c->cycles_in_df++;
  else if(c->f!=rdsbcppHugeDouble)
    { double d=fabs(c->f-c->Old_Val);
      if(d<Tab_DfDelta_setup)
        c->cycles_in_df++; // В трубке
      else
        c->cycles_in_df=0; // Вне трубки
    }

  // Уходим на цикл - продолжаем обработку этого кандидата

#endif
  return FALSE;
}
//---------------------------------------------------------------------------
#endif

#if defined(L57_PARAMSCOUNT) && L57_TWOARG_ARGMAX_CNT!=0
// Уточнение одной точки двумерной таблицы
// (Вычисляем точку _CurIndex1,_CurIndex2 таблицы _ArgMax_TwoArg_Cur)
// Возврат: TRUE - берем следующий индекс, FALSE - повторяем обработку этой точки (шаги уменьшены)
BOOL rdsbcppBlockClass::Model_TabPrecCalc_TwoArgPoint(void)
{
  L57_MaxCandidate *c=&_ArgMax_TablePoint;
  L57_Table2ArgFuncData *f=_TwoargArgMaxF+_ArgMax_TwoArg_Cur;
  L57_Parameter *p1=_Params.P+f->Arg1Index;
  L57_Parameter *p2=_Params.P+f->Arg2Index;

  double baseparams[L57_PARAMSCOUNT];
  BOOL _cand_ready;

  if(_ArgMax_TP_Index1!=f->_CurIndex1 || _ArgMax_TP_Index2!=f->_CurIndex2)
    { // Индекс обрабатываемой точки изменился - инициализация рабочего объекта
      double *base=f->IndexBase(f->_CurIndex1,f->_CurIndex2);
      double *fval=base+L57_PARAMSCOUNT; // Значение функции
      _ArgMax_TP_Index1=f->_CurIndex1;
      _ArgMax_TP_Index2=f->_CurIndex2;
      _Progress_TabPrecCalcCur++;
      c->f=*fval;
      c->cycles_in_df=0;
      c->StepsDown=0;
      for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          if(i==f->Arg1Index) // Этот параметр не трогаем
            { c->Deltas[i]=0.0;
              c->CurParams[i]=f->Args1Array->Data[f->_CurIndex1]; // Значение аргумента
            }
          else if(i==f->Arg2Index) // Этот параметр не трогаем
            { c->Deltas[i]=0.0;
              c->CurParams[i]=f->Args2Array->Data[f->_CurIndex2]; // Значение аргумента
            }
          else
            { c->Deltas[i]=p->StartStep;
              c->CurParams[i]=base[i];
            }
        }
    } // if(_ArgMax_TP_Index*!=f->_CurIndex*)

  // Обрабатываем кандидата c
  // Проверяем, не готов ли он уже
  _cand_ready=TRUE;
  if(Tab_PrecToEndSteps_setup) // Сначала нужно дойти до конечных шагов
    { for(int i=0;i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          double dx=c->Deltas[i];
          if(dx!=rdsbcppHugeDouble && dx>p->EndStep)
            { _cand_ready=FALSE;
              break;
            }
        }
    }
  if(_cand_ready && Tab_PrecToDf_setup) // Нужно войти в трубку погрешности
    { if(c->cycles_in_df<Tab_DfCycles_setup) // Недавно в трубке
        _cand_ready=FALSE;
    }
  if(_cand_ready) // Максимум уточнен
    { // Записываем его обратно в таблицу и переходим к следующему
      double *base=f->IndexBase(f->_CurIndex1,f->_CurIndex2);
      double *fval=base+L57_PARAMSCOUNT; // Значение функции
      memcpy(base,c->CurParams,L57_PARAMSCOUNT*sizeof(double));
      *fval=c->f;
      return TRUE;
    }

  // Максимум c не готов - уменьшаем его шаги в два раза
  c->Old_Val=c->f;
  for(int i=0;i<L57_PARAMSCOUNT;i++)
    c->Deltas[i]*=0.5;
  c->StepsDown++;
  memcpy(baseparams,c->CurParams,L57_PARAMSCOUNT*sizeof(double));

  // Осматриваемся
  switch(Tab_PrecGridSize_setup)
    { case L57PRECGRID_4X4: _Params.InitAuxIndex(-2,2); break;
      default:              _Params.InitAuxIndex(-1,1);
    }

  for(;;)
    { // Текущие значения параметров
      BOOL valueinside=TRUE,all_2=TRUE;
      double f;
      for(int i=0;valueinside && i<L57_PARAMSCOUNT;i++)
        { L57_Parameter *p=_Params.P+i;
          double v=baseparams[i]+c->Deltas[i]*p->AuxIndex;
          if(v<p->_Min || v>p->_Max)
            valueinside=FALSE;
          else
            { p->Current=v;
              if(p->AuxIndex==1 || p->AuxIndex==-1) // Не в старой сетке
                all_2=FALSE;
            }
        } // for(int i=0;...)
      if(!all_2) // Значение не попадает в старую сетку
        { if(valueinside)
            { // Вычисление
              L57ParamsToCalc();
              f=_Calculator._Calc_main();
            }
          else
            f=rdsbcppHugeDouble;
          // Вычислено значение функции при текущих параметрах baseparams + c->Deltas*p->AuxIndex
          if(f!=rdsbcppHugeDouble && f>c->f) // Нашли новый максимум
            { c->f=f;
              for(int i=0;i<L57_PARAMSCOUNT;i++)
                c->CurParams[i]=_Params.P[i].Current;
            } // if(f!=rdsbcppHugeDouble)
        } // if(!all_2)
      if(_Params.StepAuxIndex())
        break;
    } // for(;;)
  // Осмотрели окрестности c, новый максимум в c->f и c->CurParams
  if(c->f==c->Old_Val) // В точности совпадает
   c->cycles_in_df++;
  else if(c->f!=rdsbcppHugeDouble)
    { double d=fabs(c->f-c->Old_Val);
      if(d<Tab_DfDelta_setup)
        c->cycles_in_df++; // В трубке
      else
        c->cycles_in_df=0; // Вне трубки
    }

  // Уходим на цикл - продолжаем обработку этого кандидата

  return FALSE;
}
//---------------------------------------------------------------------------
#endif

// Такт расчета - стадия уточнения таблиц
void rdsbcppBlockClass::Model_TabPrecCalc(void)
{ L57_LoopBreak _check;
  if(_ArgMax_NArg<1)
    { if(!Model_NextArgMaxTable())
        { Model_NextStage();
          return;
        }
    }
  if(_ArgMax_NArg<1)
    return;

  for(int _iteration=0;_iteration<tactcount_setup;_iteration++)
    {
      // Расчет одномерных таблиц
      #if L57_ONEARG_ARGMAX_CNT!=0
      if(_ArgMax_NArg==1)
        { // Вычисляем точку _CurIndex таблицы _ArgMax_OneArg_Cur
          L57_Table1ArgFuncData *f=_OneargArgMaxF+_ArgMax_OneArg_Cur;
          if(!Model_TabPrecCalc_OneArgPoint())
            continue; // Повторяем обработку с уменьшенными шагами

          // Берем следующий индекс
          f->_CurIndex++;
          if(f->_CurIndex>=f->ArgsArray->PrevCount) // Таблица кончилась (хвост после PrevCount не требует уточнения)
            {
              if(!Model_NextArgMaxTable())
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif // #if L57_ONEARG_ARGMAX_CNT!=0

      #if L57_TWOARG_ARGMAX_CNT!=0
      if(_ArgMax_NArg==2)
        {
          // Вычисляем точку _CurIndex1,_CurIndex2 таблицы _ArgMax_TwoArg_Cur
          L57_Table2ArgFuncData *f=_TwoargArgMaxF+_ArgMax_TwoArg_Cur;
          if(!Model_TabPrecCalc_TwoArgPoint())
            continue; // Повторяем обработку с уменьшенными шагами

          // Берем следующий индекс
          f->_CurIndex2++;
          if(f->_CurIndex2>=f->Args2Array->Count)
            { f->_CurIndex1++;
              f->_CurIndex2=0;
            }
          if(f->_CurIndex1>=f->Args1Array->Count) // Таблица кончилась
            { f->_CurIndex2=f->Args2Array->Count;
              if(!Model_NextArgMaxTable())
               { Model_NextStage();
                 return;
               }
            }
          continue;
        }
      #endif
      if(_check.Check())
        break;
    } // for(int _iteration=0;...)
}
//---------------------------------------------------------------------------


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

  ADDTOPROGRESS(_Progress_PreTabCur,_Progress_PreTabMax)
  ADDTOPROGRESS(_Progress_GridCur,_Progress_GridMax)
  ADDTOPROGRESS(_Progress_PrecCur,_Progress_PrecMax)
  ADDTOPROGRESS(_Progress_TabAfterCalcCurS,_Progress_TabAfterCalcMaxS)
  ADDTOPROGRESS(_Progress_TabAfterCalcCurAM,_Progress_TabAfterCalcMaxAM)
  ADDTOPROGRESS(_Progress_TabPrecCalcCur,_Progress_TabPrecCalcMax)

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

#endif // #ifdef L57MS_AFTERCLASS
//---------------------------------------------------------------------------

