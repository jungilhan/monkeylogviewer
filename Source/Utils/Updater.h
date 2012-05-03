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

#ifndef UPDATER_H
#define UPDATER_H

#include "Source/Http/Http.h"

class ProgressDialog;
class LoadingPopup;

class Updater : public QObject
{
    Q_OBJECT

public:
    Updater(QObject* parent = 0);
    virtual ~Updater();

    void start();

private:
    void init();
    void startDownloadXml();
    void startDownloadBinary();
    void startPatch();
    void parseXml(const QByteArray& data);
    bool isAvailable();
    void showLoadingPopup();
    void closeLoadingPopup();
    QWidget* mainWindow();

private slots:
    void downloadXmlFinished(QIODevice* data, bool error);
    void downloadBinaryFinished(QIODevice* data, bool error);
    void downloadBinaryProgress(int done, int total);
    void patchRequested();
    void patchCancled();

private:
    Http* m_xmlDownloader;
    Http* m_binaryDownloader;
    ProgressDialog* m_progressDialog;
    LoadingPopup* m_loadingPopup;
    QString m_name;
    QString m_version;
    QString m_versionStr;
    QString m_downloadUrl;
    QString m_webPage;
    QString m_patchFile;
};

#endif // UPDATER_H
