#pragma once

#include <QtCore/qglobal.h>

#if defined(MUSICLYRIC_LIBRARY)
#  define MUSICLYRICSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MUSICLYRICSHARED_EXPORT Q_DECL_IMPORT
#endif

