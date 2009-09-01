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


#include "options.h"
#include "mediaspy.h"


/** \var Options* Options::singleton_
  * \brief pointer to the unique instance of Options
  */
Options* Options::singleton_ = 0;


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn Options::Options()
  * \brief Class constructor of the Options object. Note that it defines the default values of the options.
  */
Options::Options(MediaSpy* parent)
        : parent_(parent) // default option values
//        , automaticUpdate_(false)
{}

/** \fn Options::~Options()
  * \brief Class destructor.
  */
Options::~Options() {}


/////////////
// methods //
/////////////
/** \fn Options* Options::getInstance()
  * \brief Returns the unique instance of Options, creates it the first time.
  */
Options* Options::getInstance(MediaSpy* parent) {
    if (0 == singleton_)
        singleton_ =  new Options(parent);
    return singleton_;
}


/** \fn Options::kill()
  * \brief Deletes the unique instance of Options.
  */
void Options::kill() {
    if (0 != singleton_) {
        delete singleton_;
        singleton_ = 0;
    }
}


void Options::readOptions() {
    QSettings settings;

//    settings.beginGroup("MediaSpy");
//    parent_->resize(settings.value("size", QSize(800, 600)).toSize());
//    parent_->move(settings.value("pos", QPoint(0, 0)).toPoint());
//    settings.endGroup();

//    automaticUpdate_ = settings.value("AutomaticUpdate", automaticUpdate_).toBool();
}


void Options::writeOptions() {
    QSettings settings;

//    settings.beginGroup("MediaSpy");
//    settings.setValue("size", parent_->size());
//    settings.setValue("pos", parent_->pos());
//    settings.endGroup();

//    settings.setValue("AutomaticUpdate", automaticUpdate_);
}




////////////////////
// option get/set //
////////////////////
//bool Options::automaticUpdate() const {return automaticUpdate_;}
//void Options::setAutomaticUpdate(const bool automaticUpdate) {automaticUpdate_ = automaticUpdate;}

