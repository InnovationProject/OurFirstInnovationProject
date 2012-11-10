
// FinalProjectView.cpp : CFinalProjectView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFinalProjectView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, &CFinalProjectView::OnFileOpen)
END_MESSAGE_MAP()

// CFinalProjectView 构造/析构

CFinalProjectView::CFinalProjectView()
{
	// TODO: 在此处添加构造代码

}

CFinalProjectView::~CFinalProjectView()
{
}

BOOL CFinalProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CFinalProjectView 绘制

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

	// TODO: 在此处为本机数据添加绘制代码
}

void CFinalProjectView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CFinalProjectView 打印


void CFinalProjectView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CFinalProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFinalProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFinalProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CFinalProjectView 诊断

#ifdef _DEBUG
void CFinalProjectView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFinalProjectView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFinalProjectDoc* CFinalProjectView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFinalProjectDoc)));
	return (CFinalProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CFinalProjectView 消息处理程序


void CFinalProjectView::OnFileOpen()
{

	// TODO: 在此添加命令处理程序代码
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
		nelements *=  NAXES[i];//计算总像素个数
	}
	fpixel = 1;//从第一个像素开始读取
	int nulval = 1;//默认空值
	int anynul = 1;//默认任意值
	//读取数据
	unsigned short * FITSDataArray;
	if(BITPIX == SHORT_IMG)
	{
		FITSDataArray = new unsigned short[nelements];

		fits_read_img(fptr, USHORT_IMG, fpixel, nelements, &nulval, FITSDataArray, &anynul, &status);
	}
	fits_close_file(fptr, &status);
	int kkkk=FITSDataArray[19];
	
	// 成功打开文件
	SuccessOpen = true;

    //求最值
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

	//线性,对数变换
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
 

		//赋值给图像数据矩阵
		BMP[l++] = BMPDataArray[i];
		BMP[l++] = BMPDataArray[i];
		BMP[l++] = BMPDataArray[i];
	}

	//BMP文件头信息
	
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
	
    //绘图
	Invalidate(false);
	this->UpdateWindow();
	//设置滚动条
	CSize size(NAXES[0] - 1, NAXES[1] - 1);
    this->SetScrollSizes(MM_TEXT, size, sizeDefault, sizeDefault );
	// TODO: 在此添加命令处理程序代码
}
