#include <CapacitiveSensor.h>

CapacitiveSensor cs_3_2 = CapacitiveSensor(3,2); 
CapacitiveSensor cs_5_4 = CapacitiveSensor(5,4);
CapacitiveSensor cs_7_6 = CapacitiveSensor(7,6);
CapacitiveSensor cs_9_8 = CapacitiveSensor(9,8);

int value;
int oldValue;

long start, end;

void setup() {
  cs_3_2.set_CS_AutocaL_Millis(0xFFFFFFFF); 
  cs_5_4.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_7_6.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_9_8.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(115200);

  value = 0;
  oldValue = 0;
  start = 0;
  end = 0;
}

void loop() {
  long result = 0;

  // read from touch sensors
  long total1 = cs_3_2.capacitiveSensor(30);
  long total2 = cs_5_4.capacitiveSensor(30);
  long total3 = cs_7_6.capacitiveSensor(30);
  long total4 = cs_9_8.capacitiveSensor(30);

  if(total1 > 1000) {
    result = result | 0x1;
  }
  if(total2 > 1000) {
    result = result | 0x2;
  }
  if(total3 > 1000) {
    result = result | 0x4;
  }
  if(total4 > 1000) {
    result = result | 0x8;
  }

  result = result << 8;
  if(result == 0) {
    result = 1;
  }

  value = analogRead(0);

  // check if wheel has completed a revolution
  if(value > 500 && oldValue <= 500) {
    end = millis() - start;
    start = millis();
  }
  oldValue = value;

  result = result + end;

  // send result to supercollider, delinated with 'a'
  Serial.print(result);
  Serial.print('a');

  delay(0.1);
}
