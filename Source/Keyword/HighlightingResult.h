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

#ifndef HIGHLIGHTINGRESULT_H
#define HIGHLIGHTINGRESULT_H

#include <QtGui/QStandardItemModel>

class HighlightingResult : public QStandardItemModel
{
    Q_OBJECT

signals:
    void modelCreated(QStandardItemModel*);

public:
    HighlightingResult(QObject* parent = 0);
    virtual ~HighlightingResult();

    void putData(const QString& sentence, const QString& keyword, int line, bool caseSensitive, bool wholeWords);
    void generate();
    void removeAll();

    QHash<QString, QStringList> hashTable() const;

private:
    QRegExp createRegExp(const QString& keyword, bool caseSensitivity, bool wholeWords);

private:
    QHash<QString, QStringList> m_results;  // QString means keyword, QStringList means lines.
};

#endif // HIGHLIGHTINGRESULT_H
