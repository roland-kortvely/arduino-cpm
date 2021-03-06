;MASTER CATALOG UPDATE - UCAT.ASM
;01/07/78 BY WARD CHRISTENSEN
;01/10/78 ADD:
; 01/12/81 W. EARNEST MOD FOR CPM 2.X,
; ADD USER NR FIELD SUPPORT.
;		(RUN UCAT)
;		ERA MAST.BAK
;		REN MAST.BAK=MAST.CAT
;		REN MAST.CAT=NEW.CAT
;		ERA NAMES.SUB
	ORG	100H
BSIZE	EQU	4096	;DK BUF SIZE (TIMES 3)
IGNSIZE	EQU	1024	;BUFF FOR IGNORED NAMES
DFCB	EQU	5CH	;CMD FILE NAME
TEST	EQU	0	;TESTING?
	MACLIB	SEQIO
FILERR	SET	EXIT	;EXIT IF ERRORS
MOVE	MACRO	?F,?T,?L
	MVI	B,?L
	LXI	D,?F
	LXI	H,?T
	CALL	MOVER
	ENDM
COMPARE	MACRO	?F1,?F2,?L
	MVI	B,?L
	LXI	D,?F1
	LXI	H,?F2
	CALL	COMPR
	ENDM
;
;SAVE CP/M'S STACK
;
	POP	H	;GET CP/M RET ADDR
	SHLD	EXIT+1	;SAVE IN EXIT JMP
	LXI	SP,STACK
	FILE	INFILE,MASTIN,,MAST,CAT,BSIZE
	FILE	INFILE,NAMES,,NAMES,SUB,BSIZE
	IF	NOT TEST
	FILE	OUTFILE,MASTOUT,,NEW,CAT,BSIZE
	ENDIF
	FILE	SETFILE,BAK,,MAST,BAK
;
;SETUP THE FILES
;
INITRD	CALL	READNA	;READ NAMES FILE
;
;IF NAME IS $$$.SUB THEN SKIP IT
;
	COMPARE	NADAT,SUBNAME,12
	JZ	INITRD
;
;CHECK FOR '-DISKNAME' HAVING BEEN READ
;
	LDA	NADAT
	CPI	'-'
	JZ	NAMEOK
	LDA	DFCB+2
	CPI	'-'
	JNZ	NODASH
	LDA	DFCB+10
	CPI	' '
	JNZ	CMDNAM
	CALL	MSGXIT
	DB	'++SERIAL MUST BE 3 DIGITS$'
CMDNAM	MOVE	DFCB+2,DKNAME,7
	MOVE	DFCB+8,DKNAME+9,3
	JMP	GETIGN
;
NODASH	CALL	MSGXIT
	DB	'++NO ''-NAME.NNN'' IN NAMES.SUB'
	DB	0DH,0AH
	DB	'++REISSUE: UCAT -NAME.NNN$'
;
NAMEOK	MOVE	NADAT+1,DKNAME,7
	MOVE	NADAT+9,DKNAME+9,3
;
;READ IN THE NAMES TO BE IGNORED (I.E. NOT
;CATALOGED).  THEY ARE AT THE FRONT OF MAST CAT.
;THEY ARE SIMPLY A LIST OF FILENAME.FILETYPE (CR/LF)
;WITH THE FIRST HAVING A ( BEFORE IT, AND THE
;LAST HAVING A ) AFTER IT.
;
GETIGN	GET	MASTIN	;GET THE LEADING '('
	JZ	NOIGN	;IF EOF
	CPI	'('
	JNZ	NOIGN
	PUT	MASTOUT
	LXI	H,IGNORE ;POINT TO BUFFER
	LXI	B,IGNSIZE ;FOR BUFFER OVERFLOW TEST
IGNRD	PUSH	B
	PUSH	H
	GET	MASTIN
	JZ	IGNEOF	;UNEXPECTED EOF
	PUSH	PSW	;SAVE CHAR
	PUT	MASTOUT
	POP	PSW
	POP	H
	POP	B
	MOV	M,A
	INX	H
	CPI	')'
	JZ	IGNEND	;TABLE IS LOADED
	DCX	B	;MORE ROOM IN TABLE?
	MOV	A,B
	ORA	C
	JNZ	IGNRD
;TABLE OVERFLOW
	CALL	MSGXIT
	DB	'++TOO MANY IGNORE NAMES FOR TABLE$'
NOIGN	CALL	MSGXIT
	DB	'++NO IGNORE NAMES IN MAST.CAT$'
IGNEOF	CALL	MSGXIT
	DB	'++EOF READING FOR IGNORE NAMES.'
	DB	0DH,0AH,'MAY BE MISSING ) AFTER LAST NAME.$'
IGNEND	GET	MASTIN
	JZ	IGNEOF
	PUSH	PSW
	PUT	MASTOUT
	POP	PSW
	CPI	0AH
	JNZ	IGNEND	;DELETE CR/LF
	JMP	MASTLP	;LEAVE DISK NR IN PLACE
;PRIME THE BUFFERS
;
NAMELP	CALL	READNA	;READ NAME INTO NADAT
MASTLP	CALL	READMI	;READ MASTER INTO MIDAT
;IF EOF ON BOTH FILES, WE ARE ALL DONE
COMPLP	LDA	NAEOFLG	;NAME FILE EOF?
	ORA	A
	JZ	NOTEOF	;..NO
	LDA	MIEOFLG	;MASTER IN EOF?
	ORA	A
	JNZ	ALLDONE	;YES, THAT'S IT.
NOTEOF	LDA	NADAT
	MOV	B,A
	LDA	MIDAT
	CMP	B
	JNZ	NOTE2	;CAN'T BE REPLACE
	CPI	'['
	JNZ	NOTE2	;NOT SPACE LEFT NAME
	COMPARE NADAT+9,MIDAT+9,18 ;JUST CK TYPE
	JMP	NOTE3	;GO WITH THE FLAGS
;
NOTE2	COMPARE	NADAT,MIDAT,27
NOTE3	JZ	EQUAL
	JC	WRITEN
;
;MASTER IS LOWER - WRITE IT IF FOR ANOTHER DISK
;
	COMPARE	MIDSK,DKNAME,12 ;SAME DISK?
	JZ	DELMI	;DELETING MIDAT
	MOVE	MIDAT,MODAT,27
	CALL	WRITEMO
	JMP	MASTLP
;
;DELETE MASTER IN
;
DELMI	LXI	D,DELMSG
	LXI	H,MIDAT
	CALL	MESG
	JMP	MASTLP
DELMSG	DB	'DEL: $'
;
;NAME IS LOWER - WRITE IT
;
WRITEN	MOVE	NADAT,MODAT,27
	CALL	WRITEMO
;
;PRINT THAT NAME WAS ADDED
;
	LXI	D,ADDMSG
	LXI	H,NADAT
	CALL	MESG
	CALL	READNA	;READ NEXT NAME
	JMP	COMPLP
ADDMSG	DB	'ADD: $'
;
;BOTH FILES EQUAL
;
EQUAL	MOVE	NADAT,MODAT,27
	CALL	WRITEMO	;WRITE OUT MASTER
	JMP	NAMELP	;READ