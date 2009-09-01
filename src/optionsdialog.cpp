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


#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <QDebug>


/////////////////////////////
// constructors/destructor //
/////////////////////////////
OptionsDialog::OptionsDialog(QWidget *parent)
        : QDialog(parent)
        , ui_(new Ui::OptionsDialog)
{
    ui_->setupUi(this);
    init();
    getOptions();
}

OptionsDialog::~OptionsDialog() {
    delete ui_;
    qDeleteAll(optionItems_.begin(), optionItems_.end());
    optionItems_.clear();
}



/////////////
// methods //
/////////////
void OptionsDialog::init() {
    static const QStringList optionsName = QStringList()
            << tr("General")
            << tr("Tags"); // order is important !!

    ui_->optionsTree->setColumnCount(1);
    for (int i = 0; i < optionsName.count(); ++i)
        optionItems_.append(new QTreeWidgetItem(QStringList(optionsName.at(i))));
    ui_->optionsTree->insertTopLevelItems(0, optionItems_);
}


void OptionsDialog::changePage(QTreeWidgetItem* item, int column) {
    Q_UNUSED(column);

    int index = optionItems_.indexOf(item);
    if(item != currentItem_) {
        ui_->optionsStack->setCurrentIndex(index);
        currentItem_ = item;
    }
}


void OptionsDialog::getOptions() {
//    ui_->automaticUpdate->setChecked(Options::getInstance()->automaticUpdate());
    ui_->tagsListLabel->setText(DatabaseManager::getInstance()->getTagList().join(", "));
    ui_->orphanTagsLabel->setText(DatabaseManager::getInstance()->getOrphanTagList().join(", "));
    ui_->playerEdit->setText(Options::getInstance()->getPlayer());

}


void OptionsDialog::setOptions() {
//    Options::getInstance()->setAutomaticUpdate(ui_->automaticUpdate->isChecked());
//    Options::getInstance()->setOrphanTags(ui_->automaticUpdate->isChecked());
    Options::getInstance()->setPlayer(ui_->playerEdit->text());
}


///////////
// slots //
///////////
void OptionsDialog::on_optionsTree_itemActivated(QTreeWidgetItem* item, int column) {
    changePage(item, column);
}

void OptionsDialog::on_optionsTree_itemEntered(QTreeWidgetItem* item, int column) {
    changePage(item, column);
}

