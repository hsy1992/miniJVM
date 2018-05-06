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
public class Nanovg_h_2_java {

    public static void main(String[] args) {

        Nanovg_h_2_java gt = new Nanovg_h_2_java();
        gt.buildC();
    }

    String[] input_path = {
        "../../c/gui/jni_gui.h",
        "../../c/gui/jni_stb_assist.h",
        "../../c/gui/jni_nanovg_assist.h",};
    String[] output_path = {"src/org/mini/nanovg/Nanovg.java"};

    String CLASS_TEMPLATE
            = //
            "package org.mini.nanovg;\n"
            + "/*this file generated by Nanovg_h_2_java.java ,dont modify it manual.*/\n"
            + "public class Nanovg {\n"
            + "    \n"
            + "${FIELDS}\n"
            + "${METHODS}\n"
            + "}\n\n";
    String FIELDS = "${FIELDS}";
    String METHODS = "${METHODS}";

    String FIELD_TEMPLATE = "    public static final ${FIELD_TYPE} ${FIELD_NAME} = ${FIELD_VALUE};";
    String FIELD_NAME = "${FIELD_NAME}";
    String FIELD_TYPE = "${FIELD_TYPE}";
    String FIELD_VALUE = "${FIELD_VALUE}";

    String METHOD_DEC_TEMPLATE = "    public static native ${JAVA_RETURN} ${METHOD_NAME}(${JAVA_ARGV}); //${NATIVE_ARGV} //${NATIVE_RETURN}";
    String METHOD_NAME = "${METHOD_NAME}";
    String JAVA_ARGV = "${JAVA_ARGV}";
    String JAVA_RETURN = "${JAVA_RETURN}";

    String GET_VAR = "${GET_VAR}";
    //native
    String NATIVE_RETURN = "${NATIVE_RETURN}";
    String NATIVE_ARGV = "${NATIVE_ARGV}";

    static public String[] INT_TYPE = {"int", "int", "GLuint", "unsigned", "enum nk_glfw_init_state", "enum nk_style_colors", "enum nk_widget_layout_states", "enum nk_font_atlas_format", "enum nk_heading", "enum nk_style_cursor", "enum nk_popup_type", "enum nk_color_format", "enum nk_button_behavior", "enum nk_tree_type", "enum nk_show_states", "enum nk_keys", "enum nk_draw_list_stroke", "enum nk_buttons", "enum nk_anti_aliasing", "const enum nk_chart_type", "enum nk_chart_type", "enum nk_buffer_allocation_type", "enum nk_symbol_type", "enum nk_collapse_states", "enum nk_layout_format", "nk_uint", "nk_hash", "nk_flags", "nk_rune", "unsigned int", "const unsigned int",};
    static public String[] ARR_INT_TYPE = {"int*", "const int*", "nk_size*", "nk_flags*", "enum nk_collapse_states*", "nk_uint*", "const nk_uint*", "nk_scroll*", "unsigned int*", "const int*", "const struct nk_color*", "struct nk_color*", "struct nk_scroll*",};
    static public String[] SHORT_TYPE = {"short", "nk_short", "short", "nk_ushort", "unsigned short"};
    static public String[] ARR_SHORT_TYPE = {"short*", "nk_vec2i*", "nk_recti*",};
    static public String[] BYTE_TYPE = {"nk_byte", "char", "nk_char", "unsigned char",};
    static public String[] ARR_BYTE_TYPE = {"nk_byte*", "const nk_byte*", ""};
    static public String[] STRING_TYPE = {"char*", "char*", "const char*", "const char*", "char const*", "char const*", "unsigned char*", "const unsigned char*", "NK_PRINTF_FORMAT_STRING const char*", "",};
    static public String[] ARR_STRING_TYPE = {"char**", "char**", "const char**", "const char**", "char const**", "char const**",};
    static public String[] LONG_TYPE = {""};
    static public String[] ARR_LONG_TYPE = {""};
    static public String[] FLOAT_TYPE = {"float",};
    static public String[] ARR_FLOAT_TYPE = {"float*", "struct nk_rect*", "const float*", "struct nk_colorf*", "struct nk_vec2*", "const struct nk_vec2*",};
    static public String[] DOUBLE_TYPE = {"double",};
    static public String[] ARR_DOUBLE_TYPE = {"double*",};
    static public String[] OBJECT_TYPE = {"struct NVGtextRow*", "struct NVGglyphPosition*", "stbi_uc const*", "stbi_uc*", "const stbi_uc*", "void*", "const void*", "nk_ptr", "nk_size", "NVGcontext*", "NVGtextRow*", "NVGglyphPosition*", "const stbtt_fontinfo*", "stbtt_fontinfo*", "struct nk_font_config*", "struct nk_user_font*", "GLFWwindow*", "const struct nk_font_glyph*", "struct nk_window*", "struct nk_panel*", "struct nk_style_item*", "nk_plugin_filter", "struct nk_memory_status*", "nk_command_custom_callback", "struct nk_list_view*", "struct nk_draw_null_texture*", "struct nk_font*", "const struct nk_allocator*", "const struct nk_image*", "struct nk_text_edit*", "const struct nk_text_edit*", "const struct nk_str*", "const nk_rune*", "nk_rune*", "struct nk_str*", "struct nk_cursor*", "const struct nk_cursor*", "const struct nk_font_config*", "const struct nk_style_button*", "const struct nk_draw_command*", "const struct nk_convert_config*", "struct nk_command_buffer*", "const struct nk_command*", "struct nk_command_buffer*", "struct nk_context*", "struct nk_context*", "const struct nk_context*", "const struct nk_context*", "struct nk_buffer*", "struct nk_buffer*", "const struct nk_buffer*", "const struct nk_input*", "const struct nk_input*", "struct nk_draw_list*", "struct nk_draw_list*", "const struct nk_draw_list*", "const struct nk_user_font*", "struct nk_allocator*", "struct nk_font_atlas*"};
    static public String[] ARR_OBJECT_TYPE = {"void**", "nk_ptr*", "struct nk_font_atlas**",};
    static public String[] VOID_TYPE = {"void"};
    static public String[] MULT_TYPE = {"..."};
    static public String[] STRUCT_FLOAT_ARR_TYPE = {"NVGcolor", "struct nk_rect", "struct nk_vec2", "struct nk_colorf", "", "", "", "", "",};
    static public String[] STRUCT_INT_ARR_TYPE = {"struct nk_vec2", "struct nk_color", "", "", "", "", "", "",};
    static public String[] STRUCT_BYTE_ARR_TYPE = {"NVGpaint", "struct stbtt_fontinfo", "nk_glyph", "const nk_glyph", "struct nk_font_config", "struct nk_image", "struct nk_color", "struct nk_style_item", "nk_handle", "struct nk_font_atlas", "", "", "", "", "",};

    static public String[][] TYPES_ALL = {INT_TYPE, ARR_INT_TYPE, SHORT_TYPE, ARR_SHORT_TYPE, BYTE_TYPE, ARR_BYTE_TYPE, STRING_TYPE, ARR_STRING_TYPE, LONG_TYPE, ARR_LONG_TYPE, FLOAT_TYPE, ARR_FLOAT_TYPE, DOUBLE_TYPE, ARR_DOUBLE_TYPE, OBJECT_TYPE, ARR_OBJECT_TYPE, VOID_TYPE, MULT_TYPE, STRUCT_FLOAT_ARR_TYPE, STRUCT_INT_ARR_TYPE, STRUCT_BYTE_ARR_TYPE,};

    void buildC() {
        BufferedReader br = null;
        BufferedWriter bw = null;
        StringBuilder fields = new StringBuilder();
        StringBuilder methods = new StringBuilder();
        List<String> lines = new ArrayList();
        int lineNo = 0;
        try {

            String line;

            for (String filename : input_path) {
                File ifile = new File(filename);
                br = new BufferedReader(new FileReader(ifile));
                System.out.println("open input file:" + ifile.getAbsolutePath());
                while ((line = br.readLine()) != null) {

                    line = line.replaceAll("/\\*.*\\*/", "");
                    line = line.replaceAll("//.*", "");
                    line = line.replaceAll("NK_PRINTF_VARARG_FUNC\\([0-9]\\)", "");
                    line = line.trim();
                    lines.add(line);
                }
                br.close();
            }

            File ofile = new File(output_path[0]);
            bw = new BufferedWriter(new FileWriter(ofile));
            System.out.println("open output file:" + ofile.getAbsolutePath());

            //fields
            String enumHeader = "enum ", enumHeader1 = "{";
            String enumEnder = "};";
            String nativeFieldHeader = "NK_";
            boolean inEnum = false;
            String enumBody = "";
            for (int i = 0, imax = lines.size(); i < imax; i++) {
                lineNo = i;
                line = lines.get(i);

                if (line.contains("NK_WINDOW_BORDER")) {
                    int debug = 1;
                }

                if (line.startsWith(enumHeader) && line.contains(enumHeader1)) {
                    inEnum = true;
                    enumBody += line;
                } else if (inEnum && line.contains(enumEnder)) {
                    enumBody += line;
                } else if (inEnum) {
                    enumBody += line;
                }
                if (enumBody.contains(enumHeader1) && enumBody.contains(enumEnder)) {
                    enumBody = enumBody.substring(enumBody.indexOf(enumHeader1) + 1, enumBody.indexOf(enumEnder));

                    enumBody = enumBody.replace("\r", "").replace("\n", "");
                    String[] enumItems = enumBody.split(",");
                    int enum_value = 0;
                    for (String item : enumItems) {
                        String fieldCode = FIELD_TEMPLATE;
                        String[] tmps = item.split("=");
                        String typeCode = "int";
                        String fieldName = tmps[0].replace(",", "").trim();
                        fieldCode = fieldCode.replace(FIELD_TYPE, typeCode);
                        fieldCode = fieldCode.replace(FIELD_NAME, fieldName);
                        if (tmps.length > 1) {
                            String value = tmps[1].replace(",", "").trim();
                            try {
                                enum_value = Integer.parseInt(value, 10);
                            } catch (Exception e) {
                                int debug = 1;
                                fieldCode = fieldCode.replace(FIELD_VALUE, "" + value);
                            }
                        }
                        fieldCode = fieldCode.replace(FIELD_VALUE, "" + enum_value);
                        fields.append(fieldCode);
                        fields.append("\n");
                        enum_value++;
                    }
                    enumBody = "";
                    inEnum = false;
                }

            }

            //
            //methods
            String nativeMethodHeader = "NUTIL_API ", nativeHeader2 = ";", nativeHeader3 = "{";
            for (int i = 0, imax = lines.size(); i < imax; i++) {
                lineNo = i;
                line = lines.get(i);
                if (line.startsWith(nativeMethodHeader) && line.contains(nativeHeader2) && !line.contains(nativeHeader3)) {
                    String output = METHOD_DEC_TEMPLATE;
                    String[] tmps = line.split("\\(");
                    int spactAt = tmps[0].lastIndexOf(" ");
                    int starAt = tmps[0].lastIndexOf("*");
                    int splitTypeName = Math.max(spactAt, starAt);
                    String mtype = tmps[0].substring(tmps[0].indexOf(" "), splitTypeName + 1).trim();
                    String javaNameCode = tmps[0].substring(splitTypeName + 1).trim();
                    String javaReturnCode = "";
                    String javaArgvCode = "";
                    String nativeCommentCode = "";

                    //native func str
                    if (i == 2044) {
                        int debug = 1;
                    }
                    String mdef = line;

                    //GLuint shader, GLsizei count, const GLchar*const*string, const GLint*length
                    mdef = mdef.substring(mdef.indexOf("(") + 1, mdef.lastIndexOf(")")).trim();
                    //func_pointer
                    mdef = mdef.replaceAll("\\,[ a-zA-z0-9]{1,}\\(\\*.*\\)\\(.*\\)", ",void*");

                    mdef = mdef.replace(" *", "*");
                    if (mdef.length() > 0) {
                        String[] nArgvs = mdef.split(",");
                        for (int j = 0; j < nArgvs.length; j++) {
                            String nargv = nArgvs[j].trim();
                            String arType = Util.getType(TYPES_ALL, nargv);
                            String arName = "arg" + j;
                            if (arType == null) {
                                System.out.println("error argv type:" + nargv);
                                continue;
                            }
                            if (arType.length() != nargv.length()) {
                                arName = nargv.substring(arType.length()).trim();
                            }

                            arName = arName.trim();
                            arName = "p" + arName;

                            if (Util.isTypes(INT_TYPE, arType)) {
                                javaArgvCode += "int " + arName;
                            } else if (Util.isTypes(ARR_INT_TYPE, arType)) {
                                javaArgvCode += "int[] " + arName;
                            } else if (Util.isTypes(SHORT_TYPE, arType)) {
                                javaArgvCode += "short " + arName;
                            } else if (Util.isTypes(ARR_SHORT_TYPE, arType)) {
                                javaArgvCode += "short[] " + arName;
                            } else if (Util.isTypes(BYTE_TYPE, arType)) {
                                javaArgvCode += "byte " + arName;
                            } else if (Util.isTypes(ARR_BYTE_TYPE, arType)) {
                                javaArgvCode += "byte[] " + arName;
                            } else if (Util.isTypes(LONG_TYPE, arType)) {
                                javaArgvCode += "long " + arName;
                            } else if (Util.isTypes(ARR_LONG_TYPE, arType)) {
                                javaArgvCode += "long[] " + arName;
                            } else if (Util.isTypes(FLOAT_TYPE, arType)) {
                                javaArgvCode += "float " + arName;
                            } else if (Util.isTypes(ARR_FLOAT_TYPE, arType)) {
                                javaArgvCode += "float[] " + arName;
                            } else if (Util.isTypes(DOUBLE_TYPE, arType)) {
                                javaArgvCode += "double " + arName;
                            } else if (Util.isTypes(ARR_DOUBLE_TYPE, arType)) {
                                javaArgvCode += "double[] " + arName;
                            } else if (Util.isTypes(OBJECT_TYPE, arType)) {
                                javaArgvCode += "long " + arName;
                                arType += "/*ptr*/";
                            } else if (Util.isTypes(ARR_OBJECT_TYPE, arType)) {
                                javaArgvCode += "long[] " + arName;
                                arType += "/*ptr*/";
                            } else if (Util.isTypes(MULT_TYPE, arType)) {
                                javaArgvCode += "byte[]" + arType + " " + arName;
                            } else if (Util.isTypes(STRUCT_FLOAT_ARR_TYPE, arType)) {
                                javaArgvCode += "float[] " + arName;
                                arType += "/*none_ptr*/";
                            } else if (Util.isTypes(STRUCT_INT_ARR_TYPE, arType)) {
                                javaArgvCode += "int[] " + arName;
                                arType += "/*none_ptr*/";
                            } else if (Util.isTypes(STRUCT_BYTE_ARR_TYPE, arType)) {
                                javaArgvCode += "byte[] " + arName;
                                arType += "/*none_ptr*/";
                            } else if (Util.isTypes(STRING_TYPE, arType)) {
                                javaArgvCode += "byte[] " + arName;
                            } else if (Util.isTypes(ARR_STRING_TYPE, arType)) {
                                javaArgvCode += "byte[][] " + arName;
                            } else if (Util.isTypes(VOID_TYPE, arType)) {
                            } else {
                                System.out.println("java argv type:" + arType);
                            }
                            javaArgvCode += ", ";
                            nativeCommentCode += arType + ",";
                        }

                        int lastS = javaArgvCode.lastIndexOf(",");
                        if (lastS == javaArgvCode.length() - 2) {
                            javaArgvCode = javaArgvCode.substring(0, lastS);
                        }
                    }
                    //
                    mtype = mtype.replace(" *", "*");
                    String nativeReturnCode = mtype;
                    if (Util.isTypes(INT_TYPE, mtype)) {
                        javaReturnCode = "int";
                    } else if (Util.isTypes(ARR_INT_TYPE, mtype)) {
                        javaReturnCode = "int[]";
                    } else if (Util.isTypes(FLOAT_TYPE, mtype)) {
                        javaReturnCode = "float";
                    } else if (Util.isTypes(ARR_FLOAT_TYPE, mtype)) {
                        javaReturnCode = "float[]";
                    } else if (Util.isTypes(LONG_TYPE, mtype)) {
                        javaReturnCode = "long";
                    } else if (Util.isTypes(ARR_BYTE_TYPE, mtype)) {
                        javaReturnCode = "byte[]";
                    } else if (Util.isTypes(STRING_TYPE, mtype)) {
                        javaReturnCode = "byte[]";
                    } else if (Util.isTypes(DOUBLE_TYPE, mtype)) {
                        javaReturnCode = "double";
                    } else if (Util.isTypes(ARR_LONG_TYPE, mtype)) {
                        javaReturnCode = "long[]";
                    } else if (Util.isTypes(OBJECT_TYPE, mtype)) {
                        javaReturnCode += "long ";
                        nativeReturnCode += "/*ptr*/ ";
                    } else if (Util.isTypes(STRUCT_FLOAT_ARR_TYPE, mtype)) {
                        javaReturnCode += "float[] ";
                        nativeReturnCode += "/*none_ptr*/ ";
                    } else if (Util.isTypes(STRUCT_INT_ARR_TYPE, mtype)) {
                        javaReturnCode += "int[] ";
                        nativeReturnCode += "/*none_ptr*/ ";
                    } else if (Util.isTypes(STRUCT_BYTE_ARR_TYPE, mtype)) {
                        javaReturnCode += "byte[] ";
                        nativeReturnCode += "/*none_ptr*/ ";
                    } else if (mtype.equals("void")) {
                        javaReturnCode = "void";
                    } else {
//                        javaReturnCode = "long";
                        System.out.println("java return :" + mtype);
                    }

                    //result
                    output = output.replace(METHOD_NAME, javaNameCode);
                    output = output.replace(JAVA_RETURN, javaReturnCode);
                    output = output.replace(JAVA_ARGV, javaArgvCode);
                    output = output.replace(NATIVE_ARGV, nativeCommentCode);
                    output = output.replace(NATIVE_RETURN, nativeReturnCode);
                    if (methods.indexOf(javaNameCode + "(") < 0) {
                        methods.append(output);
                        methods.append("\n");
                    }
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
                Logger.getLogger(Nanovg_h_2_java.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        System.out.println("success.");
    }

}
