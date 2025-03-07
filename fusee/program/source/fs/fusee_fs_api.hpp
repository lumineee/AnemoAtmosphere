/*
 * Copyright (c) 2018-2020 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <exosphere.hpp>

namespace ams::fs {

    enum OpenMode {
        OpenMode_Read   = (1 << 0),
        OpenMode_Write  = (1 << 1),
        OpenMode_AllowAppend = (1 << 2),

        OpenMode_ReadWrite = (OpenMode_Read | OpenMode_Write),
        OpenMode_All       = (OpenMode_ReadWrite | OpenMode_AllowAppend),
    };

    struct ReadOption {
        u32 value;

        static const ReadOption None;
    };

    inline constexpr const ReadOption ReadOption::None = {0};

    inline constexpr bool operator==(const ReadOption &lhs, const ReadOption &rhs) {
        return lhs.value == rhs.value;
    }

    inline constexpr bool operator!=(const ReadOption &lhs, const ReadOption &rhs) {
        return !(lhs == rhs);
    }

    static_assert(util::is_pod<ReadOption>::value && sizeof(ReadOption) == sizeof(u32));

    struct WriteOption {
        u32 value;

        constexpr inline bool HasFlushFlag() const {
            return this->value & 1;
        }

        static const WriteOption None;
        static const WriteOption Flush;
    };

    inline constexpr const WriteOption WriteOption::None = {0};
    inline constexpr const WriteOption WriteOption::Flush = {1};

    inline constexpr bool operator==(const WriteOption &lhs, const WriteOption &rhs) {
        return lhs.value == rhs.value;
    }

    inline constexpr bool operator!=(const WriteOption &lhs, const WriteOption &rhs) {
        return !(lhs == rhs);
    }

    static_assert(util::is_pod<WriteOption>::value && sizeof(WriteOption) == sizeof(u32));

    enum DirectoryEntryType {
        DirectoryEntryType_Directory = 0,
        DirectoryEntryType_File      = 1,
    };

    struct DirectoryEntry {
        u64 file_size;
        u16 file_date;
        u16 file_time;
        u8 file_attr;
        char altname[13];
        char file_name[0x100];
    };

    constexpr ALWAYS_INLINE DirectoryEntryType GetEntryType(const DirectoryEntry &entry) {
        return (entry.file_attr & 0x10) ? DirectoryEntryType_Directory : DirectoryEntryType_File;
    }

    struct FileHandle {
        void *_handle;
    };

    struct DirectoryHandle {
        void *_handle;
    };

    bool MountSdCard();
    void UnmountSdCard();

    bool MountSystem();
    void UnmountSystem();

    Result GetEntryType(DirectoryEntryType *out_entry_type, bool *out_archive, const char *path);

    Result CreateFile(const char *path, s64 size);
    Result CreateDirectory(const char *path);
    Result OpenFile(FileHandle *out_file, const char *path, int mode);

    Result OpenDirectory(DirectoryHandle *out_dir, const char *path);

    Result ReadDirectory(s64 *out_count, DirectoryEntry *out_entries, DirectoryHandle handle, s64 max_entries);
    void CloseDirectory(DirectoryHandle handle);

    Result ReadFile(FileHandle handle, s64 offset, void *buffer, size_t size, const fs::ReadOption &option);
    Result ReadFile(FileHandle handle, s64 offset, void *buffer, size_t size);
    Result ReadFile(size_t *out, FileHandle handle, s64 offset, void *buffer, size_t size, const fs::ReadOption &option);
    Result ReadFile(size_t *out, FileHandle handle, s64 offset, void *buffer, size_t size);
    Result GetFileSize(s64 *out, FileHandle handle);
    Result FlushFile(FileHandle handle);
    Result WriteFile(FileHandle handle, s64 offset, const void *buffer, size_t size, const fs::WriteOption &option);
    Result SetFileSize(FileHandle handle, s64 size);
    int GetFileOpenMode(FileHandle handle);
    void CloseFile(FileHandle handle);

}
