//---------------------------------------------------------------------------
// ������������� �������������� �������� ����� ��� ������ "IpuL57Mk2SimpleCalc3.mdl"
// �������� ���� ������: C:\Users\�����\Documents\��� ���\BR-2\IpuL57Mk2SimpleCalc3.mdl
// ������ ��������������: Open Watcom C++ (��������)
//---------------------------------------------------------------------------
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define RDSBCPP_MODELNAME "IpuL57Mk2SimpleCalc3.mdl" // ��� ������ ��� ���������

#include <RdsDef.h>
#define RDS_SERV_FUNC_BODY rdsbcppGetService
#include <RdsFunc.h>
#include <CommonBl.h>
#include <CommonAC.hpp>

//---------------------------------------------------------------------------
// �������������� ���������
//---------------------------------------------------------------------------
#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif
#ifndef M_E
  #define M_E 2.71828182845904523536
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� ��������� ����������
//---------------------------------------------------------------------------
// ��������� ���������� ��������
#define RDSBCPP_EXCEPTIONS
// �������� try
#define RDSBCPP_TRY try
// �������� catch
#define RDSBCPP_CATCHALL catch(...)
//---------------------------------------------------------------------------

// ������ �������������� �������
double rdsbcppHugeDouble;

//---------------------------------------------------------------------------
// �������� ������ ����������
//---------------------------------------------------------------------------
int matherr(struct _exception *a)
{ a->retval=(a->type==UNDERFLOW)?0.0:rdsbcppHugeDouble;
  return 1;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ���������� �������� ������������
//---------------------------------------------------------------------------
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */



































// ������ ������
#define L57_MODELVERSION 3


#define L57MS_GLOBALS
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_GLOBALS

// ��������� ����� ��� ���������� �������� �������
class L57_Calc
{
  public:
    double x;
    double A;
    L57_RdsDoubleArray X;
    double i;
    // ������� ���������� (������������)
    double _Calc_gen(void);
    // ������� ���������� (� ���������� ����������)
    double _Calc_main(void)
    { double __temp;
      RDSBCPP_TRY
        { __temp=_Calc_gen(); }
      RDSBCPP_CATCHALL
        { __temp=rdsbcppHugeDouble; }
      return __temp;
    };
};
// ������� ���������� (������������)
double L57_Calc::_Calc_gen(void)
{
int n=X.Size();
double S=0;

for (int j=0; j<n; j++){
	S=S+X[j];
};

if (i<n) x=(1-A/1000000)*X[i]+A*S/n/1000000;

return x; // 3
; /* */
  return rdsbcppHugeDouble;
}
//----------------------------------------------------------------------

/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� ��� ������� �����
//---------------------------------------------------------------------------
// ������� "IPU.L57.UserFuncTransfer.IsFuncSource"
class rdsbcppFunction0G : public rdsbcppFunction // ����������
{ public:
    // ����� � �����
    int Call(RDS_BHANDLE Block)
      {return rdsCallBlockFunction(Block,_Id,NULL);};
    // ����� � ������ ����������
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,NULL,Flags);};
    rdsbcppFunction0G(void):rdsbcppFunction(){};
    ~rdsbcppFunction0G(){};
};
rdsbcppFunction0G L57_BlkFunc_IsFuncSource;
class rdsbcppFunction0L : public rdsbcppFunction // ���������
{ public:
    // ����� � �����
    int Call(RDS_BHANDLE Block)
      {return rdsCallBlockFunction(Block,_Id,NULL);};
    // ����� � ������ ����������
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,NULL,Flags);};
    // ����� � ����������� �������
    int Call(void)
      { return _Link?rdsCallBlockFunction(_Link->Block,_Id,NULL):0;};
    // ������� ������� ��������/����������� � public
    inline void RegisterProvider(void){_RegisterProvider();};
    inline void UnregisterProvider(void){_UnregisterProvider();};
    inline void SubscribeToProvider(void){_SubscribeToProvider();};
    inline void UnsubscribeFromProvider(void){_UnsubscribeFromProvider();};
    rdsbcppFunction0L(void):rdsbcppFunction(){};
    ~rdsbcppFunction0L(){};
};
// ������� "IPU.L57.UserFuncTransfer.AcceptFunction"
class rdsbcppFunction1G : public rdsbcppFunction // ����������
{ public:
    // ����� � �����
    int Call(RDS_BHANDLE Block,L57_AcceptFunc_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // ����� � ������ ����������
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_AcceptFunc_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    rdsbcppFunction1G(void):rdsbcppFunction(){};
    ~rdsbcppFunction1G(){};
};
rdsbcppFunction1G L57_BlkFunc_AcceptFunc;
class rdsbcppFunction1L : public rdsbcppFunction // ���������
{ public:
    // ����� � �����
    int Call(RDS_BHANDLE Block,L57_AcceptFunc_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // ����� � ������ ����������
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_AcceptFunc_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    // ����� � ����������� �������
    int Call(L57_AcceptFunc_Param* param)
      { return _Link?rdsCallBlockFunction(_Link->Block,_Id,param):0;};
    // ������� ������� ��������/����������� � public
    inline void RegisterProvider(void){_RegisterProvider();};
    inline void UnregisterProvider(void){_UnregisterProvider();};
    inline void SubscribeToProvider(void){_SubscribeToProvider();};
    inline void UnsubscribeFromProvider(void){_UnsubscribeFromProvider();};
    rdsbcppFunction1L(void):rdsbcppFunction(){};
    ~rdsbcppFunction1L(){};
};
// ������� "IPU.L57.UserFuncTransfer.AcceptParameter"
class rdsbcppFunction2G : public rdsbcppFunction // ����������
{ public:
    // ����� � �����
    int Call(RDS_BHANDLE Block,L57_AcceptParam_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // ����� � ������ ����������
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_AcceptParam_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    rdsbcppFunction2G(void):rdsbcppFunction(){};
    ~rdsbcppFunction2G(){};
};
rdsbcppFunction2G L57_BlkFunc_AcceptParam;
class rdsbcppFunction2L : public rdsbcppFunction // ���������
{ public:
    // ����� � �����
    int Call(RDS_BHANDLE Block,L57_AcceptParam_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // ����� � ������ ����������
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_AcceptParam_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    // ����� � ����������� �������
    int Call(L57_AcceptParam_Param* param)
      { return _Link?rdsCallBlockFunction(_Link->Block,_Id,param):0;};
    // ������� ������� ��������/����������� � public
    inline void RegisterProvider(void){_RegisterProvider();};
    inline void UnregisterProvider(void){_UnregisterProvider();};
    inline void SubscribeToProvider(void){_SubscribeToProvider();};
    inline void UnsubscribeFromProvider(void){_UnsubscribeFromProvider();};
    rdsbcppFunction2L(void):rdsbcppFunction(){};
    ~rdsbcppFunction2L(){};
};
// ������� "IPU.L57.UserFuncTransfer.IsParameterTransferredWithFunc"
class rdsbcppFunction3G : public rdsbcppFunction // ����������
{ public:
    // ����� � �����
    int Call(RDS_BHANDLE Block,L57_CheckParamConn_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // ����� � ������ ����������
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_CheckParamConn_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    rdsbcppFunction3G(void):rdsbcppFunction(){};
    ~rdsbcppFunction3G(){};
};
rdsbcppFunction3G L57_BlkFunc_CheckTransParam;
class rdsbcppFunction3L : public rdsbcppFunction // ���������
{ public:
    // ����� � �����
    int Call(RDS_BHANDLE Block,L57_CheckParamConn_Param* param)
      {return rdsCallBlockFunction(Block,_Id,param);};
    // ����� � ������ ����������
    int Broadcast(RDS_BHANDLE Parent,DWORD Flags,L57_CheckParamConn_Param* param)
      {return rdsBroadcastFunctionCallsEx(Parent,_Id,param,Flags);};
    // ����� � ����������� �������
    int Call(L57_CheckParamConn_Param* param)
      { return _Link?rdsCallBlockFunction(_Link->Block,_Id,param):0;};
    // ������� ������� ��������/����������� � public
    inline void RegisterProvider(void){_RegisterProvider();};
    inline void UnregisterProvider(void){_UnregisterProvider();};
    inline void SubscribeToProvider(void){_SubscribeToProvider();};
    inline void UnsubscribeFromProvider(void){_UnsubscribeFromProvider();};
    rdsbcppFunction3L(void):rdsbcppFunction(){};
    ~rdsbcppFunction3L(){};
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����� ����� DLL
//---------------------------------------------------------------------------
int WINAPI DllMain(HINSTANCE hinst,unsigned long reason,void *lpReserved)
{ if(reason==DLL_PROCESS_ATTACH)
    { if(!RDS_SERV_FUNC_BODY())
        { MessageBox(NULL,"���������� �������� ������ � ��������� �������� RDS",RDSBCPP_MODELNAME,MB_OK | MB_ICONERROR);
          return 0;
        }
      else // ���� ������ � ��������� ��������
        { rdsGetHugeDouble(&rdsbcppHugeDouble);
          // ����������� ������� �����
          L57_BlkFunc_IsFuncSource.Register("IPU.L57.UserFuncTransfer.IsFuncSource");
          L57_BlkFunc_AcceptFunc.Register("IPU.L57.UserFuncTransfer.AcceptFunction");
          L57_BlkFunc_AcceptParam.Register("IPU.L57.UserFuncTransfer.AcceptParameter");
          L57_BlkFunc_CheckTransParam.Register("IPU.L57.UserFuncTransfer.IsParameterTransferredWithFunc");
        }
    }
  return 1;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// �������� ������� ���������� �����
//---------------------------------------------------------------------------
// ���������� char ("C")
RDSBCPP_STATICPLAINCLASS(rdsbcstChar,char);
// ���������� double ("D")
RDSBCPP_STATICPLAINCLASS(rdsbcstDouble,double);
// ���������� char ("S")
RDSBCPP_STATICPLAINCLASS(rdsbcstSignal,char);
// ���������� short int ("H")
RDSBCPP_STATICPLAINCLASS(rdsbcstShort,short int);
// ������ double ("MD")
RDSBCPP_STATICPLAINARRCLASS(rdsbcstADouble,double);
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ����� �����
//---------------------------------------------------------------------------
class rdsbcppBlockClass
{ public:
    // ��������� ������ �����
    RDS_PBLOCKDATA rdsbcppBlockData;
    // ����������� ����������
    rdsbcstSignal _Start;
    rdsbcstSignal _Ready;
    rdsbcstShort Aux_in;
    rdsbcstShort Aux_out;
    rdsbcstChar Func_in;
    rdsbcstChar Func_out;
    rdsbcstDouble Calc_out;
    rdsbcstSignal _allinval_dchg;
    rdsbcstSignal _X_mchg;
    rdsbcstDouble x;
    rdsbcstDouble A;
    rdsbcstADouble X;
    rdsbcstDouble i;

    // �������� ����������� �������
    rdsbcppStaticStub rdsbcppStaticStubVar;

    // ������� ������� ������ (�� �� �����, ��� � � ����������)
    rdsbcppFunction0L L57_BlkFunc_IsFuncSource;
    rdsbcppFunction1L L57_BlkFunc_AcceptFunc;
    rdsbcppFunction2L L57_BlkFunc_AcceptParam;
    rdsbcppFunction3L L57_BlkFunc_CheckTransParam;

    // ������������� ���������� �����
    void rdsbcppInitVars(void *base)
    {
      // ����������� ����������
      _Start.InitNum(base,0,0);
      _Ready.InitNum(base,1,1);
      Aux_in.InitNum(base,2,2);
      Aux_out.InitNum(base,4,3);
      Func_in.InitNum(base,6,4);
      Func_out.InitNum(base,7,5);
      Calc_out.InitNum(base,8,6);
      _allinval_dchg.InitNum(base,16,7);
      _X_mchg.InitNum(base,17,8);
      x.InitNum(base,18,9);
      A.InitNum(base,26,10);
      X.InitNum(base,34,11);
      i.InitNum(base,42,12);
    };

    // �������� ������������� ������������ ����������
    BOOL rdsbcppDynVarsOk(void)
    { return TRUE; };

    // ������� ������� �� �������
    void rdsbcppModel(RDS_PINITIALCALCDATA InitialCalc);
    void rdsbcppSetupFunc(int &Result);
    void rdsbcppInit(void);
    void rdsbcppStartCalc(RDS_PSTARTSTOPDATA StartStopData);
    void rdsbcppCalcMode(void);
    void rdsbcppLoadText(char *LoadedText);
    void rdsbcppSaveText(void);
    void rdsbcppOther(int CallMode,void *ExtParam,int &Result);
    void L57_BlkFunc_IsFuncSourceEvent(RDS_PFUNCTIONCALLDATA FData,int &Result);
    void L57_BlkFunc_AcceptFuncEvent(L57_AcceptFunc_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result);
    void L57_BlkFunc_AcceptParamEvent(L57_AcceptParam_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result);
    void L57_BlkFunc_CheckTransParamEvent(L57_CheckParamConn_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result);
    void rdsbcppStaticVarsInit(void);
    void rdsbcppResetCalc(void);

    // �����������
    rdsbcppBlockClass(RDS_PBLOCKDATA data);
    // ����������
    ~rdsbcppBlockClass();
//---------------------------------------------------------------------------
// �������� ������������ � ������ �����
//---------------------------------------------------------------------------
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_CLASS
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_CLASS

  // ������� ��� �������� ������-���������� �������
  L57_FuncTransfer _Transfer;
  L57_FuncTransferParameter *x_trans;
  L57_FuncTransferParameter *A_trans;
  L57_FuncTransferParameter *X_trans;
  L57_FuncTransferParameter *i_trans;
  // �����
  void L57_Reset_Trans_Data(void)
  {
    x_trans=NULL;
    A_trans=NULL;
    X_trans=NULL;
    i_trans=NULL;
  };
  // �������� ���� ���������� ��������
  void L57_Create_Trans_Data(void);

  // ��������� ��� �������� ��������� ����������
  double x_old;
  double A_old;
  L57_DblMatrTempStorage X_old;
  double i_old;
  // ������������� �������� ��������
  void L57_Init_Old_Data(void)
  {
    x_old=rdsbcppHugeDouble;
    A_old=rdsbcppHugeDouble;
    i_old=rdsbcppHugeDouble;
    X_old.Clear();
  };

/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
//---------------------------------------------------------------------------

}; // class rdsbcppBlockClass
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// �������� ������������ ����� ������ �����
//---------------------------------------------------------------------------
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_AFTERCLASS
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_AFTERCLASS

// ������� ������ ������� ��� �������������� � ������� ������� �� ������
char *rdsbcppBlockClass::_FuncExportText="V A V 1\n"
"V i V 0\n"
"V X A\n"
"V x V 0\n"
"F\n"
"T����� A*x\n"
"X\n"
"|int n=X.Size();\n"
"|double S=0;\n"
"|\n"
"|for (int j=0; j<n; j++){\n"
"|\tS=S+X[j];\n"
"|};\n"
"|\n"
"|if (i<n) x=(1-A/1000000)*X[i]+A*S/n/1000000;\n"
"|\n"
"|return x; // 3\n"
"}\n"
"#ORDER#\n"
"x\n"
"A\n"
"X\n"
"i";

// ��������� ������������ ���������� � ����������� �������
void rdsbcppBlockClass::L57InputsToCalc(void)
{
  _Calculator.x=x;
  _Calculator.A=A;
  _Calculator.X.Setup(X);
  _Calculator.i=i;
}
//-----------------------------------------------------------------------

// �������� ���� ���������� ��������
void rdsbcppBlockClass::L57_Create_Trans_Data(void)
{
  // ������������ ������: 4
  _Transfer.ClearParameters();
  _Transfer.EnlargeParameters(4);

  x_trans=_Transfer.AddParameterRds("x",L57_VARSUFFIX_NONE,FALSE,x.Num(),&x);
  A_trans=_Transfer.AddParameterRds("A",L57_VARSUFFIX_NONE,FALSE,A.Num(),&A);
  X_trans=_Transfer.AddParameterRds("X",L57_VARSUFFIX_NONE,TRUE,X.Num(),&X);
  i_trans=_Transfer.AddParameterRds("i",L57_VARSUFFIX_NONE,FALSE,i.Num(),&i);

}
//-----------------------------------------------------------------------

// �������� ��������� ���������� � �������� ����������
void rdsbcppBlockClass::L57InputsChange(void)
{ BOOL _changes=FALSE;

  // ������������ ������������ ������: 3
  // ��������� ������������ ������: 0
  // ����� ������������: 3

  // ������������ �����
  if(_allinval_dchg!=0)
    { _allinval_dchg=0;
      _changes=FALSE;

      if(x_old!=x)
        { _changes=TRUE;
          _Transfer.TransferParameter(rdsbcppBlockData->Block,x_trans,_Transfer.DestBlocks,TRUE);
        }
      if(A_old!=A)
        { _changes=TRUE;
          _Transfer.TransferParameter(rdsbcppBlockData->Block,A_trans,_Transfer.DestBlocks,TRUE);
        }
      if(i_old!=i)
        { _changes=TRUE;
          _Transfer.TransferParameter(rdsbcppBlockData->Block,i_trans,_Transfer.DestBlocks,TRUE);
        }

      if(_changes)
        { // ����������� ������������
          x_old=x;
          A_old=A;
          i_old=i;
        }
    }

  // ��������� ����� (�������, �������, �������)
  if(X_old.Check(_X_mchg.GetPtr(),&X,_changes,FALSE))
    _Transfer.TransferParameter(rdsbcppBlockData->Block,X_trans,_Transfer.DestBlocks,TRUE);

}
//-----------------------------------------------------------------------

/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� �����
//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) int RDSCALL rdsbcppBlockEntryPoint(
  int CallMode,             // ����� ������ (��������� RDS_BFM_*)
  RDS_PBLOCKDATA BlockData, // ������ �����
  LPVOID ExtParam           // �������������� ������ (������� �� CallMode)
)
{ int result=RDS_BFR_DONE;  // ��� ��������
  // ������ ������ ����� (�������� � ������ ������� ������)
  rdsbcppBlockClass *data=(rdsbcppBlockClass*)(BlockData->BlockData);

  // �������� ������ ���������� - ������
  volatile unsigned int FPMask=_control87(0,0);
  _control87(MCW_EM,MCW_EM);

  switch(CallMode)
    { case RDS_BFM_INIT: // ������������� �����
        BlockData->BlockData=(data=new rdsbcppBlockClass(BlockData));
        data->rdsbcppInit();
        BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
        break;

      case RDS_BFM_CLEANUP: // ������� (���������� ����� ���������)
        delete data;
        break;

      case RDS_BFM_VARCHECK: // �������� ������������ ��������� ����������
        if(strcmp((char*)ExtParam,"{SSHHCCDSSDDMDD}")!=0)
          return RDS_BFR_BADVARSMSG;
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppStaticVarsInit();
        BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
        break;

      case RDS_BFM_FUNCTIONCALL: // ������� �� ����� ������� �����
        if(((RDS_PFUNCTIONCALLDATA)ExtParam)->Function==L57_BlkFunc_IsFuncSource.Id())
          { // ������� "IPU.L57.UserFuncTransfer.IsFuncSource"
            data->rdsbcppInitVars(BlockData->VarData);
            data->L57_BlkFunc_IsFuncSourceEvent((RDS_PFUNCTIONCALLDATA)ExtParam,result);
            BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
          }
        else if(((RDS_PFUNCTIONCALLDATA)ExtParam)->Function==L57_BlkFunc_AcceptFunc.Id())
          { // ������� "IPU.L57.UserFuncTransfer.AcceptFunction"
            data->rdsbcppInitVars(BlockData->VarData);
            data->L57_BlkFunc_AcceptFuncEvent((L57_AcceptFunc_Param*)(((RDS_PFUNCTIONCALLDATA)ExtParam)->Data),(RDS_PFUNCTIONCALLDATA)ExtParam,result);
            BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
          }
        else if(((RDS_PFUNCTIONCALLDATA)ExtParam)->Function==L57_BlkFunc_AcceptParam.Id())
          { // ������� "IPU.L57.UserFuncTransfer.AcceptParameter"
            data->rdsbcppInitVars(BlockData->VarData);
            data->L57_BlkFunc_AcceptParamEvent((L57_AcceptParam_Param*)(((RDS_PFUNCTIONCALLDATA)ExtParam)->Data),(RDS_PFUNCTIONCALLDATA)ExtParam,result);
            BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
          }
        else if(((RDS_PFUNCTIONCALLDATA)ExtParam)->Function==L57_BlkFunc_CheckTransParam.Id())
          { // ������� "IPU.L57.UserFuncTransfer.IsParameterTransferredWithFunc"
            data->rdsbcppInitVars(BlockData->VarData);
            data->L57_BlkFunc_CheckTransParamEvent((L57_CheckParamConn_Param*)(((RDS_PFUNCTIONCALLDATA)ExtParam)->Data),(RDS_PFUNCTIONCALLDATA)ExtParam,result);
            BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
          }
        break;

      case RDS_BFM_MODEL: // ���� ���� �������������
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppModel((RDS_PINITIALCALCDATA)ExtParam);
        BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
        break;

      case RDS_BFM_SETUP: // ����� ������� ���������
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppSetupFunc(result);
        BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
        break;

      case RDS_BFM_STARTCALC: // ������ �������
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppStartCalc((RDS_PSTARTSTOPDATA)ExtParam);
        BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
        break;

      case RDS_BFM_CALCMODE: // ���� � ����� �������������
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppCalcMode();
        BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
        break;

      case RDS_BFM_LOADTXT: // �������� ������ ����� � ��������� ����
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppLoadText((char*)ExtParam);
        BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
        break;

      case RDS_BFM_SAVETXT: // ������ ������ ����� � ��������� ����
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppSaveText();
        BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
        break;

      case RDS_BFM_RESETCALC: // ����� �������
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppResetCalc();
        BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
        break;

      default: // ������ �������
        data->rdsbcppInitVars(BlockData->VarData);
        data->rdsbcppOther(CallMode,ExtParam,result);
        BlockData->BlockData=data; // ������������ �� ���������� ��������� �������������
    } // switch(CallMode)

  // �������� ������ ���������� - �����
  _clear87();
  _control87(FPMask,MCW_EM);

  return result;
}
//---------------------------------------------------------------------------

// ����������� ������ �����
rdsbcppBlockClass::rdsbcppBlockClass(RDS_PBLOCKDATA data)
{ // ���������� ������ ��������� ������ �����
  rdsbcppBlockData=data;
  // ������������� ��������� ��������-������� �� ����������
  L57_BlkFunc_IsFuncSource.InitLocal(::L57_BlkFunc_IsFuncSource);
  L57_BlkFunc_AcceptFunc.InitLocal(::L57_BlkFunc_AcceptFunc);
  L57_BlkFunc_AcceptParam.InitLocal(::L57_BlkFunc_AcceptParam);
  L57_BlkFunc_CheckTransParam.InitLocal(::L57_BlkFunc_CheckTransParam);

}
//---------------------------------------------------------------------------

// ���������� ������ �����
rdsbcppBlockClass::~rdsbcppBlockClass()
{
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� ������� �� �������
//---------------------------------------------------------------------------
// ���� ���� �������������
void rdsbcppBlockClass::rdsbcppModel(RDS_PINITIALCALCDATA InitialCalc)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_MODEL
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_MODEL

/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::rdsbcppModel
//---------------------------------------------------------------------------

// ����� ������� ���������
void rdsbcppBlockClass::rdsbcppSetupFunc(int &Result)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_SETUP
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_SETUP

/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::rdsbcppSetupFunc
//---------------------------------------------------------------------------

// ����������� ������� � ����������� ����������
#define _Start rdsbcppStaticStubVar
#define _Ready rdsbcppStaticStubVar
#define Aux_in rdsbcppStaticStubVar
#define Aux_out rdsbcppStaticStubVar
#define Func_in rdsbcppStaticStubVar
#define Func_out rdsbcppStaticStubVar
#define Calc_out rdsbcppStaticStubVar
#define _allinval_dchg rdsbcppStaticStubVar
#define _X_mchg rdsbcppStaticStubVar
#define x rdsbcppStaticStubVar
#define A rdsbcppStaticStubVar
#define X rdsbcppStaticStubVar
#define i rdsbcppStaticStubVar
// ������������� ������ �����
void rdsbcppBlockClass::rdsbcppInit(void)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_INIT
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_INIT


/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::rdsbcppInit
// �������������� ������� � ����������� ����������
#undef i
#undef X
#undef A
#undef x
#undef _X_mchg
#undef _allinval_dchg
#undef Calc_out
#undef Func_out
#undef Func_in
#undef Aux_out
#undef Aux_in
#undef _Ready
#undef _Start
//---------------------------------------------------------------------------

// ������ �������
void rdsbcppBlockClass::rdsbcppStartCalc(RDS_PSTARTSTOPDATA StartStopData)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_START
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_START

/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::rdsbcppStartCalc
//---------------------------------------------------------------------------

// ���� � ����� �������������
void rdsbcppBlockClass::rdsbcppCalcMode(void)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_CALCMODE
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_CALCMODE

/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::rdsbcppCalcMode
//---------------------------------------------------------------------------

// �������� ������ ����� � ��������� ����
void rdsbcppBlockClass::rdsbcppLoadText(char *LoadedText)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_LOADPARAMS
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_LOADPARAMS
/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::rdsbcppLoadText
//---------------------------------------------------------------------------

// ������ ������ ����� � ��������� ����
void rdsbcppBlockClass::rdsbcppSaveText(void)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_SAVEPARAMS
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_SAVEPARAMS
/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::rdsbcppSaveText
//---------------------------------------------------------------------------

// ������ �������
void rdsbcppBlockClass::rdsbcppOther(int CallMode,void *ExtParam,int &Result)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_OTHER
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_OTHER
/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::rdsbcppOther
//---------------------------------------------------------------------------

// ������� �� ������� ����� "IPU.L57.UserFuncTransfer.IsFuncSource"
void rdsbcppBlockClass::L57_BlkFunc_IsFuncSourceEvent(RDS_PFUNCTIONCALLDATA FData,int &Result)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

Result=1;
/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::L57_BlkFunc_IsFuncSourceEvent
//---------------------------------------------------------------------------

// ������� �� ������� ����� "IPU.L57.UserFuncTransfer.AcceptFunction"
void rdsbcppBlockClass::L57_BlkFunc_AcceptFuncEvent(L57_AcceptFunc_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_ACCEPTFUNC
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_ACCEPTFUNC

/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::L57_BlkFunc_AcceptFuncEvent
//---------------------------------------------------------------------------

// ������� �� ������� ����� "IPU.L57.UserFuncTransfer.AcceptParameter"
void rdsbcppBlockClass::L57_BlkFunc_AcceptParamEvent(L57_AcceptParam_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_ACCEPTPARAM
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_ACCEPTPARAM

/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::L57_BlkFunc_AcceptParamEvent
//---------------------------------------------------------------------------

// ������� �� ������� ����� "IPU.L57.UserFuncTransfer.IsParameterTransferredWithFunc"
void rdsbcppBlockClass::L57_BlkFunc_CheckTransParamEvent(L57_CheckParamConn_Param* Param,RDS_PFUNCTIONCALLDATA FData,int &Result)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_CHECKTRANSFUNC
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_CHECKTRANSFUNC

/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::L57_BlkFunc_CheckTransParamEvent
//---------------------------------------------------------------------------

// ����� ��������� �������� ����������� ����������
void rdsbcppBlockClass::rdsbcppStaticVarsInit(void)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_VARCHECK
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_VARCHECK

/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::rdsbcppStaticVarsInit
//---------------------------------------------------------------------------

// ����� �������
void rdsbcppBlockClass::rdsbcppResetCalc(void)
{
/* ��������! ��� ������ �� ������������� ��� �������������� �������������,
�� ����� ������������ ������������� */

#define L57MS_RESET
#include <RdsL57Mk2/L57Mk2SimpleCalc.cpp>
#undef L57MS_RESET


/* ��������� �����������, �������������� �� �������������� ����������� � ������ ������������ */
} // rdsbcppBlockClass::rdsbcppResetCalc
//---------------------------------------------------------------------------

