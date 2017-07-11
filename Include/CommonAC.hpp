//---------------------------------------------------------------------------
// Описания модуля автокомпиляции BCpp55 в Common.dll
// (компилятор - Borland C++ 5.5)
//---------------------------------------------------------------------------
#ifndef CommonACH
#define CommonACH

#ifndef RdsDefH
  // Для нормальной компиляции необходимы описания из RdsDef.h
  #include "RdsDef.h"
#endif

#ifndef RdsFuncH
  // Для нормальной компиляции необходимы описания из RdsFunc.h
  #include "RdsFunc.h"
#endif

//---------------------------------------------------------------------------
// Класс-предок всех переменных
//---------------------------------------------------------------------------
class rdsbcppVarAncestor
{ protected:
    int _Num;
  public:
    inline int Num(void){return _Num;};
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return FALSE;};
    virtual void * GetVoidPtr(void) { return NULL;};


    rdsbcppVarAncestor(void){_Num=-1;};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Генератор класса для доступа к простой статической переменной блока
//---------------------------------------------------------------------------
#define RDSBCPP_STATICPLAINCLASS(classname,vartype) \
  class classname : public rdsbcppVarAncestor { protected: vartype *Ptr;  public: \
    inline void Init(void *base,int offset){Ptr=(vartype*)(((unsigned char*)base)+offset);}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    inline operator vartype() const {return *Ptr;}; \
    inline vartype * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline vartype operator=(classname &v){*Ptr=(vartype)v; return *Ptr; }; \
    inline vartype operator=(vartype v){*Ptr=v; return *Ptr; }; \
    inline vartype operator+=(vartype v){(*Ptr)+=v; return *Ptr; }; \
    inline vartype operator-=(vartype v){(*Ptr)-=v; return *Ptr; }; \
    inline vartype operator*=(vartype v){(*Ptr)*=v; return *Ptr; }; \
    inline vartype operator/=(vartype v){(*Ptr)/=v; return *Ptr; }; \
    inline vartype operator++(){++(*Ptr); return *Ptr; }; \
    inline vartype operator++(int){vartype tmp=*Ptr; (*Ptr)++; return tmp; }; \
    inline vartype operator--(){--(*Ptr); return *Ptr; }; \
    inline vartype operator--(int){vartype tmp=*Ptr; (*Ptr)--; return tmp; }; \
    classname(void):rdsbcppVarAncestor(){}; \
  }
/*
Объект этого класса ссылается на статическую переменную блока типа <vartype>,
адрес которой в дереве переменных задается функцией Init(<база>,<смещение>).
Определены различные операторы присваивания и преобразование к типу <vartype>.
Осмысленное создание объектов этого типа, не ссылающихся на переменную, невозможно.
Пример:
    Дерево переменных блока описывается строкой "{SSD}". Для доступа к третьей
    переменной (типа double) необходимо использовать следующий фрагмент:

    RDSBCPP_STATICPLAINCLASS(TAuxDoubleClass,double);  // Описание типа
    .....
    TAuxDoubleClass x;            // Объект для ссылки на переменную
    x.Init(BlockData->VarData,2); // BlockData - передаваемый в функцию
                                  // отклика блока параметр типа RDS_PBLOCKDATA
                                  // 2 - смещение в дереве (sizeof(char)+sizeof(char))
    ......
    x=2.0*x;                      // Использование объекта

В случаях, когда возможны разночтения (например, в аргументе функций типа
printf), следует использовать явное приведение типа:
    char buf[100];
    sprintf(buf,"%lf",(double)x);
Следует также помнить, что выражение &x в данном примере будет иметь тип
TAuxDoubleClass*, а не double*. Чтобы получить указатель на переменную,
на которую ссылается объект x, необходимо использовать функцию x.GetPtr().
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Класс-заглушка для простой статической переменной блока (ничего не делает)
//---------------------------------------------------------------------------
class rdsbcppStaticStub : public rdsbcppVarAncestor
{ protected:
    double Internal;
    BOOL ShowMsg;
    void Msg(void)
      { if(ShowMsg)
          rdsMessageBox("При инициализации и очистке блока нельзя обращаться "
                        "к статическим переменным",
          #ifdef RDSBCPP_MODELNAME
                        RDSBCPP_MODELNAME,
          #else
                        NULL
          #endif
                        MB_OK|MB_ICONERROR);
        ShowMsg=FALSE;
      };
  public:
    inline operator double() { Msg(); return 0.0;};
    inline void * GetPtr(void) { Msg(); return &Internal;};
    virtual void * GetVoidPtr(void) { Msg(); return &Internal;}; 
    inline double operator=(rdsbcppVarAncestor &v){ Msg(); return 0.0; };
    inline double operator=(double v){ Msg(); return 0.0; };
    inline double operator=(void *v){ Msg(); return 0.0; };
    inline double operator+=(double v){ Msg(); return 0.0; };
    inline double operator-=(double v){ Msg(); return 0.0; };
    inline double operator*=(double v){ Msg(); return 0.0; };
    inline double operator/=(double v){ Msg(); return 0.0; };
    inline double operator++(){ Msg(); return 0.0; };
    inline double operator++(int){ Msg(); return 0.0; };
    inline double operator--(){ Msg(); return 0.0; };
    inline double operator--(int){ Msg(); return 0.0; };
    inline rdsbcppStaticStub & operator[](int i){ Msg(); return *this; };
    inline int Cols(void){Msg(); return 0;};
    inline int Rows(void){Msg(); return 0;};
    inline int Size(void){Msg(); return 0;};
    inline BOOL IsEmpty(void){Msg(); return TRUE;};
    inline BOOL HasData(void){Msg(); return FALSE;};
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){Msg(); return FALSE;};
    inline void * GetBuffer(void){return &Internal;};
    inline BOOL Resize(int rows,int cols,BOOL keep){Msg(); return FALSE;};
    inline BOOL Resize(int size,BOOL keep){Msg(); return FALSE;};
    inline BOOL Resize(int size){Msg(); return FALSE;};
    inline rdsbcppStaticStub & Item(int r,int c){ Msg(); return *this; };
    rdsbcppStaticStub(void):rdsbcppVarAncestor(){Internal=0.0;ShowMsg=TRUE;};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Генератор класса для доступа к статическому двухиндексному массиву (матрице)
// простых переменных блока
//---------------------------------------------------------------------------
/*
#ifdef RDSBCPP_INDEXCHECK
  #define RDSBCPP_INDEXCHECKOP(clname,vtype) \
    inline clname##_ operator[](int r) \
      { clname##_ el; el.Ptr=Ptr; el.r=r; return el; };
#else
  #define RDSBCPP_INDEXCHECKOP(clname,vtype) \
      inline vtype * operator[](int i){return ((vtype*)(RDS_ARRAYDATA(Ptr)))+i*(RDS_ARRAYCOLS(Ptr));};
#endif
*/

#ifndef RDSBCPP_INDEXCHECK

#define RDSBCPP_STATICPLAINMATRCLASS(classname,vartype) \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline vartype * operator[](int i){return ((vartype*)(RDS_ARRAYDATA(Ptr)))+i*(RDS_ARRAYCOLS(Ptr));}; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYCOLS(Ptr):0;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYROWS(Ptr):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline vartype * GetBuffer(void){return (vartype*)(RDS_ARRAYDATA(Ptr));}; \
    inline BOOL Resize(int rows,int cols,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,rows,cols,keep,NULL); } \
    inline vartype & Item(int r,int c) \
      { static vartype baddata; \
        if(!rdsVarArrayIndexCheck(Ptr,r,c,RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,NULL,NULL)) \
          { memset(&baddata,0,sizeof(vartype));  return baddata; } \
        return *(GetBuffer()+r*(RDS_ARRAYCOLS(Ptr))+c); }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; }; \
  }

#endif
/*
Объект этого класса ссылается на статическую переменную блока типа "матрица <vartype>",
адрес которой в дереве переменных задается функцией Init(<база>,<смещение>).
<vartype> должен быть ПРОСТЫМ типом, т.е. char, int, short int, float или double.
В блоках это соответствует переменным 'S', 'L', 'C', 'H', 'I', 'F' и 'D'.
Определен оператор [], возвращающий начало массива для второго индекса, т.е. к элементу
массива X (i,j) можно обращаться при помощи операторов X[i][j] (проверка допустимости
индексов не производится).
Также определены функции:
    int Rows(void)     - размерность по первому индексу (число строк)
    int Cols(void)     - размерность по второму индексу (число столбцов)
    int Size(void)     - общее число элементов (Rows()*Cols())
    BOOL HasData(void) - массив существует (т.е. не 0x0)
    BOOL IsEmpty(void) - !HasData()
    BOOL GetAccessData(RDS_PARRAYACCESSDATA data)  - заполняет структуру
                                                     RDS_ARRAYACCESSDATA (см. RdsDef.h)
    BOOL Resize(int rows,int cols,BOOL keep=FALSE) - меняет размер массива
                                                     (см. ф-цию rdsResizeVarArray)
    <vartype> * GetBuffer(void) - возвращает начало внутреннего массива элементов
                                  <vartype> размера Size()
    static int DataSize(void)   - размер переменной в дереве (массив и матрица всегда
                                  занимают два адреса, т.е. 2*sizeof(void*)
    void * GetPtr(void)         - возвращает адрес начала переменной (определенный при
                                  вызове Init()
Пример:
    Дерево переменных блока описывается строкой "{SSDMI}". Для доступа к четвертой
    перменной (типа "MI", т.е. массиву int) необходимо использовать следующий фрагмент:

    RDSBCPP_STATICPLAINMATRCLASS(TAuxIntArrayClass,int);  // Описание типа
    .....
    TAuxIntArrayClass x;          // Объект для ссылки на переменную
    x.Init(BlockData->VarData,10);// BlockData - передаваемый в функцию
                                  // отклика блока параметр типа RDS_PBLOCKDATA
                                  // 10 - смещение в дереве (sizeof(char)+sizeof(char)+sizeof(double))
    ......
   for(int i=0;i<x.Rows();i++)           // Использование объекта
     for(int j=0;j<x.Cols();j++)
       x[i][j]=i+j;

Возможно создание вспомогательных переменных этого типа для доступа к массивам, которые
сами являются элементами массивов или структур. Для таких вспомогательных переменных
определен оператор присваивания. Пример использования вспомогательных переменных
см. в описании генератора классов для массивов сложных переменных
(RDSBCPP_STATICCOMPLMATRCLASS)
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Генератор класса для доступа к статическому одноиндексному массиву
// простых переменных блока
//---------------------------------------------------------------------------
#ifndef RDSBCPP_INDEXCHECK

#define RDSBCPP_STATICPLAINARRCLASS(classname,vartype) \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?1:0;}; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int size,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,1,size,keep,NULL); } \
    inline vartype * GetBuffer(void){return (vartype*)(RDS_ARRAYDATA(Ptr));}; \
    inline vartype & Item(int n) \
      { static vartype baddata; \
        if(!rdsVarArrayIndexCheck(Ptr,n,1,RDS_VAIC_SINGLE|RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,NULL,NULL)) \
          { memset(&baddata,0,sizeof(vartype));  return baddata; } \
        return *(GetBuffer()+n); }; \
    inline vartype & operator[](int i){ return *(((vartype*)(RDS_ARRAYDATA(Ptr)))+i); }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; }; \
  }

#endif
/*
Объект этого класса ссылается на статическую переменную блока типа "массив <vartype>",
где <vartype> - ПРОСТОЙ тип. Этот класс аналогичен классу, описанному с помощью
RDSBCPP_STATICPLAINMATRCLASS за исключением работы с индексами - у массива, в отличие
от матрицы, индекс только один. Оператор [] вовращает значение типа <vartype>, а в
функцию Resize передается общий размер массива вместо числа строк и числа столбцов.
Поскольку одноиндексные и двухиндексные массивы (матрицы) в RDS физически хранятся одинаково,
оба класса RDSBCPP_STATICPLAINMATRCLASS и RDSBCPP_STATICPLAINARRCLASS можно использовать
как для массивов, так и для матриц.
Пример:
    Дерево переменных блока описывается строкой "{SSDMI}". Для доступа к четвертой
    перменной (типа "MI", т.е. массиву int) необходимо использовать следующий фрагмент:

    RDSBCPP_STATICPLAINARRCLASS(TAuxIntArrayClass,int);  // Описание типа
    .....
    TAuxIntArrayClass x;          // Объект для ссылки на переменную
    x.Init(BlockData->VarData,10);// BlockData - передаваемый в функцию
                                  // отклика блока параметр типа RDS_PBLOCKDATA
                                  // 10 - смещение в дереве (sizeof(char)+sizeof(char)+sizeof(double))
    ......
    for(int i=0;i<x.Size();i++)    // Использование объекта
      x[i]=i;
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Генератор классов для доступа к статическому двухиндексному массиву (матрице)
// сложных переменных блока
//---------------------------------------------------------------------------
#ifndef RDSBCPP_INDEXCHECK

#define RDSBCPP_STATICCOMPLMATRCLASS(classname,elementclass) \
  class classname##_ { public: unsigned char *Base; \
    inline elementclass operator[](int i) \
      { elementclass el; \
        el.Init(Base+i*elementclass::DataSize(),0); \
        return el; \
      }; \
    }; \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline classname##_ operator[](int i) \
      { classname##_ el; \
        el.Base=((unsigned char*)RDS_ARRAYDATA(Ptr))+i*RDS_ARRAYCOLS(Ptr)*elementclass::DataSize(); \
        return el; \
      }; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYCOLS(Ptr):0;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYROWS(Ptr):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int rows,int cols,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,rows,cols,keep,NULL); } \
    inline elementclass Item(int r,int c) \
      { elementclass el; \
        classname##_ elc; \
        if(!rdsVarArrayIndexCheck(Ptr,r,c,RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,NULL,NULL)) \
          return el; \
        elc.Base=((unsigned char*)RDS_ARRAYDATA(Ptr))+r*RDS_ARRAYCOLS(Ptr)*elementclass::DataSize(); \
        return elc[c]; \
      }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; }; \
  }

#endif
/*
Объект этого класса ссылается на статическую переменную блока типа "матрица сложных объектов".
Сложный объект описывается классом <elementclass>. В качестве <elementclass> может быть
использован один из следующих классов:
  - массив или матрица простых объектов (класс, сгенерированный макросами
    RDSBCPP_STATICPLAINMATRCLASS или RDSBCPP_STATICPLAINARRCLASS)
  - массив или матрица сложных объектов (класс, сгенерированный макросами
    RDSBCPP_STATICCOMPLMATRCLASS или RDSBCPP_STATICCOMPLARRCLASS)
  - структура-потомок rdsbcppBasicStruct (см. ниже)
В классе <elementclass> обязательно должна быть определена функции Init(<база>,<смещение>) и
статическая функция DataSize(void). Описание класса <elementclass> должно предшествовать
данному описанию.
Адрес переменной-массива в дереве переменных задается функцией Init(<база>,<смещение>).
Определен оператор [], возвращающий объект вспомогательного класса, для которого тоже
определен оператор [], возвращающий временный (вспомогательный) объект класса <elementclass>.
Таким образом, к элементу массива X (i,j) можно обращаться при помощи операторов X[i][j]
(проверка допустимости индексов не производится).
Также определены функции:
    int Rows(void)     - размерность по первому индексу (число строк)
    int Cols(void)     - размерность по второму индексу (число столбцов)
    int Size(void)     - общее число элементов (Rows()*Cols())
    BOOL HasData(void) - массив существует (т.е. не 0x0)
    BOOL IsEmpty(void) - !HasData()
    BOOL GetAccessData(RDS_PARRAYACCESSDATA data)  - заполняет структуру
                                                     RDS_ARRAYACCESSDATA (см. RdsDef.h)
    BOOL Resize(int rows,int cols,BOOL keep=FALSE) - меняет размер массива
                                                     (см. ф-цию rdsResizeVarArray)
    static int DataSize(void)   - размер переменной в дереве (массив и матрица всегда
                                  занимают два адреса, т.е. 2*sizeof(void*)
    void * GetPtr(void)         - возвращает адрес начала переменной (определенный при
                                  вызове Init()
Пример:
    Дерево переменных блока описывается строкой "{SSMMD}". Для доступа к третьей
    перменной (типа "MMD", т.е. массиву массивов double) необходимо использовать
    следующий фрагмент:

    RDSBCPP_STATICPLAINMATRCLASS(TAuxDoubleMatrClass,double);// Описание типа MD,
                                                             // т.е. массив double
    RDSBCPP_STATICCOMPLMATRCLASS(TAuxDoubleMatrMatrClass,    // Описание типа MMD,
                                 TAuxDoubleMatrClass);       // т.е. массив TAuxDoubleMatrClass
    .....
    TAuxDoubleMatrMatrClass x;    // Объект для ссылки на переменную
    x.Init(BlockData->VarData,2); // BlockData - передаваемый в функцию
                                  // отклика блока параметр типа RDS_PBLOCKDATA
                                  // 2 - смещение в дереве (sizeof(char)+sizeof(char))
    ......
    x.Resize(10,10);              // Задание размера массива x
    x[0][0].Resize(5,5);          // Задание размера массива-элемента x(0,0)
    for(int i=0;i<5;i++)          // Занесение значений
      for(int j=0;j<5;j++)
         x[0][0][i][j]=i+j;
    ......

Можно использовать вспомогательные переменные для доступа к массивам-элементам:

    TAuxDoubleMatrClass temp;     // Вспомогательная переменная
    temp=x[1][2];                 // Теперь temp ссылается на массив-элемент x(1,2)
    temp.Resize(10,5);            // Задается размер x(1,2)
    for(int i=0;i<10;i++)         // Занесение значений в x(1,2)
      for(int j=0;j<5;j++)
         temp[i][j]=0;

Следует помнить, что переменную temp в данном примере можно использовать
только до тех пор, пока размер массива x не будет изменен:

    x.Resize(20,20,TRUE);         // Размер x изменен с сохранением данных, но память
                                  // переотведена. Теперь все ссылки временных переменных
                                  // недействительны.
    temp[0][0]=100;               // Вероятно, возникнет исключение типа Access Violation

Чтобы избежать этой ситуации, следует заново присвоить значение temp после переотведения
массива x:

    x.Resize(20,20,TRUE);         // Размер x изменен, память переотведена.
    temp=x[1][2];                 // Теперь temp ссылается на массив-элемент x(1,2)
                                  // уже в новом блоке памяти
    temp[0][0]=100;               // Все в порядке
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Генератор классов для доступа к статическому одноиндексному массиву
// сложных переменных блока
//---------------------------------------------------------------------------
#ifndef RDSBCPP_INDEXCHECK

#define RDSBCPP_STATICCOMPLARRCLASS(classname,elementclass) \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline elementclass operator[](int i) \
      { elementclass el; \
        el.Init(RDS_ARRAYDATA(Ptr),i*elementclass::DataSize()); \
        return el; \
      }; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?1:0;}; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int size,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,1,size,keep,NULL); } \
    inline elementclass Item(int n) \
      { elementclass el; \
        if(!rdsVarArrayIndexCheck(Ptr,n,1,RDS_VAIC_SINGLE|RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,NULL,NULL)) \
          return el; \
        el.Init(RDS_ARRAYDATA(Ptr),n*elementclass::DataSize()); \
        return el; \
      }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; }; \
  }

#endif
/*
Объект этого класса ссылается на статическую переменную блока типа "массив сложных объектов"
и практически полностью аналогичен RDSBCPP_STATICCOMPLMATRCLASS. Отличия:
  - оператор [] сразу возвращает вспомогательный объект типа <elementclass>,
  - функции Rows() и Cols() возвращают полный размер и 1 соответственно,
  - функция Resize() принимает только один аргумент.
Пример:
    Пример для ситуации, описанной в примере к RDSBCPP_STATICCOMPLMATRCLASS, только вместо
    матрицы матриц используется массив матриц.

    RDSBCPP_STATICPLAINMATRCLASS(TAuxDoubleMatrClass,double);// Описание типа MD,
                                                             // т.е. массив double
    RDSBCPP_STATICCOMPLARRCLASS(TAuxDoubleMatrArrClass,      // Описание типа MMD,
                                 TAuxDoubleMatrClass);       // т.е. одноиндексный массив TAuxDoubleMatrClass
    .....
    TAuxDoubleMatrArrClass x;     // Объект для ссылки на переменную
    x.Init(BlockData->VarData,2); // BlockData - передаваемый в функцию
                                  // отклика блока параметр типа RDS_PBLOCKDATA
                                  // 2 - смещение в дереве (sizeof(char)+sizeof(char))
    ......
    x.Resize(10);                 // Задание размера массива x
    x[0].Resize(5,5);             // Задание размера массива-элемента x(0)
    for(int i=0;i<5;i++)          // Занесение значений
      for(int j=0;j<5;j++)
         x[0][i][j]=i+j;
    ......
    TAuxDoubleMatrClass temp;     // Вспомогательная переменная
    temp=x[1];                    // Теперь temp ссылается на массив-элемент x(1)
    temp.Resize(10,5);            // Задается размер x(1,2)
    for(int i=0;i<10;i++)         // Занесение значений в x(1,2)
      for(int j=0;j<5;j++)
        temp[i][j]=0;
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Базовый класс для доступа к структурам. Все реальные классы доступа к
// структурам должны производиться от него.
//---------------------------------------------------------------------------
class rdsbcppBasicStruct : public rdsbcppVarAncestor
{ protected:
    void **Ptr;         // Базовый адрес структуры
    BOOL Fixed;         // Признак статического объекта (т.е. ссылающегося на
                        // данные блока)

    // Функция присваивания для переопределяемого оператора присваивания
    inline BOOL Assign(const rdsbcppBasicStruct &val)
      { if(!Fixed) // Присваивание разрешено
          { Ptr=val.GetPtr();
            InitMembers(*Ptr);
            return TRUE;
          }
        return FALSE;
      };

  public:
    // Инициализация полей структуры, должна переопределяться в каждом
    // производном классе
    virtual void InitMembers(void *base)=0;

    // Инициализация структуры
    inline void Init(void *base,int offset)
      { Ptr=(void**)(((unsigned char*)base)+offset);
        Fixed=TRUE;
        InitMembers(*Ptr);
      };
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;};

    // Размер переменной в дереве (любая структура всегда
    // занимает один адрес, т.е. sizeof(void*))
    static inline int DataSize(void){return sizeof(void*);};

    // Адрес переменной в дереве
    inline void ** GetPtr(void) const {return Ptr;};
    virtual void * GetVoidPtr(void) { return Ptr;}; 

    // Переопределенный оператор присваивания должен выглядеть так:
    // TMyClass & operator=(const TMyClass &val)
    //   { if(Assign(val)) return *this; // Присвоено значение временной переменной
    //     <действия по присвоению значений полям статической переменной>
    //     return *this;
    //   };

    // Конструктор
    rdsbcppBasicStruct(void):rdsbcppVarAncestor(){ Fixed=FALSE; };
    // Деструктор (пустой)
    virtual ~rdsbcppBasicStruct(){};
};
/*
Объект потомка этого класса ссылается на статическую переменную блока типа "структура".
Для каждого типа структуры необходим свой класс-потомок со своими полями, оператором
присваивания и функцией InitMembers.
Пример:
    Дерево переменных блока описывается строкой "{SS{DD}D}". Третья переменная блока - структура
    "Complex" с двумя полями Re и Im типа double. Четвертая переменная - обычная переменная
    типа double (введена для большей осмысленности примера). Для доступа к переменным блока
    можно использовать следующий фрагмент:

    RDSBCPP_STATICPLAINCLASS(TAuxDoubleClass,double);  // Описание типа ссылки на double
                                                       // (необходим для полей структуры)
   // Описание структуры
   class TrdsComplex : public rdsbcppBasicStruct
     { public:
         TAuxDoubleClass Re,Im; // Поля

        // Инициализация полей структуры (вместо виртуальной rdsbcppBasicStruct::InitMembers)
        void InitMembers(void *base)
          { Re.Init(base,0);      // Поле Re - первое в стркутуре
            Im.Init(base,8);      // Поле Im следует за Re, т.е. через sizeof(double) байт
          };

        // Переопределенный оператор присваивания
        TrdsComplex & operator=(const TrdsComplex &val)
          { if(Assign(val)) return *this; // Присвоено значение временной переменной
            // Присвоение значений статической переменной (поэлементное)
            Re=val.Re;
            Im=val.Im;
            return *this;
          };

        // Конструктор
        TrdsComplex(void):rdsbcppBasicStruct()
          { // Здесь могут быть какие-нибудь пользовательские операции, но следует помнить,
            // что на момент вызова конструктора InitMembers еще не вызывалась, и поля структуры
            // ни на что не ссылаются
          };
    }; // class TrdsComplex

    // ......
    TrdsComplex x;      // Переменная для ссылки на структуру
    TAuxDoubleClass y;  // Переменная для ссылки на 4-ю переменную блока
    x.Init(BlockData->VarData,2); // BlockData - передаваемый в функцию
                                  // отклика блока параметр типа RDS_PBLOCKDATA
                                  // 2 - смещение в дереве (sizeof(char)+sizeof(char))
    y.Init(BlockData->VarData,6); // sizeof(char)+sizeof(char)+sizeof(void*)
    // ......
    y=sqrt(x.Re*x.Re+x.Im*x.Im);  // Использование полей как переменных

Можно объявлять вспомогательные переменные для ссылки на настоящие структуры.

    TrdsComplex aux;    // Переменная для ссылки на структуру
    aux=x;              // Теперь aux ссылается на x, и любые операции с ней
                        // на самом деле будут производиться над x.

Присваивание временной переменной отличается от присваивания переменной, ссылающейся
на структуру блока. В первом случае (см. пример выше) функция rdsbcppBasicStruct::Assign
вернет TRUE, и временная переменная будет ссылаться на статическую. Во втором случае
Assign вернет FALSE, и будет выполнено последовательное присваивание всем полям одной
структуры полей другой.

Не следует обращаться к полям временной переменной до присвоения ей значения, поскольку
изначально временная переменная не ссылается не на какую статическую структуру:
    TrdsComplex aux1;   // Переменная для ссылки на структуру
    aux1.Re=5.0;        // Access Violation, т.к. aux1 и aux1.Re не инициализированы
*/

// Макрос для переопределяемого оператора присваивания структур
#define RDSBCPP_STRUCTASSIGNMENTOP(classname,assignmentfunction) \
  classname & operator=(const classname &val) \
    { if(Assign(val)) return *this; \
      assignmentfunction(val); \
      return *this; \
    }
/*
Этот макрос можно использовать для переопределения оператора присваивания в потомке класса
rdsbcppBasicStruct. В качестве параметров следует указать имя класса потомка
<classname> и имя функции присваивания <assignmentfunction>, созданной пользователем,
которая присвоит всем полям этой структуры значения полей структуры, переданной в
качестве параметра. В приведенном выше примере для TrdsComplex переопределение
оператора присваивания могло выглядеть так:
   class TrdsComplex : public rdsbcppBasicStruct
     { public:
         .........
         // Функция присваивания
         void AssignMemebers(const TrdsComplex &val)
           { Re=val.Re;
             Im=val.Im;
           }
         // Переопределенный оператор присваивания
         RDSBCPP_STRUCTASSIGNMENTOP(TrdsComplex,AssignMemebers);
         .........
     };
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Генератор класса для доступа к простой динамической переменной блока
//---------------------------------------------------------------------------
#define RDSBCPP_DYNAMICPLAINCLASS(classname,vartype,rdsvartypestring) \
  class classname : public rdsbcppVarAncestor { protected: RDS_PDYNVARLINK Link; public: \
    inline void DynInit(void) {}; \
    inline operator vartype() const {return *((vartype*)(Link->Data));}; \
    inline vartype * GetPtr(void) const {return (vartype*)(Link->Data);}; \
    virtual void * GetVoidPtr(void) { return (vartype*)(Link->Data);}; \
    inline RDS_PDYNVARLINK GetLink(void) const {return Link;}; \
    inline BOOL CheckLink(RDS_PDYNVARLINK chglnk) const {return Link==chglnk;}; \
    inline void NotifySubscribers(void) const {rdsNotifyDynVarSubscribers(Link);}; \
    inline vartype operator=(classname &v) \
      {vartype aux; *((vartype*)(Link->Data))=aux=(vartype)v; return aux; }; \
    inline vartype operator=(vartype v) \
      {*((vartype*)(Link->Data))=v; return v; }; \
    inline vartype operator+=(vartype v) \
      {vartype *aux=(vartype*)(Link->Data); (*aux)+=v; return *aux; }; \
    inline vartype operator-=(vartype v) \
      {vartype *aux=(vartype*)(Link->Data); (*aux)-=v; return *aux; }; \
    inline vartype operator*=(vartype v) \
      {vartype *aux=(vartype*)(Link->Data); (*aux)*=v; return *aux; }; \
    inline vartype operator/=(vartype v) \
      {vartype *aux=(vartype*)(Link->Data); (*aux)/=v; return *aux; }; \
    inline vartype operator++() \
      {++(*((vartype*)(Link->Data))); return *((vartype*)(Link->Data)); }; \
    inline vartype operator++(int) \
      {vartype tmp=*((vartype*)(Link->Data)); (*((vartype*)(Link->Data)))++; return tmp; }; \
    inline vartype operator--() \
      {--(*((vartype*)(Link->Data))); return *((vartype*)(Link->Data)); }; \
    inline vartype operator--(int) \
      {vartype tmp=*((vartype*)(Link->Data)); (*((vartype*)(Link->Data)))--; return tmp; }; \
    inline BOOL Exists(void) \
      {return ((Link!=NULL) && (Link->Data!=NULL)); }; \
    inline void Unsubscribe(void){rdsUnsubscribeFromDynamicVar(Link);Link=NULL;}; \
    inline BOOL Subscribe(int Block,char *Name,BOOL Search) \
      { Unsubscribe(); return (Link=rdsSubscribeToDynamicVar(Block,Name,rdsvartypestring,Search))!=NULL; }; \
    inline BOOL Delete(void) \
      { RDS_PDYNVARLINK tmp=Link; Link=NULL; return rdsDeleteDVByLink(tmp); }; \
    inline BOOL Create(int Block,char *Name,BOOL fixed) \
      { Delete(); return (Link=rdsCreateAndSubscribeDV(Block,Name,rdsvartypestring,fixed,NULL))!=NULL; }; \
    classname(void):rdsbcppVarAncestor(){ Link=NULL; }; \
  }
/*
Объект этого класса ссылается на динамическую переменную типа <vartype>. Необходимо также
указать строку описания типа переменной <rdsvartypestring>, используемую в RDS (для
double - "D", для int - "I" и т.п., см. все возможные описания в RdsDef.h).
Определены различные операторы присваивания и преобразование к типу <vartype>.
Кроме того, определены следующие функции:
  BOOL Exists(void) - динамическая переменная существует (подписка прошла успешно)
  BOOL Subscribe(int Block,char *Name,BOOL Search) - подписаться на переменную Name
                                                     в блоке Block с возможностью поиска
                                                     (см. ф-цию rdsSubscribeToDynamicVar)
  void Unsubscribe(void) - отменить подписку на динамическую переменную
  BOOL Create(int Block,char *Name,BOOL fixed) - создать динамическую переменную
                                                 (см. ф-цию rdsCreateAndSubscribeDV)
  BOOL Delete(void) - удалить динамическую переменную.
  BOOL CheckLink(RDS_PDYNVARLINK chglnk) - сравнить указатель на переданную структуру
                                           связи с собственной (может использоваться при
                                           реакции на изменение динамической переменной,
                                           когда передается указатель на изменившуюся)
  void NotifySubscribers(void) - уведомить подписчиков об изменении динамической переменной
                                 (см. ф-цию rdsNotifyDynVarSubscribers)

Пример:
  Доступ к стандартной динамической переменной времени для динамического расчета:

    RDSBCPP_DYNAMICPLAINCLASS(TAuxDynDouble,double,"D"); // Описание класса
    // ......
    TAuxDynDouble time;                          // Создание объекта (пока не ссылается
                                                 // на переменную)
    time.Subscribe(RDS_DVPARENT,"DynTime",TRUE); // Подписка
    // ......
    if(time.Exists())                            // Переменная существует
      { double v=time*2.0;                       // Использование объекта
        // ......
      }
    // ......
    time.Unsubscribe();                          // Прекращение подписки

  Создание динамической переменной:

    RDSBCPP_DYNAMICPLAINCLASS(TAuxDynDouble,double,"D"); // Описание класса
    // ......
    TAuxDynDouble test;                          // Создание объекта (пока не ссылается
                                                 // на переменную)
    test.Create(RDS_DVSELF,"TestVar",TRUE);      // Создание переменной
    // ......
    if(test.Exists())                            // Переменная существует
      { test=1234.56;                            // Использование объекта
        // ......
      }
    // ......
    test.Delete();                               // Удаление переменной
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Генератор класса для доступа к сложной динамической переменной блока
//---------------------------------------------------------------------------
#define RDSBCPP_DYNAMICCLASS(classname,varclass,rdsvartype) \
  class classname : public varclass \
  { protected: RDS_PDYNVARLINK Link; public: \
    inline BOOL Exists(void){return Link!=NULL && Link->Data!=NULL; }; \
    void DynInit(void) { if(Exists()) Init(Link->Data,0); }; \
    inline RDS_PDYNVARLINK GetLink(void) const {return Link;}; \
    inline BOOL CheckLink(RDS_PDYNVARLINK chglnk) const {return Link==chglnk;}; \
    inline void NotifySubscribers(void) const {rdsNotifyDynVarSubscribers(Link);}; \
    inline void Unsubscribe(void){rdsUnsubscribeFromDynamicVar(Link);Link=NULL;}; \
    inline BOOL Subscribe(int Block,char *Name,BOOL Search) \
      { Unsubscribe(); return (Link=rdsSubscribeToDynamicVar(Block,Name,rdsvartype,Search))!=NULL; }; \
    inline BOOL Delete(void) \
      { RDS_PDYNVARLINK tmp=Link; Link=NULL; return rdsDeleteDVByLink(tmp); }; \
    inline BOOL Create(int Block,char *Name,BOOL fixed) \
      { Delete(); return (Link=rdsCreateAndSubscribeDV(Block,Name,rdsvartype,fixed,NULL))!=NULL; }; \
    varclass & operator=(const varclass &val) \
       { this->varclass::operator=(val); \
         return *this; \
       }; \
    classname(void):varclass(){ Link=NULL; }; \
  }
/*
Объект этого класса ссылается на динамическую переменную, соответствующую классу <varclass>.
Необходимо также указать строку описания типа переменной <rdsvartypestring>, используемую в RDS.
Генерируемый класс <classname> будет являться потомком класса <varclass>, который должен
быть уже описан ранее. В качестве <varclass> может быть использован один из следующих классов:
  - массив или матрица простых объектов (класс, сгенерированный макросами
    RDSBCPP_STATICPLAINMATRCLASS или RDSBCPP_STATICPLAINARRCLASS)
  - массив или матрица сложных объектов (класс, сгенерированный макросами
    RDSBCPP_STATICCOMPLMATRCLASS или RDSBCPP_STATICCOMPLARRCLASS)
  - структура-потомок rdsbcppBasicStruct
В генерируемом классе определены функции Exists, Subscribe, Unsubscribe, Create и Delete,
аналогичные RDSBCPP_DYNAMICPLAINCLASS, и оператор присваивания, вызывающий оператор
присваивания класса-предка.

Пример:
  Подписка на динамическую переменную типа Complex ("{DD}") и получение ее значений

    // Описание структуры
    class TrdsComplex : public rdsbcppBasicStruct
      { public:
          TAuxDoubleClass Re,Im; // Поля

          // Инициализация полей структуры (вместо виртуальной rdsbcppBasicStruct::InitMembers)
          void InitMembers(void *base)
            { Re.Init(base,0);      // Поле Re - первое в стркутуре
              Im.Init(base,8);      // Поле Im следует за Re, т.е. через sizeof(double) байт
            };

          // Функция присваивания
          void AssignMemebers(const TrdsComplex &val)
            { Re=val.Re;
              Im=val.Im;
            }
          // Переопределенный оператор присваивания
          RDSBCPP_STRUCTASSIGNMENTOP(TrdsComplex,AssignMemebers);
      }; // class TrdsComplex

    // Класс для ссылки на динамическую переменную
    RDSBCPP_DYNAMICCLASS(TrdsComplex_dyn,TrdsComplex,"{DD}");
    // ......
    TrdsComplex_dyn dynComp;            // Описание переменной
    // ......
    dynComp.Subscribe(RDS_DVPARENT,"TestCompl",FALSE); // Подписка на переменную
    // ......
    dynComp.DynInit();  // Инициализация
    if(dynComp.Exists())
      { double x=dynComp.Re*2.0;        // Использование
        // ......
      }
    // ......
    dynComp.Unsubscribe(); // Прекращение подписки

Возможно присваивание объектов класса <varclass> объектам класса <classname> и наоборот:

    TrdsComplex aux;
    aux.Init(......);   // Инициализация для работы со статической переменной блока
    // ......
    dynComp=aux;        // Допустимое присваивание
    // ......
    aux=dynComp;        // Тоже допустимое присваивание
*/
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Вспомогательный класс строки
//---------------------------------------------------------------------------
#ifndef RDSBCPP_NOSTRINGCLASS
class rdsbcppString : public rdsbcppVarAncestor
{ protected:
    char *Data; // Используется для локальных переменных
    char **Ptr; // Используется для переменных блока
  public:

    // Инициализация для переменных блока
    void Init(void *base,int offset)
      { Ptr=(char**)(((unsigned char*)base)+offset);
        if(Data) rdsFree(Data);
        Data=NULL;              // Переменная больше не локальная
      };
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;};
    // Размер переменной в дереве блока
    static inline int DataSize(void){return sizeof(char*);};
    // Адрес переменной в дереве
    inline void * GetPtr(void) const {return Ptr;};
    virtual void * GetVoidPtr(void) { return Ptr;}; 

    char *c_str(void) const
    #ifdef RDSBCPP_DMWORKAROUND
      {return (char*)((*Ptr)?(*Ptr):"");};
    #else
      {return (*Ptr)?(*Ptr):"";};
    #endif

    BOOL IsEmpty(void)
      { return (*Ptr)==NULL; };

    int Length(void)
      { return (*Ptr)?strlen(*Ptr):0;};

    char & operator[](int i){ return (*Ptr)[i]; };

    // Операторы сложения
    rdsbcppString operator+(const rdsbcppString &val) const
      { return rdsbcppString(*Ptr,val.c_str()); };
    rdsbcppString operator+(char *str) const
      { return rdsbcppString(*Ptr,str); };

    // Операторы сложения с присваиванием
    rdsbcppString & operator+=(const rdsbcppString &val)
      { char *aux=rdsDynStrCat(*Ptr,val.c_str(),TRUE);
        if(*Ptr) rdsFree(*Ptr);
        *Ptr=aux;
        return *this;
      };
    rdsbcppString & operator+=(char *str)
      { char *aux=rdsDynStrCat(*Ptr,str,TRUE);
        if(*Ptr) rdsFree(*Ptr);
        *Ptr=aux;
        return *this;
      };

    // Операторы присваивания
    rdsbcppString & operator=(const rdsbcppString &val)
       { if(*Ptr) rdsFree(*Ptr);
         *Ptr=rdsDynStrCat(val.c_str(),NULL,TRUE);
         return *this;
       };
    rdsbcppString & operator=(char *str)
       { if(*Ptr) rdsFree(*Ptr);
         *Ptr=rdsDynStrCat(str,NULL,TRUE);
         return *this;
       };

    // Конструкторы
    rdsbcppString(void){Data=NULL;Ptr=&Data;};
    rdsbcppString(char *str) { Data=rdsDynStrCat(str,NULL,TRUE); Ptr=&Data;};
    rdsbcppString(char *str1,char *str2) { Data=rdsDynStrCat(str1,str2,TRUE); Ptr=&Data;};
    ~rdsbcppString(){if(Data) rdsFree(Data);};
};
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
// Класс-предок глобального объекта для функции блока
//---------------------------------------------------------------------------
class rdsbcppFunction
{ protected:
    int _Id;    // Идентификатор функции в RDS
    int _State; // 0 - глобальный объект,
                // 1 - локальный (в классе блока) объект, не объявлявший свой блок исполнителем функции
                // 2 - локальный объект, объявивший свой блок исполнителем функции
    RDS_PFUNCPROVIDERLINK _Link; // Ссылка на структуру подписки на исполнителя функции

    // Объявление своего блока исполнителем функции
    void _RegisterProvider(void)
      { if(_State==1)
          { rdsRegisterFuncProvider(_Id,FALSE);
            _State=2;
          }
      };
    // Отмена объявления своего блока исполнителем функции
    void _UnregisterProvider(void)
      { if(_State==2) // Убираем регистрацию
          { rdsUnregisterFuncProvider(_Id);
            _State=1;
          }
      };

    // Подписаться на исполнителя функции
    void _SubscribeToProvider(void)
      { if(_Link==NULL && _State!=0) // Запрещено для глобальных объектов
          _Link=rdsSubscribeToFuncProvider(_Id);
      };
    // Отписаться от исполнителя функции
    void _UnsubscribeFromProvider(void)
      { if(_Link)
          { rdsUnsubscribeFromFuncProvider(_Id);
            _Link=NULL;
          }
      };
  public:
    // Идентификатор функции
    inline int Id(void){return _Id;};
    // Регистрация функции
    inline void Register(char *name)
      { _Id=rdsRegisterFunction(name); };

    // Блок успешно зарегисирирован как исполнитель функции
    inline BOOL IsProvider(void){return _State==2;};

    // Блок успешно подписан на исполнителя функции
    inline BOOL Subscribed(void){return _Link!=NULL && _Link->Block!=NULL;};
    // Блок-исполнитель функции
    inline RDS_BHANDLE Provider(void){return _Link?(_Link->Block):NULL;};

    // Очистка объекта
    void Clear(void)
      { _UnregisterProvider(); // Убираем регистрацию
        _UnsubscribeFromProvider(); // Отписываемся от исполнителя функции
      };
    // Инициализация локального по глобальному
    inline void InitLocal(rdsbcppFunction &g){Clear();_Id=g.Id();_State=1;};

    // Конструктор и деструктор
    rdsbcppFunction(void){_Id=_State=0;_Link=NULL;};
    virtual ~rdsbcppFunction(){Clear();};
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Генераторы классов (массивы, матрицы) с проверкой индексов
//---------------------------------------------------------------------------
#ifdef RDSBCPP_INDEXCHECK

#define RDSBCPP_STATICPLAINMATRCLASS(classname,vartype) \
  class classname##_ { public: void *Ptr; int r; LPSTR name; \
    inline vartype & operator[](int c) \
      { static vartype baddata; \
        if(!rdsVarArrayIndexCheck(Ptr,r,c,RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,name,NULL)) \
          { memset(&baddata,0,sizeof(vartype));  return baddata; } \
        return *(((vartype*)(RDS_ARRAYDATA(Ptr)))+r*(RDS_ARRAYCOLS(Ptr))+c); \
      }; \
    }; \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; LPSTR name; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitN(void *base,int offset,LPSTR vn){Ptr=((unsigned char*)base)+offset; Fixed=TRUE; name=vn;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    inline void InitNumN(void *base,int offset,LPSTR vn,int num){InitN(base,offset,vn);_Num=num;}; \
    inline void SetName(LPSTR n){name=n;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline classname##_ operator[](int r) { classname##_ el; el.Ptr=Ptr; el.r=r; el.name=name; return el; }; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYCOLS(Ptr):0;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYROWS(Ptr):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline vartype * GetBuffer(void){return (vartype*)(RDS_ARRAYDATA(Ptr));}; \
    inline BOOL Resize(int rows,int cols,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,rows,cols,keep,NULL); } \
    inline vartype & Item(int r,int c) \
      { static vartype baddata; \
        if(!rdsVarArrayIndexCheck(Ptr,r,c,RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,name,NULL)) \
          { memset(&baddata,0,sizeof(vartype));  return baddata; } \
        return *(GetBuffer()+r*(RDS_ARRAYCOLS(Ptr))+c); }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; name=NULL;}; \
  }

#define RDSBCPP_STATICPLAINARRCLASS(classname,vartype) \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; LPSTR name; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitN(void *base,int offset,LPSTR vn){Ptr=((unsigned char*)base)+offset; Fixed=TRUE; name=vn;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    inline void InitNumN(void *base,int offset,LPSTR vn,int num){InitN(base,offset,vn);_Num=num;}; \
    inline void SetName(LPSTR n){name=n;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?1:0;}; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int size,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,1,size,keep,NULL); } \
    inline vartype * GetBuffer(void){return (vartype*)(RDS_ARRAYDATA(Ptr));}; \
    inline vartype & Item(int n) \
      { static vartype baddata; \
        if(!rdsVarArrayIndexCheck(Ptr,n,1,RDS_VAIC_SINGLE|RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,name,NULL)) \
          { memset(&baddata,0,sizeof(vartype));  return baddata; } \
        return *(GetBuffer()+n); }; \
    inline vartype & operator[](int i){ return Item(i); }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; name=NULL;}; \
  }

#define RDSBCPP_STATICCOMPLMATRCLASS(classname,elementclass) \
  class classname##_ { public: void *Ptr; int r; LPSTR name; \
    inline elementclass operator[](int i) \
      { elementclass el; unsigned char *Base; \
        if(!rdsVarArrayIndexCheck(Ptr,r,i,RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,name,NULL)) \
          return el; \
        Base=((unsigned char*)RDS_ARRAYDATA(Ptr))+r*RDS_ARRAYCOLS(Ptr)*elementclass::DataSize(); \
        el.Init(Base+i*elementclass::DataSize(),0); \
        return el; \
      }; \
    }; \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; LPSTR name; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitN(void *base,int offset,LPSTR vn){Ptr=((unsigned char*)base)+offset; Fixed=TRUE; name=vn;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    inline void InitNumN(void *base,int offset,LPSTR vn,int num){InitN(base,offset,vn);_Num=num;}; \
    inline void SetName(LPSTR n){name=n;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline classname##_ operator[](int i) \
      { classname##_ el; el.Ptr=Ptr; el.r=i; el.name=name;\
        return el; \
      }; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYCOLS(Ptr):0;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?RDS_ARRAYROWS(Ptr):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int rows,int cols,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,rows,cols,keep,NULL); } \
    inline elementclass Item(int r,int c) \
      { classname##_ elc; elc.Ptr=Ptr; elc.r=r; elc.name=name;\
        return elc[c]; \
      }; \
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; name=NULL;}; \
  }

#define RDSBCPP_STATICCOMPLARRCLASS(classname,elementclass) \
  class classname : public rdsbcppVarAncestor { protected: void *Ptr; BOOL Fixed; LPSTR name; public: \
    inline void Init(void *base,int offset){Ptr=((unsigned char*)base)+offset; Fixed=TRUE;}; \
    inline void InitN(void *base,int offset,LPSTR vn){Ptr=((unsigned char*)base)+offset; Fixed=TRUE; name=vn;}; \
    inline void InitNum(void *base,int offset,int num){Init(base,offset);_Num=num;}; \
    inline void InitNumN(void *base,int offset,LPSTR vn,int num){InitN(base,offset,vn);_Num=num;}; \
    inline void SetName(LPSTR n){name=n;}; \
    static inline int DataSize(void){return 2*sizeof(void*);}; \
    inline void * GetPtr(void) const {return Ptr;}; \
    virtual void * GetVoidPtr(void) { return Ptr;}; \
    inline int Rows(void){return RDS_ARRAYEXISTS(Ptr)?1:0;}; \
    inline int Cols(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline int Size(void){return RDS_ARRAYEXISTS(Ptr)?(RDS_ARRAYROWS(Ptr)*RDS_ARRAYCOLS(Ptr)):0;}; \
    inline BOOL IsEmpty(void){return !RDS_ARRAYEXISTS(Ptr);}; \
    inline BOOL HasData(void){return RDS_ARRAYEXISTS(Ptr);}; \
    virtual BOOL GetAccessData(RDS_PARRAYACCESSDATA data){return rdsGetVarArrayAccessData(Ptr,data);}; \
    inline BOOL Resize(int size,BOOL keep=FALSE) \
      { RDS_ARRAYACCESSDATA data; GetAccessData(&data); return rdsResizeVarArray(Ptr,1,size,keep,NULL); } \
    inline elementclass Item(int n) \
      { elementclass el; \
        if(!rdsVarArrayIndexCheck(Ptr,n,1,RDS_VAIC_SINGLE|RDS_VAIC_MESSAGE|RDS_VAIC_SINGLEMSG|RDS_VAIC_STOPCALC,NULL,NULL)) \
          return el; \
        el.Init(RDS_ARRAYDATA(Ptr),n*elementclass::DataSize()); \
        return el; \
      }; \
    inline elementclass operator[](int i) {return Item(i);};\
    classname & operator=(const classname &val) \
      { if(Fixed) \
          { rdsCopyVarArray(Ptr,val.GetPtr()); \
            return *this; \
          } \
        Ptr=val.GetPtr(); \
        return *this; \
      }; \
    classname(void):rdsbcppVarAncestor(){ static void *nullarray[]={NULL,NULL}; Ptr=nullarray; Fixed=FALSE; }; \
  }

#endif

#endif

