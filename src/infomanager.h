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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>

#include "databasemanager.h"

class InfoManager : public QObject {
    Q_OBJECT

    // Constructors
    InfoManager();
    ~InfoManager();

    // Fields
    static InfoManager *singleton_;
    QNetworkAccessManager* networkManager_;
    unsigned int nRequests_;

    // Operations
    bool processImdbPage(QNetworkReply*);
    QUrl redirectUrl(const QUrl&, const QUrl&) const;
    void searchRedirectedToMoviePage(const QUrl&);
    QString imdbUrl2MediaName(const QUrl&);


public:
    // Operations
    static InfoManager *getInstance();
    static void kill();
    void updateMediaCollectionInfo();
    void searchImdb(QString);


private slots:
    void finishSearchImdbReply(QNetworkReply*);


signals:
    void messageToStatus(QString);
    void searchImdbFinished(bool);
};

#endif // INFOMANAGER_H
