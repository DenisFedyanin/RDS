//---------------------------------------------------------------------------


#pragma hdrstop

#include "MultiTableSaver.h"
#include "MultiTableFileLibrary.h"
#include "FileLibrary.h"
#ifdef MTFL_SOURCE
  #include "MultiTableEdit.h"
#endif
#if defined(MTFL_TABLE) || defined(MTFL_DEFECTS)
  #include "MultiTable.h"
#endif

//---------------------------------------------------------------------------
// Сохранение в библиотеке исходных данных таблицы и сгенерированной таблицы
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// Идентификаторы частей и префиксы
#define MLTFL_PREFIX    "MultiTableFileLibrary"
#define MLTFL_SOURCE    0       // Исходные данные
#define MLTFL_TABLE     1       // Сгенерированная таблица
#define MLTFL_DEFECTS   2       // Сгенерированная таблица дефектов
#define MLTFL_REMARK    3       // Комментарий
//---------------------------------------------------------------------------

// Сохранение
bool __fastcall mtflSaveLibrary(char *filename,
                                #ifdef MTFL_SOURCE
                                  TMultiTableSource *Source,
                                #endif
                                #ifdef MTFL_TABLE
                                  TMultiTable *Result,
                                #endif
                                #ifdef MTFL_DEFECTS
                                  TMultiTable *Defects,
                                #endif
                                char *remark,
                                FILETIME *filetime)
{ TFileLibrary lib;
  TMultiTableSaverHandle saver;

  if(!lib.OpenFileWrite(filename,MLTFL_PREFIX))
    return false;

  #ifdef MTFL_SOURCE
    if(Source)
      { if(!lib.BeforeWrite(MLTFL_SOURCE))
          return false;
        saver.SetHandle(lib.File);
        if(!Source->Save(&saver))
          return false;
        if(!lib.AfterWrite())
          return false;
      }
  #endif

  #ifdef MTFL_TABLE
    if(Result)
      { if(!lib.BeforeWrite(MLTFL_TABLE))
          return false;
        saver.SetHandle(lib.File);
        if(!Result->Save(&saver))
          return false;
        if(!lib.AfterWrite())
          return false;
      }
  #endif

  #ifdef MTFL_DEFECTS
    if(Defects)
      { if(!lib.BeforeWrite(MLTFL_DEFECTS))
          return false;
        saver.SetHandle(lib.File);
        if(!Defects->Save(&saver))
          return false;
        if(!lib.AfterWrite())
          return false;
      }
  #endif

  if(remark)
    { if(!lib.BeforeWrite(MLTFL_REMARK))
        return false;
      saver.SetHandle(lib.File);
      if(!saver.WriteString(remark))
        return false;
      if(!lib.AfterWrite())
        return false;
    }

  if(filetime==NULL)
    return lib.SaveIndexAndCloseFile();

  // Нужно будет заново открыть файл для чтения и считать время последней записи
  if(!lib.SaveIndexAndCloseFile())
    return false;

  if(!lib.OpenFileRead(filename,MLTFL_PREFIX))
    return false;
  if(!GetFileTime(lib.File,NULL,NULL,filetime))
    return false;
  lib.CloseFile();
  return true;
}
//---------------------------------------------------------------------------

// Загрузка - начало
bool __fastcall mtflLoadLibrary_Begin(TFileLibrary *lib,
                                      char *filename,
                                      FILETIME *filetime)
{
  if(!lib)
    return false;

  if(!lib->OpenFileRead(filename,MLTFL_PREFIX))
    return false;

  if(filetime)
    { // CompareFileTime
      if(!GetFileTime(lib->File,NULL,NULL,filetime))
        return false;
    }
  return true;
}
//---------------------------------------------------------------------------

// Загрузка - проверка, есть ли в файле данные таблицы
bool __fastcall mtflLoadLibrary_HasResultTable(TFileLibrary *lib)
{
  if(!lib)
    return false;
  return lib->FindItem(MLTFL_TABLE)!=NULL;
}
//---------------------------------------------------------------------------

// Загрузка - основные действия (файл открыт)
bool __fastcall mtflLoadLibrary_End(TFileLibrary *lib,
                                    #ifdef MTFL_SOURCE
                                      TMultiTableSource *Source,
                                    #endif
                                    #ifdef MTFL_TABLE
                                      TMultiTable *Result,
                                    #endif
                                    #ifdef MTFL_DEFECTS
                                      TMultiTable *Defects,
                                    #endif
                                    char **dynremark)
{ TMultiTableSaverHandle saver;

  if(!lib)
    return false;

  #ifdef MTFL_SOURCE
    if(Source)
      { bool notfound;
        if(lib->PrepareToRead(MLTFL_SOURCE,&notfound))
          { // Найдено и готово
            saver.SetHandle(lib->File);
            if(!Source->Load(&saver))
              return false;
          }
        else // Не найдено или ошибки
          { if(!notfound) // Ошибки
              return false;
          }
      }
  #endif

  #ifdef MTFL_TABLE
    if(Result)
      { bool notfound;
        if(lib->PrepareToRead(MLTFL_TABLE,&notfound))
          { // Найдено и готово
            saver.SetHandle(lib->File);
            if(!Result->Load(&saver))
              return false;
          }
        else // Не найдено или ошибки
          { if(!notfound) // Ошибки
              return false;
          }
      }
  #endif

  #ifdef MTFL_DEFECTS
    if(Defects)
      { bool notfound;
        if(lib->PrepareToRead(MLTFL_DEFECTS,&notfound))
          { // Найдено и готово
            saver.SetHandle(lib->File);
            if(!Defects->Load(&saver))
              return false;
          }
        else // Не найдено или ошибки
          { if(!notfound) // Ошибки
              return false;
          }
      }
  #endif

  if(dynremark)
    { bool notfound;
      *dynremark=NULL;
      if(lib->PrepareToRead(MLTFL_REMARK,&notfound))
        { // Найдено и готово
          saver.SetHandle(lib->File);
          if(!saver.ReadString(dynremark))
            return false;
        }
      else // Не найдено или ошибки
        { if(!notfound) // Ошибки
            return false;
        }
    }

  return true;
}
//---------------------------------------------------------------------------


// Загрузка
bool __fastcall mtflLoadLibrary(char *filename,
                                #ifdef MTFL_SOURCE
                                  TMultiTableSource *Source,
                                #endif
                                #ifdef MTFL_TABLE
                                  TMultiTable *Result,
                                #endif
                                #ifdef MTFL_DEFECTS
                                  TMultiTable *Defects,
                                #endif
                                char **dynremark,
                                FILETIME *filetime)
{ TFileLibrary lib;

  if(!mtflLoadLibrary_Begin(&lib,filename,filetime))
    return false;
  return mtflLoadLibrary_End(&lib,
                             #ifdef MTFL_SOURCE
                               Source,
                             #endif
                             #ifdef MTFL_TABLE
                               Result,
                             #endif
                             #ifdef MTFL_DEFECTS
                               Defects,
                             #endif
                             dynremark);

   /*TFileLibrary lib;
  TMultiTableSaverHandle saver;

  if(!lib.OpenFileRead(filename,MLTFL_PREFIX))
    return false;

  if(filetime)
    { // CompareFileTime
      if(!GetFileTime(lib.File,NULL,NULL,filetime))
        return false;
    }

  if(Source)
    { bool notfound;
      if(lib.PrepareToRead(MLTFL_SOURCE,&notfound))
        { // Найдено и готово
          saver.SetHandle(lib.File);
          if(!Source->Load(&saver))
            return false;
        }
      else // Не найдено или ошибки
        { if(!notfound) // Ошибки
            return false;
        }
    }

  if(Result)
    { bool notfound;
      if(lib.PrepareToRead(MLTFL_TABLE,&notfound))
        { // Найдено и готово
          saver.SetHandle(lib.File);
          if(!Result->Load(&saver))
            return false;
        }
      else // Не найдено или ошибки
        { if(!notfound) // Ошибки
            return false;
        }
    }

  if(Defects)
    { bool notfound;
      if(lib.PrepareToRead(MLTFL_DEFECTS,&notfound))
        { // Найдено и готово
          saver.SetHandle(lib.File);
          if(!Defects->Load(&saver))
            return false;
        }
      else // Не найдено или ошибки
        { if(!notfound) // Ошибки
            return false;
        }
    }

  if(dynremark)
    { bool notfound;
      *dynremark=NULL;
      if(lib.PrepareToRead(MLTFL_REMARK,&notfound))
        { // Найдено и готово
          saver.SetHandle(lib.File);
          if(!saver.ReadString(dynremark))
            return false;
        }
      else // Не найдено или ошибки
        { if(!notfound) // Ошибки
            return false;
        }
    }

  return true;
  */
}
//---------------------------------------------------------------------------

