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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TabWidget;
class KeywordButtonGroup;
class RecentFilesMenu;
class WindowsMenu;
class Updater;
class FileBrowser;
class ResultView;
class ChartView;
class QStandardItemModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openFile(const QString& fileName,
                  bool openNewTab = false,
                  bool reload = false,
                  bool saveCurrentDir = false);

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private slots:
    void openTriggered();
    void openClipboardTriggered();
    void saveAsTriggered();
    void saveHtmlTriggered();
    void printTriggered();
    void closeTabTriggered();
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
    void openRecentFiles(QAction* action);
    void fileBrowserVisibleChanged(bool visible);
    void resultViewVisibleChanged(bool visible);
    void chartViewVisibleChanged(bool visible);

    void newTab(const QString& text = QString());
    void closeAll();
    void closeOthers(int index);
    void closeTab(int index);
    void tabChanged(int index);

    void fileLoadStarted(QWidget* viewer);
    void fileLoadProgress(QWidget* requester, QString* text, int progress = 100);
    void codecDected(const QString& name);

    void selectionChanged(QWidget* viewer);
    void cursorPositionChanged(QWidget* viewer);
    void fileDropped(const QString& fileName);
    void textEncodingChanged(const QString& encoding);
    void gotoLine(int line);
    void modelCreated(QWidget* viewer, QStandardItemModel* resultModel);
    void chartLoaded(QWidget* viewer, QPixmap* chart);

private:
    void init();
    void saveStatus();
    void loadStatus();
    void createDockWidget();
    void createFileBrowser();
    void detachFileBrowser();
    void createResultView();
    void detachResultView();
    void createChartView();
    void detachChartView();

    int currentTabIndex();
    int tabCount();
    void showLineColumn(QWidget* viewer);
    void showFileSize(QWidget* viewer);
    void showTotalLine(QWidget* viewer);
    void showWindowTitle(QWidget* viewer);
    void showWindowTitle(const QString& text);
    void showTextEncoding(QWidget* viewer);

    QWidget* currentTabWidget();
    QWidget* tabWidgetAt(int index);

    KeywordButtonGroup* keywordButtonGroup();
    RecentFilesMenu* recentFilesMenu();
    WindowsMenu* windowsMenu();

private:
    TabWidget* m_tabWidget;
    Updater* m_updater;
    FileBrowser* m_fileBrowser;
    ResultView* m_resultView;
    ChartView* m_chartView;
};

#endif // MAINWINDOW_H
