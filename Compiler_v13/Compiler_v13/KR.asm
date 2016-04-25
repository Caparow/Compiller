Data1 Segment
    BinCon1    		db 			10011b
    BinCon2 		db	  		11101b
    StrConst		db 			"Test string или кирилица."
    DecConst		dw 			4567d
    HexConst		dd 			7856fdh
Data1 ends

dfg  equ 1
dfg1 equ 0

Code Segment
	nop
	mov ah, 0
	inc ah
	mov eax, dfg
	inc eax
	dec byte ptr es:[edx+esi+6]

label1:
	add eax, esi
	IF dfg1
		add ah, bh
	ELSE
		or dword ptr [edx+esi+6], 123h
	ENDIF
	cmp eax, [edx+esi+12]
	jb label1
	jb label2

label2:
	IF dfg
		and dword ptr [edx+esi+6], eax
	ELSE
		or dword ptr [edx+esi+6], 123h
		cmp ah, bh
		jb label2
	ENDIF 

Code ends
	END