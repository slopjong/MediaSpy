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
#include "controller.h"


static const int MEDIA_TYPE_MOVIE   = 0;
static const int MEDIA_TYPE_MUSIC   = 1;
static const int MEDIA_TYPE_DOC     = 2;


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn MediaCollection::MediaCollection()
  * \brief class constructor
  */
MediaCollection::MediaCollection() :
        mediaListModel_(new QStandardItemModel()) {

    mediaListModel_->setHorizontalHeaderLabels(QStringList() << qApp->tr("Title")); // TODO
}

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
    QString mediaFileName;
    int i;
    foreach(mediaFileName, mediaList) {
        QFileInfo mediaFileInfo = QFileInfo(mediaFileName);

        // if the file exists
        if(mediaFileInfo.exists()) {
            Media tempMedia;

            // and is not in the database
            if(!DatabaseManager::getInstance()->hasMedia(mediaFileName)) {

                tempMedia.setType(MEDIA_TYPE_MOVIE);
                tempMedia.setFileName(mediaFileName);
                tempMedia.setLoaned(false);
                tempMedia.setSeen(false);
                tempMedia.setRecommended(false);
                tempMedia.setNotes(NULL);

                // let's add it!
                DatabaseManager::getInstance()->insertMedia(tempMedia);
                mediaMap_.insert(nMedia_, tempMedia);
                nMedia_++;

fprintf(stdout, "adding %s\n", mediaFileName.toAscii().constData());

            }

            // and is already in the database
            else {
                // let's check if it needs an update!
fprintf(stdout, "updating %s ?\n", mediaFileName.toAscii().constData());
            }
        }

        // if the file does not exist
        else {
            // and is in the database
            if(!DatabaseManager::getInstance()->hasMedia(mediaFileName)) {
                // let's remove it!
                DatabaseManager::getInstance()->removeMedia(mediaFileName);
fprintf(stdout, "removing %s\n", mediaFileName.toAscii().constData());
            } // if it is not in the database... let's do nothing!
            else {
fprintf(stdout, "do nothing\n");
            }
        }
//        Controller::getInstance()->setProgressStep(i);
        i++;
    }
    updateListModel();

//    Controller::getInstance()->progressStop();
}


void MediaCollection::updateListModel() {
    mediaListModel_->clear();

    for(unsigned int i = 0; i < (unsigned int)mediaMap_.count(); i++) {
        Media tempMedia = mediaMap_.value(i);
        mediaListModel_->setItem(i, 0, new QStandardItem(QString(tempMedia.getBaseName())));
    }

    Controller::getInstance()->mediaListUpdated();
}


///////////////////////
// accessors methods //
///////////////////////
QStandardItemModel* MediaCollection::getMediaListModel() const {
    return mediaListModel_;
}

unsigned int MediaCollection::getNMedia() const {
    return nMedia_;
}

void MediaCollection::setNMedia(const unsigned int nMedia) {
    nMedia_ = nMedia;
    Controller::getInstance()->setProgressMax(nMedia);
}



