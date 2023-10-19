# evcc_power_ring

A LED ring displaying the current status from EVCC (https://github.com/evcc-io/evcc)

In contrary to the EVCC bar info, the ring shows the current absolute values, not the relative ones.

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/36a74dee-c43e-4e8f-a362-762036d6f335)

Each LED represent a certain amount of WATT (e.g. 400 Watt)
The color logic is the nearly the same as the EVCC displays it in the top bar 

LEDs will light
- "green" if PV power is used by household and/or charging the car
- "yellow" if PV power is sent grid
- "red" if power is used from grid (same as "grey" as in EVCC)

***

## Hardware:

- 24Bit RGB LED Ring WS2812
- ESP-01S (or any other board running ESP Easy FW)
- AMS1117 DC-DC Step Down Buck Converter 3.3 V BREAKOUT Board
- EVCC running on e.g. RaspberryPi with MQTT configured
- (optional) Adafruit 1833 USB MICRO-B BREAKOUT BOARD

### Schematic:

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/c3c2fa3b-7bf5-470b-b107-2ba651add660)

### Board:

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/60877ca7-3e40-4be3-86e5-47ae1d2307df)

Soldered board example (ESP01S + AMS1117)

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/bb434075-1779-4083-a693-986484157e78)

First breadboard setup with ESP12 and the 24 LED ring in action:

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/9307b9b5-e34e-4c3a-8112-3f387d36da28)


## Software:

### 1. Flash the suitable ESP Easy FW on your ESP Board

see https://github.com/letscontrolit/ESPEasy for details

### 2. Configure ESP Easy Firmware

via webinterface by calling your devices IP adress in a browser

#### 2.1 Configure MQTT Broker 

go to "Controllers" tab and add your IP to your MQTT broker

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/b3325e60-f2bd-42fc-9052-30f77e1a137a)

#### 2.2 Enable "Rules" feature

Go to "Tools" -> "Advanced" -> tick the first box "Rules"
  
![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/3c56d7f7-92ac-4004-b3b8-0b6bd467071e)

#### 2.3 Create the devices

"Output - NeoPixel (Basic)" & "Generic - MQTT Import"

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/2660ef64-a41e-40ef-b3c2-23098da7dba6)

#### 2.4 Configuration of "Generic - MQTT Import" device

- MQTT Topic 1: evcc/site/homePower
- MQTT Topic 2: evcc/site/gridPower
- MQTT Topic 3: evcc/site/pvPower
- MQTT Topic 4: evcc/loadpoints/1/chargePower (maybe needs to be adapted according to your EVCC setup)

and name them:

1. homePower
2. gridPower
3. pvPower
4. chargePower

Hint: the names are referenced in the rules set code so changes needs to be also done in rules code.

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/31e9f45a-0b91-4266-8969-63adf3e98dc9)

#### 2.5 Configuration of the "Output - NeoPixel (Basic)" device

Set the "Led Count" to 24 and configure the "GPIO -> DIN" to "GPIO-0 (D3)"
(e.g. if you're using an ESP01S board)

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/492a0e20-b1eb-4961-b9ad-d11baae73fca)

#### 2.6 Copy the "rules"

Copy paste the code from the "rule_set_1" file here in GitHub to the "Rules" field and save:

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/27584a7a-f552-45b8-96ff-0ac4e496c07a)

#### 2.7 Adapt the "rules"

in line 2 of the rules

> Let,5,400 // Define resolution in watt per LED

you can define how much watts 1 LED should represent.

The command "Let,5" sets the 5th internal variable and ",400" is the value to be set according to this schematic:

> Let,\<n\>,\<value\>

*More info about rules: https://espeasy.readthedocs.io/en/latest/Rules/Rules.html#internal-variables*

So in this case the 1 LED represents 400 Watt which means when all 24 LEDs of the ring lit up, this equals 9600 W

## Watch the magic happen

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/7311a2d0-8275-4f12-bef8-2e016e3a4295)

![image](https://github.com/maschiach/evcc_power_ring/assets/57842368/0e5f662b-c1b2-4f35-ab43-01e8e7346347)



Further info: About rules
Link to ESP Easy Rules description (https://espeasy.readthedocs.io/en/latest/Rules/Rules.html)https://espeasy.readthedocs.io/en/latest/Rules/Rules.html
