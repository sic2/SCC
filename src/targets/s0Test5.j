.class public s0Test5
.super java/lang/Object
.field static public ENV I
.method public <init>()V
	aload 0
	invokespecial java/lang/Object/<init>()V
	return
.end method

.method public static AND_SUBROUTINE(ZZ)Z ; START SUBROUTINE
.limit stack 2 
.limit locals 2 
	 iload 0 
	 ifeq Label_T 
	 iload 1 
	 ifeq Label_T 
	 iconst_1 
	 goto Label_R 
 Label_T:
	 iconst_0 
 Label_R:
	 ireturn 
.end method ; END SUBROUTINE

.method public static main([Ljava/lang/String;)V
	.limit stack 10
	.limit locals 100
	getstatic java/lang/System/out Ljava/io/PrintStream;
	iconst_1
	iconst_0
	invokestatic s0Test5.AND_SUBROUTINE(ZZ)Z
	invokevirtual java/io/PrintStream/println(Z)V
	return	; return from main
.end method
