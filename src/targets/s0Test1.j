.class public simple
.super java/lang/Object
.field public ENV I
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

.method public static MUL_SUBROUTINE(II)I ; START SUBROUTINE
.limit stack 2 
.limit locals 2 
	 iload 0 
	 iload 1 
	 imul
	 ireturn 
.end method ; END SUBROUTINE

.method public static main([Ljava/lang/String;)V
	.limit stack 5
	.limit locals 100
	getstatic java/lang/System/out Ljava/io/PrintStream;
	iconst_0
	iconst_1
	invokestatic simple.ADD_SUBROUTINE(II)I
	iconst_2
	invokestatic simple.MUL_SUBROUTINE(II)I
	invokevirtual java/io/PrintStream/println(I)V
	return	; return from main
.end method
