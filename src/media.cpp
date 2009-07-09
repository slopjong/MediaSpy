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
    type_ = media.type_;
    baseName_ = media.baseName_;
    fileName_ = media.fileName_;
    loaned_ = media.loaned_;
    seen_ = media.seen_;
    recommended_ = media.recommended_;
    notes_ = media.notes_;
}

/** \fn Media::~Media()
  * \brief class destructor
  */
Media::~Media() {}



/////////////
// methods //
/////////////
/** \fn void Media::generateBaseName()
 *  \brief Generates the baseName based on the fileName.
 */
void Media::generateBaseName() {
    QFileInfo fileInfo = QFileInfo(fileName_);
    this->setBaseName(fileInfo.completeBaseName());
}




///////////////////////
// accessors methods //
///////////////////////
/** \fn qulonglong Media::getId() const
 *  \brief Returns the id of the media file.
 *  \return the id of the media file
 */
qulonglong Media::getId() const { return id_; }

/** \fn void Media::setId(const qulonglong id)
 *  \brief Sets the id of the media file.
 *  \param the new id of the media file
 */
void Media::setId(const qulonglong id) { id_ = id; }

/** \fn int Media::getType() const
 *  \brief Returns the type of the media file.
 *  \return the type of the media file
 */
int Media::getType() const { return type_; }

/** \fn void Media::setType(const int type)
 *  \brief Sets the type of the media file.
 *  \param the new type of the media file
 */
void Media::setType(const int type) { type_ = type; }

/** \fn QString Media::getFileName() const
 *  \brief Returns the fileName of the media file.
 *  \return the fileName of the media file
 */
QString Media::getFileName() const { return fileName_; }

/** \fn void Media::setFileName(const QString& fileName)
 *  \brief Sets the fileName of the media file.
 *  \param the new fileName of the media file
 */
void Media::setFileName(const QString& fileName) {
    fileName_ = fileName;
    generateBaseName();
}

/** \fn QString Media::getBaseName() const
 *  \brief Returns the baseName of the media file.
 *  \return the baseName of the media file
 */
QString Media::getBaseName() const { return baseName_; }

/** \fn void Media::setBaseName(const QString& fileName)
 *  \brief Sets the baseName of the media file.
 *  \param the new baseName of the media file
 */
void Media::setBaseName(const QString& baseName) { baseName_ = baseName; }

/** \fn bool Media::isLoaned() const
 *  \brief Returns true if the media has been loaned.
 */
bool Media::isLoaned() const { return loaned_; }

/** \fn bool Media::setLoaned()
 *  \brief Sets the media loaned attribute.
 *  \param the attribute value
 */
void Media::setLoaned(const bool loaned) { loaned_ = loaned; }

/** \fn bool Media::isLoaned() const
 *  \brief Returns true if the media has been seen.
 */
bool Media::isSeen() const { return seen_; }

/** \fn bool Media::setSeen()
 *  \brief Sets the media seen attribute.
 *  \param the attribute value
 */
void Media::setSeen(const bool seen ) { seen_ = seen; }

/** \fn bool Media::isRecommended() const
 *  \brief Returns true if the media has been recommended.
 */
bool Media::isRecommended() const { return recommended_; }

/** \fn bool Media::setRecommended()
 *  \brief Sets the media recommendation attribute.
 *  \param the attribute value
 */
void Media::setRecommended(const bool recommended) { recommended_ = recommended; }

/** \fn QString Media::getNotes() const
 *  \brief Returns the notes of the media file.
 *  \return the notes of the media file
 */
QString Media::getNotes() const { return notes_; }

/** \fn void Media::setNotes(const QString& fileName)
 *  \brief Sets the notes of the media file.
 *  \param the new notes of the media file
 */
void Media::setNotes(const QString& note) { notes_ = note; }


