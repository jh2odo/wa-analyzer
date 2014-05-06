#
# The MIT License (MIT)
#
# Copyright (c) 2012 Jose Aguado (jh2odo)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE
#
#
# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

QT += core \
      gui \
      xml \
      network \
      webkit

#symbian:TARGET.UID3 = 0x2006DFC8

TARGET = WAAnalyzer
ICON = WAAnalyzer.svg

symbian: {

    DEPLOYMENT.display_name = WAAnalyzer
    VERSION = 2.2.0

# 0xE277CC23 - Development
     TARGET.UID3 = 0xE277CC23
     TARGET.CAPABILITY += NetworkServices \
                          LocalServices \
                          ReadUserData \
                          WriteUserData \
                          UserEnvironment
#                           ReadDeviceData \
#                           WriteDeviceData

    vendorinfo += "%{\"jh2odo\"}" ":\"jh2odo\""


    #set UID, app name and version for sis
    packageheader = "$${LITERAL_HASH}{\"WAAnalyzer\"}, (0xE277CC23), 2, 2, 0, TYPE=SA"

    my_deployment.pkg_prerules += packageheader vendorinfo

    DEPLOYMENT += my_deployment

}


# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF



# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable.

CONFIG += mobility
MOBILITY += contacts \
            feedback \
            systeminfo

SOURCES += main.cpp mainwindow.cpp \
    wa/nbu.cpp \
    wa/logger.cpp \
    wa/log.cpp \
    wa/line.cpp \
    wa/group.cpp \
    wa/contacts.cpp \
    wa/contact.cpp \
    util/QsKineticScroller.cpp \
    wa/analyzer.cpp \
    wa/notification.cpp \
    wa/message.cpp \
    wa/graph.cpp \
    wa/stats.cpp \
    util/charts.cpp \
    wa/notificationlistviewdelegate.cpp \
    util/Gesture.cpp
HEADERS += mainwindow.h \
    wa/nbu.h \
    wa/logger.h \
    wa/log.h \
    wa/line.h \
    wa/group.h \
    wa/contacts.h \
    wa/contact.h \
    util/QsKineticScroller.h \
    wa/analyzer.h \
    wa/notification.h \
    wa/message.h \
    wa/graph.h \
    wa/stats.h \
    util/charts.h \
    wa/notificationlistviewdelegate.h \
    util/Gesture.h
FORMS += mainwindow.ui

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

RESOURCES += \
    images.qrc \
    translations.qrc

TRANSLATIONS += translations/wa_es.ts \
                translations/wa_en.ts \
                translations/wa_fr.ts \
                translations/wa_de.ts \
                translations/wa_pt.ts \
                translations/wa_it.ts \
                translations/wa_ru.ts

OTHER_FILES += \
    docs/TODO.txt \
    docs/RELEASE.txt \
    docs/README.txt \
    docs/NOKIASUPPORT.txt \
    docs/NOKIASTORE.txt \
    docs/FEATURES.txt \
    docs/CHANGELOG.txt

