#ifndef QMQTT2QML_DEFINES
#define QMQTT2QML_DEFINES

#include <QtCore/qglobal.h>

#if defined(QMQTT2QML_SHARED)
#  if defined(QMQTT2QML_SHARED_EXPORT)
#    define QMQTT2QML_EXPORT Q_DECL_EXPORT
#  else
#    define QMQTT2QML_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define QMQTT2QML_EXPORT
#endif

#endif // QMQTT2QML_DEFINES
