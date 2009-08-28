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
#include <QStatusBar>

#include "databasemanager.h"

namespace Ui {
    class EditMediaDialog;
}

namespace tabPage { enum e { info, imdb }; }

/** \class EditMediaDialog
  * \brief The EditMediaDialog is the dialog used to edit the medias details.
  */
class EditMediaDialog : public QDialog {
    Q_OBJECT

    // Fields
    Ui::EditMediaDialog *ui_;
    QDataWidgetMapper* mapper_;
    QModelIndexList indexList_;
    int nMedia_;
    int selectionPos_;
    int originPos_;
    QStringList* tagsSet_;
    QStringList* tagsUnset_;
    QStatusBar* statusBar_;

    // operators
    void init();
    void makeConnections();
    void setTagsInfo();
    void updateComboBox();


public:
    EditMediaDialog(QModelIndexList, QDataWidgetMapper*, QWidget *parent = 0);
    virtual ~EditMediaDialog();


private slots:
    void on_seenCheckBox_clicked();
    void on_minusToolButton_clicked();
    void on_plusToolButton_clicked();
    void toNext();
    void toPrevious();

};

#endif // EDITMEDIADIALOG_H


