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

#include "StatusBar.h"

#include <QLabel>
#include <QComboBox>
#include <QtCore/QTextCodec>
#include <QtCore/QByteArray>

StatusBar::StatusBar(QWidget* parent)
    : QStatusBar(parent)
    , m_lineColumn(0)
    , m_selectedCountLabel(0)
    , m_totalLine(0)
    , m_fileSizeLable(0)
    , m_encodingComboBox(0)
{
    init();
}

StatusBar::~StatusBar()
{
    delete m_selectedCountLabel;
    delete m_totalLine;
    delete m_fileSizeLable;
    delete m_lineColumn;
    delete m_encodingComboBox;
}

void StatusBar::init()
{
    /* Right side */
    m_encodingComboBox = new QComboBox(this);
    m_encodingComboBox->setMaxVisibleItems(20);
#ifdef Q_OS_WIN32
    m_encodingComboBox->setMinimumHeight(16);
    m_encodingComboBox->setMaximumHeight(16);
#endif

    QList<QByteArray> textCodecs(QTextCodec::availableCodecs());
    foreach (const QString& codec, textCodecs) {
        m_encodingComboBox->addItem(codec);
    }

    m_encodingComboBox->setCurrentIndex(m_encodingComboBox->findText(QTextCodec::codecForLocale()->name()));
    connect(m_encodingComboBox, SIGNAL(activated(QString)), this, SIGNAL(textEncodingChanged(QString)));
    insertPermanentWidget(0, m_encodingComboBox);

    m_selectedCountLabel = new QLabel(this);
    setSelectedCount(0);
    insertPermanentWidget(1, m_selectedCountLabel);

    m_totalLine = new QLabel(this);
    setTotalLine(0);
    insertPermanentWidget(2, m_totalLine);

    m_fileSizeLable = new QLabel(this);
    setFileSize(0);
    insertPermanentWidget(3, m_fileSizeLable);

    /* Left side */
    m_lineColumn = new QLabel(this);
    setLineColumn(0, 0);
    insertWidget(0, m_lineColumn);
}

void StatusBar::setFileSize(long fileSize)
{
    m_fileSizeLable->setText(QString("<b style=\"color:#e75d13;\"> %1</b> Bytes ").arg(fileSize));
}

void StatusBar::setSelectedCount(int count)
{
    m_selectedCountLabel->setText(QString("<b style=\"color:#e75d13;\"> %1</b> character(s) selected. ").arg(count));
}

void StatusBar::setTotalLine(int line)
{
    m_totalLine->setText(QString("<b style=\"color:#e75d13;\"> %1</b> Line ").arg(line));
}

void StatusBar::setLineColumn(int line, int column)
{
    m_lineColumn->setText(QString(" Line: <b style=\"color:#e75d13;\"> %1</b> Column: <b style=\"color:#e75d13;\"> %2</b> ").arg(line).arg(column));
}

void StatusBar::setTextCodec(const QString& name)
{
    m_encodingComboBox->setCurrentIndex(m_encodingComboBox->findText(name));
}
