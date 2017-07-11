//---------------------------------------------------------------------------
// Стандартные части модели ограничения для внешнего максимизатора
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Глобальные описания
//---------------------------------------------------------------------------
#ifdef L57MS_GLOBALS
#include <RdsL57/L57TextBasic.cpp>

#define L57_HELPUIFILE "Blocks\\L57.ui"
#define L57_HELPTOPIC  "L57_EcoMaxLim"

// Признак наличия файла справки
int L57_HelpState=0;

class TL57TextFileEML : public TL57TextFileBasic
{ public:
    TL57TextString InputCode,OutputCode;

    // Разобрать скрытую секцию
    void ParseHiddenSection(TL57TextFileLine *line);
    // Записать скрытую секцию
    BOOL WriteHiddenSection(void);
    // Записать какую-то секцию и вернуть маркер ее конца
    BOOL WriteSection(int id,int *pendid);

    TL57TextFileEML(void):TL57TextFileBasic(){};
    virtual ~TL57TextFileEML(){};
};
//---------------------------------------------------------------------------

// Разобрать скрытую секцию
void TL57TextFileEML::ParseHiddenSection(TL57TextFileLine *line)
{ char *aux;

  InputCode.Clear();
  OutputCode.Clear();

  // Читаем функции
  for(TL57TextFileLine *l=line;l!=NULL;l=l->Next)
    { char *s=l->Line;
      if(!s)
        continue;
      if((*s)=='$') // Конец данных (начало следующей секции)
        break;
      switch(*s)
        { case '1': // Ограничения по входу
            aux=rdsProcessText(s+1,RDS_PT_STRINGTOTEXT,NULL);
            if(aux)
              { if(InputCode.IsNotEmpty())
                  InputCode.AddString("\n");
                InputCode.AddString(aux);
                rdsFree(aux);
              }
            break;
          case '2': // Ограничения по выходу
            aux=rdsProcessText(s+1,RDS_PT_STRINGTOTEXT,NULL);
            if(aux)
              { if(OutputCode.IsNotEmpty())
                  OutputCode.AddString("\n");
                OutputCode.AddString(aux);
                rdsFree(aux);
              }
            break;
        }

    }

}
//---------------------------------------------------------------------------

// Записать скрытую секцию
BOOL TL57TextFileEML::WriteHiddenSection(void)
{ TL57TextFileBasic splitter;
  BOOL ok=TRUE;
  char *aux;

  splitter.SplitText(InputCode.c_str());
  for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
    if(l->Line)
      { aux=rdsProcessText(l->Line,RDS_PT_TEXTTOSTRINGSL,NULL);
        if(aux)
          { ok=ok && WriteLine("1");
            ok=ok && WriteLine(aux,1);
            rdsFree(aux);
          }
      }

  splitter.SplitText(OutputCode.c_str());
  for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
    if(l->Line)
      { aux=rdsProcessText(l->Line,RDS_PT_TEXTTOSTRINGSL,NULL);
        if(aux)
          { ok=ok && WriteLine("2");
            ok=ok && WriteLine(aux,1);
            rdsFree(aux);
          }
      }

  return ok;
}
//---------------------------------------------------------------------------

// Записать какую-то секцию и вернуть маркер ее конца
BOOL TL57TextFileEML::WriteSection(int id,int *pendid)
{ TL57TextFileBasic splitter;
  BOOL ok=TRUE;

  switch(id)
    { case 1: // Начало функции ограничения по входам
        *pendid=2;
        splitter.SplitText(InputCode.c_str());
        for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
          if(l->Line)
            { ok=ok && WriteLine("  ");
              ok=ok && WriteLine(l->Line,1);
            }
        ok=ok && WriteLine("  ; /* */",1);
        break;

      case 3: // Начало функции ограничения по выходам
        *pendid=4;
        splitter.SplitText(OutputCode.c_str());
        for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
          if(l->Line)
            { ok=ok && WriteLine("  ");
              ok=ok && WriteLine(l->Line,1);
            }
        ok=ok && WriteLine("  ; /* */",1);
        break;
    }
  return ok;
}
//---------------------------------------------------------------------------

// Единый аргумент функции
class TL57TextEMLFuncArg
{ public:
    double _Crit;
    TL57TextDoubleArray X_Params;
    TL57TextDoubleArray A_Params;
    TL57TextFuncFArray Functions;

    TL57TextEMLFuncArg(void){_Crit=rdsbcppHugeDouble;};
};
//---------------------------------------------------------------------------

// Прототип функции вычисления входного ограничения - служебная (без перехвата исключений)
int _InFunc(const TL57TextEMLFuncArg &arguments);
// Прототип функции вычисления выходного ограничения - служебная (без перехвата исключений)
int _OutFunc(const TL57TextEMLFuncArg &arguments);
//-----------------------------------------------------------------------

// Функция вычисления входного ограничения с перехватом исключений
int InFunc(const TL57TextEMLFuncArg &arguments)
{ int ok=1;
  RDSBCPP_TRY
    { ok=_InFunc(arguments); }
  RDSBCPP_CATCHALL
    { ok=0; }
  return ok;
}
//-----------------------------------------------------------------------

// Функция вычисления выходного ограничения с перехватом исключений
int OutFunc(const TL57TextEMLFuncArg &arguments)
{ int ok=1;
  RDSBCPP_TRY
    { ok=_OutFunc(arguments); }
  RDSBCPP_CATCHALL
    { ok=0; }
  return ok;
}
//-----------------------------------------------------------------------

#endif // #ifdef L57MS_GLOBALS
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// Настройки
//---------------------------------------------------------------------------
#ifdef L57MS_SETUP
TL57TextFileEML file;
RDS_HOBJECT win;
BOOL res;
char caption[]="Ограничения для внешнего максимизатора";
TL57TextFileBasic_ServMarker markers[]=
  {{"L57_ECOMAXLIM_INPUT_BEGIN_MARKER", 1,-1},
   {"L57_ECOMAXLIM_INPUT_END_MARKER",   2,-1},
   {"L57_ECOMAXLIM_OUTPUT_BEGIN_MARKER",3,-1},
   {"L57_ECOMAXLIM_OUTPUT_END_MARKER",  4,-1},
   {NULL,0,-1}
  };

RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

if(model==NULL)
  { rdsMessageBox("Для блока отключена автоматическая компиляция модели. Изменение функций ограничений при "
                  "этом невозможно.",caption,MB_OK|MB_ICONWARNING);
    return;
  }

if(rdscompIsEditorOpen(model->Model))
  { rdsMessageBox("Для настройки этого блока необходимо закрыть редактор его модели. "
                  "Пожалуйста, закройте редактор.",caption,MB_OK|MB_ICONWARNING);
    rdscompOpenBlockModelEditor(rdsbcppBlockData->Block);
    return;
  }

win=rdsFORMCreate(TRUE,600,-1,caption);
if(!win)
  return;

#ifdef L57_HELPUIFILE
  rdsFORMHelpButton(win,RDS_FORMHELP_UI|RDS_FORMHELP_CHECK,L57_HELPUIFILE,L57_HELPTOPIC);
#endif

// Чтение функций
file.ReadAndParseModel(rdsbcppBlockData->Block,markers,TRUE);

rdsFORMAddTab(win,1,"По входу");
rdsFORMAddEdit(win,1,1,RDS_FORMCTRL_MULTILINE,
  "Исходный текст функции ограничения:",80);
rdsSetObjectInt(win,1,RDS_FORMVAL_MLRETURNS,1);
rdsFORMSetMultilineHeight(win,1,4*24);
rdsSetObjectStr(win,1,RDS_FORMVAL_VALUE,file.InputCode.c_str());

rdsFORMAddEdit(win,1,100,RDS_FORMCTRL_MULTILABEL,
"Необходимо ввести оператор(ы) проверки допустимости входных параметров в синтаксисе языка C. "
"Входные параметры - элементы массива \"X[...]\". Если сочетание параметров допустимо, нужно либо "
"присвоить ненулевое значение переменной \"ok\", либо вернуть ненулевое значение оператором return. "
"Для недопустимого сочетания параметров присваивается или возвращается ноль.",0);
rdsSetObjectInt(win,100,RDS_FORMVAL_MLHEIGHT,100);
rdsSetObjectInt(win,100,RDS_FORMVAL_MLABELBEVEL,1);

rdsFORMAddTab(win,2,"По выходу");
rdsFORMAddEdit(win,2,2,RDS_FORMCTRL_MULTILINE,
  "Исходный текст функции ограничения:",80);
rdsSetObjectInt(win,2,RDS_FORMVAL_MLRETURNS,1);
rdsFORMSetMultilineHeight(win,2,4*24);
rdsSetObjectStr(win,2,RDS_FORMVAL_VALUE,file.OutputCode.c_str());

rdsFORMAddEdit(win,2,200,RDS_FORMCTRL_MULTILABEL,
"Необходимо ввести оператор(ы) проверки допустимости значения максимизируемой величины \"Crit\" в синтаксисе языка C. "
"Если значение допустимо, нужно либо "
"присвоить ненулевое значение переменной \"ok\", либо вернуть ненулевое значение оператором return. "
"Для недопустимого значения присваивается или возвращается ноль.",0);
rdsSetObjectInt(win,200,RDS_FORMVAL_MLHEIGHT,100);
rdsSetObjectInt(win,200,RDS_FORMVAL_MLABELBEVEL,1);

res=rdsFORMShowModalServ(win,NULL);

if(res)
  { char *in_code=rdsGetObjectStr(win,1,RDS_FORMVAL_VALUE);
    char *out_code=rdsGetObjectStr(win,2,RDS_FORMVAL_VALUE);
    file.InputCode.Assign(in_code);
    file.OutputCode.Assign(out_code);
    file.SaveFileToModel(rdsbcppBlockData->Block,L57_BAK);
  }

rdsDeleteObject(win);
Result=res?1:0;

#endif // #ifdef L57MS_SETUP
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// Описания в классе
//---------------------------------------------------------------------------
#ifdef L57MS_CLASS
TL57TextEMLFuncArg Arguments;

#endif // #ifdef L57MS_CLASS
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// Модель
//---------------------------------------------------------------------------
#ifdef L57MS_MODEL
// Исходно запрещаем работу всех выходов
Ready=Crit_Out_Ready=crit_out_en=0;
y_enabled=0;
Crit_Out=rdsbcppHugeDouble; // Для отладки

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
        Arguments.Functions.Realloc(n);
        for(int i=0;i<n;i++)
          Arguments.Functions[i].Setup(&(FS[i]));
      }
  }

// Параметры
if(A.IsEmpty())
  Arguments.A_Params.Setup(NULL);
else
  Arguments.A_Params.Setup(A.GetBuffer(),A.Size());

if(X.IsEmpty())
  Arguments.X_Params.Setup(NULL);
else
  Arguments.X_Params.Setup(X.GetBuffer(),X.Size());

// Параметры
if(x_chg)
  { // Поступили новые параметры
    x_chg=0;
    Arguments._Crit=rdsbcppHugeDouble;
    if(InFunc(Arguments))
      { // OK
        Y=X;
        y_enabled=1;
        Ready=1;
      }
    else
      { // Плохие аргументы
        Crit_Out=rdsbcppHugeDouble;
        Crit_Out_Ready=crit_out_en=1;
        Ready=1;
      }
    /*
    if(CritInReady)
      { static int nn=10;
        nn--;
        if(!nn)
          rdsStopCalc();
        rdsMessageBox("InReady","",MB_OK);
      }
    */
    Crit_In_Ready=crit_in_chg=0;
    return;
  }

if(Crit_In_Ready)
  { // Поступил новый критерий
    Crit_In_Ready=crit_in_chg=0;
    Arguments._Crit=Crit_In;
    if(OutFunc(Arguments)) // OK
      Crit_Out=Crit_In;
    else // Ошибка
      Crit_Out=rdsbcppHugeDouble;
    Crit_Out_Ready=crit_out_en=1;
    Ready=1;
  }

#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------



//---------------------------------------------------------------------------
// Режим моделирования
//---------------------------------------------------------------------------
#ifdef L57MS_CALCMODE

#endif // #ifdef L57MS_CALCMODE
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// Запуск расчета
//---------------------------------------------------------------------------
#ifdef L57MS_START

#endif // #ifdef L57MS_START
//-----------------------------------------------------------------------

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

  #ifdef L57_HELPUIFILE
    switch(CallMode)
      { case RDS_BFM_HELP:
          if(L57_HelpUIFileExists(L57_HELPUIFILE,&L57_HelpState))
            { rdsShowHelp(L57_HELPUIFILE,L57_HELPTOPIC,FALSE);
              Result=1;
            }
          break;
      }
  #endif

#endif // #ifdef L57MS_OTHER
//---------------------------------------------------------------------------

