#include <LowPower.h>
#include <Wire.h>
#include "mpu_regs.h"

#define SITTIN_DURATION_MAX_MIN 1

#define BUZZER_PIN          11
#define MEASURES_PERIOD_S   8
#define MEASURES_DURATION_S 10
#define SENSOR_TOLERANCE    0x002f

constexpr int number_of_8s_cycles = MEASURES_PERIOD_S / 8;
constexpr int sitting_cycles_limit = SITTIN_DURATION_MAX_MIN * 60 / (number_of_8s_cycles * 8);

class Sensor{
  int addr;
  int init_x;
  int init_y;
  int init_z;
  int read(int addr, int count){
    Wire.beginTransmission(MPU_6050_I2C_ADDR);
    Wire.write(addr);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_6050_I2C_ADDR, count);
    int final= 0;
    for(int i = 0; i < count; ++i){
      final += Wire.read() << 8*(count - 1 - i);
    }
    return final;
  }
  void read_all(){
    x = read(REG_AX_H, 2);
    y = read(REG_AY_H, 2);
    z = read(REG_AZ_H, 2);
  }
 public:
  int x = 0;
  int y = 0;
  int z = 0;
  Sensor(bool AD0 = false){
    addr = MPU_6050_I2C_ADDR | (AD0 ? 1 : 0);
  }
  void init(){
    Wire.begin();
    Wire.setClock(400000);

    wake(false);

    //set minimal sensitivity
    Wire.beginTransmission(MPU_6050_I2C_ADDR);
    Wire.write(REG_A_CFG);
    Wire.write(A_CFG_16G);
    Wire.endTransmission();

    set_this_state_as_stationary();
  }
  void set_this_state_as_stationary(){
    init_x = read(REG_AX_H, 2);
    init_y = read(REG_AY_H, 2);
    init_z = read(REG_AZ_H, 2);
  }
  void sleep(){
    Wire.beginTransmission(addr);
    Wire.write(REG_PWR_MGMT_1);
    Wire.write(0b01011000);   //reset = 0,    sleep = 1, cycle = 0, gyro_standby = 1
                              //temp_dis = 1, clock_src = 0 (internal)
    Wire.endTransmission();
  }
  void wake(bool endTrans = true){
    Wire.beginTransmission(addr);
    Wire.write(REG_PWR_MGMT_1);
    Wire.write(0b00011000);   //reset = 0,    sleep = 0, cycle = 0, gyro_standby = 1
                              //temp_dis = 1, clock_src = 0 (internal)
    Wire.endTransmission(false);
    Wire.write(REG_USER_CTRL);
    Wire.write(USER_CTRL_SIG_COND_RESET);
    Wire.endTransmission(endTrans);
  }
  bool moved(){
    read_all();
    return (abs(x - init_x) > SENSOR_TOLERANCE)
        || (abs(y - init_y) > SENSOR_TOLERANCE)
        || (abs(z - init_z) > SENSOR_TOLERANCE);
  }
  bool moved(long measure_for){
    set_this_state_as_stationary();
    long start_time = millis();
    while(millis() < start_time + measure_for){
      if(moved()) return true;
    }
    return false;
  }
};

class Led{
 public:
  void init(){
    pinMode(LED_BUILTIN, OUTPUT);
  }
  void On(long length = 0){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(length);
  }
  void Off(long length = 0){
    digitalWrite(LED_BUILTIN, LOW);
    delay(length);
  }
  void Blink(unsigned long period, int count){
    for(int i = 0; i < count; ++i){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(period);
      digitalWrite(LED_BUILTIN, LOW);
      delay(period);
    }
  }
};

class Signaler{
 public:
  virtual void init();
  virtual void signal(int);
}; 

class LedSignaler : Signaler{
  Led led{};
 public:
  void init() override {
    led.init();
  }
  void signal(int blinkCount = 1) override{
    for(int i = 0; i < blinkCount; ++i){
      led.On(100);
      led.Off(100);
    }
  }
};

class BuzzerSignaler : Signaler{
 public:
  void init() override{
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
  }
  void signal(int time = 1) override{
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100*time);
    digitalWrite(BUZZER_PIN, LOW);
  }
};

class PeripheriesManager{
  Sensor sensor{};
  LedSignaler ledSignaler{};
  BuzzerSignaler buzzer{};

  int sitting_for = 0;
 public:
  void init(){
    sensor.init();
    ledSignaler.init();
    buzzer.init();
    sensor.sleep();
  }
  void signal_if_sitting_too_long(){
    sensor.wake();
    delay(100);    //wait for accelerometer to measure
    if(sensor.moved(1000 * MEASURES_DURATION_S)){
      sitting_for++;
      ledSignaler.signal(2);
    }
    else
      sitting_for = 0;

    if(sitting_for >= sitting_cycles_limit){
      sitting_for = 0;
      buzzer.signal(5);
    }
    sensor.sleep();
  }
};

PeripheriesManager peripheries{};
Led led{};

void setup() {
  // put your setup code here, to run once:
  peripheries.init();
  led.init();
}

void loop() {
  for(int i = 0; i<number_of_8s_cycles; ++i)
    LowPower.powerDown(SLEEP_8S, ADC_ON, BOD_OFF);
  

  led.On();
  peripheries.signal_if_sitting_too_long();
  led.Off(); 
}
