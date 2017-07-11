//---------------------------------------------------------------------------

#ifndef MultiTableSaverH
#define MultiTableSaverH
#include <windows.h>
//---------------------------------------------------------------------------
// Вспомогательный класс для потока сохранения/загрузки
// (от него нужно произвести потомка)
class TMultiTableSaver
{ public:
    virtual bool __fastcall Write(void *buf,DWORD count)=0;
    virtual bool __fastcall Read(void *buf,DWORD count)=0;

    // Запись строки
    bool __fastcall WriteString(char *str);
    // Чтение строки
    bool __fastcall ReadString(char **pstr);

    TMultiTableSaver(void){};
    virtual ~TMultiTableSaver(){};
};
//---------------------------------------------------------------------------

// Вспомогательный класс для потока сохранения/загрузки
class TMultiTableSaverHandle : public TMultiTableSaver
{ protected:
    HANDLE h;
  public:
    inline void SetHandle(HANDLE ss){h=ss;};

    bool __fastcall Write(void *buf,DWORD count);
    bool __fastcall Read(void *buf,DWORD count);

    // Вспомогательные - запись и чтение данных
    static bool __fastcall WindowsWrite(HANDLE h,const void *data,size_t *size);
    static bool __fastcall WindowsRead(HANDLE h, void *data, size_t *size);


    TMultiTableSaverHandle(void):TMultiTableSaver(){h=INVALID_HANDLE_VALUE;};
    virtual ~TMultiTableSaverHandle(){};
};
//---------------------------------------------------------------------------

// Вспомогательный класс для файла сохранения/загрузки
class TMultiTableSaverFile : public TMultiTableSaverHandle
{ public:
    // Открыть файл на запись
    bool __fastcall OpenFileWrite(char *filename);
    // Открыть файл на чтение
    bool __fastcall OpenFileRead(char *filename);
    // Закрыть файл
    void __fastcall CloseFile(void);

    TMultiTableSaverFile(void):TMultiTableSaverHandle(){};
    virtual ~TMultiTableSaverFile(){CloseFile();};
};
//---------------------------------------------------------------------------
#endif
