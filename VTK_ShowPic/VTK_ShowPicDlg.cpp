#pragma
#include "pch.h"
#include <gdiplus.h>
#include "framework.h"
#include "VTK_ShowPic.h"
#include "VTK_ShowPicDlg.h"
#include "afxdialogex.h"
#include <gdipluseffects.h>
#include "vtkSmartPointer.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkJPEGReader.h"
#include <vtkBMPReader.h>
#include <vtkImageData.h>
#include<vtkImageViewer2.h>
#include "vtkTexture.h"
#include "vtkImageActor.h"
#include <vtkAutoInit.h>
#include <vtkPlaneSource.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkImageMapper.h>
#include <vtkSTLReader.h>
#include"vtkTransform.h"
#include <vtkCellPicker.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include<vtkInteractorStyleTrackballActor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkBMPWriter.h>
#include <vtkCamera.h>
#include <vtkMath.h>
#include "CVTKRenderDialog.h"
#include <fstream>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
vtkSmartPointer<CMyStyle> m_iStyle;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:

	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CVTKShowPicDlg 对话框

CVTKShowPicDlg::CVTKShowPicDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VTK_SHOWPIC_DIALOG, pParent)
{		
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVTKShowPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRANSLATE_X_SLIDER, m_translateXSlider);
	DDX_Control(pDX, IDC_TRANSLATE_Y_SLIDER, m_translateYSlider);
	DDX_Control(pDX, IDC_TRANSLATE_Z_SLIDER, m_translateZSlider);
	DDX_Control(pDX, IDC_ROTATE_X_SLIDER, m_rotateXSlider);
	DDX_Control(pDX, IDC_ROTATE_Y_SLIDER, m_rotateYSlider);
	DDX_Control(pDX, IDC_ROTATE_Z_SLIDER, m_rotateZSlider);
	DDX_Control(pDX, IDC_SCROLLBAR6, m_colorSlider);
	DDX_Control(pDX, IDC_SCROLLBAR7, m_colorSlider2);
	DDX_Control(pDX, IDC_SCROLLBAR8, m_colorSlider3);
	DDX_Control(pDX, IDC_SCROLLBAR9, m_colorSlider4);
}

BEGIN_MESSAGE_MAP(CVTKShowPicDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CVTKShowPicDlg::OnLoadVectorImageButtonClicked)
	//ON_NOTIFY(NM_THEMECHANGED, IDC_TRANSLATE_X_SLIDER, &CVTKShowPicDlg::OnNMThemeChangedTranslateXSlider)
	ON_BN_CLICKED(IDC_BUTTON6, &CVTKShowPicDlg::OnBnClickedSaveBmpButton)
	ON_BN_CLICKED(IDC_BUTTON4, &CVTKShowPicDlg::OnBnClickedSavePoses)
	ON_BN_CLICKED(IDC_BUTTON2, &CVTKShowPicDlg::setCameraParametersButton)
	ON_MESSAGE(UM_CUR_MSG_POS, OnCurPosMsg)

	ON_BN_CLICKED(IDC_BUTTON5, &CVTKShowPicDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CVTKShowPicDlg 消息处理程序

BOOL CVTKShowPicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor> interactor;;
	vtkSmartPointer<CMyStyle> interactorStyle;
	// 将列表框控件的指针与资源中的列表框关联起来
	m_listBox.SubclassDlgItem(IDC_LIST2, this);

	// 设置列表框的属性（可选）

	m_listBox.SetHorizontalExtent(800); // 设置水平滚动条的初始长度

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 在水平编辑框中显示50   
	
	SetDlgItemInt(IDC_HSCROLL_EDIT, 0);
	SetDlgItemInt(IDC_HSCROLL_EDIT2, 0);
	SetDlgItemInt(IDC_HSCROLL_EDIT3, 0);
	// 在旋转编辑框中显示50 
	SetDlgItemInt(IDC_HSCROLL_EDIT4, 0);
	SetDlgItemInt(IDC_HSCROLL_EDIT5, 0);
	SetDlgItemInt(IDC_HSCROLL_EDIT6, 0);
	// 在颜色编辑框中显示255
	SetDlgItemInt(IDC_EDIT3, 100);
	SetDlgItemInt(IDC_EDIT4, 255);
	SetDlgItemInt(IDC_EDIT5, 255);
	SetDlgItemInt(IDC_EDIT6, 255);

	// 设置水平滚动条的滚动范围为1到100   
	m_translateXSlider.SetScrollRange(1, 500);
	m_translateYSlider.SetScrollRange(1, 500);
	m_translateZSlider.SetScrollRange(1, 500);
	//设置旋转滚动条的滚动范围1-100
	m_rotateXSlider.SetScrollRange(-180, 180);
	m_rotateYSlider.SetScrollRange(-180, 180);
	m_rotateZSlider.SetScrollRange(-180, 180);
	//设置颜色滚动条的滚动范围1-255
	m_colorSlider.SetScrollRange(0, 100);
	m_colorSlider2.SetScrollRange(0, 255);
	m_colorSlider3.SetScrollRange(0, 255);
	m_colorSlider4.SetScrollRange(0, 255);

	// 设置水平滚动条的初始位置为50   
	m_translateXSlider.SetScrollPos(200);
	m_translateYSlider.SetScrollPos(200);
	m_translateZSlider.SetScrollPos(200);
	// 设置旋转滚动条的初始位置为50   
	m_rotateXSlider.SetScrollPos(0);
	m_rotateYSlider.SetScrollPos(0);
	m_rotateZSlider.SetScrollPos(0);
	// 设置颜色滚动条的初始位置  
	m_colorSlider.SetScrollPos(100);
	m_colorSlider2.SetScrollPos(255);
	m_colorSlider3.SetScrollPos(255);
	m_colorSlider4.SetScrollPos(255);

	UpdateData(false);
	UpdateWindow();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVTKShowPicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码

void CVTKShowPicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

// 按钮点击获取文件路径

void CVTKShowPicDlg::OnLoadVectorImageButtonClicked()
{
	// 弹出文件对话框选择BMP图片
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("BMP Files (*.bmp)|*.bmp||"), this);
	if (dlg.DoModal() == IDOK)
	{
		CString bmpfilename = dlg.GetPathName();
		SetDlgItemText(IDC_EDIT1, bmpfilename);

		// 弹出文件对话框选择第一个STL文件
		CFileDialog dlg1(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("STL Files (*.stl)|*.stl||"), this);
		if (dlg1.DoModal() == IDOK)
		{
			stlFilename1 = dlg1.GetPathName();
			SetDlgItemText(IDC_EDIT2, stlFilename1);

			// 弹出文件对话框选择第二个STL文件
			CFileDialog dlg2(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("STL Files (*.stl)|*.stl||"), this);
			if (dlg2.DoModal() == IDOK)
			{
				stlFilename2 = dlg2.GetPathName();
				// 调用渲染函数，传入两个STL文件路径
				RenderBMPAndSTL(bmpfilename, stlFilename1, stlFilename2);
			}
		}
	}
}

// 修改的 RenderBMPAndSTL 函数，使其接受两个STL文件路径
void CVTKShowPicDlg::RenderBMPAndSTL(const CString& bmpFilename, const CString& stlFilename1, const CString& stlFilename2)
{
	// 创建 BMP 图片读取器和 Actor
	const char* bmpFilenameChar = bmpFilename.GetString();
	vtkSmartPointer<vtkBMPReader> bmpReader = vtkSmartPointer<vtkBMPReader>::New();
	bmpReader->SetFileName(bmpFilenameChar);
	bmpReader->Update();

	vtkSmartPointer<vtkImageActor> bmpActor = vtkSmartPointer<vtkImageActor>::New();
	bmpActor->SetInputData(bmpReader->GetOutput());
	//bmpActor->SetScale(2);
	//禁用背景图片的交互性
	//bmpActor->PickableOff();

	// 创建 STL 模型读取器和 Mapper
	const char* stlFilename1Char = stlFilename1.GetString();
	vtkSmartPointer<vtkSTLReader> stlReader1 = vtkSmartPointer<vtkSTLReader>::New();
	stlReader1->SetFileName(stlFilename1Char);
	stlReader1->Update();

	vtkSmartPointer<vtkPolyDataMapper> stlMapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	stlMapper1->SetInputConnection(stlReader1->GetOutputPort());

	// 创建 STL 模型 Actor
	stlActor = vtkSmartPointer<vtkActor>::New();
	stlActor->SetMapper(stlMapper1);

	// 创建第二个 STL 模型读取器和 Mapper
	const char* stlFilename2Char = stlFilename2.GetString();
	vtkSmartPointer<vtkSTLReader> stlReader2 = vtkSmartPointer<vtkSTLReader>::New();
	stlReader2->SetFileName(stlFilename2Char);
	stlReader2->Update();

	vtkSmartPointer<vtkPolyDataMapper> stlMapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	stlMapper2->SetInputConnection(stlReader2->GetOutputPort());

	// 创建第二个 STL 模型 Actor
	stlActor2 = vtkSmartPointer<vtkActor>::New();
	stlActor2->SetMapper(stlMapper2);
	
	// 设置模型的缩放比例，使其显示更大
	stlActor->SetScale(3);
	stlActor2->SetScale(3);// 放大3倍，数字越大模型显示越大

	// 获取模型的包围盒
	double bounds[6];
	stlActor->GetBounds(bounds);
	stlActor2->GetBounds(bounds);

	// 计算包围盒的中心
	double centerX = (bounds[0] + bounds[1]) / 2.0;
	double centerY = (bounds[2] + bounds[3]) / 2.0;
	double centerZ = (bounds[4] + bounds[5]) / 2.0;

	// 将模型放置在坐标原点(0, 0, 0)
	stlActor->SetPosition(-centerX, -centerY, -centerZ);
	stlActor2->SetPosition(-centerX, -centerY, -centerZ);
	
	// 创建渲染器
	renderer = vtkSmartPointer<vtkRenderer>::New();

	renderer->AddActor(bmpActor); // 添加 BMP 图片 Actor
	renderer->AddActor(stlActor); // 添加第一个 STL 模型 Actor
	renderer->AddActor(stlActor2); // 添加第二个 STL 模型 Actor
	renderer->SetBackground(0, 0, 255);  // 蔚蓝色背景
	// 创建渲染窗口和交互器
	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetNumberOfLayers(2);
	renderer->SetLayer(0);
	
	interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	// 初始化自定义交互样式
	vtkSmartPointer<CMyStyle> interactorStyle = vtkSmartPointer<CMyStyle>::New();
	interactorStyle->SetDefaultRenderer(renderer);


	// 关联滑动条
	interactorStyle->m_translateXSlider = &m_translateXSlider;
	interactorStyle->m_translateYSlider = &m_translateYSlider;
	interactorStyle->m_translateZSlider = &m_translateZSlider;
	interactorStyle->m_rotateXSlider = &m_rotateXSlider;
	interactorStyle->m_rotateYSlider = &m_rotateYSlider;
	interactorStyle->m_rotateZSlider = &m_rotateZSlider;

	interactor->SetInteractorStyle(interactorStyle);
   ///< 设置自定义交互风格
    //m_iStyle = vtkSmartPointer<CMyStyle>::New();
    //m_iStyle->SetDefaultRenderer(renderer);
    //interactor->SetInteractorStyle(m_iStyle);
    //m_iStyle->SetDlg(this);



   // 渲染并显示
	renderWindow->SetWindowName("VTK Show");
	renderWindow->SetSize(800, 600);
	renderWindow->Render();
	interactor->Start();

}

// 渲染 VTK 模型
void CVTKShowPicDlg::RenderModel()
{
	// 渲染并显示
	renderWindow->Render();
	
}

void CVTKShowPicDlg::HandleScrollBarEvent(CScrollBar* scrollBar, UINT nSBCode, UINT nPos, int editBoxID, CDialogEx* pDialog)
{
	if (stlFilename1.IsEmpty() || stlFilename2.IsEmpty()) {
		// 如果STL文件路径为空，则禁用滚动条或者设置其范围为默认值
		scrollBar->EnableWindow(FALSE);
		scrollBar->SetScrollRange(0, 0); // 将滚动条范围设置为0，禁止滑动
		return;
	}

	int minpos;
	int maxpos;
	scrollBar->GetScrollRange(&minpos, &maxpos);
	maxpos = scrollBar->GetScrollLimit();

	// 获取滚动条的当前位置
	int curpos = scrollBar->GetScrollPos();

	// 确定滚动条的新位置
	switch (nSBCode)
	{
	case SB_LEFT:      // 滚动到最左边
		curpos = minpos;
		break;

	case SB_RIGHT:      // 滚动到最右边
		curpos = maxpos;
		break;

	case SB_ENDSCROLL:   // 滚动结束
		break;

	case SB_LINELEFT:      // 向左滚动
		if (curpos > minpos)
			curpos -= 1; // 递减1
		break;

	case SB_LINERIGHT:   // 向右滚动
		if (curpos < maxpos)
			curpos += 1; // 递增1
		break;

	case SB_PAGELEFT:    // 向左翻页
		if (curpos > minpos)
			curpos = max(minpos, curpos - 10); // 每页滚动10
		break;

	case SB_PAGERIGHT:      // 向右翻页
		if (curpos < maxpos)
			curpos = min(maxpos, curpos + 10); // 每页滚动10
		break;

	case SB_THUMBPOSITION: // 滚动到绝对位置
	case SB_THUMBTRACK:    // 拖动滚动块到指定位置
		curpos = nPos;      // 拖动操作结束时的位置或拖动滚动块到的位置
		break;
	}

	// 更新滚动条位置
	scrollBar->SetScrollPos(curpos);
	UpdateData();
	UpdateWindow();

	// 更新相应的编辑框
	CString strPos;
	strPos.Format(_T("%.1f"), curpos / 10.0); // 将滚动条位置除以10，得到精确到0.1的值
	pDialog->SetDlgItemText(editBoxID, strPos);
}

void CVTKShowPicDlg::UpdateModelProperties()
{
	if (stlFilename1.IsEmpty() || stlFilename2.IsEmpty()) {
		// 如果STL文件路径为空，则禁用滚动条或者设置其范围为默认值
	// 显示保存成功的提示消息
		AfxMessageBox(_T("Please open stl model!"));
		return;
	}

	CMyStyle* style = dynamic_cast<CMyStyle*>(interactor->GetInteractorStyle());
	if (style && style->SelectedActor)
	{
		// 处理平移滚动条消息
		// 获取滚动条当前位置
		double translateX = m_translateXSlider.GetScrollPos();
		double translateY = m_translateYSlider.GetScrollPos();
		double translateZ = m_translateZSlider.GetScrollPos();
		// 处理旋转滚动条消息
		// 获取滚动条当前位置
		double rotateX = m_rotateXSlider.GetScrollPos();
		double rotateY = m_rotateYSlider.GetScrollPos();
		double rotateZ = m_rotateZSlider.GetScrollPos();
		// 处理颜色滚动条消息
		// 获取滚动条当前位置
		double color = (m_colorSlider.GetScrollPos() - 1.0) / 99.0; // 根据实际范围调整，将值映射到 0 到 1 的范围
		double colorRed = m_colorSlider2.GetScrollPos() / 255.0;	// 处理颜色滚动条消息
		double colorGreen = m_colorSlider3.GetScrollPos() / 255.0;// 获取滚动条当前位置
		double colorBlue = m_colorSlider4.GetScrollPos() / 255.0;

		// 创建 VTK 变换
		vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
		transform->Identity(); // 使用更现代的 API

		transform->Translate(translateX, translateY, translateZ);
		transform->RotateX(rotateX);
		transform->RotateY(rotateY);
		transform->RotateZ(rotateZ);


		vtkSmartPointer<vtkMatrix4x4> testMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
		testMatrix->Identity();

		vtkSmartPointer<vtkMatrix4x4> testRMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
		testRMatrix->Identity();
		 //应用变换到模型
		//if (m_iStyle->LastPickedActor)
		//{
		//	m_iStyle->LastPickedActor->SetUserTransform(transform);

		//}
		
		testMatrix = transform->GetMatrix();

		// 应用变换到选中的模型
	    style->SelectedActor->SetUserTransform(transform);
		style->SelectedActor->GetProperty()->SetColor(colorRed, colorGreen, colorBlue);
		style->SelectedActor->GetProperty()->SetOpacity(color);
		
		// 重新渲染
		RenderModel();
	}
}

///<实现每个滑动条的滑动，并于编辑框对应
void CVTKShowPicDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	
	if (pScrollBar != nullptr)
	{
		if (pScrollBar == &m_translateXSlider)
		{
			HandleScrollBarEvent(&m_translateXSlider, nSBCode, nPos, IDC_HSCROLL_EDIT, this);
			// 创建 VTK 平移变换
			UpdateModelProperties();

		}
		else if (pScrollBar == &m_translateYSlider)
		{
			HandleScrollBarEvent(&m_translateYSlider, nSBCode, nPos, IDC_HSCROLL_EDIT2, this);
			// 创建 VTK 平移变换
			UpdateModelProperties();
		}
		else if (pScrollBar == &m_translateZSlider)
		{
			HandleScrollBarEvent(&m_translateZSlider, nSBCode, nPos, IDC_HSCROLL_EDIT3, this);
			// 创建 VTK 平移变换
			UpdateModelProperties();

		}
		else if (pScrollBar == &m_rotateXSlider)
		{
			HandleScrollBarEvent(&m_rotateXSlider, nSBCode, nPos, IDC_HSCROLL_EDIT4, this);
			// 创建 VTK 旋转变换
			UpdateModelProperties();

		}
		else if (pScrollBar == &m_rotateYSlider)
		{
			HandleScrollBarEvent(&m_rotateYSlider, nSBCode, nPos, IDC_HSCROLL_EDIT5, this);
			// 创建 VTK 旋转变换
			UpdateModelProperties();
		}
		else if (pScrollBar == &m_rotateZSlider)
		{
			HandleScrollBarEvent(&m_rotateZSlider, nSBCode, nPos, IDC_HSCROLL_EDIT6, this);
			// 创建 VTK 旋转变换
			UpdateModelProperties();
		}
		// 可以继续添加其他滑动条的处理
		else if (pScrollBar == &m_colorSlider)
		{

			HandleScrollBarEvent(&m_colorSlider, nSBCode, nPos, IDC_EDIT3, this);
			UpdateModelProperties();
		}
		else if (pScrollBar == &m_colorSlider2)
		{

			HandleScrollBarEvent(&m_colorSlider2, nSBCode, nPos, IDC_EDIT4, this);
			// 设置模型的颜色
			UpdateModelProperties();
		}
		else if (pScrollBar == &m_colorSlider3)
		{

			HandleScrollBarEvent(&m_colorSlider3, nSBCode, nPos, IDC_EDIT5, this);
			UpdateModelProperties();
		}
		else if (pScrollBar == &m_colorSlider4)
		{

			HandleScrollBarEvent(&m_colorSlider4, nSBCode, nPos, IDC_EDIT6, this);
			UpdateModelProperties();
		}
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);


}

//void CVTKShowPicDlg::OnNMThemeChangedTranslateXSlider(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	// 处理平移滚动条消息
//	// 获取滚动条当前位置
//	double translateX = m_translateXSlider.GetScrollPos(); // 将滚动条值转换为适当的平移量
//	//double translateY = m_translateYSlider.GetScrollPos();
//	//double translateZ = m_translateZSlider.GetScrollPos();
//
//
//	vtkSmartPointer<vtkMatrix4x4> testMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
//	testMatrix.Get()->Identity();
//	//// 创建 VTK 平移变换
//	//transform = vtkSmartPointer<vtkTransform>::New();
//	//transform->Translate(translateX, translateY, translateZ);
//
//	testMatrix.Get()->SetElement(0, 3, translateX);
//	//testMatrix.Get()->SetElement(1, 3, translateY);
//	//testMatrix.Get()->SetElement(2, 3, translateZ);
//	// 应用变换到模型
//	//stlActor->SetUserTransform(transform);
//	stlActor->SetUserMatrix(testMatrix);
//	RenderModel();
//	*pResult = 0;
//
//}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVTKShowPicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//保存bmp图片代码
void CVTKShowPicDlg::OnBnClickedSaveBmpButton()
{
	// 创建文件对话框
	CFileDialog dlg(FALSE, _T(".bmp"), NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_ENABLESIZING, _T("BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"), NULL);

	// 显示文件对话框
	if (dlg.DoModal() == IDOK)
	{
		// 获取用户选择的文件路径
		CString filePath = dlg.GetPathName();

		// 创建一个 VTK 渲染窗口截图
		vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
		windowToImageFilter->SetInput(renderWindow);
		windowToImageFilter->SetInputBufferTypeToRGBA(); // 使用 RGBA 缓冲区类型，以支持 alpha 通道
		windowToImageFilter->ReadFrontBufferOff(); // 禁用读取前缓冲区，以确保渲染完成后截图

		// 创建一个 BMP 编码器
		vtkSmartPointer<vtkBMPWriter> bmpWriter = vtkSmartPointer<vtkBMPWriter>::New();
		bmpWriter->SetInputConnection(windowToImageFilter->GetOutputPort());

		// 设置保存文件路径
		bmpWriter->SetFileName(CT2A(filePath)); // 将 CString 转换为 char* 类型

		// 写入 BMP 文件
		bmpWriter->Write();

		// 显示保存成功的提示消息
		AfxMessageBox(_T("BMP image saved successfully!"));
	}
}

// 读取 .cam 文件，并设置 VTK 相机对象
/**void setCameraParametersFromCamFile(const CString& camFilePath, vtkCamera* camera)
{
	std::ifstream camFile(camFilePath);
	if (camFile.is_open()) {
		double focalLengthX, focalLengthY;
		double opticalCenterX, opticalCenterY;
		int imageWidth, imageHeight;

		// 读取参数值
		camFile >> focalLengthX >> focalLengthY;
		camFile >> opticalCenterX >> opticalCenterY;
		camFile >> imageWidth >> imageHeight;

		// 设置相机的焦距
		camera->SetFocalPoint(focalLengthX, focalLengthY, 0.0);

		// 设置相机的图像中心（在VTK中没有设置相机光学中心的方法，可以近似认为是图像中心）
		camera->SetViewUp(0, 1, 0); // 设置相机的上向量
		camera->SetPosition(0, 0, 0); // 设置相机位置为原点

		// 设置相机的图像分辨率
		camera->SetWindowCenter(imageWidth / 2.0, imageHeight / 2.0);
	}
	else {
		std::cerr << "Failed to open .cam file: " << camFilePath << std::endl;
	}
}
**/
//退出程序
//void CVTKShowPicDlg::OnBnClickedExitButton()
//{
//	// TODO: 退出程序
//	PostQuitMessage(0);
//}

// 读取 .cam 文件，并设置 VTK 相机对象
void CVTKShowPicDlg::setCameraParametersButton()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("CAM Files (*.cam)|*.cam||"), this);
	if (dlg.DoModal() == IDOK)
	{
		CString camFilePath = dlg.GetPathName();
		SetDlgItemText(IDC_EDIT2, camFilePath);

		// 创建 VTK 相机对象的智能指针
		camera = vtkSmartPointer<vtkCamera>::New();
		std::ifstream camFile(camFilePath);
		if (camFile.is_open()) {
			double focalLengthX, focalLengthY;
			double opticalCenterX, opticalCenterY;
			int imageWidth, imageHeight;

			// 读取参数值
			camFile >> focalLengthX >> focalLengthY;
			camFile >> opticalCenterX >> opticalCenterY;
			camFile >> imageWidth >> imageHeight;

			// 设置相机的焦距
			camera->SetFocalPoint(focalLengthX, focalLengthY, 0.0);

			// 设置相机的图像中心（在VTK中没有设置相机光学中心的方法，可以近似认为是图像中心）
			camera->SetViewUp(0, 1, 0); // 设置相机的上向量
			camera->SetPosition(0, 0, 0); // 设置相机位置为原点

			// 设置相机的图像分辨率
			camera->SetWindowCenter(imageWidth / 2.0, imageHeight / 2.0);
		}
		else {
			std::cerr << "Failed to open .cam file: " << camFilePath << std::endl;
		}
	}
}

LRESULT CVTKShowPicDlg::OnCurPosMsg(WPARAM wParam, LPARAM lParam)
{
	//double *pos = m_iStyle->GetCurPos();
	double* pos = (double*)wParam;
	CString str;
	str.Format("(%.2f,%.2f,%.2f)", pos[0], pos[1], pos[2]);
	m_listBox.AddString(str); // 将空间坐标信息添加到列表框中
	return 0;
}

void CVTKShowPicDlg::SaveModelPoses(const char* filename)
{
	std::ofstream outFile;
	outFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try
	{
		outFile.open(filename);

		// 检查 stlActor 和 stlActor2 的矩阵
		vtkSmartPointer<vtkMatrix4x4> temp1 = stlActor->GetUserMatrix();
		if (!temp1) temp1 = stlActor->GetMatrix();
		if (!temp1)
		{
			std::cerr << "Error: Model 1 does not have a valid transformation matrix!" << std::endl;
			return;
		}

		vtkSmartPointer<vtkMatrix4x4> temp2 = stlActor2->GetUserMatrix();
		if (!temp2) temp2 = stlActor2->GetMatrix();
		if (!temp2)
		{
			std::cerr << "Error: Model 2 does not have a valid transformation matrix!" << std::endl;
			return;
		}

		// 写入模型1的矩阵
		outFile << "model1 "
			<< temp1->GetElement(0, 0) << " ，" << temp1->GetElement(0, 1) << " ，" << temp1->GetElement(0, 2) << " ，" << temp1->GetElement(0, 3) << " ，"
			<< temp1->GetElement(1, 0) << " ，" << temp1->GetElement(1, 1) << " ，" << temp1->GetElement(1, 2) << " ，" << temp1->GetElement(1, 3) << " ，"
			<< temp1->GetElement(2, 0) << " ，" << temp1->GetElement(2, 1) << " ，" << temp1->GetElement(2, 2) << " ，" << temp1->GetElement(2, 3) << " ，"
			<< temp1->GetElement(3, 0) << " ，" << temp1->GetElement(3, 1) << " ，" << temp1->GetElement(3, 2) << " ，" << temp1->GetElement(3, 3) << std::endl;

		// 写入模型2的矩阵
		outFile << "model2 "
			<< temp2->GetElement(0, 0) << " ，" << temp2->GetElement(0, 1) << " ，" << temp2->GetElement(0, 2) << " ，" << temp2->GetElement(0, 3) << " ，"
			<< temp2->GetElement(1, 0) << " ，" << temp2->GetElement(1, 1) << " ，" << temp2->GetElement(1, 2) << " ，" << temp2->GetElement(1, 3) << " ，"
			<< temp2->GetElement(2, 0) << " ，" << temp2->GetElement(2, 1) << " ，" << temp2->GetElement(2, 2) << " ，" << temp2->GetElement(2, 3) << " ，"
			<< temp2->GetElement(3, 0) << " ，" << temp2->GetElement(3, 1) << " ，" << temp2->GetElement(3, 2) << " ，" << temp2->GetElement(3, 3) << std::endl;

		outFile.close();
	}
	catch (const std::ofstream::failure& e)
	{
		std::cerr << "Error opening or writing to file: " << e.what() << std::endl;
	}
}

void CVTKShowPicDlg::OnBnClickedSavePoses()
{
	if (stlFilename1.IsEmpty() || stlFilename2.IsEmpty()) {
		// 如果STL文件路径为空，则警告不能够保存位姿矩阵

		AfxMessageBox(_T("没有加载STL模型!"));
	}
	else
	{
	SaveModelPoses("model_poses.txt");
	}
	
}
void CVTKShowPicDlg::OnBnClickedButton5()
{
	//// 创建一个新的 VTK 渲染对话框
	CVTKRenderDialog* pRenderDialog = new CVTKRenderDialog();

	// 调用 DoModal() 以显示渲染窗口
	pRenderDialog->DoModal();

	//// 释放对话框
	//delete pRenderDialog;
	//LoadModelPoses("model_poses.txt", stlActor, stlActor2);
	//renderWindow->Render();  // 更新渲染
}
