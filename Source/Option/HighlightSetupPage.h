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

#ifndef HIGHLIGHTSETUPPAGE_H
#define HIGHLIGHTSETUPPAGE_H

#include <QWidget>
#include <QtCore/QHash>

class QLabel;
class QPushButton;
class QListWidget;
class QComboBox;
class QGroupBox;
class QListWidgetItem;
class QTextCharFormat;
class QColorDialog;
class SearchLineEdit;

class HighlightSetupPage : public QWidget
{
    Q_OBJECT

signals:
    void highlightOptionChanged();
    void keywordStateChanged(QHash<QString, bool>&);
    void keywordColorChanged();

public:
    HighlightSetupPage(QWidget* parent = 0);
    virtual ~HighlightSetupPage();

public slots:
    void highlightingRuleChanged();

protected:
    virtual void showEvent(QShowEvent* event);

private:
    void initHighlightTextGroup();
    void initColorNStyleWidgetGroup();
    void initAddNRemoveWidgetGroup();
    QColorDialog* createColorDialog(const QColor& initialColor);
    void paintColor(const QColor& foreground, const QColor& background);
    void loadHighlightingRules();
    void changeButtonColor(QPushButton* button, const QColor color);

private slots:
    void addClicked();
    void removeClicked();
    void textListItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void openBackgroundColorDialog();
    void foregroudColorChanged(const QColor& color);
    void backgroudColorChanged(const QColor& color);
    void styleChanged(const QString& style);
    void caseSensitiveToggled(bool checked);
    void wholeWordsToggled(bool checked);

private:
    QGroupBox* m_highlightTextGroupBox;
    QListWidget* m_textListWidget;

    QGroupBox* m_colorNStyleGroupBox;
    QLabel* m_textLabel;
    QLabel* m_backgroudLabel;
    QLabel* m_styleLabel;
    QPushButton* m_foregroundColorButton;
    QPushButton* m_backgroundColorButton;
    QComboBox* m_styleComboBox;

    QGroupBox* m_addNRemoveGroupBox;
    SearchLineEdit* m_lineEdit;
    QPushButton* m_addButton;
    QPushButton* m_removeButton;

    QColorDialog* m_colorDialog;

    QHash<QString, QTextCharFormat> m_highlightingRules;
    QHash<QString, bool> m_keywordStateHash;
    bool m_colorChanged;
    bool m_optionChanged;
    bool m_caseSensitive;
    bool m_wholeWords;
};

#endif // HIGHLIGHTSETUPPAGE_H
