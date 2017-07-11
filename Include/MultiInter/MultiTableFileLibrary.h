//---------------------------------------------------------------------------

#ifndef MultiTableFileLibraryH
#define MultiTableFileLibraryH
#include <windows.h>
//---------------------------------------------------------------------------
// Сохранение в библиотеке исходных данных таблицы и сгенерированной таблицы
//---------------------------------------------------------------------------
#if defined(MTFL_TABLE) || defined(MTFL_DEFECTS)
  class TMultiTable;
#endif
#ifdef MTFL_SOURCE
  class TMultiTableSource;
#endif
class TFileLibrary;
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
                                  TMultiTable *Defects=NULL,
                                #endif
                                char *remark=NULL,
                                FILETIME *filetime=NULL);

// Загрузка - начало
bool __fastcall mtflLoadLibrary_Begin(TFileLibrary *lib,
                                      char *filename,
                                      FILETIME *filetime=NULL);
// Загрузка - основные действия (файл открыт)
bool __fastcall mtflLoadLibrary_End(TFileLibrary *lib,
                                    #ifdef MTFL_SOURCE
                                      TMultiTableSource *Source,
                                    #endif
                                    #ifdef MTFL_TABLE
                                      TMultiTable *Result,
                                    #endif
                                    #ifdef MTFL_DEFECTS
                                      TMultiTable *Defects=NULL,
                                    #endif
                                    char **dynremark=NULL);

// Загрузка - проверка, есть ли в файле данные таблицы
bool __fastcall mtflLoadLibrary_HasResultTable(TFileLibrary *lib);

// Загрузка
bool __fastcall mtflLoadLibrary(char *filename,
                                #ifdef MTFL_SOURCE
                                  TMultiTableSource *Source,
                                #endif
                                #ifdef MTFL_TABLE
                                  TMultiTable *Result,
                                #endif
                                #ifdef MTFL_DEFECTS
                                  TMultiTable *Defects=NULL,
                                #endif
                                char **dynremark=NULL,
                                FILETIME *filetime=NULL);
//---------------------------------------------------------------------------
#endif
