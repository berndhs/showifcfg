#ifndef GEUZEN_QML_UTILS_PLUGIN_H
#define GEUZEN_QML_UTILS_PLUGIN_H

#include <QDeclarativeExtensionPlugin>

class GeuzenQmlUtilsPlugin : public QDeclarativeExtensionPlugin
{
Q_OBJECT
public:

  void registerTypes (const char *uri);
  
};


#endif