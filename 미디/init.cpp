//������ ��Ÿ�� ������Դϴ�.
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//�������
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
//#include <crtdbg.h>
#include "RtMidi.h"
#include "keys.hpp"
#include "key_data.h"


/*��ġ�е� Ű
* 71 72 73 74 75 76 77 78
* 61 62 63 64 65 66 67 68
* 51 52 53 54 55 56 57 58
* 41 42 43 44 45 46 47 48
* 31 32 33 34 35 36 37 38
* 21 22 23 24 25 26 27 28
* 11 12 13 14 15 16 17 18
* 01 02 03 04 05 06 07 08
*/

//���ӽ����̽�
using namespace std;

//��������
INPUT keyinput; 
bool up = false; //Ű���庰 �̸� ��� �� ���� �Ʒ��� ���п�
WORD keys[8][8] = { 0, };
string command;
bool succedchange;
int changing_key;
RtMidiIn* midiin = 0;
HHOOK hHook;

//�Լ� �κ�
void gotoxy(short x, short y) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void presskey(WORD vkey, bool press) {
    if (vkey == 0) return;
    keyinput.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
    keyinput.ki.wVk = vkey;
    if (press) { keyinput.ki.dwFlags = 0; }
    else { keyinput.ki.dwFlags = KEYEVENTF_KEYUP; }
    SendInput(1, &keyinput, sizeof(INPUT));
}
void changecolor(int foreground, int background) {
    int color = foreground + background * 16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
string getmidiname(int port, RtMidiIn* mi) {
    try {
        string name = mi->getPortName(port);
        return name.substr(0, name.size() - 2);
    }
    catch (RtMidiError& error) {
        throw error;
    }
}
COORD lchpadxy;
void launchpadToXY(int num) {
    lchpadxy.X = (num % 10) - 1;
    lchpadxy.Y = 7 - ((num / 10) % 10);
}
//�Ϲ� �ݹ�
void mycallback(double deltatime, vector< unsigned char >* message, void* userData)
{
    if (message->size() > 2) {
        if ((int)message->at(0) == 144) {
            int lkey = (int)message->at(1) - 10;
            if (lkey < 0) return;
            if ((int)message->at(2) == 127) {
                //��ġ�е� Ű ����
                //(int)message->at(1) - 10 �� ��ư ��ȣ�Դϴ�.
                launchpadToXY(lkey);
                presskey(keys[lchpadxy.X][lchpadxy.Y], true);
            }
            else {
                //��ġ�е� Ű ����
                launchpadToXY(lkey);
                presskey(keys[lchpadxy.X][lchpadxy.Y], false);
            }
        }
    }
}
//Ű ����� �ݹ�
void changkeycallback(double daltatime, vector<unsigned char>* message, void* userData) {
    if (message->size() > 2) {
        if ((int)message->at(0) == 144) {
            if ((int)message->at(2) == 127) {
                //��ġ�е� Ű ����
                //(int)message->at(1) - 10 �� ��ư ��ȣ�Դϴ�.
                changing_key = (int)message->at(1) - 10;
                launchpadToXY(changing_key);
                succedchange = false;
            }
        }
    }
}
WORD changekey;
void UnHook();
void SetHook();
//Ű �Է¿� �ݹ�
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;
        //if (nCode >= 0 && (int)wParam == 256) {
        if (key_to_string(pKey->vkCode, false) == "err ") {
            cout << "�������� ���� Ű�Դϴ�." << endl;
        }
        else {
            changekey = (int)pKey->vkCode;
            PostQuitMessage(0);
            UnHook();
            return 0;
        }
    }
    CallNextHookEx(hHook, nCode, wParam, lParam);
    return 0;
}

bool continuey;
LRESULT CALLBACK KeyboardProc2(int nCode, WPARAM wParam, LPARAM lParam) {
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;
        if (pKey->vkCode == key_y) {
            continuey = true;
            PostQuitMessage(0);
            UnHook();
            return 0;
        }
        else if (pKey->vkCode == VK_RETURN) {
            PostQuitMessage(0);
            UnHook();
            return 0;
        }
    }
    CallNextHookEx(hHook, nCode, wParam, lParam);
    return 0;
}

int keymode;
LRESULT CALLBACK KeyboardProc3(int nCode, WPARAM wParam, LPARAM lParam) {
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;
        if (pKey->vkCode == VK_ESCAPE) {
            keymode = 1;
            succedchange = false;
            PostQuitMessage(0);
            UnHook();
            return 0;
        }
        if (pKey->vkCode == VK_DELETE) {
            keymode = 2;
            succedchange = false;
            PostQuitMessage(0);
            UnHook();
            return 0;
        }
        if (pKey->vkCode == VK_RETURN) {
            keymode = 0;
            succedchange = false;
            PostQuitMessage(0);
            UnHook();
            return 0;
        }
    }
    CallNextHookEx(hHook, nCode, wParam, lParam);
    return 0;
}

void designer(int port) {
    cout << "����� ��ġ �̸� : " << getmidiname(port - 1, midiin) << endl << endl;
    //��Ʈ ����
    try {
        midiin->openPort(port - 1);
    }
    catch (int)
    {
        throw;
    }
    //�ݹ��Լ� ����
    midiin->setCallback(&mycallback);
    //RtMidi ���� ����
    midiin->ignoreTypes(false, false, false);
    //�̺κп��� cin.get();�̳� ��Ÿ ��ɾ �̿��ؼ� �Է¹����� �����մϴ�.
    //���� �����ؾ��� �κ��� �����, cleanup();�̳�, delete midiin;�� ����ϼ���.
    for (int c1 = 0; c1 < 8; c1++) {
        cout << "������������ ������������ ������������ ������������ ������������ ������������ ������������ ������������" << endl;
        for (int c2 = 0; c2 < 2; c2++) {
            for (int c3 = 0; c3 < 8; c3++) {
                cout << "��" << key_to_string(keys[c3][c1], c2) << "�� ";
            }
            cout << endl;
        }
        cout << "������������ ������������ ������������ ������������ ������������ ������������ ������������ ������������" << endl;
    }
    cout << endl << "��ɾ�" << endl << "quit : �����մϴ�." << endl << "change : �Է� ��ġ�� �����մϴ�." << endl << "edit : Ű�� �����մϴ�." << endl << ":";
}

void SetHook3() {
    HMODULE hInstance = GetModuleHandle(NULL);
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc3, hInstance, NULL);
}

void SetHook2() {
    HMODULE hInstance = GetModuleHandle(NULL);
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc2, hInstance, NULL);
}

void edit(int port) {
    MSG msg;
    system("cls");
keychange:    cout << "��ġ�е� ��ư�� �����մϴ�. ENTERŰ�� ������ ��ӵǰ�," << endl << "ESC�� ������ ������, delŰ�� ������ Ű ������ �����մϴ�:";
    keymode = 0;
    SetHook3();
    GetMessage(&msg, NULL, NULL, NULL);
    if (keymode == 1){//Ż��
        delete midiin;
        midiin = new RtMidiIn();
        return;
    }
    else if (keymode == 2) {//����
        system("cls");
        cout << "��ġ�е� ��ư�� �����մϴ�." << endl << "������ ��ġ�е� ��ư�� �����ּ���";
        delete midiin;
        midiin = new RtMidiIn();
        succedchange = true;
        midiin->openPort(port - 1);
        midiin->setCallback(&changkeycallback);
        midiin->ignoreTypes(false, false, false);
        while (succedchange) { Sleep(500); }
        cout << endl;
        changecolor(7, 0);
        for (int c1 = 0; c1 < 8; c1++) {
            cout << "������������ ������������ ������������ ������������ ������������ ������������ ������������ ������������" << endl;
            for (int c2 = 0; c2 < 2; c2++) {
                for (int c3 = 0; c3 < 8; c3++) {
                    cout << "��" << key_to_string(keys[c3][c1], c2) << "�� ";
                }
                cout << endl;
            }
            cout << "������������ ������������ ������������ ������������ ������������ ������������ ������������ ������������" << endl;
        }
        keys[lchpadxy.X][lchpadxy.Y] = 0;
        changecolor(0, 7);
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 2);
        cout << "������������ ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 3);
        cout << "��" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], false) << "�� ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 4);
        cout << "��" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], true) << "�� ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 5);
        cout << "������������ ";
        changecolor(7, 0);
        gotoxy(0, 34);
        goto end;
    }
    else {
        system("cls");
        cout << "��ġ�е� ��ư�� �����մϴ�." << endl << "������ ��ġ�е� ��ư�� �����ּ���:";
        delete midiin;
        midiin = new RtMidiIn();
        succedchange = true;
        midiin->openPort(port - 1);
        midiin->setCallback(&changkeycallback);
        midiin->ignoreTypes(false, false, false);
        while (succedchange) { Sleep(500); }
        cout << endl;
        changecolor(7, 0);
        for (int c1 = 0; c1 < 8; c1++) {
            cout << "������������ ������������ ������������ ������������ ������������ ������������ ������������ ������������" << endl;
            for (int c2 = 0; c2 < 2; c2++) {
                for (int c3 = 0; c3 < 8; c3++) {
                    cout << "��" << key_to_string(keys[c3][c1], c2) << "�� ";
                }
                cout << endl;
            }
            cout << "������������ ������������ ������������ ������������ ������������ ������������ ������������ ������������" << endl;
        }
        changecolor(0, 7);
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 2);
        cout << "������������ ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 3);
        cout << "��" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], false) << "�� ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 4);
        cout << "��" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], true) << "�� ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 5);
        cout << "������������ ";
        changecolor(7, 0);
        gotoxy(0, 33);
        cout << endl << "���������� �� ��ư�� ���õǾ����ϴ�." << endl << "Ű���忡�� ������ Ű�� �����ּ���:" << endl;
    hooking:
        SetHook();
        GetMessage(&msg, NULL, NULL, NULL);

        if (key_to_string(changekey, false) == "err ") {
            cout << "�������� ���� Ű�Դϴ�. Ű�� �ٽ� �����ּ���." << endl;
            goto hooking;
        }
        keys[lchpadxy.X][lchpadxy.Y] = changekey;
        cout << "������ ���� ����Ǿ����ϴ�." << endl;
        cout << "������������ " << endl
            << "��" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], false) << "�� " << endl
            << "��" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], true) << "�� " << endl
            << "������������ " << endl;
end:
        delete midiin;
        midiin = new RtMidiIn();
        ofstream out("key.lchkey");
        if (!out.is_open()) {
            //���� ���� ����
            MessageBox(NULL, TEXT("Ű ���� ������ �� �� �����ϴ�. Ű ������ ������� �ʽ��ϴ�."), TEXT("����"), MB_ICONERROR | MB_OK);
        }
        else {
            for (int c = 7; c > -1; c--) {
                for (int c1 = 0; c1 < 8; c1++) {
                    out << keys[c1][c] << endl;
                }
            }
        }
        out.close();
        cout << "���������� �����Ǿ����ϴ�." << endl;
        cout << "����ؼ� �ٸ� Ű�� �����Ͻðڽ��ϱ�?" << endl << "(yŰ�� ������ �����ϰ�, ����Ű�� ������ ���ư��ϴ�.):";
        continuey = false;
        SetHook2();
        GetMessage(&msg, NULL, NULL, NULL);
        if (continuey) {
            system("cls");
            goto keychange;
        }
    }
}

void SetHook() {
    HMODULE hInstance = GetModuleHandle(NULL);
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, NULL);
}

void UnHook() {
    UnhookWindowsHookEx(hHook);
}

int main()
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //â ����(ũ������, Ȯ���ư ����, ��ũ�� ����)
    system("mode con cols=72 lines=41");
    SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
pushenter:    
    cout << endl;
scandevice:
    midiin = new RtMidiIn();
    int nPorts = midiin->getPortCount();
    if (nPorts == 0) {
        cout << "��ġ�� �˻��� �� �����ϴ�." << endl << endl << "������ Ȯ���� �� ����Ű�� �����ּ���.";
        delete midiin;
        cin.get();
        system("cls");
        goto pushenter;
    }
    cout << "�� " << nPorts << "���� ��ġ�� �˻��Ǿ����ϴ�." << endl;

    int devicenum;
    string portName;
    cout << endl;
    for (int i = 0; i < nPorts; i++) {
        try {
            portName = getmidiname(i, midiin);
        }
        catch (RtMidiError& error) {
            MessageBox(NULL, TEXT("�� �� ���� ������ �߻��߽��ϴ�."), TEXT("����"), MB_OK);
            error.printMessage();
            delete midiin;
            return -1;
        }
        std::cout << i + 1 << "�� ��ġ : " << portName << '\n';
    }
    cout << endl << "����� ��ġ�� ��ȣ�� �Է����ּ���. \n���� ��ġ�� ǥ�õ��� �ʴ´ٸ�, ������ Ȯ�� �� ENTERŰ�� �����ּ���. : ";
    string input;
    getline(cin, input);

    if (input == "") {//����
        system("cls");
        cout << endl;
        delete midiin;
        goto scandevice;
    }
    //int -> string
    stringstream ssint(input);
    ssint >> devicenum;

    if (ssint.fail()) {
        system("cls");
        cout << "��ȣ�� �Է����ּ���." << endl;
        delete midiin;
        goto scandevice;
    }

    if (devicenum > nPorts || devicenum < 1) {
        system("cls");
        cout << "��ġ ������ �̻��� �ֽ��ϴ�. �ٽ� �����մϴ�." << endl;
        delete midiin;
        goto scandevice;
    }

    string lastname = getmidiname(devicenum, midiin);

    //��ġ ��Ȯ��
    delete midiin;
    midiin = new RtMidiIn();
    int newPorts = midiin->getPortCount();
    if (nPorts != newPorts) {
        system("cls");
        cout << "��Ʈ ������ ������ �ֽ��ϴ�. �ٽ� �����մϴ�." << endl;
        delete midiin;
        goto scandevice;
    }
    if (lastname != getmidiname(devicenum, midiin)) {
        system("cls");
        cout << "��Ʈ ������ ������ �ֽ��ϴ�. �ٽ� �����մϴ�." << endl;
        delete midiin;
        goto scandevice;
    }

    system("cls");
    keyinput.type = INPUT_KEYBOARD;
    keyinput.ki.time = 0;
    keyinput.ki.dwExtraInfo = 0;

    ifstream in("key.lchkey");

    if (!in.is_open()) {
        //���� ���� ����
        MessageBox(NULL, TEXT("Ű ���� ������ �� �� �����ϴ�."), TEXT("����"), MB_ICONERROR | MB_OK);
        ofstream out("key.lchkey");
        out.close();
    }
    else {
        int count = 0;
        while (in.peek() != EOF) {
            if (count == 78) break;
            count++;
            if (count % 10 > 8) {
                count = count + 2;
            }
            string s;
            getline(in, s);
            //string -> int
            stringstream ssint(s);
            int toint;
            ssint >> toint;
            if (!ssint.fail()) {
                launchpadToXY(count);
                keys[lchpadxy.X][lchpadxy.Y] = toint;
            }
        }
    }
    in.close();

    try {
        designer(devicenum);
    }
    catch (int) {
        cout << "�̵� ��ġ�� �� �� �����ϴ�.\n�� ������ ��� ��Ÿ����, ���α׷��� ������� �ּ���." << endl;
        MessageBox(NULL, TEXT("�̵� ��ġ�� �� �� �����ϴ�.\n�� ������ ��� ��Ÿ����, ���α׷��� ������� �ּ���"), TEXT("����"), MB_ICONERROR | MB_OK);
        system("pause");
        system("cls");
        cout << endl;
        delete midiin;
        goto scandevice;
    }

    while (true) {
        gotoxy(1, 39);
        cout << "                                              ";
        gotoxy(1, 39);
        getline(cin, command);
        //�Է¹��� ��ɾ� ó��
        if (command == "quit") {
            break;
        }
        if (command == "edit") {
            edit(devicenum);
            system("cls");
            designer(devicenum);
        }
        if (command == "change") {
            system("cls");
            cout << endl;
            delete midiin;
            goto scandevice;
        }
    }

    delete midiin;
    return 0;
}