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

#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include "Source/LogViewer/LogViewer.h"
#include <QWidget>

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(LogViewer* viewer) : QWidget(viewer) {
        m_viewer = viewer;
    }

    QSize sizeHint() const {
        return QSize(m_viewer->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        m_viewer->lineNumberAreaPaintEvent(event);
    }

private:
    LogViewer* m_viewer;
};

#endif // LINENUMBERAREA_H
