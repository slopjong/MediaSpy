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


#include "networkchecker.h"


NetworkChecker::NetworkChecker() {
    connect(this, SIGNAL(connected()), this, SLOT(connexionOk()));
    connect(this, SIGNAL(error(int)), this, SLOT(connexionNotOk(int)));
}

NetworkChecker::~NetworkChecker() {}

void NetworkChecker::runConnectionTest() {
    connectToHost("google.com", 80);
}


void NetworkChecker::connexionOk() {
    emit(connexionAvailable(true));
}

void NetworkChecker::connexionNotOk(int e) {
    fprintf(stdout, "error = %d\n", e);
    emit(connexionAvailable(false));
}
