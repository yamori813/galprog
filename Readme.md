GAL Programer by FT2232 

You must use 5V FT2232. Because of GAL is 5V. 

Connection 

|FT2232|GAL|
|:--|:--|
|AD0|SDIN|
|AD1-AD6|RA0-RA5|
|AD7|SCLK|
|BD0|SDOUT|
|BD1|STB-|
|BD2|P/V-|
|BD3|VPP(EDIT) CTL|

All connect to FT2232 line is pullup 4.7K resistors excepting VPP CTL.
All other line pulldown 4.7K resistors excepting VCC and GND and CLK.
CLK is connect to GND.

VPP make by voltage step up converter MAX662A. This chip out 12V and 20V. 
Write VPP make LM317 from 20V. 

This project start on Mac OS X. Also you can build on Windows. 

If you want build on Windows you need set _CRT_SECURE_NO_WARNINGS. 
