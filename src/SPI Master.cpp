#include <SPI.h>

//Define ALTERNATE_PINS to use non-standard GPIO pins for SPI bus

#ifdef ALTERNATE_PINS
    #define HSPI_MISO 26
    #define HSPI_MOSI 27
    #define HSPI_SCLK 25
    #define HSPI_SS 32
#else
    #define HSPI_MISO MISO
    #define HSPI_MOSI MOSI
    #define HSPI_SCLK SCK
    #define HSPI_SS SS
#endif

static const int spiClk = 1000000; //1 MHz

//uninitialized pointers to SPI objects
SPIClass * hspi = NULL;

void setup() {
    //initialise instance of the SPIClass attached to HSPI
    hspi = new SPIClass(HSPI);

    //clock miso mosi ss
    #ifndef ALTERNATE_PINS
        //initialise hspi with default pins
        //SCLK =14, MISO = 12, MOSI = 13, SS = 15
        hspi->begin();
    #else
        hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS); //SCLK, MISO, MOSI, SS
    #endif

    //set up slave select pins as outputs as the Arduino API
    //doesn't handle automatically pulling SS low
    pinMode(HSPI_SS, OUTPUT); //HSPI SS
}

void loop() {
    hspi_send_command();
    delay(100);
}

void hspi_send_command() {
    byte data_on = 0b00000001; //data 1 to turn on LED of slave
    byte data_off = 0b00000000; // data 0 to turn off LED of slave

    hspi->beginTransaction(SPISettings(spiClk, SPI_MSBFIRST, SPI_MODE0));
    digitalWrite(HSPI_SS, LOW);
    hspi->transfer(data_on);
    digitalWrite(HSPI_SS, HIGH);
    
    delay(1000);

    hspi->beginTransaction(SPISettings(spiClk, SPI_MSBFIRST, SPI_MODE0));
    digitalWrite(HSPI_SS, LOW);
    hspi->transfer(data_on);
    digitalWrite(HSPI_SS, HIGH);

    delay(1000);

}