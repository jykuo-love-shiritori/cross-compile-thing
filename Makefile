setup:
	i2cset -y 1 0x68 0x6B 0x01
	i2cset -y 1 0x68 0x1C 0x00
setup-fifo:
	# The MPU-6050 has a 1024 byte internal FIFO, which is useful when reading data with a high sample rate. You must first activate this FIFO (and RESET the data as it passes by):
	i2cset -y 1 0x68 0x6A 0x44
	# We will activate the copy of the accelerometer data into the FIFO:
	i2cset -y 1 0x68 0x23 0x08 # To get the data from the accelerometer
	i2cset -y 1 0x68 0x23 0x80 # To get the temperature data instead
read-fifo-set:
	i2cget -y 1 0x68 0x72 w # read two registers at once
read-fifo:
	i2cget -y 1 0x68 0x74
read:
	i2cget -y 1 0x68 0x3F w

