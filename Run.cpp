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

	// サウンドの読み込み
	int bgm = LoadSoundMem("sound/PlayBGM.mp3");
	int jump = LoadSoundMem("sound/Jamp.mp3");
	int run = LoadSoundMem("sound/Run.mp3");
	int gameOver = LoadSoundMem("sound/GamOver.mp3");

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
	const float jump_power = -13.0f;
	const float ground_y = 440.0f;

	int obstacleType = ZAKO;
	int obstacleX = 1300;
	int obstacleY = 420;
	int obstacleSpeed = 10;
	int bgX = 0;
	int timer = 0;
	int score = 0;
	int speedLevel = 0;
	const int LEVEL_UP_TIME = 600;

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
		SetFontSize(48);

		// シーン分岐処理
		switch (scene)
		{
			// 1. タイトル画面
		case SCENE_TITLE:
			DxLib::DrawString(450, 250, "=== RUN GAME ===", GetColor(255, 255, 255));
			DxLib::DrawString(420, 350, "PRESS SPACE TO START", GetColor(255, 255, 0));
			PlaySoundMem(run, DX_PLAYTYPE_LOOP);
			

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

				score = 0;
				timer = 0;
				speedLevel = 0;

				scene = SCENE_GAME; // ゲーム画面へ移行
			}
			break;

			// 2. ゲーム画面
		// 2. ゲーム画面
		case SCENE_GAME:
		{
			StopSoundMem(run);
			PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
			timer++;

			if (timer >= LEVEL_UP_TIME)
			{
				timer = 0;
				speedLevel++;
			}

			// スピードの計算
			int baseObstacleSpeed = obstacleSpeed + (speedLevel * 2);
			int currentBgSpeed = 6 + (speedLevel * 1); // 背景のスピード

			int currentSpeed = baseObstacleSpeed;
			if (obstacleType == TOGE)
			{
				currentSpeed = currentBgSpeed;
			}

			int scoreAdd = 1 + speedLevel;
			score += scoreAdd;

			// 障害物の移動
			obstacleX -= currentSpeed;

			if (obstacleX <= -200)
			{
				score += 100;

				obstacleX = 1300;
				if (obstacleType == ZAKO)
				{
					obstacleType = TOGE;
					obstacleY = 470;
					// ※TOGEの速度は背景と同期するため、ここで個別のスピード指定は不要になります
				}
				else
				{
					obstacleType = ZAKO;
					obstacleY = 420;
					obstacleSpeed = 13;
				}
			}

			// 背景スクロール
			bgX = bgX - currentBgSpeed;
			if (bgX <= -WIDTH)
			{
				bgX += WIDTH;
			}
			DxLib::DrawGraph(bgX + WIDTH, 0, imgBG, FALSE);
			DxLib::DrawGraph(bgX, 0, imgBG, FALSE);

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
					PlaySoundMem(jump,DX_PLAYTYPE_BACK);
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
			DxLib::DrawGraph((int)Playerx, (int)Playery, imgC[GREEN], TRUE);
			DxLib::DrawGraph(obstacleX, obstacleY, imgC[obstacleType], TRUE);

			DxLib::SetFontSize(40);
			DxLib::DrawFormatString(900, 20, GetColor(150, 0, 255), "SCORE: %06d", score);
		}
		break;

		// 3. ゲームオーバー
		case SCENE_GAMEOVER:
			StopSoundMem(bgm);
			PlaySoundMem(gameOver, DX_PLAYTYPE_LOOP);
			// 背景とキャラクターをそのまま残す（停止画面にする）
			DxLib::DrawGraph(bgX + WIDTH, 0, imgBG, FALSE);
			DxLib::DrawGraph(bgX, 0, imgBG, FALSE);
			DxLib::DrawGraph((int)Playerx, (int)Playery, imgC[GREEN], TRUE);
			DxLib::DrawGraph(obstacleX, obstacleY, imgC[obstacleType], TRUE);

			// メッセージ表示
			DxLib::DrawString(480, 200, "GAME OVER", GetColor(255, 0, 0));
			DxLib::DrawFormatString(480, 280, GetColor(150, 0, 255), "SCORE: %d", score);
			DxLib::DrawString(400, 360, "PRESS 'R' TO RESTART", GetColor(200, 150, 0));

			// Rキーでタイトル画面へ戻る
			if (CheckHitKey(KEY_INPUT_R) == 1)
			{
				scene = SCENE_TITLE;
				StopSoundMem(gameOver);
			}
			break;
		}

		ScreenFlip();
	}

	DxLib_End();
	return 0;
}