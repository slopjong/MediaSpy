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


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn InfoManager::InfoManager()
  * \brief class constructor
  */
InfoManager::InfoManager(MediaCollection* mediaCollection, QString coverDir, Ui_MediaSpy* uiParent)
        : ui_(uiParent)
        , mediaCollection_(mediaCollection)
        , coverDir_(coverDir)
        , imdbThread_(new ImdbThread(this))
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
void InfoManager::init() {
    setFirstPage();

    // init the stats page
    statsSettings_ = ui_->statsWebView->settings();
    statsSettings_->setUserStyleSheetUrl(QUrl::fromEncoded("qrc:/templates/default.css"));
    QString statsView = QString("<html><body><h1>%1</h1></body></html>").arg(tr("Your collection"));
    ui_->statsWebView->setHtml(statsView);
}


// init the info page
void InfoManager::setFirstPage() {
    infoSettings_ = ui_->infoWebView->settings();
    infoSettings_->setUserStyleSheetUrl(QUrl::fromEncoded("qrc:/templates/default.css"));
    QString infoView = QString("<html><body><h1>%1</h1>").arg(tr("Welcome in Mediaspy!"));
    infoView += createFirstPage() + "</body></html>";
    ui_->infoWebView->setHtml(infoView, QUrl(coverDir_));
}


bool InfoManager::updateMediaCollectionInfo() {
    bool stop = false;
    QStringList mediaList = DatabaseManager::getInstance()->queryMediaWithNoImdbInfo();
    imdbThread_->setInfoList(mediaList);

    if(mediaList.count() > 0)
        imdbThread_->start();
    else
        stop = true;

    setFirstPage();
    updateStats();

    return stop;
}


QString InfoManager::getStats() {
    QString view;
    int pourcent    = 0;
    int nMediaSeen  = DatabaseManager::getInstance()->getNMediaSeen();
    int nMedia      = mediaCollection_->getNMedia();
    if(nMedia>0) {
        pourcent    = (100*nMediaSeen)/nMedia;

        // seen/unseen
        view += tr("You've seen %n media(s) on a total of %1.", "", nMediaSeen).arg(nMedia);
        view += "<div style=\"width:100%; height:18px; background-color:#ffc8c8;\">";//255, 200, 200
        view += QString("<div style=\"width:%1%; height:18px; background-color:#adaa4c; border-right:1px white solid;\"></div>").arg(pourcent);
        view += QString("<div style=\"margin-top:-17px; color:black; text-align:center;\">%1%</div>").arg(pourcent);
        view += "</div>";

        // imdb stats
        StatsImdb stats = DatabaseManager::getInstance()->getImdbStats();
        view += QString("<h2>%1 (%2/%3)</h2>").arg(tr("Imdb stats")).arg(stats.nImdbInfo).arg(nMedia);

        view += QString("<h3>%1</h3>").arg(tr("Runtime"));
        view += QString("<p>%1 %2 - %3 %4 - %5 %6</p>").arg(tr("Min:")).arg(stats.minRuntime).arg(tr("Mean:")).arg(stats.avgRuntime).arg(tr("Max:")).arg(stats.maxRuntime);
        // histogram

        // genre
        /*view += QString("<h3>%1</h3>").arg(tr("Genre"));
        // histogram

        // year
        view += QString("<h3>%1</h3>").arg(tr("Year"));*/
        // histogram
    }
    else
        view = QString("<p>%1</p>").arg(tr("...is empty!"));

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
    QString view;
    QStringList coverList = DatabaseManager::getInstance()->getCoverList();

    if(coverList.count()>0) {
        view = "<div class=\"firstpage\">";
        foreach(cover, coverList) {
            QFileInfo file(MediaSpy::getCoverDirectory() + cover);
            if(file.exists())
                view += QString("<img src=\"%1\" />").arg(cover);
        }
    }
    else
        view = QString("<div>%1").arg(tr("No movie :-("));
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

