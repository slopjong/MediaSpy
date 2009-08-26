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


#ifndef EDITMEDIADIALOG_H
#define EDITMEDIADIALOG_H

#include <QtGui/QDialog>
#include <QDataWidgetMapper>

#include "../build/ui/ui_editmediadialog.h"
#include "databasemanager.h"

namespace Ui {
    class EditMediaDialog;
}

namespace tabPage { enum e { info, imdb }; }

class EditMediaDialog : public QDialog {
    Q_OBJECT

    // Fields
    Ui::EditMediaDialog *ui_;
    QDataWidgetMapper* mapper_;
    QModelIndexList indexList_;
    int nMedia_;
    int selectionPos_;
    int originPos_;

    // operators
    void init();
    void makeConnections();
    void setSeenCheckBoxState();
    void setTagLineEdit();


public:
    EditMediaDialog(QModelIndexList, QDataWidgetMapper*);
    virtual ~EditMediaDialog();


private slots:
    void on_parMediaCheckBox_clicked(bool checked);
    void toNext();
    void toPrevious();

};

#endif // EDITMEDIADIALOG_H


