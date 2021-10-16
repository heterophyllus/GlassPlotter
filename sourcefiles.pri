#############################################################################
###################### PROJECT FILES SECTION ################################
#############################################################################

SOURCES += \
    src/air.cpp \
    src/preference_dialog.cpp \
    src/catalog_view_form.cpp \
    src/catalog_view_setting_dialog.cpp \
    src/curve_fitting_dialog.cpp \
    src/dispersion_plot_form.cpp \
    src/dndt_plot_form.cpp \
    src/glass.cpp \
    src/glass_catalog.cpp \
    src/glass_catalog_manager.cpp \
    src/glass_datasheet_form.cpp \
    src/glass_selection_dialog.cpp \
    src/glass_search_form.cpp \
    src/glassmap_form.cpp \
    src/load_catalog_result_dialog.cpp \
    src/main.cpp \
    src/main_window.cpp \
    src/preset_dialog.cpp \
    src/property_plot_form.cpp \
    src/qcpscatterchart.cpp \
    src/spectral_line.cpp \
    src/transmittance_plot_form.cpp \
    3rdparty/QCustomPlot/qcustomplot.cpp \
    3rdparty/pugixml/src/pugixml.cpp


HEADERS += \
    src/air.h \
    src/preference_dialog.h \
    src/catalog_view_form.h \
    src/catalog_view_setting_dialog.h \
    src/curve_fitting_dialog.h \
    src/dispersion_formula.h \
    src/dispersion_plot_form.h \
    src/dndt_plot_form.h \
    src/glass.h \
    src/glass_catalog.h \
    src/glass_catalog_manager.h \
    src/glass_datasheet_form.h \
    src/glass_selection_dialog.h \
    src/glass_search_form.h \
    src/glassmap_form.h \
    src/load_catalog_result_dialog.h \
    src/main_window.h \
    src/preset_dialog.h \
    src/property_plot_form.h \
    src/qcpscatterchart.h \
    src/spectral_line.h \
    src/transmittance_plot_form.h \
    3rdparty/QCustomPlot/qcustomplot.h

    #pugixml/src/pugiconfig.hpp \   # pugixml
    #pugixml/src/pugixml.hpp \
    #spline/src/spline.h            # spline

FORMS += \
    src/preference_dialog.ui \
    src/glassmap_form.ui \
    src/catalog_view_form.ui \
    src/catalog_view_setting_dialog.ui \
    src/curve_fitting_dialog.ui \
    src/dispersion_plot_form.ui \
    src/dndt_plot_form.ui \
    src/glass_datasheet_form.ui \
    src/glass_selection_dialog.ui \
    src/glass_search_form.ui \
    src/load_catalog_result_dialog.ui \
    src/main_window.ui \
    src/preset_dialog.ui \
    src/transmittance_plot_form.ui
