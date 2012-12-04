#include <stdio.h>
#include <string.h>

#include "postgres.h"
#include "fmgr.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#include "utils/builtins.h"

#include "vmatch.h"
#include "normalize.h"
#include "similar.h"

#define VMATCH_LIMIT 0.75

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(get_similar_rate);
Datum get_similar_rate(PG_FUNCTION_ARGS);

Datum get_similar_rate(PG_FUNCTION_ARGS) {
    text* txt1 = PG_GETARG_TEXT_P(0);
    text* txt2 = PG_GETARG_TEXT_P(1);
    char* str1 = text_to_cstring(txt1);
    char* str2 = text_to_cstring(txt2);

    char* n_str1; /* 正規化された文字列 */
    char* n_str2; /* 正規化された文字列 */
    float4 rate;
    float4* result;

elog(DEBUG1, "str_1 = %s", str1);
elog(DEBUG1, "str_2 = %s", str2);
    n_str1 = normalize_text(str1);
    n_str2 = normalize_text(str2);
elog(DEBUG1, "n_str_1 = %s", n_str1);
elog(DEBUG1, "n_str_2 = %s", n_str2);
    rate = similar_rate(n_str1, n_str2);
elog(DEBUG1, "rate = %f", rate);

    result = palloc(sizeof(float));
    *result = rate;

    PG_RETURN_FLOAT4( rate);
}

PG_FUNCTION_INFO_V1(vmatch);
Datum vmatch(PG_FUNCTION_ARGS);

Datum vmatch(PG_FUNCTION_ARGS) {
    text* txt1 = PG_GETARG_TEXT_P(0);
    text* txt2 = PG_GETARG_TEXT_P(1);
    char* str1 = text_to_cstring(txt1);
    char* str2 = text_to_cstring(txt2);

    char* n_str1; /* 正規化された文字列 */
    char* n_str2; /* 正規化された文字列 */
    float4 rate;

elog(DEBUG1, "str_1 = %s", str1);
elog(DEBUG1, "str_2 = %s", str2);
    n_str1 = normalize_text(str1);
    n_str2 = normalize_text(str2);
elog(DEBUG1, "n_str_1 = %s", n_str1);
elog(DEBUG1, "n_str_2 = %s", n_str2);
    rate = similar_rate(n_str1, n_str2);
elog(DEBUG1, "rate = %f", rate);

    if (rate >= VMATCH_LIMIT) {
        PG_RETURN_BOOL(true);
    } else {
        PG_RETURN_BOOL(false);
    }
}
