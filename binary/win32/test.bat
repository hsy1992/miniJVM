mini_jvm.exe -cp ../lib/minijvm_rt.jar;../lib/minijvm_test.jar test/Foo1 888
mini_jvm.exe -cp ../lib/minijvm_rt.jar;../lib/jni_test.jar test/JniTest
mini_jvm.exe -cp ../lib/minijvm_rt.jar;../lib/glfw_gui.jar test/GuiTest
mini_jvm.exe -cp ../lib/minijvm_rt.jar;../lib/glfw_gui.jar test/Light

@echo compile BpDeepTest.java
mini_jvm -cp ../lib/minijvm_rt.jar;../lib/janino.jar;../lib/commons-compiler.jar org.codehaus.janino.Compiler  ../res/BpDeepTest.java

@echo execute BpDeepTest
mini_jvm -cp ../lib/minijvm_rt.jar;../res/ BpDeepTest
pause