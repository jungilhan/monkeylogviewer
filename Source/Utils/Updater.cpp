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

#include "Updater.h"
#include "Source/Config.h"
#include "Source/Utils/ProgressDialog.h"
#include "Source/Utils/LoadingPopup.h"

#include <QApplication>
#include <QDesktopServices>
#include <QMessageBox>

#include <QtCore/QProcess>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QFile>
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#include <shellapi.h>
#endif

#define XML_URL "http://monkeylogviewer.googlecode.com/files/monkeyLogViewer.xml"

Updater::Updater(QObject* parent)
    : QObject(parent)
    , m_xmlDownloader(new Http(this))
    , m_binaryDownloader(new Http(this))
    , m_progressDialog(0)
    , m_loadingPopup(0)
{
    init();
}

Updater::~Updater()
{
//    if (m_xmlDownloader->currentId())
//        m_xmlDownloader->abort();
//
//    if (m_binaryDownloader->currentId())
//        m_binaryDownloader->abort();

    delete m_xmlDownloader;
    delete m_binaryDownloader;

    delete m_loadingPopup;
    delete m_progressDialog;
}

void Updater::init()
{
    connect(m_xmlDownloader, SIGNAL(readFinished(QIODevice*,bool)), this, SLOT(downloadXmlFinished(QIODevice*,bool)));

    connect(m_binaryDownloader, SIGNAL(dataReadProgress(int,int)), this, SLOT(downloadBinaryProgress(int,int)));
    connect(m_binaryDownloader, SIGNAL(readFinished(QIODevice*,bool)), this, SLOT(downloadBinaryFinished(QIODevice*,bool)));
}

void Updater::start()
{
    showLoadingPopup();
    startDownloadXml();
}

void Updater::startDownloadXml()
{
    m_xmlDownloader->connectUrl(XML_URL);
}

void Updater::startDownloadBinary()
{    
    m_binaryDownloader->connectUrl(m_downloadUrl);

    if (!m_progressDialog) {
        m_progressDialog = new ProgressDialog(mainWindow());
        connect(m_progressDialog, SIGNAL(accepted()), this, SLOT(patchRequested()));
        connect(m_progressDialog, SIGNAL(rejected()), this, SLOT(patchCancled()));
    }

    m_progressDialog->setWindowTitle(tr("Update"));
    m_progressDialog->setLabelText(tr("Downloading"));
    m_progressDialog->setAutoClose(5000);
    m_progressDialog->show();
}

void Updater::downloadXmlFinished(QIODevice* data, bool error)
{
    closeLoadingPopup();

    if (error) {
//        QMessageBox::critical(mainWindow(), tr("Update Error"), QString("Error Code: %1").arg(m_binaryDownloader->lastResponse().statusCode()) + "\n" + m_binaryDownloader->errorString());
        return;
    }

    data->open(QIODevice::ReadOnly);
    parseXml(data->readAll());
    data->close();

    if (isAvailable()) {
#if defined(Q_OS_WIN32)
        int result = QMessageBox::information(mainWindow(), tr("Check for Updates"), tr("A new version, <b>") + m_versionStr + tr(" </b>is available to download.<br/>Would you like to download it?"),
                                 QMessageBox::Ok, QMessageBox::Cancel);
#elif defined(Q_OS_LINUX)
        int result = QMessageBox::information(mainWindow(), tr("Check for Updates"), tr("A new version, <b>") + m_versionStr + tr(" </b>is available to download.<br/>Please go to http://code.google.com/p/monkeylogviewer to get it."),
                                 QMessageBox::Ok, QMessageBox::Cancel);
#endif
        switch (result) {
        case QMessageBox::Ok:
#if defined(Q_OS_WIN32)
            startDownloadBinary();
#elif defined(Q_OS_LINUX)
            QDesktopServices::openUrl(QUrl(m_webPage));
#endif
            break;
        case QMessageBox::Cancel:

            break;
        }

    } else {
        QMessageBox::information(mainWindow(), tr("Check for Updates"), tr("This version is latest."));
    }
}

void Updater::downloadBinaryFinished(QIODevice* data, bool error)
{
    if (error) {
        if (m_progressDialog->isVisible())
            m_progressDialog->hide();

//        QMessageBox::critical(mainWindow(), tr("Update Error"), QString("Error Code: %1").arg(m_binaryDownloader->lastResponse().statusCode()) + "\n" + m_binaryDownloader->errorString());
        return;
    }

    data->open(QIODevice::ReadOnly);

    m_patchFile = QDir::tempPath() + "/" + m_name;

    QFile file(m_patchFile);
    file.open(QIODevice::WriteOnly);
    file.write(data->readAll());
    file.close();

    data->close();

    // Apply the patch.
    m_progressDialog->setLabelText(tr("Application will be restarted after 5 seconds."));
}

void Updater::downloadBinaryProgress(int done, int total)
{
    Q_ASSERT(m_progressDialog);

    int percent = done * 100 / total;
    m_progressDialog->setValue(percent);
}

void Updater::patchRequested()
{
    startPatch();
}

void Updater::patchCancled()
{
//    if (m_binaryDownloader->currentId())
//        m_binaryDownloader->abort();
}

void Updater::startPatch()
{
    m_patchFile.insert(0, "\"");
    m_patchFile += "\"";
    m_patchFile.replace("/", "\\");

#ifdef Q_OS_WIN
    // Support for unicode - patched by cowo78@gamil.com
#ifdef UNICODE
    int result = (int)::ShellExecuteW(0, L"open", m_patchFile.toStdWString().c_str(), 0, 0, SW_SHOWNORMAL);
#else
    int result = (int)::ShellExecuteA(0, "open", m_patchFile.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
#endif
    if (result == SE_ERR_ACCESSDENIED) {
        // Requesting elevation
#ifdef UNICODE
        result = (int)::ShellExecuteW(0, L"runas", m_patchFile.toStdWString().c_str(), 0, 0, SW_SHOWNORMAL);
#else
        result = (int)::ShellExecuteA(0, "runas", m_patchFile.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
#endif
    }
#endif

    qApp->quit();
}

void Updater::parseXml(const QByteArray& data)
{
    QXmlStreamReader xmlReader(data);

    while (!xmlReader.atEnd()) {
        xmlReader.readNext();

        if (xmlReader.isStartElement() && xmlReader.name() == "name") {
            m_name = xmlReader.readElementText();
#if DEBUG
            qDebug() << m_name;
#endif
        }

        if (xmlReader.isStartElement() && xmlReader.name() == "version") {
            m_version = xmlReader.readElementText();
#if DEBUG
            qDebug() << m_version;
#endif
        }

        if (xmlReader.isStartElement() && xmlReader.name() == "versionStr") {
            m_versionStr = xmlReader.readElementText();
#if DEBUG
            qDebug() << m_versionStr;
#endif
        }

        if (xmlReader.isStartElement() && xmlReader.name() == "url") {
            m_downloadUrl = xmlReader.readElementText();
#if DEBUG
            qDebug() << m_downloadUrl;
#endif
        }

        if (xmlReader.isStartElement() && xmlReader.name() == "webPage") {
            m_webPage = xmlReader.readElementText();
#if DEBUG
            qDebug() << m_webPage;
#endif
        }
    }

    if (xmlReader.hasError())
        qDebug() << xmlReader.errorString();
}

bool Updater::isAvailable()
{
    int latestVersion = m_version.mid(QString("0x").length()).toInt();
    int currentVersion = QString(VERSION).mid(QString("0x").length()).toInt();

#if DEBUG
    qDebug() << latestVersion;
    qDebug() << currentVersion;
#endif

    return latestVersion > currentVersion ? true : false;
}

void Updater::showLoadingPopup()
{
    if (m_loadingPopup)
        delete m_loadingPopup;

    m_loadingPopup = new LoadingPopup(mainWindow());
    m_loadingPopup->setText("Connecting...");
    m_loadingPopup->open();
}

void Updater::closeLoadingPopup()
{
    m_loadingPopup->accept();

    delete m_loadingPopup;
    m_loadingPopup = 0;
}

QWidget* Updater::mainWindow()
{
    QWidget* mainWindow = 0;

    foreach (QWidget* widget, QApplication::allWidgets()) {
        if (widget->objectName() == "MainWindow")
            mainWindow = widget;
    }

    Q_ASSERT(mainWindow);
    return mainWindow;
}
