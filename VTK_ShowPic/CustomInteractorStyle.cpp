#include"pch.h"
#include "CustomInteractorStyle.h"
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include<vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCoordinate.h>
#include<Windows.h>

 vtkStandardNewMacro(CustomInteractorStyle);

void CustomInteractorStyle::OnMouseMove()
{
    vtkRenderWindowInteractor* interactor = this->GetInteractor();
    int* clickPos = interactor->GetInteractorStyle()->GetInteractor()->GetLastEventPosition();

    if (this->CurrentActor)
    {
        // ��ȡ��ǰ���λ��
        int x = clickPos[0];
        int y = clickPos[1];

        // ������������
        vtkRenderer* renderer = this->GetDefaultRenderer();
        vtkSmartPointer<vtkCoordinate> coordinate = vtkSmartPointer<vtkCoordinate>::New();
        coordinate->SetCoordinateSystemToNormalizedDisplay();
        coordinate->SetValue(x, y, 0.0);

        double* worldPos = coordinate->GetComputedWorldValue(renderer);

        // ����ģ�͵�ƽ�Ʊ任
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->Translate(worldPos[0], worldPos[1], worldPos[2]);
        this->CurrentActor->SetUserTransform(transform);

        // ���¹�������λ��
        UpdateScrollBarWithActorPosition();
    }

    vtkInteractorStyleTrackballActor::OnMouseMove();
}

void CustomInteractorStyle::SetCurrentActor(vtkActor* actor)
{
    this->CurrentActor = actor;
}

void CustomInteractorStyle::SetScrollBar(CScrollBar* scrollBar)
{
    this->ScrollBar = scrollBar;
}

void CustomInteractorStyle::UpdateScrollBarWithActorPosition()
{
    if (this->ScrollBar && this->CurrentActor)
    {
        double pos[3];
        this->CurrentActor->GetPosition(pos);

        // ��ģ��λ��ӳ�䵽��������ֵ
        int scrollPos = static_cast<int>(pos[0] * 10); // ����: ӳ��Xλ�õ�������
        this->ScrollBar->SetScrollPos(scrollPos);
    }
}
