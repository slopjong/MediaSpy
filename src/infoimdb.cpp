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


#include "infoimdb.h"


static const QString searchPrefix = "http://www.imdb.com/find?s=all&q=";
static const QString titlePrefix = "http://www.imdb.com/title/";


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn InfoImdb::InfoImdb()
  * \brief class constructor
  */
InfoImdb::InfoImdb() :
    networkManager_(new QNetworkAccessManager(this)),
    nRequests_(0)
{
    connect(networkManager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishReply(QNetworkReply*)));
}

/** \fn InfoImdb::~InfoImdb()
  * \brief class destructor
  */
InfoImdb::~InfoImdb() {
    delete networkManager_;

    // deleting the MovieMedia objects
    MovieMedia* movieMediaPointer;
    foreach(movieMediaPointer, replyMap_.uniqueKeys())
        delete movieMediaPointer;
}



//////////////
// methods //
//////////////
void InfoImdb::searchImdb(QString& mediaName) {
    // construct a MediaMovie from mediaName
    MovieMedia* movieMedia = new MovieMedia(mediaName);

    QString url = QString(searchPrefix + movieMedia->getBaseName());
    makeRequest(url, movieMedia);
}


void InfoImdb::getMoviePage(unsigned int id, MovieMedia* movieMedia) {
    // id is padded to 7 char to avoid infinite redirection loop
    QString url = QString(titlePrefix + "tt" + QString("%1/").arg(id, 7, 10, QLatin1Char('0')));
    makeRequest(url, movieMedia);
}


void InfoImdb::makeRequest(QString& url, MovieMedia* movieMedia) {
    QNetworkReply* reply = networkManager_->get(QNetworkRequest(QUrl(url)));
    replyMap_.insert(movieMedia, reply);
}


void InfoImdb::finishReply(QNetworkReply* networkReply) {

    if(!replyOk(networkReply))
        return;

    if(networkReply->url().toString().contains(searchPrefix)) { // this was a searchImdb() request

        // Reply is finished! We'll ask for the reply about the Redirection attribute
        // http://doc.trolltech.com/qnetworkrequest.html#Attribute-enum
        // Code snippet from:
        // http://wiki.forum.nokia.com/index.php/CS001432_-_Handling_an_HTTP_redirect_with_QNetworkAccessManager
        QVariant possibleRedirectUrl =
            networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute);

        // We'll deduct if the redirection is valid in the redirectUrl function
        QUrl urlRedirectedTo = redirectUrl(possibleRedirectUrl.toUrl(), urlRedirectedTo);

        // If the URL is not empty, we're being redirected.
        if(!urlRedirectedTo.isEmpty()) {
            searchRedirectedToMoviePage(possibleRedirectUrl.toUrl(), urlRedirectedTo, replyMap_.key(networkReply));
        }
        else {
            urlRedirectedTo.clear();

            // process the search page
            bool ok = processSearchPage(networkReply);
            emit searchFinished(ok);
        }
    }
    else if(networkReply->url().toString().contains(titlePrefix)) { // this was a getMoviePage() request
        // process the movie page
        bool ok = processMoviePage(networkReply);
        emit searchFinished(ok);
    }
    else { // this was a strange request! Please, do nothing stupid with it!
        return;
    }

    networkReply->close();
    networkReply->deleteLater();
}


bool InfoImdb::processSearchPage(QNetworkReply* source) {

    QUrl requestUrl = source->url();
    fprintf(stdout, "Search Page: %s\n", requestUrl.toString().toAscii().constData());

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


bool InfoImdb::processMoviePage(QNetworkReply* source) {

    QUrl requestUrl = source->url();
    fprintf(stdout, "Movie Page: %s\n", requestUrl.toString().toAscii().constData());

    // get the movieMedia object from the map
    QList<MovieMedia*> movieMediaKeys = replyMap_.keys(source);
    MovieMedia* movieMedia = movieMediaKeys.last();

    // from QIODevice to QString
    QTextStream* textStream = new QTextStream(source);
    QString line;
    QString titleString;
    QString titleContent = "<meta name=\"title\" content=\"";

    do { // looking for special lines to get data
        line = textStream->readLine();
        if(line.contains(titleContent)) {
            titleString = line.remove(titleContent);
            titleString.chop(2);
        }

    } while (!line.isNull());

    fprintf(stdout, "The title of the movie is: %s\n", titleString.toAscii().constData());

//    QString i = movieMedia->getTitle();

    movieMedia->setTitle(titleString);

//    fprintf(stdout, "The title of the movie is: %s\n", movieMedia->getTitle().toAscii().constData());

    return true;
}


void InfoImdb::searchRedirectedToMoviePage(const QUrl& requestUrl, const QUrl& movieUrl, MovieMedia* movieMedia) {
    QString requestMediaName = url2MediaName(requestUrl);
    int mediaImdbId = url2Id(movieUrl);
    getMoviePage(mediaImdbId, movieMedia);

    // we have both media name and imdb id, let's fill the database!
//    movieMedia->setImdbInfo();
//    DatabaseManager::getInstance()->insertMovieMedia(movieMedia);
    delete movieMedia;
}


//const MovieMedia* InfoImdb::getInfoFromId(unsigned int id, const MovieMedia* movieMedia) {
//    getMoviePage(id, movieMedia);
//    return movieMedia;
//}


QString InfoImdb::url2MediaName(const QUrl& url) {
    return url.toString().remove(searchPrefix);
}


unsigned int InfoImdb::url2Id(const QUrl& url) {
    QString string = url.toString().remove(titlePrefix + "tt");
    unsigned int slashIndex = string.indexOf("/");
    string = string.left(slashIndex);
    return string.toInt();
}

