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

#include "AboutDialog.h"
#include "Source/Config.h"

#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>

#if defined(Q_OS_LINUX)
#define FONT_FAMILY qApp->font().family()
#elif defined(Q_OS_WIN32)
#define FONT_FAMILY "Verdana"
#endif

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent)
    , m_logoLabel(0)
    , m_logoBgLabel(0)
    , m_titleLabel(0)
    , m_textLabel(0)
    , m_progectHomeLabel(0)
    , m_blogLabel(0)
    , m_okButton(0)
{
    init();
}

AboutDialog::~AboutDialog()
{
    delete m_okButton;
    delete m_progectHomeLabel;
    delete m_blogLabel;
    delete m_textLabel;
    delete m_titleLabel;
    delete m_logoBgLabel;
    delete m_logoLabel;
}

void AboutDialog::init()
{
    resize(400, 320);
    setFixedSize(400, 320);
    setWindowTitle("About - Money Log Viewer");

    m_logoBgLabel = new QLabel(this);
    m_logoBgLabel->setGeometry(10, 10, 110, 300);
    m_logoBgLabel->setStyleSheet("QLabel { border: 1px solid #8f8f91; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1 stop: 0 #d6d6d6, stop: 1 #737373); }");

    m_logoLabel = new QLabel(this);
    m_logoLabel->setGeometry(15, 15, 100, 100);
    m_logoLabel->setStyleSheet("border: none;");
    m_logoLabel->setPixmap(QPixmap(":/About/Logo.png"));

    m_titleLabel = new QLabel(this);
    m_titleLabel->setGeometry(125, 10, 260,30);
    m_titleLabel->setFont(QFont(FONT_FAMILY, 11));
    m_titleLabel->setText(QString("<b>%1 %2</b>").arg(PROGRAM_NAME).arg(VERSION_STR));

    m_textLabel = new QLabel(this);
    m_textLabel->setGeometry(125, 40, 260, 260);
    m_textLabel->setFont(QFont(FONT_FAMILY, 8));
    m_textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    m_textLabel->setOpenExternalLinks(true);
    m_textLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    m_progectHomeLabel = new QLabel(this);
    m_progectHomeLabel->setGeometry(125, 198, 260, 30);
    m_progectHomeLabel->setFont(QFont(FONT_FAMILY, 8));
    m_progectHomeLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    m_progectHomeLabel->setOpenExternalLinks(true);
    m_progectHomeLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QString projectHome(QString("<a href=\"%1\">%2</a>").arg(PROJECT_HOME).arg(PROJECT_HOME));
    m_progectHomeLabel->setText(projectHome);

    m_blogLabel = new QLabel(this);
    m_blogLabel->setGeometry(125, 238, 260, 30);
    m_blogLabel->setFont(QFont(FONT_FAMILY, 8));
    m_blogLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    m_blogLabel->setOpenExternalLinks(true);
    m_blogLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QString blog(QString("<a href=\"%1\">%2</a>").arg(BLOG).arg(BLOG));
    m_blogLabel->setText(blog);

    QString library(QString("Base on Qt %1").arg(LIBRARY_VERSION));
    QString build(QString("Built on %1").arg(BUILT_DATE));
    QString copyright("Monkey Log Viewer is Free Software.\nCopyright (C) 2010 - 2011 jungil Han.\nMaintainer jungil.Han@gmail.com\n");
    QString contributor("Andy Jang - paraline@gmail.com\nChanghun Jeong - bullseye73@gmail.com");
    QString icons("Icons by DryIcons' Works.\nhttp://dryicons.com");

    QString bodyText;
    bodyText += library;
    bodyText += "\n" + build;
    bodyText += "\n\n" + copyright;
    bodyText += "\n" + QString("[Contributor]\n");
    bodyText += contributor;
    bodyText += "\n\n" + QString("[Project Home]\n");
    bodyText += "\n\n" + QString("[Blog]\n");
    bodyText += "\n\n" + icons;

    m_textLabel->setText(bodyText);

    m_okButton = new QPushButton("OK", this);
    m_okButton->setGeometry(300, 285, 75, 25);
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(accept()));
}
