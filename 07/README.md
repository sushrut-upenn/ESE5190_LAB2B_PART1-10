### TODO:  PART 7

In Part 7, we modified our sequencer code to use PIO as its primary I/O engine. When we press the boot button of QtPy, it captures the data which we have shown using both serial terminal Putty and PySerial. We used the code from part 6 of logic analyser and integrated it with our sequencer code from part 3 to implement PioSequencer. We record boot button presses on QtPy which are then played on neopixel while simultaneously capturing the data using the logic analyser. The capture on the serial terminal Putty and Pyserial is as shown below.


Putty: 

![Capture 1](https://user-images.githubusercontent.com/114092860/202813138-f475f024-55ba-492e-ae6d-48a402cf3117.png)


![Capture 2](https://user-images.githubusercontent.com/114092860/202813153-9e9d5a78-c384-4183-9ff6-5f6a7403d70f.png)


PySerial:


![Capture using Pyserial](https://user-images.githubusercontent.com/114092860/202813300-b4ef74c8-8299-438f-97b2-36e6a4f77f84.png)


GIF : 


![Piosequencer](https://user-images.githubusercontent.com/114092860/202813366-11f14172-84ef-4022-8b66-3cc4b84d8c62.gif)



