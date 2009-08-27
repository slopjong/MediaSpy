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


#include "myqsortfilterproxymodel.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
myQSortFilterProxyModel::myQSortFilterProxyModel(QObject *parent)
        : QSortFilterProxyModel(parent)
        , myIndexChanged_(0)
{}

myQSortFilterProxyModel::~myQSortFilterProxyModel() {}



/////////////
// methods //
/////////////
bool myQSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    QModelIndex index = sourceModel()->index(sourceRow, tableMedia::baseName, sourceParent);
    QString mediaName = sourceModel()->data(index).toString();
    QStringList tagsMedia = DatabaseManager::getInstance()->getMediaTagList(mediaName);

    bool baseNameContained = mediaName.contains(filterRegExp());
    bool tagsContained = true;
    QString tagTried;
    foreach(tagTried, tagsSearched_)
        tagsContained = tagsContained && tagsMedia.contains(tagTried);

    if(myIndexChanged_ == 0) // "All"
        return(baseNameContained && tagsContained);
    else if(myIndexChanged_ == 1) // "Watched"
        return (DatabaseManager::getInstance()->isMediaSeen(mediaName) &&
                baseNameContained && tagsContained);
    else if(myIndexChanged_ == 2) // "Unwatched"
        return (!DatabaseManager::getInstance()->isMediaSeen(mediaName) &&
                baseNameContained && tagsContained);
    else
        return(false);
    return(true);
}


bool myQSortFilterProxyModel::filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const {
    Q_UNUSED(sourceParent);
    return(sourceColumn==tableMedia::baseName);
}


//////////////////////
// accessor methods //
//////////////////////
void myQSortFilterProxyModel::setIndexChanged(int index) {
    Q_ASSERT(index==0 || index==1 || index==2);

    myIndexChanged_ = index;
    invalidateFilter();
}


void myQSortFilterProxyModel::addTagToFilter(QString& tag) {
    tagsSearched_.append(tag);
    invalidateFilter();
}


void myQSortFilterProxyModel::removeTagToFilter(QString& tag) {
    tagsSearched_.removeOne(tag);
    invalidateFilter();
}


///////////
// slots //
///////////
void myQSortFilterProxyModel::invalidateProxyModel() {
    invalidateFilter();
}

