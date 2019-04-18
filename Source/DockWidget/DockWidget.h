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

#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>

class DockWidget : public QDockWidget
{
    Q_OBJECT

signals:
    void closeTriggered();

public:
    DockWidget(const QString& title, QWidget* parent = 0);
    virtual ~DockWidget();

protected:
    virtual void closeEvent(QCloseEvent *event);

protected slots:
    virtual void dockLocationChanged(Qt::DockWidgetArea area) = 0;

};

#endif // DOCKWIDGET_H
