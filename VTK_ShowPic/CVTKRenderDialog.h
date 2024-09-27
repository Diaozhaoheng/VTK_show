#pragma once
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSTLReader.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>

// CVTKRenderDialog 对话框
class CVTKRenderDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CVTKRenderDialog)

public:
    CVTKRenderDialog(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CVTKRenderDialog();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_VTK_RENDER_DIALOG }; // 对话框资源 ID
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    
    DECLARE_MESSAGE_MAP()

private:
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;

    CString STLFilename1;
    CString STLFilename2;
public:
    virtual BOOL OnInitDialog();  // 对话框初始化
    afx_msg void OnStnClickedVtkRenderShow();
 
};

