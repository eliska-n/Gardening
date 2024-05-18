#include <AM2302-Sensor.h>
const int LED1 = 3;
const int LED2 = 5;
const int LED3 = 6;
const int LED4 = 9;
const int LED5 = 10;

#define SENSOR_PIN 7
AM2302::AM2302_Sensor am2302 { SENSOR_PIN };

// Arduino kapacitní senzor vlhkosti v půdě
#define analogPin3 A3
#define analogPin4 A4


// proměnná pro ukládání výsledků měření
int data3 = 0;
int data4 = 0;

unsigned long previousMillis = 0;
const long interval = 10000;  // interval at which to read (milliseconds)

void setup() {
  // inicializace sériové linky s rychlostí 9600 baud
  Serial.begin(9600);
  am2302.begin();
}

void measure_soil() {
  data3 = analogRead(analogPin3);
  data4 = analogRead(analogPin4);

  // vytištění výsledků po sériové lince
  Serial.print("Hygrometer 3: ");
  Serial.println(data3);

  Serial.print("Hygrometer 4: ");
  Serial.println(data4);
}

void measure_inner() {
  // Measure a voltage
  int sensorValue = analogRead(A5);
  Serial.print("Voltage: ");
  Serial.println(sensorValue);
  // Measure a temperature and humidity
  auto status = am2302.read();
  if (status == 0) {
    Serial.print("Temperature: ");
    Serial.println(am2302.get_Temperature());
    Serial.print("Humidity: ");
    Serial.println(am2302.get_Humidity());
  }
}

void blink() {
  float in, out;
  for (in = 0; in < 6.283; in = in + 0.002)
  {
    out = sin(in) * 127.5 + 127.5;
    analogWrite(LED1, out);
    out = sin(in+0.628) * 127.5 + 127.5;
    analogWrite(LED2, out);
    out = sin(in+0.628*2) * 127.5 + 127.5;
    analogWrite(LED3, out);
    out = sin(in+0.628*3) * 127.5 + 127.5;
    analogWrite(LED4, out);
    out = sin(in+0.628*4) * 20.0 + 20.0;
    analogWrite(LED5, out);
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    measure_soil();
	measure_inner();
	blink();
  }
  delay(1000);
}
