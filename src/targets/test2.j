.class public simple
.super java/lang/Object
.method public <init>()V
  aload_0
  invokespecial java/lang/Object/<init>()V
  return
.end method

.method public static TEST()V
  .limit stack 5
  .limit locals 100
    iconst_3
    istore_0
    iconst_4
    istore_1
    iload_0
    iload_1
    iadd
    istore_2
    return
.end method

.method public static main([Ljava/lang/String;)V
  .limit stack 5
  .limit locals 100
  invokestatic simple.TEST()V
  return
.end method