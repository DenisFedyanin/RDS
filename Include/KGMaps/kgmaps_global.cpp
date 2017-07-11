#include    		<stdio.h>

#define	NF_SUM		0		// ��� ������� ���� - ������������
#define	NF_MUL		1		// ��� ������� ���� - ���������
#define NF_SRC		2		// ��� ������� ���� - ��������
#define NF_LIM		3		// ��� ������� ���� - �����

#define	TINYVAL		1e-12   // ���������� ��������, �� ����������� �� ����

//----------------------------------------------------------------------------------------------------

// ������������� ������� ���������, ��� ���� �������� ����� �������
int IamNodeFuncId=0;
// ������������� ������� �������������� �������������� ������
int RecompFuncId=0;
// ������������� ������� ��������� ���� �������� (������)
int RegimeFuncId=0;
// ������������� ������� ������������ ������� ���������
int MxFuncId=0;
// ������������� ������� ��������� ������ ��� ������������ ������� ���������
int MxCallBackFuncId=0;
// �������� ��������� ������� �����
RDS_PFUNCTIONCALLDATA func;
// �������� ���������� ������ ����
RDS_PMENUFUNCDATA menu;
// ��������� ��������� ������� �����
typedef struct{
	DWORD servSize;
	// ������ ���� ��������� ��� ��������
	int Command;
	// ������� ����� (0, 1 ��� 2)
	} TProgGuideEditCtrlSetParams;
TProgGuideEditCtrlSetParams callparams;

// ���� � ������������� k ������� ���� � �����, ������� ������������� id 
struct TKgCon{
	RDS_BHANDLE cid;
	double k;
	};
// ���������, ������������ ����� ������������ ������� ���������
struct TKgNode{
	RDS_BHANDLE id;
	char* name;
	int qcons;
	int cmass_size;
	TKgCon *connections;
	};

// ��������� ������
class TLimit{
	public:
		double limit;	// ��������� ��������
		double output;	// �������� �������� ��� ���������� ������
		COLORREF color; // ����, ������� ��������� ���� ��� ���������� ������
		TLimit(void){
			limit=0.0;
			output=0.0;
			color=0xffffff;
			};
		~TLimit(void){};
	};

//----------------------------------------------------------------------------------------------------

// ������ � ������ ������ ������������� �������
struct TMultyLineParams{
	int w;
	int h;
	};

// ��������� �����
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

// ������� ��������� ������ ��� CheckInputSource
BOOL RDSCALL CheckInputSourceCB(RDS_PPOINTDESCRIPTION /*thispoint*/, RDS_PPOINTDESCRIPTION thatpoint, LPVOID /*data*/){
	if(!thatpoint->Source) // �� �����
		return TRUE;
	// ����� ������-�� �����
	return FALSE;
	}
	
//---------------------------------------------------------------------------

// ��������� ������� ��������� ������ � ����� �����
// block - ����
// inputnum - ����� �����
// pConnBlock - ��������� ��� �������� ����� �� ������ ����� ��������� �����.
// ������� - ���� ���� �� ���� ��������
BOOL CheckInputSource(RDS_BHANDLE block,int inputnum,RDS_BHANDLE *pConnBlock){
	RDS_BHANDLE
	b=rdsEnumConnectedBlocksByVar(block,inputnum,RDS_BEN_TRACELINKS,CheckInputSourceCB,NULL);
	if(pConnBlock)
		*pConnBlock=b;
	return b!=0;
	}

//----------------------------------------------------------------------------------------------------

// ��������� ������� ���� � ����� ������ �����
char * L57_GetDynModelFileName(RDS_BHANDLE block){
	RDS_PCOMPMODELDATA model;
  	if(!block)
    	return NULL;
  	model=rdscompGetBlockModelData(block);
  	if(!model)
    	return NULL;
  	// � ����� ���� ������
  	return rdsGetFullFilePath(model->ModelName,NULL,NULL);
	}

//----------------------------------------------------------------------------------------------------

// ������ �� ����� �������� ����� ������ � ������� ������ (���������)

#define Windows_ChunkSizeMax (1 << 22) // ������/������ �� ������ �����

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

// �������� ������ ����� (��������)
// psize - ������������ ������
// pTooLong - �����.1 ���� ���� ������� �������
// ������� - ����������
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

// ������� �������� ���� � ����������� ���������� ����� (����������� rdsFree)
// path       - ����
// psize      - ������������ ������
// addzerobyte - ��������� ��� ��� ������� ���� � �����
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

  	// ������ ����
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
	
// ������� ����� ��������� ������ � ��������� ����
BOOL EditWin(char *caption, double *plimit, double *poutput, COLORREF *pcolor){
	BOOL res;
	// �������� ����
	RDS_HOBJECT window=rdsFORMCreate(FALSE,-1,-1,caption);
	// ���� ����� ������
	rdsFORMAddEdit(window,0,0,RDS_FORMCTRL_EDIT,"�����:",80);
	rdsSetObjectDouble(window,0,RDS_FORMVAL_VALUE,*plimit);
	// ���� ����� ��������� ��������
	rdsFORMAddEdit(window,0,1,RDS_FORMCTRL_EDIT,"�������� ��������:",80);
	rdsSetObjectDouble(window,1,RDS_FORMVAL_VALUE,*poutput);
	// ���� ����� �����
	rdsFORMAddEdit(window,0,2,RDS_FORMCTRL_COLOR,"����:",80);
	rdsSetObjectInt(window,2,RDS_FORMVAL_VALUE,*pcolor);
	// �������� ���� (������� ��������� ������ �� ������������)
	res=rdsFORMShowModalServ(window,NULL);
	if(res){ // ������ OK
		*plimit=rdsGetObjectDouble(window,0,RDS_FORMVAL_VALUE);
		*poutput=rdsGetObjectDouble(window,1,RDS_FORMVAL_VALUE);
		*pcolor=(COLORREF)rdsGetObjectInt(window,2,RDS_FORMVAL_VALUE);
		}
	// ����������� ����
	rdsDeleteObject(window);
	return res;
	}

// ���������� ��������� ����� ���� ��������� ����
void RDSCALL NSetupEditsEnable(RDS_HOBJECT window, RDS_PFORMSERVFUNCDATA data){
	bool flag;
	int field_id=BASIC_TAB;
	// �������� ����
	field_id++;
	// �����
	field_id++;
	// ���� �������
	field_id++;
	// ���������� ������� ��������
	flag=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
	// ����� ���� ����� ������� � ������� �������� ����
	rdsSetObjectInt(window,++field_id,RDS_FORMVAL_ENABLED, flag);
	// ��������� ��������
	field_id++;
	// ��� �������
	flag=(rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE)==NF_LIM)?1:0;
	
	field_id=LIMITS_TAB;
	// �������� ����������� ������
	rdsSetObjectInt(window,++field_id,RDS_FORMVAL_ENABLED, flag);
	// ������� ��������� ��������
	//rdsSetObjectInt(window,++field_id,RDS_FORMVAL_ENABLED, flag);
	field_id++;
	int count;
	double l, output;
	COLORREF c;
	switch(data->Event){
		// ����������� ��������� ������ �����
		case RDS_FORMSERVEVENT_DRAW:
			// ���� ������� ������ ��� ������ ����� ������� �������
			if(data->CtrlId==field_id && data->ColId==2){
				int w=30, h=12; // ������� �������� ��������������
				int x=data->Left+(data->Width-w)/2,
				y=data->Top+(data->Height-h)/2;
				// ��������� ��������������
				c=(COLORREF)atoi(data->CellValue);
				rdsXGSetPenStyle(0,PS_SOLID,1,data->FGColor,R2_COPYPEN);
				rdsXGSetBrushStyle(0,RDS_GFS_SOLID,c);
				rdsXGRectangle(x,y,x+w,y+h);
				}
			break;
			
		// ������� ����� �� ������ ���������� �������
		case RDS_FORMSERVEVENT_TBLBUTTON:
			if(data->CtrlId==field_id)
				switch(data->Button){
					// ������ ���������� ������
					case RDS_FORMVAL_TBLBTN_ADD:
						// ��������� �������� ��� ����������� ������
						c=0xffffff; // ����� ����
						l=0.0;		// ��������� ��������
						output=0.0; 	// �������� ��������
						// ����� ����� � �������
						count=rdsGetObjectInt(window,data->CtrlId,RDS_FORMVAL_TBLCOUNT);
						// �������� ���� ��� ����� ����� � ��������
						if(EditWin("����������",&l,&output,&c)){
							// ���������� - ��� ��������� ������ count
							rdsFORMTBLSetDouble(window,data->CtrlId,count,0,l);
							rdsFORMTBLSetDouble(window,data->CtrlId,count,1,output);
							rdsFORMTBLSetInt(window,data->CtrlId,count,2,c);
							data->IndexAfterButton=count;
							}
					break;
				// ������ �������������� ������
				case RDS_FORMVAL_TBLBTN_PROP:
					// ���������� ������ ������� ������
					l=rdsFORMTBLGetDouble(window,data->CtrlId,data->TableRow,0,NULL);
					output=rdsFORMTBLGetDouble(window,data->CtrlId,data->TableRow,1,NULL);
					c=(COLORREF)rdsFORMTBLGetInt(window,data->CtrlId,data->TableRow,2,NULL);
					// �������� ���� ��� ����� ����� � ��������
					if(EditWin("��������������",&l, &output, &c)){
						// ������ OK - ������ ����� �������� � �������
						rdsFORMTBLSetDouble(window,data->CtrlId,data->TableRow,0,l);
						rdsFORMTBLSetDouble(window,data->CtrlId,data->TableRow,1,output);
						rdsFORMTBLSetInt(window,data->CtrlId,data->TableRow,2,c);
						}
					break;
				}
			break;
			
	// ���������� �������������� ������
	case RDS_FORMSERVEVENT_TBLBTNENABLE:
		if(data->CtrlId==field_id)
			switch(data->Button){
				// ������ ��������
				case RDS_FORMVAL_TBLBTN_DEL:
					// ���� � ������� �������� ������ ���� ������, �� ������ ��������
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

#define	RF_K		0		// ��� ������� ����� - �����������
#define	RF_CSTM		1		// ��� ������� ����� - �����������

// ���������� ��������� ����� ���� ��������� �����
void RDSCALL RSetupEditsEnable(RDS_HOBJECT window, RDS_PFORMSERVFUNCDATA /*data*/){
	int id, flag;
	id=0;
	flag=rdsGetObjectInt(window,++id,RDS_FORMVAL_VALUE);
	rdsSetObjectInt(window,++id,RDS_FORMVAL_ENABLED, flag==RF_K);
	rdsSetObjectInt(window,++id,RDS_FORMVAL_ENABLED, flag==RF_CSTM);
	}

//----------------------------------------------------------------------------------------------------
