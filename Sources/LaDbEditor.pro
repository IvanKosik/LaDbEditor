#-------------------------------------------------
#
# Project created by QtCreator 2013-02-13T15:22:50
#
#-------------------------------------------------

QT       += core gui sql

TARGET = LaDbEditor
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SqlQuery.cpp \
    DB.cpp \
    Dialogs/DiagnosisCreationDialog.cpp \
    Dialogs/PhaseCreationDialog.cpp \
    ItemSelectionModels/DiagnosisItemSelectionModel.cpp \
    ItemSelectionModels/ItemSelectionModel.cpp \
    Dialogs/ParameterAddingDialog.cpp \
    Dialogs/ParameterCreationDialog.cpp \
    Dialogs/ResearchAddingDialog.cpp \
    Dialogs/ResearchCreationDialog.cpp \
    Dialogs/RangeCreationDialog.cpp \
    Dialogs/DbConnectionDialog.cpp \
    PhaseParameterModel.cpp

HEADERS  += MainWindow.h \
    SqlQuery.h \
    DB.h \
    Dialogs/DiagnosisCreationDialog.h \
    Dialogs/PhaseCreationDialog.h \
    ItemSelectionModels/DiagnosisItemSelectionModel.h \
    ItemSelectionModels/ItemSelectionModel.h \
    Dialogs/ParameterAddingDialog.h \
    Dialogs/ParameterCreationDialog.h \
    Dialogs/ResearchAddingDialog.h \
    Dialogs/ResearchCreationDialog.h \
    Dialogs/RangeCreationDialog.h \
    Dialogs/DbConnectionDialog.h \
    PhaseParameterModel.h

FORMS    += MainWindow.ui \
    Dialogs/DiagnosisCreationDialog.ui \
    Dialogs/PhaseCreationDialog.ui \
    Dialogs/ParameterAddingDialog.ui \
    Dialogs/ParameterCreationDialog.ui \
    Dialogs/ResearchAddingDialog.ui \
    Dialogs/ResearchCreationDialog.ui \
    Dialogs/RangeCreationDialog.ui \
    Dialogs/DbConnectionDialog.ui

DEPENDPATH +=.
INCLUDEPATH +=.

TRANSLATIONS += Translations/LaDbEditor_ru.ts
