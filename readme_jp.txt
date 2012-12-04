vmatch EXTENSION

【概要】
このEXTENSIONでは、文字列間の類似度を算出する関数と、
算出結果を元に真偽を評価する /= 演算子を提供します。
contrib/fuzzystrmatch に似ていますが、
qwerty配列に起因するtypoの重み付けなど
ちょっとだけ独自の規則で類似度を算出しています。
(本当に実用的なのかは検証していませんが・・・)

【インストール】
make USE_PGXS=1
make USE_PGXS=1 install

【使用例】
vmatch=# CREATE EXTENSION vmatch ;
CREATE EXTENSION
vmatch=# \dx
                 List of installed extensions
  Name   | Version |   Schema   |         Description          
---------+---------+------------+------------------------------
 plpgsql | 1.0     | pg_catalog | PL/pgSQL procedural language
 vmatch  | 1.0     | public     | vague equal expression.
(2 rows)

vmatch=# \df
                              List of functions
 Schema |       Name       | Result data type | Argument data types |  Type  
--------+------------------+------------------+---------------------+--------
 public | get_similar_rate | real             | text, text          | normal
 public | vmatch           | boolean          | text, text          | normal

vmatch=# SELECT get_similar_rate(data, 'postgresql'), data FROM test WHERE data /= 'postgresql';
 get_similar_rate |    data     
------------------+-------------
                1 | PostgreSQL
                1 | postgresql
              0.8 | POSTGRES
         0.840909 | PostgresSQL
(4 rows)

vmatch=# SELECT get_similar_rate(data, 'postgres'), data FROM test WHERE data /= 'postgres';
 get_similar_rate |    data    
------------------+------------
              0.8 | PostgreSQL
              0.8 | postgresql
                1 | POSTGRES
(3 rows)

vmatch=# SELECT get_similar_rate(data, 'pastgres'), data FROM test WHERE data /= 'pastgres';
 get_similar_rate |   data   
------------------+----------
            0.875 | POSTGRES
(1 row)

vmatch=# SELECT get_similar_rate(data, 'pstgres'), data FROM test WHERE data /= 'pstgres';
 get_similar_rate |   data   
------------------+----------
          0.78125 | POSTGRES
(1 row)

vmatch=# SELECT get_similar_rate(data, 'postgresql'), data FROM test WHERE data /= 'postgresql';
 get_similar_rate |    data     
------------------+-------------
                1 | PostgreSQL
                1 | postgresql
              0.8 | POSTGRES
         0.840909 | PostgresSQL
(4 rows)

vmatch=# SELECT get_similar_rate(data, 'postgres'), data FROM test WHERE data /= 'postgres';
 get_similar_rate |    data    
------------------+------------
              0.8 | PostgreSQL
              0.8 | postgresql
                1 | POSTGRES
(3 rows)

vmatch=# SELECT get_similar_rate(data, 'pastgres'), data FROM test WHERE data /= 'pastgres';
 get_similar_rate |   data   
------------------+----------
            0.875 | POSTGRES
(1 row)

vmatch=# SELECT get_similar_rate(data, 'pstgres'), data FROM test WHERE data /= 'pstgres';
 get_similar_rate |   data   
------------------+----------
          0.78125 | POSTGRES
(1 row)

【TODO】
・日本語への対応
・類似度計算式の改善
・判定閾値のGUCパラメータ化(現状は0.75固定)

【作者】
ぬこ＠横浜(@nuko_yokohama)
