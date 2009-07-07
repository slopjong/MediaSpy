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

#include <QFileInfo>


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn Collection::Collection()
  * \brief class constructor
  */
Collection::Collection() :
        databaseManager_(DatabaseManager::getInstance()), nDir_(0) {}

/** \fn Collection::~Collection()
  * \brief class destructor
  */
Collection::~Collection() {
}



/////////////
// Methods //
/////////////
/** \fn Collection::init()
  * \brief Initiates the Collection.
  */
void Collection::init() {
    QStringList stringList = databaseManager_->getCollection(); // read directories in db
    initDirList(stringList); // put them in the Collection
    // TODO: put a QFileSystemWatcher on them
}


/** \fn Collection::setDirDatabase(const QStringList& dirList)
  * \brief Populates the collection object and the database with the QStringList dirList. It clears their content first.
  * \param QStringList dirList
  */
void Collection::setDirDatabase(const QStringList& dirs) {
    databaseManager_->cleanCollection();
    for (int i = 0; i < dirs.size(); ++i)
        if(!databaseManager_->hasDir(dirs.at(i))) {
            QSqlError qError = databaseManager_->insertDirToCollection(dirs.at(i));
            if(qError.type())
                throw(qError);
        }
    initDirList(dirs);
}


void Collection::initDirList(const QStringList& dirs) {
    this->dirList_.clear();
    this->dirList_.append(dirs);
    nDir_ = dirs.size();
}


/** \fn Collection::buildFileList()
  * \brief Builds the list of the media files found in the directories of the collection.
  * \return the list of the media
  */
QStringList Collection::buildFileList() {
    QStringList fileList;

    for (int i = 0; i < this->dirList_.size(); ++i)
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

    // filtering the names of the files in the directory
    QStringList nameFilter = QStringList() << "*.avi" << "*.mpeg" << "*.mkv";
    qdir.setNameFilters(nameFilter);
    qdir.setFilter(QDir::Files | QDir::Readable | QDir::Hidden | QDir::NoSymLinks);
    qdir.setSorting(QDir::Name);

    // we need the full name
    QString fileString;
    foreach(fileString, qdir.entryList())
        fileList << dir + QDir::separator() + fileString;

    //filtering the sub-directories
    QStringList dirList = qdir.entryList(QDir::AllDirs | QDir::Drives | QDir::NoDotAndDotDot);

    // recursive call
    for (int i = 0; i < dirList.size(); ++i)
        fileList << ScanRecDir( dir + QDir::separator() + dirList.at(i) );

    return fileList;
}


///////////////////////
// accessors methods //
///////////////////////
/** \fn QString Collection::getDirAt(const int i)  const
  * \brief Returns the name of the directory at i-th rank in the model.
  * \return the directory name
  */
QString Collection::getDirAt(const int i)  const {
    return this->dirList_.at(i);
}

void Collection::setNDir(const int nDir) {
    nDir_ = nDir;
}
int Collection::getNDir() const {
    return nDir_;
}
