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
        , menu_(new QMenu(this))
        , addTagMenu_(new QMenu(tr("Edit tag"), this))
        , checkBox_(new QCheckBox(this))
        , editLine_(new QLineEdit(this))
        , seenMediaAct_(new QWidgetAction(this))
        , tagLineAct_(new QWidgetAction(this))
        , editTagAct_(new QAction(this))
{
    seenMediaAct_->setDefaultWidget(checkBox_);
    tagLineAct_->setDefaultWidget(editLine_);
    checkBox_->setText(tr("Seen"));

    // connections
    connect(this->checkBox_, SIGNAL(clicked(bool)), this, SLOT(seenMedia(bool)));
    connect(this->editLine_, SIGNAL(editingFinished()), this, SLOT(newTag()));
//    connect(this->editLine_, SIGNAL(cursorPositionChanged (int, int)), this->editLine_, SLOT(clear()));
}

myQListView::~myQListView() {
    delete menu_;
    delete addTagMenu_;
    delete checkBox_;
    delete editLine_;
    delete seenMediaAct_;
    delete tagLineAct_;
    delete editTagAct_;
}



/////////////
// methods //
/////////////
void myQListView::contextMenuEvent(QContextMenuEvent* event) {
    menu_->clear();
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
        createMenu(menu_);
        menu_->exec(QCursor::pos());
    }
}


void myQListView::createMenu(QMenu* menu) {
    addTagMenu_->clear();
    editLine_->setText(tr("New tag"));

    menu->addAction(seenMediaAct_);
    menu->addMenu(addTagMenu_);

    // get tags to add
    QStringList tagList = DatabaseManager::getInstance()->getMediaTagList();
    for(int i = 0; i < tagList.count(); ++i) {
        QAction* newTagAct = new QAction(tagList.at(i), this);
        connect(newTagAct, SIGNAL(triggered()), this, SLOT(tagSlot()));
        newTagAct->setCheckable(true);
        addTagMenu_->addAction(newTagAct);
//        if(DatabaseManager::getInstance()->isTagSet(tagList.at(i), ))
//            newTagAct->setChecked(true);
    }
//    addTagMenu_->addSeparator();
    addTagMenu_->addAction(tagLineAct_);
}


void myQListView::applyTag(QString& tagName) {
    QItemSelectionModel* selectionModel = this->selectionModel();
    QModelIndexList indexList = selectionModel->selectedIndexes();
    for (int i = 0; i < indexList.size(); ++i)
        if (indexList.at(i).isValid())
            DatabaseManager::getInstance()->addTagToMedia(tagName, indexList.at(i).data().toString());
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


void myQListView::newTag() {
    QString newTagName = editLine_->text();
    if(!newTagName.isEmpty()) {
        DatabaseManager::getInstance()->insertTag(newTagName); // add tag to the database
        applyTag(newTagName); // apply this tag to the selection
        menu_->setVisible(false);
    }
}


void myQListView::tagSlot() {
    QAction* actionSender = (QAction*)this->sender();
    QString tagName = actionSender->text();
    applyTag(tagName);

    QItemSelectionModel* selectionModel = this->selectionModel();
    QModelIndexList indexList = selectionModel->selectedIndexes();
    QString s = QString(indexList.at(indexList.count() - 1).data().toString());
    QString mediaName = DatabaseManager::getInstance()->getMediaFullName(s);

    emit tagApplied(mediaName);
}
