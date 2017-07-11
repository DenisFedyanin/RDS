//---------------------------------------------------------------------------


#pragma hdrstop

#include "FileLibrary.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// Элемент библиотеки
//---------------------------------------------------------------------------
// Вычитание двух двухDWORDовых чисел
bool __fastcall TFileLibraryItem::SubtractQWORDS(DWORD FromHigh,DWORD FromLow,
                                                 DWORD SubHigh,DWORD SubLow,
                                                 DWORD &ResHigh,DWORD &ResLow)
{
  if(FromLow>=SubLow)
    { // Младшие вычитаются без переноса
      ResLow=FromLow-SubLow;
      ResHigh=FromHigh-SubHigh;
      return FromHigh>=SubHigh; // Иначе отрицательный результат
    }
  // Младшие вычитаются с переносом
  ResLow=FromLow-SubLow;
  ResHigh=FromHigh-SubHigh-1;
  return FromHigh>SubHigh;  // Иначе отрицательный результат
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
// Библиотека
//---------------------------------------------------------------------------
// Очистить все
void __fastcall TFileLibrary::Clear(void)
{
  #define REMOVELIST(type,var) \
    while(var) \
      { type *i=var->Next; delete var; var=i; }

  REMOVELIST(TFileLibraryItem,First)
  #undef REMOVELIST
}
//---------------------------------------------------------------------------

// Добавить элемент
TFileLibraryItem * __fastcall TFileLibrary::Add(void)
{ TFileLibraryItem *i=new TFileLibraryItem();
  i->Next=First;
  First=i;
  return i;
}
//---------------------------------------------------------------------------

// Вспомогательные - запись и чтение данных
#define Windows_ChunkSizeMax (1 << 22) // Читать/писать не больше этого
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

// Открыть файл на запись
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
// Открыть файл на чтение
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
        { // Читаем и сравниваем префикс
          char *aux=new char[l];
          if(!Read(aux,l))
            { delete[] aux;
              return false;
            }
          if(memcmp(aux,prefix,l)) // Не совпало
            { delete[] aux;
              if(pBadFormat) *pBadFormat=true;
              return false;
            }
          // Совпало
          delete[] aux;
        }
    }
  // Читаем оглавление
  if(!ReadIndex())
    { if(pBadFormat) *pBadFormat=true;
      return false;
    }
  return true;
}
//---------------------------------------------------------------------------
// Закрыть файл
void __fastcall TFileLibrary::CloseFile(void)
{ if(File!=INVALID_HANDLE_VALUE)
    CloseHandle(File);
  File=INVALID_HANDLE_VALUE;
  Waiting=false;
}
//---------------------------------------------------------------------------

// Получить текущее положение открытого файла
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

// Установить положение открытого файла
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


// Вызов перед началом записи очередного куска
bool __fastcall TFileLibrary::BeforeWrite(int id)
{ if(File==INVALID_HANDLE_VALUE)
    return false;
  Add(); // First - добавленный элемент
  First->Id=id;
  if(GetFilePos(First->Start))
    { Waiting=true;
      return true;
    }
  return false;
}
//---------------------------------------------------------------------------

// Вызов после записи очередного куска
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

// Записать оглавление и закрыть файл
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
          // Признак элемента
          if(!Write(&tag,sizeof(tag)))
            ok=false;
          // Идентификатор элемента
          if(!Write(&(i->Id),sizeof(i->Id)))
            ok=false;
          // Положение элемента
          if(!Write(&(i->Start.High),sizeof(DWORD)))
            ok=false;
          if(!Write(&(i->Start.Low),sizeof(DWORD)))
            ok=false;
          // Длина элемента
          if(!Write(&(i->Length.High),sizeof(DWORD)))
            ok=false;
          if(!Write(&(i->Length.Low),sizeof(DWORD)))
            ok=false;
        }
    }
  // Конец
  tag=0;
  ok=ok && Write(&tag,sizeof(tag));
  // Запись положения начала оглавления
  ok=ok && Write(&(IndexPos.High),sizeof(DWORD)) &&
           Write(&(IndexPos.Low),sizeof(DWORD));
  CloseFile();
  return ok;
}
//---------------------------------------------------------------------------

// Считать оглавление открытого файла
bool __fastcall TFileLibrary::ReadIndex(void)
{ TFileLibraryQWord IndexPos,aux,size;
  LONG high;
  DWORD low;
  bool ok;
  BYTE tag;

  Clear();


  if(File==INVALID_HANDLE_VALUE)
    return false;

  // В конце файла записано положение начала оглавления (2xDWORD)
  size.Low=GetFileSize(File,&(size.High));
  if(size.Low==0xFFFFFFFF && (GetLastError()!=NO_ERROR) )
    return false;
  // Нужно вычесть из размера файла 2*DWORD
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
  // Читаем
  ok=Read(&(IndexPos.High),sizeof(DWORD)) &&
     Read(&(IndexPos.Low),sizeof(DWORD));
  if(!ok)
    return false;
  // Переходим на IndexPos
  if(!SetFilePos(IndexPos))
    return false;
  // Читаем оглавление
  for(;;)
    { TFileLibraryItem *i;
      // Признак элемента
      if(!Read(&tag,sizeof(tag)))
        return false;
      if(tag!=1) // Конец
        break;
      i=Add();
      // Идентификатор элемента
      if(!Read(&(i->Id),sizeof(i->Id)))
        return false;
      // Положение элемента
      if(!Read(&(i->Start.High),sizeof(DWORD)))
        return false;
      if(!Read(&(i->Start.Low),sizeof(DWORD)))
        return false;
      // Длина элемента
      if(!Read(&(i->Length.High),sizeof(DWORD)))
        return false;
      if(!Read(&(i->Length.Low),sizeof(DWORD)))
        return false;
    } // for(;;)
  return true;
}
//---------------------------------------------------------------------------

// Найти первый попавшийся кусок по идентификатору
TFileLibraryItem * __fastcall TFileLibrary::FindItem(int id)
{
  for(TFileLibraryItem *i=First;i!=NULL;i=i->Next)
    if(i->Id==id)
      return i;
  return NULL;
}
//---------------------------------------------------------------------------

// Позиционирование для чтения куска
TFileLibraryItem * __fastcall TFileLibrary::PrepareToRead(int id,bool *pNotFound)
{ TFileLibraryItem *i=FindItem(id);
  if(pNotFound) *pNotFound=(i==NULL);
  if(!i)
    return NULL;
  return SetFilePos(i->Start)?i:NULL;
}
//---------------------------------------------------------------------------

