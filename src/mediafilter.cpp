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

#include "mediafilter.h"


/** \var static const int FILTER_TYPE_TEXT
  * \var static const int FILTER_TYPE_BOOL
  * \brief filters types (text or bool)
  */
static const int FILTER_TYPE_TEXT = 0;
static const int FILTER_TYPE_BOOL = 1;

/** \var static const QStringList mediaFilters_
  * \brief list of the filters available
  */
static const QStringList filterNames_ =
        QStringList() << qApp->tr("Title") << qApp->tr("Seen") << qApp->tr("Tag");

/** \var static const int filterTypes_
  * \brief array of the filters types (text or bool)
  */
static const int filterTypes_[] =
        {FILTER_TYPE_TEXT, FILTER_TYPE_BOOL, FILTER_TYPE_TEXT};


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn MediaFilter::MediaFilter()
  * \brief class constructor
  */
MediaFilter::MediaFilter() : model_(new QStringListModel()) {
    model_->setStringList(filterNames_);
}

/** \fn MediaFilter::~MediaFilter()
  * \brief class destructor
  */
MediaFilter::~MediaFilter() {
    delete model_;
}



/////////////
// methods //
/////////////



int MediaFilter::getFilterLimit() const {
    return filterNames_.count();
}

QStringListModel* MediaFilter::getModel() const {
    return model_;
}




