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

#ifndef RECENTFILESMENU_H
#define RECENTFILESMENU_H

#include <QMenu>
#include <QActionGroup>

class RecentFilesMenu : public QMenu
{
    Q_OBJECT

    enum {
        eMaxRecentFiles = 10
    };

signals:
    void openRecentFiles(QAction*);

public:
    RecentFilesMenu(const QString& title, QWidget* parent = 0);
    virtual ~RecentFilesMenu();

    void appendRecentFile(const QString& fileName);

private:
    void init();

    void updateRecentFileActions(const QStringList files);
    QString strippedName(const QString& fullFileName);

private:
    QActionGroup* m_recentFilesActionGroup;
};

#endif // RECENTFILESMENU_H
