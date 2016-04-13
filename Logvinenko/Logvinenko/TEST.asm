Data1	segment
str3	db	"wbvVDSwb итд"
vByte1	db	23h
vWord1	dw	23dah
vDWord1	dd	756fd5h
Data1	ends

deb	equ	1

Code	segment
dt1	db	33h
st1:
	if	deb
	dec	eax
	endif
	stosd
	inc	vByte1[ebx*2]
	cmp	eax, ecx
	mov	al, cs:dt2
	or	ah, str3[edx*4]
	jz	st2
	and	vDWord1[eax*2], edx
	mov	esi, vDWord1[ebp*4]
	and	vDWord1[eax*2], edx
st2:
	add	vByte1[ebx*2], 3h
	jz	st1
dt2	db	0h
Code	ends
	end	