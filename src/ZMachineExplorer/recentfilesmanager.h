#ifndef RECENTFILESMANAGER_H
#define RECENTFILESMANAGER_H

#include <QStringList>
#include <QSettings>

#define RECENT_FILES_KEY "recentFiles"

class RecentFilesManager
{
public:
	RecentFilesManager();
	QStringList getRecentFiles();
	void clearRecentFiles();
	void addRecentFile(QString filePath);

private:
	QSettings m_settings;
};

#endif	// RECENTFILESMANAGER_H
