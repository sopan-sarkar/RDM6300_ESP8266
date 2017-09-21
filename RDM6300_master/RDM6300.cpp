//#include <SoftwareSerial.h>
#include "Arduino.h"
#include "RDM6300.h"


RDM6300::RDM6300(int rxPin, int txPin)
{
    _rfidIO = new SoftwareSerial(rxPin, txPin);
    _rfidIO->begin(9600);

    /*********************************************************************************
     Reset the variables
    *********************************************************************************/
    _byteRead = 0;
    _readTag = false;
    _readSuccess = false;
    _oldTag = 0;
    _idAvailable = false;
    _realTagString = "";
    realTagString = "";
}


boolean RDM6300::isIdAvailable()
{
    return isIdUartAvailable();
}


/*********************************************************************************
   EM cards has a tag length of 14 bytes. The first byte 2 indicates the start
   and the last byte 3 indicates the end of the tag. Out of the rest 12 bytes,first
   4 bytes is manufacturer ID,next 6 byte is tag number and the rest 2 byte is
   checksum.For example, a tag is 2 52 48 48 48 56 54 66 49 52 70 51 56 3

   |Start byte|     MFG ID    |     Tag number    | Checksum |End byte|
   |    2     |  52 48 48 48  | 56 54 66 49 52 70 |  51 56   |    3   |

   RDM6300 has no way of detecting if a tag was read just previously.
 *********************************************************************************/
boolean RDM6300::isIdUartAvailable()
{
    if (_rfidIO->available() > 0 && (_byteRead =_rfidIO->read()) == 2)
    {
        delay(1);
#ifdef  DEBUG
        Serial.println(F("Card Found"));
        Serial.println(F("\nReading Tag"));
#endif

        /*******************************************************************************
           When a new tag is detected,it is saved in the array _tagNumber[]
         *******************************************************************************/
        for (int i = 0; i <= 12; i++)
        {
            _byteRead =_rfidIO->read();
            if(i == 12 && _byteRead == 3)
            {
                _readSuccess = true;
                break;
            }
            else
            {
                _readSuccess = false;
            }
            _tagNumber[i] = _byteRead;
#ifdef  DEBUG
            Serial.print(_tagNumber[i]);
           Serial.print(" ");
#endif
            yield();
            delay(1);
        }

#ifdef  DEBUG
        Serial.println(F("\nReading Finished"));
#endif
        _initialTime = millis();
        /*******************************************************************************
           This is used to clear the Serial input buffer.
         *******************************************************************************/
        while (_rfidIO->available() != 0)
        {
            char d = _rfidIO->read();
        }
        /*******************************************************************************
           Sometimes it is seen that the Serial buffer is not cleared properly. So, wrong
           values may be read. We check if the first and last bytes are 2 and 3 respectively.
           If it doesn't match we discard the value and take another read. We convert the
           _tagNumber to string and place it in string variable tagString. realTagNum
           contains the actual 6 bytes of tagId that we need for verification.
         *******************************************************************************/
        if ( _readSuccess == true)
        {
            char buffer[2];
            _tagString = _tagNumber;
#ifdef  DEBUG
            Serial.print(F("tagString : "));
            Serial.println(_tagString);
#endif
            _realTagString = _tagString.substring(4, 10);
            _tagId = strtol(_tagString.substring(4, 10).c_str(), NULL, 16);

#ifdef  DEBUG
            Serial.print(F("tagId : "));
            Serial.println(_tagId);
            Serial.print(F("Tagstring  : "));
            Serial.println(_realTagString);
#endif

#ifdef  DEBUG
            Serial.println(F("inside available code"));
#endif

            /*******************************************************************************
              If a new Id is detected we check the registry for any match. If the Id matches
              we save the log to EEPROM.
            *******************************************************************************/
            if (_tagId != _oldTag)
            {
                _oldTag = _tagId;
                tagId   = _tagId;
                realTagString = _realTagString;
#ifdef  DEBUG
                Serial.print(F("oldTag : "));
                Serial.println(_oldTag, HEX);
#endif
                _readTag = true;
                _idAvailable = true;

#ifdef  DEBUG
                Serial.print(F("tagID : "));
                Serial.println(tagId, HEX);
#endif
            }
            else
            {
                _idAvailable = false;
            }
        }
    }
    else if (_rfidIO->available() == 0 &&  _readTag == true)
    {
#ifdef  DEBUG
        Serial.println(F("inside reset condition"));
#endif
        _finalTime = millis();
        if ((_finalTime - _initialTime) > 100)
        {
            _oldTag = 0;
            _readTag = false;

#ifdef  DEBUG
            Serial.println(F("reset done!!!"));
#endif
        }
    }

    return _idAvailable;
}

long RDM6300::readId()
{
    _idAvailable = false;

    for(int shiftCount=0; shiftCount<3; shiftCount++)
    {
        uidByte[shiftCount] =  tagId >> 8*(2-shiftCount);
#ifdef  DEBUG
        Serial.print(F("Values in byte "));
        Serial.print(shiftCount);
        Serial.print(F("  : "));
        Serial.println(uidByte[shiftCount],HEX);
#endif
    }

    return tagId;
}
