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

#include "MenuBar.h"
#include "RecentFilesMenu.h"
#include "WindowsMenu.h"
#include "Source/Config.h"
#include "Source/Keyword/KeywordButtonGroup.h"
#include "Source/Keyword/SearchLineEdit.h"
#include "Source/Setting/INIManager.h"

#include <QToolBar>
#include <QPushButton>
#include <QLineEdit>

MenuBar::MenuBar(QToolBar* toolBar, QWidget* parent)
    : QMenuBar(parent)
    , m_toolBar(toolBar)
    , m_keywordButtonGroup(0)
    , m_searchLineEdit(0)
    , m_recentFilesMenu(0)
    , m_windowsMenu(0)
    , m_newAction(0)
    , m_openAction(0)
    , m_openClipboardAction(0)
    , m_saveAsAction(0)
    , m_saveHtmlAction(0)
    , m_printAction(0)
    , m_closeTabAction(0)
    , m_quitAction(0)
    , m_findAction(0)
    , m_findNextAction(0)
    , m_findPreviousAction(0)
    , m_gotoLine(0)
    , m_refreshAction(0)
    , m_zoomInAction(0)
    , m_zoomOutAction(0)
    , m_zoomNormalAction(0)
    , m_lineWrapAction(0)
    , m_optionsAction(0)
    , m_updateAction(0)
    , m_reportBugAction(0)
    , m_requestFeatureAction(0)
    , m_aboutAction(0)
{
    m_toolBar->setMovable(false);

    init();
}

MenuBar::~MenuBar()
{
    delete m_newAction;
    delete m_openAction;
    delete m_openClipboardAction;
    delete m_saveAsAction;
    delete m_saveHtmlAction;
    delete m_printAction;
    delete m_closeTabAction;
    delete m_quitAction;
    delete m_findAction;
    delete m_findNextAction;
    delete m_findPreviousAction;
    delete m_gotoLine;
    delete m_refreshAction;
    delete m_zoomInAction;
    delete m_zoomOutAction;
    delete m_zoomNormalAction;
    delete m_lineWrapAction;
    delete m_optionsAction;
    delete m_updateAction;
    delete m_reportBugAction;
    delete m_requestFeatureAction;
    delete m_aboutAction;
    delete m_recentFilesMenu;
    delete m_windowsMenu;

    while (!m_menuList.isEmpty())
        delete m_menuList.takeFirst();

    // don't delete explicitly.
    //delete m_keywordButtonGroup;
    //delete m_searchLineEdit;
}

void MenuBar::init()
{
    /* File */
    m_menuList.append(addMenu(tr("&File")));

    m_newAction = new QAction(QIcon(":/Toolbar/New.png"), tr("&New Tab"), this);
    m_newAction->setShortcuts(QKeySequence::AddTab);
    addToolBarAction(m_newAction);
    connect(m_newAction, SIGNAL(triggered()), this, SIGNAL(newTabTriggered()));
    m_menuList.last()->addAction(m_newAction);

    m_openAction = new QAction(QIcon(":/Toolbar/Folder.png"), tr("&Open..."), this);
    m_openAction->setShortcuts(QKeySequence::Open);
    addToolBarAction(m_openAction);
    connect(m_openAction, SIGNAL(triggered()), this, SIGNAL(openTriggered()));
    m_menuList.last()->addAction(m_openAction);

    m_openClipboardAction = new QAction(QIcon(":/Toolbar/Clipboard.png"), tr("&Open Clipboard"), this);
    m_openClipboardAction->setShortcut(QKeySequence::Paste);
    addToolBarAction(m_openClipboardAction);
    connect(m_openClipboardAction, SIGNAL(triggered()), this, SIGNAL(openClipboardTriggered()));
    m_menuList.last()->addAction(m_openClipboardAction);

    createRecentMenu(m_menuList.last());

    m_saveAsAction = new QAction(QIcon(":/Toolbar/Save.png"), tr("&Save As..."), this);
    m_saveAsAction->setShortcuts(QKeySequence::Save);
    addToolBarAction(m_saveAsAction);
    connect(m_saveAsAction, SIGNAL(triggered()), this, SIGNAL(saveAsTriggered()));
    m_menuList.last()->addAction(m_saveAsAction);

    m_saveHtmlAction = new QAction(tr("&Save To HTML..."), this);
    connect(m_saveHtmlAction, SIGNAL(triggered()), this, SIGNAL(saveHtmlTriggered()));
    m_menuList.last()->addAction(m_saveHtmlAction);

    m_printAction = new QAction(QIcon(":/Toolbar/Print.png"), tr("&Print..."), this);
    m_printAction->setShortcuts(QKeySequence::Print);
    addToolBarAction(m_printAction, true);
    connect(m_printAction, SIGNAL(triggered()), this, SIGNAL(printTriggered()));
    m_menuList.last()->addAction(m_printAction);

    m_menuList.last()->addSeparator();

    m_closeTabAction = new QAction(QIcon(":/Toolbar/Delete.png"), tr("&Close Tab"), this);
    m_closeTabAction->setShortcuts(QKeySequence::Close);
    connect(m_closeTabAction, SIGNAL(triggered()), this, SIGNAL(closeTabTriggered()));
    m_menuList.last()->addAction(m_closeTabAction);

    m_quitAction = new QAction(tr("&Quit"), this);
    m_quitAction->setShortcuts(QKeySequence::Quit);
    connect(m_quitAction, SIGNAL(triggered()), this, SIGNAL(quitTriggered()));
    m_menuList.last()->addAction(m_quitAction);

    /* Edit */
    m_menuList.append(addMenu(tr("&Edit")));

    m_findAction = new QAction(QIcon(":/Toolbar/Find.png"), tr("&Find in Text"), this);
    m_findAction->setShortcuts(QKeySequence::Find);
    m_menuList.last()->addAction(m_findAction);

    m_findNextAction = new QAction(QIcon(":/Toolbar/FindNext.png"), tr("Find &Next"), this);
    m_findNextAction->setShortcuts(QKeySequence::FindNext);
    connect(m_findNextAction, SIGNAL(triggered()), this, SIGNAL(findNextTriggered()));
    m_menuList.last()->addAction(m_findNextAction);

    m_findPreviousAction = new QAction(tr("Find &Previous"), this);
    m_findPreviousAction->setShortcuts(QKeySequence::FindPrevious);
    connect(m_findPreviousAction, SIGNAL(triggered()), this, SIGNAL(findPreviousTriggered()));
    m_menuList.last()->addAction(m_findPreviousAction);

    m_gotoLine = new QAction(tr("&Go To Line"), this);
    m_gotoLine->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(m_gotoLine, SIGNAL(triggered()), this, SIGNAL(gotoLineTriggered()));
    m_menuList.last()->addAction(m_gotoLine);

    /* View */
    m_menuList.append(addMenu(tr("&View")));

    createWindowsMenu(m_menuList.last());
    m_menuList.last()->addSeparator();

    m_refreshAction = new QAction(QIcon(":/Toolbar/Refresh.png"), tr("&Refresh"), this);
    m_refreshAction->setShortcuts(QKeySequence::Refresh);
    addToolBarAction(m_refreshAction);
    connect(m_refreshAction, SIGNAL(triggered()), this, SIGNAL(reloadTriggered()));
    m_menuList.last()->addAction(m_refreshAction);

    m_zoomInAction = new QAction(QIcon(":/Toolbar/ZoomIn.png"), tr("Zoom &In"), this);
    m_zoomInAction->setShortcuts(QKeySequence::ZoomIn);
    addToolBarAction(m_zoomInAction);
    connect(m_zoomInAction, SIGNAL(triggered()), this, SIGNAL(zoomInTriggered()));
    m_menuList.last()->addAction(m_zoomInAction);

    m_zoomOutAction = new QAction(QIcon(":/Toolbar/ZoomOut.png"), tr("Zoom &Out"), this);
    m_zoomOutAction->setShortcuts(QKeySequence::ZoomOut);
    addToolBarAction(m_zoomOutAction, true);
    connect(m_zoomOutAction, SIGNAL(triggered()), this, SIGNAL(zoomOutTriggered()));
    m_menuList.last()->addAction(m_zoomOutAction);

    m_zoomNormalAction = new QAction(tr("Normal &Size"), this);
    connect(m_zoomNormalAction, SIGNAL(triggered()), this, SIGNAL(zoomNormalTriggered()));
    m_menuList.last()->addAction(m_zoomNormalAction);

    m_menuList.last()->addSeparator();

    m_lineWrapAction = new QAction(tr("Line &Wrap"), this);
    m_lineWrapAction->setCheckable(true);
    m_lineWrapAction->setEnabled(false);
    connect(m_lineWrapAction, SIGNAL(triggered()), this, SIGNAL(lineWrapTriggered()));
    m_menuList.last()->addAction(m_lineWrapAction);

    /* Tools */
    m_menuList.append(addMenu(tr("&Tools")));

    m_optionsAction = new QAction(QIcon(":/Toolbar/Tool.png"), tr("&Options"), this);
    m_optionsAction->setShortcut(Qt::CTRL + Qt::Key_Comma);
    addToolBarAction(m_optionsAction, true);    
    connect(m_optionsAction, SIGNAL(triggered()), this, SIGNAL(optionTriggered()));
    m_menuList.last()->addAction(m_optionsAction);

    /* Help */
    m_menuList.append(addMenu(tr("&Help")));

    m_updateAction = new QAction(tr("Check for &Updates"), this);
    connect(m_updateAction, SIGNAL(triggered()), this, SIGNAL(updateTriggered()));
    m_menuList.last()->addAction(m_updateAction);

    m_menuList.last()->addSeparator();

    m_reportBugAction = new QAction(tr("Report a &Bug..."), this);
    connect(m_reportBugAction, SIGNAL(triggered()), this, SIGNAL(reportBugTriggered()));
    m_menuList.last()->addAction(m_reportBugAction);

    m_requestFeatureAction = new QAction(tr("Request a &Feature..."), this);
    connect(m_requestFeatureAction, SIGNAL(triggered()), this, SIGNAL(requestFeatureTriggered()));
    m_menuList.last()->addAction(m_requestFeatureAction);

    m_menuList.last()->addSeparator();

    m_aboutAction = new QAction(QIcon(":/About/Info.png"), tr("&About Monkey Log Viewer..."), this);
    connect(m_aboutAction, SIGNAL(triggered()), this, SIGNAL(aboutTriggered()));
    m_menuList.last()->addAction(m_aboutAction);

    /* Search Line Edit */
    m_searchLineEdit = new SearchLineEdit(INIMANAGER()->isEnabledFindCaseSensitive(),
                                          INIMANAGER()->isEnabledFindWholeWords(),
                                          true);
    m_searchLineEdit->setMaximumWidth(SEARCH_LINEEDIT_WIDTH);
    m_searchLineEdit->setMinimumWidth(SEARCH_LINEEDIT_WIDTH);
    m_searchLineEdit->setInactiveText("<type text>");
    connect(m_findAction, SIGNAL(triggered()), m_searchLineEdit, SLOT(setFocus()));
    connect(m_searchLineEdit, SIGNAL(caseSensitiveToggled(bool)), this, SLOT(caseSensitiveToggled(bool)));
    connect(m_searchLineEdit, SIGNAL(wholeWordsToggled(bool)), this, SLOT(wholeWordsToggled(bool)));
    addToolBarWidget(m_searchLineEdit, false);
    addToolBarAction(m_findNextAction, true);

    /* Keyword Buttons */
    m_keywordButtonGroup = new KeywordButtonGroup;
    addToolBarWidget(m_keywordButtonGroup, false);
}

void MenuBar::addToolBarAction(QAction* action, const bool separator)
{
    if (m_toolBar) {
        m_toolBar->addAction(action);

        if (separator)
            m_toolBar->addSeparator();
    }
}

void MenuBar::addToolBarWidget(QWidget* widget, const bool separator)
{
    if (m_toolBar) {
        m_toolBar->addWidget(widget);

        if (separator)
            m_toolBar->addSeparator();
    }
}

KeywordButtonGroup* MenuBar::keywordButtonGroup()
{
    Q_ASSERT(m_keywordButtonGroup);
    return m_keywordButtonGroup;
}

SearchLineEdit* MenuBar::searchLineEdit()
{
    Q_ASSERT(m_searchLineEdit);
    return m_searchLineEdit;
}

RecentFilesMenu* MenuBar::recentFilesMenu()
{
    Q_ASSERT(m_recentFilesMenu);
    return m_recentFilesMenu;
}

WindowsMenu* MenuBar::windowsMenu()
{
    Q_ASSERT(m_windowsMenu);
    return m_windowsMenu;
}

void MenuBar::createRecentMenu(QMenu* after)
{
    m_recentFilesMenu = new RecentFilesMenu(tr("Recent &Files..."), this);
    connect(m_recentFilesMenu, SIGNAL(openRecentFiles(QAction*)), this, SIGNAL(openRecentFiles(QAction*)));

    after->addMenu(m_recentFilesMenu);
}

void MenuBar::createWindowsMenu(QMenu* after)
{
    m_windowsMenu = new WindowsMenu(tr("&Windows"), this);
    connect(m_windowsMenu, SIGNAL(fileBrowserVisibleChanged(bool)), this, SIGNAL(fileBrowserVisibleChanged(bool)));
    connect(m_windowsMenu, SIGNAL(resultViewVisibleChanged(bool)), this, SIGNAL(resultViewVisibleChanged(bool)));
    connect(m_windowsMenu, SIGNAL(chartViewVisibleChanged(bool)), this, SIGNAL(chartViewVisibleChanged(bool)));

    after->addMenu(m_windowsMenu);
}

void MenuBar::setEnableTabbar(bool enable)
{
    m_saveAsAction->setEnabled(enable);
    m_saveHtmlAction->setEnabled(enable);
    m_printAction->setEnabled(enable);
    m_closeTabAction->setEnabled(enable);
    m_findAction->setEnabled(enable);
    m_findNextAction->setEnabled(enable);
    m_findPreviousAction->setEnabled(enable);
    m_refreshAction->setEnabled(enable);
    m_zoomInAction->setEnabled(enable);
    m_zoomOutAction->setEnabled(enable);
    m_zoomNormalAction->setEnabled(enable);
    m_zoomNormalAction->setEnabled(enable);
    //m_lineWrapAction->setEnabled(enable);
    m_optionsAction->setEnabled(enable);
    m_gotoLine->setEnabled(enable);
}

void MenuBar::caseSensitiveToggled(bool checked)
{
    INIMANAGER()->setFindCaseSensitive(checked);
}

void MenuBar::wholeWordsToggled(bool checked)
{
    INIMANAGER()->setFindWholeWords(checked);
}
