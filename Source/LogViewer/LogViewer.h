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

#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QtGui/QPlainTextEdit>

class LineNumberArea;
class KeywordHighlighter;
class QButtonGroup;
class QTextCodec;
class QStandardItemModel;

class LogViewer : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void selectionChanged(QWidget*);
    void cursorPositionChanged(QWidget*);
    void fileDropped(QString);
    void modelCreated(QWidget*, QStandardItemModel*);
    void chartLoaded(QWidget*, QPixmap*);

public:
    LogViewer(QWidget* parent = 0, bool showLineNumber = true);
    virtual ~LogViewer();

    int lineNumberAreaWidth();
    void lineNumberAreaPaintEvent(QPaintEvent* event);

    void setTabWidth(int spaceCount);

    QTextCursor moveCursor(QTextCursor::MoveOperation position);

    bool isZoomMode();
    void setZoom(int steps);
    void setNormalZoom();

    QTextCursor moveCursorPosition(QTextCursor::MoveOperation position);
    void gotoLine(int lineNumber);

    QColor forgroundColor();
    void setForegroundColor(const QColor& color);

    QColor backgroundColor();
    void setBackgroundColor(const QColor& color);

    QColor customBackgroundColor();
    void setCustomBackgroundColor(const QColor& color);

    QColor currentLineFgColor();
    QColor currentLineBgColor();
    void setCurrentLineFgColor(const QColor& color);
    void setCurrentLineBgColor(const QColor& color);

    void reloadKeywordHighlightRules();
    void initBlockState();

    long fileSize();
    QString fileName();

    void saveAs(const QString& fileName);
    void saveToHtml(const QString& fileName);

    bool find(const QRegExp& keyword, QTextDocument::FindFlags options = 0);

    void setTextCodec(QTextCodec* textCodec);
    QTextCodec* textCodec();

    bool isEmpty();
    QStandardItemModel* resultModel();
    QPixmap* chart();

public slots:
    void fileLoadStarted();
    void fileLoadProgress(const QString& text);
    void fileLoadFinished();
    void fileSizeNotified(long size);
    void fileNameNotified(const QString& fileName);

    void keywordFilterTableRefresh();
    void keywordButtonGroupClicked(QButtonGroup* buttonGroup);
    void keywordButtonGroupAdded(QStringList& keywordList);
    void keywordButtonGroupRemoved(QStringList& keywordList);

protected:
    virtual void resizeEvent(QResizeEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dragMoveEvent(QDragMoveEvent* event);
    virtual void dropEvent(QDropEvent* event);
    virtual void paintEvent(QPaintEvent* event);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);
    void drawCurrentLine();
    void blockCountChanged(int newBlockCount);
    void selectionChanged();
    void modelCreated(QStandardItemModel* resultModel);
    void chartLoaded(QPixmap* chart);

private:
    void init();
    void drawCustomBackgroud(int startBlock, int endBlock);
    void drawTextBlock(const QTextBlock &textBlock, const QColor &backgroundColor);
    void insertForegroundStyle(QString& text);
    void insertLineBreak(QString& text);
    bool isEmptyBlock(const QString& text);
    QStringList mimeTypeToLocalUrl(const QMimeData* mimeData);

private:
    bool m_showLineNumber;
    bool m_isFirstAppend;
    bool m_isZoomMode;
    long m_fileSize;

    QString m_fileName;
    QColor m_customBackgroundColor;
    QColor m_currentLineFgColor;
    QColor m_currentLineBgColor;

    LineNumberArea* m_lineNumberArea;
    KeywordHighlighter* m_keywordHighlighter;

    QTextCodec* m_textCodec;
};

#endif // LOGVIEWER_H
