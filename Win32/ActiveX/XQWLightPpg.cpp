// XQWLightPpg.cpp : Implementation of the CXQWLightPropPage property page class.

#include "stdafx.h"
#include "XQWLight.h"
#include "XQWLightPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CXQWLightPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CXQWLightPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CXQWLightPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CXQWLightPropPage, "XQWLIGHT.XQWLightPropPage.1",
	0x649d1fa0, 0x3298, 0x4c82, 0x92, 0x42, 0xc3, 0x11, 0x8b, 0x7a, 0x99, 0x34)


/////////////////////////////////////////////////////////////////////////////
// CXQWLightPropPage::CXQWLightPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CXQWLightPropPage

BOOL CXQWLightPropPage::CXQWLightPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_XQWLIGHT_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CXQWLightPropPage::CXQWLightPropPage - Constructor

CXQWLightPropPage::CXQWLightPropPage() :
	COlePropertyPage(IDD, IDS_XQWLIGHT_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CXQWLightPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CXQWLightPropPage::DoDataExchange - Moves data between page and properties

void CXQWLightPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CXQWLightPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CXQWLightPropPage message handlers
