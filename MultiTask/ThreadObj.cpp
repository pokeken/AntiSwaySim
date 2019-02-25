#pragma once

#include "stdafx.h"
#include "ThreadObj.h"

#include <windowsx.h>
#include <commctrl.h> //�R�����R���g���[���p


using namespace std;

//# �}���`�X���b�h��{�N���X �����o�[�֐� 

CThreadObj::CThreadObj() {//�R���X�g���N�^
	inf.hndl = NULL; 
	inf.cycle_ms = DEFAUT_TASK_CYCLE;
	inf.thread_com = REPEAT_INFINIT;
	wsprintf(inf.name, L"Who am I?"); 
	inf.act_count = inf.act_time = 0;
	inf.priority = THREAD_PRIORITY_NORMAL;
	inf.work_select = THREAD_WORK_IDLE;
	inf.hWnd_parent = inf.hWnd_work = inf.hWnd_opepane = inf.hWnd_msgStatics = NULL;
	inf.hBmp = NULL;
};

CThreadObj::~CThreadObj() {
	delete inf.hBmp;
};//�f�X�g���N�^

void CThreadObj::init_task(void *pobj) {
	set_panel_tip_txt();  
	return; 
};

//# �X���b�h���C���֐�
unsigned CThreadObj::run(void *param) {

	while (this->inf.thread_com != TERMINATE_THREAD){
		inf.event_triggered=WaitForMultipleObjects(inf.n_active_events,((CThreadObj*)param)->inf.hevents,FALSE, INFINITE);//���C���X���b�h�����SIGNAL��ԑ҂�

		//���������m�F�p
		DWORD start_time = timeGetTime();
		inf.period = start_time - inf.start_time;
		inf.start_time = start_time;

		if (inf.work_select == THREAD_WORK_ROUTINE) routine_work(param);
		else if (inf.work_select == THREAD_WORK_OPTION1) optional_work1(param);
		else if (inf.work_select == THREAD_WORK_OPTION2) optional_work2(param);
		else default_work(param);//������

		inf.act_time = timeGetTime() - start_time;//���������ԑ���p
		if (this->inf.thread_com != REPEAT_INFINIT) break;//���������̏ꍇ�͔�����
	}
	return 0;
}

void CThreadObj::routine_work(void *param) {

	ws << L"I am working!"<< *(inf.psys_counter);
	tweet2owner(ws.str()); ws.str(L"") ; ws.clear();

	wstring wstr = L"ZZZ";
	txout2msg_listbox(wstr);

};

//# ���C���E�B���h�E�ւ̃V���[�g���b�Z�[�W�\���@string
void CThreadObj::tweet2owner(const std::string &src) {
	wstring srcw;
	tool.Str2Wstr(src, srcw);
	if(inf.hWnd_msgStatics != NULL) SetWindowText(inf.hWnd_msgStatics, srcw.c_str()); return;
};

//# ���C���E�B���h�E�ւ̃V���[�g���b�Z�[�W�\���@wstring
void CThreadObj::tweet2owner(const std::wstring &srcw) {
	if (inf.hWnd_msgStatics != NULL) SetWindowText(inf.hWnd_msgStatics, srcw.c_str()); return;
}; //���C���E�B���h�E�ւ̃V���[�g���b�Z�[�W�\��

void CThreadObj::txout2msg_listbox(std::wstring str) {

	const wchar_t *pwc; pwc = str.c_str();

	inf.hWnd_msgList = GetDlgItem(inf.hWnd_opepane, IDC_LIST1);
	LVITEM item;

	item.mask = LVIF_TEXT;
	item.pszText = (wchar_t *)pwc;   // �e�L�X�g
	item.iItem = inf.cnt_PNLlist_msg % MSG_LIST_MAX;	// �ԍ�
	item.iSubItem = 1;									// �T�u�A�C�e���̔ԍ�
	ListView_SetItem(inf.hWnd_msgList, &item);

	SYSTEMTIME st; TCHAR tbuf[32];
	::GetLocalTime(&st);
	wsprintf(tbuf, L"%02d:%02d:%02d.%01d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds/100);

	item.pszText = tbuf;   // �e�L�X�g
	item.iSubItem = 0;									// �T�u�A�C�e���̔ԍ�
	ListView_SetItem(inf.hWnd_msgList, &item);

	//InvalidateRect(inf.hWnd_msgList, NULL, TRUE);
	inf.cnt_PNLlist_msg++;

};
void CThreadObj::txout2msg_listbox(std::string str) {
	wstring wstr; tool.Str2Wstr(str, wstr);
	txout2msg_listbox(wstr);
	return;
};

LRESULT CALLBACK CThreadObj::PanelProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {

	switch (msg) {
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDC_TASK_FUNC_RADIO1:inf.panel_func_id = IDC_TASK_FUNC_RADIO1; set_panel_tip_txt(); break;
		case IDC_TASK_FUNC_RADIO2:inf.panel_func_id = IDC_TASK_FUNC_RADIO2; set_panel_tip_txt(); break;
		case IDC_TASK_FUNC_RADIO3:inf.panel_func_id = IDC_TASK_FUNC_RADIO3; set_panel_tip_txt(); break;
		case IDC_TASK_FUNC_RADIO4:inf.panel_func_id = IDC_TASK_FUNC_RADIO4; set_panel_tip_txt(); break;
		case IDC_TASK_FUNC_RADIO5:inf.panel_func_id = IDC_TASK_FUNC_RADIO5; set_panel_tip_txt(); break;
		case IDC_TASK_FUNC_RADIO6:inf.panel_func_id = IDC_TASK_FUNC_RADIO6; set_panel_tip_txt(); break;
		case IDC_TASK_ITEM_RADIO1:inf.panel_type_id = IDC_TASK_ITEM_RADIO1; set_panel_tip_txt(); SetFocus(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT1)); break;
		case IDC_TASK_ITEM_RADIO2:inf.panel_type_id = IDC_TASK_ITEM_RADIO2; set_panel_tip_txt(); SetFocus(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT2)); break;
		case IDC_TASK_ITEM_RADIO3:inf.panel_type_id = IDC_TASK_ITEM_RADIO3; set_panel_tip_txt(); SetFocus(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT3)); break;
		case IDC_TASK_ITEM_RADIO4:inf.panel_type_id = IDC_TASK_ITEM_RADIO4; set_panel_tip_txt(); SetFocus(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT4)); break;
		case IDC_TASK_ITEM_RADIO5:inf.panel_type_id = IDC_TASK_ITEM_RADIO5; set_panel_tip_txt(); SetFocus(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT5)); break;
		case IDC_TASK_ITEM_RADIO6:inf.panel_type_id = IDC_TASK_ITEM_RADIO6; set_panel_tip_txt(); SetFocus(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT6)); break;
			break;
		}
		break;
	}
	return 0;
};
HWND CThreadObj::CreateOwnWindow(HWND h_parent_wnd) {

	return inf.hWnd_work;
};

void CThreadObj::set_PNLparam_value(float p1, float p2, float p3, float p4, float p5, float p6) {
	wstring wstr;
	wstr += std::to_wstring(p1); SetWindowText(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT1), wstr.c_str()); wstr.clear();
	wstr += std::to_wstring(p2); SetWindowText(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT2), wstr.c_str()); wstr.clear();
	wstr += std::to_wstring(p3); SetWindowText(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT3), wstr.c_str()); wstr.clear();
	wstr += std::to_wstring(p4); SetWindowText(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT4), wstr.c_str()); wstr.clear();
	wstr += std::to_wstring(p5); SetWindowText(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT5), wstr.c_str()); wstr.clear();
	wstr += std::to_wstring(p6); SetWindowText(GetDlgItem(inf.hWnd_opepane, IDC_TASK_EDIT6), wstr.c_str());
}
void CThreadObj::set_panel_tip_txt()
{
	wstring wstr_type; wstring wstr;
	switch (inf.panel_func_id) {
	case IDC_TASK_FUNC_RADIO1: {
		wstr = L"Type for Func1 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		default:break;
		}
	}break;
	case IDC_TASK_FUNC_RADIO2: {
		wstr = L"Type for Func2 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		default:break;
		}
	}break;
	case IDC_TASK_FUNC_RADIO3: {
		wstr = L"Type for Func3 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		default:break;
		}
	}break;
	case IDC_TASK_FUNC_RADIO4: {
		wstr = L"Type for Func4 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		default:break;
		}
	}break;
	case IDC_TASK_FUNC_RADIO5: {
		wstr = L"Type for Func5 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		default:break;
		}
	}break;
	case IDC_TASK_FUNC_RADIO6: {
		wstr = L"Type for Func6 \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		switch (inf.panel_type_id) {
		case IDC_TASK_ITEM_RADIO1:
			wstr_type += L"Param of type1 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO2:
			wstr_type += L"Param of type2 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO3:
			wstr_type += L"Param of type3 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO4:
			wstr_type += L"Param of type4 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO5:
			wstr_type += L"Param of type5 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		case IDC_TASK_ITEM_RADIO6:
			wstr_type += L"Param of type6 \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
			set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			break;
		default:break;
		}
	}break;
	default: {
		wstr = L"Type for Func? \n\r 1:?? 2:?? 3:?? \n\r 4:?? 5:?? 6:??";
		wstr_type += L"(Param of type?) \n\r 1:?? 2:??  3:?? \n\r 4:?? 5:?? 6:??";
		set_PNLparam_value(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	}break;
	}

	SetWindowText(GetDlgItem(inf.hWnd_opepane, IDC_STATIC_TASKSET3), wstr.c_str());
	SetWindowText(GetDlgItem(inf.hWnd_opepane, IDC_STATIC_TASKSET4), wstr_type.c_str());
}

