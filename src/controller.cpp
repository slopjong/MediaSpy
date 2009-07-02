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


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn Controller::Controller()
  * \brief class constructor
  */
Controller::Controller(MediaSpy* mediaSpy) :
        view_(mediaSpy), databaseManager_(new DatabaseManager()),
        collection_(new Collection()), collectionModel_(new QStandardItemModel()) {

    dataTables_ << "Collection"
                << "Media";
}

/** \fn Controller::Controller(const Controller &controller)
  * \brief copy constructor
  */
Controller::Controller(const Controller &controller) :
        databaseManager_(new DatabaseManager()) {
    databaseManager_ = controller.databaseManager_;
    view_ = controller.view_;
}

/** \fn Controller::~Controller()
  * \brief class destructor
  */
Controller::~Controller() {
    delete databaseManager_;
    delete collection_;
    delete collectionModel_;
}





/////////////
// methods //
/////////////
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

    QSqlError qError = databaseManager_->init(dataTables_, view_->getAppDirectory() + view_->getDbFileName());
    if(!qError.type()) {
    // populating table view
    }
    else {
        errorMessage_ = qError.text();
        return;
    }


    /////////////////////
    // collection init //
    /////////////////////
    QStringList stringList = databaseManager_->getCollection(); // 1. read directories in db
    collection_->createCollection(stringList); // 2. put them in collection_
    createCollectionModel();

    // 3. put a QFileSystemWatcher on them
}


void Controller::createCollectionModel() {
    for (int i = 0; i < collection_->getSize(); ++i)
        collectionModel_->setItem(i, new QStandardItem(collection_->getDirAt(i)));
}


void Controller::insertDirCollection(QString& s) {
    // no need to insert something already in the database
    if(!databaseManager_->hasDir(s)) {
        // inside the model
        collectionModel_->setItem(collection_->getSize(), new QStandardItem(s));

        // inside the Collection object
        collection_->addDirectory(s);

        // inside the database
        QSqlError qError = databaseManager_->insertDirToCollection(s);
        if(qError.type()) {
            errorMessage_ = qError.text();
        }
    }
    return;
}


void Controller::removeDirCollection(QString& s) {
    // from the model
    QList<QStandardItem *> listItems = collectionModel_->findItems(s, Qt::MatchExactly, 0 ) ;

    if(listItems.empty() || listItems.size() != 1)
        return;

    QStandardItem* item = listItems[0] ; // the item to be removed...
    collectionModel_->removeRow(item->row()); // ...now!

    // from the Collection object
    collection_->removeDirectory(s);

    // from the database
    QSqlError qError = databaseManager_->removeDirToCollection(s);
    if(qError.type()) {
        errorMessage_ = qError.text();
    }
    return;
}








///////////////////////
// accessors methods //
///////////////////////
QString Controller::getErrorMessage() {
    return errorMessage_;
}

QStandardItemModel* Controller::getCollectionModel() {
    return collectionModel_;
}



