/*
 * Copyright (C) 2009 Stéphane Péchard.
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


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn DatabaseManager::DatabaseManager()
  * \brief class constructor
  */
DatabaseManager::DatabaseManager() {}

/** \fn DatabaseManager::DatabaseManager(const DatabaseManager &databaseManager)
  * \brief copy constructor
  */
DatabaseManager::DatabaseManager(const DatabaseManager &databaseManager) {
    Q_UNUSED(databaseManager);
}

/** \fn DatabaseManager::~DatabaseManager()
  * \brief class destructor
  */
DatabaseManager::~DatabaseManager() {
    db_.close();
}



//////////////
// methods //
//////////////
/** \fn DatabaseManager::init(QStringList &tables, const QString &dbFilePath)
  * \brief initiates the database
  */
QSqlError DatabaseManager::init(QStringList &tables, const QString &dbFilePath) {

    db_ = QSqlDatabase::addDatabase("QSQLITE"); // sqlite-dependancy
    db_.setDatabaseName(dbFilePath);
    if (!db_.open())
        return db_.lastError();

    tables = db_.tables();

    // we check if tables are already in the db, if not,they are created
    // no table update is possible here!
    if (tables.contains("Collection") && tables.contains("Media") && tables.contains("ImdbInfo") && tables.contains("MovieGenre")
             && tables.contains("MusicGenre") && tables.contains("Tag"))
        return QSqlError();

    // in case of proposing different sql backend (mysql, postgresql, etc.), we will have to put this
    // either in the controller or in a backend-independant class
    QSqlQuery q;
    if (!q.exec(QLatin1String("create table Collection(id INTEGER PRIMARY KEY AUTOINCREMENT, directory VARCHAR(255))")))
        return q.lastError();

    if (!q.exec(QLatin1String("create table Media(id INTEGER PRIMARY KEY, \
            type INTEGER, name VARCHAR(255), loaned BOOLEAN, seen BOOLEAN, recommended BOOLEAN, notes TEXT)")))
        return q.lastError();

    if (!q.exec(QLatin1String("create table ImdbInfo(id INTEGER PRIMARY KEY, \
            mediaId INTEGER, ImdbId INTEGER, genre INTEGER, year INTEGER, runtime INTEGER, rating DOUBLE, \
            title VARCHAR(255), director VARCHAR(255), country VARCHAR(255), image VARCHAR(255), \
            studio VARCHAR(255), cast TEXT, plot TEXT, notes TEXT)")))
        return q.lastError();

    if (!q.exec(QLatin1String("create table MovieGenre(id INTEGER PRIMARY KEY, genre VARCHAR(255))")))
        return q.lastError();

    if (!q.exec(QLatin1String("create table MusicGenre(id INTEGER PRIMARY KEY, genre VARCHAR(255))")))
        return q.lastError();

    if (!q.exec(QLatin1String("create table Tag(id INTEGER PRIMARY KEY, mediaId INTEGER, name VARCHAR(255))")))
        return q.lastError();

    if (!q.exec(QLatin1String("create table Loans(id INTEGER PRIMARY KEY, mediaId INTEGER, person VARCHAR(255))")))
        return q.lastError();

    return QSqlError();
}


/** \fn DatabaseManager::getCollection()
  * \brief get collection from the database
  */
QStringList DatabaseManager::getCollection() {
    QSqlQuery q;
    if (!q.exec(QLatin1String("SELECT * FROM Collection")))
        QSqlError qError = q.lastError(); // TODO handle this!

    QStringList stringList;
    while (q.next()) {
        stringList << q.value(1).toString();
    }

   return stringList;
}


QSqlError DatabaseManager::insertDirToCollection(QString& s) {
    QSqlQuery q;
    if (!q.exec(QString("INSERT INTO Collection(directory) VALUES('%1')").arg(s)))
        return q.lastError();

    return QSqlError();
}


QSqlError DatabaseManager::removeDirToCollection(QString& s) {
    QSqlQuery q;
    if (!q.exec(QString("DELETE FROM Collection WHERE directory = '%1'").arg(s)))
        return q.lastError();

    return QSqlError();
}


bool DatabaseManager::hasDir(QString& s) {
    QSqlQuery q;
    if (!q.exec(QString("SELECT * FROM Collection WHERE directory = '%1'").arg(s)))
        return true;

    return q.next();
}



