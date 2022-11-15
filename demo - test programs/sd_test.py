'''
 # Demonstrates RPi Pico interface to MicroSD Card Adapter
 # Create a text file and write running numbers.
 # Open text file, read and print the content on debug port
   
 * The Raspberry Pi Pico pin connections for MicroSD Card Adapter SPI
 
 # MicroSD Card Adapter Power Pins
 * MicroSD VCC pin to Pico VBUS
 * MicroSD  GND pin to Pico GND
 
 # MicroSD SPI Pins
 * MicroSD MISO pin to Pico GPIO-12
 * MicroSD MOSI pin to Pico GPIO-11
 * MicroSD SCK pin to Pico GPIO-10
 * MicroSD CS pin to Pico GPIO-13
 
 Name:- M.Pugazhendi
 Date:-  28thJul2021
 Version:- V0.1
 e-mail:- muthuswamy.pugazhendi@gmail.com
'''

from machine import Pin, SPI
import sdcard
import os
 
#Initialize the onboard LED as output
led = machine.Pin(25,machine.Pin.OUT)

# Toggle LED functionality
def BlinkLED(timer_one):
    led.toggle()

# Initialize the SD card
spi=SPI(0,baudrate=40000000,sck=Pin(18),mosi=Pin(19),miso=Pin(16))
sd=sdcard.SDCard(spi,Pin(5))

# Create a instance of MicroPython Unix-like Virtual File System (VFS),
vfs=os.VfsFat(sd)
 
# Mount the SD card
os.mount(sd,'/sd')

# Debug print SD card directory and files
print(os.listdir('/sd'))

# Create / Open a file in write mode.
# Write mode creates a new file.
# If  already file exists. Then, it overwrites the file.
file = open("/sd/sample.txt","w")

# Write sample text
for i in range(20):
    file.write("Sample text = %s\r\n" % i)
    
# Close the file
file.close()

# Again, open the file in "append mode" for appending a line
file = open("/sd/sample.txt","a")
file.write("Appended Sample Text at the END \n")
file.close()

# Open the file in "read mode". 
# Read the file and print the text on debug port.
file = open("/sd/sample.txt", "r")
if file != 0:
    print("Reading from SD card")
    read_data = file.read()
    print (read_data)
file.close()

# Initialize timer_one. Used for toggling the on board LED
timer_one = machine.Timer()
# Timer one initialization for on board blinking LED at 200mS interval
timer_one.init(freq=5, mode=machine.Timer.PERIODIC, callback=BlinkLED)