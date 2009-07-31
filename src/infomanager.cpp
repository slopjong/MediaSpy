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


#include "infomanager.h"


/** \var InfoManager* InfoManager::singleton_
  * \brief pointer to the unique instance of InfoManager
  */
InfoManager* InfoManager::singleton_ = 00;


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn InfoManager::InfoManager()
  * \brief class constructor
  */
InfoManager::InfoManager() : infoImdb_(0) {}

/** \fn InfoManager::~InfoManager()
  * \brief class destructor
  */
InfoManager::~InfoManager() {}


//////////////
// methods //
//////////////
/** \fn InfoManager* InfoManager::getInstance()
  * \brief returns the unique instance of InfoManager, creates it the first time
  */
InfoManager* InfoManager::getInstance() {
    if (00 == singleton_)
        singleton_ =  new InfoManager;
    return singleton_;
}

/** \fn InfoManager::kill()
  * \brief deletes the unique instance of InfoManager
  */
void InfoManager::kill() {
    if (00 != singleton_) {
        delete singleton_;
        singleton_ = 00;
    }
}


void InfoManager::updateMediaCollectionInfo() {
    indexImdbSearch_ = 0;

    // if an ongoing update is still on
    if(infoImdb_ != 0) {
        delete infoImdb_;
        infoImdb_ = 0;
    }

    //    if(connected()) {

    // which media ? TODO: arrange this in order to search only for empty info
    // maybe get the id more than the name?
    QStringList mediaIdWithNoInfoList = DatabaseManager::getInstance()->queryMediaWithNoImdbInfo();
    QString mediaId;

    nImdbSearch_ = mediaIdWithNoInfoList.count();
    if(nImdbSearch_>0) {
        emit messageToStatus(tr("Updating imdb info..."));
        infoImdb_ = new InfoImdb(nImdbSearch_);
        connect(infoImdb_, SIGNAL(searchFinished(bool, QString)), this, SLOT(searchReply(bool, QString)));

        emit(startSearch(nImdbSearch_));
        foreach(mediaId, mediaIdWithNoInfoList) {
            infoImdb_->searchImdb(mediaId);
        }
    }
    //}
}


void InfoManager::searchReply(bool ok, QString reply) {
    emit searchResult(ok, reply);
    emit searchProgress(++indexImdbSearch_);
    if(nImdbSearch_ == indexImdbSearch_)
        emit searchEnd();
}

