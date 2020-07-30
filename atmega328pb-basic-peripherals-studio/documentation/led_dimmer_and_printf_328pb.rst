Introduction
============

This is the documentation for the LED dimmer and printf application.


Related documents / Application notes
-------------------------------------

This application:
1. Uses the ADC to measure an analog input voltage, typically from a potmeter
2. Uses the sampled value to control the PWM duty cycle of an I/O pin, typically connected to a LED
3. Uses printf() redirected to USART to prints status information
4. Uses the Timeout Driver to schedule the timing of these operations

Supported evaluation kit
------------------------

 - `Mega328PB-XplainedMINI <http://www.atmel.com/tools/mega328pb-xmini.aspx>`_

The user must manually connect:
- ADC Channel 0 (PC0) to analog input voltage source, such as potmeter
- PWM Channel 0 (PD6) to observable output, such as a LED


Interface settings
------------------

- USART for printf() over EDBG USART
	- No parity
	- 8-bit character size
	- 1 stop bit
	- 57600 baud-rate

Running the demo
----------------

1. Click Export Project and open the .atzip file in Studio
2. Build and flash into supported evaluation board
3. Open the EDBG COM port in a terminal window to observe the printf() output
4. Operate the analog input voltage source (e.g. potmeter) and observe the change in LED and printf output.
