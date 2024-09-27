
// VTK_ShowPicDlg.h: 头文件
//

#pragma once
#include "afxwin.h"
#include "vtkSmartPointer.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkImageActor.h>
#include"vtkPolyData.h"
#include "CMyStyle.h"


// CVTKShowPicDlg 对话框
class CVTKShowPicDlg : public CDialogEx
{
// 构造
public:
	CVTKShowPicDlg(CWnd* pParent = nullptr) ;
		// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VTK_SHOWPIC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// VTK 渲染器和交互器
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderer> stlRenderer;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor> interactor;
	//vtkSmartPointer<CMyStyle> interactorStyle;
	vtkSmartPointer<vtkActor> stlActor;
	vtkSmartPointer<vtkActor> stlActor2;
	vtkSmartPointer<vtkTransform> transform;
	vtkSmartPointer<vtkCamera> camera;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void HandleScrollBarEvent(CScrollBar* scrollBar, UINT nSBCode, UINT nPos, int editBoxID, CDialogEx* pDialog);
	afx_msg void OnLoadVectorImageButtonClicked();
	//afx_msg void OnNMThemeChangedTranslateXSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnCurPosMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedSaveBmpButton();
	afx_msg void OnBnClickedExitButton();
	afx_msg void setCameraParametersButton();

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void RenderBMPAndSTL(const CString& bmpFilename, const CString& stlFilename1, const CString& stlFilename2);
	void RenderModel();
	void SaveModelPoses(const char* filename);
	void UpdateModelProperties();
	void OnBnClickedSavePoses();
	void setCameraParametersFromCamFile(const CString& camFilePath, vtkSmartPointer<vtkCamera> camera);
	// 声明滚动条控件变量
	CString stlFilename1;
	CString stlFilename2;
	CScrollBar m_translateXSlider;
	CScrollBar m_translateYSlider;
	CScrollBar m_translateZSlider;
	CScrollBar m_rotateXSlider;
	CScrollBar m_rotateYSlider;
	CScrollBar m_rotateZSlider;
	CScrollBar m_colorSlider;
	CScrollBar m_colorSlider2;
	CScrollBar m_colorSlider3;
	CScrollBar m_colorSlider4;
	CListBox m_listBox;

	afx_msg void OnBnClickedButton5();
};
