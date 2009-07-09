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
        ui_(new Ui::MediaSpy)
{
    ui_->setupUi(this);

    // view settings
    ui_->progressBar->setVisible(false);
    ui_->progressBar->setMinimum(0);

    // first connections
    connect(MediaCollection::getInstance(), SIGNAL(startUpdate(const int)), this, SLOT(setProgressbarMaximum(const int)));
    connect(MediaCollection::getInstance(), SIGNAL(stepUpdate(const int)), this, SLOT(setProgressbarCurrent(const int)));
    connect(MediaCollection::getInstance(), SIGNAL(finishedUpdate()), this, SLOT(setProgressbarOff()));

    init();

    if(!(errorMessage_.isEmpty()))
        QMessageBox::critical(this, tr("Error"), errorMessage_);
}

/** \fn MediaSpy::~MediaSpy()
  * \brief class destructor
  */
MediaSpy::~MediaSpy() {
    delete ui_;
    DatabaseManager::getInstance()->kill();
}



/////////////
// methods //
/////////////
/** \fn void MediaSpy::init()
  * \brief Initiates all parts of the program.
  */
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
    Collection::getInstance()->init();
    QStringList mediaList = Collection::getInstance()->buildFileList();

    ///////////////////////////
    // media collection init //
    ///////////////////////////
    MediaCollection::getInstance()->init();
    MediaCollection::getInstance()->updateMediaCollection(mediaList);

    ////////////////////
    // tableView init //
    ////////////////////
    sqlTableModel_ = new QSqlTableModel(this);
    sqlTableModel_->setTable("Media");
    sqlTableModel_->select();
    sqlTableModel_->removeColumns(0, 2);
    sqlTableModel_->removeColumns(1, 5);
    sqlTableModel_->setHeaderData(0, Qt::Horizontal, tr("Title"));

    // sorting the list by alphabetical order without case sensitivity
    mediaListProxyModel_ = new QSortFilterProxyModel(this);
    mediaListProxyModel_->setSourceModel(sqlTableModel_);
    mediaListProxyModel_->sort(0, Qt::AscendingOrder);
    mediaListProxyModel_->setSortCaseSensitivity(Qt::CaseInsensitive);

    ui_->mediaListView->setModel(mediaListProxyModel_);
}


/** \fn void MediaSpy::updateCollections(QStringList& dirList)
  * \brief Updates the Collections.
  */
void MediaSpy::updateCollections(QStringList& dirList) {
    ui_->statusBar->showMessage(QString(tr("Building the list of files...")));
    Collection::getInstance()->update(dirList);
    QStringList mediaList = Collection::getInstance()->buildFileList();

    ui_->statusBar->showMessage(QString(tr("Updating your media Collection...")));
    MediaCollection::getInstance()->updateMediaCollection(mediaList);
    sqlTableModel_->select();
}





///////////
// slots //
///////////
/** \fn void MediaSpy::on_actionAdd_directory_triggered()
 *  \brief Opens the CollectionDialog dialog and gets the user's choice into the Collection.
 */
void MediaSpy::on_actionAdd_directory_triggered() {
    CollectionDialog dialog(this);
    for(int i = 0; i < Collection::getInstance()->getNDir(); i++)
        dialog.listWidget->addItem(Collection::getInstance()->getDirAt(i));

    if (dialog.exec() != QDialog::Accepted)
        return;

    QStringList upCollectionList = dialog.getUpdate();
    updateCollections(upCollectionList);
}


void MediaSpy::on_actionRebuild_collection_triggered() {

}

/** \fn void MediaSpy::setProgressbarMaximum(const int maximum) const
 *  \brief Sets the maximum of the progress bar.
 *  \param the maximum value of the progress bar.
 */
void MediaSpy::setProgressbarMaximum(const int maximum) const {
    ui_->progressBar->setMaximum(maximum);
    ui_->progressBar->setVisible(true);
}

/** \fn void MediaSpy::setProgressbarCurrent(const int value) const
 *  \brief Sets the current value of the progress bar.
 *  \param the progress bar value
 */
void MediaSpy::setProgressbarCurrent(const int value) const {
    ui_->progressBar->setValue(value);
}

/** \fn void MediaSpy::setProgressbarOff() const
 *  \brief Ends the progress bar and shows a status message with the number of Media in the collection.
 */
void MediaSpy::setProgressbarOff() const {
    ui_->progressBar->setVisible(false);
    QString message = QString(tr("%n movie(s) in the collection", "", MediaCollection::getInstance()->getNMedia()));
    ui_->statusBar->showMessage(message);
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



//////////////////////
// accessor methods //
//////////////////////
/** \fn const QString MediaSpy::getAppDirectory()
 *  \brief Returns the app directory.
 *  \return the app directory
 */
const QString MediaSpy::getAppDirectory() {
    return appDirectory;
}

/** \fn const QString MediaSpy::getDbFileName()
 *  \brief Returns the name of the database file.
 *  \return the name of the database file
 */
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


