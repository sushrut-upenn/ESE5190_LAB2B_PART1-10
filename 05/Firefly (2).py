import time
import neopixel
import board
import busio
import adafruit_apds9960.apds9960

i2c = board.STEMMA_I2C()
sensor = adafruit_apds9960.apds9960.APDS9960(i2c)
sensor.enable_color = True
pixel = neopixel.NeoPixel(board.NEOPIXEL, 1)

while True:
    r, g, b, c = sensor.color_data
    print('Red: {0}, Green: {1}, Blue: {2}, Clear: {3}'.format(r, g, b, c))
    if (c >= 5500):
        pixel.fill((255, 0, 0))
        time.sleep(0.125)
        pixel.fill((0, 0, 0))
        time.sleep(0.125)
