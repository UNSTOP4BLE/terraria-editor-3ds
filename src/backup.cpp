#include "backup.h"
#include "app.h"
#include "fslib/DirectoryFunctions.hpp"
#include "fslib/FileFunctions.hpp"
#include "savefile.h"
#include "fslib/FsLib.hpp"
#include <cstdio>
#include <sstream>

void initFileSystem(const char* path) {
    const char* firstSlash = strchr(path, '/');
    std::string prefix = std::string(path, firstSlash ? firstSlash - path : 0);
    if (firstSlash) path = firstSlash;

    std::stringstream ss(path);
    std::string segment, current = prefix;

    while (std::getline(ss, segment, '/')) {
        if (!segment.empty()) {
            current += "/" + segment;
            if (ss.peek() == EOF) break;

            if (!FsLib::DirectoryExists(utf8_to_utf16(current)))
                ASSERTFUNC(FsLib::CreateDirectory(utf8_to_utf16(current)), current.c_str());
        }
    }
}

void writeBackup(const char *backupPrefix, std::u16string srcpath, std::string filename) {
    //read
    FsLib::File fin(srcpath, FS_OPEN_READ);

    ASSERTFUNC(fin.IsOpen(), "failed to open input save file");

    int filesize = fin.GetSize();
    uint8_t indata[filesize];

    fin.Read(&indata, filesize); 
    fin.Close();

    //write 
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t *)&unixTime);
	int day = timeStruct->tm_mday;
	int month = timeStruct->tm_mon;
	int year = timeStruct->tm_year +1900;

    char path[128];
    sprintf(path, "%s%d-%d-%d/%s.bak", backupPrefix, day, month, year, filename.c_str());
    if (!strcmp(backupPrefix, AUTOBACKUP_PATH))   
        sprintf(path, "%s%s.bak", backupPrefix, filename.c_str());
    initFileSystem(path);
    
    FILE *fout = fopen(path, "wb");
    char msg[256];
    sprintf(msg, "failed to write backup save file: %s", path);
    ASSERTFUNC(fout, msg);
    fwrite(indata, filesize, 1, fout);
    fclose(fout);
}

void writeAutoBackup(std::u16string srcpath, std::string filename) {
    writeBackup(AUTOBACKUP_PATH, srcpath, filename);
}

void deleteBackup(const char *path) {
    ASSERTFUNC(FsLib::DeleteFile(utf8_to_utf16(path)), "failed to delete backup file");
}

void restoreBackup(std::string path) {
    //read
    FILE *fin = fopen(path.c_str(), "rb");
    ASSERTFUNC(fin, "failed to open backup file");
    std::string fileext = path.substr(path.find_last_of("/") + 1, path.rfind(".") - path.find_last_of("/") - 1);
    
    fseek(fin, 0, SEEK_END);
    int filesize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    uint8_t indata[filesize];

    fread(&indata, filesize, 1, fin); 
    fclose(fin);

    //write
    path = EXTDATA_PATH + fileext;
    if (FsLib::FileExists(utf8_to_utf16(path)))
        ASSERTFUNC(FsLib::DeleteFile(utf8_to_utf16(path)), "failed to delete file");
    ASSERTFUNC(FsLib::CreateFile(utf8_to_utf16(path), filesize), "failed to replace file");
    FsLib::File fout(utf8_to_utf16(path), FS_OPEN_WRITE);
    ASSERTFUNC(fout.IsOpen(), "failed to restore save file");

    fout.Write(&indata, filesize); 
    fout.Close();
}

