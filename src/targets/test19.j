.class public s0Test1
.super java/lang/Object
.field static public ENV I
.method public <init>()V
	aload 0
	invokespecial java/lang/Object/<init>()V
	return
.end method

.method public static ADD_SUBROUTINE(II)I ; START SUBROUTINE
.limit stack 2 
.limit locals 2 
	 iload 0 
	 iload 1 
	 iadd
	 ireturn 
.end method ; END SUBROUTINE

.method public static main([Ljava/lang/String;)V
	.limit stack 10
	.limit locals 100
	getstatic java/lang/System/out Ljava/io/PrintStream;

	new ADTByteCode ; create new generic object for type 
	dup 
	invokespecial ADTByteCode.<init>()V 
	astore 0
	aload 0
	iconst_1
	putfield ADTByteCode/noObjs I 
	aload 0
	bipush 1
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set constructorID tag
	aload 0
	ldc "x"
	putfield ADTByteCode/constrTag Ljava/lang/String; 
; loading expr type 0 to  x

	new ADTByteCode ; create new generic object for type Int
	dup 
	invokespecial ADTByteCode.<init>()V 
	astore 1
	aload 1
	iconst_0
	putfield ADTByteCode/noObjs I 
	aload 1
	bipush 0
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set constructorID tag
	aload 1
	ldc "Int"
	putfield ADTByteCode/constrTag Ljava/lang/String; 
; Set up value int iconst_3
	aload 1
	iconst_3
	putfield ADTByteCode/intVal I 
	aload 1
getfield ADTByteCode/intVal I
; LOAD OBJ 1 to array of objs in 0 in position 0
aload 0
getfield ADTByteCode/objs [LADTByteCode; 
iconst_0
aload 1
aastore

	new ADTByteCode ; create new generic object for type 
	dup 
	invokespecial ADTByteCode.<init>()V 
	astore 2
	aload 2
	iconst_1
	putfield ADTByteCode/noObjs I 
	aload 2
	bipush 1
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set constructorID tag
	aload 2
	ldc "y"
	putfield ADTByteCode/constrTag Ljava/lang/String; 
; loading expr type 0 to  y

	new ADTByteCode ; create new generic object for type Int
	dup 
	invokespecial ADTByteCode.<init>()V 
	astore 3
	aload 3
	iconst_0
	putfield ADTByteCode/noObjs I 
	aload 3
	bipush 0
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set constructorID tag
	aload 3
	ldc "Int"
	putfield ADTByteCode/constrTag Ljava/lang/String; 
; Set up value int iconst_3
	aload 3
	iconst_3
	putfield ADTByteCode/intVal I 
	aload 3
getfield ADTByteCode/intVal I
; LOAD OBJ 3 to array of objs in 2 in position 0
aload 2
getfield ADTByteCode/objs [LADTByteCode; 
iconst_0
aload 3
aastore
	invokestatic s0Test1.ADD_SUBROUTINE(II)I
	invokevirtual java/io/PrintStream/println(I)V
	return	; return from main
.end method