# CLItetris

tetris in linux terminal

# key map

- h: left
- j: down
- k: hard drop
- l: right
- s: left-rotation
- d: right-rotation
- a: hold

# テトリスガイドラインについて

このテトリスは可能な部分は[テトリスガイドライン](https://www.dropbox.com/s/g55gwls0h2muqzn/tetris%20guideline%20docs%202009.zip?dl=0)に準拠して作成した。

### フィールド

大きさは横10マス縦40マス。画面に表示されるのは縦21マス分だけ。

### ミノの色

9色以上表示できる環境ならば正しく表示される、と思う。

### ネクスト

表示ネクストは五つ、七回一巡で各種類のミノが一回だけ出現する。

### ホールド

実装済み。

### ゴースト

実装済み

### key map

全く準拠していない。

### オートリピート

仕様上ここの挙動を変更することは不可能だった。準拠していない。

### Super Rotation System

実装済み。

### ハードドロップ

実装済み。

### ソフトドロップ

オートリピートと同じ理由で落下速度はガイドラインに準拠していない。
また、Extended Placement Lock Downとの兼ね合いでソフトドロップではミノの位置を確定できないようにしてある。

### Extended Placement Lock Down

実装済み。読み違いで実装が少し間違っているかもしれない。

### 対戦

未実装。

### スコア

未実装。

### Tスピン

未実装。スコアも対戦もない状況では実装しても無意味なので。

### 自動落下

現状、落下速度はコンパイル時に決定。

### ゲームオーバー

Block Outのみ実装。
