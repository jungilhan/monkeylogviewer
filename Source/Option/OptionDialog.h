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

#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

class QTabWidget;
class QPushButton;

class OptionDialog : public QDialog
{
    Q_OBJECT

signals:
    void highlightOptionChanged();
    void keywordStateChanged(QHash<QString, bool>&);
    void keywordColorChanged();

public:
    OptionDialog(QWidget* parent = 0);
    virtual ~OptionDialog();

private:
    void init();
    QWidget* createFontSetupPage();
    QWidget* createHighlightSetupPage();

private:
    QTabWidget* m_tabWidget;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
};

#endif // OPTIONDIALOG_H
