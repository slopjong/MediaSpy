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


#ifndef MEDIAFILTER_H
#define MEDIAFILTER_H

#include <QStringListModel>

#include "databasemanager.h"


class MediaFilter : public QObject {
    Q_OBJECT

public:
    MediaFilter();
    virtual ~MediaFilter();
    QStringListModel* getModel() const;
    int getFilterLimit() const ;

private:
    QStringListModel* model_;

    /** \var static const QStringList filterNames_
      * \brief list of the filters available
      */
    QStringList filterNames_;
};

#endif // MEDIAFILTER_H
