  ;; Copyright (C) <2014>  <@RKX1209> 

  ;; This program is free software: you can redistribute it and/or modify
  ;; it under the terms of the GNU General Public License as published by
  ;; the Free Software Foundation; either version 3 of the License, or
  ;; (at your option) any later version.

  ;; This program is distributed in the hope that it will be useful,
  ;; but WITHOUT ANY WARRANTY; without even the implied warranty of
  ;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  ;; GNU General Public License for more details.

  ;; You should have received a copy of the GNU General Public License
  ;; along with this program.  If not, see <http://www.gnu.org/licenses/>

	
;------------------------------------------------ 
; Boot Loader stage2 (loaded by stage1 loader ) |
;------------------------------------------------


KERNEL	EQU		0x00280000	
DSKCDST	EQU		0x00100000
DSKCSRC	EQU		0x00008000
	
VBEMODE	EQU		0x105
CYLS	EQU		0x0ff0
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2
SCRNX	EQU		0x0ff4
SCRNY	EQU		0x0ff6
VRAM	EQU		0x0ff8
	
ORG	0x08200
	
JMP	boot2

boot2:
	;; Init registers
	XOR	AX,AX
	XOR	BX,BX
	XOR	CX,CX
	XOR	DX,DX
	MOV	DS,AX
	MOV	ES,AX

	MOV	AX,0x9000
	MOV	SS,AX
	MOV	SP,0xfffc

	;; ;; VBE(?)
	;; MOV		AX,0x9000
	;; MOV		ES,AX
	;; MOV		DI,0
	;; MOV		AX,0x4f00
	;; INT		0x10
	;; CMP		AX,0x004f
	;; JNE		scrn320

	;; ;; VBE version(?)

	;; MOV		AX,[ES:DI+4]
	;; CMP		AX,0x0200
	;; JB		scrn320			; if (AX < 0x0200) goto scrn320

	;; ;; Monitor mode(?)
	;; MOV		CX,VBEMODE
	;; MOV		AX,0x4f01
	;; INT		0x10
	;; CMP		AX,0x004f
	;; JNE		scrn320

	;; ;; Confirm monitor mode(?)

	;; CMP		BYTE [ES:DI+0x19],8
	;; JNE		scrn320
	;; CMP		BYTE [ES:DI+0x1b],4
	;; JNE		scrn320
	;; MOV		AX,[ES:DI+0x00]
	;; AND		AX,0x0080
	;; JZ		scrn320

	;; ;; Change monitor mode(?)

	;; MOV		BX,VBEMODE+0x4000
	;; MOV		AX,0x4f02
	;; INT		0x10
	;; MOV		BYTE [VMODE],8
	;; MOV		AX,[ES:DI+0x12]
	;; MOV		[SCRNX],AX
	;; MOV		AX,[ES:DI+0x14]
	;; MOV		[SCRNY],AX
	;; MOV		EAX,[ES:DI+0x28]
	;; MOV		[VRAM],EAX
	;; JMP		keystatus

scrn320:
	MOV		AL,0x13
	MOV		AH,0x00
	INT		0x10
	MOV		BYTE [VMODE],8
	MOV		WORD [SCRNX],320
	MOV		WORD [SCRNY],200
	MOV		DWORD [VRAM],0x000a0000

	;; Get keyboard LED status using BIOS
keystatus:
	MOV		AH,0x02
	INT		0x16 			; keyboard BIOS
	MOV		[LEDS],AL

	;; PIC Init(?)
	MOV		AL,0xff
	OUT		0x21,AL
	NOP
	OUT		0xa1,AL
	CLI

	;; enable A20
	CALL	waitkbdout
	MOV	AL,0xd1
	OUT	0x64,AL
	CALL	waitkbdout
	MOV	AL,0xdf
	OUT	0x60,AL
	CALL	waitkbdout

	;; set gdt
	CALL	set_gdt

	;; Enter protected mode
	MOV	EAX,CR0
	AND	EAX,0x7fffffff	
	OR	EAX,0x00000001	
	MOV	CR0,EAX
	JMP	pipelineflash

pipelineflash:
	MOV		AX,1*8
	MOV		DS,AX
	MOV		ES,AX
	MOV		FS,AX
	MOV		GS,AX
	MOV		SS,AX

	;; Transfer kernel_entry to "KERNEL"
	MOV		ESI,kernel_entry
	MOV		EDI,KERNEL
	MOV		ECX,512*1024/4
	CALL		memcpy
	
	;; Transfer disk image to "DSKCDST" (Memory maped)
	
	;; 	[*] Transfer bootsector
	MOV		ESI,0x7c00
	MOV		EDI,DSKCDST
	MOV		ECX,512/4
	CALL		memcpy

	;; 	[*] Transfer image body
	MOV		ESI,DSKCSRC+512	
	MOV		EDI,DSKCDST+512
	MOV		ECX,0
	MOV		CL,BYTE [CYLS]
	IMUL	ECX,512*18*2/4
	SUB		ECX,512/4
	CALL	memcpy

	;; Boot kernel
	JMP	DWORD 2*8:0x00000000
	;; JMP	0x280000
FIN:
	HLT
	JMP	FIN
	
	
set_gdt:
	CLI
	PUSHA
	LGDT	[gdt_info]	
	STI
	POPA
	RET

waitkbdout:
	IN		 AL,0x64
	AND		 AL,0x02
	IN		 AL,0x60
	JNZ		waitkbdout
	RET
	
memcpy:
	MOV	EAX,[ESI]
	ADD	ESI,4
	MOV	[EDI],EAX
	ADD	EDI,4
	SUB	ECX,1
	JNZ	memcpy
	RET
	
gdt:
	RESB	8
	DW		0xffff,0x0000,0x9200,0x00cf
	DW		0xffff,0x0000,0x9a28,0x0047
	DW		0	
gdt_info:
	DW	8*3-1
	DD	gdt
	
kernel_entry:	
