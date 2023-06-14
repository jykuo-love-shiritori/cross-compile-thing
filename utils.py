'''
        Read Gyro and Accelerometer by Interfacing Raspberry Pi with MPU6050 using Python
        http://www.electronicwings.com
'''
import smbus  # import SMBus module of I2C
from time import sleep  # import
from os import system

import requests

WEBHOOK_URL = "https://discord.com/api/webhooks/1116683787879862373/0sZuth7KpTqYviQR8jTU3HLU5HJrg05g1SJRbDjMdZ9v2DaszZYeAQIwoT2MYmm5Lt3-"
JENSEN_URL = "https://external-content.duckduckgo.com/iu/?u=http%3A%2F%2F2.bp.blogspot.com%2F-TwBqcOSRgso%2FUGJK1-iRjxI%2FAAAAAAAABt4%2FA1510L819Uc%2Fs1600%2FBiography%2BJen%2BHsun%2BHuang%2B-%2BCompany%2BFounder%2BNvidia.jpg&f=1&nofb=1&ipt=154a80437319fe951668dd109bce45d17c1f1f7d0396c85b14fda2bdf4cebe10&ipo=images"

# some MPU6050 Registers and their Address
PWR_MGMT_1 = 0x6B
SMPLRT_DIV = 0x19
CONFIG = 0x1A
GYRO_CONFIG = 0x1B
ACCEL_CONFIG = 0x1C
INT_ENABLE = 0x38
ACCEL_XOUT_H = 0x3B
ACCEL_YOUT_H = 0x3D
ACCEL_ZOUT_H = 0x3F
GYRO_XOUT_H = 0x43
GYRO_YOUT_H = 0x45
GYRO_ZOUT_H = 0x47

def MPU_Init():
    # write to sample rate register
    bus.write_byte_data(Device_Address, SMPLRT_DIV, 7)

    # Write to power management register
    bus.write_byte_data(Device_Address, PWR_MGMT_1, 1)

    # Write to Configuration register
    bus.write_byte_data(Device_Address, CONFIG, 0)

    # Write to Gyro configuration register
    bus.write_byte_data(Device_Address, GYRO_CONFIG, 0b00011000)

    bus.write_byte_data(Device_Address, ACCEL_CONFIG, 0b00010000)

    # Write to interrupt enable register
    bus.write_byte_data(Device_Address, INT_ENABLE, 1)


def read_raw_data(addr):
    # Accelero and Gyro value are 16-bit
    high = bus.read_byte_data(Device_Address, addr)
    low = bus.read_byte_data(Device_Address, addr+1)

    # concatenate higher and lower value
    value = ((high << 8) | low)

    # to get signed value from mpu6050
    if(value > 32768):
        value = value - 65536
    return value


bus = smbus.SMBus(1)         # or bus = smbus.SMBus(0) for older version boards
Device_Address = 0x68   # MPU6050 device address
