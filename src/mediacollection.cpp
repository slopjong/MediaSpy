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


#include "mediacollection.h"


static const int MEDIA_TYPE_MOVIE   = 0;
static const int MEDIA_TYPE_MUSIC   = 1;
static const int MEDIA_TYPE_DOC     = 2;


/** \var MediaCollection* MediaCollection::singleton_
  * \brief pointer to the unique instance of MediaCollection
  */
MediaCollection* MediaCollection::singleton_ = NULL;



/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn MediaCollection::MediaCollection()
  * \brief class constructor
  */
MediaCollection::MediaCollection() {}

/** \fn MediaCollection::~MediaCollection()
  * \brief class destructor
  */
MediaCollection::~MediaCollection() {}



/////////////
// methods //
/////////////
/** \fn MediaCollection* MediaCollection::getInstance()
  * \brief returns the unique instance of MediaCollection, creates it the first time
  */
MediaCollection* MediaCollection::getInstance() {
    if (NULL == singleton_)
        singleton_ =  new MediaCollection;
    return singleton_;
}


/** \fn MediaCollection::kill()
  * \brief deletes the unique instance of MediaCollection
  */
void MediaCollection::kill() {
    if (NULL != singleton_) {
        delete singleton_;
        singleton_ = NULL;
    }
}


/** \fn MediaCollection::init()
  * \brief creates Media objects from stored data
  */
void MediaCollection::init() {

    QSqlQuery q;
    DatabaseManager::getInstance()->queryMedias(q);

    int fieldId             = q.record().indexOf("id");
    int fieldType           = q.record().indexOf("type");
    int fieldBaseName       = q.record().indexOf("baseName");
    int fieldFileName       = q.record().indexOf("fileName");
    int fieldLoaned         = q.record().indexOf("loaned");
    int fieldSeen           = q.record().indexOf("seen");
    int fieldRecommended    = q.record().indexOf("recommended");
    int fieldNotes          = q.record().indexOf("notes");

    while (q.next()) {
        qulonglong id = q.value(fieldId).toULongLong() - 1;

        Media tempMedia;
        tempMedia.setId(id);
        tempMedia.setType(q.value(fieldType).toInt());
        tempMedia.setBaseName(q.value(fieldBaseName).toString());
        tempMedia.setFileName(q.value(fieldFileName).toString());
        tempMedia.setLoaned(q.value(fieldLoaned).toBool());
        tempMedia.setSeen(q.value(fieldSeen).toBool());
        tempMedia.setRecommended(q.value(fieldRecommended).toBool());
        tempMedia.setNotes(q.value(fieldNotes).toString());

        mediaMap_.insert(id, tempMedia);
    }
    nMedia_ = mediaMap_.count();
}


void MediaCollection::updateMediaCollection(QStringList& mediaList) {

    nMedia_ = mediaList.count();
    emit startUpdate(nMedia_);
    int currentStep = 0;

    // we get the whole Media data table
    QSqlQuery q;
    DatabaseManager::getInstance()->queryMedias(q);
    int fieldFileName = q.record().indexOf("fileName");

    // first step, we check what is in the database and not in the list
    QStringList tempMediaList = mediaList;
    while (q.next()) {
        QString fileName = q.value(fieldFileName).toString();
        if(!tempMediaList.contains(fileName)) // let's remove it!
            DatabaseManager::getInstance()->removeMedia(fileName);

        currentStep += tempMediaList.removeAll(fileName);
        emit stepUpdate(currentStep);
    }

    // second step, we check what is in the list and not in the database
    QString mediaFileName;
    foreach(mediaFileName, tempMediaList) {
            // let's create it!
            Media tempMedia;
            tempMedia.setType(MEDIA_TYPE_MOVIE);
            tempMedia.setFileName(mediaFileName);
            tempMedia.setLoaned(false);
            tempMedia.setSeen(false);
            tempMedia.setRecommended(false);
            tempMedia.setNotes(NULL);

            // let's add it!
            int mediaId = DatabaseManager::getInstance()->insertMedia(tempMedia);
            mediaMap_.insert(mediaId, tempMedia);

            currentStep += tempMediaList.removeAll(mediaFileName);
            emit stepUpdate(currentStep);
    }
    emit finishedUpdate();
}


///////////////////////
// accessors methods //
///////////////////////
unsigned int MediaCollection::getNMedia() const {
    return nMedia_;
}



