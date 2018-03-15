/******************************************************
 * 浩一科技，提供云录播、云监控的全平台无插件解决方案。
 * 支持按需直播，多点布控，分布式海量存储，动态扩容；
 * 支持微信扫码登录，全平台帐号统一，关联微信小程序；
 * 支持多种数据输入：摄像头IPC、rtmp、rtsp、MP4文件；
 * 支持全实时、全动态、全网页管理，网页前后台兼容IE8；
 * 支持多终端无插件自适应播放，flvjs/hls/rtmp自动适配；
 ******************************************************
 * 官方网站 => https://myhaoyi.com
 * 技术博客 => http://blog.csdn.net/haoyitech
 * 开源代码 => https://github.com/HaoYiTech/
 * 云监控群 => 483663026（QQ群）
 * 云录播群 => 630379661（QQ群）
 ******************************************************
 * VS2010自动生成的对话框类头文件
 ******************************************************/

#pragma once

#include "HyperLink.h"
#include "HCNetSDK.h"

// Csample_hk_cfgDlg 对话框
class Csample_hk_cfgDlg : public CDialogEx
{
public:
	Csample_hk_cfgDlg(CWnd* pParent = NULL);	// 标准构造函数
	~Csample_hk_cfgDlg();
	enum { IDD = IDD_SAMPLE_HK_CFG_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
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
	void		WaitForExit();						// 等待异步登录退出...
	void		ClearResource();					// 释放建立资源...
private:
	static void CALLBACK DeviceLoginResult(LONG lUserID, DWORD dwResult, LPNET_DVR_DEVICEINFO_V30 lpDeviceInfo, void * pUser);
	static void CALLBACK DeviceException(DWORD dwType, LONG lUserID, LONG lHandle, void * pUser);
protected:
	HICON					m_hIcon;
	CHyperLink				m_ctrlHome;
private:
	LONG					m_dwHKErrCode;			// 记录海康错误码...
	BOOL					m_bIsExiting;			// 正在等待退出中...
	BOOL					m_HKLoginIng;			// 正在异步登录中...
	LONG					m_HKPlayID;				// 实时播放编号...
	LONG					m_HKLoginID;			// 海康登录编号...
	NET_DVR_DEVICEINFO_V30	m_HKDeviceInfo;			// 海康设备信息...
};
