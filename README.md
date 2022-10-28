# smart_ventilation

## Pin Connections
  
### RF-433 RX
  -------------------------------
    GND  --> GND
    VCC  --> 5V
    DATA --> D2

### DISPLAY - I2C
  -------------------------------
    SDA is the serial data
    SCL is the serial clock

    GND --> GND
    VCC --> 5V
    SDA --> A4
    SCL --> A5

### DISPLAY - I2C
  -------------------------------
    SDA is the serial data
    SCL is the serial clock

### RTC-DS3231 - I2C
  -------------------------------
    SDA is the serial data
    SCL is the serial clock

### RELAY
  -------------------------------
    D5  --> Relay1
    D6  --> Relay2
    D7  --> Relay3
    D8  --> Relay4

### MISC
  -------------------------------
    D13 --> Red led
    GND --> button <-- RST
    D4  --> Scheduler () <-- resistor --> GND


  -------------------------------


