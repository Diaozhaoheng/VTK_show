#pragma once
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSTLReader.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>

// CVTKRenderDialog �Ի���
class CVTKRenderDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CVTKRenderDialog)

public:
    CVTKRenderDialog(CWnd* pParent = nullptr);   // ��׼���캯��
    virtual ~CVTKRenderDialog();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_VTK_RENDER_DIALOG }; // �Ի�����Դ ID
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    
    DECLARE_MESSAGE_MAP()

private:
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;

    CString STLFilename1;
    CString STLFilename2;
public:
    virtual BOOL OnInitDialog();  // �Ի����ʼ��
    afx_msg void OnStnClickedVtkRenderShow();
 
};

