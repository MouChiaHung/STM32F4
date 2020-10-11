# STM32F4
Work with WINUSB as an USB CDC Device.

![image](https://github.com/MouChiaHung/STM32F4/blob/master/UniveralSerialDevicesNode.PNG)

USB can transmit loop back message which is more than 64 bytes (firstly sent to ENDPOINT as DATAOUT then transmitted to HOST as DATAIN).

![image](https://github.com/MouChiaHung/STM32F4/blob/master/stm32F4_usb_more_than_64B.PNG)


Reference:

1.This book "The Definitive Guide to ARM Cortex-M0 and Cortex-M0+ Processors" can offer general cortex knowledge.

https://www.sciencedirect.com/book/9780128032770/the-definitive-guide-to-arm-cortex-m0-and-cortex-m0-and-processors

2.This youtube video is an official STM32 tutorial video explaining how to have a skeleton of this project using Keil and CobeMX. 

https://www.youtube.com/watch?v=h9T0RTu9Muc&list=PLnMKNibPkDnFFRBVD206EfnnHhQZI4Hxa&index=9&ab_channel=STMicroelectronics

3.This blog is an independent developer explaining how to modify your Keil project and in order to use winusb.sys as your STM32 board's device driver, he uses STMStudio and I read it and port it into my Keil project.

https://damogranlabs.com/2018/02/stm32-usb-cdc/

4.Microsoft doc explains how to use their WINUSB API to have applications communicate with an "winusb" device (a device recognized by WINDOWS as a device using winusb.sys as your STM32) board's device driver).

https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/winusb

https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/using-winusb-api-to-communicate-with-a-usb-device
