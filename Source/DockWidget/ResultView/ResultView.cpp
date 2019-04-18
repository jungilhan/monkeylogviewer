/*
    Copyright (C) 2010 jungil Han <jungil.Han@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ResultView.h"
#include "Source/Setting/INIManager.h"

#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>

#include <QtCore/QModelIndex>

ResultView::ResultView(const QString& title, QWidget* parent)
    : DockWidget(title, parent)
    , m_treeView(new QTreeView(this))
    , m_emptyItemModel(new QStandardItemModel(this))
{
    init();
}

ResultView::~ResultView()
{
    delete m_emptyItemModel;
    delete m_treeView;
}

void ResultView::init()
{
    m_treeView->setModel(m_emptyItemModel);
    m_treeView->setHeaderHidden(true);
    m_treeView->setAnimated(true);

    setWidget(m_treeView);

    connect(m_treeView, SIGNAL(activated(QModelIndex)), this, SLOT(activated(QModelIndex)));
}

void ResultView::setModel(QStandardItemModel* model)
{
    if (model) {
        m_treeView->setModel(model);
    } else {
        m_treeView->setModel(m_emptyItemModel);
    }
}

void ResultView::activated(const QModelIndex& index)
{
    QStandardItemModel* standardItemModel = qobject_cast<QStandardItemModel*>(m_treeView->model());
    Q_ASSERT(standardItemModel);

    if (standardItemModel->hasChildren(index))
        return;

    QString line = standardItemModel->itemFromIndex(index)->text();
    line.remove("Line: ");

    emit selected(line.toInt());
}

void ResultView::dockLocationChanged(Qt::DockWidgetArea area)
{
    INIMANAGER()->setResultViewArea(area);
}
