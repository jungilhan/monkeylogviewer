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

#include "HighlightSetupPage.h"
#include "Source/TypeDef.h"
#include "Source/Setting/INIManager.h"
#include "Source/Keyword/SearchLineEdit.h"

#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QTextCharFormat>
#include <QColorDialog>

HighlightSetupPage::HighlightSetupPage(QWidget* parent)
    : QWidget(parent)
    , m_highlightTextGroupBox(0)
    , m_textListWidget(0)
    , m_colorNStyleGroupBox(0)
    , m_textLabel(0)
    , m_backgroudLabel(0)
    , m_styleLabel(0)
    , m_backgroundColorButton(0)
    , m_styleComboBox(0)
    , m_addNRemoveGroupBox(0)
    , m_lineEdit(0)
    , m_addButton(0)
    , m_removeButton(0)
    , m_colorDialog(0)
    , m_colorChanged(false)
    , m_optionChanged(false)
    , m_caseSensitive(INIMANAGER()->isEnabledHighlightCaseSensitive())
    , m_wholeWords(INIMANAGER()->isEnabledHighlightWholeWords())
{
    initHighlightTextGroup();
    initColorNStyleWidgetGroup();
    initAddNRemoveWidgetGroup();
}

HighlightSetupPage::~HighlightSetupPage()
{
    delete m_textListWidget;
    delete m_highlightTextGroupBox;

    delete m_textLabel;
    delete m_backgroudLabel;
    delete m_styleLabel;
    delete m_backgroundColorButton;
    delete m_styleComboBox;
    delete m_colorNStyleGroupBox;

    delete m_lineEdit;
    delete m_addButton;
    delete m_removeButton;
    delete m_addNRemoveGroupBox;

    delete m_colorDialog;

    // TODO 해시 삭제.
}

void HighlightSetupPage::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    QString text = m_textLabel->text();
    paintColor(INIMANAGER()->foregroundColor(text), INIMANAGER()->backgroundColor(text));

    if (m_textListWidget->count() > 0) {
        m_textListWidget->setFocus();
    } else {
        m_lineEdit->setFocus();
    }
}

void HighlightSetupPage::initHighlightTextGroup()
{
    const int offsetW = 90;
    const int offsetH = 24;
#ifdef Q_OS_LINUX
    const int groupBoxTextMarginTop = 4;
#elif defined(Q_OS_WIN32)
    const int groupBoxTextMarginTop = 0;
#endif

    m_highlightTextGroupBox = new QGroupBox(tr("Highlight Text"), this);
    m_highlightTextGroupBox->setGeometry(10, 10, 210 + offsetW, 160 + offsetH);

    QStringList m_highlightingTextList = INIMANAGER()->highlightingKeywordList();
    m_textListWidget = new QListWidget(m_highlightTextGroupBox);    
    m_textListWidget->setGeometry(10, 20 + groupBoxTextMarginTop, 190 + offsetW, 124 + offsetH - groupBoxTextMarginTop);
    m_textListWidget->addItems(m_highlightingTextList);
    connect(m_textListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(textListItemChanged(QListWidgetItem*, QListWidgetItem*)));

    loadHighlightingRules();
}

void HighlightSetupPage::initColorNStyleWidgetGroup()
{
#ifdef Q_OS_LINUX
    const int groupBoxTextMarginTop = 4;
#elif defined(Q_OS_WIN32)
    const int groupBoxTextMarginTop = 0;
#endif

    m_colorNStyleGroupBox = new QGroupBox(tr("Color && Style"), this);
    m_colorNStyleGroupBox->setGeometry(320, 10, 144, 184);

    m_textLabel = new QLabel(m_colorNStyleGroupBox);
    m_textLabel->setGeometry(10, 20 + groupBoxTextMarginTop, 120, 20);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setAutoFillBackground(true);
    m_textLabel->setFrameStyle(QFrame::Box);

    m_backgroudLabel = new QLabel(m_colorNStyleGroupBox);
    m_backgroudLabel->setGeometry(10, 57 + groupBoxTextMarginTop, 84, 20);
    m_backgroudLabel->setText(tr("Line Color"));

    m_styleLabel = new QLabel(m_colorNStyleGroupBox);
    m_styleLabel->setGeometry(10, 90 + groupBoxTextMarginTop, 40, 20);
    m_styleLabel->setText(tr("Style"));

    m_backgroundColorButton = new QPushButton(m_colorNStyleGroupBox);
    m_backgroundColorButton->setGeometry(99, 50 + groupBoxTextMarginTop, 30, 30);
    m_backgroundColorButton->setFlat(true);
    m_backgroundColorButton->setAutoFillBackground(true);
    connect(m_backgroundColorButton, SIGNAL(clicked()), this, SLOT(openBackgroundColorDialog()));

    m_styleComboBox = new QComboBox(m_colorNStyleGroupBox);
    m_styleComboBox->setGeometry(50, 90 + groupBoxTextMarginTop, 80, 22);
    m_styleComboBox->addItem(tr("Normal"));
    //m_styleComboBox->addItem(tr("Italic"));
    //m_styleComboBox->addItem(tr("Bold"));
    //m_styleComboBox->addItem(tr("Bold Italic"));
    //m_styleComboBox->setCurrentIndex(m_styleComboBox->findText("Bold"));
    connect(m_styleComboBox, SIGNAL(activated(QString)), this, SLOT(styleChanged(QString)));

    paintColor(INIMANAGER()->foregroundColor(), INIMANAGER()->backgroundColor());
}

void HighlightSetupPage::initAddNRemoveWidgetGroup()
{
    const int offsetW = 90;
#ifdef Q_OS_LINUX
    const int groupBoxTextMarginTop = 4;
#elif defined(Q_OS_WIN32)
    const int groupBoxTextMarginTop = 0;
#endif

    m_addNRemoveGroupBox = new QGroupBox(tr("Add && Remove"), this);
    m_addNRemoveGroupBox->setGeometry(10, 204, 360 + offsetW, 60 + groupBoxTextMarginTop);

    m_lineEdit = new SearchLineEdit(INIMANAGER()->isEnabledHighlightCaseSensitive(),
                                    INIMANAGER()->isEnabledHighlightWholeWords(),
                                    true,
                                    m_addNRemoveGroupBox);
    m_lineEdit->setGeometry(9, 21 + groupBoxTextMarginTop, 180 + offsetW, 26);
    connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(addClicked()));
    connect(m_lineEdit, SIGNAL(caseSensitiveToggled(bool)), this, SLOT(caseSensitiveToggled(bool)));
    connect(m_lineEdit, SIGNAL(wholeWordsToggled(bool)), this, SLOT(wholeWordsToggled(bool)));

    m_addButton = new QPushButton(tr("Add"), m_addNRemoveGroupBox);
    m_addButton->setGeometry(200 + offsetW, 20 + groupBoxTextMarginTop, 70, 26);
    connect(m_addButton, SIGNAL(clicked()), this, SLOT(addClicked()));

    m_removeButton = new QPushButton(tr("Remove"), m_addNRemoveGroupBox);
    m_removeButton->setGeometry(276 + offsetW, 20 + groupBoxTextMarginTop, 74, 26);
    connect(m_removeButton, SIGNAL(clicked()), this, SLOT(removeClicked()));
}

QColorDialog* HighlightSetupPage::createColorDialog(const QColor& initialColor)
{
    if (!m_colorDialog) {
        m_colorDialog = new QColorDialog(this);
    } else {
        m_colorDialog->disconnect(this);
    }

    m_colorDialog->setCurrentColor(initialColor);

    return m_colorDialog;
}

void HighlightSetupPage::paintColor(const QColor& foreground, const QColor& background)
{
    Q_ASSERT(m_textLabel && m_backgroundColorButton);

    QPalette palette;
    palette.setColor(QPalette::WindowText, foreground);
    palette.setColor(QPalette::Window, background);
    m_textLabel->setPalette(palette);

    changeButtonColor(m_backgroundColorButton, background);
}

void HighlightSetupPage::loadHighlightingRules()
{
    QStringList highlightingKeywordList = INIMANAGER()->highlightingKeywordList();

    foreach (const QString &keyword, highlightingKeywordList) {
        QColor foregroundColor = INIMANAGER()->foregroundColor(keyword);
        QColor backgroundColor = INIMANAGER()->backgroundColor(keyword);

        QTextCharFormat textCharFormat;
        textCharFormat.setForeground(foregroundColor);
        textCharFormat.setBackground(backgroundColor);

        m_highlightingRules.insert(keyword, textCharFormat);
    }
}

void HighlightSetupPage::highlightingRuleChanged()
{
    // Remove the keyword.
    INIMANAGER()->removeHighlightingRulesGroup();

    // Rewrite the keyword.
    QStringList highlightingKeywordList;
    foreach (const QString &keyword, m_highlightingRules.keys()) {
        highlightingKeywordList << keyword;

        QTextCharFormat textCharFormat = m_highlightingRules.value(keyword);
        INIMANAGER()->setForegroundColor(keyword, textCharFormat.foreground().color());
        INIMANAGER()->setBackgroundColor(keyword, textCharFormat.background().color());
    }

    INIMANAGER()->setHighlightingKeywordList(highlightingKeywordList);

    if (m_keywordStateHash.count() > 0) {
        emit keywordStateChanged(m_keywordStateHash);
        m_keywordStateHash.clear();
    }

    if (m_optionChanged) {
        INIMANAGER()->setHighlightCaseSensitive(m_caseSensitive);
        INIMANAGER()->setHighlightWholeWords(m_wholeWords);
        emit highlightOptionChanged();
    }

    if (m_colorChanged || m_optionChanged)
        emit keywordColorChanged();
}

void HighlightSetupPage::addClicked()
{
    QString text = m_lineEdit->text();
    if (text.isEmpty())
        return ;

    m_lineEdit->selectAll();

    QList<QListWidgetItem *> items = m_textListWidget->findItems(text, INIMANAGER()->isEnabledHighlightCaseSensitive() ? Qt::MatchCaseSensitive : Qt::MatchFixedString);
    if (items.count() > 0) {
        m_textListWidget->setCurrentItem(items.first());
        return ;
    }

    m_textListWidget->addItem(text);
    m_textListWidget->setCurrentRow(m_textListWidget->count() - 1);

    QList<QString> keys = m_highlightingRules.keys();
    if (keys.indexOf(text) == -1) {
        QColor defaultForegroundColor = INIMANAGER()->foregroundColor();
        QColor defaultBackgroundColor = INIMANAGER()->backgroundColor();

        QTextCharFormat textCharFormat;
        textCharFormat.setForeground(defaultForegroundColor);
        textCharFormat.setBackground(defaultBackgroundColor);
        m_highlightingRules.insert(text, textCharFormat);

        paintColor(defaultForegroundColor, defaultBackgroundColor);

        // keyword state not changed (case. remove and then insert)
        if (m_keywordStateHash.contains(text) && !m_keywordStateHash.value(text)) {
            m_keywordStateHash.remove(text);
        } else {
            m_keywordStateHash.insert(text, KEYWORD_ADDED);
        }
    }
}

void HighlightSetupPage::removeClicked()
{
    QListWidgetItem* item = m_textListWidget->takeItem(m_textListWidget->currentRow());
    if (!item)
        return;

    // keyword state not changed (case. insert and then remove)
    if (m_keywordStateHash.contains(item->text())) {
        m_keywordStateHash.remove(item->text());
    } else {
        m_keywordStateHash.insert(item->text(), KEYWORD_REMOVED);
    }

    m_highlightingRules.remove(item->text());
    m_textListWidget->removeItemWidget(item);
    delete item;
}

void HighlightSetupPage::textListItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    Q_UNUSED(previous);

    QString text;
    if (current)
        text = current->text();

    QFont font;
    QFontMetrics fontMetrics(font);
    QString elidedText = fontMetrics.elidedText(text, Qt::ElideRight, m_textLabel->width() - 8);
    m_textLabel->setText(elidedText);

    QTextCharFormat textFormat = m_highlightingRules.value(text);
    paintColor(textFormat.foreground().color(), textFormat.background().color());
}

void HighlightSetupPage::openBackgroundColorDialog()
{
    if (m_textLabel->text().isEmpty())
        return ;

    QColorDialog *dialog = createColorDialog(m_backgroundColorButton->palette().button().color());

    connect(dialog, SIGNAL(colorSelected(QColor)), this, SLOT(backgroudColorChanged(QColor)));
    connect(dialog, SIGNAL(colorSelected(QColor)), this, SLOT(foregroudColorChanged(QColor)));
    dialog->open();
}

void HighlightSetupPage::foregroudColorChanged(const QColor& color)
{
    Q_ASSERT(!m_textLabel->text().isEmpty());    

    QString text = m_textListWidget->currentItem()->text();
    QTextCharFormat textFormat = m_highlightingRules.value(text);
    //textFormat.setForeground(color);
    textFormat.setForeground(QColor(255, 255, 255));
    m_highlightingRules.insert(text, textFormat);

    paintColor(color, textFormat.background().color());

    m_colorChanged = true;
}

void HighlightSetupPage::backgroudColorChanged(const QColor& color)
{
    Q_ASSERT(!m_textLabel->text().isEmpty());

    QString text = m_textListWidget->currentItem()->text();
    QTextCharFormat textFormat = m_highlightingRules.value(text);
    textFormat.setBackground(color);
    m_highlightingRules.insert(text, textFormat);

    paintColor(textFormat.foreground().color(), color);

    m_colorChanged = true;
}

void HighlightSetupPage::styleChanged(const QString& style)
{
    Q_UNUSED(style);
}

void HighlightSetupPage::changeButtonColor(QPushButton* button, const QColor color)
{
    QString backgroundColor = color.name();
    QString styleSheet = QString("QPushButton { border: 1px solid #8f8f91; background-color: %1; } ").arg(backgroundColor);
    button->setStyleSheet(styleSheet);
}

void HighlightSetupPage::caseSensitiveToggled(bool checked)
{
    m_optionChanged = true;
    m_caseSensitive = checked;
}

void HighlightSetupPage::wholeWordsToggled(bool checked)
{
    m_optionChanged = true;
    m_wholeWords = checked;
}
