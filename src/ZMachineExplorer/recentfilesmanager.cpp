#include "recentfilesmanager.h"

RecentFilesManager::RecentFilesManager() {}

QStringList RecentFilesManager::getRecentFiles()
{
    return m_settings.value(RECENT_FILES_KEY).toStringList();
}

void RecentFilesManager::clearRecentFiles()
{
    m_settings.setValue(RECENT_FILES_KEY, QStringList());
}

void RecentFilesManager::addRecentFile(QString filePath)
{
    QStringList list = m_settings.value(RECENT_FILES_KEY).toStringList();
    list.prepend(filePath);
    m_settings.setValue(RECENT_FILES_KEY, list);
}
