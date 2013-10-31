.class public s0Test2
.super java/lang/Object
.field static public ENV I
.method public <init>()V
	aload 0
	invokespecial java/lang/Object/<init>()V
	return
.end method

.method public static main([Ljava/lang/String;)V
	.limit stack 10
	.limit locals 100
	getstatic java/lang/System/out Ljava/io/PrintStream;
	iconst_1
	invokevirtual java/io/PrintStream/println(I)V
	return	; return from main
.end method
