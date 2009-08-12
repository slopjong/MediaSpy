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
#include <QTcpSocket>

#include "infoimdb.h"
#include "imdbthread.h"
#include "../databasemanager.h"
#include "../media/moviemedia.h"
#include "../qtElements/networkchecker.h"


class InfoManager : public QObject {
    Q_OBJECT

    // Constructors
    InfoManager();
    virtual ~InfoManager();

    // Fields
    static InfoManager *singleton_;
    ImdbThread* imdbThread_;
    NetworkChecker* checker_;
    bool isConnected_;


public:
    // Operations
    static InfoManager *getInstance();
    static void kill();
    void updateMediaCollectionInfo();
    ImdbThread* getImdbThread() const;


public slots:
    void endImdbThread() const;
    void checkConnection(bool);


signals:
    void messageToStatus(QString);

};

#endif // INFOMANAGER_H
