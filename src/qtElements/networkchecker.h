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


#ifndef NETWORKCHECKER_H
#define NETWORKCHECKER_H

#include <QTcpSocket>


class NetworkChecker : public QTcpSocket {
    Q_OBJECT;

public:
    NetworkChecker();
    virtual ~NetworkChecker();
    void runConnectionTest();


private slots:
    void connexionOk();
    void connexionNotOk( QAbstractSocket::SocketError );


signals:
    void connexionAvailable( bool );

};

#endif // NETWORKCHECKER_H
