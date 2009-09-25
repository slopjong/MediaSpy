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


#include "myqsqltablemodel.h"

#include <QtDebug>


/////////////////////////////
// constructors/destructor //
/////////////////////////////
myQSqlTableModel::myQSqlTableModel(QObject *parent)
        : QSqlTableModel(parent)
{}

myQSqlTableModel::~myQSqlTableModel() {}


/////////////
// methods //
/////////////
QVariant myQSqlTableModel::data( const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::BackgroundRole) {
        if(this->record(index.row()).value("imdbInfo").toBool())
            return QBrush( Qt::transparent );
        else
            return QBrush( QColor(255, 200, 200) ); // no info (red)
    }

    return QSqlTableModel::data( index, role );
}


Qt::ItemFlags myQSqlTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QSqlTableModel::flags(index) | Qt::ItemIsEditable;
}


bool myQSqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        DatabaseManager::getInstance()->updateMediaBaseName(index.data(Qt::DisplayRole).toString(), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}



///////////
// slots //
///////////
void myQSqlTableModel::updateModel() {
    select();
}

