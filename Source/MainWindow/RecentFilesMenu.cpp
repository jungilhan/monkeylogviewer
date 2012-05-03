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

#include "RecentFilesMenu.h"
#include "Source/Setting/INIManager.h"

#include <QtCore/QFileInfo>

RecentFilesMenu::RecentFilesMenu(const QString& title, QWidget *parent)
    : QMenu(title, parent)
    , m_recentFilesActionGroup(0)
{
    init();
}

RecentFilesMenu::~RecentFilesMenu()
{
    while (!m_recentFilesActionGroup->actions().isEmpty())
        delete m_recentFilesActionGroup->actions().takeFirst();

    delete m_recentFilesActionGroup;
}

void RecentFilesMenu::init()
{
    m_recentFilesActionGroup = new QActionGroup(this);

    for (int count = 0; count < eMaxRecentFiles; ++count) {
        m_recentFilesActionGroup->addAction(new QAction(this));
        m_recentFilesActionGroup->setVisible(false);
    }

    addActions(m_recentFilesActionGroup->actions());
    connect(m_recentFilesActionGroup, SIGNAL(triggered(QAction*)), SIGNAL(openRecentFiles(QAction*)));

    updateRecentFileActions(INIMANAGER()->recentFiles());
}

void RecentFilesMenu::appendRecentFile(const QString& fileName)
{
    QStringList recentFiles = INIMANAGER()->recentFiles();
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);

    while (recentFiles.size() > eMaxRecentFiles)
        recentFiles.removeLast();

    INIMANAGER()->setRecentFiles(recentFiles);
    updateRecentFileActions(recentFiles);
}

void RecentFilesMenu::updateRecentFileActions(const QStringList files)
{
    int numRecentFiles = qMin(files.size(), (int)eMaxRecentFiles);

    for (int index = 0; index < numRecentFiles; ++index) {
        QString file = QString("&%1 %2").arg(index + 1).arg(strippedName(files.at(index)));
        m_recentFilesActionGroup->actions().at(index)->setText(file);
        m_recentFilesActionGroup->actions().at(index)->setData(files.at(index));
        m_recentFilesActionGroup->actions().at(index)->setVisible(true);
    }

    for (int index = numRecentFiles; index < eMaxRecentFiles; ++index)
        m_recentFilesActionGroup->actions().at(index)->setVisible(false);

    setEnabled(numRecentFiles > 0);
}

QString RecentFilesMenu::strippedName(const QString& fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
