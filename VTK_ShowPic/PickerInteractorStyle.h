

//#if !defined(AFX_PICKERINTERACTORSTYLE_H__5FE56854_DCE4_4C82_BCBD_4DAD7A25BB81__INCLUDED_)
//#define AFX_PICKERINTERACTORSTYLE_H__5FE56854_DCE4_4C82_BCBD_4DAD7A25BB81__INCLUDED_

#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPropPicker.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCellPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkCommand.h>
#include <vtkPointPicker.h>
#include <vtkInteractorStyleImage.h>
#include <vtkLineWidget.h>
#include <vtkPointWidget.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyData.h>
#include <vtkSplineWidget.h>
#include <vtkAlgorithmOutput.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkInteractorStyleJoystickActor.h>
#include <vtkInteractorStyle.h>

//#if _MSC_VER > 1000
#pragma once
//#endif // _MSC_VER > 1000

class vtkMyCallback : public vtkCommand
{
public:
friend class vtkInteractorStyleJoystickActor;
	vtkMyCallback::vtkMyCallback() { m_pvtkActorSelection = NULL; };
	static vtkMyCallback *New() { return new vtkMyCallback; }
	void PrintSelf(ostream&, vtkIndent) { }
	void PrintTrailer(ostream&, vtkIndent) { }
	void PrintHeader(ostream&, vtkIndent) { }
	void CollectRevisions(ostream&) {}
	void SetSelectionActor(vtkActor* pvtkActorSelection) { m_pvtkActorSelection = pvtkActorSelection; };

	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkRenderWindowInteractor *iren = reinterpret_cast<vtkRenderWindowInteractor*>(caller);
		vtkPointPicker *picker = (vtkPointPicker *)iren->GetPicker();
		cout << "PointId: " << picker->GetPointId() << "\n";
		if (picker->GetPointId() != -1)
		{
			if (m_pvtkActorSelection)
				m_pvtkActorSelection->SetPosition(picker->GetPickPosition());
			iren->Render();
		}
	}
private:
	vtkActor* m_pvtkActorSelection;
};


class vtkTransInteractorStyle : public vtkInteractorStyleImage
{
	bool ctrldown;
	double mx, my, mz, oldpos[3], pickpos[3];
public:
	int MousePos[2];
	vtkPropPicker * mPicker;
	vtkRenderer	  * mRender;

	static vtkTransInteractorStyle *New()
	{
		return new vtkTransInteractorStyle;
	}
	//	virtual void OnLeftButtonDown();
	//	virtual void OnLeftButtonUp();
	//	virtual void OnRightButtonDown();
	//	virtual void OnRightButtonUp();
	//	virtual void OnMouseMove();


protected:
	vtkTransInteractorStyle() {
		ctrldown = false;
		mx = my = mz = 0;
	};
	~vtkTransInteractorStyle() {};

};


class CPickerInteractorStyle :
	public vtkInteractorStyleTrackballCamera
{
public:
	static CPickerInteractorStyle* New() { return new CPickerInteractorStyle; }
	void SetData(vtkAlgorithmOutput* data);

	CPickerInteractorStyle();
	~CPickerInteractorStyle();
	virtual void OnLeftButtonDown();
	virtual void OnKeyDown();
	virtual void OnKeyUp();
	virtual void OnMouseMove();
	virtual void OnRightButtonDown();
protected:
	vtkActor* LastPickedActor;
	vtkProperty *LastPickedProperty;
	vtkSmartPointer<vtkOutlineFilter> m_Outline;
	vtkSmartPointer<vtkSplineWidget> m_SplineWidget;
	int m_nNumOfHandles;
	//曲线点
	vtkSmartPointer<vtkPoints> m_Points;
	//初始位置
	int m_nXBegin;
	int m_nYBegin;
};


//#endif // !defined(AFX_PICKERINTERACTORSTYLE_H__5FE56854_DCE4_4C82_BCBD_4DAD7A25BB81__INCLUDED_)