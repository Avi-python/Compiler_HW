;************** test1.asm ****************
;
	ORG	100H
	JMP	_start1
_intstr	DB	'     ','$'
_buf	TIMES 256 DB ' '
	DB 13,10,'$'
%include	"dispstr.mac"
%include	"itostr.mac"
%include	"readstr.mac"
%include	"strtoi.mac"
%include	"newline.mac"
test1_a	DW	0
test1_b	DW	0
test1_c	DW	0
_start1:
	PUSH	5
	POP	AX
	MOV	[test1_a], AX
	PUSH	2
	POP	AX
	MOV	[test1_b], AX
	PUSH	WORD [test1_a]
	PUSH	WORD [test1_b]
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go2
	JMP	_go3
_go2:
	PUSH	WORD [test1_a]
	PUSH	WORD [test1_b]
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go4
	JMP	_go5
_go4:
_go5:
	itostr	test1_c, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	MOV	AX, 4C00H
	INT	21H
