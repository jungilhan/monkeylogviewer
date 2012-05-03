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

#ifndef GOTOLINEDIALOG_H
#define GOTOLINEDIALOG_H

#include <QtGui/QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class GotoLineDialog : public QDialog
{
    Q_OBJECT

signals:
    void gotoLine(int line);

public:
    GotoLineDialog(QWidget* parent = 0);
    virtual ~GotoLineDialog();

private:
    void init();

private slots:
    void gotoLine();

private:
    QLabel* m_label;
    QLineEdit* m_lineEdit;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
};

#endif // GOTOLINEDIALOG_H
