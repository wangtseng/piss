#ifndef VTKRESLICECURSORCALLBACK_H
#define VTKRESLICECURSORCALLBACK_H

#include "vtkResliceCursorWidget.h"
#include "vtkCommand.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkPlaneSource.h"

#include "vtkResliceImageViewer.h"
#include "vtkDistanceWidget.h"
#include "vtkResliceImageViewerMeasurements.h"
#include "vtkCellPicker.h"
#include "vtkImagePlaneWidget.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"


class vtkResliceCursorCallback : public vtkCommand
{

public:
  static vtkResliceCursorCallback *New(){
      return new vtkResliceCursorCallback;
  }

  void Execute(vtkObject *caller, unsigned long ev, void *callData);

  vtkImagePlaneWidget* IPW[3];
  vtkResliceCursorWidget *RCW[3];

public:
    vtkResliceCursorCallback();

};

#endif // VTKRESLICECURSORCALLBACK_H
