//---------------------------------------------------------------------------
// �������������� ������ � ������� ��� ��������� ��������������������� ������
// (����� ������� ������ � �������, ��������� � ���������� ���������)
//---------------------------------------------------------------------------
#ifndef RdsLinInterH
  #include <RdsAC/LinInter.cpp>
#endif

// ���������������� ��������� ����� �����
#define LINEID_NONE             0       // ��� �����
#define LINEID_HIDDEN           -1      // ������ "$HIDDEN", ���������� ������� ������
#define LINEID_OPTIONS          -2      // ������ "$OPTIONS", ���������� ������ ����� ������
//---------------------------------------------------------------------------
// ������� ������� ��������� �����
struct TL57TextFileBasic_ServMarker
{ char *Text;
  int Id;
  int TextLen;
};
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

// ������� �������� ������ ����� (������ �� ������� ������)
class TL57TextFileLine
{ public:
    TL57TextFileLine *Next;
    char *Line;
    int Id;

    TL57TextFileLine(void){Next=NULL;Line=NULL;Id=LINEID_NONE;};
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
    // �������������� ����� double � ������
    void SetDouble(double d,int dec=-1);

    TL57TextString(void){Data=NULL;};
    ~TL57TextString(){Clear();};
};
//---------------------------------------------------------------------------
// ��������� �����������
#define AUTOTEXT_START "\x2f***** ������ ������������� ��������������� ������ *****/"
#define AUTOTEXT_END   "\x2f***** ����� ������������� ��������������� ������  *****/"
// ������ � ������ ���������� �����
class TL57TextFileBasic
{ protected:
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

    // �������� ������ ��� ����� ������ ����� (������������, ���. rdsFree)
    char *GetModelFileName(RDS_BHANDLE block);
  public:
    HANDLE Handle;

    TL57TextFileLine *FirstLine; // ��������� �����, �������� �� ������

    // �������� ����������� �����
    void ClearText(void);

    // ������� ���� ������ ����� � ��������� �� �������
    BOOL ReadAndParseModel(RDS_BHANDLE block,TL57TextFileBasic_ServMarker *markers,BOOL message);
    // ������� ���� � ��������� �� �������
    BOOL ReadAndParse(char *path,TL57TextFileBasic_ServMarker *markers);
    // �������� ���� �� ������ �������
    int SaveFile(char *path);
    // �������� ���� �� ������ ������� � ������ �����
    int SaveFileToModel(RDS_BHANDLE block,BOOL bak=FALSE);
    // ��������� ����� ����������� ������ �� ������
    void SplitText(char *text);

    // �����������
    // ��������� ������� ������
    virtual void ParseHiddenSection(TL57TextFileLine */*line*/){};
    // �������� ������� ������
    virtual BOOL WriteHiddenSection(void){return TRUE;};
    // �������� �����-�� ������ � ������� ������ �� �����
    virtual BOOL WriteSection(int /*id*/,int */*pendid*/){return TRUE;};


    // ��������� ������� ����� � ����� �����
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
// ����
//---------------------------------------------------------------------------
// ������� ����
void TL57TextFileBasic::Close(void)
{
  if(Handle!=INVALID_HANDLE_VALUE)
    { CloseHandle(Handle);
      Handle=INVALID_HANDLE_VALUE;
    }
}
//---------------------------------------------------------------------------

// ������� �� ������
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

// ������� �� ������
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

// �������� ����������� �����
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


#define Windows_ChunkSizeMax (1 << 22) // ������/������ �� ������ �����
// ������ � ����
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

// ������ �� �����
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

// �������� ������ ����� (��������)
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

// ������� �������� ���� � ����������� ���������� �����
// path       - ����
// psize      - ������������ ������
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

// ��������� ������� ����� � ����� �����
BOOL TL57TextFileBasic::CheckConnectionForBlockVar(RDS_BHANDLE Block,int num)
{ RDS_VARDESCRIPTION vdescr;

  RDS_INITSERVSIZE(vdescr);

  // ����� ����������
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

// ��������� Text �� ������ (������ ������ ������ ���� ������)
void TL57TextFileBasic::SplitTextServ(void)
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
void TL57TextFileBasic::SplitText(char *text)
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
BOOL TL57TextFileBasic::ReadAndSplit(char *path)
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

// �������� ������ ��� ����� ������ ����� (������������, ���. rdsFree)
char *TL57TextFileBasic::GetModelFileName(RDS_BHANDLE block)
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
    rdsMessageBox("���� ������ ����� ����������� ��� ����� �������� ������","�������� �������",MB_ICONERROR|MB_OK);
  return ok;
}
//---------------------------------------------------------------------------

// ������� ���� � ��������� �� �������
BOOL TL57TextFileBasic::ReadAndParse(char *path,TL57TextFileBasic_ServMarker *markers)
{ int n;
  char *aux;
  TL57TextFileLine *line;

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

  // ������ ������� (�������� �����������)
  if(line)
    ParseHiddenSection(line);
  // ����������� ����� �������� � ������
  return TRUE;
}
//---------------------------------------------------------------------------

// �������� ���� �� ������ ������� � ������ �����
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
int TL57TextFileBasic::SaveFile(char *path)
{ int ok=1,funcwritten=0,endmarker;
  TL57TextFileLine *l=FirstLine;

  if(!OpenWrite(path))
    return 0;

  while(ok && l!=NULL)
    switch(l->Id)
      { case LINEID_HIDDEN: // ������ ������� ������ - ����� ���� ������ ����� ������ �������
          if(!funcwritten) // ������� ��� �� ��������
            { ok=ok && WriteLine(l->Line,1);
              // ������
              ok=ok && WriteHiddenSection(); // �����������
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
          if(!funcwritten) // ������� ��� �� ��������
            { ok=ok && WriteLine("$HIDDEN",1);
              // ������
              ok=ok && WriteHiddenSection(); // �����������
              funcwritten=1;
            }
          ok=ok && WriteLine(l->Line,1); // $OPTIONS
          l=l->Next;
          break;

        case LINEID_NONE: // ������ �� ��������������
          ok=ok && WriteLine(l->Line,1);
          l=l->Next;
          break;

        default: // �����-�� ������ �������
          endmarker=LINEID_NONE;
          ok=ok && WriteLine(l->Line,1); // ��� ������
          ok=ok && WriteSection(l->Id,&endmarker); // ����������� - ������ ������
          // ������� �� ��������� �������
          l=l->Next;
          if(endmarker!=LINEID_NONE)
            while(l)
              { if(l->Id==endmarker)
                  { ok=ok && WriteLine(l->Line,1); // �������� ������
                    l=l->Next;
                    break;
                  }
                l=l->Next;
              }
          // ������ l - ��������� ����� ��������� ������� ������ ��� ����� ��������
          break;
      }
  return ok;
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

// �������������� ����� double � ������
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
#undef LINEID_NONE

// ��������� ������������� �����
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


// ��������� ������������� ����� �������
// *pstate - ���� ��� ���������� ������������ �������� (����� ���� NULL)
//      ==0 - ��� �� ��������
//      ==1 - ��� �����
//      ==2 - ���� ����
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

