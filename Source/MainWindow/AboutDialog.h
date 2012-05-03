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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui/QDialog>

class QLabel;
class QPushButton;

class AboutDialog : public QDialog
{
public:
    AboutDialog(QWidget* parent = 0);
    virtual ~AboutDialog();

private:
    void init();

private:
    QLabel* m_logoLabel;
    QLabel* m_logoBgLabel;
    QLabel* m_titleLabel;
    QLabel* m_textLabel;
    QLabel* m_progectHomeLabel;
    QLabel* m_blogLabel;
    QPushButton* m_okButton;
};

#endif // ABOUTDIALOG_H
