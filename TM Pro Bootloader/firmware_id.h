#ifndef FIRMWARE_ID_H
#define FIRMWARE_ID_H

// VERSION OF FIRMWARE -> MAJOR_VER.MINOR_VER 
// (for example 1.0 - major = 1, minor = 0)
#define MAJOR_VER		4
#define MINOR_VER		0

 typedef union
 {
    struct
    {
       unsigned int major_version;
       unsigned int minor_version;
    };
    unsigned char    bytes[8];
 } idreg_t;


extern idreg_t far rom idreg;


#endif // FIRMWARE_ID_H
