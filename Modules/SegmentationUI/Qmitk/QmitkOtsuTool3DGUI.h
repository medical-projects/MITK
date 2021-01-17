/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef QmitkOtsuTool3DGUI_h_Included
#define QmitkOtsuTool3DGUI_h_Included

#include "QmitkAutoMLSegmentationToolGUIBase.h"

#include "ui_QmitkOtsuToolWidgetControls.h"

#include <MitkSegmentationUIExports.h>

/**
  \ingroup org_mitk_gui_qt_interactivesegmentation_internal
  \brief GUI for mitk::.
  \sa mitk::

  This GUI shows ...

  Last contributor: $Author$
*/
class MITKSEGMENTATIONUI_EXPORT QmitkOtsuTool3DGUI : public QmitkAutoMLSegmentationToolGUIBase
{
  Q_OBJECT

public:
  mitkClassMacro(QmitkOtsuTool3DGUI, QmitkAutoMLSegmentationToolGUIBase);
  itkFactorylessNewMacro(Self);
  itkCloneMacro(Self);

protected slots :

  void OnSpinboxValueAccept();

  void OnRegionSpinboxChanged(int);

private slots:

  void OnAdvancedSettingsButtonToggled(bool toggled);

protected:
  QmitkOtsuTool3DGUI();
  ~QmitkOtsuTool3DGUI() = default;

  void ConnectNewTool(mitk::AutoSegmentationWithPreviewTool* newTool) override;
  void InitializeUI(QBoxLayout* mainLayout) override;

  void EnableWidgets(bool enabled) override;

  Ui_QmitkOtsuToolWidgetControls m_Controls;

  bool m_FirstAccept = true;
};

#endif
