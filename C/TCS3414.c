// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TCS3414
// This code is designed to work with the TCS3414_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Color?sku=TCS3414_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, TCS3414 I2C address is 0x39(57)
	ioctl(file, I2C_SLAVE, 0x39);

	// Select control register OR with Command register(0x00 | 0x80)
	// Power ON, ADC enable(0x03)
	char config[2] = {0};
	config[0] = 0x00 | 0x80;
	config[1] = 0x03;
	write(file, config, 2);
	// Select gain register OR with Command register(0x07 | 0x80)
	// Gain = 1x, Prescaler Mode = Divide by 1(0x00)
	config[0] = 0x07 | 0x80;
	config[1] = 0x00;
	write(file, config, 2);
	sleep(1);

	// Read 8 bytes of data from register(0x10)
	// green lsb, green msb, red lsb, red msb
	// blue lsb, blue msb, cData lsb, cData msb
	char reg[1] = {0x10 | 0x80};
	write(file, reg, 1);
	char data[8] = {0};
	if(read(file, data, 8) != 8)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else
	{
		// Convert the data
		int green = (data[1] * 256 + data[0]);
		int red = (data[3] * 256 + data[2]);
		int blue = (data[5] * 256 + data[4]);
		int cData = (data[7] * 256 + data[6]);

		// Calculate luminance
		float luminance = (-0.32466 * red) + (1.57837 * green) + (-0.73191 * blue);
		if(luminance < 0)
		{
			luminance = 0;
		}

		// Output data to screen
		printf("Green color luminance : %d lux \n", green);
		printf("Red color luminance : %d lux \n", red);
		printf("Blue color luminance : %d lux \n", blue);
		printf("Clear Data  Luminance : %d lux \n ", cData);
		printf("Ambient Light Luminance : %.2f lux \n", luminance);
	}
}
