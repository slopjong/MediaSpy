/*
 * Copyright (C) 2009 Stéphane Péchard <stephanepechard@gmail.com>
 *
 * This file is part of MediaSpy.
 *
 * MediaSpy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MediaSpy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MediaSpy.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef MYQLINEEDIT_H
#define MYQLINEEDIT_H

#include <QLineEdit>

class QToolButton;


/** \class myQLineEdit
  * \brief This line edit presents a clear button at the end of the line.
  * The button is only visible when text is in the line edit.
  */
class myQLineEdit : public QLineEdit {
    Q_OBJECT;

public:
    explicit myQLineEdit(QWidget *parent = 0);
    virtual ~myQLineEdit();


protected:
    void resizeEvent(QResizeEvent *);


private slots:
    void updateCloseButton(const QString &text);


private:
    QToolButton *clearButton;

};

#endif // MYQLINEEDIT_H
