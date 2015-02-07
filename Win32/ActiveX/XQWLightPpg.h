#if !defined(AFX_XQWLIGHTPPG_H__C13755CF_A986_4F6F_86E8_8D4361C9E610__INCLUDED_)
#define AFX_XQWLIGHTPPG_H__C13755CF_A986_4F6F_86E8_8D4361C9E610__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// XQWLightPpg.h : Declaration of the CXQWLightPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CXQWLightPropPage : See XQWLightPpg.cpp.cpp for implementation.

class CXQWLightPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CXQWLightPropPage)
	DECLARE_OLECREATE_EX(CXQWLightPropPage)

// Constructor
public:
	CXQWLightPropPage();

// Dialog Data
	//{{AFX_DATA(CXQWLightPropPage)
	enum { IDD = IDD_PROPPAGE_XQWLIGHT };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CXQWLightPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XQWLIGHTPPG_H__C13755CF_A986_4F6F_86E8_8D4361C9E610__INCLUDED)
