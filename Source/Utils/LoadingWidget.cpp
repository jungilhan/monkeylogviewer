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

#include "LoadingWidget.h"

#include <QMovie>

LoadingWidget::LoadingWidget(QWidget *parent)
    : QLabel(parent)
    , m_movie(0)
{

}

LoadingWidget::~LoadingWidget()
{
    if (m_movie && m_movie->state() == QMovie::Running)
        m_movie->stop();

    delete m_movie;
}

void LoadingWidget::setDefaultImage(const QPixmap &pixmap)
{
    m_defaultPixmap = pixmap;
    setPixmap(m_defaultPixmap);
}

void LoadingWidget::start(const QString &fileName)
{
    Q_ASSERT(!fileName.isEmpty());

    show();

    if (!m_movie)
        m_movie = new QMovie(fileName, "gif", this);
        setMovie(m_movie);

    if (m_movie->isValid())
        m_movie->start();
}

void LoadingWidget::stop()
{
    if (m_movie)
        m_movie->stop();

    if (m_defaultPixmap.isNull())
        hide();
    else
        setPixmap(m_defaultPixmap);
}
