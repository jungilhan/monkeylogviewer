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

#include "ProgressDialog.h"

#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

#include <QtCore/QTimer>

ProgressDialog::ProgressDialog(QWidget *parent)
    : QDialog(parent)
    , m_contentsLabel(new QLabel(this))
    , m_progressBar(new QProgressBar(this))
    , m_okButton(new QPushButton(tr("Ok"), this))
    , m_cancelButton(new QPushButton(tr("Cancel"), this))
    , m_timer(0)
    , m_autoCloseMsec(-1)
    , m_currentMsec(-1)
{
    resize(340, 110);
    setFixedSize(340, 110);

    init();
}

ProgressDialog::~ProgressDialog()
{
    delete m_contentsLabel;
    delete m_progressBar;
    delete m_okButton;
    delete m_cancelButton;
    delete m_timer;
}

void ProgressDialog::init()
{
    m_contentsLabel->setGeometry(10, 10, 320, 30);

    m_progressBar->setGeometry(10, 40, 320, 20);
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(100);

    m_okButton->setGeometry(340 / 2 - 75, 75, 75, 25);
    m_okButton->setDisabled(true);
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(accept()));

    m_cancelButton->setGeometry(m_okButton->x() + m_okButton->width() + 10, 75, 75, 25);
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void ProgressDialog::hideEvent(QHideEvent* event)
{
    if (m_timer && m_timer->isActive())
        m_timer->stop();

    setValue(0);
    m_okButton->setDisabled(true);

    QDialog::hideEvent(event);
}

void ProgressDialog::setLabelText(const QString &text)
{
    m_contentsLabel->setText(text);
}

void ProgressDialog::setValue(int value)
{
    m_progressBar->setValue(value);

    if (value == 100) {
        if (m_autoCloseMsec >= 0)
            startAutoCloseTimer();

        m_okButton->setEnabled(true);
        m_okButton->setFocus();
    }
}

void ProgressDialog::setAutoClose(int msec)
{
    Q_ASSERT(msec >= 0);
    m_autoCloseMsec = m_currentMsec = msec;

    if (msec == 0)
        m_okButton->setText(QString("OK (0)"));
    else
        m_okButton->setText(QString("OK (%1)").arg(msec / 1000));
}

void ProgressDialog::startAutoCloseTimer()
{
    if (!m_timer) {
        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    }

    m_timer->start(1000);
}

void ProgressDialog::timeOut()
{
    if (m_currentMsec < 0) {
        m_timer->stop();
        accept();
        m_currentMsec = m_autoCloseMsec;
    }

    m_okButton->setEnabled(true);
    m_okButton->setText(QString("OK (%1)").arg(m_currentMsec / 1000));

    m_currentMsec -= 1000;
}
