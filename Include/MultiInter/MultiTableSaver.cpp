//---------------------------------------------------------------------------


#pragma hdrstop

#include "MultiTableSaver.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// Вспомогательный класс для потока сохранения/загрузки
//---------------------------------------------------------------------------
// Запись строки
bool __fastcall TMultiTableSaver::WriteString(char *str)
{ DWORD n,len=str?strlen(str):0;
  if(!Write(&len,sizeof(len))) return false;
  n=~len;
  if(!Write(&n,sizeof(n))) return false;
  if(len==0) return true;
  return Write(str,len);
}
//---------------------------------------------------------------------------
// Чтение строки
bool __fastcall TMultiTableSaver::ReadString(char **pstr)
{ DWORD len,n;
  char *buf;
  if(!pstr) return false;
  *pstr=NULL;
  if(!Read(&len,sizeof(len))) return false;
  if(!Read(&n,sizeof(n))) return false;
  if(n!=~len)
    return false;
  if(len==0)
    return true;
  buf=new char[len+1];
  if(!Read(buf,len))
    { delete[] buf;
      return false;
    }
  buf[len]=0;
  *pstr=buf;
  return true;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Вспомогательный класс для потока сохранения/загрузки
//---------------------------------------------------------------------------
#define Windows_ChunkSizeMax (1 << 22) // Читать/писать не больше этого
// Вспомогательные - запись и чтение данных
bool __fastcall TMultiTableSaverHandle::WindowsRead(HANDLE h, void *data, size_t *size)
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
bool __fastcall TMultiTableSaverHandle::WindowsWrite(HANDLE h,const void *data,size_t *size)
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
//---------------------------------------------------------------------------
bool __fastcall TMultiTableSaverHandle::Write(void *buf,DWORD count)
{ size_t act_write=count;
  if(!WindowsWrite(h,buf,&act_write))
    return false;
  return act_write==count;
}
//---------------------------------------------------------------------------
bool __fastcall TMultiTableSaverHandle::Read(void *buf,DWORD count)
{ size_t act_read=count;
  if(!WindowsRead(h,buf,&act_read))
    return false;
  return act_read==count;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Вспомогательный класс для файла сохранения/загрузки
//---------------------------------------------------------------------------
// Открыть файл на запись
bool __fastcall TMultiTableSaverFile::OpenFileWrite(char *filename)
{
  CloseFile();
  h=CreateFile(filename,GENERIC_WRITE,
               FILE_SHARE_READ, NULL,
               CREATE_ALWAYS,
               FILE_ATTRIBUTE_NORMAL,NULL);
  return (h!=INVALID_HANDLE_VALUE);
}
//---------------------------------------------------------------------------
// Открыть файл на чтение
bool __fastcall TMultiTableSaverFile::OpenFileRead(char *filename)
{
  CloseFile();
  h=CreateFile(filename,GENERIC_READ,
               FILE_SHARE_READ, NULL,
               OPEN_EXISTING,
               FILE_ATTRIBUTE_NORMAL, NULL);
  return (h!=INVALID_HANDLE_VALUE);
}
//---------------------------------------------------------------------------
// Закрыть файл
void __fastcall TMultiTableSaverFile::CloseFile(void)
{ if(h!=INVALID_HANDLE_VALUE)
    CloseHandle(h);
  h=INVALID_HANDLE_VALUE;
}
//---------------------------------------------------------------------------
