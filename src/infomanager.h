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


#ifndef INFOMANAGER_H
#define INFOMANAGER_H

#include <QtGui>
#include <QNetworkRequest>

#include "databasemanager.h"
#include "moviemedia.h"
#include "infoimdb.h"


class InfoManager : public QObject {
    Q_OBJECT

    // Constructors
    InfoManager();
    ~InfoManager();

    // Fields
    static InfoManager *singleton_;
    InfoImdb* infoImdb_;
    int nImdbSearch_;
    int indexImdbSearch_;

public:
    // Operations
    static InfoManager *getInstance();
    static void kill();
    void updateMediaCollectionInfo();
    void searchImdb(QString);


public slots:
    void searchReply(bool, QString);


signals:
    void messageToStatus(QString);
    void searchResult(bool, QString);
    void startSearch(const int);
    void searchProgress(const int);
    void searchEnd();

};

#endif // INFOMANAGER_H
