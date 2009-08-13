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


#ifndef MYSORTFILTERPROXYMODEL_H
#define MYSORTFILTERPROXYMODEL_H


#include <QSortFilterProxyModel>

#include "../databasemanager.h"


class mySortFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT;

    // Fields
    int myIndexChanged_;

public:
    // Constructors
    explicit mySortFilterProxyModel(QObject *parent = 0);
    virtual ~mySortFilterProxyModel();

    // Operations
    bool filterAcceptsRow(int, const QModelIndex&) const;

    // Accessor Methods
    void setIndexChanged(int);

};

#endif // MYSORTFILTERPROXYMODEL_H
