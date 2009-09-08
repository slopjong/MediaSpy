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
#include "../mediaspy.h"


/** \var InfoManager* InfoManager::singleton_
  * \brief pointer to the unique instance of InfoManager
  */
InfoManager* InfoManager::singleton_ = 0;


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn InfoManager::InfoManager()
  * \brief class constructor
  */
InfoManager::InfoManager(Ui_MediaSpy* uiParent, QString coverDir)
        : ui_(uiParent)
        , imdbThread_(new ImdbThread(this))
        , coverDir_(coverDir)
{
    connect(imdbThread_, SIGNAL(networkError()), this, SLOT(endImdbThread()));
}

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
InfoManager* InfoManager::getInstance(Ui_MediaSpy* uiParent, QString coverDir) {
    if (0 == singleton_)
        singleton_ = new InfoManager(uiParent, coverDir);
    return singleton_;
}

/** \fn InfoManager::kill()
  * \brief deletes the unique instance of InfoManager
  */
void InfoManager::kill() {
    if (0 != singleton_) {
        delete singleton_;
        singleton_ = 0;
    }
}


void InfoManager::init() {
    // init the info page
    infoSettings_ = ui_->infoWebView->settings();
    infoSettings_->setUserStyleSheetUrl(QUrl::fromEncoded("qrc:/templates/default.css"));
    QString infoView = QString("<html><body><h1>%1</h1>").arg(tr("Welcome in Mediaspy!"));
    infoView += createFirstPage() + "</body></html>";
    ui_->infoWebView->setHtml(infoView, QUrl(coverDir_));

    // init the stats page
    statsSettings_ = ui_->statsWebView->settings();
    statsSettings_->setUserStyleSheetUrl(QUrl::fromEncoded("qrc:/templates/default.css"));
    QString statsView = QString("<html><body><h1>%1</h1></body></html>").arg(tr("Your collection"));
    ui_->statsWebView->setHtml(statsView);
}


void InfoManager::updateMediaCollectionInfo() {
    QStringList mediaList = DatabaseManager::getInstance()->queryMediaWithNoImdbInfo();
    imdbThread_->setInfoList(mediaList);

    if(mediaList.count() > 0)
        imdbThread_->start();

    updateStats();
}


QString InfoManager::getStats() {
    QString view;
    int pourcent    = 0;
    int nMediaSeen  = DatabaseManager::getInstance()->getNMediaSeen();
    int nMedia      = MediaCollection::getInstance()->getNMedia();
    if(nMedia>0)
        pourcent    = (100*nMediaSeen)/nMedia;

    // seen/unseen
    view += tr("You've seen %n media(s) on a total of %1.", "", nMediaSeen).arg(nMedia);
    view += "<div style=\"width:100%; height:18px; background-color:#ffc8c8;\">";//255, 200, 200
    view += QString("<div style=\"width:%1%; height:18px; background-color:#adaa4c; border-right:1px white solid;\"></div>").arg(pourcent);
    view += QString("<div style=\"margin-top:-17px; color:black; text-align:center;\">%1%</div>").arg(pourcent);
    view += "</div>";
/*
    // runtime
    // SELECT MAX(runtime), AVG(runtime), MIN(runtime) FROM ImdbInfo WHERE runtime != 0
    view += QString("<h2>%1</h2>").arg(tr("Runtime"));
    // histogram
    view += QString("<p>%1 %2 - %3 %4 - %5 %6</p>").arg(tr("Min:")).arg(70).arg(tr("Mean:")).arg(80).arg(tr("Max:")).arg(90);

    // genre
    view += QString("<h2>%1</h2>").arg(tr("Genre"));
    // histogram

    // year
    view += QString("<h2>%1</h2>").arg(tr("Year"));
    // histogram
*/

    return htmlHeader() + QString("<h1>%1</h1>").arg(tr("Your collection")) + view + htmlFooter();
}


// TODO add link to imdb pages (film, actors, director, etc.)
QString InfoManager::getInfo(QString& mediaName) {
    MovieMedia* media = new MovieMedia(mediaName);
    media->getImdbInfoFromDb();

    QString infoPage;
    infoPage += htmlHeader();

    infoPage += getImdbInfo(media);
    infoPage += getLocalInfo(media);

    infoPage += htmlFooter();

    delete media;
    return infoPage;
}


QString InfoManager::getLocalInfo(MovieMedia* media) {
    QStringList tagList = media->getTagList();
    QString localInfo = 0;
    if(tagList.count()>0)
        localInfo += "<b>" + tr("Your tags: ") + "</b>" + tagList.join(", ");
    return localInfo;
}


QString InfoManager::getImdbInfo(MovieMedia* media) {
    QString view;

    QString imageFileName = MediaSpy::getCoverDirectory() + media->getImage();
    QFileInfo file(imageFileName);
    if(file.exists())
        view        += QString("<img src=\"%1\" />").arg(media->getImage());
    else
        view        += QString("<img src=\"%1\" />").arg(".default.jpg");

    if(media->getYear() < 1800)
        view        += QString("<h1>%1</h1>").arg(media->getTitle());
    else
        view        += QString("<h1>%1 (%2)</h1>").arg(media->getTitle()).arg(media->getYear());

    if(!media->getPlot().isEmpty())
        view        += QString("<p class=\"plot\">%1</p>").arg(media->getPlot());

    if(!media->getGenre().isEmpty())
        view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("Genre:")).arg(media->getGenre());

    if(media->getRuntime()>0)
        view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("Runtime:")).arg(media->getRuntime());

    if(!media->getCast().isEmpty())
        view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("With:")).arg(media->getCast());

    if(!media->getDirector().isEmpty())
        view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("Director:")).arg(media->getDirector());

    if(!media->getCountry().isEmpty())
        view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("Country:")).arg(media->getCountry());

    if(media->getRating()>0)
        view        += QString("<p><span class=\"key\">%1</span> %2/10</p>").arg(tr("Rating:")).arg(media->getRating());

    view += "<hr />";

    return view;
}


QString InfoManager::noInfo() {
    QString view = QString("<h1>%1 :-(</h1>").arg(tr("No info available!"));
    return htmlHeader() + view + htmlFooter();
}


QString InfoManager::createFirstPage() {
    QString cover;
    QString view = "<div class=\"firstpage\">";
    QStringList coverList = DatabaseManager::getInstance()->getCoverList();
    foreach(cover, coverList) {
        QFileInfo file(MediaSpy::getCoverDirectory() + cover);
        if(file.exists())
            view += QString("<img src=\"%1\" />").arg(cover);
    }

    return view + "</div>";
}


QString InfoManager::htmlHeader() {
    QString headerOut = "<html><body>";
    return headerOut;
}


QString InfoManager::htmlFooter() {
    QString footerOut = "</body></html>";
    return footerOut;
}



///////////
// slots //
///////////
void InfoManager::endImdbThread() const {
    imdbThread_->exit();
}

void InfoManager::updateStats() {
    ui_->statsWebView->setHtml(getStats());
}



///////////////
// accessors //
///////////////
ImdbThread* InfoManager::getImdbThread() const {
    return imdbThread_;
}

