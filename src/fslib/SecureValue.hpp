#pragma once
#include <cstdint>

namespace FsLib
{
    /// @brief Attempts to retrieve the secure value of the UniqueID passed.
    /// @param UniqueID UniqueID, usually the lower title ID >> 8, of the value to retrieve.
    /// @param SecureValue uint64_t to which the value is written.
    /// @return True on success. False on failure/value doesn't exist.
    bool GetSecureValueForTitle(uint32_t UniqueID, uint64_t &SecureValue);

    /// @brief Attempts to set the secure value the UniqueID passed.
    /// @param UniqueID UniqueID of the target title.
    /// @param SecureValue Value of secure value.
    /// @return True on success, false on failure.
    bool SetSecureValueForTitle(uint32_t UniqueID, uint64_t SecureValue);
} // namespace FsLib
