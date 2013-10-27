.source Jasminclass.j
.class public Jasminclass
.super java/lang/Object

.field public testField I
.field public testField Ljava/lang/String;

.method public <init>()V
.limit stack 5
.limit locals 100
	aload_0
	invokespecial java/lang/Object/<init>()V
	aload_0

	iconst_2
	putfield Jasminclass/testField I

	return
.end method