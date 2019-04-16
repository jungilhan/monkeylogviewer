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

#include "KeywordHighlighter.h"
#include "Chart.h"
#include "HighlightingResult.h"
#include "Source/Setting/INIManager.h"

#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>

#if DEBUG
#include <QtCore/QDebug>
#endif

KeywordHighlighter::KeywordHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
    , m_chart(new Chart(this))
    , m_resultsModel(new HighlightingResult(this))
{
    init();
}

KeywordHighlighter::~KeywordHighlighter()
{
    delete m_chart;
    delete m_resultsModel;
}

void KeywordHighlighter::init()
{
    connect(m_chart, SIGNAL(chartLoaded(QPixmap*)), this, SIGNAL(chartLoaded(QPixmap*)));
    connect(m_resultsModel, SIGNAL(modelCreated(QStandardItemModel*)), this, SIGNAL(modelCreated(QStandardItemModel*)));

    loadHighlightingRules();
}

void KeywordHighlighter::loadHighlightingRules()
{
    QStringList keywordList(INIMANAGER()->highlightingKeywordList());
    m_highlightingRules.clear();

    foreach (const QString& keyword, keywordList) {
        if (m_disableHighlightingRules.contains(keyword))
            continue;

        QTextCharFormat keywordFormat;

        keywordFormat.setForeground(INIMANAGER()->foregroundColor(keyword));
        m_highlightingRules.insert(keyword, keywordFormat);
    }
}

void KeywordHighlighter::startRehighlight()
{
    clearResult();
    rehighlight();
}

void KeywordHighlighter::reloadHighlightingRules()
{
    loadHighlightingRules();
}

void KeywordHighlighter::rearrangeKeyword(const QStringList& enableKeywordList, const QStringList& disableKeywordList)
{
    disableKeyword(disableKeywordList);
    enableKeyword(enableKeywordList);
}

void KeywordHighlighter::keywordAdded(const QStringList& keywordList)
{
    // remove keyword in m_disableHighlightingRules.
    foreach (const QString& keyword, keywordList) {
        if (m_disableHighlightingRules.contains(keyword))
            m_disableHighlightingRules.remove(keyword);
    }

    foreach (const QString& keyword, keywordList) {
        QTextCharFormat keywordFormat;

        keywordFormat.setForeground(INIMANAGER()->foregroundColor(keyword));
        m_highlightingRules.insert(keyword, keywordFormat);
    }
}

void KeywordHighlighter::keywordRemoved(const QStringList& keywordList)
{
    disableKeyword(keywordList);
}

void KeywordHighlighter::disableKeyword(const QStringList& keywordList)
{
    //m_disableHighlightingRules.clear();

    /* Store the disable keyword list */
    foreach (const QString& keyword, keywordList) {
        QTextCharFormat keywordFormat;

        keywordFormat.setForeground(INIMANAGER()->defaulForegroundColor());
        m_disableHighlightingRules.insert(keyword, keywordFormat);
    }

    // remove keyword in m_highlightingRules.
    foreach (const QString& keyword, keywordList) {
        if (m_highlightingRules.contains(keyword))
            m_highlightingRules.remove(keyword);
    }
}

void KeywordHighlighter::enableKeyword(const QStringList& keywordList)
{
    m_highlightingRules.clear();

    /* Store the highlight keyword list */
    foreach (const QString& keyword, keywordList) {
        QTextCharFormat keywordFormat;

        keywordFormat.setForeground(INIMANAGER()->foregroundColor(keyword));
        m_highlightingRules.insert(keyword, keywordFormat);
    }

    // remove keyword in m_disableHighlightingRules.
    foreach (const QString& keyword, keywordList) {
        if (m_disableHighlightingRules.contains(keyword))
            m_disableHighlightingRules.remove(keyword);
    }
}

void KeywordHighlighter::highlightBlock(const QString& sentence)
{
    Qt::CaseSensitivity caseSensitivity = INIMANAGER()->isEnabledHighlightCaseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    bool wholeWords = INIMANAGER()->isEnabledHighlightWholeWords();

    /* Revert the highlight block */
    foreach (const QString &keyword, m_disableHighlightingRules.keys()) {
        QRegExp expression;

        if (wholeWords) {
            expression.setPattern(QString("\\b") + keyword + QString("\\b"));
        } else {
            expression.setPattern(keyword);
        }
        expression.setCaseSensitivity(caseSensitivity);

        int index = expression.indexIn(sentence);

        if (index >= 0) {
            setCurrentBlockState(eNormalBlock);

            int length = sentence.length();
            setFormat(0, length, m_disableHighlightingRules.value(keyword));

            QTextCursor cursor(currentBlock());
            QTextBlockFormat blockFormat(cursor.blockFormat());
            blockFormat.setTopMargin(1);

            if (currentBlock().blockNumber() % 2) {
                blockFormat.setBackground(INIMANAGER()->customBackgroundColor());
            } else {
                blockFormat.setBackground(INIMANAGER()->backgroundColor());
            }
            cursor.mergeBlockFormat(blockFormat);
        }
    }

    /* Paint the highlight block */
    foreach (const QString &keyword, m_highlightingRules.keys()) {
        QRegExp expression;

        if (wholeWords) {
            expression.setPattern(QString("\\b") + keyword + QString("\\b"));
        } else {
            expression.setPattern(keyword);
        }
        expression.setCaseSensitivity(caseSensitivity);

        int index = expression.indexIn(sentence);
        if (index >= 0) {
            setCurrentBlockState(eHighlightedBlock);

            int length = sentence.length();
            setFormat(0, length, m_highlightingRules.value(keyword));

            QTextCursor cursor(currentBlock());
            QTextBlockFormat blockFormat(cursor.blockFormat());
            blockFormat.setTopMargin(1);
            blockFormat.setBackground(INIMANAGER()->backgroundColor(keyword));
            cursor.mergeBlockFormat(blockFormat);
        }
    }

    if (sentence.length() > 0)
        parse(sentence, currentBlock().blockNumber() + 1);

    // current block is reached the last block.
    if (document()->lineCount() == currentBlock().blockNumber() + 1) {
        m_resultsModel->generate();
        m_chart->generate(m_resultsModel->hashTable());
    }
}

void KeywordHighlighter::parse(const QString& sentence, int line)
{
    bool caseSensitive = INIMANAGER()->isEnabledHighlightCaseSensitive();
    bool wholeWords = INIMANAGER()->isEnabledHighlightWholeWords();

    foreach (const QString& keyword, m_highlightingRules.keys())
        m_resultsModel->putData(sentence, keyword, line, caseSensitive, wholeWords);
}

QStandardItemModel* KeywordHighlighter::resultModel()
{
    return m_resultsModel;
}

void KeywordHighlighter::clearResult()
{
    m_resultsModel->removeAll();
}

QPixmap* KeywordHighlighter::chart()
{
    return m_chart->pixmap();
}
