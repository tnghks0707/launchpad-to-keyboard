
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
        cout << "������ Ȯ���� �� ����Ű�� �����ּ���." << endl;
        delete midiout;
        cin.get();
        system("cls");
        goto scandevice;
    }
    cout << "�� " << nPorts << "���� ��ġ�� �˻��Ǿ����ϴ�." << endl;

    int devicenum;
    string portName;
    cout << endl;
    for (int i = 0; i < nPorts; i++) {
        try {
            portName = getmidiname(i, midiout);
        }
        catch (RtMidiError& error) {
            MessageBox(NULL, TEXT("�� �� ���� ������ �߻��߽��ϴ�."), TEXT("����"), MB_OK);
            error.printMessage();
            delete midiout;
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
        delete midiout;
        goto scandevice;
    }
    //int -> string
    stringstream ssint(input);
    ssint >> devicenum;

    if (ssint.fail()) {
        system("cls");
        cout << "��ȣ�� �Է����ּ���." << endl;
        delete midiout;
        goto scandevice;
    }

    if (devicenum > nPorts || devicenum < 1) {
        system("cls");
        cout << "��ġ ������ �̻��� �ֽ��ϴ�. �ٽ� �����մϴ�." << endl;
        delete midiout;
        goto scandevice;
    }

    system("cls");

    try {
        midiout->openPort(devicenum - 1);
    }
    catch (int)
    {
        cout << "�̵� ��ġ�� �� �� �����ϴ�.\n�� ������ ��� ��Ÿ����, ���α׷��� ������� �ּ���." << endl;
        MessageBox(NULL, TEXT("�̵� ��ġ�� �� �� �����ϴ�.\n�� ������ ��� ��Ÿ����, ���α׷��� ������� �ּ���"), TEXT("����"), MB_ICONERROR | MB_OK);
        system("pause");
        system("cls");
        cout << endl;
        delete midiout;
        goto scandevice;
    }

    cout << "����" << endl;
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