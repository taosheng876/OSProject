
// OSProjectDlg.h: 头文件
//

#pragma once


// COSProjectDlg 对话框
class COSProjectDlg : public CDialogEx
{
// 构造
public:
	COSProjectDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OSPROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void openFile();
	afx_msg void startDiaodu();
	void updataAll();
	BOOL InitiaDuilie();
	CButton m_startB;
	afx_msg void pauseDiaodu();
	CButton m_pauseB;
	CString m_fileName;
	//CString m_runingP;
	CListBox m_runingP;
	CWinThread* myThread;
	afx_msg void OnLbnSelchangeList1();
	CListBox m_pReady;
	CListBox m_pBackupReady;
	CListBox m_pInputWaitting;
	CListBox m_pOutputWaitting;
	CListBox m_pPureWaitting;
	int m_insRunTime=100;
};
