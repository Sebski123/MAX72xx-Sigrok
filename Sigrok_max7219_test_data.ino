/* The array for shifting the data to the devices */
byte spidata[16];
/* Data is shifted out of this pin*/
int SPI_MOSI = 8;
/* The clock is signaled on this pin */
int SPI_CLK = 16;
/* This one is driven LOW for chip selectzion */
int SPI_CS = 7;
/* The maximum number of devices we use */
int maxDevices = 4;

/*
1. pinModes
2. CS high
3. display test on
4. decode mode
5. setIntensity
6. scan limit
7. display test off
8. clear display
9. shutdown off
10. short write
11. long write
12. unknown register
13. 4x led on
14. 4x led off
*/

void setup() {
  pinMode(SPI_MOSI,OUTPUT);
  pinMode(SPI_CLK,OUTPUT);
  pinMode(SPI_CS,OUTPUT);

  digitalWrite(SPI_CS,HIGH);

  //display test on
  spiTransfer(15,1);

  //decode mode
  spiTransfer(9,0);

  //setIntensiy
  spiTransfer(10,7);

  //scan limit
  spiTransfer(11,7);

  //display test off
  spiTransfer(15,0);

  //clear display
  clearDisplay();

  //shutdown off
  spiTransfer(12,1);

  delay(100);

  //short write
  maxDevices = 3;
  spiTransfer(0,0);

  delay(100);

  //long write
  maxDevices = 5;
  spiTransfer(0,0);

  maxDevices = 4;

  delay(100);

  //unknown register
  spiTransfer(13, 14, 13, 14, 6, 9, 6, 9);

  delay(500);

  //4x led on
  spiTransfer(1, 2, 3, 4, 1, 2, 4, 8);

  delay(500);

  //4x led off
  spiTransfer(1, 2, 3, 4, 0, 0, 0, 0);

}

void loop() { 
}

void spiTransfer(volatile byte opcode1, volatile byte opcode2, volatile byte opcode3, volatile byte opcode4, volatile byte data1, volatile byte data2, volatile byte data3, volatile byte data4) {
    //Create an array with the data to shift out
    int maxbytes=maxDevices*2;

    for(int i=0;i<maxbytes;i++)
        spidata[i]=(byte)0;
    //put our device data into the array
    spidata[1]=opcode1;
    spidata[0]=data1;    
    spidata[3]=opcode2;
    spidata[2]=data2;    
    spidata[5]=opcode3;
    spidata[4]=data3;    
    spidata[7]=opcode4;
    spidata[6]=data4;
    //enable the line 
    digitalWrite(SPI_CS,LOW);
    //Now shift out the data 
    for(int i=maxbytes;i>0;i--)
        shiftOut(SPI_MOSI,SPI_CLK,MSBFIRST,spidata[i-1]);
    //latch the data onto the display
    digitalWrite(SPI_CS,HIGH);
}

void spiTransfer(volatile byte opcode, volatile byte data) {
    //Create an array with the data to shift out
    int maxbytes=maxDevices*2;

    for(int i=0;i<maxbytes;i++)
        spidata[i]=(byte)0;
    //put our device data into the array
    spidata[1]=opcode;
    spidata[0]=data;    
    spidata[3]=opcode;
    spidata[2]=data;    
    spidata[5]=opcode;
    spidata[4]=data;    
    spidata[7]=opcode;
    spidata[6]=data;
    //enable the line 
    digitalWrite(SPI_CS,LOW);
    //Now shift out the data 
    for(int i=maxbytes;i>0;i--)
        shiftOut(SPI_MOSI,SPI_CLK,MSBFIRST,spidata[i-1]);
    //latch the data onto the display
    digitalWrite(SPI_CS,HIGH);
}

void clearDisplay() {
    for(int i=0;i<8;i++) {
        spiTransfer((byte)i+1,(byte)0);
    }
}