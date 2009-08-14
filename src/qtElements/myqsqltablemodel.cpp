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
        , etatList_()
{}

myQSqlTableModel::~myQSqlTableModel() {}


/////////////
// methods //
/////////////
QVariant myQSqlTableModel::data( const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::BackgroundRole) {
        const int indexRow = index.row();

        if(etatList_[indexRow] == 1)
            return QBrush( QColor(255, 200, 200) );
        else if(etatList_[indexRow] == 2)
            return QBrush( QColor(200, 255, 200) );
        else
            return QBrush( Qt::transparent );
    }

    return QSqlTableModel::data( index, role );
}


void myQSqlTableModel::setKeyTocheck( bool ok, const QString& fileName ) {
    QFileInfo fileInfo = QFileInfo(fileName);
    QString keyToCheck = fileInfo.completeBaseName();

    for(int row = 0; row < this->rowCount(); ++row) {
        QModelIndex i = this->index(row, 0);
        const QString indexString = i.data().toString();
        if(keyToCheck == indexString)
            etatList_[row] = (ok) ? 2 : 1;
    }
}


void myQSqlTableModel::setList() {
    for(int row = 0; row < this->rowCount(); ++row)
        etatList_.insert(row, 0);
}

