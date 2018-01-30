//
// Created by gust on 2018/1/22.
//

#ifndef JNI_GUI_NUKLEAR_JNI_ASSIST_H
#define JNI_GUI_NUKLEAR_JNI_ASSIST_H

NK_API struct nk_user_font *nk_get_font_handle(struct nk_font *font);

NK_API struct nk_font_atlas nk_create_font_atlas();

NK_API void nk_set_font_cfg_range(struct nk_font_config *font_cfg, nk_rune *range);

NK_API struct nk_font *nk_load_font_file(const char *file_path, float height);

NK_API struct nk_font *nk_load_font_memory(void *bytes, int size, float height);

//stb_truetype dont delete it .for generate jni
/*
NK_API int stbtt_InitFont(stbtt_fontinfo *info, const unsigned char *data2, int fontstart);

NK_API float stbtt_ScaleForPixelHeight(const stbtt_fontinfo *info, float pixels);

NK_API void stbtt_GetFontVMetrics(const stbtt_fontinfo *info, int *ascent, int *descent, int *lineGap);

NK_API void stbtt_GetCodepointBitmapBox(const stbtt_fontinfo *font, int codepoint, float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1);

NK_API void stbtt_MakeCodepointBitmapOffset(const stbtt_fontinfo *info, unsigned char *output, int output_offset, int out_w, int out_h, int out_stride, float scale_x, float scale_y, int codepoint);

NK_API void stbtt_GetCodepointHMetrics(const stbtt_fontinfo *info, int codepoint, int *advanceWidth, int *leftSideBearing);

NK_API int  stbtt_GetCodepointKernAdvance(const stbtt_fontinfo *info, int ch1, int ch2);

NK_API struct stbtt_fontinfo stbtt_MakeFontInfo();

 NK_API int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);

 NK_API int stbi_write_bmp(char const *filename, int w, int h, int comp, const void *data);

 NK_API int stbi_write_tga(char const *filename, int w, int h, int comp, const void *data);
 */
//implementation

NK_API struct nk_user_font *nk_get_font_handle(struct nk_font *font) {
    return &font->handle;
}

NK_API struct nk_font_atlas nk_create_font_atlas() {
    struct nk_font_atlas val;
    return val;
}

NK_API void nk_set_font_cfg_range(struct nk_font_config *font_cfg, nk_rune *range) {
    font_cfg->range = range;
}

NK_STORAGE const nk_rune full_ranges[] = {
        0x0020, 0xFFFF,
        0
};

NK_API struct nk_font *nk_load_font_file(const char *file_path, float height) {
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);

    struct nk_font_config conf = nk_font_config(0);
    conf.range = full_ranges;//nk_font_chinese_glyph_ranges();//&nk_font_japanese_glyph_ranges[0];
    struct nk_font *font = nk_font_atlas_add_from_file(atlas, file_path, height, &conf);
    nk_glfw3_font_stash_end();
    return font;
}

NK_API struct nk_font *nk_load_font_memory(void *bytes, int size, float height) {
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);

    struct nk_font_config conf = nk_font_config(0);
    conf.range = full_ranges;//nk_font_chinese_glyph_ranges();//&nk_font_japanese_glyph_ranges[0];
    struct nk_font *font = nk_font_atlas_add_from_memory(atlas, bytes, size, height, &conf);
    nk_glfw3_font_stash_end();
    return font;
}

NK_API struct stbtt_fontinfo stbtt_MakeFontInfo() {
    stbtt_fontinfo info;
    memset(&info, 0, sizeof(stbtt_fontinfo));
    return info;
}

NK_API void
stbtt_MakeCodepointBitmapOffset(const stbtt_fontinfo *info, unsigned char *output, int output_offset, int out_w,
                                int out_h, int out_stride, float scale_x, float scale_y, int codepoint) {
    stbtt_MakeCodepointBitmap(info, output + output_offset, out_w, out_h, out_stride, scale_x, scale_y,
                                    codepoint);
}

#endif //JNI_GUI_NUKLEAR_JNI_ASSIST_H
