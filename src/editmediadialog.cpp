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
//        , nMedia_(indexList_.count())
{
    ui_->setupUi(this);
    ui_->previousButton->setEnabled(false);
    ui_->nextButton->setEnabled(false);
    ui_->parMediaCheckBox->setChecked(false);

    init();

    // connections
//    connect(ui_->nextButton, SIGNAL(clicked()), this, SLOT(next()));
//    connect(ui_->previousButton, SIGNAL(clicked()), this, SLOT(previous()));
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
    mapper_->addMapping(ui_->mediaNameLineEdit, 3); // title field
    mapper_->addMapping(ui_->seenCheckBox, 6); // seen field
//    mapper_->addMapping(ui_->tagLineEdit, 2); // tag field !! TODO jointure !
    mapper_->toFirst();

//    Q_ASSERT(nMedia_>0);
//
//    if(nMedia_>1)
//        this->setWindowTitle(tr("Informations on %1 medias - MediaSpy").arg(nMedia_));
//    else
//        this->setWindowTitle(QString("%1 - MediaSpy").arg(indexList_.at(0).data().toString()));
}


///////////
// slots //
///////////



///////////////
// accessors //
///////////////
