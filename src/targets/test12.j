.source test12.j
.class public simple
.super java/lang/Object
.method public <init>()V
.limit stack 5
.limit locals 100
	aload_0
	invokespecial java/lang/Object/<init>()V
	return
.end method

.method public static main([Ljava/lang/String;)V
	.limit stack 5
	.limit locals 100
	new Jasminclass
	dup
	invokespecial Jasminclass.<init>()V
	astore_1

	aload_1
	getfield Jasminclass/testField I
	istore_2

	getstatic java/lang/System/out Ljava/io/PrintStream;
	iload_2
	invokevirtual java/io/PrintStream/println(I)V

	return	; return from main
.end method