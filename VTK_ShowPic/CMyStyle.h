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

    // ������ָ��
    CScrollBar* m_translateXSlider = nullptr;
    CScrollBar* m_translateYSlider = nullptr;
    CScrollBar* m_translateZSlider = nullptr;
    CScrollBar* m_rotateXSlider = nullptr;
    CScrollBar* m_rotateYSlider = nullptr;
    CScrollBar* m_rotateZSlider = nullptr;

    // ����ƶ��¼�
    void OnMouseMove() override;

    // ��������¼�
    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;

protected:
    double m_dCurPos[3] = { 0.0, 0.0, 0.0 };
    CDialogEx* m_pDlg = nullptr;

public:
    double* GetCurPos();
    void SetDlg(CDialogEx* pDlg);
}; 

