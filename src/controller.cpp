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
        view_(mediaSpy), collection_(new Collection()), mediaCollection_(new MediaCollection()) {
}

/** \fn Controller::~Controller()
  * \brief class destructor
  */
Controller::~Controller() {
    DatabaseManager::getInstance()->kill();
    delete collection_;
    delete mediaCollection_;
}


/////////////
// methods //
/////////////
/** \fn Controller::getInstance()
  * \brief returns the unique instance of Controller, creates it the first time
  */
Controller* Controller::getInstance() {
    return singleton_;
}

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

    QSqlError qError = DatabaseManager::getInstance()->init(view_->getAppDirectory() + view_->getDbFileName());
    if(!qError.type()) {
    // populating table view
    }
    else {
        errorMessage_ = qError.text();
        return;
    }

    ///////////////////////////////
    // directory collection init // TODO in a different thread ??
    ///////////////////////////////
    collection_->init();
    QStringList mediaList = collection_->buildFileList(); // fetch the dir for content TODO update

    ///////////////////////////
    // media collection init //
    ///////////////////////////
    mediaCollection_->init();
    mediaCollection_->updateMediaCollection(mediaList);

}


void Controller::populateDirList(CollectionDialog &dialog) {
    for(int i = 0; i < collection_->getNDir(); i++)
        dialog.listWidget->addItem(collection_->getDirAt(i));
}


void Controller::updateCollections(QStringList& dirList) {
    collection_->update(dirList);
    QStringList mediaList = collection_->buildFileList();
    mediaCollection_->updateMediaCollection(mediaList);
}


void Controller::setProgressMax(const int n) const {
    view_->setProgressbarMaximum(n);
}

void Controller::setProgressStep(const int n) const {
    view_->setProgressbarCurrent(n);
}


///////////////////////
// accessors methods //
///////////////////////
QString Controller::getErrorMessage() {
    return errorMessage_;
}


void Controller::setMediaListModel(QTableView* listView) {
    listView->setModel(mediaCollection_->getMediaListModel());
}




