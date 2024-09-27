#include "pch.h"
#include "CMyStyle.h"

vtkStandardNewMacro(CMyStyle);

CMyStyle::CMyStyle()
{
    this->SelectedActor = nullptr;
    this->LastPickedActor = nullptr;
    this->m_pDlg = nullptr;
}

void CMyStyle::OnMouseMove()
{
    // 调用父类方法，处理基本的交互
    

    if (this->InteractionProp)
    {
        // 获取当前的变换
        vtkLinearTransform* linearTransform = this->InteractionProp->GetUserTransform();
        vtkTransform* transform = vtkTransform::SafeDownCast(linearTransform);

        if (!transform)
        {
            // 如果没有变换，则创建一个新的变换并设置
            transform = vtkTransform::New();
            transform->PostMultiply(); // 确保新的变换应用在现有变换之后
            this->InteractionProp->SetUserTransform(transform);
            transform->Delete(); // 因为 SetUserTransform 会增加引用计数，所以这里可以安全删除
        }

        // 获取位移信息
        double transposition[3];
        double rotateposition[3];
        transform->GetPosition(transposition);

        if (this->LastPickedActor)
        {
            double* pos = this->LastPickedActor->GetCenter();
            m_dCurPos[0] = pos[0];
            m_dCurPos[1] = pos[1];
            m_dCurPos[2] = pos[2];

            CString sTmp;
            sTmp.Format("%f,%f,%f", m_dCurPos[0], m_dCurPos[1], m_dCurPos[2]);
            AfxGetMainWnd()->SetWindowText(sTmp);

            if (m_pDlg)
            {
                m_pDlg->SendMessage(UM_CUR_MSG_POS, (WPARAM)m_dCurPos, 3);
            }
        }

        // 同步更新平移滑动条
        if (m_translateXSlider)
            m_translateXSlider->SetScrollPos(static_cast<int>(transposition[0]));

        if (m_translateYSlider)
            m_translateYSlider->SetScrollPos(static_cast<int>(transposition[1]));

        if (m_translateZSlider)
            m_translateZSlider->SetScrollPos(static_cast<int>(transposition[2]));
        // 同步更新旋转滑动条
        if (m_rotateXSlider)
            m_rotateXSlider->SetScrollPos(static_cast<int>(rotateposition[0]));

        if (m_rotateYSlider)
            m_rotateYSlider->SetScrollPos(static_cast<int>(rotateposition[1]));

        if (m_rotateZSlider)
            m_rotateZSlider->SetScrollPos(static_cast<int>(rotateposition[2]));
    }

    vtkInteractorStyleTrackballActor::OnMouseMove();
}

void CMyStyle::OnLeftButtonDown()
{
    int* clickPos = this->GetInteractor()->GetEventPosition();

    // 创建拾取器并执行拾取操作
    vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
    picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

    vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    cellPicker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
    int nCellID = cellPicker->GetCellId();

    //拾取位置
    double* pos1 = cellPicker->GetPickPosition();

    picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

    double* pos = picker->GetPickPosition();
    vtkSmartPointer<vtkPolyData> seed = vtkSmartPointer<vtkPolyData>::New();

    ///< 改变拾取actor的属性
    vtkSmartPointer<vtkProperty> p1 = vtkSmartPointer<vtkProperty>::New();
    p1->SetColor(1., .0, .0);
    p1->SetDiffuse(1.);
    p1->SetSpecular(.0);
    p1->SetOpacity(.6);

    vtkSmartPointer<vtkProperty> p2 = vtkSmartPointer<vtkProperty>::New();
    p2->SetColor(1., 1.0, 1.0);
    p2->SetDiffuse(1.);
    p2->SetSpecular(.0);
    p2->SetOpacity(.6);
    // If we picked something before, reset its property
    if (this->LastPickedActor)
    {
        this->LastPickedActor->SetProperty(p2);
        //this->LastPickedActor->GetProperty()->DeepCopy(this->LastPickedProperty);
    }
    this->LastPickedActor = picker->GetActor();
    if (this->LastPickedActor)
    {
        this->LastPickedActor->SetProperty(p1);
    }


    // 获取被拾取的 Actor
    vtkActor* actor = picker->GetActor();

    if (actor != nullptr)
    {
        this->SelectedActor = actor;
        std::cout << "STL Model Selected" << std::endl;
    }
    else
    {
        this->SelectedActor = nullptr;
    }

    // 调用父类方法以保留其他交互行为
    vtkInteractorStyleTrackballActor::OnLeftButtonDown();
}

void CMyStyle::OnLeftButtonUp()
{
    if (this->LastPickedActor)
    {
        this->LastPickedActor = nullptr;
    }
    vtkInteractorStyleTrackballActor::OnLeftButtonUp();
}

double* CMyStyle::GetCurPos()
{
    return this->m_dCurPos;
}

void CMyStyle::SetDlg(CDialogEx* pDlg)
{
    this->m_pDlg = pDlg;
}
