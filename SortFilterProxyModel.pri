!c++11: warning("SortFilterProxyModel needs c++11, add CONFIG += c++11 to your .pro")

INCLUDEPATH += $$PWD/Src/core

HEADERS += $$PWD/Src/core/qqmlsortfilterproxymodel.h \
    $$PWD/Src/core/filter.h \
    $$PWD/Src/core/sorter.h
SOURCES += $$PWD/Src/core/qqmlsortfilterproxymodel.cpp \
    $$PWD/Src/core/filter.cpp \
    $$PWD/Src/core/sorter.cpp
