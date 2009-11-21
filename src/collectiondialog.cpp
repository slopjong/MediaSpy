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


#include "collectiondialog.h"
#include "ui_collectiondialog.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn CollectionDialog::CollectionDialog(QWidget *parent)
  * \brief class constructor
  * \param parent the inherited QWidget object
  */
CollectionDialog::CollectionDialog(QSqlTableModel* model, QWidget *parent)
        : QDialog(parent)
        , ui_(new Ui::CollectionDialog)
        , model_(model)
{
    ui_->setupUi(this);
    ui_->listView->setModel(model_);
    ui_->listView->setAlternatingRowColors(true);
    connect(ui_->listView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(enableRemoveDirButton()));
}

/** \fn CollectionDialog::~CollectionDialog()
  * \brief class destructor
  */
CollectionDialog::~CollectionDialog() {
    delete ui_;
}



///////////
// slots //
///////////
/** \fn CollectionDialog::on_addDirButton_clicked()
  * \brief Defines the action when the Add button is clicked.
  * Opens a directory selection dialog. Once the directory is selected, it is added to the widget.
  */
void CollectionDialog::on_addDirButton_clicked() {

    QString newDir = QFileDialog::getExistingDirectory(this, tr("Choose a directory"),
                     QDir::homePath(),
                     QFileDialog::ShowDirsOnly |
                     QFileDialog::DontResolveSymlinks);

    if (newDir.isEmpty())
        ui_->buttonBox->setFocus();
    else {
        DatabaseManager::getInstance()->insertDirToCollection(newDir);
        model_->select();
    }
}


/** \fn CollectionDialog::on_delDirButton_clicked()
  * \brief Defines the action when the Remove button is clicked.
  * Removes the selected directory from the widget.
  */
void CollectionDialog::on_delDirButton_clicked() {
    QItemSelectionModel* selectionModel = ui_->listView->selectionModel();
    QModelIndexList indexList = selectionModel->selectedRows();
    QString oldDir = QString(indexList.at(0).data().toString());
    DatabaseManager::getInstance()->removeDirToCollection(oldDir);
    model_->select();
}


/** \fn CollectionDialog::enableRemoveDirButton()
  * \brief Enables the 'remove directory' button when a directory is selected.
  */
void CollectionDialog::enableRemoveDirButton() {
    ui_->delDirButton->setEnabled(true);
}
