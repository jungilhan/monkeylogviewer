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

#ifndef HTTP_H
#define HTTP_H

#include <QtNetwork/QHttp>

class QUrl;
class QByteArray;

class Http : public QHttp
{
    Q_OBJECT

signals:
    void readFinished(QIODevice*, bool);

public:
    Http(QObject* parent = 0);
    virtual ~Http();

    int connectUrl(const QString& url, QHttp::ConnectionMode mode = QHttp::ConnectionModeHttp);

private slots:
    void readResponseHeader(const QHttpResponseHeader& responseHeader);
    void httpRequestFinished(int requestId, bool error);

private:
    int m_Id;
    QUrl* m_url;
    QIODevice* m_data;
};

#endif // HTTP_H
