//---------------------------------------------------------------------------
// Линейная интерполяция по матрицам
//---------------------------------------------------------------------------
#ifndef RdsLinInterH
#define RdsLinInterH
// Требует CommonAC.hpp
#ifndef CommonACH
  #include <CommonAC.hpp>
#endif
//---------------------------------------------------------------------------
// Класс линейной интерполяции функции или семейства
class TRdsLinearInterpolation
{ protected:
    // Разобранные данные о матрице
    // Тип (функция или семейство)
    int Type;
    // Типы
    #define RDSLININTER_T_UNKNOWN       0       // Неизвестно (определить самостоятельно)
    #define RDSLININTER_T_SINGLE        1       // Функция
    #define RDSLININTER_T_FIELD         2       // Семейство
    #define RDSLININTER_T_ERROR         3       // Ошибка
    BOOL ByRows;                // TRUE - линия-строка, FALSE - линия-столбец
                                // Для семейств: TRUE - первый параметр в нулевом столбце, FALSE - в нулевой строке
    BOOL InputMatrixSorted;     // Входная матрица отсортирована
    RDS_ARRAYACCESSDATA Data;   // Данные доступа
    // Дополнительные флаги (используются в функции Setup)
    #define RDSLININTER_F_BYROWS        0x10     // См. ByRows
    #define RDSLININTER_F_NOTSORTED     0x20     // Обратный к InputMatrixSorted

    // Данные прошлого моделирования
    // Аргумент
    int LastSearchResult;       // Результат прошлого поиска
    int lastXIndex,lastYIndex;  // Прошлые индексы
    double LastXL,LastXR;       // Найденные данные

    // Параметр
    int LastKSearchResult;      // Результат прошлого поиска
    double LastKL,LastKR;       // Найденные значения параметра

    // Результат поиска
   #define RDSLININTER_NONE     0       // Нет данных
   #define RDSLININTER_BELOW    1       // x левее массива
   #define RDSLININTER_ABOVE    2       // x правее массива
   #define RDSLININTER_BETWEEN  3       // x внутри массива

    // Для простых функций
    double LastK,LastB;         // Найденные параметры прямой

    // Для семейств
    int LeftXIndex,RightXIndex,LeftKIndex,RightKIndex;


    // Сортировать матрицу
    BOOL SortMatrix(BOOL databyrows,int Rows,int Cols,double *MData,int XIndex);
    // Сортировать матрицу для семейства (с пропуском одного индекса)
    BOOL SortMatrixField(BOOL databyrows,int Rows,int Cols,
                         double *MData,int XIndex,int excludeindex);
    // Вычисление
    BOOL ModelSingle(double ext_x,double *py,int XIndex,int YIndex);
    BOOL ModelField(double ext_x1,int index_x1,double ext_x2,int index_x2,double *py);

    // Сброс
    inline void Reset(void)
      { InputMatrixSorted=FALSE; LastSearchResult=LastKSearchResult=RDSLININTER_NONE; lastXIndex=lastYIndex=-1;
        Type=RDSLININTER_T_UNKNOWN; };
    // Поиск ограничивающих индексов
    // array        - массив
    // count        - число точек
    // itemshift    - расстояние между соседними точками в массиве
    // pLeft,pRight - возвращаемые индексы ограничивающих точек
    // Возврат      - RDSLININTER_*
    static int FindNearestPoints(double *array,int count,int itemshift,double x,int *pLeft,int *pRight);
    // Поиск ограничивающих индексов в массиве с выкинутой точкой
    // array        - массив
    // count        - число точек
    // itemshift    - расстояние между соседними точками в массиве
    // exindex      - выкидываемфй индекс (должен обязательно находится внутри массива, -1 не допускается)
    // pLeft,pRight - возвращаемые индексы ограничивающих точек
    // Возврат      - RDSLININTER_*
    static int FindNearestPointsEx(double *array,int count,int itemshift,int exindex,double x,int *pLeft,int *pRight);
  public:
    // Настроить на матрицу (вызывается при изменении размера матрицы или данных в ней)
    void Setup(rdsbcppVarAncestor &M,int flags=RDSLININTER_T_UNKNOWN);

    // Вычислить значение функции одной переменной
    //   XIndex - индекс строки (столбца) аргумента
    //   YIndex - индекс строки (столбца) значения
    //   x      - аргумент
    double Calc_Fx(double x,int XIndex=0,int YIndex=1);
    // Вычислить значение функции двух переменных
    //   x1      - аргумент 1
    //   x2      - аргумент 2
    double Calc_Fx1x2(double x1,double x2);
    // Вычислить значение функции двух переменных
    //   x1      - аргумент 1
    //   i2      - индекс в матрице аргумента 2
    double Calc_Fx1i2(double x1,int i2);
    // Вычислить значение функции двух переменных
    //   i1      - индекс в матрице аргумента 1
    //   x2      - аргумент 2
    double Calc_Fi1x2(int i1,double x2);

    // Вычислить значение функции одной переменной (оператором скобок)
    inline double operator()(double x){ return Calc_Fx(x); };
    // Вычислить значение функции двух переменных  оператором скобок)
    inline double operator()(double x1,double x2){ return Calc_Fx1x2(x1,x2); };

    TRdsLinearInterpolation(void)
      { Reset(); };
};
//---------------------------------------------------------------------------

// Настроить на матрицу
void TRdsLinearInterpolation::Setup(rdsbcppVarAncestor &M,int flags)
{ double *arr;
  int type=flags & 0xf;

  Reset();
  ByRows=((flags & RDSLININTER_F_BYROWS)!=0); //byrows;
  InputMatrixSorted=((flags & RDSLININTER_F_NOTSORTED)==0); //sorted;

  if(!M.GetAccessData(&Data))
    { Type=RDSLININTER_T_ERROR;
      return;
    }
  if((!Data.Exists) || Data.ItemSize!=sizeof(double))
    { Type=RDSLININTER_T_ERROR;
      return;                     
    }
  arr=(double*)(Data.Data);

  if(type==RDSLININTER_T_UNKNOWN)
    { if((*arr)==rdsbcppHugeDouble)
        Type=RDSLININTER_T_FIELD;
      else
        Type=RDSLININTER_T_SINGLE;
    }
  else
    Type=type;
}
//---------------------------------------------------------------------------

// Сортировать матрицу для функции одной переменной
BOOL TRdsLinearInterpolation::SortMatrix(BOOL databyrows,int Rows,int Cols,double *MData,int XIndex)
{

  InputMatrixSorted=TRUE;

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
//---------------------------------------------------------------------------

// Поиск ограничивающих индексов
// array        - массив
// count        - число точек
// itemshift    - расстояние между соседними точками в массиве
// pLeft,pRight - возвращаемые индексы ограничивающих точек
// Возврат      - RDSLININTER_*
int TRdsLinearInterpolation::FindNearestPoints(double *array,int count,int itemshift,double x,int *pLeft,int *pRight)
{ int left=0,right=count-1,mid,diap;
  double xleft,xright,xmid;

  if(count==1)
    { *pLeft=0;
      *pRight=0;
      return RDSLININTER_NONE;
    }

  xleft=array[0];
  xright=array[(count-1)*itemshift];
  if(xleft==rdsbcppHugeDouble || xright==rdsbcppHugeDouble)
    { *pLeft=0;
      *pRight=0;
      return RDSLININTER_NONE;
    }
  if(x<xleft)
    { *pLeft=0;
      *pRight=1;
      return RDSLININTER_BELOW;
    }
  if(x>xright)
    { *pLeft=count-2;
      *pRight=count-1;
      return RDSLININTER_ABOVE;
    }

  for(;;)
    switch(diap=right-left)
      { case 0:
        case 1:
          *pLeft=left;
          *pRight=right;
          return diap==0?RDSLININTER_NONE:RDSLININTER_BETWEEN;

        default: // Одна и более точек между left и right
          mid=left+diap/2;
          xmid=array[mid*itemshift];
          if(xmid==rdsbcppHugeDouble)
            { *pLeft=0;
              *pRight=0;
              return RDSLININTER_NONE;
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


// Моделирование (функция)
// ext_x   - вход аргумента
// py      - адрес значения
// XIndex  - индекс строки (столбца) аргумента
// YIndex  - индекс строки (столбца) значения
// Возврат - успешность (есть значение)
BOOL TRdsLinearInterpolation::ModelSingle(double ext_x,double *py,int XIndex,int YIndex)
{ int Rows,Cols,leftindex,rightindex,itemshift;
  double *MData,x,y,*xarr,*yarr;
  BOOL ok;

  x=ext_x;

  if(!Data.Exists) // Нет матрицы
    return FALSE;

  if(x==rdsbcppHugeDouble)
    { *py=rdsbcppHugeDouble;
      return TRUE;
    }

  Rows=Data.Rows;
  Cols=Data.Cols;
  if(ByRows)
    { if(XIndex>=Rows || YIndex>=Rows || XIndex<0 || YIndex<0)
        return FALSE;
    }
  else
    { if(XIndex>=Cols || YIndex>=Cols || XIndex<0 || YIndex<0)
        return FALSE;
    }
  MData=(double*)(Data.Data);

  if(!InputMatrixSorted)
    { if(!SortMatrix(ByRows,Rows,Cols,MData,XIndex))
       { *py=rdsbcppHugeDouble;
         LastSearchResult=RDSLININTER_NONE;
         InputMatrixSorted=FALSE;
         return FALSE;
       }
      LastSearchResult=RDSLININTER_NONE; // Данные в матрице изменились
    }

  // Матрица отсортирована и в ней есть строки (столбцы) XIndex и YIndex

  // Проверка результатов прошлого моделирования. Если x остался в том же диапазоне,
  // новый поиск производить не нужно.
  switch(LastSearchResult)
    { case RDSLININTER_BELOW:
        ok=(lastXIndex==XIndex && lastYIndex==YIndex && x<LastXL);
        break;
      case RDSLININTER_ABOVE:
        ok=(lastXIndex==XIndex && lastYIndex==YIndex && x>LastXR);
        break;
      case RDSLININTER_BETWEEN:
        ok=(lastXIndex==XIndex && lastYIndex==YIndex && x>LastXL && x<LastXR);
        break;
      default:
        ok=FALSE;
    }

  if(ok) // Новый поиск не нужен
    { *py=LastK*x+LastB;
      return TRUE;
    }
  lastXIndex=XIndex;
  lastYIndex=YIndex;

  // Нужно найти индексы точек строки (столбца) XIndex, между которыми расположен x
  if(ByRows)
    { LastSearchResult=FindNearestPoints(xarr=MData+XIndex*Cols,Cols,itemshift=1,x,&leftindex,&rightindex);
      yarr=MData+YIndex*Cols;
    }
  else
    { LastSearchResult=FindNearestPoints(xarr=MData+XIndex,Rows,itemshift=Cols,x,&leftindex,&rightindex);
      yarr=MData+YIndex;
    }

  if(leftindex==rightindex)
    y=yarr[leftindex*itemshift];
  else
    { double yleft,yright;
      LastXL=xarr[leftindex*itemshift],
      LastXR=xarr[rightindex*itemshift],
      yleft=yarr[leftindex*itemshift],
      yright=yarr[rightindex*itemshift];
      if(LastXL==rdsbcppHugeDouble || LastXR==rdsbcppHugeDouble ||
         yleft==rdsbcppHugeDouble || yright==rdsbcppHugeDouble)
        { y=rdsbcppHugeDouble;
          LastSearchResult=RDSLININTER_NONE;
        }
      else
        { double dx=LastXR-LastXL;
          try
            { if(dx==0.0)
                { y=yright;
                  LastK=0.0;
                  LastB=yright;
                }
              else
                { LastK=(yright-yleft)/dx;
                  LastB=yright-LastK*LastXR;
                  y=LastK*x+LastB;
                }
            }
          catch(...)
            { y=yright;
              LastK=0.0;
              LastB=yright;
            }
        }
    }

  *py=y;

  return TRUE;
}
//---------------------------------------------------------------------------

// Вычислить значение функции одной переменной
//   XIndex - индекс строки (столбца) аргумента
//   YIndex - индекс строки (столбца) значения
//   x      - аргумент
double TRdsLinearInterpolation::Calc_Fx(double x,int XIndex,int YIndex)
{ double y;
  switch(Type)
    { case RDSLININTER_T_SINGLE: // Функция
        if(!ModelSingle(x,&y,XIndex,YIndex))
          return rdsbcppHugeDouble;
        break;
      case RDSLININTER_T_FIELD: // Семейство
        if(!ModelField(x,-1,0.0,1,&y))
          return rdsbcppHugeDouble;
        break;
      default:
        return rdsbcppHugeDouble;
    }
  return y;
}
//---------------------------------------------------------------------------

// Сортировать матрицу для семейства (с пропуском одного индекса)
BOOL TRdsLinearInterpolation::SortMatrixField(BOOL databyrows,int Rows,int Cols,
                                              double *MData,int XIndex,int excludeindex)
{

  InputMatrixSorted=TRUE;

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

// Поиск ограничивающих индексов в массиве с выкинутой точкой
// array        - массив
// count        - число точек
// itemshift    - расстояние между соседними точками в массиве
// exindex      - выкидываемфй индекс (должен обязательно находится внутри массива, -1 не допускается)
// pLeft,pRight - возвращаемые индексы ограничивающих точек
// Возврат      - RDSLININTER_*
int TRdsLinearInterpolation::FindNearestPointsEx(double *array,int count,int itemshift,int exindex,double x,int *pLeft,int *pRight)
{ int left=0,right=count,mid,diap,c_minus_1=count-1;
  double xleft,xright,xmid;

  if(exindex<0 || exindex>=count)
    return FindNearestPoints(array,count,itemshift,x,pLeft,pRight);

  if(exindex==0)
    { // Выкинуто первое значение - вызываем старую функцию начиная со второй точки
      // и сдвигаем индексы на 1
      mid=FindNearestPoints(array+itemshift,c_minus_1,itemshift,x,pLeft,pRight);
      (*pLeft)++;
      (*pRight)++;
      return mid;
    }
  if(exindex==c_minus_1) // Выкинуто последнее значение - вызываем старую функцию для меньшего массива
    return FindNearestPoints(array,c_minus_1,itemshift,x,pLeft,pRight);

  // exindex где-то внутри массива и не одна из крайних точек
  // Получается, что размер массива НЕ МЕНЬШЕ ТРЕХ

  xleft=array[0];
  xright=array[c_minus_1*itemshift];
  if(xleft==rdsbcppHugeDouble || xright==rdsbcppHugeDouble)
    { *pLeft=0;
      *pRight=0;
      return RDSLININTER_NONE;
    }

  if(x<xleft)
    { *pLeft=0;
      *pRight=(exindex==1)?2:1;
      return RDSLININTER_BELOW;
    }
  if(x>xright)
    { int c_minus_2=count-2;
      *pLeft=(exindex==c_minus_2)?(count-3):c_minus_2;
      *pRight=c_minus_1;
      return RDSLININTER_ABOVE;
    }

  for(;;)
    { diap=right-left;
      switch(diap)
        { case 0:
          case 1:
            *pLeft=left;
            *pRight=right;
            return diap==0?RDSLININTER_NONE:RDSLININTER_BETWEEN;

          case 2: // Ровно одна точка между left и right
            if(exindex==left+1) // Эта точка - exindex
              { *pLeft=left;
                *pRight=right;
                return RDSLININTER_BETWEEN;
              }

          default: // Две и более точек между left и right,
                   // или одна точка, но не exindex
            mid=left+diap/2;
            if(mid==exindex) // Попали в exindex - пробуем сдвинуть
              { mid--;
                if(mid==left) // Неудачно
                  mid+=2;
              }
            xmid=array[mid*itemshift];
            if(xmid==rdsbcppHugeDouble)
              { *pLeft=0;
                *pRight=0;
                return RDSLININTER_NONE;
              }
            if(x<xmid)
              { right=mid;
              }
            else
              { left=mid;
              }
        }
    }

}
//---------------------------------------------------------------------------

// Моделирование (семейство)
// ext_x1   - вход первого аргумента
// index_x1 - индекс первого аргумента (вместо входа, если <0, берется ext_x1)
// ext_x2   - вход второго аргумента
// index_x2 - индекс второго аргумента (вместо входа, если <0, берется ext_x2)
// py       - адрес значения
// Возврат  - успешность (есть значение)
BOOL TRdsLinearInterpolation::ModelField(double ext_x1,int index_x1,double ext_x2,int index_x2,double *py)
{ int Rows,Cols,itemshift_x,itemshift_k;
  double *MData,x,y,k,*yarr_kl,*yarr_kr,dx;
  double y_kl_xl,y_kl_xr,y_kr_xl,y_kr_xr,yl,yr;
  BOOL x_ok,k_ok,x_by_index=FALSE,k_by_index=FALSE;

  #define ext_x ext_x2
  #define ext_k ext_x1
  #define index_x index_x2
  #define index_k index_x1
  #define XIndex 0

  x=ext_x;
  k=ext_k;

  if(!Data.Exists) // Нет матрицы
    return FALSE;

  Rows=Data.Rows;
  Cols=Data.Cols;
  if(Rows<2 || Cols<2)
    return FALSE;
  MData=(double*)(Data.Data);

  if(index_x<=0) // Задано значение
    { if(x==rdsbcppHugeDouble)
        { *py=rdsbcppHugeDouble;
          return TRUE;
        }
    }
  else // Задан индекс
    { if(index_x>=(ByRows?Cols:Rows))
        { *py=rdsbcppHugeDouble;
          return TRUE;
        }
      x_by_index=TRUE;
    }


  if(index_k<=0) // Задано значение
    { if(k==rdsbcppHugeDouble)
        { *py=rdsbcppHugeDouble;
          return TRUE;
        }
    }
  else // Задан индекс
    { if(index_k>=(ByRows?Rows:Cols))
        { *py=rdsbcppHugeDouble;
          return TRUE;
        }
      k_by_index=TRUE;
    }

  if(!InputMatrixSorted)
    { // K
      if(!SortMatrixField(!ByRows,Rows,Cols,MData,0,0))
        { *py=rdsbcppHugeDouble;
          LastKSearchResult=RDSLININTER_NONE;
          InputMatrixSorted=FALSE;
          return TRUE;
        }
      // x
      if(!SortMatrixField(ByRows,Rows,Cols,MData,0,0))
        { *py=rdsbcppHugeDouble;
          LastSearchResult=RDSLININTER_NONE;
          InputMatrixSorted=FALSE;
          return TRUE;
        }
      LastSearchResult=LastKSearchResult=RDSLININTER_NONE;
    }


  // Проверка результатов прошлого моделирования. Если x остался в том же диапазоне,
  // новый поиск производить не нужно.
  if(x_by_index)
    { LeftXIndex=RightXIndex=index_x;
      LastSearchResult=RDSLININTER_NONE;
      x_ok=TRUE;
    }
  else switch(LastSearchResult)
    { case RDSLININTER_BELOW:
        x_ok=x<LastXL;
        break;
      case RDSLININTER_ABOVE:
        x_ok=x>LastXR;
        break;
      case RDSLININTER_BETWEEN:
        x_ok=x>LastXL && x<LastXR;
        break;
      default:
        x_ok=FALSE;
    }

  // Проверка результатов прошлого моделирования. Если k остался в том же диапазоне,
  // новый поиск производить не нужно.
  if(k_by_index)
    { LeftKIndex=RightKIndex=index_k;
      LastKSearchResult=RDSLININTER_NONE;
      k_ok=TRUE;
    }
  else switch(LastKSearchResult)
    { case RDSLININTER_BELOW:
        k_ok=k<LastKL;
        break;
      case RDSLININTER_ABOVE:
        k_ok=k>LastKR;
        break;
      case RDSLININTER_BETWEEN:
        k_ok=k>LastKL && k<LastKR;
        break;
      default:
        k_ok=FALSE;
    }

  // Нужно найти индексы точек строки (столбца) XIndex, между которыми расположен x
  if(ByRows) // Данные в строках
    { itemshift_x=1;
      itemshift_k=Cols;
      if(!x_ok)
        { double *xarr;
          LastSearchResult=FindNearestPointsEx(xarr=MData+XIndex*Cols,Cols,
                                               itemshift_x,0,x,&LeftXIndex,&RightXIndex);
          LastXL=xarr[LeftXIndex*itemshift_x];
          LastXR=xarr[RightXIndex*itemshift_x];
        }
      if(!k_ok)
        { double *karr;
          LastKSearchResult=FindNearestPointsEx(karr=MData,Rows,
                                                itemshift_k,XIndex,k,&LeftKIndex,&RightKIndex);
          LastKL=karr[LeftKIndex*itemshift_k];
          LastKR=karr[RightKIndex*itemshift_k];
        }
      yarr_kl=MData+LeftKIndex*Cols;
      yarr_kr=MData+RightKIndex*Cols;
    }
  else
    { itemshift_x=Cols;
      itemshift_k=1;
      if(!x_ok)
        { double *xarr;
          LastSearchResult=FindNearestPointsEx(xarr=MData+XIndex,Rows,
                                          itemshift_x,0,x,&LeftXIndex,&RightXIndex);
          LastXL=xarr[LeftXIndex*itemshift_x];
          LastXR=xarr[RightXIndex*itemshift_x];
        }
      if(!k_ok)
        { double *karr;
          LastKSearchResult=FindNearestPointsEx(karr=MData+XIndex*Cols,Cols,
                                                itemshift_k,XIndex,k,&LeftKIndex,&RightKIndex);
          LastKL=karr[LeftKIndex*itemshift_k];
          LastKR=karr[RightKIndex*itemshift_k];
        }
      yarr_kl=MData+LeftKIndex;
      yarr_kr=MData+RightKIndex;
    }

  // Найдены массивы и смещения
  y_kl_xl=yarr_kl[LeftXIndex*itemshift_x];
  y_kl_xr=yarr_kl[RightXIndex*itemshift_x];
  y_kr_xl=yarr_kr[LeftXIndex*itemshift_x];
  y_kr_xr=yarr_kr[RightXIndex*itemshift_x];


  if(y_kl_xl==rdsbcppHugeDouble || y_kl_xr==rdsbcppHugeDouble ||
     y_kr_xl==rdsbcppHugeDouble || y_kr_xr==rdsbcppHugeDouble)
    { *py=rdsbcppHugeDouble;
      LastSearchResult=RDSLININTER_NONE;
      return TRUE;
    }
  if(!k_by_index)
    { if(LastKL==rdsbcppHugeDouble || LastKR==rdsbcppHugeDouble)
        { *py=rdsbcppHugeDouble;
          LastSearchResult=RDSLININTER_NONE;
          return TRUE;
        }
    }

  // Найдены ограничивающие точки - аппроксимация y
  k_ok=TRUE;
  if(LeftKIndex==RightKIndex || LastKL==LastKR)
    { yl=y_kr_xl;
      yr=y_kr_xr;
    }
  else
    { double dk=LastKR-LastKL;
      try
        { yl=y_kl_xl+(k-LastKL)*(y_kr_xl-y_kl_xl)/dk;
          yr=y_kl_xr+(k-LastKL)*(y_kr_xr-y_kl_xr)/dk;
        }
      catch(...)
        { k_ok=FALSE; }
    }
  if(!k_ok)
    { yl=(y_kr_xl+y_kl_xl)/2;
      yr=(y_kr_xr+y_kl_xr)/2;
    }

  // Найдены yl и yr прямой, соответствующей значению k
  try
    { if(LeftXIndex==RightXIndex || LastXR==LastXL)
        *py=yr;
      else
        { dx=LastXR-LastXL;
          LastK=(yr-yl)/dx,
          LastB=yr-LastK*LastXR;
          *py=LastK*x+LastB;
        }
    }
  catch(...)
    { *py=yr; }

  return TRUE;
  #undef ext_x
  #undef ext_k
  #undef XIndex
  #undef index_x
  #undef index_k
}
//---------------------------------------------------------------------------

// Вычислить значение функции двух переменных
//   x1      - аргумент 1
//   x2      - аргумент 2
double TRdsLinearInterpolation::Calc_Fx1x2(double x1,double x2)
{ double y;
  switch(Type)
    { case RDSLININTER_T_SINGLE: // Функция
        if(!ModelSingle(x1,&y,0,1))
          return rdsbcppHugeDouble;
        break;
      case RDSLININTER_T_FIELD: // Семейство
        if(!ModelField(x1,-1,x2,-1,&y))
          return rdsbcppHugeDouble;
        break;
      default:
        return rdsbcppHugeDouble;
    }
  return y;
}
//---------------------------------------------------------------------------

// Вычислить значение функции двух переменных
//   x1     - аргумент 1
//   i2     - индекс в матрице аргумента 2
double TRdsLinearInterpolation::Calc_Fx1i2(double x1,int i2)
{ double y;
  switch(Type)
    { case RDSLININTER_T_SINGLE: // Функция
        if(!ModelSingle(x1,&y,0,1))
          return rdsbcppHugeDouble;
        break;
      case RDSLININTER_T_FIELD: // Семейство
        if(!ModelField(x1,-1,rdsbcppHugeDouble,i2,&y))
          return rdsbcppHugeDouble;
        break;
      default:
        return rdsbcppHugeDouble;
    }
  return y;
}
//---------------------------------------------------------------------------

// Вычислить значение функции двух переменных
//   i1      - индекс в матрице аргумента 1
//   x2      - аргумент 2
double TRdsLinearInterpolation::Calc_Fi1x2(int i1,double x2)
{ double y;
  switch(Type)
    { case RDSLININTER_T_SINGLE: // Функция
        return rdsbcppHugeDouble;
      case RDSLININTER_T_FIELD: // Семейство
        if(!ModelField(rdsbcppHugeDouble,i1,x2,-1,&y))
          return rdsbcppHugeDouble;
        break;
      default:
        return rdsbcppHugeDouble;
    }
  return y;
}
//---------------------------------------------------------------------------



// Эти описания больше не нужны
#undef RDSLININTER_NONE
#undef RDSLININTER_BELOW
#undef RDSLININTER_ABOVE
#undef RDSLININTER_BETWEEN
#endif
