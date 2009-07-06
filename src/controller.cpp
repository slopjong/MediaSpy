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


#include "controller.h"
#include "mediaspy.h" // to avoid circular reference


/** \var Controller* Controller::singleton_
  * \brief pointer to the unique instance of Controller
  */
Controller* Controller::singleton_ = NULL;


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn Controller::Controller()
  * \brief class constructor
  */
Controller::Controller(MediaSpy* mediaSpy) :
        view_(mediaSpy), databaseManager_(DatabaseManager::getInstance()),
        collection_(new Collection()), mediaCollection_(new MediaCollection()) {
}

/** \fn Controller::~Controller()
  * \brief class destructor
  */
Controller::~Controller() {
    databaseManager_->kill();
    delete collection_;
    delete mediaCollection_;
}


/////////////
// methods //
/////////////
/** \fn Controller::getInstance()
  * \brief returns the unique instance of Controller, creates it the first time
  */
Controller* Controller::getInstance(MediaSpy* mediaSpy) {
    if (NULL == singleton_)
        singleton_ =  new Controller(mediaSpy);
    return singleton_;
}

/** \fn Controller::kill()
  * \brief deletes the unique instance of Controller
  */
void Controller::kill() {
    if (NULL != singleton_) {
        delete singleton_;
        singleton_ = NULL;
    }
}


/** \fn Controller::init()
  * \brief initiates the controller with the database,
  */
void Controller::init() {

    //////////////////////////
    // local directory init //
    //////////////////////////
    if (!QDir(view_->getAppDirectory()).exists()) {
        QDir localDir(QDir::homePath());
        if(!localDir.mkdir(view_->getAppDirectory())) {
            errorMessage_ = qApp->tr("Cannot create local directory!");
            return;
        }
    }

    ///////////////////
    // database init //
    ///////////////////
    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        errorMessage_ = qApp->tr("This program needs the SQLITE driver.");
        return;
    }

    QSqlError qError = databaseManager_->init(view_->getAppDirectory() + view_->getDbFileName());
    if(!qError.type()) {
    // populating table view
    }
    else {
        errorMessage_ = qError.text();
        return;
    }

    ///////////////////////////////
    // directory collection init //
    ///////////////////////////////
    collection_->init();
    QStringList mediaList = collection_->buildFileList(); // fetch the dir for content

    ///////////////////////////
    // media collection init //
    ///////////////////////////
    mediaCollection_->init();
}


void Controller::addDirCollection(QString& dir) {
    // no need to insert something already present
    if(!databaseManager_->hasDir(dir)) {
        collection_->addDirectory(dir);
        QStringList mediaList = collection_->buildFileList();
        mediaCollection_->update(mediaList);
    }
    return;
}


void Controller::removeDirCollection(QString& dir) {
    // no need to insert something not present
    if(databaseManager_->hasDir(dir)) {
        collection_->removeDirectory(dir);
        QStringList mediaList = collection_->buildFileList();
        mediaCollection_->update(mediaList);
    }

    return;
}






///////////////////////
// accessors methods //
///////////////////////
QString Controller::getErrorMessage() {
    return errorMessage_;
}


void Controller::setCollectionModel(CollectionDialog &dialog) {
    dialog.directoryListView->setModel(collection_->getDirModel());
}

void Controller::setMediaListModel(QListView* listView) {
    listView->setModel(mediaCollection_->getMediaListModel());
}




