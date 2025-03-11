# USART (Universal Synchronous/Asyncronous Receiver/Transceiver)
Sometimes called SCI (Serial Communication Interface).

Synchronous operation uses a clock and data line while there is no separate clock accompanying the data for Asynchronous transmission.

Since there is no clock signal in asynchronous operation, one pin can be used for transmission and another pin can be used for reception.

Both transmission and reception ca occur at the same time, this know as full duplex operation.

However, when the serial port is enabled, the USART will control both pins and one cannot be used for general purpose I/O when the other is being used for transmission or reception.

The most common use of USAR is RS-232 protocol.

## RS-232 Protocol.
Is a serial transmission method of transferring data across a single wire, data is only transmitted in one direction for each wire so for bi-directional communication you need two wires

RS-232 is asynchronous protocol as there is no clock transmitted at all betwwen the receiver and transmitter.

### Baudrate.
The baudrate is simply the transmission speed measured in bits per second. It defines the frequency of each bit period.

### Receiver threshold voltage levels.
At the receiver the input the minimum voltage levels are defined as ±3V and can reach up to ±25V. i.e.:

> To receive a logic 0 the voltage must be greater than 3V.

> To receive a logic 1 the voltage must be smaller than -3V.

> This allows for losses as the signal travels down the cable and provides noise immunity i.e. any spurious noise up to a level of ±3V can be tolerated without it having any effect on the receiver and the data.

### Start bit.
At the beginning of each transmission a **start bit** is transmitted indicating to the receiver that a byte of data is about to follow.

The start bit lets hte receiver synchronize to the data bits since it can see the rising edge of the signal on the line.

Once the start bit is found, the receiver knows where the following bits will be as it is given the sample period (derived from the baudrate) as part of the initialization process.

This is why you must set the same settings in both devices under communication. i.e. baud rate, num-
ber of stop bits, number of data bits, and parity bit (on or off).

### Data bits.
Data bits follow the start bit. There will usually be seven or eight data bits with the LSB (least significant bit) transmitted first.

The reason you can choose betwwen seven or eight is that ASCII is made up of the alphabet within the first seven bits (as well the control characters).

The eight bit extends the character set for graphical symbols.

Other data bit sizes are 5,6,8, and 9 bits. However, bit length is usually set to 8 bits - this is very commonly used.

### Parity bit.
This is an error detection mechanism. You can used odd parity or even parity or none at all.

At the receiver the parity bit is used to tell if an error occurred during transmission.

#### Even parity.
The bit added to the data is positioned in such a way thta the numbers of states 1 is even on the given set + parity bit.

#### Odd parity.
The bit added to the data is positioned in such a way thta the number of states 1 is odd on the gieven set + parity bit.

### Stop bit.
It gives a period of time before the next start bit can be transmitted. It is the opposite sense to the start bit and because of this allows the start bit to be ssen then.

If there was no stop bit then the last bit in the data stream would be the parity bit (or data bit if parity is not active).

The stop bit can be set choosing from 1, 1.5, or 2- bit periods.

## Baremetal USART

For this practice USART6, PA11(TX), PA12(RX), will be used.

USART6 is at APB2 bus.

Pins PA11 and PA12 are at GPIOA

GPIOA is at AHB1.

Configuration: 8 bits data, no parity and 1 stop bit.

## Building.

main.c

    arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -std=gnu11 main.c -o main.o
    
stm32f401_startup.c

    arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -std=gnu11 stm32f401_startup.c -o stm32f401_startup.o

Linking.

    arm-none-eabi-gcc -nostdlib -T stm32f401_ls.ld *.o -o stm32f401 -Wl,-Map=stm32f401.map

### Clean up command

    rm *.o *.map stm32f401