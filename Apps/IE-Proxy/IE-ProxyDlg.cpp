// IE-ProxyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IE-Proxy.h"
#include "IE-ProxyDlg.h"
#include "Ini/SimpleIni.h"
#include "Net/InetUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace utils;

CSimpleIni gSimpleIni;

// CIEProxyDlg dialog

CIEProxyDlg::CIEProxyDlg(CWnd* pParent /*=NULL*/)
: CDialog(CIEProxyDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIEProxyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATUS, m_stcStatus);
}

BEGIN_MESSAGE_MAP(CIEProxyDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_NO_PROXY, OnBnClickedBtnNoProxy)
    ON_BN_CLICKED(IDC_BTN_PROXY1, OnBnClickedBtnProxy1)
    ON_BN_CLICKED(IDC_BTN_PROXY2, OnBnClickedBtnProxy2)
    ON_BN_CLICKED(IDC_BTN_PROXY3, OnBnClickedBtnProxy3)
    ON_BN_CLICKED(IDC_BTN_SYS_PROXY_SETTING, OnBnClickedBtnSysProxySetting)
    ON_MESSAGE(WM_GET_PROXY_STATUS, OnGetProxyStatus) 
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CIEProxyDlg message handlers

BOOL CIEProxyDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here
    m_stcStatus.SetWindowText(_T("Retrieving IE proxy settings..."));
    this->PostMessage(WM_GET_PROXY_STATUS);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIEProxyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        // CAboutDlg dlgAbout;
        // dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIEProxyDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIEProxyDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



BOOL CIEProxyDlg::OnBnClickedBtnProxy_X(int num)
{
    BOOL res;
    CWaitCursor wait;

    m_stcStatus.SetWindowText(_T("Changing IE proxy settings..."));

    CStringA strProxy;
    CStringA strBypass;

    switch(num)
    {
    case 0:
        res = EnableIEProxy(FALSE);
        break;

    case 1:
    case 2:
    case 3:
        if (num == 1)
            strProxy = "wwwgate0.mot.com";
        else if (num == 2)
            strProxy = "wwwgate0-ch.mot.com";
        else if (num == 3)
            strProxy = "127.0.0.1";

        strBypass = gSimpleIni.GetString(strProxy, "bypass", "no change").c_str();
        if (strBypass != "no change")
            res = EnableIEProxy(TRUE, strProxy, 1080, strBypass);
        else
            res = EnableIEProxy(TRUE, strProxy, 1080, NULL);
        break;

    default:
        res = FALSE;
        break;
    }

    if (res == TRUE)
    {
        m_stcStatus.SetWindowText(_T("New IE proxy is set successfully!\nThis program will be closed in few seconds."));
        this->SetTimer(1, 2500, NULL);
    }
    else
    {
        this->PostMessage(WM_GET_PROXY_STATUS);
    }

    return res;
}

void CIEProxyDlg::OnBnClickedBtnNoProxy()
{
    OnBnClickedBtnProxy_X(0);
}

void CIEProxyDlg::OnBnClickedBtnProxy1()
{
    OnBnClickedBtnProxy_X(1);
}

void CIEProxyDlg::OnBnClickedBtnProxy2()
{
    OnBnClickedBtnProxy_X(2);
}

void CIEProxyDlg::OnBnClickedBtnProxy3()
{
    OnBnClickedBtnProxy_X(3);
}

void CIEProxyDlg::OnBnClickedBtnSysProxySetting()
{
    ShowNetworkProxySettings();
}

LRESULT CIEProxyDlg::OnGetProxyStatus(WPARAM wParam, LPARAM lParam)
{
    bool bEnable;
    std::string proxy, byPass;
    BOOL res = GetIEProxy(bEnable, proxy, byPass);

    if (res)
    {
        CString status, proxyT(proxy.c_str()), byPassT(byPass.c_str());
        if (!bEnable)
            status = "IE proxy server is disabled.";
        else
            status.Format(_T("Current Proxy: %s"), proxyT); 
        m_stcStatus.SetWindowText(status);
    }

    return res;
}

void CIEProxyDlg::OnTimer(UINT nIDEvent)
{
    CDialog::OnTimer(nIDEvent);
    CIEProxyDlg::OnOK();
}
