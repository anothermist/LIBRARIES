/*
 * DigitalWriteFast for ChipKit
 * October 2011 by Bill Westfield (WestfW)
 *
 *
 * The Arduino environment provides functions digitalWrite(Pin, Val),
 * pinMode(Pin, Dir), and digitalRead(Pin) that are much slower than
 * they need to be if the arguments are constants.  Numerous discussions
 * and examples have taken place on how to implement faster functions,
 * by detecting the use of constants and producing more efficient inline
 * code (often, a single instruction on AVR.)
 *
 * This file does the same thing for the ChipKit board, which uses a PIC32
 * microcontroller (implementing the MIPS M4K architecture.)  Since this
 * environment also uses gcc, the code is very similar to several of the
 * examples created for the AVR Arduinos.  The major difference is that
 * this file uses keyboard macros (for EMACS) to derive the complex "if"
 * statement used to convert from "pin" to IOPort and bitmask operating
 * on the source code for pinToPort and pinToMask from the cpu32 "core"
 * pins_arduino_*.cxx files.  This should be less prone to error, though
 * it does produce rather long and nested macro definitions.  Note that
 * the editor macros don't do ALL the work; there is still a fair amount
 * of manual editing to do.  But the parts that are most (silently)
 * error-prone are essentially automated.
 *
 * Also, the PIC32 implements SET and CLR registers for each IO port that
 * prevent the need for read/modify/rewrite and consequent atomicity issues
 * that occur on the AVR.
 */

/*
 * EMACS keyboard macro for converting digital_pin_to_port_PGM table
 * in pins_arduino_pic32_uno.cxx into a macro definition.
 * Copy the table, edit it manually to remove blank lines, set
 * register "p" to 0 ( C-U 0 C-X r n p ) and then execute this macro
 * once for each pin.  Then add the #define at the top and enough
 * close parens at the end to make it work.  (If you don't understand
 * this, then you probably shouldn't try to modify these macros !)
 
Command: last-kbd-macro
Key: none

Macro:

2*SPC			;; self-insert-command
C-q (			;; 
C-q (			;;  "  ((P)=="
P			;; 
C-q )			;; 
==			;; 
C-x r g p		;; insert-register p
C-u 1 C-x r + p		;; increment-register p
M-f			;; forward-word past number we just added
C-q )			;; quoted-insert
C-q SPC			;;   "n) ? "
C-q ?			;; 
C-d			;; delete tab (hope its a tab!)
C-q SPC			;; quoted-insert
ESC f			;; forward-word over existing reg name
C-q SPC			;; " : ( \ "
C-q :			;; 
C-q SPC			;; 
C-q (			;; 
C-k			;; clear rest of line
C-q SPC			;; quoted-insert
C-q \			;; quoted-insert
C-n			;; next-line
C-a			;; move-beginning-of-line

*/


/* Macro to change lines containg "PB" to ones containin "&LATBSET"
 * make a couple copies of the new macro, so that they can be modified
 * to implement the necessary functions (set, clr, tris)

C-s			;; isearch-forward
?			;; self-insert-command
SPC			;; self-insert-command
P			;; self-insert-command
NUL			;; set-mark-command
DEL			;; c-electric-backspace
C-q &
LAT			;; self-insert-command * 3
C-f			;; forward-char
SET			;; self-insert-command * 3
C-n			;; next-line
C-a			;; move-beginning-of-line

*/

#if defined(_BOARD_UNO_) && !defined(__AVR__)

#define _dpin_to_set_macro(P) \
  ((P)==0) ? &LATFSET : ( \
  ((P)==1) ? &LATFSET : ( \
  ((P)==2) ? &LATDSET : ( \
  ((P)==3) ? &LATDSET : ( \
  ((P)==4) ? &LATFSET : ( \
  ((P)==5) ? &LATDSET : ( \
  ((P)==6) ? &LATDSET : ( \
  ((P)==7) ? &LATDSET : ( \
  ((P)==8) ? &LATDSET : ( \
  ((P)==9) ? &LATDSET : ( \
  ((P)==10) ? &LATDSET : ( \
  ((P)==11) ? &LATGSET : ( \
  ((P)==12) ? &LATGSET : ( \
  ((P)==13) ? &LATGSET : ( \
  ((P)==14) ? &LATBSET : ( \
  ((P)==15) ? &LATBSET : ( \
  ((P)==16) ? &LATBSET : ( \
  ((P)==17) ? &LATBSET : ( \
  ((P)==18) ? &LATBSET : ( \
  ((P)==19) ? &LATBSET : ( \
  ((P)==20) ? &LATBSET : ( \
  ((P)==21) ? &LATBSET : ( \
  ((P)==22) ? &LATBSET : ( \
  ((P)==23) ? &LATBSET : ( \
  ((P)==24) ? &LATBSET : ( \
  ((P)==25) ? &LATBSET : ( \
  ((P)==26) ? &LATESET : ( \
  ((P)==27) ? &LATESET : ( \
  ((P)==28) ? &LATESET : ( \
  ((P)==29) ? &LATESET : ( \
  ((P)==30) ? &LATESET : ( \
  ((P)==31) ? &LATESET : ( \
  ((P)==32) ? &LATESET : ( \
  ((P)==33) ? &LATESET : ( \
  ((P)==34) ? &LATDSET : ( \
  ((P)==35) ? &LATDSET : ( \
  ((P)==36) ? &LATDSET : ( \
  ((P)==37) ? &LATDSET : ( \
  ((P)==38) ? &LATFSET : ( \
  ((P)==39) ? &LATFSET : ( \
  ((P)==40) ? &LATFSET : ( \
  ((P)==41) ? &LATBSET : ( \
  ((P)==42) ? &LATBSET : ( \
  ((P)==43) ? &LATFSET : 0 )))))))))))))))))))))))))))))))))))))))))))

/* Macro to change lines containg "PB" (and etc) to ones containin "LATBCLR"

C-s			;; isearch-forward
?			;; self-insert-command
SPC			;; self-insert-command
P			;; self-insert-command
NUL			;; set-mark-command
DEL			;; c-electric-backspace
LAT			;; self-insert-command * 3
C-f			;; forward-char
CLR			;; self-insert-command * 3
C-n			;; next-line
C-a			;; move-beginning-of-line

*/

#define _dpin_to_clr_macro(P)			\
  ((P)==0) ? &LATFCLR : ( \
  ((P)==1) ? &LATFCLR : ( \
  ((P)==2) ? &LATDCLR : ( \
  ((P)==3) ? &LATDCLR : ( \
  ((P)==4) ? &LATFCLR : ( \
  ((P)==5) ? &LATDCLR : ( \
  ((P)==6) ? &LATDCLR : ( \
  ((P)==7) ? &LATDCLR : ( \
  ((P)==8) ? &LATDCLR : ( \
  ((P)==9) ? &LATDCLR : ( \
  ((P)==10) ? &LATDCLR : ( \
  ((P)==11) ? &LATGCLR : ( \
  ((P)==12) ? &LATGCLR : ( \
  ((P)==13) ? &LATGCLR : ( \
  ((P)==14) ? &LATBCLR : ( \
  ((P)==15) ? &LATBCLR : ( \
  ((P)==16) ? &LATBCLR : ( \
  ((P)==17) ? &LATBCLR : ( \
  ((P)==18) ? &LATBCLR : ( \
  ((P)==19) ? &LATBCLR : ( \
  ((P)==20) ? &LATBCLR : ( \
  ((P)==21) ? &LATBCLR : ( \
  ((P)==22) ? &LATBCLR : ( \
  ((P)==23) ? &LATBCLR : ( \
  ((P)==24) ? &LATBCLR : ( \
  ((P)==25) ? &LATBCLR : ( \
  ((P)==26) ? &LATECLR : ( \
  ((P)==27) ? &LATECLR : ( \
  ((P)==28) ? &LATECLR : ( \
  ((P)==29) ? &LATECLR : ( \
  ((P)==30) ? &LATECLR : ( \
  ((P)==31) ? &LATECLR : ( \
  ((P)==32) ? &LATECLR : ( \
  ((P)==33) ? &LATECLR : ( \
  ((P)==34) ? &LATDCLR : ( \
  ((P)==35) ? &LATDCLR : ( \
  ((P)==36) ? &LATDCLR : ( \
  ((P)==37) ? &LATDCLR : ( \
  ((P)==38) ? &LATFCLR : ( \
  ((P)==39) ? &LATFCLR : ( \
  ((P)==40) ? &LATFCLR : ( \
  ((P)==41) ? &LATBCLR : ( \
  ((P)==42) ? &LATBCLR : ( \
  ((P)==43) ? &LATFCLR : 0 )))))))))))))))))))))))))))))))))))))))))))


//************************************************************************
/*  Slightly different macro for the bitmasks in digital_pin_to_bit_mask_PGM

2*SPC			;; self-insert-command
C-q (			;; quoted-insert
C-q (			;; quoted-insert
P			;; self-insert-command
C-q )			;; quoted-insert
==			;; self-insert-command * 2
C-x r g p		;; insert-register p
C-u 1 C-x r + p		;; increment-register p
M-f			;; forward-word
C-q )			;; quoted-insert
C-q SPC			;; quoted-insert
C-q ?			;; quoted-insert
M-\			;; delete whitespace
C-q SPC			;; quoted-insert
C-s ) C-@		;; forward-search
C-q SPC			;; quoted-insert
C-q :			;; quoted-insert
C-q SPC			;; quoted-insert
C-q (			;; quoted-insert
C-k			;; kill-line
C-q SPC			;; quoted-insert
C-q \			;; quoted-insert
C-n			;; next-line
C-a			;; move-beginning-of-line

*/

#define _dpin_to_bitmask_macro(P) \
  ((P)==0) ? _BV( 2 ) : ( \
  ((P)==1) ? _BV( 3 ) : ( \
  ((P)==2) ? _BV( 8 ) : ( \
  ((P)==3) ? _BV( 0 ) : ( \
  ((P)==4) ? _BV( 1 ) : ( \
  ((P)==5) ? _BV( 1 ) : ( \
  ((P)==6) ? _BV( 2 ) : ( \
  ((P)==7) ? _BV( 9 ) : ( \
  ((P)==8) ? _BV( 10 ) : ( \
  ((P)==9) ? _BV( 3 ) : ( \
  ((P)==10) ? _BV( 4 ) : ( \
  ((P)==11) ? _BV( 8 ) : ( \
  ((P)==12) ? _BV( 7 ) : ( \
  ((P)==13) ? _BV( 6 ) : ( \
  ((P)==14) ? _BV( 2 ) : ( \
  ((P)==15) ? _BV( 4 ) : ( \
  ((P)==16) ? _BV( 8 ) : ( \
  ((P)==17) ? _BV( 10 ) : ( \
  ((P)==18) ? _BV( 12 ) : ( \
  ((P)==19) ? _BV( 14 ) : ( \
  ((P)==20) ? _BV( 3 ) : ( \
  ((P)==21) ? _BV( 5 ) : ( \
  ((P)==22) ? _BV( 9 ) : ( \
  ((P)==23) ? _BV( 11 ) : ( \
  ((P)==24) ? _BV( 13 ) : ( \
  ((P)==25) ? _BV( 15 ) : ( \
  ((P)==26) ? _BV( 0 ) : ( \
  ((P)==27) ? _BV( 1 ) : ( \
  ((P)==28) ? _BV( 2 ) : ( \
  ((P)==29) ? _BV( 3 ) : ( \
  ((P)==30) ? _BV( 4 ) : ( \
  ((P)==31) ? _BV( 5 ) : ( \
  ((P)==32) ? _BV( 6 ) : ( \
  ((P)==33) ? _BV( 7 ) : ( \
  ((P)==34) ? _BV( 5 ) : ( \
  ((P)==35) ? _BV( 11 ) : ( \
  ((P)==36) ? _BV( 6 ) : ( \
  ((P)==37) ? _BV( 7 ) : ( \
  ((P)==38) ? _BV( 6 ) : ( \
  ((P)==39) ? _BV( 4 ) : ( \
  ((P)==40) ? _BV( 5 ) : ( \
  ((P)==41) ? _BV( 1 ) : ( \
  ((P)==42) ? _BV( 0 ) : ( \
  ((P)==43) ? _BV( 0 ) : 0)))))))))))))))))))))))))))))))))))))))))))

/*
 * Input Macro.  probably should decide about PORTx vs LATx
 * IIRC, these were created using simple query/replace over a copy
 * of the previous tables, rather than a separate keyboard macro.
 */
#define _dpin_to_inp_macro(P)			\
  ((P)==0) ? &PORTF : ( \
  ((P)==1) ? &PORTF : ( \
  ((P)==2) ? &PORTD : ( \
  ((P)==3) ? &PORTD : ( \
  ((P)==4) ? &PORTF : ( \
  ((P)==5) ? &PORTD : ( \
  ((P)==6) ? &PORTD : ( \
  ((P)==7) ? &PORTD : ( \
  ((P)==8) ? &PORTD : ( \
  ((P)==9) ? &PORTD : ( \
  ((P)==10) ? &PORTD : ( \
  ((P)==11) ? &PORTG : ( \
  ((P)==12) ? &PORTG : ( \
  ((P)==13) ? &PORTG : ( \
  ((P)==14) ? &PORTB : ( \
  ((P)==15) ? &PORTB : ( \
  ((P)==16) ? &PORTB : ( \
  ((P)==17) ? &PORTB : ( \
  ((P)==18) ? &PORTB : ( \
  ((P)==19) ? &PORTB : ( \
  ((P)==20) ? &PORTB : ( \
  ((P)==21) ? &PORTB : ( \
  ((P)==22) ? &PORTB : ( \
  ((P)==23) ? &PORTB : ( \
  ((P)==24) ? &PORTB : ( \
  ((P)==25) ? &PORTB : ( \
  ((P)==26) ? &PORTE : ( \
  ((P)==27) ? &PORTE : ( \
  ((P)==28) ? &PORTE : ( \
  ((P)==29) ? &PORTE : ( \
  ((P)==30) ? &PORTE : ( \
  ((P)==31) ? &PORTE : ( \
  ((P)==32) ? &PORTE : ( \
  ((P)==33) ? &PORTE : ( \
  ((P)==34) ? &PORTD : ( \
  ((P)==35) ? &PORTD : ( \
  ((P)==36) ? &PORTD : ( \
  ((P)==37) ? &PORTD : ( \
  ((P)==38) ? &PORTF : ( \
  ((P)==39) ? &PORTF : ( \
  ((P)==40) ? &PORTF : ( \
  ((P)==41) ? &PORTB : ( \
  ((P)==42) ? &PORTB : ( \
  ((P)==43) ? &PORTF : 0 )))))))))))))))))))))))))))))))))))))))))))

#define _dpin_to_trisclr_macro(P)			\
  ((P)==0) ? &TRISFCLR : ( \
  ((P)==1) ? &TRISFCLR : ( \
  ((P)==2) ? &TRISDCLR : ( \
  ((P)==3) ? &TRISDCLR : ( \
  ((P)==4) ? &TRISFCLR : ( \
  ((P)==5) ? &TRISDCLR : ( \
  ((P)==6) ? &TRISDCLR : ( \
  ((P)==7) ? &TRISDCLR : ( \
  ((P)==8) ? &TRISDCLR : ( \
  ((P)==9) ? &TRISDCLR : ( \
  ((P)==10) ? &TRISDCLR : ( \
  ((P)==11) ? &TRISGCLR : ( \
  ((P)==12) ? &TRISGCLR : ( \
  ((P)==13) ? &TRISGCLR : ( \
  ((P)==14) ? &TRISBCLR : ( \
  ((P)==15) ? &TRISBCLR : ( \
  ((P)==16) ? &TRISBCLR : ( \
  ((P)==17) ? &TRISBCLR : ( \
  ((P)==18) ? &TRISBCLR : ( \
  ((P)==19) ? &TRISBCLR : ( \
  ((P)==20) ? &TRISBCLR : ( \
  ((P)==21) ? &TRISBCLR : ( \
  ((P)==22) ? &TRISBCLR : ( \
  ((P)==23) ? &TRISBCLR : ( \
  ((P)==24) ? &TRISBCLR : ( \
  ((P)==25) ? &TRISBCLR : ( \
  ((P)==26) ? &TRISECLR : ( \
  ((P)==27) ? &TRISECLR : ( \
  ((P)==28) ? &TRISECLR : ( \
  ((P)==29) ? &TRISECLR : ( \
  ((P)==30) ? &TRISECLR : ( \
  ((P)==31) ? &TRISECLR : ( \
  ((P)==32) ? &TRISECLR : ( \
  ((P)==33) ? &TRISECLR : ( \
  ((P)==34) ? &TRISDCLR : ( \
  ((P)==35) ? &TRISDCLR : ( \
  ((P)==36) ? &TRISDCLR : ( \
  ((P)==37) ? &TRISDCLR : ( \
  ((P)==38) ? &TRISFCLR : ( \
  ((P)==39) ? &TRISFCLR : ( \
  ((P)==40) ? &TRISFCLR : ( \
  ((P)==41) ? &TRISBCLR : ( \
  ((P)==42) ? &TRISBCLR : ( \
  ((P)==43) ? &TRISFCLR : 0 )))))))))))))))))))))))))))))))))))))))))))

#define _dpin_to_trisset_macro(P)			\
  ((P)==0) ? &TRISFSET : ( \
  ((P)==1) ? &TRISFSET : ( \
  ((P)==2) ? &TRISDSET : ( \
  ((P)==3) ? &TRISDSET : ( \
  ((P)==4) ? &TRISFSET : ( \
  ((P)==5) ? &TRISDSET : ( \
  ((P)==6) ? &TRISDSET : ( \
  ((P)==7) ? &TRISDSET : ( \
  ((P)==8) ? &TRISDSET : ( \
  ((P)==9) ? &TRISDSET : ( \
  ((P)==10) ? &TRISDSET : ( \
  ((P)==11) ? &TRISGSET : ( \
  ((P)==12) ? &TRISGSET : ( \
  ((P)==13) ? &TRISGSET : ( \
  ((P)==14) ? &TRISBSET : ( \
  ((P)==15) ? &TRISBSET : ( \
  ((P)==16) ? &TRISBSET : ( \
  ((P)==17) ? &TRISBSET : ( \
  ((P)==18) ? &TRISBSET : ( \
  ((P)==19) ? &TRISBSET : ( \
  ((P)==20) ? &TRISBSET : ( \
  ((P)==21) ? &TRISBSET : ( \
  ((P)==22) ? &TRISBSET : ( \
  ((P)==23) ? &TRISBSET : ( \
  ((P)==24) ? &TRISBSET : ( \
  ((P)==25) ? &TRISBSET : ( \
  ((P)==26) ? &TRISESET : ( \
  ((P)==27) ? &TRISESET : ( \
  ((P)==28) ? &TRISESET : ( \
  ((P)==29) ? &TRISESET : ( \
  ((P)==30) ? &TRISESET : ( \
  ((P)==31) ? &TRISESET : ( \
  ((P)==32) ? &TRISESET : ( \
  ((P)==33) ? &TRISESET : ( \
  ((P)==34) ? &TRISDSET : ( \
  ((P)==35) ? &TRISDSET : ( \
  ((P)==36) ? &TRISDSET : ( \
  ((P)==37) ? &TRISDSET : ( \
  ((P)==38) ? &TRISFSET : ( \
  ((P)==39) ? &TRISFSET : ( \
  ((P)==40) ? &TRISFSET : ( \
  ((P)==41) ? &TRISBSET : ( \
  ((P)==42) ? &TRISBSET : ( \
  ((P)==43) ? &TRISFSET : 0 )))))))))))))))))))))))))))))))))))))))))))

#endif /* _BOARD_UNO_ */

#if defined(_BOARD_MEGA_) && !defined(__AVR__)
#define _dpin_to_set_macro(P) \
  ((P)==0) ? &LATFSET : ( \
  ((P)==1) ? &LATFSET : ( \
  ((P)==2) ? &LATESET : ( \
  ((P)==3) ? &LATDSET : ( \
  ((P)==4) ? &LATCSET : ( \
  ((P)==5) ? &LATDSET : ( \
  ((P)==6) ? &LATDSET : ( \
  ((P)==7) ? &LATESET : ( \
  ((P)==8) ? &LATDSET : ( \
  ((P)==9) ? &LATDSET : ( \
  ((P)==10) ? &LATDSET : ( \
  ((P)==11) ? &LATCSET : ( \
  ((P)==12) ? &LATASET : ( \
  ((P)==13) ? &LATASET : ( \
  ((P)==14) ? &LATFSET : ( \
  ((P)==15) ? &LATFSET : ( \
  ((P)==16) ? &LATFSET : ( \
  ((P)==17) ? &LATFSET : ( \
  ((P)==18) ? &LATDSET : ( \
  ((P)==19) ? &LATDSET : ( \
  ((P)==20) ? &LATASET : ( \
  ((P)==21) ? &LATASET : ( \
  ((P)==22) ? &LATCSET : ( \
  ((P)==23) ? &LATCSET : ( \
  ((P)==24) ? &LATCSET : ( \
  ((P)==25) ? &LATFSET : ( \
  ((P)==26) ? &LATGSET : ( \
  ((P)==27) ? &LATGSET : ( \
  ((P)==28) ? &LATGSET : ( \
  ((P)==29) ? &LATGSET : ( \
  ((P)==30) ? &LATESET : ( \
  ((P)==31) ? &LATESET : ( \
  ((P)==32) ? &LATESET : ( \
  ((P)==33) ? &LATESET : ( \
  ((P)==34) ? &LATESET : ( \
  ((P)==35) ? &LATESET : ( \
  ((P)==36) ? &LATESET : ( \
  ((P)==37) ? &LATESET : ( \
  ((P)==38) ? &LATDSET : ( \
  ((P)==39) ? &LATDSET : ( \
  ((P)==40) ? &LATBSET : ( \
  ((P)==41) ? &LATBSET : ( \
  ((P)==42) ? &LATBSET : ( \
  ((P)==43) ? &LATGSET : ( \
  ((P)==44) ? &LATASET : ( \
  ((P)==45) ? &LATFSET : ( \
  ((P)==46) ? &LATFSET : ( \
  ((P)==47) ? &LATDSET : ( \
  ((P)==48) ? &LATDSET : ( \
  ((P)==49) ? &LATDSET : ( \
  ((P)==50) ? &LATGSET : ( \
  ((P)==51) ? &LATGSET : ( \
  ((P)==52) ? &LATGSET : ( \
  ((P)==53) ? &LATGSET : ( \
  ((P)==54) ? &LATBSET : ( \
  ((P)==55) ? &LATBSET : ( \
  ((P)==56) ? &LATBSET : ( \
  ((P)==57) ? &LATBSET : ( \
  ((P)==58) ? &LATBSET : ( \
  ((P)==59) ? &LATBSET : ( \
  ((P)==60) ? &LATBSET : ( \
  ((P)==61) ? &LATBSET : ( \
  ((P)==62) ? &LATBSET : ( \
  ((P)==63) ? &LATBSET : ( \
  ((P)==64) ? &LATBSET : ( \
  ((P)==65) ? &LATBSET : ( \
  ((P)==66) ? &LATBSET : ( \
  ((P)==67) ? &LATBSET : ( \
  ((P)==68) ? &LATBSET : ( \
  ((P)==69) ? &LATBSET : ( \
  ((P)==70) ? &LATASET : ( \
  ((P)==71) ? &LATASET : ( \
  ((P)==72) ? &LATASET : ( \
  ((P)==73) ? &LATASET : ( \
  ((P)==74) ? &LATDSET : ( \
  ((P)==75) ? &LATCSET : ( \
  ((P)==76) ? &LATDSET : ( \
  ((P)==77) ? &LATDSET : ( \
  ((P)==78) ? &LATGSET : ( \
  ((P)==79) ? &LATGSET : ( \
  ((P)==80) ? &LATASET : ( \
  ((P)==81) ? &LATASET : ( \
  ((P)==82) ? &LATGSET : ( \
  ((P)==83) ? &LATGSET : ( \
  ((P)==84) ? &LATGSET : ( \
  ((P)==85) ? &LATASET : 0 )))))))))))))))))))))))))))))))))))))))) \
      )))))))))))))))))))))))))))))))))))))))))))))

#define _dpin_to_clr_macro(P) \
  ((P)==0) ? &LATFCLR : ( \
  ((P)==1) ? &LATFCLR : ( \
  ((P)==2) ? &LATECLR : ( \
  ((P)==3) ? &LATDCLR : ( \
  ((P)==4) ? &LATCCLR : ( \
  ((P)==5) ? &LATDCLR : ( \
  ((P)==6) ? &LATDCLR : ( \
  ((P)==7) ? &LATECLR : ( \
  ((P)==8) ? &LATDCLR : ( \
  ((P)==9) ? &LATDCLR : ( \
  ((P)==10) ? &LATDCLR : ( \
  ((P)==11) ? &LATCCLR : ( \
  ((P)==12) ? &LATACLR : ( \
  ((P)==13) ? &LATACLR : ( \
  ((P)==14) ? &LATFCLR : ( \
  ((P)==15) ? &LATFCLR : ( \
  ((P)==16) ? &LATFCLR : ( \
  ((P)==17) ? &LATFCLR : ( \
  ((P)==18) ? &LATDCLR : ( \
  ((P)==19) ? &LATDCLR : ( \
  ((P)==20) ? &LATACLR : ( \
  ((P)==21) ? &LATACLR : ( \
  ((P)==22) ? &LATCCLR : ( \
  ((P)==23) ? &LATCCLR : ( \
  ((P)==24) ? &LATCCLR : ( \
  ((P)==25) ? &LATFCLR : ( \
  ((P)==26) ? &LATGCLR : ( \
  ((P)==27) ? &LATGCLR : ( \
  ((P)==28) ? &LATGCLR : ( \
  ((P)==29) ? &LATGCLR : ( \
  ((P)==30) ? &LATECLR : ( \
  ((P)==31) ? &LATECLR : ( \
  ((P)==32) ? &LATECLR : ( \
  ((P)==33) ? &LATECLR : ( \
  ((P)==34) ? &LATECLR : ( \
  ((P)==35) ? &LATECLR : ( \
  ((P)==36) ? &LATECLR : ( \
  ((P)==37) ? &LATECLR : ( \
  ((P)==38) ? &LATDCLR : ( \
  ((P)==39) ? &LATDCLR : ( \
  ((P)==40) ? &LATBCLR : ( \
  ((P)==41) ? &LATBCLR : ( \
  ((P)==42) ? &LATBCLR : ( \
  ((P)==43) ? &LATGCLR : ( \
  ((P)==44) ? &LATACLR : ( \
  ((P)==45) ? &LATFCLR : ( \
  ((P)==46) ? &LATFCLR : ( \
  ((P)==47) ? &LATDCLR : ( \
  ((P)==48) ? &LATDCLR : ( \
  ((P)==49) ? &LATDCLR : ( \
  ((P)==50) ? &LATGCLR : ( \
  ((P)==51) ? &LATGCLR : ( \
  ((P)==52) ? &LATGCLR : ( \
  ((P)==53) ? &LATGCLR : ( \
  ((P)==54) ? &LATBCLR : ( \
  ((P)==55) ? &LATBCLR : ( \
  ((P)==56) ? &LATBCLR : ( \
  ((P)==57) ? &LATBCLR : ( \
  ((P)==58) ? &LATBCLR : ( \
  ((P)==59) ? &LATBCLR : ( \
  ((P)==60) ? &LATBCLR : ( \
  ((P)==61) ? &LATBCLR : ( \
  ((P)==62) ? &LATBCLR : ( \
  ((P)==63) ? &LATBCLR : ( \
  ((P)==64) ? &LATBCLR : ( \
  ((P)==65) ? &LATBCLR : ( \
  ((P)==66) ? &LATBCLR : ( \
  ((P)==67) ? &LATBCLR : ( \
  ((P)==68) ? &LATBCLR : ( \
  ((P)==69) ? &LATBCLR : ( \
  ((P)==70) ? &LATACLR : ( \
  ((P)==71) ? &LATACLR : ( \
  ((P)==72) ? &LATACLR : ( \
  ((P)==73) ? &LATACLR : ( \
  ((P)==74) ? &LATDCLR : ( \
  ((P)==75) ? &LATCCLR : ( \
  ((P)==76) ? &LATDCLR : ( \
  ((P)==77) ? &LATDCLR : ( \
  ((P)==78) ? &LATGCLR : ( \
  ((P)==79) ? &LATGCLR : ( \
  ((P)==80) ? &LATACLR : ( \
  ((P)==81) ? &LATACLR : ( \
  ((P)==82) ? &LATGCLR : ( \
  ((P)==83) ? &LATGCLR : ( \
  ((P)==84) ? &LATGCLR : ( \
  ((P)==85) ? &LATACLR : 0 )))))))))))))))))))))))))))))))))))))))) \
      )))))))))))))))))))))))))))))))))))))))))))))

#define _dpin_to_trisset_macro(P) \
  ((P)==0) ? &TRISFSET : ( \
  ((P)==1) ? &TRISFSET : ( \
  ((P)==2) ? &TRISESET : ( \
  ((P)==3) ? &TRISDSET : ( \
  ((P)==4) ? &TRISCSET : ( \
  ((P)==5) ? &TRISDSET : ( \
  ((P)==6) ? &TRISDSET : ( \
  ((P)==7) ? &TRISESET : ( \
  ((P)==8) ? &TRISDSET : ( \
  ((P)==9) ? &TRISDSET : ( \
  ((P)==10) ? &TRISDSET : ( \
  ((P)==11) ? &TRISCSET : ( \
  ((P)==12) ? &TRISASET : ( \
  ((P)==13) ? &TRISASET : ( \
  ((P)==14) ? &TRISFSET : ( \
  ((P)==15) ? &TRISFSET : ( \
  ((P)==16) ? &TRISFSET : ( \
  ((P)==17) ? &TRISFSET : ( \
  ((P)==18) ? &TRISDSET : ( \
  ((P)==19) ? &TRISDSET : ( \
  ((P)==20) ? &TRISASET : ( \
  ((P)==21) ? &TRISASET : ( \
  ((P)==22) ? &TRISCSET : ( \
  ((P)==23) ? &TRISCSET : ( \
  ((P)==24) ? &TRISCSET : ( \
  ((P)==25) ? &TRISFSET : ( \
  ((P)==26) ? &TRISGSET : ( \
  ((P)==27) ? &TRISGSET : ( \
  ((P)==28) ? &TRISGSET : ( \
  ((P)==29) ? &TRISGSET : ( \
  ((P)==30) ? &TRISESET : ( \
  ((P)==31) ? &TRISESET : ( \
  ((P)==32) ? &TRISESET : ( \
  ((P)==33) ? &TRISESET : ( \
  ((P)==34) ? &TRISESET : ( \
  ((P)==35) ? &TRISESET : ( \
  ((P)==36) ? &TRISESET : ( \
  ((P)==37) ? &TRISESET : ( \
  ((P)==38) ? &TRISDSET : ( \
  ((P)==39) ? &TRISDSET : ( \
  ((P)==40) ? &TRISBSET : ( \
  ((P)==41) ? &TRISBSET : ( \
  ((P)==42) ? &TRISBSET : ( \
  ((P)==43) ? &TRISGSET : ( \
  ((P)==44) ? &TRISASET : ( \
  ((P)==45) ? &TRISFSET : ( \
  ((P)==46) ? &TRISFSET : ( \
  ((P)==47) ? &TRISDSET : ( \
  ((P)==48) ? &TRISDSET : ( \
  ((P)==49) ? &TRISDSET : ( \
  ((P)==50) ? &TRISGSET : ( \
  ((P)==51) ? &TRISGSET : ( \
  ((P)==52) ? &TRISGSET : ( \
  ((P)==53) ? &TRISGSET : ( \
  ((P)==54) ? &TRISBSET : ( \
  ((P)==55) ? &TRISBSET : ( \
  ((P)==56) ? &TRISBSET : ( \
  ((P)==57) ? &TRISBSET : ( \
  ((P)==58) ? &TRISBSET : ( \
  ((P)==59) ? &TRISBSET : ( \
  ((P)==60) ? &TRISBSET : ( \
  ((P)==61) ? &TRISBSET : ( \
  ((P)==62) ? &TRISBSET : ( \
  ((P)==63) ? &TRISBSET : ( \
  ((P)==64) ? &TRISBSET : ( \
  ((P)==65) ? &TRISBSET : ( \
  ((P)==66) ? &TRISBSET : ( \
  ((P)==67) ? &TRISBSET : ( \
  ((P)==68) ? &TRISBSET : ( \
  ((P)==69) ? &TRISBSET : ( \
  ((P)==70) ? &TRISASET : ( \
  ((P)==71) ? &TRISASET : ( \
  ((P)==72) ? &TRISASET : ( \
  ((P)==73) ? &TRISASET : ( \
  ((P)==74) ? &TRISDSET : ( \
  ((P)==75) ? &TRISCSET : ( \
  ((P)==76) ? &TRISDSET : ( \
  ((P)==77) ? &TRISDSET : ( \
  ((P)==78) ? &TRISGSET : ( \
  ((P)==79) ? &TRISGSET : ( \
  ((P)==80) ? &TRISASET : ( \
  ((P)==81) ? &TRISASET : ( \
  ((P)==82) ? &TRISGSET : ( \
  ((P)==83) ? &TRISGSET : ( \
  ((P)==84) ? &TRISGSET : ( \
  ((P)==85) ? &TRISASET : 0 )))))))))))))))))))))))))))))))))))))))) \
      )))))))))))))))))))))))))))))))))))))))))))))


#define _dpin_to_trisclr_macro(P) \
  ((P)==0) ? &TRISFCLR : ( \
  ((P)==1) ? &TRISFCLR : ( \
  ((P)==2) ? &TRISECLR : ( \
  ((P)==3) ? &TRISDCLR : ( \
  ((P)==4) ? &TRISCCLR : ( \
  ((P)==5) ? &TRISDCLR : ( \
  ((P)==6) ? &TRISDCLR : ( \
  ((P)==7) ? &TRISECLR : ( \
  ((P)==8) ? &TRISDCLR : ( \
  ((P)==9) ? &TRISDCLR : ( \
  ((P)==10) ? &TRISDCLR : ( \
  ((P)==11) ? &TRISCCLR : ( \
  ((P)==12) ? &TRISACLR : ( \
  ((P)==13) ? &TRISACLR : ( \
  ((P)==14) ? &TRISFCLR : ( \
  ((P)==15) ? &TRISFCLR : ( \
  ((P)==16) ? &TRISFCLR : ( \
  ((P)==17) ? &TRISFCLR : ( \
  ((P)==18) ? &TRISDCLR : ( \
  ((P)==19) ? &TRISDCLR : ( \
  ((P)==20) ? &TRISACLR : ( \
  ((P)==21) ? &TRISACLR : ( \
  ((P)==22) ? &TRISCCLR : ( \
  ((P)==23) ? &TRISCCLR : ( \
  ((P)==24) ? &TRISCCLR : ( \
  ((P)==25) ? &TRISFCLR : ( \
  ((P)==26) ? &TRISGCLR : ( \
  ((P)==27) ? &TRISGCLR : ( \
  ((P)==28) ? &TRISGCLR : ( \
  ((P)==29) ? &TRISGCLR : ( \
  ((P)==30) ? &TRISECLR : ( \
  ((P)==31) ? &TRISECLR : ( \
  ((P)==32) ? &TRISECLR : ( \
  ((P)==33) ? &TRISECLR : ( \
  ((P)==34) ? &TRISECLR : ( \
  ((P)==35) ? &TRISECLR : ( \
  ((P)==36) ? &TRISECLR : ( \
  ((P)==37) ? &TRISECLR : ( \
  ((P)==38) ? &TRISDCLR : ( \
  ((P)==39) ? &TRISDCLR : ( \
  ((P)==40) ? &TRISBCLR : ( \
  ((P)==41) ? &TRISBCLR : ( \
  ((P)==42) ? &TRISBCLR : ( \
  ((P)==43) ? &TRISGCLR : ( \
  ((P)==44) ? &TRISACLR : ( \
  ((P)==45) ? &TRISFCLR : ( \
  ((P)==46) ? &TRISFCLR : ( \
  ((P)==47) ? &TRISDCLR : ( \
  ((P)==48) ? &TRISDCLR : ( \
  ((P)==49) ? &TRISDCLR : ( \
  ((P)==50) ? &TRISGCLR : ( \
  ((P)==51) ? &TRISGCLR : ( \
  ((P)==52) ? &TRISGCLR : ( \
  ((P)==53) ? &TRISGCLR : ( \
  ((P)==54) ? &TRISBCLR : ( \
  ((P)==55) ? &TRISBCLR : ( \
  ((P)==56) ? &TRISBCLR : ( \
  ((P)==57) ? &TRISBCLR : ( \
  ((P)==58) ? &TRISBCLR : ( \
  ((P)==59) ? &TRISBCLR : ( \
  ((P)==60) ? &TRISBCLR : ( \
  ((P)==61) ? &TRISBCLR : ( \
  ((P)==62) ? &TRISBCLR : ( \
  ((P)==63) ? &TRISBCLR : ( \
  ((P)==64) ? &TRISBCLR : ( \
  ((P)==65) ? &TRISBCLR : ( \
  ((P)==66) ? &TRISBCLR : ( \
  ((P)==67) ? &TRISBCLR : ( \
  ((P)==68) ? &TRISBCLR : ( \
  ((P)==69) ? &TRISBCLR : ( \
  ((P)==70) ? &TRISACLR : ( \
  ((P)==71) ? &TRISACLR : ( \
  ((P)==72) ? &TRISACLR : ( \
  ((P)==73) ? &TRISACLR : ( \
  ((P)==74) ? &TRISDCLR : ( \
  ((P)==75) ? &TRISCCLR : ( \
  ((P)==76) ? &TRISDCLR : ( \
  ((P)==77) ? &TRISDCLR : ( \
  ((P)==78) ? &TRISGCLR : ( \
  ((P)==79) ? &TRISGCLR : ( \
  ((P)==80) ? &TRISACLR : ( \
  ((P)==81) ? &TRISACLR : ( \
  ((P)==82) ? &TRISGCLR : ( \
  ((P)==83) ? &TRISGCLR : ( \
  ((P)==84) ? &TRISGCLR : ( \
  ((P)==85) ? &TRISACLR : 0 )))))))))))))))))))))))))))))))))))))))) \
      )))))))))))))))))))))))))))))))))))))))))))))


#define _dpin_to_inp_macro(P) \
  ((P)==0) ? &PORTF : ( \
  ((P)==1) ? &PORTF : ( \
  ((P)==2) ? &PORTE : ( \
  ((P)==3) ? &PORTD : ( \
  ((P)==4) ? &PORTC : ( \
  ((P)==5) ? &PORTD : ( \
  ((P)==6) ? &PORTD : ( \
  ((P)==7) ? &PORTE : ( \
  ((P)==8) ? &PORTD : ( \
  ((P)==9) ? &PORTD : ( \
  ((P)==10) ? &PORTD : ( \
  ((P)==11) ? &PORTC : ( \
  ((P)==12) ? &PORTA : ( \
  ((P)==13) ? &PORTA : ( \
  ((P)==14) ? &PORTF : ( \
  ((P)==15) ? &PORTF : ( \
  ((P)==16) ? &PORTF : ( \
  ((P)==17) ? &PORTF : ( \
  ((P)==18) ? &PORTD : ( \
  ((P)==19) ? &PORTD : ( \
  ((P)==20) ? &PORTA : ( \
  ((P)==21) ? &PORTA : ( \
  ((P)==22) ? &PORTC : ( \
  ((P)==23) ? &PORTC : ( \
  ((P)==24) ? &PORTC : ( \
  ((P)==25) ? &PORTF : ( \
  ((P)==26) ? &PORTG : ( \
  ((P)==27) ? &PORTG : ( \
  ((P)==28) ? &PORTG : ( \
  ((P)==29) ? &PORTG : ( \
  ((P)==30) ? &PORTE : ( \
  ((P)==31) ? &PORTE : ( \
  ((P)==32) ? &PORTE : ( \
  ((P)==33) ? &PORTE : ( \
  ((P)==34) ? &PORTE : ( \
  ((P)==35) ? &PORTE : ( \
  ((P)==36) ? &PORTE : ( \
  ((P)==37) ? &PORTE : ( \
  ((P)==38) ? &PORTD : ( \
  ((P)==39) ? &PORTD : ( \
  ((P)==40) ? &PORTB : ( \
  ((P)==41) ? &PORTB : ( \
  ((P)==42) ? &PORTB : ( \
  ((P)==43) ? &PORTG : ( \
  ((P)==44) ? &PORTA : ( \
  ((P)==45) ? &PORTF : ( \
  ((P)==46) ? &PORTF : ( \
  ((P)==47) ? &PORTD : ( \
  ((P)==48) ? &PORTD : ( \
  ((P)==49) ? &PORTD : ( \
  ((P)==50) ? &PORTG : ( \
  ((P)==51) ? &PORTG : ( \
  ((P)==52) ? &PORTG : ( \
  ((P)==53) ? &PORTG : ( \
  ((P)==54) ? &PORTB : ( \
  ((P)==55) ? &PORTB : ( \
  ((P)==56) ? &PORTB : ( \
  ((P)==57) ? &PORTB : ( \
  ((P)==58) ? &PORTB : ( \
  ((P)==59) ? &PORTB : ( \
  ((P)==60) ? &PORTB : ( \
  ((P)==61) ? &PORTB : ( \
  ((P)==62) ? &PORTB : ( \
  ((P)==63) ? &PORTB : ( \
  ((P)==64) ? &PORTB : ( \
  ((P)==65) ? &PORTB : ( \
  ((P)==66) ? &PORTB : ( \
  ((P)==67) ? &PORTB : ( \
  ((P)==68) ? &PORTB : ( \
  ((P)==69) ? &PORTB : ( \
  ((P)==70) ? &PORTA : ( \
  ((P)==71) ? &PORTA : ( \
  ((P)==72) ? &PORTA : ( \
  ((P)==73) ? &PORTA : ( \
  ((P)==74) ? &PORTD : ( \
  ((P)==75) ? &PORTC : ( \
  ((P)==76) ? &PORTD : ( \
  ((P)==77) ? &PORTD : ( \
  ((P)==78) ? &PORTG : ( \
  ((P)==79) ? &PORTG : ( \
  ((P)==80) ? &PORTA : ( \
  ((P)==81) ? &PORTA : ( \
  ((P)==82) ? &PORTG : ( \
  ((P)==83) ? &PORTG : ( \
  ((P)==84) ? &PORTG : ( \
  ((P)==85) ? &PORTA : 0 )))))))))))))))))))))))))))))))))))))))) \
      )))))))))))))))))))))))))))))))))))))))))))))

#define _dpin_to_bitmask_macro(P) \
  ((P)==0) ? _BV( 2 ) : ( \
  ((P)==1) ? _BV( 8 ) : ( \
  ((P)==2) ? _BV( 8 ) : ( \
  ((P)==3) ? _BV( 0 ) : ( \
  ((P)==4) ? _BV( 14 ) : ( \
  ((P)==5) ? _BV( 1 ) : ( \
  ((P)==6) ? _BV( 2 ) : ( \
  ((P)==7) ? _BV( 9 ) : ( \
  ((P)==8) ? _BV( 12 ) : ( \
  ((P)==9) ? _BV( 3 ) : ( \
  ((P)==10) ? _BV( 4 ) : ( \
  ((P)==11) ? _BV( 4 ) : ( \
  ((P)==12) ? _BV( 2 ) : ( \
  ((P)==13) ? _BV( 3 ) : ( \
  ((P)==14) ? _BV( 13 ) : ( \
  ((P)==15) ? _BV( 12 ) : ( \
  ((P)==16) ? _BV( 5 ) : ( \
  ((P)==17) ? _BV( 4 ) : ( \
  ((P)==18) ? _BV( 15 ) : ( \
  ((P)==19) ? _BV( 14 ) : ( \
  ((P)==20) ? _BV( 15 ) : ( \
  ((P)==21) ? _BV( 14 ) : ( \
  ((P)==22) ? _BV( 2 ) : ( \
  ((P)==23) ? _BV( 3 ) : ( \
  ((P)==24) ? _BV( 0 ) : ( \
  ((P)==25) ? _BV( 3 ) : ( \
  ((P)==26) ? _BV( 3 ) : ( \
  ((P)==27) ? _BV( 2 ) : ( \
  ((P)==28) ? _BV( 15 ) : ( \
  ((P)==29) ? _BV( 7 ) : ( \
  ((P)==30) ? _BV( 7 ) : ( \
  ((P)==31) ? _BV( 6 ) : ( \
  ((P)==32) ? _BV( 5 ) : ( \
  ((P)==33) ? _BV( 4 ) : ( \
  ((P)==34) ? _BV( 3 ) : ( \
  ((P)==35) ? _BV( 2 ) : ( \
  ((P)==36) ? _BV( 1 ) : ( \
  ((P)==37) ? _BV( 0 ) : ( \
  ((P)==38) ? _BV( 10 ) : ( \
  ((P)==39) ? _BV( 5 ) : ( \
  ((P)==40) ? _BV( 11 ) : ( \
  ((P)==41) ? _BV( 13 ) : ( \
  ((P)==42) ? _BV( 12 ) : ( \
  ((P)==43) ? _BV( 8 ) : ( \
  ((P)==44) ? _BV( 10 ) : ( \
  ((P)==45) ? _BV( 0 ) : ( \
  ((P)==46) ? _BV( 1 ) : ( \
  ((P)==47) ? _BV( 6 ) : ( \
  ((P)==48) ? _BV( 8 ) : ( \
  ((P)==49) ? _BV( 11 ) : ( \
  ((P)==50) ? _BV( 7 ) : ( \
  ((P)==51) ? _BV( 8 ) : ( \
  ((P)==52) ? _BV( 6 ) : ( \
  ((P)==53) ? _BV( 9 ) : ( \
  ((P)==54) ? _BV( 0 ) : ( \
  ((P)==55) ? _BV( 1 ) : ( \
  ((P)==56) ? _BV( 2 ) : ( \
  ((P)==57) ? _BV( 3 ) : ( \
  ((P)==58) ? _BV( 4 ) : ( \
  ((P)==59) ? _BV( 5 ) : ( \
  ((P)==60) ? _BV( 6 ) : ( \
  ((P)==61) ? _BV( 7 ) : ( \
  ((P)==62) ? _BV( 8 ) : ( \
  ((P)==63) ? _BV( 9 ) : ( \
  ((P)==64) ? _BV( 10 ) : ( \
  ((P)==65) ? _BV( 11 ) : ( \
  ((P)==66) ? _BV( 12 ) : ( \
  ((P)==67) ? _BV( 13 ) : ( \
  ((P)==68) ? _BV( 14 ) : ( \
  ((P)==69) ? _BV( 15 ) : ( \
  ((P)==70) ? _BV( 0 ) : ( \
  ((P)==71) ? _BV( 1 ) : ( \
  ((P)==72) ? _BV( 4 ) : ( \
  ((P)==73) ? _BV( 5 ) : ( \
  ((P)==74) ? _BV( 9 ) : ( \
  ((P)==75) ? _BV( 13 ) : ( \
  ((P)==76) ? _BV( 13 ) : ( \
  ((P)==77) ? _BV( 7 ) : ( \
  ((P)==78) ? _BV( 1 ) : ( \
  ((P)==79) ? _BV( 0 ) : ( \
  ((P)==80) ? _BV( 6 ) : ( \
  ((P)==81) ? _BV( 7 ) : ( \
  ((P)==82) ? _BV( 14 ) : ( \
  ((P)==83) ? _BV( 12 ) : ( \
  ((P)==84) ? _BV( 13 ) : ( \
  ((P)==85) ? _BV( 9 ) : 0)))))))))))))))))))))))))))))) \
      )))))))))))))))))))))))))))))))))))))))))))))))))))))))

#endif /* BOARD _MEGA_ */


#ifndef digitalWriteFast
#define digitalWriteFast(P, V) \
do {								\
    if (__builtin_constant_p(P) && __builtin_constant_p(V)) {		\
	if (V) {							\
	    *(_dpin_to_set_macro(P)) = _dpin_to_bitmask_macro(P);	\
	} else {							\
	    *(_dpin_to_clr_macro(P))  = _dpin_to_bitmask_macro(P);	\
	}								\
    } else  digitalWrite((P), (V));					\
}while (0)
#endif  //#ifndef digitalWriteFast


#define pinModeFast(P, D) \
do {								\
    if (__builtin_constant_p(P) && __builtin_constant_p(D)) {		\
	if (D == OUTPUT) {						\
	    *(_dpin_to_trisclr_macro(P)) = _dpin_to_bitmask_macro(P);	\
	} else {							\
	    *(_dpin_to_trisset_macro(P))  = _dpin_to_bitmask_macro(P);	\
	}								\
    } else pinMode((P), (D));					\
}while (0)

#ifdef BEFORE_BAP
#define _BIT_READ(value, bit) (((value) >> (bit)) & 0x01)

#define digitalReadFast(P)	       \
    (__builtin_constant_p(P) ) ? (					\
	(_BIT_READ(*(_dpin_to_inp_macro(P)), _dpin_to_bitmask_macro(P))) ) : \
    digitalRead((P))

#else
#define digitalReadFast(P)	       \
({                                     \
    int rval;                          \
    if (__builtin_constant_p(P))       \
    {                                  \
	rval = *(_dpin_to_inp_macro(P)) & ( _dpin_to_bitmask_macro(P)) ? HIGH : LOW; \
    }                                  \
    else                               \
    {                                  \
        rval = digitalRead((P));       \
    }                                  \
    rval; /* return value from macro */ \
})

#endif
