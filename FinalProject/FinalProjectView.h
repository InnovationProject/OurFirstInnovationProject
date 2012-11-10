
// FinalProjectView.h : CFinalProjectView ��Ľӿ�
//

#pragma once


class CFinalProjectView : public CScrollView
{
protected: // �������л�����
	CFinalProjectView();
	DECLARE_DYNCREATE(CFinalProjectView)
	
	bool SuccessOpen;
	long * NAXES;
	char * BMP;
	BITMAPINFO *BMPInfo;

// ����
public:
	CFinalProjectDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CFinalProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
};

#ifndef _DEBUG  // FinalProjectView.cpp �еĵ��԰汾
inline CFinalProjectDoc* CFinalProjectView::GetDocument() const
   { return reinterpret_cast<CFinalProjectDoc*>(m_pDocument); }
#endif

