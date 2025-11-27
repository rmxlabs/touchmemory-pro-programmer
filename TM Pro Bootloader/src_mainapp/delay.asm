#include <P18cXXX.INC>


				EXTERN	DelayCounter1, DelayCounter2

DELAY			CODE	

Delay1KTCYx:
    			GLOBAL  Delay1KTCYx
    			movlw   0xff
    			movf    PLUSW1,0
    			movwf   DelayCounter1
    			movlw   0x48
    			bra     D1K_1
D1Kx:
    			movlw   0x4c
D1K_1:
    			movwf   INDF1
    			decfsz  INDF1,1
    			bra     $-2

    			clrf    INDF1
    			decfsz  INDF1,1
    			bra     $-2

    			decfsz  DelayCounter1,1
    			bra     D1Kx
    			nop
    			return

Delay10KTCYx:
    			GLOBAL  Delay10KTCYx
    			movlw   0xff
    			movf    PLUSW1,0
			    movwf   DelayCounter1
    			movlw   0xef
			    bra     D10K_1
D10Kx:
    			movlw   0xf3
D10K_1: 
    			movwf   INDF1
    			decfsz  INDF1,1
    			goto    $-2

    			movlw   0x0c
			    movwf   DelayCounter2
D10Kl:
    			clrf    INDF1
    			decfsz  INDF1,1
    			goto    $-2

    			decfsz  DelayCounter2,1
    			goto    D10Kl

    			decfsz  DelayCounter1,1
    			goto    D10Kx
    			return

Delay10TCYx:
    			GLOBAL  Delay10TCYx
    			movlw   0xff
    			movf    PLUSW1,0
    			dcfsnz  WREG,1
    			return
D10x:
    			nop
    			bra     $+2
    			bra     $+2
    			bra     $+2
D10_1:
    			decfsz  WREG,1
    			bra     D10x
    			return

Delay100TCYx:
				GLOBAL  Delay100TCYx
				movlw   0xff
        		movf    PLUSW1,0
        		movwf   DelayCounter1
        		movlw   0x1b
				bra     $+2
        		bra     D100_1
D100x:
        		movlw   0x20
D100_1:
        		movwf   INDF1
        		decfsz  INDF1,1
        		bra     $-2
        		decfsz  DelayCounter1,1
        		bra     D100x
        		bra     $+2
        		return


                END
