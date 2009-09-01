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


#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QtGui/QDialog>
#include <QTreeWidgetItem>

#include "options.h"
#include "databasemanager.h"

namespace Ui {
    class OptionsDialog;
}


class OptionsDialog : public QDialog {
    Q_OBJECT

    Ui::OptionsDialog *ui_;
    QList<QTreeWidgetItem *> optionItems_;
    QTreeWidgetItem* currentItem_;

    void init();
    void changePage(QTreeWidgetItem*, int);


public:
    OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();
    void setOptions();
    void getOptions();


private slots:
    void on_purgeButton_clicked();
    void on_optionsTree_itemEntered(QTreeWidgetItem* item, int column);
    void on_optionsTree_itemActivated(QTreeWidgetItem* item, int column);

};

#endif // OPTIONSDIALOG_H
