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
#include <QWebSettings>

//#include "infoview.h"
#include "infoimdb.h"
#include "imdbthread.h"
#include "../databasemanager.h"
#include "../media/moviemedia.h"
#include "../media/mediacollection.h"
#include "../qtElements/networkchecker.h"
#include "../../build/ui/ui_mediaspy.h"

//using namespace Ui;

class InfoManager : public QObject {
    Q_OBJECT

    // Constructors
    InfoManager(Ui_MediaSpy* uiParent = 0);
    virtual ~InfoManager();

    // Fields
    Ui_MediaSpy* ui_;
    static InfoManager *singleton_;
    ImdbThread* imdbThread_;
    NetworkChecker* checker_;
    bool isConnected_;
    QWebSettings* imdbSettings_;
    QWebSettings* statsSettings_;

    // Operations
    inline QString htmlHeader();
    inline QString htmlFooter();
    QString getStats();


public:
    // Operations
    static InfoManager *getInstance(Ui_MediaSpy* uiParent = 0);
    static void kill();
    void init();
    void updateMediaCollectionInfo();
    QString noImdbInfo();
    QString getImdbInfo(QString&);
    ImdbThread* getImdbThread() const;
    void updateStats();


public slots:
    void endImdbThread() const;
    void checkConnection(bool);


signals:
    void messageToStatus(QString);

};

#endif // INFOMANAGER_H
