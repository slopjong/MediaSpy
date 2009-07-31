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


#include "infosupplier.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn InfoSupplier::InfoSupplier()
  * \brief class constructor
  */
InfoSupplier::InfoSupplier() {}

/** \fn InfoSupplier::~InfoSupplier()
  * \brief class destructor
  */
InfoSupplier::~InfoSupplier() {}



//////////////
// methods //
//////////////
/** \fn QUrl InfoManager::redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const
  * \brief Check if the URL is empty and that we aren't being fooled into a infinite redirect loop.
  * Code snippet from:
  * http://wiki.forum.nokia.com/index.php/CS001432_-_Handling_an_HTTP_redirect_with_QNetworkAccessManager
  */
QUrl InfoSupplier::redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const {
    QUrl redirectUrl;
    if(!possibleRedirectUrl.isEmpty() && possibleRedirectUrl != oldRedirectUrl)
        redirectUrl = possibleRedirectUrl;
    return redirectUrl;
}


bool InfoSupplier::replyOk(QNetworkReply* networkReply) {
    if(networkReply->error() != QNetworkReply::NoError) {
        qDebug("[%p] Network request failed: %s", networkReply, networkReply->errorString().toAscii().constData());
        emit searchFinished(false, "[ERROR]");
        return false;
    }
    else
        return true;
}

