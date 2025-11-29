QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network #webenginewidgets
QT += pdf pdfwidgets multimedia multimediawidgets sql
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
    bookinfo.cpp \
    booksellwidget.cpp \
    documentanalyze.cpp \
    loginwidget.cpp \
    main.cpp \
    dialog.cpp \
    messagebuilder.cpp \
    myaccountwidget.cpp \
    pdfviewer.cpp \
    searchbar.cpp \
    tcpsocket.cpp \
    userbasicinfowidget.cpp \
    userconfigwidget.cpp \
    videoinfo.cpp \
    videoplayerwidget.cpp \
    videosellwidget.cpp

HEADERS += \
    bookinfo.h \
    booksellwidget.h \
    dialog.h \
    documentanalyze.h \
    loginwidget.h \
    messagebuilder.h \
    myaccountwidget.h \
    pdfviewer.h \
    searchbar.h \
    tcpsocket.h \
    userbasicinfowidget.h \
    userconfigwidget.h \
    videoinfo.h \
    videoplayerwidget.h \
    videosellwidget.h

#DDINCLUDEPATH += C:/msys64/mingw64/include/poppler/qt5
#LIBS += -LC:/msys64/mingw64/lib #-lpoppler -lpoppler-qt5
# 链接必要的库文件
#LIBS += -lpoppler-cpp
#LIBS += -lpoppler-qt5

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config.ini

RESOURCES += \
    img.qrc
