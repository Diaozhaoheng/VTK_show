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

    // ���ô��ڴ�С
    this->SetWindowPos(nullptr, 100, 100, 800, 600, SWP_NOZORDER);

    // �����ļ��Ի���ѡ���һ��STL�ļ�
    CFileDialog dlg1(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("STL Files (*.stl)|*.stl||"), this);
    if (dlg1.DoModal() == IDOK)
    {
        STLFilename1 = dlg1.GetPathName();
        // �����ļ��Ի���ѡ��ڶ���STL�ļ�
        CFileDialog dlg2(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("STL Files (*.stl)|*.stl||"), this);
        if (dlg2.DoModal() == IDOK)
        {
            STLFilename2 = dlg2.GetPathName();
            // �����ļ��Ի���ѡ�����λ��TXT�ļ�
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
                        // ��ȡ 4x4 ����
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
                        // ��ȡ 4x4 ����
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


                // ������һ�� STL ģ�͵Ķ�ȡ��
                vtkSmartPointer<vtkSTLReader> reader1 = vtkSmartPointer<vtkSTLReader>::New();
                reader1->SetFileName(STLFilename1);
                reader1->Update();

                vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper1->SetInputConnection(reader1->GetOutputPort());

                vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
                actor1->SetMapper(mapper1);
               // actor1->GetProperty()->SetColor(1.0, 0.0, 0.0);  // ��ɫ
                actor1->SetUserMatrix(tempMatrix1);

                // �����ڶ��� STL ģ�͵Ķ�ȡ��
                vtkSmartPointer<vtkSTLReader> reader2 = vtkSmartPointer<vtkSTLReader>::New();
                reader2->SetFileName(STLFilename2);
                reader2->Update();

                vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper2->SetInputConnection(reader2->GetOutputPort());

                vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
                actor2->SetMapper(mapper2);
                //actor2->GetProperty()->SetColor(0.0, 1.0, 0.0);  // ��ɫ
                actor2->SetUserMatrix(tempMatrix2);
                // ������Ⱦ��
                renderer = vtkSmartPointer<vtkRenderer>::New();
                renderer->AddActor(actor1);
                renderer->AddActor(actor2);
                renderer->SetBackground(0, 0, 255);  // ��ɫ����
                
                // ������Ⱦ����
                renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
                renderWindow->AddRenderer(renderer);

                // ����������
                interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
                interactor->SetRenderWindow(renderWindow);

                //// �� VTK ��Ⱦ����Ƕ�뵽 MFC �� CStatic �ؼ���
                //CWnd* pRenderWindow = GetDlgItem(IDC_STATIC_VTK_RENDER); // ��ȡ CStatic �ؼ�
                //if (!pRenderWindow)
                //{
                //    AfxMessageBox(_T("Failed to get CStatic control!"));
                //    return FALSE;
                //}
                //if (pRenderWindow)
                //{
                //    HWND hWnd = pRenderWindow->GetSafeHwnd();
                //    //renderWindow->SetWindowInfo(reinterpret_cast<void*>(hWnd));

                //    // ��ȡ CStatic �ؼ��ĳߴ�
                //    CRect rect;
                //    pRenderWindow->GetClientRect(&rect);
                //    renderWindow->SetSize(rect.Width(), rect.Height());

                //    renderWindow->SetParentId(hWnd);
                //}
               
                // ��ʼ��Ⱦ
                renderWindow->Render();

                // ��ʼ����
                interactor->Initialize();
                interactor->Start();
            }

        }
    }

    return TRUE;
}


void CVTKRenderDialog::OnStnClickedVtkRenderShow()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}
