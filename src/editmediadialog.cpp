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


#include <QDebug>

#include "editmediadialog.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn EditMediaDialog::EditMediaDialog(QWidget *parent)
  * \brief class constructor
  * \param parent the inherited QWidget object
  */
EditMediaDialog::EditMediaDialog(QModelIndexList indexList, QDataWidgetMapper* mapper)
        : QDialog()
        , ui_(new Ui::EditMediaDialog)
        , mapper_(mapper)
        , indexList_(QModelIndexList(indexList))
        , nMedia_(indexList_.count())
        , selectionPos_(indexList_.at(0).row())
        , modelPos_(0)
{
    Q_ASSERT(nMedia_>0);

    init();
    makeConnections();
}

/** \fn EditMediaDialog::~EditMediaDialog()
  * \brief class destructor
  */
EditMediaDialog::~EditMediaDialog() {
    delete ui_;
}


//////////////
// methods //
//////////////
void EditMediaDialog::init() {
    ui_->setupUi(this);
    ui_->previousButton->setEnabled(false);
    ui_->nextButton->setEnabled(false);

    // store the index of the model corresponding to the selection
    for(int i = 0; i < indexList_.count(); ++i)
        modelIndexList_ << indexList_.at(i).row();

    modelPos_ = modelIndexList_.indexOf(selectionPos_);

    // mapper
    mapper_->addMapping(ui_->mediaNameLabel, 3, "text"); // title field
    mapper_->addMapping(ui_->seenCheckBox, 6); // seen field
//    mapper_->addMapping(ui_->tagLineEdit, 2); // tag field !! TODO jointure !
    mapper_->setCurrentIndex(selectionPos_);

    // initial setup depending on the number of medias
    if(nMedia_>1) {
        setWindowTitle(tr("Details on %1 medias").arg(nMedia_) + QString(" - MediaSpy"));
        ui_->mediaNameLabel->setText(QString());
        ui_->mediaNameTitleLabel->setEnabled(false);
    }
    else {
        setWindowTitle(tr("Details on %1").arg(indexList_.at(0).data(Qt::DisplayRole).toString()) + QString(" - MediaSpy"));
        ui_->parMediaCheckBox->setEnabled(false);
    }
}


/** \fn void MediaSpy::makeConnections()
  * \brief makes the connections used by EditMediaDialog
  */
void EditMediaDialog::makeConnections() {
    connect(ui_->nextButton, SIGNAL(clicked()), mapper_, SLOT(toNext()));
    connect(ui_->nextButton, SIGNAL(clicked()), this, SLOT(toNext()));
    connect(ui_->previousButton, SIGNAL(clicked()), mapper_, SLOT(toPrevious()));
    connect(ui_->previousButton, SIGNAL(clicked()), this, SLOT(toPrevious()));
}



///////////
// slots //
///////////
void EditMediaDialog::on_parMediaCheckBox_clicked(bool checked) {
    if(checked) { // per media
        ui_->nextButton->setEnabled(true);
        ui_->mediaNameTitleLabel->setEnabled(true);
        modelPos_ = modelIndexList_.indexOf(selectionPos_);
        mapper_->setCurrentIndex(indexList_.at(modelPos_).row()); // crash!
    }
    else { // all medias
        ui_->previousButton->setEnabled(false);
        ui_->nextButton->setEnabled(false);
        ui_->mediaNameLabel->setText(QString());
        ui_->mediaNameTitleLabel->setEnabled(false);
    }
}


void EditMediaDialog::toNext() {
    ui_->previousButton->setEnabled(true);
    selectionPos_++;
    modelPos_ = modelIndexList_.indexOf(selectionPos_);
    mapper_->setCurrentIndex(indexList_.at(modelPos_).row());
    if(modelPos_ == nMedia_-1)
        ui_->nextButton->setEnabled(false);
}


void EditMediaDialog::toPrevious() {
    ui_->nextButton->setEnabled(true);
    selectionPos_--;
    modelPos_ = modelIndexList_.indexOf(selectionPos_);
    mapper_->setCurrentIndex(indexList_.at(modelPos_).row());
    if(modelPos_ == 0)
        ui_->previousButton->setEnabled(false);
}

