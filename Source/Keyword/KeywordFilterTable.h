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

#ifndef KEYWORDFILTERTABLE_H
#define KEYWORDFILTERTABLE_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QStringList>

class KeywordFilterTable : public QObject
{
public:
    KeywordFilterTable(QObject* parent = 0);
    virtual ~KeywordFilterTable();

    void putData(const QString& sentence, const QString& keyword, int line);
    QHash<QString, QStringList>& data();

    void clear();

private:
    QHash<QString, QStringList> m_keywordFilterTable;
};

#endif // KEYWORDFILTERTABLE_H
