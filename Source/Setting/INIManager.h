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

#ifndef INIMANAGER_H
#define INIMANAGER_H

#include <QtCore/QSettings>
#include <QtGui/QFont>
#include <QtGui/QColor>

#define INIMANAGER() INIManager::getInstance()

class INIManager : public QSettings
{
    Q_OBJECT

public:
    static INIManager* getInstance();
    virtual ~INIManager();

    void setValue(const QString& prefix, const QString& key, const QVariant& value);
    QVariant value(const QString& prefix, const QString& key, const QVariant& defaultValue = QVariant());

    bool isEnableCustomFont();
    void enableCustomFont(bool enable);

    bool isEnableCustomColor();
    void enableCustomColor(bool enable);

    QColor defaulForegroundColor();
    QColor foregroundColor();
    void setForegroundColor(const QColor& color);

    QColor defaulBackgroundColor();
    QColor backgroundColor();
    void setBackgroundColor(const QColor& color);

    QColor defaulCustomBackgroundColor();
    QColor customBackgroundColor();
    void setCustomBackgroundColor(const QColor& color);

    QColor currentLineFgColor();
    QColor currentLineBgColor();
    QColor defaultCurrentLineFgColor();
    QColor defaultCurrentLineBgColor();
    void setCurrentLineFgColor(const QColor& color);
    void setCurrentLineBgColor(const QColor& color);

    QFont font();
    int fontSize();
    void setFont(const QFont& font);

    QColor foregroundColor(const QString& keyword);
    void setForegroundColor(const QString& keyword, const QColor& color);

    QColor backgroundColor(const QString& keyword);
    void setBackgroundColor(const QString& keyword, const QColor& color);

    QStringList highlightingKeywordList();
    void setHighlightingKeywordList(const QStringList& keywordList);
    void removeHighlightingRulesGroup();

    void setRecentDirectory(const QString& directory);
    QString recentDirectory();

    void setRecentFiles(const QStringList& files);
    QStringList recentFiles();

    void setFindCaseSensitive(bool caseSensitive);
    void setFindWholeWords(bool wholeWords);
    void setFindRegularExpressions(bool regularExpressions);

    bool isEnabledFindCaseSensitive();
    bool isEnabledFindWholeWords();
    bool isEnabledFindRegularExpressions();

    void setHighlightCaseSensitive(bool caseSensitive);
    void setHighlightWholeWords(bool wholeWords);
    void setHighlightRegularExpressions(bool regularExpressions);

    bool isEnabledHighlightCaseSensitive();
    bool isEnabledHighlightWholeWords();
    bool isEnabledHighlightRegularExpressions();

    Qt::DockWidgetArea fileBrowserArea();
    void setFileBrowserArea(Qt::DockWidgetArea area);
    bool isVisibleFileBrowser();
    void setFileBrowserVisible(bool visible);

    Qt::DockWidgetArea resultViewArea();
    void setResultViewArea(Qt::DockWidgetArea area);
    bool isVisibleResultView();
    void setResultViewVisible(bool visible);

    Qt::DockWidgetArea chartViewArea();
    void setChartViewArea(Qt::DockWidgetArea area);
    bool isVisibleChartView();
    void setChartViewVisible(bool visible);

private:
    INIManager();
    void removeGroup(const QString& group);

private:
    static INIManager* s_this;
};

#endif // INIMANAGER_H
