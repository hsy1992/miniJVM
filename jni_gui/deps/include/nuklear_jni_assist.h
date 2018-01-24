//
// Created by gust on 2018/1/22.
//

#ifndef JNI_GUI_NUKLEAR_JNI_ASSIST_H
#define JNI_GUI_NUKLEAR_JNI_ASSIST_H

NK_API struct nk_user_font *nk_get_font_handle(struct nk_font *font);

NK_API struct nk_font_atlas nk_create_font_atlas();

NK_API void nk_set_font_cfg_range(struct nk_font_config *font_cfg, nk_rune *range);

NK_API struct nk_font *nk_load_font(const char *file_path, float height);
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

NK_API struct nk_font *nk_load_font(const char *file_path, float height) {
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);

    struct nk_font_config conf = nk_font_config(0);
    conf.range = full_ranges;//nk_font_chinese_glyph_ranges();//&nk_font_japanese_glyph_ranges[0];
    struct nk_font *font = nk_font_atlas_add_from_file(atlas, file_path, height, &conf);
    nk_glfw3_font_stash_end();
    return font;
}

#endif //JNI_GUI_NUKLEAR_JNI_ASSIST_H
