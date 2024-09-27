#include "pch.h"
#include <fstream>
#include <sstream>
#include <string>
#include "VTK_ShowPic.h"
#include "VTK_ShowPicDlg.h"
#include "vtkProperty.h"
#include "CVTKRenderDialog.h"
#include "afxdialogex.h"
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>


IMPLEMENT_DYNAMIC(CVTKRenderDialog, CDialogEx)

CVTKRenderDialog::CVTKRenderDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_VTK_RENDER_DIALOG, pParent)
{
}

CVTKRenderDialog::~CVTKRenderDialog()
{
}

void CVTKRenderDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVTKRenderDialog, CDialogEx)
    ON_STN_CLICKED(IDC_VTK_RENDER_SHOW, &CVTKRenderDialog::OnStnClickedVtkRenderShow)
END_MESSAGE_MAP()

BOOL CVTKRenderDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 设置窗口大小
    this->SetWindowPos(nullptr, 100, 100, 800, 600, SWP_NOZORDER);

    // 弹出文件对话框选择第一个STL文件
    CFileDialog dlg1(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("STL Files (*.stl)|*.stl||"), this);
    if (dlg1.DoModal() == IDOK)
    {
        STLFilename1 = dlg1.GetPathName();
        // 弹出文件对话框选择第二个STL文件
        CFileDialog dlg2(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("STL Files (*.stl)|*.stl||"), this);
        if (dlg2.DoModal() == IDOK)
        {
            STLFilename2 = dlg2.GetPathName();
            // 弹出文件对话框选择相对位置TXT文件
            CFileDialog posDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("Text Files (*.txt)|*.txt||"), this);
            if (posDlg.DoModal() == IDOK)
            {
                CString posFileName = posDlg.GetPathName();
                std::ifstream posFile((LPCTSTR)posFileName);
                std::string line;
                double pos1[3] = { 0.0, 0.0, 0.0 }, rot1[3] = { 0.0, 0.0, 0.0 };
                double pos2[3] = { 0.0, 0.0, 0.0 }, rot2[3] = { 0.0, 0.0, 0.0 };

                vtkSmartPointer<vtkMatrix4x4> tempMatrix1 = vtkSmartPointer<vtkMatrix4x4>::New();
                vtkSmartPointer<vtkMatrix4x4> tempMatrix2 = vtkSmartPointer<vtkMatrix4x4>::New();


                while (std::getline(posFile, line))
                {


                    std::istringstream iss(line);
                    std::string modelID;


                    if (line.find("model1") != std::string::npos)
                    {
                        // 读取 4x4 矩阵
                        for (int i = 0; i < 4; ++i)
                        {
                            for (int j = 0; j < 4; ++j)
                            {
                                posFile >> tempMatrix1->Element[i][j];
                            }
                        }
                    }
                    else if (line.find("model2") != std::string::npos)
                    {
                        // 读取 4x4 矩阵
                        for (int i = 0; i < 4; ++i)
                        {
                            for (int j = 0; j < 4; ++j)
                            {
                                posFile >> tempMatrix2->Element[i][j];
                            }
                        }
                    }
                }

                posFile.close();


                // 创建第一个 STL 模型的读取器
                vtkSmartPointer<vtkSTLReader> reader1 = vtkSmartPointer<vtkSTLReader>::New();
                reader1->SetFileName(STLFilename1);
                reader1->Update();

                vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper1->SetInputConnection(reader1->GetOutputPort());

                vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
                actor1->SetMapper(mapper1);
               // actor1->GetProperty()->SetColor(1.0, 0.0, 0.0);  // 红色
                actor1->SetUserMatrix(tempMatrix1);

                // 创建第二个 STL 模型的读取器
                vtkSmartPointer<vtkSTLReader> reader2 = vtkSmartPointer<vtkSTLReader>::New();
                reader2->SetFileName(STLFilename2);
                reader2->Update();

                vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper2->SetInputConnection(reader2->GetOutputPort());

                vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
                actor2->SetMapper(mapper2);
                //actor2->GetProperty()->SetColor(0.0, 1.0, 0.0);  // 绿色
                actor2->SetUserMatrix(tempMatrix2);
                // 创建渲染器
                renderer = vtkSmartPointer<vtkRenderer>::New();
                renderer->AddActor(actor1);
                renderer->AddActor(actor2);
                renderer->SetBackground(0, 0, 255);  // 蓝色背景
                
                // 创建渲染窗口
                renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
                renderWindow->AddRenderer(renderer);

                // 创建交互器
                interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
                interactor->SetRenderWindow(renderWindow);

                //// 将 VTK 渲染窗口嵌入到 MFC 的 CStatic 控件中
                //CWnd* pRenderWindow = GetDlgItem(IDC_STATIC_VTK_RENDER); // 获取 CStatic 控件
                //if (!pRenderWindow)
                //{
                //    AfxMessageBox(_T("Failed to get CStatic control!"));
                //    return FALSE;
                //}
                //if (pRenderWindow)
                //{
                //    HWND hWnd = pRenderWindow->GetSafeHwnd();
                //    //renderWindow->SetWindowInfo(reinterpret_cast<void*>(hWnd));

                //    // 获取 CStatic 控件的尺寸
                //    CRect rect;
                //    pRenderWindow->GetClientRect(&rect);
                //    renderWindow->SetSize(rect.Width(), rect.Height());

                //    renderWindow->SetParentId(hWnd);
                //}
               
                // 开始渲染
                renderWindow->Render();

                // 开始交互
                interactor->Initialize();
                interactor->Start();
            }

        }
    }

    return TRUE;
}


void CVTKRenderDialog::OnStnClickedVtkRenderShow()
{
    // TODO: 在此添加控件通知处理程序代码
}
