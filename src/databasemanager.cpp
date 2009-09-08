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
//static const QStringList dataTables_ =
//        QStringList() << "Collection" << "Media" << "ImdbInfo" << "MovieGenre" << "MusicGenre" << "Tag" << "Media_Tag";

/** \var DatabaseManager* DatabaseManager::singleton_
  * \brief pointer to the unique instance of DatabaseManager
  */
DatabaseManager* DatabaseManager::singleton_ = 0;


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
    if (0 == singleton_)
        singleton_ =  new DatabaseManager;
    return singleton_;
}

/** \fn DatabaseManager::kill()
  * \brief deletes the unique instance of DatabaseManager
  */
void DatabaseManager::kill() {
    if (0 != singleton_) {
        delete singleton_;
        singleton_ = 0;
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
            && tables.contains("MovieGenre") && tables.contains("MusicGenre") && tables.contains("Tag")
            && tables.contains("Media_Tag"))
        return QSqlError(); // TODO handle this!


    // in case of proposing different sql backend (mysql, postgresql, etc.), we will have to put this
    // in a backend-independant class
    QSqlQuery q;
    if (!q.exec(QString("create table Collection(id INTEGER PRIMARY KEY AUTOINCREMENT, directory VARCHAR(255), UNIQUE (directory))")))
        return q.lastError();


    // !! be careful if changing the order of these fields !!
    if (!q.exec(QString("create table Media(id INTEGER PRIMARY KEY AUTOINCREMENT," \
            "type INTEGER, baseName VARCHAR(255) UNIQUE, fileName VARCHAR(255) UNIQUE, imdbInfo BOOLEAN," \
            "loaned BOOLEAN, seen BOOLEAN, recommended BOOLEAN, notes TEXT)")))
        return q.lastError();

    if (!q.exec(QString("create table ImdbInfo(id INTEGER PRIMARY KEY AUTOINCREMENT," \
            "mediaId INTEGER UNIQUE, ImdbId INTEGER, genre VARCHAR(255), year INTEGER, runtime INTEGER," \
            "rating DOUBLE, title VARCHAR(255), director VARCHAR(255), country VARCHAR(255), image VARCHAR(255)," \
            "cast TEXT, plot TEXT)")))
        return q.lastError();

    if (!q.exec(QString("create table MovieGenre(id INTEGER PRIMARY KEY AUTOINCREMENT, genre VARCHAR(255))")))
        return q.lastError();

    if (!q.exec(QString("create table MusicGenre(id INTEGER PRIMARY KEY AUTOINCREMENT, genre VARCHAR(255))")))
        return q.lastError();

    if (!q.exec(QString("create table Tag(id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(255) UNIQUE)")))
        return q.lastError();

    if (!q.exec(QString("create table Media_Tag(id INTEGER PRIMARY KEY AUTOINCREMENT, mediaId INTEGER, tagId INTEGER," \
                        "CONSTRAINT uc_MediaTagId UNIQUE (mediaId, tagId))")))
        return q.lastError();





//    if (!q.exec(QString("create table Loans(id INTEGER PRIMARY KEY AUTOINCREMENT, mediaId INTEGER, person VARCHAR(255))")))
//        return q.lastError();

//    if (!q.exec(QString("create table Visualisation(id INTEGER PRIMARY KEY AUTOINCREMENT, mediaId INTEGER, date TIMESTAMP)")))
//        return q.lastError();

    return QSqlError();
}



//////////////////////////////
// Collection table methods //
//////////////////////////////
/** \fn DatabaseManager::getCollectionDir()
  * \brief get collection from the database
  */
QStringList DatabaseManager::getCollectionDir() {
    QSqlQuery q;
    if (!q.exec(QString("SELECT directory FROM Collection")))
        QSqlError qError = q.lastError(); // TODO handle this!

    QStringList stringList;
    while (q.next())
        stringList << q.value(0).toString();

   return stringList;
}


/** \fn DatabaseManager::insertDirToCollection(const QString& dir)
  * \brief Inserts the directory \var dir into the Collection table.
  */
QSqlError DatabaseManager::insertDirToCollection(const QString& dir) {
    QSqlQuery q;
    q.prepare("INSERT INTO Collection (directory) VALUES (?)");
    q.bindValue(0, dir);

    if (!q.exec()) {
        if(q.lastError().type()==1 && q.lastError().text()=="constraint failed Unable to fetch row")
            return QSqlError();
        throw(q.lastError()); // TODO handle this!
    }

    return QSqlError();
}


/** \fn DatabaseManager::removeDirToCollection(const QString& dir)
  * \brief Removes the directory \var dir from the Collection table and all its content from the Media table.
  */
QSqlError DatabaseManager::removeDirToCollection(const QString& dir) {
    QSqlQuery q;
    // delete the info from imdb
    q.prepare("DELETE FROM ImdbInfo WHERE mediaId in (SELECT id FROM Media WHERE Media.fileName LIKE ?)");
    q.bindValue(0, dir + '%');

    if (!q.exec())
        throw(q.lastError()); // TODO handle this!

    // delete the info media
    q.prepare("DELETE FROM Media WHERE fileName LIKE ?");
    q.bindValue(0, dir + '%');

    if (!q.exec())
        throw(q.lastError()); // TODO handle this!

    // delete the directory
    q.prepare("DELETE FROM Collection WHERE directory = ?");
    q.bindValue(0, dir);

    if (!q.exec())
        throw(q.lastError()); // TODO handle this!

    return QSqlError();
}



/////////////////////////
// Media table methods //
/////////////////////////
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
void DatabaseManager::setMediaSeen(const QStringList& baseNameList, bool checked) {
    QSqlDatabase::database().transaction();
    QSqlQuery q;
    q.prepare("UPDATE Media SET seen = ? WHERE baseName = ?");

    QString baseName;
    foreach(baseName, baseNameList) {
        q.bindValue(0, checked);
        q.bindValue(1, baseName);
        if (!q.exec()) {
            QSqlDatabase::database().rollback();
            throw(q.lastError()); // TODO handle this!
        }
    }
    QSqlDatabase::database().commit();
}


/** \fn DatabaseManager::getNMediaSeen()
  * \brief Returns the number of media set as seen.
  */
int DatabaseManager::getNMediaSeen() {
    QSqlQuery q;
    if (!q.exec("SELECT COUNT(*) FROM Media WHERE seen = 'true'"))
        throw(q.lastError()); // TODO handle this!

    q.next();
    return q.value(0).toInt();
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

        if (!q.exec()) {
            if(q.lastError().type()==1 && q.lastError().text()=="constraint failed Unable to fetch row")
                continue;
            QSqlDatabase::database().rollback();
            throw(q.lastError()); // TODO handle this!
        }
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
        if (!q.exec()) {
            QSqlDatabase::database().rollback();
            throw(q.lastError()); // TODO handle this!
        }
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

        if (!q.exec()) {
            QSqlDatabase::database().rollback();
            throw(q.lastError()); // TODO handle this!
        }
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

    if (!q.exec()) {
        QSqlDatabase::database().rollback();
        throw(q.lastError()); // TODO handle this!
    }

    // change infoImdb in Media
    q.prepare("UPDATE Media SET imdbInfo = 'true' WHERE fileName = ?");
    q.bindValue(0, movieMedia.getFileName());

    if (!q.exec()) {
        QSqlDatabase::database().rollback();
        throw(q.lastError()); // TODO handle this!
    }

    QSqlDatabase::database().commit();
}


/** \fn QString DatabaseManager::getMediaFullName(QString baseName)
  * \brief Returns the full name of the \var baseName file.
  */
QString DatabaseManager::getMediaFullName(QString baseName) {
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


/** \fn int DatabaseManager::getMediaId(QString& baseName)
  * \brief Returns the id of the \var baseName file.
  */
int DatabaseManager::getMediaId(QString& baseName) {
    QSqlQuery q;
    q.prepare("SELECT id FROM Media WHERE baseName = ?");
    q.bindValue(0, baseName);

    if (!q.exec())
        throw(q.lastError()); // TODO handle this!

    int id = 0;
    while (q.next())
         id = q.value(0).toInt();

    return id;
}


QStringList DatabaseManager::getCoverList() {
    QSqlQuery q;
    if (!q.exec("SELECT DISTINCT image FROM ImdbInfo"))
        throw(q.lastError()); // TODO handle this!

    QStringList list;
    while (q.next())
        list << q.value(0).toString();

    return list;
}



///////////////////////
// Tag table methods //
///////////////////////
QStringList DatabaseManager::getMediaTagList(int id) {
    QSqlQuery q;
    q.prepare("SELECT DISTINCT name FROM Tag JOIN Media_Tag ON Media_Tag.tagId = Tag.id WHERE Media_Tag.mediaId = ?");
    q.bindValue(0, id);
    if (!q.exec())
        throw(q.lastError()); // TODO handle this!

    QStringList list;
    while (q.next())
        list << q.value(0).toString();

    return list;
}


QStringList DatabaseManager::getMediaTagList(QString& baseName) {
    QSqlQuery q;
    q.prepare("SELECT id FROM Media WHERE baseName = ?");
    q.bindValue(0, baseName);
    if (!q.exec())
        throw(q.lastError()); // TODO handle this!

    int id;
    while (q.next())
        id = q.value(0).toInt();

    return getMediaTagList(id);
}


QStringList DatabaseManager::getTagList() {
    QSqlQuery q;
    if (!q.exec("SELECT name FROM Tag ORDER BY name"))
        throw(q.lastError()); // TODO handle this!

    QStringList list;
    while (q.next())
        list << q.value(0).toString();

    return list;
}


QStringList DatabaseManager::getOrphanTagList() {
    QSqlQuery q;
    if (!q.exec("SELECT name FROM Tag WHERE Tag.id NOT IN (SELECT tagId FROM Media_Tag)"))
        throw(q.lastError()); // TODO handle this!

    QStringList list;
    while (q.next())
        list << q.value(0).toString();

    return list;
}


void DatabaseManager::purgeOrphanTags() {
    QSqlQuery q;
    if (!q.exec("DELETE FROM Tag WHERE Tag.id NOT IN (SELECT tagId FROM Media_Tag)"))
        throw(q.lastError()); // TODO handle this!
}


void DatabaseManager::insertTag(QString& tagName) {
    QSqlQuery q;
    q.prepare("INSERT INTO Tag (name) VALUES(?)");
    q.bindValue(0, tagName);
    if (!q.exec()) {
        if(q.lastError().type()==1 && q.lastError().text()=="constraint failed Unable to fetch row")
            return;
        throw(q.lastError()); // TODO handle this!
    }
}


void DatabaseManager::addTagToMedia(QString& tagName, QString mediaBaseName) {
    QSqlQuery q;
    q.prepare("INSERT INTO Media_Tag (tagId, mediaId) " \
              "SELECT Tag.id, Media.id FROM Tag, Media WHERE Tag.name = ? AND Media.baseName = ?");
    q.bindValue(0, tagName);
    q.bindValue(1, mediaBaseName);
    if (!q.exec()) {
        if(q.lastError().type()==1 && q.lastError().text()=="constraint failed Unable to fetch row")
            return;
        throw(q.lastError()); // TODO handle this!
    }
}

void DatabaseManager::removeTagFromMedia(QString& tagName, QString mediaBaseName) {
    QSqlQuery q;
    q.prepare("DELETE FROM Media_Tag WHERE tagId in (SELECT id FROM Tag WHERE Tag.name = ?)" \
              "AND mediaId in (SELECT id FROM Media WHERE Media.baseName = ?)");
    q.bindValue(0, tagName);
    q.bindValue(1, mediaBaseName);
    if (!q.exec())
        throw(q.lastError()); // TODO handle this!
}

