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


#include <QTableView>

/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn MediaSpy::MediaSpy(QWidget *parent)
  * \brief class constructor
  * \param parent the inherited QWidget object
  */
MediaSpy::MediaSpy(QWidget *parent) :
        QMainWindow(parent),
        ui_(new Ui::MediaSpy),
        collection_(new Collection()),
        mediaCollection_(new MediaCollection())//,
//        sqlTableModel_(new QSqlTableModel())
{
    ui_->setupUi(this);

    // view settings
    ui_->progressBar->setVisible(false);
    ui_->progressBar->setMinimum(0);

//    ui_->mediaListView->sortByColumn(0, Qt::AscendingOrder);
//    ui_->mediaListView->setAlternatingRowColors(true);

    init();


    if(!(errorMessage_.isEmpty()))
        QMessageBox::critical(this, tr("Error"), errorMessage_);
}

/** \fn MediaSpy::~MediaSpy()
  * \brief class destructor
  */
MediaSpy::~MediaSpy() {
    delete ui_;
    delete collection_;
    delete mediaCollection_;
    DatabaseManager::getInstance()->kill();
}



/////////////
// methods //
/////////////
void MediaSpy::init() {
    //////////////////////////
    // local directory init //
    //////////////////////////
    if (!QDir(getAppDirectory()).exists()) {
        QDir localDir(QDir::homePath());
        if(!localDir.mkdir(getAppDirectory())) {
            errorMessage_ = qApp->tr("Cannot create local directory!");
            return;
        }
    }

    ///////////////////
    // database init //
    ///////////////////
    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        errorMessage_ = qApp->tr("This program needs the SQLITE driver.");
        return;
    }

    QSqlError qError = DatabaseManager::getInstance()->init(this->getAppDirectory() + this->getDbFileName());
    if(qError.type()) {
        errorMessage_ = qError.text();
        return;
    }

    ///////////////////////////////
    // directory collection init // TODO in a different thread ??
    ///////////////////////////////
    collection_->init();
    QStringList mediaList = collection_->buildFileList(); // fetch the dir for content TODO update

    ///////////////////////////
    // media collection init //
    ///////////////////////////
    mediaCollection_->init();
    mediaCollection_->updateMediaCollection(mediaList);

    ////////////////////
    // tableView init //
    ////////////////////
    sqlTableModel_ = new QSqlTableModel;
    sqlTableModel_->setTable("Media");
    sqlTableModel_->select();
    sqlTableModel_->removeColumns(0, 2);
    sqlTableModel_->removeColumns(1, 5);
    sqlTableModel_->setHeaderData(0, Qt::Horizontal, tr("Title"));
    ui_->mediaListView->setModel(sqlTableModel_);
}


void MediaSpy::updateCollections(QStringList& dirList) {
    collection_->update(dirList);
    QStringList mediaList = collection_->buildFileList();
    mediaCollection_->updateMediaCollection(mediaList);
    sqlTableModel_->select();
}


void MediaSpy::setProgressbarMaximum(const int n) {
    ui_->progressBar->setMaximum(n);
    ui_->progressBar->setVisible(true);
}

void MediaSpy::setProgressbarCurrent(const int n) const {
    ui_->progressBar->setMaximum(n);
}


//void MediaSpy::tableViewUpdated() {
//    ui_->mediaListView->resizeColumnsToContents();
//    ui_->mediaListView->resizeRowsToContents();
//}



///////////
// slots //
///////////
/** \fn void MediaSpy::on_actionAdd_directory_triggered()
 *  \brief Opens the CollectionDialog dialog and gets the user's choice into the Collection.
*/
void MediaSpy::on_actionAdd_directory_triggered() {
    CollectionDialog dialog(this);
    for(int i = 0; i < collection_->getNDir(); i++)
        dialog.listWidget->addItem(collection_->getDirAt(i));

    if (dialog.exec() != QDialog::Accepted)
        return;

    QStringList upCollectionList = dialog.getUpdate();
    updateCollections(upCollectionList);
}


void MediaSpy::on_actionRebuild_collection_triggered() {

}


/** \fn void MediaSpy::on_actionAbout_MediaSpy_triggered()
 *  \brief Shows the MediaSpy About window.
*/
void MediaSpy::on_actionAbout_MediaSpy_triggered()
{
    QString myCopyright = QString::fromUtf8(PACKAGE_COPYRIGHTS);
    QMessageBox::about(this, tr("About ") + PACKAGE_NAME,
    QString("<h3>") + PACKAGE_NAME + " " + PACKAGE_VERSION + QString("</h3><p>") + myCopyright +
    tr("<p>MediaSpy is a movie collection cataloging software. Still in heavy development!"));

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



/////////////////////
// actions methods //
/////////////////////
/** \fn void MediaSpy::on_actionAbout_Qt_triggered()
 *  \brief Simply shows the built-in Qt About window.
*/
void MediaSpy::on_actionAbout_Qt_triggered() {
    qApp->aboutQt();
}


