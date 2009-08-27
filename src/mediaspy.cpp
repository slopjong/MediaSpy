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
#include "../build/ui/ui_mediaspy.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn MediaSpy::MediaSpy(QWidget *parent)
  * \brief class constructor
  * \param parent the inherited QWidget object
  */
MediaSpy::MediaSpy(QWidget *parent) :
          QMainWindow(parent)
        , ui_(new Ui::MediaSpy)
        , updateThread_(new UpdateThread(this))
        , mediaListProxyModel_(new myQSortFilterProxyModel(this))
        , statusLabel_(new QLabel(this))
        , filterTitleString_(QString(tr("Search")))
        , filterTagString_(QString(tr("Search tags (comma separated)")))
        , filtersLineEditStyle_(QString("font: italic; color: darkgray;"))
{
    // view settings
    ui_->setupUi(this);
    // progress bar
    ui_->progressBar->setMinimum(0);
    ui_->progressBar->setVisible(false);
    ui_->progressButton->setVisible(false);
    // splitter
    ui_->splitter->setSizes(QList<int>() << ui_->centralWidget->size().width()/2 << ui_->centralWidget->size().width()/2);
    // permanent status bar
    ui_->statusBar->addPermanentWidget(statusLabel_);
    statusLabel_->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    // filters style
    ui_->filterLineEdit->installEventFilter(this);
    ui_->filterLineEdit->setFocus(Qt::MouseFocusReason);
    ui_->filterTagLineEdit->installEventFilter(this);
    ui_->filterTagLineEdit->setStyleSheet(filtersLineEditStyle_);
    ui_->filterTagLineEdit->setText(filterTagString_);
    // seen filter combobox
    static const QStringList filterList = QStringList() << tr("All") << tr("Watched") << tr("Unwatched"); // order is important!
    ui_->filterSeenComboBox->addItems(filterList);
    ui_->filterSeenComboBox->setItemData(0, Qt::TextAlignmentRole, Qt::AlignCenter);




    // program really begins here!
    readSettings();
    init();
    makeConnections();
    updateThread_->start();

    // (light) error management
    if(!(errorMessage_.isEmpty())) {
        qWarning() << errorMessage_;
        QMessageBox::critical(this, tr("Error"), errorMessage_);
    }
}

/** \fn MediaSpy::~MediaSpy()
  * \brief class destructor
  */
MediaSpy::~MediaSpy() {
    delete ui_;
    delete updateThread_;
    delete sqlTableModel_;
    delete mediaListProxyModel_;
    delete statusLabel_;
    InfoManager::getInstance()->kill();
    MediaCollection::getInstance()->kill();
    Collection::getInstance()->kill();
    DatabaseManager::getInstance()->kill();
}



/////////////
// methods //
/////////////
/** \fn void MediaSpy::writeSettings()
  * \brief writes the settings used by MediaSpy
  */
void MediaSpy::writeSettings() {
    QSettings settings;

    settings.beginGroup("MediaSpy");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}


/** \fn void MediaSpy::readSettings()
  * \brief reads the settings used by MediaSpy
  */
void MediaSpy::readSettings() {
    QSettings settings;

    settings.beginGroup("MediaSpy");
    resize(settings.value("size", QSize(800, 600)).toSize());
    move(settings.value("pos", QPoint(0, 0)).toPoint());
    settings.endGroup();
}


/** \fn void MediaSpy::makeConnections()
  * \brief makes the connections used by MediaSpy
  */
void MediaSpy::makeConnections() {
    // for MediaCollection
    connect(MediaCollection::getInstance(), SIGNAL(startUpdate(const int)), this, SLOT(setProgressbarMaximum(const int)));
    connect(MediaCollection::getInstance(), SIGNAL(stepUpdate(const int)), this, SLOT(setProgressbarCurrent(const int)));
    connect(MediaCollection::getInstance(), SIGNAL(finishedUpdate()), this, SLOT(setProgressbarOff()));
    connect(MediaCollection::getInstance(), SIGNAL(messageToStatus(QString)), this, SLOT(displayMessage(QString)));

    // for Collection
    connect(Collection::getInstance(), SIGNAL(messageToStatus(QString)), this, SLOT(displayMessage(QString)));
    connect(ui_->actionRescan_collection, SIGNAL(triggered()), this, SLOT(updateCollections()));

    // for updateThread_
    connect(updateThread_, SIGNAL(finished()), this, SLOT(finishedUpdateThread()) );
    connect(updateThread_, SIGNAL(messageToStatus(QString)), this, SLOT(displayMessage(QString)));

    // for InfoManager
    connect(InfoManager::getInstance()->getImdbThread(), SIGNAL(messageToStatus(QString)), this, SLOT(displayMessage(QString)));
    connect(InfoManager::getInstance()->getImdbThread(), SIGNAL(searchResult(bool, QString)),
            this, SLOT(isMediaFound(bool, QString)));
    connect(InfoManager::getInstance()->getImdbThread(), SIGNAL(startSearch(const int)),
            this, SLOT(setProgressbarMaximum(const int)));
    connect(InfoManager::getInstance()->getImdbThread(), SIGNAL(searchProgress(const int)),
            this, SLOT(setProgressbarCurrent(const int)));
    connect(InfoManager::getInstance()->getImdbThread(), SIGNAL(finished()),
            this, SLOT(setProgressbarOff()));
    connect(ui_->mediaListView, SIGNAL(tagApplied(QString&)),
            InfoManager::getInstance(), SLOT(updateInfo(QString&)));

    // for mediaListView
    connect(ui_->mediaListView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this, SLOT(selectedMovie(QModelIndex, QModelIndex)));
    connect(ui_->mediaListView, SIGNAL(updateMedia()),
            mediaListProxyModel_, SLOT(invalidateProxyModel()));
    connect(ui_->mediaListView, SIGNAL(updateMedia()),
            InfoManager::getInstance(), SLOT(updateStats()));

    // for myqlistview
    connect(ui_->mediaListView->editTagAct_, SIGNAL(triggered()), this, SLOT(editDialog()));
    connect(ui_->actionEdit_information, SIGNAL(triggered()), this, SLOT(editDialog()));
}


/** \fn void MediaSpy::init()
  * \brief Initiates required parts of the program.
  */
void MediaSpy::init() {
    ////////////////////////////
    // local directories init //
    ////////////////////////////
    if (!QDir(getAppDirectory()).exists()) {
        QDir localDir(QDir::homePath());
        if(!localDir.mkdir(getAppDirectory())) {
            errorMessage_ = tr("Cannot create local directory!");
            return;
        }
    }
    if (!QDir(getCssDirectory()).exists()) {
        QDir localDir(QDir::homePath());
        if(!localDir.mkdir(getCssDirectory())) {
            errorMessage_ = tr("Cannot create local style directory!");
            return;
        }
    }
    if (!QDir(getCoverDirectory()).exists()) {
        QDir localDir(QDir::homePath());
        if(!localDir.mkdir(getCoverDirectory())) {
            errorMessage_ = tr("Cannot create local cover directory!");
            return;
        }
        QFile resourceCover(":/icons/defaultCover.png");
        QFile localCover(getCoverDirectory() + getDefaultCoverName());
        resourceCover.open(QIODevice::ReadOnly);
        localCover.open(QIODevice::WriteOnly);
        localCover.write(resourceCover.readAll());
        localCover.close();
        resourceCover.close();
    }

    ///////////////////
    // database init //
    ///////////////////
    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        errorMessage_ = tr("This program needs the SQLITE driver.");
        return;
    }

    QSqlError qError = DatabaseManager::getInstance()->init(this->getAppDirectory() + this->getDbFileName());
    if(qError.type()) {
        errorMessage_ = qError.text();
        return;
    }

    //////////////////////
    // infomanager init //
    //////////////////////
    InfoManager::getInstance(ui_)->init();

    ////////////////////
    // tableView init //
    ////////////////////
    sqlTableModel_ = new myQSqlTableModel(this);
    sqlTableModel_->setTable("Media");

    // sorting the list in alphabetical order without case sensitivity
    mediaListProxyModel_->setSourceModel(sqlTableModel_);
    mediaListProxyModel_->sort(0, Qt::AscendingOrder);
    mediaListProxyModel_->setSortCaseSensitivity(Qt::CaseInsensitive);

    ui_->mediaListView->setModel(mediaListProxyModel_);

    //////////////////////
    // collections init //
    //////////////////////
    Collection::getInstance()->update();
}


/** \fn void MediaSpy::closeEvent(QCloseEvent *event)
  * \brief This event handler is called with the given
  * event when Qt receives a window close request
  * for a top-level widget from the window system.
  */
void MediaSpy::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    writeSettings();
}


/** \fn void MediaSpy::updateCollections(QStringList& dirList)
  * \brief Updates the Collections in a dedicated thread.
  */
void MediaSpy::updateCollections() {
    Collection::getInstance()->update();
    updateThread_->start();
}


bool MediaSpy::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui_->filterTagLineEdit) {
        if (event->type() == QEvent::FocusIn) {
            ui_->filterTagLineEdit->clear();
            ui_->filterTagLineEdit->setStyleSheet(QString());
        }
        if (event->type() == QEvent::FocusOut && ui_->filterTagLineEdit->text().isEmpty()) {
            ui_->filterTagLineEdit->setText(filterTagString_);
            ui_->filterTagLineEdit->setStyleSheet(filtersLineEditStyle_);
        }
    }
    else if (obj == ui_->filterLineEdit) {
        if (event->type() == QEvent::FocusIn) {
            ui_->filterLineEdit->clear();
            ui_->filterLineEdit->setStyleSheet(QString());
        }
        if (event->type() == QEvent::FocusOut && ui_->filterLineEdit->text().isEmpty()) {
            ui_->filterLineEdit->setText(filterTitleString_);
            ui_->filterLineEdit->setStyleSheet(filtersLineEditStyle_);
        }
    }

    return QMainWindow::eventFilter(obj, event);
}



///////////
// slots //
///////////
/** \fn void MediaSpy::on_actionAbout_MediaSpy_triggered()
 *  \brief Shows the MediaSpy About window.
 */
void MediaSpy::on_actionAbout_MediaSpy_triggered() {
    QString myCopyright = QString::fromUtf8(PACKAGE_COPYRIGHTS);
    QMessageBox::about(this, tr("About ") + PACKAGE_NAME,
    QString("<h3>") + PACKAGE_NAME + " " + PACKAGE_VERSION + QString("</h3><p>") + myCopyright +
    tr("<p>MediaSpy is a movie collection cataloging software. Still in heavy development!") +
    QString("<p><a href=\"http://spechard.wordpress.com/\">http://spechard.wordpress.com/</a>"));
}


/** \fn void MediaSpy::on_actionSelectdirectories_triggered()
 *  \brief Opens the CollectionDialog dialog and gets the user's choice into the Collection.
 */
void MediaSpy::on_actionSelectdirectories_triggered() {
    // the model
    QSqlTableModel* model = new QSqlTableModel(this);
    model->setTable("Collection");
    model->removeColumns(tableCollection::id, 1);
    model->select();

    // the view
    CollectionDialog dialog(model, this);
    dialog.listView->setModel(model);

    QSqlDatabase::database().transaction();
    if (dialog.exec() != QDialog::Accepted) {
        QSqlDatabase::database().rollback();
        return;
    }
    QSqlDatabase::database().commit();

    updateCollections();
}


/** \fn void MediaSpy::editDialog()
 *  \brief Opens a EditMediaDialog dialog
 */
void MediaSpy::editDialog() {
    QItemSelectionModel* selectionModel = ui_->mediaListView->selectionModel();
    QModelIndexList indexList = selectionModel->selectedRows();

    if(indexList.count()>0) {
        QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(sqlTableModel_);
        proxyModel->sort(2, Qt::AscendingOrder);
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

        QDataWidgetMapper* mapper = new QDataWidgetMapper(this);
        mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        mapper->setModel(proxyModel);

        EditMediaDialog dialog(indexList, mapper);

        QSqlDatabase::database().transaction();
        if (dialog.exec() != QDialog::Accepted) {
            QSqlDatabase::database().rollback();
            return;
        }
        QSqlDatabase::database().commit();
    }
    return;
}


/** \fn void MediaSpy::finishedUpdateThread()
 *  \brief Defines what is done when the update thread is done.
 */
void MediaSpy::finishedUpdateThread() {
    sqlTableModel_->select();

    // this may freeze window with long table!
    while(sqlTableModel_->canFetchMore())
        sqlTableModel_->fetchMore();

    sqlTableModel_->setList();
    InfoManager::getInstance()->updateMediaCollectionInfo();
}


/** \fn void MediaSpy::selectedMovie(QModelIndex current, QModelIndex previous)
 *  \brief Defines what is done when a movie is selected in the list.
 */
void MediaSpy::selectedMovie(QModelIndex current, QModelIndex previous) {
    Q_UNUSED(previous);

    QString s = QString(current.data().toString());
    QString mediaName = DatabaseManager::getInstance()->getMediaFullName(s);
    if(DatabaseManager::getInstance()->hasImdbInfo(mediaName))
        ui_->infoWebView->setHtml(InfoManager::getInstance()->getInfo(mediaName), QUrl(getCoverDirectory()));
    else {
        QString noImdbView = InfoManager::getInstance()->noInfo();
        ui_->infoWebView->setHtml(noImdbView);
    }
}


/** \fn void MediaSpy::isMediaFound(bool ok, QString fileName)
 *  \brief Sets the key to be checked if info has been found on the media.
 */
void MediaSpy::isMediaFound(bool ok, QString fileName) {
    sqlTableModel_->setKeyTocheck(ok, fileName);
}


/** \fn void MediaSpy::setProgressbarMaximum(const int maximum) const
 *  \brief Sets the maximum of the progress bar.
 *  \param the maximum value of the progress bar.
 */
void MediaSpy::setProgressbarMaximum(const int maximum) const {
    ui_->progressBar->setValue(0);
    ui_->progressBar->setMaximum(maximum);
    ui_->progressBar->setVisible(true);
    ui_->progressButton->setVisible(true);
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
void MediaSpy::setProgressbarOff() {
    ui_->progressBar->setVisible(false);
    ui_->progressButton->setVisible(false);
    QString message = QString(tr("%n movie(s)", "", MediaCollection::getInstance()->getNMedia()));
    displayPermanentMessage(message);
    displayMessage();
}


/** \fn void MediaSpy::displayMessage(QString message)
 *  \brief Displays a message in the status bar.
 */
void MediaSpy::displayMessage(const QString message) {
    ui_->statusBar->showMessage(message);
}


/** \fn void MediaSpy::displayPermanentMessage(QString message)
 *  \brief Displays a message in the permanent status bar.
 */
void MediaSpy::displayPermanentMessage(const QString message) {
    statusLabel_->setText(message);
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

/** \fn const QString MediaSpy::getCssDirectory()
 *  \brief Returns the app directory.
 *  \return the app directory
 */
const QString MediaSpy::getCssDirectory() {
    return cssDirectory;
}

/** \fn const QString MediaSpy::getCoverDirectory()
 *  \brief Returns the app directory.
 *  \return the app directory
 */
const QString MediaSpy::getCoverDirectory() {
    return coverDirectory;
}

/** \fn const QString MediaSpy::getDbFileName()
 *  \brief Returns the name of the database file.
 *  \return the name of the database file
 */
const QString MediaSpy::getDbFileName() {
    return dbFileName;
}

/** \fn const QString MediaSpy::getDefaultCoverName()
 *  \brief Returns the name of the default cover.
 *  \return the name of the default cover
 */
const QString MediaSpy::getDefaultCoverName() {
    return defaultCoverName;
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


/** \fn void MediaSpy::on_progressButton_clicked()
 *  \brief Stops the info research thread.
 */
void MediaSpy::on_progressButton_clicked() {
    updateThread_->quit();
    InfoManager::getInstance()->getImdbThread()->quit();
    displayMessage();
}


/** \fn void MediaSpy::on_filterLineEdit_textChanged(QString newString)
  *  \brief Puts the text into the filter.
  */
void MediaSpy::on_filterLineEdit_textChanged(QString newString) {
    if(newString != filterTitleString_)
        mediaListProxyModel_->setFilterRegExp(QRegExp(newString, Qt::CaseInsensitive, QRegExp::FixedString));
}


/** \fn void MediaSpy::on_filterSeenComboBox_currentIndexChanged(int index)
  *  \brief Changes the index of the model depending of the choice made by in combo box.
  */
void MediaSpy::on_filterSeenComboBox_currentIndexChanged(int index) {
    mediaListProxyModel_->setIndexChanged(index);
}
