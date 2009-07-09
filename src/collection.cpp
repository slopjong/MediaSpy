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


/** \var Collection* Collection::singleton_
  * \brief pointer to the unique instance of Collection
  */
Collection* Collection::singleton_ = NULL;


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
/** \fn Collection* Collection::getInstance()
  * \brief returns the unique instance of Collection, creates it the first time
  */
Collection* Collection::getInstance() {
    if (NULL == singleton_)
        singleton_ =  new Collection;
    return singleton_;
}


/** \fn Collection::kill()
  * \brief Deletes the unique instance of Collection.
  */
void Collection::kill() {
    if (NULL != singleton_) {
        delete singleton_;
        singleton_ = NULL;
    }
}


/** \fn Collection::init()
  * \brief Initiates the Collection.
  */
void Collection::init() {
    QStringList stringList = DatabaseManager::getInstance()->getCollectionDir(); // read directories in db
    initDirList(stringList); // put them in the Collection
    // TODO: put a QFileSystemWatcher on them
}


/** \fn Collection::update(const QStringList& dirList)
  * \brief Updates the database with the QStringList dirList. Then the list object is updated as well.
  * It clears their content first.
  * \param QStringList& dirList
  */
void Collection::update(const QStringList& dirs) {
    DatabaseManager::getInstance()->cleanCollection();
    for (int i = 0; i < dirs.size(); ++i)
        if(!DatabaseManager::getInstance()->hasDir(dirs.at(i))) {
            QSqlError qError = DatabaseManager::getInstance()->insertDirToCollection(dirs.at(i));
            if(qError.type())
                throw(qError);
        }
    initDirList(dirs);
}


/** \fn void Collection::initDirList(const QStringList& dirs)
  * \brief Initiates the list of directories. It clears its content first.
  * \param QStringList& dirList
  */
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
    QStringList nameFilterMovie = QStringList() << "*.avi" << "*.mpeg" << "*.mpg" << "*.mkv" << "*.mp4" << "*.wmv";
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
    for (int i = 0; i < dirList.size(); ++i)
        fileList << ScanRecDir( dir + QDir::separator() + dirList.at(i) );

    return fileList;
}



///////////////////////
// accessors methods //
///////////////////////
/** \fn QString Collection::getDirAt(const int i) const
  * \brief Returns the name of the directory at i-th rank in the model.
  * \return the directory name
  */
QString Collection::getDirAt(const int i)  const {
    return this->dirList_.at(i);
}

/** \fn void Collection::setNDir(const int nDir)
  * \brief Sets the number of directories in the Collection.
  * \param the number of directories in the Collection
  */
void Collection::setNDir(const int nDir) {
    nDir_ = nDir;
}

/** \fn int Collection::getNDir() const
  * \brief Returns the number of directories in the Collection.
  * \return the numbeer of directories in the Collection
  */
int Collection::getNDir() const {
    return nDir_;
}
