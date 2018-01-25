#ifndef MYGUI_GLOBAL_H
#define MYGUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MYGUI_LIBRARY)
#  define MYGUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MYGUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MYGUI_GLOBAL_H
