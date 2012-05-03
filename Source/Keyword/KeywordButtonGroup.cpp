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

#include "KeywordButtonGroup.h"
#include "Source/Setting/INIManager.h"

#include <QtGui/QButtonGroup>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>

#if DEBUG
#include <QtCore/QDebug>
#endif

KeywordButtonGroup::KeywordButtonGroup(QWidget* parent)
    : QWidget(parent)
    , m_buttonGroup(new QButtonGroup(this))
    , m_horizontalLayout(new QHBoxLayout(this))
    , m_currentViewer(0)
{
    init();
}

KeywordButtonGroup::~KeywordButtonGroup()
{
    delete m_buttonGroup;
    delete m_horizontalLayout;
}

void KeywordButtonGroup::init()
{
    QStringList keywordList(INIMANAGER()->highlightingKeywordList());
    foreach (const QString& keyword, keywordList) {
        addButton(keyword);
    }

    m_buttonGroup->setExclusive(false);
    connect(m_buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));

    m_horizontalLayout->setSpacing(0);
    m_horizontalLayout->setMargin(0);
    setLayout(m_horizontalLayout);
}

void KeywordButtonGroup::removeStateAll()
{
    m_checkStateHash.clear();
}

void KeywordButtonGroup::removeStateOthers(const QWidget* viewer)
{
    foreach (QWidget* key, m_checkStateHash.keys()) {
        if (key != viewer)
            removeState(key);
    }
}

void KeywordButtonGroup::removeState(const QWidget* viewer)
{
    m_checkStateHash.remove(const_cast<QWidget*>(viewer));

    if (m_currentViewer == viewer)
        m_currentViewer = 0;
}

void KeywordButtonGroup::keywordStateChanged(QHash<QString, bool>& keywordStateHash)
{
    QStringList addedKeywordList;
    QStringList removedKeywordList;

    // 삭제 삽입 정보 갱신.
    foreach (const QString& keyword, keywordStateHash.keys()) {
#if DEBUG
        qDebug() << "[keywordStateChanged]" << keyword << ": " << keywordStateHash.value(keyword);
#endif

        if (keywordStateHash.value(keyword)) {
            addButton(keyword);
            addedKeywordList << keyword;

#if DEBUG
            qDebug() <<"[Add Button] " << keyword << " [Button Group Count] " << m_buttonGroup->buttons().count();
#endif
        } else {
            QAbstractButton* abstractButton = button(keyword);
            removedKeywordList << abstractButton->text();
            delete m_horizontalLayout->takeAt(m_horizontalLayout->indexOf(abstractButton));
            m_buttonGroup->removeButton(abstractButton);

#if DEBUG
            qDebug() <<"[Delete Button] " << keyword << " [Button Group Count] " << m_buttonGroup->buttons().count();
#endif
        }               
    }    

    reloadButtonColor();

    if (!addedKeywordList.isEmpty())
        emit buttonAdded(addedKeywordList);

    if (!removedKeywordList.isEmpty())
        emit buttonRemoved(removedKeywordList);
}

void KeywordButtonGroup::keywordButtonReload()
{
    reloadButtonColor();
    emit buttonReloaded();
}

void KeywordButtonGroup::tabChanged(const QWidget* viewer)
{
    initChecked(false);

    if (!viewer)
        return;

    m_currentViewer = const_cast<QWidget*>(viewer);
    QStringList checkedButtonList = m_checkStateHash.value(m_currentViewer);

    // Recover the check state.
    foreach (const QString& buttonText, checkedButtonList) {
        button(buttonText)->setChecked(true);
    }
}

void KeywordButtonGroup::buttonClicked(QAbstractButton* button)
{
    Q_UNUSED(button);

    QStringList checkedButtonList;

    QList<QAbstractButton*> buttonList = m_buttonGroup->buttons();
    foreach (const QAbstractButton* button, buttonList) {
        if (button->isChecked()) {
            checkedButtonList << button->text();
        }
    }

#if DEBUG
    qDebug() << "[Checked] " << checkedButtonList;
#endif
    m_checkStateHash.insert(m_currentViewer, checkedButtonList);

    if (m_currentViewer)
        emit buttonClicked(m_currentViewer, m_buttonGroup);
}

void KeywordButtonGroup::initChecked(bool check)
{
    QList<QAbstractButton*> buttonList = m_buttonGroup->buttons();
    foreach (QAbstractButton* button, buttonList) {
        if (button->isChecked())
            button->setChecked(check);
    }
}

void KeywordButtonGroup::addButton(const QString& keyword)
{
    QPushButton* pushButton = new QPushButton(keyword);
    pushButton->setCheckable(true);
    pushButton->setAutoFillBackground(true);

#ifdef Q_OS_WIN32
    QFontMetrics fontMetrics(pushButton->font());
    pushButton->setFixedWidth(fontMetrics.width(keyword) + 10);
#endif

    QString foregroundColor = INIMANAGER()->foregroundColor(keyword).name();
    QString backgroundColor = INIMANAGER()->backgroundColor(keyword).name();
    QString styleSheet = QString("QPushButton { color: %1; background-color: %2; } QPushButton::checked { color: black; background-color: gray; }").arg(foregroundColor).arg(backgroundColor);
    pushButton->setStyleSheet(styleSheet);

    m_horizontalLayout->addWidget(pushButton);
    m_buttonGroup->addButton(pushButton);
}

QAbstractButton* KeywordButtonGroup::button(const QString& keyword)
{
    QList<QAbstractButton*> buttonList = m_buttonGroup->buttons();
    foreach (QAbstractButton* button, buttonList) {
        if (button->text() == keyword)
            return button;
    }

    return 0;
}

void KeywordButtonGroup::reloadButtonColor()
{
    QList<QAbstractButton*> buttonList = m_buttonGroup->buttons();
    foreach (QAbstractButton* button, buttonList) {
        QString foregroundColor = INIMANAGER()->foregroundColor(button->text()).name();
        QString backgroundColor = INIMANAGER()->backgroundColor(button->text()).name();
        QString styleSheet = QString("QPushButton { color: %1; background-color: %2; } QPushButton::checked { color: black; background-color: gray; }").arg(foregroundColor).arg(backgroundColor);
        button->setStyleSheet(styleSheet);
    }
}
