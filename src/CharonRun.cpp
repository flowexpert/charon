#include "CharonRun.h"
#include "CharonRun.moc"

#include "FileManager.h"

CharonRun::CharonRun(QObject* pp) : QObject(pp)
{
}

void CharonRun::updatePlugins() const {
	FileManager::instance().loadPluginInformation();
	FileManager::instance().updateMetadata();
}
