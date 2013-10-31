.class public s0Test6
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
ldc "String"
	invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V
	return	; return from main
.end method
