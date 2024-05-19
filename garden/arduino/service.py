import logging

import asab

import asyncio
import serial_asyncio

#

L = logging.getLogger(__name__)

#

asab.Config.add_defaults({
	'arduino': {
		'port': '/dev/ttyUSB0',
		'baudrate': 9600,
	},
})


class ArduinoService(asab.Service):

	def __init__(self, app, service_name="arduino.ArduinoService"):
		super().__init__(app, service_name)

		self.Writer = None

		metrics_svc = self.App.get_service('asab.MetricsService')
		self.Gauge = metrics_svc.create_gauge("arduino")


	async def main(self):
		port = asab.Config.get('arduino', 'port')
		baudrate = asab.Config.getint('arduino', 'baudrate')

		reader, self.Writer = await serial_asyncio.open_serial_connection(url=port, baudrate=int(baudrate))
		# reader, self.Writer = await asyncio.open_connection('127.0.0.1', 2002)
		try:
			while True:
				line = await reader.readline()
				if len(line) == 0:
					L.warning("Connection closed, reconnecting...")
					await asyncio.sleep(5)
					return

				line = line.decode('ascii').strip()

				name, value = line.split(':', 1)

				if name == "Voltage":
					value = 5.0 * (float(value) / 1024.0)
					R1 = 23000
					R2 = 8200
					value = value * ((R1 + R2) / R2)

				else:
					value = float(value)

				self.Gauge.set(name, value)

		finally:
			self.Writer = None

	async def finalize(self, app):
		if self.Writer is not None:
			self.Writer.close()
			await self.Writer.wait_closed()
