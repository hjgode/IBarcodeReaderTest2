// IBarcodeReaderTest2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "IBarcodeReaderTest2.h"
#include "IBarcodeReaderTest2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TCHAR *Symbologies[] =
{

	TEXT("-"),
	TEXT("Code39"),
	TEXT("Code93"),
	TEXT("Code49"),
	TEXT("I2Of5"),
	TEXT("Codabar"),
	TEXT("Upc	"),
	TEXT("Code128"),
	TEXT("Code16K"),
	TEXT("Plessey"),
	TEXT("Code11"),
	TEXT("Msi	"),
	TEXT("Pdf417"),
	TEXT("D2Of5"),
	TEXT("Telepen"),
	TEXT("Matrix2Of5"),
	TEXT("Codablock"),
	TEXT("Ankercode"),
	TEXT("Maxicode"),
	TEXT("Otherbc"),
	TEXT("System_Ex"),
	TEXT("Non_Barcode"),
	TEXT("Duplicate"),
	TEXT("Datamatrix"),
	TEXT("Qrcode"),
	TEXT("Rss"),
	TEXT("Aztec"),
	TEXT("Ucc"),
	TEXT("Postal"),
	TEXT("-"),
	TEXT("-"),
	TEXT("-"),
	TEXT("Unknown")
};

// CIBarcodeReaderTest2Dlg dialog

CIBarcodeReaderTest2Dlg::CIBarcodeReaderTest2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIBarcodeReaderTest2Dlg::IDD, pParent)
	, m_EditText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_barcode = _T("");
	m_symbology = _T("");
	m_read = 1; // run the readthread
	m_timestamp = _T("");
}

void CIBarcodeReaderTest2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_EditText);
}

BEGIN_MESSAGE_MAP(CIBarcodeReaderTest2Dlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
	ON_MESSAGE(WM_ITC_RECEIVED_BARCODE, OnReceivedBarcode)
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_Enabled, &CIBarcodeReaderTest2Dlg::OnBnClickedButtonEnabled)
	ON_BN_CLICKED(IDC_BUTTON_Disabled, &CIBarcodeReaderTest2Dlg::OnBnClickedButtonDisabled)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CIBarcodeReaderTest2Dlg message handlers

BOOL CIBarcodeReaderTest2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	UpdateData(true);
    CFont *m_pFont = new CFont();
    m_pFont->CreatePointFont(132, _T("Arial"));
    GetDlgItem(IDC_EDIT1)->SetFont(m_pFont, TRUE);
	GetDlgItem(IDC_BUTTON_Enabled)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Disabled)->EnableWindow(FALSE);
	UpdateData(false);

	initReader();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CIBarcodeReaderTest2Dlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_IBARCODEREADERTEST2_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_IBARCODEREADERTEST2_DIALOG));
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////////////
// ReadThread
//

UINT ReadLabelThread(LPVOID pVoid)
{
	BYTE						BCData[2048];
	DWORD						BytesReceived;
	ITC_BARCODE_DATA_DETAILS	bd;
    TCHAR DispData[2048];	

	CIBarcodeReaderTest2Dlg* pTestDlg = (CIBarcodeReaderTest2Dlg *) pVoid;
	while (pTestDlg->m_read) // set to 0 before terminating the app
	{
		memset(BCData, 0, sizeof(BCData));
		memset(&bd, 0, sizeof(bd));
		bd.wStructSize = sizeof(bd);
		BytesReceived = 0;

/* use for test only
		CTimer::starttimer ;
		// Obtain the frequency of the counter
		LARGE_INTEGER frq;
		  QueryPerformanceFrequency(&frq);

		// Start the count
		  LARGE_INTEGER d1;
		   QueryPerformanceCounter(&d1);

		//Operations you need to measure
*/
		pTestDlg->m_pBCControl->Read(BCData, sizeof(BCData), 
									 &BytesReceived, 
									 &bd, 
									 500);
									 //INFINITE);

		if(BytesReceived)
		{
/* use for test only
			// Finish the count
 		    LARGE_INTEGER d2;
			QueryPerformanceCounter(&d2);
			//Compute the diference;
			  double duration =   double( d2.QuadPart - d1.QuadPart ) / frq.QuadPart;
*/
			int index = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
											(LPCSTR)&BCData,
											BytesReceived, 
											DispData, 2048);
			DispData[index] = 0;
			pTestDlg->m_barcode = DispData;
			DEBUGMSG(1, (L"got boarcode:'%s'\n", DispData));
			if(bd.eSymbology <= 32)
			{
				pTestDlg->m_symbology = Symbologies[bd.eSymbology];
			}
			else
			{
				pTestDlg->m_symbology = Symbologies[0];
			}
/* use for test only
			TCHAR lpText[255];
			swprintf(lpText, _T("Timeout Duration = %05d Counts per second=%05d"), duration ,frq );
			MessageBox(NULL, lpText, TEXT("Timer"), MB_OK | MB_TOPMOST | MB_SETFOREGROUND);
*/			

			pTestDlg->m_timestamp.Format(TEXT("%02d.%02d.%02d %02d:%02d:%02d"),
										 bd.stTimeStamp.wDay,
										 bd.stTimeStamp.wMonth,
										 bd.stTimeStamp.wYear % 100,
										 bd.stTimeStamp.wHour,
										 bd.stTimeStamp.wMinute,
										 bd.stTimeStamp.wSecond
										 );
			  pTestDlg->SendMessage(WM_ITC_RECEIVED_BARCODE, 0, 0);
		}
	}
	DEBUGMSG(1, (L"ReadLabelThread stopped"));
	return 0;
}

void CIBarcodeReaderTest2Dlg::OnBnClickedButtonEnabled()
{
	// TODO: Add your control notification handler code here
    HRESULT hrStatus;
    TCHAR   szMessage[80];
	BOOL    ScanEnable = TRUE;

    CLSID clsBCControl;
	HRESULT apiRes;
	apiRes=CLSIDFromProgID(TEXT("Intermec.BarCodeReader"), &clsBCControl);

	if(apiRes == S_OK)
    {
        if(CoCreateInstance(clsBCControl, 
							NULL,
							CLSCTX_INPROC_SERVER,
							IID_IBarCodeReaderControl,
							(void **)(&m_pBCControl2)) == S_OK)
        {
            if(m_pBCControl2->Initialize(TEXT("default"),
										ITC_DHDEVFLAG_READAHEAD) == S_OK)
			{
				hrStatus = m_pBCControl2->SetAttribute(ITC_RDRATTR_SCANNER_ENABLE, (BYTE *)(&ScanEnable), sizeof(ScanEnable));
				if(FAILED(hrStatus))
				{
					wsprintf(szMessage, TEXT("ScanEnable Failed: 0x%08x"), hrStatus);
					AfxMessageBox(szMessage);
				}
				else{
					GetDlgItem(IDC_BUTTON_Enabled)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_Disabled)->EnableWindow(TRUE);
				}
			}
			m_pBCControl2->Release();
		}
		else
		{
			switch (apiRes){
				case REGDB_E_CLASSNOTREG:
					MessageBox(TEXT("CoCreateInstance(). REGDB_E_CLASSNOTREG"), TEXT("Error"), MB_ICONERROR|MB_OK);
					break;
				case CLASS_E_NOAGGREGATION:
					MessageBox(TEXT("CoCreateInstance(). CLASS_E_NOAGGREGATION"), TEXT("Error"), MB_ICONERROR|MB_OK);
					break;
				case E_NOINTERFACE:
					MessageBox(TEXT("CoCreateInstance(). E_NOINTERFACE"), TEXT("Error"), MB_ICONERROR|MB_OK);
					break;
				default:
					MessageBox(TEXT("CoCreateInstance(). unknown result"), TEXT("Error"), MB_ICONERROR|MB_OK);
			}
		}
	}
	else
	{
		MessageBox(TEXT("CLSIDFromProgID()."), TEXT("Error"),
			MB_ICONERROR|MB_OK);
	}
}

void CIBarcodeReaderTest2Dlg::OnBnClickedButtonDisabled()
{
	// TODO: Add your control notification handler code here
    HRESULT hrStatus;
    TCHAR   szMessage[80];
	BOOL    ScanEnable = FALSE;

    CLSID clsBCControl;
	
	if(CLSIDFromProgID(TEXT("Intermec.BarCodeReader"), &clsBCControl) == S_OK)
    {
		HRESULT apiRes;
		apiRes=CoCreateInstance(clsBCControl, 
							NULL,
							CLSCTX_INPROC_SERVER,
							IID_IBarCodeReaderControl,
							(void **)(&m_pBCControl2));
        if(apiRes == S_OK)
        {
            if(m_pBCControl2->Initialize(TEXT("default"), ITC_DHDEVFLAG_READAHEAD) == S_OK)
			{
				hrStatus = m_pBCControl2->SetAttribute(ITC_RDRATTR_SCANNER_ENABLE, 
													  (BYTE *)(&ScanEnable),
													  sizeof(ScanEnable));
				if(FAILED(hrStatus))
				{
					wsprintf(szMessage, TEXT("ScanDisable Failed: 0x%08x"), hrStatus);
					AfxMessageBox(szMessage);
				}
				else{
					GetDlgItem(IDC_BUTTON_Enabled)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_Disabled)->EnableWindow(FALSE);
				}
			}
			m_pBCControl2->Release();
		}
		else
		{
			switch (apiRes){
				case REGDB_E_CLASSNOTREG:
					MessageBox(TEXT("CoCreateInstance(). REGDB_E_CLASSNOTREG"), TEXT("Error"), MB_ICONERROR|MB_OK);
					break;
				case CLASS_E_NOAGGREGATION:
					MessageBox(TEXT("CoCreateInstance(). CLASS_E_NOAGGREGATION"), TEXT("Error"), MB_ICONERROR|MB_OK);
					break;
				case E_NOINTERFACE:
					MessageBox(TEXT("CoCreateInstance(). E_NOINTERFACE"), TEXT("Error"), MB_ICONERROR|MB_OK);
					break;
				default:
					MessageBox(TEXT("CoCreateInstance(). unknown result"), TEXT("Error"), MB_ICONERROR|MB_OK);
			}
		}
	}
	else
	{
		MessageBox(TEXT("CLSIDFromProgID()."), TEXT("Error"), MB_ICONERROR|MB_OK);
	}
}

void CIBarcodeReaderTest2Dlg::CheckInstalledClasses(){
	//BarCode Reader Control?
	CLSID clsBCControl;
	if(CLSIDFromProgID(TEXT("Intermec.BarCodeReader"), &clsBCControl) == S_OK)
	{
		//Disable Configure button
		GetDlgItem(IDC_BUTTON_Enabled)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_Enabled)->EnableWindow(FALSE);
		
	}
}

int CIBarcodeReaderTest2Dlg::initReader(void)
{
	CLSID clsBCControl;
	HRESULT hClsID;
	hClsID = CLSIDFromProgID(TEXT("Intermec.BarCodeReader"), &clsBCControl);

	if(hClsID == S_OK)
    {
		CoInitializeEx(NULL,COINIT_MULTITHREADED);
		//hClsID = CoInitialize(0);

        //
        // Create the object and initialize it.
        //
        hClsID = CoCreateInstance(clsBCControl, 
							NULL,
							CLSCTX_INPROC_SERVER,
							IID_IBarCodeReaderControl,
							(void **)(&m_pBCControl));
		if( hClsID == S_OK )
        //
        // If the bar code control object was created, then initialize
        //  it as Read Ahead and set up the grid.
        //
        {
			//
			// The Bar Code Reader Engine device name that is
			// a subkey in HKLM/Software/Intermec/ADCDevices.
			//
            if(m_pBCControl->Initialize(TEXT("default"), ITC_DHDEVFLAG_READAHEAD) == S_OK)
			{
				AfxBeginThread(ReadLabelThread, this);
				GetDlgItem(IDC_BUTTON_Enabled)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_Disabled)->EnableWindow(TRUE);
			}
			else
			{
				MessageBox(TEXT("Initialize()."), TEXT("Error"), MB_ICONERROR|MB_OK);
				EndDialog(1);
				return -3;
			}
        }
		else //CoCreateInstance
		{
			switch (hClsID)
			{
			case CO_E_CLASSSTRING:
				MessageBox(TEXT("Fehler: CO_E_CLASSSTRING"), TEXT("CoCreateInstance() Error"), MB_ICONERROR|MB_OK);
				break;
			case REGDB_E_WRITEREGDB:
				MessageBox(TEXT("Fehler: REGDB_E_WRITEREGDB"), TEXT("CoCreateInstance() Error"), MB_ICONERROR|MB_OK);
				break;
			case REGDB_E_CLASSNOTREG:
				MessageBox(TEXT("Fehler: REGDB_E_CLASSNOTREG"), TEXT("CoCreateInstance() Error"), MB_ICONERROR|MB_OK);
				break;
			default:
				  TCHAR syserrs[ 512 ];
				  TCHAR errs[512];

				  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, hClsID, 0, syserrs, sizeof( syserrs ) / sizeof( syserrs[ 0 ] ), 0 );

				  _stprintf( errs, _T( "Error in CoCreateInstance 0x%x: %s\r\n" ),
				   hClsID, syserrs );
				  //OutputDebugString( errs );

				MessageBox(errs, TEXT("CoCreateInstance() Error"), MB_ICONERROR|MB_OK);
			}

			EndDialog(1);
			return -2;
		}
    }
	else //CLSIDFromProgID
	{
	  TCHAR syserrs[ 512 ];
	  TCHAR errs[512];
	  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, hClsID, 0, syserrs, sizeof( syserrs ) / sizeof( syserrs[ 0 ] ), 0 );
	  _stprintf( errs, _T( "Error in CLSIDFromProgID 0x%x: %s\r\n" ), hClsID, syserrs );
	  //OutputDebugString( errs );

		MessageBox(errs, TEXT("CLSIDFromProgID() Error"), MB_ICONERROR|MB_OK);

		//MessageBox(TEXT("CLSIDFromProgID()."), TEXT("Error"),	MB_ICONERROR|MB_OK);
		EndDialog(1);
		return -1;
	}
	
	//test for the needed installed ActiveX object classes
	CheckInstalledClasses();

	return 0;
}

LRESULT CIBarcodeReaderTest2Dlg::OnReceivedBarcode(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);
	DEBUGMSG(1, (L"OnReceivedBarcode: '%s'\n", m_barcode));
	m_EditText = m_barcode +L"\r\n" + m_timestamp +L"\r\n"+m_symbology;
	UpdateData(false);
	
	return 0;
}

void CIBarcodeReaderTest2Dlg::OnProgExit(void)
{
	m_read = 0;	// make shure the readthread terminates
	m_pBCControl->Release();
}

void CIBarcodeReaderTest2Dlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	OnProgExit();
	CDialog::OnClose();
}
