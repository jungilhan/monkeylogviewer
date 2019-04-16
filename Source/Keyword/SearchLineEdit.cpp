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

#include "SearchLineEdit.h"
#include "Source/Setting/INIManager.h"

#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QPainter>

SearchLineEdit::SearchLineEdit(bool enableCaseSensitive, bool enableWholeWords, bool enableRegularExpressions, QWidget* parent)
    : QLineEdit(parent)
    , m_searchButton(new QToolButton(this))
    , m_optionMenu(0)
    , m_caseSensitive(0)
    , m_wholeWords(0)
    , m_regularExpressions(0)
{
    init(enableCaseSensitive, enableWholeWords, enableRegularExpressions);
}

SearchLineEdit::~SearchLineEdit()
{
    delete m_caseSensitive;
    delete m_wholeWords;
    delete m_regularExpressions;
    delete m_optionMenu;
    delete m_searchButton;
}

void SearchLineEdit::init(bool enableCaseSensitive, bool enableWholeWords, bool enableRegularExpressions)
{
    m_optionMenu = createOptionMenu(enableCaseSensitive, enableWholeWords, enableRegularExpressions);

    QPixmap pixmap(":/TextFinder/Magnifying.png");
    m_searchButton->setIcon(QIcon(pixmap));
    m_searchButton->setIconSize(pixmap.size());
    m_searchButton->setCursor(Qt::ArrowCursor);
#if defined(Q_OS_LINUX)
    m_searchButton->setStyleSheet("QToolButton { border: none; padding: 0px; margin-left: 3px; }");
    setTextMargins(pixmap.width() + 6, 0, 0, 0);
#elif defined(Q_OS_WIN32)
    m_searchButton->setStyleSheet("QToolButton { border: none; padding: 0px; margin-left: 1px; }");
    setTextMargins(pixmap.width() + 3, 0, 0, 0);
#endif

    connect(m_searchButton, SIGNAL(clicked()), this, SLOT(showOptionMenuTriggered()));    
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
}

QMenu* SearchLineEdit::createOptionMenu(bool enableCaseSensitive, bool enableWholeWords, bool enableRegularExpressions)
{
    QMenu* optionMenu = new QMenu(this);

    m_caseSensitive = new QAction(tr("Case Sensitive"), this);
    m_caseSensitive->setCheckable(true);
    m_caseSensitive->setChecked(enableCaseSensitive);
    connect(m_caseSensitive, SIGNAL(toggled(bool)), this, SIGNAL(caseSensitiveToggled(bool)));
    optionMenu->addAction(m_caseSensitive);

    m_wholeWords = new QAction(tr("Whole Words Only"), this);
    m_wholeWords->setCheckable(true);
    m_wholeWords->setChecked(enableWholeWords);
    connect(m_wholeWords, SIGNAL(toggled(bool)), this, SIGNAL(wholeWordsToggled(bool)));
    optionMenu->addAction(m_wholeWords);

    m_regularExpressions = new QAction(tr("Use Regular Expressions"), this);
    m_regularExpressions->setCheckable(true);
    m_regularExpressions->setChecked(enableRegularExpressions);
    m_regularExpressions->setDisabled(true);
    connect(m_regularExpressions, SIGNAL(toggled(bool)), this, SIGNAL(regularExpressionsToggled(bool)));
    optionMenu->addAction(m_regularExpressions);

    return optionMenu;
}

void SearchLineEdit::setInactiveText(const QString& text)
{
    m_inactiveText = text;
}

void SearchLineEdit::focusInEvent(QFocusEvent* event)
{
    QLineEdit::focusInEvent(event);

    if (displayText().length() > 0)
        selectAll();
}

void SearchLineEdit::resizeEvent(QResizeEvent* event)
{
    QLineEdit::resizeEvent(event);

    int y = (size().height() - m_searchButton->iconSize().height()) / 2 - 1;
    if (y > 0)
        m_searchButton->move(m_searchButton->x(), y);
}

void SearchLineEdit::paintEvent(QPaintEvent* event)
{
    QLineEdit::paintEvent(event);

    if (text().isEmpty() && !m_inactiveText.isEmpty() && !hasFocus()) {
        QPainter painter(this);
        painter.setPen(QColor(Qt::lightGray));

        int left, top, right, bottom;
        getTextMargins(&left, &top, &right, &bottom);
        painter.drawText(left, top, width(), height(), Qt::AlignLeft | Qt::AlignVCenter, m_inactiveText);
    }
}

void SearchLineEdit::textEdited(QString text)
{
    if (text.contains("\n")) {
        int index = text.indexOf("\n");
        text.remove(index, text.length());
        setText(text);
    }
}

void SearchLineEdit::showOptionMenuTriggered()
{
    Q_ASSERT(m_optionMenu);

    m_optionMenu->exec(QCursor::pos());
}

