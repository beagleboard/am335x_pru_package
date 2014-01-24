; *********************************************************************
; * Interrupt Vector Table for PRU example code
; *
; *********************************************************************

; Global symbols defined here and exported out of this file
   .global _intcVectorTable
   .global _c_int00
   .global _vector1
   .global _vector2
   .global _vector3
   .global _PRUSS_EVTOUT0_isr

; This is a macro that instantiates one entry in the interrupt service table.
VEC_ENTRY .macro addr
    STW   B0,*--B15
    MVKL  addr,B0
    MVKH  addr,B0
    B     B0
    LDW   *B15++,B0
    NOP   2
    NOP
    NOP
   .endm

; This is a dummy interrupt service routine used to initialize the IST.
_vec_dummy:
  B    B3
  NOP  5

; This is the actual interrupt service table (IST).
 .sect ".vecs"
 .align 1024

_intcVectorTable:
_vector0:   VEC_ENTRY _c_int00      ;RESET
_vector1:   VEC_ENTRY _vec_dummy    ;NMI
_vector2:   VEC_ENTRY _vec_dummy    ;RSVD
_vector3:   VEC_ENTRY _vec_dummy    ;RSVD
_vector4:   VEC_ENTRY _vec_dummy    ;isr0
_vector5:   VEC_ENTRY _vec_dummy    ;isr1
_vector6:   VEC_ENTRY _vec_dummy    ;isr2
_vector7:   VEC_ENTRY _vec_dummy    ;isr3
_vector8:   VEC_ENTRY _vec_dummy    ;isr4
_vector9:   VEC_ENTRY _vec_dummy    ;isr5
_vector10:  VEC_ENTRY _vec_dummy    ;isr6
_vector11:  VEC_ENTRY _vec_dummy    ;isr7
_vector12:  VEC_ENTRY _PRUSS_EVTOUT0_isr  ;isr8

