TEMPLATE = app
TARGET = TheGame

QT += core gui widgets

CONFIG += c++14

SOURCES += \
    main.cpp \
    startdialog.cpp \
    core/objectmanager.cpp \
    core/gameeventhandler.cpp \
    graphics/gamescene.cpp \
    tiles/mountain.cpp \
    mainwindow.cpp \
    graphics/graphicsitem.cpp \
    tiles/village.cpp \
    core/misc.cpp \
    tiles/water.cpp \
    workers/soldier.cpp \
    core/perlinnoise.cpp \
    workers/farmer.cpp \
    workers/miner.cpp \
    buildings/barracks.cpp \
    workers/saboteur.cpp \
    buildings/mine.cpp

HEADERS += \
    startdialog.h \
    core/objectmanager.h \
    core/gameeventhandler.hh \
    graphics/gamescene.h \
    tiles/mountain.h \
    mainwindow.h \
    graphics/graphicsitem.h \
    tiles/village.hh \
    core/misc.h \
    tiles/water.h \
    workers/soldier.h \
    core/perlinnoise.h \
    workers/farmer.h \
    workers/miner.h \
    buildings/barracks.h \
    workers/saboteur.h \
    buildings/mine.h

win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/release/ -lCourseLib
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/debug/ -lCourseLib
else:unix: LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/ -lCourseLib

INCLUDEPATH += \
    $$PWD/../Course/CourseLib

DEPENDPATH += \
    $$PWD/../Course/CourseLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/release/libCourseLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/debug/libCourseLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/release/CourseLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/debug/CourseLib.lib
else:unix: PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/libCourseLib.a

FORMS += \
    startdialog.ui \
    mainwindow.ui

RESOURCES += \
    Images/forest.png \
    Images/grassland.png \
    Images/mountain.png \
    Images/village.png \
    Images/water.png \
    Images/house.png \
    Images/person.png
