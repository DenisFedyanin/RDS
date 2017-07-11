//---------------------------------------------------------------------------
// Стандартные части модели внутреннего максимизатора
// функции трех переменных f(x,y,z)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Глобальные описания
//---------------------------------------------------------------------------
#ifdef L57MS_GLOBALS
#ifndef L57_MODELVERSION
  #define L57_MODELVERSION 0
#endif
#include <RdsL57/L57FuncXYZ.cpp>

// Имя функции перечисления сохраненных функций
#define L57FUNC_LIST ListFunc

// Имя входа идентификатора функции
#ifndef L57_FUNCINPUT
  #define L57_FUNCINPUT "FuncId"
#endif

#ifndef L57_3DFUNCOUT_XY
  #define L57_3DFUNCOUT_XY "F0xy"
  #define L57_3DFUNCOUT_YZ "F0yz"
  #define L57_3DFUNCOUT_XZ "F0xz"
#endif

//-----------------------------------------------------------------------

// Прототип функции вычисления - служебная (без перехвата исключений)
double _SelectedFunc(int id,const TL57TextFuncArg &arguments);
//-----------------------------------------------------------------------

// Функция вычисления с перехватом исключений
double SelectedFunc(int id,const TL57TextFuncArg &arguments)
{ double f=0.0;
  RDSBCPP_TRY
    { f=_SelectedFunc(id,arguments); }
  RDSBCPP_CATCHALL
    { f=rdsbcppHugeDouble; }
  return f;
}
//-----------------------------------------------------------------------

// Элемент связного списка кандидатов в максимумы
class TMaxCandidate
{ public:
    TMaxCandidate *Next;
    int x_index,y_index,z_index; // Индексы исходной точки в исходном массиве
    double x_init,y_init,z_init; // Аргументы исходной точки (потом понадобятся для вставки)
    double x,y,z;	// Текущие значения аргументов
    double f; // Значение функции
    double dx,dy,dz; // Текущие шаги
    int cycles_in_df; // Число шагов внутри трубки погрешности
    int steps_down; // Количество сделанных уменьшений шагов

    /*
    static int Count; // Счетчик созданных
    static void CheckMsg(int fixed=0); // Проверка на полное уничтожение
    */

    // Служебная функция уменьшения шага
    static void StepDown(double &step,int init_index,int arr_count,
                         int &dx0,int &dx1,double &aux_step,BOOL *p_reset=NULL);
    // Служебная функция уменьшения шага для уточнения таблиц
    static void StepDownStage4(double &step,int arr_count,
                               int &dx0,int &dx1,double &aux_step);

    TMaxCandidate(void)
      {Next=NULL;x_index=y_index=z_index=-1;
       x=y=z=f=x_init=y_init=z_init=dx=dy=dz=rdsbcppHugeDouble;
       cycles_in_df=0; steps_down=0;
       //Count++;
      };
    ~TMaxCandidate(){/*Count--;*/};
};
/*
int TMaxCandidate::Count=0;
*/
// Связный список кандидиатов в максимумы
class TMaxList
{ public:
    TMaxCandidate *First;

    // Пустой?
    inline BOOL IsEmpty(void){return First==NULL;};
    // Очистить
    void Clear(void);
    // Добавить
    TMaxCandidate *Add(void);
    // Отсоединить верхний
    TMaxCandidate *RemoveTop(void);
    // Вставить готовый
    void AddExisting(TMaxCandidate *f);
    // Вставить набор чисел
    void AddNew(double x,double y,double z,double f);
    // Подсчитать
    int Count(void);
    // Сумма всех шагов (для прогресса)
    double SumDeltas(void);
    // Поменяться содержимым с другим списком
    void Exchange(TMaxList &list);

    TMaxList(void){First=NULL;};
    ~TMaxList(){Clear();};
};
//-----------------------------------------------------------------------

/*
// Проверка на полное уничтожение записей о максимумах
void TMaxCandidate::CheckMsg(int fixed)
{
  if(Count>fixed)
    { rdsStopCalc();
      rdsMessageBox("Внутренняя ошибка - утечка памяти","Максимизатор",
        MB_ICONERROR|MB_OK);
      Count=fixed;
    }
}
//---------------------------------------------------------------------------
*/

// Служебная функция уменьшения шага
// step - текущий шаг (уменьшается)
// init_index - индекс координаты, с которой все началось
// arr_count - число координат в массиве
// dx0,dx1 - начало и конец смещения (-2, 0, 2)
// aux_step - копия step, но вместо значения ошибки записан ноль
// p_reset - указатель на переменную, сбрасываемую в FALSE если мы на границе протяженного диапазона
void TMaxCandidate::StepDown(double &step,int init_index,int arr_count,
                             int &dx0,int &dx1,double &aux_step,BOOL *p_reset)
{
  if(step==rdsbcppHugeDouble || arr_count==1)  // Вообще не трогаем эту координату
    { dx0=dx1=0;
      aux_step=0.0;
      return;
    }
  step/=2;
  aux_step=step;

  if(init_index==0) // Левее не смещаемся
    { dx0=0;
      if(p_reset) *p_reset=FALSE;
    }
  else
    dx0=-2;
  if(init_index==(arr_count-1)) // Правее не смещаемся
    { dx1=0;
      if(p_reset) *p_reset=FALSE;
    }
  else
    dx1=2;
}
//---------------------------------------------------------------------------

// Служебная функция уменьшения шага для уточнения таблиц
// step - текущий шаг (уменьшается)
// arr_count - число координат в массиве
// dx0,dx1 - начало и конец смещения (-2, 0, 2)
// aux_step - копия step, но вместо значения ошибки записан ноль
void TMaxCandidate::StepDownStage4(double &step,int arr_count,
                             int &dx0,int &dx1,double &aux_step)
{
  if(step==rdsbcppHugeDouble || arr_count==1)  // Вообще не трогаем эту координату
    { dx0=dx1=0;
      aux_step=0.0;
      return;
    }
  step/=2;
  aux_step=step;

  dx0=-2;
  dx1=2;
}
//---------------------------------------------------------------------------

// Очистить связный список кандидатов
void TMaxList::Clear(void)
{ TMaxCandidate *f,*fn;

  f=First;
  while(f)
    { fn=f->Next;
      delete f;
      f=fn;
    }
  First=NULL;
}
//---------------------------------------------------------------------------

// Добавить кандидата в связный список
TMaxCandidate *TMaxList::Add(void)
{ TMaxCandidate *f=new TMaxCandidate();
  f->Next=First;
  First=f;
  return f;
}
//---------------------------------------------------------------------------

// Вставить набор чисел
void TMaxList::AddNew(double x,double y,double z,double f)
{ TMaxCandidate *ef=new TMaxCandidate();
  ef->Next=First;
  First=ef;
  ef->x=x;
  ef->y=y;
  ef->z=z;
  ef->f=f;
}
//---------------------------------------------------------------------------

// Отсоединить верхнего кандидата от связного списка
TMaxCandidate *TMaxList::RemoveTop(void)
{ TMaxCandidate *f=First;
  if(!f)
    return f;
  First=f->Next;
  f->Next=NULL;
  return f;
}
//---------------------------------------------------------------------------

// Вставить готового кандидата
void TMaxList::AddExisting(TMaxCandidate *f)
{ f->Next=First;
  First=f;
}
//---------------------------------------------------------------------------

// Подсчитать кандидатов
int TMaxList::Count(void)
{ int n=0;
  for(TMaxCandidate *f=First;f!=NULL;f=f->Next)
    n++;
  return n;
}
//---------------------------------------------------------------------------

// Сумма всех шагов (для прогресса)
double TMaxList::SumDeltas(void)
{ double sum=0;
  for(TMaxCandidate *f=First;f!=NULL;f=f->Next)
    { if(f->dx!=rdsbcppHugeDouble) sum+=f->dx;
      if(f->dy!=rdsbcppHugeDouble) sum+=f->dy;
      if(f->dz!=rdsbcppHugeDouble) sum+=f->dz;
    }
  return sum;
}
//---------------------------------------------------------------------------

// Поменяться содержимым с другим списком
void TMaxList::Exchange(TMaxList &list)
{ TMaxCandidate *f=First;
  First=list.First;
  list.First=f;
}
//---------------------------------------------------------------------------

// Связанные массивы целых и вещественных чисел
class TDoubleArray
{ public:
    double *Dbl;
    //int *Int;
    int Count,Size;

    // Очистить
    void Clear(void); 
    // Отвести
    void Allocate(int n,BOOL setcount=FALSE);
    // Изменить количество
    void SetCount(int cnt);
    
    // Отсутствует ли в массиве число и находится ли оно внутри диапазона
    BOOL CanInsertValue(double v,double delta=1e-38);
  
    TDoubleArray(void){Dbl=NULL;/*Int=NULL;*/Count=Size=0;};
    ~TDoubleArray(){Clear();};
};
//---------------------------------------------------------------------------

// Очистить
void TDoubleArray::Clear(void)
{ //if(Int) delete[] Int;
  if(Dbl) delete[] Dbl;
  //Int=NULL; 
  Dbl=NULL;
  Count=Size=0;
}
//---------------------------------------------------------------------------

// Изменить количество
void TDoubleArray::SetCount(int cnt)
{
  if(cnt>=0 && cnt<Size)
    Count=cnt;
}
//---------------------------------------------------------------------------

// Отвести
void TDoubleArray::Allocate(int n,BOOL setcount)
{ Clear();
  if(n>0)  
    { //Int=new int[Size=n];
      Dbl=new double[Size=n];
      if(setcount)
        Count=Size;
    }
}
//---------------------------------------------------------------------------

// Поиск ограничивающих индексов
// array        - массив
// count        - число точек
// itemshift    - расстояние между соседними точками в массиве
// pLeft,pRight - возвращаемые индексы ограничивающих точек
// Возврат      - LININTER_*
#define LININTER_NONE     0       // Нет данных
#define LININTER_BELOW    1       // x левее массива
#define LININTER_ABOVE    2       // x правее массива
#define LININTER_BETWEEN  3       // x внутри массива
int FindNearestPoints(double *array,int count,int itemshift,double x,int *pLeft,int *pRight)
{ int left=0,right=count-1,mid,diap;
  double xleft,xright,xmid;

  if(count==1)
    { *pLeft=0;
      *pRight=0;
      return LININTER_NONE;
    }

  xleft=array[0];
  xright=array[(count-1)*itemshift];
  if(xleft==rdsbcppHugeDouble || xright==rdsbcppHugeDouble)
    { *pLeft=0;
      *pRight=0;
      return LININTER_NONE;
    }
  if(x<xleft)
    { *pLeft=0;
      *pRight=1;
      return LININTER_BELOW;
    }
  if(x>xright)
    { *pLeft=count-2;
      *pRight=count-1;
      return LININTER_ABOVE;
    }

  for(;;)
    switch(diap=right-left)
      { case 0:
        case 1:
          *pLeft=left;
          *pRight=right;
          return diap==0?LININTER_NONE:LININTER_BETWEEN;

        default: // Одна и более точек между left и right
          mid=left+diap/2;
          xmid=array[mid*itemshift];
          if(xmid==rdsbcppHugeDouble)
            { *pLeft=0;
              *pRight=0;
              return LININTER_NONE;
            }
          if(x<xmid)
            { right=mid;
            }
          else
            { left=mid;
            }
      }

}
//---------------------------------------------------------------------------


// Отсутствует ли в массиве число и находится ли оно внутри диапазона
BOOL TDoubleArray::CanInsertValue(double v,double delta)
{ int left,right;
  if(Count<2)
    return FALSE;
  if(FindNearestPoints(Dbl,Count,1,v,&left,&right)!=LININTER_BETWEEN)
    return FALSE;
  if(fabs(v-Dbl[left])<=delta)
    return FALSE;
  if(fabs(v-Dbl[right])<=delta)
    return FALSE;
  return TRUE;
}
//---------------------------------------------------------------------------


// Определение числа точек по диапазону и шагу
int CalcNPointsByDiap(double vmin,double vmax,double dv)
{ int n;
  if(vmin==rdsbcppHugeDouble)
    return -1;
  // М.б. весь диапазон состоит из одной точки?
  if(vmax==rdsbcppHugeDouble || dv==rdsbcppHugeDouble || dv<=0.0 || vmax<vmin+0.5*dv)
    return 1;
  RDSBCPP_TRY
    { double d=(vmax-vmin)/dv;
      n=1+(int)(d);
      if(n<2)
        n=-1;
    }
  RDSBCPP_CATCHALL
    { n=-1; }
  if(n>0)
    { // Проверяем последнюю точку
      double last=vmin+(n-1)*dv;
      if(vmax-last>0.1*dv)
        n++;
    }
  return n;
}
//-----------------------------------------------------------------------

// Значение из диапазона по номеру
inline double vInDiap(double vmin,double vmax,double dv,int i,int cnt)
{ return (i==0) ? vmin : ( (i==(cnt-1))?vmax:(vmin+i*dv) ); }
//-----------------------------------------------------------------------

// Сортировать матрицу для семейства (с пропуском одного индекса)
BOOL SortMatrixField(BOOL databyrows,int Rows,int Cols,
                     double *MData,int XIndex,int excludeindex)
{


  if(databyrows)
    { // Сначала - проверка, не отсортирована ли она
      int start;
      BOOL sorted=TRUE;
      double *rowBaseX=MData+XIndex*Cols,*rowBaseY;
      double last,cur;
      if(excludeindex==0)
        { if(Cols<2) return TRUE; // Сортировка невозможна
          last=rowBaseX[1];
          start=2;
        }
      else
        { last=*rowBaseX;
          start=1;
        }
      if(last==rdsbcppHugeDouble)
        return FALSE;
      for(int i=start;sorted && i<Cols;i++)
        if(i!=excludeindex)
          { cur=rowBaseX[i];
            if(cur==rdsbcppHugeDouble)
              return FALSE;
            if(cur<last)
              sorted=FALSE;
            else
              last=cur;
          }
      if(sorted) // Все в порядке
        return TRUE;
      // Нужно сортировать по строке XIndex
      //rowBaseY=MData+YIndex*Cols;
      for(int i=0;i<Cols-1;i++)
        if(i!=excludeindex)
          { // Поиск минимального значения от i до Cols
            int minindex=i;
            last=rowBaseX[i];
            for(int j=i+1;j<Cols;j++)
              if(j!=excludeindex)
                { cur=rowBaseX[j];
                  if(cur<last)
                    { last=cur;
                      minindex=j;
                    }
                }
            if(i!=minindex)
              { // Поменять местами ячейки i и minindex В КАЖДОЙ СТРОКЕ
                rowBaseY=MData;
                for(int j=0;j<Rows;j++,rowBaseY+=Cols)
                  { cur=rowBaseY[minindex];
                    rowBaseY[minindex]=rowBaseY[i];
                    rowBaseY[i]=cur;
                  }
              }
          } // for(int i=0;...)

    } // if(databyrows)
  else // Сортировка по столбцу XIndex
    { // Сначала - проверка, не отсортирована ли она
      int start;
      BOOL sorted=TRUE;
      double last,cur;
      if(excludeindex==0)
        { if(Rows<2) return TRUE; // Сортировка невозможна
          last=MData[XIndex+Cols];
          start=2;
        }
      else
        { last=MData[XIndex];
          start=1;
        }
      if(last==rdsbcppHugeDouble)
        return FALSE;
      for(int i=start;sorted && i<Rows;i++)
        if(i!=excludeindex)
          { cur=MData[XIndex+i*Cols];
            if(cur==rdsbcppHugeDouble)
              return FALSE;
            if(cur<last)
              sorted=FALSE;
            else
              last=cur;
          }
      if(sorted) // Все в порядке
        return TRUE;
      // Нужно сортировать по столбцу XIndex
      for(int i=0;i<Rows-1;i++)
        if(i!=excludeindex)
          { // Поиск минимального значения от i до Rows
            int minindex=i;
            last=MData[XIndex+i*Cols];
            for(int j=i+1;j<Rows;j++)
              if(j!=excludeindex)
                { cur=MData[XIndex+j*Cols];
                  if(cur<last)
                    { last=cur;
                      minindex=j;
                    }
                }
            if(i!=minindex)
              { // Поменять местами ячейки i и minindex В КАЖДОМ СТОЛБЦЕ
                double *cell_mi=MData+minindex*Cols,
                       *cell_i=MData+i*Cols;
                for(int j=0;j<Cols;j++)
                  { cur=cell_i[j];
                    cell_i[j]=cell_mi[j];
                    cell_mi[j]=cur;
                  }
              }
          } // for(int i=0;...)
    }
  return TRUE;
}
//---------------------------------------------------------------------------

// Сортировать матрицу для функции одной переменной
BOOL SortMatrix(BOOL databyrows,int Rows,int Cols,double *MData,int XIndex)
{

  if(databyrows)
    { // Сначала - проверка, не отсортирована ли она
      BOOL sorted=TRUE;
      double *rowBaseX=MData+XIndex*Cols,*rowBaseY;
      double last=*rowBaseX,cur;
      if(last==rdsbcppHugeDouble)
        return FALSE;
      for(int i=1;sorted && i<Cols;i++)
        { cur=rowBaseX[i];
          if(cur==rdsbcppHugeDouble)
            return FALSE;
          if(cur<last)
            sorted=FALSE;
          else
            last=cur;
        }
      if(sorted) // Все в порядке
        return TRUE;
      // Нужно сортировать по строке XIndex
      //rowBaseY=MData+YIndex*Cols;
      for(int i=0;i<Cols-1;i++)
        { // Поиск минимального значения от i до Cols
          int minindex=i;
          last=rowBaseX[i];
          for(int j=i+1;j<Cols;j++)
            { cur=rowBaseX[j];
              if(cur<last)
                { last=cur;
                  minindex=j;
                }
            }
          if(i!=minindex)
            { // Поменять местами ячейки i и minindex В КАЖДОЙ СТРОКЕ
              rowBaseY=MData;
              for(int j=0;j<Rows;j++,rowBaseY+=Cols)
                { cur=rowBaseY[minindex];
                  rowBaseY[minindex]=rowBaseY[i];
                  rowBaseY[i]=cur;
                }
            }
        } // for(int i=0;...)

    } // if(databyrows)
  else // Сортировка по столбцу XIndex
    { // Сначала - проверка, не отсортирована ли она
      BOOL sorted=TRUE;
      double last=MData[XIndex],cur;
      if(last==rdsbcppHugeDouble)
        return FALSE;
      for(int i=1;sorted && i<Rows;i++)
        { cur=MData[XIndex+i*Cols];
          if(cur==rdsbcppHugeDouble)
            return FALSE;
          if(cur<last)
            sorted=FALSE;
          else
            last=cur;
        }
      if(sorted) // Все в порядке
        return TRUE;
      // Нужно сортировать по столбцу XIndex
      for(int i=0;i<Rows-1;i++)
        { // Поиск минимального значения от i до Rows
          int minindex=i;
          last=MData[XIndex+i*Cols];
          for(int j=i+1;j<Rows;j++)
            { cur=MData[XIndex+j*Cols];
              if(cur<last)
                { last=cur;
                  minindex=j;
                }
            }
          if(i!=minindex)
            { // Поменять местами ячейки i и minindex В КАЖДОМ СТОЛБЦЕ
              double *cell_mi=MData+minindex*Cols,
                     *cell_i=MData+i*Cols;
              for(int j=0;j<Cols;j++)
                { cur=cell_i[j];
                  cell_i[j]=cell_mi[j];
                  cell_mi[j]=cur;
                }
            }
        } // for(int i=0;...)
    }

  return TRUE;
}
#endif // #ifdef L57MS_GLOBALS
//-----------------------------------------------------------------------





//---------------------------------------------------------------------------
// Настройки
//---------------------------------------------------------------------------
#ifdef L57MS_SETUP
TL57TextFile file;
TL57TextString str;
RDS_HOBJECT win;
BOOL res;
char caption[]="Максимизатор";

RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

if(model!=NULL && rdscompIsEditorOpen(model->Model))
  { rdsMessageBox("Для настройки этого блока необходимо закрыть редактор его модели. "
                  "Пожалуйста, закройте редактор.",caption,MB_OK|MB_ICONWARNING);
    rdscompOpenBlockModelEditor(rdsbcppBlockData->Block);
    return;
  }

win=rdsFORMCreate(TRUE,-1,-1,caption);
if(!win)
  return;

CheckConnection();


// Читаем список функций из файла
file.CanEditFuncId=L57_FUNCIDEDIT;
file.Setup_ReadFuncFile(rdsbcppBlockData->Block,model!=NULL,L57FUNC_LIST,win);

// Вкладка выбора функции
rdsFORMAddTab(win,1,"Функция");
file.Setup_AddFuncSelectFields(win,NULL,1,1,SetupFuncId,model!=NULL,L57TF_ALLBUTTONS,NULL,
#if L57_MODELVERSION>=1
  5*24
#else
  4*24
#endif
);

// Вкладка перебора
rdsFORMAddTab(win,3,"Перебор");
SetupVar(win,3,100,"x",setup_xmin,setup_xmax,setup_dx,
         in_xdiap,in_dx,TRUE,TRUE);
SetupVar(win,3,200,"y",setup_ymin,setup_ymax,setup_dy,
         in_ydiap,in_dy,FALSE,TRUE);
SetupVar(win,3,300,"z",setup_zmin,setup_zmax,setup_dz,
         in_zdiap,in_dz,FALSE,FALSE);

// Вкладка общих параметров
rdsFORMAddTab(win,6,"Уточнение");

rdsFORMAddEdit(win,6,1008,RDS_FORMCTRL_EDIT,"Погрешность одинаковости максимумов:",80);
rdsSetObjectDouble(win,1008,RDS_FORMVAL_VALUE,setup_df_equal);

rdsFORMAddEdit(win,6,1000,RDS_FORMCTRL_CHECKBOX,"Уточнять максимумы",80);
rdsSetObjectInt(win,1000,RDS_FORMVAL_VALUE,deep_search?1:0);

#if L57_MODELVERSION>=1
  rdsFORMAddEdit(win,6,1009,RDS_FORMCTRL_CHECKBOX,"Убирать лишние точки на \"плато\"",80);
  rdsSetObjectInt(win,1009,RDS_FORMVAL_VALUE,setup_no_plateau?1:0);
#endif

rdsFORMAddEdit(win,6,1003,RDS_FORMCTRL_CHECKBOX,"Уточнять до заданных шагов",80);
rdsSetObjectInt(win,1003,RDS_FORMVAL_VALUE,setup_end_steps?1:0);

rdsFORMAddEdit(win,6,1004,RDS_FORMCTRL_EDIT,"Конечный шаг по X:",80);
rdsSetObjectDouble(win,1004,RDS_FORMVAL_VALUE,setup_dx_min);

rdsFORMAddEdit(win,6,1005,RDS_FORMCTRL_EDIT,"Конечный шаг по Y:",80);
rdsSetObjectDouble(win,1005,RDS_FORMVAL_VALUE,setup_dy_min);

rdsFORMAddEdit(win,6,1006,RDS_FORMCTRL_EDIT,"Конечный шаг по Z:",80);
rdsSetObjectDouble(win,1006,RDS_FORMVAL_VALUE,setup_dz_min);

rdsFORMAddEdit(win,6,1001,RDS_FORMCTRL_CHECKBOX,"Уточнять до погрешности функции",80);
rdsSetObjectInt(win,1001,RDS_FORMVAL_VALUE,search_by_df?1:0);

rdsFORMAddEdit(win,6,1002,RDS_FORMCTRL_EDIT,"Погрешность значения функции:",80);
rdsSetObjectDouble(win,1002,RDS_FORMVAL_VALUE,setup_df);

rdsFORMAddEdit(win,6,1007,RDS_FORMCTRL_UPDOWN,"Итераций в трубке погрешности:",80);
rdsSetObjectInt(win,1007,RDS_FORMVAL_VALUE,setup_df_cycles);
rdsSetObjectInt(win,1007,RDS_FORMVAL_UPDOWNMIN,0);
rdsSetObjectInt(win,1007,RDS_FORMVAL_UPDOWNMAX,30000);
rdsSetObjectInt(win,1007,RDS_FORMVAL_UPDOWNINC,1);


#if L57_MODELVERSION>=2
  // Вкладка уточнения таблиц
  rdsFORMAddTab(win,8,"Таблицы");

  rdsFORMAddEdit(win,8,2000,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицу XY: (x,y)*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2000,RDS_FORMVAL_VALUE,setup_ds_XY?1:0);

  rdsFORMAddEdit(win,8,2001,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицу XZ: (x,z)*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2001,RDS_FORMVAL_VALUE,setup_ds_XZ?1:0);

  rdsFORMAddEdit(win,8,2002,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицу YZ: (y,z)*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2002,RDS_FORMVAL_VALUE,setup_ds_YZ?1:0);

  rdsFORMAddEdit(win,8,2003,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицы X и Fx: x*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2003,RDS_FORMVAL_VALUE,setup_ds_X?1:0);

  rdsFORMAddEdit(win,8,2004,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицы Y и Fy: y*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2004,RDS_FORMVAL_VALUE,setup_ds_Y?1:0);

  rdsFORMAddEdit(win,8,2005,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицы Z и Fz: z*=arg max f(x,y,z) ",80);
  rdsSetObjectInt(win,2005,RDS_FORMVAL_VALUE,setup_ds_Z?1:0);

  rdsFORMAddEdit(win,8,2100,RDS_FORMCTRL_EDIT,"Конечный шаг по X:",80);
  rdsSetObjectDouble(win,2100,RDS_FORMVAL_VALUE,setup_ds_edx);

  rdsFORMAddEdit(win,8,2101,RDS_FORMCTRL_EDIT,"Конечный шаг по Y:",80);
  rdsSetObjectDouble(win,2101,RDS_FORMVAL_VALUE,setup_ds_edy);

  rdsFORMAddEdit(win,8,2102,RDS_FORMCTRL_EDIT,"Конечный шаг по Z:",80);
  rdsSetObjectDouble(win,2102,RDS_FORMVAL_VALUE,setup_ds_edz);
#endif

#if L57_MODELVERSION>=7
  // Вкладка вывода поверхности
  rdsFORMAddTab(win,9,"Поверхности");

  rdsFORMAddEdit(win,9,3002,RDS_FORMCTRL_MULTILABEL|RDS_FORMFLAG_LINE,
                     "Фунции двух аргументов на выходах \"" L57_3DFUNCOUT_XY "\", \"" L57_3DFUNCOUT_YZ "\" и \""
                     L57_3DFUNCOUT_XZ "\":",0);
  rdsSetObjectInt(win,3002,RDS_FORMVAL_MLHEIGHT,20);
  rdsSetObjectInt(win,3002,RDS_FORMVAL_MLABELBEVEL,0);

  rdsFORMAddEdit(win,9,3000,RDS_FORMCTRL_CHECKBOX,"Выводить после каждого прохода первого аргумента",80);
  rdsSetObjectInt(win,3000,RDS_FORMVAL_VALUE,setup_surf_everyrow?1:0);

  rdsFORMAddEdit(win,9,3001,RDS_FORMCTRL_EDIT|RDS_FORMFLAG_CHECK,"Минимальный интервал вывода, сек.:",80);
  rdsSetObjectDouble(win,3001,RDS_FORMVAL_VALUE,setup_surf_delayval);
  rdsSetObjectInt(win,3001,RDS_FORMVAL_CHECK,setup_surf_bydelay?1:0);
#endif


// Открываем окно
res=rdsFORMShowModalServ(win,WinCallback);

if(res)
  { // Ищем Num функции
    SetupFuncId=file.FuncNumByIndex(rdsGetObjectInt(win,1,RDS_FORMVAL_VALUE),SetupFuncId);

    // Сохраняем файл, если он изменился
    if(model!=NULL)
      file.Setup_WriteFuncFile(rdsbcppBlockData->Block,L57_BAK);

     // Вкладка X
     ReadSetupVar(win,100,setup_xmin,setup_xmax,setup_dx,
                  in_xdiap,in_dx);
     // Вкладка Y
     ReadSetupVar(win,200,setup_ymin,setup_ymax,setup_dy,
                  in_ydiap,in_dy);
     // Вкладка Z
     ReadSetupVar(win,300,setup_zmin,setup_zmax,setup_dz,
                  in_zdiap,in_dz);

     // Вкладка общих параметров
     deep_search=(rdsGetObjectInt(win,1000,RDS_FORMVAL_VALUE)!=0);
     setup_df=rdsGetObjectDouble(win,1002,RDS_FORMVAL_VALUE);

     search_by_df=(rdsGetObjectInt(win,1001,RDS_FORMVAL_VALUE)!=0);
     setup_end_steps=(rdsGetObjectInt(win,1003,RDS_FORMVAL_VALUE)!=0);
     setup_dx_min=rdsGetObjectDouble(win,1004,RDS_FORMVAL_VALUE);
     setup_dy_min=rdsGetObjectDouble(win,1005,RDS_FORMVAL_VALUE);
     setup_dz_min=rdsGetObjectDouble(win,1006,RDS_FORMVAL_VALUE);
     setup_df_cycles=rdsGetObjectInt(win,1007,RDS_FORMVAL_VALUE);
     setup_df_equal=rdsGetObjectDouble(win,1008,RDS_FORMVAL_VALUE);
     #if L57_MODELVERSION>=1
       setup_no_plateau=(rdsGetObjectInt(win,1009,RDS_FORMVAL_VALUE)!=0);
     #endif

    #if L57_MODELVERSION>=2
      setup_ds_XY=(rdsGetObjectInt(win,2000,RDS_FORMVAL_VALUE)!=0);
      setup_ds_XZ=(rdsGetObjectInt(win,2001,RDS_FORMVAL_VALUE)!=0);
      setup_ds_YZ=(rdsGetObjectInt(win,2002,RDS_FORMVAL_VALUE)!=0);
      setup_ds_X=(rdsGetObjectInt(win,2003,RDS_FORMVAL_VALUE)!=0);
      setup_ds_Y=(rdsGetObjectInt(win,2004,RDS_FORMVAL_VALUE)!=0);
      setup_ds_Z=(rdsGetObjectInt(win,2005,RDS_FORMVAL_VALUE)!=0);
      setup_ds_edx=rdsGetObjectDouble(win,2100,RDS_FORMVAL_VALUE);
      setup_ds_edy=rdsGetObjectDouble(win,2101,RDS_FORMVAL_VALUE);
      setup_ds_edz=rdsGetObjectDouble(win,2102,RDS_FORMVAL_VALUE);
    #endif

    #if L57_MODELVERSION>=7
      setup_surf_everyrow=(rdsGetObjectInt(win,3000,RDS_FORMVAL_VALUE)!=0);
      setup_surf_delayval=rdsGetObjectDouble(win,3001,RDS_FORMVAL_VALUE);
      setup_surf_bydelay=(rdsGetObjectInt(win,3001,RDS_FORMVAL_CHECK)!=0);
    #endif

     // Взводим сигналы для пересчета всего
     xdiap_chg=ydiap_chg=zdiap_chg=1;
     Start=1;
 }

rdsDeleteObject(win);
Result=res?1:0;

#endif // #ifdef L57MS_SETUP
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// Описания в классе
//---------------------------------------------------------------------------
#ifdef L57MS_CLASS
TL57TextFuncArg Arguments;
BOOL InputConn;
TMaxList LocalMax; // Кандидаты в максимумы
TMaxList NextMax; // При обработке по шагам - обрабатываемые на следующем шаге
TMaxList FinalMax; // Готовые глобальные максимумы
TDoubleArray AddXArg,AddYArg,AddZArg; // Добавочные аргументы уточненных максимумов
int AddXBase,AddYBase,AddZBase; // Базовые индексы добавленных аргументов в матрицах
int RealFuncId; // Идентификатор функции (из настроек или с входа)

double Dx,Dy,Dz; // Реальные шаги исходной сетки
double _Xmin,_Xmax,_Ymin,_Ymax,_Zmin,_Zmax; // Реальные границы диапазонов
int xy_cnt; // x_cnt*y_cnt

#if L57_MODELVERSION>=5
  // Связи у выходов трехмерных функций
  BOOL F0xyConn,F0yzConn,F0xzConn;
  // Наличие каких-то данных в матрицах трехмерных функций
  BOOL F0xyData,F0yzData,F0xzData;

  // Отведение и заполнение массивов поверхностей
  BOOL SetupSurfaces(void);

  BOOL SurfacesArraySet;        // Массивы поверхностей отведены
  DWORD LastSurfaceOut_xy,      // Время последнего обновления выходов
        LastSurfaceOut_yz,
        LastSurfaceOut_xz;
  DWORD SurfaceOutDelayMs;      // Задержка в мс
#endif

#if L57_MODELVERSION>=2
  // Подстадия стадии 4
  int SubStage4;
  #define L57SS4_NONE     -1      // Нет
  #define L57SS4_XY       0       // уточнение (x,y)* - перебор z
  #define L57SS4_XZ       1       // уточнение (x,z)* - перебор y
  #define L57SS4_YZ       2       // уточнение (y,z)* - перебор x
  #define L57SS4_X        3       // уточнение x* - перебор (y,z)
  #define L57SS4_Y        4       // уточнение y* - перебор (x,z)
  #define L57SS4_Z        5       // уточнение z* - перебор (x,y)
  TMaxCandidate Stage4Max; // Уточняемый максимум на стадии 4
  void Stage4_StorePointToMatr(void); // Запись уточненной точки в матрицу из Stage4Max
  BOOL Stage4_NextPoint(void); // Взять в Stage4Max следующую точку
  void Stage4_StepDownAndInspect(void); // Уменьшить шаг в Stage4Max и вычислить точки
  // Признаки фиксации аргументов
  BOOL Stage4_FixX,Stage4_FixY,Stage4_FixZ;
#endif

// Прогресс на разных стадиях
double stage0_progress,stage0_progmax;
double stage1_progress,stage1_progmax;
double stage2_progress,stage2_progmax;
double stage3_progress,stage3_progmax;
#if L57_MODELVERSION>=2
  double stage4_progress,stage4_progmax;
#endif

#if L57_MODELVERSION>=9
  // Время
  DWORD dwStartTime;
  // Вычислить время
  void ShowTime(void)
    {
      if(Working!=0)
        Time=((double)(GetTickCount()-dwStartTime))/1000.0;
    };
#endif


// Инициализация переменных прогресса
inline void InitProgress(void)
{ stage0_progress=stage1_progress=stage2_progress=stage3_progress=0;
  stage0_progmax=stage1_progmax=stage2_progmax=stage3_progmax=1.0;

#if L57_MODELVERSION>=2
  stage4_progress=0.0;
  stage4_progmax=1.0;
#endif
}
// Проверка наличия связи у входа идентификатора функции
void CheckConnection(void)
{ InputConn=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_FUNCINPUT);
  #if L57_MODELVERSION>=5
    F0xyConn=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_3DFUNCOUT_XY);
    F0yzConn=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_3DFUNCOUT_YZ);
    F0xzConn=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_3DFUNCOUT_XZ);
  #endif
}
//-----------------------------------------------------------------------

// Сформировать выход прогресса
void ProgressOut(void);
//---------------------------------------------------------------------------

// Элементы выходных матриц X,Y,Z (аргументы)
inline double & XItem(int y_index,int z_index)
  { return X[y_index+1][z_index+1]; };
inline double & YItem(int x_index,int z_index)
  { return Y[x_index+1][z_index+1]; };
inline double & ZItem(int x_index,int y_index)
  { return Z[x_index+1][y_index+1]; };
// Элементы служебных матриц FX,FY,FZ (максимумы)
inline double & FxItem(int y_index,int z_index)
  { return Fx[y_index+1][z_index+1]; };
inline double & FyItem(int x_index,int z_index)
  { return Fy[x_index+1][z_index+1]; };
inline double & FzItem(int x_index,int y_index)
  { return Fz[x_index+1][y_index+1]; };
//---------------------------------------------------------------------------

// Значения аргументов по номеру
inline double &GetX(int ix)
  { return Y[ix+1][0]; }
inline double &GetY(int iy)
  { return X[iy+1][0]; }
inline double &GetZ(int iz)
  { return X[0][iz+1]; }
//---------------------------------------------------------------------------

// Значение функции во вспомогательном массиве по номеру
inline double &FuncVal(int ix,int iy,int iz)
{ //return *(FVal.GetBuffer()+iz*xy_cnt+iy*x_cnt+ix); 
  return FVal[iz*xy_cnt+iy*x_cnt+ix];
}
//---------------------------------------------------------------------------

// Стадия 1 - поиск кандидатов в максимумы
void DoStage1(BOOL init);
// Стадия 2 - уточнение максимумов
void DoStage2(BOOL init);
// Для стадии 2 - переместить максимум в готовые
void StoreFinalMax(TMaxCandidate *fnew);
// Записать число в готовые максимумы (при отключенном уточнении(
void StoreFinalMaxSimple(double x,double y,double z,double f);
// Стадия 3 - дорасчет матриц
void DoStage3(BOOL init);
// Для стадии 3 - вставка в матрицы новых строк и столбцов по FinalMax
void InsertNewArgs(void);
#if L57_MODELVERSION>=2
  // Стадия 4 - уточнение таблиц
  void DoStage4(BOOL init);
#endif
//---------------------------------------------------------------------------

// Очистка временных данных
void ClearTempData(void)
{ FVal.Resize(0);
  LocalMax.Clear();
  NextMax.Clear();
  FinalMax.Clear();
  AddXArg.Clear();
  AddYArg.Clear();
  AddZArg.Clear();
}
//---------------------------------------------------------------------------

// Настройка выходных значений после поиска
void SetupOutputs(BOOL _err=FALSE);
// Вывести список максимумов в выходную матрицу AllMax
void FillAllMax(TMaxList &list);
// Сортировать обычную матрицу (функции одной переменной)
BOOL SortLineMatrix(rdsbcppVarAncestor &M);
// Сортировать матрицу функции двух переменных
BOOL SortFieldMatrix(rdsbcppVarAncestor &M);
// Взять следующую точку
BOOL GetNextPoint(void);
// Обработать значения функции по xi,yi,zi
void ProcessValues_xiyizi(double *pf=NULL);

#endif // #ifdef L57MS_CLASS
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// Режим моделирования
//---------------------------------------------------------------------------
#ifdef L57MS_CALCMODE
CheckConnection();
#endif // #ifdef L57MS_CALCMODE
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// Описания после класса
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS
// Функция обратного вызова окна настройки
void RDSCALL WinCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data)
{ TL57TextFile *file=(TL57TextFile*)rdsGetObjectPtr(win,0,NULL);
  int deep=rdsGetObjectInt(win,1000,RDS_FORMVAL_VALUE);
  int steps=deep && rdsGetObjectInt(win,1003,RDS_FORMVAL_VALUE),
      prec=deep && rdsGetObjectInt(win,1001,RDS_FORMVAL_VALUE);
  if(file)
    file->Setup_WindowCallback(win,data);
  rdsSetObjectInt(win,1001,RDS_FORMVAL_ENABLED,deep);
  rdsSetObjectInt(win,1003,RDS_FORMVAL_ENABLED,deep);
  //rdsSetObjectInt(win,1008,RDS_FORMVAL_ENABLED,deep);
  rdsSetObjectInt(win,1004,RDS_FORMVAL_ENABLED,steps);
  rdsSetObjectInt(win,1005,RDS_FORMVAL_ENABLED,steps);
  rdsSetObjectInt(win,1006,RDS_FORMVAL_ENABLED,steps);
  rdsSetObjectInt(win,1007,RDS_FORMVAL_ENABLED,prec);
  rdsSetObjectInt(win,1002,RDS_FORMVAL_ENABLED,prec);
  #if L57_MODELVERSION>=1
    rdsSetObjectInt(win,1009,RDS_FORMVAL_ENABLED,deep);
  #endif

  /*
  rdsFORMAddEdit(win,8,2000,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицу XY: (x,y)*=arg max f(x,y,z) ",80);
  rdsFORMAddEdit(win,8,2001,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицу XZ: (x,z)*=arg max f(x,y,z) ",80);
  rdsFORMAddEdit(win,8,2002,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицу YZ: (y,z)*=arg max f(x,y,z) ",80);
  rdsFORMAddEdit(win,8,2003,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицы X и Fx: x*=arg max f(x,y,z) ",80);
  rdsFORMAddEdit(win,8,2004,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицы Y и Fy: y*=arg max f(x,y,z) ",80);
  rdsFORMAddEdit(win,8,2005,RDS_FORMCTRL_CHECKBOX,"Уточнять таблицы Z и Fz: z*=arg max f(x,y,z) ",80);
  */

#if L57_MODELVERSION>=2
  // Вкладка уточнения таблиц
  rdsSetObjectInt(win,2100,RDS_FORMVAL_ENABLED,         // шаг x
    rdsGetObjectInt(win,2000,RDS_FORMVAL_VALUE)||       // xy=F(z)
    rdsGetObjectInt(win,2001,RDS_FORMVAL_VALUE)||       // xz=F(y)
    rdsGetObjectInt(win,2003,RDS_FORMVAL_VALUE)         // x=F(y,z)
    );

  rdsSetObjectInt(win,2101,RDS_FORMVAL_ENABLED,         // шаг y
    rdsGetObjectInt(win,2000,RDS_FORMVAL_VALUE)||       // xy=F(z)
    rdsGetObjectInt(win,2002,RDS_FORMVAL_VALUE)||       // yz=F(x)
    rdsGetObjectInt(win,2004,RDS_FORMVAL_VALUE)         // y=F(x,z)
    );

  rdsSetObjectInt(win,2102,RDS_FORMVAL_ENABLED,         // шаг z
    rdsGetObjectInt(win,2001,RDS_FORMVAL_VALUE)||       // xz=F(y)
    rdsGetObjectInt(win,2002,RDS_FORMVAL_VALUE)||       // yz=F(x)
    rdsGetObjectInt(win,2005,RDS_FORMVAL_VALUE)         // z=F(x,y)
    );
#endif

}
//---------------------------------------------------

// Заполнить поля ввода для диапазона и значения переменной
void SetupVar(RDS_HOBJECT win,int tab,int basefield,char *var,
               double vmin,double vmax,double dv,
               BOOL in_vdiap,BOOL in_dv,BOOL head,BOOL line)
{ int id=basefield;
  TL57TextString str;

  if(head)
    { rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_MULTILABEL|RDS_FORMFLAG_LINE,
                     "Следующие параметры можно зафиксировать в "
                     "настройках блока, а не читать с его входов:",0);
      rdsSetObjectInt(win,id,RDS_FORMVAL_MLHEIGHT,34);
      rdsSetObjectInt(win,id,RDS_FORMVAL_MLABELBEVEL,0);
    }

  id++;
  str.Assign("Фиксировать диапазон ");
  str.AddString(var);
  str.AddString("min...");
  str.AddString(var);
  str.AddString("max:");
  rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_RANGEEDIT|RDS_FORMFLAG_CHECK,
                 str.c_str(),160);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_VALUE,vmin);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_RANGEMAX,vmax);
  rdsSetObjectInt(win,id,RDS_FORMVAL_CHECK,!in_vdiap);

  id++;
  str.Assign("Фиксировать шаг d");
  str.AddString(var);
  str.AddString(":");
  rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_EDIT|RDS_FORMFLAG_CHECK|(line?RDS_FORMFLAG_LINE:0),
                 str.c_str(),80);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_VALUE,dv);
  rdsSetObjectInt(win,id,RDS_FORMVAL_CHECK,!in_dv);
}
//---------------------------------------------------

// Считать поля ввода для диапазона и значения переменной
void ReadSetupVar(RDS_HOBJECT win,int basefield,
                  double &vmin,double &vmax,double &dv,
                  BOOL &in_vdiap,BOOL &in_dv)
{ int id=basefield+1;

  vmin=rdsGetObjectDouble(win,id,RDS_FORMVAL_VALUE);
  vmax=rdsGetObjectDouble(win,id,RDS_FORMVAL_RANGEMAX);
  in_vdiap=(rdsGetObjectInt(win,id,RDS_FORMVAL_CHECK)==0);

  id++;
  dv=rdsGetObjectDouble(win,id,RDS_FORMVAL_VALUE);
  in_dv=(rdsGetObjectInt(win,id,RDS_FORMVAL_CHECK)==0);
}
//---------------------------------------------------

#if L57_MODELVERSION>=5
// Отведение и заполнение массивов поверхностей
BOOL rdsbcppBlockClass::SetupSurfaces(void)
{ BOOL rok=TRUE;

  SurfacesArraySet=TRUE;

  if(F0xyConn)
    rok=rok && F0xy.Resize(x_cnt+1,y_cnt+1);
  else
    F0xy.Resize(0,0);
  if(F0xzConn && rok)
    rok=rok && F0xz.Resize(x_cnt+1,z_cnt+1);
  else
    F0xz.Resize(0,0);
  if(F0yzConn && rok)
    rok=rok && F0yz.Resize(y_cnt+1,z_cnt+1);
  else
    F0yz.Resize(0,0);

  if(!rok)
    return FALSE;

  for(int i=0;i<F0xy.Size();i++)
    *(F0xy.GetBuffer()+i)=rdsbcppHugeDouble;
  for(int i=0;i<F0xz.Size();i++)
    *(F0xz.GetBuffer()+i)=rdsbcppHugeDouble;
  for(int i=0;i<F0yz.Size();i++)
    *(F0yz.GetBuffer()+i)=rdsbcppHugeDouble;

  for(int xi=0;xi<x_cnt;xi++)
    { double v=vInDiap(_Xmin,_Xmax,Dx,xi,x_cnt); //(xi==(x_cnt-1)?_Xmax:(_Xmin+xi*Dx));
      if(F0xyConn)
        F0xy[xi+1][0]=v;
      if(F0xzConn)
        F0xz[xi+1][0]=v;
    }

  for(int yi=0;yi<y_cnt;yi++)
    { double v=vInDiap(_Ymin,_Ymax,Dy,yi,y_cnt); //(yi==(y_cnt-1)?_Ymax:(_Ymin+yi*Dy));
      if(F0xyConn)
        F0xy[0][yi+1]=v;
      if(F0yzConn)
        F0yz[yi+1][0]=v;
    }

  for(int zi=0;zi<z_cnt;zi++)
    { double v=vInDiap(_Zmin,_Zmax,Dz,zi,z_cnt); //(zi==(z_cnt-1)?_Zmax:(_Zmin+zi*Dz));
      if(F0xzConn)
        F0xz[0][zi+1]=v;
      if(F0yzConn)
        F0yz[0][zi+1]=v;
    }

  return TRUE;
}
//---------------------------------------------------
#endif

// Настройка выходных значений после поиска
void rdsbcppBlockClass::SetupOutputs(BOOL _err)
{ double delta;
  BOOL fchg=FALSE;
  Error=_err?1:0;
  Complete=OutReady=Finished=1;
  #if L57_MODELVERSION>=9
    ShowTime();
  #endif
  Working=0;
  Progress=1.0;
  stage=0;
  #if L57_MODELVERSION>=5
    if(!F0xyData)
      F0xy.Resize(0,0);
    if(!F0yzData)
      F0yz.Resize(0,0);
    if(!F0xzData)
      F0xz.Resize(0,0);
    F0xy_ready=1;
    F0xz_ready=1;
    F0yz_ready=1;
  #endif
  // Выводим матрицу финальных максимумов
  FillAllMax(FinalMax);
  ClearTempData();
  // Фильтрованные выходы
  delta=Dx*1.1;
  if(Xmax_f==rdsbcppHugeDouble || fabs(Xmax_f-Xmax)>delta)
    { Xmax_f=Xmax; fchg=TRUE; }
  delta=Dy*1.1;
  if(Ymax_f==rdsbcppHugeDouble || fabs(Ymax_f-Ymax)>delta)
    { Ymax_f=Ymax; fchg=TRUE; }
  delta=Dz*1.1;
  if(Zmax_f==rdsbcppHugeDouble || fabs(Zmax_f-Zmax)>delta)
    { Zmax_f=Zmax; fchg=TRUE; }
  if(fchg)
    Fmax_f=Fmax;
}
//---------------------------------------------------------------------------

// Вывести список максимумов в выходную матрицу AllMax
void rdsbcppBlockClass::FillAllMax(TMaxList &list)
{ int n=list.Count();
  if(!n)
    { AllMax.Resize(1,4);
      AllMax[0][0]=Xmax;
      AllMax[0][1]=Ymax;
      AllMax[0][2]=Zmax;
      AllMax[0][3]=Fmax;
      return;
    }
  if(!AllMax.Resize(n,4))
    { AllMax.Resize(0,0);
      rdsStopCalc();
      rdsMessageBox("Найдено слишком много максимумов - не хватает памяти для матрицы результатов",
        rdsbcppBlockData->BlockName,MB_ICONERROR|MB_OK);
      return;
    }
  n=0;
  for(TMaxCandidate *f=list.First;f!=NULL;f=f->Next,n++)
    { AllMax[n][0]=f->x;
      AllMax[n][1]=f->y;
      AllMax[n][2]=f->z;
      AllMax[n][3]=f->f;
      if(f->f>Fmax)
        { Fmax=f->f;
          Xmax=f->x;
          Ymax=f->y;
          Zmax=f->z;
        }
    }
}
//---------------------------------------------------------------------------

// Сформировать выход прогресса
void rdsbcppBlockClass::ProgressOut(void)
{ double p;
  int n;
  if(deep_search)
    { p=stage0_progress/stage0_progmax+ // Исходная сетка
        stage1_progress/stage1_progmax+ // Отбор кандидатов
        stage2_progress/stage2_progmax+ // Уточнение максимумов
        stage3_progress/stage3_progmax; // Дорасчет точек
      n=4;
    }
  else
   { p=stage0_progress/stage0_progmax;
     n=1;
   }
  #if L57_MODELVERSION>=2
    if(setup_ds_X || setup_ds_Y || setup_ds_Z ||
       setup_ds_XY || setup_ds_XZ || setup_ds_YZ)
      { p+=stage4_progress/stage4_progmax; // Уточнение таблиц
        n++;
      }
  #endif
  Progress=p/n;
}
//---------------------------------------------------------------------------


// Стадия 1 - поиск кандидатов в максимумы
void rdsbcppBlockClass::DoStage1(BOOL init)
{ double f;
  BOOL _finish=FALSE;
  if(init)
    { xi=yi=zi=0;
      return;
    }
  // Работаем

  // Проверяем точку xi,yi,zi на возможный максимум
  f=FuncVal(xi,yi,zi);
  if(f!=rdsbcppHugeDouble)
    { BOOL low=TRUE;
      // Осматриваем соседей
      for(int idx=-1;low && idx<=1;idx++)
        { int i_x=xi+idx;
          if(i_x<0 || i_x>=x_cnt)
            continue;
          for(int idy=-1;low && idy<=1;idy++)
            { int i_y=yi+idy;
              if(i_y<0 || i_y>=y_cnt)
                continue;
              for(int idz=-1;low && idz<=1;idz++)
                { double fn;
                  int i_z=zi+idz;
                  if(i_z<0 || i_z>=z_cnt)
                    continue;
                  if(idx==0 && idy==0 && idz==0)
                    continue;
                  fn=FuncVal(i_x,i_y,i_z);
                  if(fn==rdsbcppHugeDouble)
                    continue;
                  if(fn>f) // Не все соседи ниже
                    low=FALSE;
                } // for(int idz=-1;...)
            } // for(int idy=-1;...)
        } // for(int idx=-1;...)
      if(low)
        { // Все соседи точки xi,yi,zi оказались ниже
          TMaxCandidate *c=LocalMax.Add();
          c->x_index=xi;
          c->y_index=yi;
          c->z_index=zi;
          c->f=f;
          c->x=c->x_init=GetX(xi);
          c->y=c->y_init=GetY(yi);
          c->z=c->z_init=GetZ(zi);
          c->dx=(x_cnt==1?rdsbcppHugeDouble:Dx);
          c->dy=(y_cnt==1?rdsbcppHugeDouble:Dy);
          c->dz=(z_cnt==1?rdsbcppHugeDouble:Dz);
        }
    } // if(f!=rdsbcppHugeDouble)

  // Берем следующую точку
  zi+=1;
  stage1_progress+=1.0;
  if(zi>=z_cnt)
    { zi=0;
      yi+=1;
      if(yi>=y_cnt)
        { yi=0;
          xi+=1;
          if(xi>=x_cnt)
            _finish=TRUE;
        }
    }

  Ready=1;
  if(_finish) // Закончили
    { stage1_progress=stage1_progmax;
      ProgressOut();
      stage=2;
      Start=1;
      // FillAllMax(LocalMax);
      // SetupOutputs();
      DoStage2(TRUE);
      return;
    }
  // Еще не закончили
  Start=1;
  ProgressOut();
  Complete=OutReady=0;
}
//-----------------------------------------------------------------------

// Стадия 2 - уточнение максимумов
void rdsbcppBlockClass::DoStage2(BOOL init)
{ TMaxCandidate *f;
  BOOL cand_ready;
  int count=1000;

  if(init)
    { int total=LocalMax.Count();
      FVal.Resize(0);
      if(total==0) // Нет кандидатов в максимумы, что странно
        { stage2_progress=stage2_progmax=1.0;
          return;
        }
      // Максимум прогресса - сумма начальных шагов всех кандидатов
      stage2_progmax=LocalMax.SumDeltas();
      // Текщий прогресс будет равен stage2_progmax-(сумма шагов)
      return;
    }

  for(;;) // До обнуления count для ускорения расчета
  {

  // Работаем
  if(LocalMax.IsEmpty())
    { // Текущий шаг кончился - переходим к следующему
      if(NextMax.IsEmpty())
        { // Все посчитали - переходим к стадии 3
          stage2_progress=stage2_progmax=1.0;
          ProgressOut();
          stage=3;
          Start=Ready=1;
          DoStage3(TRUE);
          return;
        }
      LocalMax.Exchange(NextMax);
    }

  f=LocalMax.RemoveTop();
  // Обрабатываем кандидата f

  // Проверяем, не готов ли он уже
  cand_ready=TRUE;
  if(setup_end_steps) // Сначала нужно дойти до конечных шагов
    { if(f->dx!=rdsbcppHugeDouble && f->dx>setup_dx_min)
        cand_ready=FALSE;
      else if(f->dy!=rdsbcppHugeDouble && f->dy>setup_dy_min)
        cand_ready=FALSE;
      else if(f->dz!=rdsbcppHugeDouble && f->dz>setup_dz_min)
        cand_ready=FALSE;
    }
  if(cand_ready && search_by_df) // Нужно войти в трубку погрешности
    { if(f->cycles_in_df<setup_df_cycles) // Недавно в трубке
        cand_ready=FALSE;
    }

  if(cand_ready) // Максимум уточнен
    StoreFinalMax(f);
  else // Уточняем дальше
    { // Уменьшаем шаги в два раза
      int idx_0,idx_1,idy_0,idy_1,idz_0,idz_1;
      double dx_aux,dy_aux,dz_aux;
      BOOL changed=FALSE,equal;
      double _xnew,_ynew,_znew,_fnew=f->f;
      #if L57_MODELVERSION>=1
        equal=setup_no_plateau && (f->steps_down==0);
      #else
        equal=FALSE;
      #endif
      f->StepDown(f->dx,f->x_index,x_cnt,idx_0,idx_1,dx_aux,&equal);
      f->StepDown(f->dy,f->y_index,y_cnt,idy_0,idy_1,dy_aux,&equal);
      f->StepDown(f->dz,f->z_index,z_cnt,idz_0,idz_1,dz_aux,&equal);
      f->steps_down++;
      // Осматриваем окрестности (x,y,z)
      for(int idx=idx_0;idx<=idx_1;idx++)
        { double x=f->x+dx_aux*idx;
          for(int idy=idy_0;idy<=idy_1;idy++)
            { double y=f->y+dy_aux*idy;
              for(int idz=idz_0;idz<=idz_1;idz++)
                { double z=f->z+dz_aux*idz,fval;
                  Arguments.xval=x;
                  Arguments.yval=y;
                  Arguments.zval=z;
                  fval=SelectedFunc(RealFuncId,Arguments);
                  if(fval==rdsbcppHugeDouble)
                    { equal=FALSE;
                      continue;
                    }
                  if(equal && fabs(fval-f->f)>1e-38) // Не одинаковые
                    equal=FALSE;
                  if(fval>_fnew)
                    { _xnew=x; _ynew=y; _znew=z; _fnew=fval; changed=TRUE;
                       equal=FALSE;
                    }
                } // for(int idz=idz_0;...)
            } // for(int idy=idy_0;...)
        } // for(int idx=idx_0;...)
      #if L57_MODELVERSION>=1
        if(setup_no_plateau && equal) // Все соседи равны исходному - прекращаем и отбрасываем
          { delete f;
            break;
          }
      #endif
      if(changed) // Найден новый максимум
        { if(_fnew-f->f<setup_df) // В трубке
            f->cycles_in_df++;
          else
            f->cycles_in_df=0;
          f->x=_xnew;
          f->y=_ynew;
          f->z=_znew;
          f->f=_fnew;
        }
      else
        f->cycles_in_df++; // В трубке
      // Переносим на следующий шаг
      NextMax.AddExisting(f);
    } // else [ if(cand_ready) ]

    count--;
    if(count<=0)
      break;

  } // for(;;)

  // Перезапускаемся
  Start=Ready=1;
  stage2_progress=stage2_progmax-LocalMax.SumDeltas();
  ProgressOut();
  Complete=OutReady=0;
}
//-----------------------------------------------------------------------

// Записать число в готовые максимумы (при отключенном уточнении(
void rdsbcppBlockClass::StoreFinalMaxSimple(double x,double y,double z,double f)
{ // Есть ли вообще элементы
  if(!FinalMax.First)
    { FinalMax.AddNew(x,y,z,f);
      return;
    }
  // Сравниваем с первым элементом
  if(fabs(FinalMax.First->f-f)<=setup_df_equal) // Условно равны
    { FinalMax.AddNew(x,y,z,f);
      return;
    }
  if(f>FinalMax.First->f) // Новый больше старых
    { FinalMax.Clear();
      FinalMax.AddNew(x,y,z,f);
    }
  // Новый меньше старых - ничего не делаем
}
//-----------------------------------------------------------------------

// Для стадии 2 - переместить максимум в готовые
void rdsbcppBlockClass::StoreFinalMax(TMaxCandidate *fnew)
{ BOOL higher=FALSE,lower=FALSE;
  // Сравниваем с имеющимися
  for(TMaxCandidate *f=FinalMax.First;f!=NULL;f=f->Next)
    { if(fabs(fnew->f-f->f)<=setup_df_equal) // Условно равны
        continue;
      if(fnew->f>f->f) // Новый больше старых
        { higher=TRUE;
          break;
        }
      // Новый меньше старых
      lower=TRUE;
      break;
    }

  if(lower)
    { // Новый максимум меньше старых - не запоминаем
      delete fnew;
      return;
    }
  if(higher)
    { // Новый максимум больше старых - стираем старые
      FinalMax.Clear();
    }
  FinalMax.AddExisting(fnew);
}
//-----------------------------------------------------------------------

// Для стадии 3 - вставка в матрицы новых строк и столбцов по FinalMax
void rdsbcppBlockClass::InsertNewArgs(void)
{ int cnt=FinalMax.Count();
  TDoubleArray oldx,oldy,oldz;

  // Строим массивы исходных аргументов (для проверок)
  oldx.Allocate(x_cnt,TRUE);
  for(int i=0;i<x_cnt;i++)
    oldx.Dbl[i]=GetX(i);
  oldy.Allocate(y_cnt,TRUE);
  for(int i=0;i<y_cnt;i++)
    oldy.Dbl[i]=GetY(i);
  oldz.Allocate(z_cnt,TRUE);
  for(int i=0;i<z_cnt;i++)
    oldz.Dbl[i]=GetZ(i);

  // Строим массивы добавочных аргументов
  // В худшем случае каждый максимум порождает новое значение каждого аргумента
  AddXArg.Allocate(cnt);
  AddYArg.Allocate(cnt);
  AddZArg.Allocate(cnt);
  for(TMaxCandidate *f=FinalMax.First;f!=NULL;f=f->Next)
    { // x
      if(oldx.CanInsertValue(f->x))
        AddXArg.Dbl[AddXArg.Count++]=f->x;
      // y
      if(oldy.CanInsertValue(f->y))
        AddYArg.Dbl[AddYArg.Count++]=f->y;
      // z
      if(oldz.CanInsertValue(f->z))
        AddZArg.Dbl[AddZArg.Count++]=f->z;
    }

  // Переотведение матриц с сохранением содержимого
  AddXBase=x_cnt;
  AddYBase=y_cnt;
  AddZBase=z_cnt;
  x_cnt+=AddXArg.Count;
  y_cnt+=AddYArg.Count;
  z_cnt+=AddZArg.Count;
  X.Resize(y_cnt+1,z_cnt+1,TRUE);
  Y.Resize(x_cnt+1,z_cnt+1,TRUE);
  Z.Resize(x_cnt+1,y_cnt+1,TRUE);
  Fx.Resize(y_cnt+1,z_cnt+1,TRUE);
  Fy.Resize(x_cnt+1,z_cnt+1,TRUE);
  Fz.Resize(x_cnt+1,y_cnt+1,TRUE);
  XY.Resize(z_cnt,4,TRUE);
  XZ.Resize(y_cnt,4,TRUE);
  YZ.Resize(x_cnt,4,TRUE);
  // Записываем дополнительные аргументы в конец матриц
  for(int xi=0;xi<AddXArg.Count;xi++)
    { double v=AddXArg.Dbl[xi];
      Y[xi+1+AddXBase][0]=v;
      Fy[xi+1+AddXBase][0]=v;
      Z[xi+1+AddXBase][0]=v;
      Fz[xi+1+AddXBase][0]=v;
      YZ[xi+AddXBase][0]=v;
    }
  for(int yi=0;yi<AddYArg.Count;yi++)
    { double v=AddYArg.Dbl[yi];
      X[yi+1+AddYBase][0]=v;
      Fx[yi+1+AddYBase][0]=v;
      Z[0][yi+1+AddYBase]=v;
      Fz[0][yi+1+AddYBase]=v;
      XZ[yi+AddYBase][0]=v;
    }
  for(int zi=0;zi<AddZArg.Count;zi++)
    { double v=AddZArg.Dbl[zi];
      X[0][zi+1+AddZBase]=v;
      Fx[0][zi+1+AddZBase]=v;
      Y[0][zi+1+AddZBase]=v;
      Fy[0][zi+1+AddZBase]=v;
      XY[zi+AddZBase][0]=v;
    }
}
//-----------------------------------------------------------------------

// Сортировать обычную матрицу (функции одной переменной)
BOOL rdsbcppBlockClass::SortLineMatrix(rdsbcppVarAncestor &M)
{ RDS_ARRAYACCESSDATA Data;
  if(!M.GetAccessData(&Data))
    return FALSE;
  if((!Data.Exists) || Data.ItemSize!=sizeof(double) || rdsGetVarArrayElementType(M.GetVoidPtr())!=RDS_VARTYPE_DOUBLE)
    return FALSE;
  return SortMatrix(FALSE,Data.Rows,Data.Cols,(double*)(Data.Data),0);
}
//-----------------------------------------------------------------------

// Сортировать матрицу функции двух переменных
BOOL rdsbcppBlockClass::SortFieldMatrix(rdsbcppVarAncestor &M)
{ RDS_ARRAYACCESSDATA Data;
  if(!M.GetAccessData(&Data))
    return FALSE;
  if((!Data.Exists) || Data.ItemSize!=sizeof(double) || rdsGetVarArrayElementType(M.GetVoidPtr())!=RDS_VARTYPE_DOUBLE)
    return FALSE;
  return SortMatrixField(TRUE,Data.Rows,Data.Cols,(double*)(Data.Data),0,0) &&
         SortMatrixField(FALSE,Data.Rows,Data.Cols,(double*)(Data.Data),0,0);
}
//-----------------------------------------------------------------------

// Обработать значения функции по xi,yi,zi
void rdsbcppBlockClass::ProcessValues_xiyizi(double *pf)
{ double x,y,z,f,old_f;

#define XY_f(zi) (XY[zi][3])
#define XZ_f(yi) (XZ[yi][3])
#define YZ_f(xi) (YZ[xi][3])

  //inline double &GetX(int ix) {return Y[ix+1][0]; }
  //inline double &GetY(int iy) { return X[iy+1][0]; }
  //inline double &GetZ(int iz) { return X[0][iz+1]; }

  x=Y[xi+1][0];
  y=X[yi+1][0];
  z=X[0][zi+1];
  Arguments.xval=x;
  Arguments.yval=y;
  Arguments.zval=z;
  f=SelectedFunc(RealFuncId,Arguments);
  if(pf) *pf=f;
  if(f!=rdsbcppHugeDouble)
    { // Максимум f(*,y,z), т.е. X(y,z)
      old_f=FxItem(yi,zi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { FxItem(yi,zi)=f;
          XItem(yi,zi)=x;
        }
      // Максимум f(x,*,z), т.е. Y(x,z)
      old_f=FyItem(xi,zi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { FyItem(xi,zi)=f;
          YItem(xi,zi)=y;
        }
      // Максимум f(x,y,*), т.е. Z(x,y)
      old_f=FzItem(xi,yi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { FzItem(xi,yi)=f;
          ZItem(xi,yi)=z;
        }
      // Глобальный максимум
      if(absFmax==rdsbcppHugeDouble || absFmax<f)
        { absFmax=Fmax=f;
          Xmax=x;
          Ymax=y;
          Zmax=z;
        }
      // Максимум f(x,*,*), т.е. YZ(x)
      old_f=YZ_f(xi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { YZ_f(xi)=f;
          YZ[xi][1]=y;
          YZ[xi][2]=z;
        }
      // Максимум f(*,y,*), т.е. XZ(y)
      old_f=XZ_f(yi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { XZ_f(yi)=f;
          XZ[yi][1]=x;
          XZ[yi][2]=z;
        }
      // Максимум f(*,*,z), т.е. XY(z)
      old_f=XY_f(zi);
      if(old_f==rdsbcppHugeDouble || old_f<f)
        { XY_f(zi)=f;
          XY[zi][1]=x;
          XY[zi][2]=y;
        }
    } // if(f!=rdsbcppHugeDouble)

#undef XY_f
#undef XZ_f
#undef YZ_f
}
//-----------------------------------------------------------------------


// Взять следующую точку
BOOL rdsbcppBlockClass::GetNextPoint(void)
{ BOOL end=FALSE;
  zi+=1;
  if(zi>=z_cnt)
    { zi=0;
      yi+=1;
      if(yi>=y_cnt)
        { yi=0;
          xi+=1;
          if(xi>=x_cnt)
            end=TRUE;
        }
    }
  return end;
}
//-----------------------------------------------------------------------

// Стадия 3 - дорасчет матриц
void rdsbcppBlockClass::DoStage3(BOOL init)
{ BOOL _finished=FALSE;
  double x,y,z,f;
  if(init)
    { // Нужно вставить во все массивы строки и столбцы,
      // соответствующие аргументам FinalMax
      InsertNewArgs();
      // Максимум прогресса
      stage3_progmax=((double)x_cnt)*((double)y_cnt)*((double)z_cnt);
      stage3_progress=0;
      xi=yi=zi=0;
      Start=1;
      return;
    }

  // Вычисляем максимумы и их аргументы - считаем
  // точку xi,yi,zi
  while(!_finished)
    { if(xi<AddXBase && yi<AddYBase && zi<AddZBase)
        { // Эта точка - из уже посчитанных
          stage3_progress+=1.0;
          _finished=GetNextPoint();
        }
      else // Эту точку нужно считать
        break;
    }
  if(!_finished)
    { ProcessValues_xiyizi();
      stage3_progress+=1.0;
      _finished=GetNextPoint();
    }


  Ready=1;
  if(_finished)
    { // Сортировка матриц
      bool _err=FALSE;
      if(!SortLineMatrix(XY))
        _err=TRUE;
      if(!SortLineMatrix(XZ))
        _err=TRUE;
      if(!SortLineMatrix(YZ))
        _err=TRUE;
      if(!SortFieldMatrix(X))
        _err=TRUE;
      if(!SortFieldMatrix(Y))
        _err=TRUE;
      if(!SortFieldMatrix(Z))
        _err=TRUE;
      if(!SortFieldMatrix(Fx))
        _err=TRUE;
      if(!SortFieldMatrix(Fy))
        _err=TRUE;
      if(!SortFieldMatrix(Fz))
        _err=TRUE;

      #if L57_MODELVERSION>=2
        // Есть стадия уточнения таблиц
        if(setup_ds_X || setup_ds_Y || setup_ds_Z ||
           setup_ds_XY || setup_ds_XZ || setup_ds_YZ)
          { // Переходим к стадии 4 (уточнение выбранных таблиц)
            stage3_progress=stage3_progmax=1.0;
            ProgressOut();
            stage=4;
            Start=Ready=1;
            DoStage4(TRUE);
            return;
          }
        else
          SetupOutputs(_err);
      #else
        SetupOutputs(_err);
      #endif
    }
  else
    { Start=1;
      ProgressOut();
      Complete=OutReady=0;
    }
}
//-----------------------------------------------------------------------

#if L57_MODELVERSION>=2
// Стадия 4 - уточнение таблиц
void rdsbcppBlockClass::DoStage4(BOOL init)
{ int count=1000;
  BOOL cand_ready;

  if(init)
    {// Прогресс
      stage4_progmax=0;
      if(setup_ds_X)
        stage4_progmax+=y_cnt*z_cnt;
      if(setup_ds_Y)
        stage4_progmax+=x_cnt*z_cnt;
      if(setup_ds_Z)
        stage4_progmax+=x_cnt*y_cnt;
      if(setup_ds_XY)
        stage4_progmax+=z_cnt;
      if(setup_ds_XZ)
        stage4_progmax+=y_cnt;
      if(setup_ds_YZ)
        stage4_progmax+=x_cnt;
      stage4_progress=0;
      if(stage4_progmax<0.5)
        stage4_progmax=1.0;
      SubStage4=L57SS4_NONE;
      stage=4;
      Start=1;
      return;
    }
  // Работаем

  for(;;) // До обнуления count для ускорения расчета
    { BOOL nextpoint=TRUE;
      if(SubStage4!=L57SS4_NONE)
        { // Вычисляем
          nextpoint=FALSE;

          // Проверяем, не готов ли текущий максимум
          cand_ready=TRUE;
          if((!Stage4_FixX) && Stage4Max.dx!=rdsbcppHugeDouble && Stage4Max.dx>setup_ds_edx)
            cand_ready=FALSE;
          else if((!Stage4_FixY) && Stage4Max.dy!=rdsbcppHugeDouble && Stage4Max.dy>setup_ds_edy)
            cand_ready=FALSE;
          else if((!Stage4_FixZ) && Stage4Max.dz!=rdsbcppHugeDouble && Stage4Max.dz>setup_ds_edz)
            cand_ready=FALSE;
          if(cand_ready) // Максимум уточнен
            { // Записываем в матрицу
              Stage4_StorePointToMatr();
              nextpoint=TRUE;
            }
          else // Уточняем дальше
            Stage4_StepDownAndInspect();
        } // if(SubStage4!=L57SS4_NONE)

      if(nextpoint)
        { // Берем следующую точку
          if(Stage4_NextPoint())
            { // Перебор закончен
              Ready=1;
              SetupOutputs();
              return;
            }
          stage4_progress+=1.0;  
        }

      count--;
      if(count<=0)
        break;
    } // for(;;)

  // Перезапускаемся
  Start=Ready=1;
  ProgressOut();
  Complete=OutReady=0;

}
//-----------------------------------------------------------------------

// Уменьшить шаг в Stage4Max и вычислить точки
void rdsbcppBlockClass::Stage4_StepDownAndInspect(void)
{ int idx_0=0,idx_1=0,idy_0=0,idy_1=0,idz_0=0,idz_1=0;
  double dx_aux=0.0,dy_aux=0.0,dz_aux=0.0;
  double _xnew,_ynew,_znew,_fnew=Stage4Max.f;
  BOOL changed=FALSE;

  /*
  if(fabs(Stage4Max.x-0.2)<1e-12 || fabs(Stage4Max.x-0.3)<1e-12)
            changed=changed;
  */

  if(!Stage4_FixX)
    Stage4Max.StepDownStage4(Stage4Max.dx,x_cnt,idx_0,idx_1,dx_aux);
  if(!Stage4_FixY)
    Stage4Max.StepDownStage4(Stage4Max.dy,y_cnt,idy_0,idy_1,dy_aux);
  if(!Stage4_FixZ)
    Stage4Max.StepDownStage4(Stage4Max.dz,z_cnt,idz_0,idz_1,dz_aux);

  // Осматриваем окрестности (x,y,z)
  for(int idx=idx_0;idx<=idx_1;idx++)
    { double x=Stage4Max.x+dx_aux*idx;
      if(idx!=0 && (x<_Xmin || x>_Xmax))
        continue;
      for(int idy=idy_0;idy<=idy_1;idy++)
        { double y=Stage4Max.y+dy_aux*idy;
          if(idy!=0 && (y<_Ymin || y>_Ymax))
            continue;
          for(int idz=idz_0;idz<=idz_1;idz++)
            { double z=Stage4Max.z+dz_aux*idz,fval;
              if(idz!=0 && (z<_Zmin || z>_Zmax))
                continue;
              Arguments.xval=x;
              Arguments.yval=y;
              Arguments.zval=z;
              fval=SelectedFunc(RealFuncId,Arguments);
              if(fval==rdsbcppHugeDouble)
                continue;
              if(fval>_fnew)
                { _xnew=x; _ynew=y; _znew=z; _fnew=fval; changed=TRUE;
                }
            }
        }
    }
  if(changed) // Найден новый максимум
    { Stage4Max.x=_xnew;
      Stage4Max.y=_ynew;
      Stage4Max.z=_znew;
      Stage4Max.f=_fnew;
    }
}
//-----------------------------------------------------------------------


// Запись уточненной точки в матрицу из Stage4Max
void rdsbcppBlockClass::Stage4_StorePointToMatr(void)
{
  switch(SubStage4)
    { case L57SS4_XY: // уточнение (x,y)* - перебор z, рабочая матрица XY
        XY[zi][1]=Stage4Max.x;
        XY[zi][2]=Stage4Max.y;
        XY[zi][3]=Stage4Max.f;
        break;

      case L57SS4_XZ: // уточнение (x,z)* - перебор y, рабочая матрица XZ
        XZ[yi][1]=Stage4Max.x;
        XZ[yi][2]=Stage4Max.z;
        XZ[yi][3]=Stage4Max.f;
        break;

      case L57SS4_YZ: // уточнение (y,z)* - перебор x, рабочая матрица YZ
        YZ[xi][1]=Stage4Max.y;
        YZ[xi][2]=Stage4Max.z;
        YZ[xi][3]=Stage4Max.f;
        break;

      case L57SS4_X: // уточнение x* - перебор (y,z), рабочие матрицы X,Fx
        XItem(yi,zi)=Stage4Max.x;
        FxItem(yi,zi)=Stage4Max.f;
        break;

      case L57SS4_Y: // уточнение y* - перебор (x,z), рабочие матрицы Y,Fy
        YItem(xi,zi)=Stage4Max.y;
        FyItem(xi,zi)=Stage4Max.f;
        break;

      case L57SS4_Z: // уточнение z* - перебор (x,y), рабочие матрицы Z,Fz
        ZItem(xi,yi)=Stage4Max.z;
        FzItem(xi,yi)=Stage4Max.f;
        break;
    } // switch(SubStage4)
}
//-----------------------------------------------------------------------

// Взять в Stage4Max следующую точку
BOOL rdsbcppBlockClass::Stage4_NextPoint(void)
{ BOOL inc=TRUE;

  Stage4Max.dx=(x_cnt==1?rdsbcppHugeDouble:Dx);
  Stage4Max.dy=(y_cnt==1?rdsbcppHugeDouble:Dy);
  Stage4Max.dz=(z_cnt==1?rdsbcppHugeDouble:Dz);

  for(;;)
    { switch(SubStage4)
        { case L57SS4_NONE: // Нет
            break;

          case L57SS4_XY: // уточнение (x,y)* - перебор z, рабочая матрица XY
            if(!setup_ds_XY) // Подстадия запрещена
              break;
            if(inc)
              { zi++;
                if(zi>=z_cnt)
                  break;
              }
            // Остаемся в текущей стадии
            Stage4_FixX=FALSE;
            Stage4_FixY=FALSE;
            Stage4_FixZ=TRUE;
            Stage4Max.x=XY[zi][1];
            Stage4Max.y=XY[zi][2];
            Stage4Max.z=XY[zi][0];
            Stage4Max.f=XY[zi][3];
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          case L57SS4_XZ: // уточнение (x,z)* - перебор y, рабочая матрица XZ
            if(!setup_ds_XZ) // Подстадия запрещена
              break;
            if(inc)
              { yi++;
                if(yi>=y_cnt)
                  break;
              }
            // Остаемся в текущей стадии
            Stage4_FixX=FALSE;
            Stage4_FixY=TRUE;
            Stage4_FixZ=FALSE;
            Stage4Max.x=XZ[yi][1];
            Stage4Max.y=XZ[yi][0];
            Stage4Max.z=XZ[yi][2];
            Stage4Max.f=XZ[yi][3];
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          case L57SS4_YZ: // уточнение (y,z)* - перебор x, рабочая матрица YZ
            if(!setup_ds_YZ) // Подстадия запрещена
              break;
            if(inc)
              { xi++;
                if(xi>=x_cnt)
                  break;
              }
            // Остаемся в текущей стадии
            Stage4_FixX=TRUE;
            Stage4_FixY=FALSE;
            Stage4_FixZ=FALSE;
            Stage4Max.x=YZ[xi][0];
            Stage4Max.y=YZ[xi][1];
            Stage4Max.z=YZ[xi][2];
            Stage4Max.f=YZ[xi][3];
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          case L57SS4_X: // уточнение x* - перебор (y,z), рабочие матрицы X,Fx
            if(!setup_ds_X) // Подстадия запрещена
              break;
            if(inc)
              { yi++;
                if(yi>=y_cnt)
                  { yi=0;
                    zi++;
                    if(zi>=z_cnt)
                      break;
                  }
              }
            // Остаемся в текущей стадии
            Stage4_FixX=FALSE;
            Stage4_FixY=TRUE;
            Stage4_FixZ=TRUE;
            Stage4Max.x=XItem(yi,zi);
            Stage4Max.y=GetY(yi);
            Stage4Max.z=GetZ(zi);
            Stage4Max.f=FxItem(yi,zi);
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          case L57SS4_Y: // уточнение y* - перебор (x,z), рабочие матрицы Y,Fy
            if(!setup_ds_Y) // Подстадия запрещена
              break;
            if(inc)
              { xi++;
                if(xi>=x_cnt)
                  { xi=0;
                    zi++;
                    if(zi>=z_cnt)
                      break;
                  }
              }
            // Остаемся в текущей стадии
            Stage4_FixX=TRUE;
            Stage4_FixY=FALSE;
            Stage4_FixZ=TRUE;
            Stage4Max.x=GetX(xi);
            Stage4Max.y=YItem(xi,zi);
            Stage4Max.z=GetZ(zi);
            Stage4Max.f=FyItem(xi,zi);
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          case L57SS4_Z: // уточнение z* - перебор (x,y), рабочие матрицы Z,Fz
            if(!setup_ds_Z) // Подстадия запрещена
              break;
            if(inc)
              { xi++;
                if(xi>=x_cnt)
                  { xi=0;
                    yi++;
                    if(yi>=y_cnt)
                      break;
                  }
              }
            // Остаемся в текущей стадии
            Stage4_FixX=TRUE;
            Stage4_FixY=TRUE;
            Stage4_FixZ=FALSE;
            Stage4Max.x=GetX(xi);
            Stage4Max.y=GetY(yi);
            Stage4Max.z=ZItem(xi,yi);
            Stage4Max.f=FzItem(xi,yi);
            Stage4Max.x_index=xi;
            Stage4Max.y_index=yi;
            Stage4Max.z_index=zi;
            return FALSE;

          default:
            rdsStopCalc();
            rdsMessageBox("Внутренняя ошибка максимизатора - неизвестный режим уточнения таблиц",
              rdsbcppBlockData->BlockName,MB_ICONERROR|MB_OK);
            return TRUE;

        } // switch(SubStage4)
      // Сюда попадаем только если нужно сменить подстадию
      inc=FALSE;
      SubStage4++;
      xi=yi=zi=0;
      if(SubStage4>L57SS4_Z) // Все
        return TRUE;

      // Уходим на цикл, но уже с inc=FALSE
    } // for(;;)

}
//-----------------------------------------------------------------------



#endif // #if L57_MODELVERSION>=2


#endif // #ifdef L57MS_AFTERCLASS
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// Запуск расчета
//---------------------------------------------------------------------------
#ifdef L57MS_START
Start=1;
#endif // #ifdef L57MS_START
//-----------------------------------------------------------------------





//---------------------------------------------------------------------------
// Модель
//---------------------------------------------------------------------------
#ifdef L57MS_MODEL

BOOL changed=FALSE;
//double x,y,z,f,old_f,maxval;
double f;

int _count=1000;


// Исходно запрещаем работу всех выходов
Ready=0;

// Проверка разрешения работы
if(enabled_chg)
  { BOOL enabled=TRUE;
    enabled_chg=0;
    for(int i=0;enabled && i<Enabled.Size();i++)
      if(!Enabled[i])
        enabled=FALSE;
    _enabled=enabled?1:0;
    if(_enabled)
      { if(!Working)
          Complete=1;
        Ready=1;
      }
  }
if(!_enabled)
  { Progress=0.0;
    Ready=1;
    Complete=0;
    return;
  }

// Идентификатор функции
if(InputConn)
  RealFuncId=FuncId;
else
  RealFuncId=SetupFuncId;
if(RealFuncId!=OldFuncId)
  { OldFuncId=RealFuncId;
    changed=TRUE;
  }

if(func_chg) // Изменились функции
  { BOOL FuncsChanged=FALSE;
    func_chg=0;
    int n=F.Size();
    // Проверяем, какие именно
    if(n!=FS.Size())
      { FS.Resize(n,TRUE);
        FuncsChanged=TRUE;
      }
    for(int i=0;i<n;i++)
      { rdsbcstMDouble Fi=F[i],FSi=FS[i];
        if(TL57TextFuncFArray::IsBadMatrix(&Fi)) // Нет новых данных на F[i]
          continue;
        // Данные есть, но новые ли?
        if(TL57TextFuncFArray::CompareMatr(&Fi,&FSi))
          { // Такие уже есть
            Fi.Resize(2,2);
            TL57TextFuncFArray::SetBadMatrix(&Fi);
            continue;
          }
        // Новые данные
        FuncsChanged=TRUE;
        rdsExchangeVarArrays(Fi.GetPtr(),FSi.GetPtr());
        Fi.Resize(2,2);
        TL57TextFuncFArray::SetBadMatrix(&Fi);
      }
    if(FuncsChanged)
      { changed=TRUE;
        Arguments.Functions.Realloc(n);
        for(int i=0;i<n;i++)
          Arguments.Functions[i].Setup(&(FS[i]));
      }
  }

if(ArrayChanged)
  { // Проверяем реальное изменение массива
    if(A.Size()!=A_old.Size()) // Изменился размер
      changed=TRUE;
    else // Размер тот же
      for(int i=0;i<A.Size();i++)
        if(A[i]!=A_old[i])
          { changed=TRUE;
            break;
          } 
    if(changed)
      A_old=A;
  }

// Параметры
if(A.IsEmpty())
  Arguments.Params.Setup(NULL);
else
  Arguments.Params.Setup(A.GetBuffer(),A.Size());

// Диапазоны
if(in_xdiap) { _Xmin=xmin; _Xmax=xmax; }
else { _Xmin=setup_xmin; _Xmax=setup_xmax;}
Dx=in_dx?dx:setup_dx;
if(in_ydiap) { _Ymin=ymin; _Ymax=ymax; }
else { _Ymin=setup_ymin; _Ymax=setup_ymax;}
Dy=in_dy?dy:setup_dy;
if(in_zdiap) { _Zmin=zmin; _Zmax=zmax; }
else { _Zmin=setup_zmin; _Zmax=setup_zmax;}
Dz=in_dz?dz:setup_dz;

if(!changed)
  { // Не изменились ли диапазоны
    if(xdiap_chg && (_Xmin!=old_xmin || _Xmax!=old_xmax || Dx!=old_dx))
      changed=TRUE;
    else if(ydiap_chg && (_Ymin!=old_ymin || _Ymax!=old_ymax || Dy!=old_dy))
      changed=TRUE;
    else if(zdiap_chg && (_Zmin!=old_zmin || _Zmax!=old_zmax || Dz!=old_dz))
      changed=TRUE;
  }


if(changed)
  { // Что-то изменилось
    BOOL rok;
    old_xmin=_Xmin; old_xmax=_Xmax; old_dx=Dx;
    old_ymin=_Ymin; old_ymax=_Ymax; old_dy=Dy;
    old_zmin=_Zmin; old_zmax=_Zmax; old_dz=Dz;
    // Сбрасываем сигналы срабатывания связей
    xdiap_chg=ydiap_chg=zdiap_chg=0;
    ArrayChanged=0;
    stage=0; // Начинаем поиск с начала

    // Размерности матриц
    x_cnt=CalcNPointsByDiap(_Xmin,_Xmax,Dx);
    y_cnt=CalcNPointsByDiap(_Ymin,_Ymax,Dy);
    z_cnt=CalcNPointsByDiap(_Zmin,_Zmax,Dz);
    if(x_cnt<1 || y_cnt<1 || z_cnt<1)
      { // Ошибка
        X.Resize(0,0);
        Y.Resize(0,0);
        Z.Resize(0,0);
        XY.Resize(0,0);
        XZ.Resize(0,0);
        YZ.Resize(0,0);
        Fx.Resize(0,0);
        Fy.Resize(0,0);
        Fz.Resize(0,0);
        ClearTempData();
        Xmax=Ymax=Zmax=Fmax=
        Xmax_f=Ymax_f=Zmax_f=Fmax_f=rdsbcppHugeDouble;
        Complete=OutReady=Ready=Finished=1;
        AllMax.Resize(0,0);
        return;
      }
    if(x_cnt==1)
      { _Xmax=_Xmin; Dx=1.0; }
    if(y_cnt==1)
      { _Ymax=_Ymin; Dy=1.0; }
    if(z_cnt==1)
      { _Zmax=_Zmin; Dz=1.0; }

    #if L57_MODELVERSION>=9
      // Время и сигнал начала
      Started=1;
      dwStartTime=GetTickCount();
      Time=0.0;
    #endif

    // Размерности подходят - отводим
    rok=X.Resize(y_cnt+1,z_cnt+1);
    rok=rok && Y.Resize(x_cnt+1,z_cnt+1);
    rok=rok && Z.Resize(x_cnt+1,y_cnt+1);
    rok=rok && Fx.Resize(y_cnt+1,z_cnt+1);
    rok=rok && Fy.Resize(x_cnt+1,z_cnt+1);
    rok=rok && Fz.Resize(x_cnt+1,y_cnt+1);
    rok=rok && XY.Resize(z_cnt,4);
    rok=rok && XZ.Resize(y_cnt,4);
    rok=rok && YZ.Resize(x_cnt,4);
    #if L57_MODELVERSION>=5
      /*
      if(F0xyConn)
        rok=rok && F0xy.Resize(x_cnt+1,y_cnt+1);
      else
        F0xy.Resize(0,0);
      if(F0xzConn)
        rok=rok && F0xz.Resize(x_cnt+1,z_cnt+1);
      else
        F0xz.Resize(0,0);
      if(F0yzConn)
        rok=rok && F0yz.Resize(y_cnt+1,z_cnt+1);
      else
        F0yz.Resize(0,0);
      */
      F0xy.Resize(0,0);
      F0xz.Resize(0,0);
      F0yz.Resize(0,0);
      SurfacesArraySet=FALSE;
    #endif
    if(deep_search)
      { xy_cnt=x_cnt*y_cnt;
        rok=rok && FVal.Resize(xy_cnt*z_cnt,FALSE);
      }
    else
      { xy_cnt=0;
        FVal.Resize(0,FALSE);
      }
    if(!rok)
      { rdsStopCalc();
        X.Resize(0,0);
        Y.Resize(0,0);
        Z.Resize(0,0);
        XY.Resize(0,0);
        XZ.Resize(0,0);
        YZ.Resize(0,0);
        Fx.Resize(0,0);
        Fy.Resize(0,0);
        Fz.Resize(0,0);
        #if L57_MODELVERSION>=5
          F0xy.Resize(0,0);
          F0xz.Resize(0,0);
          F0yz.Resize(0,0);
        #endif
        ClearTempData();
        rdsMessageBox("Выбраны слишком маленькие шаги по одной или нескольким переменным, не хватает памяти для "
                      "размещения матриц табличных функций такого размера.",
                      rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
        return;
      }

    // Заполняем основные матрицы значением ошибки
    for(int i=0;i<X.Size();i++)
      *(X.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<Y.Size();i++)
      *(Y.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<Z.Size();i++)
      *(Z.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<XY.Size();i++)
      *(XY.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<XZ.Size();i++)
      *(XZ.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<YZ.Size();i++)
      *(YZ.GetBuffer()+i)=rdsbcppHugeDouble;
    Xmax=Ymax=Zmax=rdsbcppHugeDouble;
    absFmax=Fmax=rdsbcppHugeDouble;

    // Заполняем служебные матрицы значением ошибки
    // (там будут считаться максимумы функции)
    for(int i=0;i<Fx.Size();i++)
      *(Fx.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<Fy.Size();i++)
      *(Fy.GetBuffer()+i)=rdsbcppHugeDouble;
    for(int i=0;i<Fz.Size();i++)
      *(Fz.GetBuffer()+i)=rdsbcppHugeDouble;

    // Заполняем служебную строку и столбец каждой матрицы значениями
    // аргументов семейства функций
    X[0][0]=rdsbcppHugeDouble;
    Y[0][0]=rdsbcppHugeDouble;
    Z[0][0]=rdsbcppHugeDouble;
    Fx[0][0]=rdsbcppHugeDouble;
    Fy[0][0]=rdsbcppHugeDouble;
    Fz[0][0]=rdsbcppHugeDouble;
    /*
    #if L57_MODELVERSION>=5
      for(int i=0;i<F0xy.Size();i++)
        *(F0xy.GetBuffer()+i)=rdsbcppHugeDouble;
      for(int i=0;i<F0xz.Size();i++)
        *(F0xz.GetBuffer()+i)=rdsbcppHugeDouble;
      for(int i=0;i<F0yz.Size();i++)
        *(F0yz.GetBuffer()+i)=rdsbcppHugeDouble;
    #endif
    */
    for(int xi=0;xi<x_cnt;xi++)
      { double v=vInDiap(_Xmin,_Xmax,Dx,xi,x_cnt); //(xi==(x_cnt-1)?_Xmax:(_Xmin+xi*Dx));
        Y[xi+1][0]=v;
        Fy[xi+1][0]=v;
        Z[xi+1][0]=v;
        Fz[xi+1][0]=v;
        YZ[xi][0]=v;
        /*
        #if L57_MODELVERSION>=5
          if(F0xyConn)
            F0xy[xi+1][0]=v;
          if(F0xzConn)
            F0xz[xi+1][0]=v;
        #endif
        */
      }
    for(int yi=0;yi<y_cnt;yi++)
      { double v=vInDiap(_Ymin,_Ymax,Dy,yi,y_cnt); //(yi==(y_cnt-1)?_Ymax:(_Ymin+yi*Dy));
        X[yi+1][0]=v;
        Fx[yi+1][0]=v;
        Z[0][yi+1]=v;
        Fz[0][yi+1]=v;
        XZ[yi][0]=v;
        /*
        #if L57_MODELVERSION>=5
          if(F0xyConn)
            F0xy[0][yi+1]=v;
          if(F0yzConn)
            F0yz[yi+1][0]=v;
        #endif
        */
      }
    for(int zi=0;zi<z_cnt;zi++)
      { double v=vInDiap(_Zmin,_Zmax,Dz,zi,z_cnt); //(zi==(z_cnt-1)?_Zmax:(_Zmin+zi*Dz));
        X[0][zi+1]=v;
        Fx[0][zi+1]=v;
        Y[0][zi+1]=v;
        Fy[0][zi+1]=v;
        XY[zi][0]=v;
        /*
        #if L57_MODELVERSION>=5
          if(F0xzConn)
            F0xz[0][zi+1]=v;
          if(F0yzConn)
            F0yz[0][zi+1]=v;
        #endif
        */
      }
    xi=yi=zi=0;
    InitProgress();
    Complete=OutReady=0;
    Working=1; // Идет расчет
    #if L57_MODELVERSION>=5
      F0xy_ready=1; // Чтобы пустые матрицы передались на выход
      F0xz_ready=1;
      F0yz_ready=1;
      LastSurfaceOut_xy=GetTickCount();
      LastSurfaceOut_xz=LastSurfaceOut_yz=LastSurfaceOut_xy;
      #if L57_MODELVERSION>=7
        SurfaceOutDelayMs=(DWORD)(setup_surf_delayval*1000);
      #endif
      F0xyData=F0yzData=F0xzData=FALSE;
    #endif
    // Отведение вспомогательных массивов и вычисления
    LocalMax.Clear();
    NextMax.Clear();
    FinalMax.Clear();
    //TMaxCandidate::CheckMsg();
    AllMax.Resize(0,0);
    // Максимум прогресса нулевой и первой (если есть) сдедий одинаков? число точек
    stage0_progmax=stage1_progmax=((double)x_cnt)*((double)y_cnt)*((double)z_cnt);

    Ready=1;
    Start=1;
    ProgressOut();
    Complete=OutReady=0;
    return;
  } // if(changed)
else
  { // Ничего не изменилось
    if(!Working) // А мы и не работали
      { Finished=Ready=1;
        return;
      }
  }

if(stage)
  { // Какой-то шаг уточнения максимумов
    switch(stage)
      { case 1: // Поиск кандидатов
          DoStage1(FALSE);
          break;
        case 2: // Уточнение максимумов
          DoStage2(FALSE);
          break;
        case 3: // Дорасчет матриц
          DoStage3(FALSE);
          break;
        #if L57_MODELVERSION>=2
          case 4: // Уточнение таблиц
            DoStage4(FALSE);
            break;
        #endif
        default: // Ошибка
          rdsStopCalc();
          rdsMessageBox("Внутренняя ошибка максимизатора - неизвестный режим работы",
                        rdsbcppBlockData->BlockName,MB_ICONERROR|MB_OK);
      }
    #if L57_MODELVERSION>=9
      ShowTime();
    #endif
    return;
  }


// Первый этап

for(;;)
{

// Вычисляем максимумы и их аргументы - считаем
// точку xi,yi,zi
ProcessValues_xiyizi(&f);
if(deep_search)
  { // Запись во временный массив значений
    if(FuncVal(xi,yi,zi)!=rdsbcppHugeDouble)
      { rdsStopCalc();
        rdsMessageBox("Внутренняя ошибка максимизатора - повторная запись данных",
          rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
        return;
      }
    FuncVal(xi,yi,zi)=f;
  }
else if(f!=rdsbcppHugeDouble)
  { // Поддержка многих максимумов
    double x=Y[xi+1][0],
           y=X[yi+1][0],
           z=X[0][zi+1];
    StoreFinalMaxSimple(x,y,z,f);
  }
#if L57_MODELVERSION>=5
  if(!SurfacesArraySet)
    { if(!SetupSurfaces())
        { rdsStopCalc();
          rdsMessageBox("Внутренняя ошибка максимизатора - не удалось отвести массивы поверхностей",
            rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
          return;
        }
    }

  // Добавляем точку в матрицы для графиков
  if(f!=rdsbcppHugeDouble)
    { if(F0xyConn && zi==0)
        { F0xyData=TRUE;
          F0xy[xi+1][yi+1]=f;
        }
      if(F0xzConn && yi==0)
        { F0xzData=TRUE;
          F0xz[xi+1][zi+1]=f;
        }
      if(F0yzConn && xi==0)
        { F0yzData=TRUE;
          F0yz[yi+1][zi+1]=f;
        }
    }

#endif

// Берем следующую точку
stage0_progress+=1.0;
if(GetNextPoint())
  Working=0;

Ready=1;
if(Working) // Считаем дальше
  {
    #if L57_MODELVERSION>=7
      // Не пора ли обновить поверхности
      DWORD now=GetTickCount();
      // Изменение: z -> y -> x
      if(F0xyConn)
        { // По (x,y), z в начале диапазона
          if((setup_surf_everyrow && yi==0 && zi==0) ||
             (setup_surf_bydelay && (now-LastSurfaceOut_xy)>SurfaceOutDelayMs) )
            { LastSurfaceOut_xy=now;
              F0xy_ready=1;
              _count=0; // Для выхода из внутреннего цикла
            }
        }
      if(F0yzConn)
        { // По (y,z), x в начале диапазона
          if((setup_surf_everyrow && zi==0 && xi==0) ||
             (setup_surf_bydelay && (now-LastSurfaceOut_yz)>SurfaceOutDelayMs) )
            { LastSurfaceOut_yz=now;
              F0yz_ready=1;
              _count=0; // Для выхода из внутреннего цикла
            }
        }
      if(F0xzConn)
        { // По (x,z), y в начале диапазона
          if((setup_surf_everyrow && yi==0 && zi==0) ||
             (setup_surf_bydelay && (now-LastSurfaceOut_xz)>SurfaceOutDelayMs) )
            { LastSurfaceOut_xz=now;
              F0xz_ready=1;
              _count=0; // Для выхода из внутреннего цикла
            }
        }
    #endif
    Start=1;
    ProgressOut();
    Complete=OutReady=0;
  }
else // Расчет окончен
  { _count=0; // Для выхода из внутреннего цикла
    #if L57_MODELVERSION>=7
      F0xy_ready=1; // Чтобы матрицы поверхностей передались на выход - они заполнены
      F0xz_ready=1;
      F0yz_ready=1;
    #endif
    if(deep_search) // Закончилась только нулевая стадия поиска
      { Complete=OutReady=0;
        Working=1;
        Start=1;
        stage=1;
        DoStage1(TRUE); // Инициализация
        return;
      }
    // Дальше считать не надо
    SetupOutputs();
  }

  _count--;
  if(_count<=0)
    break;

} // for(;;)

#if L57_MODELVERSION>=9
  ShowTime();
#endif

#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------
