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
#include <QDebug>


/////////////////////////////
// constructors/destructor //
/////////////////////////////
myQListView::myQListView(QWidget *parent)
        : QListView(parent)
        , seenMediaAct_(new QAction(tr("Seen"), this))
{
    seenMediaAct_->setCheckable(true);

    // connections
    connect(this->seenMediaAct_, SIGNAL(triggered(bool)), this, SLOT(seenMedia(bool)));
}

myQListView::~myQListView() {}



/////////////
// methods //
/////////////
void myQListView::contextMenuEvent(QContextMenuEvent* event) {
    QMenu *menu = new QMenu(this);
    contextMenuIndex_ = indexAt(event->pos());
    if (contextMenuIndex_.isValid()) {
        QString indexContent = QString(contextMenuIndex_.data().toString());
        seenMediaAct_->setChecked(DatabaseManager::getInstance()->isMediaSeen(indexContent));
        menu->addAction(seenMediaAct_);
        menu->exec(QCursor::pos());
    }
}



///////////
// slots //
///////////
void myQListView::seenMedia(bool checked) {
    QString indexContent = QString(contextMenuIndex_.data().toString());
    if(checked)
        DatabaseManager::getInstance()->setMediaSeen(indexContent);
    else
        DatabaseManager::getInstance()->setMediaSeen(indexContent, false);

    emit updateMedia();
}

