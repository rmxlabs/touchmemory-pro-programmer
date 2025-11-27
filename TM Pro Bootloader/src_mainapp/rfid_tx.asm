
#include "P18cXXX.INC"

	radix dec

#define	port_out	PSTRCON, 2

#define START_GAP	d'350'		; 200
#define WRITE_GAP	d'330'		; 180

#define DATA_0		d'115'		; 180 160
#define DATA_1		d'335'		; 400 380

				EXTERN	temp1, temp2, temp3, temp4, temp5
				EXTERN	TXBuff
				EXTERN	wr_buff

RFC				CODE	

DelayUs 		macro time
					movlw	high(0x10004-(time/2))
					movwf	TMR3H,0 				
					movlw	low(0x10004-(time/2))
					movwf	TMR3L,0 				
					bcf		PIR2,1,0 				
				 	btfss 	PIR2,1,0 				
					bra		$-2
				endm

TX_bit:
				btfsc	STATUS,Z
				bra		tx_0
				nop
				DelayUs	DATA_1
				bra		TX_WrGap
tx_0:
				DelayUs	DATA_0
				bra		TX_WrGap

TX_StGap:
				bcf		port_out
				DelayUs	START_GAP
				bsf		port_out
				return

TX_WrGap:
				bcf		port_out
				DelayUs	WRITE_GAP
				bsf		port_out
				return

WriteRF:
				GLOBAL	WriteRF
    			movlw   -1
    			movff   PLUSW1,temp1
				lfsr	FSR0,TXBuff
				movlw	d'4'
				movwf	temp5
				rcall	TX_StGap
				bra		$+2			; ?
				bra		$+2
				bcf		STATUS,Z
				rcall	TX_bit
				bra		$+2
				nop
				movf	temp1,w
				andlw	0x80
				rcall	TX_bit
				bra		$+2
				bra		$+2
				bsf		STATUS,Z
				rcall	TX_bit
				bra		$+2
				nop
t5wb_loop:
				movf	INDF0,w
				andlw	0x80
				rcall	TX_bit
				bra		$+2
				nop
				movf	INDF0,w
				andlw	0x40
				rcall	TX_bit
				bra		$+2
				nop
				movf	INDF0,w
				andlw	0x20
				rcall	TX_bit
				bra		$+2
				nop
				movf	INDF0,w
				andlw	0x10
				rcall	TX_bit
				bra		$+2
				nop
				movf	INDF0,w
				andlw	0x08
				rcall	TX_bit
				bra		$+2
				nop
				movf	INDF0,w
				andlw	0x04
				rcall	TX_bit
				bra		$+2
				nop
				movf	INDF0,w
				andlw	0x02
				rcall	TX_bit
				bra		$+2
				nop
				movf	POSTINC0,w
				andlw	0x01
				rcall	TX_bit
				decfsz	temp5,f
				bra		t5wb_loop
				nop
				movf	temp1,w
				andlw	b'00000100'
				rcall	TX_bit
				bra		$+2
				nop
				movf	temp1,w
				andlw	b'00000010'
				rcall	TX_bit
				bra		$+2
				nop
				movf	temp1,w
				andlw	b'00000001'
				bra		TX_bit


PackEM:
				GLOBAL	PackEM
				lfsr	FSR0,TXBuff
				movlw	b'11111111'
				movwf	POSTINC0
				movlw	b'10000000'
				movwf	POSTINC0
				movlw	b'00000000'
				movwf	POSTINC0
				movwf	POSTINC0
				movwf	POSTINC0
				movwf	POSTINC0
				movwf	POSTINC0
				movwf	POSTINC0
				lfsr	FSR0,TXBuff + 1
				movlw	0x80
				movwf	temp2
				clrf	temp3
				swapf	wr_buff+5,w
				rcall	EMbyte
				movf	wr_buff+5,w
				rcall	EMbyte
				swapf	wr_buff+4,w
				rcall	EMbyte
				movf	wr_buff+4,w
				rcall	EMbyte
				swapf	wr_buff+3,w
				rcall	EMbyte
				movf	wr_buff+3,w
				rcall	EMbyte
				swapf	wr_buff+2,w
				rcall	EMbyte
				movf	wr_buff+2,w
				rcall	EMbyte
				swapf	wr_buff+1,w
				rcall	EMbyte
				movf	wr_buff+1,w
				rcall	EMbyte
				movf	temp3,w
				rcall	EMbyte
				comf	temp2,w
				andwf	INDF0,f
				return

EMbyte:
				movwf	temp4
				rcall	em_next_bit
				clrf	temp1
				btfss	temp4,3
				bra		em4p_skip1
				btg		temp1,0
				btg		temp3,3
				movf	temp2,w
				iorwf	INDF0,f
em4p_skip1:
				rcall	em_next_bit
				btfss	temp4,2
				bra		em4p_skip2
				btg		temp1,0
				btg		temp3,2
				movf	temp2,w
				iorwf	INDF0,f
em4p_skip2:
				rcall	em_next_bit
				btfss	temp4,1
				bra		em4p_skip3
				btg		temp1,0
				btg		temp3,1
				movf	temp2,w
				iorwf	INDF0,f
em4p_skip3:
				rcall	em_next_bit
				btfss	temp4,0
				bra		em4p_skip4
				btg		temp1,0
				btg		temp3,0
				movf	temp2,w
				iorwf	INDF0,f
em4p_skip4:
				rcall	em_next_bit
				btfss	temp1,0
				bra		em4p_skip5
				movf	temp2,w
				iorwf	INDF0,f
em4p_skip5:
				return

em_next_bit:
				rrncf	temp2,f
				btfsc	temp2,7
				movf	POSTINC0,w
				return


				END

