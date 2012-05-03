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

#include "FontSetupPage.h"
#include "Source/Config.h"
#include "Source/LogViewer/LogViewer.h"
#include "Source/Setting/INIManager.h"

#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QFontComboBox>
#include <QtGui/QComboBox>
#include <QtGui/QFontDatabase>
#include <QtGui/QPushButton>
#include <QtGui/QColorDialog>
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>

FontSetupPage::FontSetupPage(QWidget *parent)
    : QWidget(parent)
    , m_fontGroup(0)
    , m_familyLabel(0)
    , m_styleLabel(0)
    , m_sizeLabel(0)
    , m_familyComboBox(0)
    , m_styleComboBox(0)
    , m_sizeComboBox(0)
    , m_colorGroup(0)
    , m_foregroudLabel(0)
    , m_backgroudLabel(0)
    , m_highlightLabel(0)
    , m_foregroudColorButton(0)
    , m_backgroudColorButton(0)
    , m_customBackgroudColorButton(0)
    , m_currentLineFgColorButton(0)
    , m_currentLineBgColorButton(0)
    , m_colorDialog(0)
    , m_plainTextEdit(0)
    , m_customFontEnabled(0)
    , m_customColorEnabled(0)
{   
    m_plainTextEdit = new LogViewer(this, false);
    m_plainTextEdit->setAutoFillBackground(true);
    m_plainTextEdit->setGeometry(330, 10, 136, 248);
    m_plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_plainTextEdit->setPlainText(QTextCodec::codecForName("UTF-8")->toUnicode(("안녕\nHello\nこんにちは\nЗдрасти\nПривеx\nBok\nMerhaba")));
    m_plainTextEdit->setFont(INIMANAGER()->font());

    initFontGroup();
    initColorGroup();
}

FontSetupPage::~FontSetupPage()
{
    delete m_familyLabel;
    delete m_styleLabel;
    delete m_sizeLabel;
    delete m_familyComboBox;
    delete m_styleComboBox;
    delete m_sizeComboBox;
    delete m_fontGroup;

    delete m_foregroudLabel;
    delete m_backgroudLabel;
    delete m_highlightLabel;
    delete m_foregroudColorButton;
    delete m_backgroudColorButton;
    delete m_customBackgroudColorButton;
    delete m_currentLineFgColorButton;
    delete m_currentLineBgColorButton;
    delete m_colorGroup;

    delete m_colorDialog;

    delete m_plainTextEdit;
}

void FontSetupPage::initFontGroup()
{
    const int offsetW = 40;
    const int offsetH = 20;

    m_fontGroup = new QGroupBox(tr("Use Custom Font"), this);
    m_fontGroup->setCheckable(true);
    m_fontGroup->setChecked(INIMANAGER()->isEnableCustomFont());
    m_fontGroup->setGeometry(10, 10, 270 + offsetW, 110 + offsetH);
    m_fontGroup->setFixedSize(270 + offsetW, 110 + offsetH);
    connect(m_fontGroup, SIGNAL(clicked(bool)), this, SLOT(fontGroupChecked(bool)));
    m_customFontEnabled = INIMANAGER()->isEnableCustomFont();

    m_familyLabel = new QLabel(tr("Family"), m_fontGroup);
    m_familyLabel->setGeometry(10, 30, 46, 20);

    m_styleLabel = new QLabel(tr("Style"), m_fontGroup);
    m_styleLabel->setGeometry(10, 60, 46, 20);

    m_sizeLabel = new QLabel(tr("Size"), m_fontGroup);
    m_sizeLabel->setGeometry(10, 90, 46, 20);

    m_familyComboBox = new QFontComboBox(m_fontGroup);
    m_familyComboBox->setGeometry(66, 30, 194 + offsetW, 20);
    m_familyComboBox->setCurrentFont(m_plainTextEdit->font());
    connect(m_familyComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(familyChanged(QFont)));

    m_styleComboBox = new QComboBox(m_fontGroup);
    m_styleComboBox->setGeometry(66, 60, 194 + offsetW, 20);
    m_styleComboBox->addItem(tr("Normal"));
    m_styleComboBox->addItem(tr("Italic"));
    m_styleComboBox->addItem(tr("Bold"));
    m_styleComboBox->addItem(tr("Bold Italic"));
    connect(m_styleComboBox, SIGNAL(activated(QString)), this, SLOT(styleChanged(QString)));

    QFont::Style style = m_plainTextEdit->font().style();
    bool bold = m_plainTextEdit->font().bold();
    int index = 0;
    if (!bold && !style) {
        index = 0;
    } else if (!bold && style == 1) {
        index = 1;
    } else if (bold && !style) {
        index = 2;
    } else if (bold && style == 1) {
        index = 3;
    }
    m_styleComboBox->setCurrentIndex(index);

    m_sizeComboBox = new QComboBox(m_fontGroup);
    m_sizeComboBox->setGeometry(66, 90, 194 + offsetW, 20);
    m_sizeComboBox->setMaxVisibleItems(10);
    for (int index = 6; index <= 72; ++index) {
        m_sizeComboBox->addItem(QString("%1").arg(index));
    }

    m_sizeComboBox->setCurrentIndex(m_sizeComboBox->findText(QString("%1").arg(m_plainTextEdit->font().pointSize())));
    connect(m_sizeComboBox, SIGNAL(activated(QString)), this, SLOT(sizeChanged(QString)));

    m_font = m_plainTextEdit->font();
}

void FontSetupPage::initColorGroup()
{
    m_colorGroup = new QGroupBox(tr("Use Custom Color"), this);
    m_colorGroup->setCheckable(true);
    m_colorGroup->setChecked(INIMANAGER()->isEnableCustomColor());
    m_colorGroup->setGeometry(10, 150, 310, 110);
    connect(m_colorGroup, SIGNAL(clicked(bool)), this, SLOT(colorGroupChecked(bool)));
    m_customColorEnabled = INIMANAGER()->isEnableCustomColor();

    m_foregroudLabel = new QLabel(tr("Foreground"), m_colorGroup);
    m_foregroudLabel->setGeometry(10, 30, 84, 20);

    m_backgroudLabel = new QLabel(tr("Background"), m_colorGroup);
    m_backgroudLabel->setGeometry(152, 30, 84, 20);

    m_highlightLabel = new QLabel(tr("Highlight"), m_colorGroup);
    m_highlightLabel->setGeometry(10, 70, 84, 20);

    m_foregroudColorButton = new QPushButton(m_colorGroup);
    m_foregroudColorButton->setFlat(true);
    m_foregroudColorButton->setAutoFillBackground(true);
    m_foregroudColorButton->setGeometry(94, 24, 30, 30);
    changeButtonColor(m_foregroudColorButton, INIMANAGER()->foregroundColor());
    connect(m_foregroudColorButton, SIGNAL(clicked()), this, SLOT(openForegroundColorDialog()));

    m_backgroudColorButton = new QPushButton(m_colorGroup);
    m_backgroudColorButton->setFlat(true);
    m_backgroudColorButton->setAutoFillBackground(true);
    m_backgroudColorButton->setGeometry(236, 24, 30, 30);
    changeButtonColor(m_backgroudColorButton, INIMANAGER()->backgroundColor());
    connect(m_backgroudColorButton, SIGNAL(clicked()), this, SLOT(openBackgroundColorDialog()));

    m_customBackgroudColorButton = new QPushButton(m_colorGroup);
    m_customBackgroudColorButton->setFlat(true);
    m_customBackgroudColorButton->setAutoFillBackground(true);
    m_customBackgroudColorButton->setGeometry(272, 24, 30, 30);
    changeButtonColor(m_customBackgroudColorButton, INIMANAGER()->customBackgroundColor());
    connect(m_customBackgroudColorButton, SIGNAL(clicked()), this, SLOT(openCustomBackgroundColorDialog()));

    m_currentLineFgColorButton = new QPushButton(m_colorGroup);
    m_currentLineFgColorButton->setFlat(true);
    m_currentLineFgColorButton->setAutoFillBackground(true);
    m_currentLineFgColorButton->setGeometry(94, 64, 30, 30);
    changeButtonColor(m_currentLineFgColorButton, INIMANAGER()->currentLineFgColor());
    connect(m_currentLineFgColorButton, SIGNAL(clicked()), this, SLOT(openCurrentLineFgColorDialog()));

    m_currentLineBgColorButton = new QPushButton(m_colorGroup);
    m_currentLineBgColorButton->setFlat(true);
    m_currentLineBgColorButton->setAutoFillBackground(true);
    m_currentLineBgColorButton->setGeometry(129, 64, 30, 30);
    changeButtonColor(m_currentLineBgColorButton, INIMANAGER()->currentLineBgColor());
    connect(m_currentLineBgColorButton, SIGNAL(clicked()), this, SLOT(openCurrentLineBgColorDialog()));

    m_foregroudColor = INIMANAGER()->foregroundColor();
    m_backgroudColor = INIMANAGER()->backgroundColor();
    m_customBackgroudColor = INIMANAGER()->customBackgroundColor();
    m_currentLineFgColor = INIMANAGER()->currentLineFgColor();
    m_currentLineBgColor = INIMANAGER()->currentLineBgColor();
}

void FontSetupPage::fontGroupChecked(const bool checked)
{
    if (!checked) {
        changeFont(QApplication::font());
        changeFont(QApplication::font().pointSize());
        changeFont("Normal");
    } else {
        changeFont(QFont(m_familyComboBox->currentText()));
        changeFont(QString(m_styleComboBox->currentText()));
        changeFont(QString(m_sizeComboBox->currentText()).toInt());
    }

    m_customFontEnabled = checked;
}

void FontSetupPage::colorGroupChecked(const bool checked)
{
    if (!checked) {
        foregroundColorChanged(INIMANAGER()->defaulForegroundColor());
        backgroudColorChanged(INIMANAGER()->defaulBackgroundColor());
        customBackgroudColorChanged(INIMANAGER()->defaulCustomBackgroundColor());
        currentLineFgColorChanged(INIMANAGER()->defaultCurrentLineFgColor());
        currentLineBgColorChanged(INIMANAGER()->defaultCurrentLineBgColor());
    } else {
        foregroundColorChanged(static_cast<LogViewer*>(m_plainTextEdit)->forgroundColor());
        backgroudColorChanged(static_cast<LogViewer*>(m_plainTextEdit)->backgroundColor());
        customBackgroudColorChanged(static_cast<LogViewer*>(m_plainTextEdit)->customBackgroundColor());
        currentLineFgColorChanged(static_cast<LogViewer*>(m_plainTextEdit)->currentLineFgColor());
        currentLineBgColorChanged(static_cast<LogViewer*>(m_plainTextEdit)->currentLineBgColor());
    }

    m_customColorEnabled = checked;
}

void FontSetupPage::familyChanged(const QFont &font)
{
    changeFont(font);
}

void FontSetupPage::styleChanged(const QString &text)
{
    changeFont(text);
}

void FontSetupPage::sizeChanged(const QString &text)
{
    changeFont(text.toInt());
}

void FontSetupPage::changeFont(const QFont &font)
{
    m_plainTextEdit->moveCursor(QTextCursor::Start);
    m_plainTextEdit->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);

    QFont newFont(font);
    newFont.setBold(m_plainTextEdit->font().bold());
    newFont.setStyle(m_plainTextEdit->font().style());
    newFont.setPointSize(m_plainTextEdit->font().pointSize());
    m_plainTextEdit->setFont(newFont);

    m_plainTextEdit->moveCursor(QTextCursor::Start);

    m_font = font;
}

void FontSetupPage::changeFont(const QString &style)
{
    QFont font = m_plainTextEdit->font();
    bool bold = font.bold();
    QFont::Style italic = font.style();

    if (style == "Italic") {
        bold = false;
        italic = QFont::StyleItalic;
    } else if (style == "Bold") {
        bold = true;
        italic = QFont::StyleNormal;
    } else if (style == "Bold Italic") {
        bold = true;
        italic = QFont::StyleItalic;
    } else if (style == "Normal") {
        bold = false;
        italic = QFont::StyleNormal;
    }

    font.setBold(bold);
    font.setStyle(italic);
    m_plainTextEdit->setFont(font);

    m_font = font;
}

void FontSetupPage::changeFont(const int size)
{
    Q_ASSERT(size > 0);

    QFont font = m_plainTextEdit->font();
    font.setPointSize(size);
    m_plainTextEdit->setFont(font);

    m_font = font;
}

QColorDialog* FontSetupPage::createColorDialog(const QColor &initialColor)
{
    if (!m_colorDialog) {
        m_colorDialog = new QColorDialog(this);
    } else {
        m_colorDialog->disconnect(this);
    }

    m_colorDialog->setCurrentColor(initialColor);

    return m_colorDialog;
}

void FontSetupPage::openForegroundColorDialog()
{
    QColorDialog *dialog = createColorDialog(INIMANAGER()->foregroundColor());

    connect(dialog, SIGNAL(colorSelected(QColor)), this, SLOT(foregroundColorChanged(QColor)));
    dialog->open();
}

void FontSetupPage::openBackgroundColorDialog()
{
    QColorDialog *dialog = createColorDialog(INIMANAGER()->backgroundColor());

    connect(dialog, SIGNAL(colorSelected(QColor)), this, SLOT(backgroudColorChanged(QColor)));
    dialog->open();
}

void FontSetupPage::openCustomBackgroundColorDialog()
{
    QColorDialog *dialog = createColorDialog(INIMANAGER()->customBackgroundColor());

    connect(dialog, SIGNAL(colorSelected(QColor)), this, SLOT(customBackgroudColorChanged(QColor)));
    dialog->open();
}

void FontSetupPage::openCurrentLineFgColorDialog()
{
    QColorDialog *dialog = createColorDialog(INIMANAGER()->currentLineFgColor());

    connect(dialog, SIGNAL(colorSelected(QColor)), this, SLOT(currentLineFgColorChanged(QColor)));
    dialog->open();
}

void FontSetupPage::openCurrentLineBgColorDialog()
{
    QColorDialog *dialog = createColorDialog(INIMANAGER()->currentLineBgColor());

    connect(dialog, SIGNAL(colorSelected(QColor)), this, SLOT(currentLineBgColorChanged(QColor)));
    dialog->open();
}

void FontSetupPage::foregroundColorChanged(const QColor &color)
{
    static_cast<LogViewer*>(m_plainTextEdit)->setForegroundColor(color);
    changeButtonColor(m_foregroudColorButton, color);
    m_foregroudColor = color;
}

void FontSetupPage::backgroudColorChanged(const QColor &color)
{
    static_cast<LogViewer*>(m_plainTextEdit)->setBackgroundColor(color);
    changeButtonColor(m_backgroudColorButton, color);
    m_backgroudColor = color;
}

void FontSetupPage::customBackgroudColorChanged(const QColor &color)
{
    static_cast<LogViewer*>(m_plainTextEdit)->setCustomBackgroundColor(color);
    changeButtonColor(m_customBackgroudColorButton, color);
    m_customBackgroudColor = color;
}

void FontSetupPage::currentLineFgColorChanged(const QColor &color)
{
    static_cast<LogViewer*>(m_plainTextEdit)->setCurrentLineFgColor(color);
    changeButtonColor(m_currentLineFgColorButton, color);
    m_currentLineFgColor = color;
}

void FontSetupPage::currentLineBgColorChanged(const QColor &color)
{
    static_cast<LogViewer*>(m_plainTextEdit)->setCurrentLineBgColor(color);
    changeButtonColor(m_currentLineBgColorButton, color);
    m_currentLineBgColor = color;
}

void FontSetupPage::optionApplied()
{
    INIMANAGER()->enableCustomFont(m_customFontEnabled);
    INIMANAGER()->enableCustomColor(m_customColorEnabled);
    INIMANAGER()->setFont(m_font);
    INIMANAGER()->setForegroundColor(m_foregroudColor);
    INIMANAGER()->setBackgroundColor(m_backgroudColor);
    INIMANAGER()->setCustomBackgroundColor(m_customBackgroudColor);
    INIMANAGER()->setCurrentLineFgColor(m_currentLineFgColor);
    INIMANAGER()->setCurrentLineBgColor(m_currentLineBgColor);
}

void FontSetupPage::changeButtonColor(QPushButton* button, const QColor color)
{
    QString backgroundColor = color.name();
    QString styleSheet = QString("QPushButton { border: 1px solid #8f8f91; background-color: %1; } ").arg(backgroundColor);
    button->setStyleSheet(styleSheet);
}
