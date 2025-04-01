#pragma once
#include <3ds.h>
#include <cstdint>

namespace FsLib
{
    /// @brief Attempts to delete extra data with ExtraDataID
    /// @param MediaType MediaType. Either MEDIA_TYPE_SD or MEDIA_TYPE_NAND can be passed. Most Extra Data is SD.
    /// @param ExtraDataID ID of extra data to delete.
    /// @return True on success. False on failure.
    bool DeleteExtraData(FS_MediaType MediaType, uint32_t ExtraDataID);
} // namespace FsLib
