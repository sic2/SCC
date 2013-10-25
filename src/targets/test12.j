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
	 istore_0
	iload_0
	lookupswitch
		0 : Label_0
		1 : Label_1
		default : DLABEL
Label_0:
	iconst_1
	 istore_0
	goto DLABEL
Label_1:
	iconst_0
	 istore_0
	goto DLABEL
DLABEL:
	 iload_0
	invokevirtual java/io/PrintStream/println(I)V
	return	; return from main
.end method