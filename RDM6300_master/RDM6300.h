#ifndef RDM6300_h
#define RDM6300_h

#include <SoftwareSerial.h>
#include "Arduino.h"

//#define DEBUG true

class RDM6300
{
private:

    SoftwareSerial * _rfidIO;
    String _tagString;
    long _oldTag;
    boolean _readTag;
    boolean _readSuccess;
    long _initialTime;
    long _finalTime;
    int _byteRead;
    char _tagNumber[12];
    char _realTagNum[7];
    long _tagId;
    boolean _idAvailable;
    String _realTagString;

public:
    long tagId;
    byte uidByte[3];
    RDM6300(int rxPin, int txPin);
    boolean isIdAvailable();
    boolean isIdUartAvailable();
    long readId();
    String realTagString;
};
#endif // RDM6300_h
