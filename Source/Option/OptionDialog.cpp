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

#include "OptionDialog.h"
#include "FontSetupPage.h"
#include "HighlightSetupPage.h"

#include <QTabWidget>
#include <QPushButton>

OptionDialog::OptionDialog(QWidget* parent)
    : QDialog(parent)
    , m_tabWidget(0)
    , m_okButton(0)
    , m_cancelButton(0)
{
    init();
}

OptionDialog::~OptionDialog()
{
    delete m_tabWidget;
    delete m_okButton;
    delete m_cancelButton;
}

void OptionDialog::init()
{
    setWindowTitle("Options");

    const int offsetW = 100;
    const int offsetH = 40;

    resize(400 + offsetW, 320 + offsetH);
    setFixedSize(400 + offsetW, 320 + offsetH);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(createHighlightSetupPage(), tr("Highlight Settings"));
    m_tabWidget->addTab(createFontSetupPage(), tr("Font && Colors"));    
    m_tabWidget->setGeometry(10, 20, 380 + offsetW, 260 + offsetH);

    m_okButton = new QPushButton(tr("OK"), this);
    m_okButton->setGeometry(235 + offsetW, 290 + offsetH, 75, 23);
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(accept()));

    m_cancelButton = new QPushButton(tr("Cancel"), this);
    m_cancelButton->setGeometry(315 + offsetW, 290 + offsetH, 75, 23);
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

QWidget* OptionDialog::createFontSetupPage()
{
    QWidget* fontSetupPage = new FontSetupPage(this);
    connect(this, SIGNAL(accepted()), fontSetupPage, SLOT(optionApplied()));

    return fontSetupPage;
}

QWidget* OptionDialog::createHighlightSetupPage()
{
    QWidget* highlightSetupPage = new HighlightSetupPage(this);
    connect(this, SIGNAL(accepted()), highlightSetupPage, SLOT(highlightingRuleChanged()));
    connect(highlightSetupPage, SIGNAL(keywordStateChanged(QHash<QString, bool>&)), this, SIGNAL(keywordStateChanged(QHash<QString, bool>&)));
    connect(highlightSetupPage, SIGNAL(keywordColorChanged()), this, SIGNAL(keywordColorChanged()));
    connect(highlightSetupPage, SIGNAL(highlightOptionChanged()), this, SIGNAL(highlightOptionChanged()));

    return highlightSetupPage;
}
