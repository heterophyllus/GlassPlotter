QT       += core gui
QT       += printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# application icon
win32{
    RC_ICONS = GlassPlotterIcon.ico
}
macx{
    ICON = GlassPlotterIcon.icns
}

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
    aboutdialog.cpp \
    catalogviewform.cpp \
    curvefittingdialog.cpp \
    dispersionplotform.cpp \
    dndtplotform.cpp \
    glass.cpp \
    glasscatalog.cpp \
    glassdatasheetform.cpp \
    glassmapform.cpp \
    glassselectiondialog.cpp \
    main.cpp \
    mainwindow.cpp \
    presetdialog.cpp \
    pugixml.cpp \
    qcpscatterchart.cpp \
    qcputil.cpp \
    qcustomplot.cpp \
    spectralline.cpp \
    transmittanceplotform.cpp

HEADERS += \
    aboutdialog.h \
    catalogviewform.h \
    curvefittingdialog.h \
    dispersionformula.h \
    dispersionplotform.h \
    dndtplotform.h \
    glass.h \
    glasscatalog.h \
    glassdatasheetform.h \
    glassmapform.h \
    glassselectiondialog.h \
    mainwindow.h \
    presetdialog.h \
    pugiconfig.hpp \
    pugixml.hpp \
    qcpscatterchart.h \
    qcputil.h \
    qcustomplot.h \
    spectralline.h \
    spline.h \
    transmittanceplotform.h

FORMS += \
    aboutdialog.ui \
    catalogviewform.ui \
    curvefittingdialog.ui \
    dispersionplotform.ui \
    dndtplotform.ui \
    glassdatasheetform.ui \
    glassmapform.ui \
    glassselectiondialog.ui \
    mainwindow.ui \
    presetdialog.ui \
    transmittanceplotform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
