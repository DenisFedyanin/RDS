//---------------------------------------------------------------------------

#ifndef FileLibraryH
#define FileLibraryH
#include <windows.h>
//---------------------------------------------------------------------------
// Библиотека каких-то данных в файле
//---------------------------------------------------------------------------
// Пара DWORD для указателя файла
struct TFileLibraryQWord
{ DWORD High,Low; };
//---------------------------------------------------------------------------
// Элемент библиотеки
class TFileLibraryItem
{ public:
    TFileLibraryItem *Next;     // Следующий элемент
    int Id;                     // Тип (целый идентификатор)
    TFileLibraryQWord Start;    // Положение в файле
    TFileLibraryQWord Length;   // Длина

    // Вычитание двух двухDWORDовых чисел
    static bool __fastcall SubtractQWORDS(DWORD FromHigh,DWORD FromLow,
                                          DWORD SubHigh,DWORD SubLow,
                                          DWORD &ResHigh,DWORD &ResLow);

    TFileLibraryItem(void){Next=NULL;};
};
//---------------------------------------------------------------------------
// Библиотека
class TFileLibrary
{ private:
    bool Waiting;    // Ожидание вызова функции After...

    // Считать оглавление открытого файла
    bool __fastcall ReadIndex(void);
  public:
    TFileLibraryItem *First; // Первый элемент
    HANDLE File;             // HANDLE открытого файла

    // Очистить все
    void __fastcall Clear(void);

    // Добавить элемент
    TFileLibraryItem * __fastcall Add(void);

    // Вспомогательные - запись и чтение данных
    static bool __fastcall WindowsWrite(HANDLE h,const void *data,size_t *size);
    static bool __fastcall WindowsRead(HANDLE h, void *data, size_t *size);
    bool __fastcall Write(void *buf,DWORD count);
    bool __fastcall Read(void *buf,DWORD count);

    // Открыть файл на запись
    bool __fastcall OpenFileWrite(char *filename,char *prefix);
    // Открыть файл на чтение и считать индекс
    bool __fastcall OpenFileRead(char *filename,char *prefix,bool *pBadFormat=NULL);
    // Закрыть файл (при чтении, при записи - использовать SaveIndexAndCloseFile)
    void __fastcall CloseFile(void);
    // Записать оглавление и закрыть файл
    bool __fastcall SaveIndexAndCloseFile(void);

    // Получить текущее положение открытого файла
    bool __fastcall GetFilePos(TFileLibraryQWord &pos);
    // Установить положение открытого файла
    bool __fastcall SetFilePos(const TFileLibraryQWord &pos);

    // Вызов перед началом записи очередного куска
    bool __fastcall BeforeWrite(int id);
    // Вызов после записи очередного куска
    bool __fastcall AfterWrite(void);

    // Найти первый попавшийся кусок по идентификатору
    TFileLibraryItem * __fastcall FindItem(int id);

    // Позиционирование для чтения куска
    TFileLibraryItem * __fastcall PrepareToRead(int id,bool *pNotFound=NULL);

    TFileLibrary(void){First=NULL;File=INVALID_HANDLE_VALUE;Waiting=false;};
    ~TFileLibrary(){Clear();CloseFile();};
};
//---------------------------------------------------------------------------
#endif
