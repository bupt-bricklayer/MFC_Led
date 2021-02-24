///////////////////////////////////////////////////////////////////////////////
// Led.h : header file
// Visual Source Safe: $Revision: 1 $
//
// Led static control. Will display a LED in 4 different colors and two shapes.
///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-1999 Monte Variakojis ( monte@apollocom.com )
// All rights reserved - not to be sold.
///////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_LEDWND_H__2D837381_FFEC_11D1_A1CE_00A024D311C0__INCLUDED_)
#define AFX_LEDWND_H__2D837381_FFEC_11D1_A1CE_00A024D311C0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
///////////////////////////////////////////////////////////////////////////////

#define LED_SIZE	32			// Led are 16 X 16 pixels

/////////////////////////////////////////////////////////////////////////////
// CLed window
class CLed : public CStatic
{

protected:
	int m_nLedColor;
	//CBitmap m_LedBitmap;
	CRect rect_old;

public:

	enum 
	{
		LED_COLOR_RED = 0 * LED_SIZE,		// Row 0
		LED_COLOR_GREEN = 1 * LED_SIZE,		// Row 1
		LED_COLOR_GREY = 2 * LED_SIZE,		// Row 2 new
		LED_COLOR_BLUE = 3 * LED_SIZE,		// Row 3
		LED_COLOR_YELLOW = 4 * LED_SIZE		// Row 4
	};

// Construction
public:
	CLed();

// Attributes
public:

// Operations
public:
	void SetLed(int nLedColor);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLed)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_CurLedColor;
	int m_nLedSecondColor;
	int m_nLedFirstColor;

	void StopFlashLed();
	void StartFlashLed(int nLedFirstColor, int nLedSecondColor, int dwTimeout=500);
	void DrawLed( CDC *pDC);
	virtual ~CLed();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLed)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEDWND_H__2D837381_FFEC_11D1_A1CE_00A024D311C0__INCLUDED_)
