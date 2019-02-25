#pragma once

//  ���L�������X�e�[�^�X
#define	OK_SHMEM			0			// ���L������ ����/�j������
#define	ERR_SHMEM_CREATE	-1			// ���L������ Create�ُ�
#define	ERR_SHMEM_VIEW		-2			// ���L������ View�ُ�

#define	ON				1		// ON
#define	OFF				0		// OFF

using namespace std;

class CHelper
{
public:
	CHelper();
	~CHelper();
	void Str2Wstr(const std::string &src, std::wstring &dest);
	void put_bmp_built(HDC hdc, HBITMAP hbmp, POINT dst_pt, POINT src_pt);
	void put_bmp_stretch(HDC hdc, HBITMAP hbmp, POINT dst_pt, POINT src_pt, int retio_persent);
	int cmnCreateShmem(LPCTSTR, DWORD, HANDLE*, LPVOID*, DWORD*);	//���L�������ݒ�
	int	cmnDeleteShMem(HANDLE*, LPVOID*);							//���L����������
};
