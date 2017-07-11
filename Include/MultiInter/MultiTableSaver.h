//---------------------------------------------------------------------------

#ifndef MultiTableSaverH
#define MultiTableSaverH
#include <windows.h>
//---------------------------------------------------------------------------
// ��������������� ����� ��� ������ ����������/��������
// (�� ���� ����� ���������� �������)
class TMultiTableSaver
{ public:
    virtual bool __fastcall Write(void *buf,DWORD count)=0;
    virtual bool __fastcall Read(void *buf,DWORD count)=0;

    // ������ ������
    bool __fastcall WriteString(char *str);
    // ������ ������
    bool __fastcall ReadString(char **pstr);

    TMultiTableSaver(void){};
    virtual ~TMultiTableSaver(){};
};
//---------------------------------------------------------------------------

// ��������������� ����� ��� ������ ����������/��������
class TMultiTableSaverHandle : public TMultiTableSaver
{ protected:
    HANDLE h;
  public:
    inline void SetHandle(HANDLE ss){h=ss;};

    bool __fastcall Write(void *buf,DWORD count);
    bool __fastcall Read(void *buf,DWORD count);

    // ��������������� - ������ � ������ ������
    static bool __fastcall WindowsWrite(HANDLE h,const void *data,size_t *size);
    static bool __fastcall WindowsRead(HANDLE h, void *data, size_t *size);


    TMultiTableSaverHandle(void):TMultiTableSaver(){h=INVALID_HANDLE_VALUE;};
    virtual ~TMultiTableSaverHandle(){};
};
//---------------------------------------------------------------------------

// ��������������� ����� ��� ����� ����������/��������
class TMultiTableSaverFile : public TMultiTableSaverHandle
{ public:
    // ������� ���� �� ������
    bool __fastcall OpenFileWrite(char *filename);
    // ������� ���� �� ������
    bool __fastcall OpenFileRead(char *filename);
    // ������� ����
    void __fastcall CloseFile(void);

    TMultiTableSaverFile(void):TMultiTableSaverHandle(){};
    virtual ~TMultiTableSaverFile(){CloseFile();};
};
//---------------------------------------------------------------------------
#endif
