/*
 * This file is an MPU6050 demonstration.
 * https://openest.io/en/2020/01/21/mpu6050-accelerometer-on-raspberry-pi/
 * Copyright (c) 2020 Julien Grossholtz - https://openest.io.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define MPU6050_I2C_ADDR 0x68

#define REG_ACCEL_ZOUT_H 0x3F
#define REG_ACCEL_ZOUT_L 0x40
#define REG_PWR_MGMT_1 0x6B
#define REG_ACCEL_CONFIG 0x1C
#define REG_SMPRT_DIV 0x19
#define REG_CONFIG 0x1A
#define REG_FIFO_EN 0x23
#define REG_USER_CTRL 0x6A
#define REG_FIFO_COUNT_L 0x72
#define REG_FIFO_COUNT_H 0x73
#define REG_FIFO 0x74
#define REG_WHO_AM_I 0x75

int file = -1;

// Please note, this is not the recommanded way to write data
// to i2c devices from user space.
void i2c_write(__u8 reg_address, __u8 val) {
	char buf[2];
	if(file < 0) {
		perror("Error, i2c bus is not available");
		exit(1);
	}

	buf[0] = reg_address;
	buf[1] = val;

	if (write(file, buf, 2) != 2) {
		perror("Error, unable to write to i2c device");
		exit(1);
	}

}

// Please note, this is not thre recommanded way to read data
// from i2c devices from user space.
char i2c_read(uint8_t reg_address) {
	char buf[1];
	if(file < 0) {
		printf("Error, i2c bus is not available\n");
		exit(1);
	}

	buf[0] = reg_address;

	if (write(file, buf, 1) != 1) {
		printf("Error, unable to write to i2c device\n");
		exit(1);
	}


	if (read(file, buf, 1) != 1) {
		printf("Error, unable to read from i2c device\n");
		exit(1);
	}

	return buf[0];

}

uint16_t merge_bytes( uint8_t LSB, uint8_t MSB) {
	return  (uint16_t) ((( LSB & 0xFF) << 8) | MSB);
}

// 16 bits data on the MPU6050 are in two registers,
// encoded in two complement. So we convert those to int16_t
int16_t two_complement_to_int( uint8_t LSB, uint8_t MSB) {
	int16_t signed_int = 0;
	uint16_t word;

	word = merge_bytes(LSB, MSB);

	if((word & 0x8000) == 0x8000) { // negative number
		signed_int = (int16_t) -(~word);
	} else {
		signed_int = (int16_t) (word & 0x7fff);
	}

	return signed_int;
}

int main(int argc, char *argv[]) {
	int adapter_nr = 1; /* probably dynamically determined */
	char bus_filename[250];
	char accel_x_h,accel_x_l,accel_y_h,accel_y_l,accel_z_h,accel_z_l,temp_h,temp_l;
	uint16_t fifo_len = 0;
	int16_t x_accel = 0;
	int16_t y_accel = 0;
	int16_t z_accel = 0;
	int16_t temp = 0;
	float x_accel_g, y_accel_g, z_accel_g, temp_f;

	snprintf(bus_filename, 250, "/dev/i2c-1", adapter_nr);
	file = open(bus_filename, O_RDWR);
	if (file < 0) {
		perror("file");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}


	if (ioctl(file, I2C_SLAVE, MPU6050_I2C_ADDR) < 0) {
		/* ERROR HANDLING; you can check errno to see what went wrong */
		perror("ioctl");
		exit(1);
	}

	i2c_write(REG_PWR_MGMT_1, 0x01);
	i2c_write(REG_ACCEL_CONFIG, 0x00);
	i2c_write(REG_SMPRT_DIV, 0x07);
	i2c_write(REG_CONFIG, 0x00);
	i2c_write(REG_FIFO_EN, 0x88);
	i2c_write(REG_USER_CTRL, 0x44);

	// while(1) {
	// 	static float pos = 0;
	// 	static float ppppp = 0;
	// 	char h = i2c_read(0x43);
	// 	char l = i2c_read(0x44);
	// 	uint16_t t = two_complement_to_int(h, l);
	// 	//printf("0x%08x", t);
	// 	float f = ((float) t)/16384;
	// 	pos += f-1;
	// 	//printf("pos %f", pos);
	// 		//printf("\n");
	// 	if (f-ppppp > 0.5 || f-ppppp < -0.5) {
	// 		//printf("%f", f);
	// 		printf("f %f\t%d", f, ppppp);
	// 		printf("\n");
	// 		//ppppp += f-1;
	// 		ppppp = f;
	// 	}
	// 	// ppppp = f;
	// 	//printf("%d ", t);
	// }
	// exit(0);

	while(1) {
		accel_x_h = i2c_read(REG_FIFO_COUNT_L);
		accel_x_l = i2c_read(REG_FIFO_COUNT_H);
		fifo_len = merge_bytes(accel_x_h,accel_x_l);

		//printf("fifo len:%i", fifo_len);
		if(fifo_len == 1024) {
			//printf("fifo overflow !\n");
			i2c_write(REG_USER_CTRL, 0x44);
			break;
			continue;
		}

		if(fifo_len >= 8) {
			char bufffff[400];
			for (int i=0;i<4; i++) {
				char a = i2c_read(REG_FIFO);
				char b = i2c_read(REG_FIFO);
				int16_t t = two_complement_to_int(a, b);
				printf("0x%08x ", t);
			}
			printf("\r");

			//accel_x_h = i2c_read(REG_FIFO);
			//accel_x_l = i2c_read(REG_FIFO);
			//accel_y_h = i2c_read(REG_FIFO);
			//accel_y_l = i2c_read(REG_FIFO);
			//accel_z_h = i2c_read(REG_FIFO);
			//accel_z_l = i2c_read(REG_FIFO);
			//temp_h = i2c_read(REG_FIFO);
			//temp_l= i2c_read(REG_FIFO);

			//x_accel= two_complement_to_int(accel_x_h,accel_x_l);
			//x_accel_g = ((float) x_accel)/16384;

			//y_accel= two_complement_to_int(accel_y_h,accel_y_l);
			//y_accel_g = ((float) y_accel)/16384;

			//z_accel= two_complement_to_int(accel_z_h,accel_z_l);
			//z_accel_g = ((float) z_accel)/16384;

			//temp = two_complement_to_int(temp_h, temp_l);
			//temp_f = (float)temp/340 + 36.53; // calculated as described in the MPU60%) register map document

			//printf("x_gyro %.3fg	y_gyro %.3fg	z_accel %.3fg	temp=%.1fc         \r", x_accel_g, y_accel_g, z_accel_g, temp_f);
		} else {
			usleep(10000);
		}

	}

	return 0;
}

