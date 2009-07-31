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

#include <QtGui/QApplication>
#include "mediaspy.h"

int main(int argc, char *argv[]) {
    QApplication MediaSpyApp(argc, argv);
    MediaSpyApp.connect(&MediaSpyApp, SIGNAL(lastWindowClosed()), &MediaSpyApp, SLOT(quit()));

    // MediaSpyHome handling
    QString MediaSpyHome = QString(getenv("MEDIASPY_HOME"));
    if (MediaSpyHome.isEmpty()) {
        #ifdef MEDIASPY_HOME
            MediaSpyHome = QString(MEDIASPY_HOME);
            if (MediaSpyHome.isEmpty())
                MediaSpyHome = QCoreApplication::applicationDirPath();
        #else
            MediaSpyHome = QCoreApplication::applicationDirPath();
        #endif
    }

    // translations handling via locale
    const QString languageFile = MediaSpyHome + QString("/translations/mediaspy_%1").arg(QLocale::system().name());
    QTranslator mediaspyTranslator;
    mediaspyTranslator.load(languageFile);
    MediaSpyApp.installTranslator(&mediaspyTranslator);

    // settings
    MediaSpyApp.setOrganizationName("spechard");
    MediaSpyApp.setOrganizationDomain("spechard.wordpress.com");
    MediaSpyApp.setApplicationName("MediaSpy");

    MediaSpy w;
    w.show();

    return MediaSpyApp.exec();
}
