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


#ifndef OPTIONS_H
#define OPTIONS_H

#include <QSettings>

class MediaSpy;


class Options {

    Options(MediaSpy* parent = 0);
    virtual ~Options();

    // Fields
    static Options *singleton_;
    MediaSpy* parent_;

    // option fields
    bool automaticUpdate_;


public:
    // Operations
    static Options* getInstance(MediaSpy* parent = 0);
    static void kill();

    void writeOptions();
    void readOptions();

    // option get/set
    bool automaticUpdate() const;
    void setAutomaticUpdate(const bool);


};

#endif // OPTIONS_H
