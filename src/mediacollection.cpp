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


/** \fn void MediaCollection::kill()
  * \brief deletes the unique instance of MediaCollection
  */
void MediaCollection::kill() {
    if (NULL != singleton_) {
        delete singleton_;
        singleton_ = NULL;
    }
}


/** \fn void MediaCollection::init()
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


/** \fn void MediaCollection::updateMediaCollection(QStringList& mediaList)
  * \brief updates the database MediaCollection table with the current list of media
  */
void MediaCollection::updateMediaCollection(QStringList& mediaList) {

    emit messageToStatus(QString(tr("Constructing the media collection...")));

    nMedia_ = mediaList.count();
    emit startUpdate(nMedia_);
    int currentStep = 0;
    QStringList mediaInDb = DatabaseManager::getInstance()->queryMediaNames();
    QString mediaFileName;

    // first step, we check what is in the database and not in the list
    QStringList removeList;
    QStringList tempMediaList = mediaList;
    foreach(mediaFileName, mediaInDb) {
        if(!tempMediaList.contains(mediaFileName)) // let's remove it!
            removeList << mediaFileName;

        currentStep += tempMediaList.removeAll(mediaFileName);
        emit stepUpdate(currentStep);
    }


    // second step, we check what is in the list and not in the database
    QList<Media> insertList;
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
            insertList << tempMedia;

            currentStep += tempMediaList.removeAll(mediaFileName);
            emit stepUpdate(currentStep);
    }

    // updating the database
    DatabaseManager::getInstance()->removeMedias(removeList);
    DatabaseManager::getInstance()->insertMedias(insertList);
    emit finishedUpdate();
}


///////////////////////
// accessors methods //
///////////////////////
/** \fn int MediaCollection::getNMedia() const
 *  \brief Returns the number of Media files.
 *  \return the number of Media files
 */
unsigned int MediaCollection::getNMedia() const {
    return nMedia_;
}



