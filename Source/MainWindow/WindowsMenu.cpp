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

#include "WindowsMenu.h"
#include "Source/Setting/INIManager.h"

WindowsMenu::WindowsMenu(const QString &title, QWidget *parent)
    : QMenu(title, parent)
    , m_windowsActionGroup(0)
{
    init();
}

WindowsMenu::~WindowsMenu()
{
    while (!m_windowsActionGroup->actions().isEmpty())
        delete m_windowsActionGroup->actions().takeFirst();

    delete m_windowsActionGroup;
}

void WindowsMenu::init()
{
    m_windowsActionGroup = new QActionGroup(this);
    m_windowsActionGroup->setExclusive(false);

    m_windowsActionGroup->addAction(new QAction(tr("File &Browser"), this));
    m_windowsActionGroup->actions().last()->setCheckable(true);
    m_windowsActionGroup->actions().last()->setChecked(INIMANAGER()->isVisibleFileBrowser());

    m_windowsActionGroup->addAction(new QAction(tr("Highlighting &Result"), this));
    m_windowsActionGroup->actions().last()->setCheckable(true);
    m_windowsActionGroup->actions().last()->setChecked(INIMANAGER()->isVisibleResultView());

    m_windowsActionGroup->addAction(new QAction(tr("&Chart View"), this));
    m_windowsActionGroup->actions().last()->setCheckable(true);
    m_windowsActionGroup->actions().last()->setChecked(INIMANAGER()->isVisibleChartView());

    addActions(m_windowsActionGroup->actions());
    connect(m_windowsActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
}

void WindowsMenu::actionTriggered(QAction* action)
{
    if (action->text() == "File &Browser") {
        emit fileBrowserVisibleChanged(action->isChecked());
        INIMANAGER()->setFileBrowserVisible(action->isChecked());
    } else if (action->text() == "Highlighting &Result") {
        emit resultViewVisibleChanged(action->isChecked());
        INIMANAGER()->setResultViewVisible(action->isChecked());
    } else if (action->text() == "&Chart View") {
        emit chartViewVisibleChanged(action->isChecked());
        INIMANAGER()->setChartViewVisible(action->isChecked());
    }
}

void WindowsMenu::fileBrowserCloseTriggered()
{
    foreach (QAction* action, m_windowsActionGroup->actions()) {
        if (action->text() == "File &Browser") {
            action->setChecked(false);
            INIMANAGER()->setFileBrowserVisible(false);
            break;
        }
    }
}

void WindowsMenu::resultViewCloseTriggered()
{
    foreach (QAction* action, m_windowsActionGroup->actions()) {
        if (action->text() == "Highlighting &Result") {
            action->setChecked(false);
            INIMANAGER()->setResultViewVisible(false);
            break;
        }
    }
}

void WindowsMenu::chartViewCloseTriggered()
{
    foreach (QAction* action, m_windowsActionGroup->actions()) {
        if (action->text() == "&Chart View") {
            action->setChecked(false);
            INIMANAGER()->setChartViewVisible(false);
            break;
        }
    }
}
