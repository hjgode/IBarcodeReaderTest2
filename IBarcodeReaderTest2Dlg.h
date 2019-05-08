// IBarcodeReaderTest2Dlg.h : header file
//

#pragma once

#include <IBarCodeReader.h>
#pragma comment (lib, "itcuuid.lib")

#define WM_ITC_RECEIVED_BARCODE	WM_USER + 20

// CIBarcodeReaderTest2Dlg dialog
class CIBarcodeReaderTest2Dlg : public CDialog
{
// Construction
public:
	IBarCodeReaderControl *m_pBCControl;
	IBarCodeReaderControl *m_pBCControl2;
	CIBarcodeReaderTest2Dlg(CWnd* pParent = NULL);	// standard constructor
	void CheckInstalledClasses();

// Dialog Data
	enum { IDD = IDD_IBARCODEREADERTEST2_DIALOG };

	CString	m_barcode;
	CString	m_symbology;
	int		m_read;
	CString	m_timestamp;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	CString m_EditText;
	afx_msg void OnBnClickedButtonEnabled();
	afx_msg void OnBnClickedButtonDisabled();
	int initReader(void);
	LRESULT OnReceivedBarcode(WPARAM wParam, LPARAM lParam);
	void OnProgExit(void);
	afx_msg void OnClose();
};
