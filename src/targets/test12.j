.class public simple
.super java/lang/Object
.method public <init>()V
	aload_0
	invokespecial java/lang/Object/<init>()V
	return
.end method

.method public static SUB_SUBROUTINE(II)I ; START SUBROUTINE
.limit stack 2
.limit locals 2
	 iload_0
	 iload_1
	 isub
	 ireturn
.end method ; END SUBROUTINE

.method public static main([Ljava/lang/String;)V
	.limit stack 5
	.limit locals 100
	getstatic java/lang/System/out Ljava/io/PrintStream;
	iconst_1
	istore_0
	iload_0
	iconst_2
	istore_1
	iload_1
	invokestatic simple.SUB_SUBROUTINE(II)I
	istore_1
	iload_1
	invokevirtual java/io/PrintStream/println(I)V
	return	; return from main
.end method