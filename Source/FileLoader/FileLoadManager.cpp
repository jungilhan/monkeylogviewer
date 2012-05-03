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

#include "FileLoadManager.h"
#include "FileReader.h"

#include <QtCore/QTimer>

FileLoadManager* FileLoadManager::s_this = 0;

FileLoadManager::FileLoadManager()
{

}

FileLoadManager::~FileLoadManager()
{

}

FileLoadManager* FileLoadManager::getInstance()
{
    if (!s_this)
        s_this = new FileLoadManager();

    return s_this;
}

void FileLoadManager::load(const QWidget* requester, const QString &fileName, const QString& codec)
{
    // Suspend thread if same task is already running.
    suspend(requester);

    FileReader *fileReader = new FileReader(const_cast<QWidget*>(requester), codec, this);
    m_fileReaderMap.insert(const_cast<QWidget*>(requester), fileReader);

    connect(fileReader, SIGNAL(readStarted(QWidget*)), this, SIGNAL(loadStarted(QWidget*)));
    connect(fileReader, SIGNAL(readProgress(QWidget*,QString*,int)), this, SIGNAL(loadProgress(QWidget*,QString*,int)), Qt::BlockingQueuedConnection);
    connect(fileReader, SIGNAL(readFinished(QWidget*,QString*)), this, SIGNAL(loadFinished(QWidget*,QString*)), Qt::BlockingQueuedConnection);
    connect(fileReader, SIGNAL(codecDected(QString)), this, SIGNAL(codecDected(QString)));
    connect(fileReader, SIGNAL(finished(QWidget*)), this, SLOT(finished(QWidget*)));
    connect(fileReader, SIGNAL(fileSizeNotify(QWidget*,long)), this, SIGNAL(fileSizeNotify(QWidget*,long)));
    connect(fileReader, SIGNAL(fileNameNotify(QWidget*,QString)), this, SIGNAL(fileNameNotify(QWidget*,QString)));

    fileReader->start(fileName);
}

void FileLoadManager::suspendAll()
{
    foreach (const QWidget* key, m_fileReaderMap.keys())
        suspend(key);
}

void FileLoadManager::suspendOthers(const QWidget* requester)
{
    foreach (const QWidget* key, m_fileReaderMap.keys()) {
        if (requester != key)
            suspend(key);
    }
}

void FileLoadManager::suspend(const QWidget* requester)
{
    if (m_fileReaderMap.contains(const_cast<QWidget*>(requester))) {
        FileReader* fileReader = m_fileReaderMap.take(const_cast<QWidget*>(requester));
        fileReader->suspend();

        m_deleteSoonQueue.enqueue(fileReader);
        QTimer::singleShot(3000, this, SLOT(deleteSoonQueueTriggered()));
    }
}

void FileLoadManager::deleteSoonQueueTriggered()
{
    if (!m_deleteSoonQueue.isEmpty())
        delete m_deleteSoonQueue.dequeue();
}

void FileLoadManager::finished(QWidget* requester)
{
    if (m_fileReaderMap.contains(const_cast<QWidget*>(requester)))
        delete m_fileReaderMap.take(const_cast<QWidget*>(requester));
}
