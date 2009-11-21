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
Collection::Collection() : nDir_(0) {}

/** \fn Collection::~Collection()
  * \brief class destructor
  */
Collection::~Collection() {}



/////////////
// Methods //
/////////////
/** \fn Collection::update()
  * \brief Updates the local collection with the db directory list.
  * \param QStringList& dirList
  */
void Collection::update() {
    dirList_ = DatabaseManager::getInstance()->getCollectionDir(); // get directories from db
    nDir_ = dirList_.count();
}


/** \fn Collection::buildFileList()
  * \brief Builds the list of the media files found in the directories of the collection.
  * \return the list of the media
  */
QStringList Collection::buildFileList() {
    QStringList fileList;
    for (int i = 0; i < this->dirList_.count(); ++i)
        fileList << ScanRecDir(this->dirList_.at(i));

    return fileList;
}


/** \fn Collection::ScanRecDir(const QString& dir)
  * \brief Scans a directory recursively and creates a list of all movie files.
  * \param dir the top directory
  * \return the list of movie files
  */
QStringList Collection::ScanRecDir(const QString& dir) {
    QStringList fileList;
    QDir qdir(dir);

    emit messageToStatus(QString(tr("Searching in %1...").arg(dir)));

    // filtering the names of the files in the directory
    QStringList nameFilterMovie = QStringList() << "*.avi" << "*.mpeg" << "*.mpg" << "*.mkv" << "*.mp4" << "*.wmv" << "*.flv" << "*.iso";
    qdir.setNameFilters(nameFilterMovie);
    qdir.setFilter(QDir::Files | QDir::Readable | QDir::Hidden | QDir::NoSymLinks);
    qdir.setSorting(QDir::Name);

    // we need the full name
    QString fileString;
    foreach(fileString, qdir.entryList())
        fileList << dir + QDir::separator() + fileString;

    //filtering the sub-directories
    QStringList dirList = qdir.entryList(QDir::AllDirs | QDir::Drives | QDir::NoDotAndDotDot);

    // recursive call
    for (int i = 0; i < dirList.count(); ++i)
        fileList << ScanRecDir( dir + QDir::separator() + dirList.at(i) );

    return fileList;
}

