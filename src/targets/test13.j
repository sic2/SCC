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

	new ADTByteCode ; create new generic object for type Person
	dup
	invokespecial ADTByteCode.<init>()V
	astore_0
	aload_0
	bipush 10
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;

	new ADTByteCode ; create new generic object for type Age
	dup
	invokespecial ADTByteCode.<init>()V
	astore_1
	aload_1
	bipush 10
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
	aload_1
	iconst_2
	putfield ADTByteCode/intVal I
; LOAD OBJ 1 to array of objs in 0
aload_0
getfield ADTByteCode/objs [LADTByteCode;
iconst_0
aload_1
aastore

	new ADTByteCode ; create new generic object for type Address
	dup
	invokespecial ADTByteCode.<init>()V
	astore_2
	aload_2
	bipush 10
	anewarray ADTByteCode
	putfield ADTByteCode/objs [LADTByteCode;
	aload_2
	iconst_0
	putfield ADTByteCode/intVal I
	aload_2
	ldc "String"
	putfield ADTByteCode/strVal Ljava/lang/String;
; LOAD OBJ 2 to array of objs in 0
aload_0
getfield ADTByteCode/objs [LADTByteCode;
iconst_1
aload_2
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