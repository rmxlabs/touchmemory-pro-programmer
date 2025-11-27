 #include "firmware_id.h"

 #pragma romdata romdata_idreg=0x200000
 idreg_t far rom idreg =
 {
	{ 
		MAJOR_VER, // MAJOR VER
		MINOR_VER  // MINOR VER
	}
 };
 #pragma romdata

