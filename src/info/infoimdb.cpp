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
#include "../mediaspy.h"


static const QString searchPrefix = "http://www.imdb.com/find?s=all&q=";
static const QString titlePrefix  = "http://www.imdb.com/title/";
static const QString mediaPrefix  = "http://ia.media-imdb.com/images/";



/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn InfoImdb::InfoImdb()
  * \brief class constructor
  */
InfoImdb::InfoImdb() :
    networkManager_(new QNetworkAccessManager(this))
{
    connect(networkManager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishReply(QNetworkReply*)));
}

/** \fn InfoImdb::InfoImdb(unsigned int nMedia)
  * \brief class constructor
  * \param nMedia the number of media to be created
  */
InfoImdb::InfoImdb(int nMedia) :
    networkManager_(new QNetworkAccessManager(this))
    , movieMedia_(new MovieMedia[nMedia])
    , iMedia_(0)
{
    connect(networkManager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishReply(QNetworkReply*)));
}

/** \fn InfoImdb::~InfoImdb()
  * \brief class destructor
  */
InfoImdb::~InfoImdb() {
    delete networkManager_;
    delete[] movieMedia_;
}



//////////////
// methods //
//////////////
/** \fn void InfoImdb::searchImdb(QString& mediaName)
  * \brief launches the imdb research for the \var mediaName media
  * \param mediaName the name of the media to be searched
  */
void InfoImdb::searchImdb(QString& mediaName) {
    Q_ASSERT(!mediaName.isEmpty());

    movieMedia_[iMedia_].getInfoFromMediaName(mediaName);
    QString imdbRequestName = movieMedia_[iMedia_].getBaseName();
    imdbRequestName.replace('&', "");
    imdbRequestName.replace(' ', "+");

    QString url = QString(searchPrefix + imdbRequestName);
    makeRequest(url, iMedia_);
    ++iMedia_;
}


/** \fn void InfoImdb::getMoviePage(unsigned int id, int movieMediaIndex)
  * \brief gets the page of the movie identified by its imdb id
  * \param imdbId the imdb id of the movie
  * \param movieMediaIndex the index of the MovieMedia object in the map
  */
void InfoImdb::getMoviePage(unsigned int imdbId, int movieMediaIndex) {

    if(imdbId == 0) {
        emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // signals for page
        emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // and image request
        return;
    }
    // id is padded to 7 char to avoid infinite redirection loop
    QString url = QString(titlePrefix + "tt" + QString("%1/").arg(imdbId, 7, 10, QLatin1Char('0')));
    makeRequest(url, movieMediaIndex);
}


/** \fn void InfoImdb::makeRequest(QString& url, int movieMediaIndex)
  * \brief makes the request
  * \param url the url of the request to be done
  * \param movieMediaIndex the index of the MovieMedia object in the map
  */
void InfoImdb::makeRequest(QString& url, int movieMediaIndex) {
    QNetworkReply* reply = networkManager_->get(QNetworkRequest(QUrl(url)));
    replyMap_.insert(movieMediaIndex, reply);
}


void InfoImdb::copyImage(QString& url, const QString localFileName) {
    QNetworkReply* reply = networkManager_->get(QNetworkRequest(QUrl(url)));
    imageMap_.insert(localFileName, reply);
}


/** \fn void InfoImdb::finishReply(QNetworkReply* networkReply)
  * \brief this slot is activated when a network request for a page is terminated
  * \param networkReply the network reply containing the page
  */
void InfoImdb::finishReply(QNetworkReply* networkReply) {

    if(!replyOk(networkReply)) {
        emit replyError();
        return;
    }

    if(networkReply->url().toString().contains(searchPrefix)) { // this was a searchImdb() request
        // get the movieMedia index from one of the map
        QList<int> movieMediaKeys = replyMap_.keys(networkReply);
        int movieMediaIndex = movieMediaKeys.last();

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
            redirectSearchToMoviePage(possibleRedirectUrl.toUrl(), urlRedirectedTo, replyMap_.key(networkReply));
            emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName());
        }
        else {
            urlRedirectedTo.clear();
            // process the search page
            bool ok = processSearchPage(networkReply);
            emit searchFinished(ok, movieMedia_[movieMediaIndex].getFileName());
        }
    }
    else if(networkReply->url().toString().contains(titlePrefix)) { // this was a getMoviePage() request
        // get the movieMedia index from one of the map
        QList<int> movieMediaKeys = replyMap_.keys(networkReply);
        int movieMediaIndex = movieMediaKeys.last();

        // process the movie page
        bool ok = processMoviePage(networkReply);
        emit searchFinished(ok, movieMedia_[movieMediaIndex].getFileName());
    }
    else if(networkReply->url().toString().contains(mediaPrefix)) { // this was a copyImage() request
        QString imageFileName = imageMap_.key(networkReply);

        if(!imageFileName.isEmpty()) {
            QFile file(imageFileName);
            file.open(QIODevice::WriteOnly);
            file.write(networkReply->readAll());
            file.close();
            emit searchFinished(true, imageFileName);
        }
        else
            emit searchFinished(false, imageFileName);
    }
    else { // this was a strange request! Please, do nothing stupid with it!
        emit searchFinished(false, "");
        return;
    }

    networkReply->close();
    networkReply->deleteLater();
}


/** \fn bool InfoImdb::processSearchPage(QNetworkReply* networkReply)
  * \brief processes an imdb research page
  * \param networkReply the network reply containing the page
  */
bool InfoImdb::processSearchPage(QNetworkReply* networkReply) {
    // get the movieMedia index from the map
    QList<int> movieMediaKeys = replyMap_.keys(networkReply);
    int movieMediaIndex = movieMediaKeys.last();

    // from QIODevice to QString
    QTextStream* textStream = new QTextStream(networkReply);
    QString line;

    do { // looking for special lines
        line = textStream->readLine();

        if(line.contains("<p style=\"margin:0 0 0.5em 0;\"><b>Media from")) {
            // regexp
            QString linkString;
            QRegExp linkRegExp("(.*)<a href=\"/title/tt(.*)/\" onclick=(.*)");
            if (linkRegExp.indexIn(line) != -1)
                linkString = linkRegExp.cap(2);

            int mediaImdbId = linkString.toInt();
            getMoviePage(mediaImdbId, movieMediaIndex);
            return true;
        }
        else if(line.contains("<b>No Matches.</b>")) {
            emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // signals for page
            emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // and image request
            return false;
        }
        else if(line.contains("(Approx Matches)</b>")) {
            emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // signals for page
            emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // and image request
            return false;
        }
        else if(line.contains("Enter a word or phrase to search on.")) {
            emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // signals for page
            emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // and image request
            return false;
        }
        else if(line.contains("<h2>Popular Results</h2>")) {
            emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // signals for page
            emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // and image request
            return false;
        }
        else if(line.contains("<b>Names (Exact Matches)</b>")) {
            emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // signals for page
            emit searchFinished(false, movieMedia_[movieMediaIndex].getFileName()); // and image request
            return false;
        }
    } while (!line.isNull());

    delete textStream;
    return false;
}


/** \fn bool InfoImdb::processMoviePage(QNetworkReply* networkReply)
  * \brief processes an imdb movie page
  * \param networkReply the network reply containing the page
  */
bool InfoImdb::processMoviePage(QNetworkReply* networkReply) {

    // get the movieMedia index from the map
    QList<int> movieMediaKeys = replyMap_.keys(networkReply);
    int movieMediaIndex = movieMediaKeys.last();

    // from QIODevice to QString
    QTextStream* textStream = new QTextStream(networkReply);
    QString line;

    // imdbId regular expression
    QString imdbIdString;
    QRegExp imdbIdRegExp("<link rel=\"canonical\" href=\"http://www.imdb.com/title/tt(.*)/\" />");

    // title and year regexp
    QString titleString;
    QString yearString;
    QRegExp titleYearRegExp("<title>(.*) \\((\\d{4})\\)</title>");

    // genre regexp
    QString genreString;
    QRegExp genreRegExp("<a href=\"/Sections/Genres/(\\w*)/\">(\\w*)</a>");

    // runtime regexp
    QString runtimeString;
    QRegExp runtimeRegExp("<h5>Runtime:</h5>");
    QRegExp runtimeRegExpNL("^(\\d*) min*");
    bool runtimeNextLine = false;

    // rating regexp
    QString ratingString;
    QRegExp ratingRegExp("<b>(.*)/10</b>");

    // director regexp
    QString directorString;
    QRegExp directorRegExp("<h5>Director:</h5>");
    QRegExp directorRegExpNL("<a href=\"/name/(.*) onclick=(.*)>(.*)</a><br(.?)/>");
    bool directorNextLine = false;

    // country regexp
    QString countryString;
    QRegExp countryRegExp("<h5>Country:</h5>");
    QRegExp countryRegExpNL("<a href=\"/Sections/Countries/(.*)/\">");
    bool countryNextLine = false;

    // image regexp
    QString imageUrlString;
    QRegExp imageUrlRegExp("<a name=\"poster\" href=(.*)><img border=(.*) src=\"(.*)\" /></a>");

    // cast regexp
    QRegExp castRegExp("<h3>Cast</h3>");
    QList<QStringList> castList;

    // plot regexp
    QString plotString;
    QRegExp plotRegExp("<h5>Plot:</h5>");
    QRegExp plotRegExpNL("(.*)<a class=\"tn15more inline\" href=\"/title/tt(.*)$");
    bool plotNextLine = false;


    do { // looking for special lines to get data
        line = textStream->readLine();

        // next lines regexp
        if(runtimeNextLine)
            if(runtimeRegExpNL.indexIn(line) != -1) {
                runtimeString = runtimeRegExpNL.cap(1);
                runtimeNextLine = false;
            }
        if(directorNextLine)
            if(directorRegExpNL.indexIn(line) != -1) {
                directorString = directorRegExpNL.cap(3);
                directorNextLine = false;
            }
        if(countryNextLine)
            if(countryRegExpNL.indexIn(line) != -1) {
                countryString = countryRegExpNL.cap(1);
                countryNextLine = false;
            }
        if(plotNextLine)
            if(plotRegExpNL.indexIn(line) != -1) {
                plotString = plotRegExpNL.cap(1);
                plotNextLine = false;
            }

        // regexp
        if (imdbIdRegExp.indexIn(line) != -1)
            imdbIdString = imdbIdRegExp.cap(1);
        else if (titleYearRegExp.indexIn(line) != -1) {
            titleString = titleYearRegExp.cap(1);
            yearString = titleYearRegExp.cap(2);
        }
        else if(genreRegExp.indexIn(line) != -1)
            genreString = genreRegExp.cap(2);
        else if(runtimeRegExp.indexIn(line) != -1)
            runtimeNextLine = true;
        else if(ratingRegExp.indexIn(line) != -1)
            ratingString = ratingRegExp.cap(1);
        else if(directorRegExp.indexIn(line) != -1)
            directorNextLine = true;
        else if(countryRegExp.indexIn(line) != -1)
            countryNextLine = true;
        else if(imageUrlRegExp.indexIn(line) != -1)
            imageUrlString = imageUrlRegExp.cap(3);
        else if(castRegExp.indexIn(line) != -1)
            castList = parseHtmlTable( line, true );
        else if(plotRegExp.indexIn(line) != -1)
            plotNextLine = true;

    } while (!line.isNull());

    movieMedia_[movieMediaIndex].setImdbId(imdbIdString.toInt());
    movieMedia_[movieMediaIndex].setGenre(genreString);
    movieMedia_[movieMediaIndex].setYear(yearString.toInt());
    movieMedia_[movieMediaIndex].setRuntime(runtimeString.toInt());
    movieMedia_[movieMediaIndex].setRating(ratingString.toDouble());
    movieMedia_[movieMediaIndex].setDirector(directorString);
    movieMedia_[movieMediaIndex].setCountry(countryString);
    movieMedia_[movieMediaIndex].setPlot(plotString);

    // processing title
    if(!titleString.isEmpty())
        movieMedia_[movieMediaIndex].setTitle(titleString);
    else
        movieMedia_[movieMediaIndex].setTitle(movieMedia_[movieMediaIndex].getBaseName());

    // processing image
    if(imageUrlString.isEmpty() || imageUrlString == ".jpg") {
        movieMedia_[movieMediaIndex].setImageUrl(MediaSpy::getDefaultCoverName());
        emit searchFinished(true, "");
    }
    else {
        movieMedia_[movieMediaIndex].setImageUrl(imageUrlString);
        copyImage(imageUrlString, MediaSpy::getCoverDirectory() + movieMedia_[movieMediaIndex].getImage());
    }

    // processing cast
    QString castString;
    for(int i = 0; i < castList.count(); ++i)
        castString.append(castList.value( i ).value( 1 )).append(", ");
    castString.chop(2);
    movieMedia_[movieMediaIndex].setCast(castString);

    // putting the MovieMedia information in the database
    DatabaseManager::getInstance()->insertMovieMedia(movieMedia_[movieMediaIndex]);

    delete textStream;
    return true;
}


/** \fn void InfoImdb::redirectSearchToMoviePage(const QUrl& requestUrl, const QUrl& movieUrl, int movieMediaIndex)
  * \brief redirects a search page to a movie page
  * \param requestUrl the original url of the search page
  * \param movieUrl the movie page url
  * \param movieMediaIndex the index of the MovieMedia object in the map
  */
void InfoImdb::redirectSearchToMoviePage(const QUrl& requestUrl, const QUrl& movieUrl, int movieMediaIndex) {
    QString requestMediaName = url2MediaName(requestUrl);
    int mediaImdbId = url2Id(movieUrl);
    getMoviePage(mediaImdbId, movieMediaIndex);
}


/** \fn QString InfoImdb::url2MediaName(const QUrl& url)
  * \brief converts a url to the corresponding media name
  * \param url the url to convert
  * \return the name of the media
  */
QString InfoImdb::url2MediaName(const QUrl& url) {
    return url.toString().remove(searchPrefix);
}


/** \fn unsigned int InfoImdb::url2Id(const QUrl& url)
  * \brief converts a url to the corresponding imdb id
  * \param url the url to convert
  * \return the imdb id of the movie
  */
unsigned int InfoImdb::url2Id(const QUrl& url) {
    QString string = url.toString().remove(titlePrefix + "tt");
    unsigned int slashIndex = string.indexOf("/");
    string = string.left(slashIndex);
    return string.toInt();
}


/** \fn QString InfoImdb::toPlainText( const QString& html )
  * \brief converts html string to plain text
  * \author PasNox
  */
QString InfoImdb::toPlainText( const QString& html ) {
    QTextDocument doc;
    doc.setHtml( html );
    return doc.toPlainText();
}


/** \fn QList<QStringList> InfoImdb::parseHtmlTable( const QString& table, bool plainText = true )
  * \brief parses a html table and returns the result
  * \author PasNox
  */
QList<QStringList> InfoImdb::parseHtmlTable( const QString& table, bool plainText = true ) {
    QList<QStringList> mResult;
    // td regexp
    QRegExp tdrx( "<td.*>((.*))</td>" );
    tdrx.setMinimal( true );
    // get tr tags
    QRegExp rx( "<tr.*>(.*)</tr>" );
    rx.setMinimal( true );
    int rxpos = 0;
    while ( ( rxpos = rx.indexIn( table, rxpos ) ) != -1 ) {
        // increment pos
        rxpos += rx.matchedLength();
        // get tr tag
        QString tr = rx.cap( 0 ).simplified();
        // get td tags
        int tdrxpos = 0;
        QStringList row;
        while ( ( tdrxpos = tdrx.indexIn( tr, tdrxpos ) ) != -1 ) {
            // increment pos
            tdrxpos += tdrx.matchedLength();
            // get tr tag
            QString td = tdrx.cap( 1 ).simplified();
            if ( plainText )
                td = toPlainText( td );
            // add value
            row << td;
        }
        if ( !row.isEmpty() )
            mResult << row;
    }
    return mResult;
}

