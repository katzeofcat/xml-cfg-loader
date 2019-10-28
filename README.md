# xml-cfg-loader
Load config from xml and send to embedded system via serial connection
How to USE Auto Test
What’s it for?
This console program is used for sending command to DT via rs232 connection and receive any reply from DT, you may specify the command to be send,
the sequence times that you want it to run as well as the debug mode and the indicator of the end of debug message.
Also, you can change the configuration for serial connection comport, baudrate, databits, parity, stopbit and flowcontrol.
Can I use it?
This can be used for all the command in DT. But for debug mode you must know which string is indicate that debug is finish.
How do I do it?
Connect DT 421 to your PC using USB A Male to B Male cable. Find out the comport from device manager 
Configuration
Create two files call “config.xml” and “command.xml” under the directory.
Note:  For config.xml you will need to specify comport, baudrate, databits, parity, stopbit and flowcontrol.
DT421 uses baud rate 115200, with no parity, 8 data bits and no flowcontrol. Comport can be found by looking at device manager.

 

For example:
<config>
	<port>COM3</port>  // from device manager
	<baudrate>115200</baudrate>    //
	<databits>8</databits>
	<parity>N</parity>
	<stopbit>1</stopbit>
	<flowcontrol>0</flowcontrol>
</config>
For flowcontrol, 0 indicate no flowcontrol and 1 is have flowcontrol.

For the Command that you want to send to the DT you need the following format:

For example:
<command>
	<sequence loop="1" name='test1'>
		<operation cmd="ETM TEST ' FY110660830'"/>
		<operation cmd="ETM INFO"/>
		<operation cmd="gps replay read" mode='d' eol="End of replay file"/>
		<operation cmd="ETM TEST 0"/>
		<operation cmd="ETM INFO"/>
	</sequence>
</command>

Which the loop indicates how many times you want that sequence under to run. For the name you can change to the name you prefer
For cmd is the command that need to send to DT and if you want to use “” inside the command you may replace it by using ‘’ 
Mode indicate which debug mode that you want to use in DT and eol is the end of line indicator that to stop the debug mode

IF you want more then one sequence you can add like:
<command>
	<sequence loop="1" name='test1'>
		<operation cmd="ETM TEST ' FY110660830'"/>
		<operation cmd="ETM INFO"/>
		<operation cmd="gps replay read" mode='d' eol="End of replay file"/>
		<operation cmd="ETM TEST 0"/>
		<operation cmd="ETM INFO"/>
	</sequence>
	<sequence loop="3" name='test2'>
		<operation cmd="ETM list"/>
		<operation cmd="SYS LIST"/>
	</sequence>
</command>
