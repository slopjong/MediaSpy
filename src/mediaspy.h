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
//#include <QSettings>
#include <QDir>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QToolButton>

#include "collectiondialog.h"
#include "collection.h"
#include "mediacollection.h"
#include "updatethread.h"
#include "mediafilter.h"
#include "infoview.h"
#include "infomanager.h"


/** \var appDirectory
  * \brief name of the local directory used by the program
  */
static const QString appDirectory   = QDir::homePath() + "/.MediaSpy/";
static const QString cssDirectory   = QDir::homePath() + "/.MediaSpy/stylesheets/";
static const QString coverDirectory = QDir::homePath() + "/.MediaSpy/covers/";

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

public:
    // Constructors
    explicit MediaSpy(QWidget *parent = 0);
    ~MediaSpy();

    // Accessor Methods
    static const QString getAppDirectory();
    static const QString getDbFileName();
    static const QString getCssDirectory();
    static const QString getCoverDirectory();

    // Operations
    void tableViewUpdated();


private:
    // Fields
    Ui::MediaSpy* ui_;
    UpdateThread* updateThread_;
    MediaFilter* filter_;
    const int filterLimit_;
    int nFilter_;
    QString errorMessage_;
    QSqlTableModel* sqlTableModel_;
    QSortFilterProxyModel* mediaListProxyModel_;
    QLabel* newFilterLabel;
    QComboBox* newFilterComboBox;
    QLineEdit* newFilterLineEdit;
    QToolButton* newFilterToolButton;
    QHBoxLayout* newFilterLayout;
    QWidget* newFilterWidget;


    // Operations
    void init();
    void updateCollections(QStringList&);
    void writeSettings();
    void readSettings();
    void closeEvent(QCloseEvent*);
    void makeConnections();


private slots:
    void minusFilter_clicked();
    void on_filterToolButton_clicked();
    void on_actionAbout_MediaSpy_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionSelectdirectories_triggered();
    void setProgressbarMaximum(const int) const;
    void setProgressbarCurrent(const int) const;
    void setProgressbarOff() const;
    void displayMessage(QString);
    void finishedThread();
    void selectedMovie(QModelIndex, QModelIndex);
};

#endif // MEDIASPY_H
