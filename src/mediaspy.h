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
#include <QDir>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QToolButton>

#include "collectiondialog.h"
#include "collection.h"
#include "updatethread.h"
#include "info/infoview.h"
#include "info/infomanager.h"
#include "media/mediacollection.h"
#include "qtElements/myqlineedit.h"
#include "qtElements/myqsqltablemodel.h"
#include "qtElements/myqsortfilterproxymodel.h"


/** \var appDirectory
  * \brief name of the local directory used by the program
  */
static const QString appDirectory     = QDir::homePath() + "/.MediaSpy/";
static const QString cssDirectory     = QDir::homePath() + "/.MediaSpy/stylesheets/";
static const QString coverDirectory   = QDir::homePath() + "/.MediaSpy/covers/";

/** \var defaultImageName
  * \brief name of the default cover
  */
static const QString defaultCoverName = ".default.jpg";

/** \var dbFileName
  * \brief name of the database file
  */
static const QString dbFileName     = "MediaSpy.sqlite";


/** \namespace Ui
  * \brief the namespace used for the .ui Object
  */
namespace Ui {
    class MediaSpy;
}


/** \class MediaSpy
  *
  */
class MediaSpy : public QMainWindow {
    Q_OBJECT

    // Fields
    Ui::MediaSpy* ui_;
    UpdateThread* updateThread_;
    myQSqlTableModel* sqlTableModel_;
    myQSortFilterProxyModel* mediaListProxyModel_;
    QLabel* statusLabel_;
    QString errorMessage_;

    // Operations
    void init();
    void updateCollections(QStringList&);
    void writeSettings();
    void readSettings();
    void closeEvent(QCloseEvent*);
    void makeConnections();
    void displayPermanentMessage(const QString = 0);


public:
    // Constructors
    explicit MediaSpy(QWidget *parent = 0);
    virtual ~MediaSpy();

    // Accessor Methods
    static const QString getAppDirectory();
    static const QString getDbFileName();
    static const QString getCssDirectory();
    static const QString getCoverDirectory();
    static const QString getDefaultCoverName();

    // Operations
    void tableViewUpdated();


private slots:
    void on_filterSeenComboBox_currentIndexChanged(int index);
    void on_filterLineEdit_textChanged(QString );
    void on_actionRescan_collection_triggered();
    void on_progressButton_clicked();
    void isMediaFound(bool, QString);
    void on_actionAbout_MediaSpy_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionSelectdirectories_triggered();
    void setProgressbarMaximum(const int) const;
    void setProgressbarCurrent(const int) const;
    void setProgressbarOff();
    void displayMessage(const QString = 0);
    void finishedUpdateThread();
    void selectedMovie(QModelIndex, QModelIndex);

};

#endif // MEDIASPY_H
