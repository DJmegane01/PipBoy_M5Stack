# PipBoy_M5Stack 仕様書

## 1. 概要

このプロジェクトは、M5Stack ESP32 ベースのデバイスに搭載される「Pip-Boy風」スマートウォッチアプリです。  
`src/main.cpp` を起点として、以下の機能を提供します。

- Wi-Fi 接続によるネットワーク初期化
- NTP を利用した時刻同期
- 3軸加速度センサを用いた歩数計
- 健康画面とバージョン画面の切り替え
- SD カードから画像の読み込み・表示
- スリープ機能を使った省電力動作

本アプリは M5Stack の LCD と IMU を主に使い、画面上でステータス情報を表示する構成になっています。   
主な制御入口は `setup()` と `loop()` で、各機能は個別クラスとして分離されています。

---

## 2. ファイル構成

- `src/main.cpp` : アプリの起動処理、メインループ、スリープ制御
- `src/ScreenManager.h/.cpp` : 画面切替・画面描画制御
- `src/HealthDisplay.h/.cpp` : 健康画面の描画と更新
- `src/VersionDisplay.h/.cpp` : バージョン画面の描画
- `src/StepCounter.h/.cpp` : 歩数計ロジック
- `src/TimeDisplay.h/.cpp` : 時刻表示と NTP 同期
- `src/NetworkManager.h/.cpp` : Wi-Fi 接続・切断管理
- `src/SDUtil.h/.cpp` : SD カードの初期化とファイル一覧管理
- `src/LcdUtil.h/.cpp` : LCD 中央座標計算ユーティリティ

---

## 3. 実行フロー

### 3.1 `enable_deep_sleep(int time)`

- 画面のバックライトを 0 にして表示を消す
- `esp_sleep_enable_timer_wakeup(SLEEP_MIN(time))` により、指定分後に復帰するタイマーを設定
- `esp_deep_sleep_start()` により ESP32 を深いスリープ状態へ移行する

この関数は「一定時間後に自動的に再起動する省電力モード」を実現する役割があります。  
コメントアウトされている一部のコードでは、ボタン操作でスリープへ入る処理も想定されています。

### 3.2 `setup()`

`setup()` は起動時に一度だけ実行されます。

- `M5.begin()` : M5Stack の初期化
- `M5.Power.begin()` : 電源管理の初期化
- `M5.Lcd.clear(BLACK)` : 画面の消去
- `SPIFFS.begin()` : SPIFFS の初期化
- `M5.Lcd.setBrightness(200)` : LCD の明るさ設定
- `networkManager.init()` : Wi-Fi 接続
- `screenManager.init()` : 画面管理クラス初期化

### 3.3 `loop()`

`loop()` はメインループで継続実行されます。

- `M5.update()` : M5Stack ボタン入力状態の更新
- `screenManager.update()` : 現在の画面の更新処理
- `delay(10)` : ループ間隔の調整

---

## 4. クラス概要

| クラス名 | 役割 | 主な機能 |
| --- | --- | --- |
| `StepCounter` | 歩数計の計測 | 加速度計測、歩数の加算、表示 |
| `TimeDisplay` | 時刻表示 | NTP からの時刻取得、表示更新 |
| `HealthDisplay` | 健康画面 | 画像表示、歩数表示、時刻表示 |
| `NetworkManager` | Wi-Fi 接続制御 | 接続、再接続、切断 |
| `ScreenManager` | 画面制御 | 画面切替、画面描画 |
| `VersionDisplay` | バージョン表示 | ロゴ表示、バージョン文字列の描画 |
| `SDUtil` | SD カード管理 | 初期化、画像ファイル検索、パス取得 |
| `LcdUtil` | LCD位置計算 | 中央座標や半分サイズの計算 |

---

## 5. クラス詳細と関数説明

### 5.1 `StepCounter`

#### 役割
M5Stack の IMU を使って、加速度の増減を検知し、歩数をカウントするためのクラスです。

#### メンバー変数
- `int stepCount` : 累積歩数
- `bool isStep` : 一歩を検出したかどうかの状態
- `float threshold` : 歩行判定の閾値
- `const int LcdFontSize` : 画面表示のフォントサイズ
- `const int LcdWidth` : 表示位置 X
- `const int LcdHeight` : 表示位置 Y

#### 関数

##### `StepCounter(float threshold)`
- コンストラクタ
- 歩数計の閾値を設定し、初期値として `stepCount = 0` と `isStep = false` を初期化する

##### `void init()`
- `M5.IMU.Init()` を呼び出して、加速度センサの初期化を行う

##### `void update()`
- `M5.IMU.getAccelData(&accX, &accY, &accZ)` で現在の加速度を取得
- `sqrt(accX * accX + accY * accY + accZ * accZ)` により総加速度を計算
- 総加速度が `threshold` を超え、直前に歩行中でなければ `stepCount++`
- 一度閾値を超えた後、閾値を下回った時に `isStep = false` に戻ることで、連続カウントを防ぐ

##### `int getStepCount() const`
- 現在の歩数を返す

##### `void displayStepCnt()`
- LCD に歩数を表示する
- `M5.Lcd.setCursor(LcdWidth, LcdHeight)` で位置設定後、`printf("%05d", stepCount)` で表示

#### 機能説明
- 3軸加速度の変化を監視することで、歩行の判定を行う
- 閾値を超えた瞬間のみ歩数を一つ増やすため、振動やノイズで連続カウントされるのを防ぐ
- 健康画面の「STEP」表示に利用される

---

### 5.2 `TimeDisplay`

#### 役割
NTP サーバーから現在時刻を取得し、M5Stack LCD に表示するクラスです。

#### メンバー変数
- `unsigned long lastUpdateTime` : 最後に時刻表示を更新した時間
- `const int LcdTimeHeight` : 時刻表示の Y 座標
- `const int LcdTimeWidth` : 時刻表示の X 座標
- `const int LcdTimeFontSize` : 文字サイズ
- `const char *ssid`, `password` : 設定中の未使用値（現状は使われていない）
- `const char *ntpServer` : `ntp.nict.jp`
- `const long gmtOffset_sec` : JST の 9 時間分の秒数
- `const int daylightOffset_sec` : 夏時間オフセット

#### 関数

##### `TimeDisplay()`
- 初期化時に `lastUpdateTime = 0` をセットする

##### `void init()`
- `configTime(gmtOffset_sec, daylightOffset_sec, ntpServer)` により時刻同期を開始する
- `lastUpdateTime = millis()` で更新タイミングを初期化する

##### `void update()`
- `millis()` で現在時刻の経過を取得
- `getLocalTime(&timeinfo)` で現地時刻を取得
- 1 秒ごとに `displayTime()` を呼び出して表示を更新する

##### `void displayTime(int hour, int minute, int second)`
- `printf("%02d:%02d:%02d", hour, minute, second)` で時刻表示
- 例: `13:25:09`

#### 機能説明
- NTP による時刻同期で時計の精度を確保する
- 1 秒ごとに表示を更新して、現実時間に合わせて時刻表示を維持する
- 健康画面に「現在時刻」を表示する役割を担う

---

### 5.3 `HealthDisplay`

#### 役割
健康情報画面を描画し、歩数と時刻を表示するクラスです。  
初期化時にロゴ画像/バージョン表示を行い、更新時に健康情報を描画します。

#### メンバー変数
- `float accX, accY, accZ` : 3軸加速度
- `float bAccX, bAccY, bAccZ` : 過去の加速度値
- `int walkCnt` : 歩数を保持する変数

#### 関数

##### `void init()`
- `M5.IMU.Init()` で加速度センサを初期化
- `sdUtil.getFilePath("valuboy2.png")` を使って画像ファイルのパスを取得
- PNG が存在する場合は `M5.Lcd.drawPngFile()` で表示
- 画像がない場合は `PNG Not Found` を表示
- 「PIP BOY ver:0.3」 を画面上に表示
- `timeDisplay.init()` で時刻同期開始
- `stepCounter.init()` と `stepCounter.update()` により歩数計を初期化

##### `void update()`
- `stepCounter.update()` で歩数を更新
- `timeDisplay.update()` で時刻表示更新
- `M5.Lcd.printf("STEP:%04d", stepCounter.getStepCount())` で現在の歩数を描画

#### 機能説明
- 健康画面のメイン画面として役立つ
- Pip-Boy 風のロゴと健康表示を組み合わせたUIを作成する
- M5Stack の LCD 上で、時間と歩数を同時に確認できるように構成されている

---

### 5.4 `NetworkManager`

#### 役割
Wi-Fi 接続の管理を行うクラスです。

#### メンバー変数
- `const char* ssid` : 接続先 SSID
- `const char* password` : 接続先パスワード

#### 関数

##### `void init()`
- `M5.Lcd.print("Connecting to Your SSID")` を表示
- `WiFi.begin(ssid, password)` で接続試行
- `WiFi.status() != WL_CONNECTED` の間は `.` を出力しながら待機
- 接続完了後に `CONNECTED` と表示し、画面をクリアする

##### `void connect()`
- すでに Wi-Fi 接続中であれば再接続を試みる
- 接続待機中は `lcdUtil.getLcdCenter()` を使って画面中央に `.` を出力する

##### `void disConnect()`
- `WiFi.isConnected()` が真なら `WiFi.disconnect(true)` で切断
- `WiFi.mode(WIFI_OFF)` により Wi-Fi をオフにする

#### 機能説明
- アプリ起動時にネットワーク環境を初期化する
- NTP や遠隔通信を前提にした機能の前準備として利用される
- 接続中の待機状態を LCD 上で見える形で表す

---

### 5.5 `ScreenManager`

#### 役割
アプリの画面遷移を管理するクラスです。

#### メンバー変数
- `int currentScreen` : 現在表示中の画面番号
- `const int screenCnt` : 画面数
- `static const int maxFile` : 画像ファイル一覧の上限
- `String fileList[maxFile]` : ファイル名を保持する配列

#### 関数

##### `void init()`
- `M5.begin()`, `M5.Power.begin()` を呼んで初期化
- `M5.Lcd.clear(BLACK)` と `M5.Lcd.setBrightness(200)` で画面表示の基本設定を行う
- `SPIFFS.begin()` を実行
- `sdUtil.init()` により SD カードを準備
- `drawScreen(currentScreen)` により現在の画面を描画

##### `void update()`
- `M5.BtnA.wasPressed()` で `prevScreen()` を実行
- `M5.BtnC.wasPressed()` で `nextScreen()` を実行
- `currentScreen == 0` の時は `healthDisplay.update()`
- `currentScreen == 1` の時は `versionDisplay.update()`

##### `void drawScreen(int screen)`
- `screen == 0` なら `healthDisplay.init()` を呼ぶ
- `screen == 1` なら `versionDisplay.init()` を呼ぶ
- 画面切替時に LCD をクリアして新しい画面を描画する

##### `void nextScreen()`
- 画面番号を `+1` し、`screenCnt` で割った余りで循環させる
- 切り替え後に `drawScreen()` を実行

##### `void prevScreen()`
- 画面番号を `-1` とし、`(currentScreen - 1 + 2) % screenCnt` で前の画面へ戻る

#### 機能説明
- 画面の切替は `A` ボタンと `C` ボタンで制御される
- 健康表示とバージョン表示の 2 画面を循環表示する構成
- 画面遷移の管理を一箇所に集約し、他の表示クラスと分離している

---

### 5.6 `VersionDisplay`

#### 役割
バージョン情報を表示する画面を管理するクラスです。

#### メンバー変数
- `const int maxFile` : 画像ファイルの最大件数
- `int fileCount` : 画像数のカウンタ
- `float accX, accY, accZ` : 3軸加速度
- `float bAccX, bAccY, bAccZ` : 直前の加速度値
- `int walkCnt` : 歩数

#### 関数

##### `void init()`
- `logoName = "piplogo.png"` を定義
- `sdUtil.getFilePath(logoName)` を使ってロゴファイルの絶対パスを取得
- `M5.Lcd.drawPngFile()` でロゴ画像を描画
- 画面中央に「ver:0.3」を表示

##### `void update()`
- 現状は空実装で、更新処理は未実装

#### 機能説明
- バージョン画面を構成するロゴと文字情報を表示する
- 現在の実装では静的表示が中心で、動的な更新はほぼ行っていない

---

### 5.7 `SDUtil`

#### 役割
SD カードの初期化と、画像ファイルの検索・取得を行うユーティリティです。

#### メンバー変数
- `static const int maxFile` : 最大ファイル数
- `String fileList[maxFile]` : 画像ファイル名一覧

#### 関数

##### `void init()`
- `SD.begin(GPIO_NUM_4, SPI, 15000000)` により SD カードを初期化
- `fileList` にデータがある場合は `resetFileList()` を呼び出して初期化
- `createFileList()` で SD 内の画像ファイルを列挙

##### `void createFileList()`
- `/` 直下を走査し、`SD.openNextFile()` で各ファイルを順に確認
- ディレクトリは無視し、`.png` または `.bmp` のファイルだけを `fileList` に格納する

##### `String getFilePath(String fileName)`
- `fileList` から指定したファイル名を探す
- 見つかったファイルのパスを `"/" + filename` 形式で返す
- 表示用ロゴやアイコン読み込みで利用される

##### `void resetFileList()`
- `fileList->clear()` によりファイル一覧をリセットする

#### 機能説明
- ロゴ画像やアイコンの配置先として SD カードを利用する
- 画像ファイルのパス検索を簡潔に行い、表示側からはファイル名だけを指定できる構造になっている

---

### 5.8 `LcdUtil`

#### 役割
LCD の座標計算を共通化するユーティリティです。

#### メンバー変数
- `const int lcdWidth` : LCD の幅
- `const int lcdHeight` : LCD の高さ

#### 関数

##### `std::tuple<int, int> getLcdCenter()`
- LCD の中央座標 `(width / 2, height / 2)` を返す
- 文字やロゴを中央寄せにする際に利用される

##### `int getLcdHalfWidth()`
- LCD 幅の半分を返す

##### `int getLcdHalfHeight()`
- LCD 高さの半分を返す

#### 機能説明
- 文字やロゴを画面中央に配置する処理を簡潔にするための補助クラス
- 複数の描画クラスで共通利用される

---

## 6. 各機能の説明

### 6.1 Wi-Fi 接続機能
- `NetworkManager` が Wi-Fi の初期接続と再接続を担当する
- 接続完了まで LCD に進捗表示を出し、接続失敗時や待機時のユーザー確認をしやすくしている
- 本アプリでは NTP 時刻同期や外部通信の前提となる基盤機能

### 6.2 時刻表示機能
- `TimeDisplay` が NTP 時刻を取得し、LCD に表示する
- `configTime()` により日本標準時 (JST) を設定している
- `update()` が 1 秒単位で表示更新するため、時計として連続動作する

### 6.3 歩数計機能
- `StepCounter` が 3軸加速度を監視して、閾値以上の動きが検出されるたびに歩数を増やす
- 連続して閾値を超えると二重カウントしないように `isStep` フラグを使っている
- 「STEP」表示の主要要素となる

### 6.4 健康画面機能
- `HealthDisplay` が Pip-Boy 風の健康画面を描画する
- 画像ファイル、時刻、歩数をまとめて表示する
- スクリーン管理のメイン画面として利用される

### 6.5 画面切り替え機能
- `ScreenManager` が 2 画面（Health / Version）を制御する
- ボタン `A` と `C` で前後の画面を切り替えられる
- 切り替え時には `drawScreen()` を呼んで画面を再描画する

### 6.6 バージョン表示機能
- `VersionDisplay` がロゴ画像とバージョン文字列を表示する
- 画面中央に `ver:0.3` を表示する構成になっている
- アプリの識別やリリースバージョン確認に使用される

### 6.7 SD カード連携機能
- `SDUtil` が `/.png` / `/.bmp` を列挙し、表示対象イメージのファイルパスを返す
- 画像表示が必要なクラスから簡単に画像を取り出せる
- ロゴやアイコンのファイル管理に役立つ

### 6.8 省電力機能
- `enable_deep_sleep(int time)` により、スリープ状態へ遷移することができる
- 背景の動作を抑えながら、一定時間後に自動再開することが可能
- 低消費電力運用を想定した機能を持つ

---

## 7. まとめ

このアプリは、M5Stack の LCD・IMU・Wi-Fi・SD カードを組み合わせて、Pip-Boy 風の健康管理インターフェースを構築することを目的としています。  
設計としては、以下のような責務分離が行われています。

- 表示責務: `HealthDisplay`, `VersionDisplay`, `TimeDisplay`, `StepCounter`
- 画面制御: `ScreenManager`
- 通信: `NetworkManager`
- ユーティリティ: `SDUtil`, `LcdUtil`
- 実行入口: `main.cpp`

これにより、機能ごとにクラスが分離されており、将来的な機能追加や画面変更がしやすい構造になっています。
