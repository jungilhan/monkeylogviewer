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

#ifndef WINDOWSMENU_H
#define WINDOWSMENU_H

#include <QMenu>

class WindowsMenu : public QMenu
{
    Q_OBJECT

signals:
    void fileBrowserVisibleChanged(bool visible);
    void resultViewVisibleChanged(bool visible);
    void chartViewVisibleChanged(bool visible);

public:
    WindowsMenu(const QString& title, QWidget* parent = 0);
    virtual ~WindowsMenu();

public slots:
    void fileBrowserCloseTriggered();
    void resultViewCloseTriggered();
    void chartViewCloseTriggered();

private:
    void init();

private slots:
    void actionTriggered(QAction* action);

private:
    QActionGroup* m_windowsActionGroup;
};

#endif // WINDOWSMENU_H
