#ifndef PLOTMOD_GLOBAL_H
#define PLOTMOD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLOTMOD_LIBRARY)
#  define PM_EXPORT Q_DECL_EXPORT
#else
#  define PM_EXPORT Q_DECL_IMPORT
#endif

#endif // PLOTMOD_GLOBAL_H
