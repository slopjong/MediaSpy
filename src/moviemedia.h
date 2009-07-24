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


#ifndef MOVIEMEDIA_H
#define MOVIEMEDIA_H

#include "media.h"

class MovieMedia : public Media {

    // Operations
    void setInfoFromImdbId(unsigned int);

public:
    // Constructors
    MovieMedia();
    MovieMedia(QString&);
    MovieMedia(unsigned int);
    MovieMedia(QString&, unsigned int);
    virtual ~MovieMedia();

    // Accessor Methods
    void setImdbId(const int);
    int getImdbId() const;
    void setGenre(const int);
    int getGenre() const;
    void setYear(const int);
    int getYear() const;
    void setRuntime(const int);
    int getRuntime() const;
    void setRating(const double);
    double getRating() const;
    QString getTitle() const;
    void setTitle(const QString&);
    QString getDirector() const;
    void setDirector(const QString&);
    QString getCountry() const;
    void setCountry(const QString&);
    QString getImage() const;
    void setImage(const QString&);
    QString getStudio() const;
    void setStudio(const QString&);
    QString getCast() const;
    void setCast(const QString&);
    QString getPlot() const;
    void setPlot(const QString&);
    QString getNotes() const;
    void setNotes(const QString&);

//    bool hasImdbInfo() const;
//    void setImdbInfo(const bool);
//
//    bool isLoaned() const;
//    void setLoaned(const bool);
//    bool isSeen() const;
//    void setSeen(const bool);
//    bool isRecommended() const;
//    void setRecommended(const bool);


private:
    // Fields
    int         imdbId_;
    int         genre_;
    int         year_;
    int         runtime_;
    double      rating_;
    QString     title_;
    QString     director_;
    QString     country_;
    QString     image_;
    QString     studio_;
    QString     cast_;
    QString     plot_;
    QString     notes_;

    // Constructors
    // Accessor Methods
    // Operations


protected:
    // Fields
    // Constructors
    // Accessor Methods
    // Operations

};

#endif // MOVIEMEDIA_H
