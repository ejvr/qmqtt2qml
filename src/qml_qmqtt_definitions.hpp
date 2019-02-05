#ifndef QML_QMQTT_DEFINITIONS_H
#define QML_QMQTT_DEFINITIONS_H

#include <QtGlobal>

#ifndef QT_STATIC
#  if defined(QMQTT2QML_EXPORT_SHARED)
#    define QMQTT2QML_EXPORT Q_DECL_EXPORT
#  else
#    define QMQTT2QML_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define QMQTT2QML_EXPORT
#endif

#endif // QML_QMQTT_DEFINITIONS_H
