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

#include "TabBar.h"
#include "Source/Utils/LoadingWidget.h"

#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QMouseEvent>

TabBar::TabBar(QWidget* parent)
    : QTabBar(parent)
    , m_triggeredIndex(0)
{
    init();
}

TabBar::~TabBar()
{
    delete m_contextMenu;
}

void TabBar::init()
{
    /* Create context menu */
    m_contextMenu = new QMenu(this);

    QAction *action;
    action = new QAction(tr("New &Tab"), this);
    connect(action, SIGNAL(triggered()), this, SIGNAL(newTab()));
    m_contextMenu->addAction(action);
    m_contextMenu->addSeparator();

    action = new QAction(tr("&Close"), this);
    connect(action, SIGNAL(triggered()), this, SLOT(closeTriggered()));
    m_contextMenu->addAction(action);

    action = new QAction(tr("Close &Others"), this);
    connect(action, SIGNAL(triggered()), this, SLOT(closeOthersTriggered()));
    m_contextMenu->addAction(action);

    action = new QAction(tr("Close &All"), this);
    connect(action, SIGNAL(triggered()), this, SIGNAL(closeAll()));
    m_contextMenu->addAction(action);
}

void TabBar::mousePressEvent(QMouseEvent* event)
{
    QTabBar::mousePressEvent(event);

    if (event->button() == Qt::RightButton) {
        m_triggeredIndex = tabAt(event->pos());
        m_contextMenu->exec(QCursor::pos());
    }
}

void TabBar::tabInserted(int index)
{
    QTabBar::tabInserted(index);

    // LoadingWidget will be deleted by TabBar when it is destroyed.
    LoadingWidget* loadingWidget = new LoadingWidget(this);
    loadingWidget->resize(16, 16);
    loadingWidget->setDefaultImage(QPixmap(":/Tabbar/Document.png"));
    setTabButton(index, QTabBar::LeftSide, loadingWidget);
}

void TabBar::tabRemoved(int index)
{
    QTabBar::tabRemoved(index);
}

void TabBar::closeTriggered()
{
    emit tabCloseRequested(m_triggeredIndex);
}

void TabBar::closeOthersTriggered()
{
    emit closeOthers(m_triggeredIndex);
}

void TabBar::loadingStart(int index)
{
    LoadingWidget* loadingWidget = static_cast<LoadingWidget*>(tabButton(index, QTabBar::LeftSide));
    Q_ASSERT(loadingWidget);

    loadingWidget->start(":Utils/Loading.gif");
}

void TabBar::loadingFinish(int index)
{
    LoadingWidget* loadingWidget = static_cast<LoadingWidget*>(tabButton(index, QTabBar::LeftSide));
    Q_ASSERT(loadingWidget);

    loadingWidget->stop();
}

