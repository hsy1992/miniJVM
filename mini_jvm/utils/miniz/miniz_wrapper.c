#include "../bytebuf.h"
#include "miniz.c"


s32 zip_loadfile(char *jarpath, char *filename, ByteBuf *buf) {
    int file_index = 0;
    mz_zip_archive zipArchive = {0};
    mz_zip_archive_file_stat file_stat = {0};

    if (mz_zip_reader_init_file(&zipArchive, jarpath, 0) == MZ_FALSE) {//"../../javalib/dist/mini_jvm_java.jar"
        return -1;
    }

    file_index = mz_zip_reader_locate_file(&zipArchive, filename, NULL, 0);//"java/lang/Object.class"
    if (!mz_zip_reader_file_stat(&zipArchive, file_index, &file_stat)) {
        return -1;
    }
    size_t uncompressed_size = (size_t) file_stat.m_uncomp_size;
    void *p = mz_zip_reader_extract_file_to_heap(&zipArchive, file_stat.m_filename, &uncompressed_size, 0);
    if (!p) {
        return -1;
    }

    bytebuf_write_batch(buf, p, uncompressed_size);
    mz_free(p);
    mz_zip_reader_end(&zipArchive);
    return 0;
}

