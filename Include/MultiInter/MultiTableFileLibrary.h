//---------------------------------------------------------------------------

#ifndef MultiTableFileLibraryH
#define MultiTableFileLibraryH
#include <windows.h>
//---------------------------------------------------------------------------
// ���������� � ���������� �������� ������ ������� � ��������������� �������
//---------------------------------------------------------------------------
#if defined(MTFL_TABLE) || defined(MTFL_DEFECTS)
  class TMultiTable;
#endif
#ifdef MTFL_SOURCE
  class TMultiTableSource;
#endif
class TFileLibrary;
//---------------------------------------------------------------------------

// ����������
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

// �������� - ������
bool __fastcall mtflLoadLibrary_Begin(TFileLibrary *lib,
                                      char *filename,
                                      FILETIME *filetime=NULL);
// �������� - �������� �������� (���� ������)
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

// �������� - ��������, ���� �� � ����� ������ �������
bool __fastcall mtflLoadLibrary_HasResultTable(TFileLibrary *lib);

// ��������
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
