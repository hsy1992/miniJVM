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
 * @deprecated old file
 * @author gust
 */
public class Fix_GL_java {

    public static void main(String[] args) {
        Fix_GL_java gt = new Fix_GL_java();
        gt.removeBuffer();
    }

    String[] path = {"src/org/mini/jnibuilder/GL.java_old", "org_mini_gl_GL_", "org/mini/gl/GL", "src/org/mini/jnibuilder/GL.java"};

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
    List<String> lines = new ArrayList();

    int processNo = 0;

    void removeBuffer() {
        BufferedReader br = null;
        BufferedWriter bw = null;
        try {
            File file = new File(path[0]);
            br = new BufferedReader(new FileReader(file));
            bw = new BufferedWriter(new FileWriter(path[3]));
            String line, whole;
            String header = "public static native";
            while ((line = br.readLine()) != null) {
                line = line.trim();
                lines.add(line);
            }
            //去掉Buffer
            for (int i = 0, imax = lines.size(); i < imax; i++) {
                processNo = i;
                line = lines.get(i);
                whole = new String(line.getBytes());
                if (line.startsWith(header)) {
                    line = line.substring(header.length()).trim();
                    String returnType = line.substring(0, line.indexOf(' ')).trim();
                    line = line.substring(returnType.length()).trim();
                    String methodName = line.substring(0, line.indexOf('('));
                    line = line.substring(line.indexOf('(') + 1, line.indexOf(')')).trim();
                    if (line.length() > 0) {
                        String[] argvs = line.split(",");

                        String BUFFER = "Buffer";
                        String nativeAgrvTypes = "";

                        if (i == 9454) {
                            int debug = 1;
                        }
                        String commentLine = findNextCommentFunc(i, methodName);
                        if (commentLine != null) { //glClipPlane((GLenum)plane, (const GLdouble*)(equation + equationByteOffset));
                            commentLine = commentLine.substring(commentLine.indexOf(methodName));
                            commentLine = commentLine.substring(commentLine.indexOf("(") + 1, commentLine.lastIndexOf(")"));
                            String[] glArgvs = commentLine.split(",");
                            for (int k = 0; k < glArgvs.length; k++) {
                                String gla = glArgvs[k].trim();
                                int left = gla.indexOf("(");
                                int right = gla.indexOf(")") + 1;
                                if (left < 0 || right > gla.length()) {
                                    int debug = 1;
                                }
                                nativeAgrvTypes += gla.substring(left, right) + ",";
                            }
                        } else {
                            int debug = 1;
                        }

                        String[] nativeArgvs = nativeAgrvTypes.split(",");

                        String newLine = header + " " + returnType + " " + methodName + "(";
                        //

                        int nativeArgvPos = 0;
                        for (int j = 0; j < argvs.length; j++, nativeArgvPos++) {
                            String tmps = argvs[j].trim();

                            if (tmps.startsWith(BUFFER)) {
                                if (nativeArgvPos >= nativeArgvs.length) {
                                    int debug = 1;
                                }
                                if (nativeArgvs[nativeArgvPos].equals("(GLint*)") || nativeArgvs[nativeArgvPos].equals("(const GLint*)")) {
                                    tmps = tmps.replace(BUFFER, "int[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLint64*)") || nativeArgvs[nativeArgvPos].equals("(const GLint64*)") || nativeArgvs[nativeArgvPos].equals("(GLuint64*)") || nativeArgvs[nativeArgvPos].equals("(const GLuint64*)")) {
                                    tmps = tmps.replace(BUFFER, "long[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLint64EXT*)") || nativeArgvs[nativeArgvPos].equals("(const GLint64EXT*)") || nativeArgvs[nativeArgvPos].equals("(GLuint64EXT*)") || nativeArgvs[nativeArgvPos].equals("(const GLuint64EXT*)")) {
                                    tmps = tmps.replace(BUFFER, "long[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLsizei*)") || nativeArgvs[nativeArgvPos].equals("(const GLsizei*)")) {
                                    tmps = tmps.replace(BUFFER, "int[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLshort*)") || nativeArgvs[nativeArgvPos].equals("(const GLshort*)") || nativeArgvs[nativeArgvPos].equals("(GLhalfNV*)") || nativeArgvs[nativeArgvPos].equals("(const GLhalfNV*)")) {
                                    tmps = tmps.replace(BUFFER, "short[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLchar*)") || nativeArgvs[nativeArgvPos].equals("(const GLchar*)") || nativeArgvs[nativeArgvPos].equals("(GLchar**)") || nativeArgvs[nativeArgvPos].equals("(const GLchar**)")) {
                                    tmps = tmps.replace(BUFFER, "byte[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLcharARB*)") || nativeArgvs[nativeArgvPos].equals("(const GLcharARB*)")) {
                                    tmps = tmps.replace(BUFFER, "byte[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLbyte*)") || nativeArgvs[nativeArgvPos].equals("(const GLbyte*)")) {
                                    tmps = tmps.replace(BUFFER, "byte[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLubyte*)") || nativeArgvs[nativeArgvPos].equals("(const GLubyte*)")) {
                                    tmps = tmps.replace(BUFFER, "byte[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLushort*)") || nativeArgvs[nativeArgvPos].equals("(const GLushort*)")) {
                                    tmps = tmps.replace(BUFFER, "short[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLuint*)") || nativeArgvs[nativeArgvPos].equals("(const GLuint*)") || nativeArgvs[nativeArgvPos].equals("(GLfixed*)") || nativeArgvs[nativeArgvPos].equals("(const GLfixed*)")) {
                                    tmps = tmps.replace(BUFFER, "int[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLfloat*)") || nativeArgvs[nativeArgvPos].equals("(const GLfloat*)")) {
                                    tmps = tmps.replace(BUFFER, "float[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLclampf*)") || nativeArgvs[nativeArgvPos].equals("(const GLclampf*)")) {
                                    tmps = tmps.replace(BUFFER, "float[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLdouble*)") || nativeArgvs[nativeArgvPos].equals("(const GLdouble*)")) {
                                    tmps = tmps.replace(BUFFER, "double[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLclampd*)") || nativeArgvs[nativeArgvPos].equals("(const GLclampd*)")) {
                                    tmps = tmps.replace(BUFFER, "double[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLvoid*)") || nativeArgvs[nativeArgvPos].equals("(const GLvoid*)") || nativeArgvs[nativeArgvPos].equals("(GLvoid**)") || nativeArgvs[nativeArgvPos].equals("(const GLvoid**)")) {
                                    tmps = tmps.replace(BUFFER, "Object");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLhandleARB*)") || nativeArgvs[nativeArgvPos].equals("(const GLhandleARB*)")) {
                                    tmps = tmps.replace(BUFFER, "Object");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLvdpauSurfaceNV*)") || nativeArgvs[nativeArgvPos].equals("(const GLvdpauSurfaceNV*)")) {
                                    tmps = tmps.replace(BUFFER, "Object");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLenum*)") || nativeArgvs[nativeArgvPos].equals("(const GLenum*)")) {
                                    tmps = tmps.replace(BUFFER, "int[]");
                                } else if (nativeArgvs[nativeArgvPos].equals("(GLboolean*)") || nativeArgvs[nativeArgvPos].equals("(const GLboolean*)") || nativeArgvs[nativeArgvPos].equals("(GLboolean**)") || nativeArgvs[nativeArgvPos].equals("(const GLboolean**)")) {
                                    tmps = tmps.replace(BUFFER, "boolean[]");
                                } else {
                                    int debug = 1;
                                }

                            }
                            if (tmps.contains("[]") || tmps.contains("Object")) {
                                nativeArgvPos--;
                            }
                            newLine += tmps;
                            if (j < argvs.length - 1) {
                                newLine += ", ";
                            }

                        }
                        newLine += "); //" + nativeAgrvTypes;
                        lines.set(i, newLine);
                        remNextSameMethod(i, methodName);
                    }
                }
            }
            for (String s : lines) {
                bw.write(s + "\n");
            }

        } catch (Exception ex) {
            ex.printStackTrace();
            System.out.println("line no:" + processNo);
        } finally {
            try {
                br.close();
                bw.close();
            } catch (IOException ex) {
                Logger.getLogger(Fix_GL_java.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        System.out.println("success.");
    }

    /**
     * 返回注释行
     *
     * @param lineNo
     * @param methodName
     * @return
     */
    String findNextCommentFunc(int lineNo, String methodName) {
        boolean start = false;
        boolean end = false;
        while (true) {
            if (lineNo >= lines.size()) {
                break;
            }
            String line = lines.get(lineNo);
            if (line.equals("/*")) {
                start = true;
            } else if (line.equals("*/")) {
                end = true;
            }
            if (start && !end) {
                String mName = getMethodName(lineNo, methodName);
                if (mName != null && mName.contains(methodName)) {
                    return line;
                }
            }
            lineNo++;
        }
        return null;
    }

    String getMethodName(int lineNo, String methodName) {
        String line = lines.get(lineNo);
        if (line.contains(methodName)) {
            line = line.substring(line.indexOf(methodName));
            int si = line.indexOf("(");
            if (si > 0) {
                line = line.substring(0, si).trim();
                si = line.indexOf(" ");
                if (si > 0) {
                    line = line.substring(line.lastIndexOf(' '));
                    return line;
                } else {
                    return line;
                }
            }
        }
        return null;
    }

    /**
     * 把另一个函数注释掉
     *
     * @param lineNo
     * @param methodName
     * @return
     */
    void remNextSameMethod(int lineNo, String methodName) {
        while (true) {
            lineNo++;
            if (lineNo >= lines.size()) {
                break;
            }
            String line = lines.get(lineNo);
            String mn = methodName + "(";
            if (line.contains(mn)) {
                line = "//" + line;
                lines.set(lineNo, line);
            }
        }
        ;
    }
}
