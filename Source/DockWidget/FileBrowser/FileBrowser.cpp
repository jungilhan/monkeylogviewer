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

#include "FileBrowser.h"
#include "Source/Setting/INIManager.h"

#include <QtGui/QTreeView>
#include <QtGui/QDirModel>
#include <QtCore/QModelIndex>

FileBrowser::FileBrowser(const QString& title, QWidget *parent)
    : DockWidget(title, parent)
    , m_treeView(new QTreeView(this))
    , m_dirModel(new QDirModel(this))
{
    init();
}

FileBrowser::~FileBrowser()
{
    delete m_dirModel;
    delete m_treeView;
}

void FileBrowser::init()
{
    m_dirModel->setReadOnly(true);
    m_dirModel->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);

    QModelIndex index = m_dirModel->index(INIMANAGER()->recentDirectory());

    m_treeView->setModel(m_dirModel);
    m_treeView->expand(index);
    m_treeView->scrollTo(index);
    m_treeView->setCurrentIndex(index);
    m_treeView->resizeColumnToContents(0);
    m_treeView->setAnimated(true);

    // hide other columns(size, type, date)
    for (int column = 1; column < m_dirModel->columnCount(); column++)
        m_treeView->hideColumn(column);

    setWidget(m_treeView);

    connect(m_treeView, SIGNAL(activated(QModelIndex)), this, SLOT(activated(QModelIndex)));
}

void FileBrowser::activated(const QModelIndex& index)
{
    if (!m_dirModel->fileInfo(index).isFile())
        return;

    emit selected(m_dirModel->fileInfo(index).absoluteFilePath());
}

void FileBrowser::dockLocationChanged(Qt::DockWidgetArea area)
{
    INIMANAGER()->setFileBrowserArea(area);
}
