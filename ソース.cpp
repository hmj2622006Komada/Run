//#include "DxLib.h"
//#include <stdlib.h>
//
//// ボートの画像を管理する定数と配列
//enum { WHITE, BLACK, RED, BLUE, YELLOW, GREEN};
//const int BOAT_MAX = 6;
//int imgBoat[BOAT_MAX];
//const int BOAT_W[BOAT_MAX] = { 32,26,26,40 };
//const int BOAT_H[BOAT_MAX] = { 48,48,48,100 };
//
//// ボートを表示する関数
//void DrawBoat(int x, int y, int type)
//{
//	DrawGraph(x - BOAT_W[type] / 2, y - BOAT_H[type] / 2, imgBoat[type], true);
//}
//
//int APIENTRY WinMain(_In_ HINSTANCE hIstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
//{
//	// 定数
//	const int WIDTH = 1500, HEIGHT = 800;  // ウィンドウの幅と高さ
//	
//	SetWindowText("ボートレース");  // ウィンドウのタイトル
//	SetGraphMode(WIDTH, HEIGHT, 32);  // ウィンドウの大きさとカラービット数の指定
//	ChangeWindowMode(true);  // ウィンドウで起動
//	if (DxLib_Init() == 1)return -1;  // ライブラリ初期化　エラーが起きたら終了
//	SetBackgroundColor(0, 0, 0);  // 背景色の指定
//	SetDrawScreen(DX_SCREEN_BACK);  // 描画面を裏画面にする
//
//	int imgBG = LoadGraph("image/bg.png");  // 背景の画面
//	
//
//	int White = LoadGraph("image/1号艇.png");
//	int BLACK= LoadGraph("image/2号艇.png");
//	int RED = LoadGraph("image/3号艇.png");
//	int BLUE = LoadGraph("image/4号艇.png");
//	int YELLOW = LoadGraph("image/5号艇.png");
//	int GREEN = LoadGraph("image/6号艇.png");
//	
//	
//
//	while (1)  // メインループ
//	{
//		ClearDrawScreen();  // 画面をクリアにする
//
//		DrawGraph(0, 0, White, true);
//		DrawGraph(0, 120, BLACK, true);
//		DrawGraph(0, 240, RED, true);
//		DrawGraph(0, 360, BLUE, true);
//		DrawGraph(0, 480, YELLOW, true);
//		DrawGraph(0, 600, GREEN, true);
//
//		ScreenFlip();   // 裏画面の内容を表画面に反映させる
//		WaitTimer(16);  // 一定時間待つ
//		if (ProcessMessage() == -1)break;              // Windowsから情報を受け取りエラーが起きたら終了
//		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;  // ESCキーが押されたら終了
//	}
//	
//	DxLib_End();  // DXライブラリ使用の終了処理
//	return 0;     // ソフトの終了
//}