#include <AM2302-Sensor.h>

/// Paramterization
#define temperatureHumidityPin 7

#define voltagePin A5

// Arduino kapacitní senzor vlhkosti v půdě
#define hygro3Pin A3
#define hygro4Pin A4

/////

AM2302::AM2302_Sensor am2302 { temperatureHumidityPin };

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

void loop() {
	measure_soil();
	measure_voltage();
	measure_temperature_humidity();

	delay(30 * 1000); // Measure once per 30 seconds
}
