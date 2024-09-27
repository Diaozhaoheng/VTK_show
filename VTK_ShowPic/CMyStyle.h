#pragma once
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>
#include <vtkCellPicker.h>
#include <vtkProperty.h>
#include <vtkTransform.h>
#include "VTK_ShowPicDlg.h"

class CMyStyle : public vtkInteractorStyleTrackballActor
{
public:
    static CMyStyle* New();
    vtkTypeMacro(CMyStyle, vtkInteractorStyleTrackballActor);

    CMyStyle();
    ~CMyStyle() override = default;

    vtkSmartPointer<vtkActor> SelectedActor = nullptr;
    vtkSmartPointer<vtkActor> LastPickedActor = nullptr;

    // 滑动条指针
    CScrollBar* m_translateXSlider = nullptr;
    CScrollBar* m_translateYSlider = nullptr;
    CScrollBar* m_translateZSlider = nullptr;
    CScrollBar* m_rotateXSlider = nullptr;
    CScrollBar* m_rotateYSlider = nullptr;
    CScrollBar* m_rotateZSlider = nullptr;

    // 鼠标移动事件
    void OnMouseMove() override;

    // 左键按下事件
    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;

protected:
    double m_dCurPos[3] = { 0.0, 0.0, 0.0 };
    CDialogEx* m_pDlg = nullptr;

public:
    double* GetCurPos();
    void SetDlg(CDialogEx* pDlg);
}; 

