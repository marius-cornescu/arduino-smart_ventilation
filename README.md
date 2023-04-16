# Smart Ventilation

https://mariuscornescu.com/2022/11/03/arduino-smart-iot-prilex-ventilation-system/

# A. Pin Connections

## A.1 RF-433 RX
    GND  --> GND
    VCC  --> 5V
    DATA --> D2
  -------------------------------
## A.2 I2C  
    SDA is the serial data
    SCL is the serial clock
  
### A.2.1 I2C - DISPLAY
    GND --> GND
    VCC --> 5V
    SDA --> A4
    SCL --> A5
  
### A.2.2 I2C - RTC-DS3231
    Real time clock
    
### A.2.3 I2C - PCF8575 (Menu module)

    P0  --> Menu activity indicator LED
    P1  --> 
    P2  --> 
    P3  --> 
    P4  --> 
    P5  --> 
    P6  --> 
    P7  --> 
  
  -------------------------------
  
    P8  --> BTN_VENTILATION_1
    P9  --> BTN_VENTILATION_2
    P10 --> BTN_VENTILATION_3
    P11 --> BTN_UP
    P12 --> BTN_DOWN
    P13 --> BTN_BACK
    P14 --> BTN_FWARD
    P15 --> BTN_ENTER;
  
  
## A.3 RELAY
    D5  --> Relay1
    D6  --> Relay2
    D7  --> Relay3
    D8  --> Relay4
  -------------------------------
  
## A.4 MISC
    D13 --> Red led
    GND --> button <-- RST
    D4  --> Scheduler () <-- resistor --> GND
  -------------------------------

  -------------------------------


