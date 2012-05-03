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

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QtGui/QStatusBar>
#include <QtGui/QComboBox>

class QStatusBar;
class QLabel;

class StatusBar : public QStatusBar
{
    Q_OBJECT

signals:
    void textEncodingChanged(const QString& encoding);

public:
    StatusBar(QWidget* parent = 0);
    virtual ~StatusBar();

    void setFileSize(long fileSize);
    void setSelectedCount(int count);
    void setTotalLine(int line);
    void setLineColumn(int line, int column);
    void setTextCodec(const QString& name);

private:
    void init();

private:
    QLabel* m_lineColumn;
    QLabel* m_selectedCountLabel;
    QLabel* m_totalLine;
    QLabel* m_fileSizeLable;
    QComboBox* m_encodingComboBox;
};

#endif // STATUSBAR_H
