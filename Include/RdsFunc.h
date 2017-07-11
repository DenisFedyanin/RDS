//---------------------------------------------------------------------------
// ����� �������� ��� ������� � ������������ �������� RDS �� DLL
//---------------------------------------------------------------------------
#ifndef RdsFuncH
#define RdsFuncH

#ifndef RdsDefH
  // ��� ���������� ���������� ���������� �������� �� RdsDef.h
  #include "RdsDef.h"
#endif

#ifndef RdsCompH
  // ��� ���������� ���������� ���������� �������� �� RdsComp.h
  #include "RdsComp.h"
#endif

//===================================================================================
// ����� ���������� ���������� �� ������������ ������� � ������� ��� ��������� ����
// ����������.
// ������������� ������ ���� ����� ���������� #include "RdsFunc.h" ���� �������� ����
//     #define RDS_SERV_FUNC_BODY FuncName
// ��� FuncName - ��� ������������ ������� ��������� ����������.
// ���� ����� ����� ���� ������� ������ � ���� ���� �������, �.�. ������ � ����� �����
// ����� #include "RdsFunc.h" ����� ���������� #define RDS_SERV_FUNC_BODY.
// ������ ������ RDS_INTFUNCACCESS ������ ������������ ��-�� ����������� �� ����������� �����
// ���������������� � 4096 ����.
// ������ ������������� (� Borland C++ 5.5):
//    ------- ������� ���� ������� -------
//    #define RDS_SERV_FUNC_BODY MyGetService
//    #include "RdsFunc.h"  // ����� ����� ��������� ���� ������� BOOL MyGetService(void)
//    ......
//    int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
//    {
//      if(reason==DLL_PROCESS_ATTACH)
//        {
//          if(!MyGetService())
//            MessageBox(NULL,"������ ������� � ��������� ��������","DLL",MB_OK | MB_ICONERROR);
//        }
//      return 1;
//    }
//    ......
//    ------- ������ ����� ������� -------
//    #include "RdsFunc.h"
//    ......
// ����� RdsFunc.h ����� ���������� �������� ����, ���������� �� �������� ����� �
// ��������� ������� Windows API (� ���������, ���� BOOL � ������� GetModuleHandle).
// ��� Borland C++ �������� "windows.h".
//---------------------------------------------------------------------------
//===================================================================================
#ifdef RDS_SERV_FUNC_BODY
  // ��������������� ������, ����������� ����������-���������
  #define _RDS_AUX_FUNC(name,type) type name;

  // �������� ����������-���������� �� �������
  #include "RdsFunc_.h"

  #undef _RDS_AUX_FUNC
// ������� ��������� ����������
BOOL RDS_SERV_FUNC_BODY(void)
  { HMODULE exe=GetModuleHandle(NULL);
    BOOL ok=TRUE;
    // ��������������� ������, ����������� ������� � ����������
    #define _RDS_AUX_FUNC(name,type) ok=((name=(type)GetProcAddress(exe,#name))!=NULL)&&ok;

    // ���������� ���������� ����������
    #include "RdsFunc_.h"

    #undef _RDS_AUX_FUNC
    return ok;
  }
// ����� #ifdef RDS_SERV_FUNC_BODY
#endif
//===================================================================================


//===================================================================================
// �������� ���������� �� ������� ��� ������, �� ������������ #define RDS_SERV_FUNC_BODY
// (� "extern")
//===================================================================================
#if !defined(RDS_NOEXTERNFUNCPTRS) && !defined(RDS_SERV_FUNC_BODY)
  // ��������������� ������, ����������� ������� ����������-���������
  #define _RDS_AUX_FUNC(name,type) extern type name;

  // �������� � extern
  #include "RdsFunc_.h"

  #undef _RDS_AUX_FUNC
// ����� #ifndef RDS_AUX_NOEXTERNFUNCPTRS
#endif
//===================================================================================


//===================================================================================
// �������-�������� (������ ����������� ������������� ������� ��� ���������
// ���������� �������� �������)
//===================================================================================
#ifndef RDS_NOHOBJMACROS
  // ������������� ����� �� ���������������� �������
  #define rdsCEEditConnection           rdsCEEditConnBus
  // ������������� ���� �� ���������������� �������
  #define rdsCEEditBus                  rdsCEEditConnBus
  // ������� ����� �� ���������������� �������
  #define rdsCECreateConnection(o,p,e)  rdsCECreateConnBus((o),(p),RDS_CTCONNECTION,(e))
  // ������� ���� �� ���������������� �������
  #define rdsCECreateBus(o,p,e)         rdsCECreateConnBus((o),(p),RDS_CTBUS,(e))
  // �������� ������-�������� �����
  #define rdsCEClearEditor(o)           rdsCommandObject((o),RDS_HCE_RESET)
  // �������� ������ ������ �� �������-������
  #define rdsBCLGetBlockArray(o,ps)     ((RDS_BHANDLE*)rdsGetObjectArray((o),RDS_HBCL_BLOCKARRAY,0,(ps)))
  // �������� ������ ������ �� �������-������
  #define rdsBCLGetConnArray(o,ps)      ((RDS_CHANDLE*)rdsGetObjectArray((o),RDS_HBCL_CONNARRAY,0,(ps)))
  // ���������� ��� ��������� ��� ������ ����������
  #define rdsVSSetStructName(o,name)    rdsSetObjectStr((o),RDS_HVAR_SETTYPENAME,0,(name))
  // �������� ��� ��������� ������ ����������
  #define rdsVSGetStructName(o)         rdsGetObjectStr((o),RDS_HVAR_GETTYPENAME,0)
  // �������� ������-����� ����������
  #define rdsVSClearEditor(o)           rdsCommandObject((o),RDS_HVAR_RESET)
  // ������� ���������� �� ������ ����������
  #define rdsVSDeleteVar(o,n)           rdsCommandObjectEx((o),RDS_HVAR_DELVAR,(n),NULL)
  // ������� �������������� �� ������ ����������
  #define rdsVSDeleteAutoConn(o,n)      rdsCommandObjectEx((o),RDS_HVAR_DELAUTO,(n),NULL)
  // �������� ��� ������� ���������� �������������� � ��������� ������� � ������ ����������
  #define rdsVSGetAutoMain(o,n)         rdsGetObjectStr((o),RDS_HVAR_GETAUTOMAIN,(n))
  // �������� ��� ��������� ���������� �������������� � ��������� ������� � ������ ����������
  #define rdsVSGetAutoConn(o,n)         rdsGetObjectStr((o),RDS_HVAR_GETAUTOCONN,(n))
  // �������� ����� ���������� � ������ ����������
  #define rdsVSGetFieldCount(o)         rdsGetObjectInt((o),RDS_HVAR_GETFIELDCOUNT,0)
  // �������� ����� �������������� ����������  � ������ ����������
  #define rdsVSGetAutoCount(o)          rdsGetObjectInt((o),RDS_HVAR_GETAUTOCOUNT,0)
  // �������� ������� ������ ����������
  #define rdsVSGetStructRank(o)         rdsGetObjectInt((o),RDS_HVAR_GETVARRANK,0)
  // �������� ������ �������������� ��������
  #define rdsVSClearTypeRenames(o)      rdsCommandObject((o),RDS_HVAR_CLEARTYPEREN)
  // �������� ������ �������������� ����������
  #define rdsVSClearVarRenames(o)       rdsCommandObject((o),RDS_HVAR_CLEARVARREN)
  // ���������� ����� ��� ����������� (rdsSTRCreateTextReader)
  #define rdsSTRSetTextToRead(obj,text) rdsSetObjectStr((obj),RDS_HSTR_SETTEXT,0,(text))
  // ������� ����� �����
  #define rdsSTRGetIntWord(obj,skiplf)  rdsGetObjectInt((o),RDS_HSTR_READINT,(skiplf))
  // ������� ����� double
  #define rdsSTRGetDoubleWord(obj,skiplf) rdsGetObjectDouble((o),RDS_HSTR_READDOUBLE,(skiplf))
  // �������� ����� ����������� INI-������ (rdsINICreateTextHolder)
  #define rdsINIClearText(o)            rdsCommandObject((o),RDS_HINI_RESET)
  // ���������� ����� ��� ����������� INI-������
  #define rdsINISetText(obj,text)       rdsSetObjectStr((obj),RDS_HINI_SETTEXT,0,(text))
  // ��������� ���� ��� ����������� INI-������
  #define rdsINILoadFile(obj,filename)  rdsSetObjectStr((obj),RDS_HINI_LOADFILE,0,(filename))
  // �������� ���� ��� ����������� INI-������
  #define rdsINISaveFile(obj,filename)  rdsSetObjectStr((obj),RDS_HINI_SAVEFILE,0,(filename))
  // ������� ������ ��� ����������� INI-������
  #define rdsINICreateSection(obj,section)     rdsSetObjectStr((obj),RDS_HINI_CREATESECTION,0,(section))
  // ������� ������ ��� ����������� INI-������
  #define rdsINIDeleteSection(obj,section)     rdsSetObjectStr((obj),RDS_HINI_DELETESECTION,0,(section))
  // �������� ���������� �������� � ������� ������ ����������� INI-������
  #define rdsINIWriteBool(obj,key,val)  rdsINIWriteInt((obj),(key),(val)?1:0)
  // ������� ���������� �������� �� ������� ������ ����������� INI-������
  #define rdsINIReadBool(obj,key,defval)        (rdsINIReadInt((obj),(key),(defval)?1:0)!=0)
  // ������� �������� �� ������� ������ ����������� INI-������
  #define rdsINIDeleteValue(obj,key)            rdsSetObjectStr((obj),RDS_HINI_DELETEKEYLAST,0,(key))
  // �������� ����� ����������� INI-������ ��� ������ ����� � ��������� �������
  #define rdsINISaveBlockText(o,newln)          rdsCommandObjectEx((o),RDS_HINI_SAVEBLOCKTEXT,(newln)?1:0,NULL)
  // �������� ����� ����� ����� (��� ������� rdsFORMCreate)
  #define rdsFORMSetInt(o,id,val)               rdsSetObjectInt((o),(id),RDS_FORMVAL_VALUE,(val))
  // �������� � ����� ����� ����� (��� ������� rdsFORMCreate)
  #define rdsFORMGetInt(o,id)                   rdsGetObjectInt((o),(id),RDS_FORMVAL_VALUE)
  // �������� ����� ������ (��� ������� rdsFORMCreate)
  #define rdsFORMSetString(o,id,str)            rdsSetObjectStr((o),(id),RDS_FORMVAL_VALUE,(str))
  // �������� � ����� ������ (��� ������� rdsFORMCreate)
  #define rdsFORMGetString(o,id)                rdsGetObjectStr((o),(id),RDS_FORMVAL_VALUE)
  // �������� ����� ����� double (��� ������� rdsFORMCreate)
  #define rdsFORMSetDouble(o,id,val)            rdsSetObjectDouble((o),(id),RDS_FORMVAL_VALUE,(val))
  // �������� � ����� ����� double (��� ������� rdsFORMCreate)
  #define rdsFORMGetDouble(o,id)                rdsGetObjectDouble((o),(id),RDS_FORMVAL_VALUE)
  // �������� ����� BOOL (��� ������� rdsFORMCreate)
  #define rdsFORMSetBool(o,id,val)              rdsSetObjectInt((o),(id),RDS_FORMVAL_VALUE,(val)?1:0)
  // �������� � ����� BOOL (��� ������� rdsFORMCreate)
  #define rdsFORMGetBool(o,id)                  (rdsGetObjectInt((o),(id),RDS_FORMVAL_VALUE)!=0)
  // ���������� ������ ��������� ��� ���� ����� (��� ������� rdsFORMCreate)
  #define rdsFORMSetComboList(o,id,list)        rdsSetObjectStr((o),(id),RDS_FORMVAL_LIST,(list))
  // ���������� �������������� ���� ���������� ���� ����� (��� ������� rdsFORMCreate)
  #define rdsFORMSetEnableCheck(o,id,enabled)   rdsSetObjectInt((o),(id),RDS_FORMVAL_CHECK,(enabled)?1:0)
  // �������� �������������� ���� ���������� ���� ����� (��� ������� rdsFORMCreate)
  #define rdsFORMGetEnableCheck(o,id)           (rdsGetObjectInt((o),(id),RDS_FORMVAL_CHECK)!=0)
  // ���������/��������� ���� ����� ����������
  #define rdsFORMEnableControl(o,id,enabled)    rdsSetObjectInt((o),(id),RDS_FORMVAL_ENABLED,(enabled)?1:0)
  // ���������� ������ ��� �������������� ���� ����� (��� ������� rdsFORMCreate)
  #define rdsFORMSetMultilineHeight(o,id,height)   rdsSetObjectInt((o),(id),RDS_FORMVAL_MLHEIGHT,(height))
  // �������� ����� (��� ������� rdsFORMCreate)
  #define rdsFORMClear(o)                       rdsCommandObject((o),RDS_FORM_CLEAR)
  // �������� ����� (��� ������� rdsFORMCreate)
  #define rdsFORMShowModal(o)                   rdsCommandObject((o),RDS_FORM_SHOWMODAL)
  // �������� ��� ������ ProgressBar (��� ������� rdsPBARCreate)
  #define rdsPBARShow(o,show)                   rdsCommandObject((o),(show)?RDS_PBAR_SHOW:RDS_PBAR_HIDE)
  // ���������� ������� �������� ProgressBar (��� ������� rdsPBARCreate)
  #define rdsPBARSetPos(o,pos)                  rdsSetObjectInt((o),RDS_PBAR_POSITION,0,(pos))
  // �������� ������� �������� ProgressBar (��� ������� rdsPBARCreate)
  #define rdsPBARIncrement(o,inc)               rdsSetObjectInt((o),RDS_PBAR_ADDTOPOS,0,(inc))
  //  �������� ����� ��������� ����� � undo (��� ������� rdsBEUCreate)
  #define rdsBEUStore(o)                        rdsCommandObject((o),RDS_BEU_STORECHANGED)
#endif
//===================================================================================


#endif

