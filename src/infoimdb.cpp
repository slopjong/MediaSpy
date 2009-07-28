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
    networkManager_(new QNetworkAccessManager(this))
{
    connect(networkManager_, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishReply(QNetworkReply*)));
}

/** \fn InfoImdb::InfoImdb(unsigned int nMedia)
  * \brief class constructor
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
void InfoImdb::searchImdb(QString& mediaName) {
    movieMedia_[iMedia_].getInfoFromMediaName(mediaName);
    QString url = QString(searchPrefix + movieMedia_[iMedia_].getBaseName());
    makeRequest(url, iMedia_);
    iMedia_++;
}


void InfoImdb::getMoviePage(unsigned int id, int movieMediaIndex) {
    // id is padded to 7 char to avoid infinite redirection loop
    QString url = QString(titlePrefix + "tt" + QString("%1/").arg(id, 7, 10, QLatin1Char('0')));
    makeRequest(url, movieMediaIndex);
}


void InfoImdb::makeRequest(QString& url, int movieMediaIndex) {
    QNetworkReply* reply = networkManager_->get(QNetworkRequest(QUrl(url)));
    replyMap_.insert(movieMediaIndex, reply);
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
        if(!urlRedirectedTo.isEmpty())
            searchRedirectedToMoviePage(possibleRedirectUrl.toUrl(), urlRedirectedTo, replyMap_.key(networkReply));
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
    else // this was a strange request! Please, do nothing stupid with it!
        return;

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

    // get the movieMedia index from the map
    QList<int> movieMediaKeys = replyMap_.keys(source);
    int movieMediaIndex = movieMediaKeys.last();

    // from QIODevice to QString
    QTextStream* textStream = new QTextStream(source);
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
    movieMedia_[movieMediaIndex].setTitle(titleString);
    movieMedia_[movieMediaIndex].setDirector(directorString);
    movieMedia_[movieMediaIndex].setCountry(countryString);
    movieMedia_[movieMediaIndex].setImageUrl(imageUrlString);
    movieMedia_[movieMediaIndex].setPlot(plotString);

    // processing cast
    QString castString;
    for(int i = 0; i < castList.count(); i++)
        castString.append(castList.value( i ).value( 1 )).append(", ");
    castString.chop(2);

    movieMedia_[movieMediaIndex].setCast(castString);

    // putting the MovieMedia information in the database
    DatabaseManager::getInstance()->insertMovieMedia(movieMedia_[movieMediaIndex]);

    return true;
}


void InfoImdb::searchRedirectedToMoviePage(const QUrl& requestUrl, const QUrl& movieUrl, int movieMediaIndex) {
    QString requestMediaName = url2MediaName(requestUrl);
    int mediaImdbId = url2Id(movieUrl);
    getMoviePage(mediaImdbId, movieMediaIndex);
}


QString InfoImdb::url2MediaName(const QUrl& url) {
    return url.toString().remove(searchPrefix);
}


unsigned int InfoImdb::url2Id(const QUrl& url) {
    QString string = url.toString().remove(titlePrefix + "tt");
    unsigned int slashIndex = string.indexOf("/");
    string = string.left(slashIndex);
    return string.toInt();
}


/** \fn QString InfoImdb::toPlainText( const QString& html )
  * \brief convertS html string to plain text
  * \author PasNox
  */
QString InfoImdb::toPlainText( const QString& html ) {
    QTextDocument doc;
    doc.setHtml( html );
    return doc.toPlainText();
}


/** \fn QList<QStringList> InfoImdb::parseHtmlTable( const QString& table, bool plainText = true )
  * \brief parses a html table and return the result
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

