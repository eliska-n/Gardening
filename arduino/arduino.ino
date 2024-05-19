#include <AM2302-Sensor.h>

/// Paramterization

const int LED1 = 3;
const int LED2 = 5;
const int LED3 = 6;
const int LED4 = 9;
const int LED5 = 10;

#define temperatureHumidityPin 7
AM2302::AM2302_Sensor am2302 { temperatureHumidityPin };

#define voltagePin A5

// Arduino kapacitní senzor vlhkosti v půdě
#define hygro3Pin A3
#define hygro4Pin A4

/////

unsigned long previousMillis = 0;
const long interval = 10000;  // interval at which to read (milliseconds)

void setup() {
	// inicializace sériové linky s rychlostí 9600 baud pro odesilani vysledku
	Serial.begin(9600);
	am2302.begin();
}

void measure_soil() {
	auto data3 = analogRead(hygro3Pin);
	Serial.print("Hygrometer 3: ");
	Serial.println(data3);

	auto data4 = analogRead(hygro4Pin);
	Serial.print("Hygrometer 4: ");
	Serial.println(data4);
}

void measure_voltage() {
	auto sensorValue = analogRead(voltagePin);
	Serial.print("Voltage: ");
	Serial.println(sensorValue);
}

void measure_temperature_humidity() {
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
		measure_voltage();
		measure_temperature_humidity();
		blink();
	}
	delay(1000);
}
