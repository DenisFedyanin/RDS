#define MULTITABLE_NOVCL
#define MTFL_TABLE
#define __fastcall
#define bool BOOL
#include "MultiTableFileLibrary.cpp"
#include "MultiTable.cpp"
#include "FileLibrary.cpp"
#include "MultiTableSaver.cpp"
#undef __fastcall
#undef bool
//---------------------------------------------------------------------------

class TMultiInterpolator
{ private:
    rdsbcppString LastLoadedFile; // Последний загруженный файл
    FILETIME LastWrite; // Время последней записи в связанный файл

    BOOL Error; // Признак ошибки

    double *LastResult; // Результат последней интерполяции
    int LastResultCount; // Размер массива результатов

    // Очистить последний результат
    void ClearResult(void);
  public:
    TMultiTable Table;  // Интерполяционная таблица

    // Загрузить таблицу из FileName
    BOOL LoadTable(char *FileName,BOOL check=TRUE);
    // Получить ошибку
    inline BOOL GetError(void){return Error;};

    // Установить аргумент с заданным номером
    void SetArg(int i,double x);

    // Получить значение с заданным номером
    inline double GetVal(int i)
      { return (i<LastResultCount)?LastResult[i]:0.0;};

    // Интерполировать (аргументы уже загружены в Table)
    BOOL Interpolate(void);
    // Интерполировать (4 аргумента)
    BOOL Interpolate(double x0,double x1,double x2,double x3);


    TMultiInterpolator(void){Error=TRUE; LastResult=NULL; LastResultCount=0;};
    ~TMultiInterpolator();
};
//---------------------------------------------------------------------------

TMultiInterpolator::~TMultiInterpolator()
{
  if(LastResult)
    delete[] LastResult;
}
//---------------------------------------------------------------------------

// Очистить последний результат
void TMultiInterpolator::ClearResult(void)
{
  if(LastResult)
    memset(LastResult,0,sizeof(double)*LastResultCount);
}
//---------------------------------------------------------------------------

// Загрузить таблицу из FileName
// check - проводить проверку необходимости загрузки
BOOL TMultiInterpolator::LoadTable(char *FileName,BOOL check)
{ BOOL ok;
  char *dynname;
  TFileLibrary lib;
  FILETIME ft;

  if(FileName==NULL || FileName[0]==0)
    { LastLoadedFile="";
      Error=TRUE;
      return FALSE;
    }

  if(LastLoadedFile.IsEmpty())
    check=FALSE;
  if(Table.IsEmpty())
    check=FALSE;

  // Добавляем путь в имя файла
  dynname=rdsGetFullFilePath(FileName,NULL,NULL);
  if(!dynname)
    { LastLoadedFile="";
      Error=TRUE;
      return FALSE;
    }

  // Открываем файл
  if(!mtflLoadLibrary_Begin(&lib,dynname,&ft))
    { LastLoadedFile="";
      rdsFree(dynname);
      Error=TRUE;
      return FALSE;
    }
  rdsFree(dynname);

  if(check)
    { // Если время и имя файла не изменились, перезагрузка не нужна
      if(strcmp(LastLoadedFile.c_str(),FileName)==0 && CompareFileTime(&LastWrite,&ft)>=0)
        return TRUE;
    }
  // Загрузка необходима
  LastLoadedFile=FileName;
  memcpy(&LastWrite,&ft,sizeof(FILETIME));

  if(mtflLoadLibrary_HasResultTable(&lib))
    { ok=mtflLoadLibrary_End(&lib,&Table,NULL);
      if(ok)
        Error=Table.IsEmpty();
      else
        Error=FALSE;
    }
  else
    { ok=FALSE;
      Error=TRUE;
    }
  Table.ResetInterpolation();
  return ok;
}
//---------------------------------------------------------------------------

// Интерполировать (аргументы уже загружены в Table)
BOOL TMultiInterpolator::Interpolate(void)
{ double *result=Table.Interpolate();
  int vc;
  if(!result)
    { ClearResult();
      return FALSE;
    }
  vc=Table.GetValuesCount();
  if(vc>LastResultCount)
    { if(LastResult)
        delete[] LastResult;
      LastResult=new double[LastResultCount=vc];
    }
  memcpy(LastResult,result,vc*sizeof(double));
  return TRUE;
}
//---------------------------------------------------------------------------

// Установить аргумент с заданным номером
void TMultiInterpolator::SetArg(int i,double x)
{ if(i<Table.GetDimension())
    Table.SetArgumentFAST(i,x);
}
//---------------------------------------------------------------------------


// Интерполировать (4 аргумента)
BOOL TMultiInterpolator::Interpolate(double x0,double x1,double x2,double x3)
{ int d=Table.GetDimension();
  if(d<1)
    return FALSE;
  Table.SetArgumentFAST(0,x0);
  if(d>1)
    { Table.SetArgumentFAST(1,x1);
      if(d>2)
       { Table.SetArgumentFAST(2,x2);
         if(d>3)
           Table.SetArgumentFAST(3,x3);
       }
    }
  for(int i=4;i<d;i++)
    Table.SetArgumentFAST(i,0.0);
  return Interpolate();
}
//---------------------------------------------------------------------------

