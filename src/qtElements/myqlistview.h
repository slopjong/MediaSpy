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


#ifndef MYQLISTVIEW_H
#define MYQLISTVIEW_H

#include <QContextMenuEvent>
#include <QListView>
#include <QWidgetAction>
#include <QMenu>
#include <QCheckBox>
#include <QLineEdit>
#include <QMainWindow>

#include "../databasemanager.h"
#include "../options.h"


class myQListView : public QListView {
    Q_OBJECT;

    // Fields
    QModelIndex contextMenuIndex_;
    QMenu* menu_;
    QCheckBox* checkBox_;
    QWidgetAction* seenMediaAct_;

    void contextMenuEvent(QContextMenuEvent*);


public:
    // Constructors
    explicit myQListView(QWidget *parent = 0);
    virtual ~myQListView();

   // Fields
   QAction* editTagAct_;
   QAction* playMediaAct_;


private slots:
    void seenMedia(bool);
    void playMedia();


signals:
    void updateMedia();
    void tagApplied(QString&);
    void messageToStatus(QString);

};

#endif // MYQLISTVIEW_H
