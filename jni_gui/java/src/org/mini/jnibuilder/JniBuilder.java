/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.jnibuilder;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author gust
 */
public class JniBuilder {

    public static void main(String[] args) {
        JniBuilder gt = new JniBuilder();
        gt.buildC();
    }

    String[] path = {"src/org/mini/gl/GL.java", "org_mini_gl_GL_", "org/mini/gl/GL"};

    String FUNC_BODY_TEMPLATE
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
    String PKG_PATH = "${PKG_PATH}";
    String METHOD_NAME = "${METHOD_NAME}";
    String GET_VAR = "${GET_VAR}";
    String RETURN_TYPE = "${RETURN_TYPE}";
    String NATIVE_ARGV = "${NATIVE_ARGV}";
    String JAVA_ARGV = "${JAVA_ARGV}";
    String JAVA_RETURN = "${JAVA_RETURN}";
    String PUSH_RESULT = "${PUSH_RESULT}";

    String VOID = "void";

    String FUNC_TABLE_TEMPLATE = "{\"${PKG_PATH}\",  \"${METHOD_NAME}\",  \"(${JAVA_ARGV})${JAVA_RETURN}\",  ${PKG_NAME}${METHOD_NAME}},";

    void buildC() {
        BufferedReader br = null;
        BufferedWriter bw = null;
        List<String> funcTable = new ArrayList();
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
                    String output = new String(FUNC_BODY_TEMPLATE.getBytes());
                    output = output.replace(PKG_NAME, path[1]);
                    output = output.replace(METHOD_NAME, methodName);
                    String funcTableLine = new String(FUNC_TABLE_TEMPLATE.getBytes());
                    funcTableLine = funcTableLine.replace(PKG_NAME, path[1]);
                    funcTableLine = funcTableLine.replace(METHOD_NAME, methodName);
                    funcTableLine = funcTableLine.replace(PKG_PATH, path[2]);
                    //process return 
                    String returnCode = "", pushCode = "", javaReturnCode = "";
                    if (!VOID.equals(returnType)) {
                        if ("int".equals(returnType)) {
                            returnCode = "s32 ret_value = (s32)";
                            pushCode = "env->push_int(runtime->stack, ret_value);";
                            javaReturnCode = "I";
                        } else if ("float".equals(returnType)) {
                            returnCode = "f64 ret_value = (f64)";
                            pushCode = "env->push_float(runtime->stack, ret_value);";
                            javaReturnCode = "D";
                        } else if ("byte".equals(returnType)) {
                            returnCode = "s8 ret_value = (s8)";
                            pushCode = "env->push_int(runtime->stack, ret_value);";
                            javaReturnCode = "B";
                        } else if ("short".equals(returnType)) {
                            returnCode = "s16 ret_value = (s16)";
                            pushCode = "env->push_int(runtime->stack, ret_value);";
                            javaReturnCode = "S";
                        } else if ("boolean".equals(returnType)) {
                            returnCode = "u8 ret_value = (u8)";
                            pushCode = "env->push_int(runtime->stack, ret_value);";
                            javaReturnCode = "Z";
                        } else if ("long".equals(returnType)) {
                            returnCode = "s64 ret_value = (s64)";
                            pushCode = "env->push_long(runtime->stack, ret_value);";
                            javaReturnCode = "J";
                        } else if ("double".equals(returnType)) {
                            returnCode = "f64 ret_value = (f64)";
                            pushCode = "env->push_double(runtime->stack, ret_value);";
                            javaReturnCode = "F";
                        } else if ("String".equals(returnType)) {
                            returnCode = "c8* cstr = (c8*)";
                            pushCode = "if (cstr) {\n"
                                    + "        Instance *jstr = createJavaString(runtime, cstr);\n"
                                    + "        env->push_ref(runtime->stack, jstr);\n"
                                    + "    } else {\n"
                                    + "        env->push_ref(runtime->stack, NULL);\n"
                                    + "    }";
                            javaReturnCode = "Ljava/lang/String;";
                        } else {
                            System.out.println(" " + lineNo + " return type:" + returnType + " in :" + whole);
                        }
                    } else {
                        javaReturnCode = "V";
                    }
                    output = output.replace(RETURN_TYPE, returnCode);
                    output = output.replace(PUSH_RESULT, pushCode);
                    funcTableLine = funcTableLine.replace(JAVA_RETURN, javaReturnCode);

                    //process body
                    String varCode = "";
                    String nativeArgvCode = "";
                    String javaArgvCode = "";
                    for (int i = 0; i < argvs.length; i++) {
                        String argv = argvs[i];
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
                            javaArgvCode += "I";
                        } else if ("short".equals(argvType)) {
                            varCode += "    s32 " + argvName + " = env->localvar_getInt(runtime, pos++);\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLshort) " + argvName;
                            javaArgvCode += "S";
                        } else if ("byte".equals(argvType)) {
                            varCode += "    s32 " + argvName + " = env->localvar_getInt(runtime, pos++);\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLbyte) " + argvName;
                            javaArgvCode += "B";
                        } else if ("boolean".equals(argvType)) {
                            varCode += "    s32 " + argvName + " = env->localvar_getInt(runtime, pos++);\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLboolean) " + argvName;
                            javaArgvCode += "Z";
                        } else if ("long".equals(argvType)) {
                            varCode += "    s64 " + argvName + " = getParaLong(runtime, pos);pos += 2;\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLlong) " + argvName;
                            javaArgvCode += "J";
                        } else if ("float".equals(argvType)) {
                            varCode += "    Int2Float " + argvName + ";" + argvName + ".i = env->localvar_getInt(runtime, pos++);\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLfloat) " + argvName;
                            javaArgvCode += "F";
                        } else if ("double".equals(argvType)) {
                            varCode += "    Long2Double " + argvName + ";" + argvName + ".l = getParaLong(runtime, pos);pos += 2;\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(GLdouble) " + argvName;
                            javaArgvCode += "D";
                        } else if (argvType.indexOf("[]") > 0) {
                            varCode += "    Instance *" + argvName + " = env->localvar_getRefer(runtime, pos++);\n";
                            varCode += "    s32 offset = env->localvar_getInt(runtime, pos++);\n";
                            varCode += "    offset *= env->data_type_bytes[arr->mb.arr_type_index];\n";
                            nativeArgvCode += nativeArgvCode.length() > 0 ? "," : "";
                            nativeArgvCode += "(const GLvoid *) (" + argvName + "->arr_body + offset)";
                            i++;
                            if (argvType.startsWith("int")) {
                                javaArgvCode += "[II";
                            } else if (argvType.startsWith("short")) {
                                javaArgvCode += "[SI";
                            } else if (argvType.startsWith("byte")) {
                                javaArgvCode += "[BI";
                            } else if (argvType.startsWith("long")) {
                                javaArgvCode += "[JI";
                            } else if (argvType.startsWith("float")) {
                                javaArgvCode += "[FI";
                            } else if (argvType.startsWith("double")) {
                                javaArgvCode += "[DI";
                            } else if (argvType.startsWith("String")) {
                                javaArgvCode += "[Ljava/lang/String;I";
                            } else if (argvType.startsWith("boolean")) {
                                javaArgvCode += "[ZI";
                            } else {
                                System.out.println(" " + lineNo + " array type:" + returnType + " in :" + whole);
                            }
                        } else {
                            System.out.println(" " + lineNo + " argv type:" + returnType + " in :" + whole);
                        }
                    }
                    output = output.replace(GET_VAR, varCode);
                    output = output.replace(NATIVE_ARGV, nativeArgvCode);
                    bw.write(output);

                    funcTableLine = funcTableLine.replace(JAVA_ARGV, javaArgvCode);
                    funcTable.add(funcTableLine);
                }
            }
            bw.write("\n\n\n");
            for (String s : funcTable) {
                bw.write(s + "\n");
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

    void buildJava() {
        BufferedReader br = null;
        BufferedWriter bw = null;
        try {
            File file = new File(path[0]);
            br = new BufferedReader(new FileReader(file));
            bw = new BufferedWriter(new FileWriter("./output.c"));
            String line, whole;
            String defineHeader = "public static native";
            String funcHeader = "WINGDIAPI";
            int lineNo = 0;
            while ((line = br.readLine()) != null) {
                lineNo++;
                line = line.trim();
                whole = new String(line.getBytes());
                if (line.startsWith(defineHeader)) {
                } else if (line.startsWith(funcHeader)) {
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
