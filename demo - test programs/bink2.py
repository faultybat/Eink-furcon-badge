from machine import Pin
led1 = Pin(28, Pin.OUT)
led2 = Pin(27, Pin.OUT)
led3 = Pin(26, Pin.OUT)
led4 = Pin(22, Pin.OUT)

led1.toggle()
led2.toggle()
led3.toggle()
led4.toggle()
