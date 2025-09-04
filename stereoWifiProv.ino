
#include "esp_timer.h"
#include <WiFi.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include <WebServer.h>
#include "soundPost.h"
#include "AsyncUDP.h"

char XML[1024];
// just some buffer holder for char operations
char buf[256];

AsyncUDP udp;

String newString = "old";
String oldString = "old";
String st99 = "99";
unsigned long start, startHoldr, tests, avjSum0, avjSum1, avjCnt;
uint16_t avj0 = 1023;
uint16_t avj1 = 1023;
uint16_t avj = 1023;
uint16_t loopCount, udpClock;
uint64_t espStart, espEnd, smplStrt, msgHoldr, onStart, onTime;
long convol, hfNoyz, mDif, total, wHoldr;
int sample = 144; //data set size
int fixdSmpl = 144; //data set size for reference
const int gutter = 36;  // >max # samples between sound travel between mics: 0.24m/(343m/s)*(41k samples/s)
uint8_t pack[1400]; //UDP data packet holds audio sample data, should be < 1500b TCP MTU
uint8_t sPac[2];  //UDP packet for sound location data only (direction+confidence)
size_t packSize = sizeof(pack);
size_t shortSize = 2;
int mic0[144];  //@ 41khz*144 samples is ~3.5mS of data and >4x microphone array distance (343m/s * .0035s = 1.2m)
int mic1[144];  //for tracking LF (engine,muffler) noise, need longer sample to capture large wavelength
int mic2[144];  //holds mic0 data for processing so mic0 can continue to sample
int mic3[144];  //holds mic1 data for processing so mic0 can continue to sample
int window[72];  //holds convolution volume history for all window points, degrading over time (*0.9)
bool toggle = false;  //runs led for wifi status indicator
int uCt, xCt, yCt, zCt, yxu, vSum, vAvg, vNow, vConf, prevX, dirxn;
uint8_t wifiRe, hx0b, hh0b, onCnt;
int hy0, hy1, lo0, lo1, mark, fxU;
int testHoldr, valids, wifiCnt, hxb, hhb, hxp, hhp;
int chng0, chng1, noyzLim, ceiling, prev0, prev1;
int confH = 0;  //sound direction estimate confidence value. Higher confidence is better
int count, winHi, winMark, winHoldr, thrsh, moment0, moment1, rawSmpl0, rawSmpl1;
int yTime=0;
bool trip = false;    //triggers 2 mics sampling for convolution 
int loopUs = 0;
int hx0, hx1, hx2, hx3, hx4, hx5, hx6, hh0, hh1, hh2, hh3, hh4, hh5, hh6, hxAv, hhAv;
uint16_t chngHold0, chngHold1, vCount, onHoldr;
uint16_t pacPt = 2; //audio UDP pack[] index
uint16_t tClock = 999;   //threshold  for tossing datasets for inordinate delays

#define USE_INTRANET

WebServer server(80);

//#define CONFIG_ESPTOOLPY_FLASHMODE "qout"

//ADC continuous mode documentation: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html

// Define how many conversion per pin will happen and reading the data will be and average of all conversions
#define CONVERSIONS_PER_PIN 1


// Declare array of ADC pins that will be used for ADC Continuous mode - ONLY ADC1 pins are supported
// Number of selected pins can be from 1 to ALL ADC1 pins.
#ifdef CONFIG_IDF_TARGET_ESP32


uint8_t adc_pins[] = { 36, 39, 34, 35 };  //some of ADC1 pins for ESP32
#else
uint8_t adc_pins[] = {4, 2};  //ADC1 common pins for ESP32S2/S3 + ESP32C3/C6 + ESP32H2
#endif

// Calculate how many pins are declared in the array - needed as input for the setup function of ADC Continuous
uint8_t adc_pins_count = sizeof(adc_pins) / sizeof(uint8_t);

// Flag which will be set in ISR when conversion is done
volatile bool adc_coversion_done = false;

// Result structure for ADC Continuous reading
adc_continuous_data_t *result = NULL;

// ISR Function that will be triggered when ADC conversion is done
void ARDUINO_ISR_ATTR adcComplete() {
  adc_coversion_done = true;
}

/*
webpage display+updates code modified from Kris Kasprzak example:

https://github.com/KrisKasprzak/ESP32_WebPage

  this example will show
  1. how to use and ESP 32 for reading pins
  2. building a web page for a client (web browser, smartphone, smartTV) to connect to
  3. sending data from the ESP to the client to update JUST changed data
  4. sending data from the web page (like a slider or button press) to the ESP to tell the ESP to do something

  If you are not familiar with HTML, CSS page styling, and javascript, be patient, these code platforms are
  not intuitive and syntax is very inconsitent between platforms

  I know of 4 ways to update a web page
  1. send the whole page--very slow updates, causes ugly page redraws and is what you see in most examples
  2. send XML data to the web page that will update just the changed data--fast updates but older method
  3. JSON strings which are similar to XML but newer method
  4. web sockets very very fast updates, but not sure all the library support is available for ESP's

  I use XML here...

  compile options
  1. esp32 dev module
  2. upload speed 921600
  3. cpu speed 240 mhz
  flash speed 80 mhz
  flash mode qio
  flash size 4mb
  partition scheme default

  NOTE if your ESP fails to program press the BOOT button during programm when the IDE is "looking for the ESP"

  The MIT License (MIT)

  code writen by Kris Kasprzak
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  On a personal note, if you develop an application or product using this code 
  and make millions of dollars, I'm happy for you!
*/

// code to send the main web page
// PAGE_MAIN is a large char defined in SuperMon.h
void SendWebsite() {

  Serial.println("sending web page");
  // you may have to play with this value, big pages need more porcessing time, and hence
  // a longer timeout that 200 ms
  server.send(200, "text/html", PAGE_MAIN);
}

// code to send the main web page
// I avoid string data types at all cost hence all the char mainipulation code
void SendXML() {

  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");

  sprintf(buf, "<H0>%d</H0>\n", testHoldr);
  strcat(XML, buf);

  sprintf(buf, "<VLD>%d</VLD>\n", valids);
  strcat(XML, buf);

  sprintf(buf, "<WH>%d</WH>\n", winHoldr);
  strcat(XML, buf);

  sprintf(buf, "<AVJ0>%d</AVJ0>\n", udpClock);
  strcat(XML, buf);

  sprintf(buf, "<AVJ1>%d</AVJ1>\n", onHoldr);    //onHoldr
  strcat(XML, buf);

  sprintf(buf, "<NL>%d</NL>\n", noyzLim);
  strcat(XML, buf);

  sprintf(buf, "<THR>%d</THR>\n", thrsh);
  strcat(XML, buf);

  //sprintf(buf, "<W2>%d</W2>\n", window[4]);
  //strcat(XML, buf);

  sprintf(buf, "<W3>%d</W3>\n", window[6]);
  strcat(XML, buf);

  sprintf(buf, "<W4>%d</W4>\n", window[8]);
  strcat(XML, buf);

  sprintf(buf, "<W5>%d</W5>\n", window[10]);
  strcat(XML, buf);

  sprintf(buf, "<W6>%d</W6>\n", window[12]);
  strcat(XML, buf);

  sprintf(buf, "<W7>%d</W7>\n", window[14]);
  strcat(XML, buf);

  sprintf(buf, "<W8>%d</W8>\n", window[16]);
  strcat(XML, buf);

  sprintf(buf, "<W9>%d</W9>\n", window[18]);
  strcat(XML, buf);

  sprintf(buf, "<W10>%d</W10>\n", window[20]);
  strcat(XML, buf);

  sprintf(buf, "<W11>%d</W11>\n", window[22]);
  strcat(XML, buf);

  sprintf(buf, "<W12>%d</W12>\n", window[24]);
  strcat(XML, buf);

  sprintf(buf, "<W13>%d</W13>\n", window[26]);
  strcat(XML, buf);

  sprintf(buf, "<W14>%d</W14>\n", window[28]);
  strcat(XML, buf);

  sprintf(buf, "<W15>%d</W15>\n", window[30]);
  strcat(XML, buf);

  sprintf(buf, "<W16>%d</W16>\n", window[32]);
  strcat(XML, buf);

  sprintf(buf, "<W17>%d</W17>\n", window[34]);
  strcat(XML, buf);

  sprintf(buf, "<W18>%d</W18>\n", window[36]);
  strcat(XML, buf);

  sprintf(buf, "<W19>%d</W19>\n", window[38]);
  strcat(XML, buf);

  sprintf(buf, "<W20>%d</W20>\n", window[40]);
  strcat(XML, buf);

  sprintf(buf, "<W21>%d</W21>\n", window[42]);
  strcat(XML, buf);

  sprintf(buf, "<W22>%d</W22>\n", window[44]);
  strcat(XML, buf);

  sprintf(buf, "<W23>%d</W23>\n", window[46]);
  strcat(XML, buf);

  sprintf(buf, "<W24>%d</W24>\n", window[48]);
  strcat(XML, buf);

  sprintf(buf, "<W25>%d</W25>\n", window[50]);
  strcat(XML, buf);

  sprintf(buf, "<W26>%d</W26>\n", window[52]);
  strcat(XML, buf);

  sprintf(buf, "<W27>%d</W27>\n", window[54]);
  strcat(XML, buf);

  sprintf(buf, "<W28>%d</W28>\n", window[56]);
  strcat(XML, buf);

  sprintf(buf, "<W29>%d</W29>\n", window[58]);
  strcat(XML, buf);

  sprintf(buf, "<W30>%d</W30>\n", window[60]);
  strcat(XML, buf);

  sprintf(buf, "<W31>%d</W31>\n", window[62]);
  strcat(XML, buf);

  sprintf(buf, "<W32>%d</W32>\n", window[64]);
  strcat(XML, buf);

  strcat(XML, "</Data>\n");
  // wanna see what the XML code looks like?
  // actually print it to the serial monitor and use some text editor to get the size
  // then pad and adjust char XML[2048]; above
  // Serial.println(XML);

  // you may have to play with this value, big pages need more porcessing time, and hence
  // a longer timeout that 200 ms
  server.send(200, "text/xml", XML);
}

IRAM_ATTR void udpRXCallBack(AsyncUDPPacket &packet) {
  char *tmpStr = (char *)malloc(packet.length() + 1);
  memcpy(tmpStr, packet.data(), packet.length());
  tmpStr[packet.length()] = '\0';  // ensure null termination
  newString = String(tmpStr);      // comes from stackoverflow example; don't change
  free(tmpStr);                    // Strign(char*) creates a copy so we can delete our one
  
  if(newString==st99){
    pack[0] = msgHoldr & 255;
    pack[1] = wHoldr & 255;
    pack[pacPt]=avj0/10;
    pack[pacPt+1]=avj1/10;
    pacPt+=2;
    packSize=pacPt;
    packet.write((uint8_t *)pack, packSize);
    pacPt=2;
    wifiRe=222;
  }else{
    sPac[0] = msgHoldr & 255;
    sPac[1] = wHoldr & 255;
    packet.write((uint8_t *)sPac, shortSize);
    udpClock=777;
  } 
}

void startUDP(){
  //from Arduino IDE example AsyncUDPMulticastServer:
    if (udp.listenMulticast(IPAddress(239, 1, 2, 3), 1234)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket(udpRXCallBack);
    /*
        udp.onPacket([](AsyncUDPPacket packet) {

      char *tmpStr = (char *)malloc(packet.length() + 1);
      memcpy(tmpStr, packet.data(), packet.length());
      tmpStr[packet.length()] = '\0';  // ensure null termination
      newString = String(tmpStr);      // comes from stackoverflow example; don't change
      free(tmpStr);                    // Strign(char*) creates a copy so we can delete our one
      
      if(newString==st99){
        pack[0] = msgHoldr & 255;
        pack[1] = wHoldr & 255;
        pack[pacPt]=avj0/10;
        pack[pacPt+1]=avj1/10;
        pacPt+=2;
        packSize=pacPt;
        packet.write((uint8_t *)pack, packSize);
        pacPt=2;
        wifiRe=222;
      }else{
        sPac[0] = msgHoldr & 255;
        sPac[1] = wHoldr & 255;
        packet.write((uint8_t *)sPac, shortSize);
        udpClock=0;
      } 
    });
    */
  }
}

void setup() {

  for (int q = 0; q < 72; q++) {
    window[q] = 0;
  }
  for (int r = 0; r < 1399; r++) {
    pack[r] = 0;
  }

  total = 0;    //holds largest product from mics convolution
  tests = 0;    //# of datasets, resets to zero after (tHoldr) mS of no loud noise
  yCt = 0;    //some counters
  xCt = 0;
  zCt = 0;
  yxu=0;
  avjSum0 = 0;    //holds ADC average of opamp1 (reference V for single-supply op amp)
  avjSum1 = 0;    //holds ADC avg of opamp2
  onCnt=111;      //counter
  avjCnt = 0;
  hhAv = avj1;
  hxAv = avj0;
  hh0 = 0;
  hx0 = 0;
  mDif = 0;    //holds sum of difference in convolved signals, determines confidence
  mark = 0;   //holds the estimated sound direction
  loopCount = 0;
  winHoldr = 0;
  prevX=0;
  vSum=0;
  dirxn=0;
  vAvg=0;
  thrsh = 55;   //holds the sound pressure threshold for triggering data sampling
  noyzLim = 99;   //holds high frequency noise limit for determining data quality (ESP32S2 seems noisy MCU)
  ceiling = 222;  //max value for noyzLim
  wifiRe = 22;    //counter for maintaining wifi connection, apparently can't do concurrent w analog continuous mode
  valids = 0;   //# valid datasets from total attempts
  testHoldr = 0;    //# of datasets sampled
  vCount=0;
  moment0 = 0;  
  moment1 = 0;
  chngHold0 = 0;    //holds sum of HF noise for mic0
  chngHold1 = 0;    //holds sum of HF noise for mic1
  //unfortunately to use third ADC channel for circuit noise reference reduces max sampling speed by half
  wifiCnt = 0;
  onHoldr = 0;      //holds minutes since ESP32 reset
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  pinMode(15, OUTPUT);  // set the LED pin mode
  digitalWrite(15, LOW);
  //analogSetPinAttenuation(4, ADC_11db);
  //analogSetPinAttenuation(8, ADC_11db);

  delay(99);

  WiFiManager wm;
  //first parameter is name of access point, second is the password
  bool res;
  res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    //res = wm.autoConnect("AutoConnectAP","password"); // password protected ap
 
    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

  // Optional for ESP32: Set the resolution to 9-12 bits (default is 12 bits)
  analogContinuousSetWidth(12);

  // Optional: Set different attenaution (default is ADC_11db)
  analogContinuousSetAtten(ADC_11db);

  // Setup ADC Continuous with following input:
  // array of pins, count of the pins, how many conversions per pin in one cycle will happen, sampling frequency, callback function
  analogContinuous(adc_pins, adc_pins_count, CONVERSIONS_PER_PIN, 82000, &adcComplete); //82khz/(2 channels) = 41khz

  // Start ADC Continuous conversions


  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(15, HIGH);
  onStart = millis();

  startUDP();

  // these calls will handle data coming back from your web page
  // this one is a page request, upon ESP getting / string the web page will be sent
  server.on("/", SendWebsite);

  // upon esp getting /XML string, ESP will build and send the XML, this is how we refresh
  // just parts of the web page

  server.on("/xml", SendXML);

  server.begin();   //runs local webpage convolution product visualizer

  analogContinuousStart();
}

void loop() {
  // Check if conversion is done and try to read data
  //espStart = esp_timer_get_time();
  hy0 = 0;
  lo0 = 9999;
  lo1 = 9999;
  hy1 = 0;
  trip = false;
  sample = 1;
  while (count < sample) {
    loopCount++;
    if (adc_coversion_done == true) {
      //loopUs = esp_timer_get_time() - espStart;
      //espStart = esp_timer_get_time();
      // Set ISR flag back to false
      adc_coversion_done = false;
      // Read data from ADC
      if (analogContinuousRead(&result, 0)) {
        if (trip) {
          if (count > 0) {
            mic0[count] = hxAv;
            mic1[count] = hhAv;

            espEnd = esp_timer_get_time() - smplStrt;

            if (espEnd > (99 + count)) {    //sometimes long delays between samples make poor dataset
              trip = false;                 //so throw it out & start over
              sample = 1;
              //Serial.print(" delayed sample reject@c ");
              //Serial.print(loopCount);
              //Serial.print(": ");
              //Serial.print(espEnd);
            } else if (abs(rawSmpl0 - result[0].avg_read_raw) > thrsh || abs(rawSmpl1 - result[1].avg_read_raw) > thrsh) {
              trip = false;   //too large value jump between samples = noisy: abort
              sample = 1;
              //tests--;
              thrsh++;
             // Serial.print(" sample noise reject@c ");
              //Serial.println(loopCount);
            }
          } else if (!count) {
            mic0[0] = hxAv;
            mic1[0] = hhAv;
            //espStart = esp_timer_get_time();
            start = millis();   //
          }
          smplStrt = esp_timer_get_time();

          if (mic0[count] > hy0) {
            hy0 = mic0[count];
          } else if (mic0[count] < lo0) {
            lo0 = mic0[count];
          }
          if (mic1[count] > hy1) {
            hy1 = mic1[count];
          } else if (mic1[count] < lo1) {
            lo1 = mic1[count];
          }
          if (count == gutter) {
            hfNoyz = chngHold1 + chngHold0;
            hfNoyz /= count;
            if (hfNoyz > noyzLim) {
              trip = false;
              sample = 1;
              //tests--;
              start = startHoldr;
              //Serial.print(" Avg crcuit noiz reject@c ");
              //Serial.println(loopCount);
              //if(noyzLim <111) noyzLim++;
            }  //else thrsh++;
          }
          count++;
        } else if (result[0].avg_read_raw > (avj0 + thrsh) || result[0].avg_read_raw < (avj0 - thrsh) || result[1].avg_read_raw > (avj1 + thrsh) || result[1].avg_read_raw < (avj1 - thrsh)) {
          if (abs(result[0].avg_read_raw - rawSmpl0) < thrsh && abs(result[1].avg_read_raw - rawSmpl1) < thrsh) {
            //espStart = esp_timer_get_time();
            trip = true;
            sample = 144;
            chngHold1 = 0;
            chngHold0 = 0;
            //loopCount=0;
            if (!tests) valids = 0;
            tests++;
            //count--;
            testHoldr = tests;
            count = -6;
            if (result[1].avg_read_raw > avj1) {  //added 5/16/25 to prevent intermittent false trips
              avj1++;
            } else if (avj1 > 888) avj1--;
            if (result[0].avg_read_raw > avj0) {
              avj0++;
            } else if (avj0 > 888) avj0--;
          }
        } else {
          count++;
          avjSum0 += result[0].avg_read_raw;
          avjSum1 += result[1].avg_read_raw;
          avjCnt++;
          if (avjCnt > 999) {   //determine op amp average/reference v (Vdd/2)
            avjSum0 /= avjCnt;
            avjSum1 /= avjCnt;
            avj0 += avjSum0;
            avj1 += avjSum1;
            avj0 /= 2;
            avj1 /= 2;
            avj = (avjSum0 + avjSum1) / 2;
            avjSum0 = 0;
            avjSum1 = 0;
            avjCnt = 0;
          }
          if ((millis() - start) > tClock) {   //if no trips in (tClock) mS, lower loudness threshold & decrease hf noise threshold
            if (noyzLim < ceiling) noyzLim++;   //increase hf noise limit to increase ratio of valid/invalid tests
            tests = 0;    //resets # tests after period of peaceful ambience
            start = millis();
            startHoldr = start;
            thrsh *= 0.9;   //decrease loudness threshold to increase testing frequency
          }
          if (zCt) {
            zCt--;
            window[zCt] *= 0.9;   //reducing old stale convolution hx data to zero
            wHoldr *= 0.99;
          } else zCt = 2 * gutter;
        }

        rawSmpl0 = result[0].avg_read_raw;
        rawSmpl1 = result[1].avg_read_raw;
        moment0 = 10 * abs(rawSmpl0 - hx0 - (hx0 - hx1)); //measuring volume of HF noise spikes
        moment1 = 10 * abs(rawSmpl1 - hh0 - (hh0 - hh1));
        chng0 = abs(rawSmpl0 - hx1);    //(current sample) - (sample before last) 
        chng1 = abs(rawSmpl1 - hh1);
        if (chng1) chngHold1 += (moment1 / chng1);
        else chngHold1 += moment1;
        if (chng0) chngHold0 += (moment0 / chng0);
        else chngHold0 += moment0;
        //prev0 = hxAv;
        //prev1 = hhAv;
        hh5 = hh4;
        hh4 = hh3;
        hh3 = hh2;
        hh2 = hh1;
        hh1 = hh0;
        hh0 = rawSmpl1;
        hhAv = hh5 + hh4 + hh3 + hh2 + hh1 + hh0;
        hhAv /= 6;

        hx5 = hx4;
        hx4 = hx3;
        hx3 = hx2;
        hx2 = hx1;
        hx1 = hx0;
        hx0 = rawSmpl0;
        hxAv = hx5 + hx4 + hx3 + hx2 + hx1 + hx0;
        hxAv /= 6;

        if(wifiRe > 33){
          if(!(count%3) && pacPt<1395){
            hh0b = hh0 / 10;
            hx0b = hx0 / 10;
            if(pacPt<96){   //96 captures past 48 samples from each mic
              hhb=hhAv-avj1;
              //hhb*=(2*pacPt);   //nonlinear smoothing shape, less good in practice
              //hhb/=(pacPt+96);
              hhb*=pacPt;     //from pack[0], increase volume from 0 to 100% gradually
              hhb/=96;      // because audio sounds discontinuous and staticky
              hhb+=avj1;  //due i think to discontinuity caused by UDP interrupts and wifi servicing
              hxb=hxAv-avj0;  
              //hxb*=(2*pacPt);
              //hxb/=(96+pacPt);
              hxb*=pacPt;   
              hxb/=96;    
              hxb+=avj0;    
              hh0b=hhb/10;  
              hx0b=hxb/10;
            }
            pack[pacPt]=hx0b;
            pack[pacPt+1]=hh0b;   //End of packet smoothing handled by rPi
            pacPt+=2;
          }
        }

      } else {
        Serial.println("Error occurred during reading data. Set Core Debug Level to error or lower for more information.");
      }
    }
    if (yCt) {    //convolve and analyze last dataset between sampling of next dataset
      if (yCt == 4) {
        if(uCt < 0 && xCt < fixdSmpl){    //convolve mic2[] signal with mic3[] signal
         // espStart = esp_timer_get_time();
            convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));    //sum of product of data points
            mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));     //sum of difference in data points
            xCt++;
            if(xCt < fixdSmpl){
              convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));
              mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));
              xCt++;
              if(xCt < fixdSmpl){
                convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));
                mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));
                xCt++;
                if(xCt < fixdSmpl){
                  convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));
                  mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));
                  xCt++;
                  if(xCt < fixdSmpl){
                    convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));
                    mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));
                    xCt++;
                    if(xCt < fixdSmpl){
                      convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));
                      mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));
                      xCt++;
                    }
                  }
                }
              }
            }
            /*
            for (int v = 0; v < 6; v++) {
            convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));
            mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));
            xCt++;    //but then xCt might exceed mic[] size, not sure why it let me do this
          }
            */
         // loopUs = esp_timer_get_time() - espStart;
         // yTime+=loopUs;
          //yxu++;
        } else if (uCt < 0) {
          convol /= (mDif + 1);
          window[uCt + gutter] += convol;
          if (convol > total) {
            total = convol;
            mark = uCt;
          }
          uCt++;
          xCt = -uCt;
          convol = 0;
          mDif = 0;
          //yxu++;
        } else {
          convol = 0;
          mDif = 0;
          yCt--;
          uCt = 0;
          xCt = 0;
          fxU = fixdSmpl;
          //yxu++;
          //Serial.print("yCt4@c ");
          //Serial.print(loopCount);
        }
      }else if (yCt == 3) {
        if(uCt < gutter && xCt < fxU){
          convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));      //sum of product of data points
          mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));     //sum of difference in data points
          xCt++;
          if(xCt<fxU){
            convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));
            mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));
            xCt++;
            if(xCt<fxU){
              convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));
              mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));
              xCt++;
              if(xCt<fxU){
                convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));
                mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));
                xCt++;
                if(xCt<fxU){
                  convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));
                  mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));
                  xCt++;
                  if(xCt<fxU){
                    convol += ((mic2[xCt] - avj0) * (mic3[xCt + uCt] - avj1));
                    mDif += (abs(mic2[xCt] - mic3[xCt + uCt]));
                    xCt++;
                  }
                }
              }
            }
          }
          //yxu++;
        } else if (uCt < gutter) {
          convol /= (mDif + 1);
          window[uCt + gutter] += convol;
          if (convol > total) {   //tracking position of highest convolution product/difference
            total = convol;   //holds value of highest convolution volume
            mark = uCt;     //holds position (direction) of highest convolution volume
          }
          uCt++;
          fxU = fixdSmpl-uCt;
          xCt = 0;
          convol = 0;
          mDif = 0;
          //yxu++;
        } else {
          convol = 0;
          confH = total - hfNoyz;   //subtracting hf noise (circuit noise) value beecause it reduces data quality
          if (confH < 1) confH = 1;
          if (confH < noyzLim && noyzLim > 22) noyzLim--;   //if receiving low confidence data, lower hf noise limit
          yCt--;
          uCt = 4;
          xCt = 2 * gutter;
          //yxu++;
        }
      } else if (yCt == 2) {
        if (uCt == 4 && xCt) {
          for (int y = 0; y < 6; y++) {
            xCt--;
            window[xCt] *= ((100 * gutter) / (gutter + abs(confH * (xCt - gutter - mark- dirxn)) / (4 * gutter)));
            window[xCt] /= 100;   //increase attrition of convolution volume hx by distance from latest apex
                                  //dirxn added to emphasize direction of sound movement
          }
          //yxu++;
        } else if (uCt == 4 && !xCt) {
          xCt = 2 * gutter;
          yCt--;
          //yxu++;
        }
      } else if (yCt == 1) {
        if (xCt) {
          for (int w = 0; w < 6; w++) {
            xCt--;
            if (window[xCt] > winHi) {   // extracting highest value point of convolution hx after attrition manipulation
              winHi = window[xCt];    
              winMark = xCt;
            }
            window[xCt] *= 0.99;    //attrit
          }
          //yxu++;
        } else {
          msgHoldr = winMark;
          winHoldr = winMark - gutter;
          winHi = 0;
          wHoldr = window[msgHoldr] / 22;  //volume scaled down by /22
          if(msgHoldr != prevX && wifiRe > 33){   //tracking sound source avg vector
            vNow = msgHoldr-prevX;    //latest vector = current direction - previous direction
            vSum+=vNow;   //add to sum of vectors
            if(vSum > 0) dirxn = 1;
            else if(vSum < 0) dirxn = -1;
            vCount++; 
            vAvg=1000*vSum/vCount;  //1000* to avoid decimals <1
            prevX=msgHoldr;
            if(vAvg){
              vConf=(vNow+vAvg)/vAvg;  //1 if vNow in same direction as vAvg, 0 if not
              wHoldr+=(vConf*wHoldr);   //increase confidence if vector aligns w avg
            } 
          }
          if (wHoldr > 255){
            wHoldr = 255;
          } else if (wHoldr < 0) wHoldr = 0;
          yCt = 0;
          //yxu++;
        }
      }
    }
  }
  hfNoyz = chngHold1 + chngHold0;   //total hf noise on both ADC channels
  hfNoyz /= sample; 
  if (trip) {  
    //if hf noise (circuit noise) is low and lf noise (vehicle noise) is high, then do analyze data:
    if(hfNoyz < noyzLim && (hy0 - lo0) > thrsh && (hy1 - lo1) > thrsh) {
      // espStart = esp_timer_get_time();
      //loopUs = esp_timer_get_time() - espStart;
      // Serial.print("loopUs= ");
      //Serial.println(loopUs);
      //if(yxu) yTime/=yxu;
      /*
      Serial.print(tests);
      Serial.print(" good! loopUs: ");
      Serial.print(loopUs);
      Serial.print("; yLoops= ");
      Serial.print(yxu);
      Serial.print("; yCt= ");
      Serial.print(yCt);
      Serial.print("; Loops: ");
      Serial.println(loopCount);
      */
      for (int i = 0; i < 144; i++) {  // moving data so mic0 and mic1 can collect next dataset
        mic2[i] = mic0[i];
        mic3[i] = mic1[i];
      }
      //loopCount = 0;
      valids++;
      start = millis();  //timing between loud noises
      total = 0;      //highest convolution 
      convol = 0;   //mics convolution product
      mDif = 0;     //mics signal difference sum, for determine confidence
      yCt = 4;    //counters
      uCt = -gutter;
      xCt = gutter;
      //loopUs = esp_timer_get_time() - espStart;
    } else {    //if dataset not valid d/t high hf noise or low lf volume
      if (noyzLim < ceiling) noyzLim++;   //because if don't increase it then datasets will just keep getting rejected
      else thrsh++;     //Higher volume threshold gives better SNR, higher quality datasets
    }
  }
  //loopUs = esp_timer_get_time() - espStart;
  //espStart = esp_timer_get_time();
  if (!wifiCnt) {
    analogContinuousStop();
    if (newString != oldString) {
      if(newString != st99){
          wifiRe = 22;
          vAvg = 0;   //start of sound source tracking, so reset sound source vector to zero
          vSum = 0;
          vCount = 0;
          prevX = 0;
          dirxn = 0;
      }
      oldString = newString;
    } else if (wifiRe > 33){
      wifiRe/=2;
    } else if(vCount){    // no loud sounds, so reset vector to zero
      vAvg = 0;
      vSum = 0;
      vCount = 0;
      prevX = 0;
      dirxn = 0;
    }
    // no matter what you must call this handleClient repeatidly--otherwise the web page
    // will not get instructions to do something
    server.handleClient();
    wifiCnt = wifiRe;
    //delay(1);
    if(udpClock > 999){
      //udp.end();
      ESP.restart();
      delay(1);
      startUDP();     //dunno why UDP sometimes quits working, so restart it
      udpClock = 777;
    } 
    udpClock++;
    onCnt--;
    if(!onCnt){
      onTime = millis()-onStart;
      onHoldr = onTime/60000;    //to minutes since ESP reset, for debugging
      onCnt = 111;
    }
    analogContinuousStart();
  } else wifiCnt--;
  loopCount = 0;
  //yxu = 0;
  //yTime=0;
  count = -144;  //because unknown time delays in first samples when count started @ zero
  //loopUs = esp_timer_get_time() - espStart;
  //espStart = esp_timer_get_time();
}