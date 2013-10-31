.class public s0Test1
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
	ldc "1"
	ldc "0"
	 invokevirtual java/lang/String.equals(Ljava/lang/Object;)Z 
	 ifeq Label_0
	iconst_1
goto def 
Label_0:
	ldc "1"
	ldc "1"
	 invokevirtual java/lang/String.equals(Ljava/lang/Object;)Z 
	 ifeq Label_1
	iconst_0
goto def 
Label_1:
iconst_m1
def: 
	invokevirtual java/io/PrintStream/println(I)V
	return	; return from main
.end method
