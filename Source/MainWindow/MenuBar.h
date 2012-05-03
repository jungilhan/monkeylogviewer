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

#ifndef MENUBAR_H
#define MENUBAR_H

#include <QtGui/QMenuBar>

class QToolBar;
class KeywordButtonGroup;
class SearchLineEdit;
class RecentFilesMenu;
class WindowsMenu;

class MenuBar : public QMenuBar
{
    Q_OBJECT

signals:
    void newTabTriggered();
    void openTriggered();
    void openClipboardTriggered();
    void saveAsTriggered();
    void saveHtmlTriggered();
    void printTriggered();
    void closeTabTriggered();
    void quitTriggered();
    void findNextTriggered();
    void findPreviousTriggered();
    void gotoLineTriggered();
    void reloadTriggered();
    void zoomInTriggered();
    void zoomOutTriggered();
    void zoomNormalTriggered();
    void lineWrapTriggered();
    void optionTriggered();
    void updateTriggered();
    void reportBugTriggered();
    void requestFeatureTriggered();
    void aboutTriggered();
    void openRecentFiles(QAction*);
    void fileBrowserVisibleChanged(bool);
    void resultViewVisibleChanged(bool);
    void chartViewVisibleChanged(bool);

public:
    MenuBar(QToolBar* toolBar, QWidget* parent = 0);
    virtual ~MenuBar();

    KeywordButtonGroup* keywordButtonGroup();
    SearchLineEdit* searchLineEdit();
    RecentFilesMenu* recentFilesMenu();
    WindowsMenu* windowsMenu();

    void setEnableTabbar(bool enable);

private:
    void init();
    void createRecentMenu(QMenu* after);
    void createWindowsMenu(QMenu* after);
    void addToolBarAction(QAction* action, const bool separator = false);
    void addToolBarWidget(QWidget* widget, const bool separator = false);

private slots:
    void caseSensitiveToggled(bool checked);
    void wholeWordsToggled(bool checked);

private:
    QList<QMenu*> m_menuList;
    QToolBar* m_toolBar;
    KeywordButtonGroup* m_keywordButtonGroup;
    SearchLineEdit* m_searchLineEdit;
    RecentFilesMenu* m_recentFilesMenu;
    WindowsMenu* m_windowsMenu;

    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_openClipboardAction;
    QAction* m_saveAsAction;
    QAction* m_saveHtmlAction;
    QAction* m_printAction;
    QAction* m_closeTabAction;
    QAction* m_quitAction;
    QAction* m_findAction;
    QAction* m_findNextAction;
    QAction* m_findPreviousAction;
    QAction* m_gotoLine;
    QAction* m_refreshAction;
    QAction* m_zoomInAction;
    QAction* m_zoomOutAction;
    QAction* m_zoomNormalAction;
    QAction* m_lineWrapAction;
    QAction* m_optionsAction;
    QAction* m_updateAction;
    QAction* m_reportBugAction;
    QAction* m_requestFeatureAction;
    QAction* m_aboutAction;
};

#endif // MENUBAR_H
