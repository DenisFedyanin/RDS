//---------------------------------------------------------------------------
// ����������� ����� ������ ���������� ���� ��������� �������
// ����� ���������� �� ������� ���� ���������� f(x,y,z)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ���������� ��������
//---------------------------------------------------------------------------
#ifdef L57MS_GLOBALS
#ifndef L57_MODELVERSION
  #define L57_MODELVERSION 0
#endif
#include <RdsL57/L57FuncXYZ.cpp>

// ��� ������� ������������ ����������� �������
#define L57FUNC_LIST ListFunc

// ��� ����� �������������� �������
#define L57_FUNCINPUT "FuncId"
//-----------------------------------------------------------------------

// ������� ��������� ������ ���� ���������
void RDSCALL WinCallback(RDS_HOBJECT win,RDS_PFORMSERVFUNCDATA data)
{ TL57TextFile *file=(TL57TextFile*)rdsGetObjectPtr(win,0,NULL);
  if(file)
    file->Setup_WindowCallback(win,data);
}
//---------------------------------------------------

// ��������� ���� ����� ��� ��������� � �������� ����������
void SetupVar(RDS_HOBJECT win,int tab,int basefield,char *var,
               double v0,double vmin,double vmax,double dv,
               BOOL in_v0,BOOL in_vdiap,BOOL in_dv)
{ int id=basefield;
  TL57TextString str;

  rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_MULTILABEL|RDS_FORMFLAG_LINE,
                 "��������� ��������� ����� ������������� � "
                 "���������� �����, � �� ������ � ��� ������:",0);
  rdsSetObjectInt(win,id,RDS_FORMVAL_MLHEIGHT,34);
  rdsSetObjectInt(win,id,RDS_FORMVAL_MLABELBEVEL,0);

  id++;
  str.Assign("����������� �������� ");
  str.AddString(var);
  str.AddString("0:");
  rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_EDIT|RDS_FORMFLAG_CHECK,
                 str.c_str(),80);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_VALUE,v0);
  rdsSetObjectInt(win,id,RDS_FORMVAL_CHECK,!in_v0);

  id++;
  str.Assign("����������� �������� ");
  str.AddString(var);
  str.AddString("min...");
  str.AddString(var);
  str.AddString("max:");
  rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_RANGEEDIT|RDS_FORMFLAG_CHECK,
                 str.c_str(),160);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_VALUE,vmin);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_RANGEMAX,vmax);
  rdsSetObjectInt(win,id,RDS_FORMVAL_CHECK,!in_vdiap);

  id++;
  str.Assign("����������� ��� d");
  str.AddString(var);
  str.AddString(":");
  rdsFORMAddEdit(win,tab,id,RDS_FORMCTRL_EDIT|RDS_FORMFLAG_CHECK,
                 str.c_str(),80);
  rdsSetObjectDouble(win,id,RDS_FORMVAL_VALUE,dv);
  rdsSetObjectInt(win,id,RDS_FORMVAL_CHECK,!in_dv);
}
//---------------------------------------------------

// ������� ���� ����� ��� ��������� � �������� ����������
void ReadSetupVar(RDS_HOBJECT win,int basefield,
                  double &v0,double &vmin,double &vmax,double &dv,
                  BOOL &in_v0,BOOL &in_vdiap,BOOL &in_dv)
{ int id=basefield+1;

  v0=rdsGetObjectDouble(win,id,RDS_FORMVAL_VALUE);
  in_v0=(rdsGetObjectInt(win,id,RDS_FORMVAL_CHECK)==0);

  id++;
  vmin=rdsGetObjectDouble(win,id,RDS_FORMVAL_VALUE);
  vmax=rdsGetObjectDouble(win,id,RDS_FORMVAL_RANGEMAX);
  in_vdiap=(rdsGetObjectInt(win,id,RDS_FORMVAL_CHECK)==0);

  id++;
  dv=rdsGetObjectDouble(win,id,RDS_FORMVAL_VALUE);
  in_dv=(rdsGetObjectInt(win,id,RDS_FORMVAL_CHECK)==0);
}
//---------------------------------------------------

// �������� ������� ���������� - ��������� (��� ��������� ����������)
double _SelectedFunc(int id,const TL57TextFuncArg &arguments);
//-----------------------------------------------------------------------

// ������� ���������� � ���������� ����������
double SelectedFunc(int id,const TL57TextFuncArg &arguments)
{ double f=0.0;
  RDSBCPP_TRY
    { f=_SelectedFunc(id,arguments); }
  RDSBCPP_CATCHALL
    { f=rdsbcppHugeDouble; }
  return f;
}
#endif // #ifdef L57MS_GLOBALS
//-----------------------------------------------------------------------





//---------------------------------------------------------------------------
// ���������
//---------------------------------------------------------------------------
#ifdef L57MS_SETUP
TL57TextFile file;
RDS_HOBJECT win;
BOOL res;
char caption[]="���������� ���� �������";

RDS_PCOMPMODELDATA model=rdscompGetBlockModelData(rdsbcppBlockData->Block);

if(model!=NULL && rdscompIsEditorOpen(model->Model))
  { rdsMessageBox("��� ��������� ����� ����� ���������� ������� �������� ��� ������. "
                  "����������, �������� ��������.",caption,MB_OK|MB_ICONWARNING);
    rdscompOpenBlockModelEditor(rdsbcppBlockData->Block);
    return;
  }

win=rdsFORMCreate(TRUE,-1,-1,caption);
if(!win)
  return;

CheckConnection();

// ������ ������ ������� �� �����
file.CanEditFuncId=L57_FUNCIDEDIT;
file.Setup_ReadFuncFile(rdsbcppBlockData->Block,model!=NULL,L57FUNC_LIST,win);

// ������� ������ �������
rdsFORMAddTab(win,1,"�������");
file.Setup_AddFuncSelectFields(win,NULL,1,1,SetupFuncId,model!=NULL,L57TF_ALLBUTTONS,
  InputConn?L57_FUNCINPUT:NULL);

// ������� X
rdsFORMAddTab(win,3," X");
SetupVar(win,3,100,"x",setup_x0,setup_xmin,setup_xmax,setup_dx,
         in_x0,in_xdiap,in_dx);

// ������� Y
rdsFORMAddTab(win,4," Y");
SetupVar(win,4,200,"y",setup_y0,setup_ymin,setup_ymax,setup_dy,
         in_y0,in_ydiap,in_dy);

// ������� Z
rdsFORMAddTab(win,5," Z");
SetupVar(win,5,300,"z",setup_z0,setup_zmin,setup_zmax,setup_dz,
         in_z0,in_zdiap,in_dz);

// ��������� ����
res=rdsFORMShowModalServ(win,WinCallback);

if(res)
  { // ���� Num �������
    SetupFuncId=file.FuncNumByIndex(rdsGetObjectInt(win,1,RDS_FORMVAL_VALUE),SetupFuncId);

    // ��������� ����, ���� �� ���������
    if(model!=NULL)
      file.Setup_WriteFuncFile(rdsbcppBlockData->Block,L57_BAK);

     // ������� X
     ReadSetupVar(win,100,setup_x0,setup_xmin,setup_xmax,setup_dx,
                  in_x0,in_xdiap,in_dx);
     // ������� Y
     ReadSetupVar(win,200,setup_y0,setup_ymin,setup_ymax,setup_dy,
                  in_y0,in_ydiap,in_dy);
     // ������� Z
     ReadSetupVar(win,300,setup_z0,setup_zmin,setup_zmax,setup_dz,
                  in_z0,in_zdiap,in_dz);
     // ������� ������� ��� ��������� �����
     x0_chg=y0_chg=z0_chg=1;
     xdiap_chg=ydiap_chg=zdiap_chg=1;
     Start=1;
 }

rdsDeleteObject(win);
Result=res?1:0;

#endif // #ifdef L57MS_SETUP
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// �������� � ������
//---------------------------------------------------------------------------
#ifdef L57MS_CLASS
TL57TextFuncArg Arguments;
BOOL InputConn;

// �������� ������� ����� � ����� �������������� �������
void CheckConnection(void)
{ InputConn=TL57TextFile::CheckConnectionForBlockVar(rdsbcppBlockData->Block,L57_FUNCINPUT); }
//-----------------------------------------------------------------------
// ��������� �������� �������
void CalcOutput(int fid,RDSBCPP_VT_FX &M,
                int varnum,
                double vmin,double vmax,double dv,
                TL57TextFuncArg &arg);
#endif // #ifdef L57MS_CLASS
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// ����� �������������
//---------------------------------------------------------------------------
#ifdef L57MS_CALCMODE
CheckConnection();
#endif // #ifdef L57MS_CALCMODE
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// �������� ����� ������
//---------------------------------------------------------------------------
#ifdef L57MS_AFTERCLASS
// ��������� �������� �������
void rdsbcppBlockClass::CalcOutput(int fid,RDSBCPP_VT_FX &M,
                                   int varnum,
                                   double vmin,double vmax,double dv,
                                   TL57TextFuncArg &arg)
{ BOOL bad;
  double _x0,_y0,_z0; 
  int n;
  // ������� ����� � ����� ��� �� ��������
  _x0=in_x0?x0:setup_x0;
  _y0=in_y0?y0:setup_y0;
  _z0=in_z0?z0:setup_z0;

  // ��������
  if(vmin==rdsbcppHugeDouble || vmax==rdsbcppHugeDouble || 
     dv==rdsbcppHugeDouble || dv<=0.0 || vmax<=vmin)
    bad=TRUE;
  else switch(varnum)
    { case 0: // x
        bad=(_y0==rdsbcppHugeDouble || _z0==rdsbcppHugeDouble);
        break;
      case 1: // y  
        bad=(_x0==rdsbcppHugeDouble || _z0==rdsbcppHugeDouble);
        break;
      case 2: // z
        bad=(_x0==rdsbcppHugeDouble || _y0==rdsbcppHugeDouble);
        break;
      default:
        bad=TRUE;
    }
  if(!bad)
    { // ���������� ����� ���������� ��������
      RDSBCPP_TRY
        { double d=(vmax-vmin)/dv;
          n=1+(int)(d); 
          bad=n<2;
        }
      RDSBCPP_CATCHALL
        { bad=TRUE; }
    }
  if(bad)
    { M.Resize(0,0,FALSE);
      return;
    }
  else
    { // ��������� ��������� �����
      double last=vmin+(n-1)*dv;
      if(vmax-last>0.1*dv)
        n++;
    }
  // ����� ���������
  if(!M.Resize(n,2,FALSE))
    { // �� ������� ������� �������
      rdsStopCalc();
      M.Resize(0,0,FALSE);
      rdsMessageBox("������� ������� ��������� ���� �� ����� ��� ���������� ����������, �� ������� ������ ��� "
                    "���������� ������ ��������� ������� ������ �������.",
                    rdsbcppBlockData->BlockName,MB_OK|MB_ICONERROR);
      return;
    }
  for(int i=0;i<n;i++)
    { double v,f;
      v=(i==(n-1)?vmax:(vmin+i*dv));
      switch(varnum)
        { case 0: // x
            arg.xval=v; arg.yval=_y0; arg.zval=_z0;
            f=SelectedFunc(fid,arg/*v,_y0,_z0*/);
            break;
          case 1: // y  
            arg.xval=_x0; arg.yval=v; arg.zval=_z0;
            f=SelectedFunc(fid,arg/*_x0,v,_z0*/);
            break;
          case 2: // z
            arg.xval=_x0; arg.yval=_y0; arg.zval=v;
            f=SelectedFunc(fid,arg/*_x0,_y0,v*/);
            break;
        }
      M[i][0]=v;
      M[i][1]=f;
    }
}
#endif // #ifdef L57MS_AFTERCLASS
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
// ������ �������
//---------------------------------------------------------------------------
#ifdef L57MS_START
Start=1;
#endif // #ifdef L57MS_START
//-----------------------------------------------------------------------





//---------------------------------------------------------------------------
// ������
//---------------------------------------------------------------------------
#ifdef L57MS_MODEL
BOOL arraychanged=FALSE;

// ������������� �������
int id;
if(InputConn)
  id=FuncId;
else
  id=SetupFuncId;
if(id!=OldFuncId)
  { OldFuncId=id;
    arraychanged=TRUE;
  }

// ������� ��������� ������ ���� �������
FX_en=FY_en=FZ_en=0;
Ready=0;

if(func_chg) // ���������� �������
  { BOOL FuncsChanged=FALSE;
    func_chg=0;
    int n=F.Size();
    // ���������, ����� ������
    if(n!=FS.Size())
      { FS.Resize(n,TRUE);
        FuncsChanged=TRUE;
      }
    for(int i=0;i<n;i++)
      { rdsbcstMDouble Fi=F[i],FSi=FS[i];
        if(TL57TextFuncFArray::IsBadMatrix(&Fi)) // ��� ����� ������ �� F[i]
          continue;
        // ������ ����, �� ����� ��?
        if(TL57TextFuncFArray::CompareMatr(&Fi,&FSi))
          { // ����� ��� ����
            Fi.Resize(2,2);
            TL57TextFuncFArray::SetBadMatrix(&Fi);
            continue;
          }
        // ����� ������
        FuncsChanged=TRUE;
        rdsExchangeVarArrays(Fi.GetPtr(),FSi.GetPtr());
        Fi.Resize(2,2);
        TL57TextFuncFArray::SetBadMatrix(&Fi);
      }
    if(FuncsChanged)
      { //test=GetTickCount();
        arraychanged=TRUE;
        Arguments.Functions.Realloc(n);
        for(int i=0;i<n;i++)
          Arguments.Functions[i].Setup(&(FS[i]));
      }
  }

if(ArrayChanged)
  { // ��������� �������� ��������� �������
    if(A.Size()!=A_old.Size()) // ��������� ������
      arraychanged=TRUE;
    else // ������ ��� ��
      for(int i=0;i<A.Size();i++)
        if(A[i]!=A_old[i])
          { arraychanged=TRUE;
            break;
          } 
    if(arraychanged)
      A_old=A;
  }

// ���������
if(A.IsEmpty())
  Arguments.Params.Setup(NULL);
else
  Arguments.Params.Setup(A.GetBuffer(),A.Size());

// ���������� FX
if(y0_chg || z0_chg || xdiap_chg || arraychanged)
  { double vmin,vmax,dv;
    if(in_xdiap) { vmin=xmin; vmax=xmax; }
    else { vmin=setup_xmin; vmax=setup_xmax;}
    dv=in_dx?dx:setup_dx;
    if(arraychanged||vmin!=old_xmin || vmax!=old_xmax || dv!=old_dx || old_y0!=y0 || old_z0!=z0)
      { CalcOutput(id,FX,0,vmin,vmax,dv,Arguments);
        old_xmin=vmin; old_xmax=vmax; old_dx=dv;
        FX_en=1;
        Ready=1;
      }
  }

// ���������� FY
if(x0_chg || z0_chg || ydiap_chg || arraychanged)
  { double vmin,vmax,dv;
    if(in_ydiap) { vmin=ymin; vmax=ymax; }
    else { vmin=setup_ymin; vmax=setup_ymax;}
    dv=in_dy?dy:setup_dy;
    if(arraychanged||vmin!=old_ymin || vmax!=old_ymax || dv!=old_dy || old_x0!=x0 || old_z0!=z0)
      { CalcOutput(id,FY,1,vmin,vmax,dv,Arguments);
        old_ymin=vmin; old_ymax=vmax; old_dy=dv;
        FY_en=1;
        Ready=1;
      }
  }

// ���������� FZ
if(x0_chg || y0_chg || zdiap_chg || arraychanged)
  { double vmin,vmax,dv;
    if(in_zdiap) { vmin=zmin; vmax=zmax; }
    else { vmin=setup_zmin; vmax=setup_zmax;}
    dv=in_dz?dz:setup_dz;
    if(arraychanged||vmin!=old_zmin || vmax!=old_zmax || dv!=old_dz || old_x0!=x0 || old_y0!=y0)
      { CalcOutput(id,FZ,2,vmin,vmax,dv,Arguments);
        old_zmin=vmin; old_zmax=vmax; old_dz=dv;
        FZ_en=1;
        Ready=1;
      }
  }

old_x0=x0; old_y0=y0; old_z0=z0;

// ���������� ������� ������������ ������
x0_chg=y0_chg=z0_chg=0;
xdiap_chg=ydiap_chg=zdiap_chg=0;
ArrayChanged=0;

#endif // #ifdef L57MS_MODEL
//-----------------------------------------------------------------------
