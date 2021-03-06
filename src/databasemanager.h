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


#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql>

#include "media/media.h"
#include "media/moviemedia.h"

namespace tableCollection { enum e { id, directory }; }
namespace tableMedia { enum e { id, baseName, fileName, type, imdbInfo, loaned, seen, recommended, notes }; }

// StatsImdb structure
struct StatsImdb {
    int     nImdbInfo;
    double  maxRuntime;
    double  avgRuntime;
    double  minRuntime;
};


/** \class DatabaseManager
  *
  */
class DatabaseManager {
    // Constructors
    DatabaseManager();
    virtual ~DatabaseManager();

    // Fields
    static DatabaseManager *singleton_;
    QSqlDatabase db_;

public:
    // Operations
    static DatabaseManager *getInstance();
    static void kill();

    QSqlError init(const QString&);

    QSqlError insertDirToCollection(const QString&);
    QSqlError removeDirToCollection(const QString&);
    QStringList getCollectionDir();

    void insertMedias(const QList<Media>&);
    void removeMedias(const QStringList&);
    bool hasMedia(const QString&);
    bool hasMediaBaseName(const QString&);
    void updateMediaBaseName(const QString&, const QString&);
    bool isMediaSeen(const QString&);
    void setMediaSeen(const QStringList&, bool checked = true);
    int getNMediaSeen();

    QStringList queryMediaNames();
    QSqlQuery queryMedias(QSqlQuery&);
    QSqlQuery queryMediaWhere(QSqlQuery&, QString&, QString&);
    QSqlQuery queryImdbInfoWhere(QSqlQuery&, QString&, QString&);
    QStringList queryMediaWithNoImdbInfo();
    QStringList getCoverList();
    StatsImdb getImdbStats();
    QString getMediaFullName(QString);
    int getMediaId(QString&);

    QStringList getMediaTagList(int);
    QStringList getMediaTagList(QString&);
    QStringList getTagList();
    QStringList getOrphanTagList();
    void insertTag(QString&);
    void addTagToMedia(QString&, QString);
    void removeTagFromMedia(QString&, QString);
    void purgeOrphanTags();

    void insertMovieMedia(MovieMedia);
    bool hasImdbInfo(const QString&);

};


#endif // DATABASEMANAGER_H
