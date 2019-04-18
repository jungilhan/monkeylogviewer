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

#include "LogViewer.h"
#include "LineNumberArea.h"
#include "Source/Config.h"
#include "Source/Setting/INIManager.h"
#include "Source/Keyword/KeywordHighlighter.h"

#include <QTextBlock>
#include <QApplication>
#include <QWheelEvent>
#include <QPainter>
#include <QScrollBar>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QtCore/QRegExp>
#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>

#if DEBUG
#include <QtCore/QDebug>
#endif

#if QT_VERSION >= 0x040700 && PERFORMANCE_CHECK
#include <QtCore/QElapsedTimer>
#endif
#include <QMimeData>
#define LINENUMBER_AREA_LEFT_MARGIN 2
#define LINENUMBER_AREA_RIGHT_MARGIN 5

LogViewer::LogViewer(QWidget* parent, bool showLineNumber)
    : QPlainTextEdit(parent)
    , m_showLineNumber(showLineNumber)
    , m_isFirstAppend(true)
    , m_isZoomMode(false)
    , m_fileSize(0)
    , m_customBackgroundColor(INIMANAGER()->defaulCustomBackgroundColor())
    , m_currentLineFgColor(INIMANAGER()->defaultCurrentLineFgColor())
    , m_currentLineBgColor(INIMANAGER()->defaultCurrentLineBgColor())
    , m_lineNumberArea(new LineNumberArea(this))
    , m_keywordHighlighter(new KeywordHighlighter(document()))
    , m_textCodec(QTextCodec::codecForLocale())
{
    init();
}

LogViewer::~LogViewer()
{
    delete m_lineNumberArea;
}

void LogViewer::init()
{
    setAcceptDrops(true);
    setReadOnly(true);
    setTabWidth(4);
    setLineWrapMode(QPlainTextEdit::NoWrap);
    setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);

    QPalette palette;
    palette.setColor(QPalette::Inactive, QPalette::Highlight, palette.color(QPalette::Active, QPalette::Highlight));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, palette.color(QPalette::Active, QPalette::HighlightedText));
    setPalette(palette);

    // Read settings.
    setFont(INIMANAGER()->font());
    setForegroundColor(INIMANAGER()->foregroundColor());
    setBackgroundColor(INIMANAGER()->backgroundColor());
    setCustomBackgroundColor(INIMANAGER()->customBackgroundColor());
    setCurrentLineFgColor(INIMANAGER()->currentLineFgColor());
    setCurrentLineBgColor(INIMANAGER()->currentLineBgColor());

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(drawCurrentLine()));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(blockCountChanged(int)));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));

    // Line Number.
    updateLineNumberAreaWidth(0);
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    // Keyword Highlighter.
    connect(m_keywordHighlighter, SIGNAL(modelCreated(QStandardItemModel*)), this, SLOT(modelCreated(QStandardItemModel*)));
    connect(m_keywordHighlighter, SIGNAL(chartLoaded(QPixmap*)), this, SLOT(chartLoaded(QPixmap*)));
}

void LogViewer::resizeEvent(QResizeEvent* event)
{
    QPlainTextEdit::resizeEvent(event);

    if (m_showLineNumber) {
        QRect contentRect = contentsRect();
        m_lineNumberArea->setGeometry(QRect(contentRect.left(), contentRect.top(), lineNumberAreaWidth(), contentRect.height()));
    }
}

void LogViewer::wheelEvent(QWheelEvent* event)
{
    int degrees = event->delta() / 8;
    int steps = degrees / 15;

    if (event->modifiers() == Qt::ControlModifier) {
        setZoom(steps);
    } else {
        QPlainTextEdit::wheelEvent(event);
    }
}

void LogViewer::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

void LogViewer::dragMoveEvent(QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

void LogViewer::dropEvent(QDropEvent* event)
{
    QStringList localUrls = mimeTypeToLocalUrl((QMimeData*)event->mimeData());

    foreach (const QString &url, localUrls) {
        emit fileDropped(url);
    }
}

QStringList LogViewer::mimeTypeToLocalUrl(QMimeData* mimeData)
{
    QStringList localUrls;

    if (!mimeData->hasUrls())
        return localUrls;

    auto urls = mimeData->urls();
    for(auto& url : urls)
        localUrls << url.toLocalFile();

    return localUrls;
}

int LogViewer::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    const int leftMargin = LINENUMBER_AREA_LEFT_MARGIN;
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * (digits + leftMargin);

    return space;
}

void LogViewer::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    if (!m_showLineNumber)
        return ;

    QPainter painter(m_lineNumberArea);
    QColor bgColor = QColor(Qt::lightGray).lighter(125);
    painter.fillRect(event->rect(), bgColor);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingGeometry(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);

            textCursor().blockNumber() == blockNumber ? painter.setPen(Qt::black) : painter.setPen(Qt::gray);
            painter.drawText(0, top, m_lineNumberArea->width() - LINENUMBER_AREA_RIGHT_MARGIN, fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }

    QPoint p1 = event->rect().topRight();
    QPoint p2 = event->rect().bottomRight();
    painter.setPen(QColor(Qt::darkGreen));
    painter.drawLine(p1, p2);
}

void LogViewer::setTabWidth(int spaceCount)
{
    setTabStopWidth(fontMetrics().width(" ") * spaceCount);
}

bool LogViewer::isZoomMode()
{
    return m_isZoomMode;
}

void LogViewer::setZoom(int steps)
{
    QFont newFont(font());

    int fontSize = newFont.pointSize() + steps;
    if (MIN_FONT_SIZE <= fontSize && MAX_FONT_SIZE >= fontSize) {
        newFont.setPointSize(fontSize);
        setFont(newFont);
    }

    if (INIMANAGER()->fontSize() == fontSize) {
        m_isZoomMode = false;
    } else {
        m_isZoomMode = true;
    }
}

void LogViewer::setNormalZoom()
{
    QFont newFont(font());
    newFont.setPointSize(INIMANAGER()->fontSize());
    setFont(newFont);

    m_isZoomMode = false;
}

void LogViewer::reloadKeywordHighlightRules()
{
    m_keywordHighlighter->reloadHighlightingRules();
    m_keywordHighlighter->startRehighlight();
}

void LogViewer::initBlockState()
{
    QTextBlock block = document()->begin();
    while (block.isValid()) {
        if (block.userState() == KeywordHighlighter::eHighlightedBlock) {
            block.setUserState(KeywordHighlighter::eNormalBlock);

            if (block.blockNumber() % 2) {
                drawTextBlock(block, QColor(m_customBackgroundColor));
            } else {
                drawTextBlock(block, QColor(INIMANAGER()->backgroundColor()));
            }
        }

        block = block.next();
    }
}

void LogViewer::keywordButtonGroupClicked(QButtonGroup* buttonGroup)
{
    Q_ASSERT(buttonGroup);

    QStringList checkedButtonList;
    QStringList uncheckedButtonList;

    QList<QAbstractButton*> buttonList = buttonGroup->buttons();
    foreach (const QAbstractButton* button, buttonList) {
        if (button->isChecked()) {
            checkedButtonList << button->text();
        } else {
            uncheckedButtonList << button->text();
        }
    }

#if DEBUG
    qDebug() << "[Checked] " << checkedButtonList << "[Unchecked] " << uncheckedButtonList;
#endif
    QApplication::setOverrideCursor(Qt::WaitCursor);

    m_keywordHighlighter->rearrangeKeyword(uncheckedButtonList, checkedButtonList);
    m_keywordHighlighter->startRehighlight();

    QApplication::restoreOverrideCursor();
}

void LogViewer::keywordButtonGroupAdded(QStringList& keywordList)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    m_keywordHighlighter->keywordAdded(keywordList);
    m_keywordHighlighter->startRehighlight();

    QApplication::restoreOverrideCursor();
}

void LogViewer::keywordButtonGroupRemoved(QStringList& keywordList)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    m_keywordHighlighter->keywordRemoved(keywordList);
    m_keywordHighlighter->startRehighlight();

    QApplication::restoreOverrideCursor();
}

void LogViewer::modelCreated(QStandardItemModel* resultModel)
{
    emit modelCreated(this, resultModel);
}

void LogViewer::chartLoaded(QPixmap* chart)
{
    emit chartLoaded(this, chart);
}

QTextCursor LogViewer::moveCursorPosition(QTextCursor::MoveOperation position)
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(position);
    setTextCursor(cursor);

    return cursor;
}

void LogViewer::updateLineNumberAreaWidth(int newBlockCount)
{
    Q_UNUSED(newBlockCount);

    if (!m_showLineNumber)
        return ;

    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void LogViewer::updateLineNumberArea(const QRect &rect, int dy)
{
    if (!m_showLineNumber)
        return ;

    if (dy) {
        m_lineNumberArea->scroll(0, dy);
    } else {
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void LogViewer::drawCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    selection.format.setForeground(m_currentLineFgColor);
    selection.format.setBackground(m_currentLineBgColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    setExtraSelections(extraSelections);

    emit cursorPositionChanged(this);
}

void LogViewer::drawCustomBackgroud(int startBlock, int endBlock)
{
#if QT_VERSION >= 0x040700 && PERFORMANCE_CHECK
    QElapsedTimer timer;
    timer.start();
#endif

    for (int index = startBlock; index < endBlock; index++) {
        if (index % 2) {
            QTextBlock textBlock = document()->findBlockByLineNumber(index);
            if (textBlock.userState() != KeywordHighlighter::eHighlightedBlock)
                drawTextBlock(textBlock, QColor(m_customBackgroundColor));
        }
    }

#if QT_VERSION >= 0x040700 && PERFORMANCE_CHECK
    qDebug() << "[Performance Check] DrawCustomBackround: " << timer.elapsed() << "ms";
#endif
}

void LogViewer::drawTextBlock(const QTextBlock &textBlock, const QColor &backgroundColor)
{
    QTextCursor cursor(textBlock);

    QTextBlockFormat blockFormat(cursor.blockFormat());
    blockFormat.setBackground(backgroundColor);

    cursor.mergeBlockFormat(blockFormat);
}

QTextCursor LogViewer::moveCursor(QTextCursor::MoveOperation position)
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(position);
    setTextCursor(cursor);

    return cursor;
}

void LogViewer::gotoLine(int lineNumber)
{
    int line = lineNumber - 1;
    QTextCursor cursor(document()->findBlockByNumber(line));
    if (line < 0 || line > blockCount()) {
        return;
    }

    setCenterOnScroll(true);

    if (!cursor.isNull())
        setTextCursor(cursor);

    setCenterOnScroll(false);
}

void LogViewer::fileLoadStarted()
{
    clear();
    m_keywordHighlighter->clearResult();

    m_isFirstAppend = true;
}

void LogViewer::fileLoadProgress(const QString& text)
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(text);
}

void LogViewer::fileLoadFinished()
{

}

void LogViewer::fileSizeNotified(long size)
{
    m_fileSize = size;
}

long LogViewer::fileSize()
{
    return m_fileSize;
}

void LogViewer::fileNameNotified(const QString& fileName)
{
    m_fileName = fileName;
}

QString LogViewer::fileName()
{
    return m_fileName;
}

void LogViewer::saveAs(const QString& fileName)
{
    QString plainText = document()->toPlainText();
    QFile file(fileName);

    if (file.exists())
        file.remove();

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    QTextStream fileStream(&file);
    fileStream.setAutoDetectUnicode(false);
    fileStream.setCodec(QTextCodec::codecForName("utf-8"));
    fileStream << plainText;

    file.close();
}

void LogViewer::saveToHtml(const QString& fileName)
{
    QString htmlContents = document()->toHtml("utf-8");
    QFile file(fileName);

    if (file.exists())
        file.remove();

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    QTextStream outStream(&htmlContents);
    outStream.seek(0);

    QString htmlContentsForegroundAdded;
    while (!outStream.atEnd()) {
        QString line = outStream.readLine();
        if (isEmptyBlock(line)) {
            insertLineBreak(line);
        } else {
            insertForegroundStyle(line);
        }
        htmlContentsForegroundAdded += line + "\n";
    }

    QTextStream fileStream(&file);
    fileStream.setAutoDetectUnicode(false);
    fileStream.setCodec(QTextCodec::codecForName("utf-8"));
    fileStream << htmlContentsForegroundAdded;

    file.close();
}

void LogViewer::insertForegroundStyle(QString& text)
{
    Qt::CaseSensitivity caseSensitivity = INIMANAGER()->isEnabledHighlightCaseSensitive()
                                          ? Qt::CaseSensitive : Qt::CaseInsensitive;

    if (text.contains("-qt-user-state:1")) {
        QStringList keywordList = INIMANAGER()->highlightingKeywordList();
        QString containKeyword;
        foreach (const QString& keyword, keywordList) {
            if (text.contains(keyword, caseSensitivity))
                containKeyword = keyword;
        }

        if (!containKeyword.isEmpty()) {
            int nIndex = text.indexOf("background-color:") + strlen("background-color:0xffffff");
            text.insert(nIndex, QString(" color:%1;").arg(INIMANAGER()->foregroundColor(containKeyword).name()));
        }
    }
}

void LogViewer::insertLineBreak(QString& text)
{
    int nIndex = text.indexOf("></p>") + strlen(">");
    text.insert(nIndex, QString(" "));
}

bool LogViewer::isEmptyBlock(const QString& text)
{
    return text.contains("-qt-paragraph-type:empty") ? true : false;
}

void LogViewer::keywordFilterTableRefresh()
{

}

void LogViewer::blockCountChanged(int newBlockCount)
{
    static int customBlockStartIndex;

    // Initialize cusor position. (move to the start of the document)
    if (m_isFirstAppend) {
        moveCursor(QTextCursor::Start);
        m_isFirstAppend = false;

        customBlockStartIndex = 0;
    }

    drawCustomBackgroud(customBlockStartIndex, newBlockCount);
    customBlockStartIndex = newBlockCount;
}

void LogViewer::selectionChanged()
{
    emit selectionChanged(this);
}

QColor LogViewer::forgroundColor()
{
    return palette().color(QPalette::Text);
}

void LogViewer::setForegroundColor(const QColor& color)
{
    QPalette palette(this->palette());
    if (palette.color(QPalette::Text) == color)
        return ;

    palette.setColor(QPalette::Text, color);
    setPalette(palette);
}

QColor LogViewer::backgroundColor()
{
    return palette().color(QPalette::Base);
}

void LogViewer::setBackgroundColor(const QColor& color)
{
    QPalette palette(this->palette());
    if (palette.color(QPalette::Base) == color)
        return ;

    palette.setColor(QPalette::Base, color);
    setPalette(palette);
}

QColor LogViewer::customBackgroundColor()
{
    return m_customBackgroundColor;
}

void LogViewer::setCustomBackgroundColor(const QColor& color)
{
    if (m_customBackgroundColor == color)
        return ;

    m_customBackgroundColor = color;
    drawCustomBackgroud(0, blockCount());
}

QColor LogViewer::currentLineFgColor()
{
    return m_currentLineFgColor;
}

QColor LogViewer::currentLineBgColor()
{
    return m_currentLineBgColor;
}

void LogViewer::setCurrentLineFgColor(const QColor& color)
{
    if (m_currentLineFgColor == color)
        return ;

    m_currentLineFgColor = color;
    drawCurrentLine();
}

void LogViewer::setCurrentLineBgColor(const QColor& color)
{
    if (m_currentLineBgColor == color)
        return ;

    m_currentLineBgColor = color;
    drawCurrentLine();
}

bool LogViewer::find(const QRegExp& keyword, QTextDocument::FindFlags options)
{
    QTextCursor cursor = document()->find(keyword, textCursor(), options);
    if (!cursor.isNull()) {
        setTextCursor(cursor);
        selectionChanged();
        return true;
    } else {
        return false;
    }
}

void LogViewer::setTextCodec(QTextCodec* textCodec)
{
    m_textCodec = textCodec;
}

QTextCodec* LogViewer::textCodec()
{
    return m_textCodec;
}

bool LogViewer::isEmpty()
{
    return document()->isEmpty();
}

QStandardItemModel* LogViewer::resultModel()
{
    return m_keywordHighlighter->resultModel();
}

QPixmap* LogViewer::chart()
{
    return m_keywordHighlighter->chart();
}

static void fillBackground(QPainter *p, const QRectF &rect, QBrush brush, QRectF gradientRect = QRectF())
{
    p->save();
    if (brush.style() >= Qt::LinearGradientPattern && brush.style() <= Qt::ConicalGradientPattern) {
        if (!gradientRect.isNull()) {
            QTransform m = QTransform::fromTranslate(gradientRect.left(), gradientRect.top());
            m.scale(gradientRect.width(), gradientRect.height());
            brush.setTransform(m);
            const_cast<QGradient *>(brush.gradient())->setCoordinateMode(QGradient::LogicalMode);
        }
    } else {
        p->setBrushOrigin(rect.topLeft());
    }

    p->fillRect(rect.x(), rect.y(), rect.width(), rect.height(), brush);

    if (INIMANAGER()->backgroundColor() != brush.color()) {
        p->setPen(Qt::white);
        p->drawLine(rect.topLeft(), rect.topRight());
    }
    p->restore();
}

void LogViewer::paintEvent(QPaintEvent* e)
{
    QPainter painter(viewport());
    Q_ASSERT(qobject_cast<QPlainTextDocumentLayout*>(document()->documentLayout()));

    QPointF offset(contentOffset());

    QRect er = e->rect();
    QRect viewportRect = viewport()->rect();

    bool editable = !isReadOnly();

    QTextBlock block = firstVisibleBlock();
    qreal maximumWidth = document()->documentLayout()->documentSize().width();

    // Set a brush origin so that the WaveUnderline knows where the wave started
    painter.setBrushOrigin(offset);

    // keep right margin clean from full-width selection
    int maxX = offset.x() + qMax((qreal)viewportRect.width(), maximumWidth)
               - document()->documentMargin();
    er.setRight(qMin(er.right(), maxX));
    painter.setClipRect(er);


    QAbstractTextDocumentLayout::PaintContext context = getPaintContext();

    while (block.isValid()) {

        QRectF r = blockBoundingRect(block).translated(offset);
        QTextLayout *layout = block.layout();

        if (!block.isVisible()) {
            offset.ry() += r.height();
            block = block.next();
            continue;
        }

        if (r.bottom() >= er.top() && r.top() <= er.bottom()) {

            QTextBlockFormat blockFormat = block.blockFormat();

            QBrush bg = blockFormat.background();
            if (bg != Qt::NoBrush) {
                QRectF contentsRect = r;
                contentsRect.setWidth(qMax(r.width(), maximumWidth));
                fillBackground(&painter, contentsRect, bg);
            }

            QVector<QTextLayout::FormatRange> selections;
            int blpos = block.position();
            int bllen = block.length();
            for (int i = 0; i < context.selections.size(); ++i) {
                const QAbstractTextDocumentLayout::Selection &range = context.selections.at(i);
                const int selStart = range.cursor.selectionStart() - blpos;
                const int selEnd = range.cursor.selectionEnd() - blpos;
                if (selStart < bllen && selEnd > 0
                    && selEnd > selStart) {
                    QTextLayout::FormatRange o;
                    o.start = selStart;
                    o.length = selEnd - selStart;
                    o.format = range.format;
                    selections.append(o);
                } else if (!range.cursor.hasSelection() && range.format.hasProperty(QTextFormat::FullWidthSelection)
                           && block.contains(range.cursor.position())) {
                    // for full width selections we don't require an actual selection, just
                    // a position to specify the line. that's more convenience in usage.
                    QTextLayout::FormatRange o;
                    QTextLine l = layout->lineForTextPosition(range.cursor.position() - blpos);
                    o.start = l.textStart();
                    o.length = l.textLength();
                    if (o.start + o.length == bllen - 1)
                        ++o.length; // include newline
                    o.format = range.format;
                    selections.append(o);
                }
            }

            bool drawCursor = (editable
                               && context.cursorPosition >= blpos
                               && context.cursorPosition < blpos + bllen);

            bool drawCursorAsBlock = drawCursor && overwriteMode() ;

            if (drawCursorAsBlock) {
                if (context.cursorPosition == blpos + bllen - 1) {
                    drawCursorAsBlock = false;
                } else {
                    QTextLayout::FormatRange o;
                    o.start = context.cursorPosition - blpos;
                    o.length = 1;
                    o.format.setForeground(palette().base());
                    o.format.setBackground(palette().text());
                    selections.append(o);
                }
            }


            layout->draw(&painter, offset, selections, er);
            if ((drawCursor && !drawCursorAsBlock)
                || (editable && context.cursorPosition < -1
                    && !layout->preeditAreaText().isEmpty())) {
                int cpos = context.cursorPosition;
                if (cpos < -1)
                    cpos = layout->preeditAreaPosition() - (cpos + 2);
                else
                    cpos -= blpos;
                layout->drawCursor(&painter, offset, cpos, cursorWidth());
            }
        }

        offset.ry() += r.height();
        if (offset.y() > viewportRect.height())
            break;
        block = block.next();
    }

    if (backgroundVisible() && !block.isValid() && offset.y() <= er.bottom()
        && (centerOnScroll() || verticalScrollBar()->maximum() == verticalScrollBar()->minimum())) {
        painter.fillRect(QRect(QPoint((int)er.left(), (int)offset.y()), er.bottomRight()), palette().background());
    }
}
