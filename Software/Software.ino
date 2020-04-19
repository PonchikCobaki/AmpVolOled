#define OLED_SOFT_BUFFER_64

#include <GyverOLED.h>
#include <Servo.h>

Servo motor;
GyverOLED oled;


const int kv = 1300; //оборотов на вольт
const int s = 4;
const int res = 510; // Сопротивление дв. в мОм

//int8_t  amp_max;
//int8_t  vol_max;
//int8_t  amp_min;
//int8_t  vol_max;

int vol_p = 1;
int amp_p = 2;

float cur_vol;




void setup() {
  Serial.begin(9600);
  motor.attach(2); 
  motor.write(90);

  oled.init(OLED128x64, 1200);   //  кГц шина i2c для экрана 
  oled.setContrast(126);
}

void loop() {
  delay(50);
  cur_vol = (float)map(analogRead(vol_p), 0, 1023, (1400), 1680)/100;
  Serial.println(cur_vol);

  oled.clear();
  oled.home();
  oled.roundRect(0, 0, 127, 63);

  oled.println("");

  oled.print("  Газ: ");
  oled.print(map(analogRead(0), 0, 1023, 0, 100));
  oled.println("%");

  oled.print("  Сила тока: ");
  oled.print(map(analogRead(amp_p), 0, 1023, 0, 150));
  oled.println("А");

  oled.print("  Нпряжение: ");
  oled.print(cur_vol);
  oled.println("В");

  oled.print("  Мощьность: ");
  oled.print(map(analogRead(amp_p), 0, 1023, 0, 150) * (int)cur_vol);
  oled.println("Вт");
  
  oled.println("");
  oled.print("  ОБОРОТЫ: ");
  oled.println(kv*map(analogRead(amp_p), 0, 1023, 0, 150) * res/1000);



  oled.update();
  
  motor.write(map(analogRead(0), 0, 1023, 0, 180));

}
