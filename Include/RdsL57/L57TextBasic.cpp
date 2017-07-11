//---------------------------------------------------------------------------
// Дополнительные классы и функции для поддержки полуавтокомпилируемых блоков
// (общие функции работы с файлами, массивами и табличными функциями)
//---------------------------------------------------------------------------
#ifndef RdsLinInterH
  #include <RdsAC/LinInter.cpp>
#endif

// Предопределенные служебные метки строк
#define LINEID_NONE             0       // Нет метки
#define LINEID_HIDDEN           -1      // Строка "$HIDDEN", начинающая скрытую секцию
#define LINEID_OPTIONS          -2      // Строка "$OPTIONS", начинающая секцию опций модели
//---------------------------------------------------------------------------
// Элемент массива служебных меток
struct TL57TextFileBasic_ServMarker
{ char *Text;
  int Id;
  int TextLen;
};
//---------------------------------------------------------------------------

// Массив double (ссылка на внешние данные)
class TL57TextDoubleArray
{ private:
    double *Array;
    int Count;
    double DefValue;
  public:
    inline void Setup(double *a,int cnt=0,double def=0.0){Array=a;Count=cnt;DefValue=def;};
    inline double operator[](int i)const{ return (Array!=NULL && i>=0 && i<Count)?Array[i]:DefValue; };
    inline double operator()(int i)const{ return (Array!=NULL && i>=0 && i<Count)?Array[i]:DefValue; };

    TL57TextDoubleArray(void){Array=NULL;Count=0;};
};
//---------------------------------------------------------------------------
// Служебный объект-ссылка
class TL57TextFuncFTableLink
{ public:
    TRdsLinearInterpolation *Engine; // Ссылка на интерполятор
    BOOL ForceYIndex;                // Принудительный индекс значения для функции одной переменной
    int YIndex;                      // Индекс значения для функции одной переменной (при ForceYIndex==TRUE)

    // Настройка
    inline void Setup(rdsbcppVarAncestor *M,int flags=RDSLININTER_T_UNKNOWN){Engine->Setup(*M,flags);};
    // Тип функции
    inline int Type(void){return Engine->GetType();};

    // Функция двух переменных (для функции одной x2 - индекс значения в матрице)
    inline double operator()(double x1,double x2){ return Engine->CalcGeneral(x1,x2); };
    // Функция одной переменной
    inline double operator()(double x){ return ForceYIndex?Engine->Calc_Fx(x,0,YIndex):Engine->Calc_Fx(x); };
    // Функция одной переменной с указанием конкретного индекса значения
    inline TL57TextFuncFTableLink operator[](int i){ return TL57TextFuncFTableLink(Engine,i); };

    TL57TextFuncFTableLink(TRdsLinearInterpolation *p){Engine=p;ForceYIndex=FALSE;};
    TL57TextFuncFTableLink(TRdsLinearInterpolation *p,int yindex){Engine=p;ForceYIndex=TRUE;YIndex=yindex;};
    TL57TextFuncFTableLink(const TL57TextFuncFTableLink &p){Engine=p.Engine;ForceYIndex=p.ForceYIndex;YIndex=p.YIndex;};
};
//---------------------------------------------------------------------------
// Массив таблиц кусочно-линейных функций
class TL57TextFuncFArray
{ private:
    TRdsLinearInterpolation **Array; // Массив таблиц
    int Size;                        // Отведенный размер массива
    static TRdsLinearInterpolation Dummy;  // Объект, возвращаемый для ссылок "мимо"
    // Элемент (если нет - Dummy)
    TRdsLinearInterpolation *Element(int i)const;
  public:
    // Число
    inline int GetCount(void){return Size;};
    // Переотвести
    void Realloc(int newsize);
    // Очистить
    void Clear(void);
    // Функция по номеру
    inline TL57TextFuncFTableLink operator[](int i)const{ return TL57TextFuncFTableLink(Element(i)); };
    inline TL57TextFuncFTableLink operator()(int i)const{ return TL57TextFuncFTableLink(Element(i)); };

    // Сравнить две матрицы
    static BOOL CompareMatr(rdsbcppVarAncestor *M1,rdsbcppVarAncestor *M2);
    // Сравнить матрицу со специальной "ошибочной", которая, по идее, не должна никогда появиться на входе
    static BOOL IsBadMatrix(rdsbcppVarAncestor *M);
    // Записать специальную "ошибочную" матрицу (размер уже должен быть 2x2)
    static void SetBadMatrix(rdsbcppVarAncestor *M);

    TL57TextFuncFArray(void){Array=NULL;Size=0;};
    ~TL57TextFuncFArray(){Clear();};
};
TRdsLinearInterpolation TL57TextFuncFArray::Dummy;
//---------------------------------------------------------------------------

// Элемент связного списка строк (ссылки на внешние данные)
class TL57TextFileLine
{ public:
    TL57TextFileLine *Next;
    char *Line;
    int Id;

    TL57TextFileLine(void){Next=NULL;Line=NULL;Id=LINEID_NONE;};
};
//---------------------------------------------------------------------------
// Строка
class TL57TextString
{ private:
    char *Data;                 // Хранимая строка
    static char ZeroByte;       // Нулевой байт (для пустой строки)
  public:

    // Очистить данные
    void Clear(void);
    // Внутренняя строка
    inline char *c_str(void)const{ZeroByte=0;return Data?Data:(&ZeroByte);};
    // Длина
    int Length(void) const;
    // Пустота
    inline int IsEmpty(void) const {return Data==NULL || (*Data)==0;};
    inline int IsNotEmpty(void) const {return !IsEmpty();};
    // Присвоить
    void Assign(const char *s);
    void Assign(const char *s,int n);
    inline void Assign(const TL57TextString &s){Assign(s.c_str());};
    // Добавить к строке
    void AddString(char *s);

    // Преобразование числа int в строку
    void SetInt(int i);
    // Преобразование числа double в строку
    void SetDouble(double d,int dec=-1);

    TL57TextString(void){Data=NULL;};
    ~TL57TextString(){Clear();};
};
//---------------------------------------------------------------------------
// Служебные комментарии
#define AUTOTEXT_START "\x2f***** НАЧАЛО АВТОМАТИЧЕСКИ СФОРМИРОВАННОГО ТЕКСТА *****/"
#define AUTOTEXT_END   "\x2f***** КОНЕЦ АВТОМАТИЧЕСКИ СФОРМИРОВАННОГО ТЕКСТА  *****/"
// Чтение и запись текстового файла
class TL57TextFileBasic
{ protected:
    char *Text; // Считанный текст

    // Писать в файл
    int _WindowsWrite(const void *data,size_t *size);
    int WindowsWrite(const void *data,size_t size);
    // Читать из файла
    int WindowsRead(void *data, size_t *size);
    int WindowsRead(void *data, size_t size);
    // Считать короткий файл в динамически отведенный буфер
    char * ReadFileToMemory(char *path,DWORD *psize,int addzerobyte);
    // Получить размер файла (короткий)
    int WindowsGetFileSizeShort(DWORD *psize,int *pTooLong=NULL);

    // Разобрать Text на строки (старые строки должны быть стерты)
    void SplitTextServ(void);

    // Считать файл и разобрать на строки
    BOOL ReadAndSplit(char *path);

    // Открыть на запись
    int OpenWrite(char *path);
    // Открыть на чтение
    int OpenRead(char *path);
    // Открыт?
    inline int IsOpen(void){return Handle!=INVALID_HANDLE_VALUE;};
    // Записать строку
    int WriteLine(char *line,int crlf=0);
    // Закрыть файл
    void Close(void);

    // Найти в тексте строку
    TL57TextFileLine * FindLine(TL57TextFileLine *from,char *text,BOOL skipspaces);

    // Получить полное имя файла модели блока (динамическое, осв. rdsFree)
    char *GetModelFileName(RDS_BHANDLE block);
  public:
    HANDLE Handle;

    TL57TextFileLine *FirstLine; // Считанный текст, разбитый на строки

    // Очистить загруженный текст
    void ClearText(void);

    // Считать файл модели блока и разобрать на функции
    BOOL ReadAndParseModel(RDS_BHANDLE block,TL57TextFileBasic_ServMarker *markers,BOOL message);
    // Считать файл и разобрать на функции
    BOOL ReadAndParse(char *path,TL57TextFileBasic_ServMarker *markers);
    // Записать файл из списка функций
    int SaveFile(char *path);
    // Записать файл из списка функций в модель блока
    int SaveFileToModel(RDS_BHANDLE block,BOOL bak=FALSE);
    // Разобрать копию переданного текста на строки
    void SplitText(char *text);

    // Виртуальные
    // Разобрать скрытую секцию
    virtual void ParseHiddenSection(TL57TextFileLine */*line*/){};
    // Записать скрытую секцию
    virtual BOOL WriteHiddenSection(void){return TRUE;};
    // Записать какую-то секцию и вернуть маркер ее конца
    virtual BOOL WriteSection(int /*id*/,int */*pendid*/){return TRUE;};


    // Проверить наличие связи у входа блока
    static BOOL CheckConnectionForBlockVar(RDS_BHANDLE Block,int num);
    static BOOL CheckConnectionForBlockVar(RDS_BHANDLE Block,char *name);

    TL57TextFileBasic(void)
      {Handle=INVALID_HANDLE_VALUE;Text=NULL;FirstLine=NULL;
      };
    virtual ~TL57TextFileBasic()
      { Close(); ClearText();
      };
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Файл
//---------------------------------------------------------------------------
// Закрыть файл
void TL57TextFileBasic::Close(void)
{
  if(Handle!=INVALID_HANDLE_VALUE)
    { CloseHandle(Handle);
      Handle=INVALID_HANDLE_VALUE;
    }
}
//---------------------------------------------------------------------------

// Открыть на запись
int TL57TextFileBasic::OpenWrite(char *path)
{ char *fullpath;
  Close();
  fullpath=rdsGetFullFilePath(path,NULL,NULL);
  if(!fullpath)
    return 0;
  Handle=CreateFile(fullpath,GENERIC_WRITE,
                    FILE_SHARE_READ, NULL,
                    CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,NULL);
  rdsFree(fullpath);
  return Handle!=INVALID_HANDLE_VALUE;
}
//---------------------------------------------------------------------------

// Открыть на чтение
int TL57TextFileBasic::OpenRead(char *path)
{ char *fullpath;
  Close();
  fullpath=rdsGetFullFilePath(path,NULL,NULL);
  if(!fullpath)
    return 0;
  Handle=CreateFile(fullpath,GENERIC_READ,
                    FILE_SHARE_READ, NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,NULL);
  rdsFree(fullpath);
  return Handle!=INVALID_HANDLE_VALUE;
}
//---------------------------------------------------------------------------

// Очистить загруженный текст
void TL57TextFileBasic::ClearText(void)
{ TL57TextFileLine *l,*ln;
  if(Text)
    delete[] Text;
  Text=NULL;

  l=FirstLine;
  while(l)
    { ln=l->Next;
      delete l;
      l=ln;
    }
  FirstLine=NULL;
}
//---------------------------------------------------------------------------


#define Windows_ChunkSizeMax (1 << 22) // Читать/писать не больше этого
// Писать в файл
int TL57TextFileBasic::_WindowsWrite(const void *data,size_t *size)
{ size_t originalSize = *size;
  if (originalSize == 0)
    return 1/*true*/;
  if(Handle==INVALID_HANDLE_VALUE)
    return 0/*false*/;

  *size = 0;
  do
  {
    DWORD curSize = (originalSize > Windows_ChunkSizeMax) ? Windows_ChunkSizeMax : (DWORD)originalSize;
    DWORD processed = 0;
    BOOL res = WriteFile(Handle, data, curSize, &processed, NULL);
    data = (void *)((BYTE*)data + processed);
    originalSize -= processed;
    *size += processed;
    if (!res)
      return 0/*false*/;
    if (processed == 0)
      break;
  }
  while (originalSize > 0);
  return 1/*true*/;
}
int TL57TextFileBasic::WindowsWrite(const void *data,size_t size)
{ size_t act_write=size;
  if(!_WindowsWrite(data,&act_write))
    return 0/*false*/;
  return act_write==size;
}

// Читать из файла
int TL57TextFileBasic::WindowsRead(void *data, size_t *size)
{
  size_t originalSize = *size;
  if (originalSize == 0)
    return 1/*true*/;

  if(Handle==INVALID_HANDLE_VALUE)
    return 0/*false*/;

  *size = 0;
  do
  {
    DWORD curSize = (originalSize > Windows_ChunkSizeMax) ? Windows_ChunkSizeMax : (DWORD)originalSize;
    DWORD processed = 0;
    BOOL res = ReadFile(Handle, data, curSize, &processed, NULL);
    data = (void *)((BYTE*)data + processed);
    originalSize -= processed;
    *size += processed;
    if (!res)
      return 0/*false*/;
    if (processed == 0)
      break;
  }
  while (originalSize > 0);
  return 1/*true*/;
}
int TL57TextFileBasic::WindowsRead(void *data, size_t size)
{ size_t act_read=size;
  if(!WindowsRead(data,&act_read))
    return 0/*false*/;
  return act_read==size;
}
#undef Windows_ChunkSizeMax
//---------------------------------------------------------------------------

// Получить размер файла (короткий)
int TL57TextFileBasic::WindowsGetFileSizeShort(DWORD *psize,int *pTooLong)
{ DWORD sizeHigh,sizeLow;
  if(!IsOpen())
    return 0;
  sizeLow = GetFileSize(Handle,&sizeHigh);
  if(pTooLong) *pTooLong=0/*false*/;
  if (sizeLow == 0xFFFFFFFF)
    { DWORD res = GetLastError();
      if(res!=NO_ERROR)
        return 0/*false*/;
      if(sizeHigh)
        { if(pTooLong) *pTooLong=1/*true*/;
          return 0/*false*/;
        }
     }
  *psize=sizeLow;
  return 1/*true*/;
}
//---------------------------------------------------------------------------

// Считать короткий файл в динамически отведенный буфер
// path       - путь
// psize      - возвращаемый размер
char * TL57TextFileBasic::ReadFileToMemory(char *path,DWORD *psize,int addzerobyte)
{ HANDLE h;
  DWORD size;
  size_t act_size;
  char *mainbuf,*fullpath;
  bool ok;

  Close();

  if(psize) *psize=0;

  fullpath=rdsGetFullFilePath(path,NULL,NULL);
  if(!fullpath)
    return NULL;

  Handle=CreateFile(fullpath,GENERIC_READ,
               FILE_SHARE_READ, NULL,
               OPEN_EXISTING,
               FILE_ATTRIBUTE_NORMAL, NULL);
  rdsFree(fullpath);
  if(Handle==INVALID_HANDLE_VALUE)
    return NULL;
  if(!WindowsGetFileSizeShort(&size))
    { Close();
      return NULL;
    }

  mainbuf=new char[addzerobyte?(size+1):size];

  // Читаем файл
  act_size=size;
  ok=WindowsRead(mainbuf,&act_size);
  Close();
  if(ok)
    ok=(act_size==size);
  if(!ok)
    { delete[] mainbuf;
      return NULL;
    }
  if(psize) *psize=size;
  if(addzerobyte)
    mainbuf[size]=0;
  return mainbuf;
}
//---------------------------------------------------------------------------

// Записать строку
int TL57TextFileBasic::WriteLine(char *line,int crlf)
{
  if(!IsOpen())
    return 0;
  if(line!=NULL && (*line)!=0)
    { if(!WindowsWrite(line,strlen(line)))
        return 0;
    }
  if(crlf)
    { if(!WindowsWrite("\r\n",2))
        return 0;
    }
  return 1;
}
//---------------------------------------------------------------------------

// Проверить наличие связи у входа блока
BOOL TL57TextFileBasic::CheckConnectionForBlockVar(RDS_BHANDLE Block,int num)
{ RDS_VARDESCRIPTION vdescr;

  RDS_INITSERVSIZE(vdescr);

  // Поиск переменной
  if(!rdsGetBlockVar(Block,num,&vdescr))
    return FALSE;
  return TL57TextFileBasic::CheckConnectionForBlockVar(Block,vdescr.Name);
}
BOOL TL57TextFileBasic::CheckConnectionForBlockVar(RDS_BHANDLE Block,char *name)
{ RDS_CHANDLE c=NULL;
  RDS_POINTDESCRIPTION pdescr;
  RDS_CONNDESCRIPTION cdescr;
  int varlen;

  if(!name)
    return FALSE;
  varlen=strlen(name);
  if(!varlen)
    return FALSE;

  RDS_INITSERVSIZE(pdescr);
  RDS_INITSERVSIZE(cdescr);


  for(;;)
    { c=rdsGetBlockLink(Block,c,TRUE,TRUE,&pdescr);
      if(!c) break;
      // Какая-то связь
      rdsGetConnDescription(c,&cdescr);
      if(!cdescr.Active) continue;
      if(!pdescr.VarName) continue;
      if(strncmp(pdescr.VarName,name,varlen)==0)
        return TRUE;
    }
  return FALSE;
}
//---------------------------------------------------------------------------

// Разобрать Text на строки (старые строки должны быть стерты)
void TL57TextFileBasic::SplitTextServ(void)
{ char *s;
  TL57TextFileLine *curline,*l;

  if(!Text)
    return;
  s=Text;
  for(;;)
    { if((*s)==0) // Текст кончился
        break;
      // Добавляем строку
      l=new TL57TextFileLine();
      l->Line=s;
      if(FirstLine)
        { curline->Next=l;
          curline=l;
        }
      else
        FirstLine=curline=l;
      // Ищем конец строки
      while( (*s)!=0 && (*s)!='\r' && (*s)!='\n')
        s++;
      if((*s)==0)  // Текст кончился
        break;
      *s=0; // Ставим нулевой байт в конец
      s++;
      if((*s)=='\r'||(*s)=='\n') // Два конца строки подряд
        s++;
    }
}
//---------------------------------------------------------------------------

// Разобрать копию переданного текста на строки
void TL57TextFileBasic::SplitText(char *text)
{ int l;
  // Очистить загруженный текст
  ClearText();
  if(!text)
    return;
  if((*text)==0)
    return;
  l=strlen(text);
  Text=new char[l+1];
  strcpy(Text,text);
  SplitTextServ();
}
//---------------------------------------------------------------------------

// Считать файл и разобрать на строки
BOOL TL57TextFileBasic::ReadAndSplit(char *path)
{ // Очистить загруженный текст
  ClearText();
  Text=ReadFileToMemory(path,NULL,1);
  if(!Text)
    return FALSE;
  SplitTextServ();
  return TRUE;
}
//---------------------------------------------------------------------------

// Найти в тексте строку
TL57TextFileLine * TL57TextFileBasic::FindLine(TL57TextFileLine *from,char *text,BOOL skipspaces)
{
  for(TL57TextFileLine *l=from?from:FirstLine;l!=NULL;l=l->Next)
    { char *s=l->Line;
      if(!s)
        continue;
      if(skipspaces)
        { while((*s)==' ' || (*s)=='\t')
            s++;
        }
      if(strcmp(s,text)==0)
        return l;
    }
  return NULL;
}
//---------------------------------------------------------------------------

// Получить полное имя файла модели блока (динамическое, осв. rdsFree)
char *TL57TextFileBasic::GetModelFileName(RDS_BHANDLE block)
{ RDS_PCOMPMODELDATA model;
  RDS_PCOMPMODULEDATA module;
  if(!block)
    return NULL;
  model=rdscompGetBlockModelData(block);
  if(!model)
    return NULL;
  // У блока есть модель
  return rdsGetFullFilePath(model->ModelName,NULL,NULL);
}
//---------------------------------------------------------------------------

// Считать файл модели блока и разобрать на функции
BOOL TL57TextFileBasic::ReadAndParseModel(RDS_BHANDLE block,TL57TextFileBasic_ServMarker *markers,BOOL message)
{ char *path=GetModelFileName(block);
  bool ok;
  ClearText();
  if(path)
    { ok=ReadAndParse(path,markers);
      rdsFree(path);
    }
  else
    ok=FALSE;
  if((!ok) && message)
    rdsMessageBox("Файл модели блока отсутствует или имеет неверный формат","Загрузка функций",MB_ICONERROR|MB_OK);
  return ok;
}
//---------------------------------------------------------------------------

// Считать файл и разобрать на функции
BOOL TL57TextFileBasic::ReadAndParse(char *path,TL57TextFileBasic_ServMarker *markers)
{ int n;
  char *aux;
  TL57TextFileLine *line;

  if(!ReadAndSplit(path))
    return FALSE;
  // Текст разбит на строки

  // Первая строка должна быть "$BC55MODEL..."
  if(FirstLine)
    { bool error=TRUE;
      char *s=FirstLine->Line;
      if(s)
        { while((*s)==' ' || (*s)=='\t')
            s++;
          if(strncmp(s,"$BC55MODEL",10)==0)
            error=FALSE;
        }
      if(error)
        return FALSE;
    }
  else
    return FALSE;

  // Ищем "$HIDDEN"
  line=FindLine(NULL,"$HIDDEN",FALSE);
  if(line)
    { line->Id=LINEID_HIDDEN;
      line=line->Next;
    }
  else // Нет скрытой секции
     { line=FindLine(NULL,"$OPTIONS",FALSE);
       if(line)
         { line->Id=LINEID_OPTIONS;
           line=NULL;
         }
     }
  // В line - первая строка описания функций, или NULL, если описания нет

  // Просматриваем остальные и сравниваем со служебными метками
  if(markers)
    for(TL57TextFileLine *l=FirstLine;l!=NULL;l=l->Next)
      { char *s=l->Line;
        TL57TextFileBasic_ServMarker *m=markers;
        if(!s)
          continue;
        while((*s)==' ' || (*s)=='\t' || (*s)=='/')
          s++;
        while(m->Text)
          { if(m->TextLen<0)
              m->TextLen=strlen(m->Text);
            if(strncmp(s,m->Text,m->TextLen)==0)
              { l->Id=m->Id;
                break;
              }
            m++;
          }
      }

  // Читаем функции (вызываем виртуальную)
  if(line)
    ParseHiddenSection(line);
  // Загруженный текст остается в памяти
  return TRUE;
}
//---------------------------------------------------------------------------

// Записать файл из списка функций в модель блока
int TL57TextFileBasic::SaveFileToModel(RDS_BHANDLE block,BOOL bak)
{ char *path=GetModelFileName(block),*bakpath=NULL;
  int res=0;
  if(path)
    { if(bak)
        { bakpath=rdsTransformFileName(path,RDS_TFN_CHANGEEXT,".bak",NULL);
          if(bakpath)
            { DeleteFile(bakpath);
              CopyFile(path,bakpath,FALSE);
            }
        }
      res=SaveFile(path);
      if((!res) && bakpath!=NULL)
        { // Запись не удалась - пытаемся вернуть
          DeleteFile(path);
          CopyFile(bakpath,path,FALSE);
        }
      rdsFree(path);
    }
  if(bakpath)
    rdsFree(bakpath);
  return res;
}
//---------------------------------------------------------------------------

// Записать файл из списка функций
int TL57TextFileBasic::SaveFile(char *path)
{ int ok=1,funcwritten=0,endmarker;
  TL57TextFileLine *l=FirstLine;

  if(!OpenWrite(path))
    return 0;

  while(ok && l!=NULL)
    switch(l->Id)
      { case LINEID_HIDDEN: // Начало скрытой секции - после этой строки пишем данные функций
          if(!funcwritten) // Функции еще не писались
            { ok=ok && WriteLine(l->Line,1);
              // Данные
              ok=ok && WriteHiddenSection(); // Виртуальная
              funcwritten=1;
            }
          // Функции записаны - пропускаем все до начала следующей секции
          l=l->Next;
          while(l)
            { if(l->Line!=NULL && l->Line[0]=='$')
                break;
              l=l->Next;
            }
          // Теперь l - начало следующей секции
          break;

        case LINEID_OPTIONS: // Секция опций модели
          if(!funcwritten) // Функции еще не писались
            { ok=ok && WriteLine("$HIDDEN",1);
              // Данные
              ok=ok && WriteHiddenSection(); // Виртуальная
              funcwritten=1;
            }
          ok=ok && WriteLine(l->Line,1); // $OPTIONS
          l=l->Next;
          break;

        case LINEID_NONE: // Строка не обрабатывается
          ok=ok && WriteLine(l->Line,1);
          l=l->Next;
          break;

        default: // Какой-то маркер вставки
          endmarker=LINEID_NONE;
          ok=ok && WriteLine(l->Line,1); // Сам маркер
          ok=ok && WriteSection(l->Id,&endmarker); // Виртуальная - запись секции
          // Пропуск до конечного маркера
          l=l->Next;
          if(endmarker!=LINEID_NONE)
            while(l)
              { if(l->Id==endmarker)
                  { ok=ok && WriteLine(l->Line,1); // Конечный маркер
                    l=l->Next;
                    break;
                  }
                l=l->Next;
              }
          // Теперь l - следующая после конечного маркера строка или текст кончился
          break;
      }
  return ok;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Строка
//---------------------------------------------------------------------------

char TL57TextString::ZeroByte=0;

// Очистить данные
void TL57TextString::Clear(void)
{
  if(Data)
    delete[] Data;
  Data=NULL;
}
//---------------------------------------------------------------------------

// Длина
int TL57TextString::Length(void) const
{return Data?strlen(Data):0;}
//---------------------------------------------------------------------------

// Присвоить
void TL57TextString::Assign(const char *s)
{ int l;
  Clear();
  if(s==NULL || (*s)==0)
    return;
  l=strlen(s);
  Data=new char[l+1];
  strcpy(Data,s);
}
void TL57TextString::Assign(const char *s,int n)
{ int l;
  Clear();
  if(s==NULL || (*s)==0)
    return;
  l=strlen(s);
  if(l>n)
    l=n;
  Data=new char[l+1];
  strncpy(Data,s,l);
  Data[l]=0;
}
//---------------------------------------------------------------------------

// Добавить к строке
void TL57TextString::AddString(char *s)
{ char *ns;
  int l1,l2,l;
  if(Data==NULL || (*Data)==0)
    { Assign(s);
      return;
    }
  if(s==NULL || (*s)==0)
    return;
  l1=strlen(Data);
  l2=strlen(s);
  l=l1+l2;
  ns=new char[l+1];
  strncpy(ns,Data,l1);
  strncpy(ns+l1,s,l2);
  ns[l]=0;
  delete[] Data;
  Data=ns;
}
//---------------------------------------------------------------------------

// Преобразование числа int в строку
void TL57TextString::SetInt(int i)
{ char tempstr[100];
  //sprintf(tempstr,"%d",i);
  itoa(i,tempstr,10);
  Assign(tempstr);
}
//---------------------------------------------------------------------------

// Преобразование числа double в строку
void TL57TextString::SetDouble(double d,int dec)
{ char *s=rdsDtoA(d,dec,NULL);
  if(s)
    { Assign(s);
      rdsFree(s);
    }
  else
    Clear();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Массив таблиц кусочно-линейных функций
//---------------------------------------------------------------------------
// Очистить
void TL57TextFuncFArray::Clear(void)
{
  for(int i=0;i<Size;i++)
    if(Array[i])
      delete Array[i];

  if(Array) delete[] Array;
  Array=NULL;
  Size=0;
}
//---------------------------------------------------------------------------

// Переотвести
void TL57TextFuncFArray::Realloc(int newsize)
{ TRdsLinearInterpolation **newData,*e;

  if(newsize==Size)
    return;

  if(newsize<=0)
    { Clear();
      return;
    }

  newData=new TRdsLinearInterpolation*[newsize];

  if(Array)
    { if(Size>0) memcpy(newData,Array,Size*sizeof(TRdsLinearInterpolation*));
      delete[] Array;
    }
  // Пустой "хвост"
  for(int i=Size;i<newsize;i++)
    { e=new TRdsLinearInterpolation();
      e->SetDefValue(0.0);
      newData[i]=e;
    }
  Array=newData;
  Size=newsize;
}
//---------------------------------------------------------------------------

// Элемент (если нет - Dummy)
TRdsLinearInterpolation *TL57TextFuncFArray::Element(int i)const
{ TRdsLinearInterpolation *e;
  if(i<0 || i>=Size)
    return &Dummy;
  e=Array[i];
  if(!e)
    return &Dummy;
  return e;
}
//---------------------------------------------------------------------------

// Сравнить две матрицы
BOOL TL57TextFuncFArray::CompareMatr(rdsbcppVarAncestor *M1,rdsbcppVarAncestor *M2)
{ RDS_ARRAYACCESSDATA Data1,Data2;

  if(!M1->GetAccessData(&Data1))
    return FALSE;
  if(!M2->GetAccessData(&Data2))
    return FALSE;
  if(Data1.Rows!=Data2.Rows || Data1.Cols!=Data2.Cols ||
     Data1.ItemSize!=sizeof(double) || Data2.ItemSize!=sizeof(double) ||
     rdsGetVarArrayElementType(M1->GetVoidPtr())!=RDS_VARTYPE_DOUBLE ||
     rdsGetVarArrayElementType(M2->GetVoidPtr())!=RDS_VARTYPE_DOUBLE ||
     Data1.Data==NULL || Data2.Data==NULL)
    return FALSE;

  // Размеры массивов совпадают
  return memcmp(Data1.Data,Data2.Data,Data1.Rows*Data1.Cols*sizeof(double))==0;
}
//---------------------------------------------------------------------------

// Сравнить матрицу со специальной "ошибочной", которая, по идее, не должна никогда появиться на входе
BOOL TL57TextFuncFArray::IsBadMatrix(rdsbcppVarAncestor *M)
{ RDS_ARRAYACCESSDATA Data;
  double *arr;

  if(!M->GetAccessData(&Data))
    return FALSE;
  if(Data.Rows!=2 || Data.Cols!=2 || Data.ItemSize!=sizeof(double) ||
     rdsGetVarArrayElementType(M->GetVoidPtr())!=RDS_VARTYPE_DOUBLE ||
     Data.Data==NULL)
    return FALSE;

  // Матрица 2x2
  arr=(double*)(Data.Data);
  return arr[0]==rdsbcppHugeDouble &&
         arr[1]==rdsbcppHugeDouble &&
         arr[2]==rdsbcppHugeDouble &&
         arr[3]==rdsbcppHugeDouble;
}
//---------------------------------------------------------------------------

// Записать специальную "ошибочную" матрицу (размер уже должен быть 2x2)
void TL57TextFuncFArray::SetBadMatrix(rdsbcppVarAncestor *M)
{ RDS_ARRAYACCESSDATA Data;
  double *arr;

  if(!M->GetAccessData(&Data))
    return;
  if(Data.Rows!=2 || Data.Cols!=2 || Data.ItemSize!=sizeof(double) ||
     rdsGetVarArrayElementType(M->GetVoidPtr())!=RDS_VARTYPE_DOUBLE ||
     Data.Data==NULL)
    return;

  // Матрица 2x2
  arr=(double*)(Data.Data);
  arr[0]=rdsbcppHugeDouble;
  arr[1]=rdsbcppHugeDouble;
  arr[2]=rdsbcppHugeDouble;
  arr[3]=rdsbcppHugeDouble;
}
//---------------------------------------------------------------------------
#undef LINEID_HIDDEN
#undef LINEID_OPTIONS
#undef LINEID_NONE

// Проверить существование файла
BOOL L57_FileExists(char *filename)
{ WIN32_FIND_DATA data;
  HANDLE h;
  if(filename==NULL || (*filename)==0)
    return FALSE;
  h=FindFirstFile(filename,&data);
  if(h!=INVALID_HANDLE_VALUE)
    { FindClose(h);
      return TRUE;
    }
  return FALSE;
}
//---------------------------------------------------------------------------


// Проверить существование файла справки
// *pstate - флаг для исключения многократных проверок (может быть NULL)
//      ==0 - еще не известно
//      ==1 - нет файла
//      ==2 - есть файл
BOOL L57_HelpUIFileExists(char *filename,int *pstate)
{ char *full=NULL;
  BOOL res=TRUE;

  if(pstate)
    { switch(*pstate)
        { case 1: return FALSE;
          case 2: return TRUE;
        }
    }

  if(filename==NULL || (*filename)==0)
    res=FALSE;
  if(res)
    { full=rdsGetFullFilePath(filename,rdsGetSystemPath(RDS_GSPDOCPATH),NULL);
      if(!full)
        res=FALSE;
    }
  if(res)
    { if(full[0])
        res=L57_FileExists(full);
      else
        res=FALSE;
    }
  rdsFree(full);
  if(pstate)
    *pstate=res?2:1;
  return res;
}
//---------------------------------------------------------------------------

