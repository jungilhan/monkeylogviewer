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

#include "GotoLineDialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

GotoLineDialog::GotoLineDialog(QWidget* parent)
    : QDialog(parent)
    , m_label(0)
    , m_lineEdit(0)
    , m_okButton(0)
    , m_cancelButton(0)
{
    setWindowTitle(tr("Go To Line"));
    resize(260, 90);
    setFixedSize(260, 90);

    QFont font;
    font.setPointSize(8);
    setFont(font);

    init();
}

GotoLineDialog::~GotoLineDialog()
{
    delete m_cancelButton;
    delete m_okButton;
    delete m_lineEdit;
    delete m_label;
}

void GotoLineDialog::init()
{
    m_label = new QLabel(tr("Line Number:"), this);
    m_label->setGeometry(10, 10, 240, 16);

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setGeometry(10, 30, 240, 20);
    m_lineEdit->setInputMask("999999999999999");

    m_okButton = new QPushButton(tr("OK"), this);
    m_okButton->setGeometry(120, 60, 60, 20);
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(gotoLine()));

    m_cancelButton = new QPushButton(tr("Cancel"), this);
    m_cancelButton->setGeometry(185, 60, 60, 20);
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void GotoLineDialog::gotoLine()
{
    emit gotoLine(m_lineEdit->text().toInt());
    accept();
}
