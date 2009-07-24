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


#include "moviemedia.h"
#include "databasemanager.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn MovieMedia::MovieMedia()
  * \brief class constructor
  */
MovieMedia::MovieMedia() {}

/** \fn MovieMedia::MovieMedia(QString& mediaName)
  * \brief class constructor
  */
MovieMedia::MovieMedia(QString& mediaName) {
    setInfoFromMediaName(mediaName);
}

/** \fn MovieMedia::MovieMedia(unsigned int imdbId)
  * \brief class constructor
  */
MovieMedia::MovieMedia(unsigned int imdbId) {
    setInfoFromImdbId(imdbId);
}

/** \fn MovieMedia::MovieMedia(QString& mediaName, unsigned int imdbId)
  * \brief class constructor
  */
MovieMedia::MovieMedia(QString& mediaName, unsigned int imdbId) {
    setInfoFromMediaName(mediaName);
    setInfoFromImdbId(imdbId);
}

/** \fn MovieMedia::~MovieMedia()
  * \brief class destructor
  */
MovieMedia::~MovieMedia() {}



/////////////
// methods //
/////////////
void MovieMedia::setInfoFromImdbId(unsigned int id) {
    QSqlQuery q;
    QString whereName = "mediaId";
    QString idString = QString("%1").arg(id);
    DatabaseManager::getInstance()->queryImdbInfoWhere(q, whereName, idString);

    int fieldImdbId         = q.record().indexOf("ImdbId");
    int fieldGenre          = q.record().indexOf("genre");
    int fieldYear           = q.record().indexOf("year");
    int fieldRuntime        = q.record().indexOf("runtime");
    int fieldRating         = q.record().indexOf("rating");
    int fieldTitle          = q.record().indexOf("title");
    int fieldDirector       = q.record().indexOf("director");
    int fieldCountry        = q.record().indexOf("country");
    int fieldImage          = q.record().indexOf("image");
    int fieldStudio         = q.record().indexOf("studio");
    int fieldCast           = q.record().indexOf("cast");
    int fieldPlot           = q.record().indexOf("plot");
    int fieldNotes          = q.record().indexOf("notes");

    while (q.next()) {
        this->setImdbId(q.value(fieldImdbId).toInt());
        this->setGenre(q.value(fieldGenre).toInt());
        this->setYear(q.value(fieldYear).toInt());
        this->setRuntime(q.value(fieldRuntime).toInt());
        this->setRating(q.value(fieldRating).toDouble());
        this->setTitle(q.value(fieldTitle).toString());
        this->setDirector(q.value(fieldDirector).toString());
        this->setCountry(q.value(fieldCountry).toString());
        this->setImage(q.value(fieldImage).toString());
        this->setStudio(q.value(fieldStudio).toString());
        this->setCast(q.value(fieldCast).toString());
        this->setPlot(q.value(fieldPlot).toString());
        this->setNotes(q.value(fieldNotes).toString());
    }
}




///////////////////////
// accessors methods //
///////////////////////
void MovieMedia::setImdbId(const int imdbId) {imdbId_ = imdbId;}
int MovieMedia::getImdbId() const {return imdbId_;}
void MovieMedia::setGenre(const int genre) {genre_ = genre;}
int MovieMedia::getGenre() const {return genre_;}
void MovieMedia::setYear(const int year) {year_ = year;}
int MovieMedia::getYear() const {return year_;}
void MovieMedia::setRuntime(const int runtime) {runtime_ = runtime;}
int MovieMedia::getRuntime() const {return runtime_;}
void MovieMedia::setRating(const double rating) {rating_ = rating;}
double MovieMedia::getRating() const {return rating_;}
void MovieMedia::setTitle(const QString& title) {title_ = title;}
QString MovieMedia::getTitle() const {return title_;}
void MovieMedia::setDirector(const QString& director) {director_ = director;}
QString MovieMedia::getDirector() const {return director_;}
void MovieMedia::setCountry(const QString& country) {country_ = country;}
QString MovieMedia::getCountry() const {return country_;}
void MovieMedia::setImage(const QString& image) {image_ = image;}
QString MovieMedia::getImage() const {return image_;}
void MovieMedia::setStudio(const QString& studio) {studio_ = studio;}
QString MovieMedia::getStudio() const {return studio_;}
void MovieMedia::setCast(const QString& cast) {cast_ = cast;}
QString MovieMedia::getCast() const {return cast_;}
void MovieMedia::setPlot(const QString& plot) {plot_ = plot;}
QString MovieMedia::getPlot() const {return plot_;}
void MovieMedia::setNotes(const QString& notes) {notes_ = notes;}
QString MovieMedia::getNotes() const {return notes_;}

