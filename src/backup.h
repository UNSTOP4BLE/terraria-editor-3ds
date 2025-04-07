#pragma once

#include <string>
#include <3ds.h>

#define EXTDATA_PATH "extdata:/v2/"
#define BACKUP_PATH "sdmc:/terraedit3d/backups/"
#define AUTOBACKUP_PATH "sdmc:/terraedit3d/auto-backups/"
void writeBackup(const char *backupPrefix, std::u16string srcpath, std::string filename);
void writeAutoBackup(std::u16string srcpath, std::string filename);
void restoreBackup(std::string path);
void deleteBackup(const char *path);