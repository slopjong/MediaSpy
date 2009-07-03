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


#include "mediaspy.h"
#include "ui_mediaspy.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn MediaSpy::MediaSpy(QWidget *parent)
  * \brief class constructor
  * \param parent the inherited QWidget object
  */
MediaSpy::MediaSpy(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MediaSpy), controller(Controller::getInstance(this)) {

    ui->setupUi(this);

//    readSettings();


    controller->init();
    if(!(controller->getErrorMessage().isEmpty()))
        QMessageBox::critical(this, tr("Error"), controller->getErrorMessage());


}

/** \fn MediaSpy::~MediaSpy()
  * \brief class destructor
  */
MediaSpy::~MediaSpy() {
    delete ui;
    controller->kill();
}



/////////////
// methods //
/////////////
void MediaSpy::readSettings() {
    QSettings settings;

//    if(settings.contains("DatabasePath"))
//        controller->initDatabase(settings.value("DatabasePath").toString());
//    else
//        controller->initDatabase();
}


void MediaSpy::writeDatabasePathSetting() {
    QSettings settings;
//    QString DatabasePath = appDirectory + dbFileName;
//    settings.setValue("DatabasePath", DatabasePath);
}



/////////////////////
// actions methods //
/////////////////////
/** \fn void MediaSpy::on_actionAbout_Qt_triggered()
 *  \brief Simply shows the built-in Qt About window.
*/
void MediaSpy::on_actionAbout_Qt_triggered() {
    qApp->aboutQt();
}



///////////////////////
// accessors methods //
///////////////////////
const QString MediaSpy::getAppDirectory() {
    return appDirectory;
}

const QString MediaSpy::getDbFileName() {
    return dbFileName;
}


Controller* MediaSpy::getController() {
    return controller;
}



///////////
// slots //
///////////
void MediaSpy::on_actionAdd_directory_triggered() {
    CollectionDialog dialog(this);
    
    connect(&dialog, SIGNAL(dirAdded(QString&)), this, SLOT(addDir(QString&)));
    connect(&dialog, SIGNAL(dirRemoved(QString&)), this, SLOT(removeDir(QString&)));
    controller->setCollectionModel(dialog);

    // je fais l'affichage des trucs mais pas de modif dans la bd - TODO

    if (dialog.exec() != QDialog::Accepted)
        return;

    // synchro entre modif et bd
    updateCollection();
}


void MediaSpy::addDir(QString& s) {
    controller->addDirCollection(s);
}


void MediaSpy::removeDir(QString& s) {
    controller->removeDirCollection(s);
}


void MediaSpy::updateCollection() {

}


//////////////////
// about method //
//////////////////
void MediaSpy::on_actionAbout_MediaSpy_triggered()
{
    QString myCopyright = QString::fromUtf8(PACKAGE_COPYRIGHTS);
    QMessageBox::about(this, tr("About ") + PACKAGE_NAME,
    QString("<h3>") + PACKAGE_NAME + " " + PACKAGE_VERSION + QString("</h3><p>") + myCopyright +
    tr("<p>MediaSpy is a movie collection cataloging software. Still in heavy development!"));

}
