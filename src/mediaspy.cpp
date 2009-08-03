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
          QMainWindow(parent)
        , ui_(new Ui::MediaSpy)
        , updateThread_(new UpdateThread(this))
        , filter_(new MediaFilter())
        , filterLimit_(filter_->getFilterLimit() - 1)
        , nFilter_(0)
{
    Q_CHECK_PTR(ui_);

    // view settings
    ui_->setupUi(this);
    ui_->progressBar->setMinimum(0);
    ui_->splitter->setSizes(QList<int>() << ui_->centralWidget->size().width()/2 << ui_->centralWidget->size().width()/2);

    // program really begins here!
    readSettings();
    init();
    makeConnections();
    updateThread_->start();

    // (light) error management
    if(!(errorMessage_.isEmpty()))
        QMessageBox::critical(this, tr("Error"), errorMessage_);
}

/** \fn MediaSpy::~MediaSpy()
  * \brief class destructor
  */
MediaSpy::~MediaSpy() {
    Q_CHECK_PTR(ui_);
    Q_CHECK_PTR(updateThread_);
    Q_CHECK_PTR(filter_);
    Q_CHECK_PTR(newFilterLabel);
    Q_CHECK_PTR(newFilterComboBox);
    Q_CHECK_PTR(newFilterLineEdit);
    Q_CHECK_PTR(newFilterToolButton);
    Q_CHECK_PTR(newFilterLayout);
    Q_CHECK_PTR(newFilterWidget);

    delete ui_;
    delete updateThread_;
    delete filter_;
    delete[] newFilterLabel;
    delete[] newFilterComboBox;
    delete[] newFilterLineEdit;
    delete[] newFilterToolButton;
    delete[] newFilterLayout;
    delete[] newFilterWidget;
    delete sqlTableModel_;
    delete mediaListProxyModel_;
    InfoView::getInstance()->kill();
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

    // for ui
    connect(ui_->mediaListView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this, SLOT(selectedMovie(QModelIndex, QModelIndex)));
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

    ///////////////////
    // infoview init //
    ///////////////////
    QString voidInfoView = InfoView::getInstance()->init(ui_->mediaInfoView->settings());
    ui_->mediaInfoView->setHtml(voidInfoView);

    /////////////////////////
    // filter objects init //
    /////////////////////////
    newFilterLabel      = new QLabel[filterLimit_];
    newFilterComboBox   = new QComboBox[filterLimit_];
    newFilterLineEdit   = new QLineEdit[filterLimit_];
    newFilterToolButton = new QToolButton[filterLimit_];
    newFilterLayout     = new QHBoxLayout[filterLimit_];
    newFilterWidget     = new QWidget[filterLimit_];

    for(int iFilter = 0; iFilter < filterLimit_; ++iFilter) {
        newFilterLabel[iFilter].setText(tr("Filter"));
        newFilterLabel[iFilter].setAlignment(Qt::AlignVCenter | Qt::AlignRight);

        newFilterComboBox[iFilter].setModel(filter_->getModel());
        newFilterComboBox[iFilter].setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);

        newFilterToolButton[iFilter].setIcon(QIcon(":/icons/minus.png"));
        connect(&newFilterToolButton[iFilter], SIGNAL(clicked()), this, SLOT(minusFilter_clicked()));

        newFilterLayout[iFilter].addWidget(&newFilterLabel[iFilter]);
        newFilterLayout[iFilter].addWidget(&newFilterComboBox[iFilter]);
        newFilterLayout[iFilter].addWidget(&newFilterLineEdit[iFilter]);
        newFilterLayout[iFilter].addWidget(&newFilterToolButton[iFilter]);

        newFilterWidget[iFilter].setLayout(&newFilterLayout[iFilter]);
        newFilterWidget[iFilter].setVisible(false);
        newFilterLayout[iFilter].setMargin(0);

        ui_->verticalLayout->addWidget(&newFilterWidget[iFilter]);
    }

    ////////////////////
    // tableView init //
    ////////////////////
    sqlTableModel_ = new MyQSqlTableModel();
    sqlTableModel_->setTable("Media");
    sqlTableModel_->removeColumns(0, 2);
    sqlTableModel_->removeColumns(1, 5);
    sqlTableModel_->setHeaderData(0, Qt::Horizontal, tr("Title"));

    // sorting the list in alphabetical order without case sensitivity
    mediaListProxyModel_ = new QSortFilterProxyModel(this);
    mediaListProxyModel_->setSourceModel(sqlTableModel_);
    mediaListProxyModel_->sort(0, Qt::AscendingOrder);
    mediaListProxyModel_->setSortCaseSensitivity(Qt::CaseInsensitive);

    ui_->mediaListView->setModel(mediaListProxyModel_);

    //////////////////////
    // mediafilter init //
    //////////////////////
    ui_->filterComboBox->setModel(filter_->getModel());

    //////////////////////
    // collections init //
    //////////////////////
    Collection::getInstance()->init();
    MediaCollection::getInstance()->init();
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
void MediaSpy::updateCollections(QStringList& dirList) {
    Collection::getInstance()->update(dirList);
    updateThread_->start();
}



///////////
// slots //
///////////
/** \fn void MediaSpy::on_actionSelectdirectories_triggered()
 *  \brief Opens the CollectionDialog dialog and gets the user's choice into the Collection.
 */
void MediaSpy::on_actionSelectdirectories_triggered() {
    CollectionDialog dialog(this);
    for(int i = 0; i < Collection::getInstance()->getNDir(); ++i)
        dialog.listWidget->addItem(Collection::getInstance()->getDirAt(i));

    if (dialog.exec() != QDialog::Accepted)
        return;

    QStringList upCollectionList = dialog.getUpdate();
    updateCollections(upCollectionList);
}


/** \fn void MediaSpy::selectedMovie(QModelIndex current, QModelIndex previous)
 *  \brief Defines what is done when a movie is selected in the list.
 */
void MediaSpy::selectedMovie(QModelIndex current, QModelIndex previous) {
    Q_UNUSED(previous);

    QString s = QString(current.data().toString());
    ui_->movieTitleLabel->setText(QString(tr("Title: %1").arg(s)));

    QString mediaName = DatabaseManager::getInstance()->getMediaFullName(s);
    if(DatabaseManager::getInstance()->hasImdbInfo(mediaName))
        ui_->mediaInfoView->setHtml(InfoView::getInstance()->getImdbInfo(mediaName), QUrl(getCoverDirectory()));
    else {
        QString voidInfoView = InfoView::getInstance()->noImdbInfo();
        ui_->mediaInfoView->setHtml(voidInfoView);
    }
}


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
void MediaSpy::setProgressbarOff() const {
    ui_->progressBar->setVisible(false);
    ui_->progressButton->setVisible(false);
    QString message = QString(tr("%n movie(s) in the collection", "", MediaCollection::getInstance()->getNMedia()));
    ui_->statusBar->showMessage(message);
}


/** \fn void MediaSpy::on_actionAbout_MediaSpy_triggered()
 *  \brief Shows the MediaSpy About window.
 */
void MediaSpy::on_actionAbout_MediaSpy_triggered() {
    QString myCopyright = QString::fromUtf8(PACKAGE_COPYRIGHTS);
    QMessageBox::about(this, tr("About ") + PACKAGE_NAME,
    QString("<h3>") + PACKAGE_NAME + " " + PACKAGE_VERSION + QString("</h3><p>") + myCopyright +
    tr("<p>MediaSpy is a movie collection cataloging software. Still in heavy development!"));
}


/** \fn void MediaSpy::displayMessage(QString message)
 *  \brief Displays a message in the status bar.
 */
void MediaSpy::displayMessage(QString message) {
    ui_->statusBar->showMessage(message);
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


void MediaSpy::on_filterToolButton_clicked() {
    if(nFilter_<filterLimit_) {
        newFilterWidget[nFilter_].setVisible(true);
        newFilterLineEdit[nFilter_].setFocus(Qt::MouseFocusReason);
        ++nFilter_;
    }
    if(nFilter_==filterLimit_)
        ui_->filterToolButton->setEnabled(false);
}


void MediaSpy::minusFilter_clicked() {
    --nFilter_;
    if(nFilter_ >= 0) {
        newFilterWidget[nFilter_].setVisible(false);
        ui_->filterToolButton->setEnabled(true);
    }
}


void MediaSpy::on_progressButton_clicked() {
    InfoManager::getInstance()->getImdbThread()->quit();
}


/** \fn void MediaSpy::on_actionRescan_triggered()
  *  \Rescans the Collection.
  */
void MediaSpy::on_actionRescan_collection_triggered()
{
    QStringList upCollectionList;
    for(int i = 0; i < Collection::getInstance()->getNDir(); ++i)
        upCollectionList << Collection::getInstance()->getDirAt(i);

    updateCollections(upCollectionList);
}

void MediaSpy::on_filterLineEdit_textChanged(QString newString) {
    mediaListProxyModel_->setFilterRegExp(QRegExp(newString, Qt::CaseInsensitive, QRegExp::FixedString));
}
