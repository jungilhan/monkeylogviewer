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

#include "KeywordFilterTable.h"

#include <QtCore/QDebug>

KeywordFilterTable::KeywordFilterTable(QObject* parent)
    : QObject(parent)
{

}

KeywordFilterTable::~KeywordFilterTable()
{

}

void KeywordFilterTable::putData(const QString& sentence, const QString& keyword, int line)
{
    if (sentence.contains(keyword)) {
        QStringList lineInfo = m_keywordFilterTable[keyword];
        lineInfo << QString::number(line);
        m_keywordFilterTable.insert(keyword, lineInfo);

#ifdef DEBUG
        foreach (const QStringList& lineInfo, m_keywordFilterTable.values())
            qDebug() << m_keywordFilterTable.key(lineInfo) << lineInfo;
#endif
    }
}

QHash<QString, QStringList>& KeywordFilterTable::data()
{
    return m_keywordFilterTable;
}

void KeywordFilterTable::clear()
{
    m_keywordFilterTable.clear();
}
