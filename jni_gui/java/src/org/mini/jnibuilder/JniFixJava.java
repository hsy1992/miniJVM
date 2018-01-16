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
public class JniFixJava {

    public static void main(String[] args) {
        JniFixJava gt = new JniFixJava();
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
        try {
            File file = new File(path[0]);
            br = new BufferedReader(new FileReader(file));
            bw = new BufferedWriter(new FileWriter("./GL.h"));
            String line, whole;
            String header = "public static native";
            int lineNo = 0;
            List<String> lines = new ArrayList();
            while ((line = br.readLine()) != null) {
                lineNo++;
                line = line.trim();
                lines.add(line);
            }
            //去掉Buffer
            for (int i = 0; i < lineNo; i++) {
                whole = new String(lines.get(i).getBytes());
                if (line.startsWith(header)) {
                    line = line.substring(header.length()).trim();
                    String returnType = line.substring(0, line.indexOf(' ')).trim();
                    line = line.substring(returnType.length()).trim();
                    String methodName = line.substring(0, line.indexOf('('));
                    line = line.substring(line.indexOf('(') + 1, line.indexOf(')')).trim();
                    String[] argvs = line.split(",");
                }
            }
            bw.write("\n\n\n");

        } catch (Exception ex) {
            Logger.getLogger(JniFixJava.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                br.close();
                bw.close();
            } catch (IOException ex) {
                Logger.getLogger(JniFixJava.class.getName()).log(Level.SEVERE, null, ex);
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
            Logger.getLogger(JniFixJava.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                br.close();
                bw.close();
            } catch (IOException ex) {
                Logger.getLogger(JniFixJava.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        System.out.println("success.");

    }
}
