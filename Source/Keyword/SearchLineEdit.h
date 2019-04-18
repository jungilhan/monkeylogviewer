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

#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include <QLineEdit>

class QToolButton;
class QMenu;
class QAction;

class SearchLineEdit : public QLineEdit
{
    Q_OBJECT

signals:
    void caseSensitiveToggled(bool checked);
    void wholeWordsToggled(bool checked);
    void regularExpressionsToggled(bool checked);

public:
    SearchLineEdit(bool enableCaseSensitive, bool enableWholeWords, bool enableRegularExpressions, QWidget* parent = 0);
    virtual ~SearchLineEdit();

    void setInactiveText(const QString& text);

protected:
    virtual void focusInEvent(QFocusEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

private slots:
    void textEdited(QString text);
    void showOptionMenuTriggered();

private:
    void init(bool enableCaseSensitive, bool enableWholeWords, bool enableRegularExpressions);
    QMenu* createOptionMenu(bool enableCaseSensitive, bool enableWholeWords, bool enableRegularExpressions);

private:
    QToolButton* m_searchButton;
    QMenu* m_optionMenu;
    QAction* m_caseSensitive;
    QAction* m_wholeWords;
    QAction* m_regularExpressions;
    QString m_inactiveText;
};

#endif // SEARCHLINEEDIT_H
