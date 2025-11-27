
#include "P18cXXX.INC"

	radix dec

#define			p_in			PORTA,3
#define			p_out			LATC,4

				EXTERN	DelayCounter1, DelayCounter2
        		EXTERN 	tm_buff, wr_buff
        		EXTERN 	temp1, temp2, temp3, temp4, temp5, temp6
        		EXTERN 	temp7, temp8, temp9, temp10, temp11

TM1990			CODE	

Detect_TM:
				GLOBAL	Detect_TM

                rcall	DSReset
				bz		ds_err
                movlw   0x3C
                rcall   byte_snd
                movlw   0x00
                rcall   byte_snd
                movlw   0x00
                rcall   byte_snd
                movlw   0x55
                rcall   byte_snd
                rcall   byte_rcv
				movwf	temp1
                bsf     p_out
                movlw   d'200'
                rcall	delay2
                bcf     p_out
				movf	temp1,w
				sublw	0xBE
				btfss	STATUS,Z
				retlw	0x02
				retlw	0x00


TM2004Write:
				GLOBAL  TM2004Write

                rcall	DSReset
				bz		ds_err
                movlw   0x3C
                rcall	byte_snd
                movlw   0x00
                rcall	byte_snd
                movlw   0x00
                rcall	byte_snd
                movlw	d'8'
                movwf	temp3
                lfsr	FSR0,wr_buff
w_loop2:
                movf	INDF0,w
                rcall	byte_snd
                rcall	byte_rcv
                movlw	82h
                rcall	delay2
                bsf     p_out
                movlw	01h
                rcall	delay2
                bcf     p_out
                movlw 	15h
                rcall	delay
                rcall   byte_rcv
                incf    FSR0L,f
                decfsz  temp3,f
                bra     w_loop2
				retlw	0x00
Detect_IZ:
				GLOBAL	Detect_IZ

				bcf		temp10,0
				rcall	IZ_W_F
				sublw	0x01
				btfsc	STATUS,Z
				retlw	0x01
				rcall	IZ_RF
				btfsc	WREG,0
				retlw	0x01
				btfsc	WREG,7
				retlw	0x02
				bsf		temp10,0
				rcall	IZ_W_F
				sublw	0x01
				btfsc	STATUS,Z
				retlw	0x01
				rcall	IZ_RF
				btfsc	WREG,0
				retlw	0x01
				btfss	WREG,7
				retlw	0x02
				retlw	0x00
IZ_Write:
				GLOBAL  IZ_Write
                lfsr    FSR0, wr_buff
                call    DSReset
				bz		ds_err
                movlw   0xD5
                call    byte_snd
				movlw	0x08
				movwf	temp7
izs_loop1:
				movlw	0x08
				movwf	temp8
				movf	POSTINC0,w
				movwf	temp9
izs_loop2:
				btfss	temp9,0			
				call	send_one
				btfsc	temp9,0			
				call	send_zero
				rcall	power
				rrcf	temp9,f				
				decfsz	temp8,f
				bra		izs_loop2
				decfsz	temp7,f
				bra		izs_loop1
				retlw	0x00
ds_err:
				retlw	0x01

IZ_RF:
				GLOBAL  IZ_RF
                call    DSReset
				bz		ds_err
                movlw   0xB5
                call    byte_snd
                call    byte_rcv
				rrncf	WREG,w
				andlw	0x80
                return

IZ_WF:
				GLOBAL  IZ_WF
	    	    movlw   -1
    	    	movf    PLUSW1,w
				andlw	0x01
				movwf	temp10
IZ_W_F:
				clrf	temp11
                call    DSReset
				bz		ds_err
		        movlw   0xC1
				btfss	temp11,0
		        movlw   0xD1
                call    byte_snd
				btfss	temp10,0			
				call	send_one
				btfsc	temp10,0			
				call	send_zero
				rcall	power
				retlw	0x00

RW_Write:
				GLOBAL  RW_Write
                lfsr    FSR0, wr_buff
                call    DSReset
				bz		ds_err
                movlw   0xD5
                call    byte_snd
				movlw	0x08
				movwf	temp7
rws_loop1:
				movlw	0x08
				movwf	temp8
				movf	POSTINC0,w
				movwf	temp9
				comf	temp9,f
rws_loop2:
				btfss	temp9,0			
				call	send_one
				btfsc	temp9,0			
				call	send_zero
				rcall	power
				rrcf	temp9,f				
				decfsz	temp8,f
				bra		rws_loop2
				decfsz	temp7,f
				bra		rws_loop1
				retlw	0x00

RW_WF:
				GLOBAL  RW_WF
	    	    movlw   -1
    	    	movf    PLUSW1,w
				movwf	temp10
                call    DSReset
				bz		ds_err
                movlw   0x1D
                call    byte_snd
				btfsc	temp10,0			
				call	send_one
				btfss	temp10,0			
				call	send_zero
				call	power
				retlw	0x00

power:
				rcall	DelayDS
				rcall	DelayDS
;				rcall	DelayDS
				return

DelayDS:
                movlw   d'255'
                call    delay2
                movlw   d'255'
                call    delay2
                movlw   d'255'
                call    delay2
                movlw   d'255'
                call    delay2
                movlw   d'255'
                call    delay2
				return

TmReset:
                GLOBAL  TmReset
res_loop1:
                bcf     p_out
                movlw   d'240'
                rcall	delay2
                bsf     p_out
                movlw   d'150'
                rcall	delay2
                bcf     p_out
                movlw   d'15'
                rcall	delay2
                btfsc   p_in
                bra     _no_data
                movlw   d'60'
                rcall	delay2
				btfss   p_in
				bra     _no_data
                movlw   d'75'
                rcall	delay2
                retlw   0x00
_no_data:
                movlw   d'75'
                rcall	delay2
				retlw	0x01
DSReset:
				movlw	d'100'
				movwf	temp3
dr_loop:
                rcall	TmReset
				iorlw	0x00
				bz		dr_exit
				decfsz	temp3,f
				bra		dr_loop
				bsf		STATUS,Z
				retlw	0x01
dr_exit:
				bcf		STATUS,Z
				retlw	0x00

Tm_Read:
                GLOBAL  Tm_Read
                movlw   0x33
                rcall   byte_snd
                lfsr    FSR0,tm_buff
                movlw   d'8'
                movwf   temp3
tr:
                rcall   byte_rcv
                movwf   INDF0
                incf    FSR0L,f
                decfsz  temp3,f
                bra     tr
                return


CheckCRC:
                GLOBAL  CheckCRC
				rcall   CalcCRCTM
				xorwf   tm_buff+7,w
				bnz     err
				retlw   0x00
err:
				retlw   0x01


CalcCRCTM:
                lfsr    FSR0,tm_buff
                clrf    temp4
                movlw   d'7'
                movwf   temp1
l1:
                movlw   0x08
                movwf   temp2
                movf    INDF0,w
                movwf   temp3
l2:
                xorwf   temp4,w
                movwf   temp5
                rrcf    temp5,w
                movf    temp4,w
                btfsc   STATUS,C
                xorlw   b'00011000'
                movwf   temp5
                rrcf    temp5,w
                movwf   temp4
                bcf     STATUS,C
                rrcf    temp3,f
                movf    temp3,w
                decfsz  temp2,f
                bra     l2
                incf    FSR0L,f
                decfsz  temp1,f
                bra     l1
                movf    temp4,w
                return

send_bit:
				btfss	WREG,0
				bra		send_zero
send_one:
                nop
                bsf     p_out
                nop
                rcall   delay1
                nop
                bcf     p_out
                nop
                movlw   d'13'
                rcall   delay2
				bra		send_oz_exit
send_zero:
                nop
                bsf     p_out
                nop
                movlw   d'13'
                rcall   delay2
                nop
                bcf     p_out
                nop
                rcall   delay1
				bra		$+2
send_oz_exit:
				bra		$+2
				bra		$+2
				bra		$+2
				bra		$+2
				bra		$+2
				bra		$+2
				return


byte_snd:
                movwf   temp1
                movlw   d'8'
                movwf   temp2
next_tr_b:
                rrcf    temp1,f
                btfss   STATUS,C
                bra     _set_in
                nop
                bsf     p_out
                nop
                rcall   delay1
                nop
 	            bcf     p_out
                nop
                movlw   d'13'
                rcall   delay2
                bra     _skip_set_in
_set_in:
                nop
                bsf     p_out
                nop
                movlw   d'13'
                rcall   delay2
                nop
                bcf     p_out
                nop
                rcall   delay1
                rcall   delay1
                rcall   delay1
_skip_set_in:
                decfsz  temp2,f
                bra     next_tr_b
                return

receive_bit:
                movlw   d'1'
                movwf   temp1
                clrf    temp2
				rcall	next_rc_b
				rlncf	temp2,w
				return				
byte_rcv:
                GLOBAL  byte_rcv
                movlw   d'8'
                movwf   temp1
                clrf    temp2
next_rc_b:
                nop
                bsf     p_out
                nop
                rcall   delay1
                nop
                bcf     p_out
                nop
                rcall   delay1
                bcf     STATUS,C
                btfsc   p_in
                bsf     STATUS,C
                rrcf    temp2,f
                movlw   d'9'
                rcall   delay2
                decfsz  temp1,f
                bra     next_rc_b
                movf    temp2,w
                return

delay1:
				call	delay12
				call	delay12
				goto	delay12
delay12:
				return						

delay2:
                movwf   DelayCounter1
l4dl:  			call	x4delay
                decfsz  DelayCounter1,f
                bra     l4dl
                return

x4delay:
                nop
				goto	x4delay1
x4delay1:
				return

delay:
                movwf   DelayCounter2
delay_loop:
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                rcall   delay2
                decfsz  DelayCounter2,f
                bra     delay_loop
                return


                END
