.class public simple
.super java/lang/Object
.method public <init>()V
	aload_0
	invokespecial java/lang/Object/<init>()V
	return
.end method

.method public static AND_SUBROUTINE(ZZ)Z ; START SUBROUTINE
.limit stack 2
.limit locals 2
	 iload_0
	 ifeq Label_T
	 iload_1
	 ifeq Label_T
	 iconst_1
	 goto Label_R
 Label_T:
	 iconst_0
 Label_R:
	 ireturn
.end method ; END SUBROUTINE

.method public static main([Ljava/lang/String;)V
	.limit stack 5
	.limit locals 100
	getstatic java/lang/System/out Ljava/io/PrintStream;
	iconst_1
	istore_0
	iload_0
	iconst_1
	istore_1
	iload_1
	invokestatic simple.AND_SUBROUTINE(ZZ)Z
	istore_1
	iload_1
	invokevirtual java/io/PrintStream/println(Z)V
	return	; return from main
.end method