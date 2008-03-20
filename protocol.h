/*
 * GAINER I/O module command list    
 * version: 1.0.0.15   
 * date: 2006.5.27   
 * original: io-module_protocol.xls
 *
 * format:
 *   command result  comments
 */

/*
 * Configuration   
 */

KONFIGURATION_n KONFIGURATION_n*  n: configuration (1~8)

/*
 * I/O   
 */
Dxxxx Dxxxx*  set all digital outputs, xx: value
Hn  Hn* set specified digital output high, n: port number
Ln  Ln* set specified digital output low, n: port number
R Rxxxx*  get all digital inputs, xx: value
r rxxxx*rxxxx*…  get all digital inputs (continuous mode), xx: value
E E*  exit continuous mode

Tx  Tx* set sensitivity for capacitive sensing inputs

Axx…xx A*  set all analog outputs, xx: value
anxx  anxx* set specified analog output, n: port number, xx: value (00~FF, 8bit)
anx…x  a*  set specified analog output, n: row number, x: value (0~F, 4bit)
I Ixx…xx*  get all analog inputs, xx: value
i ixx…xx*  get all analog inputs (continuous mode), xx: value
E E*  exit continuous mode

Sx  Sxx*  get specified analog input (continuous mode), xx: value
E E*  exit continuous mode

Mx  Mx* ain sampling mode, x:mode (0: scan all channles, 1: scan ain 0/4 only)

/*
 * Button/LED    
 */
(button pressed)  N*  the on-board button is pressed (no need to send a get command)
(button released) F*  the on-board button is released (no need to send a get command)
h h*  turn the on-board LED on
l l*  turn the on-board LED off

/*
 * PGA   
 */
Gxy Gxy*  set PGA gain and PGA reference, x: gain, y: reference

/*
 * Misc.   
 */
Q Q*  reboot
Vx  Vx* verbose mode, x: mode (0: disabled, 1: enabled)
? ?1.0.0.xx*  version number, xx: build number

/*
  General Notes   
  all values are in hex and should be in upper case (e.g. 0A, F8...)    

  Configurations    
  configuration ain/din/aout/dout comments
  0  0/ 0/ 0/ 0 initial configuration just after rebooting
  1  4/ 4/ 4/ 4 default configuration
  2  8/ 0/ 4/ 4 
  3  4/ 4/ 8/ 0 
  4  8/ 0/ 8/ 0 
  5  0/16/ 0/ 0 
  6  0/ 0/ 0/16 
  7  0/ 8/ 8/ 0 matrix LED control mode
  8  0/ 8/ 0/ 8 capacitive sensing switch mode (first 4 ports only)
*/
