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
	// ����IPC�����ú��ж�...
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
	// �ͷŵ�¼��Դ...
	if( m_HKLoginID >= 0 ) {
		NET_DVR_Logout_V30(m_HKLoginID);
		m_HKLoginID = -1;
		memset(&m_HKDeviceInfo, 0, sizeof(m_HKDeviceInfo));
	}
}
