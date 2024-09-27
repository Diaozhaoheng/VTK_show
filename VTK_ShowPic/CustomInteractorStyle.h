#ifndef CUSTOM_INTERACTOR_STYLE_H
#define CUSTOM_INTERACTOR_STYLE_H
#include"pch.h"
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <afxwin.h>
//#include <CScrollBar.h>

class CustomInteractorStyle : public vtkInteractorStyleTrackballActor
{
public:
    static CustomInteractorStyle* New();
    vtkTypeMacro(CustomInteractorStyle, vtkInteractorStyleTrackballActor);

    virtual void OnMouseMove() override;

    void SetCurrentActor(vtkActor* actor);
    void SetScrollBar(CScrollBar* scrollBar);

private:
    vtkActor* CurrentActor = nullptr;
    CScrollBar* ScrollBar = nullptr;

    void UpdateScrollBarWithActorPosition();
};

#endif // CUSTOM_INTERACTOR_STYLE_H
