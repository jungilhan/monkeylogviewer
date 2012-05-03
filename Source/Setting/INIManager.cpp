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

#include "INIManager.h"
#include "Source/Config.h"

#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>

#if DEBUG
#include <QtCore/QDebug>
#endif

#define DEFAULT_FOREGROUND_COLOR QRgb(0x000000)
#define DEFAULT_BACKGROUND_COLOR QRgb(0xffffff)
#define DEFAULT_CUSTOM_BACKGROUND_COLOR QRgb(0xeeeeee)
#define DEFAULT_CURRENT_LINE_FG_COLOR QRgb(0x000000)
#define DEFAULT_CURRENT_LINE_BG_COLOR QRgb(0xc0e7ff) //QRgb(0xffcc66)
#define DEFAULT_FONT_SIZE 9

#define WINDOW_GROUP "WINDOW"
#define VIEWER_GROUP "VIEWER"
#define FILE_BROWSER_GROUP "FILE_BROWSER"
#define RESUT_VIEW_GROUP "RESULT_VIEW"
#define CHART_VIEW_GROUP "CHART_VIEW"
#define HIGHLIGHTING_RULES_GROUP "HIGHLIGHTING_RULES"
#define HIGHLIGHTING_OPTION_GROUP "HIGHLIGHTING_OPTION"

INIManager* INIManager::s_this = 0;

INIManager::INIManager()
    : QSettings(QCoreApplication::applicationDirPath() + "/" + INI_FILE, QSettings::IniFormat)
{

}

INIManager::~INIManager()
{

}

INIManager* INIManager::getInstance()
{
    if (!s_this)
        s_this = new INIManager;

    return s_this;
}

QVariant INIManager::value(const QString& prefix, const QString& key, const QVariant& defaultValue)
{
    QVariant variant;

    beginGroup(prefix);
    variant = QSettings::value(key, defaultValue);
    endGroup();

    return variant;
}

void INIManager::setValue(const QString& prefix, const QString& key, const QVariant& value)
{
    beginGroup(prefix);
    QSettings::setValue(key, value);
    endGroup();
}

void INIManager::removeGroup(const QString& group)
{
    remove(group);
}

bool INIManager::isEnableCustomFont()
{
    return value(VIEWER_GROUP, "CustomFont", false).toBool();
}

void INIManager::enableCustomFont(bool enable)
{
    setValue(VIEWER_GROUP, "CustomFont", enable);
}

bool INIManager::isEnableCustomColor()
{
    return value(VIEWER_GROUP, "CustomColor", false).toBool();
}

void INIManager::enableCustomColor(bool enable)
{
    setValue(VIEWER_GROUP, "CustomColor", enable);
}

QColor INIManager::defaulForegroundColor()
{
    return QColor(DEFAULT_FOREGROUND_COLOR);
}

QColor INIManager::foregroundColor()
{
    return value(VIEWER_GROUP, "Foreground", DEFAULT_FOREGROUND_COLOR).toUInt();
}

void INIManager::setForegroundColor(const QColor& color)
{
    setValue(VIEWER_GROUP, "Foreground", static_cast<unsigned int>(color.rgba()));
}

QColor INIManager::defaulBackgroundColor()
{
    return QColor(DEFAULT_BACKGROUND_COLOR);
}

QColor INIManager::backgroundColor()
{
    return value(VIEWER_GROUP, "Background", DEFAULT_BACKGROUND_COLOR).toUInt();
}

void INIManager::setBackgroundColor(const QColor& color)
{
    setValue(VIEWER_GROUP, "Background", static_cast<unsigned int>(color.rgba()));
}

QColor INIManager::defaulCustomBackgroundColor()
{
    return QColor(DEFAULT_CUSTOM_BACKGROUND_COLOR);
}

QColor INIManager::customBackgroundColor()
{
    return value(VIEWER_GROUP, "CustomBackground", DEFAULT_CUSTOM_BACKGROUND_COLOR).toUInt();
}

void INIManager::setCustomBackgroundColor(const QColor& color)
{
    setValue(VIEWER_GROUP, "CustomBackground", static_cast<unsigned int>(color.rgba()));
}

QColor INIManager::currentLineFgColor()
{
    return value(VIEWER_GROUP, "CurrentLineFgColor", DEFAULT_CURRENT_LINE_FG_COLOR).toUInt();
}

QColor INIManager::currentLineBgColor()
{
    return value(VIEWER_GROUP, "CurrentLineBgColor", DEFAULT_CURRENT_LINE_BG_COLOR).toUInt();
}

QColor INIManager::defaultCurrentLineFgColor()
{
    return QColor(DEFAULT_CURRENT_LINE_FG_COLOR);
}

QColor INIManager::defaultCurrentLineBgColor()
{
    return QColor(DEFAULT_CURRENT_LINE_BG_COLOR);
}

void INIManager::setCurrentLineFgColor(const QColor& color)
{
    setValue(VIEWER_GROUP, "CurrentLineFgColor", static_cast<unsigned int>(color.rgba()));
}

void INIManager::setCurrentLineBgColor(const QColor& color)
{
    setValue(VIEWER_GROUP, "CurrentLineBgColor", static_cast<unsigned int>(color.rgba()));
}

QFont INIManager::font()
{
    QFont font;

    QByteArray family(value(VIEWER_GROUP, "FontFamily", QApplication::font().family()).toByteArray());
    font.setFamily(QString::fromLocal8Bit(family));

    font.setPointSize(value(VIEWER_GROUP, "FontSize", QApplication::font().pointSize()).toInt());
    font.setBold(value(VIEWER_GROUP, "Bold", false).toBool());

    QFont::Style style = static_cast<QFont::Style>(value(VIEWER_GROUP, "FontStyle", 0).toUInt());
    font.setStyle(style);

#if DEBUG
    qDebug() << "[INIMANAGER::getFont]" << font.family() << font.pointSize() << font.bold() << font.style();
#endif

    return font;
}

int INIManager::fontSize()
{
    return value(VIEWER_GROUP, "FontSize", QApplication::font().pointSize()).toInt();
}

void INIManager::setFont(const QFont& font)
{
    QString family(font.family());
    QByteArray ba = family.toLocal8Bit();

    setValue(VIEWER_GROUP, "FontFamily", ba);
    setValue(VIEWER_GROUP, "FontSize", font.pointSize());
    setValue(VIEWER_GROUP, "Bold", font.bold());
    setValue(VIEWER_GROUP, "FontStyle", font.style());

#if DEBUG
    qDebug() << "[INIMANAGER::setFont]" << font.family() << font.pointSize() << font.bold() << font.style();
#endif
}

QColor INIManager::foregroundColor(const QString& keyword)
{
    QByteArray encodedBase64(keyword.toUtf8());
    QString key(encodedBase64.toBase64());
    key += "_F";

    return value(HIGHLIGHTING_RULES_GROUP, key, DEFAULT_FOREGROUND_COLOR).toUInt();
}

void INIManager::setForegroundColor(const QString& keyword, const QColor& color)
{
    QByteArray encodedBase64(keyword.toUtf8());
    QString key(encodedBase64.toBase64());
    key += "_F";

    setValue(HIGHLIGHTING_RULES_GROUP, key, static_cast<unsigned int>(color.rgba()));
}

QColor INIManager::backgroundColor(const QString& keyword)
{
    QByteArray encodedBase64(keyword.toUtf8());
    QString key(encodedBase64.toBase64());
    key += "_B";

    return value(HIGHLIGHTING_RULES_GROUP, key, DEFAULT_BACKGROUND_COLOR).toUInt();
}

void INIManager::setBackgroundColor(const QString& keyword, const QColor& color)
{
    QByteArray encodedBase64(keyword.toUtf8());
    QString key(encodedBase64.toBase64());
    key += "_B";

    setValue(HIGHLIGHTING_RULES_GROUP, key, static_cast<unsigned int>(color.rgba()));
}

QStringList INIManager::highlightingKeywordList()
{
    return value(HIGHLIGHTING_RULES_GROUP, "KEYWORD_LIST", QStringList()).toStringList();
}

void INIManager::setHighlightingKeywordList(const QStringList& keywordList)
{
    setValue(HIGHLIGHTING_RULES_GROUP, "KEYWORD_LIST", keywordList);
}

void INIManager::removeHighlightingRulesGroup()
{
    removeGroup(HIGHLIGHTING_RULES_GROUP);
}

void INIManager::setRecentDirectory(const QString& directory)
{
    setValue(WINDOW_GROUP, "RecentDirectory", directory);
}

QString INIManager::recentDirectory()
{    
    return value(WINDOW_GROUP, "RecentDirectory", QCoreApplication::applicationDirPath()).toString();
}

void INIManager::setRecentFiles(const QStringList& files)
{
    setValue(WINDOW_GROUP, "RecentFiles", files);
}

QStringList INIManager::recentFiles()
{
    return value(WINDOW_GROUP, "RecentFiles", QStringList()).toStringList();
}

void INIManager::setFindCaseSensitive(bool caseSensitive)
{
    setValue(VIEWER_GROUP, "CaseSensitive", caseSensitive);
}

void INIManager::setFindWholeWords(bool wholeWords)
{
    setValue(VIEWER_GROUP, "WholeWords", wholeWords);
}

void INIManager::setFindRegularExpressions(bool regularExpressions)
{
    setValue(VIEWER_GROUP, "RegularExpressions", regularExpressions);
}

bool INIManager::isEnabledFindCaseSensitive()
{
    return value(VIEWER_GROUP, "CaseSensitive", false).toBool();
}

bool INIManager::isEnabledFindWholeWords()
{
    return value(VIEWER_GROUP, "WholeWords", false).toBool();
}

bool INIManager::isEnabledFindRegularExpressions()
{
    return value(VIEWER_GROUP, "RegularExpressions", false).toBool();
}

void INIManager::setHighlightCaseSensitive(bool caseSensitive)
{
    setValue(HIGHLIGHTING_OPTION_GROUP, "CaseSensitive", caseSensitive);
}

void INIManager::setHighlightWholeWords(bool wholeWords)
{
    setValue(HIGHLIGHTING_OPTION_GROUP, "WholeWords", wholeWords);
}

void INIManager::setHighlightRegularExpressions(bool regularExpressions)
{
    setValue(HIGHLIGHTING_OPTION_GROUP, "RegularExpressions", regularExpressions);
}

bool INIManager::isEnabledHighlightCaseSensitive()
{
    return value(HIGHLIGHTING_OPTION_GROUP, "CaseSensitive", true).toBool();
}

bool INIManager::isEnabledHighlightWholeWords()
{
    return value(HIGHLIGHTING_OPTION_GROUP, "WholeWords", false).toBool();
}

bool INIManager::isEnabledHighlightRegularExpressions()
{
    return value(HIGHLIGHTING_OPTION_GROUP, "RegularExpressions", false).toBool();
}

Qt::DockWidgetArea INIManager::fileBrowserArea()
{
    return (Qt::DockWidgetArea)value(FILE_BROWSER_GROUP, "FileBrowserArea", (unsigned int)Qt::LeftDockWidgetArea).toUInt();
}

void INIManager::setFileBrowserArea(Qt::DockWidgetArea area)
{
    setValue(FILE_BROWSER_GROUP, "FileBrowserArea", (unsigned int)area);
}

bool INIManager::isVisibleFileBrowser()
{
    return value(FILE_BROWSER_GROUP, "FileBrowserVisible", false).toBool();
}

void INIManager::setFileBrowserVisible(bool visible)
{
    setValue(FILE_BROWSER_GROUP, "FileBrowserVisible", visible);
}

Qt::DockWidgetArea INIManager::resultViewArea()
{
    return (Qt::DockWidgetArea)value(RESUT_VIEW_GROUP, "ResultViewArea", (unsigned int)Qt::RightDockWidgetArea).toUInt();
}

void INIManager::setResultViewArea(Qt::DockWidgetArea area)
{
    setValue(RESUT_VIEW_GROUP, "ResultViewArea", (unsigned int)area);
}

bool INIManager::isVisibleResultView()
{
    return value(RESUT_VIEW_GROUP, "ResultViewVisible", true).toBool();
}

void INIManager::setResultViewVisible(bool visible)
{
    setValue(RESUT_VIEW_GROUP, "ResultViewVisible", visible);
}

Qt::DockWidgetArea INIManager::chartViewArea()
{
    return (Qt::DockWidgetArea)value(CHART_VIEW_GROUP, "ChartViewArea", (unsigned int)Qt::RightDockWidgetArea).toUInt();
}

void INIManager::setChartViewArea(Qt::DockWidgetArea area)
{
    setValue(CHART_VIEW_GROUP, "ChartViewArea", (unsigned int)area);
}

bool INIManager::isVisibleChartView()
{
    return value(CHART_VIEW_GROUP, "ChartViewVisible", true).toBool();
}

void INIManager::setChartViewVisible(bool visible)
{
    setValue(CHART_VIEW_GROUP, "ChartViewVisible", visible);
}
