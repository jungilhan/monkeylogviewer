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

#ifndef KEYWORDHIGHLIGHTER_H
#define KEYWORDHIGHLIGHTER_H

#include <QtCore/QObject>
#include <QtGui/QSyntaxHighlighter>

class Chart;
class HighlightingResult;
class QStandardItemModel;

class KeywordHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

signals:
    void modelCreated(QStandardItemModel*);
    void chartLoaded(QPixmap*);

public:
    typedef enum {
        eNormalBlock = 0,
        eHighlightedBlock,
    } eBlockState;

public:
    KeywordHighlighter(QTextDocument* parent);
    virtual ~KeywordHighlighter();

    void startRehighlight();
    void reloadHighlightingRules();
    void rearrangeKeyword(const QStringList& enableKeywordList, const QStringList& disableKeywordList);
    void keywordAdded(const QStringList& keywordList);
    void keywordRemoved(const QStringList& keywordList);

    QStandardItemModel* resultModel();
    void clearResult();

    QPixmap* chart();

protected:
    virtual void highlightBlock(const QString& keyword);

private:
    void init();
    void loadHighlightingRules();
    void disableKeyword(const QStringList& keywordList);
    void enableKeyword(const QStringList& keywordList);
    void parse(const QString& sentence, int line);

private:
    QHash<QString, QTextCharFormat> m_highlightingRules;
    QHash<QString, QTextCharFormat> m_disableHighlightingRules; // for default highlighting rules.

    Chart* m_chart;
    HighlightingResult* m_resultsModel;
};

#endif // KEYWORDHIGHLIGHTER_H
