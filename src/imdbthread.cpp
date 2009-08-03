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


#include "imdbthread.h"


ImdbThread::ImdbThread(QObject* parent): QThread(parent) {}


ImdbThread::~ImdbThread() {}


void ImdbThread::run() {
    QString mediaId;
    indexImdbSearch_ = 0;

    emit messageToStatus(tr("Updating imdb info..."));
    emit startSearch(3*nImdbSearch_); // because there is 3 requests per media (search - page - image)

    infoImdb_ = new InfoImdb(nImdbSearch_);
    connect(infoImdb_, SIGNAL(searchFinished(bool, QString)), this, SLOT(searchReply(bool, QString)));

    foreach(mediaId, mediaIdWithNoInfoList_)
        infoImdb_->searchImdb(mediaId);

    exec();
}


void ImdbThread::searchReply(bool ok, QString reply) {
    emit searchResult(ok, reply);
    emit searchProgress(++indexImdbSearch_);
    if(indexImdbSearch_ == 3*nImdbSearch_) {
        emit searchEnd();
        quit();
    }
}


void ImdbThread::setInfoList(const QStringList& list) {
    mediaIdWithNoInfoList_ = list;
    nImdbSearch_ = mediaIdWithNoInfoList_.count();
}
