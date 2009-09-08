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


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn Options::Options()
  * \brief Class constructor of the Options object. Note that it defines the default values of the options.
  */
Options::Options(MediaSpy* parent)
        : parent_(parent)
        , player_(defaultPlayer) // default option values
{}

/** \fn Options::~Options()
  * \brief Class destructor.
  */
Options::~Options() {}


/////////////
// methods //
/////////////
void Options::readOptions() {
    Q_CHECK_PTR(parent_);
    QSettings settings;

    settings.beginGroup("MediaSpy");
    parent_->resize(settings.value("size", QSize(800, 600)).toSize());
    parent_->move(settings.value("pos", QPoint(0, 0)).toPoint());
    settings.endGroup();

    player_ = settings.value("player", player_).toString();
}


void Options::writeOptions() {
    QSettings settings;

    settings.beginGroup("MediaSpy");
    settings.setValue("size", parent_->size());
    settings.setValue("pos", parent_->pos());
    settings.endGroup();

    settings.setValue("player", player_);
    emit updated();
}



/////////////////////
// getters/setters //
/////////////////////
QString Options::getPlayer() const {return player_;}
void Options::setPlayer(const QString player) {player_ = player;}

