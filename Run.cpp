#include "DxLib.h"

// 障害物・プレイヤー画像用
enum { GREEN, ZAKO, TOGE, C_MAX };
int imgC[C_MAX];

// シーンの定義
enum GameScene {
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_GAMEOVER
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	// 定数
	const int WIDTH = 1200, HEIGHT = 675;

	// 初期設定
	SetWindowText("ラン");
	SetGraphMode(WIDTH, HEIGHT, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) return -1;
	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像の読み込み
	int imgBG = LoadGraph("image/Bg.png");
	imgC[GREEN] = LoadGraph("image/Dragon.png");
	imgC[ZAKO] = LoadGraph("image/Teki.png");
	imgC[TOGE] = LoadGraph("image/toge1png.png");

	// 画面サイズの取得
	int playerW, playerH;
	int enemyW, enemyH;
	int togeW, togeH;
	GetGraphSize(imgC[GREEN], &playerW, &playerH);
	GetGraphSize(imgC[ZAKO], &enemyW, &enemyH);
	GetGraphSize(imgC[TOGE], &togeW, &togeH);

	// 現在のシーン
	int scene = SCENE_TITLE;

	// ゲーム変数
	float Playerx = 40.0f;
	float Playery = 440.0f;
	float velocity_y = 0.0f;
	bool is_jumping = false;
	const float gravity = 0.5f;
	const float jump_power = -14.0f;
	const float ground_y = 440.0f;

	int obstacleType = ZAKO;
	int obstacleX = 1300;
	int obstacleY = 420;
	int obstacleSpeed = 10;
	int bgX = 0;

	// メインループ
	while (ProcessMessage() == 0)
	{
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;

		ClearDrawScreen();

		// シーン分岐処理
		switch (scene)
		{
			// 1. タイトル画面
		case SCENE_TITLE:
			DrawString(450, 250, "=== RUN GAME ===", GetColor(255, 255, 255));
			DrawString(420, 350, "PRESS SPACE TO START", GetColor(255, 255, 0));

			// SPACEキーでゲーム開始
			if (CheckHitKey(KEY_INPUT_SPACE) == 1)
			{
				// ゲームパラメータの初期化（リセット）
				Playerx = 40.0f;
				Playery = ground_y;
				velocity_y = 0.0f;
				is_jumping = false;

				obstacleType = ZAKO;
				obstacleX = 1300;
				obstacleY = 420;
				obstacleSpeed = 10;

				scene = SCENE_GAME; // ゲーム画面へ移行
			}
			break;

			// 2. ゲーム画面
		case SCENE_GAME:
			// 障害物の移動 
			obstacleX -= obstacleSpeed;

			if (obstacleX <= -200)
			{
				obstacleX = 1300;
				if (obstacleType == ZAKO)
				{
					obstacleType = TOGE;
					obstacleY = 470;
					obstacleSpeed = 5;
				}
				else
				{
					obstacleType = ZAKO;
					obstacleY = 420;
					obstacleSpeed = 10;
				}
			}

			// 背景スクロール
			bgX = (bgX - 5) % WIDTH;
			DrawGraph(bgX + WIDTH, 0, imgBG, FALSE);
			DrawGraph(bgX, 0, imgBG, FALSE);

			// ジャンプ処理
			if (is_jumping) {
				velocity_y += gravity;
				Playery += velocity_y;

				if (Playery >= ground_y) {
					Playery = ground_y;
					velocity_y = 0.0f;
					is_jumping = false;
				}
			}
			else {
				if (CheckHitKey(KEY_INPUT_SPACE) == 1) {
					velocity_y = jump_power;
					is_jumping = true;
				}
			}

			// 当たり判定
			{
				int currentW = (obstacleType == ZAKO) ? enemyW / 2 : togeW;
				int currentH = (obstacleType == ZAKO) ? enemyH : togeH;

				if (Playerx < obstacleX + currentW && Playerx + playerW > obstacleX &&
					Playery < obstacleY + currentH && Playery + playerH > obstacleY)
				{
					scene = SCENE_GAMEOVER; // 当たったらゲームオーバー画面へ
				}
			}

			// 描画
			DrawGraph((int)Playerx, (int)Playery, imgC[GREEN], TRUE);
			DrawGraph(obstacleX, obstacleY, imgC[obstacleType], TRUE);
			break;

			// 3. ゲームオーバー
		case SCENE_GAMEOVER:
			// 背景とキャラクターをそのまま残す（停止画面にする）
			DrawGraph(bgX + WIDTH, 0, imgBG, FALSE);
			DrawGraph(bgX, 0, imgBG, FALSE);
			DrawGraph((int)Playerx, (int)Playery, imgC[GREEN], TRUE);
			DrawGraph(obstacleX, obstacleY, imgC[obstacleType], TRUE);

			// メッセージ表示
			DrawString(480, 250, "GAME OVER", GetColor(255, 0, 0));
			DrawString(420, 350, "PRESS 'R' TO RESTART", GetColor(255, 255, 255));

			// Rキーでタイトル画面へ戻る
			if (CheckHitKey(KEY_INPUT_R) == 1)
			{
				scene = SCENE_TITLE;
			}
			break;
		}

		ScreenFlip();   //
		WaitTimer(16);
	}

	DxLib_End();
	return 0;
}