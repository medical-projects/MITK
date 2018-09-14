/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "mitkExceptionMacro.h"

#include "QmitkFitPlotDataModel.h"


QmitkFitPlotDataModel::
QmitkFitPlotDataModel(QObject* parent) :
  QAbstractTableModel(parent)
{
}

const mitk::ModelFitPlotData*
QmitkFitPlotDataModel::
GetPlotData() const
{
  return &m_PlotData;
};

void
QmitkFitPlotDataModel::
SetPlotData(const mitk::ModelFitPlotData* data)
{
  emit beginResetModel();
  m_PlotData = *data;
  emit endResetModel();
};

const std::string&
QmitkFitPlotDataModel::
GetXName() const
{
  return m_XName;
};

void
QmitkFitPlotDataModel::
SetXName(const std::string& xName)
{
  emit beginResetModel();
  m_XName = xName;
  emit endResetModel();
};

int
QmitkFitPlotDataModel::
rowCount(const QModelIndex& parent) const
{
  //we assume that all plots have the same nr of values (except the interpolated signal which will be ignored).
  //Thus we have only to check one plot. Reason: One fit is always derived from one input data and therefore
  //all derived data should have the same size.
  auto plot = GetCurveByColumn(0);
  if (plot.second)
  {
    return plot.second->GetValues().size();
  }
  return 0;
}

std::pair<bool, mitk::Point3D> QmitkFitPlotDataModel::GetPositionalCurvePoint(const mitk::PlotDataCurve* curve) const
{
  mitk::Point3D result;

  for (auto collection : this->m_PlotData.positionalPlots)
  {
    result = collection.first;

    for (auto aCurve : collection.second->CastToSTLContainer())
    {
      if (curve == aCurve.second.GetPointer())
      {
        return std::make_pair(true, result);
      }
    }
  }
  return std::make_pair(false, result);
}


int
QmitkFitPlotDataModel::
columnCount(const QModelIndex& parent) const
{
  if (parent.isValid())
  {
    return 0;
  }
  else
  {
    auto size = this->m_PlotData.currentPositionPlots->size() + this->m_PlotData.staticPlots->size();
    if (mitk::ModelFitPlotData::GetInterpolatedSignalPlot(this->m_PlotData.currentPositionPlots))
    { //don't take the interpolated signal into account
      size -= 1;
    }
    if (mitk::ModelFitPlotData::GetInterpolatedSignalPlot(this->m_PlotData.staticPlots))
    { //don't take the interpolated signal into account
      size -= 1;
    }

    for (const auto& coll : this->m_PlotData.positionalPlots)
    {
      size += coll.second->size();
      if (mitk::ModelFitPlotData::GetInterpolatedSignalPlot(coll.second))
      { //don't take the interpolated signal into account
        size -= 1;
      }
    }
    return size+2;
  }
}

bool GetCurveFromCollection(const mitk::PlotDataCurveCollection* collection, std::size_t index, std::pair<std::string, const mitk::PlotDataCurve*>& finding, std::size_t& actualCount)
{
  actualCount = 0;
  for (const auto& iter : *(collection))
  {
    if (iter.first != mitk::MODEL_FIT_PLOT_INTERPOLATED_SIGNAL_NAME())
    {
      if (actualCount == index)
      {
        finding = std::make_pair(iter.first, iter.second.GetPointer());
        return true;
      }
      ++actualCount;
    }
  }
  return false;
}

std::pair<std::string, const mitk::PlotDataCurve*>
QmitkFitPlotDataModel::
GetCurveByColumn(int col) const
{
  if (col > 1)
  {
    col -= 2;
  }

  std::pair<std::string, const mitk::PlotDataCurve*> finding;
  std::size_t actualCount;
  if (GetCurveFromCollection(this->m_PlotData.currentPositionPlots.GetPointer(), col, finding, actualCount))
  {
    return finding;
  }
  col -= actualCount;

  for (auto collection : this->m_PlotData.positionalPlots)
  {
    if (GetCurveFromCollection(collection.second.GetPointer(), col, finding, actualCount))
    {
      return finding;
    }
    col -= actualCount;
  }

  GetCurveFromCollection(this->m_PlotData.staticPlots.GetPointer(), col, finding, actualCount);
  return finding;
};


QVariant
QmitkFitPlotDataModel::
data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  QVariant result;

  if (index.column() == 0)
  {
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
      result = QVariant(QString::number(index.row()));
    }
    else if (role == Qt::ToolTipRole)
    {
      result = QVariant("Index in plot.");
    }
  }
  else if (index.column() == 1)
  {
    auto finding = GetCurveByColumn(index.column());
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
      if (finding.second && index.row() < finding.second->GetValues().size())
      {
        result = QVariant(QString::number(finding.second->GetValues()[index.row()].first));
      }
    }
    else if (role == Qt::ToolTipRole)
    {
      result = QVariant("x value in plot.");
    }
  }
  else
  {
    auto finding = GetCurveByColumn(index.column());
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
      if (finding.second && index.row() < finding.second->GetValues().size())
      {
        result = QVariant(QString::number(finding.second->GetValues()[index.row()].second));
      }
    }
    else if (role == Qt::ToolTipRole)
    {
      result = QVariant("This is a value of the respective curve.");
    }
  }

  return result;
}

Qt::ItemFlags
QmitkFitPlotDataModel::
flags(const QModelIndex& index) const
{
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);

  return flags;
}

QVariant
QmitkFitPlotDataModel::
headerData(int section, Qt::Orientation orientation, int role) const
{
  if ((Qt::DisplayRole == role) &&
      (Qt::Horizontal == orientation))
  {
    if (section == 0)
    {
      return QVariant("#");
    }
    else if (section == 1)
    {
      return QVariant(QString::fromStdString(m_XName));
    }
    else
    {
      auto finding = GetCurveByColumn(section);

      auto pointFinding = GetPositionalCurvePoint(finding.second);

      std::ostringstream nameStrm;
      nameStrm.imbue(std::locale("C"));
      nameStrm << finding.first;

      if (pointFinding.first)
      {
        nameStrm << " @ " << std::setprecision(3) << "(" << pointFinding.second[0] << "|" << pointFinding.second[1] << "|" << pointFinding.second[2] << ")";
      }

      return QVariant(QString::fromStdString(nameStrm.str()));

    }
  }

  return QVariant();
}

bool
QmitkFitPlotDataModel::
setData(const QModelIndex& index, const QVariant& value, int role)
{
  return false;
};
