#include <AM2302-Sensor.h>
#include <LowPower.h>


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

void idle_sleep_8() {
	LowPower.idle(
		SLEEP_8S,
		ADC_OFF,
		TIMER4_OFF,
		TIMER3_OFF,
		TIMER1_OFF, 
		TIMER0_OFF,
		SPI_OFF,
		USART1_OFF,
		TWI_OFF,
		USB_OFF
	);
}

void loop() {
	idle_sleep_8();
	
	delay(2000);
	measure_soil();
	idle_sleep_8();

	delay(2000);
	measure_voltage();
	idle_sleep_8();

	delay(2000);
	measure_temperature_humidity();
	idle_sleep_8();
}
