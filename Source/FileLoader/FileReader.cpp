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

#include "FileReader.h"
#include "Source/Setting/INIManager.h"
#include "Source/Config.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtCore/QDebug>

#if QT_VERSION >= 0x040700 && PERFORMANCE_CHECK
#include <QtCore/QElapsedTimer>
#endif

#define APPEND_LINE 700

FileReader::FileReader(QWidget* requester, const QString& codec, QObject* parent)
    : QThread(parent)
    , m_requester(requester)
    , m_codec(codec)
    , m_isRunning(true)
{
    connect(this, SIGNAL(finished()), this, SLOT(finished()));
}

FileReader::~FileReader()
{
    clearList();    
}

void FileReader::clearList()
{
    while (!m_byteArrayList.isEmpty())
        delete m_byteArrayList.takeFirst();
}

void FileReader::start(const QString& fileName)
{
    m_fileName = fileName;
    clearList();

    QThread::start();
}

void FileReader::suspend()
{
    disconnect();
    m_isRunning = false;
}

void FileReader::run()
{
    QFile file(m_fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;

    emit fileSizeNotify(m_requester, file.size());
    emit fileNameNotify(m_requester, m_fileName);
    emit readStarted(m_requester);

    int line = 0;
    int index = 0;
    int progressBytes = 0;
    bool codecDetected = false;
    m_byteArrayList.append(new QString);

#if QT_VERSION >= 0x040700 && PERFORMANCE_CHECK
    QElapsedTimer timer;
    timer.start();
#endif

    QTextStream textStream(&file);
    if (m_codec != QString("System")) {
        textStream.setAutoDetectUnicode(false);
        textStream.setCodec(m_codec.toLocal8Bit());
    }

    while (!textStream.atEnd()) {
        QString lineText = textStream.readLine() + "\n";
        m_byteArrayList.at(index)->append(lineText);

        if (!codecDetected) {
            emit codecDected(textStream.codec()->name());
            codecDetected = true;
        }

        if (!(++line % APPEND_LINE)) {
            progressBytes += m_byteArrayList.at(index)->size();
            emit readProgress(m_requester, m_byteArrayList.at(index), progressBytes * 100 / file.size());

            m_byteArrayList.append(new QString);
            ++index;

            if (!m_isRunning)
                break;
        }
    }

    emit readFinished(m_requester, m_byteArrayList.at(index));

#if QT_VERSION >= 0x040700 && PERFORMANCE_CHECK
    qDebug() << "Text Codec: " << textStream.codec()->name();
    qDebug() << "[Performance Check] FileReader: " << timer.elapsed() << "ms " << "Bytes: " << progressBytes;
#endif
}

void FileReader::finished()
{
    emit finished(m_requester);
}
