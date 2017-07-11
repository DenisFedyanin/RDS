//---------------------------------------------------------------------------
// �������������� ������ � ������� ��� ��������� ��������������������� �������
// ������� ���� ���������� f(x,y,z)
//---------------------------------------------------------------------------
#ifndef RdsLinInterH
  #include <RdsAC/LinInter.cpp>     
#endif

// ��������� �� ������� ��������� ��������
typedef int (*TL57TextFileFuncDescr)(int,int*,char**,char**);
//---------------------------------------------------------------------------
// ������ double (������ �� ������� ������)
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
// ��������� ������-������
class TL57TextFuncFTableLink
{ public:
    TRdsLinearInterpolation *Engine; // ������ �� ������������
    BOOL ForceYIndex;                // �������������� ������ �������� ��� ������� ����� ����������
    int YIndex;                      // ������ �������� ��� ������� ����� ���������� (��� ForceYIndex==TRUE)

    // ���������
    inline void Setup(rdsbcppVarAncestor *M,int flags=RDSLININTER_T_UNKNOWN){Engine->Setup(*M,flags);};
    // ��� �������
    inline int Type(void){return Engine->GetType();};

    // ������� ���� ���������� (��� ������� ����� x2 - ������ �������� � �������)
    inline double operator()(double x1,double x2){ return Engine->CalcGeneral(x1,x2); };
    // ������� ����� ����������
    inline double operator()(double x){ return ForceYIndex?Engine->Calc_Fx(x,0,YIndex):Engine->Calc_Fx(x); };
    // ������� ����� ���������� � ��������� �������� ��������� � ��������
    inline double operator()(double x,int xindex,int yindex){ return Engine->Calc_Fx(x,xindex,yindex); };
    // ������� ����� ���������� � ��������� ����������� ������� ��������
    inline TL57TextFuncFTableLink operator[](int i){ return TL57TextFuncFTableLink(Engine,i); };

    TL57TextFuncFTableLink(TRdsLinearInterpolation *p){Engine=p;ForceYIndex=FALSE;};
    TL57TextFuncFTableLink(TRdsLinearInterpolation *p,int yindex){Engine=p;ForceYIndex=TRUE;YIndex=yindex;};
    TL57TextFuncFTableLink(const TL57TextFuncFTableLink &p){Engine=p.Engine;ForceYIndex=p.ForceYIndex;YIndex=p.YIndex;};
};
//---------------------------------------------------------------------------
// ������ ������ �������-�������� �������
class TL57TextFuncFArray
{ private:
    TRdsLinearInterpolation **Array; // ������ ������
    int Size;                        // ���������� ������ �������
    static TRdsLinearInterpolation Dummy;  // ������, ������������ ��� ������ "����"
    // ������� (���� ��� - Dummy)
    TRdsLinearInterpolation *Element(int i)const;
  public:
    // �����
    inline int GetCount(void){return Size;};
    // �����������
    void Realloc(int newsize);
    // ��������
    void Clear(void);
    // ������� �� ������
    inline TL57TextFuncFTableLink operator[](int i)const{ return TL57TextFuncFTableLink(Element(i)); };
    inline TL57TextFuncFTableLink operator()(int i)const{ return TL57TextFuncFTableLink(Element(i)); };

    // �������� ��� �������
    static BOOL CompareMatr(rdsbcppVarAncestor *M1,rdsbcppVarAncestor *M2);
    // �������� ������� �� ����������� "���������", �������, �� ����, �� ������ ������� ��������� �� �����
    static BOOL IsBadMatrix(rdsbcppVarAncestor *M);
    // �������� ����������� "���������" ������� (������ ��� ������ ���� 2x2)
    static void SetBadMatrix(rdsbcppVarAncestor *M);

    TL57TextFuncFArray(void){Array=NULL;Size=0;};
    ~TL57TextFuncFArray(){Clear();};
};
TRdsLinearInterpolation TL57TextFuncFArray::Dummy;
//---------------------------------------------------------------------------
// ������ �������� �������
class TL57TextFuncArg
{ public:
    double xval,yval,zval;
    TL57TextDoubleArray Params;
    TL57TextFuncFArray Functions;

    TL57TextFuncArg(void){xval=yval=zval=0.0;};
};
//---------------------------------------------------------------------------
// ������� �������� ������ ����� (������ �� ������� ������)
class TL57TextFileLine
{ public:
    TL57TextFileLine *Next;
    char *Line;
    int Id;

    TL57TextFileLine(void){Next=NULL;Line=NULL;Id=-1;};
};
//---------------------------------------------------------------------------
// ������������ �������������� �����
#define LINEID_HIDDEN           1       // ������ "$HIDDEN", ���������� ������� ������
#define LINEID_OPTIONS          3       // ������ "$OPTIONS", ���������� ������ ����� ������
#define LINEID_CASESBEGIN       4       // ������ ����� ������� case ���������� �������
#define LINEID_CASESEND         5       // ����� ����� ������� case ���������� �������
#define LINEID_IFSBEGIN         6       // ������ ����� ������� if ���������� �������
#define LINEID_IFSEND           7       // ����� ����� ������� if ���������� �������
#define LINEID_LISTBEGIN        8       // ������ ����� ������� ������� ������������ �������
#define LINEID_LISTEND          9       // ����� ����� ������� ������� ������������ �������
// ������� �������� ������ ��������������� ����� ��������
class TL57TextFileField
{ public:
    TL57TextFileField *Next;
    int ListId,TextId,ChgButtonId,AddButtonId,DelButtonId,NumId;
    int FuncIdToSelect;

    TL57TextFileField(void){Next=NULL;ListId=TextId=ChgButtonId=AddButtonId=DelButtonId=FuncIdToSelect=NumId=-1;};
};
//---------------------------------------------------------------------------
// ������
class TL57TextString
{ private:
    char *Data;                 // �������� ������
    static char ZeroByte;       // ������� ���� (��� ������ ������)
  public:

    // �������� ������
    void Clear(void);
    // ���������� ������
    inline char *c_str(void)const{ZeroByte=0;return Data?Data:(&ZeroByte);};
    // �����
    int Length(void) const;
    // �������
    inline int IsEmpty(void) const {return Data==NULL || (*Data)==0;};
    inline int IsNotEmpty(void) const {return !IsEmpty();};
    // ���������
    void Assign(const char *s);
    void Assign(const char *s,int n);
    inline void Assign(const TL57TextString &s){Assign(s.c_str());};
    // �������� � ������
    void AddString(char *s);

    // �������������� ����� int � ������
    void SetInt(int i);

    TL57TextString(void){Data=NULL;};
    ~TL57TextString(){Clear();};
};
//---------------------------------------------------------------------------
// ������� �������� ������ ������� (������ ��������)
class TL57TextFunc
{ public:
    int Num;    // ����� �������
    TL57TextString Name; // ��� �������
    TL57TextString Code; // ��������� �������

    TL57TextFunc *Next;

    TL57TextFunc(void){Next=NULL;Num=-1;};
};
//---------------------------------------------------------------------------
// ��������� �����������
#define AUTOTEXT_START "\x2f***** ������ ������������� ��������������� ������ *****/"
#define AUTOTEXT_END   "\x2f***** ����� ������������� ��������������� ������  *****/"
// ������ � ������ ���������� �����
class TL57TextFile
{ private:
    char *Text; // ��������� �����

    // ������ � ����
    int _WindowsWrite(const void *data,size_t *size);
    int WindowsWrite(const void *data,size_t size);
    // ������ �� �����
    int WindowsRead(void *data, size_t *size);
    int WindowsRead(void *data, size_t size);
    // ������� �������� ���� � ����������� ���������� �����
    char * ReadFileToMemory(char *path,DWORD *psize,int addzerobyte);
    // �������� ������ ����� (��������)
    int WindowsGetFileSizeShort(DWORD *psize,int *pTooLong=NULL);

    // ��������� Text �� ������ (������ ������ ������ ���� ������)
    void SplitTextServ(void);

    // ������� ���� � ��������� �� ������
    BOOL ReadAndSplit(char *path);

    // ������� �� ������
    int OpenWrite(char *path);
    // ������� �� ������
    int OpenRead(char *path);
    // ������?
    inline int IsOpen(void){return Handle!=INVALID_HANDLE_VALUE;};
    // �������� ������
    int WriteLine(char *line,int crlf=0);
    // ������� ����
    void Close(void);

    // ����� � ������ ������
    TL57TextFileLine * FindLine(TL57TextFileLine *from,char *text,BOOL skipspaces);

    // �������� ������ ������� � �������� ����
    int WriteFuncData(void);
    // �������� case ���������� ������� � �������� ����
    int WriteFuncCases(void);
    // �������� ����� if ���������� ������� � �������� ����
    int WriteFuncIfs(void);
    // �������� ������� �������� ��������� � ������ ������� � �������� ����
    int WriteFuncList(void);

    // �������� ������ ��� ����� ������ ����� (������������, ���. rdsFree)
    char *GetModelFileName(RDS_BHANDLE block);
  public:
    HANDLE Handle;

    int Changed;
    BOOL CanEditFuncId; // ���������� ������� ������������� �������������� �������

    TL57TextFileLine *FirstLine; // ��������� �����, �������� �� ������
    TL57TextFunc *FirstFunc,*LastFunc;     // �� ��, �������� �� �������

    TL57TextFileField *FirstField; // ������ ��������������� ����������� ����� ����� ������ ������� � ���� ��������
    // �������� ������ �����
    void ClearFields(void);

    // �������� ����������� �����
    void ClearText(void);
    // �������� ������ �������
    void ClearFunc(void);


    // ������� �� ��������������
    TL57TextFunc * FuncById(int id);
    // ������� �� ������
    TL57TextFunc * FuncByIndex(int index);
    // ������������� ������� �� ������
    int FuncNumByIndex(int index,int defaultval=-1);
    // ������� ������� �� ������
    void DeleteFuncByIndex(int index);
    // ����� �������
    TL57TextFunc * AddFunction(int findnum=1);
    // ����� ������������ ������������� �������
    int FindMaxFuncId(void);

    // ��������� ������ ������� ��� ������ ������� ������� ��������
    void FillByFunc(TL57TextFileFuncDescr func);

    // ������� ���� ������ ����� � ��������� �� �������
    BOOL ReadAndParseModel(RDS_BHANDLE block,BOOL message);
    // ������� ���� � ��������� �� �������
    BOOL ReadAndParse(char *path);
    // �������� ���� �� ������ �������
    int SaveFile(char *path);
    // �������� ���� �� ������ ������� � ������ �����
    int SaveFileToModel(RDS_BHANDLE block,BOOL bak=FALSE);
    // ��������� ����� ����������� ������ �� ������
    void SplitText(char *text);

    // ������� � ��������� ��� ���� ��������
    // ��������� ������ ������� � ������� � ��� ���������
    void Setup_FillFuncList(RDS_HOBJECT win,int funcfield,int textfield,int funcid,int numfield,int *pIndex=NULL);
    // �������� � ���� RDS ���� ��� ������ �������
    void Setup_AddFuncSelectFields(RDS_HOBJECT win,char *name,int tabid,int basefield,int funcid,BOOL allowchange,
                                   DWORD flags,char *connvar=NULL,int mlheight=-1);
    // ����� Setup_AddFuncSelectFields
    #define L57TF_CHANGE  1 // ������ ��������� �������
    #define L57TF_ADD     2 // ������ ���������� �������
    #define L57TF_DELETE  4 // ������ �������� �������
    #define L57TF_ALLBUTTONS (L57TF_CHANGE|L57TF_ADD|L57TF_DELETE)
    #define L57TF_LINE    8 // ����� ����� ����� �����
    // ������ ������ ������� �� ����� ��� ���������� �� ������� ������������
    BOOL Setup_ReadFuncFile(RDS_BHANDLE block,BOOL allowchange,TL57TextFileFuncDescr listfunc,RDS_HOBJECT win=NULL);
    // ������ ������ ������� � ����
    BOOL Setup_WriteFuncFile(RDS_BHANDLE block,BOOL dobak,int force=0);
    // ����� ���� ���������� ��� �������������� �������
    TL57TextFunc * Setup_AddOrEditFunc(int index,int mlheight=-1);
    // ������� ��������� ������ ���������� ����
    BOOL Setup_WindowCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data);

    // ��������� ������� ����� � ����� �����
    static BOOL CheckConnectionForBlockVar(RDS_BHANDLE Block,int num);
    static BOOL CheckConnectionForBlockVar(RDS_BHANDLE Block,char *name);

    TL57TextFile(void)
      {Handle=INVALID_HANDLE_VALUE;Text=NULL;FirstLine=NULL;FirstFunc=NULL;Changed=0;
       FirstField=NULL;
       CanEditFuncId=FALSE;
      };
    ~TL57TextFile()
      { Close(); ClearText();ClearFunc();
        ClearFields();
      };
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------
// ������� ����
void TL57TextFile::Close(void)
{
  if(Handle!=INVALID_HANDLE_VALUE)
    { CloseHandle(Handle);
      Handle=INVALID_HANDLE_VALUE;
    }
}
//---------------------------------------------------------------------------

// ������� �� ������
int TL57TextFile::OpenWrite(char *path)
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

// ������� �� ������
int TL57TextFile::OpenRead(char *path)
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

// �������� ����������� �����
void TL57TextFile::ClearText(void)
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

// �������� ������ �������
void TL57TextFile::ClearFunc(void)
{ TL57TextFunc *f,*fn;

  f=FirstFunc;
  while(f)
    { fn=f->Next;
      delete f;
      f=fn;
    }
  FirstFunc=LastFunc=NULL;
}
//---------------------------------------------------------------------------


#define Windows_ChunkSizeMax (1 << 22) // ������/������ �� ������ �����
// ������ � ����
int TL57TextFile::_WindowsWrite(const void *data,size_t *size)
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
int TL57TextFile::WindowsWrite(const void *data,size_t size)
{ size_t act_write=size;
  if(!_WindowsWrite(data,&act_write))
    return 0/*false*/;
  return act_write==size;
}

// ������ �� �����
int TL57TextFile::WindowsRead(void *data, size_t *size)
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
int TL57TextFile::WindowsRead(void *data, size_t size)
{ size_t act_read=size;
  if(!WindowsRead(data,&act_read))
    return 0/*false*/;
  return act_read==size;
}
#undef Windows_ChunkSizeMax
//---------------------------------------------------------------------------

// �������� ������ ����� (��������)
int TL57TextFile::WindowsGetFileSizeShort(DWORD *psize,int *pTooLong)
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

// ������� �������� ���� � ����������� ���������� �����
// path       - ����
// psize      - ������������ ������
char * TL57TextFile::ReadFileToMemory(char *path,DWORD *psize,int addzerobyte)
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

  // ������ ����
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

// �������� ������
int TL57TextFile::WriteLine(char *line,int crlf)
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

// ��������� Text �� ������ (������ ������ ������ ���� ������)
void TL57TextFile::SplitTextServ(void)
{ char *s;
  TL57TextFileLine *curline,*l;

  if(!Text)
    return;
  s=Text;
  for(;;)
    { if((*s)==0) // ����� ��������
        break;
      // ��������� ������
      l=new TL57TextFileLine();
      l->Line=s;
      if(FirstLine)
        { curline->Next=l;
          curline=l;
        }
      else
        FirstLine=curline=l;
      // ���� ����� ������
      while( (*s)!=0 && (*s)!='\r' && (*s)!='\n')
        s++;
      if((*s)==0)  // ����� ��������
        break;
      *s=0; // ������ ������� ���� � �����
      s++;
      if((*s)=='\r'||(*s)=='\n') // ��� ����� ������ ������
        s++;
    }
}
//---------------------------------------------------------------------------

// ��������� ����� ����������� ������ �� ������
void TL57TextFile::SplitText(char *text)
{ int l;
  // �������� ����������� �����
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

// ������� ���� � ��������� �� ������
BOOL TL57TextFile::ReadAndSplit(char *path)
{ // �������� ����������� �����
  ClearText();
  Text=ReadFileToMemory(path,NULL,1);
  if(!Text)
    return FALSE;
  SplitTextServ();
  return TRUE;
}
//---------------------------------------------------------------------------

// ����� � ������ ������
TL57TextFileLine * TL57TextFile::FindLine(TL57TextFileLine *from,char *text,BOOL skipspaces)
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

// �������� ������ ��� ����� ������ ����� (������������, ���. rdsFree)
char *TL57TextFile::GetModelFileName(RDS_BHANDLE block)
{ RDS_PCOMPMODELDATA model;
  RDS_PCOMPMODULEDATA module;
  if(!block)
    return NULL;
  model=rdscompGetBlockModelData(block);
  if(!model)
    return NULL;
  // � ����� ���� ������
  return rdsGetFullFilePath(model->ModelName,NULL,NULL);
}
//---------------------------------------------------------------------------

// ������� ���� ������ ����� � ��������� �� �������
BOOL TL57TextFile::ReadAndParseModel(RDS_BHANDLE block,BOOL message)
{ char *path=GetModelFileName(block);
  bool ok;
  ClearText();
  ClearFunc();
  if(path)
    { ok=ReadAndParse(path);
      rdsFree(path);
    }
  else
    ok=FALSE;
  if((!ok) && message)
    rdsMessageBox("���� ������ ����� ����������� ��� ����� �������� ������","�������� �������",MB_ICONERROR|MB_OK);
  return ok;
}
//---------------------------------------------------------------------------


// ������� ���� � ��������� �� �������
BOOL TL57TextFile::ReadAndParse(char *path)
{ int n;
  char *aux;
  TL57TextFunc *func;
  TL57TextFileLine *line;
  // ��������� �����
  char casesbegin[]="L57_FUNCTIONS_CASES_BEGIN_MARKER";
  int casesbeginlen=strlen(casesbegin);
  char casesend[]="L57_FUNCTIONS_CASES_END_MARKER";
  int casesendlen=strlen(casesend);
  char ifsbegin[]="L57_FUNCTIONS_IFS_BEGIN_MARKER";
  int ifsbeginlen=strlen(ifsbegin);
  char ifsend[]="L57_FUNCTIONS_IFS_END_MARKER";
  int ifsendlen=strlen(ifsend);
  char listbegin[]="L57_FUNCTIONS_LIST_BEGIN_MARKER";
  int listbeginlen=strlen(listbegin);
  char listend[]="L57_FUNCTIONS_LIST_END_MARKER";
  int listendlen=strlen(listend);

  ClearFunc();
  if(!ReadAndSplit(path))
    return FALSE;
  // ����� ������ �� ������

  // ������ ������ ������ ���� "$BC55MODEL..."
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

  // ���� "$HIDDEN"
  line=FindLine(NULL,"$HIDDEN",FALSE);
  if(line)
    { line->Id=LINEID_HIDDEN;
      line=line->Next;
    }
  else // ��� ������� ������
     { line=FindLine(NULL,"$OPTIONS",FALSE);
       if(line)
         { line->Id=LINEID_OPTIONS;
           line=NULL;
         }
     }
  // � line - ������ ������ �������� �������, ��� NULL, ���� �������� ���

  // ������������� ��������� � ���������� �� ���������� �������
  for(TL57TextFileLine *l=FirstLine;l!=NULL;l=l->Next)
    { char *s=l->Line;
      if(!s)
        continue;
      while((*s)==' ' || (*s)=='\t' || (*s)=='/')
        s++;
      if(strncmp(s,casesbegin,casesbeginlen)==0)
        l->Id=LINEID_CASESBEGIN;
      else if(strncmp(s,casesend,casesendlen)==0)
        l->Id=LINEID_CASESEND;
      else if(strncmp(s,ifsbegin,ifsbeginlen)==0)
        l->Id=LINEID_IFSBEGIN;
      else if(strncmp(s,ifsend,ifsendlen)==0)
        l->Id=LINEID_IFSEND;
      else if(strncmp(s,listbegin,listbeginlen)==0)
        l->Id=LINEID_LISTBEGIN;
      else if(strncmp(s,listend,listendlen)==0)
        l->Id=LINEID_LISTEND;
    }

  // ������ �������
  for(TL57TextFileLine *l=line;l!=NULL;l=l->Next)
    { char *s=l->Line;
      if(!s)
        continue;
      if((*s)=='$') // ����� ������ (������ ��������� ������)
        break;
      // ������ �����-�� �������
      while((*s)==' ' || (*s)=='\t')
        s++;
      switch(*s)
        { case '&': // Id ������� (���� ������)
            n=atoi(s+1);
            func=new TL57TextFunc();
            func->Num=n;
            if(FirstFunc)
              { LastFunc->Next=func;
                LastFunc=func;
              }
            else
              FirstFunc=LastFunc=func;
            break;
          case '*': // ��� ������� (���� ������)
            aux=rdsProcessText(s+1,RDS_PT_STRINGTOTEXT,NULL);
            if(!aux)
              break;
            if(LastFunc!=NULL && aux!=NULL)
              LastFunc->Name.Assign(aux);
            rdsFree(aux);
            break;
          case '|': // ������ ������ �������
            aux=rdsProcessText(s+1,RDS_PT_STRINGTOTEXT,NULL);
            if(LastFunc!=NULL && aux!=NULL)
              { if(LastFunc->Code.IsNotEmpty())
                  LastFunc->Code.AddString("\n");
                LastFunc->Code.AddString(aux);
              }
            rdsFree(aux);
            break;
        }
    }
  // ����������� ����� �������� � ������
  return TRUE;
}
//---------------------------------------------------------------------------

// �������� ���� �� ������ ������� � ������ �����
int TL57TextFile::SaveFileToModel(RDS_BHANDLE block,BOOL bak)
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
        { // ������ �� ������� - �������� �������
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

// �������� ���� �� ������ �������
int TL57TextFile::SaveFile(char *path)
{ int ok=1,funcwritten=0;
  TL57TextFileLine *l=FirstLine;

  if(!OpenWrite(path))
    return 0;

  while(ok && l!=NULL)
    switch(l->Id)
      { case LINEID_HIDDEN: // ������ ������� ������ - ����� ���� ������ ����� ������ �������
          if((!funcwritten) && FirstFunc!=NULL) // ������� ��� �� �������� � ���� ��� ������
            { ok=ok && WriteLine(l->Line,1);
              // ������
              ok=ok && WriteFuncData();
              funcwritten=1;
            }
          // ������� �������� - ���������� ��� �� ������ ��������� ������
          l=l->Next;
          while(l)
            { if(l->Line!=NULL && l->Line[0]=='$')
                break;
              l=l->Next;
            }
          // ������ l - ������ ��������� ������
          break;

        case LINEID_OPTIONS: // ������ ����� ������
          if((!funcwritten) && FirstFunc!=NULL) // ������� ��� �� �������� � ���� ��� ������
            { ok=ok && WriteLine("$HIDDEN",1);
              // ������
              ok=ok && WriteFuncData();
              funcwritten=1;
            }
          ok=ok && WriteLine(l->Line,1); // $OPTIONS
          l=l->Next;
          break;

        case LINEID_CASESBEGIN: // case ��������� �������
          ok=ok && WriteLine(l->Line,1);
          ok=ok && WriteFuncCases();
          // ������� �� ��������� �������
          while(l)
            { if(l->Id==LINEID_CASESEND)
                break;
              l=l->Next;
            }
          // ������ l - �������� ������ ������� (��� ����� ��������)
          break;

        case LINEID_IFSBEGIN: // if ��������� �������
          ok=ok && WriteLine(l->Line,1);
          ok=ok && WriteFuncIfs();
          // ������� �� ��������� �������
          while(l)
            { if(l->Id==LINEID_IFSEND)
                break;
              l=l->Next;
            }
          // ������ l - �������� ������ ������� (��� ����� ��������)
          break;

        case LINEID_LISTBEGIN: // ������� ������������ �������
          ok=ok && WriteLine(l->Line,1);
          ok=ok && WriteFuncList();
          // ������� �� ��������� �������
          while(l)
            { if(l->Id==LINEID_LISTEND)
                break;
              l=l->Next;
            }
          // ������ l - �������� ������ ������� (��� ����� ��������)
          break;

        default: // ������ �� ��������������
          ok=ok && WriteLine(l->Line,1);
          l=l->Next;
      }
  return ok;
}
//---------------------------------------------------------------------------

// �������� ������ ������� � �������� ����
int TL57TextFile::WriteFuncData(void)
{ TL57TextFile splitter;
  int ok=1;

  for(TL57TextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
    { // Id
      TL57TextString num;
      char *aux;
      num.SetInt(f->Num);
      ok=ok && WriteLine("&");
      ok=ok && WriteLine(num.c_str(),1);
      // ���
      aux=rdsProcessText(f->Name.c_str(),RDS_PT_TEXTTOSTRINGSL,NULL);
      ok=ok && WriteLine("*");
      if(aux)
        { ok=ok && WriteLine(aux);
          rdsFree(aux);
        }
      ok=ok && WriteLine(NULL,1);
      // �����
      splitter.SplitText(f->Code.c_str());
      // ����� ������ �������� �� ������ � splitter
      for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
        if(l->Line)
          { aux=rdsProcessText(l->Line,RDS_PT_TEXTTOSTRINGSL,NULL);
            if(aux)
              { ok=ok && WriteLine("|");
                ok=ok && WriteLine(aux,1);
                rdsFree(aux);
              }
          }
    } // for(TL57TextFunc *f=...)
  return ok;
}
//---------------------------------------------------------------------------

// �������� case ���������� ������� � �������� ����
// (switch �� ������������)
int TL57TextFile::WriteFuncCases(void)
{ int ok=1;
  int empty=1;
  TL57TextFile splitter;

  ok=ok && WriteLine(AUTOTEXT_START,1);

  // ���� �� �� ������
  for(TL57TextFunc *f=FirstFunc;f!=NULL;f=f->Next)
    if(f->Code.IsNotEmpty())
      { empty=0;
        break;
      }
  if(empty)
    { // ��� �� ����� ������� - ������ ����� "������" case �� ������ ������
      ok=ok && WriteLine("case 0:",1);
      ok=ok && WriteLine("  break;",1);
      ok=ok && WriteLine(AUTOTEXT_END,1);
      return ok;
    }

  // ���� �������� �������

  ok=ok && WriteLine("// ���������� �������: case",1);
  for(TL57TextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
    if(f->Code.IsNotEmpty())
      { TL57TextString num;
        num.SetInt(f->Num);
        ok=ok && WriteLine("case ");
        ok=ok && WriteLine(num.c_str());
        ok=ok && WriteLine(":",1);
        ok=ok && WriteLine("  {",1);
        splitter.SplitText(f->Code.c_str());
        // ����� ������ �������� �� ������
        for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
          if(l->Line)
            { ok=ok && WriteLine("    ");
              ok=ok && WriteLine(l->Line,1);
            }
        ok=ok && WriteLine("  ; /* */",1);
        ok=ok && WriteLine("  }",1);
        ok=ok && WriteLine("  break;",1);
      }
  ok=ok && WriteLine(AUTOTEXT_END,1);

  return ok;
}
//---------------------------------------------------------------------------

// �������� ����� if ���������� ������� � �������� ����
// ��������� ����� L57VAR_IFCALC � ������ ���������� ��� ������� if
int TL57TextFile::WriteFuncIfs(void)
{ int ok=1;
  int empty=1,first;
  TL57TextFile splitter;

  // ���� �� �� ������
  for(TL57TextFunc *f=FirstFunc;f!=NULL;f=f->Next)
    if(f->Code.IsNotEmpty())
      { empty=0;
        break;
      }
  if(empty)
    return ok;

  ok=ok && WriteLine(AUTOTEXT_START,1);
  ok=ok && WriteLine("// ���������� �������: if, ���������� L57VAR_IFCALC",1);
  first=1;
  for(TL57TextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
    if(f->Code.IsNotEmpty())
      { TL57TextString num;
        num.SetInt(f->Num);
        if(!first)
          ok=ok && WriteLine("else ");
        first=0;
        ok=ok && WriteLine("if(L57VAR_IFCALC==");
        ok=ok && WriteLine(num.c_str());
        ok=ok && WriteLine(")",1);
        ok=ok && WriteLine("  {",1);
        splitter.SplitText(f->Code.c_str());
        // ����� ������ �������� �� ������
        for(TL57TextFileLine *l=splitter.FirstLine;l!=NULL;l=l->Next)
          if(l->Line)
            { ok=ok && WriteLine("    ");
              ok=ok && WriteLine(l->Line,1);
            }
        ok=ok && WriteLine("  ; /* */",1);
        ok=ok && WriteLine("  }",1);
      }
  ok=ok && WriteLine(AUTOTEXT_END,1);

  return ok;
}
//---------------------------------------------------------------------------

// �������� ������� �������� ��������� � ������ ������� � �������� ����
// ��������� ����� L57FUNC_LIST � ������ ������������ �������
int TL57TextFile::WriteFuncList(void)
{ int ok=1,count,first,n;
  TL57TextFile splitter;

  count=0;
  for(TL57TextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
    count++;

  ok=ok && WriteLine(AUTOTEXT_START,1);
  ok=ok && WriteLine("// ������ �������",1);
  ok=ok && WriteLine("#ifdef L57FUNC_LIST",1);
  ok=ok && WriteLine("int L57FUNC_LIST(int num,int *pId,char **pName,char **pCode)",1);
  ok=ok && WriteLine("{",1);
  if(count==0) // ��� �������
    ok=ok && WriteLine(" return 0;",1);
  else
    { TL57TextString aux;
      // ������ ���������������
      ok=ok && WriteLine("  static int Ids[]={");
      first=1;
      n=0;
      for(TL57TextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
        { if(!first)
            ok=ok && WriteLine(",");
          else
            first=0;
          n++;
          if(n>10)
            { ok=ok && WriteLine("\r\n    ");
              n=0;
            }
          aux.SetInt(f->Num);
          ok=ok && WriteLine(aux.c_str());
        }
      ok=ok && WriteLine("};",1);
      // ������ ����
      ok=ok && WriteLine("  static char *Names[]={",1);
      first=1;
      for(TL57TextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
        { char *s;
          if(!first)
            ok=ok && WriteLine(",\r\n");
          else
            first=0;
          s=rdsProcessText(f->Name.c_str(),RDS_PT_TEXTTOSTRING,NULL);
          ok=ok && WriteLine("    \"");
          ok=ok && WriteLine(s);
          ok=ok && WriteLine("\"");
          rdsFree(s);
        }
      ok=ok && WriteLine("};",1);
      // ������ ��������� ������
      ok=ok && WriteLine("  static char *Codes[]={",1);
      first=1;
      for(TL57TextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
        { char *s;
          int firstline=1;
          if(!first)
            ok=ok && WriteLine(",\r\n");
          else
            first=0;
          splitter.SplitText(f->Code.c_str());
          // ����� ������ �������� �� ������
          for(TL57TextFileLine *l=splitter.FirstLine;ok && l!=NULL;l=l->Next)
            if(l->Line)
              { if(firstline)
                  ok=ok && WriteLine("    \"");
                else
                  ok=ok && WriteLine("\r\n    \"");
                firstline=0;
                s=rdsProcessText(l->Line,RDS_PT_TEXTTOSTRING,NULL);
                ok=ok && WriteLine(s);
                rdsFree(s);
                if(l->Next) // ���� ��� ������
                  ok=ok && WriteLine("\\n\"");
                else
                  ok=ok && WriteLine("\"");
              }
        }
      ok=ok && WriteLine("};",1);
      // ��������
      aux.SetInt(count);
      ok=ok && WriteLine("  if(num<0 || num>=");
      ok=ok && WriteLine(aux.c_str());
      ok=ok && WriteLine(") return 0;",1);
      ok=ok && WriteLine("  if(pId) *pId=Ids[num];",1);
      ok=ok && WriteLine("  if(pName) *pName=Names[num];",1);
      ok=ok && WriteLine("  if(pCode) *pCode=Codes[num];",1);
      ok=ok && WriteLine("  return 1;",1);
    }
  ok=ok && WriteLine("}",1);
  ok=ok && WriteLine("#endif",1);
  ok=ok && WriteLine(AUTOTEXT_END,1);
  return ok;
}
//---------------------------------------------------------------------------

// ������� �� ��������������
TL57TextFunc * TL57TextFile::FuncById(int id)
{ for(TL57TextFunc *f=FirstFunc;f!=NULL;f=f->Next)
    if(f->Num==id)
      return f;
  return NULL;
}
//---------------------------------------------------------------------------

// ������� �� ������
TL57TextFunc * TL57TextFile::FuncByIndex(int index)
{ int n=0;
  for(TL57TextFunc *f=FirstFunc;f!=NULL;f=f->Next,n++)
    if(n==index)
      return f;
  return NULL;
}
//---------------------------------------------------------------------------

// ������������� ������� �� ������
int TL57TextFile::FuncNumByIndex(int index,int defaultval)
{ TL57TextFunc *f=FuncByIndex(index);
  if(f)
    return f->Num;
  return defaultval;
}
//---------------------------------------------------------------------------

// ����� ������������ ������������� �������
int TL57TextFile::FindMaxFuncId(void)
{ int maxid=-1;
  for(TL57TextFunc *f=FirstFunc;f!=NULL;f=f->Next)
    if(f->Num>maxid)
      maxid=f->Num;
  return maxid;
}
//---------------------------------------------------------------------------

// ������� ������� �� ������
void TL57TextFile::DeleteFuncByIndex(int index)
{ int n=0;
  TL57TextFunc *prev=NULL;
  for(TL57TextFunc *f=FirstFunc;f!=NULL;f=f->Next,n++)
    { if(n==index)
        { if(prev)
            prev->Next=f->Next;
          else
            FirstFunc=f->Next;
          delete f;
          break;
        }
      prev=f;
    }
  if(FirstFunc)
    for(LastFunc=FirstFunc;LastFunc->Next!=NULL;LastFunc=LastFunc->Next);
  else
    LastFunc=NULL;
}
//---------------------------------------------------------------------------

// ����� �������
TL57TextFunc * TL57TextFile::AddFunction(int findnum)
{ TL57TextFunc *f=new TL57TextFunc();
  if(findnum)
    f->Num=FindMaxFuncId()+1;
  if(FirstFunc)
    { LastFunc->Next=f;
      LastFunc=f;
    }
  else
    FirstFunc=LastFunc=f;
  return f;
}
//---------------------------------------------------------------------------

// ��������� ������ ������� ��� ������ ������� ������� ��������
void TL57TextFile::FillByFunc(TL57TextFileFuncDescr func)
{ int num,cnt;
  char *name,*code;
  ClearFunc();
  if(!func)
    return;
  cnt=0;
  for(;;)
    { TL57TextFunc *f;
      if(! ((func)(cnt,&num,&name,&code)) )
        break;
      cnt++;
      f=AddFunction(0);
      f->Num=num;
      f->Name.Assign(name);
      f->Code.Assign(code);
    }
}
//---------------------------------------------------------------------------

// �������� ������ �����
void TL57TextFile::ClearFields(void)
{ TL57TextFileField *f,*fn;

  f=FirstField;
  while(f)
    { fn=f->Next;
      delete f;
      f=fn;
    }
  FirstField=NULL;
}
//---------------------------------------------------------------------------

// ��������� ������ ������� � ������� � ��� ���������
// win        - ����
// funcfield  - ������������� ���� ������ ������� (���������� ������)
// textfield  - ������������� ���� ��������� ������ ������� (������������� ����) ��� -1
// funcid     - ������������� �������
// numfield   - ������������� ���� �������������� ������� ��� -1
// pIndex     - ������������ ������ �������������� ��� NULL
void TL57TextFile::Setup_FillFuncList(RDS_HOBJECT win,int funcfield,int textfield,int funcid,int numfield,int *pIndex)
{ TL57TextString str;
  int curnum=-1,n=0;
  TL57TextFunc *cur_f=NULL;

  if(pIndex) *pIndex=-1;

  if(!win)
    return;

  // ���������� ������ ���� ������� (�� �������)
  for(TL57TextFunc *f=FirstFunc;f!=NULL;f=f->Next,n++)
    { if(str.IsNotEmpty())
        str.AddString("\n");
      str.AddString(f->Name.c_str());
      if(f->Num==funcid)
        { curnum=n;
          cur_f=f;
        }
    }
  rdsFORMSetComboList(win,funcfield,str.c_str());
  rdsSetObjectInt(win,funcfield,RDS_FORMVAL_VALUE,curnum);
  if(numfield>=0)
    { if(funcid>=0 && cur_f!=NULL)
        rdsSetObjectInt(win,numfield,RDS_FORMVAL_VALUE,funcid);
      else
        rdsSetObjectStr(win,numfield,RDS_FORMVAL_VALUE,"");
    }
  if(pIndex) *pIndex=curnum;

  if(textfield>=0)
    rdsSetObjectStr(win,textfield,RDS_FORMVAL_VALUE,cur_f?cur_f->Code.c_str():"");
}
//---------------------------------------------------------------------------

// �������� � ���� RDS ���� ��� ������ �������
// win         - ����
// name        - ����� ����� ���� ��� NULL
// tabid       - ������������� ������� ����
// basefield   - ��������� ����� ������������ ���� �����
// funcid      - ������������� �������, ������� ������ ���� ������� ������� � ������
// allowchange - ����� �� �������� ������� (����� �� ������ "��������")
// flags       - �����
// mlheight    - ������ �������������� ���� � ������� ������� ��� -1 (�� ��������� 3*24)
// ����������� ����:
//     basefield+6 - �������������� � ������������� ����� (���� ����)
//     basefield   - ���������� ������ ������ �������
//     basefield+5 - ����� ������������� ������� ��� ��������� (���� �������� � �������)
//     basefield+1 - ������������� ���� � ������� ������� (������ ������)
//     basefield+2 - ������ ��������� (���� ���������)
//     basefield+3 - ������ ���������� (���� ���������)
//     basefield+4 - ������ �������� (���� ���������)
void TL57TextFile::Setup_AddFuncSelectFields(RDS_HOBJECT win,char *name,int tabid,int basefield,int funcid,
                                             BOOL allowchange,DWORD flags,char *connvar,int mlheight)
{ int textid=basefield+1,chg_id=-1,add_id=-1,del_id=-1,num_id=-1;
  DWORD ml_line=0,chg_line=0,add_line=0,del_line=0;
  int index;
  if(!win)
    return;

   if(flags & L57TF_LINE)
    { if(flags & L57TF_DELETE) del_line=RDS_FORMFLAG_LINE;
      else if(flags & L57TF_ADD) add_line=RDS_FORMFLAG_LINE;
      else if(flags & L57TF_CHANGE) chg_line=RDS_FORMFLAG_LINE;
      else ml_line=RDS_FORMFLAG_LINE;
    }
  else if(flags & L57TF_DELETE)
    { if(flags & L57TF_ADD) add_line=RDS_FORMFLAG_LINE;
      else if(flags & L57TF_CHANGE) chg_line=RDS_FORMFLAG_LINE;
      else ml_line=RDS_FORMFLAG_LINE;
    }

  if(connvar)
    { TL57TextString str;
      int fid=basefield+6;
      str.Assign("������������� ������� ��������� � ���� ���� �� �����, ������������ � ����� \"");
      str.AddString(connvar);
      str.AddString("\". ����� ����� ���� ������� ������� � ���������� �����, ���������� ��������� ��� �����.");
      rdsFORMAddEdit(win,tabid,fid,RDS_FORMCTRL_MULTILABEL,str.c_str(),0);
      rdsSetObjectInt(win,fid,RDS_FORMVAL_MLHEIGHT,60);
      rdsSetObjectInt(win,fid,RDS_FORMVAL_MLABELBEVEL,1);
    }

  rdsFORMAddEdit(win,tabid,basefield,RDS_FORMCTRL_COMBOLIST,name?name:"���:",400);

  if(CanEditFuncId)
    { num_id=basefield+5;
      rdsFORMAddEdit(win,tabid,num_id,RDS_FORMCTRL_DISPLAY|RDS_FORMFLAG_LINE,"������������� ������� (��� ����� �����):",40);
    }

  rdsFORMAddEdit(win,tabid,textid,RDS_FORMCTRL_MULTILINE|ml_line,
    allowchange?"������� (������� \"�������� �������\" ��� ��������������):":
    "�������� ����� �������:",80);
  rdsFORMSetMultilineHeight(win,textid,mlheight<=0?(3*24):mlheight);
  rdsSetObjectInt(win,textid,RDS_FORMVAL_MLREADONLY,1);
  rdsSetObjectInt(win,textid,RDS_FORMVAL_MLRETURNS,1);

  if((flags & L57TF_CHANGE) && allowchange)
    { chg_id=basefield+2;
      rdsFORMAddEdit(win,tabid,chg_id,RDS_FORMCTRL_BUTTON|chg_line,"",80);
      rdsSetObjectStr(win,chg_id,RDS_FORMVAL_VALUE,"�������� �������");
    }
  if((flags & L57TF_ADD) &&  allowchange)
    { add_id=basefield+3;
      rdsFORMAddEdit(win,tabid,add_id,RDS_FORMCTRL_BUTTON|add_line,"",80);
      rdsSetObjectStr(win,add_id,RDS_FORMVAL_VALUE,"�������� �������");
    }

  if((flags & L57TF_DELETE) &&  allowchange)
    { del_id=basefield+4;
      rdsFORMAddEdit(win,tabid,del_id,RDS_FORMCTRL_BUTTON|del_line,"",80);
      rdsSetObjectStr(win,del_id,RDS_FORMVAL_VALUE,"������� �������");
    }

  Setup_FillFuncList(win,basefield,textid,funcid,num_id,&index);

  if(chg_id>=0)
    rdsSetObjectInt(win,chg_id,RDS_FORMVAL_ENABLED,index>=0);
  if(del_id>=0)
    rdsSetObjectInt(win,del_id,RDS_FORMVAL_ENABLED,index>=0);

  TL57TextFileField *f=new TL57TextFileField();
  f->Next=FirstField;
  FirstField=f;
  f->ListId=basefield;
  f->TextId=textid;
  f->ChgButtonId=chg_id;
  f->AddButtonId=add_id;
  f->DelButtonId=del_id;
  f->FuncIdToSelect=funcid;
  f->NumId=num_id;
}
//---------------------------------------------------------------------------

// ������ ������ ������� �� ����� ��� ���������� �� ������� ������������
// block       - ����, �� ������ �������� ��������
// allowchange - ����� �� �������� ������� (������ �� ����� - 1, �� ������� - 0)
// listfunc    - ������� ������������
// win         - ������� ������, � ������� ������������ ��������� �� ���� ����
// caneditfuncid - ����� �� ������������ ������/�������� ����� ������������� �������
BOOL TL57TextFile::Setup_ReadFuncFile(RDS_BHANDLE block,BOOL allowchange,TL57TextFileFuncDescr listfunc,RDS_HOBJECT win)
{ if(allowchange)
    { if(!ReadAndParseModel(block,TRUE))
        return FALSE;
    }
  else
    FillByFunc(listfunc);
  Changed=0;
  if(win)
    rdsSetObjectPtr(win,0,NULL,this);
  return TRUE;
}
//---------------------------------------------------------------------------

// ������ ������ ������� � ����
// block       - ����, � ������ �������� �������
// dobak       - ������� ��������� �����
// force       - ������, ���� ���� Changed==0
BOOL TL57TextFile::Setup_WriteFuncFile(RDS_BHANDLE block,BOOL dobak,int force)
{
  if(force)
    Changed=1;
  if(Changed)
    { int res=SaveFileToModel(block,dobak);
      if(res)
        Changed=0;
      return res?TRUE:FALSE;
    }
  return FALSE;
}
//---------------------------------------------------------------------------

// ����� ���� ���������� ��� �������������� �������
// index - ����� ������� � ������
TL57TextFunc * TL57TextFile::Setup_AddOrEditFunc(int index,int mlheight)
{ RDS_HOBJECT win=rdsFORMCreate(FALSE,-1,-1,
                  index<0?"���������� �������":"��������� �������");
  BOOL res;
  TL57TextFunc *func=NULL;
  if(!win)
    return NULL;
  rdsFORMAddEdit(win,1,1,RDS_FORMCTRL_EDIT,"���:",400);

  if(CanEditFuncId)
    { rdsFORMAddEdit(win,1,3,RDS_FORMCTRL_UPDOWN,"������������� ������� (��� ����� �����):",60);
      rdsSetObjectInt(win,3,RDS_FORMVAL_UPDOWNMIN,0);
      rdsSetObjectInt(win,3,RDS_FORMVAL_UPDOWNINC,1);
    }


  rdsFORMAddEdit(win,1,2,RDS_FORMCTRL_MULTILINE,"�������:",80);
  rdsSetObjectInt(win,2,RDS_FORMVAL_MLRETURNS,1);
  rdsFORMSetMultilineHeight(win,2,mlheight<=0?3*24:mlheight);

  rdsFORMAddEdit(win,1,100,RDS_FORMCTRL_MULTILABEL,
  "���������� ������ ��������(�) ���������� ������� � ���������� ����� C. "
  "������� ���������� ������� - \"x\", \"y\" � \"z\". "
  "����������� �������� ������� ���� ������������� ���������� \"f\", "
  "���� ������������ ���������� return.",0);
  rdsSetObjectInt(win,100,RDS_FORMVAL_MLHEIGHT,75);
  rdsSetObjectInt(win,100,RDS_FORMVAL_MLABELBEVEL,1);

  if(index>=0) // ��������������
    func=FuncByIndex(index);

  if(func)
    { rdsSetObjectStr(win,1,RDS_FORMVAL_VALUE,func->Name.c_str());
      rdsSetObjectStr(win,2,RDS_FORMVAL_VALUE,func->Code.c_str());
      if(CanEditFuncId)
        rdsSetObjectInt(win,3,RDS_FORMVAL_VALUE,func->Num);
    }
  else if(CanEditFuncId)
    rdsSetObjectInt(win,3,RDS_FORMVAL_VALUE,FindMaxFuncId()+1); 

  for(;;)
    { res=rdsFORMShowModalServ(win,NULL);
      if(res)
         { char *name=rdsGetObjectStr(win,1,RDS_FORMVAL_VALUE);
           char *code=rdsGetObjectStr(win,2,RDS_FORMVAL_VALUE);
           bool badnum=FALSE;
           int num=-1;
           if(CanEditFuncId)
             { num=rdsGetObjectInt(win,3,RDS_FORMVAL_VALUE);
               if(func==NULL || func->Num!=num) // ������ �� ���� ��� �� ���������
                 { for(TL57TextFunc *ff=FirstFunc;ff!=NULL;ff=ff->Next)
                     { if(ff->Num==num) // ��� �����
                         badnum=TRUE;
                     }
                 }
             }
           if(name==NULL || (*name)==0)
             { rdsMessageBox("��� ������� �� ����� ���� ������","������",MB_ICONWARNING|MB_OK);
               continue;
             }
           if(code==NULL || (*code)==0)
             { rdsMessageBox("�������� ������� �� ����� ���� ������","������",MB_ICONWARNING|MB_OK);
               continue;
             }
           if(badnum)
             { TL57TextString str,snum;
               int freenum=FindMaxFuncId()+1;
               snum.SetInt(num);
               str.Assign("������������� ");
               str.AddString(snum.c_str());
               str.AddString(" ��� ����� ������ ��������. ������������� ");
               snum.SetInt(freenum);
               str.AddString(snum.c_str());
               str.AddString(" ��������. ������������ ���?");
               if(rdsMessageBox(str.c_str(),"������",MB_ICONWARNING|MB_YESNO)!=IDYES)
                 continue;
               num=freenum;
             }


           // ��� � �������
           Changed=1;
           if(!func) // ����������
             func=AddFunction(CanEditFuncId?0:1);
           if(CanEditFuncId)
             func->Num=num;
           func->Name.Assign(name);
           func->Code.Assign(code);
         }
       else
         func=NULL;
       break;
    }
  rdsDeleteObject(win);
  return func;
}
//---------------------------------------------------------------------------

// ������� ��������� ������ ���������� ����
// �����. TRUE, ���� �������� ���������
BOOL TL57TextFile::Setup_WindowCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data)
{ TL57TextFunc *f;
  int index;
  BOOL ok=FALSE;

  switch(data->Event)
    { case RDS_FORMSERVEVENT_CHANGE:
        // ���������� �� ���� ����� ������ �������
        for(TL57TextFileField *field=FirstField;field!=NULL;field=field->Next)
          if(field->ListId>=0 && data->CtrlId==field->ListId) // ��� �������
            { // ������ ������ �����
              int index=rdsGetObjectInt(win,field->ListId,RDS_FORMVAL_VALUE);
              f=FuncByIndex(index);
              if(f)
                field->FuncIdToSelect=f->Num;
              rdsSetObjectStr(win,field->TextId,RDS_FORMVAL_VALUE,f?f->Code.c_str():"");
              if(field->NumId>=0)
                { if(f!=NULL && f->Num>=0)
                    rdsSetObjectInt(win,field->NumId,RDS_FORMVAL_VALUE,f->Num);
                  else
                    rdsSetObjectStr(win,field->NumId,RDS_FORMVAL_VALUE,"");
                }
              if(field->ChgButtonId>=0)
                rdsSetObjectInt(win,field->ChgButtonId,RDS_FORMVAL_ENABLED,index>=0);
              if(field->DelButtonId>=0)
                rdsSetObjectInt(win,field->DelButtonId,RDS_FORMVAL_ENABLED,index>=0);
            }
        break;
      case RDS_FORMSERVEVENT_CLICK:
        // ���������� �� ���� ������� ����� ������ �������
        for(TL57TextFileField *field=FirstField;field!=NULL;field=field->Next)
          { index=rdsGetObjectInt(win,field->ListId,RDS_FORMVAL_VALUE);
            f=FuncByIndex(index);
            if(ok) // �����-�� ������ ��� ����������
              continue;
            // ��������� ������
            if(field->ChgButtonId>=0 && data->CtrlId==field->ChgButtonId)
              { // ������ "��������"
                f=Setup_AddOrEditFunc(rdsGetObjectInt(win,field->ListId,RDS_FORMVAL_VALUE));
                if(f)
                  { Changed=1;
                    ok=TRUE;
                  }
              }
            else if(field->AddButtonId>=0 && data->CtrlId==field->AddButtonId)
              { // ������ "��������"
                f=Setup_AddOrEditFunc(-1);
                if(f)
                  { Changed=1;
                    ok=TRUE;
                    field->FuncIdToSelect=f->Num;
                  }
              }
            else if(field->DelButtonId>=0 && data->CtrlId==field->DelButtonId)
              { // ������ "�������"
                // � f - ��������� � ������� ���� �������
                //f=file->FuncByIndex(index=rdsGetObjectInt(win,field->ListId,RDS_FORMVAL_VALUE));
                if(f)
                  { TL57TextString str;
                    str.Assign("������� \"");
                    str.AddString(f->Name.c_str());
                    str.AddString("\" ����� ������� �� ������, ������������� ������ ������."
                                  " ������� ��?");
                    if(rdsMessageBox(str.c_str(),"��������",MB_ICONWARNING|MB_YESNO)==IDYES)
                      { DeleteFuncByIndex(index);
                        Changed=1;
                        f=FuncByIndex(index);
                        if(!f)
                          f=FuncByIndex(index-1);
                        if(f)
                          field->FuncIdToSelect=f->Num;
                        else
                          field->FuncIdToSelect=-1;
                        ok=TRUE;
                      }
                  } // if(f)
              } // else if(field->DelButtonId>=0 && ...)
          } // for(TL57TextFileField *field=FirstField;...)
        break;
    } // switch(data->Event)

  if(ok)
    { BOOL hasfunc=(FirstFunc!=NULL);
      for(TL57TextFileField *field=FirstField;field!=NULL;field=field->Next)
        { Setup_FillFuncList(win,field->ListId,field->TextId,field->FuncIdToSelect,field->NumId);
          // ���������� ������
          if(field->ChgButtonId>=0)
            rdsSetObjectInt(win,field->ChgButtonId,RDS_FORMVAL_ENABLED,hasfunc);
          if(field->DelButtonId>=0)
            rdsSetObjectInt(win,field->DelButtonId,RDS_FORMVAL_ENABLED,hasfunc);
        }
    }
  return ok;
}
//---------------------------------------------------------------------------

// ��������� ������� ����� � ����� �����
BOOL TL57TextFile::CheckConnectionForBlockVar(RDS_BHANDLE Block,int num)
{ RDS_VARDESCRIPTION vdescr;

  RDS_INITSERVSIZE(vdescr);

  // ����� ����������
  if(!rdsGetBlockVar(Block,num,&vdescr))
    return FALSE;
  return TL57TextFile::CheckConnectionForBlockVar(Block,vdescr.Name);
}
BOOL TL57TextFile::CheckConnectionForBlockVar(RDS_BHANDLE Block,char *name)
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
      // �����-�� �����
      rdsGetConnDescription(c,&cdescr);
      if(!cdescr.Active) continue;
      if(!pdescr.VarName) continue;
      if(strncmp(pdescr.VarName,name,varlen)==0)
        return TRUE;
    }
  return FALSE;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------

char TL57TextString::ZeroByte=0;

// �������� ������
void TL57TextString::Clear(void)
{
  if(Data)
    delete[] Data;
  Data=NULL;
}
//---------------------------------------------------------------------------

// �����
int TL57TextString::Length(void) const
{return Data?strlen(Data):0;}
//---------------------------------------------------------------------------

// ���������
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

// �������� � ������
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

// �������������� ����� int � ������
void TL57TextString::SetInt(int i)
{ char tempstr[100];
  //sprintf(tempstr,"%d",i);
  itoa(i,tempstr,10);
  Assign(tempstr);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ ������ �������-�������� �������
//---------------------------------------------------------------------------
// ��������
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

// �����������
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
  // ������ "�����"
  for(int i=Size;i<newsize;i++)
    { e=new TRdsLinearInterpolation();
      e->SetDefValue(0.0);
      newData[i]=e;
    }
  Array=newData;
  Size=newsize;
}
//---------------------------------------------------------------------------

// ������� (���� ��� - Dummy)
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

// �������� ��� �������
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

  // ������� �������� ���������
  return memcmp(Data1.Data,Data2.Data,Data1.Rows*Data1.Cols*sizeof(double))==0;
}
//---------------------------------------------------------------------------

// �������� ������� �� ����������� "���������", �������, �� ����, �� ������ ������� ��������� �� �����
BOOL TL57TextFuncFArray::IsBadMatrix(rdsbcppVarAncestor *M)
{ RDS_ARRAYACCESSDATA Data;
  double *arr;

  if(!M->GetAccessData(&Data))
    return FALSE;
  if(Data.Rows!=2 || Data.Cols!=2 || Data.ItemSize!=sizeof(double) ||
     rdsGetVarArrayElementType(M->GetVoidPtr())!=RDS_VARTYPE_DOUBLE ||
     Data.Data==NULL)
    return FALSE;

  // ������� 2x2
  arr=(double*)(Data.Data);
  return arr[0]==rdsbcppHugeDouble &&
         arr[1]==rdsbcppHugeDouble &&
         arr[2]==rdsbcppHugeDouble &&
         arr[3]==rdsbcppHugeDouble;
}
//---------------------------------------------------------------------------

// �������� ����������� "���������" ������� (������ ��� ������ ���� 2x2)
void TL57TextFuncFArray::SetBadMatrix(rdsbcppVarAncestor *M)
{ RDS_ARRAYACCESSDATA Data;
  double *arr;

  if(!M->GetAccessData(&Data))
    return;
  if(Data.Rows!=2 || Data.Cols!=2 || Data.ItemSize!=sizeof(double) ||
     rdsGetVarArrayElementType(M->GetVoidPtr())!=RDS_VARTYPE_DOUBLE ||
     Data.Data==NULL)
    return;

  // ������� 2x2
  arr=(double*)(Data.Data);
  arr[0]=rdsbcppHugeDouble;
  arr[1]=rdsbcppHugeDouble;
  arr[2]=rdsbcppHugeDouble;
  arr[3]=rdsbcppHugeDouble;
}
//---------------------------------------------------------------------------
#undef LINEID_HIDDEN
#undef LINEID_OPTIONS
#undef LINEID_CASESBEGIN
#undef LINEID_CASESSEND
#undef LINEID_IFSBEGIN
#undef LINEID_IFSEND
#undef LINEID_LISTBEGIN
#undef LINEID_LISTEND

