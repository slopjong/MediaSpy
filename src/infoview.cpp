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


#include "infoview.h"


/** \var InfoView* InfoView::singleton_
  * \brief pointer to the unique instance of InfoView
  */
InfoView* InfoView::singleton_ = 00;



/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn InfoView::InfoView()
  * \brief class constructor
  */
InfoView::InfoView() {}

/** \fn InfoView::~InfoView()
  * \brief class destructor
  */
InfoView::~InfoView() {}



/////////////
// methods //
/////////////
/** \fn InfoView* InfoView::getInstance()
  * \brief returns the unique instance of InfoView, creates it the first time
  */
InfoView* InfoView::getInstance() {
    if (00 == singleton_)
        singleton_ =  new InfoView;
    return singleton_;
}

/** \fn void InfoView::kill()
  * \brief deletes the unique instance of InfoView
  */
void InfoView::kill() {
    if (00 != singleton_) {
        delete singleton_;
        singleton_ = 00;
    }
}


QString InfoView::init(QWebSettings* webSettings) {
    webSettings_ = webSettings;
    webSettings_->setUserStyleSheetUrl(QUrl::fromEncoded("qrc:/templates/default.css"));
    QString initView = QString("<html><body><h1>%1</h1></body></html>").arg(tr("Welcome!"));
    return initView;
}


QString InfoView::header() {
    QString headerOut = "<html><body>";
    return headerOut;
}


QString InfoView::footer() {
    QString footerOut = "</body></html>";
    return footerOut;
}


// add link to imdb pages (film, actors, director, etc.)
QString InfoView::getImdbInfo(QString& mediaName) {
    MovieMedia* m = new MovieMedia(mediaName);
    m->getImdbInfoFromDb();

    QString view = QString("<h1>%1 (%2)</h1>").arg(m->getTitle()).arg(m->getYear());
    view        += QString("<img src=\"%1\" />").arg(m->getImage());
    view        += QString("<p class=\"plot\">%1</p>").arg(m->getPlot());
    view        += QString("<hr /><p><span class=\"key\">%1</span> %2</p>").arg(tr("Genre:")).arg(m->getGenre());
    view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("Runtime:")).arg(m->getRuntime());
    view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("With:")).arg(m->getCast());
    view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("Director:")).arg(m->getDirector());
    view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("Country:")).arg(m->getCountry());
    view        += QString("<p><span class=\"key\">%1</span> %2/10</p>").arg(tr("Rating:")).arg(m->getRating());
    view        += QString("<p><span class=\"key\">%1</span> %2</p>").arg(tr("Image:")).arg(m->getImage());

    return header() + view + footer();
}


QString InfoView::noImdbInfo() {
    QString view = QString("<h1>%1 :-(</h1>").arg(tr("No imdb info available!"));
    return header() + view + footer();
}






