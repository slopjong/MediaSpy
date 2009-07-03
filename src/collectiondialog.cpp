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
CollectionDialog::CollectionDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    connect(this->directoryListView, SIGNAL(clicked(const QModelIndex&)),
            this, SLOT(enableRemoveDirButton()));

    this->directoryListView->setAlternatingRowColors(true);
}

/** \fn CollectionDialog::CollectionDialog(QWidget *parent)
  * \brief class destructor
  */
CollectionDialog::~CollectionDialog() {}


//////////////
// methods //
//////////////
void CollectionDialog::enableRemoveDirButton() {
    delDirButton->setEnabled(true);
}


/** \fn CollectionDialog::on_addDirButton_clicked()
  * \brief defines the action when the Add button is clicked
  * Opens a directory selection dialog. Once the directory is selected, the dirAdded signal is emitted.
  */
void CollectionDialog::on_addDirButton_clicked()
{
    QString newDir = QFileDialog::getExistingDirectory(this, tr("Choose a directory"),
                     QDir::homePath(),
                     QFileDialog::ShowDirsOnly |
                     QFileDialog::DontResolveSymlinks);

    if (newDir.isEmpty())
        this->buttonBox->setFocus();
    else {
        emit dirAdded(newDir);
    }
}


void CollectionDialog::on_delDirButton_clicked() {
    QModelIndexList selectedItemList = this->directoryListView->selectionModel()->selectedIndexes();
    QModelIndex item = selectedItemList.at(0);
    QString delDir = item.data().toString();
    emit dirRemoved(delDir);
}









///////////////////////
// accessors methods //
///////////////////////



