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

#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include "Source/DockWidget/DockWidget.h"

class QLabel;
class QScrollArea;

class ChartView : public DockWidget
{
    Q_OBJECT

public:
    ChartView(const QString& title, bool lazyInit = true, QWidget* parent = 0);
    virtual ~ChartView();

    void setChart(QPixmap* chart);
    void clear();

protected slots:
    virtual void dockLocationChanged(Qt::DockWidgetArea area);

private:
    void init();

private slots:
    void lazyInit();

private:
    bool m_lazyInit;
    QLabel* m_chartLabel;
    QScrollArea* m_scrollArea;
};

#endif // CHARTVIEW_H
