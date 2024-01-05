QT       += core gui serialbus

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    modbustcp.cpp

HEADERS += \
    mainwindow.h \
    modbustcp.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    PNG_X/X_I_1.png \
    PNG_X/X_I_2.png \
    PNG_X/X_M_1.png \
    PNG_X/X_M_2.png \
    PNG_X/X_P_1.png \
    PNG_X/X_P_2.png \
    PNG_X/X_Palm.png \
    PNG_X/X_Palm_T_0.png \
    PNG_X/X_Palm_T_1.png \
    PNG_X/X_Palm_T_2.png \
    PNG_X/X_R_1.png \
    PNG_X/X_R_2.png \
    PNG_X/X_T_1.png \
    PNG_X/X_T_2.png
