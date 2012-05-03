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

#ifndef FONTSETUPPAGE_H
#define FONTSETUPPAGE_H

#include <QWidget>

class QLabel;
class QPushButton;
class QGroupBox;
class QFontComboBox;
class QComboBox;
class QPlainTextEdit;
class QColorDialog;

class FontSetupPage : public QWidget
{
    Q_OBJECT

signals:
    void fontChanged(const QFont &font);

public:
    FontSetupPage(QWidget *parent = 0);
    virtual ~FontSetupPage();

public slots:
    void optionApplied();

private slots:
    void fontGroupChecked(const bool checked);
    void colorGroupChecked(const bool checked);

    void familyChanged(const QFont &font);
    void styleChanged(const QString &text);
    void sizeChanged(const QString &text);

    void openForegroundColorDialog();
    void openBackgroundColorDialog();
    void openCustomBackgroundColorDialog();
    void openCurrentLineFgColorDialog();
    void openCurrentLineBgColorDialog();

    void foregroundColorChanged(const QColor &color);
    void backgroudColorChanged(const QColor &color);
    void customBackgroudColorChanged(const QColor &color);
    void currentLineFgColorChanged(const QColor &color);
    void currentLineBgColorChanged(const QColor &color);

private:
    void initFontGroup();
    void initColorGroup();
    void changeFont(const QFont &font);
    void changeFont(const QString &style);
    void changeFont(const int size);
    QColorDialog* createColorDialog(const QColor &initialColor);

    void changeButtonColor(QPushButton* button, const QColor color);

private:
    QGroupBox *m_fontGroup;
    QLabel *m_familyLabel;
    QLabel *m_styleLabel;
    QLabel *m_sizeLabel;
    QFontComboBox *m_familyComboBox;
    QComboBox *m_styleComboBox;
    QComboBox *m_sizeComboBox;

    QGroupBox *m_colorGroup;
    QLabel *m_foregroudLabel;
    QLabel *m_backgroudLabel;
    QLabel *m_highlightLabel;

    QPushButton *m_foregroudColorButton;
    QPushButton *m_backgroudColorButton;
    QPushButton *m_customBackgroudColorButton;
    QPushButton *m_currentLineFgColorButton;
    QPushButton *m_currentLineBgColorButton;
    QColorDialog *m_colorDialog;

    QPlainTextEdit *m_plainTextEdit;

    QColor m_foregroudColor;
    QColor m_backgroudColor;
    QColor m_customBackgroudColor;
    QColor m_currentLineFgColor;
    QColor m_currentLineBgColor;
    QFont m_font;
    bool m_customFontEnabled;
    bool m_customColorEnabled;
};

#endif // FONTSETUPPAGE_H
