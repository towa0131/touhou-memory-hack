#include <windows.h>
#include <iostream>
#include <vector>
#include "memorystream.h"

MemoryStream::MemoryStream(HANDLE hProcess) : MemoryStream(hProcess, 0) {
}

MemoryStream::MemoryStream(HANDLE hProcess, int offset) : hProcess(hProcess), offset(offset) {
}

void MemoryStream::setOffset(int offset) {
    offset = offset;
}

int MemoryStream::getOffset() {
    return offset;
}

bool MemoryStream::write(uint8_t byte) {
    bool ret;
    uint8_t data[1] = { byte };
    DWORD oldProtect = changeProtect(sizeof(byte), PAGE_EXECUTE_READWRITE);
    ret = WriteProcessMemory(hProcess, (LPVOID)offset++, (LPVOID)data, 1, NULL);
    changeProtect(sizeof(byte), oldProtect);

    return ret;
}

bool MemoryStream::write(uint16_t word) {
    while (word != 0) {
        uint8_t byte = word % 256;
        word /= 256;
        if (!write(byte)) return false;
    }
    return true;
}

bool MemoryStream::write(uint32_t dword) {
    while (dword != 0) {
        uint8_t byte = dword % 0x100;
        dword /= 0x100;
        if (!write(byte)) return false;
    }
    return true;
}

bool MemoryStream::write(std::vector<uint8_t> bytes) {
    bool ret;
    DWORD oldProtect = changeProtect(sizeof(byte), PAGE_EXECUTE_READWRITE);
    uint8_t* data = bytes.data();
    ret = WriteProcessMemory(hProcess, (LPVOID)offset, (LPVOID)data, bytes.size(), NULL);
    changeProtect(sizeof(byte), oldProtect);
    offset += bytes.size();

    return ret;
}

void MemoryStream::close() {
    CloseHandle(hProcess);
}

DWORD MemoryStream::changeProtect(int byte, DWORD newProtect) {
    DWORD oldProtect;
    VirtualProtectEx(hProcess, (LPVOID)offset, byte, newProtect, &oldProtect);
    return oldProtect;
}