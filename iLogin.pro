#-------------------------------------------------
#
# Project created by QtCreator 2013-07-05T23:46:06
#
#-------------------------------------------------

QT       += core gui network script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iLogin
TEMPLATE = app

SOURCES += main.cpp\
    netaccess.cpp \
    qsingleapplication.cpp \
    systemtrayicon.cpp \
    appui.cpp

HEADERS  += \
    netaccess.h \
    qsingleapplication.h \
    systemtrayicon.h \
    appui.h \
    Data.h

CONFIG += mobility
MOBILITY = 

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/version.xml

FORMS += \
    appui.ui

RESOURCES += \
    res.qrc

win32:
RC_ICONS = ../icon.ico
VERSION = 1.0

