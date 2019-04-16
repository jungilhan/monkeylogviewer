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

#include "LogViewerManager.h"
#include "LogViewer.h"
#include "Source/Setting/INIManager.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QMessageBox>
#include <QtCore/QRegExp>
#include <QtCore/QTextCodec>

#define VIEWER_OBJ(viewer) qobject_cast<LogViewer*>(const_cast<QWidget*>(viewer))

LogViewerManager* LogViewerManager::s_this = 0;

LogViewerManager::LogViewerManager()
    : m_reachedEndDocument(false)
    , m_reachedStartDocument(false)
{

}

LogViewerManager::~LogViewerManager()
{
    deleteAll();
}

LogViewerManager* LogViewerManager::getInstance()
{
    if (!s_this)
        s_this = new LogViewerManager();

    return s_this;
}

QWidget* LogViewerManager::createViewer(QWidget* parent)
{
    LogViewer* viewer = new LogViewer(parent);
    connect(viewer, SIGNAL(selectionChanged(QWidget*)), this, SIGNAL(selectionChanged(QWidget*)));
    connect(viewer, SIGNAL(cursorPositionChanged(QWidget*)), this, SIGNAL(cursorPositionChanged(QWidget*)));
    connect(viewer, SIGNAL(fileDropped(QString)), this, SIGNAL(fileDropped(QString)));
    connect(viewer, SIGNAL(modelCreated(QWidget*, QStandardItemModel*)), this, SIGNAL(modelCreated(QWidget*, QStandardItemModel*)));
    connect(viewer, SIGNAL(chartLoaded(QWidget*, QPixmap*)), this, SIGNAL(chartLoaded(QWidget*, QPixmap*)));

    m_editorList.append(viewer);

    return m_editorList.last();
}

void LogViewerManager::deleteAll()
{
    while (!m_editorList.isEmpty())
        m_editorList.takeFirst()->deleteLater();
}

void LogViewerManager::deleteOthers(const QWidget* viewer)
{
    foreach(QWidget* value, m_editorList) {
        if (value != viewer)
            deleteOne(value);
    }
}

bool LogViewerManager::deleteEditor(const QWidget* viewer)
{
    return deleteOne(viewer);
}

bool LogViewerManager::deleteOne(const QWidget* viewer)
{
    VIEWER_OBJ(viewer)->deleteLater();

    return m_editorList.removeOne(const_cast<QWidget*>(viewer));
}

void LogViewerManager::setText(const QString& text, const QWidget* viewer)
{    
    VIEWER_OBJ(viewer)->setPlainText(text);
}

void LogViewerManager::appendText(const QString& text, const QWidget* viewer)
{    
    QTextCursor textCursor = VIEWER_OBJ(viewer)->textCursor();
    textCursor.movePosition(QTextCursor::End);
    textCursor.insertText(text);
}

void LogViewerManager::fileLoadStarted(QWidget* viewer)
{
    VIEWER_OBJ(viewer)->fileLoadStarted();
}

void LogViewerManager::fileLoadProgress(QWidget* viewer, const QString& text)
{
    VIEWER_OBJ(viewer)->fileLoadProgress(text);
}

void LogViewerManager::fileLoadFinished(QWidget* viewer)
{
    VIEWER_OBJ(viewer)->fileLoadFinished();
}

void LogViewerManager::fileSizeNotified(QWidget* viewer, long size)
{
    VIEWER_OBJ(viewer)->fileSizeNotified(size);
}

void LogViewerManager::fileNameNotified(QWidget* viewer, const QString& fileName)
{
    VIEWER_OBJ(viewer)->fileNameNotified(fileName);
}

void LogViewerManager::optionChanged()
{
    foreach(QWidget* viewer, m_editorList) {
        QFont font(INIMANAGER()->font());
        if (VIEWER_OBJ(viewer)->isZoomMode()) {
            font.setPointSize(viewer->font().pointSize());
        } else {
            font.setPointSize(INIMANAGER()->fontSize());
        }

        VIEWER_OBJ(viewer)->setFont(font);
        VIEWER_OBJ(viewer)->setForegroundColor(INIMANAGER()->foregroundColor());
        VIEWER_OBJ(viewer)->setBackgroundColor(INIMANAGER()->backgroundColor());
        VIEWER_OBJ(viewer)->setCustomBackgroundColor(INIMANAGER()->customBackgroundColor());
        VIEWER_OBJ(viewer)->setCurrentLineFgColor(INIMANAGER()->currentLineFgColor());
        VIEWER_OBJ(viewer)->setCurrentLineBgColor(INIMANAGER()->currentLineBgColor());
    }
}

void LogViewerManager::keywordButtonGroupReloaded()
{
    foreach(QWidget* viewer, m_editorList) {
       VIEWER_OBJ(viewer)->reloadKeywordHighlightRules();
    }
}

void LogViewerManager::keywordButtonGroupClicked(QWidget* viewer, QButtonGroup* buttonGroup)
{
    if (!viewer)
        return;

    VIEWER_OBJ(viewer)->keywordButtonGroupClicked(buttonGroup);
}

void LogViewerManager::keywordButtonGroupAdded(QStringList& keywordList)
{
    foreach(QWidget* viewer, m_editorList) {
        VIEWER_OBJ(viewer)->keywordButtonGroupAdded(keywordList);
    }
}

void LogViewerManager::keywordButtonGroupRemoved(QStringList& keywordList)
{
    foreach(QWidget* viewer, m_editorList) {
        VIEWER_OBJ(viewer)->keywordButtonGroupRemoved(keywordList);
    }
}

void LogViewerManager::highlightOptionChanged()
{
    foreach(QWidget* viewer, m_editorList) {
        VIEWER_OBJ(viewer)->initBlockState();
    }
}

void LogViewerManager::gotoLine(QWidget* viewer, int line)
{
    if (!viewer)
        return;

    VIEWER_OBJ(viewer)->gotoLine(line);
}

void LogViewerManager::print(const QWidget* viewer)
{
    if (!viewer)
        return;

    QPrinter printer;
    QPrintDialog dialog(&printer);

    if (dialog.exec() != QDialog::Accepted)
        return;

    VIEWER_OBJ(viewer)->print(&printer);
}

void LogViewerManager::setZoom(const QWidget* viewer, int steps)
{
    VIEWER_OBJ(viewer)->setZoom(steps);
}

void LogViewerManager::setNormalZoom(const QWidget* viewer)
{
    VIEWER_OBJ(viewer)->setNormalZoom();
}

void LogViewerManager::findForward(const QWidget* viewer, const QString& keyword)
{
    if (m_reachedStartDocument)
        m_reachedStartDocument = false;

    if (m_reachedEndDocument) {
        VIEWER_OBJ(viewer)->moveCursor(QTextCursor::Start);
        m_reachedEndDocument = false;
    }

    QRegExp regExp = createRegExp(keyword,
                                  INIMANAGER()->isEnabledFindCaseSensitive() ? true : false,
                                  INIMANAGER()->isEnabledFindWholeWords() ? true : false);

    bool find = VIEWER_OBJ(viewer)->find(regExp);
    if (!find) {
        QString wholeDocument = VIEWER_OBJ(viewer)->toPlainText();

        if (wholeDocument.contains(regExp)) {
            m_reachedEndDocument = true;
            //QMessageBox::information(VIEWER_OBJ(viewer), "Information", "Find reached the end of the document.");
            findForward(viewer, keyword);
        } else {
            QMessageBox::information(VIEWER_OBJ(viewer), "Information", QString("The keyword \"%1\" was not found.").arg(keyword));
        }
    }
}

void LogViewerManager::findBackward(const QWidget* viewer, const QString& keyword)
{
    if (m_reachedEndDocument)
        m_reachedEndDocument = false;

    if (m_reachedStartDocument) {
        VIEWER_OBJ(viewer)->moveCursor(QTextCursor::End);
        m_reachedStartDocument = false;
    }

    QRegExp regExp = createRegExp(keyword,
                                  INIMANAGER()->isEnabledFindCaseSensitive() ? true : false,
                                  INIMANAGER()->isEnabledFindWholeWords() ? true : false);

    bool find = VIEWER_OBJ(viewer)->find(regExp, QTextDocument::FindBackward);
    if (!find) {
        QString wholeDocument = VIEWER_OBJ(viewer)->toPlainText();

        QRegExp regExp = createRegExp(keyword,
                                      INIMANAGER()->isEnabledFindCaseSensitive() ? true : false,
                                      INIMANAGER()->isEnabledFindWholeWords() ? true : false);

        if (wholeDocument.contains(regExp)) {
            m_reachedStartDocument = true;
            //QMessageBox::information(VIEWER_OBJ(viewer), "Information", "Find reached the start of the document.");
            findBackward(viewer, keyword);
        } else {
            QMessageBox::information(VIEWER_OBJ(viewer), "Information", QString("The keyword \"%1\" was not found.").arg(keyword));
        }
    }
}

QRegExp LogViewerManager::createRegExp(const QString& keyword, bool caseSensitivity, bool wholeWords)
{
    QString pattern(keyword);

    if (wholeWords) {
        pattern.insert(0, QString("\\b"));
        pattern += QString("\\b");
    }

    return QRegExp(pattern, caseSensitivity ? Qt::CaseSensitive : Qt::CaseInsensitive);
}


long LogViewerManager::fileSize(const QWidget* viewer)
{
    if (!viewer)
        return 0;

    return VIEWER_OBJ(viewer)->fileSize();
}

QString LogViewerManager::fileName(const QWidget* viewer)
{
    if (!viewer)
        return "";

    return VIEWER_OBJ(viewer)->fileName();
}

void LogViewerManager::saveAs(const QWidget* viewer, const QString& fileName)
{
    VIEWER_OBJ(viewer)->saveAs(fileName);
}

void LogViewerManager::saveToHtml(const QWidget* viewer, const QString& fileName)
{
    VIEWER_OBJ(viewer)->saveToHtml(fileName);
}

QString LogViewerManager::selectedText(const QWidget* viewer)
{
    if (!viewer)
        return "";

    QTextCursor textCursor = VIEWER_OBJ(viewer)->textCursor();
    return textCursor.selectedText();
}

int LogViewerManager::line(const QWidget* viewer)
{
    if (!viewer)
        return 0;

    QTextCursor textCursor = VIEWER_OBJ(viewer)->textCursor();
    return textCursor.blockNumber();
}

int LogViewerManager::column(const QWidget* viewer)
{
    if (!viewer)
        return 0;

    QTextCursor textCursor = VIEWER_OBJ(viewer)->textCursor();
    return textCursor.columnNumber() + 1;
}

int LogViewerManager::blockCount(const QWidget* viewer)
{
    if (!viewer)
        return 0;

    return VIEWER_OBJ(viewer)->blockCount();
}

void LogViewerManager::setTextCodec(const QWidget* viewer, QTextCodec* textCodec)
{
    if (!viewer)
        return;

    VIEWER_OBJ(viewer)->setTextCodec(textCodec);
}

QTextCodec* LogViewerManager::textCodec(const QWidget* viewer)
{
    if (!viewer)
        return QTextCodec::codecForLocale();

    return VIEWER_OBJ(viewer)->textCodec();
}

bool LogViewerManager::isEmpty(const QWidget* viewer)
{
    if (!viewer)
        return true;

    return VIEWER_OBJ(viewer)->isEmpty();
}

QStandardItemModel* LogViewerManager::resultModel(const QWidget* viewer)
{
    if (!viewer)
        return 0;

    return VIEWER_OBJ(viewer)->resultModel();
}

QPixmap* LogViewerManager::chart(const QWidget* viewer)
{
    if (!viewer)
        return 0;

    return VIEWER_OBJ(viewer)->chart();
}
