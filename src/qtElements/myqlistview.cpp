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


#include "myqlistview.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
myQListView::myQListView(QWidget *parent)
        : QListView(parent)
        , editMediaAct_(new QAction(tr("&Watched"), this))
{
    // set context menu policy
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    // connections
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showListContextMenu(const QPoint&)));
    connect(this->editMediaAct_, SIGNAL(triggered()),
            this, SLOT(editMedia()));

    // taking care of actions
    editMediaAct_->setCheckable(true);
}

myQListView::~myQListView() {}



/////////////
// methods //
/////////////





///////////
// slots //
///////////
void myQListView::showListContextMenu(const QPoint& pnt) {
    QList<QAction*> actions;
    if (this->indexAt(pnt).isValid()) {
        actions.append(this->editMediaAct_);
    }
//    if (actions.count() > 0)
//        QMenu::exec(actions, this->mapToGlobal(pnt));
}


void myQListView::editMedia() { // misses the right position of the click!!
    if(editMediaAct_->isChecked())
        fprintf(stdout, "isChecked\n");
    else
        fprintf(stdout, "!isChecked\n");
}

