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


#ifndef COLLECTIONDIALOG_H
#define COLLECTIONDIALOG_H

#include <QDir>
#include <QFileDialog>

#include <QDialog>

#include "databasemanager.h"

namespace Ui {
    class CollectionDialog;
}

/** \class CollectionDialog
  * \brief The CollectionDialog is the dialog used to add directories in the Collection.
  */
class CollectionDialog : public QDialog {
    Q_OBJECT

    // Fields
    Ui::CollectionDialog *ui_;
    QSqlTableModel* model_;


public:
    // Constructors
    explicit CollectionDialog(QSqlTableModel*, QWidget *parent = 0);
    virtual ~CollectionDialog();


private slots:
    void on_delDirButton_clicked();
    void on_addDirButton_clicked();
    void enableRemoveDirButton();
};

#endif // COLLECTIONDIALOG_H
