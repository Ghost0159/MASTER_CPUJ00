■ setpath.bat について

環境変数 NITRO*_ROOT を lib 下のライブラリツリーに設定します。


■ cygwin.bat について

setpath.bat で環境変数を設定したあと cygwin の bash を起動します。
そのまま main ディレクトリに移動し make を走らせれば srl を作成できます。
ただし、事前に以下のディレクトリやコマンドにパスが通っている必要があります。

  ・cygwin/bin
  ・ruby (cygwin/bin にあれば不要)
  ・perl (cygwin/bin にあれば不要)

また、 CodeWarrior は zip ディレクトリ内のパッケージからインストールしてください。