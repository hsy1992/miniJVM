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
public class Build_GL_java {

    public static void main(String[] args) {
        Build_GL_java gt = new Build_GL_java();
        gt.buildC();
    }

    String[] path = {"src/org/mini/jnibuilder/glad.h", "src/org/mini/gl/GL.java"};

    String CLASS_TEMPLATE
            = //
            "package org.mini.gl;\n"
            + "\n"
            + "public class GL {\n"
            + "${FIELDS}\n"
            + "    public static native void init();\n"
            + "${METHODS}\n"
            + "}\n\n";
    String FIELDS = "${FIELDS}";
    String METHODS = "${METHODS}";

    String FIELD_TEMPLATE = "    public static final ${FIELD_TYPE} ${FIELD_NAME} = ${FIELD_VALUE};";
    String FIELD_NAME = "${FIELD_NAME}";
    String FIELD_TYPE = "${FIELD_TYPE}";
    String FIELD_VALUE = "${FIELD_VALUE}";

    String METHOD_DEC_TEMPLATE = "    public static native ${JAVA_RETURN} ${METHOD_NAME}(${JAVA_ARGV}); //${JAVA_COMMENT}";
    String METHOD_NAME = "${METHOD_NAME}";
    String JAVA_ARGV = "${JAVA_ARGV}";
    String JAVA_RETURN = "${JAVA_RETURN}";
    String JAVA_COMMENT = "${JAVA_COMMENT}";

    String GET_VAR = "${GET_VAR}";
    //native
    String NATIVE_RETURN = "${NATIVE_RETURN}";
    String NATIVE_ARGV = "${NATIVE_ARGV}";
    String PUSH_RESULT = "${PUSH_RESULT}";

    static public String[] INT_TYPE = {"GLint", "GLuint", "GLenum", "GLbitfield", "GLboolean", "GLclampx", "GLsizei", "GLfixed",};
    static public String[] ARR_INT_TYPE = {"GLint *", "GLuint *", "GLenum *", "GLbitfield *", "GLboolean *", "GLclampx *", "GLsizei *", "GLfixed *", "const GLint *", "const GLuint *", "const GLenum *", "const GLbitfield *", "const GLboolean *", "const GLclampx *", "const GLsizei *", "const GLfixed *",};
    static public String[] SHORT_TYPE = {"GLshort", "GLushort",};
    static public String[] ARR_SHORT_TYPE = {"GLshort *", "const GLshort *", "GLushort *", "const GLushort *",};
    static public String[] BYTE_TYPE = {"GLbyte", "GLubyte", "GLchar", "GLcharARB",};
    static public String[] ARR_BYTE_TYPE = {"GLbyte *", "const GLbyte *", "GLubyte *", "const GLubyte *", "GLchar *", "const GLchar *", "GLcharARB *", "const GLcharARB *", "const GLchar *const*"};
    static public String[] LONG_TYPE = {"GLint64", "GLuint64", "GLsync", "GLDEBUGPROC", "GLDEBUGPROCKHR", "GLsizeiptr", "GLintptr",};
    static public String[] ARR_LONG_TYPE = {"GLint64 *", "const GLint64 *", "GLuint64 *", "const GLuint64 *", "GLsync *", "const GLsync *",};
    static public String[] FLOAT_TYPE = {"GLfloat", "GLclampf",};
    static public String[] ARR_FLOAT_TYPE = {"GLfloat *", "const GLfloat *", "GLclampf *", "const GLclampf *",};
    static public String[] DOUBLE_TYPE = {"GLdouble", "GLclampd",};
    static public String[] ARR_DOUBLE_TYPE = {"GLdouble *", "const GLdouble *", "GLclampd *", "const GLclampd *",};
    static public String[] OBJECT_TYPE = {"GLvoid *", "const GLvoid *", "GLsync *", "const void *const*", "void *", "const void *", "void **",};

    void buildC() {
        BufferedReader br = null;
        BufferedWriter bw = null;
        StringBuilder fields = new StringBuilder();
        StringBuilder methods = new StringBuilder();
        List<String> lines = new ArrayList();
        int lineNo = 0;
        try {
            File file = new File(path[0]);
            br = new BufferedReader(new FileReader(file));
            bw = new BufferedWriter(new FileWriter(path[1]));
            String line;

            //fields
            String nativeFieldHeader = "#define GL_";
            while ((line = br.readLine()) != null) {

                line = line.trim();
                lines.add(line);
                if (line.startsWith(nativeFieldHeader)) {
                    String output = FIELD_TEMPLATE;
                    String[] tmps = line.split(" ");
                    String typeCode = "int";
                    String value = tmps[2];
                    try {
                        String v = tmps[2].replace("0x", "");
                        Integer.parseInt(v, 16);
                    } catch (Exception e) {
                        typeCode = "long";
                        tmps[2] += "L";
                    }
                    output = output.replace(FIELD_TYPE, typeCode);
                    output = output.replace(FIELD_NAME, tmps[1]);
                    output = output.replace(FIELD_VALUE, tmps[2]);
                    fields.append(output);
                    fields.append("\n");
                }
            }

            // typedef void * (APIENTRYP PFNGLMAPBUFFERPROC)(GLenum target, GLenum access);
            // GLAPI PFNGLMAPBUFFERPROC glad_glMapBuffer;
            // #define glMapBuffer glad_glMapBuffer
            //
            //methods
            String nativeMethodHeader = "#define gl";
            for (int i = 0, imax = lines.size(); i < imax; i++) {
                lineNo = i;
                line = lines.get(i);
                if (line.startsWith(nativeMethodHeader)) {
                    String output = METHOD_DEC_TEMPLATE;
                    String[] tmps = line.split(" ");
                    String javaNameCode = tmps[1];
                    String javaReturnCode = "";
                    String javaArgvCode = "";
                    String javaCommentCode = "";

                    //native func str
                    String mdef = lines.get(i - 2);
                    if (i == 1405) {
                        int debug = 1;
                    }
                    String mtype = mdef.substring(mdef.indexOf(" "), mdef.indexOf("(")).trim();

                    //GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length
                    mdef = mdef.substring(mdef.lastIndexOf("(") + 1, mdef.lastIndexOf(")")).trim();
                    if (mdef.length() > 0) {
                        String[] nArgvs = mdef.split(",");
                        for (int j = 0; j < nArgvs.length; j++) {
                            String ar = nArgvs[j];
                            int nameAt = 0;
                            int starAt = ar.lastIndexOf("*");
                            int spaceAt = ar.lastIndexOf(" ");
                            if (spaceAt > starAt) {
                                nameAt = spaceAt + 1;
                            } else {
                                nameAt = starAt + 1;
                            }
                            String arName = ar.substring(nameAt).trim();
                            if (arName.equals("near")) {
                                arName = "pnear";
                            }
                            if (arName.equals("far")) {
                                arName = "pfar";
                            }
                            String arType = ar.substring(0, nameAt).trim();

                            if (isTypes(INT_TYPE, arType)) {
                                javaArgvCode += "int " + arName;
                            } else if (isTypes(ARR_INT_TYPE, arType)) {
                                javaArgvCode += "int[] " + arName;
                                javaArgvCode += ", int offset_" + arName;
                            } else if (isTypes(SHORT_TYPE, arType)) {
                                javaArgvCode += "short " + arName;
                            } else if (isTypes(ARR_SHORT_TYPE, arType)) {
                                javaArgvCode += "short[] " + arName;
                                javaArgvCode += ", int offset_" + arName;
                            } else if (isTypes(BYTE_TYPE, arType)) {
                                javaArgvCode += "byte " + arName;
                            } else if (isTypes(ARR_BYTE_TYPE, arType)) {
                                javaArgvCode += "byte[] " + arName;
                                javaArgvCode += ", int offset_" + arName;
                            } else if (isTypes(LONG_TYPE, arType)) {
                                javaArgvCode += "long " + arName;
                            } else if (isTypes(ARR_LONG_TYPE, arType)) {
                                javaArgvCode += "long[] " + arName;
                                javaArgvCode += ", int offset_" + arName;
                            } else if (isTypes(FLOAT_TYPE, arType)) {
                                javaArgvCode += "float " + arName;
                            } else if (isTypes(ARR_FLOAT_TYPE, arType)) {
                                javaArgvCode += "float[] " + arName;
                                javaArgvCode += ", int offset_" + arName;
                            } else if (isTypes(DOUBLE_TYPE, arType)) {
                                javaArgvCode += "double " + arName;
                            } else if (isTypes(ARR_DOUBLE_TYPE, arType)) {
                                javaArgvCode += "double[] " + arName;
                                javaArgvCode += ", int offset_" + arName;
                            } else if (isTypes(OBJECT_TYPE, arType)) {
                                javaArgvCode += "Object " + arName;
                                javaArgvCode += ", int offset_" + arName;
                            } else {
                                System.out.println("java argv type:" + arType);
                            }
                            javaArgvCode += ", ";
                            javaCommentCode += arType + ",";
                        }

                        int lastS = javaArgvCode.lastIndexOf(",");
                        if (lastS == javaArgvCode.length() - 2) {
                            javaArgvCode = javaArgvCode.substring(0, lastS);
                        }
                    }
                    //
                    if (isTypes(INT_TYPE, mtype)) {
                        javaReturnCode = "int";
                    } else if (isTypes(FLOAT_TYPE, mtype)) {
                        javaReturnCode = "float";
                    } else if (isTypes(LONG_TYPE, mtype)) {
                        javaReturnCode = "long";
                    } else if (isTypes(ARR_BYTE_TYPE, mtype)) {
                        javaReturnCode = "byte[]";
                    } else if (isTypes(ARR_LONG_TYPE, mtype)) {
                        javaReturnCode = "long[]";
                    } else if (isTypes(OBJECT_TYPE, mtype)) {
                        javaReturnCode = "Object";
                    } else if (mtype.equals("void")) {
                        javaReturnCode = "void";
                    } else {
                        System.out.println("java return :" + mtype);
                    }
                    //result
                    output = output.replace(METHOD_NAME, javaNameCode);
                    output = output.replace(JAVA_RETURN, javaReturnCode);
                    output = output.replace(JAVA_ARGV, javaArgvCode);
                    output = output.replace(JAVA_COMMENT, javaCommentCode);
                    methods.append(output);
                    methods.append("\n");
                }
            }

            String classCode = CLASS_TEMPLATE.replace(FIELDS, fields.toString());
            classCode = classCode.replace(METHODS, methods.toString());
            bw.write(classCode);
        } catch (Exception ex) {
            ex.printStackTrace();
            System.out.println("line no:" + lineNo);
        } finally {
            try {
                br.close();
                bw.close();
            } catch (IOException ex) {
                Logger.getLogger(Build_GL_java.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        System.out.println("success.");
    }

    static public boolean isTypes(String[] types, String s) {
        for (String t : types) {
            if (t.equals(s)) {
                return true;
            }
        }
        return false;
    }
}
