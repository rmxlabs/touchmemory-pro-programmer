#include "P18cXXX.INC"

	radix dec


#define	p_in		PORTC,RC6
#define	bit_b		b'01000000'
#define MINVAL      (d'32' * d'50')
#define MAXVAL      (d'32' * d'70')
#define	BYTESMAX	d'18'


RX_ACS			UDATA_ACS

data_rx			RES		12	; 10 + 2bytes
tm_buff			RES		8	; 8 bytes
wr_buff			RES		8	; 8 bytes

temp1			RES		1
temp2			RES		1
temp3			RES		1
temp4			RES		1
temp5			RES		1
temp6			RES		1
temp7			RES		1
temp8			RES		1
temp9			RES		1
temp10			RES		1
temp11			RES		1	
TempL			RES		1
TempH			RES		1
DelayL			RES		1
DelayH			RES		1

        		GLOBAL 	tm_buff, wr_buff
        		GLOBAL 	temp1, temp2, temp3, temp4, temp5, temp6
        		GLOBAL 	temp7, temp8, temp9, temp10, temp11

RX				UDATA

id_array		RES		BYTESMAX + 1

saveFSR0L		RES		1
saveFSR0H		RES		1


RXC				CODE	
 
SCAN_RFID:
				GLOBAL	SCAN_RFID


				movlw	d'5'
				movwf	temp1
				movlw	d'8'
				movwf	temp4
				lfsr	FSR0,id_array
				rcall	RxSync
				bz		sr_error
				bra		sr_inp
sr_loop1:
				rcall	get_period
				bz		sr_error
sr_inp:
				rcall	wait_period
				decfsz	temp4,f
				bra		sr_loop1
				bsf		temp4,3
				decfsz	temp1,f
				bra		sr_loop1
				retlw	0x00
sr_error:
				retlw	0x01



ReciveRFID:
				GLOBAL	ReciveRFID

				movlw	BYTESMAX
				call	ReciveM
				btfsc	STATUS,Z
				retlw	0x01
				call	Decode_EM4100
				iorlw	0x00
				bz		rr_found
				call	not_buff
				call	Decode_EM4100
				iorlw	0x00
				bz		rr_found
				retlw	0x01
rr_found:
				retlw	0x00

ReciveM:
				movwf	temp1
				movlw	d'8'
				movwf	temp4
				lfsr	FSR0, id_array
				rcall	RxSync
				bz		rm_error
				bra		rm_inp
rm_loop1:
				rcall	get_period
				bz		rm_error
rm_inp:
				rcall	wait_period
				bcf		STATUS,C
				btfsc	p_in
				bsf		STATUS,C

				rlcf	INDF0,f

				decfsz	temp4,f
				bra		rm_loop1

				movf	POSTINC0,w
				bsf		temp4,3
				decfsz	temp1,f
				bra		rm_loop1
				bcf		STATUS,Z
				retlw	0x00
rm_error:
				bsf		STATUS,Z
				retlw	0x01


RxSync:
				movlw	d'130'
				movwf	temp2
				clrf	TMR1H
				clrf	TMR1L
				clrf	temp3
				btfsc	p_in
				decf	temp3,f
				clrf	TMR1H
				clrf	TMR1L
				bcf		PIR1, TMR1IF
				call	get_period
				call	get_period
				call	get_period
				bz		rs_error
rs_loop:
				movff	temp9, temp5
				movff	temp10, temp6
				dcfsnz	temp2,f
				bra		rs_error
				call	get_period
				bz		rs_error
				movff	temp9, temp7
				movff	temp10, temp8
				bcf		STATUS,C
				rrcf	temp8,f
				rrcf	temp7,f
				movf	temp9,w
				subwf	temp5,w
				movwf	TempL
				movf	temp10,w
				subwfb	temp6,w
				movwf	TempH
				bcf		temp11,0
				btfss	TempH,7
				bra		rs_plus
				bsf		temp11,0
				movf	TempL,w	
				sublw	0x00
				movwf	TempL
				movlw	0x00
				subfwb	TempH,f
rs_plus:
				movf	TempL,w
				subwf	temp7,w
				movf	TempH,w
				subwfb	temp8,w
				bc		rs_loop
				btfsc	temp11,0
				bra		rs_skip1
				movff	temp9, temp5
				movff	temp10, temp6
				call	get_period
				bz		rs_error
				movf	temp5,w
				addwf	temp9,f
				movf	temp6,w
				addwfc	temp10,f
rs_skip1:
				bcf		STATUS,C
				rrcf	temp10,f
				rrcf	temp9,f
				movff	temp10, DelayH
				movff	temp9, DelayL
;				movf	TMR1L,w
;				movlw	d'23'
;				btfsc	STATUS,Z
;				btg		PLUSW0,0
				bcf		STATUS,C
				rrcf	temp10,f
				rrcf	temp9,f
				bcf		STATUS,C
				rrcf	temp10,f
				rrcf	temp9,f
				movf	temp9,w
				addwf	DelayL,f
				movf	temp10,w
				addwfc	DelayH,f
				movf	DelayL,w
				sublw	0x00
				movwf	DelayL
				movlw	0x00
				subfwb	DelayH,f
				bcf		STATUS,Z
				return
rs_error:
				bsf		STATUS,Z
				return
get_period:
				movf	PORTC,w
				andlw	bit_b
				movwf	temp3
gp_loop1:
				btfsc	PIR1, TMR1IF
				bra		gp_error
				movf	temp3,w
				xorwf	PORTC,w
				andlw	bit_b
				btfsc	STATUS,Z
				bra		gp_loop1
				movff	TMR1L, temp9
				movff	TMR1H, temp10
				clrf	TMR1H
				clrf	TMR1L
				bcf		PIR1, TMR1IF
;				movlw	low(MINVAL)
;				subwf	temp9,w
;				movlw	high(MINVAL)
;				subwfb	temp10,w
;				bnc		gp_error
				movlw	low(MAXVAL)
				subwf	temp9,w
				movlw	high(MAXVAL)
				subwfb	temp10,w
				bc		gp_error
gp_ok:
				bcf		STATUS,Z
				return
gp_error:
				bsf		STATUS,Z
				return

wait_period:
				movff	DelayH, TMR3H
				movff	DelayL, TMR3L
				bcf		PIR2, TMR3IF
wa_loop1:
				btfss	PIR2, TMR3IF
				bra		wa_loop1
				return


Decode_EM4100:
				movlw	BYTESMAX
				movwf	temp1
em64_search_loop:
				movlw	d'8'
				movwf	temp4
em64_search_loop1:
				call	scroll
rm_next9:
				movff	id_array+0x00,WREG
				andlw	b'00000011'
				sublw	b'00000001'
				bnz		em64_next
				movff	id_array+1,WREG
				sublw	b'11111111'
				bz		em64_found
em64_next:
				decfsz	temp4,f
				bra		em64_search_loop1
				decfsz	temp1,f
				bra		em64_search_loop
				bra		em64_error
em64_found:
				clrf	temp3
				movlw	d'10'
				movlb	temp5
				movwf	temp5
				lfsr	FSR0,data_rx
em_64_loop1:
				movff	id_array+2,WREG
				call	HPC
				bnz		em64_next
				movlb	id_array+2
				btfsc	id_array+2,7,1
				btg		temp3,7
				btfsc	id_array+2,6,1
				btg		temp3,6
				btfsc	id_array+2,5,1
				btg		temp3,5
				btfsc	id_array+2,4,1
				btg		temp3,4
				movf	id_array+2,w,1
				rrncf	WREG,w
				rrncf	WREG,w
				rrncf	WREG,w
				rrncf	WREG,w
				andlw	b'00001111'
				movwf	POSTINC0
				call	scroll
				call	scroll
				call	scroll
				call	scroll
				call	scroll
				movlb	temp5
				decfsz	temp5,f
				bra		em_64_loop1
				movff	id_array+2,WREG
				xorwf	temp3,w
				andlw	b'11110000'
				bnz		em64_next
				movlb	data_rx+0
				swapf	data_rx+0,w
				movlb	data_rx+1
				iorwf	data_rx+1,w
				movwf	tm_buff+5
				movlb	data_rx+2
				swapf	data_rx+2,w
				movlb	data_rx+3
				iorwf	data_rx+3,w
				movwf	tm_buff+4
				movlb	data_rx+4
				swapf	data_rx+4,w
				movlb	data_rx+5
				iorwf	data_rx+5,w
				movwf	tm_buff+3
				movlb	data_rx+6
				swapf	data_rx+6,w
				movlb	data_rx+7
				iorwf	data_rx+7,w
				movwf	tm_buff+2
				movlb	data_rx+8
				swapf	data_rx+8,w
				movlb	data_rx+9
				iorwf	data_rx+9,w
				movwf	tm_buff+1
				retlw	0x00
em64_error:
				retlw	0x01


HPC:
				movlb	temp2
				movwf	temp6
				clrf	temp2
				btfsc	temp6,7
				incf	temp2,f
				btfsc	temp6,6
				incf	temp2,f
				btfsc	temp6,5
				incf	temp2,f
				btfsc	temp6,4
				incf	temp2,f
				movf	temp6,w
				rrcf	WREG,w
				rrcf	WREG,w
				rrcf	WREG,w
				xorwf	temp2,w
				andlw	0x01
				return

scroll:
				movff	FSR0L,saveFSR0L
				movff	FSR0H,saveFSR0H
				lfsr	FSR0,id_array + (BYTESMAX - 1)
				movlb	id_array
				rlcf	id_array,w,1
				movlw	BYTESMAX
rl_loop:
				rlcf	POSTDEC0,f
				decfsz	WREG,w
				bra		rl_loop
				movff	saveFSR0L,FSR0L
				movff	saveFSR0H,FSR0H
				return

not_buff:
				lfsr	FSR0,id_array
				movlw	BYTESMAX
cm_loop:
				comf	POSTINC0,f
				decfsz	WREG,w
				bra		cm_loop
				return


				END



