
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <sstream>
#include "RtMidi.h"
#include <conio.h>

using namespace std;

string getmidiname(int port, RtMidiOut* mi) {
    try {
        string name = mi->getPortName(port);
        return name.substr(0, name.size() - 2);
    }
    catch (RtMidiError& error) {
        throw error;
    }
}

int main()
{
    cout << endl;
scandevice:
    RtMidiOut* midiout;
    midiout = new RtMidiOut();
    int nPorts = midiout->getPortCount();
    if (nPorts == 0) {
        cout << "연결을 확인한 후 엔터키를 눌러주세요." << endl;
        delete midiout;
        cin.get();
        system("cls");
        goto scandevice;
    }
    cout << "총 " << nPorts << "개의 장치가 검색되었습니다." << endl;

    int devicenum;
    string portName;
    cout << endl;
    for (int i = 0; i < nPorts; i++) {
        try {
            portName = getmidiname(i, midiout);
        }
        catch (RtMidiError& error) {
            MessageBox(NULL, TEXT("알 수 없는 오류가 발생했습니다."), TEXT("오류"), MB_OK);
            error.printMessage();
            delete midiout;
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
        delete midiout;
        goto scandevice;
    }
    //int -> string
    stringstream ssint(input);
    ssint >> devicenum;

    if (ssint.fail()) {
        system("cls");
        cout << "번호를 입력해주세요." << endl;
        delete midiout;
        goto scandevice;
    }

    if (devicenum > nPorts || devicenum < 1) {
        system("cls");
        cout << "장치 설정에 이상이 있습니다. 다시 설정합니다." << endl;
        delete midiout;
        goto scandevice;
    }

    system("cls");

    try {
        midiout->openPort(devicenum - 1);
    }
    catch (int)
    {
        cout << "미디 장치를 열 수 없습니다.\n이 오류가 계속 나타나면, 프로그램을 재시작해 주세요." << endl;
        MessageBox(NULL, TEXT("미디 장치를 열 수 없습니다.\n이 오류가 계속 나타나면, 프로그램을 재시작해 주세요"), TEXT("오류"), MB_ICONERROR | MB_OK);
        system("pause");
        system("cls");
        cout << endl;
        delete midiout;
        goto scandevice;
    }

    cout << "시작" << endl;
    vector<unsigned char> message;
    // Send out a series of MIDI messages.
    // Program change: 192, 5
    message.push_back(192);
    message.push_back(5);
    midiout->sendMessage(&message);
    // Control Change: 176, 7, 100 (volume)
    message[0] = 176;
    message[1] = 7;
    message.push_back(100);
    midiout->sendMessage(&message);
    // Note On: 144, 64, 90
    while (true) {
        int f, s;
        cin >> f >> s;
        message[0] = 144;
        message[1] = f + 10;
        message[2] = s;
        midiout->sendMessage(&message);
    }
    // Clean up
    delete midiout;
    return 0;
}