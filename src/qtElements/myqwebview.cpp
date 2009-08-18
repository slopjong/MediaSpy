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


#include "myqwebview.h"


/////////////////////////////
// constructors/destructor //
/////////////////////////////
/** \fn myQLineEdit::myQLineEdit(QWidget *parent)
  * \brief Constructor.
  */
myQWebView::myQWebView(QWidget *parent) : QWebView(parent) {
//    setContextMenuPolicy(Qt::NoContextMenu);
}

myQWebView::~myQWebView() {}

