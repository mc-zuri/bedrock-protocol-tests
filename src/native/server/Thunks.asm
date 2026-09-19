; BinaryStream vtable thunks: report (stream, slot, doc name) and tail-jump to the original method
; with all argument registers restored, so the call proceeds as if unhooked.

EXTERN bpx_stream_call:PROC
EXTERN bpx_original_slots:QWORD

.code

bpx_thunk_common PROC
    push rcx
    push rdx
    push r8
    push r9
    push r10
    sub rsp, 60h                 ; 32 shadow + 4 xmm; entry rsp%16 == 8, now 0
    movdqu [rsp+20h], xmm0
    movdqu [rsp+30h], xmm1
    movdqu [rsp+40h], xmm2
    movdqu [rsp+50h], xmm3
    mov rdx, r10                 ; rcx = stream, rdx = slot, r8 = doc name
    call bpx_stream_call
    movdqu xmm0, [rsp+20h]
    movdqu xmm1, [rsp+30h]
    movdqu xmm2, [rsp+40h]
    movdqu xmm3, [rsp+50h]
    add rsp, 60h
    pop r10
    pop r9
    pop r8
    pop rdx
    pop rcx
    lea rax, bpx_original_slots
    jmp qword ptr [rax + r10*8]
bpx_thunk_common ENDP

bpx_thunk_0 PROC
    mov r10, 0
    jmp bpx_thunk_common
bpx_thunk_0 ENDP

bpx_thunk_1 PROC
    mov r10, 1
    jmp bpx_thunk_common
bpx_thunk_1 ENDP

bpx_thunk_2 PROC
    mov r10, 2
    jmp bpx_thunk_common
bpx_thunk_2 ENDP

bpx_thunk_3 PROC
    mov r10, 3
    jmp bpx_thunk_common
bpx_thunk_3 ENDP

bpx_thunk_4 PROC
    mov r10, 4
    jmp bpx_thunk_common
bpx_thunk_4 ENDP

bpx_thunk_5 PROC
    mov r10, 5
    jmp bpx_thunk_common
bpx_thunk_5 ENDP

bpx_thunk_6 PROC
    mov r10, 6
    jmp bpx_thunk_common
bpx_thunk_6 ENDP

bpx_thunk_7 PROC
    mov r10, 7
    jmp bpx_thunk_common
bpx_thunk_7 ENDP

bpx_thunk_8 PROC
    mov r10, 8
    jmp bpx_thunk_common
bpx_thunk_8 ENDP

bpx_thunk_9 PROC
    mov r10, 9
    jmp bpx_thunk_common
bpx_thunk_9 ENDP

bpx_thunk_10 PROC
    mov r10, 10
    jmp bpx_thunk_common
bpx_thunk_10 ENDP

bpx_thunk_11 PROC
    mov r10, 11
    jmp bpx_thunk_common
bpx_thunk_11 ENDP

bpx_thunk_12 PROC
    mov r10, 12
    jmp bpx_thunk_common
bpx_thunk_12 ENDP

bpx_thunk_13 PROC
    mov r10, 13
    jmp bpx_thunk_common
bpx_thunk_13 ENDP

bpx_thunk_14 PROC
    mov r10, 14
    jmp bpx_thunk_common
bpx_thunk_14 ENDP

bpx_thunk_15 PROC
    mov r10, 15
    jmp bpx_thunk_common
bpx_thunk_15 ENDP

bpx_thunk_16 PROC
    mov r10, 16
    jmp bpx_thunk_common
bpx_thunk_16 ENDP

bpx_thunk_17 PROC
    mov r10, 17
    jmp bpx_thunk_common
bpx_thunk_17 ENDP

bpx_thunk_18 PROC
    mov r10, 18
    jmp bpx_thunk_common
bpx_thunk_18 ENDP

bpx_thunk_19 PROC
    mov r10, 19
    jmp bpx_thunk_common
bpx_thunk_19 ENDP

bpx_thunk_20 PROC
    mov r10, 20
    jmp bpx_thunk_common
bpx_thunk_20 ENDP

bpx_thunk_21 PROC
    mov r10, 21
    jmp bpx_thunk_common
bpx_thunk_21 ENDP

bpx_thunk_22 PROC
    mov r10, 22
    jmp bpx_thunk_common
bpx_thunk_22 ENDP

bpx_thunk_23 PROC
    mov r10, 23
    jmp bpx_thunk_common
bpx_thunk_23 ENDP

bpx_thunk_24 PROC
    mov r10, 24
    jmp bpx_thunk_common
bpx_thunk_24 ENDP

bpx_thunk_25 PROC
    mov r10, 25
    jmp bpx_thunk_common
bpx_thunk_25 ENDP

bpx_thunk_26 PROC
    mov r10, 26
    jmp bpx_thunk_common
bpx_thunk_26 ENDP

bpx_thunk_27 PROC
    mov r10, 27
    jmp bpx_thunk_common
bpx_thunk_27 ENDP

bpx_thunk_28 PROC
    mov r10, 28
    jmp bpx_thunk_common
bpx_thunk_28 ENDP

bpx_thunk_29 PROC
    mov r10, 29
    jmp bpx_thunk_common
bpx_thunk_29 ENDP

bpx_thunk_30 PROC
    mov r10, 30
    jmp bpx_thunk_common
bpx_thunk_30 ENDP

bpx_thunk_31 PROC
    mov r10, 31
    jmp bpx_thunk_common
bpx_thunk_31 ENDP

bpx_thunk_32 PROC
    mov r10, 32
    jmp bpx_thunk_common
bpx_thunk_32 ENDP

bpx_thunk_33 PROC
    mov r10, 33
    jmp bpx_thunk_common
bpx_thunk_33 ENDP

bpx_thunk_34 PROC
    mov r10, 34
    jmp bpx_thunk_common
bpx_thunk_34 ENDP

bpx_thunk_35 PROC
    mov r10, 35
    jmp bpx_thunk_common
bpx_thunk_35 ENDP

bpx_thunk_36 PROC
    mov r10, 36
    jmp bpx_thunk_common
bpx_thunk_36 ENDP

bpx_thunk_37 PROC
    mov r10, 37
    jmp bpx_thunk_common
bpx_thunk_37 ENDP

bpx_thunk_38 PROC
    mov r10, 38
    jmp bpx_thunk_common
bpx_thunk_38 ENDP

bpx_thunk_39 PROC
    mov r10, 39
    jmp bpx_thunk_common
bpx_thunk_39 ENDP

bpx_thunk_40 PROC
    mov r10, 40
    jmp bpx_thunk_common
bpx_thunk_40 ENDP

bpx_thunk_41 PROC
    mov r10, 41
    jmp bpx_thunk_common
bpx_thunk_41 ENDP

bpx_thunk_42 PROC
    mov r10, 42
    jmp bpx_thunk_common
bpx_thunk_42 ENDP

bpx_thunk_43 PROC
    mov r10, 43
    jmp bpx_thunk_common
bpx_thunk_43 ENDP

bpx_thunk_44 PROC
    mov r10, 44
    jmp bpx_thunk_common
bpx_thunk_44 ENDP

bpx_thunk_45 PROC
    mov r10, 45
    jmp bpx_thunk_common
bpx_thunk_45 ENDP

bpx_thunk_46 PROC
    mov r10, 46
    jmp bpx_thunk_common
bpx_thunk_46 ENDP

bpx_thunk_47 PROC
    mov r10, 47
    jmp bpx_thunk_common
bpx_thunk_47 ENDP

bpx_thunk_48 PROC
    mov r10, 48
    jmp bpx_thunk_common
bpx_thunk_48 ENDP

bpx_thunk_49 PROC
    mov r10, 49
    jmp bpx_thunk_common
bpx_thunk_49 ENDP

bpx_thunk_50 PROC
    mov r10, 50
    jmp bpx_thunk_common
bpx_thunk_50 ENDP

bpx_thunk_51 PROC
    mov r10, 51
    jmp bpx_thunk_common
bpx_thunk_51 ENDP

bpx_thunk_52 PROC
    mov r10, 52
    jmp bpx_thunk_common
bpx_thunk_52 ENDP

bpx_thunk_53 PROC
    mov r10, 53
    jmp bpx_thunk_common
bpx_thunk_53 ENDP

bpx_thunk_54 PROC
    mov r10, 54
    jmp bpx_thunk_common
bpx_thunk_54 ENDP

bpx_thunk_55 PROC
    mov r10, 55
    jmp bpx_thunk_common
bpx_thunk_55 ENDP

bpx_thunk_56 PROC
    mov r10, 56
    jmp bpx_thunk_common
bpx_thunk_56 ENDP

bpx_thunk_57 PROC
    mov r10, 57
    jmp bpx_thunk_common
bpx_thunk_57 ENDP

bpx_thunk_58 PROC
    mov r10, 58
    jmp bpx_thunk_common
bpx_thunk_58 ENDP

bpx_thunk_59 PROC
    mov r10, 59
    jmp bpx_thunk_common
bpx_thunk_59 ENDP

bpx_thunk_60 PROC
    mov r10, 60
    jmp bpx_thunk_common
bpx_thunk_60 ENDP

bpx_thunk_61 PROC
    mov r10, 61
    jmp bpx_thunk_common
bpx_thunk_61 ENDP

bpx_thunk_62 PROC
    mov r10, 62
    jmp bpx_thunk_common
bpx_thunk_62 ENDP

bpx_thunk_63 PROC
    mov r10, 63
    jmp bpx_thunk_common
bpx_thunk_63 ENDP

.const
PUBLIC bpx_thunk_table
bpx_thunk_table LABEL QWORD
    DQ bpx_thunk_0, bpx_thunk_1, bpx_thunk_2, bpx_thunk_3, bpx_thunk_4, bpx_thunk_5, bpx_thunk_6, bpx_thunk_7
    DQ bpx_thunk_8, bpx_thunk_9, bpx_thunk_10, bpx_thunk_11, bpx_thunk_12, bpx_thunk_13, bpx_thunk_14, bpx_thunk_15
    DQ bpx_thunk_16, bpx_thunk_17, bpx_thunk_18, bpx_thunk_19, bpx_thunk_20, bpx_thunk_21, bpx_thunk_22, bpx_thunk_23
    DQ bpx_thunk_24, bpx_thunk_25, bpx_thunk_26, bpx_thunk_27, bpx_thunk_28, bpx_thunk_29, bpx_thunk_30, bpx_thunk_31
    DQ bpx_thunk_32, bpx_thunk_33, bpx_thunk_34, bpx_thunk_35, bpx_thunk_36, bpx_thunk_37, bpx_thunk_38, bpx_thunk_39
    DQ bpx_thunk_40, bpx_thunk_41, bpx_thunk_42, bpx_thunk_43, bpx_thunk_44, bpx_thunk_45, bpx_thunk_46, bpx_thunk_47
    DQ bpx_thunk_48, bpx_thunk_49, bpx_thunk_50, bpx_thunk_51, bpx_thunk_52, bpx_thunk_53, bpx_thunk_54, bpx_thunk_55
    DQ bpx_thunk_56, bpx_thunk_57, bpx_thunk_58, bpx_thunk_59, bpx_thunk_60, bpx_thunk_61, bpx_thunk_62, bpx_thunk_63

END
