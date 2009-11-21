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

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
//#include <KLocale>

#include "mediaspy.h"


int main (int argc, char *argv[]) {
    KAboutData aboutData("mediaspy", 0,
                         ki18n("MediaSpy"), PACKAGE_VERSION,
                         ki18n("A movie collection cataloging software. Still in heavy development!"),
                         KAboutData::License_GPL_V3,
                         ki18n("Copyright (c) 2009 Stéphane Péchard"),
                         ki18n("Build Date: %1").subs( __DATE__ ),
                         "http://spechard.wordpress.com/");
    //Authors
//    aboutData.addAuthor(ki18n("Stéphane Péchard"),
//                        ki18n("Developer (spechard)"), "stephanepechard@gmail.com", "http://spechard.wordpress.com/" );

    KCmdLineArgs::init( argc, argv, &aboutData );

    KApplication MediaSpyApp;
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

    // settings
    MediaSpyApp.setOrganizationName("spechard");
    MediaSpyApp.setOrganizationDomain("spechard.wordpress.com");
    MediaSpyApp.setApplicationName("MediaSpy");

/*    // translations handling via locale (Qt way)
    const QString languageFile = MediaSpyHome + QString("/translations/mediaspy_%1").arg(QLocale::system().name());
    QTranslator mediaspyTranslator;
    mediaspyTranslator.load(languageFile);
    MediaSpyApp.installTranslator(&mediaspyTranslator);*/

    MediaSpy *w = new MediaSpy();
    w->show();

    return MediaSpyApp.exec();
}

