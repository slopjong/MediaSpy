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


static const QString defaultPlayer = "mplayer";

class Options : public QObject {
    Q_OBJECT;

    // Fields
    MediaSpy* parent_;

    // option fields
    QString player_;


public:
    Options(MediaSpy* parent = 0);
    virtual ~Options();
    // Operations
    void writeOptions();
    void readOptions();

    // option get/set
    QString getPlayer() const;
    void setPlayer(const QString);


signals:
    void updated();

};

#endif // OPTIONS_H
