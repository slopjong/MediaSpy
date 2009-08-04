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


#ifndef UPDATETHREAD_H
#define UPDATETHREAD_H

#include <QThread>
#include <QStringList>

#include "collection.h"
#include "media/mediacollection.h"


class UpdateThread : public QThread {
    Q_OBJECT

public:
    explicit UpdateThread(QObject* parent = 00);
    virtual ~UpdateThread();


protected:
    virtual void run();


signals:
    void messageToStatus(QString);

};

#endif // UPDATETHREAD_H
