#include "ExtData.hpp"
#include "String.hpp"
#include <3ds.h>
#include <string>

extern std::string g_FsLibErrorString;

bool FsLib::DeleteExtraData(FS_MediaType MediaType, uint32_t ExtraDataID)
{
    FS_ExtSaveDataInfo ExtraDataInfo = {.mediaType = MediaType, .unknown = 0, .reserved1 = 0, .saveId = ExtraDataID, .reserved2 = 0};

    Result FsError = FSUSER_DeleteExtSaveData(ExtraDataInfo);
    if (R_FAILED(FsError))
    {
        g_FsLibErrorString = String::GetFormattedString("Error deleting extra data %08X: 0x%08X.", ExtraDataID, FsError);
        return false;
    }

    return true;
}
