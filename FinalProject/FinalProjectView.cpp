
// FinalProjectView.cpp : CFinalProjectView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "FinalProject.h"
#endif

#include "FinalProjectDoc.h"
#include "FinalProjectView.h"

#include "fitsio.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFinalProjectView

IMPLEMENT_DYNCREATE(CFinalProjectView, CScrollView)

BEGIN_MESSAGE_MAP(CFinalProjectView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFinalProjectView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, &CFinalProjectView::OnFileOpen)
END_MESSAGE_MAP()

// CFinalProjectView ����/����

CFinalProjectView::CFinalProjectView()
{
	// TODO: �ڴ˴���ӹ������

}

CFinalProjectView::~CFinalProjectView()
{
}

BOOL CFinalProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CFinalProjectView ����

void CFinalProjectView::OnDraw(CDC* pDC)
{
	CFinalProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(SuccessOpen == true)
	{
		StretchDIBits(pDC->GetSafeHdc(),
			0, 0, NAXES[0], NAXES[1],
			0, 0, NAXES[0], NAXES[1],
			BMP, BMPInfo,
			DIB_RGB_COLORS,
			SRCCOPY);
	}
	//if (!pDoc)
		//return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CFinalProjectView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CFinalProjectView ��ӡ


void CFinalProjectView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CFinalProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CFinalProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CFinalProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CFinalProjectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CFinalProjectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CFinalProjectView ���

#ifdef _DEBUG
void CFinalProjectView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFinalProjectView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFinalProjectDoc* CFinalProjectView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFinalProjectDoc)));
	return (CFinalProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CFinalProjectView ��Ϣ�������


void CFinalProjectView::OnFileOpen()
{

	// TODO: �ڴ���������������
	fitsfile * fptr;
	int status;
	long fpixel = 1,naxis = 2,nelements;
	long naxes[2] = {300,200};
	//short array[200][300];
	status = 0;
	int BITPIX;
	fits_open_image(&fptr,"UC_Correction.fits",READONLY,&status);
	fits_get_img_type(fptr, &BITPIX, &status);
	int NAXIS = 2;
	NAXES = new long[NAXIS];
	fits_get_img_size(fptr, NAXIS, NAXES, &status);
	nelements = 1;
	for(int i = 0 ; i <  NAXIS ; i++)
	{
		nelements *=  NAXES[i];//���������ظ���
	}
	fpixel = 1;//�ӵ�һ�����ؿ�ʼ��ȡ
	int nulval = 1;//Ĭ�Ͽ�ֵ
	int anynul = 1;//Ĭ������ֵ
	//��ȡ����
	unsigned short * FITSDataArray;
	if(BITPIX == SHORT_IMG)
	{
		FITSDataArray = new unsigned short[nelements];

		fits_read_img(fptr, USHORT_IMG, fpixel, nelements, &nulval, FITSDataArray, &anynul, &status);
	}
	fits_close_file(fptr, &status);
	int kkkk=FITSDataArray[19];
	
	// �ɹ����ļ�
	SuccessOpen = true;

    //����ֵ
 	int FITDataMax = 0;
 	int FITDataMin = 65535;
 	for(int i = 0 ; i < nelements ; i++)
	{
 		if(FITDataMax < FITSDataArray[i])
 			FITDataMax = FITSDataArray[i];
    	if(FITDataMin > FITSDataArray[i])
    		FITDataMin = FITSDataArray[i];
	}
 	
	int FITToBMPUpperLimit = FITDataMax;
    int FITToBMPLowerLimit = FITDataMin;

	//����,�����任
	BYTE *BMPDataArray = new BYTE[nelements];
	BMP = new char[nelements * 3];
	int l = 0;
	for(int i = 0 ; i < nelements ; i++)
	{
    	if (FITSDataArray[i] >= FITToBMPUpperLimit)
            BMPDataArray[i] = BYTE(45.986 * log(1.0 + 255));
        else if (FITSDataArray[i] <= FITToBMPLowerLimit)
            BMPDataArray[i] = BYTE(45.986 * log(1.0 + 0));
        else
            BMPDataArray[i] = BYTE(45.986 * log(1.0 + 255.0 * (FITSDataArray[i] - FITToBMPLowerLimit) / (FITToBMPUpperLimit - FITToBMPLowerLimit)));           
 

		//��ֵ��ͼ�����ݾ���
		BMP[l++] = BMPDataArray[i];
		BMP[l++] = BMPDataArray[i];
		BMP[l++] = BMPDataArray[i];
	}

	//BMP�ļ�ͷ��Ϣ
	
	BMPInfo = (BITMAPINFO *)new char[sizeof(BITMAPINFOHEADER)];
	BMPInfo->bmiHeader.biBitCount = 24;
	BMPInfo->bmiHeader.biClrImportant = 0;
	BMPInfo->bmiHeader.biClrUsed = 0;
	BMPInfo->bmiHeader.biCompression = 0;
	BMPInfo->bmiHeader.biWidth = NAXES[0];
	BMPInfo->bmiHeader.biHeight = -NAXES[1];
	BMPInfo->bmiHeader.biPlanes = 1;
	BMPInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BMPInfo->bmiHeader.biSizeImage = 0;
	BMPInfo->bmiHeader.biXPelsPerMeter = 0;
	BMPInfo->bmiHeader.biYPelsPerMeter = 0;
	
    //��ͼ
	Invalidate(false);
	this->UpdateWindow();
	//���ù�����
	CSize size(NAXES[0] - 1, NAXES[1] - 1);
    this->SetScrollSizes(MM_TEXT, size, sizeDefault, sizeDefault );
	// TODO: �ڴ���������������
}
