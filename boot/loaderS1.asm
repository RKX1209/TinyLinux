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

	
;-------------------------------------- 
; Boot Loader stage1 (located in MBR) |
;--------------------------------------

	
RETRY	EQU	5
CYLS	EQU	10
KERNEL	EQU	0x0820	

	
ORG	0x7c00 			;Boot loader location
	
		; FAT12 file system
JMP	boot
DB	0x90
BS_Name		DB	"ABYON   "
BS_BytePerSec	DW	512
BS_ClSize	DB	1
BS_FATentry	DW	1
BS_FATnum	DB	2
BS_RootdSize	DW	224
BS_DriveSize	DW	2880
BS_MediaType	DB	0xf0
BS_FATsize	DW	9
BS_SecPerTr	DW	18
BS_HeadNum	DW	2
		DW	0
		DD	2880
		DB	0,0,0x29
		DD	0xffffffff
BS_DiskName	DB	"ABYON      "
BS_FileSystem	DB	"FAT12   "
		TIMES	18	DB 0

boot:
	;; Init registers
	XOR	AX,AX
	MOV	SS,AX
	MOV	SP,0x7c00
	MOV	DS,AX

	MOV	AX,KERNEL
	MOV	ES,AX
	MOV	CH,0		; cylinder 0
	MOV	DH,0		; head 0	
	MOV	CL,2		; sector 2

readloop:
	MOV	SI,0
read:
	MOV	AH,0x02
	MOV	AL,1
	MOV	BX,0
	MOV	DL,0x00
	INT	0x13
	JNC	readnext
	ADD	SI,1
	CMP	SI,RETRY
	JAE	readfail
	MOV	AH,0x00
	MOV	DL,0x00
	INT	0x13
	JMP	read
	
readnext:
	MOV	AX,ES
	ADD	AX,0x0020
	MOV	ES,AX
	ADD	CL,1
	CMP	CL,18
	JBE	readloop
	MOV	CL,1
	ADD	DH,1
	CMP	DH,2
	JB	readloop
	MOV	DH,0
	ADD	CH,1
	CMP	CH,CYLS
	JB	readloop

	MOV	[0x0ff0],CH
	JMP	KERNEL:0x0

readfail:
	MOV		AX,0
	MOV		ES,AX
	MOV		SI,msg
putloop:
	MOV		AL,[SI]
	ADD		SI,1			
	CMP		AL,0
	JE		fin
	MOV		AH,0x0e			
	MOV		BX,15			
	INT		0x10			
	JMP		putloop
fin:
	HLT				
	JMP		fin		
msg:
	DB		0x0a, 0x0a
	DB		"load error"
	DB		0x0a
	DB		0	
	
TIMES	510 - ($ - $$) DB 0
	
DW 0xAA55	
