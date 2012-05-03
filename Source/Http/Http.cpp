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

#include "Http.h"
#include "Source/Config.h"

#include <QtCore/QUrl>
#include <QtCore/QBuffer>

#if DEBUG
#include <QtCore/QFile>
#include <QtCore/QDebug>
#endif

Http::Http(QObject* parent)
    : QHttp(parent)
    , m_Id(-1)
    , m_url(new QUrl)
    , m_data(new QBuffer(this))
{
    connect(this, SIGNAL(responseHeaderReceived(const QHttpResponseHeader&)), this, SLOT(readResponseHeader(const QHttpResponseHeader&)));
    connect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
}

Http::~Http()
{
    delete m_url;
    delete m_data;
}

int Http::connectUrl(const QString& url, QHttp::ConnectionMode mode)
{
    if (m_Id != -1)
        return -1;

    m_url->setUrl(url);

    QString path = url;
    path.remove(m_url->host());
    if (path.contains("http://", Qt::CaseInsensitive))
        path.remove("http://");
    else if (path.contains("https://", Qt::CaseInsensitive))
        path.remove("https://");

    setHost(m_url->host(), mode);
    m_Id = get(path, m_data);

#if DEBUG
    qDebug() << "[Http] Host: " << m_url->host();
    qDebug() << "[Http] Path: " << path;
#endif

    return m_Id;
}

void Http::readResponseHeader(const QHttpResponseHeader& responseHeader)
{
    switch (responseHeader.statusCode()) {
    case 200:	// Ok
        break;
    case 301:	// Moved Permanently
    case 303:	// See Other
    case 307:	// Temporary Redirect
    case 302:	// Found
    case 400:	// BadRequest
    case 404:	// Not Found
    default:
        abort();
    }
}

void Http::httpRequestFinished(int requestId, bool error)
{
    if (requestId != m_Id)
        return;

    emit readFinished(m_data, error);

#if DEBUG
    if (!error) {
        m_data->open(QIODevice::ReadOnly);

        QFile httpDump("httpDump.log");
        httpDump.open(QIODevice::WriteOnly);
        httpDump.write(m_data->readAll());
        httpDump.close();

        m_data->close();
    }
#endif

    m_Id = -1;
}
