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

VPP make by voltage step up converter MAX662A. This chip out 12V and 20V. 
Write VPP make LM317 from 20V. 
