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
    void getInfoFromMediaId(unsigned int);

public:
    // Constructors
    MovieMedia();
    explicit MovieMedia(QString&);
    explicit MovieMedia(unsigned int);
    MovieMedia(const MovieMedia&);
    virtual ~MovieMedia();

    // Accessor Methods
    void setImdbId(const int);
    int getImdbId() const;
    void setGenre(const QString&);
    QString getGenre() const;
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
    QString getImageUrl() const;
    void setImageUrl(const QString&);
    QString getCast() const;
    void setCast(const QString&);
    QString getPlot() const;
    void setPlot(const QString&);

    // Operations
    void printInfo();
    void url2image();
    void getImdbInfoFromDb();

private:
    // Fields
    int         imdbId_;
    QString     genre_;
    int         year_;
    int         runtime_;
    double      rating_;
    QString     title_;
    QString     director_;
    QString     country_;
    QString     image_;
    QString     imageUrl_;
    QString     cast_;
    QString     plot_;

    // Constructors
    // Accessor Methods
    // Operations
    void string2fileName(QString&);


protected:
    // Fields
    // Constructors
    // Accessor Methods
    // Operations

};

#endif // MOVIEMEDIA_H
