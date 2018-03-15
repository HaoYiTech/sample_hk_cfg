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
	///////////////////////////////////////////////
	// 对IPC进行动态配置...
	///////////////////////////////////////////////
	LONG dwErr = GM_NoErr;
	ASSERT( m_HKPlayID < 0 && m_HKLoginID >= 0 );
	do {
		// IPC的第一个通道，一个IPC可能有多个通道...
		LONG  nDvrStartChan = m_HKDeviceInfo.byStartChan;
		DWORD dwReturn = 0;
		//(目前通过WEB配置DVR) 获取设备所有编码能力 => 需要解析xml，让程序自动选择或用户手动选择，写入设备当中...
		/*TCHAR szEncAbility[MAX_PATH*40] = {0};
		if( !NET_DVR_GetDeviceAbility(m_HKLoginID, DEVICE_ENCODE_ALL_ABILITY, NULL, 0, szEncAbility, MAX_PATH*20) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		TRACE("设备压缩能力 - V1\n%s\n", szEncAbility);
		// 获取音视频压缩能力...
		memset(szEncAbility, 0, MAX_PATH*40);
		TCHAR * lpInput = "<AudioVideoCompressInfo><AudioChannelNumber>1</AudioChannelNumber><VideoChannelNumber>1</VideoChannelNumber></AudioVideoCompressInfo>";
		if( !NET_DVR_GetDeviceAbility(m_HKLoginID, DEVICE_ENCODE_ALL_ABILITY_V20, lpInput, strlen(lpInput), szEncAbility, MAX_PATH*40) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		TRACE("设备压缩能力 - V2\n%s\n", szEncAbility);*/
		// 获取IPC的rtsp端口号...
		NET_DVR_RTSPCFG dvrRtsp = {0};
		if( !NET_DVR_GetRtspConfig(m_HKLoginID, 0, &dvrRtsp, sizeof(NET_DVR_RTSPCFG)) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// 调用正确，打印rtsp端口...
		TRACE("\nRTSP端口：%lu\n", dvrRtsp.wPort);
		// 获取更为多码流的配置 => NET_DVR_GetDeviceConfig(NET_DVR_GET_MULTI_STREAM_COMPRESSIONCFG) => NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND NET_DVR_MULTI_STREAM_COMPRESSIONCFG
		// 获取压缩配置参数信息 => 包含了 主码流 和 子码流 ...
		NET_DVR_COMPRESSIONCFG_V30 dvrCompressCfg = {0};
		if( !NET_DVR_GetDVRConfig(m_HKLoginID, NET_DVR_GET_COMPRESSCFG_V30, nDvrStartChan, &dvrCompressCfg, sizeof(dvrCompressCfg), &dwReturn) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// 判断主码流或子码流的视频类型是否正确 => 复合流
		if( dvrCompressCfg.struNormHighRecordPara.byStreamType != 1 ||
			dvrCompressCfg.struNetPara.byStreamType != 1 ) {
			dwErr = GM_DVR_VType_Err;
			MsgLogGM(dwErr);
			break;
		}
		// 判断主码流或子码流视频编码类型是否正确 => H264
		if( dvrCompressCfg.struNormHighRecordPara.byVideoEncType != NET_DVR_ENCODER_H264 ||
			dvrCompressCfg.struNetPara.byVideoEncType != NET_DVR_ENCODER_H264 ) {
			dwErr = GM_DVR_VEnc_Err;
			MsgLogGM(dwErr);
			break;
		}
		TRACE("=== RTSP - Video is Encoder H264 ===\n");
		// 判断主码流或子码流音频编码类型是否正确 => AAC
		// 音频格式不对，只是报错，但不退出，RTSP会话会自动丢弃音频...
		if( dvrCompressCfg.struNormHighRecordPara.byAudioEncType != AUDIOTALKTYPE_AAC ||
			dvrCompressCfg.struNetPara.byAudioEncType != AUDIOTALKTYPE_AAC ) {
			MsgLogGM(GM_DVR_AEnc_Err);
			//dwErr = GM_DVR_AEnc_Err;
			//MsgLogGM(dwErr);
			//break;
		}
		TRACE("=== RTSP - Audio is Encoder AAC ===\n");
		// 调整一些参数 => 根据设备的编码能力来设置，而不是随意设置的...
		// 从配置文件中读取并设置主码流大小和子码流大小 => 自定义码流...
		dvrCompressCfg.struNormHighRecordPara.dwVideoBitrate = 1024 * 1024;
		dvrCompressCfg.struNormHighRecordPara.dwVideoBitrate |= 0x80000000;
		dvrCompressCfg.struNetPara.dwVideoBitrate = 500 * 1024;
		dvrCompressCfg.struNetPara.dwVideoBitrate |= 0x80000000;
		// 设置 主码流 和 子码流 的配置参数信息...
		if( !NET_DVR_SetDVRConfig(m_HKLoginID, NET_DVR_SET_COMPRESSCFG_V30, nDvrStartChan, &dvrCompressCfg, sizeof(dvrCompressCfg)) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		TRACE("=== 主码流：1Mbps，子码流：500kbps ===\n");
		// 获取DVR设备的前端参数...
		NET_DVR_CAMERAPARAMCFG dvrCCDParam = {0};
		if( !NET_DVR_GetDVRConfig(m_HKLoginID, NET_DVR_GET_CCDPARAMCFG, nDvrStartChan, &dvrCCDParam, sizeof(dvrCCDParam), &dwReturn) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// 对镜像模式进行处理 => 镜像：0 关闭;1 左右;2 上下;3 中间
		BOOL bOpenMirror = true;
		dvrCCDParam.byMirror = (bOpenMirror ? 3 : 0);
		if( !NET_DVR_SetDVRConfig(m_HKLoginID, NET_DVR_SET_CCDPARAMCFG, nDvrStartChan, &dvrCCDParam, sizeof(dvrCCDParam)) ) {
			dwErr = NET_DVR_GetLastError(); // 注意这个错误号：NET_DVR_NETWORK_ERRORDATA
			MsgLogGM(dwErr);
			break;
		}
		// 获取图像参数 => OSD | 坐标 | 日期 | 星期 | 字体 | 属性
		NET_DVR_PICCFG_V40 dvrPicV40 = {0};
		if( !NET_DVR_GetDVRConfig(m_HKLoginID, NET_DVR_GET_PICCFG_V40, nDvrStartChan, &dvrPicV40, sizeof(dvrPicV40), &dwReturn) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// 从通道配置文件中获取是否开启OSD...
		BOOL bOpenOSD = true;
		// 设置图像格式 => OSD | 坐标 | 日期 | 星期 | 字体 | 属性
		//strcpy((char*)dvrPicV40.sChanName, "Camera"); // 通道名称...
		//dvrPicV40.dwVideoFormat = 2; // 视频制式：0- 不支持，1- NTSC，2- PAL 
		//dvrPicV40.dwShowChanName = 0; // 预览的图象上是否显示通道名称：0-不显示，1-显示（区域大小704*576） 
		//dvrPicV40.wShowNameTopLeftX = 200; // 通道名称显示位置的x坐标
		//dvrPicV40.wShowNameTopLeftY = 100; // 通道名称显示位置的y坐标
		//dvrPicV40.dwEnableHide = 1; // 是否启动隐私遮蔽：0-否，1-是
		dvrPicV40.dwShowOsd = bOpenOSD; // 预览的图象上是否显示OSD：0-不显示，1-显示（区域大小704*576）
		dvrPicV40.wOSDTopLeftX = 300; // OSD的x坐标
		dvrPicV40.wOSDTopLeftY = 20; // OSD的y坐标
		dvrPicV40.byOSDType = 2; // OSD类型(年月日格式) 0－XXXX-XX-XX 年月日; 1－XX-XX-XXXX 月日年; 2－XXXX年XX月XX日; 3－XX月XX日XXXX年; 4－XX-XX-XXXX 日月年; 5－XX日XX月XXXX年; 6－xx/xx/xxxx 月/日/年; 7－xxxx/xx/xx 年/月/日; 8－xx/xx/xxxx 日/月/年
		dvrPicV40.byDispWeek = 0; // 是否显示星期：0-不显示，1-显示
		dvrPicV40.byOSDAttrib = 2; // OSD属性（透明/闪烁）：1－透明，闪烁；2－透明，不闪烁；3－闪烁，不透明；4－不透明，不闪烁
		dvrPicV40.byHourOSDType = 0; // 小时制：0表示24小时制，1表示12小时制或am/pm 
		dvrPicV40.byFontSize = 0xFF; // 字体大小：0- 16*16(中)/8*16(英)，1- 32*32(中)/16*32(英)，2- 64*64(中)/32*64(英)，3- 48*48(中)/24*48(英)，4- 24*24(中)/12*24(英)，5- 96*96(中)/48*96(英)，0xff- 自适应
		dvrPicV40.byOSDColorType = 0; // OSD颜色模式：0- 默认（黑白），1-自定义(颜色见struOsdColor)
		dvrPicV40.struOsdColor.byRed = 255;
		dvrPicV40.struOsdColor.byGreen = 0;
		dvrPicV40.struOsdColor.byBlue = 0;
		dvrPicV40.byAlignment = 0; // 对齐方式：0- 自适应，1- 右对齐，2- 左对齐
		if( !NET_DVR_SetDVRConfig(m_HKLoginID, NET_DVR_SET_PICCFG_V40, nDvrStartChan, &dvrPicV40, sizeof(dvrPicV40)) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// 对DVR设备进行校时操作 => 设置成跟电脑时间一致...
		NET_DVR_TIME dvrTime = {0};
		CTime curTime = CTime::GetCurrentTime();
		dvrTime.dwYear = curTime.GetYear();
		dvrTime.dwMonth = curTime.GetMonth();
		dvrTime.dwDay = curTime.GetDay();
		dvrTime.dwHour = curTime.GetHour();
		dvrTime.dwMinute = curTime.GetMinute();
		dvrTime.dwSecond = curTime.GetSecond();
		if( !NET_DVR_SetDVRConfig(m_HKLoginID, NET_DVR_SET_TIMECFG, 0, &dvrTime, sizeof(dvrTime)) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// 设置设备异常消息回调接口函数...
		if( !NET_DVR_SetExceptionCallBack_V30(0, NULL, Csample_hk_cfgDlg::DeviceException, this) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// 从通道配置中获取是否开启本地预览...
		BOOL bPreview = true;
		// 配置了可以预览画面才显示...
		if( bPreview ) {
			// 准备显示预览画面需要的参数...
			CWnd * lpPreview = this->GetDlgItem(IDC_PREVIEW);
			NET_DVR_CLIENTINFO dvrClientInfo = {0};
			dvrClientInfo.hPlayWnd     = lpPreview->m_hWnd;
			dvrClientInfo.lChannel     = nDvrStartChan;
			dvrClientInfo.lLinkMode    = 0;
			dvrClientInfo.sMultiCastIP = NULL;
			// 调用实时预览接口...
			m_HKPlayID = NET_DVR_RealPlay_V30(m_HKLoginID, &dvrClientInfo, NULL, NULL, TRUE);
			if( m_HKPlayID < 0 ) {
				dwErr = NET_DVR_GetLastError();
				MsgLogGM(dwErr);
				break;
			}
		}
	}while(false);
	// 如果调用失败，清除所有资源...
	if( dwErr != GM_NoErr ) {
		// 只对IPC内部错误打印错误消息...
		if( NET_DVR_GetLastError() == dwErr ) {
			TRACE("=== 配置失败(%lu)，错误：%s ===\n", dwErr, NET_DVR_GetErrorMsg(&dwErr));
		} else {
			TRACE("=== 配置失败，错误号：%lu ===\n", dwErr);
		}
		// 释放IPC已登录资源...
		this->ClearResource();
		return dwErr;
	}
	// 打印配置完毕通知...
	TRACE("=== 配置成功 ===\n");
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
	// 释放正在录像资源，实时预览资源...
	if( m_HKPlayID >= 0 ) {
		NET_DVR_StopRealPlay(m_HKPlayID);
		m_HKPlayID = -1;
	}
	// 释放登录资源...
	if( m_HKLoginID >= 0 ) {
		NET_DVR_Logout_V30(m_HKLoginID);
		m_HKLoginID = -1;
		memset(&m_HKDeviceInfo, 0, sizeof(m_HKDeviceInfo));
	}
}
