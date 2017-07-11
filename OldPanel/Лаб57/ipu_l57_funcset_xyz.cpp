//---------------------------------------------------------------------------
// �������������� ������ � ������� ��� ��������� ��������������������� �������
// ������� ���� ����������
//---------------------------------------------------------------------------

// ��������� �� ������� ��������� ��������
typedef int (*XTextFileFuncDescr)(int,int*,char**,char**);
//---------------------------------------------------------------------------
// ������ double (������ �� ������� ������)
class XTextDoubleArray
{ private:
    double *Array;
    int Count;
    double DefValue;
  public:
    inline void Setup(double *a,int cnt=0,double def=0.0){Array=a;Count=cnt;DefValue=def;};
    inline double operator[](int i)const{ return (Array!=NULL && i>=0 && i<Count)?Array[i]:DefValue; };
    inline double operator()(int i)const{ return (Array!=NULL && i>=0 && i<Count)?Array[i]:DefValue; };

    XTextDoubleArray(void){Array=NULL;Count=0;};
};
//---------------------------------------------------------------------------
// ������ �������� �������
class XTextFuncArg
{ public:
    double xval,yval,zval;
    XTextDoubleArray Params;

    XTextFuncArg(void){xval=yval=zval=0.0;};
};
//---------------------------------------------------------------------------
// ������� �������� ������ ����� (������ �� ������� ������)
class XTextFileLine
{ public:
    XTextFileLine *Next;
    char *Line;

    XTextFileLine(void){Next=NULL;Line=NULL;};
};
//---------------------------------------------------------------------------
#ifdef RDS_INTVERSION
  // ������� �������� ������ ��������������� ����� ��������
  class XTextFileField
  { public:
      XTextFileField *Next;
      int ListId,TextId,ChgButtonId,AddButtonId,DelButtonId;
      int FuncIdToSelect;

      XTextFileField(void){Next=NULL;ListId=TextId=ChgButtonId=AddButtonId=DelButtonId=FuncIdToSelect=-1;};
  };
#endif
//---------------------------------------------------------------------------
// ������
class XTextString
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
    inline void Assign(const XTextString &s){Assign(s.c_str());};
    // �������� � ������
    void AddString(char *s);

    // �������������� ����� int � ������
    void SetInt(int i);

    XTextString(void){Data=NULL;};
    ~XTextString(){Clear();};
};
//---------------------------------------------------------------------------
// ������� �������� ������ ������� (������ ��������)
class XTextFunc
{ public:
    int Num;    // ����� �������
    XTextString Name; // ��� �������
    XTextString Code; // ��������� �������

    XTextFunc *Next;

    XTextFunc(void){Next=NULL;Num=-1;};
};
//---------------------------------------------------------------------------
// ������ � ������ ���������� �����
class XTextFile
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
  public:
    HANDLE Handle;

    int Changed;

    XTextFileLine *FirstLine; // ��������� �����, �������� �� ������
    XTextFunc *FirstFunc,*LastFunc;     // �� ��, �������� �� �������

    #ifdef RDS_INTVERSION
      XTextFileField *FirstField; // ������ ��������������� ����������� ����� ����� ������ ������� � ���� ��������
      // �������� ������ �����
      void ClearFields(void);
    #endif

    // �������� ����������� �����
    void ClearText(void);
    // �������� ������ �������
    void ClearFunc(void);


    // ������� �� ��������������
    XTextFunc * FuncById(int id);
    // ������� �� ������
    XTextFunc * FuncByIndex(int index);
    // ������������� ������� �� ������
    int FuncNumByIndex(int index,int defaultval=-1);
    // ������� ������� �� ������
    void DeleteFuncByIndex(int index);
    // ����� �������
    XTextFunc * AddFunction(int findnum=1);
    // ����� ������������ ������������� �������
    int FindMaxFuncId(void);

    // ��������� ������ ������� ��� ������ ������� ������� ��������
    void FillByFunc(XTextFileFuncDescr func);


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

    // ������� ���� � ��������� �� ������
    void ReadAndSplit(char *path);
    // ��������� ����� ����������� ������ �� ������
    void SplitText(char *text);
    // ������� ���� � ��������� �� �������
    void ReadAndParse(char *path);
    // �������� ���� �� ������ �������
    int SaveFuncList(char *path);

    // ������� � ��������� ��� ���� ��������
    #ifdef RDS_INTVERSION
      // ��������� ������ ������� � ������� � ��� ���������
      void Setup_FillFuncList(RDS_HOBJECT win,int funcfield,int textfield,int funcid,int *pIndex=NULL);
      // �������� � ���� RDS ���� ��� ������ �������
      void Setup_AddFuncSelectFields(RDS_HOBJECT win,char *name,int tabid,int basefield,int funcid,BOOL allowchange,
                                     DWORD flags,int mlheight=-1);
      // ����� Setup_AddFuncSelectFields
      #define XTF_CHANGE  1 // ������ ��������� �������
      #define XTF_ADD     2 // ������ ���������� �������
      #define XTF_DELETE  4 // ������ �������� �������
      #define XTF_ALLBUTTONS (XTF_CHANGE|XTF_ADD|XTF_DELETE)
      #define XTF_LINE    8 // ����� ����� ����� �����
      // ������ ������ ������� �� ����� ��� ���������� �� ������� ������������
      void Setup_ReadFuncFile(char *filename,BOOL allowchange,XTextFileFuncDescr listfunc,RDS_HOBJECT win=NULL);
      // ������ ������ ������� � ����
      void Setup_WriteFuncFile(char *filename,char *bakfilename,int force=0);
      // ����� ���� ���������� ��� �������������� �������
      XTextFunc * Setup_AddOrEditFunc(int index);
      // ������� ��������� ������ ���������� ����
      BOOL Setup_WindowCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data);
    #endif

    XTextFile(void)
      {Handle=INVALID_HANDLE_VALUE;Text=NULL;FirstLine=NULL;FirstFunc=NULL;Changed=0;
       #ifdef RDS_INTVERSION
         FirstField=NULL;
       #endif
      };
    ~XTextFile()
      { Close(); ClearText();ClearFunc();
        #ifdef RDS_INTVERSION
          ClearFields();
        #endif
      };
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����
//---------------------------------------------------------------------------
// ������� ����
void XTextFile::Close(void)
{
  if(Handle!=INVALID_HANDLE_VALUE)
    { CloseHandle(Handle);
      Handle==INVALID_HANDLE_VALUE;
    }
}
//---------------------------------------------------------------------------

// ������� �� ������
int XTextFile::OpenWrite(char *path)
{ Close();
  Handle=CreateFile(path,GENERIC_WRITE,
                    FILE_SHARE_READ, NULL,
                    CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,NULL);
  return Handle!=INVALID_HANDLE_VALUE;
}
//---------------------------------------------------------------------------

// ������� �� ������
int XTextFile::OpenRead(char *path)
{ Close();
  Handle=CreateFile(path,GENERIC_READ,
                    FILE_SHARE_READ, NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,NULL);
  return Handle!=INVALID_HANDLE_VALUE;
}
//---------------------------------------------------------------------------

// �������� ����������� �����
void XTextFile::ClearText(void)
{ XTextFileLine *l,*ln;
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
void XTextFile::ClearFunc(void)
{ XTextFunc *f,*fn;

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
int XTextFile::_WindowsWrite(const void *data,size_t *size)
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
int XTextFile::WindowsWrite(const void *data,size_t size)
{ size_t act_write=size;
  if(!_WindowsWrite(data,&act_write))
    return 0/*false*/;
  return act_write==size;
}

// ������ �� �����
int XTextFile::WindowsRead(void *data, size_t *size)
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
int XTextFile::WindowsRead(void *data, size_t size)
{ size_t act_read=size;
  if(!WindowsRead(data,&act_read))
    return 0/*false*/;
  return act_read==size;
}
#undef Windows_ChunkSizeMax
//---------------------------------------------------------------------------

// �������� ������ ����� (��������)
int XTextFile::WindowsGetFileSizeShort(DWORD *psize,int *pTooLong)
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
char * XTextFile::ReadFileToMemory(char *path,DWORD *psize,int addzerobyte)
{ HANDLE h;
  DWORD size;
  size_t act_size;
  char *mainbuf;
  bool ok;

  Close();

  if(psize) *psize=0;

  Handle=CreateFile(path,GENERIC_READ,
               FILE_SHARE_READ, NULL,
               OPEN_EXISTING,
               FILE_ATTRIBUTE_NORMAL, NULL);
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
int XTextFile::WriteLine(char *line,int crlf)
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
void XTextFile::SplitTextServ(void)
{ char *s;
  XTextFileLine *curline,*l;
  
  if(!Text)
    return;
  s=Text;
  for(;;)
    { // ���������� �������� �����
      while( (*s)=='\r' || (*s)=='\n')
        s++;
      if((*s)==0) // ����� ��������
        break;
      // ��������� ������
      l=new XTextFileLine();
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
    }
}
//---------------------------------------------------------------------------

// ��������� ����� ����������� ������ �� ������
void XTextFile::SplitText(char *text)
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
void XTextFile::ReadAndSplit(char *path)
{ // �������� ����������� �����
  ClearText();
  Text=ReadFileToMemory(path,NULL,1);
  SplitTextServ();
}
//---------------------------------------------------------------------------

// ������� ���� � ��������� �� �������
void XTextFile::ReadAndParse(char *path)
{ int n;
  char *aux;
  XTextFunc *func;

  ClearFunc();
  ReadAndSplit(path);
  // ����� ������ �� ������
  for(XTextFileLine *l=FirstLine;l!=NULL;l=l->Next)
    { char *s=l->Line;
      if((*s)=='*') // ����� ������
        break;
      // ������ �����-�� �������
      switch(*s)
        { case '&': // Id ������� (���� ������)
            n=atoi(s+1);
            func=new XTextFunc();
            func->Num=n;
            if(FirstFunc)
              { LastFunc->Next=func;
                LastFunc=func;
              }
            else
              FirstFunc=LastFunc=func;
            break;
          case '$': // ��� ������� (���� ������)
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
  ClearText();
}
//---------------------------------------------------------------------------

// ������� �� ��������������
XTextFunc * XTextFile::FuncById(int id)
{ for(XTextFunc *f=FirstFunc;f!=NULL;f=f->Next)
    if(f->Num==id)
      return f;
  return NULL;
}
//---------------------------------------------------------------------------

// ������� �� ������
XTextFunc * XTextFile::FuncByIndex(int index)
{ int n=0;
  for(XTextFunc *f=FirstFunc;f!=NULL;f=f->Next,n++)
    if(n==index)
      return f;
  return NULL;
}
//---------------------------------------------------------------------------

// ������������� ������� �� ������
int XTextFile::FuncNumByIndex(int index,int defaultval)
{ XTextFunc *f=FuncByIndex(index);
  if(f)
    return f->Num;
  return defaultval;
}
//---------------------------------------------------------------------------

// ����� ������������ ������������� �������
int XTextFile::FindMaxFuncId(void)
{ int maxid=-1;
  for(XTextFunc *f=FirstFunc;f!=NULL;f=f->Next)
    if(f->Num>maxid)
      maxid=f->Num;
  return maxid;
}
//---------------------------------------------------------------------------

// ������� ������� �� ������
void XTextFile::DeleteFuncByIndex(int index)
{ int n=0;
  XTextFunc *prev=NULL;
  for(XTextFunc *f=FirstFunc;f!=NULL;f=f->Next,n++)
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
XTextFunc * XTextFile::AddFunction(int findnum)
{ XTextFunc *f=new XTextFunc();
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
void XTextFile::FillByFunc(XTextFileFuncDescr func)
{ int num,cnt;
  char *name,*code;
  ClearFunc();
  if(!func)
    return;
  cnt=0;
  for(;;)
    { XTextFunc *f;
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

// �������� ���� �� ������ �������
int XTextFile::SaveFuncList(char *path)
{ int ok=1;
  int first,count,n;
  if(!OpenWrite(path))
    return 0;

  ok=ok && WriteLine("/* ����� ������� - ����������� 57",1);
  ok=ok && WriteLine("���� �� ��������!",1);

  // �������
  count=0;
  for(XTextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
    { // Id
      XTextString num;
      num.SetInt(f->Num);
      ok=ok && WriteLine("&");
      ok=ok && WriteLine(num.c_str(),1);
      count++;
      // ���
      char *aux=rdsProcessText(f->Name.c_str(),RDS_PT_TEXTTOSTRINGSL,NULL);
      ok=ok && WriteLine("$");
      if(aux)
        { ok=ok && WriteLine(aux);
          rdsFree(aux);
        }
      ok=ok && WriteLine(NULL,1);
      // �����
      SplitText(f->Code.c_str());
      // ����� ������ �������� �� ������
      for(XTextFileLine *l=FirstLine;l!=NULL;l=l->Next)
        if(l->Line)
          { aux=rdsProcessText(l->Line,RDS_PT_TEXTTOSTRINGSL,NULL);
            if(aux)
              { ok=ok && WriteLine("|");
                ok=ok && WriteLine(aux,1);
                rdsFree(aux);
              }
          }
    }
  ok=ok && WriteLine("*/",1);

  // ����� ��� ���������� - case
  ok=ok && WriteLine("\r\n// ���������� (case, ���������� L57F_CASECALC)",1);
  ok=ok && WriteLine("#ifdef L57F_CASECALC",1);
  if(count)
    { ok=ok && WriteLine("switch(L57F_CASECALC)\r\n{",1);
      for(XTextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
        if(f->Code.IsNotEmpty())
          { XTextString num;
            num.SetInt(f->Num);
            ok=ok && WriteLine("case ");
            ok=ok && WriteLine(num.c_str());
            ok=ok && WriteLine(":",1);
            ok=ok && WriteLine("  {",1);
            SplitText(f->Code.c_str());
            // ����� ������ �������� �� ������
            for(XTextFileLine *l=FirstLine;l!=NULL;l=l->Next)
              if(l->Line)
                { ok=ok && WriteLine("    ");
                  ok=ok && WriteLine(l->Line,1);
                }
            ok=ok && WriteLine("  ; /* */",1);
            ok=ok && WriteLine("  }",1);
            ok=ok && WriteLine("  break;",1);
          }
      ok=ok && WriteLine("}",1);
    }
  ok=ok && WriteLine("#endif",1);

  // ����� ��� ���������� - if
  ok=ok && WriteLine("\r\n// ���������� (if, ���������� L57F_IFCALC)",1);
  ok=ok && WriteLine("#ifdef L57F_IFCALC",1);
  first=1;
  for(XTextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
    if(f->Code.IsNotEmpty())
      { XTextString num;
        num.SetInt(f->Num);
        if(!first)
          ok=ok && WriteLine("else ");
        first=0;
        ok=ok && WriteLine("if(L57F_IFCALC==");
        ok=ok && WriteLine(num.c_str());
        ok=ok && WriteLine(")",1);
        ok=ok && WriteLine("  {",1);
        SplitText(f->Code.c_str());
        // ����� ������ �������� �� ������
        for(XTextFileLine *l=FirstLine;l!=NULL;l=l->Next)
          if(l->Line)
            { ok=ok && WriteLine("    ");
              ok=ok && WriteLine(l->Line,1);
            }
        ok=ok && WriteLine("  ; /* */",1);
        ok=ok && WriteLine("  }",1);
      }
  ok=ok && WriteLine("#endif",1);

  /*
  // ����� ��� ����������� �����
  ok=ok && WriteLine("// �����",1);
  ok=ok && WriteLine("#ifdef L57F_NAMES",1);
  for(XTextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
    if(f->Name.IsNotEmpty())
      { char *aux;
        XTextString num;
        num.SetInt(f->Num);
        ok=ok && WriteLine("case ");
        ok=ok && WriteLine(num.c_str());
        ok=ok && WriteLine(":",1);
        aux=rdsProcessText(f->Name.c_str(),RDS_PT_TEXTTOSTRING,NULL);
        ok=ok && WriteLine("  return \"");
        ok=ok && WriteLine(aux);
        rdsFree(aux);
        ok=ok && WriteLine("\";",1);
      }
  ok=ok && WriteLine("#endif",1);
  */

  // ����� ��� �������� ��������� �������
  ok=ok && WriteLine("\r\n// ������ �������",1);
  ok=ok && WriteLine("#ifdef L57F_LIST",1);
  ok=ok && WriteLine("int L57F_LIST(int num,int *pId,char **pName,char **pCode)",1);
  ok=ok && WriteLine("{",1);
  if(count==0)
    ok=ok && WriteLine(" return 0;",1);
  else
    { XTextString aux;
      // ������ ���������������
      ok=ok && WriteLine("  static int Ids[]={");
      first=1;
      n=0;
      for(XTextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
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
      for(XTextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
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
      for(XTextFunc *f=FirstFunc;ok && f!=NULL;f=f->Next)
        { char *s;
          int firstline=1;
          if(!first)
            ok=ok && WriteLine(",\r\n");
          else
            first=0;
          SplitText(f->Code.c_str());
          // ����� ������ �������� �� ������
          for(XTextFileLine *l=FirstLine;ok && l!=NULL;l=l->Next)
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

  return ok;
}
//---------------------------------------------------------------------------

#ifdef RDS_INTVERSION
// �������� ������ �����
void XTextFile::ClearFields(void)
{ XTextFileField *f,*fn;

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
// pIndex     - ������������ ������ �������������� ��� NULL
void XTextFile::Setup_FillFuncList(RDS_HOBJECT win,int funcfield,int textfield,int funcid,int *pIndex)
{ XTextString str;
  int curnum=-1,n=0;
  XTextFunc *cur_f=NULL;
  
  if(pIndex) *pIndex=-1;

  if(!win)
    return;

  // ���������� ������ ���� ������� (�� �������)
  for(XTextFunc *f=FirstFunc;f!=NULL;f=f->Next,n++)
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
//     basefield   - ���������� ������ ������ �������
//     basefield+1 - ������������� ���� � ������� ������� (������ ������)
//     basefield+2 - ������ ��������� (���� ���������)
//     basefield+3 - ������ ���������� (���� ���������)
//     basefield+4 - ������ �������� (���� ���������)
void XTextFile::Setup_AddFuncSelectFields(RDS_HOBJECT win,char *name,int tabid,int basefield,int funcid,BOOL allowchange,
                                          DWORD flags,int mlheight)
{ int textid=basefield+1,chg_id=-1,add_id=-1,del_id=-1;
  DWORD ml_line=0,chg_line=0,add_line=0,del_line=0;
  int index;
  if(!win)
    return;

   if(flags & XTF_LINE)
    { if(flags & XTF_DELETE) del_line=RDS_FORMFLAG_LINE;
      else if(flags & XTF_ADD) add_line=RDS_FORMFLAG_LINE;
      else if(flags & XTF_CHANGE) chg_line=RDS_FORMFLAG_LINE;
      else ml_line=RDS_FORMFLAG_LINE;
    }
  else if(flags & XTF_DELETE)
    { if(flags & XTF_ADD) add_line=RDS_FORMFLAG_LINE;
      else if(flags & XTF_CHANGE) chg_line=RDS_FORMFLAG_LINE;
      else ml_line=RDS_FORMFLAG_LINE;
    }

  rdsFORMAddEdit(win,tabid,basefield,RDS_FORMCTRL_COMBOLIST,name?name:"���:",400);

  rdsFORMAddEdit(win,tabid,textid,RDS_FORMCTRL_MULTILINE|ml_line,
    allowchange?"������� (������� \"�������� �������\" ��� ��������������):":
    "�������� ����� �������:",80);
  rdsFORMSetMultilineHeight(win,textid,mlheight<=0?(3*24):mlheight);
  rdsSetObjectInt(win,textid,RDS_FORMVAL_MLREADONLY,1);

  if((flags & XTF_CHANGE) && allowchange)
    { chg_id=basefield+2;
      rdsFORMAddEdit(win,tabid,chg_id,RDS_FORMCTRL_BUTTON|chg_line,"",80);
      rdsSetObjectStr(win,chg_id,RDS_FORMVAL_VALUE,"�������� �������");
    }
  if((flags & XTF_ADD) &&  allowchange)
    { add_id=basefield+3;
      rdsFORMAddEdit(win,tabid,add_id,RDS_FORMCTRL_BUTTON|add_line,"",80);
      rdsSetObjectStr(win,add_id,RDS_FORMVAL_VALUE,"�������� �������");
    }

  if((flags & XTF_DELETE) &&  allowchange)
    { del_id=basefield+4;
      rdsFORMAddEdit(win,tabid,del_id,RDS_FORMCTRL_BUTTON|del_line,"",80);
      rdsSetObjectStr(win,del_id,RDS_FORMVAL_VALUE,"������� �������");
    }

  Setup_FillFuncList(win,basefield,textid,funcid,&index);

  if(chg_id>=0)
    rdsSetObjectInt(win,chg_id,RDS_FORMVAL_ENABLED,index>=0);
  if(del_id>=0)
    rdsSetObjectInt(win,del_id,RDS_FORMVAL_ENABLED,index>=0);

  XTextFileField *f=new XTextFileField();
  f->Next=FirstField;
  FirstField=f;
  f->ListId=basefield;
  f->TextId=textid;
  f->ChgButtonId=chg_id;
  f->AddButtonId=add_id;
  f->DelButtonId=del_id;
  f->FuncIdToSelect=funcid;
}
//---------------------------------------------------------------------------

// ������ ������ ������� �� ����� ��� ���������� �� ������� ������������
// filename    - ��� ����� (����� ��� ����)
// allowchange - ����� �� �������� ������� (������ �� ����� - 1, �� ������� - 0)
// listfunc    - ������� ������������
// win         - ������� ������, � ������� ������������ ��������� �� ���� ����
void XTextFile::Setup_ReadFuncFile(char *filename,BOOL allowchange,XTextFileFuncDescr listfunc,RDS_HOBJECT win)
{ if(allowchange)
    { char *path=rdsGetFullFilePath(filename,NULL,NULL);
      ReadAndParse(path);
      rdsFree(path);
    }
  else
    FillByFunc(listfunc);
  Changed=0;
  if(win)
    rdsSetObjectPtr(win,0,NULL,this);
}
//---------------------------------------------------------------------------

// ������ ������ ������� � ����
// filename    - ��� ����� (����� ��� ����)
// bakfilename - ��� ����� ��������� ����� (����� ��� ����)
// force       - ������, ���� ���� Changed==0
void XTextFile::Setup_WriteFuncFile(char *filename,char *bakfilename,int force)
{
  if(force)
    Changed=1;
  if(Changed)
    { char *path=rdsGetFullFilePath(filename,NULL,NULL);
      char *bak=bakfilename?rdsGetFullFilePath(bakfilename,NULL,NULL):NULL;
      if(bak)
        { DeleteFile(bak);
          CopyFile(path,bak,FALSE);
        }
      if(!SaveFuncList(path))
        { DeleteFile(path);
          if(bak)
            CopyFile(bak,path,FALSE);
          rdsMessageBox("������ ������ �����","������",MB_OK|MB_ICONERROR);
        }
      else
        Changed=0;
      rdsFree(path);
      rdsFree(bak);
    }
}
//---------------------------------------------------------------------------

// ����� ���� ���������� ��� �������������� �������
// index - ����� ������� � ������
XTextFunc * XTextFile::Setup_AddOrEditFunc(int index)
{ RDS_HOBJECT win=rdsFORMCreate(FALSE,-1,-1,
                  index<0?"���������� �������":"��������� �������");
  BOOL res;
  XTextFunc *func=NULL;
  if(!win)
    return NULL;
  rdsFORMAddEdit(win,1,1,RDS_FORMCTRL_EDIT,"���:",400);

  rdsFORMAddEdit(win,1,2,RDS_FORMCTRL_MULTILINE,"�������:",80);
  rdsFORMSetMultilineHeight(win,2,3*24);

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
    }

  for(;;)
    { res=rdsFORMShowModalServ(win,NULL);
      if(res)
         { char *name=rdsGetObjectStr(win,1,RDS_FORMVAL_VALUE);
           char *code=rdsGetObjectStr(win,2,RDS_FORMVAL_VALUE);
           if(name==NULL || (*name)==0)
             { rdsMessageBox("��� ������� �� ����� ���� ������","������",MB_ICONERROR|MB_OK);
               continue;
             }
           if(code==NULL || (*code)==0)
             { rdsMessageBox("�������� ������� �� ����� ���� ������","������",MB_ICONERROR|MB_OK);
               continue;
             }
           // ��� � �������
           Changed=1;
           if(!func) // ����������
             func=AddFunction();
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
BOOL XTextFile::Setup_WindowCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data)
{ XTextFunc *f;
  int index;
  BOOL ok=FALSE;


  switch(data->Event)
    { case RDS_FORMSERVEVENT_CHANGE:
        // ���������� �� ���� ����� ������ �������
        for(XTextFileField *field=FirstField;field!=NULL;field=field->Next)
          if(field->ListId>=0 && data->CtrlId==field->ListId) // ��� �������
            { // ������ ������ �����
              int index=rdsGetObjectInt(win,field->ListId,RDS_FORMVAL_VALUE);
              f=FuncByIndex(index);
              if(f)
                field->FuncIdToSelect=f->Num;
              rdsSetObjectStr(win,field->TextId,RDS_FORMVAL_VALUE,f?f->Code.c_str():"");
              if(field->ChgButtonId>=0)
                rdsSetObjectInt(win,field->ChgButtonId,RDS_FORMVAL_ENABLED,index>=0);
              if(field->DelButtonId>=0)
                rdsSetObjectInt(win,field->DelButtonId,RDS_FORMVAL_ENABLED,index>=0);
            }
        break;
      case RDS_FORMSERVEVENT_CLICK:
        // ���������� �� ���� ������� ����� ������ �������
        for(XTextFileField *field=FirstField;field!=NULL;field=field->Next)
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
                  { XTextString str;
                    str.Assign("������� \"");
                    str.AddString(f->Name.c_str());
                    str.AddString("\" ����� ������� �� ������, ������������� ����� ������� "
                                  "���� �����. ������� ��?");
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
          } // for(XTextFileField *field=FirstField;...)
        break;
    } // switch(data->Event)

  if(ok)
    { BOOL hasfunc=(FirstFunc!=NULL);
      for(XTextFileField *field=FirstField;field!=NULL;field=field->Next)
        { Setup_FillFuncList(win,field->ListId,field->TextId,field->FuncIdToSelect);
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


#endif // #ifdef RDS_INTVERSION

//---------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------

char XTextString::ZeroByte=0;

// �������� ������
void XTextString::Clear(void)
{
  if(Data)
    delete[] Data;
  Data=NULL;
}
//---------------------------------------------------------------------------

// �����
int XTextString::Length(void) const
{return Data?strlen(Data):0;}
//---------------------------------------------------------------------------

// ���������
void XTextString::Assign(const char *s)
{ int l;
  Clear();
  if(s==NULL || (*s)==0)
    return;
  l=strlen(s);
  Data=new char[l+1];
  strcpy(Data,s);
}
void XTextString::Assign(const char *s,int n)
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
void XTextString::AddString(char *s)
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
void XTextString::SetInt(int i)
{ char tempstr[100];
  sprintf(tempstr,"%d",i);
  Assign(tempstr);
}
//---------------------------------------------------------------------------

