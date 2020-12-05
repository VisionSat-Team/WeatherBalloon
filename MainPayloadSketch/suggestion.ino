#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <SD.h>
#include "memorysaver.h"

#define CAMERA_DATA_FILE "cameraData.txt"

unsigned long captureDelay;
boolean wireHasBeenCut;
boolean currentDataSaved;
boolean commandRecieved = false;

// Variables for the camera module START

// set pin 7 as the slave select for the digital pot:
const int CS = 7;
bool is_header = false;
ArduCAM myCAM(OV5642, CS);
uint8_t read_fifo_burst(ArduCAM myCAM);
// Variables for the camera module END

//  SD Card Variables START
int SD_initCount;           // Used for Initializing SD Card
int SD_initCountLimit = 5;  // # of times to try initializing SD Card
boolean SD_init = true;     // True unless card Fails to initialize
const int chipSelect_SD = 10;
//  SD Card Variables END

unsigned long prev, now;

void setup() {
    captureDelay = 60000000L;  // can always change this variable
    prev = micros();
    currentDataSaved = true;

    // Camera Stuff START
    uint8_t temp;   //temporary var

    Serial.begin(115200);

    Serial.println(F("ACK CMD ArduCAM Start! END"));

    pinMode(CS, OUTPUT);    // set the CS as an output:
    digitalWrite(CS, HIGH);

    SPI.begin();    // initialize SPI:

    while(1) {  //  Check if the ArduCAM SPI bus is OK
        myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
        temp = myCAM.read_reg(ARDUCHIP_TEST1);

        if (temp != 0x55){
            Serial.println(F("ACK CMD SPI interface Error! END"));
            delay(1000); continue;
        } else {
            Serial.println(F("ACK CMD SPI interface OK. END"));
            break;
        }
    }

    //Change to JPEG capture mode and initialize the OV5642 module
    myCAM.set_format(JPEG);
    myCAM.InitCAM();

    myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);    // VSYNC is active HIGH
    myCAM.OV5642_set_JPEG_size(OV5642_2592x1944);       //  resolution
    // Camera stuff END

    //  Initialize SD Card START
    SD_initCount = 0; // Number of times that Card attempted to Initialize
    // If SD fails to initialize, keep trying SD_initCountLimit # of times
    while (!SD.begin(chipSelect_SD) && SD_initCount < SD_initCountLimit) {
        SD_initCount++; // Wait for SD Card to Initialize
    }
    // If card failed to initialize in while loop AND fails to initialize again
    if (!SD.begin(chipSelect_SD) && SD_initCount == SD_initCountLimit) {
        SD_init = false; // Card failed to initialize
    }
    //  Initialize SD Card END
}

void loop() {
    if (commandRecieved) {
        doCommand();
        return;
    }
    // stops capturing and saving data after wire has been cut
    if (!wireHasBeenCut) {
        now = micros();

        if (now - prev >= captureDelay) {
            prev = now;
            captureData();
            takeAndSavePicture();
            currentDataSaved = false;
        } else if (!currentDataSaved){
            saveData();
            currentDataSaved = true;
        }
    } else {
        //  whatever is to be done after the wire is cut
    }
}

void takeAndSavePicture() {
    if (SD_init) {
        uint8_t temp = 0xff, temp_last = 0;
        bool is_header = false;

        myCAM.flush_fifo();
        myCAM.clear_fifo_flag();

        //Start capture
        myCAM.start_capture();

        // Waiting for capture to be done (polling)
        Serial.print("waiting");
        while (!myCAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK)) {
            delay(10);
            Serial.print('.');
        }

        Serial.println(F("ACK CMD CAM Capture Done. END")); delay(50);

        read_fifo_burst(myCAM);

        //Clear the capture done flag
        myCAM.clear_fifo_flag();
    }
}

uint8_t read_fifo_burst(ArduCAM myCAM) {
    String pictureFileName = getTime() + CAMERA_DATA_FILE;
    uint8_t temp = 0, temp_last = 0;

    uint32_t length = 0;
    uint32_t realLength = 0;
    length = myCAM.read_fifo_length();
    Serial.print("Fifo Length: "); Serial.println(length, DEC);

    // 512KB for the OV5642
    if (length >= MAX_FIFO_SIZE) {
        Serial.println(F("ACK CMD Over size. END"));
        return 0;
    }
    if (length == 0 ) {
        Serial.println(F("ACK CMD Size is 0. END"));
        return 0;
    }

    myCAM.CS_LOW();
    myCAM.set_fifo_burst();//Set fifo burst mode
    temp = SPI.transfer(0x00);
    length--;

    File CurrentPictureFile = SD.open(pictureFileName, FILE_WRITE);
    while ( length-- ) {
        temp_last = temp;
        temp =  SPI.transfer(0x00);
        if (is_header == true){
            CurrentPictureFile.print(','); CurrentPictureFile.print(temp, DEC);
            realLength++;
        }
        else if ((temp == 0xD8) && (temp_last == 0xFF)) {
            is_header = true;
            Serial.println(F("ACK IMG END"));
            CurrentPictureFile.print(temp_last, DEC); CurrentPictureFile.print(',');
            CurrentPictureFile.print(temp, DEC);
            realLength = realLength + 2;
        }
        if ((temp == 0xD9) && (temp_last == 0xFF)) {
            break;
        }
        delayMicroseconds(15);
    }

    myCAM.CS_HIGH();
    is_header = false;
    return 1;
}

String getTime() {
    return "pretend this already works";
}

void doCommand(){}
void captureData(){}
void saveData(){}
