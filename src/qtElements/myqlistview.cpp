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
        , checkBox_(new QCheckBox(this))
        , seenMediaAct_(new QWidgetAction(this))
        , menu_(new QMenu(this))

{
    seenMediaAct_->setDefaultWidget(checkBox_);
    checkBox_->setText(tr("Seen"));

    // connections
    connect(this->checkBox_, SIGNAL( clicked(bool)), this, SLOT(seenMedia(bool)));
}

myQListView::~myQListView() {}



/////////////
// methods //
/////////////
void myQListView::contextMenuEvent(QContextMenuEvent* event) {
    QItemSelectionModel* selectionModel = this->selectionModel();
    QModelIndexList indexList = selectionModel->selectedIndexes();
    Qt::CheckState state = Qt::Unchecked;
    bool storedBool;
    bool changeBool = false;
    for (int i = 0; i < indexList.size(); ++i)
        if (indexList.at(i).isValid()) {
            QString indexContent = QString(indexList.at(i).data().toString());
            bool b = DatabaseManager::getInstance()->isMediaSeen(indexContent);
            if(i==0 || changeBool==true)
                storedBool = b;
            else
                changeBool = (b==storedBool) ? false : true;
        }
    state = (changeBool==true) ? Qt::PartiallyChecked : ((storedBool) ? Qt::Checked : Qt::Unchecked);
    checkBox_->setCheckState(state);

    contextMenuIndex_ = indexAt(event->pos());
    if (contextMenuIndex_.isValid()) {
        QString indexContent = QString(contextMenuIndex_.data().toString());
        seenMediaAct_->setChecked(DatabaseManager::getInstance()->isMediaSeen(indexContent));
        menu_->addAction(seenMediaAct_);
        menu_->exec(QCursor::pos());
    }
}



///////////
// slots //
///////////
void myQListView::seenMedia(bool checked) {
    QItemSelectionModel* selectionModel = this->selectionModel();
    QModelIndexList indexList = selectionModel->selectedIndexes();
    QStringList nameList;
    for (int i = 0; i < indexList.size(); ++i)
        if (indexList.at(i).isValid())
            nameList << QString(indexList.at(i).data().toString());
    if(checked)
        DatabaseManager::getInstance()->setMediaSeen(nameList);
    else
        DatabaseManager::getInstance()->setMediaSeen(nameList, false);

    emit updateMedia();
    menu_->setVisible(false);
}

