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
 * VS2010自动生成的对话框类实现代码
 ******************************************************/

#include "stdafx.h"
#include "sample_hk_cfg.h"
#include "sample_hk_cfgDlg.h"
#include "afxdialogex.h"

#include "UtilTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_DEVICE_LOGIN_SUCCESS		(WM_USER + 107)

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

Csample_hk_cfgDlg::Csample_hk_cfgDlg(CWnd* pParent /*=NULL*/)
  : CDialogEx(Csample_hk_cfgDlg::IDD, pParent)
  , m_bIsExiting(false)
  , m_HKLoginIng(false)
  , m_dwHKErrCode(0)
  , m_HKLoginID(-1)
  , m_HKPlayID(-1)
{
	memset(&m_HKDeviceInfo, 0, sizeof(m_HKDeviceInfo));
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Csample_hk_cfgDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_HOME_WEBSITE, m_ctrlHome);
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Csample_hk_cfgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DEVICE_LOGIN_SUCCESS, &Csample_hk_cfgDlg::OnDeviceLoginSuccess)
	ON_BN_CLICKED(IDC_BUTTON_ASYNC, &Csample_hk_cfgDlg::OnBnClickedButtonAsync)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &Csample_hk_cfgDlg::OnBnClickedButtonLogout)
END_MESSAGE_MAP()


// Csample_hk_cfgDlg 消息处理程序

BOOL Csample_hk_cfgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 初始化海康SDK资源...
    NET_DVR_Init();

	// 初始化网络、线程、套接字...
	WORD	wsVersion = MAKEWORD(2, 2);
	WSADATA	wsData	  = {0};
	(void)::WSAStartup(wsVersion, &wsData);

	// 设置访问连接地址...
	m_ctrlHome.SetUnderline(TRUE);
	m_ctrlHome.SetURL("https://myhaoyi.com");
	m_ctrlHome.SetWindowText("联系我们 => https://myhaoyi.com");
	m_ctrlHome.SetLinkCursor(CUtilTool::GetSysHandCursor());
	m_ctrlHome.SetAutoSize();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

Csample_hk_cfgDlg::~Csample_hk_cfgDlg()
{
	// 释放海康SDK资源...
	NET_DVR_Cleanup();
	// 阻塞等待退出...
	this->WaitForExit();
	// 注销登录...
	this->ClearResource();
}

void Csample_hk_cfgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Csample_hk_cfgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Csample_hk_cfgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 点击“异步登录”按钮...
void Csample_hk_cfgDlg::OnBnClickedButtonAsync()
{
	if( m_HKLoginID > 0 ) {
		TRACE("=== 当前已经处于登录状态 ===\n");
		return;
	}
	ASSERT( m_HKLoginID <= 0 );
	CString strAddress("192.168.1.65");
	CString strUser("admin");
	CString strPass("admin123");
	int nCmdPort = 8000;
	DWORD dwErr = this->doDeviceLogin(strAddress, nCmdPort, strUser, strPass);
}

// 点击“立即注销”按钮...
void Csample_hk_cfgDlg::OnBnClickedButtonLogout()
{
	this->ClearResource();
}
//
// 执行DVR登录操作...
DWORD Csample_hk_cfgDlg::doDeviceLogin(LPCTSTR lpIPAddr, int nCmdPort, LPCTSTR lpUser, LPCTSTR lpPass)
{
	// 将摄像机的错误标志复位...
	m_dwHKErrCode = NET_DVR_NOERROR;
	// 登录之前，先释放资源，保存通知窗口...
	DWORD dwErr = GM_NoErr;
	this->ClearResource();
	// 异步方式登录DVR设备...
	NET_DVR_DEVICEINFO_V40  dvrDevV40 = {0};
	NET_DVR_USER_LOGIN_INFO dvrLoginInfo = {0};
	dvrLoginInfo.cbLoginResult = Csample_hk_cfgDlg::DeviceLoginResult;
	strcpy(dvrLoginInfo.sDeviceAddress, lpIPAddr);
	strcpy(dvrLoginInfo.sUserName, lpUser);
	strcpy(dvrLoginInfo.sPassword, lpPass);
	dvrLoginInfo.bUseAsynLogin = 1;
	dvrLoginInfo.wPort = nCmdPort;
	dvrLoginInfo.pUser = this;
	// 调用异步接口函数...
	if( NET_DVR_Login_V40(&dvrLoginInfo, &dvrDevV40) < 0 ) {
		dwErr = NET_DVR_GetLastError();
		MsgLogGM(dwErr);
	}
	// 设置正在异步登录中标志...
	m_HKLoginIng = true;
	// 如果调用失败，清除所有资源...
	if( dwErr != GM_NoErr ) {
		TRACE("=== 登录失败，错误号：%lu ===\n", dwErr);
		this->ClearResource();
		return dwErr;
	}
	// 打印正在登录状态...
	TRACE("=== 正在异步登录... ===\n");
	return GM_NoErr;
}
//
// 登录回调函数接口...
void CALLBACK Csample_hk_cfgDlg::DeviceLoginResult(LONG lUserID, DWORD dwResult, LPNET_DVR_DEVICEINFO_V30 lpDeviceInfo, void * pUser)
{
	Csample_hk_cfgDlg * lpDlg = (Csample_hk_cfgDlg*)pUser;
	lpDlg->onDeviceLoginAsync(lUserID, dwResult, lpDeviceInfo);
}
//
// 实际处理登录回调的函数...
void Csample_hk_cfgDlg::onDeviceLoginAsync(LONG lUserID, DWORD dwResult, LPNET_DVR_DEVICEINFO_V30 lpDeviceInfo)
{
	// 正在登录状态复位...
	m_HKLoginIng = false;
	// 处在退出状态中，直接返回...
	if( m_bIsExiting ) {
		TRACE("=== onDeviceLoginAsync => Exit ===\n");
		return;
	}
	// 登录失败的处理过程，需要通知上层窗口...
	if( dwResult <= 0 || lUserID < 0 ) {
		m_dwHKErrCode = NET_DVR_GetLastError();
		TRACE("=== 登录失败(%lu)，错误：%s ===\n", m_dwHKErrCode, NET_DVR_GetErrorMsg(&m_dwHKErrCode));
		// 释放该通道上的资源数据...
		this->ClearResource();
		MsgLogGM(m_dwHKErrCode);
		return;
	}
	// 登录成功之后，保存数据...
	m_dwHKErrCode = NET_DVR_NOERROR;
	this->m_HKLoginID = lUserID;
	memcpy(&m_HKDeviceInfo, lpDeviceInfo, sizeof(NET_DVR_DEVICEINFO_V30));
	// 通过异步消息，离开登录线程...
	::PostMessage(this->m_hWnd, WM_DEVICE_LOGIN_SUCCESS, NULL, NULL);
}
//
// 处理异步登录成功的消息通知...
LRESULT Csample_hk_cfgDlg::OnDeviceLoginSuccess(WPARAM wParam, LPARAM lParam)
{
	// 打印异步登录成功消息...
	TRACE("=== 异步登录成功... ===\n");
	// 进行IPC的配置和判断...
	return S_OK;
}
//
// 设备异常回调函数接口...
void CALLBACK Csample_hk_cfgDlg::DeviceException(DWORD dwType, LONG lUserID, LONG lHandle, void * pUser)
{
	Csample_hk_cfgDlg * lpDlg = (Csample_hk_cfgDlg*)pUser;
	lpDlg->onDeviceException(dwType, lUserID, lHandle);
}
//
// 处理设备异常的实际函数...
void Csample_hk_cfgDlg::onDeviceException(DWORD dwType, LONG lUserID, LONG lHandle)
{
	TRACE("=== Device Exception 0x%x ===\n", dwType);
}
//
// 等待异步登录退出 => 使用互斥不起作用...
void Csample_hk_cfgDlg::WaitForExit()
{
	m_bIsExiting = true;
	while( m_HKLoginIng ) {
		::Sleep(5);
	}
	ASSERT( !m_HKLoginIng );
}
//
// 释放建立资源...
void Csample_hk_cfgDlg::ClearResource()
{
	// 释放登录资源...
	if( m_HKLoginID >= 0 ) {
		NET_DVR_Logout_V30(m_HKLoginID);
		m_HKLoginID = -1;
		memset(&m_HKDeviceInfo, 0, sizeof(m_HKDeviceInfo));
	}
}
