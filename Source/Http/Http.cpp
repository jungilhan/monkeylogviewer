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
    : QObject(parent)
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

int Http::connectUrl(const QString& url)
{

    return m_Id;
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
