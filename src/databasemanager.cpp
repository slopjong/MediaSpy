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


#include "databasemanager.h"


/** \var static const QStringList dataTables_
  * \brief list of the tables in the database
  */
static const QStringList dataTables_ = QStringList() << "Collection" << "Media" << "ImdbInfo" << "MovieGenre" << "MusicGenre" << "Tag";

/** \var DatabaseManager* DatabaseManager::singleton_
  * \brief pointer to the unique instance of DatabaseManager
  */
DatabaseManager* DatabaseManager::singleton_ = NULL;


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn DatabaseManager::DatabaseManager()
  * \brief class constructor
  */
DatabaseManager::DatabaseManager() {}

/** \fn DatabaseManager::~DatabaseManager()
  * \brief class destructor
  */
DatabaseManager::~DatabaseManager() {
    db_.close();
}


//////////////
// methods //
//////////////
/** \fn DatabaseManager* DatabaseManager::getInstance()
  * \brief returns the unique instance of DatabaseManager, creates it the first time
  */
DatabaseManager* DatabaseManager::getInstance() {
    if (NULL == singleton_)
        singleton_ =  new DatabaseManager;
    return singleton_;
}

/** \fn DatabaseManager::kill()
  * \brief deletes the unique instance of DatabaseManager
  */
void DatabaseManager::kill() {
    if (NULL != singleton_) {
        delete singleton_;
        singleton_ = NULL;
    }
}


/** \fn DatabaseManager::init(QStringList &tables, const QString &dbFilePath)
  * \brief initiates the database
  */
QSqlError DatabaseManager::init(const QString &dbFilePath) {

    db_ = QSqlDatabase::addDatabase("QSQLITE"); // sqlite-dependancy
    db_.setDatabaseName(dbFilePath);
    if (!db_.open())
        return db_.lastError();

    QStringList tables = db_.tables();

    // we check if tables are already in the db, if not,they are created
    // no table update is possible here!
    if (tables.contains("Collection") && tables.contains("Media") && tables.contains("ImdbInfo")
            && tables.contains("MovieGenre") && tables.contains("MusicGenre") && tables.contains("Tag"))
        return QSqlError();

    // in case of proposing different sql backend (mysql, postgresql, etc.), we will have to put this
    // either in the controller or in a backend-independant class
    QSqlQuery q;
    if (!q.exec(QString("create table Collection(id INTEGER PRIMARY KEY AUTOINCREMENT, directory VARCHAR(255))")))
        return q.lastError();

    if (!q.exec(QString("create table Media(id INTEGER PRIMARY KEY AUTOINCREMENT, \
            type INTEGER, baseName VARCHAR(255), fileName VARCHAR(255), loaned BOOLEAN, seen BOOLEAN, \
            recommended BOOLEAN, notes TEXT)")))
        return q.lastError();

    if (!q.exec(QString("create table ImdbInfo(id INTEGER PRIMARY KEY AUTOINCREMENT, \
            mediaId INTEGER, ImdbId INTEGER, genre INTEGER, year INTEGER, runtime INTEGER, rating DOUBLE, \
            title VARCHAR(255), director VARCHAR(255), country VARCHAR(255), image VARCHAR(255), \
            studio VARCHAR(255), cast TEXT, plot TEXT, notes TEXT)")))
        return q.lastError();

    if (!q.exec(QString("create table MovieGenre(id INTEGER PRIMARY KEY AUTOINCREMENT, genre VARCHAR(255))")))
        return q.lastError();

    if (!q.exec(QString("create table MusicGenre(id INTEGER PRIMARY KEY AUTOINCREMENT, genre VARCHAR(255))")))
        return q.lastError();

    if (!q.exec(QString("create table Tag(id INTEGER PRIMARY KEY AUTOINCREMENT, mediaId INTEGER, name VARCHAR(255))")))
        return q.lastError();

    if (!q.exec(QString("create table Loans(id INTEGER PRIMARY KEY AUTOINCREMENT, mediaId INTEGER, person VARCHAR(255))")))
        return q.lastError();

    return QSqlError();
}


/** \fn DatabaseManager::getCollectionDir()
  * \brief get collection from the database
  */
QStringList DatabaseManager::getCollectionDir() {
    QSqlQuery q;
    if (!q.exec(QString("SELECT * FROM Collection")))
        QSqlError qError = q.lastError(); // TODO handle this!

    QStringList stringList;
    while (q.next()) {
        stringList << q.value(1).toString();
    }

   return stringList;
}


/** \fn DatabaseManager::insertDirToCollection(const QString& dir)
  * \brief Inserts the directory \var dir into the Collection table.
  */
QSqlError DatabaseManager::insertDirToCollection(const QString& dir) {
    QSqlQuery q;
    if (!q.exec(QString("INSERT INTO Collection(directory) VALUES('%1')").arg(dir)))
        throw(q.lastError()); // TODO handle this!

    return QSqlError();
}


/** \fn DatabaseManager::cleanCollection()
  * \brief Cleans the whole Collection table.
  */
QSqlError DatabaseManager::cleanCollection() {
    QSqlQuery q;
    if (!q.exec(QString("DELETE FROM `Collection`")))
        throw(q.lastError()); // TODO handle this!

    return QSqlError();
}


/** \fn DatabaseManager::hasDir(const QString& dir)
  * \brief Returns whether the directory \var dir is in the Collection table or not.
  */
bool DatabaseManager::hasDir(const QString& dir) {
    QSqlQuery q;
    if (!q.exec(QString("SELECT id FROM Collection WHERE directory = '%1'").arg(dir)))
        return true;

    return q.next();
}



/** \fn DatabaseManager::queryMedias()
  * \brief get medias from the database
  * \return the database query
  */
QSqlQuery DatabaseManager::queryMedias(QSqlQuery& q) {
    if (!q.exec(QString("SELECT * FROM Media")))
        throw(q.lastError()); // TODO handle this!
    return q;
}


/** \fn DatabaseManager::hasDir(const QString& dir)
  * \brief Returns whether the directory \var dir is in the Collection table or not.
  */
bool DatabaseManager::hasMedia(const QString& fileName) {
    QSqlQuery q;
    if (!q.exec(QString("SELECT id FROM Media WHERE fileName = '%1'").arg(fileName)))
        return true;

    return q.next();
}



/** \fn DatabaseManager::insertMedia(const QString& fileName)
  * \brief Inserts the directory \var dir into the Collection table.
  */
QSqlError DatabaseManager::insertMedia(const Media& media) {
    QSqlQuery q;
    if (!q.exec(QString("INSERT INTO Media(type, fileName, baseName, loaned, seen, recommended, notes) VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
            .arg(media.getType()).arg(media.getFileName()).arg(media.getBaseName()).arg(media.isLoaned())
            .arg(media.isSeen()).arg(media.isRecommended()).arg(media.getNotes())))
        throw(q.lastError()); // TODO handle this!

    return QSqlError();
}
