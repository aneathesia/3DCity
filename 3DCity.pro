QT       += core gui widgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    city.cpp \
    flycontrol.cpp \
    ground.cpp \
    house.cpp \
    main.cpp \
    mainwindow.cpp \
    route.cpp \
    searchdialog.cpp \
    vertex.cpp

HEADERS += \
    camera.h \
    city.h \
    flycontrol.h \
    ground.h \
    house.h \
    mainwindow.h \
    route.h \
    searchdialog.h \
    vertex.h

win32: LIBS += -lOpenGL32

win32: LIBS += -lGlU32

FORMS += \
    flycontrol.ui \
    mainwindow.ui \
    searchdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES += \
    ground.frag \
    ground.vert \
    quads.frag \
    quads.vert \
    route.frag \
    route.vert \
    shadow_mapping.frag \
    shadow_mapping.vert \
    shadow_mapping_depth.frag \
    shadow_mapping_depth.vert \
    shadow_mapping_ground.frag \
    shadow_mapping_ground.vert \
    shadow_mapping_top.frag \
    shadow_mapping_top.vert \
    surface.frag \
    surface.vert \
    topsurface.frag \
    topsurface.vert
