### TODO: PROTOBOARD - Custom circuit board to sequencer



In Part 10, we have implemented UPenn’s ‘P’ logo on a protoboard.The block diagram of our custom circuit board is as shown below:

![Upenn-block](https://user-images.githubusercontent.com/114092860/202352992-9d8d7c74-bbbe-46e9-beba-cfc1d8cb06f0.png)



 Below are the images of how the protoboard looks when it is on and off.


![Protoboard-1](https://user-images.githubusercontent.com/114092860/202746505-7d26eb4a-f1bb-4fa4-871e-3b8244561ecb.jpeg)


![Protoboard2](https://user-images.githubusercontent.com/114092860/202746522-9ae16749-7e77-4dd5-a047-15e0e3dfaaea.jpeg)


We have added custom support for this protoboard using our sequencer implementation. We recorded a sequence  using the boot button and then we gave the commands Play and Loop.

During the Play command, when the sequence played on our neopixel, it simultaneously played on our custom circuit board as seen in the Play video.


https://user-images.githubusercontent.com/114092860/202746644-8e78a444-aafd-4f62-be7b-9e000fc5ffc9.mp4



During the Loop command, when the sequence looped on our neopixel, it simultaneously looped on our custom circuit board as seen in the Loop video. 


https://user-images.githubusercontent.com/114092860/202746696-d1845985-258e-44cb-8432-502b10534af3.mp4

