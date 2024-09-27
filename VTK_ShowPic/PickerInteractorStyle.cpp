#include "pch.h"
#include "PickerInteractorStyle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CPickerInteractorStyle::CPickerInteractorStyle()
{
	m_Outline = NULL;
	m_SplineWidget = NULL;
	m_nNumOfHandles = 0;
	m_Points = vtkSmartPointer<vtkPoints>::New();
}


CPickerInteractorStyle::~CPickerInteractorStyle()
{
}

void CPickerInteractorStyle::OnLeftButtonDown()
{
	//鼠标事件在窗口中的位置
	int clickPos[2];
	this->GetInteractor()->GetEventPosition(clickPos);
	vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();

	vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();
	cellPicker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
	int nCellID = cellPicker->GetCellId();
	

	//拾取位置
	double* pos1 = cellPicker->GetPickPosition();

	picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

	double* pos = picker->GetPickPosition();
	vtkSmartPointer<vtkPolyData> seed = vtkSmartPointer<vtkPolyData>::New();
	//Ctrl,Alt键按下
	if (this->GetInteractor()->GetControlKey() == 8 &&
		this->GetInteractor()->GetAltKey() == -128)
	{
		m_nNumOfHandles++;
		if (m_SplineWidget)
		{
			if (m_nNumOfHandles == 1)
			{
				//m_SplineWidget->SetNumberOfHandles(++m_nNumOfHandles);
				m_SplineWidget->SetHandlePosition(0, pos);
				m_SplineWidget->SetHandlePosition(1, pos);
				m_SplineWidget->EnabledOn();
				m_SplineWidget->On();
			}
			else
			{
				//m_SplineWidget->SetNumberOfHandles(m_nNumOfHandles);
				m_SplineWidget->SetHandlePosition(m_nNumOfHandles - 1, pos);
			}
		}

		/*vtkCallbackCommand *myCallback = vtkCallbackCommand::New();
		myCallback->SetType(0);
		myCallback->PolyData = seed;

		pointWidget->AddObserver(vtkCommand::StartInteractionEvent, myCallback);
		pointWidget->On();*/
	}

	vtkSmartPointer<vtkProperty> p1 = vtkSmartPointer<vtkProperty>::New();
	p1->SetColor(1., .6, .4);
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
		// Save the property of the picked actor so that we can restore it next time
		//this->LastPickedProperty->DeepCopy(this->LastPickedActor->GetProperty());
		// Highlight the picked actor by changing its properties
		//this->LastPickedActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
		//this->LastPickedActor->GetProperty()->SetDiffuse(1.0);
		//this->LastPickedActor->GetProperty()->SetSpecular(0.0);
	}
//	this->GetInteractor()->Initialize();
//	this->GetInteractor()->Start();

	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}
void CPickerInteractorStyle::SetData(vtkAlgorithmOutput* data)
{
	m_SplineWidget = vtkSmartPointer<vtkSplineWidget>::New();
	m_SplineWidget->SetInteractor(this->GetInteractor());
	m_SplineWidget->SetInputConnection(data);
	m_SplineWidget->SetPriority(1.0);
	m_SplineWidget->KeyPressActivationOff();
	m_SplineWidget->PlaceWidget();
	m_SplineWidget->ProjectToPlaneOff();
	m_SplineWidget->ClosedOff();
	m_SplineWidget->SetHandleSize(.003);
}

void CPickerInteractorStyle::OnKeyDown()
{
	vtkInteractorStyleTrackballCamera::OnKeyDown();
}


void CPickerInteractorStyle::OnKeyUp()
{

	vtkInteractorStyleTrackballCamera::OnKeyUp();
}

void CPickerInteractorStyle::OnMouseMove()
{
	vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void CPickerInteractorStyle::OnRightButtonDown()
{
	vtkInteractorStyleTrackballCamera::OnRightButtonDown();
}