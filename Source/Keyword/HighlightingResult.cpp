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

#include "HighlightingResult.h"
#include "Source/Setting/INIManager.h"

#include <QtGui/QStandardItem>
#include <QtCore/QRegExp>

HighlightingResult::HighlightingResult(QObject* parent)
    : QStandardItemModel(parent)
{

}

HighlightingResult::~HighlightingResult()
{

}

void HighlightingResult::putData(const QString& sentence, const QString& keyword, int line, bool caseSensitive, bool wholeWords)
{
    QRegExp regExp = createRegExp(keyword, caseSensitive, wholeWords);

    if (sentence.contains(regExp)) {
        QStringList lineInfo = m_results[keyword];

        if (!lineInfo.contains(QString::number(line))) {
            lineInfo << QString("Line: %1").arg(line);
            m_results.insert(keyword, lineInfo);
        }
    }
}

void HighlightingResult::generate()
{
    clear();

    foreach (const QString& keyword, m_results.keys()) {
        QStandardItem* keywordItem = new QStandardItem(keyword + QString(" (%1)").arg(m_results.value(keyword).length()));
        keywordItem->setEditable(false);
        keywordItem->setBackground(INIMANAGER()->backgroundColor(keyword));
        keywordItem->setForeground(INIMANAGER()->foregroundColor(keyword));

        foreach (const QString line, m_results[keyword]) {
            QStandardItem* lineItem = new QStandardItem(line);
            lineItem->setEditable(false);
            keywordItem->appendRow(lineItem);
        }

        appendRow(keywordItem);
    }

    emit modelCreated(this);
}

void HighlightingResult::removeAll()
{
    m_results.clear();
    clear();
}

QHash<QString, QStringList> HighlightingResult::hashTable() const
{
    return m_results;
}

QRegExp HighlightingResult::createRegExp(const QString& keyword, bool caseSensitivity, bool wholeWords)
{
    QString pattern(keyword);

    if (wholeWords) {
        pattern.insert(0, QString("\\b"));
        pattern += QString("\\b");
    }

    return QRegExp(pattern, caseSensitivity ? Qt::CaseSensitive : Qt::CaseInsensitive);
}
