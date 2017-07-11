//---------------------------------------------------------------------------

#ifndef FileLibraryH
#define FileLibraryH
#include <windows.h>
//---------------------------------------------------------------------------
// ���������� �����-�� ������ � �����
//---------------------------------------------------------------------------
// ���� DWORD ��� ��������� �����
struct TFileLibraryQWord
{ DWORD High,Low; };
//---------------------------------------------------------------------------
// ������� ����������
class TFileLibraryItem
{ public:
    TFileLibraryItem *Next;     // ��������� �������
    int Id;                     // ��� (����� �������������)
    TFileLibraryQWord Start;    // ��������� � �����
    TFileLibraryQWord Length;   // �����

    // ��������� ���� ����DWORD���� �����
    static bool __fastcall SubtractQWORDS(DWORD FromHigh,DWORD FromLow,
                                          DWORD SubHigh,DWORD SubLow,
                                          DWORD &ResHigh,DWORD &ResLow);

    TFileLibraryItem(void){Next=NULL;};
};
//---------------------------------------------------------------------------
// ����������
class TFileLibrary
{ private:
    bool Waiting;    // �������� ������ ������� After...

    // ������� ���������� ��������� �����
    bool __fastcall ReadIndex(void);
  public:
    TFileLibraryItem *First; // ������ �������
    HANDLE File;             // HANDLE ��������� �����

    // �������� ���
    void __fastcall Clear(void);

    // �������� �������
    TFileLibraryItem * __fastcall Add(void);

    // ��������������� - ������ � ������ ������
    static bool __fastcall WindowsWrite(HANDLE h,const void *data,size_t *size);
    static bool __fastcall WindowsRead(HANDLE h, void *data, size_t *size);
    bool __fastcall Write(void *buf,DWORD count);
    bool __fastcall Read(void *buf,DWORD count);

    // ������� ���� �� ������
    bool __fastcall OpenFileWrite(char *filename,char *prefix);
    // ������� ���� �� ������ � ������� ������
    bool __fastcall OpenFileRead(char *filename,char *prefix,bool *pBadFormat=NULL);
    // ������� ���� (��� ������, ��� ������ - ������������ SaveIndexAndCloseFile)
    void __fastcall CloseFile(void);
    // �������� ���������� � ������� ����
    bool __fastcall SaveIndexAndCloseFile(void);

    // �������� ������� ��������� ��������� �����
    bool __fastcall GetFilePos(TFileLibraryQWord &pos);
    // ���������� ��������� ��������� �����
    bool __fastcall SetFilePos(const TFileLibraryQWord &pos);

    // ����� ����� ������� ������ ���������� �����
    bool __fastcall BeforeWrite(int id);
    // ����� ����� ������ ���������� �����
    bool __fastcall AfterWrite(void);

    // ����� ������ ���������� ����� �� ��������������
    TFileLibraryItem * __fastcall FindItem(int id);

    // ���������������� ��� ������ �����
    TFileLibraryItem * __fastcall PrepareToRead(int id,bool *pNotFound=NULL);

    TFileLibrary(void){First=NULL;File=INVALID_HANDLE_VALUE;Waiting=false;};
    ~TFileLibrary(){Clear();CloseFile();};
};
//---------------------------------------------------------------------------
#endif
