QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_POST_LINK += /usr/libexec/PlistBuddy -c "Add :NSMicrophoneUsageDescription string 'This app records audio.'" $$DESTDIR/qt-app.app/Contents/Info.plist

SOURCES += \
    addpatientdialog.cpp \
    filehandler.cpp \
    llmclient.cpp \
    main.cpp \
    mainwindow.cpp \
    patientrecord.cpp \
    settings.cpp \
    windowbuilder.cpp \
    transcript.cpp \
    summary.cpp \
    audiohandler.cpp \
    summarygenerator.cpp \
    summaryformatter.cpp \
    detailedsummaryformatter.cpp \
    concisesummaryformatter.cpp

HEADERS += \
    addpatientdialog.h \
    api.h \
    filehandler.h \
    llmclient.h \
    mainwindow.h \
    patientrecord.h \
    settings.h \
    windowbuilder.h \
    transcript.h \
    summary.h \
    audiohandler.h \
    summarygenerator.h \
    summaryformatter.h \
    detailedsummaryformatter.h \
    concisesummaryformatter.h

FORMS += \
    addpatientdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES +=
