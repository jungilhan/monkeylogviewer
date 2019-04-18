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

#include "Chart.h"
#include "Source/Http/Http.h"
#include "Source/Setting/INIManager.h"

#include <QPixmap>

#define CHART_URL "http://chart.apis.google.com/chart"

Chart::Chart(QObject *parent)
    : QObject(parent)
    , m_http(new Http(this))
    , m_chart(new QPixmap)
{
    init();
}

Chart::~Chart()
{
    delete m_http;
    delete m_chart;
}

void Chart::init()
{
    connect(m_http, SIGNAL(readFinished(QIODevice*,bool)), this, SLOT(chartReady(QIODevice*,bool)));
}

void Chart::generate(QHash<QString, QStringList> results)
{
    QString param = httpParam(results);
    if (param.isEmpty()) {
        m_chart->fill(Qt::white);
        emit chartLoaded(m_chart);
        return;
    }

    m_http->connectUrl(CHART_URL + param);
}

QString Chart::httpParam(QHash<QString, QStringList> results)
{
    if (!results.size())
        return QString();

    QString chartSize("chs=250x200");
    QString chartType("cht=p");
    QString chartData("chd=t:");
    QString chartDataScale("chds=0,");
    QString chartLabel("chdl=");
    QString chartColor("chco=");

    int maxRange = 0;
    foreach (const QString& keyword, results.keys()) {
        QString length = QString("%1").arg(results.value(keyword).length());
        if (length.toInt() > maxRange)
            maxRange = length.toInt();

        chartData += length;
        chartData += ",";

        chartLabel += keyword;
        chartLabel += "|";

        chartColor += INIMANAGER()->backgroundColor(keyword).name();
        chartColor += ",";
    }

    chartDataScale += QString("%1").arg(maxRange);

    chartData.truncate(chartData.length() - 1);
    chartLabel.truncate(chartLabel.length() - 1);
    chartColor.truncate(chartColor.length() - 1);
    chartColor.remove("#");

    QString param = QString("?") + chartSize +
                    QString("&") + chartType +
                    QString("&") + chartData +
                    QString("&") + chartDataScale +
                    QString("&") + chartLabel +
                    QString("&") + chartColor;

    return param;
}

void Chart::chartReady(QIODevice* data, bool error)
{
    if (error) {
        m_chart->load(":/ChartView/Warning128x128.png");
        emit chartLoaded(m_chart);
        return;
    }

    data->open(QIODevice::ReadOnly);
    m_chart->loadFromData(data->readAll());
    data->close();

    emit chartLoaded(m_chart);
}


QPixmap* Chart::pixmap()
{
    return m_chart;
}
