# RDM6300_ESP8266
A simple library to interface RDM6300 card reader with ESP8266. Generally RDM6300 reads card continuously, which is not convenient for practical implementation. In this library the card is read only once when held near the sensor. The same card can be read again by removing the card from the sensor and holding it again.  
