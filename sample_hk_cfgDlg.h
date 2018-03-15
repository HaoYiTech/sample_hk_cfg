/******************************************************
 * ��һ�Ƽ����ṩ��¼�����Ƽ�ص�ȫƽ̨�޲�����������
 * ֧�ְ���ֱ������㲼�أ��ֲ�ʽ�����洢����̬���ݣ�
 * ֧��΢��ɨ���¼��ȫƽ̨�ʺ�ͳһ������΢��С����
 * ֧�ֶ����������룺����ͷIPC��rtmp��rtsp��MP4�ļ���
 * ֧��ȫʵʱ��ȫ��̬��ȫ��ҳ������ҳǰ��̨����IE8��
 * ֧�ֶ��ն��޲������Ӧ���ţ�flvjs/hls/rtmp�Զ����䣻
 ******************************************************
 * �ٷ���վ => https://myhaoyi.com
 * �������� => http://blog.csdn.net/haoyitech
 * ��Դ���� => https://github.com/HaoYiTech/
 * �Ƽ��Ⱥ => 483663026��QQȺ��
 * ��¼��Ⱥ => 630379661��QQȺ��
 ******************************************************
 * VS2010�Զ����ɵĶԻ�����ͷ�ļ�
 ******************************************************/

#pragma once

#include "HyperLink.h"
#include "HCNetSDK.h"

// Csample_hk_cfgDlg �Ի���
class Csample_hk_cfgDlg : public CDialogEx
{
public:
	Csample_hk_cfgDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~Csample_hk_cfgDlg();
	enum { IDD = IDD_SAMPLE_HK_CFG_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonLogout();
	afx_msg void OnBnClickedButtonAsync();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT	OnDeviceLoginSuccess(WPARAM wParam, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	DWORD		doDeviceLogin(LPCTSTR lpIPAddr, int nCmdPort, LPCTSTR lpUser, LPCTSTR lpPass);
	void		onDeviceLoginAsync(LONG lUserID, DWORD dwResult, LPNET_DVR_DEVICEINFO_V30 lpDeviceInfo);
	void		onDeviceException(DWORD dwType, LONG lUserID, LONG lHandle);
	void		WaitForExit();						// �ȴ��첽��¼�˳�...
	void		ClearResource();					// �ͷŽ�����Դ...
private:
	static void CALLBACK DeviceLoginResult(LONG lUserID, DWORD dwResult, LPNET_DVR_DEVICEINFO_V30 lpDeviceInfo, void * pUser);
	static void CALLBACK DeviceException(DWORD dwType, LONG lUserID, LONG lHandle, void * pUser);
protected:
	HICON					m_hIcon;
	CHyperLink				m_ctrlHome;
private:
	LONG					m_dwHKErrCode;			// ��¼����������...
	BOOL					m_bIsExiting;			// ���ڵȴ��˳���...
	BOOL					m_HKLoginIng;			// �����첽��¼��...
	LONG					m_HKPlayID;				// ʵʱ���ű��...
	LONG					m_HKLoginID;			// ������¼���...
	NET_DVR_DEVICEINFO_V30	m_HKDeviceInfo;			// �����豸��Ϣ...
};
