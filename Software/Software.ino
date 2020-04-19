#define OLED_SOFT_BUFFER_64

#include <GyverOLED.h>
#include <Servo.h>

Servo motor;
GyverOLED oled;


const int kv = 1300; //оборотов на вольт
const int s = 4; // число ячеек в батарее li Po
const float res = 0.510; // Сопротивление дв. в Ом

int8_t  amp_max;
int8_t  th_max;

uint32_t Timer;

int8_t vol_p = 2; // пины входов
int8_t amp_p = 1;

float cur_vol;
float cur_amp;


void setup() {
  oled.init(OLED128x64, 1200);   //  кГц шина i2c для экрана 
  
  oled.setContrast(126);
  load_logo();
  

  Serial.begin(9600);


  motor.attach(13);
  motor.write(90);
  
  delay(1000);
}

void loop() {
  cur_vol = (float)map(analogRead(vol_p), 0, 1023, 140, 168) / 10;
  cur_amp = (float)map(analogRead(amp_p), 0, 1023, -1500, 1500)/10;
  
  if (cur_amp < 0) {
    cur_amp = 0;
  }
  
  Serial.print("Напряжение: ");
  Serial.println(cur_vol);

  Serial.print("Сила тока: ");
  Serial.println(cur_amp);

  Serial.print("Зн газа: ");
  Serial.println(cur_th());


  if (cur_amp > amp_max) {
    amp_max = cur_amp;
  }
  if (cur_th() > th_max) {
    th_max = cur_th();
  }

  if (millis() - Timer >= 1000 && map(analogRead(0), 0, 1023, 0, 100) == 0) {
    Timer = millis();
    
    oled.clear();
    oled.home();
    oled.println("Макс. значения");
    oled.println("");

    max_amp();
    max_th();
    max_pw();
    max_revol();

    oled.update();

    while (cur_th() == 0)
    {
      
    }
    
    th_max = 0;
    amp_max = 0;
  }


  oled.clear();
  oled.home();
  oled.roundRect(0, 0, 127, 63);
  oled.println("");

  amp();
  th();
  pw();
  vol();
  revol();

  oled.update();


  motor.write(map(analogRead(0), 0, 1023, 0, 180));

}

void th() {
  oled.print("  Газ: ");
  oled.print(map(analogRead(0), 0, 1023, 0, 100));
  oled.println("%");
}

void max_th() {
  oled.print(" Газ: ");
  oled.print(th_max);
  oled.println("%");
}

void amp() {
  oled.print("  ТОК: ");
  oled.print(cur_amp);
  oled.println("А");
}

void max_amp() {
  oled.print(" СИЛА ТОКА: ");
  oled.print(amp_max);
  oled.println("А");
}

void vol() {
  oled.print("  Нпряжение: ");
  oled.print(cur_vol);
  oled.println("В");
}

void pw() {
  oled.print("  Мощьность: ");
  oled.print((int)cur_amp * (int)cur_vol);
  oled.println("Вт");
}

void max_pw() {
  oled.print(" Мощьность: ");
  oled.print(amp_max * (int)cur_vol);
  oled.println("Вт");
}

void revol() {
  oled.println("");
  oled.print("  ОБОРОТЫ: ");
  oled.println((long)(kv * (float)cur_amp * (float)res));
}

void max_revol() {
  oled.print(" ОБОРОТЫ: ");
  oled.println((long)(kv * amp_max * (float)res));
}

int8_t cur_th() {
  return map(analogRead(0), 0, 1023, 0, 100);
}

void load_logo() {
  oled.clear();
  oled.home();
  oled.circle(63,31,31);
  oled.circle(63, 31, 3,1);
  oled.line(32, 63, 95, 63);
  oled.update();
}
