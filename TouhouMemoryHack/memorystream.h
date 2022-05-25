class MemoryStream {
private:
    HANDLE hProcess;
    uint64_t offset;
    DWORD changeProtect(int byte, DWORD newProtect);
public:
    MemoryStream(HANDLE hProcess);

    MemoryStream(HANDLE hProcess, int offset);

    bool write(uint8_t byte);

    bool write(uint16_t word);

    bool write(uint32_t dword);

    bool write(std::vector<uint8_t> bytes);

    void setOffset(int offset);

    int getOffset();

    void close();
};