/*
 * Copyright (C) 2009 Stéphane Péchard.
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

#ifndef COLLECTION_H
#define COLLECTION_H

#include <QString>
#include <QStringList>


/** \class Collection
  *
  */
class Collection : QStringList
{
public:
    // Fields

    // Constructors
    Collection();
    ~Collection();

    // Accessor Methods
    int getSize() const;
    QString getDirAt(int);

    // Operations
    void addDirectory(const QString&);
    void removeDirectory(const QString&);
    void createCollection(const QStringList&);


};

#endif // COLLECTION_H
