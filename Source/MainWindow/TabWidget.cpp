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

#include "TabWidget.h"
#include "TabBar.h"

TabWidget::TabWidget(QWidget* parent)
    : QTabWidget(parent)
    , m_tabBar(new TabBar(this))
{
    setTabBar(m_tabBar);
    setTabsClosable(true);
    setMovable(true);

    connect(m_tabBar, SIGNAL(newTab()), this, SIGNAL(newTab()));
    connect(m_tabBar, SIGNAL(closeOthers(int)), this, SIGNAL(closeOthers(int)));
    connect(m_tabBar, SIGNAL(closeAll()), this, SIGNAL(closeAll()));
}

TabWidget::~TabWidget()
{
    delete m_tabBar;
}

void TabWidget::setTabName(int index, const QString& tabText, const QString& toolText)
{
    Q_ASSERT(index >= 0);

    setTabText(index, tabText);
    setTabToolTip(index, toolText);
}

void TabWidget::fileLoadStarted(QWidget* widget)
{
    m_tabBar->loadingStart(indexOf(widget));
}

void TabWidget::fileLoadFinished(QWidget* widget)
{
    m_tabBar->loadingFinish(indexOf(widget));
}
