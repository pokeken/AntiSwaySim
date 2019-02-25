#include "stdafx.h"
#include "Helper.h"

CHelper::CHelper(){}

CHelper::~CHelper(){}

/*****************************************************************************
string�^��wstring�^�ɕϊ����܂�
- �����@const std::string &src�F�ϊ��O���́@, std::wstring &dest�F�ϊ���o�́@
- �߂�l�@NA
*****************************************************************************/
void CHelper::Str2Wstr(const std::string &src, std::wstring &dest) {
	
	setlocale(LC_ALL, "");
	
	wchar_t *wcs = new wchar_t[src.length() + 1];
	
	size_t ret_val;
	mbstowcs_s(&ret_val, wcs, src.length() + 1, src.c_str(), _TRUNCATE);
	
	dest = wcs;delete[] wcs;
	return;
}

/*****************************************************************************************
�w�肳�ꂽDC�A�ʒu�Ɏw�肳�ꂽ�r�b�g�}�b�v���������ށi���{�j
- ����	HDC hdc�F������DC, HBITMAP hbmp�F�f�ރr�b�g�}�b�v,
		POINT dst_pt�F������̈ʒu, POINT src_pt�F�������̈ʒu
- �߂�l�@NA
******************************************************************************************/
void CHelper::put_bmp_built(HDC hdc, HBITMAP hbmp, POINT dst_pt, POINT src_pt) {

	if ((hdc == NULL) | (hbmp == NULL)) return;

	BITMAP bmp;
	GetObject(hbmp, sizeof(BITMAP), &bmp);
	int BMP_W = (int)bmp.bmWidth;
	int BMP_H = (int)bmp.bmHeight;

	HDC hmdc = CreateCompatibleDC(hdc);
	SelectObject(hmdc, hbmp);

	BitBlt(hdc, dst_pt.x, dst_pt.y, BMP_W, BMP_H, hmdc, src_pt.x, src_pt.y, SRCCOPY);
	DeleteDC(hmdc);

	return;
};

/******************************************************************************************************
�w�肳�ꂽDC�A�ʒu�Ɏw�肳�ꂽ�r�b�g�}�b�v���������ށi�ϔ{�j
- ����	HDC hdc�F������DC, HBITMAP hbmp�F�f�ރr�b�g�}�b�v,
		POINT dst_pt�F������̈ʒu, POINT src_pt�F�������̈ʒu,int retio_percent:������{��
- �߂�l	NA
******************************************************************************************************/
void CHelper::put_bmp_stretch(HDC hdc, HBITMAP hbmp, POINT dst_pt, POINT src_pt, int retio_percent) {

	if ((hdc == NULL) | (hbmp == NULL)) return;

	BITMAP bmp;
	GetObject(hbmp, sizeof(BITMAP), &bmp);
	int BMP_W = (int)bmp.bmWidth;
	int BMP_H = (int)bmp.bmHeight;

	HDC hmdc = CreateCompatibleDC(hdc);
	SelectObject(hmdc, hbmp);

	StretchBlt(hdc, dst_pt.x, dst_pt.y, BMP_W*retio_percent / 100, BMP_H * retio_percent / 100, hmdc, src_pt.x, src_pt.y, BMP_W, BMP_H, SRCCOPY);
	DeleteDC(hmdc);

	return;
};

/****************************************************************************************************************************************************
���L���������m�ۂ���
- ����
	-���́@LPCTSTR szName�F���L��������, DWORD dwSize�F�m�ۂ���T�C�Y,
	-�o�́@HANDLE* hMapFile�F�t�@�C���}�b�v�I�u�W�F�N�g�n���h��, LPVOID* pMapTop:���L�������擪�A�h���X, DWORD* dwExist:�@GetLastError()�G���[�L��

- �߂�l�@0:���튮���@-1,-2:�ُ튮��
****************************************************************************************************************************************************/
int CHelper::cmnCreateShmem(LPCTSTR szName, DWORD dwSize, HANDLE* hMapFile, LPVOID* pMapTop, DWORD* dwExist) {
	DWORD	highByte = 0;			// ���L��������32bit�T�C�Y�ȓ���z��
	DWORD	lowByte = dwSize;		// 32bit�T�C�Y�ȏ�͒�`�s��

									// ������
	*hMapFile = NULL;
	*pMapTop = NULL;
	*dwExist = OFF;

	// �t�@�C���E�}�b�s���O�E�I�u�W�F�N�g���쐬(�y�[�W�t�@�C�����g�p ,�Z�L�����e�B�����Ȃ� ,�ǂݍ���/�������݃A�N�Z�X ,���L�������̃T�C�Y ,���L��������)
	*hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, highByte, lowByte, szName);
	if (*hMapFile == NULL) return(ERR_SHMEM_CREATE);
	else {
		if (GetLastError() == ERROR_ALREADY_EXISTS)	*dwExist = ON;
		// �t�@�C���E�r���[���쐬(���L�������̃n���h��, �A�N�Z�X�̎��, �t�@�C�� �I�t�Z�b�g(���32�r�b�g), �t�@�C�� �I�t�Z�b�g(����32�r�b�g), �}�b�v����o�C�g����0�̓t�@�C��ك}�b�s���O�I�u�W�F�N�g�S��)
		*pMapTop = MapViewOfFile(*hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (*pMapTop == NULL) {
			CloseHandle(*hMapFile);	// ���L�����������
			*hMapFile = NULL; *pMapTop = NULL;
			return(ERR_SHMEM_VIEW);
		}
	}
	return(OK_SHMEM);
}

/****************************************************************************************************************************************************
���L���������J������
- �����@HANDLE* hMapFile,�F�����t�@�C���}�b�v�I�u�W�F�N�g LPVOID* pMapTop�F����惁�����擪�A�h���X
- �߂�l�@0:���튮��,-2:�ُ튮��
****************************************************************************************************************************************************/
int	CHelper::cmnDeleteShMem(HANDLE* hMapFile, LPVOID* pMapTop)
{
	int		ret = OK_SHMEM;		// �֐��X�e�[�^�X
								
	if (*pMapTop != NULL) {
		if (!UnmapViewOfFile(*pMapTop)) {	// �t�@�C���E�r���[�̍폜
			ret = ERR_SHMEM_VIEW;
		}
	}

	// �t�@�C���E�}�b�s���O�E�I�u�W�F�N�g�����
	if (*hMapFile != NULL) {
		CloseHandle(*hMapFile);
	}

	// �|�C���^������
	*hMapFile = NULL;
	*pMapTop = NULL;

	return(ret);
}

