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
        , originPos_(selectionPos_)
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
/** \fn void EditMediaDialog::init()
  * \brief Initiates required parts of the object.
  */
void EditMediaDialog::init() {
    ui_->setupUi(this);
    ui_->previousButton->setEnabled(false);
    ui_->nextButton->setEnabled(false);

    // mapper
    mapper_->addMapping(ui_->mediaNameLabel, 3, "text"); // title field
    mapper_->addMapping(ui_->seenCheckBox, 6); // seen field
//    mapper_->addMapping(ui_->tagLineEdit, 2); // tag field !! TODO jointure !
    mapper_->setCurrentIndex(selectionPos_);

    // filling what mapper cannot get
    setSeenCheckBoxState();

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


/** \fn void EditMediaDialog::makeConnections()
  * \brief makes the connections used by EditMediaDialog
  */
void EditMediaDialog::makeConnections() {
    connect(ui_->nextButton, SIGNAL(clicked()), mapper_, SLOT(toNext()));
    connect(ui_->nextButton, SIGNAL(clicked()), this, SLOT(toNext()));
    connect(ui_->previousButton, SIGNAL(clicked()), mapper_, SLOT(toPrevious()));
    connect(ui_->previousButton, SIGNAL(clicked()), this, SLOT(toPrevious()));
}


void EditMediaDialog::setSeenCheckBoxState() {
    Qt::CheckState state = Qt::Unchecked;
    bool storedBool = false;
    bool changeBool = false;
    for (int i = 0; i < indexList_.size(); ++i)
        if (indexList_.at(i).isValid()) {
            QString indexContent = QString(indexList_.at(i).data().toString());
            bool b = DatabaseManager::getInstance()->isMediaSeen(indexContent);
            if(i==0 || changeBool==true)
                storedBool = b;
            else
                changeBool = (b!=storedBool);
        }
    state = (changeBool==true) ? Qt::PartiallyChecked : ((storedBool) ? Qt::Checked : Qt::Unchecked);
    ui_->seenCheckBox->setCheckState(state);
}


void EditMediaDialog::setTagLineEdit() {
    QString mediaName = indexList_.at(selectionPos_ - originPos_).data().toString();
    int mediaId = DatabaseManager::getInstance()->getMediaId(mediaName);
    QStringList tagList = DatabaseManager::getInstance()->getMediaTagList(mediaId);

    ui_->tagLineEdit->setText(tagList.join(", "));
}


///////////
// slots //
///////////
void EditMediaDialog::on_parMediaCheckBox_clicked(bool checked) {
    if(checked) { // per media
        ui_->nextButton->setEnabled(true);
        ui_->mediaNameTitleLabel->setEnabled(true);
        mapper_->setCurrentIndex(selectionPos_);
    }
    else { // all medias
        ui_->previousButton->setEnabled(false);
        ui_->nextButton->setEnabled(false);
        ui_->mediaNameTitleLabel->setEnabled(false);
        ui_->mediaNameLabel->setText(QString());
        ui_->tagLineEdit->setText(QString());
        setSeenCheckBoxState();
    }
}


void EditMediaDialog::toNext() {
    selectionPos_++;
    setTagLineEdit();
    mapper_->setCurrentIndex(selectionPos_);    

    ui_->previousButton->setEnabled(true);
    if(selectionPos_ == indexList_.at(0).row() + indexList_.count() - 1)
        ui_->nextButton->setEnabled(false);
}


void EditMediaDialog::toPrevious() {
    selectionPos_--;
    setTagLineEdit();
    mapper_->setCurrentIndex(selectionPos_);

    ui_->nextButton->setEnabled(true);
    if(selectionPos_ == indexList_.at(0).row())
        ui_->previousButton->setEnabled(false);
}

