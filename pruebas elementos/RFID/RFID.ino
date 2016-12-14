

#define BAUD_RATE 115200

// comunication host -> sl025
#define HOST 0xBA // preamble host -> sl025
// comunication sl025 -> host
#define SL025 0xBD // preamble sl025 -> host

// --------------- COMMAND OVERVIEW (TABLE 3) ----------------
#define SELECT_MIFARE_CARD          0x01
#define LOGIN_TO_A_SECTOR           0x02
#define READ_A_DATA_BLOCK           0x03
#define WRITE_A_DATA_BLOCK          0x04
#define READ_A_VALUE_BLOCK          0x05
#define INITIALIZE_A_VALUE_BLOCK    0x06
#define WRITE_MASTER_KEY            0x07
#define INCREMENT_VALUE             0x08
#define DECREMENT_VALUE             0x09
#define COPY_VALUE                  0x0A
#define READ_A_DATA_PAGE            0x10  // DATA PAGE(ULTRALIGTH & NTAG203)
#define WRITE_A_DATA_PAGE           0x11  // DATA PAGE(ULTRALIGTH & NTAG203)
#define DOWNLOAD_KEY                0x12
#define LOGIN_SECTOR_VIA_STORED_KEY 0x13
#define MANAGE_RED_LED              0x40
#define GET_FIRMWARE_VERSION        0xF0
#define RESET                       0xFF

// --------------- STATUS OVERVIEW (TABLE 4) ----------------

// Read status
#define OPERATION_SUCCEED       0x00
#define NO_TAG                  0x01
#define LOGIN_SUCCEED           0x02  // Login results
#define LOGIN_FAIL              0x03  // Login results
#define READ_FAIL               0x04
#define WRITE_FAIL              0x05
#define UNABLE_READ_AFTER_WRITE 0x06
#define READ_AFTER_WRITE_FAIL   0x07
#define ADDRESS_OVERFLOW        0x08
#define DOWNLOAD_KEY_FAIL       0x09
#define NOT_AUTHENTICATE        0x0D
#define NOT_A_VALUE_BLOCK       0x0E
#define CHECKSUM_ERROR          0xF0
#define COMMMAND_CODE_ERROR     0xF1
#define LOAD_KEY_FAIL           0x0C  // Login results

// Login key types
#define USE_KEY_A 0xAA
#define USE_KEY_B 0xBB

// Tag types
#define MIFARE_1K_4B           0x01
#define MIFARE_1K_7B           0x02
#define MIFARE_ULTRALIGHT_7B   0x03
#define MIFARE_4K_4B           0x04
#define MIFARE_4K_7B           0x05
#define MIFARE_DESFIRE_7B      0x06
#define OTHER                  0x0A

//SL025M mifare;

// Key A to sector 2
byte keyA[6] = {0xD3, 0xA3, 0x4F, 0x11, 0xB8, 0xAA};  
// Key B to sector 2
byte keyB[6] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5};

void setup() {
  Serial.begin(BAUD_RATE);
}

void loop() {
  sendCommand(SELECT_MIFARE_CARD); 
  delay(2000);
}
