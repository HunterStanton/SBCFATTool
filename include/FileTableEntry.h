// FileTableEntry.h
// Defines the FAT file table.
// 2023 Hunter Stanton

struct FileTableEntry
{
    int datOffset;                // offset in the DAT
    int fileSize;                 // how big the file is
    int unk;                      // ?
    int fileNameOffset;           // offset to the filename in the FAT
    std::string fileName;         // filename
    std::unique_ptr<char[]> data; // the file data itself
};