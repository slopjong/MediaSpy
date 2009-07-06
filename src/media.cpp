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


#include "media.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn Media::Media()
  * \brief class constructor
  */
Media::Media() {}

/** \fn Media::Media(const Media &media)
  * \brief copy constructor
  */
Media::Media(const Media &media) {
    id_ = media.id_;
    fileName_ = media.fileName_;
}

/** \fn Media::Media()
  * \brief class constructor
  */
//Media::Media(long id, ) {}

/** \fn Media::Media(v &media)
  * \brief class destructor
  */
Media::~Media() {
}



/////////////
// methods //
/////////////



///////////////////////
// accessors methods //
///////////////////////
qulonglong Media::getId() const { return id_; }
void Media::setId(const qulonglong id) { id_ = id; }

int Media::getType() const { return type_; }
void Media::setType(const int type) { type_ = type; }

QString Media::getFileName() const { return fileName_; }
void Media::setFileName(const QString& fileName) { fileName_ = fileName; }

QString Media::getBaseName() const { return baseName_; }
void Media::setBaseName(const QString& baseName) { baseName_ = baseName; }

bool Media::isLoaned() const { return loaned_; }
void Media::setLoaned(const bool loaned) { loaned_ = loaned; }
bool Media::isSeen() const { return seen_; }
void Media::setSeen(const bool seen ) { seen_ = seen; }
bool Media::isRecommended() const { return recommended_; }
void Media::setRecommended(const bool recommended) { recommended_ = recommended; }

QString Media::getNotes() const { return notes_; }
void Media::setNotes(const QString& note) { notes_ = note; }


