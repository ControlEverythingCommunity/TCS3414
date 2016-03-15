// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TCS3414
// This code is designed to work with the TCS3414_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Color?sku=TCS3414_I2CS#tabs-0-product_tabset-2

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class TCS3414
{
	public static void main(String args[]) throws Exception
	{
		// Create I2C bus
		I2CBus Bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, TCS3414 I2C address is 0x39(57)
		I2CDevice device = Bus.getDevice(0x39);

		// Select control register OR with Command register
		// Power ON, ADC enable
		device.write(0x00 | 0x80, (byte)0x03);
		// Select gain register OR with Command register
		// Gain = 1x, Prescaler Mode = Divide by 1
		device.write(0x07 | 0x80, (byte)0x00);
		Thread.sleep(500);

		// Read 8 bytes of data from adress 0x10(16)
		// green lsb, green msb, red lsb, red msb
		// blue lsb, blue msb, cData lsb, cData msb
		byte[] data= new byte[8];
		device.read(0x10 | 0x80, data, 0, 8);

		// Convert the data
		int green = ((data[1] & 0xFF) * 256 + (data[0] & 0xFF));
		int red = ((data[3] & 0xFF) * 256 + (data[2] & 0xFF));
		int blue = ((data[5] & 0xFF) * 256 + (data[4] & 0xFF));
		int cData = ((data[7] & 0xFF) * 256 + (data[6] & 0xFF));

		// Calculate luminance
		double luminance = (-0.32466 * red) + (1.57837 * green) + (-0.73191 * blue)

		// Output data to Screen
		System.out.printf("Green Color Luminance : %d lux %n", green);
		System.out.printf("Red Color Luminance : %d lux %n", red);
		System.out.printf("Blue Color Luminance : %d lux %n", blue);
		System.out.printf("Clear data Luminance : %d lux %n", cData);
		System.out.printf("Ambient Light Luminance : %.2f lux %n", luminance);
	}
}
