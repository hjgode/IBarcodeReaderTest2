// IBarcodeReaderTest2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 

// CIBarcodeReaderTest2App:
// See IBarcodeReaderTest2.cpp for the implementation of this class
//

class CIBarcodeReaderTest2App : public CWinApp
{
public:
	CIBarcodeReaderTest2App();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CIBarcodeReaderTest2App theApp;
