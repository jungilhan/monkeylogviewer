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

#include "ChartView.h"
#include "Source/Setting/INIManager.h"

#include <QLabel>
#include <QtCore/QTimer>
#include <QScrollArea>

ChartView::ChartView(const QString &title, bool lazyInit, QWidget *parent)
    : DockWidget(title, parent)
    , m_lazyInit(lazyInit)
    , m_chartLabel(new QLabel(this))
    , m_scrollArea(new QScrollArea(this))
{
    init();
}

ChartView::~ChartView()
{
    delete m_chartLabel;
}

void ChartView::init()
{
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(m_chartLabel);
    m_scrollArea->setMinimumHeight(100);
    setWidget(m_scrollArea);

    m_chartLabel->setAlignment(Qt::AlignCenter);
    m_chartLabel->setAutoFillBackground(true);
    m_chartLabel->setPalette(QPalette(QPalette::Window, 0xffffff));

    // resize to a DockWidget of 250px by 200px.
    if (m_lazyInit)
        QTimer::singleShot(10, this, SLOT(lazyInit()));
}

void ChartView::lazyInit()
{
    clear();
}

void ChartView::setChart(QPixmap* chart)
{
    if (chart) {
        m_chartLabel->setPixmap(*chart);
    } else {
        clear();
    }
}

void ChartView::clear()
{
    QPixmap dummyPixmap(250, 200);
    dummyPixmap.fill(Qt::white);
    m_chartLabel->setPixmap(dummyPixmap);
}

void ChartView::dockLocationChanged(Qt::DockWidgetArea area)
{
    INIMANAGER()->setChartViewArea(area);
}
