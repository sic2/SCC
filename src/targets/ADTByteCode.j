.source ADTByteCode.j 
.class public ADTByteCode 
.super java/lang/Object 

.field public noObjs I 
.field public constrTag Ljava/lang/String; 
.field public objs [LADTByteCode; 
.field public intVal I 
.field public boolVal Z 
.field public strVal Ljava/lang/String; 
.method public COMPARE(LADTByteCode;)Z 
.limit stack 5 
.limit locals 10 
aload 0 
getfield LADTByteCode/noObjs I 
aload 1 
getfield LADTByteCode/noObjs I 
if_icmpne NT 
aload 0 
getfield LADTByteCode/constrTag Ljava/lang/String; 
aload 1 
getfield LADTByteCode/constrTag Ljava/lang/String; 
invokevirtual java/lang/String.equals(Ljava/lang/Object;)Z 
ifeq NT 
iconst_1 
ireturn 
NT: 
iconst_0 
ireturn 
.end method ; END Compare 

.method public <init>()V 
.limit stack 5 
.limit locals 10 
	 aload 0 
	 invokespecial java/lang/Object/<init>()V 
	 return 
.end method 
