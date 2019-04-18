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

#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

class TabBar;

class TabWidget : public QTabWidget
{
    Q_OBJECT

signals:
    void newTab();
    void closeAll();
    void closeOthers(int index);

public:
    TabWidget(QWidget* parent = 0);
    virtual ~TabWidget();

    void setTabName(int index, const QString& tabText, const QString& toolText);

public slots:
    void fileLoadStarted(QWidget* widget);
    void fileLoadFinished(QWidget* widget);

private:
    TabBar* m_tabBar;
};

#endif // TABWIDGET_H
