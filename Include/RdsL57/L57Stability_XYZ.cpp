//---------------------------------------------------------------------------
// Стандартные части модели поиска равновесия трех функций f(x,y,z)
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
// Имена входов идентификаторов функций
#define L57_FUNCINPUT1 "FuncId1"
#define L57_FUNCINPUT2 "FuncId2"
#define L57_FUNCINPUT3 "FuncId3"

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
TL57TextString str;
char caption[]="Поиск равновесия";
RDS_HOBJECT win;
BOOL res;

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

CheckConnections();

// Читаем список функций из файла
file.CanEditFuncId=L57_FUNCIDEDIT;
file.Setup_ReadFuncFile(rdsbcppBlockData->Block,model!=NULL,L57FUNC_LIST,win);

/*
// Вкладка выбора функциий
rdsFORMAddTab(win,1,"Выбор функций");
file.Setup_AddFuncSelectFields(win,"x=fx(x,y,z)",1,10,SetupFuncId1,FALSE,L57TF_LINE);
file.Setup_AddFuncSelectFields(win,"y=fy(x,y,z)",1,20,SetupFuncId2,FALSE,L57TF_LINE);
file.Setup_AddFuncSelectFields(win,"z=fz(x,y,z)",1,30,SetupFuncId3,FALSE,0);
*/

// Вкладки функций
rdsFORMAddTab(win,10,"Функция 1");
file.Setup_AddFuncSelectFields(win,NULL,10,10,SetupFuncId1,model!=NULL,L57TF_ALLBUTTONS);
rdsFORMAddTab(win,20,"Функция 2");
file.Setup_AddFuncSelectFields(win,NULL,20,20,SetupFuncId2,model!=NULL,L57TF_ALLBUTTONS);
rdsFORMAddTab(win,30,"Функция 3");
file.Setup_AddFuncSelectFields(win,NULL,30,30,SetupFuncId3,model!=NULL,L57TF_ALLBUTTONS);


// Вкладка параметров
rdsFORMAddTab(win,2,"Параметры");
rdsFORMAddEdit(win,2,100,RDS_FORMCTRL_EDIT,"Погрешность:",80);
rdsSetObjectDouble(win,100,RDS_FORMVAL_VALUE,Delta);

rdsFORMAddEdit(win,2,1007,RDS_FORMCTRL_UPDOWN,"Итераций в трубке погрешности:",80);
rdsSetObjectInt(win,1007,RDS_FORMVAL_VALUE,DeltaCycles);
rdsSetObjectInt(win,1007,RDS_FORMVAL_UPDOWNMIN,0);
rdsSetObjectInt(win,1007,RDS_FORMVAL_UPDOWNMAX,30000);
rdsSetObjectInt(win,1007,RDS_FORMVAL_UPDOWNINC,1);

/*
// Вкладка добавления и удаления функциий
rdsFORMAddTab(win,3,"Редактирование функций");
file.Setup_AddFuncSelectFields(win,NULL,3,40,-1,model!=NULL,L57TF_ALLBUTTONS);
*/

res=rdsFORMShowModalServ(win,WinCallback);

if(res)
  { // Ищем Num функции
    SetupFuncId1=file.FuncNumByIndex(rdsGetObjectInt(win,10,RDS_FORMVAL_VALUE),SetupFuncId1);
    SetupFuncId2=file.FuncNumByIndex(rdsGetObjectInt(win,20,RDS_FORMVAL_VALUE),SetupFuncId2);
    SetupFuncId3=file.FuncNumByIndex(rdsGetObjectInt(win,30,RDS_FORMVAL_VALUE),SetupFuncId3);

    // Погрешность
    Delta=rdsGetObjectDouble(win,100,RDS_FORMVAL_VALUE);
    DeltaCycles=rdsGetObjectInt(win,1007,RDS_FORMVAL_VALUE);

    // Сохраняем файл, если он изменился
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
BOOL InputConn1,InputConn2,InputConn3;
// Проверка наличия связи у входов идентификаторов функций
void CheckConnections(void)
{ InputConn1=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_FUNCINPUT1);
  InputConn2=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_FUNCINPUT2);
  InputConn3=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_FUNCINPUT3);
}
#endif // #ifdef L57MS_CLASS
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// Модель
//---------------------------------------------------------------------------
#ifdef L57MS_MODEL
double xn,yn,zn,dx,dy,dz;
BOOL bad=FALSE,id_chg=FALSE;
int id1,id2,id3;

id1=(InputConn1?FuncId1:SetupFuncId1);
id2=(InputConn2?FuncId2:SetupFuncId2);
id3=(InputConn3?FuncId3:SetupFuncId3);

if(id1!=OldFuncId1 || id2!=OldFuncId2 || id3!=OldFuncId3)
  { id_chg=TRUE;
    OldFuncId1=id1;
    OldFuncId2=id2;
    OldFuncId3=id3;
  }

// Сбрасываем готовность
Ready=OutReady=Error=Finished=0;
OK=1;

// Проверка изменения входов (нужно начинать поиск заново)
if(InputChanged||ArrayChanged||func_chg||id_chg)
  { // Сработала входная связь
    BOOL arraychanged=FALSE;
    BOOL FuncsChanged=FALSE;
    if(func_chg) // Изменились функции
      { func_chg=0;
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
          { //test=GetTickCount();
            Arguments.Functions.Realloc(n);
            for(int i=0;i<n;i++)
              Arguments.Functions[i].Setup(&(FS[i]));
          }
      } // if(func_chg)

    if(ArrayChanged)
      { // Проверяем реальное изменение массива
        if(A.Size()!=A_old.Size()) // Изменился размер
          arraychanged=TRUE;
        else // Размер тот же
          for(int i=0;i<A.Size();i++)
            if(A[i]!=A_old[i])
              { arraychanged=TRUE;
                break;
              }
      }
    if(arraychanged)
      A_old=A;
    ArrayChanged=InputChanged=0;
    if(arraychanged || FuncsChanged || x0!=x0_old || y0!=y0_old || z0!=z0_old || id_chg)
      { // Изменились входы - начинаем поиск заново
        ArrayChanged=0;
        xc=x0_old=x0;
        yc=y0_old=y0;
        zc=z0_old=z0;
        CyclesInDelta=CycleCount=0;
        x=y=z=rdsbcppHugeDouble;
        if(x0==rdsbcppHugeDouble || y0==rdsbcppHugeDouble ||
           z0==rdsbcppHugeDouble || Delta==rdsbcppHugeDouble)
          { // Недопустимые данные
            OutReady=Error=Finished=1;
            OK=0;
            return;
          }
        Start=1; // Перезапускаемся
        Ready=1; // Чтобы можно было смотреть число итераций
        return;
      }
  }

// Параметры
if(A.IsEmpty())
  Arguments.Params.Setup(NULL);
else
  Arguments.Params.Setup(A.GetBuffer(),A.Size());

// Значения следующего шага
CycleCount++;
Arguments.xval=xc;
Arguments.yval=yc;
Arguments.zval=zc;
xn=SelectedFunc(id1,Arguments);

Arguments.xval=xc;
Arguments.yval=yc;
Arguments.zval=zc;
yn=SelectedFunc(id2,Arguments);

Arguments.xval=xc;
Arguments.yval=yc;
Arguments.zval=zc;
zn=SelectedFunc(id3,Arguments);

if(xn==rdsbcppHugeDouble || yn==rdsbcppHugeDouble || zn==rdsbcppHugeDouble)
  bad=TRUE; // Какая-то внутренняя беда в функции
else
  { // Погрешность
    RDSBCPP_TRY
      { dx=fabs(xn-xc);
        dy=fabs(yn-yc);
        dz=fabs(zn-zc);
      }
    RDSBCPP_CATCHALL
      { bad=TRUE; }
  }

if(bad)
  { x=y=z=rdsbcppHugeDouble;
    OutReady=Error=Finished=1;
    OK=0;
    return;
  }

if(dx<Delta && dy<Delta && dz<Delta)
  CyclesInDelta++;
else
  CyclesInDelta=0;

if(CyclesInDelta>DeltaCycles)
  { // Больше DeltaCycles тактов значения практически не менялись
    x=xn;
    y=yn;
    z=zn;
    OutReady=Ready=Finished=1;
    OK=1;
    //Start=1; // Только при отладке!
    return;
  }

// Перезапускаемся (следующий такт)
Start=1;
xc=xn;
yc=yn;
zc=zn;
Ready=1; // Чтобы можно было смотреть число итераций
#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// Режим моделирования
//---------------------------------------------------------------------------
#ifdef L57MS_CALCMODE
CheckConnections();
#endif // #ifdef L57MS_CALCMODE
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// Запуск расчета
//---------------------------------------------------------------------------
#ifdef L57MS_START
Start=1;
#endif // #ifdef L57MS_START
//-----------------------------------------------------------------------









