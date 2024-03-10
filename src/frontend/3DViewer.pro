win32|macx {  #win32, macx
    QT       += core gui  widgets printsupport openglwidgets
    #LIBS     += -framework GLUT -framework OpenGL -framework Cocoa
    #LIBS     += -lopengl32
} else {
    QT       += core gui openglwidgets 
    #LIBS     += -lOpenGL -lGLU -lglut
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(gif/QtGifImage/src/gifimage/qtgifimage.pri)

SOURCES += \
    ../backend/parser.c \
    ../backend/my_affine.c \
    ../backend/my_matrix/my_matrix.c \
    main.cpp \
    mainwindow.cpp \
    qlwidget.cpp

HEADERS += \
    ../backend/my_matrix/my_matrix.h \
    ../backend/my_viewer.h \
    mainwindow.h \
    qlwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
