MBライブラリ マルチブート親機デモ - 操作概要 - 

プログラムを立ち上げると、親機が自動的に立ち上がり、

AID USERNAME STATE
 1  name1    NONE
 2  name2    NONE
 3  name3    NONE
 4  name4    NONE
....

といった、表が表示されます。

十字キーで、操作対象とする子機を選んで、
AボタンでSTATEに対応した操作を行います。
またスタートボタンで、接続してきた子機全てに向けて
Aボタンを押す操作と同等のことを行います。

SELECTボタンを押すと、無線をキャンセル終了し、再立ち上げします。

○各STATEに関する説明

NONE          ... 何も接続されていない状態
CONNECTED     ... 子機から接続が来た
DISCONNECTED  ... 子機が接続を切った
ENTRY REQUEST ... 子機からエントリーリクエストが来た
                  【Aボタン:エントリーを許可 Bボタン:エントリーを拒否】
KICKED        ... 子機をキックした
ENTRY OK      ... 子機からのダウンロード要求を許可した
                  【Aボタン:バイナリを送信開始】
SENDING       ... 子機へバイナリ送信中
SEND COMPLETE ... 子機へのバイナリ送信が終了した
                  【Aボタン:ブート要求送信】
BOOT REQUEST  ... 子機へブート要求の送信を開始
BOOT READY    ... ダウンロード処理が終わり、ダウンロードしたアプリを起動可
MEMBER FULL   ... 定員に達したゲームに子機が接続してきた

また、【】内に示す操作は、下に示す#define文を1にすることで
自動化することができます。

#define AUTO_ACCEPT_ENTRY	(0)
#define AUTO_SENDFILE		(0)
#define AUTO_BOOTREQ		(0)

※ 
　 このプログラムは、ASCII文字のみの表示対応となりますので、
   現在の子機バイナリmb_child.srlのユーザー名等は正しく表示されません。
   予め御了承ください。


