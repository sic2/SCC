.class public simple
.super java/lang/Object
.method public <init>()V
	aload_0
	invokespecial java/lang/Object/<init>()V
	return
.end method

.method public static ADD_SUBROUTINE(II)I
.limit stack 2
.limit locals 2
	iload_0
	iload_1
	iadd
	ireturn
.end method

.method public static main([Ljava/lang/String;)V
	.limit stack 5
	.limit locals 100
getstatic java/lang/System/out Ljava/io/PrintStream;
	iconst_0
	iconst_1
	invokestatic simple.ADD_SUBROUTINE(II)I
istore_0
iload_0
	invokevirtual java/io/PrintStream/println(I)V
	return	; return from main
.end method