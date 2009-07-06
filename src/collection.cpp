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
        dirListModel_(new QStandardItemModel()),
        databaseManager_(DatabaseManager::getInstance()) {}

/** \fn Collection::~Collection()
  * \brief class destructor
  */
Collection::~Collection() {
    delete dirListModel_;
}



/////////////
// Methods //
/////////////
/** \fn Collection::addDirectory(const QString dir)
  * \brief Adds a new directory in the list of directories and in the model.
  * \param QString dir
  */
void Collection::addDirectory(const QString& dir) {
    if(!this->dirList_.contains(dir))
        this->dirList_.append(dir);

    dirListModel_->setItem(this->dirList_.size() - 1, new QStandardItem(dir));
}


/** \fn Collection::removeDirectory(const QString dir)
  * \brief Removes the directory \var dir from the list of directories and from the model.
  * \param QString dir
  */
void Collection::removeDirectory(const QString& dir) {
    if(this->dirList_.contains(dir))
        this->dirList_.removeAll(dir);

    QList<QStandardItem *> listItems = dirListModel_->findItems(dir, Qt::MatchExactly, 0 ) ;

    if(listItems.empty() || listItems.size() != 1)
        return;

    QStandardItem* item = listItems[0] ; // the item to be removed...
    dirListModel_->removeRow(item->row()); // ...now!
}


/** \fn Collection::setDirList(const QStringList& dirList)
  * \brief Populates the collection object and the model with the QStringList dirList. It clears their content first.
  * \param QStringList dirList
  */
void Collection::setDirList(const QStringList& dirs) {
    this->dirList_.clear();
    this->dirList_.append(dirs);

    dirListModel_->clear();
    for (int i = 0; i < dirs.size(); ++i)
        dirListModel_->setItem(i, new QStandardItem(dirs.at(i)));
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

    QStringList nameFilter = QStringList() << "*.avi" << "*.mpeg" << "*.mkv";
    qdir.setNameFilters(nameFilter);
    qdir.setFilter(QDir::Files | QDir::Readable | QDir::Hidden | QDir::NoSymLinks);
    qdir.setSorting(QDir::Name);

    fileList << qdir.entryList();
    QStringList dirList = qdir.entryList(QDir::AllDirs | QDir::Drives | QDir::NoDotAndDotDot);

    for (int i = 0; i < dirList.size(); ++i)
        fileList << ScanRecDir( dir + QDir::separator() + dirList.at(i) );

    return fileList;
}


///////////////////////
// accessors methods //
///////////////////////
/** \fn Collection::getDirModel() const
  * \brief Returns the model of the list of directories.
  * \return the model
  */
QStandardItemModel* Collection::getDirModel() const {
    return dirListModel_;
}

/** \fn QString Collection::getDirAt(const int i)  const
  * \brief Returns the name of the directory at i-th rank in the model.
  * \return the directory name
  */
QString Collection::getDirAt(const int i)  const {
    return this->dirList_.at(i);
}

