.class public s1Test1
.super java/lang/Object
.field public ENV I
.method public <init>()V
	aload 0
	invokespecial java/lang/Object/<init>()V
	return
.end method

.method public static main([Ljava/lang/String;)V
	.limit stack 5
	.limit locals 100
	getstatic java/lang/System/out Ljava/io/PrintStream;

	new ADTByteCode ; create new generic object for type Person
	dup 
	invokespecial ADTByteCode.<init>()V 
	astore 0
	aload 0
	iconst_2
	putfield ADTByteCode/noObjs I 
	aload 0
	bipush 2
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set constructorID tag
	aload 0
	ldc "Person"
	putfield ADTByteCode/constrTag Ljava/lang/String; 
; loading expr type 9 to Person Person

	new ADTByteCode ; create new generic object for type Age
	dup 
	invokespecial ADTByteCode.<init>()V 
	astore 1
	aload 1
	iconst_1
	putfield ADTByteCode/noObjs I 
	aload 1
	bipush 1
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set constructorID tag
	aload 1
	ldc "Age"
	putfield ADTByteCode/constrTag Ljava/lang/String; 
; loading expr type 0 to Age Age

	new ADTByteCode ; create new generic object for type Int
	dup 
	invokespecial ADTByteCode.<init>()V 
	astore 2
	aload 2
	iconst_0
	putfield ADTByteCode/noObjs I 
	aload 2
	bipush 0
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set constructorID tag
	aload 2
	ldc "Int"
	putfield ADTByteCode/constrTag Ljava/lang/String; 
; Set up value int bipush 21
	aload 2
	bipush 21
	putfield ADTByteCode/intVal I 
; LOAD OBJ 2 to array of objs in 1 in position 0
aload 1
getfield ADTByteCode/objs [LADTByteCode; 
iconst_0
aload 2
aastore
; LOAD OBJ 2 to array of objs in 0 in position 0
aload 0
getfield ADTByteCode/objs [LADTByteCode; 
iconst_0
aload 2
aastore
; loading expr type 9 to Person Person

	new ADTByteCode ; create new generic object for type Address
	dup 
	invokespecial ADTByteCode.<init>()V 
	astore 4
	aload 4
	iconst_2
	putfield ADTByteCode/noObjs I 
	aload 4
	bipush 2
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set constructorID tag
	aload 4
	ldc "Address"
	putfield ADTByteCode/constrTag Ljava/lang/String; 
; loading expr type 0 to Address Address

	new ADTByteCode ; create new generic object for type Int
	dup 
	invokespecial ADTByteCode.<init>()V 
	astore 5
	aload 5
	iconst_0
	putfield ADTByteCode/noObjs I 
	aload 5
	bipush 0
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set constructorID tag
	aload 5
	ldc "Int"
	putfield ADTByteCode/constrTag Ljava/lang/String; 
; Set up value int iconst_0
	aload 5
	iconst_0
	putfield ADTByteCode/intVal I 
; LOAD OBJ 5 to array of objs in 4 in position 0
aload 4
getfield ADTByteCode/objs [LADTByteCode; 
iconst_0
aload 5
aastore
; loading expr type 2 to Address Address

	new ADTByteCode ; create new generic object for type String
	dup 
	invokespecial ADTByteCode.<init>()V 
	astore 6
	aload 6
	iconst_0
	putfield ADTByteCode/noObjs I 
	aload 6
	bipush 0
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set constructorID tag
	aload 6
	ldc "String"
	putfield ADTByteCode/constrTag Ljava/lang/String; 
; Set up value string "String"
	aload 6
	ldc "String"
	putfield ADTByteCode/strVal Ljava/lang/String; 
; LOAD OBJ 6 to array of objs in 4 in position 1
aload 4
getfield ADTByteCode/objs [LADTByteCode; 
iconst_1
aload 6
aastore
; LOAD OBJ 6 to array of objs in 0 in position 1
aload 0
getfield ADTByteCode/objs [LADTByteCode; 
iconst_1
aload 6
aastore
;CASE STATEMENT START 
;Start Create ALT obj 

	new ADTByteCode 
	dup 
	invokespecial ADTByteCode.<init>()V 
	astore 9
	aload 9
	iconst_2
	putfield ADTByteCode/noObjs I 
	aload 9
	bipush 2
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set constructorID tag
	aload 9
	ldc "Person"
	putfield ADTByteCode/constrTag Ljava/lang/String; 
;End Create ALT obj 
aload 0
aload 9
invokevirtual ADTByteCode.COMPARE(LADTByteCode;)Z 
	 ifeq Label_0
	iconst_1
goto def 
Label_0:
iconst_m1 
def: 
;CASE STATEMENT END 
	invokevirtual java/io/PrintStream/println(I)V
	return	; return from main
.end method