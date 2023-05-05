#!/usr/bin/python3

#read SPI data from MCP3888 (or MCP3204) chip, 8 possible adc's (0 thru 7) 
import Jetson.GPIO as GPIO 
import time
def readadc(adcnum, clockpin, mosipin, misopin, cspin): 
    if ((adcnum > 7) or (adcnum < 0)): 
        return -1
    GPIO.output (cspin, True)
    GPIO.output(clockpin, False) # start clock low 
    GPIO.output(cspin, False) # bring CS low
    commandout=adcnum
    commandout |= 0x18 #start bit single-ended bit
    commandout <<= 3 #we only need to send 5 bits here
    for i in range(5): 
        if (commandout & 0x80):
            GPIO.output(mosipin, True)
        else:
            GPIO.output(mosipin, False)
        commandout <<= 1
        GPIO.output(clockpin, True) 
        GPIO.output(clockpin, False)

    adcout=0
#read in one empty bit, one null bit and 10 ADC bits
    for i in range(12): 
        GPIO.output(clockpin, True)
        GPIO.output(clockpin, False) 
        adcout <<= 1
        if (GPIO.input (misopin)): 
            adcout|=0x1

    GPIO.output(cspin, True)
    adcout >>= 1 
    return adcout



#change these as desired they're the pins connected from the SPI port on the ADC to the Cobbler
SPICLK=11 
SPIMISO=9
SPIMOSI=10
SPICS=8
output_pin=7
#photoresistor connected to adc #e
photo_ch=0

#port init
def init():
#GPIO BOARD GPIO.BCH GPIO.CVM GPIO. TEGRA SOC
    GPIO.setmode(GPIO.BOARD)
    # GPIO.setup(output_pin, GPIO.OUT, initial=GPIO.HIGH) 
    GPIO.setwarnings(False)
    GPIO.cleanup() #clean up at the end of your script
    GPIO.setmode(GPIO.BCM) #to specify whilch pin numbering system
    #set up the SPI interface pins
    GPIO.setup(SPIMOSI, GPIO.OUT) 
    GPIO.setup(SPIMISO, GPIO.IN)
    GPIO.setup(SPICLK, GPIO.OUT)
    GPIO.setup(SPICS, GPIO.OUT)

    GPIO.setup(21, GPIO.OUT, initial=GPIO.HIGH)


def main():
    init()
    while True:
        adc_value=readadc(photo_ch, SPICLK, SPIMOSI, SPIMISO, SPICS)
        print(adc_value, end="")
        GPIO.output(21, adc_value > 400)
        time.sleep(1)
        break
if __name__=='__main__':
    #try:
        main()
