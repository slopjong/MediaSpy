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
CollectionDialog::CollectionDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    connect(this->listWidget, SIGNAL(clicked(const QModelIndex&)),
            this, SLOT(enableRemoveDirButton()));

    this->listWidget->setAlternatingRowColors(true);
}

/** \fn CollectionDialog::~CollectionDialog()
  * \brief class destructor
  */
CollectionDialog::~CollectionDialog() {}


//////////////
// methods //
//////////////
/** \fn CollectionDialog::enableRemoveDirButton()
  * \brief Enables the 'remove directory' button when a directory is selected.
  */
void CollectionDialog::enableRemoveDirButton() {
    delDirButton->setEnabled(true);
}


/** \fn CollectionDialog::on_addDirButton_clicked()
  * \brief Defines the action when the Add button is clicked.
  * Opens a directory selection dialog. Once the directory is selected, the dirAdded signal is emitted.
  */
void CollectionDialog::on_addDirButton_clicked() {

    QString newDir = QFileDialog::getExistingDirectory(this, tr("Choose a directory"),
                     QDir::homePath(),
                     QFileDialog::ShowDirsOnly |
                     QFileDialog::DontResolveSymlinks);

    if (newDir.isEmpty())
        this->buttonBox->setFocus();
    else {
        addedDirList_ << newDir;
        this->listWidget->addItem(newDir);
    }
}


/** \fn CollectionDialog::on_delDirButton_clicked()
  * \brief Defines the action when the Remove button is clicked.
  * Removes the selected directory from the widget.
  */
void CollectionDialog::on_delDirButton_clicked() {
    QList<QListWidgetItem*> selectedItemList = this->listWidget->selectedItems();
    QListWidgetItem* item = selectedItemList.at(0);
    int r = this->listWidget->row(item);
    this->listWidget->takeItem(r);
}


/** \fn CollectionDialog::getUpdate()
  * \brief Puts the listWidget items in a QStringList.
  */
QStringList CollectionDialog::getUpdate() {
    QStringList addedDirList;

    for(int i = 0; i < this->listWidget->count(); ++i) {
        QListWidgetItem* item = this->listWidget->item(i);
        addedDirList << item->text();
    }
    return addedDirList;
}



