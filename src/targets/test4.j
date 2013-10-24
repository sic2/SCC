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
	iconst_1
	getstatic java/lang/System/out Ljava/io/PrintStream;
	iconst_2
	invokevirtual java/io/PrintStream/println(I)V
	return	; return from main
.end method