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

#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QLabel>

class QMovie;

class LoadingWidget : public QLabel
{
public:
    LoadingWidget(QWidget* parent = 0);
    virtual ~LoadingWidget();

    void start(const QString& fileName);
    void stop();

    void setDefaultImage(const QPixmap &pixmap);

private:
    QMovie* m_movie;
    QPixmap m_defaultPixmap;
};

#endif // LOADINGWIDGET_H
