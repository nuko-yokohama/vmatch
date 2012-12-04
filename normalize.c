#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "normalize.h"
/**
 * normalize_text
 * 以下の規則で正規化を行う。
 * - 小文字化
 * - 2個以上連続した標準空白類文字を1文字の空白(0x20)に置き換える。
 * - テキスト両端の空白類文字を切り詰める(trim)
 * @param 正規化対象となる文字列。\0でターミネートされていること。
 * @return 正規化文字列。この文字列は内部で確保される。
 * TODO: 
 * - ワイドキャラ対応。
 * - 制御文字対応(空白文字と同じ扱いにすべきか)。
 * - 正規化文字列バッファ確保の効率化
 * - 処理最適化
 */

enum n_status { INIT, BLANK, OTHER };

char*
normalize_text(char* in) {
    char* out;
    char* current;
    enum n_status status = INIT;

    if ((out = malloc(strlen(in))) == NULL) {
        return NULL;
    }
    current = out;

    while ( *in != 0x00 ) {
        switch (status) {
            case INIT:
                if (isspace( *in )) {
                    /* nop */
                } else {
                    *current= tolower(*in);
                    current++;
                    status = OTHER;
                }
                break;
            case BLANK:
                if (isspace( *in )) {
                    status = BLANK;
                } else {
                    *current = tolower(*in);
                    current++;
                    status = OTHER;
                }
                break;
            case OTHER:
                if (isspace( *in )) {
                    *current= 0x20;
                    current++;
                    status = BLANK;
                } else {
                    *current = tolower(*in);
                    current++;
                    status = OTHER;
                }
                break;
            default:
                /* bug edge */
                ;
                
        }
        in ++;
    }
    /* insert 0x00 terminate */
    if (status == BLANK) {
      *(current - 1) = 0x00;
    } else {
      *current = 0x00;
    }

    return out;
}

#ifdef UNIT_TEST
int
main(int argc, char** argv) {
    int i;
    char* out;

    for (i=1; i < argc; i++) {
        printf("orignal text  =[%s]\n", argv[i]);
        out = normalize_text(argv[i]);
        printf("normalize text=[%s]\n", out);
        free(out);
    }
}
#endif /* UNIT_TEST */
