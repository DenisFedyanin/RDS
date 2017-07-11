#define RP_Q		3		// Количество параметров связей
#define RP_FTYPE	0		// Тип функции связи
#define RP_K		1		// Коэффициент связи
#define RP_STR		2		// Произвольная функция связи

#define BLACK 		0x00000000

//----------------------------------------------------------------------------------------------------

// Данные класса

RDS_SERVFONTPARAMS font;	// Используемый шрифт
char* name; 				// Название (подпись) узла
int name_size;				// Длина имени узла
int regime,					// Тип динамики кгкарты
	param_id[RP_Q];			// Имена параметров связи
COLORREF default_color,   	// Выбранный пользователем цвет заливки узла
	color;					// Действующий цвет заливки узла
double ival,				// Начальное значение, используемое при расчётах
	ival_setup,				// Начальное значение, вводимое в окне настроек
	lim_minval,				// Выходное значение при недостижении нижнего порога
	cval;					// Текущее значение
int nfunc,					// Тип функции узла
	ndec_cval;				// Число цифр после запятой в текущем значении узла
bool show_cval;				// Отображать текущее значение
TRParams *rparams,			// Массив настроек всех соединённых с узлом связей
	*rparams0;				// Ранее сохранённые настройки связей
int qrp, qrp0;				// Размеры массивов настроек связей
TKgNode mxformstruct;		// Структура, возвращаемая блоку формирования матрицы смежности
TLimit **limits;			// Массив указателей на указатели на параметры порогов узла пороговой сети
int lcount;					// Число порогов узла пороговой сети

//----------------------------------------------------------------------------------------------------

// Присвоение начальных значений

void Constructor(void){
	// Тип динамики
	regime=0;
	// Настройки узла
	name=NULL;
	name_size=0;
	default_color=color=0x00FFFFFF;
	show_cval=true;
	ndec_cval=1;
	nfunc=0;
	ival=ival_setup=0.0;
	lim_minval=0.0;
	// Шрифт
	font.servSize=sizeof(RDS_SERVFONTPARAMS);
    strcpy(font.Name,"Arial");
    font.Height=14;
    font.SizePriority=FALSE;
    font.Color=0;
    font.CharSet=RUSSIAN_CHARSET;
    font.Bold=FALSE;
    font.Italic=FALSE;
    font.Underline=FALSE;
    font.StrikeOut=FALSE;
	// Настройки соединённых с узлом связей
	rparams=rparams0=NULL;
	qrp=qrp0=0;
	// Пороги
	// Создаем одну полосу
	lcount=1;
	limits=new TLimit*[1];
	limits[0]=new TLimit();
	// Структура, возвращаемая блоку формирования матрицы смежности
	mxformstruct.id=NULL;
	mxformstruct.name=NULL;
	mxformstruct.qcons=0;
	mxformstruct.cmass_size=8;
	mxformstruct.connections=NULL;
	// Регистрация функций блока
	if(IamNodeFuncId==0)
		IamNodeFuncId=rdsRegisterFunction("IamKgNode");
	if(RecompFuncId==0)
		RecompFuncId=rdsRegisterFunction("RecompKgNodes");
	if(RegimeFuncId==0)
		RegimeFuncId=rdsRegisterFunction("KgMapRegime");
	if(MxFuncId==0)
		MxFuncId=rdsRegisterFunction("KgMxForm");
	if(MxCallBackFuncId==0)
		MxCallBackFuncId=rdsRegisterFunction("KgMxFormCallBack");
	// Регистрация имён параметров связи
	param_id[RP_FTYPE]=rdsRegisterConnExtParamName("RFuncType");
	param_id[RP_K]=rdsRegisterConnExtParamName("RK");
	param_id[RP_STR]=rdsRegisterConnExtParamName("RCustFunc");
	}

//----------------------------------------------------------------------------------------------------

// Копирование массива символов

char* CopyStr(char* s, int* size, char *cp){
	if(s){
		delete [] s;
		*size=0;
		s=NULL;
		}
	if(!cp){
		s=new char[*size=1];
		s[0]='\0';
		}
	else{
		s=new char[*size=(strlen(cp)+1)];
		strcpy(s,cp);
		}
	return s;
	}

//----------------------------------------------------------------------------------------------------

// Определение количества значащих цифр после запятой
int AfterPoint(double lf){
	// Отбрасывание знака
	if(lf<0.0)
		lf=(-1.0)*lf;
	// Отбрасывание целой части
	lf=lf-floor(lf);	
	// Определение количества значащих цифр после запятой с точностью до тысячной
	int n=3;
	double x, x1, x2;
	for(int i=3; i>0; i--){
		x1=lf*pow(10.0,i-1);
		if(ceil(x1)<(x1+0.5))
			x1=ceil(x1)*10.0;
		else
			x1=floor(x1)*10.0;
		x2=lf*pow(10.0,i);
		if(ceil(x2)<(x2+0.5))
			x2=ceil(x2);
		else
			x2=floor(x2);
		x=x2-x1;
		if(x>-1.0 && x<1.0)
			n--;
		else
			break;
		}
	return n;
	}

//----------------------------------------------------------------------------------------------------
	
// Установка количества порогов
void SetLCount(int n){
TLimit **l;
// Если количество не изменилось
if(n==lcount) 
	return;
// Если количество стало меньше
if(n<lcount){
	// Уничтожение лишних
	for(int i=n; i<lcount; i++)
		delete limits[i];
	// Если требуется стереть все
	if(n<=0){
		lcount=0;
		delete[] limits;
		limits=NULL;
		return;
		}
	lcount=n;
	return;
	}
// Если требуется добавить новые пороги
l=new TLimit*[n];
if(limits)
	memcpy(l,limits,lcount*sizeof(TLimit*));
delete[] limits;
limits=l;
for(int i=lcount;i<n;i++)
	limits[i]=new TLimit();
lcount=n;
}

//----------------------------------------------------------------------------------------------------
	
// Настройки узла

int NSetup(void){
	bool OK;
	int field_id, sl;
	char *fontstr;
	
	// Создание окна с единственной вкладкой
	RDS_HOBJECT window=rdsFORMCreate(TRUE,-1,-1,"Узел когнитивной карты");
	
	#define BASIC_TAB       0
	#define LIMITS_TAB		100

	rdsFORMAddTab(window,BASIC_TAB,"Основное");
	rdsFORMAddTab(window,LIMITS_TAB,"Пороги");

	field_id=BASIC_TAB;
	// Название узла
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_MULTILINE,"Подпись узла",80);
	rdsSetObjectInt(window,field_id,RDS_FORMVAL_MLRETURNS,1);
	rdsSetObjectStr(window,field_id,RDS_FORMVAL_VALUE,name);
	rdsSetObjectInt(window,field_id,RDS_FORMVAL_MLHEIGHT,54);
	// Шрифт
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_FONTSELECT,"Шрифт",75);
	fontstr=rdsStructToFontText(&font,NULL);
	rdsSetObjectStr(window,field_id,RDS_FORMVAL_VALUE,fontstr);
	rdsFree(fontstr);
	// Цвет заливки
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_COLOR,"Цвет заливки",75);
    rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,default_color);
	// Отображать текущее значение
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_CHECKBOX,"Отображать текущее значение",50);
	rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,show_cval);
	// Число цифр после запятой в текущем значении узла
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_EDIT,"Число цифр после запятой",75);
	rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,ndec_cval);
	// Начальное значение
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_EDIT,"Начальное значение",75);
	rdsSetObjectDouble(window,field_id,RDS_FORMVAL_VALUE,ival_setup);
	// Тип функции
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_COMBOLIST,"Тип функции",200);
	rdsSetObjectStr(window,field_id,RDS_FORMVAL_LIST,"Сложение\nУмножение\nИсточник\nПорог");
	rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,nfunc);
	
	field_id=LIMITS_TAB;
	// Выходное значение при недостижении наименьшего порога
	rdsFORMAddEdit(window,LIMITS_TAB,++field_id,RDS_FORMCTRL_EDIT,"Знач. при недостижении наим. порога",75);
	rdsSetObjectDouble(window,field_id,RDS_FORMVAL_VALUE,lim_minval);
	// Таблица пороговых значений
	rdsFORMAddEdit(window,LIMITS_TAB,++field_id,RDS_FORMCTRL_TABLE,"Пороги",0);
	// Столбец значения порога
	rdsFORMTBLSetHeader(window,field_id,0,"Порог",RDS_FORMTBLF_HDRCENTER|RDS_FORMTBLF_RIGHT,-1);
	// Столбец выходного значения
	rdsFORMTBLSetHeader(window,field_id,1,"Вых. знач.",RDS_FORMTBLF_HDRCENTER|RDS_FORMTBLF_RIGHT,-1);
	// Столбец цвета
	rdsFORMTBLSetHeader(window,field_id,2,"Цвет",RDS_FORMTBLF_HDRCENTER|RDS_FORMTBLF_DRAW,60);
	// Начальные значения в ячейках таблицы
	for(int i=0; i<lcount; i++){
		// Порог
		rdsFORMTBLSetInt(window,field_id,i,0,limits[i]->limit);
		// Выходное значение
		rdsFORMTBLSetInt(window,field_id,i,1,limits[i]->output);
		// Цвет
		rdsFORMTBLSetInt(window,field_id,i,2,limits[i]->color);
		}
	// Открытие окна
	OK=rdsFORMShowModalServ(window,NSetupEditsEnable);
	if(OK){
		field_id=BASIC_TAB;
		// Названия узла
		name=CopyStr(name,&name_size,rdsGetObjectStr(window,++field_id,RDS_FORMVAL_VALUE));
		// Шрифт
		rdsFontTextToStruct(rdsGetObjectStr(window,++field_id,RDS_FORMVAL_VALUE),NULL,&font);
		// Цвет заливки
		default_color=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
		// Отображать текущее значение
		show_cval=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
		// Число цифр после запятой в текущем значении узла
		ndec_cval=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
		// Начальное значение
		ival=ival_setup=rdsGetObjectDouble(window,++field_id,RDS_FORMVAL_VALUE);
		// Тип функции
		nfunc=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
		
		field_id=LIMITS_TAB;
		// Пороговое значение
		lim_minval=rdsGetObjectDouble(window,++field_id,RDS_FORMVAL_VALUE);
		// Чтение числа строк в таблице порогов
		int count=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_TBLCOUNT);
		// Создание массива по числу строк таблицы порогов
		SetLCount(count);
		// Запись в элементы массива порогов данных из таблицы
		for(int i=0;i<count;i++){
			limits[i]->limit=rdsFORMTBLGetDouble(window,field_id,i,0,NULL);
			limits[i]->output=rdsFORMTBLGetDouble(window,field_id,i,1,NULL);
			limits[i]->color=(COLORREF)rdsFORMTBLGetInt(window,field_id,i,2,NULL);
			}
		}
		
	// Уничтожение окна настройки
	rdsDeleteObject(window);
	// Возвращаемое значение
	if(OK==true){
			return 1;
			}
	else{
			return 0;
		}
	}

#undef	BASIC_TAB
#undef	LIMITS_TAB
	
//----------------------------------------------------------------------------------------------------

// Настройки ребра

int RSetup(RDS_PCONNSETUPDATA data){
	bool OK;
	int field_id;
	int rfunc;				// Тип функции связи
	char *func_str;			// Строка функции связи
	double rk;				// Вес вершины
	
	// Создание окна с единственной вкладкой
	RDS_HOBJECT window=rdsFORMCreate(FALSE,-1,-1,"Связь когнитивной карты");

	field_id=0;
	// Тип функции
	rdsFORMAddEdit(window,0,++field_id,RDS_FORMCTRL_COMBOLIST,"Тип функции",200);
	rdsSetObjectStr(window,field_id,RDS_FORMVAL_LIST,"Коэффициент\nПроизвольная функция");
	if(rdsGetConnExtParamByIdEx(data->Conn,param_id[RP_FTYPE],&rfunc,NULL,NULL,NULL)!=RDS_VARTYPE_INT)
		rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,RF_K);
	else
		rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,rfunc);
	// Поле ввода коэффициента (веса)
	rdsFORMAddEdit(window,0,++field_id,RDS_FORMCTRL_EDIT,"Коэффициент",75);
	if(rdsGetConnExtParamByIdEx(data->Conn,param_id[RP_K],NULL,&rk,NULL,NULL)!=RDS_VARTYPE_DOUBLE)
		rdsSetObjectDouble(window,field_id,RDS_FORMVAL_VALUE,0.0);
	else
		rdsSetObjectDouble(window,field_id,RDS_FORMVAL_VALUE,rk);
	// Поле ввода текста функции связи
	rdsFORMAddEdit(window,0,++field_id,RDS_FORMCTRL_EDIT,"Произвольная функция",200);
	if(rdsGetConnExtParamByIdEx(data->Conn,param_id[RP_STR],NULL,NULL,&func_str,NULL)!=RDS_VARTYPE_STRING)
		rdsSetObjectStr(window,field_id,RDS_FORMVAL_VALUE,"");
	else
		rdsSetObjectStr(window,field_id,RDS_FORMVAL_VALUE,func_str);
		
	// Открытие окна
	OK=rdsFORMShowModalServ(window,RSetupEditsEnable);
	if(OK){
		field_id=0;
		rfunc=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
		rk=rdsGetObjectDouble(window,++field_id,RDS_FORMVAL_VALUE);
		func_str=rdsGetObjectStr(window,++field_id,RDS_FORMVAL_VALUE);
		rdsSetConnExtParamByIdEx(data->Conn,param_id[RP_FTYPE],RDS_VARTYPE_INT,rfunc,0,0,0);
		rdsSetConnExtParamByIdEx(data->Conn,param_id[RP_K],RDS_VARTYPE_DOUBLE,0,rk,0,0);
		rdsSetConnExtParamByIdEx(data->Conn,param_id[RP_STR],RDS_VARTYPE_STRING,0,0,func_str,0);
		// Подпись связи
		if(rfunc==RF_K){
			char str[32];
			sprintf(str,"%.*lf", AfterPoint(rk), rk);
			rdsSetConnText(data->Conn,RDS_SCTF_TEXT|RDS_SCTF_ALIGNMENT|RDS_SCTF_SHOW,str,0,TRUE,0,0);
			}
		else if(rfunc==RF_CSTM)
			rdsSetConnText(data->Conn,RDS_SCTF_TEXT|RDS_SCTF_ALIGNMENT|RDS_SCTF_SHOW,func_str,0,TRUE,0,0);
		}
		
	// Уничтожение окна настройки
	rdsDeleteObject(window);
	// Возвращаемое значение
	if(OK==true){
			return 1;
			}
	else{
			return 0;
		}
	}

//----------------------------------------------------------------------------------------------------

// Рисование узла

void Draw(RDS_PDRAWDATA DrawData){
	RECT block;
	double zoom;
	int w, h;

	// Границы блока
	block.left=DrawData->Left;
	block.top=DrawData->Top;
	block.right=block.left+DrawData->Width;
	block.bottom=block.top+DrawData->Height;

	// Запрещение рисовать за пределами границ блока
	rdsXGSetClipRect(&block);

	// Масштабный коэффициент
	zoom=DrawData->DoubleZoom;

	// Эллипс
	rdsXGSetBrushStyle(0,RDS_GFS_SOLID,color);
	rdsXGSetPenStyle(0,PS_SOLID,1,BLACK,R2_COPYPEN);
	rdsXGEllipse(block.left, block.top, block.right, block.bottom);

	// Подпись
	if(name && DrawData->Width>0 && DrawData->Height>0){
		char *full_str=NULL;
		// Присоединение текущего значения к названию узла
		if(show_cval){
			char cval_str[64];
			sprintf(cval_str," (%.*lf)", ndec_cval, cval);
			full_str=new char[strlen(name)+strlen(cval_str)+1];
			full_str[0]='\0';
			full_str=strcat(full_str, name);
			full_str=strcat(full_str, cval_str);
			}
		else{
			full_str=new char[strlen(name)+1];
			full_str[0]='\0';
			full_str=strcat(full_str,name);
			}
		// Установка шрифта
		font.SizePriority=FALSE;
		rdsXGSetFontByParStr(&font,zoom);
		rdsXGSetBrushStyle(0,RDS_GFS_EMPTY, color);
		// Подсчёт количества строк и определение самой длинной строки в подписи узла
		int qlines=1, max_line_length=1;
		for(int i=0, ll=1; full_str[i]!='\0'; i++){
			if(full_str[i]=='\n'){
				qlines++;
				ll=1;
				}
			else{
				ll++;
				if(ll>max_line_length){
					max_line_length=ll;
					}
				}
			}

		// Отведение памяти под матрицу параметров строк подписи узла
		TMultyLineParams* wh=new TMultyLineParams[qlines];
		
		// Отведение памяти под массив отдельных строк многострочной подписи
		char **bstr=new char*[qlines];
		for(int i=0; i<qlines; i++){
			bstr[i]=new char[max_line_length+1];
			}

		if(bstr && wh){
			// Определение параметров строк подписи узла , 
			for(int i=0, j=0, k=0; k<qlines && j<max_line_length; i++){
				if(full_str[i]=='\n' || full_str[i]=='\0'){
					bstr[k][j]='\0';
					rdsXGGetTextSize(bstr[k],&wh[k].w,&wh[k].h);
					if(full_str[i]=='\0')
						break;
					j=0;
					k++;
					}
				else if(full_str[i]!='\r' && full_str[i]!='\t'){
					bstr[k][j]=full_str[i];
					j++;
					}
				}
			
			// Расчёт суммарной высоты многострочной подписи
			int sumh=0;
			for(int i=0; i<qlines; i++){
				sumh+=wh[i].h;
				}
			
			// Вывод многострочной подписи узла
			for(int i=0, h; i<qlines; i++){
				h=0;
				for(int j=(qlines-1); j>=i; j--)
					h+=wh[j].h;
				h=2*h-sumh;
				rdsXGTextOut(block.left+(block.right-block.left-wh[i].w)/2, block.top+(block.bottom-block.top-h)/2, bstr[i]);
				}
			}
		
		// Очистка памяти
		if(full_str)
			delete [] full_str;
		if(bstr){
			for(int i=0; i<qlines; i++){
				if(bstr[i]){
					delete [] bstr[i];
					}
				}
			delete [] bstr;
			}
		if(wh)
			delete [] wh;
		}
		
	// Отмена запрета рисования за пределами рамки
	rdsXGSetClipRect(NULL);
	
	#undef BLACK
	}

//----------------------------------------------------------------------------------------------------

// Запись параметров связи в данные класса модели

bool ReadRParams(void){
	int count=0;
	RDS_CHANDLE c;
	RDS_POINTDESCRIPTION point;
	point.servSize=sizeof(point);
	
	// Очистка памяти
	rparams=ClearRParams(rparams, &qrp);
	
	// Подсчёт количества входных связей
	c=NULL;
	for(;;){
		// Очередная связь, соединенная с блоком
		c=rdsGetBlockLink(rdsbcppBlockData->Block,c,TRUE,FALSE,&point);
		// Выход из цикла, если больше нет связей
		if(c==NULL)
			break;
		// Если связь является входной и её название включает "x["
		if(!point.Source && strstr(point.VarName,"x["))
			count++;
		}
	if(count<1)
		return false;
	
	int func_type;			// Тип функции связи
	char *func_str=NULL;	// Строка функции связи
	int str_size;			// Количество символов строкового параметра
	double k;				// Вес вершины
	
	// Выделение памяти под массив настроек
	rparams=new TRParams[count];
	// Заполнение массива настроек
	c=NULL;
	while(qrp<count){
		// Очередная связь, соединенная с блоком
		c=rdsGetBlockLink(rdsbcppBlockData->Block,c,TRUE,FALSE,&point);
		// Выход из цикла, если больше нет связей
		if(c==NULL)
			break;
		// Если связь является выходной или имя её переменной не содержит "x["
		if(point.Source || !strstr(point.VarName,"x["))
			continue;
		// Получение типа функции связи
		if(rdsGetConnExtParamByIdEx(c,param_id[RP_FTYPE],&func_type,NULL,NULL,NULL)!=RDS_VARTYPE_INT)
			continue;
		// Получение текста функции связи
		if(rdsGetConnExtParamByIdEx(c,param_id[RP_K],NULL,&k,NULL,NULL)!=RDS_VARTYPE_DOUBLE)
			continue;
		// Получение текста функции связи
		if(rdsGetConnExtParamByIdEx(c,param_id[RP_STR],NULL,NULL,&func_str,NULL)!=RDS_VARTYPE_STRING)
			continue;
		// Запись параметров в структуру
		rparams[qrp].vname=CopyStr(rparams[qrp].vname,&str_size,point.VarName);
		rparams[qrp].func_type=func_type;
		rparams[qrp].k=k;
		rparams[qrp].func_str=CopyStr(rparams[qrp].func_str,&str_size,func_str);			
		qrp++;
		}
	return true;
	}

//----------------------------------------------------------------------------------------------------

// Освобождение памяти

TRParams* ClearRParams(TRParams* rp, int *n){
	if(rp){
		// Освобождение памяти из под символьных массивов в структуре настроек связей
		for(int i=0; i<*n; i++){
			if(rp[i].vname)
				delete [] rp[i].vname;
			if(rp[i].func_str)
				delete [] rp[i].func_str;
			}
		delete [] rp;
		rp=NULL;
		}
	*n=0;
	return rp;
	}

//----------------------------------------------------------------------------------------------------

// Проверка на совпадение массивов настроек

bool CompareRParams(void){
	// Если число связей различное, то настройки не совпадают
	if(qrp!=qrp0)
		return false;
	// Если число связей нулевое, то настройки совпадают
	if(qrp==0)
		return true;
	// Массив учета уже совпавших настроек
	bool *check=new bool[qrp0];
	for(int i=0; i<qrp0; i++)
		check[i]=false;
	
	// Поэлементное сравенение без учёта порядка элементов
	for(int i=0; i<qrp; i++){
		for(int j=0; j<qrp0; j++){
			if(!check[j] &&
				strcmp(rparams[i].vname,rparams0[j].vname)==0 &&
				rparams[i].func_type==rparams0[j].func_type &&
				rparams[i].k<(rparams0[j].k+0.000001) &&
				rparams[i].k>(rparams0[j].k-0.000001) &&
				strcmp(rparams[i].func_str,rparams0[j].func_str)==0){
				check[j]=true;
				break;
				}
			// Если в массиве старых настроек не содержится настроек для связи с данным идентификатором
			else if(j==(qrp0-1)){
				delete [] check;
				return false;
				}
			}
		}
	// Для каждой связи из массива новых настроек связей найдена копия в массиве старых настроек
	delete [] check;
	return true;
	}

//----------------------------------------------------------------------------------------------------
	
// Копирование массива настроек

void CopyRParams(void){
	int str_size;
	// Выделение памяти под массив настроек
	rparams0=ClearRParams(rparams0, &qrp0);
	if(rparams && qrp>0){
		rparams0=new TRParams[qrp0=qrp];
		for(int i=0; i<qrp; i++){
			rparams0[i].vname=CopyStr(rparams0[i].vname,&str_size,rparams[i].vname);
			rparams0[i].func_type=rparams[i].func_type;
			rparams0[i].k=rparams[i].k;
			rparams0[i].func_str=CopyStr(rparams0[i].func_str,&str_size,rparams[i].func_str);
			}
		}
	}
	
//----------------------------------------------------------------------------------------------------

// Добавление пунктов в меню блока

void ContextPopup(RDS_PCONTEXTPOPUPDATA MenuData){
	rdsAdditionalContextMenuItemEx("Перекомпилировать все узлы",0,0,0);
	rdsAdditionalContextMenuItemEx("Работа карты по абс. значениям",regime==0?RDS_MENU_CHECKED:0,1,0);
	rdsAdditionalContextMenuItemEx("Работа карты по приращениям",regime==1?RDS_MENU_CHECKED:0,1,1);
}

//----------------------------------------------------------------------------------------------------

// Выбор пункта меню блока

void MenuFunction(RDS_PMENUFUNCDATA MenuData){
	switch(MenuData->Function){
		case 0:
			// Вызов функции принудительной перекомпиляции модели
			rdsBroadcastFunctionCallsEx(rdsbcppBlockData->Parent,RecompFuncId,NULL,0);
			break;
		case 1:
			// Заполнение структуры параметров функции
			callparams.Command=MenuData->MenuData;
			callparams.servSize=sizeof(callparams);
			// Вызов функции установки типа динамики (режима)
			rdsBroadcastFunctionCallsEx(rdsbcppBlockData->Parent,RegimeFuncId,&callparams,0);
			break;
		}
}

//----------------------------------------------------------------------------------------------------

// Прочие события

void MdlChange(int CallMode, void *ExtParam, int &Result){
	FILE *in;
	switch(CallMode){
		// Запись имени пункта меню для окна настройки связи
		case RDS_BFM_SUPPORTSCONNSETUP:
			rdsConnSetupMenuName("Задание фунции связи");
			Result=1;
			break;

		// Открытие окна настройки для связи
		 case RDS_BFM_CONNSETUP:
			Result=RSetup((RDS_PCONNSETUPDATA)ExtParam);
			break;

		// Проверка изменений и переписывание собственной модели блока при переходе в режим моделирования
		case RDS_BFM_LEAVINGEDITMODE:
			CheckAndRewriteMdl();
			Result=RDS_BFR_DONE;
			break;
		
		
		case RDS_BFM_FUNCTIONCALL:
			// Приведение ExtParam к правильному типу
			func=(RDS_PFUNCTIONCALLDATA)ExtParam;
			// Проверка идентификатора вызванной функции
			// - Сообщение, что данный блок является узлом когнитивной карты
			if(func->Function==IamNodeFuncId){
				Result=1;
				break;
				}
			// - Принудительная перкомпиляцция модели
			else if(func->Function==RecompFuncId){ 			
				RewriteMdl();
				Result=RDS_BFR_DONE;
				break;
				}
			// - Изменение типа динамики
			else if(func->Function==RegimeFuncId){
				TProgGuideEditCtrlSetParams *params;
				// Придедение указателя на параметры к нужному типу и проверка размера переданной структуры
				params=(TProgGuideEditCtrlSetParams*)(func->Data);
				if(params->servSize<sizeof(TProgGuideEditCtrlSetParams))
					// Размер недостаточен
					break; 
				// Установка типа динамики
				regime=params->Command;
				if(regime<0)
					regime=0;
				else if(regime>1)
					regime=1;
				Result=RDS_BFR_DONE;
				break;
				}
			// - Передача параметров узла для формирования матрицы смежности
			else if(func->Function==MxFuncId){
				// Очистка временной структуры
				if(mxformstruct.name){
					delete [] mxformstruct.name;
					mxformstruct.name=NULL;
					}
				if(mxformstruct.connections){
					delete [] mxformstruct.connections;
					mxformstruct.connections=NULL;
					}
				mxformstruct.id=NULL;
				mxformstruct.qcons=0;
				mxformstruct.cmass_size=8;
				// Функция обратного вызова блока формирования матрицы
				// с передачей ему параметров связей данного узла
				// - Идентификатор узла
				mxformstruct.id=rdsbcppBlockData->Block;
				// - Название узла
				mxformstruct.name=new char[strlen(name)+1];
				mxformstruct.name=strcpy(mxformstruct.name, name);
				// - Количество и описание связей узла
				RDS_CHANDLE c=NULL;
				RDS_CONNDESCRIPTION conn;
				conn.servSize=sizeof(conn);
				RDS_POINTDESCRIPTION point;
				point.servSize=sizeof(point);
				int rfunc;
				double rk;
				TKgCon *buf=NULL;
				int buf_size=0;
				// Цикл по всем связям блока
				for(;;){
					// Очередная связь, соединенная с блоком
					c=rdsGetBlockLink(rdsbcppBlockData->Block,c,TRUE,TRUE,NULL);
					// Выход из цикла, если больше нет связей
					if(c==NULL)
						break;
					// Получение типа функции связи
					if(rdsGetConnExtParamByIdEx(c,param_id[RP_FTYPE],&rfunc,NULL,NULL,NULL)!=RDS_VARTYPE_INT){
						continue;
						}
					// Если тип функции - коэффициент
					if(rfunc==RF_K){
						// Получение коэффициента связи
						if(rdsGetConnExtParamByIdEx(c,param_id[RP_K],NULL,&rk,NULL,NULL)!=RDS_VARTYPE_DOUBLE){
							continue;
							}
						// Отведение памяти, если она ещё не была отведена
						if(!mxformstruct.connections){
							mxformstruct.connections=new TKgCon[mxformstruct.cmass_size];
							}
						// Увеличение размера массива в случае необходимости
						if(mxformstruct.qcons>=mxformstruct.cmass_size){
							// Увеличение размера выделяемой памяти
							buf_size=mxformstruct.cmass_size+8;
							buf=new TKgCon[buf_size];
							// Копирование данных в больший массив
							for(int i=0; i<mxformstruct.cmass_size; i++){
								buf[i].cid=mxformstruct.connections[i].cid;
								buf[i].k=mxformstruct.connections[i].k;
								}
							// Очистка памяти из под старой версии ref
							delete [] mxformstruct.connections;
							// Копирование указателя на больший массив
							mxformstruct.connections=buf;
							mxformstruct.cmass_size=buf_size;
							}
						// Определение и запись идентификатора связанного блока
						mxformstruct.connections[mxformstruct.qcons].cid=NULL;
						rdsGetConnDescription(c, &conn);
						for(int i=0; i<conn.NumPoints; i++){
							rdsGetPointDescription(c, i, &point);
							if(point.PointType==RDS_PTBLOCK){
								if(!(point.Block==mxformstruct.id && point.Source)){
									mxformstruct.connections[mxformstruct.qcons].cid=point.Block;
									break;
									}
								}
							}
						// Запись коэффициента
						mxformstruct.connections[mxformstruct.qcons].k=rk;
						// Увеличение индекса массива связей
						mxformstruct.qcons++;
						}
					else
						continue;
					}					
				// Вызов функции формирования матрицы смежности
				rdsCallBlockFunction(func->Caller, MxCallBackFuncId, &mxformstruct);
				}
		}
	}

//----------------------------------------------------------------------------------------------------

// Проверка изменений и переписывание собственной модели блока

void CheckAndRewriteMdl(void){
	// Чтение текущих параметров связей
	ReadRParams();
	// Сравнение параметров связей
	// - если настройки не изменились, то переписывать модель не нужно
	if(CompareRParams())
		return;
	RewriteMdl();
	}

// Переписывание собственной модели блока	
void RewriteMdl(void){
	// Копирование текущих настроек связей
	CopyRParams();
	
	// 1) Запись полного пути к файлу эталонной модели в динамический массив
	char *rds_include_path=rdsGetSystemPath(RDS_GSPINCLUDEPATH);
	int l=strlen(rds_include_path)+32;
	char *stmdl_path=new char[l];
	stmdl_path[0]='\0';
	stmdl_path=strcat(stmdl_path,rds_include_path);
	stmdl_path=strcat(stmdl_path,"kgmaps\\kgnode.mdl");
	
	// 2) Определение количества символов fsize в файле stmdl_path
	// 3) Динамическое выделение массива char *stmdl_text на fsize символов
	// 4) Запись файла stmdl_path в stmdl_text
	DWORD fsize;
	char *stmdl_text=NULL;
	if(stmdl_path)
		stmdl_text=ReadFileToMemory(stmdl_path,&fsize,1);
	else
		return;
		
	// 5) Создание и открытие для записи нового файла модели блока
	// Определение пути к текущему файлу модели
	char *mdl_path=L57_GetDynModelFileName(rdsbcppBlockData->Block);
	// Открытие для записи текущий файл модели
	FILE *kgnode=NULL;
	kgnode=fopen(mdl_path, "wb");

	#define MARKER "//@ connections functions"
			
	// 6) Поиск начала метки в массиве stmdl_text
	if(kgnode && stmdl_text){
		char* substr=NULL;
		// Поиск адреса подстроки метки в тексте модели
		substr=strstr(stmdl_text,MARKER);
		// Если метка найдена
		if(substr){
			// Индекс первого символа подстроки метки в строке текста модели
			int n=(int)(substr-stmdl_text);
			// Индекс массива не может быть отрицательным
			if(n>=0){
				// 6) Запись всего содержимого массива до метки в новый файл модели
				fwrite(stmdl_text,sizeof(char),n,kgnode);
				// 7) Дописывание в новый файл модели функции связей, вызывая в цикле
				//    функцию rdsGetBlockLink (см. А.5.6.20) для получения
				//    текстового параметра связи и имени связи, которое также
				//    будет использовано при добавлении текста в новый файл модели
				RDS_CHANDLE c=NULL;
				RDS_POINTDESCRIPTION point;
				point.servSize=sizeof(point);
				int index=0;
				// Значения параметров связи
				int rfunc;				// Тип функции связи
				char *func_str=NULL;	// Строка функции связи
				double rk;				// Вес вершины
				char str[32];			// Вспомогательная строка
				for(;;){
					// Очередная связь, соединенная с блоком
					c=rdsGetBlockLink(rdsbcppBlockData->Block,c,TRUE,FALSE,&point);
					// Выход из цикла, если больше нет связей
					if(c==NULL)
						break;
					// Если связь является выходной или имя её переменной не содержит "x["
					if(point.Source || !strstr(point.VarName,"x["))
						continue;
					// Получение типа функции связи
					if(rdsGetConnExtParamByIdEx(c,param_id[RP_FTYPE],&rfunc,NULL,NULL,NULL)!=RDS_VARTYPE_INT){
						rfunc=RF_K;
						rdsSetConnExtParamByIdEx(c,param_id[RP_FTYPE],RDS_VARTYPE_INT,rfunc,0,0,0);
						//continue;
						}
					// Если тип функции - коэффициент
					if(rfunc==RF_K){
						// Получение коэффициента связи
						if(rdsGetConnExtParamByIdEx(c,param_id[RP_K],NULL,&rk,NULL,NULL)!=RDS_VARTYPE_DOUBLE){
							rk=1.0;
							rdsSetConnExtParamByIdEx(c,param_id[RP_K],RDS_VARTYPE_DOUBLE,0,rk,0,0);
							//continue;
							}
						// Формирование текста функции связи вида
						// #define y имя_переменной_n
						// z[значение_index]=y*значение_rk;
						// #undef y
						
						// Для режима работы по абсолютным значениям
						fprintf(kgnode,"\r\n\tif(regime==0){\r\n");
						fprintf(kgnode,"\t\t#define y %s\r\n",point.VarName);
						fprintf(kgnode,"\t\tz[%d]=%lf*y;\r\n", index, rk);
						fprintf(kgnode,"\t\t#undef y\r\n");
						fprintf(kgnode,"\t\t}");
						// Для режима работы по приращениям
						fprintf(kgnode,"\r\n\telse{\r\n");
						fprintf(kgnode,"\t\t#define y (%s-x0[%d])\r\n", point.VarName, index);
						fprintf(kgnode,"\t\tz[%d]=%lf*y;\r\n", index, rk);
						fprintf(kgnode,"\t\tx0[%d]=%s;\r\n", index, point.VarName);
						fprintf(kgnode,"\t\t#undef y\r\n");
						fprintf(kgnode,"\t\t}\r\n");
						index++;
						}
					else{
						// Получение текста функции связи
						if(rdsGetConnExtParamByIdEx(c,param_id[RP_STR],NULL,NULL,&func_str,NULL)!=RDS_VARTYPE_STRING){
							sprintf(func_str,"y");
							rdsSetConnExtParamByIdEx(c,param_id[RP_STR],RDS_VARTYPE_STRING,0,0,func_str,0);
							//continue;
							}
						// Формирование текста функции связи вида
						// #define y имя_переменной_n
						// z[значение_index]=текст_функции_n;
						// #undef y
						// Для режима работы по абсолютным значениям
						fprintf(kgnode,"\r\n\tif(regime==0){\r\n");
						fprintf(kgnode,"\t\t#define y %s\r\n",point.VarName);
						if(!func_str || strcmp(func_str,"\0")==0)
							fprintf(kgnode,"\t\tz[%d]=y;\r\n", index);
						else
							fprintf(kgnode,"\t\tz[%d]=%s;\r\n", index, func_str);
						fprintf(kgnode,"\t\t#undef y\r\n");
						fprintf(kgnode,"\t\t}");
						// Для режима работы по приращениям
						fprintf(kgnode,"\r\n\telse{\r\n");
						fprintf(kgnode,"\t\t#define y (%s-x0[%d])\r\n", point.VarName, index);
						if(!func_str || strcmp(func_str,"\0")==0)
							fprintf(kgnode,"\t\tz[%d]=y;\r\n", index);
						else
							fprintf(kgnode,"\t\tz[%d]=%s;\r\n", index, func_str);
						fprintf(kgnode,"\t\tx0[%d]=%s;\r\n", index, point.VarName);
						fprintf(kgnode,"\t\t#undef y\r\n");
						fprintf(kgnode,"\t\t}\r\n");
						index++;
						}
					// Подпись связи
					if(rfunc==RF_K){
						sprintf(str,"%.*lf", AfterPoint(rk), rk);
						rdsSetConnText(c,RDS_SCTF_TEXT|RDS_SCTF_ALIGNMENT|RDS_SCTF_SHOW,str,0,TRUE,0,0);
						}
					else if(rfunc==RF_CSTM)
						rdsSetConnText(c,RDS_SCTF_TEXT|RDS_SCTF_ALIGNMENT|RDS_SCTF_SHOW,func_str,0,TRUE,0,0);
					}
				// 8) Дописывание в файл остальное содержимое после метки
				fprintf(kgnode,stmdl_text+n+strlen(MARKER));
				// Указание на перекомпилирование модели
				rdscompCompileBlockModelDelayed(rdsbcppBlockData->Block,FALSE);
				}
			}
		// Если метка не найдена
		else{
			// Копирование всего содержимого эталонной модели
			fwrite(stmdl_text,sizeof(char),fsize,kgnode);
			// Указание на перекомпилирование модели
			rdscompCompileBlockModelDelayed(rdsbcppBlockData->Block,FALSE);
			}
		fclose(kgnode);
		}
	
	#undef MARKER

	// 9) Освобождение памяти
	delete [] stmdl_path;
	if(mdl_path)
		rdsFree(mdl_path);
	if(stmdl_text)
		rdsFree(stmdl_text);
	}
		
//----------------------------------------------------------------------------------------------------

// Расчёт текущего значения узла

double Calc(void){
	double y0, maxlim;
	// В случае, когда функцией узла является Порог, цвет заливки может меняться при расчёте
	color=default_color;
	// При первом запуске записывается исходное значение
	if(new_set){
		new_set=false;
		y=ival;
		}
	else{
		// Функции узла
		switch(nfunc){
			// Сложение
			case NF_SUM:
				if(regime==0)
					y=0.0;
				for(int i=0; i<z.Size(); i++)
					y+=z[i];
				break;
			// Умножение
			case NF_MUL:
				if(regime==0)
					y=1.0;
				for(int i=0; i<z.Size(); i++)
					y=y*z[i];
				break;
			// Источник
			case NF_SRC:
				y=ival;
				break;
			// Порог
			case NF_LIM:
				y0=0.0;
				for(int i=0; i<z.Size(); i++)
					y0+=z[i];
				y=lim_minval;
				if(limits && limits[0]){
					maxlim=limits[0]->limit;
					for(int i=0; i<lcount; i++){
						if(limits[i]){
							if(maxlim<=limits[i]->limit && y0>limits[i]->limit){
								y=limits[i]->output;
								maxlim=limits[i]->limit;
								color=limits[i]->color;
								}
							}
						}
					}
				break;
			default:
				y=0.0;
				break;
			}
		}
	cval=y;
	return y;
	}

//----------------------------------------------------------------------------------------------------

// Сброс данных модели

void Reset(void){
	new_set=true;
	}

//----------------------------------------------------------------------------------------------------

// Очистка памяти

void Clear(void){
if(name) delete [] name;
rparams=ClearRParams(rparams, &qrp);
rparams0=ClearRParams(rparams0, &qrp0);
// Очистка временной структуры
if(mxformstruct.name){
	delete [] mxformstruct.name;
	mxformstruct.name=NULL;
	}
if(mxformstruct.connections){
	delete [] mxformstruct.connections;
	mxformstruct.connections=NULL;
	}
// Очистка массива параметров порогов
SetLCount(0);
}

//----------------------------------------------------------------------------------------------------

// Сохранение параметров

void Save(void){
	char *fontstr, str[32];
	// Тип динамики
	rdsWriteWordValueText("Regime",regime);
	// Настройки узла
	rdsWriteWordStringText("NodeName",name);
	fontstr=rdsStructToFontText(&font,NULL);
	rdsWriteWordStringText("Font",fontstr);
	rdsFree(fontstr);
	rdsWriteWordValueText("NodeColor",default_color);
	rdsWriteWordValueText("NodeShowCVal",show_cval);
	rdsWriteWordValueText("NodeNDecCVal",ndec_cval);
	rdsWriteWordDoubleText("NodeIVal",ival_setup);
	rdsWriteWordValueText("NodeFunc",nfunc);
	rdsWriteWordDoubleText("LimMinVal",lim_minval);
	// Настройки связей
	rdsWriteWordValueText("QRP",qrp0);
	for(int i=0; i<qrp0; i++){
		sprintf(str, "RVName%d", i);
		rdsWriteWordStringText(str,rparams0[i].vname);
		sprintf(str, "RFuncType%d", i);
		rdsWriteWordValueText(str,rparams0[i].func_type);
		sprintf(str, "RK%d", i);
		rdsWriteWordDoubleText(str,rparams0[i].k);
		sprintf(str, "RFuncStr%d", i);
		rdsWriteWordStringText(str,rparams0[i].func_str);
		}
	// Пороги
	rdsWriteWordValueText("LimCount",lcount);
    for(int i=0; i<lcount; i++){
		rdsWriteBlockDataText("LimItem",TRUE);
		rdsWriteWordDoubleText("LimVal",limits[i]->limit);
        rdsWriteWordDoubleText("LimOutput",limits[i]->output);
		rdsWriteWordValueText("LimColor",limits[i]->color);
		}
	}

//----------------------------------------------------------------------------------------------------

// Загрузка параметров

void Load(char *text){
	bool flag;
	char *pointer, *word, c, str[32];
	int current_lcount=0;
	pointer=text;
	rparams0=ClearRParams(rparams0, &qrp0);
	for(;;){
		flag=false;
		// Получить из текста очередное слово
		word=rdsGetTextWord(pointer,&pointer,&c,FALSE);
		// Текст закончился - выход из цикла
		if(c==0)
			break;
		// Переход на новую строку - продолжение счтитывания
		if(c=='\n')
			continue;
		// Считывание типа динамики
		if(!flag){
			if(strcmp(word,"Regime")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				regime=atoi(word);
				flag=true;
				}
			}
		// Считывание названия узла
		if(!flag){
			if(strcmp(word,"NodeName")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				name=CopyStr(name,&name_size,word);
				flag=true;
				}
			}
		// Считывание шрифта
		if(!flag){
			if(strcmp(word,"Font")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				rdsFontTextToStruct(word,NULL,&font);
				flag=true;
				}
			}
		// Считывание цвета
		if(!flag){
			if(strcmp(word,"NodeColor")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				default_color=atoi(word);
				flag=true;
				}
			}
		// Отображать текущее значение
		if(!flag){
			if(strcmp(word,"NodeShowCVal")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				show_cval=atoi(word);
				flag=true;
				}
			}
		// Число цифр после запятой в текущем значении узла
		if(!flag){
			if(strcmp(word,"NodeNDecCVal")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				ndec_cval=atoi(word);
				flag=true;
				}
			}
		// Считывание исходного значения
		if(!flag){
			if(strcmp(word,"NodeIVal")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				ival=ival_setup=atof(word);
				flag=true;
				}
			}
		// Считывание типа функции узла
		if(!flag){
			if(strcmp(word,"NodeFunc")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				nfunc=atoi(word);
				flag=true;
				}
			}
		// Считывание выходного значения при недостижении наименьшего порога
		if(!flag){
			if(strcmp(word,"LimMinVal")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				lim_minval=atof(word);
				flag=true;
				}
			}
		// Настройки связей
		if(!flag && !rparams0){
			if(strcmp(word,"QRP")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				qrp0=atoi(word);
				rparams0=new TRParams[qrp0];
				flag=true;
				}
			}
		if(!flag && rparams0){
			for(int i=0; i<qrp0; i++){
				sprintf(str,"RVName%d",i);
				if(strcmp(word,str)==0){
					word=rdsGetTextWord(pointer,&pointer,0,false);
					rparams0[i].vname=CopyStr(rparams0[i].vname,&name_size,word);
					flag=true;
					}
				}
			}
		if(!flag && rparams0){
			for(int i=0; i<qrp0; i++){
				sprintf(str,"RFuncType%d",i);
				if(strcmp(word,str)==0){
					word=rdsGetTextWord(pointer,&pointer,0,false);
					rparams0[i].func_type=atoi(word);
					flag=true;
					}
				}
			}
		if(!flag && rparams0){
			for(int i=0; i<qrp0; i++){
				sprintf(str,"RK%d",i);
				if(strcmp(word,str)==0){
					word=rdsGetTextWord(pointer,&pointer,0,false);
					rparams0[i].k=atof(word);
					flag=true;
					}
				}
			}
		if(!flag && rparams0){
			for(int i=0; i<qrp0; i++){
				sprintf(str,"RFuncStr%d",i);
				if(strcmp(word,str)==0){
					word=rdsGetTextWord(pointer,&pointer,0,false);
					rparams0[i].func_str=CopyStr(rparams0[i].func_str,&name_size,word);
					flag=true;
					}
				}
			}
		// Число порогов
		if(!flag){
			if(strcmp(word,"LimCount")==0){
				int count;
				word=rdsGetTextWord(pointer,&pointer,0,false);
				count=atoi(word);
				if(count<=0)
					count=1;
				SetLCount(count);
				flag=true;
				}
			}
		// Наличие порога
		if(!flag){
			if(strcmp(word,"LimItem")==0){
				current_lcount++;
				if(current_lcount>lcount)
					SetLCount(current_lcount);
				flag=true;
				}
			}
		// Значение порога
		if(!flag){
			if(strcmp(word,"LimVal")==0){
				word=rdsGetTextWord(pointer,&pointer,NULL,FALSE);
				if(current_lcount>=0)
					limits[current_lcount-1]->limit=atof(word);
				flag=true;
				}
			}
		// Выходное значение для порога
        if(!flag){
			if(strcmp(word,"LimOutput")==0){
				word=rdsGetTextWord(pointer,&pointer,NULL,FALSE);
				if(current_lcount>=0)
					limits[current_lcount-1]->output=atof(word);
				flag=true;
				}
			}
		// Цвет для порога
		if(!flag){
			if(strcmp(word,"LimColor")==0){
				word=rdsGetTextWord(pointer,&pointer,NULL,FALSE);
				if(current_lcount>=0)
					limits[current_lcount-1]->color=(COLORREF)atoi(word);
				flag=true;
				}
			}
		}
	if(current_lcount<=0)
		current_lcount=1;
	SetLCount(current_lcount);
	}

//----------------------------------------------------------------------------------------------------

#undef	NF_SUM
#undef	NF_MUL
#undef	NF_SRC

#undef	RF_K
#undef	RF_CSTM

#undef 	RP_Q
#undef 	RP_FTYPE
#undef 	RP_K
#undef 	RP_STR

#undef BLACK