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
InfoManager::InfoManager() : imdbThread_(new ImdbThread(this)) {}

/** \fn InfoManager::~InfoManager()
  * \brief class destructor
  */
InfoManager::~InfoManager() {
    delete imdbThread_;
}


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
    if(isConnected()) {
        QStringList mediaList = DatabaseManager::getInstance()->queryMediaWithNoImdbInfo();
        imdbThread_->setInfoList(mediaList);

        if(mediaList.count() > 0)
            imdbThread_->start();
    }
    else
        fprintf(stdout, "[ERROR] Not connected");
}


ImdbThread* InfoManager::getImdbThread() const {
    return imdbThread_;
}


void InfoManager::endImdbThread() const {
    imdbThread_->exit();
}


/** \fn InfoManager::isConnected()
  * \brief Returns true if an Internet connection has been detected.
  */
bool InfoManager::isConnected() {

    QTcpSocket socket;
    socket.connectToHost("google.com", 80);

    const int Timeout = 5 * 1000;
    if (!socket.waitForConnected(Timeout)) {
        QMessageBox msgBox; // TODO think about something less intrusive
        msgBox.setText(tr("You seem not to be connected! I can't update your database :-("));
        msgBox.exec();
        return false;
    }

    return true;
}

