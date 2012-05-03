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

#ifndef LOGVIEWERMANAGER_H
#define LOGVIEWERMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QStringList>

#define LOGVIEWERMANAGER() LogViewerManager::getInstance()

class QWidget;
class QButtonGroup;
class QTextCodec;
class QStandardItemModel;
class QPixmap;

class LogViewerManager : public QObject
{
    Q_OBJECT

signals:
    void selectionChanged(QWidget*);
    void cursorPositionChanged(QWidget*);
    void fileDropped(QString);
    void modelCreated(QWidget*, QStandardItemModel*);
    void chartLoaded(QWidget*, QPixmap*);

public:
    static LogViewerManager* getInstance();
    virtual ~LogViewerManager();

    QWidget* createViewer(QWidget* parent = 0);

    void deleteAll();
    void deleteOthers(const QWidget* viewer);
    bool deleteEditor(const QWidget* viewer);

    void setText(const QString& text, const QWidget* viewer);
    void appendText(const QString& text, const QWidget* viewer);

    void print(const QWidget* viewer);

    void setZoom(const QWidget* viewer, int steps);
    void setNormalZoom(const QWidget* viewer);

    void findForward(const QWidget* viewer, const QString& keyword);
    void findBackward(const QWidget* viewer, const QString& keyword);
    QRegExp createRegExp(const QString& keyword, bool caseSensitivity = false, bool wholeWords = false);

    long fileSize(const QWidget* viewer);
    QString fileName(const QWidget* viewer);

    void saveAs(const QWidget* viewer, const QString& fileName);
    void saveToHtml(const QWidget* viewer, const QString& fileName);

    QString selectedText(const QWidget* viewer);

    int line(const QWidget* viewer);
    int column(const QWidget* viewer);
    int blockCount(const QWidget* viewer);

    void setTextCodec(const QWidget* viewer, QTextCodec* textCodec);
    QTextCodec* textCodec(const QWidget* viewer);

    bool isEmpty(const QWidget* viewer);
    QStandardItemModel* resultModel(const QWidget* viewer);
    QPixmap* chart(const QWidget* viewer);

public slots:
    void fileLoadStarted(QWidget* viewer);
    void fileLoadProgress(QWidget* viewer, const QString& text);
    void fileLoadFinished(QWidget* viewer);
    void fileSizeNotified(QWidget* viewer, long size);
    void fileNameNotified(QWidget* viewer, const QString& fileName);
    void optionChanged();

    void keywordButtonGroupReloaded();
    void keywordButtonGroupClicked(QWidget* viewer, QButtonGroup* buttonGroup);
    void keywordButtonGroupAdded(QStringList& keywordList);
    void keywordButtonGroupRemoved(QStringList& keywordList);
    void highlightOptionChanged();
    void gotoLine(QWidget* viewer, int line);

private:
    LogViewerManager();
    bool deleteOne(const QWidget* viewer);

private:
    QList<QWidget*> m_editorList;
    static LogViewerManager* s_this;
    bool m_reachedEndDocument;
    bool m_reachedStartDocument;
};

#endif // LOGVIEWERMANAGER_H
