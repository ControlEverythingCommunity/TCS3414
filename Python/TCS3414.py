# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# TCS3414
# This code is designed to work with the TCS3414_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/content/Color?sku=TCS3414_I2CS#tabs-0-product_tabset-2

import smbus
import time

# Get I2C bus
bus = smbus.SMBus(1)

# TCS3414 address, 0x39(57)
# Select control register, 0x00(00), with Command register, 0x80(128)
#		0x03(03)	Power ON, ADC enable
bus.write_byte_data(0x39, 0x00 | 0x80, 0x03)
# TCS3414 address, 0x39(57)
# Select gain register, 0x07(07), with Command register, 0x80(128)
#		0x00(00)	Gain : 1x, Prescaler Mode = Divide by 1
bus.write_byte_data(0x39, 0x07 | 0x80, 0x00)

time.sleep(0.5)

# TCS3414 address, 0x39(57)
# Read data back from 0x10(16), 8 bytes, with Command register, 0x80(128)
# Green LSB, Green MSB, Red LSB, Red MSB
# Blue LSB, Blue MSB, cData LSB, cData MSB
data = bus.read_i2c_block_data(0x39, 0x10 | 0x80, 8)

# Convert the data
green = data[1] * 256 + data[0]
red = data[3] * 256 + data[2]
blue = data[5] * 256 + data[4]
cData = data[7] * 256 + data[6]

# Calculate luminance
luminance = (-0.32466 * red) + (1.57837 * green) + (-0.73191 * blue)

# Output data to screen
print "Green Color Luminance : %d lux" %green
print "Red Color Luminance : %d lux" %red
print "Blue Color Luminance : %d lux" %blue
print "Clear Data Luminance : %d lux" %cData
print "Ambient Light Luminance : %.2f lux" %luminance
