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

#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <QString>
#include <QStandardItemModel>

#include "databasemanager.h"
#include "collection.h"


class MediaSpy;

/** \class Controller
  *
  */
class Controller {
public:
    explicit Controller(MediaSpy*);
    explicit Controller(const Controller &);
    ~Controller();

    void init();
    QString getErrorMessage();
    QStandardItemModel* getCollectionModel();
    void insertDirCollection(QString& s);
    void removeDirCollection(QString& s);


private:
    MediaSpy* view_;
    DatabaseManager* databaseManager_;
    Collection* collection_;
    QStandardItemModel* collectionModel_;
    QString errorMessage_;

    void createCollectionModel();

};

#endif // CONTROLLER_H
