///////////////////////////////////////////////////////////////////////////////
// Led.cpp : implementation file
// Visual Source Safe: $Revision: 1 $
//
// Led static control. Will display a LED in 4 different colors and two shapes.
///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-1999 Monte Variakojis ( monte@apollocom.com )
// All rights reserved - not to be sold.
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Resource.h"
#include "Led.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLed
#define TIMER_ID_PING		    1		// Timer Ping ID
#define TIMER_ID_FLASH_LED		2		// Timer Ping ID

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CLed::CLed()
{
	//m_LedBitmap.LoadBitmap(IDB_GREEN_LED_SMALL);
	rect_old.left=0;
	rect_old.top=0;
	m_nLedColor = LED_COLOR_GREY;
}

CLed::~CLed()
{
	//VERIFY(m_LedBitmap.DeleteObject());
}


BEGIN_MESSAGE_MAP(CLed, CStatic)
	//{{AFX_MSG_MAP(CLed)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLed message handlers

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CLed::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawLed(&dc);
	
	// Do not call CStatic::OnPaint() for painting messages
}


///////////////////////////////////////////////////////////////////////////////
// Name:		SetLed
// Description:	This method will draw the LED to the specified DC.
//
// Entry:
//				CDC *pDC - DC to draw to
//
//				int iLedColor - Where color is defined by:
//			 		LED_COLOR_RED
//					LED_COLOR_GREEN
//					LED_COLOR_YELLOW
//					LED_COLOR_BLUE
//
//				int iMode - where mode is defined by:
//					LED_ON
//					LED_OFF
//					LED_DISABLED
//
//				int iShape - where shape is defined by:
//					LED_ROUND
//					LED_SQUARE
///////////////////////////////////////////////////////////////////////////////
void CLed::DrawLed(CDC *pDC)
{
    
	CRect rect;
	GetClientRect(&rect);
	int res;

	CBitmap m_LedBitmap;	    
	if(m_nLedColor == LED_COLOR_GREEN)
		res = m_LedBitmap.LoadBitmap(IDB_GREEN_LED_SMALL);
	else if (m_nLedColor == LED_COLOR_RED)
		res = m_LedBitmap.LoadBitmap(IDB_RED_LED_SMALL);
	else 
		res = m_LedBitmap.LoadBitmap(IDB_GREY_LED_SMALL);

	if(res == 0)
		return;
	
	BITMAP pBmp;
	res = m_LedBitmap.GetBitmap(&pBmp);

	if(res == 0)
	{
		VERIFY(m_LedBitmap.DeleteObject());
		return;
	}

	CBitmap Bitmap;
	res = Bitmap.CreateBitmap(pBmp.bmWidth,pBmp.bmHeight,1,1,NULL);

	if(res == 0)
	{
		VERIFY(m_LedBitmap.DeleteObject());
		return;
	}

	CBitmap bitmapTemp;
	res = bitmapTemp.CreateCompatibleBitmap(pDC,pBmp.bmWidth,pBmp.bmHeight);

	if(res == 0)
	{
		VERIFY(m_LedBitmap.DeleteObject());
		VERIFY(Bitmap.DeleteObject());
		return;
	}

	CDC srcDC;
	CDC dcMask;
	CDC TempDC;

	res = TempDC.CreateCompatibleDC(pDC);

	if(res == 0)
	{
		VERIFY(m_LedBitmap.DeleteObject());
		VERIFY(Bitmap.DeleteObject());
		VERIFY(bitmapTemp.DeleteObject());
		return;
	}

	res = srcDC.CreateCompatibleDC(pDC);

	if(res == 0)
	{
		VERIFY(m_LedBitmap.DeleteObject());
		VERIFY(Bitmap.DeleteObject());
		VERIFY(bitmapTemp.DeleteObject());
		//
		VERIFY(TempDC.DeleteDC());
		return;
	}

	res = dcMask.CreateCompatibleDC(pDC);

	if(res == 0)
	{
		VERIFY(m_LedBitmap.DeleteObject());
		VERIFY(Bitmap.DeleteObject());
		VERIFY(bitmapTemp.DeleteObject());
		//
		VERIFY(TempDC.DeleteDC());
		VERIFY(srcDC.DeleteDC());
		return;
	}

	CBitmap* pOldBitmap = srcDC.SelectObject(&m_LedBitmap);

	if(pOldBitmap == NULL)
	{
		VERIFY(m_LedBitmap.DeleteObject());
		VERIFY(Bitmap.DeleteObject());
		VERIFY(bitmapTemp.DeleteObject());
		//
		VERIFY(TempDC.DeleteDC());
		VERIFY(srcDC.DeleteDC());
		VERIFY(dcMask.DeleteDC());
		return;
	}

	CBitmap* pOldMaskbitmap = dcMask.SelectObject(&Bitmap);

	if(pOldMaskbitmap == NULL)
	{
		srcDC.SelectObject(pOldBitmap);
		//
		VERIFY(m_LedBitmap.DeleteObject());
		VERIFY(Bitmap.DeleteObject());
		VERIFY(bitmapTemp.DeleteObject());
		//
		VERIFY(TempDC.DeleteDC());
		VERIFY(srcDC.DeleteDC());
		VERIFY(dcMask.DeleteDC());
		return;
	}

	CBitmap *pOldBitmapTemp = TempDC.SelectObject(&bitmapTemp);

	if(pOldBitmapTemp == NULL)
	{
		VERIFY(dcMask.DeleteDC());
		srcDC.SelectObject(pOldBitmap);
		//
		VERIFY(m_LedBitmap.DeleteObject());
		VERIFY(Bitmap.DeleteObject());
		VERIFY(bitmapTemp.DeleteObject());
		//
		VERIFY(TempDC.DeleteDC());
		VERIFY(srcDC.DeleteDC());
		VERIFY(dcMask.DeleteDC());
		return;
	}

	//
	// Work with tempDC and bitmapTemp to reduce flickering
	//
	
	res = TempDC.BitBlt(0, 0, pBmp.bmWidth, pBmp.bmHeight, pDC, rect_old.left, rect_old.top, SRCCOPY); 

	if(res == 0)
		goto DrawLedExit;

    if(rect.Width() > pBmp.bmWidth)
	{
			int nWidth = rect.Width();
			rect.left += (nWidth - pBmp.bmWidth)/2;
			rect.right -= ((nWidth -pBmp.bmWidth)/2+(nWidth -pBmp.bmWidth)%2);
	}
	if(rect.Height() > pBmp.bmHeight)
	{
			int nHeight = rect.Height();
			rect.top += (nHeight - pBmp.bmHeight)/2;
			rect.bottom -= ((nHeight - pBmp.bmHeight)/2+(nHeight - pBmp.bmHeight)%2);
	}
	//
	// Create mask
	//
	COLORREF OldBkColor = srcDC.SetBkColor(RGB(255,255,255));
	if(OldBkColor == 0x80000000)
		goto DrawLedExit;

	res = dcMask.BitBlt(0, 0, pBmp.bmWidth, pBmp.bmHeight,&srcDC, 0, 0, SRCCOPY); 
	if(res == 0)
		goto DrawLedExit;
	//TempDC.SetBkColor(OldBkColor);

	//
	// Using the IDB_LEDS bitmap, index into the bitmap for the appropriate
	// LED. By using the mask color (RGB(255,0,255)) a mask has been created
	// so the bitmap will appear transparent.
	//
	res = TempDC.BitBlt(0, 0, pBmp.bmWidth, pBmp.bmHeight, &srcDC, 0, 0, SRCINVERT); 
	if(res == 0)
		goto DrawLedExit;
	res = TempDC.BitBlt(0, 0, pBmp.bmWidth, pBmp.bmHeight,&dcMask, 0, 0, SRCAND); 
	if(res == 0)
		goto DrawLedExit;
	res = TempDC.BitBlt(0, 0, pBmp.bmWidth, pBmp.bmHeight, &srcDC, 0, 0, SRCINVERT); 
	if(res == 0)
		goto DrawLedExit;

	//	TempDC.BitBlt(0, 0, pBmp.bmWidth, pBmp.bmHeight, &srcDC, 0, 0, SRCINVERT); 
	//	TempDC.BitBlt(rect.left, rect.top, pBmp.bmWidth, pBmp.bmHeight,&dcMask, 0, 0, SRCAND); 
	//	TempDC.BitBlt(rect.left, rect.top, pBmp.bmWidth, pBmp.bmHeight, &srcDC, 0, 0, SRCINVERT); 

	//	TempDC.BitBlt(0, 0,pBmp.bmWidth, pBmp.bmHeight, &srcDC, 0, 0, SRCCOPY); 
		//TempDC.SetBkColor(OldBkColor);
		//
		// Since the actual minipulation is done to tempDC so there is minimal
		// flicker, it is now time to draw the result to the screen.
		//

	//	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &TempDC, 0,0,pBmp.bmWidth,pBmp.bmHeight, SRCCOPY); 
	//	COLORREF bkcolor=pDC->GetBkColor();
	//	CBrush brush;
	//	brush.CreateSolidBrush(bkcolor);
	//	pDC->FillRect(&rect,&brush);
		
	res = pDC->BitBlt(rect.left, rect.top, pBmp.bmWidth, pBmp.bmHeight, &TempDC, 0, 0, SRCCOPY); 
	if(res == 0)
		goto DrawLedExit;

	CopyRect(&rect_old,&rect);
	//
		// House cleaning

DrawLedExit:
	//
	srcDC.SelectObject(pOldBitmap);
	dcMask.SelectObject(pOldMaskbitmap);
	TempDC.SelectObject(pOldBitmapTemp);

	VERIFY(srcDC.DeleteDC());
	VERIFY(dcMask.DeleteDC());
	VERIFY(TempDC.DeleteDC());

	VERIFY(Bitmap.DeleteObject());
	VERIFY(bitmapTemp.DeleteObject());
	VERIFY(m_LedBitmap.DeleteObject());

}

///////////////////////////////////////////////////////////////////////////////
// Name:		SetLed
// Description:	This method will draw and set led parameters.
//
// Entry:		int iLedColor - Where color is defined by:
//			 		LED_COLOR_RED
//					LED_COLOR_GREEN
//					LED_COLOR_YELLOW
//					LED_COLOR_BLUE
//
//				int iMode - where mode is defined by:
//					LED_ON
//					LED_OFF
//					LED_DISABLED
//
//				int iShape - where shape is defined by:
//					LED_ROUND
//					LED_SQUARE
///////////////////////////////////////////////////////////////////////////////
void CLed::SetLed(int nLedColor)
{

	m_nLedColor = nLedColor;
	
	Invalidate();
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CLed::OnTimer(UINT nIDEvent) 
{
	
	if(nIDEvent == TIMER_ID_FLASH_LED)
	{
		if(m_CurLedColor)
		{
			SetLed(m_nLedFirstColor);
			m_CurLedColor=false;
		}
		else
		{
			SetLed(m_nLedSecondColor);
			m_CurLedColor=true;
		}
	}
	CStatic::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL CLed::OnEraseBkgnd(CDC* pDC) 
{
	// No background rendering
//	return false;
	return CStatic::OnEraseBkgnd(pDC);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void CLed::StartFlashLed(int nLedFirstColor, int nLedSecondColor, int dwTimeout)
{
	m_nLedFirstColor = nLedFirstColor;
	m_nLedSecondColor = nLedSecondColor;
	m_CurLedColor=false;

	SetTimer(TIMER_ID_FLASH_LED,dwTimeout,NULL);
}

void CLed::StopFlashLed()
{
	KillTimer(TIMER_ID_FLASH_LED);
	SetLed(m_nLedFirstColor);
}

