#include "bytebuf.h"
#include "miniz.c"


s32 zip_loadfile(char *jarpath, char *filename, ByteBuf *buf) {
    int file_index = 0;
    mz_zip_archive zipArchive = {0};
    mz_zip_archive_file_stat file_stat = {0};

    int ret = 0;
    if (mz_zip_reader_init_file(&zipArchive, jarpath, 0) == MZ_FALSE) {//
        ret = -1;
    } else {

        file_index = mz_zip_reader_locate_file(&zipArchive, filename, NULL, 0);//
        if (!mz_zip_reader_file_stat(&zipArchive, file_index, &file_stat)) {
            ret = -1;
        } else {
            size_t uncompressed_size = (size_t) file_stat.m_uncomp_size;
            void *p = mz_zip_reader_extract_file_to_heap(&zipArchive, file_stat.m_filename, &uncompressed_size, 0);
            if (!p) {
                ret = -1;
            } else {

                bytebuf_write_batch(buf, p, (s32) uncompressed_size);
                mz_free(p);
            }
        }
    }
    mz_zip_reader_end(&zipArchive);
    return ret;
}


s32 zip_savefile_mem(char *jarpath, char *filename, char *buf, int size) {
    int file_index = 0;
    mz_zip_archive zipArchive = {0};
    mz_zip_archive_file_stat file_stat = {0};

    int ret = 0;
    if (mz_zip_reader_init_file(&zipArchive, jarpath, 0) == MZ_FALSE) {//
        if (mz_zip_writer_init_file(&zipArchive, jarpath, 0) == MZ_FALSE) {//
            ret = -1;
        }
    } else {
        if (mz_zip_writer_init_from_reader(&zipArchive, jarpath) == MZ_FALSE) {//
            ret = -1;
        }
    }
    if (ret == 0) {
        if (mz_zip_writer_add_mem(&zipArchive, filename, buf, size, 0) == MZ_FALSE) {//
            ret = -1;
        }
        if (mz_zip_writer_finalize_archive(&zipArchive) == MZ_FALSE) {//
            ret = -1;
        }
    }
    mz_zip_writer_end(&zipArchive);
    return ret;
}


s32 zip_savefile(char *jarpath, char *filename, ByteBuf *buf) {
    return zip_savefile_mem(jarpath, filename, buf->buf, buf->wp);
}