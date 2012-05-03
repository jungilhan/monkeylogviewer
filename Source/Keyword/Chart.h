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

#ifndef CHART_H
#define CHART_H

#include <QtGui/QWidget>
#include <QtCore/QHash>

class Http;
class QPixmap;

class Chart : public QObject
{
    Q_OBJECT

signals:
    void chartLoaded(QPixmap*);

public:
    Chart(QObject* parent = 0);
    virtual ~Chart();

    void generate(QHash<QString, QStringList> results);
    QPixmap* pixmap();

private:
    void init();
    QString httpParam(QHash<QString, QStringList> results);

private slots:
    void chartReady(QIODevice* data, bool error);

private:
    Http* m_http;
    QPixmap* m_chart;
};

#endif // CHART_H
