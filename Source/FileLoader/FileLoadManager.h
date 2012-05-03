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

#ifndef FILELOADMANAGER_H
#define FILELOADMANAGER_H

#define FILELOADMANAGER() FileLoadManager::getInstance()

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QQueue>
#include <QtCore/QStringList>

class FileReader;

class FileLoadManager : public QObject
{
    Q_OBJECT

signals:
    void loadStarted(QWidget*);
    void loadFinished(QWidget*, QString*);
    void loadProgress(QWidget*, QString*, int progress);
    void codecDected(QString);
    void fileSizeNotify(QWidget*, long);
    void fileNameNotify(QWidget*, QString);
    void keywordFilterTableUpdated(QWidget*, QHash<QString, QStringList>*);

public:
    static FileLoadManager* getInstance();
    virtual ~FileLoadManager();

    void load(const QWidget* requester, const QString& fileName, const QString& codec = QString("System"));

    void suspendAll();
    void suspendOthers(const QWidget* requester);
    void suspend(const QWidget* requester);

private slots:
    void finished(QWidget* requester); // Thread has finished executing.
    void deleteSoonQueueTriggered();

private:
    FileLoadManager();

private:
    QHash<QWidget*, FileReader*> m_fileReaderMap;
    QQueue<FileReader*> m_deleteSoonQueue;
    static FileLoadManager* s_this;
};

#endif // FILELOADMANAGER_H
