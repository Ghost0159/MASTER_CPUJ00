===========================================================================
Stationery for Nitro SDK 3.0
Japanese description is available at the second half of this file.
===========================================================================

This stationery uses precompiled headers.

1. The files and settings relating to the use of the precompiled header.

 - Nitro_TS.pch
    This is the pch file to generate a mch file which is used by the C source.
    The actual mch file name depends on the build target you chose.
    "nitro_TS_prefix.h" and "nitro.h" are included in the precompiled header.
    If you want to include other header files in the precompiled header, please
    add the files to the bottom of this pch file.

 - Nitro_TS.pch++
    This is the pch file to generate a mch file which is used by the C++ source.
    The actual mch file name depends on the build target you chose.
    "nitro_TS_prefix.h" and "nitro.h" are included in the precompiled header.
    If you want to include other header files in the precompiled header, please
    add the files to the bottom of this pch file.

 - Nitro_TS.h
    This is the header file for each C and C++ source file to include
    the proper precompiled header.
    This header file is included from the Prefix Text field in the C/C++
    Preprocessor panel thus making it automatically included in each
    C/C++ source file.

 - The settings in the C/C++ Preprocessor panel
   The settings in the C/C++ Preprocessor panel ("Target Settings"
     -> "Language Settings" -> "C/C++ Preprocessor") is the following.
    1) "Nitro_TS.h" is included by the Prefix Text.
    2) "nitro_TS_prefix.h" is not included by the Prefix Text.
    3) "Use prefix text in precompiled header" is checked.


2. The behavior when building is as follows.

  1) Nitro_TS.pch (or Nitro_TS.pch++) is precompiled.
     The mch file with the proper file name is generated.
  2) Each C source and each C++ source is compiled.
     Nitro_TS.h is included by the Prefix Text in the C/C++
     Preprocessor panel.
     The proper mch file is included.


3. If you don't want to use precompiled headers, please follow the steps below.

  1) Remove Nitro_TS.pch from Project.
  2) Remove Nitro_TS.pch++ from Project.
  3) Remove Nitro_TS.h from Project.
  4) Change the contents of Prefix Text field on C/C++ Preprocessor panel.
     - Remove the line " #include "Nitro_TS.h" ".
     - Remove the comment out of the line "#include <nitro_TS_prefix.h>" to
       make this #include effective.


4. You may be able to remove Nitro_TS.pch or Nitro_TS.pch++.

  Both of Nitro_TS.pch and Nitro_TS.pch++ are used in both the stationery
  for C and C++ Project because both C and C++ source may be used 
  in one Project.

   - If the precompiled header for C++ is unnecessary in your C Project,
     you can remove the Nitro_TS.pch++ from your Project.
   - If the precompiled header for C is unnecessary in your C++ Project,
     you can remove the Nitro_TS.pch from your Project.


/*--- The lines below are written in Japanese character set ---*/
===========================================================================
Nitro SDK 3.0 用ステーショナリ
プリコンパイルヘッダの使用について
===========================================================================

このステーショナリではプリコンパイルヘッダを使用しています。

１．プリコンパイルヘッダの使用に関連した設定、ファイル

 ・ Nitro_TS.pch
    Cソースが使うmchファイルを生成する為のpchファイルです。
    生成されるmchファイルのファイル名はビルドターゲットによって異なります。
    プリコンパイルヘッダには nitro_TS_prefix.h と nitro.h を含めています。
    ユーザーが用意したヘッダファイルも含める場合は、このファイルの末尾に
    追記して下さい。

  ・ Nitro_TS.pch++
    C++ソースが使うmchファイルを生成する為のpchファイルです。
    生成されるmchファイルのファイル名はビルドターゲットによって異なります。
    プリコンパイルヘッダには nitro_TS_prefix.h と nitro.h を含めています。
    ユーザーが用意したヘッダファイルも含める場合は、このファイルの末尾に
    追記して下さい。

  ・ Nitro_TS.h
    各Cソース又はC++ソースが適切なプリコンパイルヘッダをインクルードする
    為のヘッダファイルです。
    各Cソース又はC++ソースのコンパイル時に必ずこのヘッダファイルが使用
    されるように、ターゲット設定パネル － 言語設定 － C/C++ Preprocessor の
    Prefix Text でインクルードされています。

 ・ C/C++ Preprocessor パネル内の設定
    ターゲット設定パネル － 言語設定 － C/C++ Preprocessor において次の設定
    がされています。
    1) Nitro_TS.h を Prefix Text でインクルードする
    2) nitro_TS_prefix.h は Prefix Text ではインクルードしない
    3) Use prefix text in precompiled header チェックボックスがオン


２．ビルド時の動きは下記のようになります。

  1) Nitro_TS.pch （またはNitro_TS.pch++）がプリコンパイルされ、ビルド
     ターゲットに応じた適切なファイル名のmchファイルが生成される。
  2) 各Cソース又はC++ソースがコンパイルされる。
     この時、C/C++ Preprocessorパネルの Prefix Textの指定により、Nitro_TS.h
     が暗黙的にインクルードされ、適切なファイル名のmchファイルがインクルード
     される。


３．プリコンパイルヘッダを使用しない構成に変更するには、下記の手順を行って
    下さい。

  1) プロジェクトから Nitro_TS.pch を削除する
  2) プロジェクトから Nitro_TS.pch++ を削除する
  3) プロジェクトから Nitro_TS.h を削除する
  4) ターゲット設定パネル － 言語設定 － C/C++ Preprocessor の Prefix Text の
     内容について、下記の変更を行う
     ・「#include "Nitro_TS.h"」の行を削除する。
     ・「#include <nitro_TS_prefix.h>」の行のコメントアウトを解除し、
         このインクルードを有効にする。


４．不要な Nitro_TS.pch 又は Nitro_TS.pch++ について。

  Cのプロジェクト用のステーショナリ及びC++プロジェクト用のステーショナリ
  の両方において、Cソース及びC++ソースが混在可能とする為に、Nitro_TS.pch と
  Nitro_TS.pch++ の両方が組み込まれています。

  もし、CのプロジェクトでC++用のプリコンパイルヘッダが全く不要であるな
  らば、プロジェクトから Nitro_TS.pch++ を削除して頂いてかまいません。
  同様に、C++のプロジェクトでC用のプリコンパイルヘッダが全く不要である
  ならば、プロジェクトから Nitro_TS.pch を削除して頂いてかまいません。


