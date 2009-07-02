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


#include "collection.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn Collection::Collection()
  * \brief class constructor
  */
Collection::Collection() : model_(new QStandardItemModel()) {}

/** \fn Collection::~Collection()
  * \brief class destructor
  */
Collection::~Collection() {}



/////////////
// Methods //
/////////////
/** \fn Collection::addDirectory(const QString dir)
  * \brief Adds a new directory in the list of directories.
  * \param QString
  */
void Collection::addDirectory(const QString& dir) {
    if(!this->contains(dir))
        this->append(dir);

    model_->setItem(this->size()-1, new QStandardItem(dir));
}


/** \fn Collection::removeDirectory(const QString dir)
  * \brief Removes the given directory from the list of directories and from the model.
  * \param QString
  */
void Collection::removeDirectory(const QString& dir) {
    if(this->contains(dir))
        this->removeAll(dir);

    QList<QStandardItem *> listItems = model_->findItems(dir, Qt::MatchExactly, 0 ) ;

    if(listItems.empty() || listItems.size() != 1)
        return;

    QStandardItem* item = listItems[0] ; // the item to be removed...
    model_->removeRow(item->row()); // ...now!
}


/** \fn Collection::createCollection(const QStringList dirList)
  * \brief Populates the collection object and the model with the QStringList dirList. It clears their content first.
  * \param QStringList dirList
  */
void Collection::createCollection(const QStringList& dirList) {
    this->clear();
    this->append(dirList);

    model_->clear();
    for (int i = 0; i < dirList.size(); ++i)
        model_->setItem(i, new QStandardItem(dirList.at(i)));
}



///////////////////////
// accessors methods //
///////////////////////
QStandardItemModel* Collection::getModel() const {
    return model_;
}

QString Collection::getDirAt(const int i)  const {
    return this->at(i);
}

