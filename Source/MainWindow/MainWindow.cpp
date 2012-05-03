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

#include "MainWindow.h"
#include "Source/Config.h"
#include "Source/MainWindow/MenuBar.h"
#include "Source/MainWindow/StatusBar.h"
#include "Source/MainWindow/TabWidget.h"
#include "Source/MainWindow/AboutDialog.h"
#include "Source/MainWindow/GotoLineDialog.h"
#include "Source/MainWindow/RecentFilesMenu.h"
#include "Source/MainWindow/WindowsMenu.h"
#include "Source/Option/OptionDialog.h"
#include "Source/Setting/INIManager.h"
#include "Source/LogViewer/LogViewerManager.h"
#include "Source/FileLoader/FileLoadManager.h"
#include "Source/Keyword/KeywordButtonGroup.h"
#include "Source/Keyword/SearchLineEdit.h"
#include "Source/Utils/Updater.h"
#include "Source/DockWidget/FileBrowser/FileBrowser.h"
#include "Source/DockWidget/ResultView/ResultView.h"
#include "Source/DockWidget/ChartView/ChartView.h"

#include <QtGui/QFileDialog>
#include <QtGui/QCloseEvent>
#include <QtGui/QDesktopServices>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QStandardItem>
#include <QtCore/QTextCodec>
#include <QtCore/QUrl>

#if DEBUG
#include <QtCore/QDebug>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_tabWidget(0)
    , m_updater(0)
    , m_fileBrowser(0)
    , m_resultView(0)
    , m_chartView(0)
{
    setObjectName("MainWindow");
    setWindowIcon(QIcon(":/Icon/Logo32x32.png"));
    setMinimumSize(MIMNUM_WIDTH, MIMNUM_HEIGHT);
    setContextMenuPolicy(Qt::NoContextMenu);

    loadStatus();

    init();
}

MainWindow::~MainWindow()
{
    delete m_tabWidget;
    delete m_updater;
    delete m_fileBrowser;
    delete m_resultView;
    delete m_chartView;
}

void MainWindow::init()
{
    /* MenuBar */
    setMenuBar(new MenuBar(addToolBar(tr("")), this));
    connect(menuBar(), SIGNAL(newTabTriggered()), this, SLOT(newTab()));
    connect(menuBar(), SIGNAL(openTriggered()), this, SLOT(openTriggered()));
    connect(menuBar(), SIGNAL(openClipboardTriggered()), this, SLOT(openClipboardTriggered()));
    connect(menuBar(), SIGNAL(saveAsTriggered()), this, SLOT(saveAsTriggered()));
    connect(menuBar(), SIGNAL(saveHtmlTriggered()), this, SLOT(saveHtmlTriggered()));
    connect(menuBar(), SIGNAL(printTriggered()), this, SLOT(printTriggered()));
    connect(menuBar(), SIGNAL(closeTabTriggered()), this, SLOT(closeTabTriggered()));
    connect(menuBar(), SIGNAL(quitTriggered()), this, SLOT(close()));
    connect(menuBar(), SIGNAL(findNextTriggered()), this, SLOT(findNextTriggered()));
    connect(menuBar(), SIGNAL(findPreviousTriggered()), this, SLOT(findPreviousTriggered()));
    connect(menuBar(), SIGNAL(gotoLineTriggered()), this, SLOT(gotoLineTriggered()));
    connect(menuBar(), SIGNAL(reloadTriggered()), this, SLOT(reloadTriggered()));
    connect(menuBar(), SIGNAL(zoomInTriggered()), this, SLOT(zoomInTriggered()));
    connect(menuBar(), SIGNAL(zoomOutTriggered()), this, SLOT(zoomOutTriggered()));
    connect(menuBar(), SIGNAL(zoomNormalTriggered()), this, SLOT(zoomNormalTriggered()));
    connect(menuBar(), SIGNAL(lineWrapTriggered()), this, SLOT(lineWrapTriggered()));
    connect(menuBar(), SIGNAL(optionTriggered()), this, SLOT(optionTriggered()));
    connect(menuBar(), SIGNAL(updateTriggered()), this, SLOT(updateTriggered()));
    connect(menuBar(), SIGNAL(reportBugTriggered()), this, SLOT(reportBugTriggered()));
    connect(menuBar(), SIGNAL(requestFeatureTriggered()), this, SLOT(requestFeatureTriggered()));
    connect(menuBar(), SIGNAL(aboutTriggered()), this, SLOT(aboutTriggered()));
    connect(menuBar(), SIGNAL(openRecentFiles(QAction*)), this, SLOT(openRecentFiles(QAction*)));
    connect(menuBar(), SIGNAL(fileBrowserVisibleChanged(bool)), this, SLOT(fileBrowserVisibleChanged(bool)));
    connect(menuBar(), SIGNAL(resultViewVisibleChanged(bool)), this, SLOT(resultViewVisibleChanged(bool)));
    connect(menuBar(), SIGNAL(chartViewVisibleChanged(bool)), this, SLOT(chartViewVisibleChanged(bool)));
    connect(static_cast<MenuBar*>(menuBar())->searchLineEdit(), SIGNAL(returnPressed()), this, SLOT(findNextTriggered()));

    /* StatusBar */
    setStatusBar(new StatusBar(this));
    connect(static_cast<StatusBar*>(statusBar()), SIGNAL(textEncodingChanged(QString)), this, SLOT(textEncodingChanged(QString)));

    /* TabWidget */
    m_tabWidget = new TabWidget(this);
    connect(m_tabWidget, SIGNAL(newTab()), this, SLOT(newTab()));
    connect(m_tabWidget, SIGNAL(closeAll()), this, SLOT(closeAll()));
    connect(m_tabWidget, SIGNAL(closeOthers(int)), this, SLOT(closeOthers(int)));
    connect(m_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    newTab();

    setCentralWidget(m_tabWidget);

    /* DockWidget */
    createDockWidget();

    /* Keyword Button Group */
    connect(keywordButtonGroup(), SIGNAL(buttonReloaded()), LOGVIEWERMANAGER(), SLOT(keywordButtonGroupReloaded()));
    connect(keywordButtonGroup(), SIGNAL(buttonClicked(QWidget*,QButtonGroup*)), LOGVIEWERMANAGER(), SLOT(keywordButtonGroupClicked(QWidget*,QButtonGroup*)));
    connect(keywordButtonGroup(), SIGNAL(buttonAdded(QStringList&)), LOGVIEWERMANAGER(), SLOT(keywordButtonGroupAdded(QStringList&)));
    connect(keywordButtonGroup(), SIGNAL(buttonRemoved(QStringList&)), LOGVIEWERMANAGER(), SLOT(keywordButtonGroupRemoved(QStringList&)));

    /* File Load Manager */
    connect(FILELOADMANAGER(), SIGNAL(loadStarted(QWidget*)), this, SLOT(fileLoadStarted(QWidget*)));
    connect(FILELOADMANAGER(), SIGNAL(loadProgress(QWidget*,QString*,int)), this, SLOT(fileLoadProgress(QWidget*,QString*,int)));
    connect(FILELOADMANAGER(), SIGNAL(loadFinished(QWidget*,QString*)), this, SLOT(fileLoadProgress(QWidget*,QString*)));
    connect(FILELOADMANAGER(), SIGNAL(codecDected(QString)), this, SLOT(codecDected(QString)));
    connect(FILELOADMANAGER(), SIGNAL(loadStarted(QWidget*)), m_tabWidget, SLOT(fileLoadStarted(QWidget*)));
    connect(FILELOADMANAGER(), SIGNAL(loadFinished(QWidget*,QString*)), m_tabWidget, SLOT(fileLoadFinished(QWidget*)));
    connect(FILELOADMANAGER(), SIGNAL(loadStarted(QWidget*)), LOGVIEWERMANAGER(), SLOT(fileLoadStarted(QWidget*)));
    connect(FILELOADMANAGER(), SIGNAL(loadFinished(QWidget*,QString*)), LOGVIEWERMANAGER(), SLOT(fileLoadFinished(QWidget*)));
    connect(FILELOADMANAGER(), SIGNAL(fileSizeNotify(QWidget*,long)), LOGVIEWERMANAGER(), SLOT(fileSizeNotified(QWidget*,long)));
    connect(FILELOADMANAGER(), SIGNAL(fileNameNotify(QWidget*,QString)), LOGVIEWERMANAGER(), SLOT(fileNameNotified(QWidget*,QString)));

    /* Log Viewer */
    connect(LOGVIEWERMANAGER(), SIGNAL(selectionChanged(QWidget*)), this, SLOT(selectionChanged(QWidget*)));
    connect(LOGVIEWERMANAGER(), SIGNAL(cursorPositionChanged(QWidget*)), this, SLOT(cursorPositionChanged(QWidget*)));
    connect(LOGVIEWERMANAGER(), SIGNAL(fileDropped(QString)), this, SLOT(fileDropped(QString)));
    connect(LOGVIEWERMANAGER(), SIGNAL(modelCreated(QWidget*, QStandardItemModel*)), this, SLOT(modelCreated(QWidget*, QStandardItemModel*)));
    connect(LOGVIEWERMANAGER(), SIGNAL(chartLoaded(QWidget*, QPixmap*)), this, SLOT(chartLoaded(QWidget*, QPixmap*)));
}

void MainWindow::saveStatus()
{
    INIMANAGER()->setValue("WINDOW", "Position", pos());
    INIMANAGER()->setValue("WINDOW", "Maximized", isMaximized());
    if (!isMaximized())
        INIMANAGER()->setValue("WINDOW", "Size", size());

    INIMANAGER()->setValue("VIEWER", "FontSize", font().pointSize());
}

void MainWindow::loadStatus()
{
    move(INIMANAGER()->value("WINDOW", "Position", QPoint(100, 100)).toPoint());
    resize(INIMANAGER()->value("WINDOW", "Size", QSize(800, 600)).toSize());
    if (INIMANAGER()->value("WINDOW", "Maximized", false).toBool())
        showMaximized();
}

void MainWindow::createDockWidget()
{
    if (INIMANAGER()->isVisibleFileBrowser())
        createFileBrowser();

    if (INIMANAGER()->isVisibleResultView())
        createResultView();

    if (INIMANAGER()->isVisibleChartView())
        createChartView();
}

void MainWindow::createFileBrowser()
{
    if (m_fileBrowser) {
        // m_fileBrowser is not removed when clicked close button of dockwidget.
        m_fileBrowser->show();
        addDockWidget(INIMANAGER()->fileBrowserArea(), m_fileBrowser);
        return;
    }

    m_fileBrowser = new FileBrowser(tr("File Browser"), this);
    addDockWidget(INIMANAGER()->fileBrowserArea(), m_fileBrowser);

    connect(m_fileBrowser, SIGNAL(selected(QString)), this, SLOT(openFile(QString)));
    connect(m_fileBrowser, SIGNAL(closeTriggered()), windowsMenu(), SLOT(fileBrowserCloseTriggered()));
}

void MainWindow::detachFileBrowser()
{
    if (!m_fileBrowser)
        return;

    removeDockWidget(m_fileBrowser);

    delete m_fileBrowser;
    m_fileBrowser = 0;
}

void MainWindow::createResultView()
{
    if (m_resultView) {
        m_resultView->show();
        addDockWidget(INIMANAGER()->resultViewArea(), m_resultView);
        return;
    }

    m_resultView = new ResultView(tr("Highlighting Results"), this);
    m_resultView->setModel(LOGVIEWERMANAGER()->resultModel(currentTabWidget()));
    addDockWidget(INIMANAGER()->resultViewArea(), m_resultView);

    connect(m_resultView, SIGNAL(selected(int)), this, SLOT(gotoLine(int)));
    connect(m_resultView, SIGNAL(closeTriggered()), windowsMenu(), SLOT(resultViewCloseTriggered()));
}

void MainWindow::detachResultView()
{
    if (!m_resultView)
        return;

    removeDockWidget(m_resultView);

    delete m_resultView;
    m_resultView = 0;
}

void MainWindow::createChartView()
{
    if (m_chartView) {
        m_chartView->show();
        addDockWidget(INIMANAGER()->chartViewArea(), m_chartView);
        return;
    }

    m_chartView = new ChartView(tr("Chart View"), LOGVIEWERMANAGER()->isEmpty(currentTabWidget()), this);
    m_chartView->setChart(LOGVIEWERMANAGER()->chart(currentTabWidget()));
    addDockWidget(INIMANAGER()->chartViewArea(), m_chartView);

    connect(m_chartView, SIGNAL(closeTriggered()), windowsMenu(), SLOT(chartViewCloseTriggered()));
}

void MainWindow::detachChartView()
{
    if (!m_chartView)
        return;

    removeDockWidget(m_chartView);

    delete m_chartView;
    m_chartView = 0;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    closeAll();
    saveStatus();

    event->accept();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
}

int MainWindow::currentTabIndex()
{
    return m_tabWidget->currentIndex();
}

int MainWindow::tabCount()
{
    return m_tabWidget->count();
}

QWidget* MainWindow::currentTabWidget()
{
    return m_tabWidget->currentWidget();
}

QWidget* MainWindow::tabWidgetAt(int index)
{
    return m_tabWidget->widget(index);
}

KeywordButtonGroup* MainWindow::keywordButtonGroup()
{
    return static_cast<MenuBar*>(menuBar())->keywordButtonGroup();
}

RecentFilesMenu* MainWindow::recentFilesMenu()
{
    return static_cast<MenuBar*>(menuBar())->recentFilesMenu();
}

WindowsMenu* MainWindow::windowsMenu()
{
    return static_cast<MenuBar*>(menuBar())->windowsMenu();
}

void MainWindow::openTriggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString(), INIMANAGER()->recentDirectory());
    if (fileName.isEmpty())
        return;

    INIMANAGER()->setRecentDirectory(QFileInfo(fileName).absolutePath());
    openFile(fileName);
}

void MainWindow::fileDropped(const QString& fileName)
{
    openFile(fileName, true);
}

void MainWindow::openRecentFiles(QAction* action)
{
    Q_ASSERT(action);

    QFile file(action->data().toString());
    if (!file.exists()) {
        QMessageBox::information(this, "Information", QString("The file \"%1\" does not exist.").arg(file.fileName()));
        return;
    }

    openFile(action->data().toString());
}

void MainWindow::fileBrowserVisibleChanged(bool visible)
{
    if (visible) {
        createFileBrowser();
    } else {
        detachFileBrowser();
    }
}

void MainWindow::resultViewVisibleChanged(bool visible)
{
    if (visible) {
        createResultView();
    } else {
        detachResultView();
    }
}

void MainWindow::chartViewVisibleChanged(bool visible)
{
    if (visible) {
        createChartView();
    } else {
        detachChartView();
    }
}

void MainWindow::openFile(const QString& fileName, bool openNewTab, bool reload, bool saveCurrentDir)
{
    if (fileName.isEmpty())
        return;

    if (!reload &&
        (tabCount() == 0 || openNewTab || !LOGVIEWERMANAGER()->isEmpty(currentTabWidget())))
        newTab();

    if (saveCurrentDir || sender() == m_fileBrowser)
        INIMANAGER()->setRecentDirectory(QFileInfo(fileName).absolutePath());

    m_tabWidget->setTabName(currentTabIndex(), QFileInfo(fileName).fileName(), fileName);
    recentFilesMenu()->appendRecentFile(fileName);
    FILELOADMANAGER()->load(currentTabWidget(), fileName, LOGVIEWERMANAGER()->textCodec(currentTabWidget())->name());
}

void MainWindow::openClipboardTriggered()
{
    const QClipboard* clipboard = QApplication::clipboard();
    newTab(clipboard->text());
}

void MainWindow::saveAsTriggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, QString(), INIMANAGER()->recentDirectory(), "Plain Text File (*.txt);;All Files (*.*)");
    if (fileName.isEmpty())
        return;

    INIMANAGER()->setRecentDirectory(QFileInfo(fileName).absolutePath());
    LOGVIEWERMANAGER()->saveAs(currentTabWidget(), fileName);
}

void MainWindow::saveHtmlTriggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, QString(), INIMANAGER()->recentDirectory(), "HTML File (*.html)");
    if (fileName.isEmpty())
        return;

    INIMANAGER()->setRecentDirectory(QFileInfo(fileName).absolutePath());
    LOGVIEWERMANAGER()->saveToHtml(currentTabWidget(), fileName);
}

void MainWindow::printTriggered()
{
    LOGVIEWERMANAGER()->print(currentTabWidget());
}

void MainWindow::closeTabTriggered()
{
    int index = currentTabIndex();

    if (index == -1) {
        close();
    } else {
        closeTab(index);
    }
}

void MainWindow::findNextTriggered()
{
    if (!currentTabWidget())
        return;

    QString keyword = static_cast<MenuBar*>(menuBar())->searchLineEdit()->text();
    if (keyword.length() > 0)
        LOGVIEWERMANAGER()->findForward(currentTabWidget(), keyword);
}

void MainWindow::findPreviousTriggered()
{
    if (!currentTabWidget())
        return;

    QString keyword = static_cast<MenuBar*>(menuBar())->searchLineEdit()->text();
    if (keyword.length() > 0)
        LOGVIEWERMANAGER()->findBackward(currentTabWidget(), keyword);
}

void MainWindow::gotoLineTriggered()
{
    GotoLineDialog gotoLineDialog(this);
    connect(&gotoLineDialog, SIGNAL(gotoLine(int)), this, SLOT(gotoLine(int)));
    gotoLineDialog.exec();
}

void MainWindow::gotoLine(int line)
{
    LOGVIEWERMANAGER()->gotoLine(currentTabWidget(), line);
}

void MainWindow::reloadTriggered()
{
    QString fileName = LOGVIEWERMANAGER()->fileName(currentTabWidget());
    openFile(fileName, false, true);
}

void MainWindow::zoomInTriggered()
{
    LOGVIEWERMANAGER()->setZoom(currentTabWidget(), 1);
}

void MainWindow::zoomOutTriggered()
{
    LOGVIEWERMANAGER()->setZoom(currentTabWidget(), -1);
}

void MainWindow::zoomNormalTriggered()
{
    LOGVIEWERMANAGER()->setNormalZoom(currentTabWidget());
}

void MainWindow::lineWrapTriggered()
{

}

void MainWindow::optionTriggered()
{
    OptionDialog optionDialog(this);
    connect(&optionDialog, SIGNAL(accepted()), LOGVIEWERMANAGER(), SLOT(optionChanged()));
    connect(&optionDialog, SIGNAL(highlightOptionChanged()), LOGVIEWERMANAGER(), SLOT(highlightOptionChanged()));
    connect(&optionDialog, SIGNAL(keywordStateChanged(QHash<QString, bool>&)), keywordButtonGroup(), SLOT(keywordStateChanged(QHash<QString, bool>&)));
    connect(&optionDialog, SIGNAL(keywordColorChanged()), keywordButtonGroup(), SLOT(keywordButtonReload()));

    optionDialog.exec();
}

void MainWindow::updateTriggered()
{
    if (!m_updater)
        m_updater = new Updater(this);

    m_updater->start();
}

void MainWindow::reportBugTriggered()
{
    QDesktopServices::openUrl(QUrl(REPORT_BUG));
}

void MainWindow::requestFeatureTriggered()
{
    QDesktopServices::openUrl(QUrl(REQUEST_FEATURE));
}

void MainWindow::aboutTriggered()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::newTab(const QString& text)
{
    static int count = 0;

    QWidget* viewer = LOGVIEWERMANAGER()->createViewer(this);
    if (!text.isEmpty())
        LOGVIEWERMANAGER()->appendText(text, viewer);

    QString tabTitle = tr("Untitled %1").arg(++count);
    m_tabWidget->addTab(viewer, tabTitle);
    m_tabWidget->setCurrentIndex(m_tabWidget->count() - 1);
    viewer->setFocus();

    showWindowTitle(tabTitle);

    if (tabCount() == 1)
        static_cast<MenuBar*>(menuBar())->setEnableTabbar(true);
}

void MainWindow::closeAll()
{
    FILELOADMANAGER()->suspendAll();
    LOGVIEWERMANAGER()->deleteAll();
    keywordButtonGroup()->removeStateAll();

    static_cast<MenuBar*>(menuBar())->setEnableTabbar(false);
}

void MainWindow::closeOthers(int index)
{
    FILELOADMANAGER()->suspendOthers(tabWidgetAt(index));
    LOGVIEWERMANAGER()->deleteOthers(tabWidgetAt(index));
    keywordButtonGroup()->removeStateOthers(tabWidgetAt(index));
}

void MainWindow::closeTab(int index)
{
    FILELOADMANAGER()->suspend(tabWidgetAt(index));
    LOGVIEWERMANAGER()->deleteEditor(tabWidgetAt(index));
    keywordButtonGroup()->removeState(tabWidgetAt(index));

    if (tabCount() == 1)
        static_cast<MenuBar*>(menuBar())->setEnableTabbar(false);
}

void MainWindow::tabChanged(int index)
{
    keywordButtonGroup()->tabChanged(tabWidgetAt(index));

    /* update StatusBar */
    showFileSize(tabWidgetAt(index));
    showTotalLine(tabWidgetAt(index));
    showWindowTitle(tabWidgetAt(index));
    selectionChanged(tabWidgetAt(index));
    cursorPositionChanged(tabWidgetAt(index));
    showTextEncoding(tabWidgetAt(index));

    /* update DockWidget */
    modelCreated(tabWidgetAt(index), LOGVIEWERMANAGER()->resultModel(tabWidgetAt(index)));
    chartLoaded(tabWidgetAt(index), LOGVIEWERMANAGER()->chart(tabWidgetAt(index)));
}

void MainWindow::showLineColumn(QWidget* viewer)
{
    int line = LOGVIEWERMANAGER()->line(viewer) + 1;
    int column = LOGVIEWERMANAGER()->column(viewer);;
    static_cast<StatusBar*>(statusBar())->setLineColumn(line, column);
}

void MainWindow::showFileSize(QWidget* viewer)
{
    long fileSize = LOGVIEWERMANAGER()->fileSize(viewer);
    static_cast<StatusBar*>(statusBar())->setFileSize(fileSize);
}

void MainWindow::showTotalLine(QWidget* viewer)
{
    long line = LOGVIEWERMANAGER()->blockCount(viewer);
    static_cast<StatusBar*>(statusBar())->setTotalLine(line);
}

void MainWindow::showWindowTitle(QWidget* viewer)
{
    QString fileName = LOGVIEWERMANAGER()->fileName(viewer);
    QString title(PROGRAM_NAME);

    if (fileName.length() > 0)
        title += " - [" + fileName + "]";
    else if (m_tabWidget->tabText(currentTabIndex()).length() > 0)
        title += " - [" + m_tabWidget->tabText(currentTabIndex()) + "]";

    setWindowTitle(title);
}

void MainWindow::showWindowTitle(const QString& text)
{
    QString title(PROGRAM_NAME);

    if (text.length() > 0)
        title += " - [" + text + "]";

    setWindowTitle(title);
}

void MainWindow::showTextEncoding(QWidget* viewer)
{
    QTextCodec* textCodec = LOGVIEWERMANAGER()->textCodec(viewer);
    Q_ASSERT(textCodec);

    QString name = textCodec->name();
    static_cast<StatusBar*>(statusBar())->setTextCodec(name);
}

void MainWindow::fileLoadStarted(QWidget* viewer)
{
    if (currentTabWidget() != viewer)
        return;

    showFileSize(viewer);
    showTotalLine(viewer);
    showWindowTitle(viewer);
    selectionChanged(viewer);
    cursorPositionChanged(viewer);
    showTextEncoding(viewer);
    modelCreated(viewer, 0);
    chartLoaded(viewer, 0);
}

void MainWindow::fileLoadProgress(QWidget* requester, QString* text, int progress)
{
    Q_UNUSED(progress);

    LOGVIEWERMANAGER()->fileLoadProgress(requester, *text);

    if (currentTabWidget() == requester) {
        showTotalLine(requester);
    }
}

void MainWindow::codecDected(const QString& name)
{
    LOGVIEWERMANAGER()->setTextCodec(currentTabWidget(), QTextCodec::codecForName(name.toLocal8Bit()));
    showTextEncoding(currentTabWidget());
}

void MainWindow::selectionChanged(QWidget* viewer)
{
    QString text = LOGVIEWERMANAGER()->selectedText(viewer);
    static_cast<StatusBar*>(statusBar())->setSelectedCount(text.length());
}

void MainWindow::cursorPositionChanged(QWidget* viewer)
{
    showLineColumn(viewer);
}

void MainWindow::textEncodingChanged(const QString& encoding)
{
    QMessageBox::information(this, "Information", QString("The text encoding has been changed.\nPlease refresh the file."));
    LOGVIEWERMANAGER()->setTextCodec(currentTabWidget(), QTextCodec::codecForName(encoding.toLocal8Bit()));
}

void MainWindow::modelCreated(QWidget* viewer, QStandardItemModel* resultModel)
{
    if (!m_resultView)
        return;

    if (viewer != currentTabWidget())
        return;

    m_resultView->setModel(resultModel);
}

void MainWindow::chartLoaded(QWidget* viewer, QPixmap* chart)
{
    if (!m_chartView)
        return;

    if (viewer != currentTabWidget())
        return;

    m_chartView->setChart(chart);
}
