## MSP432P401R Integration with UltraSonic Sensor and UART

A program which calculates distance from an obstacle using HC-SR04 and displays the distance serially with UART

***
### Technical Specs
    * Embedded-C
    * Code Composer Studio
    * MSP432P401R
    * UART
    * Putty
    * Ultrasonic Sensor ( HC - SR04 )
    * Timers
    * Interrupts
    * Register Level Code
  
### Prerequisites
* C language
* Reading MSP432P401R <a href="http://www.ti.com/lit/ds/symlink/msp432p401r.pdf">DataSheet</a>
and <a href="https://www.ti.com/lit/ug/slau356i/slau356i.pdf">Technical Reference Manual</a>
* Working knowledge of Timers, Interrupts and Interrupt Service Routine

***   
### Description:

* Ultrasonic Sensor has 4 pins. Vcc, GND, Trigger and Echo pins.
The sensor gets triggered by supplying 10 microsecond wide pulse. The sensor generates a pulse from the echo pin with a period proportional to the distance of the obstacle from sensor.
* Echo pin from the sensor is connected to a voltage divider which reduces it to 3.3V, tolerable by the MSP, to Port 2 Pin 7. 
* Trigger pin of sensor is connected to Port2 Pin 6. 
* MSP432 board is connected to a computer

## Important Note

1) Remember to supply 5V for HC-SR04 
   
2) MSP432 is not tolerant to 5V input. Use voltage divider ( max voltage => 3.7V ) 
   
3) Range of HC-SR04 is 4cm to 200cm

4) Check Device Manager in Windows to find the COM port of MSP432. You'll need to use this information while connecting to a serial console like **Putty** 
   
***
### **Execution** 



#### Feedback

Leave any suggestions or issues in the code by opening an issue page
***