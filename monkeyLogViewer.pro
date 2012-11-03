#-------------------------------------------------
#
# Project created by QtCreator 2010-11-29T00:11:54
#
#-------------------------------------------------

QT       += core gui network

TARGET = monkeyLogViewer
TEMPLATE = app


SOURCES += \
    Source/main.cpp \
    Source/MainWindow/MainWindow.cpp \
    Source/MainWindow/StatusBar.cpp \
    Source/MainWindow/MenuBar.cpp \
    Source/MainWindow/TabWidget.cpp \
    Source/MainWindow/TabBar.cpp \
    Source/Setting/INIManager.cpp \
    Source/Utils/LoadingWidget.cpp \
    Source/FileLoader/FileLoadManager.cpp \
    Source/FileLoader/FileReader.cpp \
    Source/LogViewer/LogViewerManager.cpp \
    Source/LogViewer/LogViewer.cpp \
    Source/Option/OptionDialog.cpp \
    Source/Option/FontSetupPage.cpp \
    Source/Option/HighlightSetupPage.cpp \
    Source/Keyword/KeywordHighlighter.cpp \
    Source/Keyword/KeywordButtonGroup.cpp \
    Source/Keyword/SearchLineEdit.cpp \
    Source/MainWindow/AboutDialog.cpp \
    Source/MainWindow/RecentFilesMenu.cpp \
    Source/MainWindow/GotoLineDialog.cpp \
    Source/Http/Http.cpp \
    Source/Utils/Updater.cpp \
    Source/Utils/ProgressDialog.cpp \   
    Source/MainWindow/WindowsMenu.cpp \
    Source/Keyword/HighlightingResult.cpp \
    Source/DockWidget/DockWidget.cpp \
    Source/DockWidget/FileBrowser/FileBrowser.cpp \
    Source/DockWidget/ResultView/ResultView.cpp \
    Source/DockWidget/ChartView/ChartView.cpp \
    Source/Keyword/Chart.cpp \
    Source/Utils/LoadingPopup.cpp

HEADERS  += \
    Source/MainWindow/MainWindow.h \
    Source/MainWindow/StatusBar.h \
    Source/Config.h \
    Source/MainWindow/MenuBar.h \
    Source/MainWindow/TabWidget.h \
    Source/MainWindow/TabBar.h \
    Source/Setting/INIManager.h \
    Source/Utils/LoadingWidget.h \
    Source/FileLoader/FileLoadManager.h \
    Source/FileLoader/FileReader.h \
    Source/LogViewer/LogViewer.h \
    Source/LogViewer/LogViewerManager.h \
    Source/Option/OptionDialog.h \
    Source/Option/FontSetupPage.h \
    Source/Option/HighlightSetupPage.h \
    Source/Keyword/KeywordHighlighter.h \
    Source/Keyword/KeywordButtonGroup.h \
    Source/TypeDef.h \
    Source/Keyword/SearchLineEdit.h \
    Source/LogViewer/LineNumberArea.h \
    Source/MainWindow/AboutDialog.h \
    Source/MainWindow/RecentFilesMenu.h \
    Source/MainWindow/GotoLineDialog.h \
    Source/Http/Http.h \
    Source/Utils/Updater.h \
    Source/Utils/ProgressDialog.h \
    Source/MainWindow/WindowsMenu.h \
    Source/Keyword/HighlightingResult.h \
    Source/DockWidget/DockWidget.h \
    Source/DockWidget/FileBrowser/FileBrowser.h \
    Source/DockWidget/ResultView/ResultView.h \
    Source/DockWidget/ChartView/ChartView.h \
    Source/Keyword/Chart.h \
    Source/Utils/LoadingPopup.h

OTHER_FILES += \
    README.md

RESOURCES += \
    qresource.qrc

RC_FILE = monkeyLogViewer.rc
