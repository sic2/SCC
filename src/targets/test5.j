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
getstatic java/lang/System/out Ljava/io/PrintStream;
	iconst_1
	return
Label_1:
getstatic java/lang/System/out Ljava/io/PrintStream;
	iconst_0
	return
DLABEL:
	return
	return	; return from main
.end method