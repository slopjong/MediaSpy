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


static const int MEDIA_TYPE_MOVIE = 0;
static const int MEDIA_TYPE_MUSIC = 1;


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn MediaCollection::MediaCollection()
  * \brief class constructor
  */
MediaCollection::MediaCollection() :
        mediaListModel_(new QStandardItemModel()), databaseManager_(DatabaseManager::getInstance()) {}

/** \fn MediaCollection::~MediaCollection()
  * \brief class destructor
  */
MediaCollection::~MediaCollection() {
    delete mediaListModel_;
}



/////////////
// methods //
/////////////
/** \fn MediaCollection::init()
  * \brief creates Media objects from stored data
  */
void MediaCollection::init() {

    QSqlQuery q;
    databaseManager_->queryMedias(q);

    int fieldId             = q.record().indexOf("id");
    int fieldType           = q.record().indexOf("type");
    int fieldFileName       = q.record().indexOf("fileName");
    int fieldLoaned         = q.record().indexOf("loaned");
    int fieldSeen           = q.record().indexOf("seen");
    int fieldRecommended    = q.record().indexOf("recommended");
    int fieldNotes          = q.record().indexOf("notes");

    while (q.next()) {
        qulonglong id = q.value(fieldId).toULongLong();

        Media tempMedia;
        tempMedia.setId(id);
        tempMedia.setType(q.value(fieldType).toInt());
        tempMedia.setFileName(q.value(fieldFileName).toString());
        tempMedia.setLoaned(q.value(fieldLoaned).toBool());
        tempMedia.setSeen(q.value(fieldSeen).toBool());
        tempMedia.setRecommended(q.value(fieldRecommended).toBool());
        tempMedia.setNotes(q.value(fieldNotes).toString());

        mediaMap_.insert(id, tempMedia);
    }
}

void MediaCollection::update(QStringList& mediaList) {
    QString mediaFileName;
    foreach(mediaFileName, mediaList) {
        QFileInfo mediaFileInfo = QFileInfo(mediaFileName);

        // if the file exists
        if(mediaFileInfo.exists()) {
            // and is not in the database
            if(!databaseManager_->hasMedia(mediaFileName)) {
                Media tempMedia;
                tempMedia.setType(MEDIA_TYPE_MOVIE);
                tempMedia.setFileName(mediaFileName);
                tempMedia.setLoaned(false);
                tempMedia.setSeen(false);
                tempMedia.setRecommended(false);
                tempMedia.setNotes(NULL);

                // let's add it!
                databaseManager_->insertMedia(tempMedia);
            }
            // and is already in the database
            else {
                // let's check if it needs an update!
            }
        }
        // if the file does not exist
        else {
            // and is in the database
            if(!databaseManager_->hasMedia(mediaFileName)) {
                // let's remove it!
            }
            // and is not in the database
            else {
                // let's do nothing!
            }
        }
    }
}


///////////////////////
// accessors methods //
///////////////////////
QStandardItemModel* MediaCollection::getMediaListModel() const {
    return mediaListModel_;
}




