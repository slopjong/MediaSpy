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


#include "updatethread.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn UpdateThread::UpdateThread(QObject* parent)
  * \brief Constructor.
  */
UpdateThread::UpdateThread(Collection* collection, MediaCollection* mediaCollection, QObject* parent)
        : QThread(parent)
        , collection_(collection)
        , mediaCollection_(mediaCollection)
{}

/** \fn UpdateThread::~UpdateThread()
  * \brief Destructor.
  */
UpdateThread::~UpdateThread() {
    wait();
}



/////////////
// methods //
/////////////
void UpdateThread::run() {
    QStringList mediaList = collection_->buildFileList();
    if(mediaList.count()>0) {
        emit messageToStatus(QString(tr("Updating...")));
        mediaCollection_->updateMediaCollection(mediaList);
    }
}

