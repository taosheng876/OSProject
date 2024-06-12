
// OSProjectDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "OSProject.h"
#include "OSProjectDlg.h"
#include "afxdialogex.h"
#include "process.h"
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COSProjectDlg 对话框



COSProjectDlg::COSProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OSPROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COSProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON3, m_startB);
	DDX_Control(pDX, IDC_BUTTON4, m_pauseB);
	DDX_Control(pDX, IDC_LIST1, m_runingP);
	DDX_Control(pDX, IDC_LIST5, m_pReady);
	DDX_Control(pDX, IDC_LIST4, m_pBackupReady);
	DDX_Control(pDX, IDC_LIST3, m_pInputWaitting);
	DDX_Control(pDX, IDC_LIST7, m_pOutputWaitting);
	DDX_Control(pDX, IDC_LIST6, m_pPureWaitting);
	DDX_Text(pDX, IDC_EDIT1, m_insRunTime);
}

BEGIN_MESSAGE_MAP(COSProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &COSProjectDlg::openFile)
	ON_BN_CLICKED(IDC_BUTTON3, &COSProjectDlg::startDiaodu)
	ON_BN_CLICKED(IDC_BUTTON4, &COSProjectDlg::pauseDiaodu)
	ON_LBN_SELCHANGE(IDC_LIST1, &COSProjectDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// COSProjectDlg 消息处理程序

BOOL COSProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COSProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COSProjectDlg::OnPaint()
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
HCURSOR COSProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT ThreadProc(LPVOID lpParam) {
	COSProjectDlg* d = (COSProjectDlg*)lpParam;
	m_pReadyPCBs = m_pBackupReadyPCBs;
	m_pBackupReadyPCBs = NULL;
	while (1) {
		if (m_pReadyPCBs == NULL && m_pInputWaittingPCBs == NULL && m_pOutputWaittingPCBs == NULL && m_pPureWaittingPCBs == NULL) {
			d->m_runingP.ResetContent();
			d->m_pReady.ResetContent();
			d->m_pInputWaitting.ResetContent();
			d->m_pOutputWaitting.ResetContent();
			d->m_pPureWaitting.ResetContent();
			d->MessageBox(L"进程调度完毕！");
			break;
		}
		d->updataAll();
		if (!d->InitiaDuilie()) continue;
		PCB* P;
		P = m_pReadyPCBs;
		RunOneTimeRange(P, 1);
		P = m_pInputWaittingPCBs;
		while (P != NULL) {
			RunOneTimeRange(P, 2);
			P = P->m_pNextPCB;
		}
		P = m_pOutputWaittingPCBs;
		while (P != NULL) {
			RunOneTimeRange(P, 3);
			P = P->m_pNextPCB;
		}
		P = m_pPureWaittingPCBs;
		while (P != NULL) {
			RunOneTimeRange(P, 4);
			P = P->m_pNextPCB;
		}
		CString str1 = _T("\n");
		ofs.Write(str1.GetBuffer(), str1.GetLength());
		Sleep(d->m_insRunTime);
	}
	return 0;
}


void COSProjectDlg::openFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_strFileOut = _T("");
	TCHAR szFilter[] = _T("");
	CFileDialog file(true, NULL, NULL, 0, szFilter, this);
	if (file.DoModal()) {
		m_fileName = file.GetPathName();
	}
	else {
		exit(0);
	}
	CStdioFile fp;
	int ID = 1;
	CString str;
	if (!fp.Open(m_fileName, CFile::modeRead | CFile::typeText)) {
		return;
	}
	while (fp.ReadString(str)) {
		if (str.Find('P') == 0) {
			PCB* P = new PCB(ID , str);
			ID++;
			if (m_pBackupReadyPCBs == NULL) {
				m_pBackupReadyPCBs = P;
			}
			else {
				PCB* tail = m_pBackupReadyPCBs;
				while (tail->m_pNextPCB != NULL) {
					tail = tail->m_pNextPCB;
				}
				tail->m_pNextPCB = P;
			}
			m_pBackupReady.AddString(str);
		}
		else {
			PCB* now = m_pBackupReadyPCBs;
			while (now->m_pNextPCB != NULL) {
				now = now->m_pNextPCB;
			}
			int sum = 0;
			char buf[5] = {};
			string ctr = CStringA(str.Right(2));
			strncpy_s(buf, _countof(buf), ctr.c_str(), str.Right(2).GetLength());
			sum = atoi(buf);
			CInstruction* Ins = new CInstruction(str.Left(1), sum);
			if (now->m_pInstructionList == NULL) {
				now->m_pInstructionList = Ins;
				now->m_pInstructionList->m_pNextInstruction = NULL;
			}
			else {
				CInstruction* tail = now->m_pInstructionList;
				while (tail->m_pNextInstruction != NULL) {
					tail = tail->m_pNextInstruction;
				}
				tail->m_pNextInstruction = Ins;
			}
		}
	}
	m_startB.EnableWindow(true);
	m_pauseB.EnableWindow(false);
}

void COSProjectDlg::startDiaodu()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_insRunTime == 0) {
		MessageBox(L"请输入时间片时间大小！");
		return;
	}
	if (myThread == NULL) {
		myThread = AfxBeginThread(ThreadProc, this);
		m_pauseB.EnableWindow(true);
		m_startB.EnableWindow(false);
	}
	else {
		myThread->ResumeThread();
		m_pauseB.EnableWindow(true);
		m_startB.EnableWindow(false);
	}
}


void COSProjectDlg::pauseDiaodu()
{
	// TODO: 在此添加控件通知处理程序代码
	myThread->SuspendThread();
	m_startB.EnableWindow(true);
	m_pauseB.EnableWindow(false);
}


void COSProjectDlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
}

void COSProjectDlg::updataAll() //更新所有队列
{
	PCB* P = new PCB();
	m_runingP.ResetContent();
	if (m_pReadyPCBs != NULL) m_runingP.AddString(m_pReadyPCBs->m_csProcessName);
	m_pReady.ResetContent();
	if (m_pReadyPCBs != NULL && m_pReadyPCBs->m_pNextPCB != NULL) {
		P = m_pReadyPCBs->m_pNextPCB;
		while (P != NULL) {
			m_pReady.AddString(P->m_csProcessName);
			P = P->m_pNextPCB;
		}
	}
	m_pBackupReady.ResetContent();
	P = m_pBackupReadyPCBs;
	while (P != NULL) {
		m_pBackupReady.AddString(P->m_csProcessName);
		P = P->m_pNextPCB;
	}
	m_pInputWaitting.ResetContent();
	P = m_pInputWaittingPCBs;
	while (P != NULL) {
		m_pInputWaitting.AddString(P->m_csProcessName);
		P = P->m_pNextPCB;
	}
	m_pOutputWaitting.ResetContent();
	P = m_pOutputWaittingPCBs;
	while (P != NULL) {
		m_pOutputWaitting.AddString(P->m_csProcessName);
		P = P->m_pNextPCB;
	}
	m_pPureWaitting.ResetContent();
	P = m_pPureWaittingPCBs;
	while (P != NULL) {
		m_pPureWaitting.AddString(P->m_csProcessName);
		P = P->m_pNextPCB;
	}
}

BOOL COSProjectDlg::InitiaDuilie() {
	PCB* P = m_pReadyPCBs;
	if (P == NULL) return true;
	if (P->m_pInstructionList == NULL) return true;
	if (P->m_pInstructionList->m_nInstructionID == 'I') {
		RemoveProcess(P, 1);
		AppendInputQueue(P);
		return false;
	}
	if (P->m_pInstructionList->m_nInstructionID == 'O') {
		RemoveProcess(P, 1);
		AppendOutputQueue(P);
		return false;
	}
	if (P->m_pInstructionList->m_nInstructionID == 'W') {
		RemoveProcess(P, 1);
		AppendWaitQueue(P);
		return false;
	}
	if (P->m_pInstructionList->m_nInstructionID == 'H') {
		RemoveProcess(P, 1);
		return false;
	}
	return true;
}