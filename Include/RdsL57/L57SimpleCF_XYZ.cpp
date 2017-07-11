//---------------------------------------------------------------------------
// Стандартные части модели простого вычисления функции
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
#define L57_FUNCINPUT "FuncId"

// Функция обратного вызова окна настройки
void RDSCALL WinCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data)
{ TL57TextFile *file=(TL57TextFile*)rdsGetObjectPtr(win,0,NULL);
  if(file)
    file->Setup_WindowCallback(win,data);
}
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
#endif // #ifdef L57MS_GLOBALS
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// Настройки
//---------------------------------------------------------------------------
#ifdef L57MS_SETUP
TL57TextFile file;
RDS_HOBJECT win;
BOOL res;
char caption[]="Простое вычисление функции";

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

rdsFORMAddTab(win,1,"Функция");
file.Setup_AddFuncSelectFields(win,NULL,1,1,SetupFuncId,model!=NULL,L57TF_ALLBUTTONS,
    InputConn?L57_FUNCINPUT:NULL);
res=rdsFORMShowModalServ(win,WinCallback);

if(res)
  { // Ищем Num функции
    SetupFuncId=file.FuncNumByIndex(rdsGetObjectInt(win,1,RDS_FORMVAL_VALUE),SetupFuncId);

    // Сохраняем файл, если он изменился
    if(model!=NULL)
      file.Setup_WriteFuncFile(rdsbcppBlockData->Block,L57_BAK);
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

// Проверка наличия связи у входа идентификатора функции
void CheckConnection(void)
{ InputConn=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_FUNCINPUT); }
#endif // #ifdef L57MS_CLASS
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// Модель
//---------------------------------------------------------------------------
#ifdef L57MS_MODEL
// Идентификатор функции
int id;
if(InputConn)
  id=FuncId;
else
  id=SetupFuncId;

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
      { test=GetTickCount();
        Arguments.Functions.Realloc(n);
        for(int i=0;i<n;i++)
          Arguments.Functions[i].Setup(&(FS[i]));
      }
  }

// Параметры
if(A.IsEmpty())
  Arguments.Params.Setup(NULL);
else
  Arguments.Params.Setup(A.GetBuffer(),A.Size());

Arguments.xval=x;
Arguments.yval=y;
Arguments.zval=z;

f=SelectedFunc(id,Arguments);

#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// Режим моделирования
//---------------------------------------------------------------------------
#ifdef L57MS_CALCMODE
CheckConnection();
#endif // #ifdef L57MS_CALCMODE
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// Запуск расчета
//---------------------------------------------------------------------------
#ifdef L57MS_START
Start=1;
#endif // #ifdef L57MS_START
//-----------------------------------------------------------------------









