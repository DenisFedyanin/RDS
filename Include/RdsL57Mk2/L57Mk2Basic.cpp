//---------------------------------------------------------------------------
// �������������� ������ � ������� ��� ��������� ��������������������� ������
// (����� ������� ������ � �������, ��������� � ���������� ���������)
// ������ 2
//---------------------------------------------------------------------------

#ifndef L57Mk2BasicH
  #include <RdsL57Mk2/L57Mk2Basic.h>
#endif

#ifndef L57_NORDSCLASSES
//---------------------------------------------------------------------------
// �������� �������� ������ �� ���������� ������� � �������� ������� (������� �������� ������)
//---------------------------------------------------------------------------
// ������� ������� ��� ������� ������� ����� ����������
void L57_TableFiller::AllocateArrays1Arg(int resultscount)
{
  rdsFree(MatrCol);
  rdsFree(ResultN);
  MatrCol=ResultN=NULL;
  Count=0;
  TwoArgResult=-1;
  if(resultscount<1)
    return;
  MatrCol=L57_NewInt(Count=resultscount);
  ResultN=L57_NewInt(Count);
  for(int i=0;i<Count;i++)
    MatrCol[i]=ResultN[i]=-1;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ����� �������� �������� ������
//---------------------------------------------------------------------------
// �������� ���
void L57_TableFillProgram::Clear(void)
{ L57_TableFiller *f,*fn;

  f=First;
  while(f)
    { fn=f->Next;
      delete f;
      f=fn;
    }
  First=NULL;
}
//---------------------------------------------------------------------------

// �������� ������� ������ ��� ������� ������� ����� ���������
L57_TableFiller *L57_TableFillProgram::Add1Arg(rdsbcppVarAncestor *matr,char *pReady,int resultscount)
{ L57_TableFiller *f=new L57_TableFiller();

  f->Next=First;
  First=f;

  f->Matr=matr;
  f->AllocateArrays1Arg(resultscount);
  f->pReady=pReady;
  return f;
}
//---------------------------------------------------------------------------

// �������� ������� ������ ��� ������� ������� ���� ���������
L57_TableFiller *L57_TableFillProgram::Add2Arg(rdsbcppVarAncestor *matr,char *pReady,int resultnum,int transp)
{ L57_TableFiller *f=new L57_TableFiller();

  f->Next=First;
  First=f;

  f->Matr=matr;
  f->TwoArgResult=resultnum;
  f->pReady=pReady;
  f->Transp=transp;
  return f;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ double RDS (������ �� ������� ������)
//---------------------------------------------------------------------------
void L57_RdsDoubleArray::Setup(rdsbcppVarAncestor &var,double def)
{ void *ptr=var.GetVoidPtr();
  DefVal=def;
  if(rdsGetVarArrayElementType(ptr)==RDS_VARTYPE_DOUBLE && RDS_ARRAYEXISTS(ptr))
    { Array=(double*)RDS_ARRAYDATA(ptr);
      Count=RDS_ARRAYCOLS(ptr)*RDS_ARRAYROWS(ptr);
    }
  else
    { Array=NULL;
      Count=0;
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� double RDS (������ �� ������� ������)
//---------------------------------------------------------------------------
void L57_RdsDoubleMatrix::Setup(rdsbcppVarAncestor &var,double def)
{ void *ptr=var.GetVoidPtr();
  DefVal=def;
  if(rdsGetVarArrayElementType(ptr)==RDS_VARTYPE_DOUBLE && RDS_ARRAYEXISTS(ptr))
    { Array=(double*)RDS_ARRAYDATA(ptr);
      _Cols=RDS_ARRAYCOLS(ptr);
      _Rows=RDS_ARRAYROWS(ptr);
    }
  else
    { Array=NULL;
      _Rows=_Cols=0;
    }
}
//---------------------------------------------------------------------------
#endif // #ifndef L57_NORDSCLASSES


//---------------------------------------------------------------------------
// �������� � ������
//---------------------------------------------------------------------------
// ������� ����
void L57_File::Close(void)
{
  if(Handle!=INVALID_HANDLE_VALUE)
    { CloseHandle(Handle);
      Handle=INVALID_HANDLE_VALUE;
    }
}
//---------------------------------------------------------------------------

// ������� �� ������
int L57_File::OpenWrite(char *path)
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

// ������� �� ��������
int L57_File::OpenAppend(char *path)
{ char *fullpath;
  Close();
  fullpath=rdsGetFullFilePath(path,NULL,NULL);
  if(!fullpath)
    return 0;
  Handle=CreateFile(fullpath,GENERIC_WRITE,
                    0, NULL,
                    OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,NULL);
  rdsFree(fullpath);
  if(Handle==INVALID_HANDLE_VALUE)
    return 0;
  return SetFilePointer(Handle,0,NULL,FILE_END)!=0xFFFFFFFF;
}
//---------------------------------------------------------------------------

// ������� �� ������
int L57_File::OpenRead(char *path)
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

#define Windows_ChunkSizeMax (1 << 22) // ������/������ �� ������ �����
// ������ � ����
int L57_File::_WindowsWrite(const void *data,size_t *size)
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
int L57_File::WindowsWrite(const void *data,size_t size)
{ size_t act_write=size;
  if(!_WindowsWrite(data,&act_write))
    return 0/*false*/;
  return act_write==size;
}
//---------------------------------------------------------------------------

// ������ �� �����
int L57_File::_WindowsRead(void *data, size_t *size)
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
int L57_File::WindowsRead(void *data, size_t size)
{ size_t act_read=size;
  if(!_WindowsRead(data,&act_read))
    return 0/*false*/;
  return act_read==size;
}
#undef Windows_ChunkSizeMax
//---------------------------------------------------------------------------

// �������� ������ ����� (��������)
int L57_File::WindowsGetFileSizeShort(DWORD *psize,int *pTooLong)
{ DWORD sizeHigh,sizeLow;
  if(Handle==INVALID_HANDLE_VALUE)
    return 0/*false*/;
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
char * L57_File::ReadFileToMemory(char *path,DWORD *psize,int addzerobyte)
{ DWORD size;
  size_t act_size;
  char *mainbuf;
  bool ok;

  Close();

  if(psize) *psize=0;

  if(!OpenRead(path))
    return NULL;

  if(!WindowsGetFileSizeShort(&size))
    { Close();
      return NULL;
    }

  mainbuf=(char*)rdsAllocate(addzerobyte?(size+1):size);

  // ������ ����
  act_size=size;
  ok=_WindowsRead(mainbuf,&act_size);
  Close();
  if(ok)
    ok=(act_size==size);
  if(!ok)
    { rdsFree(mainbuf);
      return NULL;
    }
  if(psize) *psize=size;
  if(addzerobyte)
    mainbuf[size]=0;
  return mainbuf;
}
//---------------------------------------------------------------------------

// �������� ������� ������ � ����
// path       - ����
// src        - ������ �������
// size       - ������
int L57_File::WriteMemoryToFile(char *path,void *src,DWORD size)
{ int ok;

  Close();

  if(!OpenWrite(path))
    return 0;

  if(src==NULL || size==0)
    ok=1; // ���� ����� ������
  else // ����� � ���� size ������
    ok=WindowsWrite(src,size);
  Close();
  return ok;
}
//---------------------------------------------------------------------------

// �������� ����� � ����
int L57_File::WriteTextToFile(char *path,char *src,DWORD len)
{
  if(len==0)
    return WriteMemoryToFile(path,src,strlen(src));
  return WriteMemoryToFile(path,src,len);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------
char L57_String::ZeroByte=0;

// ���������
void L57_String::Assign(const char *s)
{ int l;
  Clear();
  if(s==NULL || (*s)==0)
    return;
  l=strlen(s);
  Data=(char*)rdsAllocate(l+1);
  strcpy(Data,s);
}
void L57_String::Assign(const char *s,int n)
{ int l;
  Clear();
  if(s==NULL || (*s)==0)
    return;
  l=strlen(s);
  if(l>n)
    l=n;
  Data=(char*)rdsAllocate(l+1);
  strncpy(Data,s,l);
  Data[l]=0;
}
//---------------------------------------------------------------------------

// �������� ������ ������� � �������� �����
int L57_String::IsBlank(void)const
{
  if(IsEmpty())
    return 1;
  for(char *s=Data;(*s)!=0;s++)
    if((*s)!=' ' && (*s)!='\r' && (*s)!='n' && (*s)!='t')
      return 0;
  return 1;
}
//---------------------------------------------------------------------------

// �������������� ����� int � ������
void L57_String::AssignInt(int i)
{ char *s=rdsItoA(i,10,0);
  AssignRdsDynamicString(s);
}
//---------------------------------------------------------------------------

// �������������� ����� double � ������
void L57_String::AssignDouble(double d,int dec)
{ char *s=rdsDtoA(d,dec,NULL);
  AssignRdsDynamicString(s);
}
//---------------------------------------------------------------------------

// �������� int � ������
void L57_String::AddInt(int i)
{ char *s=rdsItoA(i,10,0);
  AddString(s);
  rdsFree(s);
}
//---------------------------------------------------------------------------

// �������� double � ������
void L57_String::AddDouble(double d,int dec)
{ char *s=rdsDtoA(d,dec,NULL);
  AddString(s);
  rdsFree(s);
}
//---------------------------------------------------------------------------

// ���������� �������
void L57_String::Exchange(L57_String *pStr)
{ char *this_str,*that_str;
  if(!pStr)
    return;
  this_str=DetachRdsDynamicString();
  that_str=pStr->DetachRdsDynamicString();
  AssignRdsDynamicString(that_str);
  pStr->AssignRdsDynamicString(this_str);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ ����� ����� (������� ������ �������)
//---------------------------------------------------------------------------
// ��������
void L57_IntArray::Clear(void)
{
  rdsFree(Items);
  Items=NULL;
  Size=Count=0;
}
//---------------------------------------------------------------------------

// ������� ��������� �������
void L57_IntArray::Init(int size,int initvalue)
{
  if(Items==NULL || size>=Size)
    { // ���������
      rdsFree(Items);
      Items=(int*)rdsAllocate(size*sizeof(int));
      Size=size;
    }
  Count=size;
  for(int i=0;i<Count;i++)
    Items[i]=initvalue;
}
//---------------------------------------------------------------------------

// �������� �������
void L57_IntArray::Add(int x)
{ int *newitems;
  if(Count<Size) // ���� �����
    { Items[Count]=x;
      Count++;
      return;
    }
  // �����������
  newitems=(int*)rdsAllocate((Size+Step)*sizeof(int));
  if(Items)
    { memcpy(newitems,Items,Count*sizeof(int));
      rdsFree(Items);
    }
  Items=newitems;
  Size=Size+Step;
  Items[Count]=x;
  Count++;
}
//---------------------------------------------------------------------------

// ���������� ������� (�������� ������, ���� �����)
void L57_IntArray::SetElement(int index,int value)
{ int *newitems,newsize;
  if(index<Count) // ����� ��� ����
    { Items[index]=value;
      return;
    }
  if(index<Size) // ���� �����
    { Items[index]=value;
      Count=index+1;
      return;
    }
  // �����������
  newsize=index+1+Step;
  newitems=(int*)rdsAllocate(newsize*sizeof(int));
  if(Items)
    { memcpy(newitems,Items,Count*sizeof(int));
      rdsFree(Items);
    }
  Items=newitems;
  Size=newsize;
  Items[index]=value;
  Count=index+1;
}
//---------------------------------------------------------------------------

// ����� �������
int L57_IntArray::IndexOf(int x)
{
  for(int i=0;i<Count;i++)
    if(Items[i]==x)
      return i;
  return -1;
}
//---------------------------------------------------------------------------

// ��������, ���� ������ ��� �� ����
void L57_IntArray::AddIfNew(int x)
{
  if(IndexOf(x)>=0) return;
  Add(x);
}
//---------------------------------------------------------------------------

// ������� �������
void L57_IntArray::DeleteByIndex(int index)
{
  if(index<0 || index>=Count || Count==0) return;
  if(index<Count-1)
    { for(int i=index+1;i<Count;i++)
        Items[i-1]=Items[i];
    }
  Count--;
  if(Count==0) Clear();
}
//---------------------------------------------------------------------------

// �������� � ������ ������ ����� �������
void L57_IntArray::ToString(L57_String *pStr)
{
  if(!pStr)
    return;
  pStr->Clear();
  for(int i=0;i<Count;i++)
    { if(i) pStr->AddString(",");
      pStr->AddInt(Items[i]);
    }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ �����
//---------------------------------------------------------------------------
// ��������
void L57_StringArray::Clear(void)
{
  for(int i=0;i<_Count;i++)
    delete Data[i];
  if(Data)
    { delete[] Data;
      Data=NULL;
    }
  _Count=0;
}
//---------------------------------------------------------------------------

// ������� ������� � �������� �������, ���� ��� ���
void L57_StringArray::CreateItem(int n)
{ int count=n+1;
  L57_String **newData;
  if(n<0)
    return;
  if(count<=_Count)
    return;
  // ���� ��������
  newData=new L57_String*[count];
  if(Data)
    { if(_Count)
        memcpy(newData,Data,_Count*sizeof(L57_String*));
      delete[] Data;
    }
  Data=newData;
  for(int i=_Count;i<count;i++)
    Data[i]=new L57_String();
  _Count=count;
}
//---------------------------------------------------------------------------

// ��������
BOOL L57_StringArray::IsEqualTo(L57_StringArray *pArray)
{
  if(pArray==NULL)
    return FALSE;
  if(_Count!=pArray->Count())
    return FALSE;
  for(int i=0;i<_Count;i++)
    { L57_String *s1=Data[i],
                 *s2=pArray->ItemPtrFAST(i);
      if(s1->IsNotEqual(*s2))
        return FALSE;
    }
  return TRUE;
}
//---------------------------------------------------------------------------
BOOL L57_StringArray::IsEqualTo(char **array,int count)
{
  if(_Count!=count)
    return FALSE;
  for(int i=0;i<_Count;i++)
    { L57_String *s=Data[i];
      int empty1=s->IsEmpty()?1:0;
      int empty2=(array[i]==NULL || array[i][0]==0);
      if(empty1!=empty2)
        return FALSE;
      if(empty1) // ��� ����� (�����)
        continue;
      if(strcmp(s->c_str(),array[i])!=0)
        return FALSE;
    }
  return TRUE;
}
//---------------------------------------------------------------------------

// ���� �� � ������ �������� ������, ������������ �� �����������
BOOL L57_StringArray::HasDifferentNonEmpty(char **array,int count)
{ int c_max=count;
  if(_Count>c_max)
    c_max=_Count;
  for(int i=0;i<c_max;i++)
    { char *s1=(i<_Count)?Data[i]->c_str():NULL,
           *s2=(i<count)?array[i]:NULL;
      int empty1=(s1==NULL || s1[0]==0),
          empty2=(s2==NULL || s2[0]==0);
      if(empty1) // ������ ������ � ������ - �� �����, ��� � array
        continue;
                        /*
                        char *s=rdsProcessText(s2,RDS_PT_TEXTTOSTRING,NULL);
                        rdsBlockMessageBox(NULL,s,"����",MB_OK);
                        rdsFree(s);
                        s=rdsProcessText(s1,RDS_PT_TEXTTOSTRING,NULL);
                        rdsBlockMessageBox(NULL,s,"�������",MB_OK);
                        rdsFree(s);
                        */
      // �������� ������ � ������
      if(empty2) // ������ � array
        return TRUE;
      // ��� �� ������
      if(strcmp(s1,s2)!=0)
        return TRUE;
    }
  return FALSE;
}
//---------------------------------------------------------------------------

// ����� ������
int L57_StringArray::IndexOf(char *str)
{ for(int i=0;i<_Count;i++)
    if(Data[i]->IsEqual(str))
      return i;
  return -1;
}
//---------------------------------------------------------------------------

// ��������� ������ (����������� �� /n)
void L57_StringArray::Assign(char *str)
{ char *start;
  int lf_n;

  Clear();
  if(str==NULL || (*str)==0)
    return;

  // ������� \n
  lf_n=0;
  start=str;
  while(start)
    { lf_n++;
      start=strchr(start,'\n');
      if(start)
        start++;
    }
  // � ������ ����� �� ������� ���� lf_n �����
  CreateItem(lf_n-1); // � �������

  lf_n=0;
  start=str;
  while(lf_n<_Count)
    { char *end=strchr(start,'\n');
      int n;
      if(!end) // ��������� ���� ����� ������ ����� start
        { Data[lf_n]->Assign(start);
          //lf_n++;
          break;
        }
      // ��������� start...end-1
      n=(int)(end-start);
      if(n==0) // ����� �� LF
        { lf_n++;
          start++;
          continue;
        }
      // ���-�� ����
      Data[lf_n]->Assign(start,n);
      start=end+1;
      lf_n++;
    }

  // ������� '\r'
  for(int i=0;i<_Count;i++)
    { char *s=Data[i]->c_str();
      int l=strlen(s);
      if(l==0)
        continue;
      if(s[l-1]=='\r')
        s[l-1]=0;
    }

}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// ������ ������������ ����� (������� ������ �������)
//---------------------------------------------------------------------------
void L57_DblArray::Clear(void)
{
  rdsFree(Data);
  Data=NULL;
  Count=0;
}
//---------------------------------------------------------------------------

// �������
BOOL L57_DblArray::Allocate(int n)
{ BOOL ok=TRUE;
  if(n<=0)
    { Clear();
      return TRUE;
    }
  if(n<Count)
    { Count=n;
      return TRUE;
    }
  Clear();
  RDSBCPP_TRY
    { Data=L57_NewDouble(Count=n); }
  RDSBCPP_CATCHALL
    { ok=FALSE; }
  return ok;
}
//---------------------------------------------------------------------------

// ���������� ������� � ������ ��������
void L57_DblArray::Exchange(L57_DblArray &array)
{ double *d=Data;
  int c=Count;
  Data=array.Data; Count=array.Count;
  array.Data=d; array.Count=c;
}
//---------------------------------------------------------------------------

// ��������� ������� (��� �� ������) �� ������� �������
void L57_DblArray::FillDataFrom(const L57_DblArray &array)
{ int n=array.Count;
  if(Count<n)
    n=Count;
  memcpy(Data,array.Data,n*sizeof(double));
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� �������� ������ ������� ������� ������ ��� �������� ������� ��� �� ����������
//---------------------------------------------------------------------------
// �������� �� ����������
void L57_TransferCall::Extract(void)
{
  if(Parent)    // ���� ��������� - ���� ��������� �� ����
    { // ����������� ������ �����
      if(Prev) // ��� �� ������ ������� � ������
        Prev->Next=Next;
      else
        Parent->First=Next;
      // ����������� �������� �����
      if(Next) // ��� �� ���������
        Next->Prev=Prev;
      else
        Parent->Last=Prev;
      Parent=NULL;
    }
}
//---------------------------------------------------------------------------

// ������������ � ������ ���������� (� �����)
void L57_TransferCall::ConnectLast(L57_TransferCallsList *c)
{
   Extract(); // �����������, ���� ���� ������
   if(c)
     { Parent=c;        // ��������
       Next=NULL;       // ���������� �������� ���
       Prev=c->Last;    // ���������� - ������ ���������
       c->Last=this;    // ����� ��������� - ����
       if(Prev)
         Prev->Next=this; // ����� ������� ����������� ���� ����
       else // ����������� ��� - ��� ������������ ������� � ����������
         c->First=this;
     }
   else
     { Parent=NULL; Prev=Next=NULL; }
}
//---------------------------------------------------------------------------

L57_TransferCall::~L57_TransferCall()
{ Extract();
  if(Texts)
    { for(int i=0;i<TextsCount;i++)
        rdsFree(Texts[i]);
      delete[] Texts;
    }
}
//---------------------------------------------------------------------------

// ���������� �� ��������� ������� �����
void L57_TransferCall::CopyFrom(L57_AcceptFunc_Param *param)
{
  if(!param)
    return;
  if(!L57_CANACCESSFIELD(param,DestVarNum)) // �������� ������ ���������
    return;
  if(Texts)
    { for(int i=0;i<TextsCount;i++)
        rdsFree(Texts[i]);
      delete[] Texts;
    }
  Immediate=param->Immediate;
  TextIndex=param->TextIndex;
  AcceptIndex=param->AcceptIndex;
  DestVarNum=param->DestVarNum;
  if(param->TextsCount>0)
    { Texts=new char*[param->TextsCount];
      TextsCount=param->TextsCount;
      for(int i=0;i<param->TextsCount;i++)
        Texts[i]=rdsDynStrCopy(param->Texts[i]);
    }
  else
    { Texts=NULL;
      TextsCount=0;
    }
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// ������� ������ ������� ������� ������ ��� �������� ������� ��� �� ����������
//---------------------------------------------------------------------------
// �������� ������
void L57_TransferCallsList::Clear(void)
{
  while(First)
    delete First;
}
//---------------------------------------------------------------------------

// �������� ����� �������� ����������
void L57_TransferCallsList::AddParamTransfer(RDS_BHANDLE block,int destvarnum)
{ L57_TransferCall *c=new L57_TransferCall();
  c->Block=block;
  c->DestVarNum=destvarnum;
  c->ConnectLast(this);
}
//---------------------------------------------------------------------------

// ������� ��������� ������ ��� ���������� ������ � �������
void RDSCALL L57_TransferCallsList_AddParam(LPVOID object,RDS_BHANDLE block,int destvarnum)
{ L57_TransferCallsList *list=(L57_TransferCallsList*)object;
  if(list==NULL || block==NULL)
    return;
  list->AddParamTransfer(block,destvarnum);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ����� ���������� �������, ������� ����� ��������� ����
//---------------------------------------------------------------------------
L57_FuncTransfer::L57_FuncTransfer(void)
{
  // ����������� ������ ��� ����
  varnames_set=rdsRegisterUserNameSet(L57_VARNAMESET);
  // ����������� ������� ��������
  transfer_func=rdsRegisterFunction(L57_ACCEPTUSERPARAMNAME);
  func_transfer_func=rdsRegisterFunction(L57_ACCEPTUSERFUNCNAME);
  // ����������� ���������� ���������� �������� ���������
  ParamTransferIdPtr=rdsRegisterGlobalIntVar(L57_GLOBALVAR_PARAMTRANSFERID);

  LastTransferId=0;

  Params=NULL;
  ParamsSize=ParamsCount=0;

  DestBlocks=NULL;
}
//---------------------------------------------------------------------------

// �������� ��������� ������������� ��� �������� ���������
int L57_FuncTransfer::GenerateParamTransferId(void)
{ int id;
  if(!ParamTransferIdPtr)
    return 0;
  (*ParamTransferIdPtr)++;
  id=(*ParamTransferIdPtr);
  if(id)
    return id;
  (*ParamTransferIdPtr)++;
  return (*ParamTransferIdPtr);
}
//---------------------------------------------------------------------------


// �������� ���������
void L57_FuncTransfer::ClearParameters(void)
{
  if(Params)
    { for(int i=0;i<ParamsCount;i++)
        delete Params[i];
      delete[] Params;
    }
  Params=NULL;
  ParamsSize=ParamsCount=0;
}
//---------------------------------------------------------------------------

// ��������� ������ ������� ����������
void L57_FuncTransfer::EnlargeParameters(int newsize)
{ L57_FuncTransferParameter **newData;

  if(newsize<=ParamsSize)
    return;

  newData=new L57_FuncTransferParameter*[newsize];

  if(Params)
    { if(ParamsCount>0) memcpy(newData,Params,ParamsCount*sizeof(L57_FuncTransferParameter*));
      delete[] Params;
    }

  Params=newData;
  ParamsSize=newsize;
}
//---------------------------------------------------------------------------

// �������� ��������
L57_FuncTransferParameter * L57_FuncTransfer::AddParameter(void)
{ L57_FuncTransferParameter *p;
  if(Params==NULL || ParamsCount>=ParamsSize)
    { int n=ParamsSize+10;
      if(ParamsCount>=n)
        n=ParamsCount+10;
      EnlargeParameters(n);
    }
  p=new L57_FuncTransferParameter();
  Params[ParamsCount]=p;
  ParamsCount++;
  return p;
}
//---------------------------------------------------------------------------

// �������� ��������
L57_FuncTransferParameter * L57_FuncTransfer::AddParameter(int nameid,int suffixid,BOOL matrix,int varnum)
{ L57_FuncTransferParameter *p=AddParameter();
  p->NameId=nameid;
  p->SuffixId=suffixid;
  p->Matrix=matrix;
  p->BlockVarNum=varnum;
  if(varnum<0)
    p->HasConnection=FALSE;
  return p;
}
L57_FuncTransferParameter * L57_FuncTransfer::AddParameter(char *name,int suffixid,BOOL matrix,int varnum)
{ L57_FuncTransferParameter *p;
  int nameid=rdsRegisterUserName(varnames_set,name);
  p=AddParameter(nameid,suffixid,matrix,varnum);
  return p;
}
#ifdef L57_NORDSCLASSES
L57_FuncTransferParameter * L57_FuncTransfer::AddParameter(int nameid,int suffixid,BOOL matrix,int varnum,int signaloffset,int varoffset)
{ L57_FuncTransferParameter *p=AddParameter(nameid,suffixid,matrix,varnum);
  p->ChangeSignalOffset=signaloffset;
  p->VarOffset=varoffset;
  return p;
}
#else
L57_FuncTransferParameter * L57_FuncTransfer::AddParameter(char *name,int suffixid,BOOL matrix,int varnum,double *plainvar)
{ L57_FuncTransferParameter *p=AddParameter(name,suffixid,matrix,varnum);
  p->pPlainVar=plainvar;
  return p;
}
L57_FuncTransferParameter * L57_FuncTransfer::AddParameterRds(char *name,int suffixid,BOOL matrix,int varnum,rdsbcppVarAncestor *rdsvar)
{ L57_FuncTransferParameter *p=AddParameter(name,suffixid,matrix,varnum);
  p->pRdsVar=rdsvar;
  return p;
}
#endif
//---------------------------------------------------------------------------

// ����� ��������
L57_FuncTransferParameter * L57_FuncTransfer::FindParameter(int nameid,int suffixid)
{
  for(int i=0;i<ParamsCount;i++)
    { L57_FuncTransferParameter *p=Params[i];
      if(p->NameId==nameid && p->SuffixId==suffixid)
        return p;
    }
  return NULL;
}
//---------------------------------------------------------------------------

#ifdef L57_NORDSCLASSES
// �������� ����� ������� ���������� � ���� ���������� � �������� �������� ������
void L57_FuncTransfer::ResetConnFlagsForName(char *name)
{ int nameid=rdsRegisterUserName(varnames_set,name);
  for(int i=0;i<ParamsCount;i++)
    { L57_FuncTransferParameter *p=Params[i];
      if(p->NameId==nameid)
        p->HasConnection=FALSE;
    }
}
//---------------------------------------------------------------------------
// �������� ������ ��������
void L57_FuncTransfer::ClearOldData(void)
{ for(int i=0;i<ParamsCount;i++)
    Params[i]->ClearOldData();
}
//---------------------------------------------------------------------------
// ��������� ��������� � �������� ���������
void L57_FuncTransfer::CheckInputsAndTransfer(void *vardata)
{ char *p_Start=(char*)vardata;
  void *varaddr;
  // �������� �������� � ���������
  for(int i=0;i<ParamsCount;i++)
    { L57_FuncTransferParameter *p=Params[i];
      if(!p->HasConnection)
        continue;
      if(p->BlockVarNum<0)
        continue;
      if(p_Start[p->ChangeSignalOffset]==0)
        continue;
      // ���� ������, �������� ������� ���������
      if(p->Matrix)
        { // �������
          varaddr=(p_Start+p->VarOffset);
          if(p->OldMatrix.CompareMatrAndCopy(varaddr))
            continue;
        }
      else
        { // �����
          double *pval=(double *)(p_Start+p->VarOffset);
          if( (*pval)==p->OldValue )
            continue;
          varaddr=pval;
          p->OldValue=(*pval);
        }
      // ���� ���������
      TransferParameter(varaddr,p,DestBlocks,FALSE);
    } // for(int i=0;...)

  // ���������� �������
  for(int i=0;i<ParamsCount;i++)
    { L57_FuncTransferParameter *p=Params[i];
      p_Start[p->ChangeSignalOffset]=0;
    }
}
//---------------------------------------------------------------------------
#endif


// �������� ����� ������-����������
void L57_FuncTransfer::ResetSenderBlockNames(void)
{
  for(int i=0;i<ParamsCount;i++)
    Params[i]->SenderBlockNames.Clear();
}
//---------------------------------------------------------------------------

// ������������� ������� �� ������� �������� �������� ����������
BOOL L57_FuncTransfer::CheckTransferReaction(RDS_BHANDLE block,int funcoutputnum,L57_CheckTranser_Param *param)
{
  if(param==NULL)
    return FALSE;
  if(!L57_CANACCESSFIELD(param,InfFunc)) // �������� ������ ���������
    return FALSE;
  switch(param->Mode)
    { case L57_CTM_PREPARE: // ����������
        if(block==param->DestinationBlock)
          ListInputConnections(block,param->Object,param->InfFunc);
        else
          CheckInputConnections(block);
        if(funcoutputnum>=0)
          ListFunctionDestinations(block,funcoutputnum);
        ResetSenderBlockNames();
        break;
      case L57_CTM_CHECK: // �������� ��������
        for(int i=0;i<ParamsCount;i++)
          TestTransferParameter(block,Params[i],param);
        break;
    }
  return TRUE;
}
//---------------------------------------------------------------------------

// ��������� ������ ������ �� ������� ������ � ������� ����������
void L57_FuncTransfer::ListInputConnections(RDS_BHANDLE block,LPVOID Object,L57_VARLISTCB Func)
{
  if(Func==NULL)
    { CheckInputConnections(block);
      return;
    }
  for(int i=0;i<ParamsCount;i++)
    { L57_FuncTransferParameter *p=Params[i];
      if(p->BlockVarNum<0)
        continue;
      p->HasConnection=L57_ListInputSources(block,p,Object,Func);
    }
}
//---------------------------------------------------------------------------


// ���������� ������� ������ � ������� ����������
void L57_FuncTransfer::CheckInputConnections(RDS_BHANDLE block)
{
  for(int i=0;i<ParamsCount;i++)
    { L57_FuncTransferParameter *p=Params[i];
      if(p->BlockVarNum<0)
        continue;
      p->HasConnection=L57_CheckInputSource(block,p->BlockVarNum,&(p->ConnBlock));
    }

  /*
  L57_String s;
  for(int i=0;i<ParamsCount;i++)
    { L57_FuncTransferParameter *p=Params[i];
      char *name;
      if(p->HasConnection)
        continue;
      name=rdsGetRegisteredUserName(varnames_set,p->NameId);
      s.AddString(name);
      s.AddString("_");
      s.AddInt(p->SuffixId);
      s.AddString("\n");
    }
  rdsMessageBox(s.c_str(),"��� ������",MB_OK);
  */
}
//---------------------------------------------------------------------------

// ������� ������ ������-�����������
void L57_FuncTransfer::ListFunctionDestinations(RDS_BHANDLE block,int outnum)
{
  if(!DestBlocks)
    DestBlocks=rdsBCLCreateList(NULL,0,FALSE);
  rdsCommandObject(DestBlocks,RDS_HBCL_CLEAR);
  DestInputs.Clear();
  rdsSetObjectInt(DestBlocks,RDS_HBCL_AUTODELETE,0,1); // ������������ �� ������
  L57_AppendFunctionDestList(block,outnum,DestBlocks,&DestInputs);

  /*
  L57_String s;
  int count;
  RDS_BHANDLE *arrBlocks=(RDS_BHANDLE*)rdsGetObjectArray(DestBlocks,RDS_HBCL_BLOCKARRAY,0,&count);
  for(int i=0;i<count;i++)
    { char *name=rdsCreateFullBlockNameString(arrBlocks[i],NULL);
      if(name)
        { s.AddString(name);
          s.AddString("\n");
          rdsFree(name);
        }
    }
  rdsMessageBox(s.c_str(),"����������",MB_OK);
  */
}
//---------------------------------------------------------------------------

// ���������� NameId � SuffixId � L57_AcceptParam_Param
void L57_FuncTransferParameter::CopyIds(L57_AcceptParam_Param *param)
{ if(ChangedNameId<0)
    param->NameId=NameId;
  else
    param->NameId=ChangedNameId;

  if(ClearSuffix)
    param->SuffixId=L57_VARSUFFIX_NONE;
  else
    param->SuffixId=SuffixId;
}
//---------------------------------------------------------------------------


// �������� �������� ��������� ������-�����������
void L57_FuncTransfer::TestTransferParameter(RDS_BHANDLE block,L57_FuncTransferParameter *p,L57_CheckTranser_Param *cparam)
{ L57_AcceptParam_Param param;
  int count;
  RDS_BHANDLE *arrBlocks;

  Calls.Clear();

  if(!DestBlocks)
    return;
  if(!p->HasConnection)
    return;
  if(p->BlockVarNum<0)
    return;

  arrBlocks=(RDS_BHANDLE*)rdsGetObjectArray(DestBlocks,RDS_HBCL_BLOCKARRAY,0,&count);
  if(count<=0 || p==NULL)
    return;

  param.servSize=sizeof(param);
  p->CopyIds(&param);
  param.Data=NULL;
  // param.DestVarNum ���������� �����
  param.Type=L57_APPTYPE_BLOCK;
  param.SourceBlock=block;
  param.DestinationBlock=cparam->DestinationBlock;
  param.Object=cparam->Object;
  param.InfFunc=cparam->InfFunc;

  param.CallObject=&Calls;
  param.CallFunc=L57_TransferCallsList_AddParam;
  param.TransferId=GenerateParamTransferId();

  for(int i=0;i<count;i++)
    { RDS_BHANDLE b=arrBlocks[i];
      if(b)
        { param.DestVarNum=(i>=0 && i<DestInputs.Count)?DestInputs.Items[i]:-1;
          rdsCallBlockFunction(b,transfer_func,&param);
        }
    }
  // ���������� ���������� �������
  DoDelayedTransferCalls(&param);
}
//---------------------------------------------------------------------------


// ��������� �������� �������� ������-�����������, ������� � ��� ���
void L57_FuncTransfer::RetransferParameter(L57_AcceptParam_Param* Param,int newnameid,int newsuffixid)
{ L57_AcceptParam_Param newparam;
  int count;
  RDS_BHANDLE *arrBlocks;


  if(!DestBlocks)
    return;

  if(Param==NULL)
    return;
  if(!L57_CANACCESSFIELD(Param,DestVarNum)) // �������� ������ ���������
    return;

  arrBlocks=(RDS_BHANDLE*)rdsGetObjectArray(DestBlocks,RDS_HBCL_BLOCKARRAY,0,&count);
  if(count<=0)
    return;

  #define SET(field,error) newparam.field=L57_CANACCESSFIELD(Param,field)?Param->field:error

  newparam.servSize=sizeof(newparam);
  newparam.NameId=newnameid;
  newparam.SuffixId=(newsuffixid<0?Param->SuffixId:newsuffixid);
  newparam.Data=Param->Data;
  newparam.Type=Param->Type;
  // param.DestVarNum ���������� �����

  SET(SourceBlock,NULL);
  SET(DestinationBlock,NULL);
  SET(Object,NULL);
  SET(InfFunc,NULL);

  newparam.CallFunc=L57_TransferCallsList_AddParam;
  newparam.CallObject=&Calls;
  SET(TransferId,-1);

  #undef SET

  for(int i=0;i<count;i++)
    { RDS_BHANDLE b=arrBlocks[i];
      if(b)
        { newparam.DestVarNum=(i>=0 && i<DestInputs.Count)?DestInputs.Items[i]:-1;
          rdsCallBlockFunction(b,transfer_func,&newparam);
        }
    }
  // ���������� ���������� �������
  DoDelayedTransferCalls(&newparam);
}
//---------------------------------------------------------------------------


// ��������� ���������� ������ �������� ����������
void L57_FuncTransfer::DoDelayedTransferCalls(L57_AcceptParam_Param *param)
{
  while(Calls.First)
    { L57_TransferCall *c=Calls.First;
      c->Extract();
      if(c->Block)
        { int old_vn=param->DestVarNum;
          param->DestVarNum=c->DestVarNum;
          rdsCallBlockFunction(c->Block,transfer_func,param);
          param->DestVarNum=old_vn;
        }
      delete c;
    }
}
//---------------------------------------------------------------------------

// ��������� ���������� ������ �������� �������
void L57_FuncTransfer::DoDelayedFuncTransferCalls(L57_AcceptFunc_Param_Queue *queue)
{
  while(FuncCalls.First)
    { L57_TransferCall *c=FuncCalls.First;
      c->Extract();
      if(c->Block)
        { L57_AcceptFunc_Param param;
          param.servSize=sizeof(param);
          param.Texts=c->Texts;
          param.TextsCount=c->TextsCount;
          param.TextIndex=c->TextIndex;
          param.AcceptIndex=c->AcceptIndex;
          param.Immediate=c->Immediate;
          param.DestVarNum=c->DestVarNum;
          param.Queue=queue;
             /*
             char *name=rdsCreateFullBlockNameString(c->Block,NULL);
             if(rdsMessageBox(name,"",MB_YESNO)!=IDYES)
               { rdsFree(name); delete c; FuncCalls.Clear(); return; }
             rdsFree(name);
             */
          rdsCallBlockFunction(c->Block,func_transfer_func,&param);
        }
      delete c;
    }
}
//---------------------------------------------------------------------------

// �������� �������� ������-�����������
#ifndef L57_NORDSCLASSES
void L57_FuncTransfer::TransferParameter(RDS_BHANDLE /*block*/,L57_FuncTransferParameter *p,RDS_HOBJECT /*DestBlocks*/,BOOL checkconn)
{ L57_AcceptParam_Param param;
  int count;
  BOOL notset=TRUE;
  RDS_BHANDLE *arrBlocks;

  if(!DestBlocks)
    return;
  if(checkconn)
    { if(!p->HasConnection)
        return;
    }

  arrBlocks=(RDS_BHANDLE*)rdsGetObjectArray(DestBlocks,RDS_HBCL_BLOCKARRAY,0,&count);
  if(count<=0 || p==NULL)
    return;

  param.servSize=sizeof(param);
  p->CopyIds(&param);
  //param.NameId=p->NameId;
  //param.SuffixId=p->SuffixId;
  if(p->pPlainVar) // ������ ���������� double
    { param.Data=p->pPlainVar;
      param.Type=L57_APPTYPE_DOUBLE;
      notset=FALSE;
    }
  if(notset && p->pRdsVar!=NULL)
    { // ���������� ������������������ �����
      if(p->Matrix)
        { param.Type=L57_APPTYPE_RDSMATR;
          param.Data=p->pRdsVar->GetVoidPtr();
        }
      else
        { param.Type=L57_APPTYPE_DOUBLE;
          param.Data=p->pRdsVar->GetVoidPtr();
        }
      notset=FALSE;
    }

  if(notset)
    return;

  param.CallObject=&Calls;
  param.CallFunc=L57_TransferCallsList_AddParam;
  param.TransferId=GenerateParamTransferId();

  for(int i=0;i<count;i++)
    { RDS_BHANDLE b=arrBlocks[i];
      if(b)
        { param.DestVarNum=(i>=0 && i<DestInputs.Count)?DestInputs.Items[i]:-1;
          rdsCallBlockFunction(b,transfer_func,&param);
        }
    }

  // ���������� ���������� �������
  DoDelayedTransferCalls(&param);
}
#else
void L57_FuncTransfer::TransferParameter(void *varaddr,L57_FuncTransferParameter *p,RDS_HOBJECT /*DestBlocks*/,BOOL checkconn)
{ L57_AcceptParam_Param param;
  int count;
  RDS_BHANDLE *arrBlocks;

  if(!DestBlocks)
    return;
  if(checkconn)
    { if(!p->HasConnection)
        return;
    }

  arrBlocks=(RDS_BHANDLE*)rdsGetObjectArray(DestBlocks,RDS_HBCL_BLOCKARRAY,0,&count);
  if(count<=0 || p==NULL)
    return;

  param.servSize=sizeof(param);
  p->CopyIds(&param);
  //param.NameId=p->NameId;
  //param.SuffixId=p->SuffixId;
  //param.DestVarNum=-1;
  if(p->Matrix)
    param.Type=L57_APPTYPE_RDSMATR;
  else
    param.Type=L57_APPTYPE_DOUBLE;
  param.Data=varaddr;

  param.CallObject=&Calls;
  param.CallFunc=L57_TransferCallsList_AddParam;
  param.TransferId=GenerateParamTransferId();

  for(int i=0;i<count;i++)
    { RDS_BHANDLE b=arrBlocks[i];
      if(b)
        { param.DestVarNum=(i>=0 && i<DestInputs.Count)?DestInputs.Items[i]:-1;
          rdsCallBlockFunction(b,transfer_func,&param);
        }
    }
  // ���������� ���������� �������
  DoDelayedTransferCalls(&param);
}
#endif
//---------------------------------------------------------------------------

// ������� ��������
// ������� - ���� ����� �������� � �� ���������
BOOL L57_FuncTransfer::AcceptParam(L57_AcceptParam_Param* Param,RDS_BHANDLE thisblock)
{ BOOL NameFound=FALSE,accepted=FALSE,changed=FALSE;
  L57_FuncTransferParameter *p_found=NULL;
  int namereplace;

  if(Param==NULL)
    return FALSE;
  if(!L57_CANACCESSFIELD(Param,DestVarNum)) // �������� ������ ���������
    return FALSE;

  if(L57_CANACCESSFIELD(Param,TransferId))
    { if(Param->TransferId==LastTransferId) // ������ - ��� �������� ��� �����������
        { /*rdsMessageBox("Error","Transfer",MB_OK);
          rdsStopCalc();*/
          return FALSE;
        }
      LastTransferId=Param->TransferId;
    }

  namereplace=Param->NameId; // ������� ������ ����� ���

  // ���� ����� ��������
  for(int i=0;i<ParamsCount;i++)
    { L57_FuncTransferParameter *p=Params[i];
      if(p->NameId==Param->NameId)
        { // �������� ��� �������
          if(p->SuffixId==Param->SuffixId) // ������� ������
            { if(p->HasConnection) // ���� �������� �������� �� ����� - �� ��������� � �� �������� ������
                return FALSE;
              p_found=p;
              if(p->ChangedNameId>=0)
                namereplace=p->ChangedNameId;
              break;
            }
          NameFound=TRUE;
          if(p->ChangedNameId>=0)
            namereplace=p->ChangedNameId;
        }
    }
  if(!p_found) // ������ � �������� ���
    { int count;
      RDS_BHANDLE *arrBlocks;
      if(NameFound && DestBlocks!=NULL) // ���� �������� � ����� �� �������� ������ - �������� ������
        { if(namereplace!=Param->NameId) // ���� ������ �����
            { RetransferParameter(Param,namereplace);
              DoDelayedTransferCalls(Param);
              return FALSE;
            }
          arrBlocks=(RDS_BHANDLE*)rdsGetObjectArray(DestBlocks,RDS_HBCL_BLOCKARRAY,0,&count);
          for(int i=0;i<count;i++)
            { RDS_BHANDLE b=arrBlocks[i];
              if(b)
                { int old_dv=Param->DestVarNum,newdv;
                  newdv=(i>=0 && i<DestInputs.Count)?DestInputs.Items[i]:-1;
                  if(L57_CANACCESSFIELD(Param,CallObject) && Param->CallFunc!=NULL)
                    { // ����� ������ ���������� �����
                      (Param->CallFunc)(Param->CallObject,b,newdv);
                    }
                  else
                    { Param->DestVarNum=newdv;
                      rdsCallBlockFunction(b,transfer_func,Param);
                      Param->DestVarNum=old_dv;
                    }
                }
            }
          // ���������� ���������� �������
          DoDelayedTransferCalls(Param);
        }
      return FALSE;
    }

  // ���� � �������� ����� �� �������� � ��� ����� ���������
  if(Param->Type==L57_APPTYPE_BLOCK)
    { // �������� ��������
      if(!L57_CANACCESSFIELD(Param,InfFunc)) // �������� ������ ���������
        return FALSE;
      accepted=TRUE;
      if(thisblock==Param->DestinationBlock)
        { // �������� ����� �� ����� ����������
          // ����� ��� ������ ����� � ��������, ������ ��� ���� ���������� ����� ���� �������� ��� ������� ������
          if(Param->InfFunc)
            (Param->InfFunc)(Param->Object,p_found->NameId,p_found->SuffixId,FALSE,Param->SourceBlock);
          // ������ ���������� �� �����
          return FALSE;
        }
    }

  #ifndef L57_NORDSCLASSES
  if((!accepted) && p_found->pPlainVar) // �������� - ������ ���������� double
    { // ����� ��������� ������ ���������� double
      double *src;
      if(p_found->Matrix) // ��� �� ������ ����
        return FALSE;
      if(Param->Type!=L57_APPTYPE_DOUBLE)
        return FALSE;
      src=(double*)(Param->Data);
      accepted=TRUE;
      if( (*(p_found->pPlainVar))!=(*src))
        { changed=TRUE;
          *(p_found->pPlainVar)=(*src);
        }
    }
  if( (!accepted) && p_found->pRdsVar!=NULL)
    { // ���������� ������������������ �����
      if(p_found->Matrix) // �������
        { if(Param->Type!=L57_APPTYPE_RDSMATR) // ������
            return FALSE;
          accepted=TRUE;
          if(!L57_CompareDoubleMatr(p_found->pRdsVar->GetVoidPtr(),Param->Data))
            { rdsCopyVarArray(p_found->pRdsVar->GetVoidPtr(),Param->Data);
              changed=TRUE;
            }
        }
      else // �����
        { double *dest,*src;
          if(Param->Type!=L57_APPTYPE_DOUBLE) // ������
            return FALSE;
          dest=(double*)(p_found->pRdsVar->GetVoidPtr());
          src=(double*)(Param->Data);
          accepted=TRUE;
          if((*dest)!=(*src))
            { *dest=(*src);
              changed=TRUE;
            }
        }
    }
  #else
    if(!accepted)
      accepted=TRUE;
  #endif

  if(accepted && DestBlocks!=NULL)
    { // �������� ������
      int count;
      RDS_BHANDLE *arrBlocks;
      if(namereplace!=Param->NameId) // ���� ������ �����
        { RetransferParameter(Param,namereplace,p_found->ClearSuffix?L57_VARSUFFIX_NONE:p_found->SuffixId);
        }
      else // ��� ������ �����
        { arrBlocks=(RDS_BHANDLE*)rdsGetObjectArray(DestBlocks,RDS_HBCL_BLOCKARRAY,0,&count);
          for(int i=0;i<count;i++)
            { RDS_BHANDLE b=arrBlocks[i];
              if(b)
                { int old_dv=Param->DestVarNum,newdv;
                  newdv=(i>=0 && i<DestInputs.Count)?DestInputs.Items[i]:-1;
                  if(L57_CANACCESSFIELD(Param,CallObject) && Param->CallFunc!=NULL)
                    { // ����� ������ ���������� �����
                      (Param->CallFunc)(Param->CallObject,b,newdv);
                    }
                  else
                    { Param->DestVarNum=newdv;
                      rdsCallBlockFunction(b,transfer_func,Param);
                      Param->DestVarNum=old_dv;
                    }
                }
            }
        }
      // ���������� ���������� �������
      DoDelayedTransferCalls(Param);
    }
  return changed && (Param->Type!=L57_APPTYPE_BLOCK);
}
//---------------------------------------------------------------------------

// ������� ��������� ������ ��� L57_CheckTransferParamCB (����������)
BOOL RDSCALL L57_CheckTransferParamCB(RDS_PPOINTDESCRIPTION /*thispoint*/,RDS_PPOINTDESCRIPTION thatpoint,LPVOID data)
{ L57_CheckParamConn_Param *param=(L57_CheckParamConn_Param*)data;

  if(!param)
    return FALSE;
  if(!L57_CANACCESSFIELD(param,ThisBlock)) // �������� ������ ���������
    return FALSE;
  if(!thatpoint->Source) // ���� (����� �����)
    return TRUE;
  rdsCallBlockFunction(thatpoint->Block,param->FuncId,param);
  return TRUE;
}
//---------------------------------------------------------------------------


// ���������, ���� �� �������� ��������� ���������� �� ����� ����� ��� ����� (����������)
void L57_FuncTransfer::CheckTransferParam(RDS_BHANDLE Block,L57_CheckParamConn_Param* Param,int funcin_num)
{ int unknown_count=0;
  if(Param==NULL)
    return;
  if(!L57_CANACCESSFIELD(Param,ThisBlock)) // �������� ������ ���������
    return;

  // ���������� ����������� ���������
  for(int i=0;i<Param->ParamCount;i++)
    { if(Param->Flags[i]!=L57_PTR_UNKNOWN) // ��� ����
        continue;
      // ���� �� ����� �������� � �����
      for(int j=0;j<ParamsCount;j++)
        { L57_FuncTransferParameter *p=Params[j];
          if(p->NameId==Param->NameIds[i] && p->SuffixId==Param->SuffixIds[i])
            { // �����
              if(p->HasConnection) // ���� �������� �������� �� �����
                { Param->Flags[i]=(Param->ThisBlock==Block)?L57_PTR_NOTRANS:L57_PTR_TRANS;
                  continue;
                }
            }
        }
      // �� �����, ��� ��� ���� ������ �� ��������
      unknown_count++;
    }

  if(!unknown_count) // ������ ��� ���� �� ����� ������������ �����������
    return;

  // ����� �������� �����-��������� �������
  rdsEnumConnectedBlocksByVar(Block,funcin_num,RDS_BEN_TRACELINKS,L57_CheckTransferParamCB,Param);
}
//---------------------------------------------------------------------------

// ��������� � ������� ����� ����� ��� �������� �������
void RDSCALL L57_FuncTransfer_QueueAcceptFuncCall(LPVOID object,RDS_BHANDLE block,LPVOID vparam)
{ L57_FuncTransfer *transfer=(L57_FuncTransfer*)object;
  if(transfer==NULL || vparam==NULL)
    return;
  transfer->QueueAcceptFuncCall(block,(L57_AcceptFunc_Param*)vparam);
}
//---------------------------------------------------------------------------

// ��������� � ������� ����� ����� ��� �������� �������
void L57_FuncTransfer::QueueAcceptFuncCall(RDS_BHANDLE block,L57_AcceptFunc_Param *param)
{ L57_TransferCall *call;
  if(!L57_CANACCESSFIELD(param,DestVarNum)) // �������� ������ ���������
    return;

  call=new L57_TransferCall();
  call->Block=block;
  call->ConnectLast(&FuncCalls);
  call->CopyFrom(param);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ ������������ ������� ��� �������� ���������
//---------------------------------------------------------------------------
// ������� ������ ��������� �������
BOOL L57_DblMatrTempStorage::Allocate(int rows,int cols)
{ int newsize=rows*cols*sizeof(double);
  if(newsize==0)
    { Clear();
      return TRUE;
    }
  if(Data!=NULL && newsize<=Size) // ��������� � ������
    { Rows=rows; Cols=cols; Size=newsize;
      return TRUE;
    }
  // � ������ �� ���������
  Clear();
  Rows=rows; Cols=cols; Size=newsize;
  Data=(double*)rdsAllocate(newsize);
  if(!Data)
    { Rows=Cols=Size=0;
      return FALSE;
    }
  return TRUE;
}
//---------------------------------------------------------------------------

#ifdef L57_NORDSCLASSES
// �������� � ���������� �����
BOOL L57_DblMatrTempStorage::CompareMatrAndCopy(void *matr)
{ RDS_ARRAYACCESSDATA AccData;
  BOOL equal;

  if(!rdsGetVarArrayAccessData(matr,&AccData)) // ������ - �������� ����������
    return TRUE;
  if(AccData.ItemSize!=sizeof(double) ||
     rdsGetVarArrayElementType(matr)!=RDS_VARTYPE_DOUBLE) // ������ - �������� ����������
    return TRUE;
  // matr - ������� double
  if(AccData.Rows!=Rows || AccData.Cols!=Cols)
    equal=FALSE;
  else
    { // ������� �������� ���������
      if(Data==NULL) // ��� ������ - ��������� ������
        return TRUE;
      // ��� �� ������
      equal=memcmp(AccData.Data,Data,Size)==0;
    }
  if(!equal)
    { if(Allocate(AccData.Rows,AccData.Cols))
        memcpy(Data,AccData.Data,Size);
    }
  return equal;
}
//---------------------------------------------------------------------------
#endif


#ifndef L57_NORDSCLASSES
// �������� � ���������� �����
BOOL L57_DblMatrTempStorage::CompareMatr(rdsbcppVarAncestor *matr,RDS_ARRAYACCESSDATA *pAccData)
{ RDS_ARRAYACCESSDATA _AccData,*paccess;
  if(pAccData)
    paccess=pAccData;
  else
    paccess=&_AccData;

  if(!matr->GetAccessData(paccess)) // ������ - �������� ����������
    return TRUE;
  if(paccess->ItemSize!=sizeof(double) ||
     rdsGetVarArrayElementType(matr->GetVoidPtr())!=RDS_VARTYPE_DOUBLE) // ������ - �������� ����������
    return TRUE;
  // matr - ������� double
  if(paccess->Rows!=Rows || paccess->Cols!=Cols)
    return FALSE;
  // ������� �������� ���������
  if(Data==NULL) // ��� ������ - ��������� ������
    return TRUE;
  // ��� �� ������
  return memcmp(paccess->Data,Data,Size)==0;
}
//---------------------------------------------------------------------------

// ��� ���������� ������� �������� � ���������� �����, ���� �� ���������
// ��������� ����� ������ � ������� �������� ����
// forcechange - �������, ��� ��������� ����, ���������� �� ������������ ��������
// ������� - ������� ��������� � ������ �������
BOOL L57_DblMatrTempStorage::Check(char *insignal,rdsbcppVarAncestor *matr,BOOL &outchanges,BOOL forcechange)
{ RDS_ARRAYACCESSDATA AccData;

  if(forcechange)
    { BOOL compare=CompareMatr(matr); // ��� ��������
      // ������������� �������, ��� ��������� ����
      outchanges=TRUE;
      if(matr->GetAccessData(&AccData) &&
         AccData.ItemSize==sizeof(double) &&
         rdsGetVarArrayElementType(matr->GetVoidPtr())==RDS_VARTYPE_DOUBLE)
        { if(Allocate(AccData.Rows,AccData.Cols))
            memcpy(Data,AccData.Data,Size);
        }
      return !compare;
    }


  if(insignal) // ���� ��������� �� ������� ������
    { if( (*insignal)==0 ) // ��� ���������
        return FALSE;
      // ������ ������ - ��������� ����������� ���������
      *insignal=0;
    }
  if(!CompareMatr(matr,&AccData))
    { outchanges=TRUE;
      if(!Allocate(AccData.Rows,AccData.Cols)) // ������
        return FALSE;
      memcpy(Data,AccData.Data,Size);
      return TRUE;
    }
  return FALSE;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ����������� ��������� ������� ����� ���������� (��������� ��������� ������)
//---------------------------------------------------------------------------
// �������� ���
void L57_Table1ArgFuncData::Clear(void)
{
  _ArgsArray.Clear();
  ArgsArray=&_ArgsArray;
  rdsFree(Results);
  ResArgCount=NResults=/*PrevArgCount=*/0;
  ArgIndex=-1;
  Results=NULL;
  _CurIndex=0;
  Output.Clear();
}
//---------------------------------------------------------------------------

// ������� ������� ����������� �� ArgCount � NResults
BOOL L57_Table1ArgFuncData::AllocateResultArrays(void)
{ int n,ArgCount=ArgsArray->Count;
  _CurIndex=0;
  //PrevArgCount=ResArgCount;
  OutputAllocError=FALSE; // ��� ���������� �� ������������ ��� ����������� ������ ��������� ������ ��� ��������� ������
  rdsFree(Results);
  Results=NULL;
  ResArgCount=0;
  if(ArgCount==0 || NResults==0)
    return TRUE;
  Results=L57_NewDouble(n=ArgCount*NResults);
  ResArgCount=ArgCount;
  if(Results==NULL)
    { NResults=0;
      return FALSE;
    }
  for(int i=0;i<n;i++)
    Results[i]=rdsbcppHugeDouble;
  return TRUE;
}
//---------------------------------------------------------------------------


// �������� ���������� ��� ����������� � ������ ���������� (������������ ��������� ���, ������� � PrevCount)
// ������� - ����� ����������� ����� ��� -1 ��� ������
int L57_Table1ArgFuncData::AddResultsFromPrevCount(void)
{ int ArgCount=ArgsArray->PrevCount;
  int n=ArgsArray->Count-ArgsArray->PrevCount;
  int new_count=ArgsArray->Count;
  double *newResults;
  int old_res_count=ArgCount*NResults,new_res_count;

  //PrevArgCount=ArgCount;

  if(n==0)
    return 0;
  if(n<0 || new_count<0)
    return -1;


  newResults=L57_NewDouble(new_res_count=new_count*NResults);
  if(newResults==NULL)
    return -1;
  if(Results!=NULL && old_res_count!=0)
    memcpy(newResults,Results,old_res_count*sizeof(double));
  rdsFree(Results);
  Results=newResults;
  ResArgCount=new_count;
  for(int i=old_res_count;i<new_res_count;i++)
    Results[i]=rdsbcppHugeDouble;
  return n;
}
//---------------------------------------------------------------------------

// ���������� ���������� � �������
BOOL L57_Table1ArgFuncData::SendToOutput(BOOL tocurindex)
{ int ArgCount=ArgsArray->Count;
  int endindex=tocurindex?_CurIndex:ArgCount;

  OutputAllocError=FALSE;
  if(ResArgCount!=ArgCount || ArgsArray->IsEmpty() || Results==NULL || ArgIndex<0)
    return TRUE;
  for(L57_TableFiller *t=Output.First;t!=NULL;t=t->Next)
    if(t->Matr!=NULL && t->Count!=0)
      { RDS_ARRAYACCESSDATA AccData;
        double *ptr_matr,*ptr_res;
        int maxcol=-1;
        if(t->pReady)
          *(t->pReady)=0;
        for(int i=0;i<t->Count;i++)
          if(t->MatrCol[i]>maxcol)
            maxcol=t->MatrCol[i];
        if(maxcol<1)
          continue;
        if(!t->Matr->GetAccessData(&AccData)) // ������
          continue;
        if(AccData.ItemSize!=sizeof(double) ||
          rdsGetVarArrayElementType(t->Matr->GetVoidPtr())!=RDS_VARTYPE_DOUBLE) // ������
          continue;
        // matr - ������� double
        if(!rdsResizeVarArray(t->Matr->GetVoidPtr(),ArgCount,maxcol+1,FALSE,&AccData))
          { rdsResizeVarArray(t->Matr->GetVoidPtr(),0,0,FALSE,NULL);
            OutputAllocError=TRUE;
            continue;
          }
        // ������ ����������
        ptr_matr=(double*)AccData.Data;
        ptr_res=Results;
        for(int i=0;i<endindex;i++,ptr_matr+=AccData.Cols,ptr_res+=NResults)
          { ptr_matr[0]=ArgsArray->Data[i];
            for(int j=0;j<t->Count;j++)
              { int col=t->MatrCol[j];
                if(col>0)
                  ptr_matr[col]=ptr_res[t->ResultN[j]];
              }
          }
        if(t->pReady)
          *(t->pReady)=1;
        if(ArgCount!=ArgsArray->PrevCount) // ���� ����������� ���������
          L57_SortMatrix((double*)AccData.Data,AccData.Rows,AccData.Cols);
      }
  return !OutputAllocError;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����������� ��������� ������� ���� ���������� (��������� ��������� ������)
//---------------------------------------------------------------------------
// �������� ���
void L57_Table2ArgFuncData::Clear(void)
{
  _Args1Array.Clear();
  _Args2Array.Clear();
  Args1Array=&_Args1Array;
  Args2Array=&_Args2Array;

  for(int i=0;i<ResArg1Count;i++)
    rdsFree(Results[i]);
  rdsFree(Results);
  Results=NULL;

  ResArg1Count=ResArg2Count=NResults=0;
  Arg1Index=Arg2Index=-1;
  _CurIndex1=_CurIndex2=0;
  Output.Clear();
}
//---------------------------------------------------------------------------

// ������� ������� ����������� �� Arg*Count � NResults
BOOL L57_Table2ArgFuncData::AllocateResultArrays(void)
{ int n,Arg1Count=Args1Array->Count,Arg2Count=Args2Array->Count;
  _CurIndex1=_CurIndex2=0;
  OutputAllocError=FALSE; // ��� ���������� �� ������������ ��� ����������� ������ ��������� ������ ��� ��������� ������

  for(int i=0;i<ResArg1Count;i++)
    rdsFree(Results[i]);
  rdsFree(Results);
  Results=NULL;
  ResArg1Count=ResArg2Count=0;

  if(Arg1Count==0 || Arg2Count==0 || NResults==0)
    {
      return TRUE;
    }
  Results=L57_NewDoublePtr(Arg1Count);
  if(Results==NULL)
    return FALSE;
  ResArg1Count=Arg1Count;
  ResArg2Count=Arg2Count;

  for(int i=0;i<Arg1Count;i++)
    { double *ptr=L57_NewDouble(n=Arg2Count*NResults);
      Results[i]=ptr;
      for(int j=0;j<n;j++)
        ptr[j]=rdsbcppHugeDouble;
    }
  return TRUE;
}
//---------------------------------------------------------------------------

// �������� ���������� ��� ����������� � ������ ���������� (������������ ��������� ���, ������� � PrevCount)
BOOL L57_Table2ArgFuncData::AddResultsFromPrevCount(void)
{ int Arg1Count=Args1Array->PrevCount,
      Arg2Count=Args2Array->PrevCount;
  int n1=Args1Array->Count-Args1Array->PrevCount,
      n2=Args2Array->Count-Args2Array->PrevCount;
  int new1_count=Args1Array->Count,
      new2_count=Args2Array->Count;

  if(n1==0 && n2==0)
    return TRUE;
  if(n1<0 || n2<0 || new1_count<0 || new2_count<0)
    return FALSE;

  if(NResults==0)
    return TRUE;

  // ����������� ����������
  if(n1) // ���������� ����� �����
    { double **newResults;
      int n=new2_count*NResults;
      newResults=L57_NewDoublePtr(new1_count);
      if(!newResults)
        return FALSE;
      if(Arg1Count)
        memcpy(newResults,Results,Arg1Count*sizeof(double*));
      rdsFree(Results);
      Results=newResults;
      for(int i=Arg1Count;i<new1_count;i++)
        { double *ptr=L57_NewDouble(n);
          Results[i]=ptr;
          for(int j=0;j<n;j++)
            ptr[j]=rdsbcppHugeDouble;
        }
      ResArg1Count=new1_count;
    }

  if(n2) // ���������� ����� ��������
    { // ����������� ������ ������
      int old_size=Arg2Count*NResults;
      int old_size_d=old_size*sizeof(double);
      int new_size=new2_count*NResults;
      for(int i=0;i<Arg1Count;i++)
        { double *ptr=L57_NewDouble(new_size);
          if(!ptr)
            return FALSE;
          if(Arg2Count)
            memcpy(ptr,Results[i],old_size_d);
          rdsFree(Results[i]);
          Results[i]=ptr;
          for(int j=old_size;j<new_size;j++)
            ptr[j]=rdsbcppHugeDouble;
        }
      ResArg2Count=new2_count;
    }
  return TRUE;
}
//---------------------------------------------------------------------------

// ���������� ���������� � ������� �� 0 �� _CurIndex1,_CurIndex2
BOOL L57_Table2ArgFuncData::SendToOutput(BOOL tocurindices)
{ int Arg1Count=Args1Array->Count,
      Arg2Count=Args2Array->Count;
  int endindex1,endindex2;

  if(tocurindices)
    { endindex1=_CurIndex1;
      endindex2=_CurIndex2;
    }
  else
    { endindex1=Arg1Count;
      endindex2=Arg2Count;
    }

  OutputAllocError=FALSE;
  if(Args1Array->IsEmpty() || Arg1Index<0 ||
     Args2Array->IsEmpty() || Arg2Index<0 ||
     Results==NULL || ResArg1Count==0 || ResArg2Count==0)
    return TRUE;

  for(L57_TableFiller *t=Output.First;t!=NULL;t=t->Next)
    if(t->Matr!=NULL)
      { RDS_ARRAYACCESSDATA AccData;
        double *ptr_matr,*ptr_res;

        if(t->pReady)
          *(t->pReady)=0;
        if(!t->Matr->GetAccessData(&AccData)) // ������
          continue;
        if(AccData.ItemSize!=sizeof(double) ||
          rdsGetVarArrayElementType(t->Matr->GetVoidPtr())!=RDS_VARTYPE_DOUBLE) // ������
          continue;
        // matr - ������� double
        if(t->Transp) // ���������������
          { double *lt;
            if(!rdsResizeVarArray(t->Matr->GetVoidPtr(),Arg2Count+1,Arg1Count+1,FALSE,&AccData))
              { rdsResizeVarArray(t->Matr->GetVoidPtr(),0,0,FALSE,NULL);
                OutputAllocError=TRUE;
                continue;
              }
            // ������ ����������
            ptr_matr=(double*)AccData.Data;
            // ������ ������ - �������� ��������� 1
            ptr_matr[0]=rdsbcppHugeDouble;
            for(int c=0;c<Arg1Count;c++)
              ptr_matr[c+1]=Args1Array->Data[c];
            ptr_matr+=AccData.Cols;
            lt=ptr_matr+1; // ����� ������� ���� ����������� �������
            // ������ ������� - �������� ��������� 2
            for(int r=0;r<Arg2Count;r++,ptr_matr+=AccData.Cols)
              *ptr_matr=Args2Array->Data[r];
            // ������
            for(int r=0;r<endindex2;r++,lt+=AccData.Cols)
              { // lt - ������ ������ ������ ������� (��� ������� ���������)
                for(int c=0;c<endindex1;c++)
                  lt[c]=Results[c][r*NResults+t->TwoArgResult]; // �����������������
              }
            if(Arg1Count!=Args1Array->PrevCount) // ���� ����������� �������
              L57_SortMatrixFieldByRow((double*)AccData.Data,AccData.Rows,AccData.Cols);
            if(Arg2Count!=Args1Array->PrevCount) // ���� ����������� ������
              L57_SortMatrixFieldByCol((double*)AccData.Data,AccData.Rows,AccData.Cols);
          }
        else // if(t->Transp)
          { // �� ���������������
            if(!rdsResizeVarArray(t->Matr->GetVoidPtr(),Arg1Count+1,Arg2Count+1,FALSE,&AccData))
              { rdsResizeVarArray(t->Matr->GetVoidPtr(),0,0,FALSE,NULL);
                OutputAllocError=TRUE;
                continue;
              }
            // ������ ����������
            ptr_matr=(double*)AccData.Data;
            // ������ ������ - �������� ��������� 2
            ptr_matr[0]=rdsbcppHugeDouble;
            for(int c=0;c<Arg2Count;c++)
              ptr_matr[c+1]=Args2Array->Data[c];
            ptr_matr+=AccData.Cols;
            // �� ������ ������ - [��������1][...������...]
            for(int r=0;r<endindex1;r++,ptr_matr+=AccData.Cols)
              { ptr_matr[0]=Args1Array->Data[r];
                ptr_res=Results[r];
                for(int c=0;c<endindex2;c++,ptr_res+=NResults)
                  ptr_matr[c+1]=ptr_res[t->TwoArgResult];
              }
            for(int r=endindex1;r<Arg1Count;r++,ptr_matr+=AccData.Cols)
              ptr_matr[0]=Args1Array->Data[r];
            if(Arg1Count!=Args1Array->PrevCount) // ���� ����������� ������
              L57_SortMatrixFieldByCol((double*)AccData.Data,AccData.Rows,AccData.Cols);
            if(Arg2Count!=Args1Array->PrevCount) // ���� ����������� �������
              L57_SortMatrixFieldByRow((double*)AccData.Data,AccData.Rows,AccData.Cols);
          } // if(t->Transp) ... else
        if(t->pReady)
          *(t->pReady)=1;
      }
  return !OutputAllocError;
}
//---------------------------------------------------------------------------
#endif // #ifndef L57_NORDSCLASSES

//---------------------------------------------------------------------------
// ����� �������
//---------------------------------------------------------------------------
// ������� L57.dll ��� ��������� ������������ �������� ������� ������ ��� ������� �������
void L57_ExecTool_GetDynamicDescription(L57_String &templatetext,L57_String &modeltext,
                                        L57_String *pFunc,L57_String *pInputs,L57_String *pNumbered,L57_String *pTitle)
{ RDS_TOOLEXTCALLDATA data;
  LPSTR array[2];

  if(pFunc) pFunc->Clear();
  if(pInputs) pInputs->Clear();
  if(pNumbered) pNumbered->Clear();
  if(pTitle) pTitle->Clear();

  data.servSize=sizeof(data);
  data.Command=1;
  data.NInStrings=2;
  data.InStrings=array;
  array[0]=templatetext.c_str();
  array[1]=modeltext.c_str();
  data.NOutStrings=0;
  data.OutStrings=NULL;
  rdsExecToolExt(RDS_ETF_S_BYDLL,"ProcessVarAndFunc","$DLL$\\L57.dll",&data);
  if(data.NOutStrings>=4 && data.OutStrings!=NULL)
    { if(pFunc)
        { pFunc->AssignRdsDynamicString(data.OutStrings[0]);
          data.OutStrings[0]=NULL;
        }
      if(pInputs)
        { pInputs->AssignRdsDynamicString(data.OutStrings[1]);
          data.OutStrings[1]=NULL;
        }
      if(pNumbered)
        { pNumbered->AssignRdsDynamicString(data.OutStrings[2]);
          data.OutStrings[2]=NULL;
        }
      if(pTitle)
        { pTitle->AssignRdsDynamicString(data.OutStrings[3]);
          data.OutStrings[3]=NULL;
        }
    }
  rdsClearExecToolExtResults(&data);
}
//---------------------------------------------------------------------------

// ������� L57.dll � �������� � ����������� ���������� ������
void L57_ExecTool_UpdatePossible(void)
{ RDS_TOOLEXTCALLDATA data;

  data.servSize=sizeof(data);
  data.Command=8;
  data.NInStrings=0;
  data.InStrings=NULL;
  data.NOutStrings=0;
  data.OutStrings=NULL;
  rdsExecToolExt(RDS_ETF_S_BYDLL,"ProcessVarAndFunc","$DLL$\\L57.dll",&data);
  rdsClearExecToolExtResults(&data);
}
//---------------------------------------------------------------------------

// ������� L57.dll � �������� � ���������� � ���������� ���������� ������� ������� ��� �����
BOOL L57_ExecTool_CanEditTextRect(void)
{ RDS_TOOLEXTCALLDATA data;

  data.servSize=sizeof(data);
  data.Command=10;
  data.NInStrings=0;
  data.InStrings=NULL;
  data.NOutStrings=0;
  data.OutStrings=NULL;
  rdsExecToolExt(RDS_ETF_S_BYDLL,"ProcessVarAndFunc","$DLL$\\L57.dll",&data);
  rdsClearExecToolExtResults(&data);
  return data.Result!=0;
}
//---------------------------------------------------------------------------

// �������� ������ ��� ����� ������ �����
int L57_GetDynModelFileName(RDS_BHANDLE block,L57_String &name)
{ RDS_PCOMPMODELDATA model;
  name.Clear();
  if(!block)
    return 0;
  model=rdscompGetBlockModelData(block);
  if(!model)
    return 0;
  // � ����� ���� ������
  name.AssignRdsDynamicString(rdsGetFullFilePath(model->ModelName,NULL,NULL));
  return name.IsNotEmpty();
}
//---------------------------------------------------------------------------

#ifdef L57_LOG
  // �������� ������ ��� ����� ������� ��� ������ �����
  int L57_Log_GetLogFileName(RDS_BHANDLE block,L57_String &name)
  {
    if(!L57_GetDynModelFileName(block,name))
      return 0;
    name.AddString(".log");
    return 1;
  }
  //---------------------------------------------------------------------------

  // �������� ������ ������ �����
  void L57_Log_Clear(RDS_BHANDLE block)
  { L57_String name;
    if(!L57_Log_GetLogFileName(block,name))
      return;
    if(name.IsEmpty())
      return;
    DeleteFile(name.c_str());
  }
  //---------------------------------------------------------------------------

  // �������� ������ � ������ ������ �����
  void L57_Log_Add(RDS_BHANDLE block,char *text,BOOL crlf)
  { L57_File f;
    L57_String name;
    BOOL hastext=(text!=NULL && text[0]!=0);
    if((!hastext) && (!crlf))
      return;
    if(!L57_Log_GetLogFileName(block,name))
      return;
    if(!f.OpenAppend(name.c_str()))
      return;
    if(hastext)
      f.WindowsWrite(text,strlen(text));
    if(crlf)
      f.WindowsWrite("\r\n",2);
  }
  void L57_Log_Add(RDS_BHANDLE block,const L57_String &text,BOOL crlf)
  { L57_Log_Add(block,text.c_str(),crlf);
  }
  //---------------------------------------------------------------------------

  // �������� ������ � ����� ����� � ������ ������ �����
  void L57_Log_AddInt(RDS_BHANDLE block,char *title,int v,BOOL crlf)
  { L57_String s;
    if(title)
      s.Assign(title);
    s.AddInt(v);
    L57_Log_Add(block,s,crlf);
  }
  //---------------------------------------------------------------------------

  // �������� ������ � ������������ ����� � ������ ������ �����
  void L57_Log_AddDouble(RDS_BHANDLE block,char *title,double v,BOOL crlf)
  { L57_String s;
    if(title)
      s.Assign(title);
    s.AddDouble(v);
    L57_Log_Add(block,s,crlf);
  }
  //---------------------------------------------------------------------------
#endif

// ��������� ����� ����� ������ �����
int L57_LoadModelFile(RDS_BHANDLE block,L57_String &text)
{ L57_String filename;
  L57_File f;

  if(!L57_GetDynModelFileName(block,filename))
    return 0;
  text.AssignRdsDynamicString(f.ReadFileToMemory(filename.c_str()));
  return text.IsNotEmpty();
}
//---------------------------------------------------------------------------

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

// ������� �� ����� ����� ��� ��������� ����� �����
BOOL L57_ResFile_MakeName(char *filename,L57_String &resname)
{ char *name;
  if(filename==NULL || (*filename)==0)
    return FALSE;
  name=rdsTransformFileName(filename,RDS_TFN_CHANGEEXT,".res",NULL);
  if(!name)
    return FALSE;
  resname.AssignRdsDynamicString(name);
  return TRUE;
}
//---------------------------------------------------------------------------

// ������� ����� ����� ������ ����� ����� ������ ���������� � ������� �������� �����
void L57_ResFile_MakeCopy(char *filename)
{ L57_String resname;
  if(!L57_FileExists(filename)) // ������ ����������
    return;
  // ��������� ��� �����
  if(!L57_ResFile_MakeName(filename,resname))
    return;
  // ���� ���
  if(L57_FileExists(resname.c_str())) // ���� ���� ����� - �� �������� ���
    return;
  // ����� ����� ���
  MoveFile(filename,resname.c_str());
  // ���������� ��� ����������
  rdsTMPRememberFileName(-1,resname.c_str());
}
//---------------------------------------------------------------------------

// ������� ����� ����� ������ �����, ��������� ����� ������ ���������� � ������� �������� �����
void L57_ResFile_DeleteCopy(char *filename)
{ L57_String resname;
  if(!L57_FileExists(filename)) // ��� ����� ������
    return;
  // ��������� ��� �����
  if(!L57_ResFile_MakeName(filename,resname))
    return;
  // ���� ���
  if(!L57_FileExists(resname.c_str())) // ��� ����� �����
    return;
  // ���� ����� ����
  DeleteFile(resname.c_str());
  // ������ � ���������
  rdsTMPForgetFileName(-1,resname.c_str());
}
void L57_ResFile_DeleteModelCopy(RDS_BHANDLE block)
{ L57_String filename;
  if(!L57_GetDynModelFileName(block,filename))
    return;
  L57_ResFile_DeleteCopy(filename.c_str());
}
//---------------------------------------------------------------------------

// ������������ ������ �� �����, ��������� ����� ������ ���������� � ������� �������� �����
void L57_ResFile_RestoreCopy(char *filename)
{ L57_String resname,text;
  int ok;
  if(!L57_FileExists(filename)) // ��� ����� ������
    return;
  // ��������� ��� �����
  if(!L57_ResFile_MakeName(filename,resname))
    return;
  // ���� ���
  if(!L57_FileExists(resname.c_str())) // ��� ����� �����
    return;
  // ���� ����� ����
  // ��������� ����� � ��������� � ������
  ok=L57_ReadFileToMemory(resname.c_str(),text);
  if(ok)
    { L57_File f;
      ok=f.WriteTextToFile(filename,text.c_str());
    }
  if(ok)
    DeleteFile(resname.c_str());
  else
    { DeleteFile(filename);
      MoveFile(resname.c_str(),filename);
    }
}
void L57_ResFile_RestoreModelCopy(RDS_BHANDLE block)
{ L57_String filename;
  if(!L57_GetDynModelFileName(block,filename))
    return;
  L57_ResFile_RestoreCopy(filename.c_str());
}
//---------------------------------------------------------------------------

// �������� ����� ����� ������ �����
int L57_SaveModelFile(RDS_BHANDLE block,char *text,BOOL saveres)
{ L57_String filename;
  L57_File f;
  if(!text)
    return 0;
  if(!L57_GetDynModelFileName(block,filename))
    return 0;

  if(saveres)
    { // ������ �����, ���� �� ��� ���
      L57_ResFile_MakeCopy(filename.c_str());
    }

  return f.WriteTextToFile(filename.c_str(),text);
}
int L57_SaveModelFile(RDS_BHANDLE block,L57_String &text,BOOL saveres)
{ return L57_SaveModelFile(block,text.c_str(),saveres); }
//---------------------------------------------------------------------------

// ��������� ����� ����� � ������
int L57_ReadFileToMemory(char *path,L57_String &text)
{ L57_File f;
  text.AssignRdsDynamicString(f.ReadFileToMemory(path));
  return text.IsNotEmpty();
}
//---------------------------------------------------------------------------

// ������� �������������� ������ ������ � ������ ����� � ������� ��� ��������������� �����
void L57_CreateAdditionalText(L57_String *pText,RDS_BHANDLE block,int funcin_num)
{
  if(!pText)
    return;
  if(block)
    { char *fn=rdsCreateFullBlockNameString(block,NULL);
      if(fn)
        { pText->AddString("Block=");
          pText->AddString(fn);
          pText->AddString("\n");
          rdsFree(fn);
        }
    }
  if(funcin_num>=0)
    { pText->AddString("FuncIn=");
      pText->AddInt(funcin_num);
      pText->AddString("\n");
    }
}
//---------------------------------------------------------------------------

// ������� L57.dll ��� �������� �������
int L57_ExecTool_Translate(RDS_BHANDLE block,char *templatefile,int textparam,L57_String *ptr_text)
{ RDS_TOOLEXTCALLDATA data;
  LPSTR array[3];
  L57_String modelfilename;

  if(!L57_GetDynModelFileName(block,modelfilename))
    return 0;

  data.servSize=sizeof(data);
  data.Command=11;
  data.Block=block;

  data.NInStrings=3;
  array[0]=templatefile;
  array[1]=modelfilename.c_str();
  array[2]=ptr_text?ptr_text->c_str():"";
  data.IntValue=textparam;
  data.InStrings=array;
  data.NOutStrings=0;
  data.OutStrings=NULL;

  rdsExecToolExt(RDS_ETF_S_BYDLL,"ProcessVarAndFunc","$DLL$\\L57.dll",&data);

  if(data.Result && data.OutStrings!=NULL)
    { if(data.NOutStrings>=1)
        { if(ptr_text!=NULL && data.OutStrings[0]!=NULL)
            { ptr_text->AssignRdsDynamicString(data.OutStrings[0]);
              data.OutStrings[0]=NULL;
            }
        }
    }
  rdsClearExecToolExtResults(&data);
  return data.Result;
}
//---------------------------------------------------------------------------

// ������� L57.dll ��� �������������� ������� � ����������
// templatetext==NULL - ��������� �������� �������� ��������� ����
int L57_ExecTool_Edit(char *templatetext,char *modeltext,char *params,int starttab,
                      L57_String *pNewModelText,
                      L57_String *pFunc,L57_String *pInputs,L57_String *pNumbered,L57_String *pTitle,
                      char *lockedits,L57_String *pExport,
                      char *additionaldata,RDS_BHANDLE *pShowBlock,
                      int *ptr_param,L57_String *ptr_text)
{ RDS_TOOLEXTCALLDATA data;
  LPSTR array[6];
  BOOL reopen;

  if(pNewModelText) pNewModelText->Clear();
  if(pFunc) pFunc->Clear();
  if(pInputs) pInputs->Clear();
  if(pNumbered) pNumbered->Clear();
  if(pTitle) pTitle->Clear();
  if(pExport) pExport->Clear();
  if(pShowBlock) *pShowBlock=NULL;

  reopen=(modeltext==NULL || templatetext==NULL);

  data.servSize=sizeof(data);
  data.Command=reopen?7:0;
  data.Block=NULL;
  if(reopen)
    { data.NInStrings=2;
      array[0]=additionaldata?additionaldata:"";
      array[1]="";
    }
  else
    { data.NInStrings=3; //lockedits?4:3;
      if(lockedits)
        (data.NInStrings)++;
      if(additionaldata)
        (data.NInStrings)++;
      if(ptr_text)
        (data.NInStrings)++;
      array[0]=templatetext;
      array[1]=modeltext;
      array[2]=params?params:"";
      array[3]=lockedits?lockedits:"";
      array[4]=additionaldata?additionaldata:"";
      array[5]=ptr_text?ptr_text->c_str():"";
      data.IntParam=starttab;
      data.IntValue=(ptr_param)?(*ptr_param):L57_BLOCKTEXT_NOSUPPORT;
    }
  data.InStrings=array;
  data.NOutStrings=0;
  data.OutStrings=NULL;
  rdsExecToolExt(RDS_ETF_S_BYDLL,"ProcessVarAndFunc","$DLL$\\L57.dll",&data);
  if(pShowBlock) *pShowBlock=data.Block;
  if(data.Result && data.OutStrings!=NULL)
    { if(data.NOutStrings>=5)
        { if(pNewModelText)
            { pNewModelText->AssignRdsDynamicString(data.OutStrings[0]);
              data.OutStrings[0]=NULL;
            }
          if(pFunc)
            { pFunc->AssignRdsDynamicString(data.OutStrings[1]);
              data.OutStrings[1]=NULL;
            }
          if(pInputs)
            { pInputs->AssignRdsDynamicString(data.OutStrings[2]);
              data.OutStrings[2]=NULL;
            }
          if(pNumbered)
            { pNumbered->AssignRdsDynamicString(data.OutStrings[3]);
              data.OutStrings[3]=NULL;
            }
          if(pTitle)
            { pTitle->AssignRdsDynamicString(data.OutStrings[4]);
              data.OutStrings[4]=NULL;
            }
        }
      if(data.NOutStrings>=6 && pExport!=NULL)
        { pExport->AssignRdsDynamicString(data.OutStrings[5]);
          data.OutStrings[5]=NULL;
        }
      if(data.NOutStrings>=7 && ptr_param!=NULL && data.OutStrings[6]!=0)
        { *ptr_param=atoi(data.OutStrings[6]);
        }
      if(data.NOutStrings>=8 && ptr_text!=NULL)
        { ptr_text->AssignRdsDynamicString(data.OutStrings[7]);
          data.OutStrings[7]=NULL;
        }
    }


  rdsClearExecToolExtResults(&data);
  return data.Result;
}
//---------------------------------------------------------------------------

// ������� L57.dll ��� ������ ����� � ������
int L57_ExecTool_SetOptions(char *templatetext,char *modeltext,char *options,L57_String *pNewModelText,L57_String *pNewExport)
{ RDS_TOOLEXTCALLDATA data;
  LPSTR array[3];

  if(pNewModelText) pNewModelText->Clear();
  if(pNewExport) pNewExport->Clear();

  if(modeltext==NULL || templatetext==NULL)
    return 0;

  data.servSize=sizeof(data);
  data.Command=2;
  data.NInStrings=3;
  data.InStrings=array;
  array[0]=templatetext;
  array[1]=modeltext;
  array[2]=options?options:"";
  data.NOutStrings=0;
  data.OutStrings=NULL;
  rdsExecToolExt(RDS_ETF_S_BYDLL,"ProcessVarAndFunc","$DLL$\\L57.dll",&data);

  if(data.Result && data.OutStrings!=NULL)
    { if(pNewModelText!=NULL && data.NOutStrings>=1)
        { pNewModelText->AssignRdsDynamicString(data.OutStrings[0]);
          data.OutStrings[0]=NULL;
        }
      if(pNewExport!=NULL && data.NOutStrings>=2)
        { pNewExport->AssignRdsDynamicString(data.OutStrings[1]);
          data.OutStrings[1]=NULL;
        }
    }
  rdsClearExecToolExtResults(&data);
  return data.Result;
}
//---------------------------------------------------------------------------

// ����������������� ������ �����
void L57_CompileModel(RDS_BHANDLE block,BOOL rebuild)
{ DWORD flags=rebuild?RDS_SYSCMDF_REBUILD:0;
  RDS_PCOMPMODULEDATA module;
  L57_String moduledescr;

  RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(block);
  if(!model)
    return;

  module=rdscompGetModelModuleData(model->Module);
  if(!module)
    return;

  moduledescr.Assign(module->DllFullPath);
  moduledescr.AddString("\n");
  moduledescr.AddString(module->DllFuncName);

  rdsExecuteCommand(RDS_SYSCMD_COMPILE,flags,moduledescr.c_str(),model->ModelName);
}
//---------------------------------------------------------------------------

// ����������� ����� ����� �� ��������� � ����
int L57_CalcNPointsByDiap(double vmin,double vmax,double dv)
{ int n;
  if(vmin==rdsbcppHugeDouble)
    return -1;
  // �.�. ���� �������� ������� �� ����� �����?
  if(vmax==rdsbcppHugeDouble || dv==rdsbcppHugeDouble || dv<=0.0 || vmax<vmin+0.5*dv)
    return 1;
  RDSBCPP_TRY
    { double d=(vmax-vmin)/dv;
      n=1+(int)(d);
      if(n<2)
        n=-1;
    }
  RDSBCPP_CATCHALL
    { n=-1; }
  if(n>0)
    { // ��������� ��������� �����
      double last=vmin+(n-1)*dv;
      if(vmax-last>0.1*dv)
        n++;
    }
  return n;
}
//-----------------------------------------------------------------------

// ���������� ��������� � ���� (������������� ������������ ��������)
// ���������� ����� ����� � ���������
int L57_AdjustDiapAndStep(double &_vmin,double &_vmax,double &_dv,double vmin,double vmax,double dv)
{ int n;

  if(dv==rdsbcppHugeDouble && vmin!=rdsbcppHugeDouble && vmax!=rdsbcppHugeDouble) // �������� ���������
    dv=(vmax-vmin)/2;

  n=L57_CalcNPointsByDiap(vmin,vmax,dv);
  if(n<1)
    { // ������ ��������� �����
      _vmin=_vmax=0.0;
      _dv=1.0;
      return 1;
    }
  if(n==1)
    { _vmin=_vmax=vmin;
      _dv=1.0;
      return 1;
    }

  _vmin=vmin;
  _vmax=vmax;
  _dv=dv;
  return n;
}
//-----------------------------------------------------------------------

// ����������� ������� ��� ������� ����� ����������
BOOL L57_SortMatrix(double *MData,int Rows,int Cols,int SortCol)
{ BOOL sorted=TRUE;
  double last,cur;

  #define XIndex SortCol

  if(Rows<2)
    return TRUE;
  if(SortCol>=Cols)
    return FALSE;

  // ������� - ��������, �� ������������� �� ���
  last=MData[XIndex];
  if(last==rdsbcppHugeDouble)
    return FALSE;
  for(int i=1;sorted && i<Rows;i++)
    { cur=MData[XIndex+i*Cols];
      if(cur==rdsbcppHugeDouble)
        return FALSE;
      if(cur<last)
        sorted=FALSE;
      else
        last=cur;
    }
  if(sorted) // ��� � �������
    return TRUE;
  // ����� ����������� �� ������� XIndex
  for(int i=0;i<Rows-1;i++)
    { // ����� ������������ �������� �� i �� Rows
      int minindex=i;
      last=MData[XIndex+i*Cols];
      for(int j=i+1;j<Rows;j++)
        { cur=MData[XIndex+j*Cols];
          if(cur<last)
            { last=cur;
              minindex=j;
            }
        }
      if(i!=minindex)
        { // �������� ������� ������ i � minindex � ������ �������
          double *cell_mi=MData+minindex*Cols,
                 *cell_i=MData+i*Cols;
          for(int j=0;j<Cols;j++)
            { cur=cell_i[j];
              cell_i[j]=cell_mi[j];
              cell_mi[j]=cur;
            }
        }
    } // for(int i=0;...)


  return TRUE;
  #undef XIndex
}
//---------------------------------------------------------------------------

// ����������� ������� �� ������� �� ��������
BOOL L57_SortMatrixColDesc(double *MData,int Rows,int Cols,int SortCol)
{ BOOL sorted=TRUE;
  double last,cur;

  #define XIndex SortCol

  if(Rows<2)
    return TRUE;
  if(SortCol>=Cols)
    return FALSE;

  // ������� - ��������, �� ������������� �� ���
  last=MData[XIndex];
  if(last==rdsbcppHugeDouble)
    return FALSE;
  for(int i=1;sorted && i<Rows;i++)
    { cur=MData[XIndex+i*Cols];
      if(cur==rdsbcppHugeDouble)
        return FALSE;
      if(cur>last)
        sorted=FALSE;
      else
        last=cur;
    }
  if(sorted) // ��� � �������
    return TRUE;
  // ����� ����������� �� ������� XIndex
  for(int i=0;i<Rows-1;i++)
    { // ����� ������������ �������� �� i �� Rows
      int minindex=i;
      last=MData[XIndex+i*Cols];
      for(int j=i+1;j<Rows;j++)
        { cur=MData[XIndex+j*Cols];
          if(cur>last)
            { last=cur;
              minindex=j;
            }
        }
      if(i!=minindex)
        { // �������� ������� ������ i � minindex � ������ �������
          double *cell_mi=MData+minindex*Cols,
                 *cell_i=MData+i*Cols;
          for(int j=0;j<Cols;j++)
            { cur=cell_i[j];
              cell_i[j]=cell_mi[j];
              cell_mi[j]=cur;
            }
        }
    } // for(int i=0;...)


  return TRUE;
  #undef XIndex
}
//---------------------------------------------------------------------------

// ����������� ������� ��� ��������� �� ������ (� ��������� �������� �������)
BOOL L57_SortMatrixFieldByRow(double *MData,int Rows,int Cols,int SortRow)
{ // excludeindex=0
  // ���������� �� ������ SortRow
  #define XIndex SortRow
  int start;
  BOOL sorted=TRUE;
  double *rowBaseX=MData+XIndex*Cols,*rowBaseY;
  double last,cur;

  // ������� - ��������, �� ������������� �� ���

  if(Cols<3)
    return TRUE; // ���������� ���������� ��� �� �����
  if(SortRow>=Rows)
    return FALSE;
  last=rowBaseX[1];
  start=2;

  if(last==rdsbcppHugeDouble)
    return FALSE;
  for(int i=start;sorted && i<Cols;i++)
    { cur=rowBaseX[i];
      if(cur==rdsbcppHugeDouble)
        return FALSE;
      if(cur<last)
        sorted=FALSE;
      else
        last=cur;
    }
  if(sorted) // ��� � �������
    return TRUE;
  // ����� ����������� �� ������ XIndex
  //rowBaseY=MData+YIndex*Cols;
  for(int i=1;i<Cols-1;i++)
    { // ����� ������������ �������� �� i �� Cols
      int minindex=i;
      last=rowBaseX[i];
      for(int j=i+1;j<Cols;j++)
        { cur=rowBaseX[j];
          if(cur<last)
            { last=cur;
              minindex=j;
            }
        }
      if(i!=minindex)
        { // �������� ������� ������ i � minindex � ������ ������
          rowBaseY=MData;
          for(int j=0;j<Rows;j++,rowBaseY+=Cols)
            { cur=rowBaseY[minindex];
              rowBaseY[minindex]=rowBaseY[i];
              rowBaseY[i]=cur;
            }
        }
    } // for(int i=0;...)

  return TRUE;
  #undef XIndex
}
//---------------------------------------------------------------------------

// ����������� ������� ��� ��������� �� ������� (� ��������� �������� �������)
BOOL L57_SortMatrixFieldByCol(double *MData,int Rows,int Cols,int SortCol)
{ // excludeindex=0
  // ���������� �� ������ SortRow
  #define XIndex SortCol
  int start;
  BOOL sorted=TRUE;
  double last,cur;

  // ���������� �� ������� XIndex

  // ������� - ��������, �� ������������� �� ���
  if(Rows<3) return TRUE; // ���������� ���������� ��� �� �����
  if(SortCol>=Cols)
    return FALSE;
  last=MData[XIndex+Cols];
  start=2;

  if(last==rdsbcppHugeDouble)
    return FALSE;
  for(int i=start;sorted && i<Rows;i++)
    { cur=MData[XIndex+i*Cols];
      if(cur==rdsbcppHugeDouble)
        return FALSE;
      if(cur<last)
        sorted=FALSE;
      else
        last=cur;
    }
  if(sorted) // ��� � �������
    return TRUE;
  // ����� ����������� �� ������� XIndex
  for(int i=1;i<Rows-1;i++)
    { // ����� ������������ �������� �� i �� Rows
      int minindex=i;
      last=MData[XIndex+i*Cols];
      for(int j=i+1;j<Rows;j++)
        { cur=MData[XIndex+j*Cols];
          if(cur<last)
            { last=cur;
              minindex=j;
            }
        }
      if(i!=minindex)
        { // �������� ������� ������ i � minindex � ������ �������
          double *cell_mi=MData+minindex*Cols,
                 *cell_i=MData+i*Cols;
          for(int j=0;j<Cols;j++)
            { cur=cell_i[j];
              cell_i[j]=cell_mi[j];
              cell_mi[j]=cur;
            }
        }
    } // for(int i=0;...)

  return TRUE;
  #undef XIndex
}
//---------------------------------------------------------------------------


// �������� ������ ����� ��� ���������� ���������� � undo
void L57_SaveModelToUndo(RDS_BHANDLE block,BOOL force)
{ L57_String text;

  if(!force)
    { if(rdsGetSystemInt(RDS_GSISAVELOADACTION)!=RDS_LS_SAVEUNDO)
        return;
    }

  if(!L57_LoadModelFile(block,text))
    return;
  // ������ ���������
  rdsWriteBlockDataText(NULL,TRUE);
  rdsWriteWordStringText("l57_model",text.c_str());
}
//---------------------------------------------------------------------------

// ��������� ������ ����� ��� �������������� ���������� ��� undo
void L57_LoadModelFromUndo(RDS_BHANDLE block,char *text)
{ L57_String thistext,undotext;
  char *word,*ptr,c;

  if(rdsGetSystemInt(RDS_GSISAVELOADACTION)!=RDS_LS_LOADUNDO)
    return;

  // ��������� ��������� ptr �� ������ ����������� ������
  ptr=text;
  // ���� �� ������ � ������
  for(;;)
    { // �������� �� ������ ��������� �����
      word=rdsGetTextWord(ptr,&ptr,&c,TRUE);
      if(c==0) // ����� ���������� � ����� �� �����
        break;
      if(c=='\n') // ������� ������ � ���������� � ����������
        continue;
      if(strcmp(word,"l57_model")==0) // ������
        { // ��������� ����� � ������ ����� ������
          undotext.Assign(rdsGetTextWord(ptr,&ptr,NULL,FALSE));
        }
      else // ������������ �������� �����
        return; // ������ � ���������� ���������
    } // ����� ����� for(;;)

  if(undotext.IsEmpty())
    return;

  if(!L57_LoadModelFile(block,thistext))
    thistext.Clear();

  if(thistext.IsEqual(undotext))
    return;

  L57_SaveModelFile(block,undotext,FALSE);
  if(rdsGetSystemInt(RDS_GSIUNDOACTION)!=RDS_UA_TRANSLATE) // ��� ������ �������� �������� ���������� ����� �����  
    L57_CompileModel(block);
}
//---------------------------------------------------------------------------

// ������� ��������� ������ ��� L57_CheckInputSource
BOOL RDSCALL L57_CheckInputSourceCB(RDS_PPOINTDESCRIPTION /*thispoint*/,RDS_PPOINTDESCRIPTION thatpoint,LPVOID /*data*/)
{
  if(!thatpoint->Source) // �� �����
    return TRUE;
  // ����� ������-�� �����
  return FALSE;
}
//---------------------------------------------------------------------------

// ��������� ������� ��������� ������ � ����� �����
// block           - ����
// inputnum        - ����� �����
// �������         - ���� ���� �� ���� ��������
BOOL L57_CheckInputSource(RDS_BHANDLE block,int inputnum,RDS_BHANDLE *pConnBlock)
{ RDS_BHANDLE b=rdsEnumConnectedBlocksByVar(block,inputnum,RDS_BEN_TRACELINKS,L57_CheckInputSourceCB,NULL);
  if(pConnBlock) *pConnBlock=b;
  return b!=0;
}
//---------------------------------------------------------------------------

// ��������������� ��������� ��� L57_ListInputSources
class TL57_LIS_Data
{ public:
    int NameId,SuffixId;
    int count;

    LPVOID Object;
    L57_VARLISTCB Func;

    TL57_LIS_Data(void){Object=NULL;Func=NULL;count=0;};
};
// ������� ��������� ������ ��� L57_ListInputSources
BOOL RDSCALL L57_ListInputSourcesCB(RDS_PPOINTDESCRIPTION /*thispoint*/,RDS_PPOINTDESCRIPTION thatpoint,LPVOID data)
{ TL57_LIS_Data *f=(TL57_LIS_Data*)data;
  if(!f)
    return FALSE;
  if(f->Func==NULL)
    return FALSE;
  if(!thatpoint->Source) // �� �����
    return TRUE;
  // ����� ������-�� �����
  (f->count)++;
  (f->Func)(f->Object,f->NameId,f->SuffixId,TRUE,thatpoint->Block);
  return TRUE;
}
// ������� ������ ���������� ������ � ����� �����
// block           - ����
// inputnum        - ����� �����
// Func(Object)    - �������� ����� ��� ������� ���������
// �������         - ���� ���� �� ���� ��������
BOOL L57_ListInputSources(RDS_BHANDLE block,L57_FuncTransferParameter *p,LPVOID Object,L57_VARLISTCB Func)
{ TL57_LIS_Data data;
  if(!p)
    return FALSE;
  data.NameId=p->NameId;
  data.SuffixId=p->SuffixId;
  data.Object=Object;
  data.Func=Func;
  rdsEnumConnectedBlocksByVar(block,p->BlockVarNum,RDS_BEN_TRACELINKS,L57_ListInputSourcesCB,&data);
  return data.count!=0;
}
//---------------------------------------------------------------------------

// ��������������� ��������� ��� L57_AppendFunctionDestList
class L57_AFDL_Data
{ public:
    RDS_HOBJECT List;
    L57_IntArray *Inputs;
    L57_AFDL_Data(void)
      { List=NULL; Inputs=NULL;};
};
//---------------------------------------------------------------------------

// ������� ��������� ������ ��� L57_AppendFunctionDestList
BOOL RDSCALL L57_AppendFunctionDestListCB(RDS_PPOINTDESCRIPTION thispoint,RDS_PPOINTDESCRIPTION thatpoint,LPVOID data)
{ L57_AFDL_Data *pData=(L57_AFDL_Data*)data;
  int index;

  if(!data)
    return TRUE;
  if(thatpoint->Source) // ����� (����� ����)
    return TRUE;
  if(pData->List==NULL || (!thispoint->Source))
    return TRUE;
  // ���� ������-�� �����
  index=rdsBCLAddBlock(pData->List,thatpoint->Block,FALSE);
  if(pData->Inputs)
    pData->Inputs->SetElement(index,thatpoint->VarNum);
  return TRUE;
}
//---------------------------------------------------------------------------

// �������� ����������� ������� (�������) ����� � ������ ������
void L57_AppendFunctionDestList(RDS_BHANDLE block,int outputnum,RDS_HOBJECT list,L57_IntArray *inputs)
{ L57_AFDL_Data data;
  if(list==NULL || block==NULL || outputnum<0)
    return;
  data.List=list;
  data.Inputs=inputs;
  rdsEnumConnectedBlocksByVar(block,outputnum,RDS_BEN_TRACELINKS,L57_AppendFunctionDestListCB,&data);
}
//---------------------------------------------------------------------------

// ��������������� ��������� ��� L57_CheckFunctionSource
class L57_CFS_Data
{ public:
    L57_IntArray *pArray; // ���������� ����������� ������ ����������� ��������� ����� ������� �����
    BOOL WholeConnected;  // ������������ ��� ������� ����������, � �� �� ��������� �������� (������ ��� pArray!=NULL)
    char *InName;         // ��� ���������� �����
    size_t InNameLen;     // ����� ����� ����������
    int CheckFunc;        // ������������� ������� �������� (��� ����������)
    BOOL Found;           // ������������ ������� ������� ���� �� ������ ���������

    L57_CFS_Data(void)
      { pArray=NULL;WholeConnected=FALSE;InNameLen=0;Found=FALSE;};
};
//---------------------------------------------------------------------------

// ������� ��������� ������ ��� L57_CheckFunctionSource
BOOL RDSCALL L57_CheckFunctionSourceCB(RDS_PPOINTDESCRIPTION thispoint,RDS_PPOINTDESCRIPTION thatpoint,LPVOID data)
{ L57_CFS_Data *pData=(L57_CFS_Data*)data;
  char *s;
  int n;
  if(!data)
    return FALSE;

  if(!thatpoint->Source) // �� �����
    return TRUE;

  if(!rdsCallBlockFunction(thatpoint->Block,pData->CheckFunc,NULL)) // �� �������� �������
    return TRUE;

  // ����� ����
  pData->Found=TRUE;
  if(pData->pArray==NULL) // ����� �������� �� �����
    return FALSE;

  // ����� ����� ��������
  if(thispoint->VarName==NULL)
    return TRUE;
  if(strlen(thispoint->VarName)==pData->InNameLen) // ������������ ��� ����������
    { pData->WholeConnected=TRUE;
      return FALSE;
    }
  // ������������ ����� ����������
  // 0123456789
  // Func_In[3]
  s=thispoint->VarName+pData->InNameLen;
  while((*s)=='[') s++;
  n=atoi(s);
  if(n>=0)
    pData->pArray->Add(n);
  return TRUE;
}
//---------------------------------------------------------------------------

// ��������� ������� ���������� ������� � ����� �����
// (������������ ������ ��� ������ � ������������ ��������)
// block         - ����
// inputnum      - ����� ����� �������
// �������       - ���� ��������
BOOL L57_CheckFunctionSource(RDS_BHANDLE block,int inputnum)
{ L57_CFS_Data Data;
  RDS_VARDESCRIPTION vdescr;
  static int checkfunc=0;

  if(checkfunc==0)
    checkfunc=rdsRegisterFunction(L57_ISUSERFUNCSOURCENAME);

  vdescr.servSize=sizeof(vdescr);

  if(!rdsGetBlockVar(block,inputnum,&vdescr))
    return FALSE;

  Data.InName=vdescr.Name;
  Data.InNameLen=strlen(vdescr.Name);
  Data.CheckFunc=checkfunc;
  rdsEnumConnectedBlocksByVar(block,inputnum,RDS_BEN_TRACELINKS,L57_CheckFunctionSourceCB,&Data);
  return Data.Found;
}
//---------------------------------------------------------------------------

// ��������� ������� ���������� ������� � ����� �����
// (������������ ��� ������ � ������-�������� �������)
// block           - ����
// inputnum        - ����� ����� �������
// pItems          - ����������� ������ ������� �������������� ��������
// pArrayConnected - ������������ ������� ����, ��� ����������� ���� ������ �������
// �������         - ���� ���� �� ���� ��������
BOOL L57_CheckFunctionSourceMulti(RDS_BHANDLE block,int inputnum,L57_IntArray *pItems,BOOL *pArrayConnected)
{ L57_CFS_Data Data;
  RDS_VARDESCRIPTION vdescr;
  static int checkfunc=0;

  if(pArrayConnected)
    *pArrayConnected=FALSE;
  if(pItems)
    pItems->Clear();

  if(checkfunc==0)
    checkfunc=rdsRegisterFunction(L57_ISUSERFUNCSOURCENAME);

  vdescr.servSize=sizeof(vdescr);


  if(!rdsGetBlockVar(block,inputnum,&vdescr))
    return FALSE;

  Data.InName=vdescr.Name;
  Data.InNameLen=strlen(vdescr.Name);
  Data.CheckFunc=checkfunc;
  Data.pArray=pItems;
  rdsEnumConnectedBlocksByVar(block,inputnum,RDS_BEN_TRACELINKS,L57_CheckFunctionSourceCB,&Data);
  if(pArrayConnected)
    *pArrayConnected=Data.WholeConnected;
  return Data.Found;
}
//---------------------------------------------------------------------------


// ������� ���� ������� �� �������� �����, ���� ��� ������� ����� ��� �������
// (������������ ������ ��� ������ � ������������ ��������)
// block      - ����
// inputnum   - ����� ����������-�����
// outputnum  - ����� ����������-������
// immediate  - ��-�� �������������� ������� ������������� (TRUE) ��� ��� ������������� (FALSE)
// exporttext - ����� �������� �������
void L57_SendSingleFunctionIfNoInput(RDS_BHANDLE block,int inputnum,int outputnum,BOOL immediate,char *exporttext)
{
  if(!L57_CheckFunctionSource(block,inputnum))
    { // ��� ��������� �������
      char *texts[1];
      texts[0]=exporttext;
      L57_SendFunctionByConn(block,outputnum,immediate,texts,1,NULL); // ���� �������
    }
}
//---------------------------------------------------------------------------

// ������� ���� ������� �� �������� �����
// (������������ ������ ��� ������ � ������������ ��������)
// block      - ����
// outputnum  - ����� ����������-������
// immediate  - ��-�� �������������� ������� ������������� (TRUE) ��� ��� ������������� (FALSE)
// exporttext - ����� �������� �������
void L57_SendSingleFunction(RDS_BHANDLE block,int outputnum,BOOL immediate,char *exporttext,L57_AcceptFunc_Param_Queue *queue)
{ char *texts[1];
  texts[0]=exporttext;
  L57_SendFunctionByConn(block,outputnum,immediate,texts,1,queue);
}
//---------------------------------------------------------------------------

// ��������������� ��������� ��� L57_SendFunctionByConn
class L57_SFBC_Data
{ public:
    char **Texts;       // ������ �������
    int TextsCount;     // ����� �������
    char *OutName;      // ��� ���������� ������
    int SendFunc;       // ������������� ������� ��������
    BOOL Immediate;     // TRUE - ����� ��-�� �������������� ������� � ����� �� ������
                        // FALSE - ����� � �������� ������������� ��� ������ �� ������ ��������������

    L57_AcceptFunc_Param_Queue *Queue; // ��������� ���������� ������ � �������

    L57_SFBC_Data(void)
      { Texts=NULL;TextsCount=/*OutNameLen=*/0;Queue=NULL;};
};
//---------------------------------------------------------------------------

// ������� ��������� ������ ��� L57_SendFunctionByConn
BOOL RDSCALL L57_SendFunctionByConnCB(RDS_PPOINTDESCRIPTION thispoint,RDS_PPOINTDESCRIPTION thatpoint,LPVOID data)
{ L57_SFBC_Data *pData=(L57_SFBC_Data*)data;
  int source_n=-1,dest_n=-1;
  L57_AcceptFunc_Param param;

  if(!data)
    return FALSE;

  if(thispoint->VarName==NULL || thatpoint->VarName==NULL)
    return TRUE;

  if(thatpoint->Source) // �����
    return TRUE;

  // ����� ����

  // ������� ����������-��������
  if(thispoint->VarType!=RDS_VARTYPE_ARRAY) // �� ������ (����������� ������� ������� ��� ��������� ����������)
    { char *s=strchr(thispoint->VarName,'[');
      if(s) // ���� ������
        source_n=atoi(s+1);
      else // ������������ ��� ����������, � ��� �� ������ - ����� �� ����� ������� �������
        source_n=0;
    }
  // ���� ����������� ������, source_n ��������� ������ -1

  // ������� ����������-����������
  if(thatpoint->VarType!=RDS_VARTYPE_ARRAY) // �� ������ (����������� ������� ������� ��� ��������� ����������)
    { char *s=strchr(thatpoint->VarName,'[');
      if(s) // ���� ������
        dest_n=atoi(s+1);
      else // ��� �������, ������, ������������ ��� ����������, � ��� �� ������ - ����� �� ����� ������� �������
        dest_n=0;
    }
  // ���� ����������� ������, dest_n ��������� ������ -1

  // ������ � source_n � dest_n ���� ������ �������, ���� -1, ���� ���������� ��� ������� �������
  param.servSize=sizeof(param);
  param.Texts=pData->Texts;
  param.TextsCount=pData->TextsCount;
  param.TextIndex=source_n;
  param.AcceptIndex=dest_n;
  param.Immediate=pData->Immediate;
  param.DestVarNum=thatpoint->VarNum;
  if(pData->Queue!=NULL && pData->Queue->CallFunc!=NULL)
    { // ������ ����� � �������
      (pData->Queue->CallFunc)(pData->Queue->CallObject,thatpoint->Block,&param);
      return TRUE;
    }
  param.Queue=NULL;
  rdsCallBlockFunction(thatpoint->Block,pData->SendFunc,&param);

  return TRUE;
}
//---------------------------------------------------------------------------

// ������� ������� �� �������� ������
// block       - ����
// outputnum   - ����� ����������-������
// immediate   - ��-�� �������������� ������� ������������� (TRUE) ��� ��� ������������� (FALSE)
// exporttexts - ������ ������� �������� ������� (�� �������)
// funccount   - ������ �������
// queue       - ������� ��� ���������� ������(���� ����)
void L57_SendFunctionByConn(RDS_BHANDLE block,int outputnum,BOOL immediate,char **exporttexts,int funccount,
                            L57_AcceptFunc_Param_Queue *queue)
{ RDS_VARDESCRIPTION vdescr;
  static int sendfunc=0;
  L57_SFBC_Data Data;

  if(exporttexts==NULL || funccount<=0)
    return;

  if(sendfunc==0)
    sendfunc=rdsRegisterFunction(L57_ACCEPTUSERFUNCNAME);

  vdescr.servSize=sizeof(vdescr);
  if(!rdsGetBlockVar(block,outputnum,&vdescr))
    return;

  Data.Texts=exporttexts;
  Data.TextsCount=funccount;
  Data.OutName=vdescr.Name;
  //Data.OutNameLen=strlen(vdescr.Name);
  Data.SendFunc=sendfunc;
  Data.Immediate=immediate;
  if(!queue) // ���������� ������� ����
    { L57_FuncTransfer Transfer;
      L57_AcceptFunc_Param_Queue Queue;
      Queue.CallFunc=L57_FuncTransfer_QueueAcceptFuncCall;
      Queue.CallObject=&Transfer;
      Queue.TransferId=Transfer.GenerateParamTransferId();
      Data.Queue=&Queue;
      rdsEnumConnectedBlocksByVar(block,outputnum,RDS_BEN_TRACELINKS,L57_SendFunctionByConnCB,&Data);
      Transfer.DoDelayedFuncTransferCalls(&Queue);
    }
  else
    { Data.Queue=queue;
      rdsEnumConnectedBlocksByVar(block,outputnum,RDS_BEN_TRACELINKS,L57_SendFunctionByConnCB,&Data);
    }
}
//---------------------------------------------------------------------------

// �������� ������� ���������� ������� �� ���������� ������� �������� �������
L57_AcceptFunc_Param_Queue *L57_GetQueueFromParam(void *vparam)
{ L57_AcceptFunc_Param *param=(L57_AcceptFunc_Param*)vparam;
  if(!param)
    return NULL;
  if(!L57_CANACCESSFIELD(param,Queue)) // �������� ������ ���������
    return NULL;
  return param->Queue;
}
//---------------------------------------------------------------------------

// ������� ������� �� �������� ������
// block       - ����
// outputnum   - ����� ����������-������
// immediate   - ��-�� �������������� ������� ������������� (TRUE) ��� ��� ������������� (FALSE)
// exporttexts - ������ ������� �������� ������� (�� �������)
// funccount   - ������ �������
// exportalt   - ������ �������������� ������� (���� ���� �������, �� ��������� ������ exporttexts[]
void L57_SendOutputFunctions(RDS_BHANDLE block,int outputnum,BOOL immediate,char **exporttexts,int funccount,
                             L57_StringArray *exportalt,L57_AcceptFunc_Param_Queue *queue)
{ RDS_VARDESCRIPTION vdescr;
  static int sendfunc=0;
  L57_SFBC_Data Data;
  int cnt=funccount;
  char **array;
  if(!exportalt) // ��� ��������������� �������
    { L57_SendFunctionByConn(block,outputnum,immediate,exporttexts,funccount,queue);
      return;
    }
  // ���� �������������� ������

  if(sendfunc==0)
    sendfunc=rdsRegisterFunction(L57_ACCEPTUSERFUNCNAME);

  vdescr.servSize=sizeof(vdescr);
  if(!rdsGetBlockVar(block,outputnum,&vdescr))
    return;

  if(cnt<exportalt->Count())
    cnt=exportalt->Count();
  array=new char*[cnt];


  for(int i=0;i<funccount;i++)
    array[i]=exporttexts[i];
  for(int i=funccount;i<cnt;i++)
    array[i]=NULL;
  for(int i=0;i<exportalt->Count();i++)
    { L57_String *s=exportalt->ItemPtrFAST(i);
      if(s->IsEmpty())
        continue;
      array[i]=s->c_str();
    }

  Data.Texts=array;
  Data.TextsCount=cnt;
  Data.OutName=vdescr.Name;
  //Data.OutNameLen=strlen(vdescr.Name);
  Data.SendFunc=sendfunc;
  Data.Immediate=immediate;
  if(!queue) // ���������� ������� ����
    { L57_FuncTransfer Transfer;
      L57_AcceptFunc_Param_Queue Queue;
      Queue.CallFunc=L57_FuncTransfer_QueueAcceptFuncCall;
      Queue.CallObject=&Transfer;
      Queue.TransferId=Transfer.GenerateParamTransferId();
      Data.Queue=&Queue;
      rdsEnumConnectedBlocksByVar(block,outputnum,RDS_BEN_TRACELINKS,L57_SendFunctionByConnCB,&Data);
      Transfer.DoDelayedFuncTransferCalls(&Queue);
    }
  else
    { Data.Queue=queue;
      rdsEnumConnectedBlocksByVar(block,outputnum,RDS_BEN_TRACELINKS,L57_SendFunctionByConnCB,&Data);
    }

  delete[] array;
}
//---------------------------------------------------------------------------

// �������� ��� ������ �������� �������, ��������� ������� ����������
BOOL L57_CompareExportTexts(char *text1,char *text2)
{ int len1,len2;
  char *end;
  if(text1==NULL || text2==NULL)
    return text1==text2;
  // ��� �� NULL
  len1=strlen(text1);
  len2=strlen(text2);

  end=strstr(text1,"\n" L57_ORDERLABEL);
  if(end)
    len1-=strlen(end);

  end=strstr(text2,"\n" L57_ORDERLABEL);
  if(end)
    len2-=strlen(end);

  if(len1!=len2)
    return FALSE;
  return strncmp(text1,text2,len1)==0;
}
//---------------------------------------------------------------------------

// ������� ���� (�������) ������� �� ������� �����
// (������������ ������ ��� ������ � ������������ ��������)
// block         - ����
// param         - �������� �������
// curexporttext - ������� ����� ������� (���� ���������, ������ �� �����)
// pNewExport    - ����������, � ������� ������������ ����� �������� �����
// �������       - ���������� (�� ���� ������)
BOOL L57_GetSingleFunctionByConn(RDS_BHANDLE block,char *templatefile,L57_AcceptFunc_Param *Param,
                                 char *curexporttext,L57_String *pNewExport)
{ BOOL ok;
  /*
  if(Param->servSize<sizeof(L57_AcceptFunc_Param))
    return FALSE;
  */
  if(!Param)
    return FALSE;
  if(!L57_CANACCESSFIELD(Param,AcceptIndex)) // �������� ������ ���������
    return FALSE;
  if(Param->Texts==NULL || Param->TextsCount<=0 || Param->Texts[0]==NULL)
    return FALSE;
  if(Param->AcceptIndex!=-1 && Param->AcceptIndex!=0)
    return FALSE;

  // ���������� � ���������
  if(L57_CompareExportTexts(Param->Texts[0],curexporttext))
    return TRUE;
  /*
  if(curexporttext!=NULL && strcmp(Param->Texts[0],curexporttext)==0)
    return TRUE;
  */

  // ������ �� �������
        /*
        char *s=rdsProcessText(curexporttext,RDS_PT_TEXTTOSTRING,NULL);
        rdsBlockMessageBox(block,s,"����",MB_OK);
        rdsFree(s);
        s=rdsProcessText(Param->Texts[0],RDS_PT_TEXTTOSTRING,NULL);
        rdsBlockMessageBox(block,s,"�������",MB_OK);
        rdsFree(s);
        */
  ok=L57_ExecTool_ImportFunction(block,templatefile,0,Param->Texts[0]);
  if(ok)
    { if(pNewExport) pNewExport->Assign(Param->Texts[0]);
      rdsSetSysModifiedFlag(rdsGetParentBlock(block,NULL),TRUE);
      rdsSetSystemInt(RDS_SSILEAVEEDITSTATUS,RDS_BFR_RETRY);
    }
  else
    rdsBlockMessageBox(NULL,"������ ������ ������� � ����","������",MB_OK|MB_ICONERROR);
  return ok;
}
//---------------------------------------------------------------------------

// ������� � ������ ������� � �������� �������
// block         - ����
// funcnum       - ����� �������
// curexporttext - ������� ����� ������� (���� ���������, ������ �� �����)
// newexporttext - ����� (���������) ����� �������
// �������       - ���������� (�� ���� ������)
BOOL L57_GetCertainFunctionByConn(RDS_BHANDLE block,char *templatefile,int funcnum,
                                  char *curexporttext,char *newexporttext)
{ BOOL ok,cur_empty,new_empty;
  if(funcnum<0)
    return FALSE;

  // ���������� � ���������
  cur_empty=curexporttext==NULL || (curexporttext)==0;
  new_empty=newexporttext==NULL || (newexporttext)==0;
  if(cur_empty && new_empty)
    return TRUE;

  if((!cur_empty) && (!new_empty) && L57_CompareExportTexts(newexporttext,curexporttext))
    return TRUE;

  // ������ �� �������
  ok=L57_ExecTool_ImportFunction(block,templatefile,funcnum,newexporttext);
  if(ok)
    { rdsSetSysModifiedFlag(rdsGetParentBlock(block,NULL),TRUE);
      //rdsSetSystemInt(RDS_SSILEAVEEDITSTATUS,RDS_BFR_RETRY);
    }
  else
    rdsBlockMessageBox(NULL,"������ ������ ������� � ����","������",MB_OK|MB_ICONERROR);
  return ok;
}
//---------------------------------------------------------------------------

// ������� l57.dll ��� ������� ������� � ������
BOOL L57_ExecTool_ImportFunction(RDS_BHANDLE block,char *templatefile,int funcnum,char *importtext)
{ L57_String filename;
  RDS_TOOLEXTCALLDATA data;
  LPSTR array[3];

  if(!L57_GetDynModelFileName(block,filename))
    return FALSE;

  data.servSize=sizeof(data);
  data.Command=3;
  data.IntParam=funcnum;
  data.NInStrings=3;
  data.Result=0;
  data.InStrings=array;
  array[0]=templatefile;
  array[1]=filename.c_str();
  array[2]=importtext;
  data.NOutStrings=0;
  data.OutStrings=NULL;
  rdsExecToolExt(RDS_ETF_S_BYDLL,"ProcessVarAndFunc","$DLL$\\L57.dll",&data);

  if(data.Result)
    { L57_CompileModel(block);
      rdsSetSysModifiedFlag(rdsGetParentBlock(block,NULL),TRUE);
      rdsSetSystemInt(RDS_SSILEAVEEDITSTATUS,RDS_BFR_RETRY);
    }

  rdsClearExecToolExtResults(&data);

  return data.Result!=0;
}
//---------------------------------------------------------------------------

// ������� L57.dll ��� ������� ���� ������� � ����������
BOOL L57_ExecTool_ClearAll(RDS_BHANDLE block,char *templatefile)
{ L57_String filename;
  RDS_TOOLEXTCALLDATA data;
  char *array[2];

  if(!L57_GetDynModelFileName(block,filename))
    return FALSE;

  array[0]=templatefile;
  array[1]=filename.c_str();

  data.servSize=sizeof(data);
  data.Command=9;
  data.IntParam=0; // �� ������������
  data.NInStrings=2;
  data.Result=0;
  data.InStrings=array;
  data.NOutStrings=0;
  data.OutStrings=NULL;
  rdsExecToolExt(RDS_ETF_S_BYDLL,"ProcessVarAndFunc","$DLL$\\L57.dll",&data);

  if(data.Result)
    { L57_CompileModel(block);
      rdsSetSysModifiedFlag(rdsGetParentBlock(block,NULL),TRUE);
      rdsSetSystemInt(RDS_SSILEAVEEDITSTATUS,RDS_BFR_RETRY);
    }

  rdsClearExecToolExtResults(&data);

  return data.Result!=0;
}
//---------------------------------------------------------------------------


// ������� l57.dll ��� ������� ������ ������� � ������
// exporttexts - ������ ������� �������� ������� (�� �������)
// funccount   - ������ �������
// exportalt   - ������ �������������� ������� (���� ���� �������, �� ��������� ������ exporttexts[]
BOOL L57_ExecTool_ImportFunctions(RDS_BHANDLE block,char *templatefile,char **exporttexts,int funccount,L57_StringArray *exportalt)
{ L57_String filename;
  RDS_TOOLEXTCALLDATA data;
  int cnt=funccount;
  char **array;

  if(!L57_GetDynModelFileName(block,filename))
    return FALSE;

  if(exportalt)
    { if(cnt<exportalt->Count())
        cnt=exportalt->Count();
    }

  array=new char*[cnt+2];
  array[0]=templatefile;
  array[1]=filename.c_str();

  for(int i=0;i<funccount;i++)
    array[i+2]=exporttexts[i];
  for(int i=funccount;i<cnt;i++)
    array[i+2]=NULL;
  if(exportalt)
    for(int i=0;i<exportalt->Count();i++)
      { L57_String *s=exportalt->ItemPtrFAST(i);
        if(s->IsEmpty())
          continue;
        array[i+2]=s->c_str();
      }

  data.servSize=sizeof(data);
  data.Command=6;
  data.IntParam=0; // �� ������������
  data.NInStrings=cnt+2;
  data.Result=0;
  data.InStrings=array;
  data.NOutStrings=0;
  data.OutStrings=NULL;
  rdsExecToolExt(RDS_ETF_S_BYDLL,"ProcessVarAndFunc","$DLL$\\L57.dll",&data);

  if(data.Result)
    { L57_CompileModel(block);
      rdsSetSysModifiedFlag(rdsGetParentBlock(block,NULL),TRUE);
      rdsSetSystemInt(RDS_SSILEAVEEDITSTATUS,RDS_BFR_RETRY);
    }

  rdsClearExecToolExtResults(&data);

  return data.Result!=0;
}
//---------------------------------------------------------------------------

// ������� ����� �������� ������� �� ��� �������: ���������� � ������
// � ���������� ���������� �������� �� ���������
void L57_SplitExportText(char *text,L57_StringArray &vars,L57_StringArray &other)
{ L57_StringArray array;
  int v_count,o_count,a_count;

  array.Assign(text);

  // ������� ����� ����������
  v_count=a_count=0;
  for(int i=0;i<array.Count();i++)
    { char *s=array.ItemPtrFAST(i)->c_str();
      if(strcmp(s,L57_ORDERLABEL)==0) // ������ ������� ���������� (������������)
        break;
      if((*s)=='V' || (*s)=='S')
        v_count++;
      a_count++;
    }
  // a_count - ����� ����� ��� ������ ������� ����������

  // ��������
  vars.SetSize(v_count);
  other.SetSize(a_count-v_count);
  v_count=o_count=0;
  for(int i=0;i<a_count;i++)
    { L57_String *str=array.ItemPtrFAST(i);
      char *s=str->c_str();
      if(strcmp(s,L57_ORDERLABEL)==0) // ������ ������� ���������� (������������)
        break;
      if((*s)=='V' || (*s)=='S')
        { L57_String *newstr=vars.ItemPtrFAST(v_count);
          int n;
          newstr->Exchange(str);
          v_count++;
          // ������: "{V|S} ��� ��� [��������]" - �������� ����� ���������, ���� ����
          s=newstr->c_str()+1;
          // ����� ����� ������ ������ ��������
          n=0;
          for(;;)
            { // ���������� �����
              while( (*s)!=0 && (*s)!=' ' && (*s)!='\t' ) s++;
              n++;
              if(n==3) // �� ������ ������� ������� ����� ����
                { (*s)=0;
                  break;
                }
              // ���������� �������
              while( (*s)!=0 && ( (*s)==' ' || (*s)=='\t' ) ) s++;
            }

        }
      else
        { other.ItemPtrFAST(o_count)->Exchange(str);
          o_count++;
        }
    }
}
//---------------------------------------------------------------------------

// �������� ��������� ������� � ��������� � ������� ������������� ������ (� ���������
// ����� ���� ������ ����������, ��� ���������)
BOOL L57_MustImportFunction(char *newfunc,char *oldfunc)
{ L57_StringArray new_vars,new_other,old_vars,old_other;
  //rdsBlockMessageBox(NULL,"������","search",MB_OK);

  if(newfunc==NULL || newfunc[0]==0) // ��� ����� �������
    return FALSE;
  // ���� ����� �������
  if(oldfunc==NULL || oldfunc[0]==0) // ��� ������ �������
    { //rdsBlockMessageBox(NULL,"��� ������","search",MB_OK);
      return TRUE;
    }
        /*
        char *s=rdsProcessText(oldfunc,RDS_PT_TEXTTOSTRING,NULL);
        rdsBlockMessageBox(NULL,s,"����",MB_OK);
        rdsFree(s);
        s=rdsProcessText(newfunc,RDS_PT_TEXTTOSTRING,NULL);
        rdsBlockMessageBox(NULL,s,"�������",MB_OK);
        rdsFree(s);
        */

  // ���� ��� �������
  L57_SplitExportText(newfunc,new_vars,new_other);
  L57_SplitExportText(oldfunc,old_vars,old_other);

  if(!new_other.IsEqualTo(&old_other)) // ���������� ������
    { //rdsBlockMessageBox(NULL,"������ ������","search",MB_OK);
      return TRUE;
    }

  // ������ ������� ��������� - ���������� ����������
  for(int i=0;i<new_vars.Count();i++)
    { L57_String *str=new_vars.ItemPtrFAST(i);
      if(old_vars.IndexOf(str)<0) // ����� ���������� ��� � ������
        return TRUE;
    }
  // ��� ����� ���������� ���� � ������ ������
  //rdsBlockMessageBox(NULL,"���������","search",MB_OK);
  return FALSE;
}
//---------------------------------------------------------------------------

// �������� ��� ������ �������� ������� � ������� ������������� ������
BOOL L57_MustImportFunctions(L57_StringArray *newfuncs,L57_StringArray *oldfuncs)
{
  if(newfuncs->Count()>oldfuncs->Count())
    return TRUE;
  // ������� ���������� ���������� ��� ����� ������
  for(int i=0;i<newfuncs->Count();i++)
    if(L57_MustImportFunction(newfuncs->ItemPtrFAST(i)->c_str(),oldfuncs->ItemPtrFAST(i)->c_str()))
      return TRUE;
  return FALSE;
}
BOOL L57_MustImportFunctions(L57_StringArray *newfuncs,char **oldfuncs,int oldfuncscount)
{
  if(newfuncs->Count()>oldfuncscount)
    return TRUE;
  // ������� ���������� ���������� ��� ����� ������
  for(int i=0;i<newfuncs->Count();i++)
    if(L57_MustImportFunction(newfuncs->ItemPtrFAST(i)->c_str(),oldfuncs[i]))
      return TRUE;
  return FALSE;
}
//---------------------------------------------------------------------------

// ��������� ����������� ��������� � ���� ���������
// structbase - ��������� �� ������ ���������
// structsize - ������ ���������
// fieldbase  - ��������� �� ������ ����
// fieldsize  - ������ ����
BOOL L57_CanAccessField(void *structbase,DWORD structsize,void *fieldbase,DWORD fieldsize)
{ BYTE *b_struct=(BYTE*)structbase,
       *b_field=(BYTE*)fieldbase;
  DWORD delta=b_field-b_struct; // ������� ����� ������� ���� � ������� ���������
  BOOL ok=(delta+fieldsize)<=structsize;
  return ok;
}
//---------------------------------------------------------------------------

// �������� ��� ������� double
BOOL L57_CompareDoubleMatr(void *VoidPtr1,void *VoidPtr2,bool *pError)
{ RDS_ARRAYACCESSDATA Data1,Data2;

  if(pError) *pError=FALSE;

  if(!rdsGetVarArrayAccessData(VoidPtr1,&Data1))
    { if(pError) *pError=TRUE;
      return TRUE;
    }
  if(!rdsGetVarArrayAccessData(VoidPtr2,&Data2))
    { if(pError) *pError=TRUE;
      return TRUE;
    }
  if(Data1.ItemSize!=sizeof(double) || Data2.ItemSize!=sizeof(double) ||
     rdsGetVarArrayElementType(VoidPtr1)!=RDS_VARTYPE_DOUBLE ||
     rdsGetVarArrayElementType(VoidPtr2)!=RDS_VARTYPE_DOUBLE)
    { if(pError) *pError=TRUE;
      return TRUE;
    }
  // ��� ������� �������
  if(Data1.Rows!=Data2.Rows || Data1.Cols!=Data2.Cols)
    return FALSE;
  // ������� �������� ���������
  if(Data1.Data==NULL) // ��� ������ - ��������� ������
    return TRUE;
  // ��� �� ������
  return memcmp(Data1.Data,Data2.Data,Data1.Rows*Data1.Cols*sizeof(double))==0;
}
//---------------------------------------------------------------------------

// �������� � �������� ���� � ����
void L57_SelectAndShowBlock(RDS_BHANDLE block,BOOL waitmsg)
{
  if(block)
    { RDS_BHANDLE parent=rdsGetParentBlock(block,NULL);
      BOOL edit=rdsSystemInEditMode();
      if(parent)
        { if(edit)
            rdsSelectAll(parent,FALSE,FALSE);
          rdsOpenSystemWindow(parent);
          rdsScrollWindowToBlock(block);
          if(edit)
            rdsSelectBlock(block,TRUE,TRUE);
        }
    }

  if(waitmsg)
    rdsMessageBox("������� OK ��� ������� [Esc] ��� �������� � ���� ��������","����� �����",MB_OK|MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------

// ��������� �������� �������� ���������� � �������� ����
void L57_CheckTransferParameters(RDS_BHANDLE block,LPVOID Object,L57_VARLISTCB Func)
{ static int func=0;
  L57_CheckTranser_Param param;
  RDS_BHANDLE root=rdsGetRootSystem(NULL);

  param.servSize=sizeof(param);

  if(func==0) func=rdsRegisterFunction(L57_PREPARECHECKNAME);
  param.Mode=L57_CTM_PREPARE;
  param.DestinationBlock=block;
  param.Object=Object;
  param.InfFunc=Func;
  rdsBroadcastFunctionCallsEx(root,func,&param,RDS_BCALL_SUBSYSTEMS);

  param.Mode=L57_CTM_CHECK;
  rdsBroadcastFunctionCallsEx(root,func,&param,RDS_BCALL_SUBSYSTEMS);
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

// �������� ������� ��� ����� (������������� � �������) �������� �������� �������
// block     - ����
// TR_params - ��� �������� ���� (L57_BLOCKTEXT_*)
// TR_text   - "�������" �����, ��������� �������������
// func_text - ����� �������� ������� (��� ��������� �������� ����� "\n")
// check     - ��������� ������������� � ���������� ������
void L57_ApplyFunctionToTextRect(RDS_BHANDLE block,int TR_params,char *TR_text,char *func_text,BOOL check)
{ RDS_BLOCKTEXTRECTDATA rect;
  L57_String text,func;
  char *texttoset;
  if(!block)
    return;

  if(TR_params==L57_BLOCKTEXT_NOCONTROL)
    return;
  if(check)
    { if(!L57_ExecTool_CanEditTextRect())
        return;
    }

  rect.servSize=sizeof(rect);
  if(!rdsGetBlockTextRectParams(block,&rect,NULL))
    return;

  if(func_text==NULL || (*func_text)==0)
    TR_params=L57_BLOCKTEXT_FIXED;

  text.Assign(TR_text);
  func.Assign(func_text);

  switch(TR_params)
    { case L57_BLOCKTEXT_BEFORE:
        if(func.IsNotBlank() && text.IsNotBlank())
          func.AddString("\n");
        func.AddString(text);
        texttoset=func.c_str();
        break;
      case L57_BLOCKTEXT_AFTER:
        if(text.IsNotBlank() && func.IsNotBlank())
          text.AddString("\n");
        text.AddString(func);
        texttoset=text.c_str();
        break;
      default:
        texttoset=TR_text;
    }
  if(check)
    { if(rect.Text!=NULL && strcmp(rect.Text,texttoset)==0) // �������
        return;
    }
  rect.Text=texttoset;
  rdsSetBlockTextRectParams(block,&rect,NULL,RDS_SBTRPREFRESH);
}
//---------------------------------------------------------------------------

// ������� ���� ����� ��� �������������� �������� ���� ����� � �������� ��� �������������� � �������
void L57_TextRectEdit_Setup(RDS_BHANDLE block,RDS_HOBJECT win,int TR_params,char *TR_text,int tab)
{ int n;
  RDS_BLOCKTEXTRECTDATA rect;
  RDS_SERVFONTPARAMS font;
  int mlheight=3*24;
  char *str;

  int fieldbase=L57_BLOCKTEXT_FIELDBASE;

  rect.servSize=sizeof(rect);
  font.servSize=sizeof(font);
  rdsGetBlockTextRectParams(block,&rect,&font);

  // ������� �������������� ������� � ����������
  rdsFORMAddTab(win,tab,"������� ��� �����");

  n=fieldbase;
  rdsFORMAddEdit(win,tab,n,RDS_FORMCTRL_COMBOLIST|RDS_FORMFLAG_LINE,"������� ���:",300);
  rdsFORMSetComboList(win,n,"�������� � ����� ���������� �����\n������������� � �������");
  rdsFORMSetInt(win,n,TR_params<=L57_BLOCKTEXT_NOCONTROL?0:1);

  n=fieldbase+2;
  rdsFORMAddEdit(win,tab,n,RDS_FORMCTRL_MULTILINE,"�����:",80);
  if(TR_text)
    rdsFORMSetString(win,n,TR_text);
  rdsFORMSetMultilineHeight(win,n,mlheight);
  rdsSetObjectInt(win,n,RDS_FORMVAL_MLRETURNS,1);

  n=fieldbase+6;
  rdsFORMAddEdit(win,tab,n,RDS_FORMCTRL_COMBOLIST | RDS_FORMFLAG_CHECK,"�������� �������� �������:",150);
  rdsFORMSetComboList(win,n,"����� �������\n����� ������");
  rdsFORMSetInt(win,n,TR_params==L57_BLOCKTEXT_BEFORE?0:1);
  rdsFORMSetEnableCheck(win,n,TR_params==L57_BLOCKTEXT_BEFORE||TR_params==L57_BLOCKTEXT_AFTER);

  n=fieldbase+4;
  rdsFORMAddEdit(win,tab,n,RDS_FORMCTRL_COMBOLIST,"������������ �� �����������:",150);
  rdsFORMSetComboList(win,n,"�����\n�� ������\n������");
  rdsFORMSetInt(win,n,rect.HAlign+1);

  n=fieldbase+5;
  rdsFORMAddEdit(win,tab,n,RDS_FORMCTRL_COMBOLIST,"������������ �� ���������:",150);
  rdsFORMSetComboList(win,n,"������\n�� ������\n�����");
  rdsFORMSetInt(win,n,rect.VAlign+1);

  n=fieldbase+3;
  rdsFORMAddEdit(win,tab,n,RDS_FORMCTRL_FONTSELECT|RDS_FORMFLAG_LINE,"�����:",0);
  str=rdsStructToFontText(&font,NULL);
  rdsSetObjectStr(win,n,RDS_FORMVAL_VALUE,str);
  rdsFree(str);

  n=fieldbase+8;
  rdsFORMAddEdit(win,tab,n,RDS_FORMCTRL_COLOR | RDS_FORMFLAG_CHECK,"��� ��������������:",80);
  rdsFORMSetInt(win,n,(int)rect.Fill);
  rdsFORMSetEnableCheck(win,n,rect.FillFlag);

  n=fieldbase+9;
  rdsFORMAddEdit(win,tab,n,RDS_FORMCTRL_COLOR | RDS_FORMFLAG_CHECK,"����� ��������������:",80);
  rdsFORMSetInt(win,n,(int)rect.Border);
  rdsFORMSetEnableCheck(win,n,rect.BorderFlag);

  n=fieldbase+10;
  rdsFORMAddEdit(win,tab,n,RDS_FORMCTRL_UPDOWN,"������� �����:",50);
  rdsSetObjectInt(win,n,RDS_FORMVAL_VALUE,rect.BorderWidth);
  rdsSetObjectInt(win,n,RDS_FORMVAL_UPDOWNMIN,0);
  rdsSetObjectInt(win,n,RDS_FORMVAL_UPDOWNMAX,50);
  rdsSetObjectInt(win,n,RDS_FORMVAL_UPDOWNINC,1);

}
//---------------------------------------------------------------------------

// ������� ������ ����� ����� ��� �������������� �������� ���� ����� � �������� ��� �������������� � �������
void L57_TextRectEdit_Apply(RDS_BHANDLE block,RDS_HOBJECT win,int &TR_params,L57_String &TR_text,char *functitles)
{ RDS_BLOCKTEXTRECTDATA rect;
  RDS_SERVFONTPARAMS font;
  int fieldbase=L57_BLOCKTEXT_FIELDBASE;
  char *str;
  L57_String func,text;

  rect.servSize=sizeof(rect);
  font.servSize=sizeof(font);
  rdsGetBlockTextRectParams(block,&rect,&font);

  func.Assign(functitles);

  TR_text.Assign(rdsFORMGetString(win,fieldbase+2));

  if(rdsFORMGetInt(win,fieldbase)==0)
    { TR_params=L57_BLOCKTEXT_NOCONTROL;
      return;
    }
  else if(!rdsFORMGetEnableCheck(win,fieldbase+6))
    TR_params=L57_BLOCKTEXT_FIXED;
  else
    TR_params=(rdsFORMGetInt(win,fieldbase+6)==0?L57_BLOCKTEXT_BEFORE:L57_BLOCKTEXT_AFTER);

  rect.BorderFlag=rdsFORMGetEnableCheck(win,fieldbase+9);
  rect.Border=(COLORREF)rdsFORMGetInt(win,fieldbase+9);
  rect.Fill=(COLORREF)rdsFORMGetInt(win,fieldbase+8);
  text.Assign(TR_text);
  rect.HAlign=rdsFORMGetInt(win,fieldbase+4)-1;
  rect.VAlign=rdsFORMGetInt(win,fieldbase+5)-1; // (Type==HORZSTRUPETYPE_HEAD)?0:-1;
  rect.FillFlag=rdsFORMGetEnableCheck(win,fieldbase+8);
  rect.BorderWidth=rdsGetObjectInt(win,fieldbase+10,RDS_FORMVAL_VALUE);
  rect.ScaleBorder=TRUE;
  rect.Exists=TRUE;

  str=rdsGetObjectStr(win,fieldbase+3,RDS_FORMVAL_VALUE);
  if(str)
    rdsFontTextToStruct(str,NULL,&font);

  switch(TR_params)
    { case L57_BLOCKTEXT_BEFORE:
        if(func.IsNotBlank() && text.IsNotBlank())
          func.AddString("\n");
        func.AddString(text);
        rect.Text=func.c_str();
        break;
      case L57_BLOCKTEXT_AFTER:
        if(text.IsNotBlank() && func.IsNotBlank())
          text.AddString("\n");
        text.AddString(func);
        rect.Text=text.c_str();
        break;
      default:
        rect.Text=TR_text.c_str();
    }

  rdsSetBlockTextRectParams(block,&rect,&font,RDS_SBTRPREFRESH);
}
//---------------------------------------------------------------------------


// ���������� ����� ����� ��� �������������� �������� ���� ����� � �������� ��� �������������� � �������
void L57_TextRectEdit_EnableControls(RDS_HOBJECT win)
{ int fieldbase=L57_BLOCKTEXT_FIELDBASE;
  int allow=(rdsFORMGetInt(win,fieldbase)!=0);
  int border=rdsFORMGetEnableCheck(win,fieldbase+9);

  rdsFORMEnableControl(win,fieldbase+2,allow);
  rdsFORMEnableControl(win,fieldbase+6,allow);
  rdsFORMEnableControl(win,fieldbase+4,allow);
  rdsFORMEnableControl(win,fieldbase+5,allow);
  rdsFORMEnableControl(win,fieldbase+3,allow);
  rdsFORMEnableControl(win,fieldbase+8,allow);
  rdsFORMEnableControl(win,fieldbase+9,allow);

  rdsFORMEnableControl(win,fieldbase+10,allow && border);
}
//---------------------------------------------------------------------------

