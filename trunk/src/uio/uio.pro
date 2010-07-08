######################################################################
# Automatically generated by qmake (2.01a) Tue May 18 20:38:03 2010
######################################################################

TEMPLATE = lib

CONFIG -= thread
CONFIG += opengl x11
CONFIG += debug
CONFIG -= release

linux-g++ {
        DEFINES += LIN LININTEL
        HARDWARE_PLATFORM = $$system(uname -m)
        contains( HARDWARE_PLATFORM, x86_64 ) {
                # 64-bit Linux
                message ("Adding Linux 64 bits compile flags and definitions")
                DEFINES += _OCC64
                QMAKE_CXXFLAGS += -m64

        } else {
                # 32-bit Linux
        }
} else {
        message ("Not supported on any platform other than linux!")
}

DEFINES += HAVE_CONFIG_H HAVE_IOSTREAM HAVE_FSTREAM HAVE_LIMITS

DEPENDPATH += . ../qocc ../lnk
INCLUDEPATH += /opt/occ63/inc/ . ../qocc ../lnk

LIBS += -L/opt/occ63/lib/ -L../../bin\
-lTKShHealing -lTKOffset -lTKBool -lTKSTEPBase -lTKSTEP \
-lTKService -lTKV3d -lTKernel -lTKIGES -lPTKernel -lTKSTL \
-lTKVRML -lTKTopAlgo -lTKBRep -lTKPShape -lTKShapeSchema

# Input
HEADERS += uio.hh dispShape.hh tst.hh
SOURCES += uio.cc dispShape.cc tst.cc


DESTDIR = ../../bin
TARGET = OccUio

OBJECTS_DIR = .obj/
UI_DIR = .ui/
MOC_DIR = .moc/
