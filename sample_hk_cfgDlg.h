
// sample_hk_cfgDlg.h : ͷ�ļ�
//

#pragma once


// Csample_hk_cfgDlg �Ի���
class Csample_hk_cfgDlg : public CDialogEx
{
// ����
public:
	Csample_hk_cfgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SAMPLE_HK_CFG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
