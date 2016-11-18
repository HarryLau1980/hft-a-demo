#ifndef TRADEEXECMOD_GLOBAL_H
#define TRADEEXECMOD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TRADEEXECMOD_LIBRARY)
#  define TEM_EXPORT Q_DECL_EXPORT
#else
#  define TEM_EXPORT Q_DECL_IMPORT
#endif

#endif // TRADEEXECMOD_GLOBAL_H
