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

#ifndef KEYWORDBUTTONGROUP_H
#define KEYWORDBUTTONGROUP_H

#include <QWidget>
#include <QtCore/QHash>

class QButtonGroup;
class QAbstractButton;
class QPushButton;
class QHBoxLayout;

class KeywordButtonGroup : public QWidget
{
    Q_OBJECT

signals:
    void buttonClicked(QWidget* ownerViewer, QButtonGroup* buttonGroup);
    void buttonAdded(QStringList& keywordList);
    void buttonRemoved(QStringList& keywordList);
    void buttonReloaded();

public:
    KeywordButtonGroup(QWidget* parent = 0);
    virtual ~KeywordButtonGroup();

    void removeStateAll();
    void removeStateOthers(const QWidget* viewer);
    void removeState(const QWidget* viewer);

    void tabChanged(const QWidget* viewer);

public slots:
    void keywordStateChanged(QHash<QString, bool>& keywordStateHash);
    void keywordButtonReload();

private:
    void init();
    void initChecked(bool check);
    void addButton(const QString& keyword);
    void reloadButtonColor();

    QAbstractButton* button(const QString& keyword);

private slots:
    void buttonClicked(QAbstractButton* button);

private:
    QHash<QWidget*, QStringList> m_checkStateHash;
    QButtonGroup* m_buttonGroup;
    QHBoxLayout* m_horizontalLayout;
    QWidget* m_currentViewer;
};

#endif // KEYWORDBUTTONGROUP_H
