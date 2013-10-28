.class public simple
.super java/lang/Object
.method public <init>()V
	aload_0
	invokespecial java/lang/Object/<init>()V
	return
.end method

.method public static main([Ljava/lang/String;)V
	.limit stack 5
	.limit locals 100
	getstatic java/lang/System/out Ljava/io/PrintStream;

	new ADTByteCode ; create new generic object for type Time
	dup
	invokespecial ADTByteCode.<init>()V
	astore_0
	aload_0
	bipush 10
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set typeID tag
	aload_0
	ldc "Time"
	putfield ADTByteCode/typeTag Ljava/lang/String;
; Set constructorID tag
	aload_0
	ldc "Min"
	putfield ADTByteCode/constrTag Ljava/lang/String;
; loading expr type 0 to Time Min

	new ADTByteCode ; create new generic object for type Int
	dup
	invokespecial ADTByteCode.<init>()V
	astore_1
	aload_1
	bipush 10
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
; Set typeID tag
	aload_1
	ldc "Int"
	putfield ADTByteCode/typeTag Ljava/lang/String;
; Set constructorID tag
	aload_1
	ldc "Int"
	putfield ADTByteCode/constrTag Ljava/lang/String;
	aload_1
	iconst_0
	putfield ADTByteCode/intVal I
; LOAD OBJ 1 to array of objs in 0 in position 0
aload_0
getfield ADTByteCode/objs [LADTByteCode;
iconst_0
aload_1
aastore


aload_0
	getfield ADTByteCode/objs [LADTByteCode;
	iconst_0
	aaload
	getfield ADTByteCode/intVal I
	istore_3


	iload_3
	invokevirtual java/io/PrintStream/println(I)V


	return	; return from main
.end method