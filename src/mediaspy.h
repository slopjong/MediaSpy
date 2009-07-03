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

#ifndef MEDIASPY_H
#define MEDIASPY_H

#include <QtGui/QMainWindow>
#include <QSettings>
#include <QDir>
#include <QSqlDatabase>
#include <QMessageBox>

#include "controller.h"
#include "collectiondialog.h"



/** \var appDirectory
  * \brief name of the local directory used by the program
  */
static const QString appDirectory   = QDir::homePath() + "/.MediaSpy/";

/** \var dbFileName
  * \brief name of the database file
  */
static const QString dbFileName     = "MediaSpy.sqlite";



namespace Ui {
    class MediaSpy;
}


/** \class MediaSpy
  *
  */
class MediaSpy : public QMainWindow {
    Q_OBJECT

public:
    // Constructors
    explicit MediaSpy(QWidget *parent = 0);
    ~MediaSpy();

    // Accessor Methods
    static const QString getAppDirectory();
    static const QString getDbFileName();
    Controller* getController();

    
private:
    // Fields
    Ui::MediaSpy* ui;
    Controller* controller;

    // Operations
    void readSettings();
    void writeDatabasePathSetting();
    void updateCollection();


private slots:
    void on_actionRebuild_collection_triggered();
    void on_actionAbout_MediaSpy_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionAdd_directory_triggered();
    void addDir(QString&);
    void removeDir(QString&);
};

#endif // MEDIASPY_H
