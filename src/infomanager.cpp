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


static const QString ImdbSearchPrefix = "http://www.imdb.com/find?s=all&q=";
static const QString ImdbTitlePrefix = "http://www.imdb.com/title/";


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
InfoManager::InfoManager() :
    networkManager_(new QNetworkAccessManager(this))
{
    connect(networkManager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishSearchImdbReply(QNetworkReply*)));
    nRequests_ = 0;
}

/** \fn InfoManager::~InfoManager()
  * \brief class destructor
  */
InfoManager::~InfoManager() {}


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
    // which media ?
    QStringList mediaWithoutInfoList = DatabaseManager::getInstance()->queryMediaWithImdbInfo();
    QString media;
    foreach(media, mediaWithoutInfoList) {
        searchImdb(media);
    }
}


void InfoManager::searchImdb(QString mediaName) {

//    if(connected()) {
    QString url = QString(ImdbSearchPrefix + mediaName);
    networkManager_->get(QNetworkRequest(QUrl(url)));
//    }
}


void InfoManager::finishSearchImdbReply(QNetworkReply* networkReply) {

    if(networkReply->error() != QNetworkReply::NoError) {
        qDebug() << "[ERROR] Network request failed: " << networkReply->errorString();
        emit searchImdbFinished(false);
        return;
    }

    // Reply is finished! We'll ask for the reply about the Redirection attribute
    // http://doc.trolltech.com/qnetworkrequest.html#Attribute-enum
    // Code snippet from:
    // http://wiki.forum.nokia.com/index.php/CS001432_-_Handling_an_HTTP_redirect_with_QNetworkAccessManager
    QVariant possibleRedirectUrl =
        networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    // We'll deduct if the redirection is valid in the redirectUrl function
    QUrl urlRedirectedTo = redirectUrl(possibleRedirectUrl.toUrl(), urlRedirectedTo);

    // If the URL is not empty, we're being redirected.
    if(!urlRedirectedTo.isEmpty())
        searchRedirectedToMoviePage(possibleRedirectUrl.toUrl(), urlRedirectedTo);
//        this->networkManager_->get(QNetworkRequest(urlRedirectedTo));
    else
        urlRedirectedTo.clear();

    // finally, process the page
    bool ok = processImdbPage(networkReply);
    emit searchImdbFinished(ok);

    networkReply->close();
    networkReply->deleteLater();
}


bool InfoManager::processImdbPage(QNetworkReply* source) {

    QUrl requestUrl = source->url();
    fprintf(stdout, "%s\n", requestUrl.toString().toAscii().constData());

    // from QIODevice to QString
    QTextStream* textStream = new QTextStream(source);
    bool noMatches;
    QString line, keep;

    do { // looking for special lines
        line = textStream->readLine();
        keep += line;
        if(line.contains("<b>No Matches.</b>"))
            noMatches = true;

    } while (!line.isNull());

    // temporary writing files on disk
//    QString s = QString("/tmp/a_%1.html").arg(nRequests_);
//    QFile file(s);
//    file.open(QIODevice::WriteOnly);
//    QTextStream out(&file);
//    out << keep;
//    nRequests_++;


    delete textStream;

    return true;
}


/** \fn QUrl InfoManager::redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const
  * \brief Check if the URL is empty and that we aren't being fooled into a infinite redirect loop.
  * Code snippet from:
  * http://wiki.forum.nokia.com/index.php/CS001432_-_Handling_an_HTTP_redirect_with_QNetworkAccessManager
  */
QUrl InfoManager::redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl) const {
    QUrl redirectUrl;
    if(!possibleRedirectUrl.isEmpty() && possibleRedirectUrl != oldRedirectUrl)
        redirectUrl = possibleRedirectUrl;
    return redirectUrl;
}


void InfoManager::searchRedirectedToMoviePage(const QUrl& requestUrl, const QUrl& fullUrl) {
    QString requestMediaName = imdbUrl2MediaName(requestUrl);
    int mediaImdbId = imdbUrl2Id(fullUrl);
    // we have both media name and imdb id, let's fill the database!
    MovieMedia tempMedia = getImdbInfoFromId(mediaImdbId);
    DatabaseManager::getInstance()->insertMediaImdbInfo();
}


QString InfoManager::imdbUrl2MediaName(const QUrl& url) {
    return url.toString().remove(ImdbSearchPrefix);
}


int InfoManager::imdbUrl2Id(const QUrl& url) {
    QString string = url.toString().remove(ImdbTitlePrefix + "tt");
    string.chop(string.indexOf("/"));
    return string.toInt();
}


