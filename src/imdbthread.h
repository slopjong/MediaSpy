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


#ifndef IMDBTHREAD_H
#define IMDBTHREAD_H

#include <QThread>

#include "infoimdb.h"


/** \class ImdbThread
  * \brief The thread used to search info on imdb website.
  */
class ImdbThread : public QThread {
    Q_OBJECT

    InfoImdb* infoImdb_;
    int nImdbSearch_;
    int indexImdbSearch_;
    QStringList mediaIdWithNoInfoList_;


public:
    explicit ImdbThread(QObject* parent = 00);
    virtual ~ImdbThread();
    void setInfoList(const QStringList&);


protected:
    virtual void run();


public slots:
    void searchReply(bool, QString);


signals:
    void messageToStatus(QString);
    void startSearch(const int);
    void searchResult(bool, QString);
    void searchProgress(const int);
    void searchEnd();

};

#endif // IMDBTHREAD_H
