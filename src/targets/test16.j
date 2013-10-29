.class public simple
.super java/lang/Object
.method public <init>()V
	aload 0
	invokespecial java/lang/Object/<init>()V
	return
.end method

.method public static main([Ljava/lang/String;)V
	.limit stack 5
	.limit locals 100
	getstatic java/lang/System/out Ljava/io/PrintStream;

	iconst_0
	istore_0

	 ldc "1"
	 ldc "0"
	 invokevirtual java/lang/String.equals(Ljava/lang/Object;)Z
	 ifeq Label_0
	iconst_1
	istore 0
Label_0:
	 ldc "1"
	 ldc "1"
	 invokevirtual java/lang/String.equals(Ljava/lang/Object;)Z
	 ifeq Label_1
	iconst_0
	istore 0
Label_1:
	iload 0
	invokevirtual java/io/PrintStream/println(I)V
	return	; return from main
.end method