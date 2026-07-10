#include "DxLib.h"

enum{GREEN, ZAKO};
const int C_MAX = 2;
int imgC[C_MAX];


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmShow)
{
	// 定数
	const int WIDTH = 1200, HEIGHT = 675;  // ウィンドウの幅と高さのピクセル数
	/*int x = WIDTH / 2;
	int y = HEIGHT / 2;
	double*/
	float x= 40.0f;
	float y = 440.0f;
	float velocity_y = 0.0f;
	bool is_jumping = false;
	float gravity = 0.5f;
	float jump_power = -11.5f;
	float ground_y = 440.0f;
	int timer = 0;


	SetWindowText("ラン");        // ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32);    // ウィンドウの大きさとカラービット数
	ChangeWindowMode(true);             // ウィンドウモードで起動
	if (DxLib_Init() == -1) return -1;  // ライブラリ初期化　エラーが起きたら終了
	SetBackgroundColor(0, 0, 0);        // 背景色の指定
	SetDrawScreen(DX_SCREEN_BACK);      // 描画画面を裏画面にする

	int bgX = 0;
	int imgBG = LoadGraph("image/Bg.png");  // 背景画像

	imgC[GREEN] = LoadGraph("image/Dragon.png");  // キャラクターの表示
	int Playerx = 40;
	imgC[ZAKO] = LoadGraph("image/Teki.png");  // 敵の表示
	int comx = 1300;

	while (1)
	{
		ClearDrawScreen();  // 画面をクリアにする
		char key[256];
		GetHitKeyStateAll(key);
		timer++;
		comx-=10;  // 敵のスピード
		// 背景スクロールの処理
		bgX = (bgX - 5) % WIDTH;
		DrawGraph(bgX + WIDTH, 0, imgBG, false);
		DrawGraph(bgX, 0, imgBG, false);


		// ジャンプ処理
		if (is_jumping) {
			// 空中にいる場合は重力を速度に加算
			velocity_y += gravity;
			y += velocity_y;

			// 着地判定
			if (y >= ground_y) {
				y = ground_y;
				velocity_y = 0.0f;
				is_jumping = false;
			}
		}
		else {
			// 地面にいる時にジャンプボタンが押されたら
			if (CheckHitKey(KEY_INPUT_SPACE) == 1) {
				velocity_y = jump_power; // 上方向へ初速を与える
				is_jumping = true;
			}
		}
		DrawGraph(Playerx, y, imgC[GREEN], true);

		if(comx <= -200)
		{
			comx = 1300;
		}
		DrawGraph(comx, 420, imgC[ZAKO], true);

		ScreenFlip();   // 裏画面の内容を表画面に反映させる
		WaitTimer(16);  // 一定時間待つ
		if (ProcessMessage() == -1)break;              // Windowsから情報を受け取りエラーが起きたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;  // ESCキーが押されたら終了
		
		//DrawBox((int)x, (int)y, (int)x + 32, (int)y + 32, GetColor(255, 255, 255), TRUE);
		//DrawLine(0, 440, 640, 440, GetColor(255, 0, 0)); // 地面を描画


	}
		DxLib_End();  // DXライブラリ使用の終了処理
		return 0;     // ソフトの終了
}