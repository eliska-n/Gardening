#!/usr/bin/env python3
import asab.metrics
import asab.metrics.influxdb
import serial
import asyncio
import aiohttp
import logging

import asab.api
import asab.web.rest


L = logging.getLogger(__name__)


class MyApplication(asab.Application):
	'''
	The application is available at http://localhost:8080/hello
	'''

	def __init__(self):
		super().__init__()

		self.Ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
		self.Queue = asyncio.Queue()

		mockservice = MockService()
		mockservice.App = self
		self.InfluxDBTarget = InfluxDBTarget(mockservice, "asab:metrics:influxdb")

		self.PubSub.subscribe("Application.tick!", self._on_tick_measure)
		self.PubSub.subscribe("Application.tick/10!", self._on_tick_send)

	async def finalize(self):
		self.Ser.close()

	async def _on_tick_measure(self, _):
		while True:
			now = int(self.time()) * 1000 * 1000 * 1000
			line = self.Ser.readline()
			if line:
				# Converting Byte Strings into unicode strings
				string = line.decode()
				# "testgauge,host=mockedhost.com,appclass=mockappclass,instance_id=test-instance-id-1 v1=1i 123450000000\n"
				output = [i.strip() for i in string.strip("\n").split(":")]
				influx_line = "soil_humidity,hygrometer={} drought={}i {}\n".format(output[0], output[1], now)
				self.Queue.put_nowait(influx_line)
			else:
				break


	async def _on_tick_send(self, _):
		lines = ""
		while not self.Queue.empty():
			line = self.Queue.get_nowait()
			lines += line

		if len(lines) > 0:
			await self.InfluxDBTarget.process(lines)


class MockService():
	def _init_():
		pass


class InfluxDBTarget(asab.metrics.influxdb.InfluxDBTarget):
	async def process(self, lines):
		try:
			async with aiohttp.ClientSession(headers=self.Headers) as session:
				async with session.post(self.WriteURL, data=lines) as resp:
					response = await resp.text()
					if resp.status != 204:
						L.warning(
							"Error when sending metrics to Influx: {}\n{}".format(resp.status, response),
							struct_data={"url": self.BaseURL}
						)
		except aiohttp.client_exceptions.ClientConnectorError:
			L.error("Failed to connect to InfluxDB.", struct_data={"url": self.BaseURL})
		except Exception as err:
			L.exception("Failed to send metrics to InfluxDB: {}".format(err), struct_data={"url": self.BaseURL})



if __name__ == '__main__':
	app = MyApplication()
	app.run()
