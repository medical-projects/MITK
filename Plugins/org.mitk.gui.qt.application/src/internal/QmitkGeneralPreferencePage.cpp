/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "QmitkGeneralPreferencePage.h"

#include "QmitkDataNodeGlobalReinitAction.h"

#include <QCheckBox>
#include <QFormLayout>

#include <berryIPreferencesService.h>
#include <berryPlatform.h>

QmitkGeneralPreferencePage::QmitkGeneralPreferencePage()
  : m_MainControl(nullptr)
{
  // nothing here
}

void QmitkGeneralPreferencePage::Init(berry::IWorkbench::Pointer)
{
  // nothing here
}

void QmitkGeneralPreferencePage::CreateQtControl(QWidget* parent)
{
  berry::IPreferencesService* prefService = berry::Platform::GetPreferencesService();
  m_GeneralPreferencesNode = prefService->GetSystemPreferences()->Node(QmitkDataNodeGlobalReinitAction::ACTION_ID);

  m_MainControl = new QWidget(parent);

  m_GlobalReinitOnNodeDelete = new QCheckBox;
  m_GlobalReinitOnNodeVisibilityChanged = new QCheckBox;

  auto formLayout = new QFormLayout;
  formLayout->addRow("&Call global reinit if node is deleted", m_GlobalReinitOnNodeDelete);
  formLayout->addRow("&Call global reinit if node visibility is changed", m_GlobalReinitOnNodeVisibilityChanged);

  m_MainControl->setLayout(formLayout);
  Update();
}

QWidget* QmitkGeneralPreferencePage::GetQtControl() const
{
  return m_MainControl;
}

bool QmitkGeneralPreferencePage::PerformOk()
{
  m_GeneralPreferencesNode->PutBool("Call global reinit if node is deleted", m_GlobalReinitOnNodeDelete->isChecked());
  m_GeneralPreferencesNode->PutBool("Call global reinit if node visibility is changed", m_GlobalReinitOnNodeVisibilityChanged->isChecked());

  return true;
}

void QmitkGeneralPreferencePage::PerformCancel()
{
  // nothing here
}

void QmitkGeneralPreferencePage::Update()
{
  m_GlobalReinitOnNodeDelete->setChecked(m_GeneralPreferencesNode->GetBool("Call global reinit if node is deleted", true));
  m_GlobalReinitOnNodeVisibilityChanged->setChecked(m_GeneralPreferencesNode->GetBool("Call global reinit if node visibility is changed", false));
}
