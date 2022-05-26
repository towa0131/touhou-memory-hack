#include <iostream>
#include <vector>
#include <windows.h>
#include "memorystream.h"

#define NOP 0x90

using namespace std;

int main(int argc, char** argv) {
    int option = -1;
    bool inf_life = false;
    bool inf_bomb = false;

    uint64_t addr[2] = {
        0x00428DEB,
        0x004289E1
    };

    HWND hWnd = FindWindowA(0, "東方紅魔郷　～ the Embodiment of Scarlet Devil");
    if (hWnd == 0) {
        cout << "ウィンドウが見つかりませんでした。" << endl;
        system("PAUSE");
        return 1;
    }

    while (true) {
        do {
            cout << "どの機能を設定しますか？" << endl;
            cout << "0 : 残機無限（" << (inf_life ? "有効" : "無効") << "）" << endl;
            cout << "1 : ボム無限（" << (inf_bomb ? "有効" : "無効") << "）" << endl;
            cout << "9 : 終了" << endl;
            cout << ">> ";
            cin >> option;

            if (option == 9) return 0;
            if (!(0 <= option && option <= 1)) {
                cout << "値が不正です。" << endl;
            }
        } while (!(0 <= option && option <= 1));

        DWORD pid;
        GetWindowThreadProcessId(hWnd, &pid);
        cout << "PID : " << pid << " を開いています..." << endl;
        HANDLE hProc = OpenProcess(
            PROCESS_CREATE_THREAD |
            PROCESS_VM_OPERATION |
            PROCESS_VM_WRITE, FALSE
            , pid);

        if (!hProc) {
            cout << "PID : " << pid << " を開くことができませんでした。" << endl;
            system("PAUSE");
            return 1;
        }

        vector<uint8_t> vec;

        switch (option) {
        case 0:
            if (inf_life) {
                vec = { 0x2C, 0x01 };
            } else {
                vec = { NOP, NOP };
            }
            inf_life = !inf_life;
            break;
        case 1:
            if (inf_bomb) {
                vec = { 0x80, 0xEA, 0x01 };
            } else {
                vec = { NOP, NOP, NOP };
            }
            inf_bomb = !inf_bomb;
            break;
        }

        MemoryStream stream(hProc, addr[option]);
        if (stream.write(vec)) {
            cout << "メモリの書き込みに成功しました。" << endl;
        }
        else {
            cout << "メモリの書き込みに失敗しました。" << endl;
        }

        stream.close();
    }

    return 0;
}
