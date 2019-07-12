// Code for Arduino Reading of Motorcycle NFC

//**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

// Use this line for a breakout with a SPI connection:
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);


#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
// also change #define in Adafruit_PN532.cpp library file
   #define Serial SerialUSB
#endif

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  delay(10);
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A card");
}

void loop(void) {
  boolean success;
  int ID_Match = 0;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };	// Buffer to store the returned UID
  uint8_t uid_auth[] = {4,146,218,18,255,56,128,0};  // UID of Erik's hand tag
  uint8_t uid_auth2[] = {149,190,7,27,0,0,0};  // UID of Erik's hand tag
  uint8_t uidLength;				// Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  if (success) {
    //Card Found
    Serial.println("\n\nKey Recognized.\n");
   //Card UID
    Serial.print("UID Value: \t\t");
    for (uint8_t i=0; i < uidLength; i++) {
      Serial.print(uid[i]); Serial.print("\t");
    }
    //Relate to Authorized UID (Erik's Chip)
    Serial.print("\nAuthorized Value:\t");
    for (uint8_t i=0; i < 7; i++) {
      Serial.print(uid_auth[i]); Serial.print("\t");
    }
    //Check for Match
    Serial.print("\nKey Status:\t");
    for (uint8_t i=0; i < 7; i++){
      ////delay(50);
      Serial.print("\t"); 
      if(uid_auth[i]==uid[i]){
        ID_Match +=1;
        Serial.print("Match");
      }else{
        Serial.print("X");
      }
      ////delay(50);
    }
    for (uint8_t i=0; i < 7; i++){
      ////delay(50);
      Serial.print("\t"); 
      if(uid_auth2[i]==uid[i]){
        ID_Match +=1;
        Serial.print("Match");
      }else{
        Serial.print("X");
      }
      ////delay(50);
    }
    if (ID_Match == 7){
      ////delay(200);
      Serial.print("\n\nKey is Authorized.\t");
      ////delay(500);
      Serial.print("Welcome, Erik.\t");
      ////delay(500);
      Serial.println("   WarBird is Unlocked.\n");
    }else{
      ID_Match = 0;
      Serial.println("\n\nKey is not Authorized.\n");
    }
	// Wait 10 milliseconds before continuing
	delay(200);
  }
  else
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out waiting for a card");
  }
}
