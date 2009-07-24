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


#ifndef INFOIMDB_H
#define INFOIMDB_H

#include <QtGui>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "databasemanager.h"
#include "infosupplier.h"
#include "moviemedia.h"


class InfoImdb : public InfoSupplier {
    Q_OBJECT

    // Operations
    bool processSearchPage(QNetworkReply*);
    bool processMoviePage(QNetworkReply*);
    void searchRedirectedToMoviePage(const QUrl&, const QUrl&);
    QString url2MediaName(const QUrl&);
    unsigned int url2Id(const QUrl&);
    MovieMedia getInfoFromId(unsigned int);
    void makeRequest(QString&);

    // Fields
    QNetworkAccessManager* networkManager_;
    unsigned int nRequests_;
    QMap<QString, QNetworkReply*> replyMap_;


public:
    // Constructors
    InfoImdb();
    virtual ~InfoImdb();

    // Operations
    void searchImdb(QString&);
    void getMoviePage(unsigned int);


private slots:
    void finishReply(QNetworkReply*);

};

#endif // INFOIMDB_H
