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
 * VS2010�Զ����ɵĶԻ�����ʵ�ִ���
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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

// Csample_hk_cfgDlg ��Ϣ�������
BOOL Csample_hk_cfgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ��ʼ������SDK��Դ...
    NET_DVR_Init();

	// ��ʼ�����硢�̡߳��׽���...
	WORD	wsVersion = MAKEWORD(2, 2);
	WSADATA	wsData	  = {0};
	(void)::WSAStartup(wsVersion, &wsData);

	// ���÷������ӵ�ַ...
	m_ctrlHome.SetUnderline(TRUE);
	m_ctrlHome.SetURL("https://myhaoyi.com");
	m_ctrlHome.SetWindowText("��ϵ���� => https://myhaoyi.com");
	m_ctrlHome.SetLinkCursor(CUtilTool::GetSysHandCursor());
	m_ctrlHome.SetAutoSize();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

Csample_hk_cfgDlg::~Csample_hk_cfgDlg()
{
	// �ͷź���SDK��Դ...
	NET_DVR_Cleanup();
	// �����ȴ��˳�...
	this->WaitForExit();
	// ע����¼...
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Csample_hk_cfgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Csample_hk_cfgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ������첽��¼����ť...
void Csample_hk_cfgDlg::OnBnClickedButtonAsync()
{
	if( m_HKLoginID > 0 ) {
		TRACE("=== ��ǰ�Ѿ����ڵ�¼״̬ ===\n");
		return;
	}
	ASSERT( m_HKLoginID <= 0 );
	CString strAddress("192.168.1.65");
	CString strUser("admin");
	CString strPass("admin123");
	int nCmdPort = 8000;
	DWORD dwErr = this->doDeviceLogin(strAddress, nCmdPort, strUser, strPass);
}

// ���������ע������ť...
void Csample_hk_cfgDlg::OnBnClickedButtonLogout()
{
	this->ClearResource();
}
//
// ִ��DVR��¼����...
DWORD Csample_hk_cfgDlg::doDeviceLogin(LPCTSTR lpIPAddr, int nCmdPort, LPCTSTR lpUser, LPCTSTR lpPass)
{
	// ��������Ĵ����־��λ...
	m_dwHKErrCode = NET_DVR_NOERROR;
	// ��¼֮ǰ�����ͷ���Դ������֪ͨ����...
	DWORD dwErr = GM_NoErr;
	this->ClearResource();
	// �첽��ʽ��¼DVR�豸...
	NET_DVR_DEVICEINFO_V40  dvrDevV40 = {0};
	NET_DVR_USER_LOGIN_INFO dvrLoginInfo = {0};
	dvrLoginInfo.cbLoginResult = Csample_hk_cfgDlg::DeviceLoginResult;
	strcpy(dvrLoginInfo.sDeviceAddress, lpIPAddr);
	strcpy(dvrLoginInfo.sUserName, lpUser);
	strcpy(dvrLoginInfo.sPassword, lpPass);
	dvrLoginInfo.bUseAsynLogin = 1;
	dvrLoginInfo.wPort = nCmdPort;
	dvrLoginInfo.pUser = this;
	// �����첽�ӿں���...
	if( NET_DVR_Login_V40(&dvrLoginInfo, &dvrDevV40) < 0 ) {
		dwErr = NET_DVR_GetLastError();
		MsgLogGM(dwErr);
	}
	// ���������첽��¼�б�־...
	m_HKLoginIng = true;
	// �������ʧ�ܣ����������Դ...
	if( dwErr != GM_NoErr ) {
		TRACE("=== ��¼ʧ�ܣ�����ţ�%lu ===\n", dwErr);
		this->ClearResource();
		return dwErr;
	}
	// ��ӡ���ڵ�¼״̬...
	TRACE("=== �����첽��¼... ===\n");
	return GM_NoErr;
}
//
// ��¼�ص������ӿ�...
void CALLBACK Csample_hk_cfgDlg::DeviceLoginResult(LONG lUserID, DWORD dwResult, LPNET_DVR_DEVICEINFO_V30 lpDeviceInfo, void * pUser)
{
	Csample_hk_cfgDlg * lpDlg = (Csample_hk_cfgDlg*)pUser;
	lpDlg->onDeviceLoginAsync(lUserID, dwResult, lpDeviceInfo);
}
//
// ʵ�ʴ����¼�ص��ĺ���...
void Csample_hk_cfgDlg::onDeviceLoginAsync(LONG lUserID, DWORD dwResult, LPNET_DVR_DEVICEINFO_V30 lpDeviceInfo)
{
	// ���ڵ�¼״̬��λ...
	m_HKLoginIng = false;
	// �����˳�״̬�У�ֱ�ӷ���...
	if( m_bIsExiting ) {
		TRACE("=== onDeviceLoginAsync => Exit ===\n");
		return;
	}
	// ��¼ʧ�ܵĴ�����̣���Ҫ֪ͨ�ϲ㴰��...
	if( dwResult <= 0 || lUserID < 0 ) {
		m_dwHKErrCode = NET_DVR_GetLastError();
		TRACE("=== ��¼ʧ��(%lu)������%s ===\n", m_dwHKErrCode, NET_DVR_GetErrorMsg(&m_dwHKErrCode));
		// �ͷŸ�ͨ���ϵ���Դ����...
		this->ClearResource();
		MsgLogGM(m_dwHKErrCode);
		return;
	}
	// ��¼�ɹ�֮�󣬱�������...
	m_dwHKErrCode = NET_DVR_NOERROR;
	this->m_HKLoginID = lUserID;
	memcpy(&m_HKDeviceInfo, lpDeviceInfo, sizeof(NET_DVR_DEVICEINFO_V30));
	// ͨ���첽��Ϣ���뿪��¼�߳�...
	::PostMessage(this->m_hWnd, WM_DEVICE_LOGIN_SUCCESS, NULL, NULL);
}
//
// �����첽��¼�ɹ�����Ϣ֪ͨ...
LRESULT Csample_hk_cfgDlg::OnDeviceLoginSuccess(WPARAM wParam, LPARAM lParam)
{
	// ��ӡ�첽��¼�ɹ���Ϣ...
	TRACE("=== �첽��¼�ɹ�... ===\n");
	///////////////////////////////////////////////
	// ��IPC���ж�̬����...
	///////////////////////////////////////////////
	LONG dwErr = GM_NoErr;
	ASSERT( m_HKPlayID < 0 && m_HKLoginID >= 0 );
	do {
		// IPC�ĵ�һ��ͨ����һ��IPC�����ж��ͨ��...
		LONG  nDvrStartChan = m_HKDeviceInfo.byStartChan;
		DWORD dwReturn = 0;
		//(Ŀǰͨ��WEB����DVR) ��ȡ�豸���б������� => ��Ҫ����xml���ó����Զ�ѡ����û��ֶ�ѡ��д���豸����...
		/*TCHAR szEncAbility[MAX_PATH*40] = {0};
		if( !NET_DVR_GetDeviceAbility(m_HKLoginID, DEVICE_ENCODE_ALL_ABILITY, NULL, 0, szEncAbility, MAX_PATH*20) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		TRACE("�豸ѹ������ - V1\n%s\n", szEncAbility);
		// ��ȡ����Ƶѹ������...
		memset(szEncAbility, 0, MAX_PATH*40);
		TCHAR * lpInput = "<AudioVideoCompressInfo><AudioChannelNumber>1</AudioChannelNumber><VideoChannelNumber>1</VideoChannelNumber></AudioVideoCompressInfo>";
		if( !NET_DVR_GetDeviceAbility(m_HKLoginID, DEVICE_ENCODE_ALL_ABILITY_V20, lpInput, strlen(lpInput), szEncAbility, MAX_PATH*40) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		TRACE("�豸ѹ������ - V2\n%s\n", szEncAbility);*/
		// ��ȡIPC��rtsp�˿ں�...
		NET_DVR_RTSPCFG dvrRtsp = {0};
		if( !NET_DVR_GetRtspConfig(m_HKLoginID, 0, &dvrRtsp, sizeof(NET_DVR_RTSPCFG)) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// ������ȷ����ӡrtsp�˿�...
		TRACE("\nRTSP�˿ڣ�%lu\n", dvrRtsp.wPort);
		// ��ȡ��Ϊ������������ => NET_DVR_GetDeviceConfig(NET_DVR_GET_MULTI_STREAM_COMPRESSIONCFG) => NET_DVR_MULTI_STREAM_COMPRESSIONCFG_COND NET_DVR_MULTI_STREAM_COMPRESSIONCFG
		// ��ȡѹ�����ò�����Ϣ => ������ ������ �� ������ ...
		NET_DVR_COMPRESSIONCFG_V30 dvrCompressCfg = {0};
		if( !NET_DVR_GetDVRConfig(m_HKLoginID, NET_DVR_GET_COMPRESSCFG_V30, nDvrStartChan, &dvrCompressCfg, sizeof(dvrCompressCfg), &dwReturn) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// �ж�������������������Ƶ�����Ƿ���ȷ => ������
		if( dvrCompressCfg.struNormHighRecordPara.byStreamType != 1 ||
			dvrCompressCfg.struNetPara.byStreamType != 1 ) {
			dwErr = GM_DVR_VType_Err;
			MsgLogGM(dwErr);
			break;
		}
		// �ж�����������������Ƶ���������Ƿ���ȷ => H264
		if( dvrCompressCfg.struNormHighRecordPara.byVideoEncType != NET_DVR_ENCODER_H264 ||
			dvrCompressCfg.struNetPara.byVideoEncType != NET_DVR_ENCODER_H264 ) {
			dwErr = GM_DVR_VEnc_Err;
			MsgLogGM(dwErr);
			break;
		}
		TRACE("=== RTSP - Video is Encoder H264 ===\n");
		// �ж�����������������Ƶ���������Ƿ���ȷ => AAC
		// ��Ƶ��ʽ���ԣ�ֻ�Ǳ��������˳���RTSP�Ự���Զ�������Ƶ...
		if( dvrCompressCfg.struNormHighRecordPara.byAudioEncType != AUDIOTALKTYPE_AAC ||
			dvrCompressCfg.struNetPara.byAudioEncType != AUDIOTALKTYPE_AAC ) {
			MsgLogGM(GM_DVR_AEnc_Err);
			//dwErr = GM_DVR_AEnc_Err;
			//MsgLogGM(dwErr);
			//break;
		}
		TRACE("=== RTSP - Audio is Encoder AAC ===\n");
		// ����һЩ���� => �����豸�ı������������ã��������������õ�...
		// �������ļ��ж�ȡ��������������С����������С => �Զ�������...
		dvrCompressCfg.struNormHighRecordPara.dwVideoBitrate = 1024 * 1024;
		dvrCompressCfg.struNormHighRecordPara.dwVideoBitrate |= 0x80000000;
		dvrCompressCfg.struNetPara.dwVideoBitrate = 500 * 1024;
		dvrCompressCfg.struNetPara.dwVideoBitrate |= 0x80000000;
		// ���� ������ �� ������ �����ò�����Ϣ...
		if( !NET_DVR_SetDVRConfig(m_HKLoginID, NET_DVR_SET_COMPRESSCFG_V30, nDvrStartChan, &dvrCompressCfg, sizeof(dvrCompressCfg)) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		TRACE("=== ��������1Mbps����������500kbps ===\n");
		// ��ȡDVR�豸��ǰ�˲���...
		NET_DVR_CAMERAPARAMCFG dvrCCDParam = {0};
		if( !NET_DVR_GetDVRConfig(m_HKLoginID, NET_DVR_GET_CCDPARAMCFG, nDvrStartChan, &dvrCCDParam, sizeof(dvrCCDParam), &dwReturn) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// �Ծ���ģʽ���д��� => ����0 �ر�;1 ����;2 ����;3 �м�
		BOOL bOpenMirror = true;
		dvrCCDParam.byMirror = (bOpenMirror ? 3 : 0);
		if( !NET_DVR_SetDVRConfig(m_HKLoginID, NET_DVR_SET_CCDPARAMCFG, nDvrStartChan, &dvrCCDParam, sizeof(dvrCCDParam)) ) {
			dwErr = NET_DVR_GetLastError(); // ע���������ţ�NET_DVR_NETWORK_ERRORDATA
			MsgLogGM(dwErr);
			break;
		}
		// ��ȡͼ����� => OSD | ���� | ���� | ���� | ���� | ����
		NET_DVR_PICCFG_V40 dvrPicV40 = {0};
		if( !NET_DVR_GetDVRConfig(m_HKLoginID, NET_DVR_GET_PICCFG_V40, nDvrStartChan, &dvrPicV40, sizeof(dvrPicV40), &dwReturn) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// ��ͨ�������ļ��л�ȡ�Ƿ���OSD...
		BOOL bOpenOSD = true;
		// ����ͼ���ʽ => OSD | ���� | ���� | ���� | ���� | ����
		//strcpy((char*)dvrPicV40.sChanName, "Camera"); // ͨ������...
		//dvrPicV40.dwVideoFormat = 2; // ��Ƶ��ʽ��0- ��֧�֣�1- NTSC��2- PAL 
		//dvrPicV40.dwShowChanName = 0; // Ԥ����ͼ�����Ƿ���ʾͨ�����ƣ�0-����ʾ��1-��ʾ�������С704*576�� 
		//dvrPicV40.wShowNameTopLeftX = 200; // ͨ��������ʾλ�õ�x����
		//dvrPicV40.wShowNameTopLeftY = 100; // ͨ��������ʾλ�õ�y����
		//dvrPicV40.dwEnableHide = 1; // �Ƿ�������˽�ڱΣ�0-��1-��
		dvrPicV40.dwShowOsd = bOpenOSD; // Ԥ����ͼ�����Ƿ���ʾOSD��0-����ʾ��1-��ʾ�������С704*576��
		dvrPicV40.wOSDTopLeftX = 300; // OSD��x����
		dvrPicV40.wOSDTopLeftY = 20; // OSD��y����
		dvrPicV40.byOSDType = 2; // OSD����(�����ո�ʽ) 0��XXXX-XX-XX ������; 1��XX-XX-XXXX ������; 2��XXXX��XX��XX��; 3��XX��XX��XXXX��; 4��XX-XX-XXXX ������; 5��XX��XX��XXXX��; 6��xx/xx/xxxx ��/��/��; 7��xxxx/xx/xx ��/��/��; 8��xx/xx/xxxx ��/��/��
		dvrPicV40.byDispWeek = 0; // �Ƿ���ʾ���ڣ�0-����ʾ��1-��ʾ
		dvrPicV40.byOSDAttrib = 2; // OSD���ԣ�͸��/��˸����1��͸������˸��2��͸��������˸��3����˸����͸����4����͸��������˸
		dvrPicV40.byHourOSDType = 0; // Сʱ�ƣ�0��ʾ24Сʱ�ƣ�1��ʾ12Сʱ�ƻ�am/pm 
		dvrPicV40.byFontSize = 0xFF; // �����С��0- 16*16(��)/8*16(Ӣ)��1- 32*32(��)/16*32(Ӣ)��2- 64*64(��)/32*64(Ӣ)��3- 48*48(��)/24*48(Ӣ)��4- 24*24(��)/12*24(Ӣ)��5- 96*96(��)/48*96(Ӣ)��0xff- ����Ӧ
		dvrPicV40.byOSDColorType = 0; // OSD��ɫģʽ��0- Ĭ�ϣ��ڰף���1-�Զ���(��ɫ��struOsdColor)
		dvrPicV40.struOsdColor.byRed = 255;
		dvrPicV40.struOsdColor.byGreen = 0;
		dvrPicV40.struOsdColor.byBlue = 0;
		dvrPicV40.byAlignment = 0; // ���뷽ʽ��0- ����Ӧ��1- �Ҷ��룬2- �����
		if( !NET_DVR_SetDVRConfig(m_HKLoginID, NET_DVR_SET_PICCFG_V40, nDvrStartChan, &dvrPicV40, sizeof(dvrPicV40)) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// ��DVR�豸����Уʱ���� => ���óɸ�����ʱ��һ��...
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
		// �����豸�쳣��Ϣ�ص��ӿں���...
		if( !NET_DVR_SetExceptionCallBack_V30(0, NULL, Csample_hk_cfgDlg::DeviceException, this) ) {
			dwErr = NET_DVR_GetLastError();
			MsgLogGM(dwErr);
			break;
		}
		// ��ͨ�������л�ȡ�Ƿ�������Ԥ��...
		BOOL bPreview = true;
		// �����˿���Ԥ���������ʾ...
		if( bPreview ) {
			// ׼����ʾԤ��������Ҫ�Ĳ���...
			CWnd * lpPreview = this->GetDlgItem(IDC_PREVIEW);
			NET_DVR_CLIENTINFO dvrClientInfo = {0};
			dvrClientInfo.hPlayWnd     = lpPreview->m_hWnd;
			dvrClientInfo.lChannel     = nDvrStartChan;
			dvrClientInfo.lLinkMode    = 0;
			dvrClientInfo.sMultiCastIP = NULL;
			// ����ʵʱԤ���ӿ�...
			m_HKPlayID = NET_DVR_RealPlay_V30(m_HKLoginID, &dvrClientInfo, NULL, NULL, TRUE);
			if( m_HKPlayID < 0 ) {
				dwErr = NET_DVR_GetLastError();
				MsgLogGM(dwErr);
				break;
			}
		}
	}while(false);
	// �������ʧ�ܣ����������Դ...
	if( dwErr != GM_NoErr ) {
		// ֻ��IPC�ڲ������ӡ������Ϣ...
		if( NET_DVR_GetLastError() == dwErr ) {
			TRACE("=== ����ʧ��(%lu)������%s ===\n", dwErr, NET_DVR_GetErrorMsg(&dwErr));
		} else {
			TRACE("=== ����ʧ�ܣ�����ţ�%lu ===\n", dwErr);
		}
		// �ͷ�IPC�ѵ�¼��Դ...
		this->ClearResource();
		return dwErr;
	}
	// ��ӡ�������֪ͨ...
	TRACE("=== ���óɹ� ===\n");
	return S_OK;
}
//
// �豸�쳣�ص������ӿ�...
void CALLBACK Csample_hk_cfgDlg::DeviceException(DWORD dwType, LONG lUserID, LONG lHandle, void * pUser)
{
	Csample_hk_cfgDlg * lpDlg = (Csample_hk_cfgDlg*)pUser;
	lpDlg->onDeviceException(dwType, lUserID, lHandle);
}
//
// �����豸�쳣��ʵ�ʺ���...
void Csample_hk_cfgDlg::onDeviceException(DWORD dwType, LONG lUserID, LONG lHandle)
{
	TRACE("=== Device Exception 0x%x ===\n", dwType);
}
//
// �ȴ��첽��¼�˳� => ʹ�û��ⲻ������...
void Csample_hk_cfgDlg::WaitForExit()
{
	m_bIsExiting = true;
	while( m_HKLoginIng ) {
		::Sleep(5);
	}
	ASSERT( !m_HKLoginIng );
}
//
// �ͷŽ�����Դ...
void Csample_hk_cfgDlg::ClearResource()
{
	// �ͷ�����¼����Դ��ʵʱԤ����Դ...
	if( m_HKPlayID >= 0 ) {
		NET_DVR_StopRealPlay(m_HKPlayID);
		m_HKPlayID = -1;
	}
	// �ͷŵ�¼��Դ...
	if( m_HKLoginID >= 0 ) {
		NET_DVR_Logout_V30(m_HKLoginID);
		m_HKLoginID = -1;
		memset(&m_HKDeviceInfo, 0, sizeof(m_HKDeviceInfo));
	}
}
