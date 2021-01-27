//윈도우 스타일 변경용입니다.
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//헤더파일
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


/*런치패드 키
* 71 72 73 74 75 76 77 78
* 61 62 63 64 65 66 67 68
* 51 52 53 54 55 56 57 58
* 41 42 43 44 45 46 47 48
* 31 32 33 34 35 36 37 38
* 21 22 23 24 25 26 27 28
* 11 12 13 14 15 16 17 18
* 01 02 03 04 05 06 07 08
*/

//네임스페이스
using namespace std;

//전역변수
INPUT keyinput; 
bool up = false; //키보드별 이름 출력 중 윗줄 아랫줄 구분용
WORD keys[8][8] = { 0, };
string command;
bool succedchange;
int changing_key;
RtMidiIn* midiin = 0;
HHOOK hHook;

//함수 부분
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
//일반 콜백
void mycallback(double deltatime, vector< unsigned char >* message, void* userData)
{
    if (message->size() > 2) {
        if ((int)message->at(0) == 144) {
            int lkey = (int)message->at(1) - 10;
            if (lkey < 0) return;
            if ((int)message->at(2) == 127) {
                //런치패드 키 눌림
                //(int)message->at(1) - 10 이 버튼 번호입니다.
                launchpadToXY(lkey);
                presskey(keys[lchpadxy.X][lchpadxy.Y], true);
            }
            else {
                //런치패드 키 때짐
                launchpadToXY(lkey);
                presskey(keys[lchpadxy.X][lchpadxy.Y], false);
            }
        }
    }
}
//키 변경용 콜백
void changkeycallback(double daltatime, vector<unsigned char>* message, void* userData) {
    if (message->size() > 2) {
        if ((int)message->at(0) == 144) {
            if ((int)message->at(2) == 127) {
                //런치패드 키 눌림
                //(int)message->at(1) - 10 이 버튼 번호입니다.
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
//키 입력용 콜백
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;
        //if (nCode >= 0 && (int)wParam == 256) {
        if (key_to_string(pKey->vkCode, false) == "err ") {
            cout << "지원하지 않은 키입니다." << endl;
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
    cout << "사용할 장치 이름 : " << getmidiname(port - 1, midiin) << endl << endl;
    //포트 열기
    try {
        midiin->openPort(port - 1);
    }
    catch (int)
    {
        throw;
    }
    //콜백함수 설정
    midiin->setCallback(&mycallback);
    //RtMidi 관련 설정
    midiin->ignoreTypes(false, false, false);
    //이부분에서 cin.get();이나 기타 명령어를 이용해서 입력받은후 꺼야합니다.
    //만약 종료해야할 부분이 생기면, cleanup();이나, delete midiin;을 사용하세요.
    for (int c1 = 0; c1 < 8; c1++) {
        cout << "┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐" << endl;
        for (int c2 = 0; c2 < 2; c2++) {
            for (int c3 = 0; c3 < 8; c3++) {
                cout << "│" << key_to_string(keys[c3][c1], c2) << "│ ";
            }
            cout << endl;
        }
        cout << "└────┘ └────┘ └────┘ └────┘ └────┘ └────┘ └────┘ └────┘" << endl;
    }
    cout << endl << "명령어" << endl << "quit : 종료합니다." << endl << "change : 입력 장치를 변경합니다." << endl << "edit : 키를 변경합니다." << endl << ":";
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
keychange:    cout << "런치패드 버튼을 변경합니다. ENTER키를 누르면 계속되고," << endl << "ESC를 누르면 나가고, del키를 누르면 키 설정을 삭제합니다:";
    keymode = 0;
    SetHook3();
    GetMessage(&msg, NULL, NULL, NULL);
    if (keymode == 1){//탈출
        delete midiin;
        midiin = new RtMidiIn();
        return;
    }
    else if (keymode == 2) {//삭제
        system("cls");
        cout << "런치패드 버튼을 삭제합니다." << endl << "삭제할 런치패드 버튼을 눌러주세요";
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
            cout << "┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐" << endl;
            for (int c2 = 0; c2 < 2; c2++) {
                for (int c3 = 0; c3 < 8; c3++) {
                    cout << "│" << key_to_string(keys[c3][c1], c2) << "│ ";
                }
                cout << endl;
            }
            cout << "└────┘ └────┘ └────┘ └────┘ └────┘ └────┘ └────┘ └────┘" << endl;
        }
        keys[lchpadxy.X][lchpadxy.Y] = 0;
        changecolor(0, 7);
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 2);
        cout << "┌────┐ ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 3);
        cout << "│" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], false) << "│ ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 4);
        cout << "│" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], true) << "│ ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 5);
        cout << "└────┘ ";
        changecolor(7, 0);
        gotoxy(0, 34);
        goto end;
    }
    else {
        system("cls");
        cout << "런치패드 버튼을 변경합니다." << endl << "변경할 런치패드 버튼을 눌러주세요:";
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
            cout << "┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐ ┌────┐" << endl;
            for (int c2 = 0; c2 < 2; c2++) {
                for (int c3 = 0; c3 < 8; c3++) {
                    cout << "│" << key_to_string(keys[c3][c1], c2) << "│ ";
                }
                cout << endl;
            }
            cout << "└────┘ └────┘ └────┘ └────┘ └────┘ └────┘ └────┘ └────┘" << endl;
        }
        changecolor(0, 7);
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 2);
        cout << "┌────┐ ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 3);
        cout << "│" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], false) << "│ ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 4);
        cout << "│" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], true) << "│ ";
        gotoxy(lchpadxy.X * 7, lchpadxy.Y * 4 + 5);
        cout << "└────┘ ";
        changecolor(7, 0);
        gotoxy(0, 33);
        cout << endl << "밝은색으로 된 버튼이 선택되었습니다." << endl << "키보드에서 변경할 키를 눌러주세요:" << endl;
    hooking:
        SetHook();
        GetMessage(&msg, NULL, NULL, NULL);

        if (key_to_string(changekey, false) == "err ") {
            cout << "지원하지 않은 키입니다. 키를 다시 눌러주세요." << endl;
            goto hooking;
        }
        keys[lchpadxy.X][lchpadxy.Y] = changekey;
        cout << "다음과 같이 변경되었습니다." << endl;
        cout << "┌────┐ " << endl
            << "│" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], false) << "│ " << endl
            << "│" << key_to_string(keys[lchpadxy.X][lchpadxy.Y], true) << "│ " << endl
            << "└────┘ " << endl;
end:
        delete midiin;
        midiin = new RtMidiIn();
        ofstream out("key.lchkey");
        if (!out.is_open()) {
            //파일 열기 실패
            MessageBox(NULL, TEXT("키 설정 파일을 열 수 없습니다. 키 설정이 저장되지 않습니다."), TEXT("오류"), MB_ICONERROR | MB_OK);
        }
        else {
            for (int c = 7; c > -1; c--) {
                for (int c1 = 0; c1 < 8; c1++) {
                    out << keys[c1][c] << endl;
                }
            }
        }
        out.close();
        cout << "성공적으로 설정되었습니다." << endl;
        cout << "계속해서 다른 키도 변경하시겠습니까?" << endl << "(y키를 누르면 변경하고, 엔터키를 누르면 돌아갑니다.):";
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
    //창 수정(크기조종, 확대버튼 삭제, 스크롤 삭제)
    system("mode con cols=72 lines=41");
    SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
pushenter:    
    cout << endl;
scandevice:
    midiin = new RtMidiIn();
    int nPorts = midiin->getPortCount();
    if (nPorts == 0) {
        cout << "장치를 검색할 수 없습니다." << endl << endl << "연결을 확인한 후 엔터키를 눌러주세요.";
        delete midiin;
        cin.get();
        system("cls");
        goto pushenter;
    }
    cout << "총 " << nPorts << "개의 장치가 검색되었습니다." << endl;

    int devicenum;
    string portName;
    cout << endl;
    for (int i = 0; i < nPorts; i++) {
        try {
            portName = getmidiname(i, midiin);
        }
        catch (RtMidiError& error) {
            MessageBox(NULL, TEXT("알 수 없는 오류가 발생했습니다."), TEXT("오류"), MB_OK);
            error.printMessage();
            delete midiin;
            return -1;
        }
        std::cout << i + 1 << "번 장치 : " << portName << '\n';
    }
    cout << endl << "사용할 장치의 번호를 입력해주세요. \n만약 장치가 표시되지 않는다면, 연결을 확인 후 ENTER키를 눌러주세요. : ";
    string input;
    getline(cin, input);

    if (input == "") {//엔터
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
        cout << "번호를 입력해주세요." << endl;
        delete midiin;
        goto scandevice;
    }

    if (devicenum > nPorts || devicenum < 1) {
        system("cls");
        cout << "장치 설정에 이상이 있습니다. 다시 설정합니다." << endl;
        delete midiin;
        goto scandevice;
    }

    string lastname = getmidiname(devicenum, midiin);

    //장치 재확인
    delete midiin;
    midiin = new RtMidiIn();
    int newPorts = midiin->getPortCount();
    if (nPorts != newPorts) {
        system("cls");
        cout << "포트 설정에 문제가 있습니다. 다시 설정합니다." << endl;
        delete midiin;
        goto scandevice;
    }
    if (lastname != getmidiname(devicenum, midiin)) {
        system("cls");
        cout << "포트 설정에 문제가 있습니다. 다시 설정합니다." << endl;
        delete midiin;
        goto scandevice;
    }

    system("cls");
    keyinput.type = INPUT_KEYBOARD;
    keyinput.ki.time = 0;
    keyinput.ki.dwExtraInfo = 0;

    ifstream in("key.lchkey");

    if (!in.is_open()) {
        //파일 열기 실패
        MessageBox(NULL, TEXT("키 설정 파일을 열 수 없습니다."), TEXT("오류"), MB_ICONERROR | MB_OK);
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
        cout << "미디 장치를 열 수 없습니다.\n이 오류가 계속 나타나면, 프로그램을 재시작해 주세요." << endl;
        MessageBox(NULL, TEXT("미디 장치를 열 수 없습니다.\n이 오류가 계속 나타나면, 프로그램을 재시작해 주세요"), TEXT("오류"), MB_ICONERROR | MB_OK);
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
        //입력받은 명령어 처리
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