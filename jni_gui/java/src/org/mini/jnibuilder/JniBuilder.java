/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.jnibuilder;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Reader;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author gust
 */
public class JniBuilder {

    public static void main(String[] args) {
        JniBuilder gt = new JniBuilder();
        gt.build();
    }

    String[] path = {"src/org/mini/gl/GL.java", "ort_mini_gl_GL_"};

    String template
            = //
            "int ${PKG_NAME}${METHOD_NAME}(Runtime *runtime, Class *clazz) {\n"
            + "    JniEnv *env = runtime->jnienv;\n"
            + "    s32 pos = 0;\n"
            + "    \n${GET_VAR}\n"
            + "    ${RETURN_TYPE}${METHOD_NAME}(${NATIVE_ARGV});\n"
            + "    ${PUSH_RESULT}\n"
            + "    return 0;\n"
            + "}\n\n";
    String PKG_NAME = "${PKG_NAME}";
    String METHOD_NAME = "${METHOD_NAME}";
    String GET_VAR = "${GET_VAR}";
    String RETURN_TYPE = "${RETURN_TYPE}";
    String NATIVE_ARGV = "${NATIVE_ARGV}";
    String PUSH_RESULT = "${PUSH_RESULT}";

    String VOID = "void";

    void build() {
        BufferedReader br = null;
        BufferedWriter bw = null;
        try {
            File file = new File(path[0]);
            br = new BufferedReader(new FileReader(file));
            bw = new BufferedWriter(new FileWriter("./output.c"));
            String line, whole;
            String header = "public static native";
            int lineNo = 0;
            while ((line = br.readLine()) != null) {
                lineNo++;
                line = line.trim();
                whole = new String(line.getBytes());
                if (line.startsWith(header)) {

                    line = line.substring(header.length()).trim();
                    String returnType = line.substring(0, line.indexOf(' ')).trim();
                    line = line.substring(returnType.length()).trim();
                    String methodName = line.substring(0, line.indexOf('('));
                    line = line.substring(line.indexOf('(') + 1, line.indexOf(')')).trim();
                    String[] argvs = line.split(",");

                    //
                    String output = new String(template.getBytes());
                    output = output.replace(PKG_NAME, path[1]);
                    output = output.replace(METHOD_NAME, methodName);
                    //process return 
                    String returnCode = "", pushCode = "";
                    if (!VOID.equals(returnType)) {
                        if ("byte".equals(returnType)) {
                            returnCode = "s8 ret_value = (s8)";
                            pushCode = "env->push_int(runtime->stack, ret_value);";
                        } else if ("short".equals(returnType)) {
                            returnCode = "s16 ret_value = (s16)";
                            pushCode = "env->push_int(runtime->stack, ret_value);";
                        } else if ("int".equals(returnType)) {
                            returnCode = "s32 ret_value = (s32)";
                            pushCode = "env->push_int(runtime->stack, ret_value);";
                        } else if ("boolean".equals(returnType)) {
                            returnCode = "u8 ret_value = (u8)";
                            pushCode = "env->push_int(runtime->stack, ret_value);";
                        } else if ("long".equals(returnType)) {
                            returnCode = "s64 ret_value = (s64)";
                            pushCode = "env->push_long(runtime->stack, ret_value);";
                        } else if ("double".equals(returnType)) {
                            returnCode = "f64 ret_value = (f64)";
                            pushCode = "env->push_double(runtime->stack, ret_value);";
                        } else if ("float".equals(returnType)) {
                            returnCode = "f64 ret_value = (f64)";
                            pushCode = "env->push_float(runtime->stack, ret_value);";
                        } else if ("String".equals(returnType)) {
                            returnCode = "c8* cstr = (c8*)";
                            pushCode = "if (cstr) {\n"
                                    + "        Instance *jstr = createJavaString(runtime, cstr);\n"
                                    + "        env->push_ref(runtime->stack, jstr);\n"
                                    + "    } else {\n"
                                    + "        env->push_ref(runtime->stack, NULL);\n"
                                    + "    }";
                        } else {
                            System.out.println("error " + lineNo + " return type:" + returnType + " in :" + whole);
                        }
                    }
                    output = output.replace(RETURN_TYPE, returnCode);
                    output = output.replace(PUSH_RESULT, pushCode);

                    //process body
                    String varCode = "";
                    String nativeArgvCode = "";
                    for (String argv : argvs) {
                        if (argv.length() == 0) {
                            continue;
                        }
                        String[] tmps = argv.trim().split(" ");
                        String argvType = tmps[0].trim();
                        String argvName = tmps[1].trim();
                        if ("int".equals(argvType)) {
                            varCode += "    s32 " + argvName + " = env->localvar_getInt(runtime, pos++);\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLint) " + argvName;
                        } else if ("short".equals(argvType)) {
                            varCode += "    s32 " + argvName + " = env->localvar_getInt(runtime, pos++);\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLshort) " + argvName;
                        } else if ("byte".equals(argvType)) {
                            varCode += "    s32 " + argvName + " = env->localvar_getInt(runtime, pos++);\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLbyte) " + argvName;
                        } else if ("boolean".equals(argvType)) {
                            varCode += "    s32 " + argvName + " = env->localvar_getInt(runtime, pos++);\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLboolean) " + argvName;
                        } else if ("long".equals(argvType)) {
                            varCode += "    s64 " + argvName + " = getParaLong(runtime, pos);pos += 2;\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLlong) " + argvName;
                        } else if ("float".equals(argvType)) {
                            varCode += "    Int2Float " + argvName + ";" + argvName + ".i = env->localvar_getInt(runtime, pos++);\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLfloat) " + argvName;
                        } else if ("double".equals(argvType)) {
                            varCode += "    Long2Double " + argvName + ";" + argvName + ".l = getParaLong(runtime, pos);pos += 2;\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLdouble) " + argvName;
                        } else if (argvType.indexOf("[]") > 0) {
                            varCode += "    Instance * " + argvName + " = env->localvar_getRefer(runtime, pos++);\n";
                            varCode += "    s32 offset = env->localvar_getInt(runtime, pos++);\n";
                            varCode += "    offset *= env->data_type_bytes[arr->mb.arr_type_index];\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(const GLvoid *) (" + argvName + "->arr_body + offset)";
                        } else {
                            System.out.println("error " + lineNo + " argv type:" + returnType + " in :" + whole);
                        }
                    }
                    output = output.replace(GET_VAR, varCode);
                    output = output.replace(NATIVE_ARGV, nativeArgvCode);
                    bw.write(output);
                }
            }

        } catch (Exception ex) {
            Logger.getLogger(JniBuilder.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                br.close();
                bw.close();
            } catch (IOException ex) {
                Logger.getLogger(JniBuilder.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        System.out.println("success.");
    }
}
