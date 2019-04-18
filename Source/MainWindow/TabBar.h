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

#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>

class QMenu;
class QAction;

class TabBar : public QTabBar
{
    Q_OBJECT

signals:
    void newTab();
    void closeAll();
    void closeOthers(int index);

public:
    TabBar(QWidget* parent = 0);
    virtual ~TabBar();

    void loadingStart(int index);
    void loadingFinish(int index);

protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void tabInserted(int index);
    virtual void tabRemoved(int index);

private:
    void init();

private slots:
    void closeTriggered();
    void closeOthersTriggered();

private:
    QMenu* m_contextMenu;
    int m_triggeredIndex;
};

#endif // TABBAR_H
