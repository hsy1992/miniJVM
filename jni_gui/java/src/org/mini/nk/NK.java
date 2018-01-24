package org.mini.nk;

public class NK {
    static final int NK_FLAG(int x) {return (1 << (x));};
    public static final int nk_false = 0;
    public static final int nk_true = 1;
    public static final int NK_UP = 0;
    public static final int NK_RIGHT = 1;
    public static final int NK_DOWN = 2;
    public static final int NK_LEFT = 3;
    public static final int NK_BUTTON_DEFAULT = 0;
    public static final int NK_BUTTON_REPEATER = 1;
    public static final int NK_FIXED = nk_false;
    public static final int NK_MODIFIABLE = nk_true;
    public static final int NK_VERTICAL = 0;
    public static final int NK_HORIZONTAL = 1;
    public static final int NK_MINIMIZED = nk_false;
    public static final int NK_MAXIMIZED = nk_true;
    public static final int NK_HIDDEN = nk_false;
    public static final int NK_SHOWN = nk_true;
    public static final int NK_CHART_LINES = 0;
    public static final int NK_CHART_COLUMN = 1;
    public static final int NK_CHART_MAX = 2;
    public static final int NK_CHART_HOVERING = 0x01;
    public static final int NK_CHART_CLICKED = 0x02;
    public static final int NK_RGB = 0;
    public static final int NK_RGBA = 1;
    public static final int NK_POPUP_STATIC = 0;
    public static final int NK_POPUP_DYNAMIC = 1;
    public static final int NK_DYNAMIC = 0;
    public static final int NK_STATIC = 1;
    public static final int NK_TREE_NODE = 0;
    public static final int NK_TREE_TAB = 1;
    public static final int NK_SYMBOL_NONE = 0;
    public static final int NK_SYMBOL_X = 1;
    public static final int NK_SYMBOL_UNDERSCORE = 2;
    public static final int NK_SYMBOL_CIRCLE_SOLID = 3;
    public static final int NK_SYMBOL_CIRCLE_OUTLINE = 4;
    public static final int NK_SYMBOL_RECT_SOLID = 5;
    public static final int NK_SYMBOL_RECT_OUTLINE = 6;
    public static final int NK_SYMBOL_TRIANGLE_UP = 7;
    public static final int NK_SYMBOL_TRIANGLE_DOWN = 8;
    public static final int NK_SYMBOL_TRIANGLE_LEFT = 9;
    public static final int NK_SYMBOL_TRIANGLE_RIGHT = 10;
    public static final int NK_SYMBOL_PLUS = 11;
    public static final int NK_SYMBOL_MINUS = 12;
    public static final int NK_SYMBOL_MAX = 13;
    public static final int NK_KEY_NONE = 0;
    public static final int NK_KEY_SHIFT = 1;
    public static final int NK_KEY_CTRL = 2;
    public static final int NK_KEY_DEL = 3;
    public static final int NK_KEY_ENTER = 4;
    public static final int NK_KEY_TAB = 5;
    public static final int NK_KEY_BACKSPACE = 6;
    public static final int NK_KEY_COPY = 7;
    public static final int NK_KEY_CUT = 8;
    public static final int NK_KEY_PASTE = 9;
    public static final int NK_KEY_UP = 10;
    public static final int NK_KEY_DOWN = 11;
    public static final int NK_KEY_LEFT = 12;
    public static final int NK_KEY_RIGHT = 13;
    public static final int NK_KEY_TEXT_INSERT_MODE = 14;
    public static final int NK_KEY_TEXT_REPLACE_MODE = 15;
    public static final int NK_KEY_TEXT_RESET_MODE = 16;
    public static final int NK_KEY_TEXT_LINE_START = 17;
    public static final int NK_KEY_TEXT_LINE_END = 18;
    public static final int NK_KEY_TEXT_START = 19;
    public static final int NK_KEY_TEXT_END = 20;
    public static final int NK_KEY_TEXT_UNDO = 21;
    public static final int NK_KEY_TEXT_REDO = 22;
    public static final int NK_KEY_TEXT_SELECT_ALL = 23;
    public static final int NK_KEY_TEXT_WORD_LEFT = 24;
    public static final int NK_KEY_TEXT_WORD_RIGHT = 25;
    public static final int NK_KEY_SCROLL_START = 26;
    public static final int NK_KEY_SCROLL_END = 27;
    public static final int NK_KEY_SCROLL_DOWN = 28;
    public static final int NK_KEY_SCROLL_UP = 29;
    public static final int NK_KEY_MAX = 30;
    public static final int NK_BUTTON_LEFT = 0;
    public static final int NK_BUTTON_MIDDLE = 1;
    public static final int NK_BUTTON_RIGHT = 2;
    public static final int NK_BUTTON_DOUBLE = 3;
    public static final int NK_BUTTON_MAX = 4;
    public static final int NK_ANTI_ALIASING_OFF = 0;
    public static final int NK_ANTI_ALIASING_ON = 1;
    public static final int NK_CONVERT_SUCCESS = 0;
    public static final int NK_CONVERT_INVALID_PARAM = 1;
    public static final int NK_CONVERT_COMMAND_BUFFER_FULL = NK_FLAG(1);
    public static final int NK_CONVERT_VERTEX_BUFFER_FULL = NK_FLAG(2);
    public static final int NK_CONVERT_ELEMENT_BUFFER_FULL = NK_FLAG(3);
    public static final int NK_WINDOW_BORDER = NK_FLAG(0);
    public static final int NK_WINDOW_MOVABLE = NK_FLAG(1);
    public static final int NK_WINDOW_SCALABLE = NK_FLAG(2);
    public static final int NK_WINDOW_CLOSABLE = NK_FLAG(3);
    public static final int NK_WINDOW_MINIMIZABLE = NK_FLAG(4);
    public static final int NK_WINDOW_NO_SCROLLBAR = NK_FLAG(5);
    public static final int NK_WINDOW_TITLE = NK_FLAG(6);
    public static final int NK_WINDOW_SCROLL_AUTO_HIDE = NK_FLAG(7);
    public static final int NK_WINDOW_BACKGROUND = NK_FLAG(8);
    public static final int NK_WINDOW_SCALE_LEFT = NK_FLAG(9);
    public static final int NK_WINDOW_NO_INPUT = NK_FLAG(10);
    public static final int NK_WIDGET_INVALID = 0;
    public static final int NK_WIDGET_VALID = 1;
    public static final int NK_WIDGET_ROM = 2;
    public static final int NK_WIDGET_STATE_MODIFIED = NK_FLAG(1);
    public static final int NK_WIDGET_STATE_INACTIVE = NK_FLAG(2);
    public static final int NK_WIDGET_STATE_ENTERED = NK_FLAG(3);
    public static final int NK_WIDGET_STATE_HOVER = NK_FLAG(4);
    public static final int NK_WIDGET_STATE_ACTIVED = NK_FLAG(5);
    public static final int NK_WIDGET_STATE_LEFT = NK_FLAG(6);
    public static final int NK_WIDGET_STATE_HOVERED = NK_WIDGET_STATE_HOVER|NK_WIDGET_STATE_MODIFIED;
    public static final int NK_WIDGET_STATE_ACTIVE = NK_WIDGET_STATE_ACTIVED|NK_WIDGET_STATE_MODIFIED;
    public static final int NK_TEXT_ALIGN_LEFT = 0x01;
    public static final int NK_TEXT_ALIGN_CENTERED = 0x02;
    public static final int NK_TEXT_ALIGN_RIGHT = 0x04;
    public static final int NK_TEXT_ALIGN_TOP = 0x08;
    public static final int NK_TEXT_ALIGN_MIDDLE = 0x10;
    public static final int NK_TEXT_ALIGN_BOTTOM = 0x20;
    public static final int NK_TEXT_LEFT = NK_TEXT_ALIGN_MIDDLE|NK_TEXT_ALIGN_LEFT;
    public static final int NK_TEXT_CENTERED = NK_TEXT_ALIGN_MIDDLE|NK_TEXT_ALIGN_CENTERED;
    public static final int NK_TEXT_RIGHT = NK_TEXT_ALIGN_MIDDLE|NK_TEXT_ALIGN_RIGHT;
    public static final int NK_EDIT_DEFAULT = 0;
    public static final int NK_EDIT_READ_ONLY = NK_FLAG(0);
    public static final int NK_EDIT_AUTO_SELECT = NK_FLAG(1);
    public static final int NK_EDIT_SIG_ENTER = NK_FLAG(2);
    public static final int NK_EDIT_ALLOW_TAB = NK_FLAG(3);
    public static final int NK_EDIT_NO_CURSOR = NK_FLAG(4);
    public static final int NK_EDIT_SELECTABLE = NK_FLAG(5);
    public static final int NK_EDIT_CLIPBOARD = NK_FLAG(6);
    public static final int NK_EDIT_CTRL_ENTER_NEWLINE = NK_FLAG(7);
    public static final int NK_EDIT_NO_HORIZONTAL_SCROLL = NK_FLAG(8);
    public static final int NK_EDIT_ALWAYS_INSERT_MODE = NK_FLAG(9);
    public static final int NK_EDIT_MULTILINE = NK_FLAG(10);
    public static final int NK_EDIT_GOTO_END_ON_ACTIVATE = NK_FLAG(11);
    public static final int NK_EDIT_SIMPLE = NK_EDIT_ALWAYS_INSERT_MODE;
    public static final int NK_EDIT_FIELD = NK_EDIT_SIMPLE|NK_EDIT_SELECTABLE|NK_EDIT_CLIPBOARD;
    public static final int NK_EDIT_BOX = NK_EDIT_ALWAYS_INSERT_MODE| NK_EDIT_SELECTABLE| NK_EDIT_MULTILINE|NK_EDIT_ALLOW_TAB|NK_EDIT_CLIPBOARD;
    public static final int NK_EDIT_EDITOR = NK_EDIT_SELECTABLE|NK_EDIT_MULTILINE|NK_EDIT_ALLOW_TAB| NK_EDIT_CLIPBOARD;
    public static final int NK_EDIT_ACTIVE = NK_FLAG(0);
    public static final int NK_EDIT_INACTIVE = NK_FLAG(1);
    public static final int NK_EDIT_ACTIVATED = NK_FLAG(2);
    public static final int NK_EDIT_DEACTIVATED = NK_FLAG(3);
    public static final int NK_EDIT_COMMITED = NK_FLAG(4);
    public static final int NK_COLOR_TEXT = 0;
    public static final int NK_COLOR_WINDOW = 1;
    public static final int NK_COLOR_HEADER = 2;
    public static final int NK_COLOR_BORDER = 3;
    public static final int NK_COLOR_BUTTON = 4;
    public static final int NK_COLOR_BUTTON_HOVER = 5;
    public static final int NK_COLOR_BUTTON_ACTIVE = 6;
    public static final int NK_COLOR_TOGGLE = 7;
    public static final int NK_COLOR_TOGGLE_HOVER = 8;
    public static final int NK_COLOR_TOGGLE_CURSOR = 9;
    public static final int NK_COLOR_SELECT = 10;
    public static final int NK_COLOR_SELECT_ACTIVE = 11;
    public static final int NK_COLOR_SLIDER = 12;
    public static final int NK_COLOR_SLIDER_CURSOR = 13;
    public static final int NK_COLOR_SLIDER_CURSOR_HOVER = 14;
    public static final int NK_COLOR_SLIDER_CURSOR_ACTIVE = 15;
    public static final int NK_COLOR_PROPERTY = 16;
    public static final int NK_COLOR_EDIT = 17;
    public static final int NK_COLOR_EDIT_CURSOR = 18;
    public static final int NK_COLOR_COMBO = 19;
    public static final int NK_COLOR_CHART = 20;
    public static final int NK_COLOR_CHART_COLOR = 21;
    public static final int NK_COLOR_CHART_COLOR_HIGHLIGHT = 22;
    public static final int NK_COLOR_SCROLLBAR = 23;
    public static final int NK_COLOR_SCROLLBAR_CURSOR = 24;
    public static final int NK_COLOR_SCROLLBAR_CURSOR_HOVER = 25;
    public static final int NK_COLOR_SCROLLBAR_CURSOR_ACTIVE = 26;
    public static final int NK_COLOR_TAB_HEADER = 27;
    public static final int NK_COLOR_COUNT = 28;
    public static final int NK_CURSOR_ARROW = 0;
    public static final int NK_CURSOR_TEXT = 1;
    public static final int NK_CURSOR_MOVE = 2;
    public static final int NK_CURSOR_RESIZE_VERTICAL = 3;
    public static final int NK_CURSOR_RESIZE_HORIZONTAL = 4;
    public static final int NK_CURSOR_RESIZE_TOP_LEFT_DOWN_RIGHT = 5;
    public static final int NK_CURSOR_RESIZE_TOP_RIGHT_DOWN_LEFT = 6;
    public static final int NK_CURSOR_COUNT = 7;
    public static final int NK_COORD_UV = 0;
    public static final int NK_COORD_PIXEL = 1;
    public static final int NK_FONT_ATLAS_ALPHA8 = 0;
    public static final int NK_FONT_ATLAS_RGBA32 = 1;
    public static final int NK_BUFFER_FIXED = 0;
    public static final int NK_BUFFER_DYNAMIC = 1;
    public static final int NK_BUFFER_FRONT = 0;
    public static final int NK_BUFFER_BACK = 1;
    public static final int NK_BUFFER_MAX = 2;
    public static final int NK_TEXT_EDIT_SINGLE_LINE = 0;
    public static final int NK_TEXT_EDIT_MULTI_LINE = 1;
    public static final int NK_TEXT_EDIT_MODE_VIEW = 0;
    public static final int NK_TEXT_EDIT_MODE_INSERT = 1;
    public static final int NK_TEXT_EDIT_MODE_REPLACE = 2;
    public static final int NK_COMMAND_NOP = 0;
    public static final int NK_COMMAND_SCISSOR = 1;
    public static final int NK_COMMAND_LINE = 2;
    public static final int NK_COMMAND_CURVE = 3;
    public static final int NK_COMMAND_RECT = 4;
    public static final int NK_COMMAND_RECT_FILLED = 5;
    public static final int NK_COMMAND_RECT_MULTI_COLOR = 6;
    public static final int NK_COMMAND_CIRCLE = 7;
    public static final int NK_COMMAND_CIRCLE_FILLED = 8;
    public static final int NK_COMMAND_ARC = 9;
    public static final int NK_COMMAND_ARC_FILLED = 10;
    public static final int NK_COMMAND_TRIANGLE = 11;
    public static final int NK_COMMAND_TRIANGLE_FILLED = 12;
    public static final int NK_COMMAND_POLYGON = 13;
    public static final int NK_COMMAND_POLYGON_FILLED = 14;
    public static final int NK_COMMAND_POLYLINE = 15;
    public static final int NK_COMMAND_TEXT = 16;
    public static final int NK_COMMAND_IMAGE = 17;
    public static final int NK_COMMAND_CUSTOM = 18;
    public static final int NK_CLIPPING_OFF = nk_false;
    public static final int NK_CLIPPING_ON = nk_true;
    public static final int NK_STROKE_OPEN = nk_false;
    public static final int NK_STROKE_CLOSED = nk_true;
    public static final int NK_VERTEX_POSITION = 0;
    public static final int NK_VERTEX_COLOR = 1;
    public static final int NK_VERTEX_TEXCOORD = 2;
    public static final int NK_VERTEX_ATTRIBUTE_COUNT = 3;
    public static final int NK_FORMAT_SCHAR = 0;
    public static final int NK_FORMAT_SSHORT = 1;
    public static final int NK_FORMAT_SINT = 2;
    public static final int NK_FORMAT_UCHAR = 3;
    public static final int NK_FORMAT_USHORT = 4;
    public static final int NK_FORMAT_UINT = 5;
    public static final int NK_FORMAT_FLOAT = 6;
    public static final int NK_FORMAT_DOUBLE = 7;
    public static final int NK_FORMAT_COLOR_BEGIN = 8;
    public static final int NK_FORMAT_R8G8B8 = NK_FORMAT_COLOR_BEGIN;
    public static final int NK_FORMAT_R16G15B16 = 10;
    public static final int NK_FORMAT_R32G32B32 = 11;
    public static final int NK_FORMAT_R8G8B8A8 = 12;
    public static final int NK_FORMAT_B8G8R8A8 = 13;
    public static final int NK_FORMAT_R16G15B16A16 = 14;
    public static final int NK_FORMAT_R32G32B32A32 = 15;
    public static final int NK_FORMAT_R32G32B32A32_FLOAT = 16;
    public static final int NK_FORMAT_R32G32B32A32_DOUBLE = 17;
    public static final int NK_FORMAT_RGB32 = 18;
    public static final int NK_FORMAT_RGBA32 = 19;
    public static final int NK_FORMAT_COLOR_END = NK_FORMAT_RGBA32;
    public static final int NK_FORMAT_COUNT = 21;
    public static final int NK_STYLE_ITEM_COLOR = 0;
    public static final int NK_STYLE_ITEM_IMAGE = 1;
    public static final int NK_HEADER_LEFT = 0;
    public static final int NK_HEADER_RIGHT = 1;
    public static final int NK_PANEL_WINDOW = NK_FLAG(0);
    public static final int NK_PANEL_GROUP = NK_FLAG(1);
    public static final int NK_PANEL_POPUP = NK_FLAG(2);
    public static final int NK_PANEL_CONTEXTUAL = NK_FLAG(4);
    public static final int NK_PANEL_COMBO = NK_FLAG(5);
    public static final int NK_PANEL_MENU = NK_FLAG(6);
    public static final int NK_PANEL_TOOLTIP = NK_FLAG(7);
    public static final int NK_PANEL_SET_NONBLOCK = NK_PANEL_CONTEXTUAL|NK_PANEL_COMBO|NK_PANEL_MENU|NK_PANEL_TOOLTIP;
    public static final int NK_PANEL_SET_POPUP = NK_PANEL_SET_NONBLOCK|NK_PANEL_POPUP;
    public static final int NK_PANEL_SET_SUB = NK_PANEL_SET_POPUP|NK_PANEL_GROUP;
    public static final int NK_LAYOUT_DYNAMIC_FIXED = 0;
    public static final int NK_LAYOUT_DYNAMIC_ROW = 1;
    public static final int NK_LAYOUT_DYNAMIC_FREE = 2;
    public static final int NK_LAYOUT_DYNAMIC = 3;
    public static final int NK_LAYOUT_STATIC_FIXED = 4;
    public static final int NK_LAYOUT_STATIC_ROW = 5;
    public static final int NK_LAYOUT_STATIC_FREE = 6;
    public static final int NK_LAYOUT_STATIC = 7;
    public static final int NK_LAYOUT_TEMPLATE = 8;
    public static final int NK_LAYOUT_COUNT = 9;
    public static final int NK_WINDOW_PRIVATE = NK_FLAG(11);
    public static final int NK_WINDOW_DYNAMIC = NK_WINDOW_PRIVATE;
    public static final int NK_WINDOW_ROM = NK_FLAG(12);
    public static final int NK_WINDOW_NOT_INTERACTIVE = NK_WINDOW_ROM|NK_WINDOW_NO_INPUT;
    public static final int NK_WINDOW_HIDDEN = NK_FLAG(13);
    public static final int NK_WINDOW_CLOSED = NK_FLAG(14);
    public static final int NK_WINDOW_MINIMIZED = NK_FLAG(15);
    public static final int NK_WINDOW_REMOVE_ROM = NK_FLAG(16);
    public static final int NK_ARG_TYPE_CHAR = 0;
    public static final int NK_ARG_TYPE_SHORT = 1;
    public static final int NK_ARG_TYPE_DEFAULT = 2;
    public static final int NK_ARG_TYPE_LONG = 3;
    public static final int NK_ARG_FLAG_LEFT = 0x01;
    public static final int NK_ARG_FLAG_PLUS = 0x02;
    public static final int NK_ARG_FLAG_SPACE = 0x04;
    public static final int NK_ARG_FLAG_NUM = 0x10;
    public static final int NK_ARG_FLAG_ZERO = 0x20;
    public static final int NK_DO_NOT_STOP_ON_NEW_LINE = 0;
    public static final int NK_STOP_ON_NEW_LINE = 1;
    public static final int NK_RP_HEURISTIC_Skyline_default = 0;
    public static final int NK_RP_HEURISTIC_Skyline_BL_sortHeight = NK_RP_HEURISTIC_Skyline_default;
    public static final int NK_RP_HEURISTIC_Skyline_BF_sortHeight = 2;
    public static final int NK_RP__INIT_skyline = 1;
    public static final int NK_TT_vmove = 1;
    public static final int NK_TT_vline = 2;
    public static final int NK_TT_vcurve = 3;
    public static final int NK_TT_PLATFORM_ID_UNICODE = 0;
    public static final int NK_TT_PLATFORM_ID_MAC = 1;
    public static final int NK_TT_PLATFORM_ID_ISO = 2;
    public static final int NK_TT_PLATFORM_ID_MICROSOFT = 3;
    public static final int NK_TT_UNICODE_EID_UNICODE_1_0 = 0;
    public static final int NK_TT_UNICODE_EID_UNICODE_1_1 = 1;
    public static final int NK_TT_UNICODE_EID_ISO_10646 = 2;
    public static final int NK_TT_UNICODE_EID_UNICODE_2_0_BMP = 3;
    public static final int NK_TT_UNICODE_EID_UNICODE_2_0_FULL = 4;
    public static final int NK_TT_MS_EID_SYMBOL = 0;
    public static final int NK_TT_MS_EID_UNICODE_BMP = 1;
    public static final int NK_TT_MS_EID_SHIFTJIS = 2;
    public static final int NK_TT_MS_EID_UNICODE_FULL = 10;
    public static final int NK_TT_MAC_EID_ROMAN = 0;
    public static final int NK_TT_MAC_EID_ARABIC = 4;
    public static final int NK_TT_MAC_EID_JAPANESE = 1;
    public static final int NK_TT_MAC_EID_HEBREW = 5;
    public static final int NK_TT_MAC_EID_CHINESE_TRAD = 2;
    public static final int NK_TT_MAC_EID_GREEK = 6;
    public static final int NK_TT_MAC_EID_KOREAN = 3;
    public static final int NK_TT_MAC_EID_RUSSIAN = 7;
    public static final int NK_TT_MS_LANG_ENGLISH = 0x0409;
    public static final int NK_TT_MS_LANG_ITALIAN = 0x0410;
    public static final int NK_TT_MS_LANG_CHINESE = 0x0804;
    public static final int NK_TT_MS_LANG_JAPANESE = 0x0411;
    public static final int NK_TT_MS_LANG_DUTCH = 0x0413;
    public static final int NK_TT_MS_LANG_KOREAN = 0x0412;
    public static final int NK_TT_MS_LANG_FRENCH = 0x040c;
    public static final int NK_TT_MS_LANG_RUSSIAN = 0x0419;
    public static final int NK_TT_MS_LANG_GERMAN = 0x0407;
    public static final int NK_TT_MS_LANG_SPANISH = 0x0409;
    public static final int NK_TT_MS_LANG_HEBREW = 0x040d;
    public static final int NK_TT_MS_LANG_SWEDISH = 0x041D;
    public static final int NK_TT_MAC_LANG_ENGLISH = 0;
    public static final int NK_TT_MAC_LANG_JAPANESE = 11;
    public static final int NK_TT_MAC_LANG_ARABIC = 12;
    public static final int NK_TT_MAC_LANG_KOREAN = 23;
    public static final int NK_TT_MAC_LANG_DUTCH = 4;
    public static final int NK_TT_MAC_LANG_RUSSIAN = 32;
    public static final int NK_TT_MAC_LANG_FRENCH = 1;
    public static final int NK_TT_MAC_LANG_SPANISH = 6;
    public static final int NK_TT_MAC_LANG_GERMAN = 2;
    public static final int NK_TT_MAC_LANG_SWEDISH = 5;
    public static final int NK_TT_MAC_LANG_HEBREW = 10;
    public static final int NK_TT_MAC_LANG_CHINESE_SIMPLIFIED = 33;
    public static final int NK_TT_MAC_LANG_ITALIAN = 3;
    public static final int NK_TT_MAC_LANG_CHINESE_TRAD = 19;
    public static final int NK_TOGGLE_CHECK = 0;
    public static final int NK_TOGGLE_OPTION = 1;
    public static final int NK_PROPERTY_DEFAULT = 0;
    public static final int NK_PROPERTY_EDIT = 1;
    public static final int NK_PROPERTY_DRAG = 2;
    public static final int NK_FILTER_INT = 0;
    public static final int NK_FILTER_FLOAT = 1;
    public static final int NK_PROPERTY_INT = 0;
    public static final int NK_PROPERTY_FLOAT = 1;
    public static final int NK_PROPERTY_DOUBLE = 2;
    public static final int NK_INSERT_BACK = 0;
    public static final int NK_INSERT_FRONT = 1;
    public static final int NK_GLFW3_DEFAULT = 0;
    public static final int NK_GLFW3_INSTALL_CALLBACKS = 1;

    public static native int nk_init_default(long parg0, long parg1); //struct nk_context*/*ptr*/,const struct nk_user_font*/*ptr*/, //int
    public static native int nk_init_fixed(long parg0, long pmemory, long psize, long parg3); //struct nk_context*/*ptr*/,void*/*ptr*/,nk_size/*ptr*/,const struct nk_user_font*/*ptr*/, //int
    public static native int nk_init(long parg0, long parg1, long parg2); //struct nk_context*/*ptr*/,struct nk_allocator*/*ptr*/,const struct nk_user_font*/*ptr*/, //int
    public static native int nk_init_custom(long parg0, long pcmds, long ppool, long parg3); //struct nk_context*/*ptr*/,struct nk_buffer*/*ptr*/,struct nk_buffer*/*ptr*/,const struct nk_user_font*/*ptr*/, //int
    public static native void nk_clear(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_free(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_set_user_data(long parg0, byte[] phandle); //struct nk_context*/*ptr*/,nk_handle/*none_ptr*/, //void
    public static native void nk_input_begin(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_input_motion(long parg0, int px, int py); //struct nk_context*/*ptr*/,int,int, //void
    public static native void nk_input_key(long parg0, int parg1, int pdown); //struct nk_context*/*ptr*/,enum nk_keys,int, //void
    public static native void nk_input_button(long parg0, int parg1, int px, int py, int pdown); //struct nk_context*/*ptr*/,enum nk_buttons,int,int,int, //void
    public static native void nk_input_scroll(long parg0, float[] pval); //struct nk_context*/*ptr*/,struct nk_vec2/*none_ptr*/, //void
    public static native void nk_input_char(long parg0, byte parg1); //struct nk_context*/*ptr*/,char, //void
    public static native void nk_input_glyph(long parg0, byte[] parg1); //struct nk_context*/*ptr*/,const nk_glyph/*none_ptr*/, //void
    public static native void nk_input_unicode(long parg0, int parg1); //struct nk_context*/*ptr*/,nk_rune, //void
    public static native void nk_input_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native long  nk__begin(long parg0); //struct nk_context*/*ptr*/, //const struct nk_command*/*ptr*/ 
    public static native long  nk__next(long parg0, long parg1); //struct nk_context*/*ptr*/,const struct nk_command*/*ptr*/, //const struct nk_command*/*ptr*/ 
    public static native int nk_convert(long parg0, long pcmds, long pvertices, long pelements, long parg4); //struct nk_context*/*ptr*/,struct nk_buffer*/*ptr*/,struct nk_buffer*/*ptr*/,struct nk_buffer*/*ptr*/,const struct nk_convert_config*/*ptr*/, //nk_flags
    public static native long  nk__draw_begin(long parg0, long parg1); //const struct nk_context*/*ptr*/,const struct nk_buffer*/*ptr*/, //const struct nk_draw_command*/*ptr*/ 
    public static native long  nk__draw_end(long parg0, long parg1); //const struct nk_context*/*ptr*/,const struct nk_buffer*/*ptr*/, //const struct nk_draw_command*/*ptr*/ 
    public static native long  nk__draw_next(long parg0, long parg1, long parg2); //const struct nk_draw_command*/*ptr*/,const struct nk_buffer*/*ptr*/,const struct nk_context*/*ptr*/, //const struct nk_draw_command*/*ptr*/ 
    public static native int nk_begin(long pctx, byte[] ptitle, float[] pbounds, int pflags); //struct nk_context*/*ptr*/,const char*,struct nk_rect/*none_ptr*/,nk_flags, //int
    public static native int nk_begin_titled(long pctx, byte[] pname, byte[] ptitle, float[] pbounds, int pflags); //struct nk_context*/*ptr*/,const char*,const char*,struct nk_rect/*none_ptr*/,nk_flags, //int
    public static native void nk_end(long pctx); //struct nk_context*/*ptr*/, //void
    public static native long  nk_window_find(long pctx, byte[] pname); //struct nk_context*/*ptr*/,const char*, //struct nk_window*/*ptr*/ 
    public static native float[]  nk_window_get_bounds(long pctx); //const struct nk_context*/*ptr*/, //struct nk_rect/*none_ptr*/ 
    public static native float[]  nk_window_get_position(long pctx); //const struct nk_context*/*ptr*/, //struct nk_vec2/*none_ptr*/ 
    public static native float[]  nk_window_get_size(long parg0); //const struct nk_context*/*ptr*/, //struct nk_vec2/*none_ptr*/ 
    public static native float nk_window_get_width(long parg0); //const struct nk_context*/*ptr*/, //float
    public static native float nk_window_get_height(long parg0); //const struct nk_context*/*ptr*/, //float
    public static native long  nk_window_get_panel(long parg0); //struct nk_context*/*ptr*/, //struct nk_panel*/*ptr*/ 
    public static native float[]  nk_window_get_content_region(long parg0); //struct nk_context*/*ptr*/, //struct nk_rect/*none_ptr*/ 
    public static native float[]  nk_window_get_content_region_min(long parg0); //struct nk_context*/*ptr*/, //struct nk_vec2/*none_ptr*/ 
    public static native float[]  nk_window_get_content_region_max(long parg0); //struct nk_context*/*ptr*/, //struct nk_vec2/*none_ptr*/ 
    public static native float[]  nk_window_get_content_region_size(long parg0); //struct nk_context*/*ptr*/, //struct nk_vec2/*none_ptr*/ 
    public static native long  nk_window_get_canvas(long parg0); //struct nk_context*/*ptr*/, //struct nk_command_buffer*/*ptr*/ 
    public static native int nk_window_has_focus(long parg0); //const struct nk_context*/*ptr*/, //int
    public static native int nk_window_is_collapsed(long pctx, byte[] pname); //struct nk_context*/*ptr*/,const char*, //int
    public static native int nk_window_is_closed(long parg0, byte[] parg1); //struct nk_context*/*ptr*/,const char*, //int
    public static native int nk_window_is_hidden(long parg0, byte[] parg1); //struct nk_context*/*ptr*/,const char*, //int
    public static native int nk_window_is_active(long parg0, byte[] parg1); //struct nk_context*/*ptr*/,const char*, //int
    public static native int nk_window_is_hovered(long parg0); //struct nk_context*/*ptr*/, //int
    public static native int nk_window_is_any_hovered(long parg0); //struct nk_context*/*ptr*/, //int
    public static native int nk_item_is_any_active(long parg0); //struct nk_context*/*ptr*/, //int
    public static native void nk_window_set_bounds(long parg0, byte[] pname, float[] pbounds); //struct nk_context*/*ptr*/,const char*,struct nk_rect/*none_ptr*/, //void
    public static native void nk_window_set_position(long parg0, byte[] pname, float[] ppos); //struct nk_context*/*ptr*/,const char*,struct nk_vec2/*none_ptr*/, //void
    public static native void nk_window_set_size(long parg0, byte[] pname, float[] parg2); //struct nk_context*/*ptr*/,const char*,struct nk_vec2/*none_ptr*/, //void
    public static native void nk_window_set_focus(long parg0, byte[] pname); //struct nk_context*/*ptr*/,const char*, //void
    public static native void nk_window_close(long pctx, byte[] pname); //struct nk_context*/*ptr*/,const char*, //void
    public static native void nk_window_collapse(long parg0, byte[] pname, int pstate); //struct nk_context*/*ptr*/,const char*,enum nk_collapse_states, //void
    public static native void nk_window_collapse_if(long parg0, byte[] pname, int parg2, int pcond); //struct nk_context*/*ptr*/,const char*,enum nk_collapse_states,int, //void
    public static native void nk_window_show(long parg0, byte[] pname, int parg2); //struct nk_context*/*ptr*/,const char*,enum nk_show_states, //void
    public static native void nk_window_show_if(long parg0, byte[] pname, int parg2, int pcond); //struct nk_context*/*ptr*/,const char*,enum nk_show_states,int, //void
    public static native void nk_layout_set_min_row_height(long parg0, float pheight); //struct nk_context*/*ptr*/,float, //void
    public static native void nk_layout_reset_min_row_height(long parg0); //struct nk_context*/*ptr*/, //void
    public static native float[]  nk_layout_widget_bounds(long parg0); //struct nk_context*/*ptr*/, //struct nk_rect/*none_ptr*/ 
    public static native float nk_layout_ratio_from_pixel(long parg0, float ppixel_width); //struct nk_context*/*ptr*/,float, //float
    public static native void nk_layout_row_dynamic(long pctx, float pheight, int pcols); //struct nk_context*/*ptr*/,float,int, //void
    public static native void nk_layout_row_static(long pctx, float pheight, int pitem_width, int pcols); //struct nk_context*/*ptr*/,float,int,int, //void
    public static native void nk_layout_row_begin(long pctx, int pfmt, float prow_height, int pcols); //struct nk_context*/*ptr*/,enum nk_layout_format,float,int, //void
    public static native void nk_layout_row_push(long parg0, float pvalue); //struct nk_context*/*ptr*/,float, //void
    public static native void nk_layout_row_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_layout_row(long parg0, int parg1, float pheight, int pcols, float[] pratio); //struct nk_context*/*ptr*/,enum nk_layout_format,float,int,const float*, //void
    public static native void nk_layout_row_template_begin(long parg0, float prow_height); //struct nk_context*/*ptr*/,float, //void
    public static native void nk_layout_row_template_push_dynamic(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_layout_row_template_push_variable(long parg0, float pmin_width); //struct nk_context*/*ptr*/,float, //void
    public static native void nk_layout_row_template_push_static(long parg0, float pwidth); //struct nk_context*/*ptr*/,float, //void
    public static native void nk_layout_row_template_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_layout_space_begin(long parg0, int parg1, float pheight, int pwidget_count); //struct nk_context*/*ptr*/,enum nk_layout_format,float,int, //void
    public static native void nk_layout_space_push(long parg0, float[] parg1); //struct nk_context*/*ptr*/,struct nk_rect/*none_ptr*/, //void
    public static native void nk_layout_space_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native float[]  nk_layout_space_bounds(long parg0); //struct nk_context*/*ptr*/, //struct nk_rect/*none_ptr*/ 
    public static native float[]  nk_layout_space_to_screen(long parg0, float[] parg1); //struct nk_context*/*ptr*/,struct nk_vec2/*none_ptr*/, //struct nk_vec2/*none_ptr*/ 
    public static native float[]  nk_layout_space_to_local(long parg0, float[] parg1); //struct nk_context*/*ptr*/,struct nk_vec2/*none_ptr*/, //struct nk_vec2/*none_ptr*/ 
    public static native float[]  nk_layout_space_rect_to_screen(long parg0, float[] parg1); //struct nk_context*/*ptr*/,struct nk_rect/*none_ptr*/, //struct nk_rect/*none_ptr*/ 
    public static native float[]  nk_layout_space_rect_to_local(long parg0, float[] parg1); //struct nk_context*/*ptr*/,struct nk_rect/*none_ptr*/, //struct nk_rect/*none_ptr*/ 
    public static native int nk_group_begin(long parg0, byte[] ptitle, int parg2); //struct nk_context*/*ptr*/,const char*,nk_flags, //int
    public static native int nk_group_scrolled_offset_begin(long parg0, int[] px_offset, int[] py_offset, byte[] parg3, int parg4); //struct nk_context*/*ptr*/,nk_uint*,nk_uint*,const char*,nk_flags, //int
    public static native int nk_group_scrolled_begin(long parg0, int[] parg1, byte[] ptitle, int parg3); //struct nk_context*/*ptr*/,struct nk_scroll*,const char*,nk_flags, //int
    public static native void nk_group_scrolled_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_group_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native int nk_list_view_begin(long parg0, long pout, byte[] pid, int parg3, int prow_height, int prow_count); //struct nk_context*/*ptr*/,struct nk_list_view*/*ptr*/,const char*,nk_flags,int,int, //int
    public static native void nk_list_view_end(long parg0); //struct nk_list_view*/*ptr*/, //void
    public static native int nk_tree_push_hashed(long parg0, int parg1, byte[] ptitle, int pinitial_state, byte[] phash, int plen, int pseed); //struct nk_context*/*ptr*/,enum nk_tree_type,const char*,enum nk_collapse_states,const char*,int,int, //int
    public static native int nk_tree_image_push_hashed(long parg0, int parg1, byte[] parg2, byte[] ptitle, int pinitial_state, byte[] phash, int plen, int pseed); //struct nk_context*/*ptr*/,enum nk_tree_type,struct nk_image/*none_ptr*/,const char*,enum nk_collapse_states,const char*,int,int, //int
    public static native void nk_tree_pop(long parg0); //struct nk_context*/*ptr*/, //void
    public static native int nk_tree_state_push(long parg0, int parg1, byte[] ptitle, int[] pstate); //struct nk_context*/*ptr*/,enum nk_tree_type,const char*,enum nk_collapse_states*, //int
    public static native int nk_tree_state_image_push(long parg0, int parg1, byte[] parg2, byte[] ptitle, int[] pstate); //struct nk_context*/*ptr*/,enum nk_tree_type,struct nk_image/*none_ptr*/,const char*,enum nk_collapse_states*, //int
    public static native void nk_tree_state_pop(long parg0); //struct nk_context*/*ptr*/, //void
    public static native int nk_widget(float[] parg0, long parg1); //struct nk_rect*,const struct nk_context*/*ptr*/, //enum nk_widget_layout_states
    public static native int nk_widget_fitting(float[] parg0, long parg1, float[] parg2); //struct nk_rect*,struct nk_context*/*ptr*/,struct nk_vec2/*none_ptr*/, //enum nk_widget_layout_states
    public static native float[]  nk_widget_bounds(long parg0); //struct nk_context*/*ptr*/, //struct nk_rect/*none_ptr*/ 
    public static native float[]  nk_widget_position(long parg0); //struct nk_context*/*ptr*/, //struct nk_vec2/*none_ptr*/ 
    public static native float[]  nk_widget_size(long parg0); //struct nk_context*/*ptr*/, //struct nk_vec2/*none_ptr*/ 
    public static native float nk_widget_width(long parg0); //struct nk_context*/*ptr*/, //float
    public static native float nk_widget_height(long parg0); //struct nk_context*/*ptr*/, //float
    public static native int nk_widget_is_hovered(long parg0); //struct nk_context*/*ptr*/, //int
    public static native int nk_widget_is_mouse_clicked(long parg0, int parg1); //struct nk_context*/*ptr*/,enum nk_buttons, //int
    public static native int nk_widget_has_mouse_click_down(long parg0, int parg1, int pdown); //struct nk_context*/*ptr*/,enum nk_buttons,int, //int
    public static native void nk_spacing(long parg0, int pcols); //struct nk_context*/*ptr*/,int, //void
    public static native void nk_text(long parg0, byte[] parg1, int parg2, int parg3); //struct nk_context*/*ptr*/,const char*,int,nk_flags, //void
    public static native void nk_text_colored(long parg0, byte[] parg1, int parg2, int parg3, int[] parg4); //struct nk_context*/*ptr*/,const char*,int,nk_flags,struct nk_color/*none_ptr*/, //void
    public static native void nk_text_wrap(long parg0, byte[] parg1, int parg2); //struct nk_context*/*ptr*/,const char*,int, //void
    public static native void nk_text_wrap_colored(long parg0, byte[] parg1, int parg2, int[] parg3); //struct nk_context*/*ptr*/,const char*,int,struct nk_color/*none_ptr*/, //void
    public static native void nk_label(long parg0, byte[] parg1, int palign); //struct nk_context*/*ptr*/,const char*,nk_flags, //void
    public static native void nk_label_colored(long parg0, byte[] parg1, int palign, int[] parg3); //struct nk_context*/*ptr*/,const char*,nk_flags,struct nk_color/*none_ptr*/, //void
    public static native void nk_label_wrap(long parg0, byte[] parg1); //struct nk_context*/*ptr*/,const char*, //void
    public static native void nk_label_colored_wrap(long parg0, byte[] parg1, int[] parg2); //struct nk_context*/*ptr*/,const char*,struct nk_color/*none_ptr*/, //void
    public static native void nk_image(long parg0, byte[] parg1); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/, //void
    public static native void nk_labelf(long parg0, int parg1, byte[] parg2, String... parg3); //struct nk_context*/*ptr*/,nk_flags,const char*,..., //void
    public static native void nk_labelf_colored(long parg0, int palign, int[] parg2, byte[] parg3, String... parg4); //struct nk_context*/*ptr*/,nk_flags,struct nk_color/*none_ptr*/,const char*,..., //void
    public static native void nk_labelf_wrap(long parg0, byte[] parg1, String... parg2); //struct nk_context*/*ptr*/,const char*,..., //void
    public static native void nk_labelf_colored_wrap(long parg0, int[] parg1, byte[] parg2, String... parg3); //struct nk_context*/*ptr*/,struct nk_color/*none_ptr*/,const char*,..., //void
    public static native void nk_value_bool(long parg0, byte[] pprefix, int parg2); //struct nk_context*/*ptr*/,const char*,int, //void
    public static native void nk_value_int(long parg0, byte[] pprefix, int parg2); //struct nk_context*/*ptr*/,const char*,int, //void
    public static native void nk_value_uint(long parg0, byte[] pprefix, int parg2); //struct nk_context*/*ptr*/,const char*,unsigned int, //void
    public static native void nk_value_float(long parg0, byte[] pprefix, float parg2); //struct nk_context*/*ptr*/,const char*,float, //void
    public static native void nk_value_color_byte(long parg0, byte[] pprefix, int[] parg2); //struct nk_context*/*ptr*/,const char*,struct nk_color/*none_ptr*/, //void
    public static native void nk_value_color_float(long parg0, byte[] pprefix, int[] parg2); //struct nk_context*/*ptr*/,const char*,struct nk_color/*none_ptr*/, //void
    public static native void nk_value_color_hex(long parg0, byte[] pprefix, int[] parg2); //struct nk_context*/*ptr*/,const char*,struct nk_color/*none_ptr*/, //void
    public static native int nk_button_text(long parg0, byte[] ptitle, int plen); //struct nk_context*/*ptr*/,const char*,int, //int
    public static native int nk_button_label(long parg0, byte[] ptitle); //struct nk_context*/*ptr*/,const char*, //int
    public static native int nk_button_color(long parg0, int[] parg1); //struct nk_context*/*ptr*/,struct nk_color/*none_ptr*/, //int
    public static native int nk_button_symbol(long parg0, int parg1); //struct nk_context*/*ptr*/,enum nk_symbol_type, //int
    public static native int nk_button_image(long parg0, byte[] pimg); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/, //int
    public static native int nk_button_symbol_label(long parg0, int parg1, byte[] parg2, int ptext_alignment); //struct nk_context*/*ptr*/,enum nk_symbol_type,const char*,nk_flags, //int
    public static native int nk_button_symbol_text(long parg0, int parg1, byte[] parg2, int parg3, int palignment); //struct nk_context*/*ptr*/,enum nk_symbol_type,const char*,int,nk_flags, //int
    public static native int nk_button_image_label(long parg0, byte[] pimg, byte[] parg2, int ptext_alignment); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,nk_flags, //int
    public static native int nk_button_image_text(long parg0, byte[] pimg, byte[] parg2, int parg3, int palignment); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,int,nk_flags, //int
    public static native int nk_button_text_styled(long parg0, long parg1, byte[] ptitle, int plen); //struct nk_context*/*ptr*/,const struct nk_style_button*/*ptr*/,const char*,int, //int
    public static native int nk_button_label_styled(long parg0, long parg1, byte[] ptitle); //struct nk_context*/*ptr*/,const struct nk_style_button*/*ptr*/,const char*, //int
    public static native int nk_button_symbol_styled(long parg0, long parg1, int parg2); //struct nk_context*/*ptr*/,const struct nk_style_button*/*ptr*/,enum nk_symbol_type, //int
    public static native int nk_button_image_styled(long parg0, long parg1, byte[] pimg); //struct nk_context*/*ptr*/,const struct nk_style_button*/*ptr*/,struct nk_image/*none_ptr*/, //int
    public static native int nk_button_symbol_text_styled(long parg0, long parg1, int parg2, byte[] parg3, int parg4, int palignment); //struct nk_context*/*ptr*/,const struct nk_style_button*/*ptr*/,enum nk_symbol_type,const char*,int,nk_flags, //int
    public static native int nk_button_symbol_label_styled(long pctx, long pstyle, int psymbol, byte[] ptitle, int palign); //struct nk_context*/*ptr*/,const struct nk_style_button*/*ptr*/,enum nk_symbol_type,const char*,nk_flags, //int
    public static native int nk_button_image_label_styled(long parg0, long parg1, byte[] pimg, byte[] parg3, int ptext_alignment); //struct nk_context*/*ptr*/,const struct nk_style_button*/*ptr*/,struct nk_image/*none_ptr*/,const char*,nk_flags, //int
    public static native int nk_button_image_text_styled(long parg0, long parg1, byte[] pimg, byte[] parg3, int parg4, int palignment); //struct nk_context*/*ptr*/,const struct nk_style_button*/*ptr*/,struct nk_image/*none_ptr*/,const char*,int,nk_flags, //int
    public static native void nk_button_set_behavior(long parg0, int parg1); //struct nk_context*/*ptr*/,enum nk_button_behavior, //void
    public static native int nk_button_push_behavior(long parg0, int parg1); //struct nk_context*/*ptr*/,enum nk_button_behavior, //int
    public static native int nk_button_pop_behavior(long parg0); //struct nk_context*/*ptr*/, //int
    public static native int nk_check_label(long parg0, byte[] parg1, int pactive); //struct nk_context*/*ptr*/,const char*,int, //int
    public static native int nk_check_text(long parg0, byte[] parg1, int parg2, int pactive); //struct nk_context*/*ptr*/,const char*,int,int, //int
    public static native int nk_check_flags_label(long parg0, byte[] parg1, int pflags, int pvalue); //struct nk_context*/*ptr*/,const char*,unsigned int,unsigned int, //unsigned
    public static native int nk_check_flags_text(long parg0, byte[] parg1, int parg2, int pflags, int pvalue); //struct nk_context*/*ptr*/,const char*,int,unsigned int,unsigned int, //unsigned
    public static native int nk_checkbox_label(long parg0, byte[] parg1, int[] pactive); //struct nk_context*/*ptr*/,const char*,int*, //int
    public static native int nk_checkbox_text(long parg0, byte[] parg1, int parg2, int[] pactive); //struct nk_context*/*ptr*/,const char*,int,int*, //int
    public static native int nk_checkbox_flags_label(long parg0, byte[] parg1, int[] pflags, int pvalue); //struct nk_context*/*ptr*/,const char*,unsigned int*,unsigned int, //int
    public static native int nk_checkbox_flags_text(long parg0, byte[] parg1, int parg2, int[] pflags, int pvalue); //struct nk_context*/*ptr*/,const char*,int,unsigned int*,unsigned int, //int
    public static native int nk_radio_label(long parg0, byte[] parg1, int[] pactive); //struct nk_context*/*ptr*/,const char*,int*, //int
    public static native int nk_radio_text(long parg0, byte[] parg1, int parg2, int[] pactive); //struct nk_context*/*ptr*/,const char*,int,int*, //int
    public static native int nk_option_label(long parg0, byte[] parg1, int pactive); //struct nk_context*/*ptr*/,const char*,int, //int
    public static native int nk_option_text(long parg0, byte[] parg1, int parg2, int pactive); //struct nk_context*/*ptr*/,const char*,int,int, //int
    public static native int nk_selectable_label(long parg0, byte[] parg1, int palign, int[] pvalue); //struct nk_context*/*ptr*/,const char*,nk_flags,int*, //int
    public static native int nk_selectable_text(long parg0, byte[] parg1, int parg2, int palign, int[] pvalue); //struct nk_context*/*ptr*/,const char*,int,nk_flags,int*, //int
    public static native int nk_selectable_image_label(long parg0, byte[] parg1, byte[] parg2, int palign, int[] pvalue); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,nk_flags,int*, //int
    public static native int nk_selectable_image_text(long parg0, byte[] parg1, byte[] parg2, int parg3, int palign, int[] pvalue); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,int,nk_flags,int*, //int
    public static native int nk_select_label(long parg0, byte[] parg1, int palign, int pvalue); //struct nk_context*/*ptr*/,const char*,nk_flags,int, //int
    public static native int nk_select_text(long parg0, byte[] parg1, int parg2, int palign, int pvalue); //struct nk_context*/*ptr*/,const char*,int,nk_flags,int, //int
    public static native int nk_select_image_label(long parg0, byte[] parg1, byte[] parg2, int palign, int pvalue); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,nk_flags,int, //int
    public static native int nk_select_image_text(long parg0, byte[] parg1, byte[] parg2, int parg3, int palign, int pvalue); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,int,nk_flags,int, //int
    public static native float nk_slide_float(long parg0, float pmin, float pval, float pmax, float pstep); //struct nk_context*/*ptr*/,float,float,float,float, //float
    public static native int nk_slide_int(long parg0, int pmin, int pval, int pmax, int pstep); //struct nk_context*/*ptr*/,int,int,int,int, //int
    public static native int nk_slider_float(long parg0, float pmin, float[] pval, float pmax, float pstep); //struct nk_context*/*ptr*/,float,float*,float,float, //int
    public static native int nk_slider_int(long parg0, int pmin, int[] pval, int pmax, int pstep); //struct nk_context*/*ptr*/,int,int*,int,int, //int
    public static native int nk_progress(long parg0, int[] pcur, long pmax, int pmodifyable); //struct nk_context*/*ptr*/,nk_size*,nk_size/*ptr*/,int, //int
    public static native long  nk_prog(long parg0, long pcur, long pmax, int pmodifyable); //struct nk_context*/*ptr*/,nk_size/*ptr*/,nk_size/*ptr*/,int, //nk_size/*ptr*/ 
    public static native int[]  nk_color_picker(long parg0, int[] parg1, int parg2); //struct nk_context*/*ptr*/,struct nk_color/*none_ptr*/,enum nk_color_format, //struct nk_color/*none_ptr*/ 
    public static native int nk_color_pick(long parg0, int[] parg1, int parg2); //struct nk_context*/*ptr*/,struct nk_color*,enum nk_color_format, //int
    public static native void nk_property_int(long parg0, byte[] pname, int pmin, int[] pval, int pmax, int pstep, float pinc_per_pixel); //struct nk_context*/*ptr*/,const char*,int,int*,int,int,float, //void
    public static native void nk_property_float(long parg0, byte[] pname, float pmin, float[] pval, float pmax, float pstep, float pinc_per_pixel); //struct nk_context*/*ptr*/,const char*,float,float*,float,float,float, //void
    public static native void nk_property_double(long parg0, byte[] pname, double pmin, double[] pval, double pmax, double pstep, float pinc_per_pixel); //struct nk_context*/*ptr*/,const char*,double,double*,double,double,float, //void
    public static native int nk_propertyi(long parg0, byte[] pname, int pmin, int pval, int pmax, int pstep, float pinc_per_pixel); //struct nk_context*/*ptr*/,const char*,int,int,int,int,float, //int
    public static native float nk_propertyf(long parg0, byte[] pname, float pmin, float pval, float pmax, float pstep, float pinc_per_pixel); //struct nk_context*/*ptr*/,const char*,float,float,float,float,float, //float
    public static native double nk_propertyd(long parg0, byte[] pname, double pmin, double pval, double pmax, double pstep, float pinc_per_pixel); //struct nk_context*/*ptr*/,const char*,double,double,double,double,float, //double
    public static native int nk_edit_string(long parg0, int parg1, byte[] pbuffer, int[] plen, int pmax, long parg5); //struct nk_context*/*ptr*/,nk_flags,char*,int*,int,nk_plugin_filter/*ptr*/, //nk_flags
    public static native int nk_edit_string_zero_terminated(long parg0, int parg1, byte[] pbuffer, int pmax, long parg4); //struct nk_context*/*ptr*/,nk_flags,char*,int,nk_plugin_filter/*ptr*/, //nk_flags
    public static native int nk_edit_buffer(long parg0, int parg1, long parg2, long parg3); //struct nk_context*/*ptr*/,nk_flags,struct nk_text_edit*/*ptr*/,nk_plugin_filter/*ptr*/, //nk_flags
    public static native void nk_edit_focus(long parg0, int pflags); //struct nk_context*/*ptr*/,nk_flags, //void
    public static native void nk_edit_unfocus(long parg0); //struct nk_context*/*ptr*/, //void
    public static native int nk_chart_begin(long parg0, int parg1, int pnum, float pmin, float pmax); //struct nk_context*/*ptr*/,enum nk_chart_type,int,float,float, //int
    public static native int nk_chart_begin_colored(long parg0, int parg1, int[] parg2, int[] pactive, int pnum, float pmin, float pmax); //struct nk_context*/*ptr*/,enum nk_chart_type,struct nk_color/*none_ptr*/,struct nk_color/*none_ptr*/,int,float,float, //int
    public static native void nk_chart_add_slot(long pctx, int parg1, int pcount, float pmin_value, float pmax_value); //struct nk_context*/*ptr*/,const enum nk_chart_type,int,float,float, //void
    public static native void nk_chart_add_slot_colored(long pctx, int parg1, int[] parg2, int[] pactive, int pcount, float pmin_value, float pmax_value); //struct nk_context*/*ptr*/,const enum nk_chart_type,struct nk_color/*none_ptr*/,struct nk_color/*none_ptr*/,int,float,float, //void
    public static native int nk_chart_push(long parg0, float parg1); //struct nk_context*/*ptr*/,float, //nk_flags
    public static native int nk_chart_push_slot(long parg0, float parg1, int parg2); //struct nk_context*/*ptr*/,float,int, //nk_flags
    public static native void nk_chart_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_plot(long parg0, int parg1, float[] pvalues, int pcount, int poffset); //struct nk_context*/*ptr*/,enum nk_chart_type,const float*,int,int, //void
    public static native void nk_plot_function(long parg0, int parg1, long puserdata, long parg3, int pcount, int poffset); //struct nk_context*/*ptr*/,enum nk_chart_type,void*/*ptr*/,void*/*ptr*/,int,int, //void
    public static native int nk_popup_begin(long parg0, int parg1, byte[] parg2, int parg3, float[] pbounds); //struct nk_context*/*ptr*/,enum nk_popup_type,const char*,nk_flags,struct nk_rect/*none_ptr*/, //int
    public static native void nk_popup_close(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_popup_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native int nk_combo(long parg0, byte[][] pitems, int pcount, int pselected, int pitem_height, float[] psize); //struct nk_context*/*ptr*/,const char**,int,int,int,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_separator(long parg0, byte[] pitems_separated_by_separator, int pseparator, int pselected, int pcount, int pitem_height, float[] psize); //struct nk_context*/*ptr*/,const char*,int,int,int,int,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_string(long parg0, byte[] pitems_separated_by_zeros, int pselected, int pcount, int pitem_height, float[] psize); //struct nk_context*/*ptr*/,const char*,int,int,int,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_callback(long parg0, long parg1, long puserdata, int pselected, int pcount, int pitem_height, float[] psize); //struct nk_context*/*ptr*/,void*/*ptr*/,void*/*ptr*/,int,int,int,struct nk_vec2/*none_ptr*/, //int
    public static native void nk_combobox(long parg0, byte[][] pitems, int pcount, int[] pselected, int pitem_height, float[] psize); //struct nk_context*/*ptr*/,const char**,int,int*,int,struct nk_vec2/*none_ptr*/, //void
    public static native void nk_combobox_string(long parg0, byte[] pitems_separated_by_zeros, int[] pselected, int pcount, int pitem_height, float[] psize); //struct nk_context*/*ptr*/,const char*,int*,int,int,struct nk_vec2/*none_ptr*/, //void
    public static native void nk_combobox_separator(long parg0, byte[] pitems_separated_by_separator, int pseparator, int[] pselected, int pcount, int pitem_height, float[] psize); //struct nk_context*/*ptr*/,const char*,int,int*,int,int,struct nk_vec2/*none_ptr*/, //void
    public static native void nk_combobox_callback(long parg0, long parg1, long parg2, int[] pselected, int pcount, int pitem_height, float[] psize); //struct nk_context*/*ptr*/,void*/*ptr*/,void*/*ptr*/,int*,int,int,struct nk_vec2/*none_ptr*/, //void
    public static native int nk_combo_begin_text(long parg0, byte[] pselected, int parg2, float[] psize); //struct nk_context*/*ptr*/,const char*,int,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_begin_label(long parg0, byte[] pselected, float[] psize); //struct nk_context*/*ptr*/,const char*,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_begin_color(long parg0, int[] pcolor, float[] psize); //struct nk_context*/*ptr*/,struct nk_color/*none_ptr*/,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_begin_symbol(long parg0, int parg1, float[] psize); //struct nk_context*/*ptr*/,enum nk_symbol_type,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_begin_symbol_label(long parg0, byte[] pselected, int parg2, float[] psize); //struct nk_context*/*ptr*/,const char*,enum nk_symbol_type,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_begin_symbol_text(long parg0, byte[] pselected, int parg2, int parg3, float[] psize); //struct nk_context*/*ptr*/,const char*,int,enum nk_symbol_type,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_begin_image(long parg0, byte[] pimg, float[] psize); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_begin_image_label(long parg0, byte[] pselected, byte[] parg2, float[] psize); //struct nk_context*/*ptr*/,const char*,struct nk_image/*none_ptr*/,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_begin_image_text(long parg0, byte[] pselected, int parg2, byte[] parg3, float[] psize); //struct nk_context*/*ptr*/,const char*,int,struct nk_image/*none_ptr*/,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_combo_item_label(long parg0, byte[] parg1, int palignment); //struct nk_context*/*ptr*/,const char*,nk_flags, //int
    public static native int nk_combo_item_text(long parg0, byte[] parg1, int parg2, int palignment); //struct nk_context*/*ptr*/,const char*,int,nk_flags, //int
    public static native int nk_combo_item_image_label(long parg0, byte[] parg1, byte[] parg2, int palignment); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,nk_flags, //int
    public static native int nk_combo_item_image_text(long parg0, byte[] parg1, byte[] parg2, int parg3, int palignment); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,int,nk_flags, //int
    public static native int nk_combo_item_symbol_label(long parg0, int parg1, byte[] parg2, int palignment); //struct nk_context*/*ptr*/,enum nk_symbol_type,const char*,nk_flags, //int
    public static native int nk_combo_item_symbol_text(long parg0, int parg1, byte[] parg2, int parg3, int palignment); //struct nk_context*/*ptr*/,enum nk_symbol_type,const char*,int,nk_flags, //int
    public static native void nk_combo_close(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_combo_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native int nk_contextual_begin(long parg0, int parg1, float[] parg2, float[] ptrigger_bounds); //struct nk_context*/*ptr*/,nk_flags,struct nk_vec2/*none_ptr*/,struct nk_rect/*none_ptr*/, //int
    public static native int nk_contextual_item_text(long parg0, byte[] parg1, int parg2, int palign); //struct nk_context*/*ptr*/,const char*,int,nk_flags, //int
    public static native int nk_contextual_item_label(long parg0, byte[] parg1, int palign); //struct nk_context*/*ptr*/,const char*,nk_flags, //int
    public static native int nk_contextual_item_image_label(long parg0, byte[] parg1, byte[] parg2, int palignment); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,nk_flags, //int
    public static native int nk_contextual_item_image_text(long parg0, byte[] parg1, byte[] parg2, int plen, int palignment); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,int,nk_flags, //int
    public static native int nk_contextual_item_symbol_label(long parg0, int parg1, byte[] parg2, int palignment); //struct nk_context*/*ptr*/,enum nk_symbol_type,const char*,nk_flags, //int
    public static native int nk_contextual_item_symbol_text(long parg0, int parg1, byte[] parg2, int parg3, int palignment); //struct nk_context*/*ptr*/,enum nk_symbol_type,const char*,int,nk_flags, //int
    public static native void nk_contextual_close(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_contextual_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_tooltip(long parg0, byte[] parg1); //struct nk_context*/*ptr*/,const char*, //void
    public static native void nk_tooltipf(long parg0, byte[] parg1, String... parg2); //struct nk_context*/*ptr*/,const char*,..., //void
    public static native int nk_tooltip_begin(long parg0, float pwidth); //struct nk_context*/*ptr*/,float, //int
    public static native void nk_tooltip_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_menubar_begin(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_menubar_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native int nk_menu_begin_text(long parg0, byte[] ptitle, int ptitle_len, int palign, float[] psize); //struct nk_context*/*ptr*/,const char*,int,nk_flags,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_menu_begin_label(long parg0, byte[] parg1, int palign, float[] psize); //struct nk_context*/*ptr*/,const char*,nk_flags,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_menu_begin_image(long parg0, byte[] parg1, byte[] parg2, float[] psize); //struct nk_context*/*ptr*/,const char*,struct nk_image/*none_ptr*/,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_menu_begin_image_text(long parg0, byte[] parg1, int parg2, int palign, byte[] parg4, float[] psize); //struct nk_context*/*ptr*/,const char*,int,nk_flags,struct nk_image/*none_ptr*/,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_menu_begin_image_label(long parg0, byte[] parg1, int palign, byte[] parg3, float[] psize); //struct nk_context*/*ptr*/,const char*,nk_flags,struct nk_image/*none_ptr*/,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_menu_begin_symbol(long parg0, byte[] parg1, int parg2, float[] psize); //struct nk_context*/*ptr*/,const char*,enum nk_symbol_type,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_menu_begin_symbol_text(long parg0, byte[] parg1, int parg2, int palign, int parg4, float[] psize); //struct nk_context*/*ptr*/,const char*,int,nk_flags,enum nk_symbol_type,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_menu_begin_symbol_label(long parg0, byte[] parg1, int palign, int parg3, float[] psize); //struct nk_context*/*ptr*/,const char*,nk_flags,enum nk_symbol_type,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_menu_item_text(long parg0, byte[] parg1, int parg2, int palign); //struct nk_context*/*ptr*/,const char*,int,nk_flags, //int
    public static native int nk_menu_item_label(long parg0, byte[] parg1, int palignment); //struct nk_context*/*ptr*/,const char*,nk_flags, //int
    public static native int nk_menu_item_image_label(long parg0, byte[] parg1, byte[] parg2, int palignment); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,nk_flags, //int
    public static native int nk_menu_item_image_text(long parg0, byte[] parg1, byte[] parg2, int plen, int palignment); //struct nk_context*/*ptr*/,struct nk_image/*none_ptr*/,const char*,int,nk_flags, //int
    public static native int nk_menu_item_symbol_text(long parg0, int parg1, byte[] parg2, int parg3, int palignment); //struct nk_context*/*ptr*/,enum nk_symbol_type,const char*,int,nk_flags, //int
    public static native int nk_menu_item_symbol_label(long parg0, int parg1, byte[] parg2, int palignment); //struct nk_context*/*ptr*/,enum nk_symbol_type,const char*,nk_flags, //int
    public static native void nk_menu_close(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_menu_end(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_style_default(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_style_from_table(long parg0, int[] parg1); //struct nk_context*/*ptr*/,const struct nk_color*, //void
    public static native void nk_style_load_cursor(long parg0, int parg1, long parg2); //struct nk_context*/*ptr*/,enum nk_style_cursor,const struct nk_cursor*/*ptr*/, //void
    public static native void nk_style_load_all_cursors(long parg0, long parg1); //struct nk_context*/*ptr*/,struct nk_cursor*/*ptr*/, //void
    public static native byte[] nk_style_get_color_by_name(int parg0); //enum nk_style_colors, //const char*
    public static native void nk_style_set_font(long parg0, long parg1); //struct nk_context*/*ptr*/,const struct nk_user_font*/*ptr*/, //void
    public static native int nk_style_set_cursor(long parg0, int parg1); //struct nk_context*/*ptr*/,enum nk_style_cursor, //int
    public static native void nk_style_show_cursor(long parg0); //struct nk_context*/*ptr*/, //void
    public static native void nk_style_hide_cursor(long parg0); //struct nk_context*/*ptr*/, //void
    public static native int nk_style_push_font(long parg0, long parg1); //struct nk_context*/*ptr*/,const struct nk_user_font*/*ptr*/, //int
    public static native int nk_style_push_float(long parg0, float[] parg1, float parg2); //struct nk_context*/*ptr*/,float*,float, //int
    public static native int nk_style_push_vec2(long parg0, float[] parg1, float[] parg2); //struct nk_context*/*ptr*/,struct nk_vec2*,struct nk_vec2/*none_ptr*/, //int
    public static native int nk_style_push_style_item(long parg0, long parg1, byte[] parg2); //struct nk_context*/*ptr*/,struct nk_style_item*/*ptr*/,struct nk_style_item/*none_ptr*/, //int
    public static native int nk_style_push_flags(long parg0, int[] parg1, int parg2); //struct nk_context*/*ptr*/,nk_flags*,nk_flags, //int
    public static native int nk_style_push_color(long parg0, int[] parg1, int[] parg2); //struct nk_context*/*ptr*/,struct nk_color*,struct nk_color/*none_ptr*/, //int
    public static native int nk_style_pop_font(long parg0); //struct nk_context*/*ptr*/, //int
    public static native int nk_style_pop_float(long parg0); //struct nk_context*/*ptr*/, //int
    public static native int nk_style_pop_vec2(long parg0); //struct nk_context*/*ptr*/, //int
    public static native int nk_style_pop_style_item(long parg0); //struct nk_context*/*ptr*/, //int
    public static native int nk_style_pop_flags(long parg0); //struct nk_context*/*ptr*/, //int
    public static native int nk_style_pop_color(long parg0); //struct nk_context*/*ptr*/, //int
    public static native int[]  nk_rgb(int pr, int pg, int pb); //int,int,int, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgb_iv(int[] prgb); //const int*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgb_bv(byte[] prgb); //const nk_byte*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgb_f(float pr, float pg, float pb); //float,float,float, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgb_fv(float[] prgb); //const float*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgb_hex(byte[] prgb); //const char*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgba(int pr, int pg, int pb, int pa); //int,int,int,int, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgba_u32(int parg0); //nk_uint, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgba_iv(int[] prgba); //const int*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgba_bv(byte[] prgba); //const nk_byte*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgba_f(float pr, float pg, float pb, float pa); //float,float,float,float, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgba_fv(float[] prgba); //const float*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_rgba_hex(byte[] prgb); //const char*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_hsv(int ph, int ps, int pv); //int,int,int, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_hsv_iv(int[] phsv); //const int*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_hsv_bv(byte[] phsv); //const nk_byte*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_hsv_f(float ph, float ps, float pv); //float,float,float, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_hsv_fv(float[] phsv); //const float*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_hsva(int ph, int ps, int pv, int pa); //int,int,int,int, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_hsva_iv(int[] phsva); //const int*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_hsva_bv(byte[] phsva); //const nk_byte*, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_hsva_f(float ph, float ps, float pv, float pa); //float,float,float,float, //struct nk_color/*none_ptr*/ 
    public static native int[]  nk_hsva_fv(float[] phsva); //const float*, //struct nk_color/*none_ptr*/ 
    public static native void nk_color_f(float[] pr, float[] pg, float[] pb, float[] pa, int[] parg4); //float*,float*,float*,float*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_fv(float[] prgba_out, int[] parg1); //float*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_d(double[] pr, double[] pg, double[] pb, double[] pa, int[] parg4); //double*,double*,double*,double*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_dv(double[] prgba_out, int[] parg1); //double*,struct nk_color/*none_ptr*/, //void
    public static native int nk_color_u32(int[] parg0); //struct nk_color/*none_ptr*/, //nk_uint
    public static native void nk_color_hex_rgba(byte[] poutput, int[] parg1); //char*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hex_rgb(byte[] poutput, int[] parg1); //char*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsv_i(int[] pout_h, int[] pout_s, int[] pout_v, int[] parg3); //int*,int*,int*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsv_b(byte[] pout_h, byte[] pout_s, byte[] pout_v, int[] parg3); //nk_byte*,nk_byte*,nk_byte*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsv_iv(int[] phsv_out, int[] parg1); //int*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsv_bv(byte[] phsv_out, int[] parg1); //nk_byte*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsv_f(float[] pout_h, float[] pout_s, float[] pout_v, int[] parg3); //float*,float*,float*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsv_fv(float[] phsv_out, int[] parg1); //float*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsva_i(int[] ph, int[] ps, int[] pv, int[] pa, int[] parg4); //int*,int*,int*,int*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsva_b(byte[] ph, byte[] ps, byte[] pv, byte[] pa, int[] parg4); //nk_byte*,nk_byte*,nk_byte*,nk_byte*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsva_iv(int[] phsva_out, int[] parg1); //int*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsva_bv(byte[] phsva_out, int[] parg1); //nk_byte*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsva_f(float[] pout_h, float[] pout_s, float[] pout_v, float[] pout_a, int[] parg4); //float*,float*,float*,float*,struct nk_color/*none_ptr*/, //void
    public static native void nk_color_hsva_fv(float[] phsva_out, int[] parg1); //float*,struct nk_color/*none_ptr*/, //void
    public static native byte[]  nk_handle_ptr(long parg0); //void*/*ptr*/, //nk_handle/*none_ptr*/ 
    public static native byte[]  nk_handle_id(int parg0); //int, //nk_handle/*none_ptr*/ 
    public static native byte[]  nk_image_handle(byte[] parg0); //nk_handle/*none_ptr*/, //struct nk_image/*none_ptr*/ 
    public static native byte[]  nk_image_ptr(long parg0); //void*/*ptr*/, //struct nk_image/*none_ptr*/ 
    public static native byte[]  nk_image_id(int parg0); //int, //struct nk_image/*none_ptr*/ 
    public static native int nk_image_is_subimage(long pimg); //const struct nk_image*/*ptr*/, //int
    public static native byte[]  nk_subimage_ptr(long parg0, short pw, short ph, float[] psub_region); //void*/*ptr*/,unsigned short,unsigned short,struct nk_rect/*none_ptr*/, //struct nk_image/*none_ptr*/ 
    public static native byte[]  nk_subimage_id(int parg0, short pw, short ph, float[] psub_region); //int,unsigned short,unsigned short,struct nk_rect/*none_ptr*/, //struct nk_image/*none_ptr*/ 
    public static native byte[]  nk_subimage_handle(byte[] parg0, short pw, short ph, float[] psub_region); //nk_handle/*none_ptr*/,unsigned short,unsigned short,struct nk_rect/*none_ptr*/, //struct nk_image/*none_ptr*/ 
    public static native int nk_murmur_hash(long pkey, int plen, int pseed); //const void*/*ptr*/,int,nk_hash, //nk_hash
    public static native void nk_triangle_from_direction(float[] presult, float[] pr, float ppad_x, float ppad_y, int parg4); //struct nk_vec2*,struct nk_rect/*none_ptr*/,float,float,enum nk_heading, //void
    public static native float[]  nk_vec2(float px, float py); //float,float, //struct nk_vec2/*none_ptr*/ 
    public static native float[]  nk_vec2i(int px, int py); //int,int, //struct nk_vec2/*none_ptr*/ 
    public static native float[]  nk_vec2v(float[] pxy); //const float*, //struct nk_vec2/*none_ptr*/ 
    public static native float[]  nk_vec2iv(int[] pxy); //const int*, //struct nk_vec2/*none_ptr*/ 
    public static native float[]  nk_get_null_rect(); //void, //struct nk_rect/*none_ptr*/ 
    public static native float[]  nk_rect(float px, float py, float pw, float ph); //float,float,float,float, //struct nk_rect/*none_ptr*/ 
    public static native float[]  nk_recti(int px, int py, int pw, int ph); //int,int,int,int, //struct nk_rect/*none_ptr*/ 
    public static native float[]  nk_recta(float[] ppos, float[] psize); //struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/, //struct nk_rect/*none_ptr*/ 
    public static native float[]  nk_rectv(float[] pxywh); //const float*, //struct nk_rect/*none_ptr*/ 
    public static native float[]  nk_rectiv(int[] pxywh); //const int*, //struct nk_rect/*none_ptr*/ 
    public static native float[]  nk_rect_pos(float[] parg0); //struct nk_rect/*none_ptr*/, //struct nk_vec2/*none_ptr*/ 
    public static native float[]  nk_rect_size(float[] parg0); //struct nk_rect/*none_ptr*/, //struct nk_vec2/*none_ptr*/ 
    public static native int nk_strlen(byte[] pstr); //const char*, //int
    public static native int nk_stricmp(byte[] ps1, byte[] ps2); //const char*,const char*, //int
    public static native int nk_stricmpn(byte[] ps1, byte[] ps2, int pn); //const char*,const char*,int, //int
    public static native int nk_strtoi(byte[] pstr, byte[][] pendptr); //const char*,const char**, //int
    public static native float nk_strtof(byte[] pstr, byte[][] pendptr); //const char*,const char**, //float
    public static native double nk_strtod(byte[] pstr, byte[][] pendptr); //const char*,const char**, //double
    public static native int nk_strfilter(byte[] ptext, byte[] pregexp); //const char*,const char*, //int
    public static native int nk_strmatch_fuzzy_string(byte[] pstr, byte[] ppattern, int[] pout_score); //char const*,char const*,int*, //int
    public static native int nk_strmatch_fuzzy_text(byte[] ptxt, int ptxt_len, byte[] ppattern, int[] pout_score); //const char*,int,const char*,int*, //int
    public static native int nk_utf_decode(byte[] parg0, long parg1, int parg2); //const char*,nk_rune*/*ptr*/,int, //int
    public static native int nk_utf_encode(int parg0, byte[] parg1, int parg2); //nk_rune,char*,int, //int
    public static native int nk_utf_len(byte[] parg0, int pbyte_len); //const char*,int, //int
    public static native byte[] nk_utf_at(byte[] pbuffer, int plength, int pindex, long punicode, int[] plen); //const char*,int,int,nk_rune*/*ptr*/,int*, //const char*
    public static native long  nk_font_default_glyph_ranges(); //void, //const nk_rune*/*ptr*/ 
    public static native long  nk_font_chinese_glyph_ranges(); //void, //const nk_rune*/*ptr*/ 
    public static native long  nk_font_cyrillic_glyph_ranges(); //void, //const nk_rune*/*ptr*/ 
    public static native long  nk_font_korean_glyph_ranges(); //void, //const nk_rune*/*ptr*/ 
    public static native void nk_font_atlas_init_default(long parg0); //struct nk_font_atlas*/*ptr*/, //void
    public static native void nk_font_atlas_init(long parg0, long parg1); //struct nk_font_atlas*/*ptr*/,struct nk_allocator*/*ptr*/, //void
    public static native void nk_font_atlas_init_custom(long parg0, long ppersistent, long ptransient); //struct nk_font_atlas*/*ptr*/,struct nk_allocator*/*ptr*/,struct nk_allocator*/*ptr*/, //void
    public static native void nk_font_atlas_begin(long parg0); //struct nk_font_atlas*/*ptr*/, //void
    public static native byte[]  nk_font_config(float ppixel_height); //float, //struct nk_font_config/*none_ptr*/ 
    public static native long  nk_font_atlas_add(long parg0, long parg1); //struct nk_font_atlas*/*ptr*/,const struct nk_font_config*/*ptr*/, //struct nk_font*/*ptr*/ 
    public static native long  nk_font_atlas_add_default(long parg0, float pheight, long parg2); //struct nk_font_atlas*/*ptr*/,float,const struct nk_font_config*/*ptr*/, //struct nk_font*/*ptr*/ 
    public static native long  nk_font_atlas_add_from_memory(long patlas, long pmemory, long psize, float pheight, long pconfig); //struct nk_font_atlas*/*ptr*/,void*/*ptr*/,nk_size/*ptr*/,float,const struct nk_font_config*/*ptr*/, //struct nk_font*/*ptr*/ 
    public static native long  nk_font_atlas_add_from_file(long patlas, byte[] pfile_path, float pheight, long parg3); //struct nk_font_atlas*/*ptr*/,const char*,float,const struct nk_font_config*/*ptr*/, //struct nk_font*/*ptr*/ 
    public static native long  nk_font_atlas_add_compressed(long parg0, long pmemory, long psize, float pheight, long parg4); //struct nk_font_atlas*/*ptr*/,void*/*ptr*/,nk_size/*ptr*/,float,const struct nk_font_config*/*ptr*/, //struct nk_font*/*ptr*/ 
    public static native long  nk_font_atlas_add_compressed_base85(long parg0, byte[] pdata, float pheight, long pconfig); //struct nk_font_atlas*/*ptr*/,const char*,float,const struct nk_font_config*/*ptr*/, //struct nk_font*/*ptr*/ 
    public static native long  nk_font_atlas_bake(long parg0, int[] pwidth, int[] pheight, int parg3); //struct nk_font_atlas*/*ptr*/,int*,int*,enum nk_font_atlas_format, //const void*/*ptr*/ 
    public static native void nk_font_atlas_end(long parg0, byte[] ptex, long parg2); //struct nk_font_atlas*/*ptr*/,nk_handle/*none_ptr*/,struct nk_draw_null_texture*/*ptr*/, //void
    public static native long  nk_font_find_glyph(long parg0, int punicode); //struct nk_font*/*ptr*/,nk_rune, //const struct nk_font_glyph*/*ptr*/ 
    public static native void nk_font_atlas_cleanup(long patlas); //struct nk_font_atlas*/*ptr*/, //void
    public static native void nk_font_atlas_clear(long parg0); //struct nk_font_atlas*/*ptr*/, //void
    public static native void nk_buffer_init_default(long parg0); //struct nk_buffer*/*ptr*/, //void
    public static native void nk_buffer_init(long parg0, long parg1, long psize); //struct nk_buffer*/*ptr*/,const struct nk_allocator*/*ptr*/,nk_size/*ptr*/, //void
    public static native void nk_buffer_init_fixed(long parg0, long pmemory, long psize); //struct nk_buffer*/*ptr*/,void*/*ptr*/,nk_size/*ptr*/, //void
    public static native void nk_buffer_info(long parg0, long parg1); //struct nk_memory_status*/*ptr*/,struct nk_buffer*/*ptr*/, //void
    public static native void nk_buffer_push(long parg0, int ptype, long pmemory, long psize, long palign); //struct nk_buffer*/*ptr*/,enum nk_buffer_allocation_type,const void*/*ptr*/,nk_size/*ptr*/,nk_size/*ptr*/, //void
    public static native void nk_buffer_mark(long parg0, int ptype); //struct nk_buffer*/*ptr*/,enum nk_buffer_allocation_type, //void
    public static native void nk_buffer_reset(long parg0, int ptype); //struct nk_buffer*/*ptr*/,enum nk_buffer_allocation_type, //void
    public static native void nk_buffer_clear(long parg0); //struct nk_buffer*/*ptr*/, //void
    public static native void nk_buffer_free(long parg0); //struct nk_buffer*/*ptr*/, //void
    public static native long  nk_buffer_memory(long parg0); //struct nk_buffer*/*ptr*/, //void*/*ptr*/ 
    public static native long  nk_buffer_memory_const(long parg0); //const struct nk_buffer*/*ptr*/, //const void*/*ptr*/ 
    public static native long  nk_buffer_total(long parg0); //struct nk_buffer*/*ptr*/, //nk_size/*ptr*/ 
    public static native void nk_str_init_default(long parg0); //struct nk_str*/*ptr*/, //void
    public static native void nk_str_init(long parg0, long parg1, long psize); //struct nk_str*/*ptr*/,const struct nk_allocator*/*ptr*/,nk_size/*ptr*/, //void
    public static native void nk_str_init_fixed(long parg0, long pmemory, long psize); //struct nk_str*/*ptr*/,void*/*ptr*/,nk_size/*ptr*/, //void
    public static native void nk_str_clear(long parg0); //struct nk_str*/*ptr*/, //void
    public static native void nk_str_free(long parg0); //struct nk_str*/*ptr*/, //void
    public static native int nk_str_append_text_char(long parg0, byte[] parg1, int parg2); //struct nk_str*/*ptr*/,const char*,int, //int
    public static native int nk_str_append_str_char(long parg0, byte[] parg1); //struct nk_str*/*ptr*/,const char*, //int
    public static native int nk_str_append_text_utf8(long parg0, byte[] parg1, int parg2); //struct nk_str*/*ptr*/,const char*,int, //int
    public static native int nk_str_append_str_utf8(long parg0, byte[] parg1); //struct nk_str*/*ptr*/,const char*, //int
    public static native int nk_str_append_text_runes(long parg0, long parg1, int parg2); //struct nk_str*/*ptr*/,const nk_rune*/*ptr*/,int, //int
    public static native int nk_str_append_str_runes(long parg0, long parg1); //struct nk_str*/*ptr*/,const nk_rune*/*ptr*/, //int
    public static native int nk_str_insert_at_char(long parg0, int ppos, byte[] parg2, int parg3); //struct nk_str*/*ptr*/,int,const char*,int, //int
    public static native int nk_str_insert_at_rune(long parg0, int ppos, byte[] parg2, int parg3); //struct nk_str*/*ptr*/,int,const char*,int, //int
    public static native int nk_str_insert_text_char(long parg0, int ppos, byte[] parg2, int parg3); //struct nk_str*/*ptr*/,int,const char*,int, //int
    public static native int nk_str_insert_str_char(long parg0, int ppos, byte[] parg2); //struct nk_str*/*ptr*/,int,const char*, //int
    public static native int nk_str_insert_text_utf8(long parg0, int ppos, byte[] parg2, int parg3); //struct nk_str*/*ptr*/,int,const char*,int, //int
    public static native int nk_str_insert_str_utf8(long parg0, int ppos, byte[] parg2); //struct nk_str*/*ptr*/,int,const char*, //int
    public static native int nk_str_insert_text_runes(long parg0, int ppos, long parg2, int parg3); //struct nk_str*/*ptr*/,int,const nk_rune*/*ptr*/,int, //int
    public static native int nk_str_insert_str_runes(long parg0, int ppos, long parg2); //struct nk_str*/*ptr*/,int,const nk_rune*/*ptr*/, //int
    public static native void nk_str_remove_chars(long parg0, int plen); //struct nk_str*/*ptr*/,int, //void
    public static native void nk_str_remove_runes(long pstr, int plen); //struct nk_str*/*ptr*/,int, //void
    public static native void nk_str_delete_chars(long parg0, int ppos, int plen); //struct nk_str*/*ptr*/,int,int, //void
    public static native void nk_str_delete_runes(long parg0, int ppos, int plen); //struct nk_str*/*ptr*/,int,int, //void
    public static native byte[] nk_str_at_char(long parg0, int ppos); //struct nk_str*/*ptr*/,int, //char*
    public static native byte[] nk_str_at_rune(long parg0, int ppos, long punicode, int[] plen); //struct nk_str*/*ptr*/,int,nk_rune*/*ptr*/,int*, //char*
    public static native int nk_str_rune_at(long parg0, int ppos); //const struct nk_str*/*ptr*/,int, //nk_rune
    public static native byte[] nk_str_at_char_const(long parg0, int ppos); //const struct nk_str*/*ptr*/,int, //const char*
    public static native byte[] nk_str_at_const(long parg0, int ppos, long punicode, int[] plen); //const struct nk_str*/*ptr*/,int,nk_rune*/*ptr*/,int*, //const char*
    public static native byte[] nk_str_get(long parg0); //struct nk_str*/*ptr*/, //char*
    public static native byte[] nk_str_get_const(long parg0); //const struct nk_str*/*ptr*/, //const char*
    public static native int nk_str_len(long parg0); //struct nk_str*/*ptr*/, //int
    public static native int nk_str_len_char(long parg0); //struct nk_str*/*ptr*/, //int
    public static native int nk_filter_default(long parg0, int punicode); //const struct nk_text_edit*/*ptr*/,nk_rune, //int
    public static native int nk_filter_ascii(long parg0, int punicode); //const struct nk_text_edit*/*ptr*/,nk_rune, //int
    public static native int nk_filter_float(long parg0, int punicode); //const struct nk_text_edit*/*ptr*/,nk_rune, //int
    public static native int nk_filter_decimal(long parg0, int punicode); //const struct nk_text_edit*/*ptr*/,nk_rune, //int
    public static native int nk_filter_hex(long parg0, int punicode); //const struct nk_text_edit*/*ptr*/,nk_rune, //int
    public static native int nk_filter_oct(long parg0, int punicode); //const struct nk_text_edit*/*ptr*/,nk_rune, //int
    public static native int nk_filter_binary(long parg0, int punicode); //const struct nk_text_edit*/*ptr*/,nk_rune, //int
    public static native void nk_textedit_init_default(long parg0); //struct nk_text_edit*/*ptr*/, //void
    public static native void nk_textedit_init(long parg0, long parg1, long psize); //struct nk_text_edit*/*ptr*/,struct nk_allocator*/*ptr*/,nk_size/*ptr*/, //void
    public static native void nk_textedit_init_fixed(long parg0, long pmemory, long psize); //struct nk_text_edit*/*ptr*/,void*/*ptr*/,nk_size/*ptr*/, //void
    public static native void nk_textedit_free(long parg0); //struct nk_text_edit*/*ptr*/, //void
    public static native void nk_textedit_text(long parg0, byte[] parg1, int ptotal_len); //struct nk_text_edit*/*ptr*/,const char*,int, //void
    public static native void nk_textedit_delete(long parg0, int pwhere, int plen); //struct nk_text_edit*/*ptr*/,int,int, //void
    public static native void nk_textedit_delete_selection(long parg0); //struct nk_text_edit*/*ptr*/, //void
    public static native void nk_textedit_select_all(long parg0); //struct nk_text_edit*/*ptr*/, //void
    public static native int nk_textedit_cut(long parg0); //struct nk_text_edit*/*ptr*/, //int
    public static native int nk_textedit_paste(long parg0, byte[] parg1, int plen); //struct nk_text_edit*/*ptr*/,char const*,int, //int
    public static native void nk_textedit_undo(long parg0); //struct nk_text_edit*/*ptr*/, //void
    public static native void nk_textedit_redo(long parg0); //struct nk_text_edit*/*ptr*/, //void
    public static native void nk_stroke_line(long pb, float px0, float py0, float px1, float py1, float pline_thickness, int[] parg6); //struct nk_command_buffer*/*ptr*/,float,float,float,float,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_stroke_curve(long parg0, float parg1, float parg2, float parg3, float parg4, float parg5, float parg6, float parg7, float parg8, float pline_thickness, int[] parg10); //struct nk_command_buffer*/*ptr*/,float,float,float,float,float,float,float,float,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_stroke_rect(long parg0, float[] parg1, float prounding, float pline_thickness, int[] parg4); //struct nk_command_buffer*/*ptr*/,struct nk_rect/*none_ptr*/,float,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_stroke_circle(long parg0, float[] parg1, float pline_thickness, int[] parg3); //struct nk_command_buffer*/*ptr*/,struct nk_rect/*none_ptr*/,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_stroke_arc(long parg0, float pcx, float pcy, float pradius, float pa_min, float pa_max, float pline_thickness, int[] parg7); //struct nk_command_buffer*/*ptr*/,float,float,float,float,float,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_stroke_triangle(long parg0, float parg1, float parg2, float parg3, float parg4, float parg5, float parg6, float pline_thichness, int[] parg8); //struct nk_command_buffer*/*ptr*/,float,float,float,float,float,float,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_stroke_polyline(long parg0, float[] ppoints, int ppoint_count, float pline_thickness, int[] pcol); //struct nk_command_buffer*/*ptr*/,float*,int,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_stroke_polygon(long parg0, float[] parg1, int ppoint_count, float pline_thickness, int[] parg4); //struct nk_command_buffer*/*ptr*/,float*,int,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_fill_rect(long parg0, float[] parg1, float prounding, int[] parg3); //struct nk_command_buffer*/*ptr*/,struct nk_rect/*none_ptr*/,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_fill_rect_multi_color(long parg0, float[] parg1, int[] pleft, int[] ptop, int[] pright, int[] pbottom); //struct nk_command_buffer*/*ptr*/,struct nk_rect/*none_ptr*/,struct nk_color/*none_ptr*/,struct nk_color/*none_ptr*/,struct nk_color/*none_ptr*/,struct nk_color/*none_ptr*/, //void
    public static native void nk_fill_circle(long parg0, float[] parg1, int[] parg2); //struct nk_command_buffer*/*ptr*/,struct nk_rect/*none_ptr*/,struct nk_color/*none_ptr*/, //void
    public static native void nk_fill_arc(long parg0, float pcx, float pcy, float pradius, float pa_min, float pa_max, int[] parg6); //struct nk_command_buffer*/*ptr*/,float,float,float,float,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_fill_triangle(long parg0, float px0, float py0, float px1, float py1, float px2, float py2, int[] parg7); //struct nk_command_buffer*/*ptr*/,float,float,float,float,float,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_fill_polygon(long parg0, float[] parg1, int ppoint_count, int[] parg3); //struct nk_command_buffer*/*ptr*/,float*,int,struct nk_color/*none_ptr*/, //void
    public static native void nk_draw_image(long parg0, float[] parg1, long parg2, int[] parg3); //struct nk_command_buffer*/*ptr*/,struct nk_rect/*none_ptr*/,const struct nk_image*/*ptr*/,struct nk_color/*none_ptr*/, //void
    public static native void nk_draw_text(long parg0, float[] parg1, byte[] ptext, int plen, long parg4, int[] parg5, int[] parg6); //struct nk_command_buffer*/*ptr*/,struct nk_rect/*none_ptr*/,const char*,int,const struct nk_user_font*/*ptr*/,struct nk_color/*none_ptr*/,struct nk_color/*none_ptr*/, //void
    public static native void nk_push_scissor(long parg0, float[] parg1); //struct nk_command_buffer*/*ptr*/,struct nk_rect/*none_ptr*/, //void
    public static native void nk_push_custom(long parg0, float[] parg1, long parg2, byte[] pusr); //struct nk_command_buffer*/*ptr*/,struct nk_rect/*none_ptr*/,nk_command_custom_callback/*ptr*/,nk_handle/*none_ptr*/, //void
    public static native int nk_input_has_mouse_click(long parg0, int parg1); //const struct nk_input*/*ptr*/,enum nk_buttons, //int
    public static native int nk_input_has_mouse_click_in_rect(long parg0, int parg1, float[] parg2); //const struct nk_input*/*ptr*/,enum nk_buttons,struct nk_rect/*none_ptr*/, //int
    public static native int nk_input_has_mouse_click_down_in_rect(long parg0, int parg1, float[] parg2, int pdown); //const struct nk_input*/*ptr*/,enum nk_buttons,struct nk_rect/*none_ptr*/,int, //int
    public static native int nk_input_is_mouse_click_in_rect(long parg0, int parg1, float[] parg2); //const struct nk_input*/*ptr*/,enum nk_buttons,struct nk_rect/*none_ptr*/, //int
    public static native int nk_input_is_mouse_click_down_in_rect(long pi, int pid, float[] pb, int pdown); //const struct nk_input*/*ptr*/,enum nk_buttons,struct nk_rect/*none_ptr*/,int, //int
    public static native int nk_input_any_mouse_click_in_rect(long parg0, float[] parg1); //const struct nk_input*/*ptr*/,struct nk_rect/*none_ptr*/, //int
    public static native int nk_input_is_mouse_prev_hovering_rect(long parg0, float[] parg1); //const struct nk_input*/*ptr*/,struct nk_rect/*none_ptr*/, //int
    public static native int nk_input_is_mouse_hovering_rect(long parg0, float[] parg1); //const struct nk_input*/*ptr*/,struct nk_rect/*none_ptr*/, //int
    public static native int nk_input_mouse_clicked(long parg0, int parg1, float[] parg2); //const struct nk_input*/*ptr*/,enum nk_buttons,struct nk_rect/*none_ptr*/, //int
    public static native int nk_input_is_mouse_down(long parg0, int parg1); //const struct nk_input*/*ptr*/,enum nk_buttons, //int
    public static native int nk_input_is_mouse_pressed(long parg0, int parg1); //const struct nk_input*/*ptr*/,enum nk_buttons, //int
    public static native int nk_input_is_mouse_released(long parg0, int parg1); //const struct nk_input*/*ptr*/,enum nk_buttons, //int
    public static native int nk_input_is_key_pressed(long parg0, int parg1); //const struct nk_input*/*ptr*/,enum nk_keys, //int
    public static native int nk_input_is_key_released(long parg0, int parg1); //const struct nk_input*/*ptr*/,enum nk_keys, //int
    public static native int nk_input_is_key_down(long parg0, int parg1); //const struct nk_input*/*ptr*/,enum nk_keys, //int
    public static native void nk_draw_list_init(long parg0); //struct nk_draw_list*/*ptr*/, //void
    public static native void nk_draw_list_setup(long parg0, long parg1, long pcmds, long pvertices, long pelements, int pline_aa, int pshape_aa); //struct nk_draw_list*/*ptr*/,const struct nk_convert_config*/*ptr*/,struct nk_buffer*/*ptr*/,struct nk_buffer*/*ptr*/,struct nk_buffer*/*ptr*/,enum nk_anti_aliasing,enum nk_anti_aliasing, //void
    public static native void nk_draw_list_clear(long parg0); //struct nk_draw_list*/*ptr*/, //void
    public static native long  nk__draw_list_begin(long parg0, long parg1); //const struct nk_draw_list*/*ptr*/,const struct nk_buffer*/*ptr*/, //const struct nk_draw_command*/*ptr*/ 
    public static native long  nk__draw_list_next(long parg0, long parg1, long parg2); //const struct nk_draw_command*/*ptr*/,const struct nk_buffer*/*ptr*/,const struct nk_draw_list*/*ptr*/, //const struct nk_draw_command*/*ptr*/ 
    public static native long  nk__draw_list_end(long parg0, long parg1); //const struct nk_draw_list*/*ptr*/,const struct nk_buffer*/*ptr*/, //const struct nk_draw_command*/*ptr*/ 
    public static native void nk_draw_list_path_clear(long parg0); //struct nk_draw_list*/*ptr*/, //void
    public static native void nk_draw_list_path_line_to(long parg0, float[] ppos); //struct nk_draw_list*/*ptr*/,struct nk_vec2/*none_ptr*/, //void
    public static native void nk_draw_list_path_arc_to_fast(long parg0, float[] pcenter, float pradius, int pa_min, int pa_max); //struct nk_draw_list*/*ptr*/,struct nk_vec2/*none_ptr*/,float,int,int, //void
    public static native void nk_draw_list_path_arc_to(long parg0, float[] pcenter, float pradius, float pa_min, float pa_max, int psegments); //struct nk_draw_list*/*ptr*/,struct nk_vec2/*none_ptr*/,float,float,float,unsigned int, //void
    public static native void nk_draw_list_path_rect_to(long parg0, float[] pa, float[] pb, float prounding); //struct nk_draw_list*/*ptr*/,struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/,float, //void
    public static native void nk_draw_list_path_curve_to(long parg0, float[] pp2, float[] pp3, float[] pp4, int pnum_segments); //struct nk_draw_list*/*ptr*/,struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/,unsigned int, //void
    public static native void nk_draw_list_path_fill(long parg0, int[] parg1); //struct nk_draw_list*/*ptr*/,struct nk_color/*none_ptr*/, //void
    public static native void nk_draw_list_path_stroke(long parg0, int[] parg1, int pclosed, float pthickness); //struct nk_draw_list*/*ptr*/,struct nk_color/*none_ptr*/,enum nk_draw_list_stroke,float, //void
    public static native void nk_draw_list_stroke_line(long parg0, float[] pa, float[] pb, int[] parg3, float pthickness); //struct nk_draw_list*/*ptr*/,struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/,struct nk_color/*none_ptr*/,float, //void
    public static native void nk_draw_list_stroke_rect(long parg0, float[] prect, int[] parg2, float prounding, float pthickness); //struct nk_draw_list*/*ptr*/,struct nk_rect/*none_ptr*/,struct nk_color/*none_ptr*/,float,float, //void
    public static native void nk_draw_list_stroke_triangle(long parg0, float[] pa, float[] pb, float[] pc, int[] parg4, float pthickness); //struct nk_draw_list*/*ptr*/,struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/,struct nk_color/*none_ptr*/,float, //void
    public static native void nk_draw_list_stroke_circle(long parg0, float[] pcenter, float pradius, int[] parg3, int psegs, float pthickness); //struct nk_draw_list*/*ptr*/,struct nk_vec2/*none_ptr*/,float,struct nk_color/*none_ptr*/,unsigned int,float, //void
    public static native void nk_draw_list_stroke_curve(long parg0, float[] pp0, float[] pcp0, float[] pcp1, float[] pp1, int[] parg5, int psegments, float pthickness); //struct nk_draw_list*/*ptr*/,struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/,struct nk_color/*none_ptr*/,unsigned int,float, //void
    public static native void nk_draw_list_stroke_poly_line(long parg0, float[] ppnts, int pcnt, int[] parg3, int parg4, float pthickness, int parg6); //struct nk_draw_list*/*ptr*/,const struct nk_vec2*,const unsigned int,struct nk_color/*none_ptr*/,enum nk_draw_list_stroke,float,enum nk_anti_aliasing, //void
    public static native void nk_draw_list_fill_rect(long parg0, float[] prect, int[] parg2, float prounding); //struct nk_draw_list*/*ptr*/,struct nk_rect/*none_ptr*/,struct nk_color/*none_ptr*/,float, //void
    public static native void nk_draw_list_fill_rect_multi_color(long parg0, float[] prect, int[] pleft, int[] ptop, int[] pright, int[] pbottom); //struct nk_draw_list*/*ptr*/,struct nk_rect/*none_ptr*/,struct nk_color/*none_ptr*/,struct nk_color/*none_ptr*/,struct nk_color/*none_ptr*/,struct nk_color/*none_ptr*/, //void
    public static native void nk_draw_list_fill_triangle(long parg0, float[] pa, float[] pb, float[] pc, int[] parg4); //struct nk_draw_list*/*ptr*/,struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/,struct nk_vec2/*none_ptr*/,struct nk_color/*none_ptr*/, //void
    public static native void nk_draw_list_fill_circle(long parg0, float[] pcenter, float pradius, int[] pcol, int psegs); //struct nk_draw_list*/*ptr*/,struct nk_vec2/*none_ptr*/,float,struct nk_color/*none_ptr*/,unsigned int, //void
    public static native void nk_draw_list_fill_poly_convex(long parg0, float[] ppoints, int pcount, int[] parg3, int parg4); //struct nk_draw_list*/*ptr*/,const struct nk_vec2*,const unsigned int,struct nk_color/*none_ptr*/,enum nk_anti_aliasing, //void
    public static native void nk_draw_list_add_image(long parg0, byte[] ptexture, float[] prect, int[] parg3); //struct nk_draw_list*/*ptr*/,struct nk_image/*none_ptr*/,struct nk_rect/*none_ptr*/,struct nk_color/*none_ptr*/, //void
    public static native void nk_draw_list_add_text(long parg0, long parg1, float[] parg2, byte[] ptext, int plen, float pfont_height, int[] parg6); //struct nk_draw_list*/*ptr*/,const struct nk_user_font*/*ptr*/,struct nk_rect/*none_ptr*/,const char*,int,float,struct nk_color/*none_ptr*/, //void
    public static native void nk_draw_list_push_userdata(long parg0, byte[] puserdata); //struct nk_draw_list*/*ptr*/,nk_handle/*none_ptr*/, //void
    public static native byte[]  nk_style_item_image(byte[] pimg); //struct nk_image/*none_ptr*/, //struct nk_style_item/*none_ptr*/ 
    public static native byte[]  nk_style_item_color(int[] parg0); //struct nk_color/*none_ptr*/, //struct nk_style_item/*none_ptr*/ 
    public static native byte[]  nk_style_item_hide(); //void, //struct nk_style_item/*none_ptr*/ 
    public static native long  nk_glfw3_init(long pwin, int parg1); //GLFWwindow*/*ptr*/,enum nk_glfw_init_state, //struct nk_context*/*ptr*/ 
    public static native void nk_glfw3_font_stash_begin(long[] patlas); //struct nk_font_atlas**/*ptr*/, //void
    public static native void nk_glfw3_font_stash_end(); //void, //void
    public static native void nk_glfw3_new_frame(); //void, //void
    public static native void nk_glfw3_render(int parg0); //enum nk_anti_aliasing, //void
    public static native void nk_glfw3_shutdown(); //void, //void
    public static native void nk_glfw3_char_callback(long pwin, int pcodepoint); //GLFWwindow*/*ptr*/,unsigned int, //void
    public static native void nk_gflw3_scroll_callback(long pwin, double pxoff, double pyoff); //GLFWwindow*/*ptr*/,double,double, //void
    public static native long  nk_get_font_handle(long pfont); //struct nk_font*/*ptr*/, //struct nk_user_font*/*ptr*/ 
    public static native byte[]  nk_create_font_atlas(); // //struct nk_font_atlas/*none_ptr*/ 
    public static native void nk_set_font_cfg_range(long pfont_cfg, long prange); //struct nk_font_config*/*ptr*/,nk_rune*/*ptr*/, //void
    public static native long  nk_load_font(byte[] pfile_path, float pheight); //const char*,float, //struct nk_font*/*ptr*/ 

}

