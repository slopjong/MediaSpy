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
Collection::Collection() {
}

/** \fn Collection::~Collection()
  * \brief class destructor
  */
Collection::~Collection() {
}



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
}


/** \fn Collection::removeDirectory(const QString dir)
  * \brief Removes the given directory from the list of directories.
  * \param QString
  */
void Collection::removeDirectory(const QString& dir) {
    if(this->contains(dir))
        this->removeAll(dir);
}


/** \fn Collection::createCollection(const QStringList dirList)
  * \brief Populates the collection with the QStringList dirList. It clears the list first.
  * \param QStringList dirList
  */
void Collection::createCollection(const QStringList& dirList) {
    this->clear();
    this->append(dirList);
}




///////////////////////
// accessors methods //
///////////////////////
int Collection::getSize() const {
    return this->size();
}

QString Collection::getDirAt(int i) {
    return this->at(i);
}

