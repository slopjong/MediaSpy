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


#include "editmediadialog.h"
#include "ui_editmediadialog.h"

/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn EditMediaDialog::EditMediaDialog(QWidget *parent)
  * \brief class constructor
  * \param parent the inherited QWidget object
  */
EditMediaDialog::EditMediaDialog(QModelIndexList indexList, QDataWidgetMapper* mapper, QWidget *parent)
        : QDialog(parent)
        , ui_(new Ui::EditMediaDialog)
        , mapper_(mapper)
        , indexList_(QModelIndexList(indexList))
        , nMedia_(indexList_.count())
        , selectionPos_(indexList_.at(0).row())
        , originPos_(selectionPos_)
        , tagsSet_(new QStringList())
        , tagsUnset_(new QStringList())
        , statusBar_(new QStatusBar(this))
        , currentMediaName_(QString())
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
    delete tagsSet_;
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
    ui_->pathGroupBox->setVisible(false);

    // status bar
    statusBar_->setSizeGripEnabled(false);
    ui_->statusBarLayout->addWidget(statusBar_);

    // mapper (-1 because we removed a column)
    mapper_->addMapping(ui_->mediaNameLabel, tableMedia::fileName - 1, "text"); // path
    mapper_->addMapping(ui_->mediaNameLineEdit, tableMedia::baseName - 1, "text"); // baseName field
    mapper_->addMapping(ui_->seenCheckBox, tableMedia::seen - 1); // seen field
    mapper_->addMapping(ui_->removeInfoCheckBox, tableMedia::imdbInfo - 1); // imdbInfo field
    mapper_->setCurrentIndex(selectionPos_);

    // tabs
    ui_->tabWidget->setTabEnabled(tabPage::imdb, ui_->removeInfoCheckBox->isChecked());

    // tags
    setTagsInfo();

    // initial setup depending on the number of medias
    if(nMedia_>1)
        setWindowTitle(tr("Details on %1 medias").arg(nMedia_) + QString(" - MediaSpy"));
    else {
        ui_->nextButton->setEnabled(false);
        setWindowTitle(tr("Details on %1").arg(indexList_.at(0).data(Qt::DisplayRole).toString()) + QString(" - MediaSpy"));
    }

    currentMediaName_ = ui_->mediaNameLineEdit->text();
}


/** \fn void EditMediaDialog::makeConnections()
  * \brief makes the connections used by EditMediaDialog
  */
void EditMediaDialog::makeConnections() {
//    connect(ui_->nextButton, SIGNAL(clicked()), mapper_, SLOT(toNext()));
    connect(ui_->nextButton, SIGNAL(clicked()), this, SLOT(toNext()));
//    connect(ui_->previousButton, SIGNAL(clicked()), mapper_, SLOT(toPrevious()));
    connect(ui_->previousButton, SIGNAL(clicked()), this, SLOT(toPrevious()));
}


void EditMediaDialog::setTagsInfo() {
    QString mediaName = indexList_.at(selectionPos_ - originPos_).data().toString();
    int mediaId = DatabaseManager::getInstance()->getMediaId(mediaName);
    QStringList tagList = DatabaseManager::getInstance()->getMediaTagList(mediaId);
    ui_->tagsLabel->setText(tagList.join(", "));

    // set the local lists
    tagsSet_->clear();
    tagsSet_->append(tagList);

    tagsUnset_->clear();
    QStringList allTagList = DatabaseManager::getInstance()->getTagList();
    for(int i=0; i<allTagList.count(); ++i)
        if(!tagsSet_->contains(allTagList.at(i)))
            tagsUnset_->append(allTagList.at(i));

    updateComboBox();
}


void EditMediaDialog::updateComboBox() {
    ui_->minusComboBox->clear();
    ui_->minusComboBox->addItems(tagsSet_[0]);
    ui_->plusComboBox->clear();
    ui_->plusComboBox->addItems(tagsUnset_[0]);

    // disabled if nothing to show
    ui_->minusComboBox->setEnabled(tagsSet_[0].count());
    ui_->minusToolButton->setEnabled(tagsSet_[0].count());
}


/** \fn void EditMediaDialog::updateMediaName(QString oldName, QString newName)
  * \brief Updates the media base name in the database
  */
void EditMediaDialog::updateMediaName(QString oldName, QString newName) {
    if(!newName.isEmpty() && !DatabaseManager::getInstance()->hasMediaBaseName(newName))
        DatabaseManager::getInstance()->updateMediaBaseName(oldName, newName);
}



///////////
// slots //
///////////
void EditMediaDialog::toNext() {
    if(currentMediaName_ != ui_->mediaNameLineEdit->text())
        updateMediaName(currentMediaName_, ui_->mediaNameLineEdit->text());

    selectionPos_++;
    setTagsInfo();
    mapper_->setCurrentIndex(selectionPos_);
    ui_->tabWidget->setTabEnabled(tabPage::imdb, ui_->removeInfoCheckBox->isChecked());
    statusBar_->showMessage(QString());

    ui_->previousButton->setEnabled(true);
    if(selectionPos_ == indexList_.at(0).row() + indexList_.count() - 1)
        ui_->nextButton->setEnabled(false);

    // update current name
    currentMediaName_ = ui_->mediaNameLineEdit->text();
}


void EditMediaDialog::toPrevious() {
    if(currentMediaName_ != ui_->mediaNameLineEdit->text())
        updateMediaName(currentMediaName_, ui_->mediaNameLineEdit->text());

    selectionPos_--;
    setTagsInfo();
    mapper_->setCurrentIndex(selectionPos_);
    ui_->tabWidget->setTabEnabled(tabPage::imdb, ui_->removeInfoCheckBox->isChecked());
    statusBar_->showMessage(QString());

    ui_->nextButton->setEnabled(true);
    if(selectionPos_ == indexList_.at(0).row())
        ui_->previousButton->setEnabled(false);

    // update current name
    currentMediaName_ = ui_->mediaNameLineEdit->text();
}


void EditMediaDialog::on_plusToolButton_clicked() {
    QString tagName = ui_->plusComboBox->currentText();
    if(!tagName.isEmpty()) {
        // add the tag to the db if it's not in
        if(!tagsSet_->contains(tagName) && !tagsUnset_->contains(tagName)) {
            DatabaseManager::getInstance()->insertTag(tagName);
            statusBar_->showMessage(tr("New tag '%1' created!").arg(tagName));
        }

        // apply it to the current media
        DatabaseManager::getInstance()->addTagToMedia(tagName, ui_->mediaNameLabel->text());

        // update the tags view
        setTagsInfo();
    }
}


void EditMediaDialog::on_minusToolButton_clicked() {
    QString tagName = ui_->minusComboBox->currentText();
    if(!tagName.isEmpty()) {
        // remove it from the current media
        DatabaseManager::getInstance()->removeTagFromMedia(tagName, ui_->mediaNameLabel->text());

        // update the tags view
        setTagsInfo();
    }
}


void EditMediaDialog::on_seenCheckBox_clicked() {
    // change the db content accordingly
    DatabaseManager::getInstance()->setMediaSeen(QStringList() << ui_->mediaNameLineEdit->text(), ui_->seenCheckBox->isChecked());
}

