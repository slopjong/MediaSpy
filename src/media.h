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


#ifndef MEDIA_H
#define MEDIA_H

#include <QString>
#include <QFileInfo>
#include <QSqlQuery>


/** \class Media
  * A Media object represent any type of media found.
  */
class Media {

    // Fields
    qulonglong  id_;
    int         type_;
    QString     baseName_;
    QString     fileName_;
    bool        imdbInfo_;
    bool        loaned_;
    bool        seen_;
    bool        recommended_;
    QString     notes_;


public:
    // Fields
    // Operations
    void getInfoFromMediaName(QString&);

    // Constructors
    Media();
    explicit Media(const Media&);
    virtual ~Media();

    // Accessor Methods
    qulonglong getId() const;
    void setId(const qulonglong);
    int getType() const;
    void setType(const int);
    QString getBaseName() const;
    void setBaseName(const QString&);
    QString getFileName() const;
    void setFileName(const QString&);
    QString getNotes() const;
    void setNotes(const QString&);

    bool hasImdbInfo() const;
    void setImdbInfo(const bool);

    bool isLoaned() const;
    void setLoaned(const bool);
    bool isSeen() const;
    void setSeen(const bool);
    bool isRecommended() const;
    void setRecommended(const bool);


protected:
    // Fields
    // Constructors
    // Accessor Methods
    // Operations
    void generateBaseName();


};

#endif // MEDIA_H
