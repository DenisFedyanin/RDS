//---------------------------------------------------------------------------


#pragma hdrstop

#include "FileLibrary.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ������� ����������
//---------------------------------------------------------------------------
// ��������� ���� ����DWORD���� �����
bool __fastcall TFileLibraryItem::SubtractQWORDS(DWORD FromHigh,DWORD FromLow,
                                                 DWORD SubHigh,DWORD SubLow,
                                                 DWORD &ResHigh,DWORD &ResLow)
{
  if(FromLow>=SubLow)
    { // ������� ���������� ��� ��������
      ResLow=FromLow-SubLow;
      ResHigh=FromHigh-SubHigh;
      return FromHigh>=SubHigh; // ����� ������������� ���������
    }
  // ������� ���������� � ���������
  ResLow=FromLow-SubLow;
  ResHigh=FromHigh-SubHigh-1;
  return FromHigh>SubHigh;  // ����� ������������� ���������
}
//---------------------------------------------------------------------------

/*
HANDLE hFile;

DWORD dwCurrentFilePosition;

dwCurrentFilePosition = SetFilePointer(
    hFile, // must have GENERIC_READ and/or GENERIC_WRITE
    0,     // do not move pointer
    NULL,  // hFile is not large enough to need this pointer
    FILE_CURRENT);  // provides offset from current position
 */

//---------------------------------------------------------------------------
// ����������
//---------------------------------------------------------------------------
// �������� ���
void __fastcall TFileLibrary::Clear(void)
{
  #define REMOVELIST(type,var) \
    while(var) \
      { type *i=var->Next; delete var; var=i; }

  REMOVELIST(TFileLibraryItem,First)
  #undef REMOVELIST
}
//---------------------------------------------------------------------------

// �������� �������
TFileLibraryItem * __fastcall TFileLibrary::Add(void)
{ TFileLibraryItem *i=new TFileLibraryItem();
  i->Next=First;
  First=i;
  return i;
}
//---------------------------------------------------------------------------

// ��������������� - ������ � ������ ������
#define Windows_ChunkSizeMax (1 << 22) // ������/������ �� ������ �����
bool __fastcall TFileLibrary::WindowsRead(HANDLE h, void *data, size_t *size)
{
  size_t originalSize = *size;
  if (originalSize == 0)
    return true;

  if(h==INVALID_HANDLE_VALUE)
    return false;

  *size = 0;
  do
  {
    DWORD curSize = (originalSize > Windows_ChunkSizeMax) ? Windows_ChunkSizeMax : (DWORD)originalSize;
    DWORD processed = 0;
    BOOL res = ReadFile(h, data, curSize, &processed, NULL);
    data = (void *)((BYTE*)data + processed);
    originalSize -= processed;
    *size += processed;
    if (!res)
      return false;
    if (processed == 0)
      break;
  }
  while (originalSize > 0);
  return true;
}
bool __fastcall TFileLibrary::WindowsWrite(HANDLE h,const void *data,size_t *size)
{ size_t originalSize = *size;
  if (originalSize == 0)
    return true;
  if(h==INVALID_HANDLE_VALUE)
    return false;

  *size = 0;
  do
  {
    DWORD curSize = (originalSize > Windows_ChunkSizeMax) ? Windows_ChunkSizeMax : (DWORD)originalSize;
    DWORD processed = 0;
    BOOL res = WriteFile(h, data, curSize, &processed, NULL);
    data = (void *)((BYTE*)data + processed);
    originalSize -= processed;
    *size += processed;
    if (!res)
      return false;
    if (processed == 0)
      break;
  }
  while (originalSize > 0);
  return true;
}
#undef Windows_ChunkSizeMax
bool __fastcall TFileLibrary::Write(void *buf,DWORD count)
{ size_t act_write=count;
  if(!WindowsWrite(File,buf,&act_write))
    return false;
  return act_write==count;
}
bool __fastcall TFileLibrary::Read(void *buf,DWORD count)
{ size_t act_read=count;
  if(!WindowsRead(File,buf,&act_read))
    return false;
  return act_read==count;
}
//---------------------------------------------------------------------------

// ������� ���� �� ������
bool __fastcall TFileLibrary::OpenFileWrite(char *filename,char *prefix)
{
  CloseFile();
  File=CreateFile(filename,GENERIC_WRITE,
                  FILE_SHARE_READ, NULL,
                  CREATE_ALWAYS,
                  FILE_ATTRIBUTE_NORMAL,NULL);
  if(File==INVALID_HANDLE_VALUE)
    return false;
  if(prefix)
    { int l=strlen(prefix);
      if(!l) return true;
      return Write(prefix,l);
    }
  return true;
}
//---------------------------------------------------------------------------
// ������� ���� �� ������
bool __fastcall TFileLibrary::OpenFileRead(char *filename,char *prefix,bool *pBadFormat)
{
  if(pBadFormat) *pBadFormat=false;
  Clear();
  CloseFile();
  File=CreateFile(filename,GENERIC_READ,
                  FILE_SHARE_READ, NULL,
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL, NULL);
  if(File==INVALID_HANDLE_VALUE)
    return false;
  if(prefix)
    { int l=strlen(prefix);
      if(l)
        { // ������ � ���������� �������
          char *aux=new char[l];
          if(!Read(aux,l))
            { delete[] aux;
              return false;
            }
          if(memcmp(aux,prefix,l)) // �� �������
            { delete[] aux;
              if(pBadFormat) *pBadFormat=true;
              return false;
            }
          // �������
          delete[] aux;
        }
    }
  // ������ ����������
  if(!ReadIndex())
    { if(pBadFormat) *pBadFormat=true;
      return false;
    }
  return true;
}
//---------------------------------------------------------------------------
// ������� ����
void __fastcall TFileLibrary::CloseFile(void)
{ if(File!=INVALID_HANDLE_VALUE)
    CloseHandle(File);
  File=INVALID_HANDLE_VALUE;
  Waiting=false;
}
//---------------------------------------------------------------------------

// �������� ������� ��������� ��������� �����
bool __fastcall TFileLibrary::GetFilePos(TFileLibraryQWord &pos)
{ if(File==INVALID_HANDLE_VALUE)
    return false;
  LONG high=0;
  pos.Low=SetFilePointer(File,
                         0,     // do not move pointer
                         &high,  // hFile is not large enough to need this pointer
                         FILE_CURRENT);  // provides offset from current position
  pos.High=high;
  if(pos.Low==0xFFFFFFFF && (GetLastError()!=NO_ERROR) )
    return false;
  return true;
}
//---------------------------------------------------------------------------

// ���������� ��������� ��������� �����
bool __fastcall TFileLibrary::SetFilePos(const TFileLibraryQWord &pos)
{ LONG high;
  DWORD low;
  if(File==INVALID_HANDLE_VALUE)
    return false;
  high=pos.High;
  low=SetFilePointer(File,
                     pos.Low,
                     &high,  // hFile is not large enough to need this pointer
                     FILE_BEGIN);  // provides offset from current position
  if(low==0xFFFFFFFF && (GetLastError()!=NO_ERROR) )
    return false;
  if(low!=pos.Low || ((DWORD)high)!=pos.High)
    return false;
  return true;
}
//---------------------------------------------------------------------------


// ����� ����� ������� ������ ���������� �����
bool __fastcall TFileLibrary::BeforeWrite(int id)
{ if(File==INVALID_HANDLE_VALUE)
    return false;
  Add(); // First - ����������� �������
  First->Id=id;
  if(GetFilePos(First->Start))
    { Waiting=true;
      return true;
    }
  return false;
}
//---------------------------------------------------------------------------

// ����� ����� ������ ���������� �����
bool __fastcall TFileLibrary::AfterWrite(void)
{ TFileLibraryQWord pos;
  if(File==INVALID_HANDLE_VALUE || First==NULL || (!Waiting))
    return false;
  if(!GetFilePos(pos))
    return false;
  return First->SubtractQWORDS(pos.High,pos.Low,
                               First->Start.High,First->Start.Low,
                               First->Length.High,First->Length.Low);
}
//---------------------------------------------------------------------------

// �������� ���������� � ������� ����
bool __fastcall TFileLibrary::SaveIndexAndCloseFile(void)
{ bool ok;
  TFileLibraryQWord IndexPos;
  BYTE tag;

  if(File==INVALID_HANDLE_VALUE)
    return false;

  ok=GetFilePos(IndexPos);
  if(ok)
    { for(TFileLibraryItem *i=First;i!=NULL && ok;i=i->Next)
        { tag=1;
          // ������� ��������
          if(!Write(&tag,sizeof(tag)))
            ok=false;
          // ������������� ��������
          if(!Write(&(i->Id),sizeof(i->Id)))
            ok=false;
          // ��������� ��������
          if(!Write(&(i->Start.High),sizeof(DWORD)))
            ok=false;
          if(!Write(&(i->Start.Low),sizeof(DWORD)))
            ok=false;
          // ����� ��������
          if(!Write(&(i->Length.High),sizeof(DWORD)))
            ok=false;
          if(!Write(&(i->Length.Low),sizeof(DWORD)))
            ok=false;
        }
    }
  // �����
  tag=0;
  ok=ok && Write(&tag,sizeof(tag));
  // ������ ��������� ������ ����������
  ok=ok && Write(&(IndexPos.High),sizeof(DWORD)) &&
           Write(&(IndexPos.Low),sizeof(DWORD));
  CloseFile();
  return ok;
}
//---------------------------------------------------------------------------

// ������� ���������� ��������� �����
bool __fastcall TFileLibrary::ReadIndex(void)
{ TFileLibraryQWord IndexPos,aux,size;
  LONG high;
  DWORD low;
  bool ok;
  BYTE tag;

  Clear();


  if(File==INVALID_HANDLE_VALUE)
    return false;

  // � ����� ����� �������� ��������� ������ ���������� (2xDWORD)
  size.Low=GetFileSize(File,&(size.High));
  if(size.Low==0xFFFFFFFF && (GetLastError()!=NO_ERROR) )
    return false;
  // ����� ������� �� ������� ����� 2*DWORD
  aux.High=0; aux.Low=2*sizeof(DWORD);
  if(!TFileLibraryItem::SubtractQWORDS(size.High,size.Low,
                                       aux.High,aux.Low,
                                       IndexPos.High,IndexPos.Low))
    return false;

  high=IndexPos.High;
  low=SetFilePointer(File,
                     IndexPos.Low,
                     &high,
                     FILE_BEGIN);
  if(low==0xFFFFFFFF && (GetLastError()!=NO_ERROR) )
    return false;
  // ������
  ok=Read(&(IndexPos.High),sizeof(DWORD)) &&
     Read(&(IndexPos.Low),sizeof(DWORD));
  if(!ok)
    return false;
  // ��������� �� IndexPos
  if(!SetFilePos(IndexPos))
    return false;
  // ������ ����������
  for(;;)
    { TFileLibraryItem *i;
      // ������� ��������
      if(!Read(&tag,sizeof(tag)))
        return false;
      if(tag!=1) // �����
        break;
      i=Add();
      // ������������� ��������
      if(!Read(&(i->Id),sizeof(i->Id)))
        return false;
      // ��������� ��������
      if(!Read(&(i->Start.High),sizeof(DWORD)))
        return false;
      if(!Read(&(i->Start.Low),sizeof(DWORD)))
        return false;
      // ����� ��������
      if(!Read(&(i->Length.High),sizeof(DWORD)))
        return false;
      if(!Read(&(i->Length.Low),sizeof(DWORD)))
        return false;
    } // for(;;)
  return true;
}
//---------------------------------------------------------------------------

// ����� ������ ���������� ����� �� ��������������
TFileLibraryItem * __fastcall TFileLibrary::FindItem(int id)
{
  for(TFileLibraryItem *i=First;i!=NULL;i=i->Next)
    if(i->Id==id)
      return i;
  return NULL;
}
//---------------------------------------------------------------------------

// ���������������� ��� ������ �����
TFileLibraryItem * __fastcall TFileLibrary::PrepareToRead(int id,bool *pNotFound)
{ TFileLibraryItem *i=FindItem(id);
  if(pNotFound) *pNotFound=(i==NULL);
  if(!i)
    return NULL;
  return SetFilePos(i->Start)?i:NULL;
}
//---------------------------------------------------------------------------

