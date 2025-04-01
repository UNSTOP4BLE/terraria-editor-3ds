#include "SecureValue.hpp"
#include "String.hpp"
#include <3ds.h>

extern std::string g_FsLibErrorString;

bool FsLib::GetSecureValueForTitle(uint32_t UniqueID, uint64_t &SecureValue)
{
    // This is more or less how the function returns if the secure exists.
    bool ValueExists = false;

    Result FsError = FSUSER_GetSaveDataSecureValue(&ValueExists, &SecureValue, SECUREVALUE_SLOT_SD, UniqueID, 0);
    if (R_FAILED(FsError))
    {
        g_FsLibErrorString = String::GetFormattedString("Error retrieving secure value: 0x%08X.", FsError);
        return false;
    }

    if (!ValueExists)
    {
        g_FsLibErrorString = String::GetFormattedString("Secure value for %08X doesn't exist.", UniqueID);
        return false;
    }

    return true;
}

bool FsLib::SetSecureValueForTitle(uint32_t UniqueID, uint64_t SecureValue)
{
    Result FsError = FSUSER_SetSaveDataSecureValue(SecureValue, SECUREVALUE_SLOT_SD, UniqueID, 0);
    if (R_FAILED(FsError))
    {
        g_FsLibErrorString = String::GetFormattedString("Error setting secure value: 0x%08X.", FsError);
        return false;
    }

    return true;
}
