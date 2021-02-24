QT       += core gui
QT       += printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# application icon
win32{
    RC_ICONS = icon/GlassPlotterIcon.ico
}
macx{
    ICON = icon/GlassPlotterIcon.icns
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

INCLUDEPATH += $$PWD/QCustomPlot # QCustomPlot
INCLUDEPATH += $$PWD/pugixml/src # pugixml
INCLUDEPATH += $$PWD/spline/src  # spline

SOURCES += \
    about_dialog.cpp \
    catalog_view_form.cpp \
    catalog_view_setting_dialog.cpp \
    curve_fitting_dialog.cpp \
    dispersion_plot_form.cpp \
    dndt_plot_form.cpp \
    glass.cpp \
    glass_catalog.cpp \
    glass_datasheet_form.cpp \
    glass_selection_dialog.cpp \
    glassmap_form.cpp \
    main.cpp \
    mainwindow.cpp \
    preset_dialog.cpp \
    qcpscatterchart.cpp \
    qcputil.cpp \
    spectral_line.cpp \
    transmittance_plot_form.cpp \
    QCustomPlot/qcustomplot.cpp \ # QCustomPlot
    pugixml/src/pugixml.cpp       # pugixml

HEADERS += \
    about_dialog.h \
    catalog_view_form.h \
    catalog_view_setting_dialog.h \
    curve_fitting_dialog.h \
    dispersion_formula.h \
    dispersion_plot_form.h \
    dndt_plot_form.h \
    glass.h \
    glass_catalog.h \
    glass_datasheet_form.h \
    glass_selection_dialog.h \
    glassmap_form.h \
    mainwindow.h \
    preset_dialog.h \
    qcpscatterchart.h \
    qcputil.h \
    spectral_line.h \
    transmittance_plot_form.h \
    QCustomPlot/qcustomplot.h \    # QCustomPlot
    pugixml/src/pugiconfig.hpp \   # pugixml
    pugixml/src/pugixml.hpp \
    spline/src/spline.h            # spline

FORMS += \
    about_dialog.ui \
    catalog_view_form.ui \
    catalog_view_setting_dialog.ui \
    curve_fitting_dialog.ui \
    dispersion_plot_form.ui \
    dndt_plot_form.ui \
    glass_datasheet_form.ui \
    glass_selection_dialog.ui \
    glassmap_form.ui \
    mainwindow.ui \
    preset_dialog.ui \
    transmittance_plot_form.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

