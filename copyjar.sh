#!/bin/sh

cp javalib/dist/minijvm_rt.jar binary/lib/
cp javalib/dist/minijvm_rt.jar ex_lib/mobile/resfiles/

cp ex_lib/jni_gui/java/dist/gui_lib.jar binary/lib/
cp ex_lib/luaj/dist/luaj.jar binary/lib/

cp test/jni_test/java/dist/jni_test.jar binary/lib/
cp test/javalib_test/dist/minijvm_test.jar binary/lib/


