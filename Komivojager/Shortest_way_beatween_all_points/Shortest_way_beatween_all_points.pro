#-------------------------------------------------
#
# Project created by QtCreator 2018-01-12T13:07:26
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport
QT       += webenginewidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Shortest_way_beatween_all_points
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
           mainwindow.cpp \
           Funcs/Funcs.cpp \
           CItem_Delegate_LineEdit/CItem_Delegate_Line_Edit.cpp \
           algorithm/m_algorithm.cpp


HEADERS  += mainwindow.h \
            Funcs/Funcs.h \
            CItem_Delegate_LineEdit/CItem_Delegate_Line_Edit.h \
            algorithm/m_algorithm.h \
            include_libraries.h


FORMS    += mainwindow.ui
