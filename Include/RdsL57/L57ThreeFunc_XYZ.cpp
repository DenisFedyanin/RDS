//---------------------------------------------------------------------------
// Стандартные части модели построения трех табличных функций
// одной переменной из функции трех переменных f(x,y,z)
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
//-----------------------------------------------------------------------

// Функция обратного вызова окна настройки
void RDSCALL WinCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data)
{ TL57TextFile *file=(TL57TextFile*)rdsGetObjectPtr(win,0,NULL);
  if(file)
    file->Setup_WindowCallback(win,data);
}
//---------------------------------------------------

// Заполнить поля ввода для диапазона и значения переменной
void SetupVar(RDS_HOBJECT win,int tab,int basefield,char *var,
               double v0,double vmin,double vmax,double dv,
               BOOL in_v0,BOOL in_vdiap,BOOL in_dv)
{ int id=basefield;
  TL57TextString str;

  rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_MULTILABEL|RDS_FORMFLAG_LINE,
                 "Следующие параметры можно зафиксировать в "
                 "настройках блока, а не читать с его входов:",0);
  rdsSetObjectInt(win,id,RDS_FORMVAL_MLHEIGHT,34);
  rdsSetObjectInt(win,id,RDS_FORMVAL_MLABELBEVEL,0);

  id++;
  str.Assign("Фиксировать значение ");
  str.AddString(var);
  str.AddString("0:");
  rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_EDIT|RDS_FORMFLAG_CHECK,
                 str.c_str(),80);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_VALUE,v0);
  rdsSetObjectInt(win,id,RDS_FORMVAL_CHECK,!in_v0);

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
  rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_EDIT|RDS_FORMFLAG_CHECK,
                 str.c_str(),80);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_VALUE,dv);
  rdsSetObjectInt(win,id,RDS_FORMVAL_CHECK,!in_dv);
}
//---------------------------------------------------

// Считать поля ввода для диапазона и значения переменной
void ReadSetupVar(RDS_HOBJECT win,int basefield,
                  double &v0,double &vmin,double &vmax,double &dv,
                  BOOL &in_v0,BOOL &in_vdiap,BOOL &in_dv)
{ int id=basefield+1;

  v0=rdsGetObjectDouble(win,id,RDS_FORMVAL_VALUE);
  in_v0=(rdsGetObjectInt(win,id,RDS_FORMVAL_CHECK)==0);

  id++;
  vmin=rdsGetObjectDouble(win,id,RDS_FORMVAL_VALUE);
  vmax=rdsGetObjectDouble(win,id,RDS_FORMVAL_RANGEMAX);
  in_vdiap=(rdsGetObjectInt(win,id,RDS_FORMVAL_CHECK)==0);

  id++;
  dv=rdsGetObjectDouble(win,id,RDS_FORMVAL_VALUE);
  in_dv=(rdsGetObjectInt(win,id,RDS_FORMVAL_CHECK)==0);
}
//---------------------------------------------------

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
char caption[]="Вычисление трех функций";

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
file.Setup_AddFuncSelectFields(win,NULL,1,1,SetupFuncId,model!=NULL,L57TF_ALLBUTTONS,
  InputConn?L57_FUNCINPUT:NULL);

// Вкладка X
rdsFORMAddTab(win,3," X");
SetupVar(win,3,100,"x",setup_x0,setup_xmin,setup_xmax,setup_dx,
         in_x0,in_xdiap,in_dx);

// Вкладка Y
rdsFORMAddTab(win,4," Y");
SetupVar(win,4,200,"y",setup_y0,setup_ymin,setup_ymax,setup_dy,
         in_y0,in_ydiap,in_dy);

// Вкладка Z
rdsFORMAddTab(win,5," Z");
SetupVar(win,5,300,"z",setup_z0,setup_zmin,setup_zmax,setup_dz,
         in_z0,in_zdiap,in_dz);

// Открываем окно
res=rdsFORMShowModalServ(win,WinCallback);

if(res)
  { // Ищем Num функции
    SetupFuncId=file.FuncNumByIndex(rdsGetObjectInt(win,1,RDS_FORMVAL_VALUE),SetupFuncId);

    // Сохраняем файл, если он изменился
    if(model!=NULL)
      file.Setup_WriteFuncFile(rdsbcppBlockData->Block,L57_BAK);

     // Вкладка X
     ReadSetupVar(win,100,setup_x0,setup_xmin,setup_xmax,setup_dx,
                  in_x0,in_xdiap,in_dx);
     // Вкладка Y
     ReadSetupVar(win,200,setup_y0,setup_ymin,setup_ymax,setup_dy,
                  in_y0,in_ydiap,in_dy);
     // Вкладка Z
     ReadSetupVar(win,300,setup_z0,setup_zmin,setup_zmax,setup_dz,
                  in_z0,in_zdiap,in_dz);
     // Взводим сигналы для пересчета всего
     x0_chg=y0_chg=z0_chg=1;
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

// Проверка наличия связи у входа идентификатора функции
void CheckConnection(void)
{ InputConn=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_FUNCINPUT); }
//-----------------------------------------------------------------------
// Вычислить выходную матрицу
void CalcOutput(int fid,RDSBCPP_VT_FX &M,
                int varnum,
                double vmin,double vmax,double dv,
                TL57TextFuncArg &arg);
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
// Вычислить выходную матрицу
void rdsbcppBlockClass::CalcOutput(int fid,RDSBCPP_VT_FX &M,
                                   int varnum,
                                   double vmin,double vmax,double dv,
                                   TL57TextFuncArg &arg)
{ BOOL bad;
  double _x0,_y0,_z0; 
  int n;
  // Нулевая точка с входа или из настроек
  _x0=in_x0?x0:setup_x0;
  _y0=in_y0?y0:setup_y0;
  _z0=in_z0?z0:setup_z0;

  // Проверки
  if(vmin==rdsbcppHugeDouble || vmax==rdsbcppHugeDouble || 
     dv==rdsbcppHugeDouble || dv<=0.0 || vmax<=vmin)
    bad=TRUE;
  else switch(varnum)
    { case 0: // x
        bad=(_y0==rdsbcppHugeDouble || _z0==rdsbcppHugeDouble);
        break;
      case 1: // y  
        bad=(_x0==rdsbcppHugeDouble || _z0==rdsbcppHugeDouble);
        break;
      case 2: // z
        bad=(_x0==rdsbcppHugeDouble || _y0==rdsbcppHugeDouble);
        break;
      default:
        bad=TRUE;
    }
  if(!bad)
    { // Переменные имеют допустимые значения
      RDSBCPP_TRY
        { double d=(vmax-vmin)/dv;
          n=1+(int)(d); 
          bad=n<2;
        }
      RDSBCPP_CATCHALL
        { bad=TRUE; }
    }
  if(bad)
    { M.Resize(0,0,FALSE);
      return;
    }
  else
    { // Проверяем последнюю точку
      double last=vmin+(n-1)*dv;
      if(vmax-last>0.1*dv)
        n++;
    }
  // Можно вычислять
  if(!M.Resize(n,2,FALSE))
    { // Не удалось отвести матрицу
      rdsStopCalc();
      M.Resize(0,0,FALSE);
      rdsMessageBox("Выбраны слишком маленькие шаги по одной или нескольким переменным, не хватает памяти для "
                    "размещения матриц табличных функций такого размера.",
                    rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
      return;
    }
  for(int i=0;i<n;i++)
    { double v,f;
      v=(i==(n-1)?vmax:(vmin+i*dv));
      switch(varnum)
        { case 0: // x
            arg.xval=v; arg.yval=_y0; arg.zval=_z0;
            f=SelectedFunc(fid,arg/*v,_y0,_z0*/);
            break;
          case 1: // y  
            arg.xval=_x0; arg.yval=v; arg.zval=_z0;
            f=SelectedFunc(fid,arg/*_x0,v,_z0*/);
            break;
          case 2: // z
            arg.xval=_x0; arg.yval=_y0; arg.zval=v;
            f=SelectedFunc(fid,arg/*_x0,_y0,v*/);
            break;
        }
      M[i][0]=v;
      M[i][1]=f;
    }
}
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
BOOL arraychanged=FALSE;

// Идентификатор функции
int id;
if(InputConn)
  id=FuncId;
else
  id=SetupFuncId;
if(id!=OldFuncId)
  { OldFuncId=id;
    arraychanged=TRUE;
  }

// Исходно запрещаем работу всех выходов
FX_en=FY_en=FZ_en=0;
Ready=0;

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
      { //test=GetTickCount();
        arraychanged=TRUE;
        Arguments.Functions.Realloc(n);
        for(int i=0;i<n;i++)
          Arguments.Functions[i].Setup(&(FS[i]));
      }
  }

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
    if(arraychanged)
      A_old=A;
  }

// Параметры
if(A.IsEmpty())
  Arguments.Params.Setup(NULL);
else
  Arguments.Params.Setup(A.GetBuffer(),A.Size());

// Вычисление FX
if(y0_chg || z0_chg || xdiap_chg || arraychanged)
  { double vmin,vmax,dv;
    if(in_xdiap) { vmin=xmin; vmax=xmax; }
    else { vmin=setup_xmin; vmax=setup_xmax;}
    dv=in_dx?dx:setup_dx;
    if(arraychanged||vmin!=old_xmin || vmax!=old_xmax || dv!=old_dx || old_y0!=y0 || old_z0!=z0)
      { CalcOutput(id,FX,0,vmin,vmax,dv,Arguments);
        old_xmin=vmin; old_xmax=vmax; old_dx=dv;
        FX_en=1;
        Ready=1;
      }
  }

// Вычисление FY
if(x0_chg || z0_chg || ydiap_chg || arraychanged)
  { double vmin,vmax,dv;
    if(in_ydiap) { vmin=ymin; vmax=ymax; }
    else { vmin=setup_ymin; vmax=setup_ymax;}
    dv=in_dy?dy:setup_dy;
    if(arraychanged||vmin!=old_ymin || vmax!=old_ymax || dv!=old_dy || old_x0!=x0 || old_z0!=z0)
      { CalcOutput(id,FY,1,vmin,vmax,dv,Arguments);
        old_ymin=vmin; old_ymax=vmax; old_dy=dv;
        FY_en=1;
        Ready=1;
      }
  }

// Вычисление FZ
if(x0_chg || y0_chg || zdiap_chg || arraychanged)
  { double vmin,vmax,dv;
    if(in_zdiap) { vmin=zmin; vmax=zmax; }
    else { vmin=setup_zmin; vmax=setup_zmax;}
    dv=in_dz?dz:setup_dz;
    if(arraychanged||vmin!=old_zmin || vmax!=old_zmax || dv!=old_dz || old_x0!=x0 || old_y0!=y0)
      { CalcOutput(id,FZ,2,vmin,vmax,dv,Arguments);
        old_zmin=vmin; old_zmax=vmax; old_dz=dv;
        FZ_en=1;
        Ready=1;
      }
  }

old_x0=x0; old_y0=y0; old_z0=z0;

// Сбрасываем сигналы срабатывания связей
x0_chg=y0_chg=z0_chg=0;
xdiap_chg=ydiap_chg=zdiap_chg=0;
ArrayChanged=0;

#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------
