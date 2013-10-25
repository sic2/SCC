.class public simple
.super java/lang/Object
.method public <init>()V
	aload_0
	invokespecial java/lang/Object/<init>()V
	return
.end method

	.method public static LESS_THAN_SUBROUTINE(II)Z
.limit stack 2
.limit locals 2
	 iload_0
	 iload_1
	 if_icmpge Label_NT
	 iconst_1
	 goto Label_T
 Label_NT:
	 iconst_0
 Label_T:
	 ireturn
.end method
.method public static main([Ljava/lang/String;)V
	.limit stack 5
	.limit locals 100
	getstatic java/lang/System/out Ljava/io/PrintStream;
	iconst_1
	iconst_2
	invokestatic simple.LESS_THAN_SUBROUTINE(II)Z
	 istore_0
	 iload_0
	invokevirtual java/io/PrintStream/println(Z)V
	return	; return from main
.end method