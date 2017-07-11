#include    		<stdio.h>

#define	NF_SUM		0		// Тип функции узла - суммирование
#define	NF_MUL		1		// Тип функции узла - умножение
#define NF_SRC		2		// Тип функции узла - источник
#define NF_LIM		3		// Тип функции узла - порог

#define	TINYVAL		1e-12   // Наименьшее значение, не принимаемое за ноль

//----------------------------------------------------------------------------------------------------

// Идентификатор функции сообщения, что блок является узлом кгкарты
int IamNodeFuncId=0;
// Идентификатор функции принудительной перекомпиляции модели
int RecompFuncId=0;
// Идентификатор функции установки типа динамики (режима)
int RegimeFuncId=0;
// Идентификатор функции формирования матрицы смежности
int MxFuncId=0;
// Идентификатор функции обратного вызова при формировании матрицы смежности
int MxCallBackFuncId=0;
// Описание вызванной функции блока
RDS_PFUNCTIONCALLDATA func;
// Описание выбранного пункта меню
RDS_PMENUFUNCDATA menu;
// Параметры вызванной функции блока
typedef struct{
	DWORD servSize;
	// Размер этой структуры для проверки
	int Command;
	// Команда блоку (0, 1 или 2)
	} TProgGuideEditCtrlSetParams;
TProgGuideEditCtrlSetParams callparams;

// Свзь с коэффициентом k данного узла с узлом, имеющим идентификатор id 
struct TKgCon{
	RDS_BHANDLE cid;
	double k;
	};
// Структура, возвращаемая блоку формирования матрицы смежности
struct TKgNode{
	RDS_BHANDLE id;
	char* name;
	int qcons;
	int cmass_size;
	TKgCon *connections;
	};

// Параметры порога
class TLimit{
	public:
		double limit;	// Пороговое значение
		double output;	// Выходное значение при достижении порога
		COLORREF color; // Цвет, который принимает узел при достижении порога
		TLimit(void){
			limit=0.0;
			output=0.0;
			color=0xffffff;
			};
		~TLimit(void){};
	};

//----------------------------------------------------------------------------------------------------

// Ширина и высота строки многострочной подписи
struct TMultyLineParams{
	int w;
	int h;
	};

// Настройки связи
class TRParams{
	public:
		char* vname;	
		int func_type;
		double k;
		char* func_str;
		TRParams(void){vname=func_str=NULL;}
		~TRParams(void){}
	};

//---------------------------------------------------------------------------

// Функция обратного вызова для CheckInputSource
BOOL RDSCALL CheckInputSourceCB(RDS_PPOINTDESCRIPTION /*thispoint*/, RDS_PPOINTDESCRIPTION thatpoint, LPVOID /*data*/){
	if(!thatpoint->Source) // Не выход
		return TRUE;
	// Выход какого-то блока
	return FALSE;
	}
	
//---------------------------------------------------------------------------

// Проверить наличие источника данных у входа блока
// block - блок
// inputnum - номер входа
// pConnBlock - указатель для возврата блока на другом конце найденной связи.
// Возврат - есть хотя бы один источник
BOOL CheckInputSource(RDS_BHANDLE block,int inputnum,RDS_BHANDLE *pConnBlock){
	RDS_BHANDLE
	b=rdsEnumConnectedBlocksByVar(block,inputnum,RDS_BEN_TRACELINKS,CheckInputSourceCB,NULL);
	if(pConnBlock)
		*pConnBlock=b;
	return b!=0;
	}

//----------------------------------------------------------------------------------------------------

// Получение полного пути к файлу модели блока
char * L57_GetDynModelFileName(RDS_BHANDLE block){
	RDS_PCOMPMODELDATA model;
  	if(!block)
    	return NULL;
  	model=rdscompGetBlockModelData(block);
  	if(!model)
    	return NULL;
  	// У блока есть модель
  	return rdsGetFullFilePath(model->ModelName,NULL,NULL);
	}

//----------------------------------------------------------------------------------------------------

// Читать из файла заданное число байтов с обходом ошибки (служебная)

#define Windows_ChunkSizeMax (1 << 22) // Читать/писать не больше этого

int _WindowsRead(HANDLE Handle,void *data, size_t *size){
	size_t originalSize = *size;
  	if (originalSize == 0)
    	return 1;

  	if(Handle==INVALID_HANDLE_VALUE)
    	return 0;

  	*size = 0;
  	do{
    	DWORD curSize = (originalSize > Windows_ChunkSizeMax) ? Windows_ChunkSizeMax : (DWORD)originalSize;
   		DWORD processed = 0;
    	BOOL res = ReadFile(Handle, data, curSize, &processed, NULL);
    	data = (void *)((BYTE*)data + processed);
    	originalSize -= processed;
    	*size += processed;
    	if (!res)
      		return 0;
    	if (processed == 0)
      		break;
  		}
  	while (originalSize > 0);
  	return 1;
	}

#undef Windows_ChunkSizeMax

//----------------------------------------------------------------------------------------------------

// Получить размер файла (короткий)
// psize - возвращаемый размер
// pTooLong - возвр.1 если файл слишком длинный
// возврат - успешность
int WindowsGetFileSizeShort(HANDLE Handle,DWORD *psize,int *pTooLong=NULL){
	DWORD sizeHigh,sizeLow;
  	if(Handle==INVALID_HANDLE_VALUE)
    	return 0;
  	sizeLow = GetFileSize(Handle,&sizeHigh);
  	if(pTooLong) *pTooLong=0;
  	if (sizeLow == 0xFFFFFFFF){
		DWORD res = GetLastError();
      	if(res!=NO_ERROR)
        	return 0;
      	if(sizeHigh){
			if(pTooLong)
				*pTooLong=1;
          		return 0;
        		}
     		}
  	*psize=sizeLow;
  	return 1;
	}

//----------------------------------------------------------------------------------------------------

// Считать короткий файл в динамически отведенный буфер (освобождать rdsFree)
// path       - путь
// psize      - возвращаемый размер
// addzerobyte - добавлять или нет нулевой байт в конец
char * ReadFileToMemory(char *path,DWORD *psize,int addzerobyte){
	DWORD size;
  	size_t act_size;
  	char *mainbuf;
  	BOOL ok;
  	HANDLE Handle;

  	if(psize) *psize=0;
  	Handle=CreateFile(path,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
  	if(Handle==INVALID_HANDLE_VALUE)
    	return NULL;

  	if(!WindowsGetFileSizeShort(Handle,&size)){
		CloseHandle(Handle);
      	return NULL;
    	}

  	mainbuf=(char*)rdsAllocate(addzerobyte?(size+1):size);

  	// Читаем файл
  	act_size=size;
  	ok=_WindowsRead(Handle,mainbuf,&act_size);
  	CloseHandle(Handle);
  	if(ok)
    	ok=(act_size==size);
  	if(!ok){
		rdsFree(mainbuf);
      	return NULL;
    	}
  	if(psize)
		*psize=size;
  	if(addzerobyte)
    	mainbuf[size]=0;
  	return mainbuf;
	}

//----------------------------------------------------------------------------------------------------

#define BASIC_TAB       0
#define LIMITS_TAB		100
	
// Функция ввода араметров порога в отдельном окне
BOOL EditWin(char *caption, double *plimit, double *poutput, COLORREF *pcolor){
	BOOL res;
	// Создание окна
	RDS_HOBJECT window=rdsFORMCreate(FALSE,-1,-1,caption);
	// Поле ввода порога
	rdsFORMAddEdit(window,0,0,RDS_FORMCTRL_EDIT,"Порог:",80);
	rdsSetObjectDouble(window,0,RDS_FORMVAL_VALUE,*plimit);
	// Поле ввода выходного значения
	rdsFORMAddEdit(window,0,1,RDS_FORMCTRL_EDIT,"Выходное значение:",80);
	rdsSetObjectDouble(window,1,RDS_FORMVAL_VALUE,*poutput);
	// Поле ввода цвета
	rdsFORMAddEdit(window,0,2,RDS_FORMCTRL_COLOR,"Цвет:",80);
	rdsSetObjectInt(window,2,RDS_FORMVAL_VALUE,*pcolor);
	// Открытие окна (функция обратного вызова не используется)
	res=rdsFORMShowModalServ(window,NULL);
	if(res){ // Нажата OK
		*plimit=rdsGetObjectDouble(window,0,RDS_FORMVAL_VALUE);
		*poutput=rdsGetObjectDouble(window,1,RDS_FORMVAL_VALUE);
		*pcolor=(COLORREF)rdsGetObjectInt(window,2,RDS_FORMVAL_VALUE);
		}
	// Уничтожение окна
	rdsDeleteObject(window);
	return res;
	}

// Разрешение отдельных полей окна настройки узла
void RDSCALL NSetupEditsEnable(RDS_HOBJECT window, RDS_PFORMSERVFUNCDATA data){
	bool flag;
	int field_id=BASIC_TAB;
	// Название узла
	field_id++;
	// Шрифт
	field_id++;
	// Цвет заливки
	field_id++;
	// Отображать текущее значение
	flag=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
	// Число цифр после запятой в текущем значении узла
	rdsSetObjectInt(window,++field_id,RDS_FORMVAL_ENABLED, flag);
	// Начальное значение
	field_id++;
	// Тип функции
	flag=(rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE)==NF_LIM)?1:0;
	
	field_id=LIMITS_TAB;
	// Значение наименьшего порога
	rdsSetObjectInt(window,++field_id,RDS_FORMVAL_ENABLED, flag);
	// Таблица пороговых значений
	//rdsSetObjectInt(window,++field_id,RDS_FORMVAL_ENABLED, flag);
	field_id++;
	int count;
	double l, output;
	COLORREF c;
	switch(data->Event){
		// Программное рисование ячейки цвета
		case RDS_FORMSERVEVENT_DRAW:
			// Если событие вывано для ячейки цвета таблицы порогов
			if(data->CtrlId==field_id && data->ColId==2){
				int w=30, h=12; // Размеры цветного прямоугольника
				int x=data->Left+(data->Width-w)/2,
				y=data->Top+(data->Height-h)/2;
				// Рисование прямоугольника
				c=(COLORREF)atoi(data->CellValue);
				rdsXGSetPenStyle(0,PS_SOLID,1,data->FGColor,R2_COPYPEN);
				rdsXGSetBrushStyle(0,RDS_GFS_SOLID,c);
				rdsXGRectangle(x,y,x+w,y+h);
				}
			break;
			
		// Нажатие одной из кнопок управления таблицы
		case RDS_FORMSERVEVENT_TBLBUTTON:
			if(data->CtrlId==field_id)
				switch(data->Button){
					// Кнопка добавления строки
					case RDS_FORMVAL_TBLBTN_ADD:
						// Начальные значения для добавляемой строки
						c=0xffffff; // Белый цвет
						l=0.0;		// Пороговое значение
						output=0.0; 	// Выходное значение
						// Число строк в таблице
						count=rdsGetObjectInt(window,data->CtrlId,RDS_FORMVAL_TBLCOUNT);
						// Открытие окна для ввода цвета и значения
						if(EditWin("Добавление",&l,&output,&c)){
							// Добавление - это установка строки count
							rdsFORMTBLSetDouble(window,data->CtrlId,count,0,l);
							rdsFORMTBLSetDouble(window,data->CtrlId,count,1,output);
							rdsFORMTBLSetInt(window,data->CtrlId,count,2,c);
							data->IndexAfterButton=count;
							}
					break;
				// Кнопка редактирования строки
				case RDS_FORMVAL_TBLBTN_PROP:
					// Считывание данных текущей строки
					l=rdsFORMTBLGetDouble(window,data->CtrlId,data->TableRow,0,NULL);
					output=rdsFORMTBLGetDouble(window,data->CtrlId,data->TableRow,1,NULL);
					c=(COLORREF)rdsFORMTBLGetInt(window,data->CtrlId,data->TableRow,2,NULL);
					// Открытие окна для ввода цвета и значения
					if(EditWin("Редактирование",&l, &output, &c)){
						// Нажата OK - запись новых значений в таблицу
						rdsFORMTBLSetDouble(window,data->CtrlId,data->TableRow,0,l);
						rdsFORMTBLSetDouble(window,data->CtrlId,data->TableRow,1,output);
						rdsFORMTBLSetInt(window,data->CtrlId,data->TableRow,2,c);
						}
					break;
				}
			break;
			
	// Управление разрешенностью кнопок
	case RDS_FORMSERVEVENT_TBLBTNENABLE:
		if(data->CtrlId==field_id)
			switch(data->Button){
				// Кнопка удаления
				case RDS_FORMVAL_TBLBTN_DEL:
					// Если в таблице осталась только одна строка, то запрет удаления
					if(rdsGetObjectInt(window,data->CtrlId,RDS_FORMVAL_TBLCOUNT)<2)
						data->ButtonResult=RDS_FORMSERVRESULT_NO;
					break;
				}
		break;
		}
	}

#undef	BASIC_TAB
#undef	LIMITS_TAB

//----------------------------------------------------------------------------------------------------

#define	RF_K		0		// Тип функции связи - коэффициент
#define	RF_CSTM		1		// Тип функции связи - произволная

// Разрешение отдельных полей окна настройки связи
void RDSCALL RSetupEditsEnable(RDS_HOBJECT window, RDS_PFORMSERVFUNCDATA /*data*/){
	int id, flag;
	id=0;
	flag=rdsGetObjectInt(window,++id,RDS_FORMVAL_VALUE);
	rdsSetObjectInt(window,++id,RDS_FORMVAL_ENABLED, flag==RF_K);
	rdsSetObjectInt(window,++id,RDS_FORMVAL_ENABLED, flag==RF_CSTM);
	}

//----------------------------------------------------------------------------------------------------
