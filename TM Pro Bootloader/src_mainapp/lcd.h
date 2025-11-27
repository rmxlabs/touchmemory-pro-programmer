#ifndef __LCD_H
#define __LCD_H


#define LCD_2L4B	0x28  // define 2 line of LCD, 4-bit interface, font 5x8
#define LCD_1L4B	0x20  // define 1 line of LCD, 4-bit interface, font 5x8
#define LCD_RESET	0x01  // reset of LCD
#define LCD_RETURN	0x02  // return of start position LCD
#define LCD_INCR	0x06  // increment mode,no shift display
#define LCD_ON		0x0C  // display - on, cursor - off, blinking - off
#define LCD_ONB		0x0D  // display - on, cursor - off, blinking - on
#define LCD_ONC		0x0E  // display - on, cursor - on, blinking - off
#define LCD_ONCB	0x0F  // display - on, cursor - on, blinking - on
#define LCD_OFF		0x08  // display - off, cursor - off, blinking - off

#define LCD_LEFT	0x10  // shift cursor left
#define LCD_RIGHT	0x14  // shift cursor right
#define LCD_LINE1	0x80  // set line_1 LCD с начала
#define LCD_LINE2	0xC0  // set line_2 LCD с начала


extern char LCD_buffer[40];

void InitLCD(void);
void LCD_Command(unsigned char cc);
void LCD_Data(unsigned char cc);
void LCD_setCOMMAND(unsigned char cc);

void LCD_POS(unsigned char pos);
void LCD_ROM(const rom char *s);
void LCD_RAM(char *s);

void LCD_CLR(void);
void LCD_HEX(unsigned char data, unsigned char pos);

void Dec_Hex(unsigned char c);
void LCD_code(unsigned char mode);
void LCD_Error(unsigned char err);
void LCD_Display(rom char *text);

#endif
