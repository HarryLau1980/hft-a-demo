#ifndef EVENTBUSMOD_GLOBAL_H
#define EVENTBUSMOD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EVENTBUSMOD_LIBRARY)
#  define EBM_EXPORT Q_DECL_EXPORT
#else
#  define EBM_EXPORT Q_DECL_IMPORT
#endif

#endif // EVENTBUSMOD_GLOBAL_H
