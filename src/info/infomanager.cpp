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
InfoManager* InfoManager::singleton_ = 00;


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn InfoManager::InfoManager()
  * \brief class constructor
  */
InfoManager::InfoManager(Ui_MediaSpy* uiParent)
        : ui_(uiParent)
        , imdbThread_(new ImdbThread(this))
        , checker_(new NetworkChecker())
{
    connect(checker_, SIGNAL(connexionAvailable(bool)), this, SLOT(checkConnection(bool)));
    checker_->runConnectionTest();
}

/** \fn InfoManager::~InfoManager()
  * \brief class destructor
  */
InfoManager::~InfoManager() {
    delete imdbThread_;
    delete checker_;
}


//////////////
// methods //
//////////////
/** \fn InfoManager* InfoManager::getInstance()
  * \brief returns the unique instance of InfoManager, creates it the first time
  */
InfoManager* InfoManager::getInstance(Ui_MediaSpy* uiParent) {
    if (00 == singleton_)
        singleton_ =  new InfoManager(uiParent);
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


void InfoManager::init() {
    // init the stats page
    statSettings_ = ui_->statsWebView->settings();
    statSettings_->setUserStyleSheetUrl(QUrl::fromEncoded("qrc:/templates/default.css"));
    QString statView = QString("<html><body><h1>%1</h1></body></html>").arg(tr("Your collection"));
    ui_->statsWebView->setHtml(statView);

    // init the imdb page
    imdbSettings_ = ui_->imdbWebView->settings();
    imdbSettings_->setUserStyleSheetUrl(QUrl::fromEncoded("qrc:/templates/default.css"));
    QString imdbView = QString("<html><body><h1>%1</h1></body></html>").arg(tr("Welcome in Mediaspy!"));
    ui_->imdbWebView->setHtml(imdbView);
}


void InfoManager::updateMediaCollectionInfo() {
//    if(isConnected_){
        QStringList mediaList = DatabaseManager::getInstance()->queryMediaWithNoImdbInfo();
        imdbThread_->setInfoList(mediaList);

        if(mediaList.count() > 0)
            imdbThread_->start();
//    }
//    else
//        fprintf(stdout, "[ERROR] Not connected\n");
}




QString InfoManager::header() {
    QString headerOut = "<html><body>";
    return headerOut;
}


QString InfoManager::footer() {
    QString footerOut = "</body></html>";
    return footerOut;
}


// add link to imdb pages (film, actors, director, etc.)
QString InfoManager::getImdbInfo(QString& mediaName) {
    MovieMedia* m = new MovieMedia(mediaName);
    m->getImdbInfoFromDb();

    QString view;

    QString imageFileName = MediaSpy::getCoverDirectory() + m->getImage();
    QFileInfo file(imageFileName);
    if(file.exists())
        view        += QString("<img src=\"%1\" />").arg(m->getImage());
    else
        view        += QString("<img src=\"%1\" />").arg(".default.jpg");

    if(m->getYear() < 1800)
        view        += QString("<h1>%1</h1>").arg(m->getTitle());
    else
        view        += QString("<h1>%1 (%2)</h1>").arg(m->getTitle()).arg(m->getYear());

    if(!m->getPlot().isEmpty())
        view        += QString("<p class=\"plot\">%1</p>").arg(m->getPlot());

    if(!m->getGenre().isEmpty())
        view        += QString("<hr /><p><span class=\"key\">%1</span> %2</p>").arg(tr("Genre:")).arg(m->getGenre());

    if(m->getRuntime()>0)
        view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("Runtime:")).arg(m->getRuntime());

    if(!m->getCast().isEmpty())
        view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("With:")).arg(m->getCast());

    if(!m->getDirector().isEmpty())
        view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("Director:")).arg(m->getDirector());

    if(!m->getCountry().isEmpty())
        view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("Country:")).arg(m->getCountry());

    if(m->getRating()>0)
        view        += QString("<p><span class=\"key\">%1</span> %2/10</p>").arg(tr("Rating:")).arg(m->getRating());

    delete m;
    return header() + view + footer();
}


QString InfoManager::noImdbInfo() {
    QString view = QString("<h1>%1 :-(</h1>").arg(tr("No imdb info available!"));
    return header() + view + footer();
}



///////////
// slots //
///////////
void InfoManager::checkConnection(bool available) {
    isConnected_ = available;
}


void InfoManager::endImdbThread() const {
    imdbThread_->exit();
}



///////////////
// accessors //
///////////////
ImdbThread* InfoManager::getImdbThread() const {
    return imdbThread_;
}

