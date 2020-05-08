#include <OLED_I2C.h>
#include <ArduinoJson.h>
#include <MsTimer2.h>
#include <TimeLib.h>
#include <avr/delay.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(8, 9);

long Millis_updated = 0;
String bt_msg = "";

OLED  display(SDA, SCL, 4);

extern uint8_t SmallFont[];
extern uint8_t BigNumbers[];


static const uint8_t PROGMEM bat_0 [] = {
  0xE0, 0xF0, 0xF8, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xF0, 0xE0, 0x80,
  0x07, 0x0F, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x0F, 0x07, 0x01
};
static const uint8_t PROGMEM bat_1 [] = {
  0xE0, 0xF0, 0xF8, 0x38, 0xF8, 0xF8, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xF0, 0xE0, 0x80,
  0x07, 0x0F, 0x1F, 0x1C, 0x1F, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x0F, 0x07, 0x01
};
static const uint8_t PROGMEM bat_2 [] = {
  0xE0, 0xF0, 0xF8, 0x38, 0xF8, 0xF8, 0x38, 0xF8, 0xF8, 0x38, 0x38, 0x38, 0x38, 0xF0, 0xE0, 0x80,
  0x07, 0x0F, 0x1F, 0x1C, 0x1F, 0x1F, 0x1C, 0x1F, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x0F, 0x07, 0x01
};
static const uint8_t PROGMEM bat_3 [] = {
  0xE0, 0xF0, 0xF8, 0x38, 0xF8, 0xF8, 0x38, 0xF8, 0xF8, 0x38, 0xF8, 0xF8, 0x38, 0xF0, 0xE0, 0x80,
  0x07, 0x0F, 0x1F, 0x1C, 0x1F, 0x1F, 0x1C, 0x1F, 0x1F, 0x1C, 0x1F, 0x1F, 0x1C, 0x0F, 0x07, 0x01
};
static const uint8_t PROGMEM bat_4 [] = {
  0xE0, 0xF0, 0xF8, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xB8, 0xB8, 0x38, 0x38, 0xF0, 0xE0, 0x80,
  0x07, 0x0F, 0x1F, 0x1C, 0x1C, 0x1D, 0x1F, 0x1F, 0x1D, 0x1D, 0x1D, 0x1D, 0x1C, 0x0F, 0x07, 0x01
};
static const uint8_t PROGMEM  talk [] = {
  0xFC, 0xFE, 0xFF, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0F, 0xFF, 0xFE, 0xFC,
  0x0F, 0x1F, 0x3F, 0x7C, 0xF0, 0xE0, 0x70, 0x38, 0x38, 0x38, 0x38, 0x38, 0x3C, 0x1F, 0x0F, 0x07
};
static const uint8_t PROGMEM bluetooth []  = {
  0x00, 0x00, 0x00, 0x00, 0x30, 0x60, 0xC0, 0xFE, 0x86, 0xCC, 0x78, 0x30, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x18, 0x0C, 0x06, 0xFF, 0xC3, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00
};


void setup() {
  Serial.begin(115200);
  BT_init("SeeEYE", 9600);
  display_int();
  MsTimer2::set(1000, clock);
  MsTimer2::start();
}

/*/
   Пример отправки данных:
   {
    1: "10-21",
    2: 60,
    3: 3,
    4: 1,
    5: "Aleksandr Ruban"
    6: "Privet, urod"
   }
  /*/

void loop() {
  if  (BT_check_data()) {
    BT_readData();
    DynamicJsonBuffer jsonBuffer;
    Serial.println(bt_msg);
    JsonObject& data = jsonBuffer.parseObject(bt_msg);
    if (!data.success()) {
      display_error(1);
      return;
    }
    setTime(data["1"]);

    display.clrScr();

    //Батарея
    if (data["2"] < 25) display.drawBitmap(112, 0, bat_0, 16, 16);
    else if (data["2"] >= 25 && data["2"] < 50) display.drawBitmap(112, 0, bat_1, 16, 16);
    else if (data["2"] >= 50 && data["2"] < 75) display.drawBitmap(112, 0, bat_2, 16, 16);
    else if (data["2"] >= 75 && data["2"] < 100) display.drawBitmap(112, 0, bat_3, 16, 16);
    else display.drawBitmap(112, 0, bat_4, 16, 16);
    //Что-то другое

  }
  if (millis() - Millis_updated > 1000) {
    Millis_updated = millis();
    display.update();
  }
}

void msg(byte type, char* from_msg, char* msg) {
  switch (type) {

  }
}

void display_error(byte type) {
  display.clrScr();
  display.setFont(SmallFont);
  switch (type) {
    case 1: display.print("parseObject failed", CENTER, 10);
      display.update();
      display.invert(true);
      delay(500);
      display.invert(false);
      delay(500);
      display.invert(true);
      delay(500);
      display.invert(false);
      break;
    case 2: display.print("SeeEYE v.2", CENTER, 10);
      display.print("Connect", CENTER, 19);
      display.print("Waiting for data", CENTER, 28);
      display.update();
      break;
    case 3: display.print("SeeEYE v.2", CENTER, 10);
      display.print("Connection...", CENTER, 19);
      display.update();
      break;
    default: display.print("Unknown error", CENTER, 10);
      display.update();
  }
}

void check_bet() {
  uint8_t check = analogRead(A0);

  //Тута батарея
}

void clock() {
  display.setFont(BigNumbers);
  display.print(String(hour()) + "-" + String(minute()), CENTER, 0);
  display.update();
}

void display_int() {
  display.begin();
  display.setFont(SmallFont);
  display.setBrightness(150);
  //open test
  display.drawBitmap(112, 0, bat_4, 16, 16);
  display.drawBitmap(112, 16, bluetooth, 16, 16);
  display.drawBitmap(112, 32, talk, 16, 16);
  //close test
}

void BT_init(const char *_name, int _rate) {
  BTSerial.begin(_rate);
  return;
  /*if(BT_check_bt_connect()){
      BTSerial.print("AT+NAME");
      BTSerial.print(_name);
      return;
    }*/
}

bool BT_readData() {
  _delay_ms(50);
  bt_msg = "";
  char last_read;
  bool record = false;
  while (last_read != '}') {
    if (BT_check_data()) {
      last_read = (char)BTSerial.read();
      if (last_read == '{') record = true;
      if (record) bt_msg += last_read;
      Serial.println(bt_msg);
    }
  }
  return 1;
}

bool BT_sendData(const char *_data) {
  BTSerial.print(_data);
  return 1;
}

bool BT_check_bt_connect() {
  BT_sendData("AT");
  _delay_ms(800);
  if (BT_readData() && bt_msg == "OK") {
    return 1;
  }
  else {
    return 0;
  }
}

bool BT_check_data() {
  if (BTSerial.available()) {
    return 1;
  }
  else {
    return 0;
  }
}
