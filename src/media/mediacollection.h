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


#ifndef MEDIACOLLECTION_H
#define MEDIACOLLECTION_H

#include <QStandardItemModel>

#include "../databasemanager.h"
#include "media.h"


class MediaCollection : public QObject {

    Q_OBJECT

    // Constructors
    MediaCollection();
    virtual ~MediaCollection();

    // Fields
    static MediaCollection *singleton_;
    QMap<int, Media> mediaMap_;
    unsigned int nMedia_;


public:
    // Operations
    static MediaCollection *getInstance();
    static void kill();
    void updateMediaCollection(QStringList&) ;

    // Accessor Methods
    unsigned int getNMedia() const;


signals:
    void startUpdate(const int);
    void stepUpdate(const int);
    void finishedUpdate();
    void messageToStatus(QString);

};

#endif // MEDIACOLLECTION_H
