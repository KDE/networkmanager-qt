#ifndef LIBNMQT_DEBUG_H
#define LIBNMQT_DEBUG_H

#include <QDebug>

#define nmDebug() \
	qDebug() << "libnm-qt" << QCoreApplication::applicationName() << '(' << QCoreApplication::applicationPid() << ')' << __PRETTY_FUNCTION__ << ':'

#endif
