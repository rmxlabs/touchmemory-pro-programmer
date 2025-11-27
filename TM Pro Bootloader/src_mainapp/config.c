

#pragma config CPUDIV = NOCLKDIV 	// No CPU System Clock divide  
//#pragma config CPUDIV = CLKDIV2	// CPU System Clock divided by 2  
//#pragma config CPUDIV = CLKDIV3	// CPU System Clock divided by 3  
//#pragma config CPUDIV = CLKDIV4	// CPU System Clock divided by 4  

//USB Clock Selection bit:
#pragma config  USBDIV = OFF		// USB Clock comes directly from the OSC1/OSC2 oscillator block; no divide  
//#pragma config USBDIV = ON		// USB clock comes from the OSC1/OSC2 divided by 2  

//#pragma config FOSC = LP			// LP
//#pragma config FOSC = XT			// XT
#pragma config FOSC = HS			// HS
//#pragma config FOSC = ERCCLKOUT	// External RC oscillator, CLKOUT function on OSC2  
//#pragma config FOSC = ECCLKOUTH	// EC, CLKOUT function on OSC2 (high)  
//#pragma config FOSC = ECH			// EC (high)  
//#pragma config FOSC = ERC			// External RC oscillator  
//#pragma config FOSC = IRC			// Internal RC oscillator  
//#pragma config FOSC = IRCCLKOUT	// Internal RC oscillator, CLKOUT function on OSC2  
//#pragma config FOSC = ECCLKOUTM	// EC, CLKOUT function on OSC2 (medium)  
//#pragma config FOSC = ECM			// EC (medium)  
//#pragma config FOSC = ECCLKOUTL	// EC, CLKOUT function on OSC2 (low)  
//#pragma config FOSC = ECL			// EC (low)  

//#pragma config PLLEN = OFF	//PLL is under software control  
#pragma config PLLEN = ON		//Oscillator multiplied by 4  

#pragma config PCLKEN = OFF		// Primary clock is under software control  
//#pragma config PCLKEN = ON	// Primary clock enabled  

#pragma config FCMEN = OFF		// Fail-Safe Clock Monitor disabled  
//#pragma config FCMEN = ON		// Fail-Safe Clock Monitor enabled  

//#pragma config IESO = OFF		// Oscillator Switchover mode disabled  
#pragma config IESO = ON		// Oscillator Switchover mode enabled  

#pragma config PWRTEN = ON		// Power-up Timer enabled  
//#pragma config PWRTEN = OFF	// Power-up Timer disabled  

//#pragma config BOREN = OFF		// Brown-out Reset disabled in hardware and software  
#pragma config BOREN = ON			// Brown-out Reset enabled and controlled by software (SBOREN is enabled)  
//#pragma config BOREN = NOSLP		// Brown-out Reset enabled in hardware only and disabled in Sleep mode (SBOREN is disabled)  
//#pragma config BOREN = SBORDIS	// Brown-out Reset enabled in hardware only (SBOREN is disabled)  

#pragma config BORV = 30	// VBOR set to 3.0 V nominal  
//#pragma config BORV = 27	// VBOR set to 2.7 V nominal  
//#pragma config BORV = 22	// VBOR set to 2.2 V nominal  
//#pragma config BORV = 19	// VBOR set to 1.9 V nominal  

#pragma config WDTEN = OFF	// WDT is controlled by SWDTEN bit of the WDTCON register  
//#pragma config WDTEN = ON	// WDT is always enabled. SWDTEN bit has no effect.  

//Watchdog Timer Postscale Select bits:
#pragma config WDTPS = 1
//WDTPS = 2
//WDTPS = 4
//WDTPS = 8
//WDTPS = 16
//WDTPS = 32
//WDTPS = 64
//WDTPS = 128
//WDTPS = 256
//WDTPS = 512
//WDTPS = 1024
//WDTPS = 2048
//WDTPS = 4096
//WDTPS = 8192
//WDTPS = 16384
//WDTPS = 32768

//MCLR Pin Enable bit:
#pragma config MCLRE = OFF		// RE3 input pin enabled; MCLR disabled  
//#pragma config MCLRE = ON		// MCLR pin enabled, RE3 input pin disabled  

//HFINTOSC Fast Start-up bit:
#pragma config HFOFST = OFF		// The system clock is held off until the HFINTOSC is stable.  
//#pragma config HFOFST = ON	// HFINTOSC starts clocking the CPU without waiting for the oscillator to stabilize.

//Stack Full/Underflow Reset Enable bit:
//#pragma config STVREN = OFF	// Stack full/underflow will not cause Reset  
#pragma config STVREN = ON		// Stack full/underflow will cause Reset  

//Single-Supply ICSP Enable bit:
#pragma config LVP = OFF	// Single-Supply ICSP disabled  
//#pragma config LVP = ON	// Single-Supply ICSP enabled  

//Boot Block Size Select Bit:
#pragma config BBSIZ = OFF	// 1kW boot block size  
//#pragma config BBSIZ = ON	// 2kW boot block size  

//Extended Instruction Set Enable bit:
#pragma config XINST = OFF
//#pragma config XINST = ON

#pragma config CP0 = ON		// Block 0 code-protected  
//#pragma config CP0 = OFF	// Block 0 not code-protected  

#pragma config CP1 = ON		// Block 1 code-protected  
//#pragma config CP1 = OFF	// Block 1 not code-protected  

#pragma config CPB = ON		// Boot block code-protected  
//#pragma config CPB = OFF	// Boot block not code-protected  

#pragma config CPD = ON		// Data EEPROM code-protected  
//#pragma config CPD = OFF	// Data EEPROM not code-protected  

#pragma config WRT0  = OFF	// Block 0 not write-protected  
#pragma config WRT1  = OFF	// Block 1 not write-protected  
#pragma config WRTB  = OFF	// Boot block not write-protected  
#pragma config WRTC  = OFF	// Configuration registers not write-protected  
#pragma config WRTD  = OFF	// Data EEPROM not write-protected  
#pragma config EBTR0 = OFF	// Block 0 not protected from table reads executed in other blocks  
#pragma config EBTR1 = OFF	// Block 1 not protected from table reads executed in other blocks  
#pragma config EBTRB = OFF	// Boot block not protected from table reads executed in other blocks  


