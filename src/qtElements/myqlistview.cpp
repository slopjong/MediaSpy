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
        , menu_(new QMenu(this))
        , checkBox_(new QCheckBox(this))
        , seenMediaAct_(new QWidgetAction(this))
        , editTagAct_(new QAction(this))
        , playMediaAct_(new QAction(this))
{
    seenMediaAct_->setDefaultWidget(checkBox_);
    checkBox_->setText(tr("Mark as seen"));
    editTagAct_->setShortcut(Qt::CTRL + Qt::Key_E);

    // connections
    connect(checkBox_, SIGNAL(clicked(bool)), this, SLOT(seenMedia(bool)));
    connect(playMediaAct_, SIGNAL(triggered()), this, SLOT(playMedia()));
}

myQListView::~myQListView() {
    delete menu_;
    delete checkBox_;
    delete seenMediaAct_;
    delete editTagAct_;
}



/////////////
// methods //
/////////////
void myQListView::contextMenuEvent(QContextMenuEvent* event) {
    menu_->clear();
    QItemSelectionModel* selectionModel = this->selectionModel();
    QModelIndexList indexList = selectionModel->selectedRows();
    Qt::CheckState state = Qt::Unchecked;
    bool storedBool = false;
    bool changeBool = false;
    for (int i = 0; i < indexList.count(); ++i)
        if (indexList.at(i).isValid()) {
            QString indexContent = QString(indexList.at(i).data().toString());
            bool b = DatabaseManager::getInstance()->isMediaSeen(indexContent);
            if(i==0 || changeBool==true)
                storedBool = b;
            else
                changeBool = (b!=storedBool);
        }
    state = (changeBool==true) ? Qt::PartiallyChecked : ((storedBool) ? Qt::Checked : Qt::Unchecked);
    checkBox_->setCheckState(state);

    contextMenuIndex_ = indexAt(event->pos());
    if (contextMenuIndex_.isValid()) {
        QString indexContent = QString(contextMenuIndex_.data().toString());
        seenMediaAct_->setChecked(DatabaseManager::getInstance()->isMediaSeen(indexContent));
        editTagAct_->setText(tr("Edit tags"));
        editTagAct_->setIcon(QIcon(":/icons/edit.png"));
        playMediaAct_->setText(tr("Play media(s) (%1)", "", indexList.count()).arg(property("player").toString()));
        playMediaAct_->setIcon(QIcon(":/icons/play.png"));

        menu_->addAction(seenMediaAct_);
        menu_->addAction(editTagAct_);
        menu_->addSeparator();
        menu_->addAction(playMediaAct_);
        menu_->exec(QCursor::pos());
    }
}



///////////
// slots //
///////////
void myQListView::seenMedia(bool checked) {
    QItemSelectionModel* selectionModel = this->selectionModel();
    QModelIndexList indexList = selectionModel->selectedRows();
    QStringList nameList;

    for (int i = 0; i < indexList.count(); ++i)
        if (indexList.at(i).isValid())
            nameList << QString(indexList.at(i).data().toString());

    if(checked)
        DatabaseManager::getInstance()->setMediaSeen(nameList);
    else
        DatabaseManager::getInstance()->setMediaSeen(nameList, false);

    emit updateMedia();
    menu_->setVisible(false);
}


void myQListView::playMedia() {
    QItemSelectionModel* selectionModel = this->selectionModel();
    QModelIndexList indexList = selectionModel->selectedRows();

    if(indexList.count()>0) {
        QStringList arguments;

        for (int i = 0; i < indexList.count(); ++i)
            if (indexList.at(i).isValid())
                arguments << DatabaseManager::getInstance()->getMediaFullName(indexList.at(i).data().toString());

        QString playerName = property("player").toString();
        if(playerName.contains("mplayer"))
            arguments << "-quiet"; // in order 1. to be quiet (!) 2. to *really* detach the process

        QProcess playProcess;
        bool ok = playProcess.startDetached(playerName, arguments);

        if(!ok)
            emit messageToStatus(tr("[ERROR] Player not launched!"));
    }
    else
        emit messageToStatus(tr("[ERROR] No media selected!"));
}

