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
        // 获取当前鼠标位置
        int x = clickPos[0];
        int y = clickPos[1];

        // 计算世界坐标
        vtkRenderer* renderer = this->GetDefaultRenderer();
        vtkSmartPointer<vtkCoordinate> coordinate = vtkSmartPointer<vtkCoordinate>::New();
        coordinate->SetCoordinateSystemToNormalizedDisplay();
        coordinate->SetValue(x, y, 0.0);

        double* worldPos = coordinate->GetComputedWorldValue(renderer);

        // 更新模型的平移变换
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->Translate(worldPos[0], worldPos[1], worldPos[2]);
        this->CurrentActor->SetUserTransform(transform);

        // 更新滚动条的位置
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

        // 将模型位置映射到滚动条的值
        int scrollPos = static_cast<int>(pos[0] * 10); // 例如: 映射X位置到滚动条
        this->ScrollBar->SetScrollPos(scrollPos);
    }
}
