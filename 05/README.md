# ESE5190_LAB2B_PART 5

For this Lab the firefly code from the previous lab is used. For the firefly implementation the Qt Py RP2040 was interfaced with the APDS9960 sensor board using the I2C protocol. In the firefly code the led blinks depending on the light intensity and mimics a firefly.    
In this lab we had to generate the APDS9960 I2C traffic and display it on the oscilloscope. We can see I2C traffic by connecting the SDA and SCL pins of the sensor board to the oscilloscope. We have used the Agilent technologies MSO7034B oscilloscope. The oscilloscope is interfaced with a logic analyzer which is connected to the SDA and SCL pins. As the logic analyzer is used the mode of the oscilloscope is set to digital. The connections can be seen in the attached images. Select D1 and D0 on the oscilloscope and select SCL to D0 and SDA to D1 of the logic analyzer  and the ground pin to the ground probe of the logic analyzer. The oscilloscope output is shown in the attached images. We can see 4 waveforms. The green and yellow are the ones using a normal probe which appear to have noise. The red and the blue waves are the ones using the logic analyzer and appear to be clear. The oscilloscope has many inbuilt features to store the display images on the USC device or the PC. A usb device was used to save the oscilloscope display information. 


![Oscilloscope](https://user-images.githubusercontent.com/114092860/202834371-757151a8-62f1-40d2-93c6-9ea3565c95ca.jpg)

![Connections](https://user-images.githubusercontent.com/114092860/202834373-4635ab2e-b7c6-4d1b-ab06-1a2ba560c5ce.jpg)

![I2C image 1](https://user-images.githubusercontent.com/114092860/202834374-37a50dc0-472c-4454-950d-353dbd7a0daa.png)

![I2C image 2](https://user-images.githubusercontent.com/114092860/202834377-5d895429-ebe5-4db0-8ca5-960810225ceb.png)

![I2C image 3](https://user-images.githubusercontent.com/114092860/202834380-bf005a6b-6747-4210-9e2d-32b0ba4ca511.png)
