#define RP_Q		3		// ���������� ���������� ������
#define RP_FTYPE	0		// ��� ������� �����
#define RP_K		1		// ����������� �����
#define RP_STR		2		// ������������ ������� �����

#define BLACK 		0x00000000

//----------------------------------------------------------------------------------------------------

// ������ ������

RDS_SERVFONTPARAMS font;	// ������������ �����
char* name; 				// �������� (�������) ����
int name_size;				// ����� ����� ����
int regime,					// ��� �������� �������
	param_id[RP_Q];			// ����� ���������� �����
COLORREF default_color,   	// ��������� ������������� ���� ������� ����
	color;					// ����������� ���� ������� ����
double ival,				// ��������� ��������, ������������ ��� ��������
	ival_setup,				// ��������� ��������, �������� � ���� ��������
	lim_minval,				// �������� �������� ��� ������������ ������� ������
	cval;					// ������� ��������
int nfunc,					// ��� ������� ����
	ndec_cval;				// ����� ���� ����� ������� � ������� �������� ����
bool show_cval;				// ���������� ������� ��������
TRParams *rparams,			// ������ �������� ���� ���������� � ����� ������
	*rparams0;				// ����� ���������� ��������� ������
int qrp, qrp0;				// ������� �������� �������� ������
TKgNode mxformstruct;		// ���������, ������������ ����� ������������ ������� ���������
TLimit **limits;			// ������ ���������� �� ��������� �� ��������� ������� ���� ��������� ����
int lcount;					// ����� ������� ���� ��������� ����

//----------------------------------------------------------------------------------------------------

// ���������� ��������� ��������

void Constructor(void){
	// ��� ��������
	regime=0;
	// ��������� ����
	name=NULL;
	name_size=0;
	default_color=color=0x00FFFFFF;
	show_cval=true;
	ndec_cval=1;
	nfunc=0;
	ival=ival_setup=0.0;
	lim_minval=0.0;
	// �����
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
	// ��������� ���������� � ����� ������
	rparams=rparams0=NULL;
	qrp=qrp0=0;
	// ������
	// ������� ���� ������
	lcount=1;
	limits=new TLimit*[1];
	limits[0]=new TLimit();
	// ���������, ������������ ����� ������������ ������� ���������
	mxformstruct.id=NULL;
	mxformstruct.name=NULL;
	mxformstruct.qcons=0;
	mxformstruct.cmass_size=8;
	mxformstruct.connections=NULL;
	// ����������� ������� �����
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
	// ����������� ��� ���������� �����
	param_id[RP_FTYPE]=rdsRegisterConnExtParamName("RFuncType");
	param_id[RP_K]=rdsRegisterConnExtParamName("RK");
	param_id[RP_STR]=rdsRegisterConnExtParamName("RCustFunc");
	}

//----------------------------------------------------------------------------------------------------

// ����������� ������� ��������

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

// ����������� ���������� �������� ���� ����� �������
int AfterPoint(double lf){
	// ������������ �����
	if(lf<0.0)
		lf=(-1.0)*lf;
	// ������������ ����� �����
	lf=lf-floor(lf);	
	// ����������� ���������� �������� ���� ����� ������� � ��������� �� ��������
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
	
// ��������� ���������� �������
void SetLCount(int n){
TLimit **l;
// ���� ���������� �� ����������
if(n==lcount) 
	return;
// ���� ���������� ����� ������
if(n<lcount){
	// ����������� ������
	for(int i=n; i<lcount; i++)
		delete limits[i];
	// ���� ��������� ������� ���
	if(n<=0){
		lcount=0;
		delete[] limits;
		limits=NULL;
		return;
		}
	lcount=n;
	return;
	}
// ���� ��������� �������� ����� ������
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
	
// ��������� ����

int NSetup(void){
	bool OK;
	int field_id, sl;
	char *fontstr;
	
	// �������� ���� � ������������ ��������
	RDS_HOBJECT window=rdsFORMCreate(TRUE,-1,-1,"���� ����������� �����");
	
	#define BASIC_TAB       0
	#define LIMITS_TAB		100

	rdsFORMAddTab(window,BASIC_TAB,"��������");
	rdsFORMAddTab(window,LIMITS_TAB,"������");

	field_id=BASIC_TAB;
	// �������� ����
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_MULTILINE,"������� ����",80);
	rdsSetObjectInt(window,field_id,RDS_FORMVAL_MLRETURNS,1);
	rdsSetObjectStr(window,field_id,RDS_FORMVAL_VALUE,name);
	rdsSetObjectInt(window,field_id,RDS_FORMVAL_MLHEIGHT,54);
	// �����
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_FONTSELECT,"�����",75);
	fontstr=rdsStructToFontText(&font,NULL);
	rdsSetObjectStr(window,field_id,RDS_FORMVAL_VALUE,fontstr);
	rdsFree(fontstr);
	// ���� �������
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_COLOR,"���� �������",75);
    rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,default_color);
	// ���������� ������� ��������
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_CHECKBOX,"���������� ������� ��������",50);
	rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,show_cval);
	// ����� ���� ����� ������� � ������� �������� ����
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_EDIT,"����� ���� ����� �������",75);
	rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,ndec_cval);
	// ��������� ��������
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_EDIT,"��������� ��������",75);
	rdsSetObjectDouble(window,field_id,RDS_FORMVAL_VALUE,ival_setup);
	// ��� �������
	rdsFORMAddEdit(window,BASIC_TAB,++field_id,RDS_FORMCTRL_COMBOLIST,"��� �������",200);
	rdsSetObjectStr(window,field_id,RDS_FORMVAL_LIST,"��������\n���������\n��������\n�����");
	rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,nfunc);
	
	field_id=LIMITS_TAB;
	// �������� �������� ��� ������������ ����������� ������
	rdsFORMAddEdit(window,LIMITS_TAB,++field_id,RDS_FORMCTRL_EDIT,"����. ��� ������������ ����. ������",75);
	rdsSetObjectDouble(window,field_id,RDS_FORMVAL_VALUE,lim_minval);
	// ������� ��������� ��������
	rdsFORMAddEdit(window,LIMITS_TAB,++field_id,RDS_FORMCTRL_TABLE,"������",0);
	// ������� �������� ������
	rdsFORMTBLSetHeader(window,field_id,0,"�����",RDS_FORMTBLF_HDRCENTER|RDS_FORMTBLF_RIGHT,-1);
	// ������� ��������� ��������
	rdsFORMTBLSetHeader(window,field_id,1,"���. ����.",RDS_FORMTBLF_HDRCENTER|RDS_FORMTBLF_RIGHT,-1);
	// ������� �����
	rdsFORMTBLSetHeader(window,field_id,2,"����",RDS_FORMTBLF_HDRCENTER|RDS_FORMTBLF_DRAW,60);
	// ��������� �������� � ������� �������
	for(int i=0; i<lcount; i++){
		// �����
		rdsFORMTBLSetInt(window,field_id,i,0,limits[i]->limit);
		// �������� ��������
		rdsFORMTBLSetInt(window,field_id,i,1,limits[i]->output);
		// ����
		rdsFORMTBLSetInt(window,field_id,i,2,limits[i]->color);
		}
	// �������� ����
	OK=rdsFORMShowModalServ(window,NSetupEditsEnable);
	if(OK){
		field_id=BASIC_TAB;
		// �������� ����
		name=CopyStr(name,&name_size,rdsGetObjectStr(window,++field_id,RDS_FORMVAL_VALUE));
		// �����
		rdsFontTextToStruct(rdsGetObjectStr(window,++field_id,RDS_FORMVAL_VALUE),NULL,&font);
		// ���� �������
		default_color=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
		// ���������� ������� ��������
		show_cval=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
		// ����� ���� ����� ������� � ������� �������� ����
		ndec_cval=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
		// ��������� ��������
		ival=ival_setup=rdsGetObjectDouble(window,++field_id,RDS_FORMVAL_VALUE);
		// ��� �������
		nfunc=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
		
		field_id=LIMITS_TAB;
		// ��������� ��������
		lim_minval=rdsGetObjectDouble(window,++field_id,RDS_FORMVAL_VALUE);
		// ������ ����� ����� � ������� �������
		int count=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_TBLCOUNT);
		// �������� ������� �� ����� ����� ������� �������
		SetLCount(count);
		// ������ � �������� ������� ������� ������ �� �������
		for(int i=0;i<count;i++){
			limits[i]->limit=rdsFORMTBLGetDouble(window,field_id,i,0,NULL);
			limits[i]->output=rdsFORMTBLGetDouble(window,field_id,i,1,NULL);
			limits[i]->color=(COLORREF)rdsFORMTBLGetInt(window,field_id,i,2,NULL);
			}
		}
		
	// ����������� ���� ���������
	rdsDeleteObject(window);
	// ������������ ��������
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

// ��������� �����

int RSetup(RDS_PCONNSETUPDATA data){
	bool OK;
	int field_id;
	int rfunc;				// ��� ������� �����
	char *func_str;			// ������ ������� �����
	double rk;				// ��� �������
	
	// �������� ���� � ������������ ��������
	RDS_HOBJECT window=rdsFORMCreate(FALSE,-1,-1,"����� ����������� �����");

	field_id=0;
	// ��� �������
	rdsFORMAddEdit(window,0,++field_id,RDS_FORMCTRL_COMBOLIST,"��� �������",200);
	rdsSetObjectStr(window,field_id,RDS_FORMVAL_LIST,"�����������\n������������ �������");
	if(rdsGetConnExtParamByIdEx(data->Conn,param_id[RP_FTYPE],&rfunc,NULL,NULL,NULL)!=RDS_VARTYPE_INT)
		rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,RF_K);
	else
		rdsSetObjectInt(window,field_id,RDS_FORMVAL_VALUE,rfunc);
	// ���� ����� ������������ (����)
	rdsFORMAddEdit(window,0,++field_id,RDS_FORMCTRL_EDIT,"�����������",75);
	if(rdsGetConnExtParamByIdEx(data->Conn,param_id[RP_K],NULL,&rk,NULL,NULL)!=RDS_VARTYPE_DOUBLE)
		rdsSetObjectDouble(window,field_id,RDS_FORMVAL_VALUE,0.0);
	else
		rdsSetObjectDouble(window,field_id,RDS_FORMVAL_VALUE,rk);
	// ���� ����� ������ ������� �����
	rdsFORMAddEdit(window,0,++field_id,RDS_FORMCTRL_EDIT,"������������ �������",200);
	if(rdsGetConnExtParamByIdEx(data->Conn,param_id[RP_STR],NULL,NULL,&func_str,NULL)!=RDS_VARTYPE_STRING)
		rdsSetObjectStr(window,field_id,RDS_FORMVAL_VALUE,"");
	else
		rdsSetObjectStr(window,field_id,RDS_FORMVAL_VALUE,func_str);
		
	// �������� ����
	OK=rdsFORMShowModalServ(window,RSetupEditsEnable);
	if(OK){
		field_id=0;
		rfunc=rdsGetObjectInt(window,++field_id,RDS_FORMVAL_VALUE);
		rk=rdsGetObjectDouble(window,++field_id,RDS_FORMVAL_VALUE);
		func_str=rdsGetObjectStr(window,++field_id,RDS_FORMVAL_VALUE);
		rdsSetConnExtParamByIdEx(data->Conn,param_id[RP_FTYPE],RDS_VARTYPE_INT,rfunc,0,0,0);
		rdsSetConnExtParamByIdEx(data->Conn,param_id[RP_K],RDS_VARTYPE_DOUBLE,0,rk,0,0);
		rdsSetConnExtParamByIdEx(data->Conn,param_id[RP_STR],RDS_VARTYPE_STRING,0,0,func_str,0);
		// ������� �����
		if(rfunc==RF_K){
			char str[32];
			sprintf(str,"%.*lf", AfterPoint(rk), rk);
			rdsSetConnText(data->Conn,RDS_SCTF_TEXT|RDS_SCTF_ALIGNMENT|RDS_SCTF_SHOW,str,0,TRUE,0,0);
			}
		else if(rfunc==RF_CSTM)
			rdsSetConnText(data->Conn,RDS_SCTF_TEXT|RDS_SCTF_ALIGNMENT|RDS_SCTF_SHOW,func_str,0,TRUE,0,0);
		}
		
	// ����������� ���� ���������
	rdsDeleteObject(window);
	// ������������ ��������
	if(OK==true){
			return 1;
			}
	else{
			return 0;
		}
	}

//----------------------------------------------------------------------------------------------------

// ��������� ����

void Draw(RDS_PDRAWDATA DrawData){
	RECT block;
	double zoom;
	int w, h;

	// ������� �����
	block.left=DrawData->Left;
	block.top=DrawData->Top;
	block.right=block.left+DrawData->Width;
	block.bottom=block.top+DrawData->Height;

	// ���������� �������� �� ��������� ������ �����
	rdsXGSetClipRect(&block);

	// ���������� �����������
	zoom=DrawData->DoubleZoom;

	// ������
	rdsXGSetBrushStyle(0,RDS_GFS_SOLID,color);
	rdsXGSetPenStyle(0,PS_SOLID,1,BLACK,R2_COPYPEN);
	rdsXGEllipse(block.left, block.top, block.right, block.bottom);

	// �������
	if(name && DrawData->Width>0 && DrawData->Height>0){
		char *full_str=NULL;
		// ������������� �������� �������� � �������� ����
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
		// ��������� ������
		font.SizePriority=FALSE;
		rdsXGSetFontByParStr(&font,zoom);
		rdsXGSetBrushStyle(0,RDS_GFS_EMPTY, color);
		// ������� ���������� ����� � ����������� ����� ������� ������ � ������� ����
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

		// ��������� ������ ��� ������� ���������� ����� ������� ����
		TMultyLineParams* wh=new TMultyLineParams[qlines];
		
		// ��������� ������ ��� ������ ��������� ����� ������������� �������
		char **bstr=new char*[qlines];
		for(int i=0; i<qlines; i++){
			bstr[i]=new char[max_line_length+1];
			}

		if(bstr && wh){
			// ����������� ���������� ����� ������� ���� , 
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
			
			// ������ ��������� ������ ������������� �������
			int sumh=0;
			for(int i=0; i<qlines; i++){
				sumh+=wh[i].h;
				}
			
			// ����� ������������� ������� ����
			for(int i=0, h; i<qlines; i++){
				h=0;
				for(int j=(qlines-1); j>=i; j--)
					h+=wh[j].h;
				h=2*h-sumh;
				rdsXGTextOut(block.left+(block.right-block.left-wh[i].w)/2, block.top+(block.bottom-block.top-h)/2, bstr[i]);
				}
			}
		
		// ������� ������
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
		
	// ������ ������� ��������� �� ��������� �����
	rdsXGSetClipRect(NULL);
	
	#undef BLACK
	}

//----------------------------------------------------------------------------------------------------

// ������ ���������� ����� � ������ ������ ������

bool ReadRParams(void){
	int count=0;
	RDS_CHANDLE c;
	RDS_POINTDESCRIPTION point;
	point.servSize=sizeof(point);
	
	// ������� ������
	rparams=ClearRParams(rparams, &qrp);
	
	// ������� ���������� ������� ������
	c=NULL;
	for(;;){
		// ��������� �����, ����������� � ������
		c=rdsGetBlockLink(rdsbcppBlockData->Block,c,TRUE,FALSE,&point);
		// ����� �� �����, ���� ������ ��� ������
		if(c==NULL)
			break;
		// ���� ����� �������� ������� � � �������� �������� "x["
		if(!point.Source && strstr(point.VarName,"x["))
			count++;
		}
	if(count<1)
		return false;
	
	int func_type;			// ��� ������� �����
	char *func_str=NULL;	// ������ ������� �����
	int str_size;			// ���������� �������� ���������� ���������
	double k;				// ��� �������
	
	// ��������� ������ ��� ������ ��������
	rparams=new TRParams[count];
	// ���������� ������� ��������
	c=NULL;
	while(qrp<count){
		// ��������� �����, ����������� � ������
		c=rdsGetBlockLink(rdsbcppBlockData->Block,c,TRUE,FALSE,&point);
		// ����� �� �����, ���� ������ ��� ������
		if(c==NULL)
			break;
		// ���� ����� �������� �������� ��� ��� � ���������� �� �������� "x["
		if(point.Source || !strstr(point.VarName,"x["))
			continue;
		// ��������� ���� ������� �����
		if(rdsGetConnExtParamByIdEx(c,param_id[RP_FTYPE],&func_type,NULL,NULL,NULL)!=RDS_VARTYPE_INT)
			continue;
		// ��������� ������ ������� �����
		if(rdsGetConnExtParamByIdEx(c,param_id[RP_K],NULL,&k,NULL,NULL)!=RDS_VARTYPE_DOUBLE)
			continue;
		// ��������� ������ ������� �����
		if(rdsGetConnExtParamByIdEx(c,param_id[RP_STR],NULL,NULL,&func_str,NULL)!=RDS_VARTYPE_STRING)
			continue;
		// ������ ���������� � ���������
		rparams[qrp].vname=CopyStr(rparams[qrp].vname,&str_size,point.VarName);
		rparams[qrp].func_type=func_type;
		rparams[qrp].k=k;
		rparams[qrp].func_str=CopyStr(rparams[qrp].func_str,&str_size,func_str);			
		qrp++;
		}
	return true;
	}

//----------------------------------------------------------------------------------------------------

// ������������ ������

TRParams* ClearRParams(TRParams* rp, int *n){
	if(rp){
		// ������������ ������ �� ��� ���������� �������� � ��������� �������� ������
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

// �������� �� ���������� �������� ��������

bool CompareRParams(void){
	// ���� ����� ������ ���������, �� ��������� �� ���������
	if(qrp!=qrp0)
		return false;
	// ���� ����� ������ �������, �� ��������� ���������
	if(qrp==0)
		return true;
	// ������ ����� ��� ��������� ��������
	bool *check=new bool[qrp0];
	for(int i=0; i<qrp0; i++)
		check[i]=false;
	
	// ������������ ���������� ��� ����� ������� ���������
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
			// ���� � ������� ������ �������� �� ���������� �������� ��� ����� � ������ ���������������
			else if(j==(qrp0-1)){
				delete [] check;
				return false;
				}
			}
		}
	// ��� ������ ����� �� ������� ����� �������� ������ ������� ����� � ������� ������ ��������
	delete [] check;
	return true;
	}

//----------------------------------------------------------------------------------------------------
	
// ����������� ������� ��������

void CopyRParams(void){
	int str_size;
	// ��������� ������ ��� ������ ��������
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

// ���������� ������� � ���� �����

void ContextPopup(RDS_PCONTEXTPOPUPDATA MenuData){
	rdsAdditionalContextMenuItemEx("����������������� ��� ����",0,0,0);
	rdsAdditionalContextMenuItemEx("������ ����� �� ���. ���������",regime==0?RDS_MENU_CHECKED:0,1,0);
	rdsAdditionalContextMenuItemEx("������ ����� �� �����������",regime==1?RDS_MENU_CHECKED:0,1,1);
}

//----------------------------------------------------------------------------------------------------

// ����� ������ ���� �����

void MenuFunction(RDS_PMENUFUNCDATA MenuData){
	switch(MenuData->Function){
		case 0:
			// ����� ������� �������������� �������������� ������
			rdsBroadcastFunctionCallsEx(rdsbcppBlockData->Parent,RecompFuncId,NULL,0);
			break;
		case 1:
			// ���������� ��������� ���������� �������
			callparams.Command=MenuData->MenuData;
			callparams.servSize=sizeof(callparams);
			// ����� ������� ��������� ���� �������� (������)
			rdsBroadcastFunctionCallsEx(rdsbcppBlockData->Parent,RegimeFuncId,&callparams,0);
			break;
		}
}

//----------------------------------------------------------------------------------------------------

// ������ �������

void MdlChange(int CallMode, void *ExtParam, int &Result){
	FILE *in;
	switch(CallMode){
		// ������ ����� ������ ���� ��� ���� ��������� �����
		case RDS_BFM_SUPPORTSCONNSETUP:
			rdsConnSetupMenuName("������� ������ �����");
			Result=1;
			break;

		// �������� ���� ��������� ��� �����
		 case RDS_BFM_CONNSETUP:
			Result=RSetup((RDS_PCONNSETUPDATA)ExtParam);
			break;

		// �������� ��������� � ������������� ����������� ������ ����� ��� �������� � ����� �������������
		case RDS_BFM_LEAVINGEDITMODE:
			CheckAndRewriteMdl();
			Result=RDS_BFR_DONE;
			break;
		
		
		case RDS_BFM_FUNCTIONCALL:
			// ���������� ExtParam � ����������� ����
			func=(RDS_PFUNCTIONCALLDATA)ExtParam;
			// �������� �������������� ��������� �������
			// - ���������, ��� ������ ���� �������� ����� ����������� �����
			if(func->Function==IamNodeFuncId){
				Result=1;
				break;
				}
			// - �������������� �������������� ������
			else if(func->Function==RecompFuncId){ 			
				RewriteMdl();
				Result=RDS_BFR_DONE;
				break;
				}
			// - ��������� ���� ��������
			else if(func->Function==RegimeFuncId){
				TProgGuideEditCtrlSetParams *params;
				// ���������� ��������� �� ��������� � ������� ���� � �������� ������� ���������� ���������
				params=(TProgGuideEditCtrlSetParams*)(func->Data);
				if(params->servSize<sizeof(TProgGuideEditCtrlSetParams))
					// ������ ������������
					break; 
				// ��������� ���� ��������
				regime=params->Command;
				if(regime<0)
					regime=0;
				else if(regime>1)
					regime=1;
				Result=RDS_BFR_DONE;
				break;
				}
			// - �������� ���������� ���� ��� ������������ ������� ���������
			else if(func->Function==MxFuncId){
				// ������� ��������� ���������
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
				// ������� ��������� ������ ����� ������������ �������
				// � ��������� ��� ���������� ������ ������� ����
				// - ������������� ����
				mxformstruct.id=rdsbcppBlockData->Block;
				// - �������� ����
				mxformstruct.name=new char[strlen(name)+1];
				mxformstruct.name=strcpy(mxformstruct.name, name);
				// - ���������� � �������� ������ ����
				RDS_CHANDLE c=NULL;
				RDS_CONNDESCRIPTION conn;
				conn.servSize=sizeof(conn);
				RDS_POINTDESCRIPTION point;
				point.servSize=sizeof(point);
				int rfunc;
				double rk;
				TKgCon *buf=NULL;
				int buf_size=0;
				// ���� �� ���� ������ �����
				for(;;){
					// ��������� �����, ����������� � ������
					c=rdsGetBlockLink(rdsbcppBlockData->Block,c,TRUE,TRUE,NULL);
					// ����� �� �����, ���� ������ ��� ������
					if(c==NULL)
						break;
					// ��������� ���� ������� �����
					if(rdsGetConnExtParamByIdEx(c,param_id[RP_FTYPE],&rfunc,NULL,NULL,NULL)!=RDS_VARTYPE_INT){
						continue;
						}
					// ���� ��� ������� - �����������
					if(rfunc==RF_K){
						// ��������� ������������ �����
						if(rdsGetConnExtParamByIdEx(c,param_id[RP_K],NULL,&rk,NULL,NULL)!=RDS_VARTYPE_DOUBLE){
							continue;
							}
						// ��������� ������, ���� ��� ��� �� ���� ��������
						if(!mxformstruct.connections){
							mxformstruct.connections=new TKgCon[mxformstruct.cmass_size];
							}
						// ���������� ������� ������� � ������ �������������
						if(mxformstruct.qcons>=mxformstruct.cmass_size){
							// ���������� ������� ���������� ������
							buf_size=mxformstruct.cmass_size+8;
							buf=new TKgCon[buf_size];
							// ����������� ������ � ������� ������
							for(int i=0; i<mxformstruct.cmass_size; i++){
								buf[i].cid=mxformstruct.connections[i].cid;
								buf[i].k=mxformstruct.connections[i].k;
								}
							// ������� ������ �� ��� ������ ������ ref
							delete [] mxformstruct.connections;
							// ����������� ��������� �� ������� ������
							mxformstruct.connections=buf;
							mxformstruct.cmass_size=buf_size;
							}
						// ����������� � ������ �������������� ���������� �����
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
						// ������ ������������
						mxformstruct.connections[mxformstruct.qcons].k=rk;
						// ���������� ������� ������� ������
						mxformstruct.qcons++;
						}
					else
						continue;
					}					
				// ����� ������� ������������ ������� ���������
				rdsCallBlockFunction(func->Caller, MxCallBackFuncId, &mxformstruct);
				}
		}
	}

//----------------------------------------------------------------------------------------------------

// �������� ��������� � ������������� ����������� ������ �����

void CheckAndRewriteMdl(void){
	// ������ ������� ���������� ������
	ReadRParams();
	// ��������� ���������� ������
	// - ���� ��������� �� ����������, �� ������������ ������ �� �����
	if(CompareRParams())
		return;
	RewriteMdl();
	}

// ������������� ����������� ������ �����	
void RewriteMdl(void){
	// ����������� ������� �������� ������
	CopyRParams();
	
	// 1) ������ ������� ���� � ����� ��������� ������ � ������������ ������
	char *rds_include_path=rdsGetSystemPath(RDS_GSPINCLUDEPATH);
	int l=strlen(rds_include_path)+32;
	char *stmdl_path=new char[l];
	stmdl_path[0]='\0';
	stmdl_path=strcat(stmdl_path,rds_include_path);
	stmdl_path=strcat(stmdl_path,"kgmaps\\kgnode.mdl");
	
	// 2) ����������� ���������� �������� fsize � ����� stmdl_path
	// 3) ������������ ��������� ������� char *stmdl_text �� fsize ��������
	// 4) ������ ����� stmdl_path � stmdl_text
	DWORD fsize;
	char *stmdl_text=NULL;
	if(stmdl_path)
		stmdl_text=ReadFileToMemory(stmdl_path,&fsize,1);
	else
		return;
		
	// 5) �������� � �������� ��� ������ ������ ����� ������ �����
	// ����������� ���� � �������� ����� ������
	char *mdl_path=L57_GetDynModelFileName(rdsbcppBlockData->Block);
	// �������� ��� ������ ������� ���� ������
	FILE *kgnode=NULL;
	kgnode=fopen(mdl_path, "wb");

	#define MARKER "//@ connections functions"
			
	// 6) ����� ������ ����� � ������� stmdl_text
	if(kgnode && stmdl_text){
		char* substr=NULL;
		// ����� ������ ��������� ����� � ������ ������
		substr=strstr(stmdl_text,MARKER);
		// ���� ����� �������
		if(substr){
			// ������ ������� ������� ��������� ����� � ������ ������ ������
			int n=(int)(substr-stmdl_text);
			// ������ ������� �� ����� ���� �������������
			if(n>=0){
				// 6) ������ ����� ����������� ������� �� ����� � ����� ���� ������
				fwrite(stmdl_text,sizeof(char),n,kgnode);
				// 7) ����������� � ����� ���� ������ ������� ������, ������� � �����
				//    ������� rdsGetBlockLink (��. �.5.6.20) ��� ���������
				//    ���������� ��������� ����� � ����� �����, ������� �����
				//    ����� ������������ ��� ���������� ������ � ����� ���� ������
				RDS_CHANDLE c=NULL;
				RDS_POINTDESCRIPTION point;
				point.servSize=sizeof(point);
				int index=0;
				// �������� ���������� �����
				int rfunc;				// ��� ������� �����
				char *func_str=NULL;	// ������ ������� �����
				double rk;				// ��� �������
				char str[32];			// ��������������� ������
				for(;;){
					// ��������� �����, ����������� � ������
					c=rdsGetBlockLink(rdsbcppBlockData->Block,c,TRUE,FALSE,&point);
					// ����� �� �����, ���� ������ ��� ������
					if(c==NULL)
						break;
					// ���� ����� �������� �������� ��� ��� � ���������� �� �������� "x["
					if(point.Source || !strstr(point.VarName,"x["))
						continue;
					// ��������� ���� ������� �����
					if(rdsGetConnExtParamByIdEx(c,param_id[RP_FTYPE],&rfunc,NULL,NULL,NULL)!=RDS_VARTYPE_INT){
						rfunc=RF_K;
						rdsSetConnExtParamByIdEx(c,param_id[RP_FTYPE],RDS_VARTYPE_INT,rfunc,0,0,0);
						//continue;
						}
					// ���� ��� ������� - �����������
					if(rfunc==RF_K){
						// ��������� ������������ �����
						if(rdsGetConnExtParamByIdEx(c,param_id[RP_K],NULL,&rk,NULL,NULL)!=RDS_VARTYPE_DOUBLE){
							rk=1.0;
							rdsSetConnExtParamByIdEx(c,param_id[RP_K],RDS_VARTYPE_DOUBLE,0,rk,0,0);
							//continue;
							}
						// ������������ ������ ������� ����� ����
						// #define y ���_����������_n
						// z[��������_index]=y*��������_rk;
						// #undef y
						
						// ��� ������ ������ �� ���������� ���������
						fprintf(kgnode,"\r\n\tif(regime==0){\r\n");
						fprintf(kgnode,"\t\t#define y %s\r\n",point.VarName);
						fprintf(kgnode,"\t\tz[%d]=%lf*y;\r\n", index, rk);
						fprintf(kgnode,"\t\t#undef y\r\n");
						fprintf(kgnode,"\t\t}");
						// ��� ������ ������ �� �����������
						fprintf(kgnode,"\r\n\telse{\r\n");
						fprintf(kgnode,"\t\t#define y (%s-x0[%d])\r\n", point.VarName, index);
						fprintf(kgnode,"\t\tz[%d]=%lf*y;\r\n", index, rk);
						fprintf(kgnode,"\t\tx0[%d]=%s;\r\n", index, point.VarName);
						fprintf(kgnode,"\t\t#undef y\r\n");
						fprintf(kgnode,"\t\t}\r\n");
						index++;
						}
					else{
						// ��������� ������ ������� �����
						if(rdsGetConnExtParamByIdEx(c,param_id[RP_STR],NULL,NULL,&func_str,NULL)!=RDS_VARTYPE_STRING){
							sprintf(func_str,"y");
							rdsSetConnExtParamByIdEx(c,param_id[RP_STR],RDS_VARTYPE_STRING,0,0,func_str,0);
							//continue;
							}
						// ������������ ������ ������� ����� ����
						// #define y ���_����������_n
						// z[��������_index]=�����_�������_n;
						// #undef y
						// ��� ������ ������ �� ���������� ���������
						fprintf(kgnode,"\r\n\tif(regime==0){\r\n");
						fprintf(kgnode,"\t\t#define y %s\r\n",point.VarName);
						if(!func_str || strcmp(func_str,"\0")==0)
							fprintf(kgnode,"\t\tz[%d]=y;\r\n", index);
						else
							fprintf(kgnode,"\t\tz[%d]=%s;\r\n", index, func_str);
						fprintf(kgnode,"\t\t#undef y\r\n");
						fprintf(kgnode,"\t\t}");
						// ��� ������ ������ �� �����������
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
					// ������� �����
					if(rfunc==RF_K){
						sprintf(str,"%.*lf", AfterPoint(rk), rk);
						rdsSetConnText(c,RDS_SCTF_TEXT|RDS_SCTF_ALIGNMENT|RDS_SCTF_SHOW,str,0,TRUE,0,0);
						}
					else if(rfunc==RF_CSTM)
						rdsSetConnText(c,RDS_SCTF_TEXT|RDS_SCTF_ALIGNMENT|RDS_SCTF_SHOW,func_str,0,TRUE,0,0);
					}
				// 8) ����������� � ���� ��������� ���������� ����� �����
				fprintf(kgnode,stmdl_text+n+strlen(MARKER));
				// �������� �� ������������������ ������
				rdscompCompileBlockModelDelayed(rdsbcppBlockData->Block,FALSE);
				}
			}
		// ���� ����� �� �������
		else{
			// ����������� ����� ����������� ��������� ������
			fwrite(stmdl_text,sizeof(char),fsize,kgnode);
			// �������� �� ������������������ ������
			rdscompCompileBlockModelDelayed(rdsbcppBlockData->Block,FALSE);
			}
		fclose(kgnode);
		}
	
	#undef MARKER

	// 9) ������������ ������
	delete [] stmdl_path;
	if(mdl_path)
		rdsFree(mdl_path);
	if(stmdl_text)
		rdsFree(stmdl_text);
	}
		
//----------------------------------------------------------------------------------------------------

// ������ �������� �������� ����

double Calc(void){
	double y0, maxlim;
	// � ������, ����� �������� ���� �������� �����, ���� ������� ����� �������� ��� �������
	color=default_color;
	// ��� ������ ������� ������������ �������� ��������
	if(new_set){
		new_set=false;
		y=ival;
		}
	else{
		// ������� ����
		switch(nfunc){
			// ��������
			case NF_SUM:
				if(regime==0)
					y=0.0;
				for(int i=0; i<z.Size(); i++)
					y+=z[i];
				break;
			// ���������
			case NF_MUL:
				if(regime==0)
					y=1.0;
				for(int i=0; i<z.Size(); i++)
					y=y*z[i];
				break;
			// ��������
			case NF_SRC:
				y=ival;
				break;
			// �����
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

// ����� ������ ������

void Reset(void){
	new_set=true;
	}

//----------------------------------------------------------------------------------------------------

// ������� ������

void Clear(void){
if(name) delete [] name;
rparams=ClearRParams(rparams, &qrp);
rparams0=ClearRParams(rparams0, &qrp0);
// ������� ��������� ���������
if(mxformstruct.name){
	delete [] mxformstruct.name;
	mxformstruct.name=NULL;
	}
if(mxformstruct.connections){
	delete [] mxformstruct.connections;
	mxformstruct.connections=NULL;
	}
// ������� ������� ���������� �������
SetLCount(0);
}

//----------------------------------------------------------------------------------------------------

// ���������� ����������

void Save(void){
	char *fontstr, str[32];
	// ��� ��������
	rdsWriteWordValueText("Regime",regime);
	// ��������� ����
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
	// ��������� ������
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
	// ������
	rdsWriteWordValueText("LimCount",lcount);
    for(int i=0; i<lcount; i++){
		rdsWriteBlockDataText("LimItem",TRUE);
		rdsWriteWordDoubleText("LimVal",limits[i]->limit);
        rdsWriteWordDoubleText("LimOutput",limits[i]->output);
		rdsWriteWordValueText("LimColor",limits[i]->color);
		}
	}

//----------------------------------------------------------------------------------------------------

// �������� ����������

void Load(char *text){
	bool flag;
	char *pointer, *word, c, str[32];
	int current_lcount=0;
	pointer=text;
	rparams0=ClearRParams(rparams0, &qrp0);
	for(;;){
		flag=false;
		// �������� �� ������ ��������� �����
		word=rdsGetTextWord(pointer,&pointer,&c,FALSE);
		// ����� ���������� - ����� �� �����
		if(c==0)
			break;
		// ������� �� ����� ������ - ����������� �����������
		if(c=='\n')
			continue;
		// ���������� ���� ��������
		if(!flag){
			if(strcmp(word,"Regime")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				regime=atoi(word);
				flag=true;
				}
			}
		// ���������� �������� ����
		if(!flag){
			if(strcmp(word,"NodeName")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				name=CopyStr(name,&name_size,word);
				flag=true;
				}
			}
		// ���������� ������
		if(!flag){
			if(strcmp(word,"Font")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				rdsFontTextToStruct(word,NULL,&font);
				flag=true;
				}
			}
		// ���������� �����
		if(!flag){
			if(strcmp(word,"NodeColor")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				default_color=atoi(word);
				flag=true;
				}
			}
		// ���������� ������� ��������
		if(!flag){
			if(strcmp(word,"NodeShowCVal")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				show_cval=atoi(word);
				flag=true;
				}
			}
		// ����� ���� ����� ������� � ������� �������� ����
		if(!flag){
			if(strcmp(word,"NodeNDecCVal")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				ndec_cval=atoi(word);
				flag=true;
				}
			}
		// ���������� ��������� ��������
		if(!flag){
			if(strcmp(word,"NodeIVal")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				ival=ival_setup=atof(word);
				flag=true;
				}
			}
		// ���������� ���� ������� ����
		if(!flag){
			if(strcmp(word,"NodeFunc")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				nfunc=atoi(word);
				flag=true;
				}
			}
		// ���������� ��������� �������� ��� ������������ ����������� ������
		if(!flag){
			if(strcmp(word,"LimMinVal")==0){
				word=rdsGetTextWord(pointer,&pointer,0,false);
				lim_minval=atof(word);
				flag=true;
				}
			}
		// ��������� ������
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
		// ����� �������
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
		// ������� ������
		if(!flag){
			if(strcmp(word,"LimItem")==0){
				current_lcount++;
				if(current_lcount>lcount)
					SetLCount(current_lcount);
				flag=true;
				}
			}
		// �������� ������
		if(!flag){
			if(strcmp(word,"LimVal")==0){
				word=rdsGetTextWord(pointer,&pointer,NULL,FALSE);
				if(current_lcount>=0)
					limits[current_lcount-1]->limit=atof(word);
				flag=true;
				}
			}
		// �������� �������� ��� ������
        if(!flag){
			if(strcmp(word,"LimOutput")==0){
				word=rdsGetTextWord(pointer,&pointer,NULL,FALSE);
				if(current_lcount>=0)
					limits[current_lcount-1]->output=atof(word);
				flag=true;
				}
			}
		// ���� ��� ������
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