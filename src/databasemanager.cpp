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
static const QStringList dataTables_ =
        QStringList() << "Collection" << "Media" << "ImdbInfo" << "MovieGenre" << "MusicGenre" << "Tag";

/** \var DatabaseManager* DatabaseManager::singleton_
  * \brief pointer to the unique instance of DatabaseManager
  */
DatabaseManager* DatabaseManager::singleton_ = 00;


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
    if (00 == singleton_)
        singleton_ =  new DatabaseManager;
    return singleton_;
}

/** \fn DatabaseManager::kill()
  * \brief deletes the unique instance of DatabaseManager
  */
void DatabaseManager::kill() {
    if (00 != singleton_) {
        delete singleton_;
        singleton_ = 00;
        QSqlDatabase::removeDatabase("QSQLITE");
    }
}


/** \fn DatabaseManager::init(QStringList &tables, const QString &dbFilePath)
  * \brief initiates the database
  */
QSqlError DatabaseManager::init(const QString &dbFilePath) {

    db_ = QSqlDatabase::addDatabase("QSQLITE"); // sqlite-dependancy
    db_.setDatabaseName(dbFilePath);
    if (!db_.isValid() || !db_.open())
        return db_.lastError();

    QStringList tables = db_.tables();

    // we check if tables are already in the db, if not,they are created
    // no table update is possible here!
    if (tables.contains("Collection") && tables.contains("Media") && tables.contains("ImdbInfo")
            && tables.contains("MovieGenre") && tables.contains("MusicGenre") && tables.contains("Tag"))
        return QSqlError();

    // in case of proposing different sql backend (mysql, postgresql, etc.), we will have to put this
    // in a backend-independant class
    QSqlQuery q;
    if (!q.exec(QString("create table Collection(id INTEGER PRIMARY KEY AUTOINCREMENT, directory VARCHAR(255))")))
        return q.lastError();

    if (!q.exec(QString("create table Media(id INTEGER PRIMARY KEY AUTOINCREMENT, \
            type INTEGER, baseName VARCHAR(255), fileName VARCHAR(255), \
            imdbInfo BOOLEAN, \
            loaned BOOLEAN, seen BOOLEAN, recommended BOOLEAN, notes TEXT)")))
        return q.lastError();

    if (!q.exec(QString("create table ImdbInfo(id INTEGER PRIMARY KEY AUTOINCREMENT, \
            mediaId INTEGER, ImdbId INTEGER, genre VARCHAR(255), year INTEGER, runtime INTEGER, rating DOUBLE, \
            title VARCHAR(255), director VARCHAR(255), country VARCHAR(255), image VARCHAR(255), \
            cast TEXT, plot TEXT)")))
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
    if (!q.exec(QString("SELECT directory FROM Collection")))
        QSqlError qError = q.lastError(); // TODO handle this!

    QStringList stringList;
    while (q.next()) {
        stringList << q.value(0).toString();
    }

   return stringList;
}


/** \fn DatabaseManager::insertDirToCollection(const QString& dir)
  * \brief Inserts the directory \var dir into the Collection table.
  */
QSqlError DatabaseManager::insertDirToCollection(const QString& dir) {
    QSqlQuery q;
    q.prepare("INSERT INTO Collection (directory) "
              "VALUES (?)");
    q.bindValue(0, dir);

    if (!q.exec())
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
    q.prepare("SELECT id FROM Collection WHERE directory = ?");
    q.bindValue(0, dir);

    if (!q.exec())
        throw(q.lastError()); // TODO handle this!

    return q.next();
}


/** \fn DatabaseManager::queryMedias()
  * \brief get a query with all medias from the database
  * \return the database query
  */
QSqlQuery DatabaseManager::queryMedias(QSqlQuery& q) {
    if (!q.exec(QString("SELECT * FROM Media")))
        throw(q.lastError()); // TODO handle this!
    return q;
}


/** \fn DatabaseManager::queryMediaWhere(QSqlQuery& q, QString& whereName, QString& whereValue)
  * \brief get a query with the media whom \var whereName attribute is \var whereValue
  * \return the database query
  */

QSqlQuery DatabaseManager::queryMediaWhere(QSqlQuery& q, QString& whereName, QString& whereValue) {
    QString preparedQuery = QString("SELECT * FROM Media WHERE %1 = ?").arg(whereName);
    q.prepare(preparedQuery);
    q.bindValue(0, whereValue);
    if (!q.exec())
        throw(q.lastError()); // TODO handle this!

    return q;
}


/** \fn DatabaseManager::queryImdbInfoWhere(QSqlQuery& q, QString& whereName, QString& whereValue)
  * \brief get a query with the media whom \var whereName attribute is \var whereValue
  * \return the database query
  */
QSqlQuery DatabaseManager::queryImdbInfoWhere(QSqlQuery& q, QString& whereName, QString& whereValue) {
    QString preparedQuery = QString("SELECT * FROM ImdbInfo WHERE %1 = ?").arg(whereName);
    q.prepare(preparedQuery);
    q.bindValue(0, whereValue);
    if (!q.exec())
        throw(q.lastError()); // TODO handle this!
    return q;
}


/** \fn DatabaseManager::queryMediaNames()
  * \brief get all media fileName from the database
  * \return the fileName list
  */
QStringList DatabaseManager::queryMediaNames() {
    QSqlQuery q;
    if (!q.exec(QString("SELECT fileName FROM Media")))
        throw(q.lastError()); // TODO handle this!

    QStringList list;
    while (q.next())
        list << q.value(0).toString();

    return list;
}


/** \fn DatabaseManager::hasMedia(const QString& fileName)
  * \brief Returns whether the media \var fileName is in the MediaCollection table or not.
  */
bool DatabaseManager::hasMedia(const QString& fileName) {
    QSqlQuery q;
    q.prepare("SELECT id FROM Media WHERE fileName = ?");
    q.bindValue(0, fileName);

    if (!q.exec())
        return true;

    return q.next();
}


/** \fn DatabaseManager::isMediaSeen(const QString& fileName)
  * \brief Returns whether the media \var fileName is marked as seen.
  */
bool DatabaseManager::isMediaSeen(const QString& baseName) {
    QSqlQuery q;
    q.prepare("SELECT id FROM Media WHERE baseName = ? AND seen = 'true'");
    q.bindValue(0, baseName);

    if (!q.exec())
        return true;

    return q.next();
}


/** \fn DatabaseManager::setMediaSeen(const QString& baseName, bool checked)
  * \brief Sets
  */
void DatabaseManager::setMediaSeen(const QString& baseName, bool checked) {
    QSqlQuery q;
    q.prepare("UPDATE Media SET seen = ? WHERE baseName = ?");
    q.bindValue(0, checked);
    q.bindValue(1, baseName);
    if (!q.exec())
        throw(q.lastError()); // TODO handle this!
}


/** \fn DatabaseManager::insertMedias(const QList<Media>& mediaList)
  * \brief Inserts the medias from the list \var mediaList into the Media table.
  */
void DatabaseManager::insertMedias(const QList<Media>& mediaList) {
    QSqlDatabase::database().transaction();
    QSqlQuery q;
    q.prepare("INSERT INTO Media (type, baseName, fileName, imdbInfo, loaned, seen, recommended, notes) "
              "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

    Media media;
    foreach(media, mediaList) {
        q.bindValue(0, media.getType());
        q.bindValue(1, media.getBaseName());
        q.bindValue(2, media.getFileName());
        q.bindValue(3, media.hasImdbInfo());
        q.bindValue(4, media.isLoaned());
        q.bindValue(5, media.isSeen());
        q.bindValue(6, media.isRecommended());
        q.bindValue(7, media.getNotes());

        if (!q.exec())
            throw(q.lastError()); // TODO handle this!
    }
    QSqlDatabase::database().commit();
}


/** \fn DatabaseManager::removeMedias(const QStringList& mediaFileNames)
  * \brief Removes the medias from the list called \var mediaFileNames off the Media table.
  */
void DatabaseManager::removeMedias(const QStringList& mediaFileNames) {
    QSqlDatabase::database().transaction();
    QSqlQuery q;
    q.prepare("DELETE FROM Media WHERE fileName = ?");

    QString s;
    foreach(s, mediaFileNames) {
        q.bindValue(0, s);
        if (!q.exec())
            throw(q.lastError()); // TODO handle this!
    }

    QSqlDatabase::database().commit();
}


/** \fn QStringList DatabaseManager::queryMediaWithNoImdbInfo()
  * \brief Returns the list of media without imdb info.
  */
QStringList DatabaseManager::queryMediaWithNoImdbInfo() {
    QSqlQuery q;
    if (!q.exec(QString("SELECT fileName FROM Media WHERE imdbInfo = 'false'")))
        throw(q.lastError()); // TODO handle this!

    QStringList list;
    while (q.next())
        list << q.value(0).toString();

    return list;
}


/** \fn DatabaseManager::hasImdbInfo(const QString& fileName)
  * \brief Returns whether the media \var fileName is in the MediaCollection table or not.
  */
bool DatabaseManager::hasImdbInfo(const QString& fileName) {
    QSqlQuery q;
    q.prepare("SELECT imdbInfo FROM Media WHERE fileName = ?");
    q.bindValue(0, fileName);

    if (!q.exec())
        return false;

    bool hasInfo = false;
    while (q.next())
         hasInfo = q.value(0).toBool();

    return hasInfo;
}


/** \fn void DatabaseManager::insertMovieMedia(MovieMedia movieMedia)
  * \brief Inserts a movie in the database.
  */
void DatabaseManager::insertMovieMedia(MovieMedia movieMedia) {
    QSqlDatabase::database().transaction();
    QSqlQuery q;

    // insert Media in case it is not yet
    if(!hasMedia(movieMedia.getFileName())) {
        q.prepare("INSERT INTO Media (type, baseName, fileName, imdbInfo, loaned, seen, recommended, notes) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

        q.bindValue(0, movieMedia.getType());
        q.bindValue(1, movieMedia.getBaseName());
        q.bindValue(2, movieMedia.getFileName());
        q.bindValue(3, movieMedia.hasImdbInfo());
        q.bindValue(4, movieMedia.isLoaned());
        q.bindValue(5, movieMedia.isSeen());
        q.bindValue(6, movieMedia.isRecommended());

        if (!q.exec())
            throw(q.lastError()); // TODO handle this!
    }

    // insert InfoImdb
    q.prepare("INSERT INTO ImdbInfo (mediaId, ImdbId, genre, year, runtime, rating, title, director, country, image, cast, plot) "
              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    q.bindValue(0, movieMedia.getId());
    q.bindValue(1, movieMedia.getImdbId());
    q.bindValue(2, movieMedia.getGenre());
    q.bindValue(3, movieMedia.getYear());
    q.bindValue(4, movieMedia.getRuntime());
    q.bindValue(5, movieMedia.getRating());
    q.bindValue(6, movieMedia.getTitle());
    q.bindValue(7, movieMedia.getDirector());
    q.bindValue(8, movieMedia.getCountry());
    q.bindValue(9, movieMedia.getImage());
    q.bindValue(10, movieMedia.getCast());
    q.bindValue(11, movieMedia.getPlot());

    if (!q.exec())
        throw(q.lastError()); // TODO handle this!

    // change infoImdb in Media
    q.prepare("UPDATE Media SET imdbInfo = 'true' WHERE fileName = ?");
    q.bindValue(0, movieMedia.getFileName());

    if (!q.exec())
        throw(q.lastError()); // TODO handle this!

    QSqlDatabase::database().commit();
}


/** \fn QString DatabaseManager::getMediaFullName(QString& baseName)
  * \brief Returns the full name of the \var baseName file.
  */
QString DatabaseManager::getMediaFullName(QString& baseName) {
    QSqlQuery q;
    q.prepare("SELECT fileName FROM Media WHERE baseName = ?");
    q.bindValue(0, baseName);

    if (!q.exec())
        throw(q.lastError()); // TODO handle this!

    QString fullName;
    while (q.next())
         fullName = q.value(0).toString();

    return fullName;
}


/*QStringList DatabaseManager::queryImdbInfoJoinMediaId() {
    QSqlQuery q;
    if (!q.exec(QString("SELECT * FROM imdbInfo JOIN Media ON imdbInfo.mediaId = Media.id")))
        throw(q.lastError()); // TODO handle this!

    QStringList list;
    while (q.next())
        list << q.value(0).toString();

    return list;
}*/

