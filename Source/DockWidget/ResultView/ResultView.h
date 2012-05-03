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

#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include "Source/DockWidget/DockWidget.h"

class QTreeView;
class QStandardItemModel;
class QModelIndex;

class ResultView : public DockWidget
{
    Q_OBJECT

signals:
    void selected(int line);

public:
    ResultView(const QString& title, QWidget* parent = 0);
    virtual ~ResultView();

    void setModel(QStandardItemModel* model);

protected slots:
    virtual void dockLocationChanged(Qt::DockWidgetArea area);

private:
    void init();

private slots:
    void activated(const QModelIndex& index);

private:
    QTreeView* m_treeView;
    QStandardItemModel* m_emptyItemModel;
};

#endif // RESULTVIEW_H
