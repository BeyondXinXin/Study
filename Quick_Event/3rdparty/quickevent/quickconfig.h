#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD

#include <QtCore/qglobal.h>

#define QuickEvent_NAME "QuickEvent"
#define QuickEvent_VER "3.0.1"
#define QuickEvent_YEARS "2020"
#define QuickEvent_VER_MAJOR "3"
#define QuickEvent_VER_MINOR "0"
#define QuickEvent_VER_PATCH "1"

#if defined(USE_SHARED)
    #define LIBRARYSHARED_EXPORT Q_DECL_EXPORT
#elif defined(USE_STATIC)
    #define LIBRARYSHARED_EXPORT
#else
    #define LIBRARYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // INCLUDE_GUARD
