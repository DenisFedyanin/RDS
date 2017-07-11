//===================================================================================
// �������� ���������� �� ������� ��� ��������� � �����-���� ���������
// (��� "extern", ��� ���� �������)
// ������:
// typedef struct {
//   #define RDSCTRL_SERV_FUNC_POINTERS
//   #include "RdsCtrl.h"
// } TFunctionPointers;
//
//===================================================================================
#ifdef RDSCTRL_SERV_FUNC_POINTERS
  // ��������������� ������, ����������� ����������-���������
  #define _RDSCTRL_AUX_FUNC(name,type) type name;
  // �������� ����������-���������� �� �������
  #include "RdsCtrl_.h"
  #undef _RDSCTRL_AUX_FUNC
  #undef RDSCTRL_SERV_FUNC_POINTERS
// ����� #ifdef RDSCTRL_SERV_FUNC_POINTERS
#endif
//===================================================================================


//===================================================================================
// ��������� �������� ���� ������� ��������� ������� ���������������� �������
//===================================================================================
#ifdef RDSCTRL_SERV_FUNC_GETPTR
// ������� ��������� ����������
BOOL RDSCTRL_SERV_FUNC_GETPTR(HMODULE dll)
  { BOOL ok=TRUE;
    // ��������������� ������, ����������� ������� � ����������
    #define _RDSCTRL_AUX_FUNC(name,type) ok=((name=(type)GetProcAddress(dll,#name))!=NULL)&&ok;
    #include "RdsCtrl_.h"
    #undef _RDSCTRL_AUX_FUNC
    return ok;
  }
  #undef RDSCTRL_SERV_FUNC_GETPTR
// ����� #ifdef RDSCTRL_SERV_FUNC_GETPTR
#endif
//===================================================================================
