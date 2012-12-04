/**
 * similar.c
 *
 * 文字列の類似度を取得する
 * この版では1文字までのtypoを許容する
 *
 * str1はDB内などの「比較される」文字列
 * str2は「比較したい文字列」を想定
 * str1, str2ともに「正規化」済みの文字列を想定する。
 * 「正規化」については normalize.c を参照。
 *
 * return値は0～1.0までの値域とする。
 * 0は全く一致しない。
 * 1.0は完全一致
 *
 * 類似度として何を考慮するか。
 * - 比較対象の文字数と一致した文字数
 * 
 * メモ
 * - typoの種別：余剰、欠損、置換により減点係数を変えるべきか。
 * - 最初と最後の文字が合っていれば中間文字順序が入れ替わっていても
 *   問題なく判断できてしまう理論を導入すべきか。
 * - qwerty配列において隣接する文字の場合、減点係数を変えるべきか？
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "similar.h"

#define TRUE 1
#define FALSE 0

int istypo(char c1, char c2) ;

float
similar_rate(char* str1, char* str2) {
    float rate;
    float matches = 0.0;
    int offset1 = 0; 
    int offset2 = 0; 
    int length1;
    int length2;
    int loop_cnt;
    int loop_limit;
    int max_length;
    int del1_flag = FALSE;
    int del2_flag = FALSE;
    
    length1 = strlen(str1);
    length2 = strlen(str2);
    max_length = (length1 > length2) ? length1 : length2 ;
    loop_limit = (length1 > length2) ? length2 : length1 ;
    
    /* byte長が短いほうを軸にループする */
    for (loop_cnt=0; loop_cnt < loop_limit; loop_cnt++) {
        /* 欠損フラグは一旦FALSEにする */
        del1_flag = FALSE;
        del2_flag = FALSE;
        if ( *(str1+offset1) == *(str2+offset2) ) {
            matches += 1.0;
            offset1++;
            offset2++;
        } else if (istypo( *(str1+offset1), *(str2+offset2))) {
            /* qwerty配列起因のtypoかもしれないので少し加算 */
            matches += 0.5;
            offset1++;
            offset2++;
        } else {
            /* 異なっている場合、相互に次のbyteと比較 */
            if ( *(str1+offset1) == *(str2+offset2+1) ) {
                /* str1が欠損の可能性 */
                matches += 0.25;
                del1_flag = TRUE;
                offset2++;
            }
            if ( *(str1+offset1 + 1 ) == *(str2+offset2) ) {
                /* str2が欠損の可能性 */
                matches += 0.25;
                del2_flag = TRUE;
                offset1++;
            }
            if ((del1_flag == FALSE) && (del2_flag == FALSE)) {
                /* 欠損ではないだろう */
                offset1++;
                offset2++;
            }
        }
    }

    rate =  matches / (float) max_length;
    return rate;
}

/**
 * istypo
 * qwerty配列的にc2がc1に隣接しているか判断する
 * かなり冗長かも・・・
 */
char typo_table[26][3] = {
  {'a', 's', 0x00 },
  {'b', 'v', 'n'},
  {'c', 'x', 'v'},
  {'d', 's', 'f'},
  {'e', 'w', 'r'},
  {'f', 'v', 'n'},
  {'g', 'd', 'g'},
  {'h', 'g', 'j'},
  {'i', 'u', 'o'},
  {'j', 'h', 'k'},
  {'k', 'j', 'l'},
  {'l', 'l', ';'},
  {'m', 'n', ','},
  {'n', 'b', 'm'},
  {'o', 'i', 'p'},
  {'p', 'o', '@'},
  {'q', 'w', 0x00},
  {'r', 'e', 't'},
  {'s', 'a', 'd'},
  {'t', 'r', 'y'},
  {'u', 'y', 'i'},
  {'v', 'c', 'b'},
  {'w', 'q', 'e'},
  {'x', 'z', 'c'},
  {'y', 't', 'u'},
  {'z', 'x', 0x00}
};

int
istypo(char c1, char c2) {
  printf("c1=%c,c2=%c\n", c1,c2);
  if (isalpha(c1)) {
    if (c2 == typo_table[c1-0x61][1] ||
        c2 == typo_table[c1-0x61][2])
        return TRUE;
  }
  return FALSE;
}

#ifdef UNIT_TEST
int
main(int argc, char** argv) {
    float rate;
    if (argc != 3) exit(0);

   printf("str1=%s\n", argv[1]);
   printf("str2=%s\n", argv[2]);
   rate = similar_rate(argv[1], argv[2]);
   printf("rate = %f\n", rate);
}
#endif
