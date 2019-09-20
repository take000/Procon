#include<Siv3D.hpp> // OpenSiv3D v0.3.2
#include<vector>
#include<string>
#include"json_get.h"

using namespace std;

//フィールドに関する情報//
int x = 0;
int y = 0;
////ポイント//
vector<vector<int>> point;
//フィールドの座標//
vector<vector<int>> tile;
int kousin[20][20] = { 0 };
//チームID//
int teamID = 5, enemyID = 6;
//領域及び範囲ポイント//
int mytile = 11, enemytile = 12;
int myarea = 13, enemyarea = 14;
//agent//
int agenttt = 8;
int agentID[8] = { 1,2,3,4,5,6,7,8 };
string agentSt[8] = { "move","remove","stay","move","remove","stay" ,"move","remove" };
//agent_coord//
int agent_coord[8][2] = {
	{2,8},{11,16},{1,8},{17,14},
	{13,19},{5,17},{8,13},{6,6},
};
//turn//
int turn = 1;

//team//
vector<team> teams(2);

//action//
vector<action> actions(0);

void Main()
{
	Json json;
	//font定義//
	const Font font(25);
	const Font p_font(50);
	const Font sougouf(60);
	//画面サイズを指定
	Window::Resize(1280, 720);
	//名前の変更
	Window::SetTitle(U"Procon30_kagawa");
	TextBox textbox(font, Vec2(900, 0), 200, none, U"A-1.json");
	while (System::Update())
	{
		//更新点滅//
		const double animation = Periodic::Square0_1(1s);
		//点数バー//
		Rect(10, 10, 380, 70).draw(Palette::Dodgerblue);
		Rect(450, 10, 380, 70).draw(Palette::Orange);
		//ポイント表示//
		//味方//
		p_font(mytile, U"+", myarea).draw(Arg::bottomLeft = Vec2(23, 90), Palette::White);
		font(U"総合                　　　　　総合").draw(Arg::bottomLeft = Vec2(250, 78), Palette::White);
		font(U"タイル     領域　　　　　　　　　　　　　　　　　　   タイル     領域").draw(Arg::bottomLeft = Vec2(20, 40), Palette::White);
		sougouf(mytile + myarea).draw(Arg::bottomLeft = Vec2(300, 90), Palette::White);
		//敵//
		sougouf(enemytile + enemyarea).draw(Arg::bottomLeft = Vec2(463, 90), Palette::White);
		p_font(enemytile, U"+", enemyarea).draw(Arg::bottomLeft = Vec2(653, 90), Palette::White);

		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				// 矩形を描く.最大で20*20最小で10*10
				//タイルが0の場合:白
				if (tile[i][j] == 0)
					Rect(10 + (j * 41), 90 + (i * 31), 40, 30).draw(Palette::White);
				//タイルがチームIDの場合:青
				else if (tile[i][j] == teamID)
					if (kousin[i][j] == teamID)
						Rect(10 + (j * 41), 90 + (i * 31), 40, 30).draw(Palette::Dodgerblue).draw(AlphaF(animation));
					else
						Rect(10 + (j * 41), 90 + (i * 31), 40, 30).draw(Palette::Dodgerblue);
				//タイルが敵チームIDの場合:オレンジ
				else if (tile[i][j] == enemyID) {
					if (kousin[i][j] == enemyID)
						Rect(10 + (j * 41), 90 + (i * 31), 40, 30).draw(Palette::Orange).draw(AlphaF(animation));
					else
						Rect(10 + (j * 41), 90 + (i * 31), 40, 30).draw(Palette::Orange);

				}
			}
		}
		//フィールドを生成
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				font(point[i][j]).drawAt(Vec2(30 + (j * 41), 105 + (i * 31)), Palette::Black);
			}
		}
		//turn//
		p_font(U"turn:", turn).draw(Vec2(850, 20), Palette::White);
		//agent status//
		for (int i = 0; i < actions.size(); i++) {
			if (i != 0 && i % 2 != 0) {
				font(U"agent:", actions[i].agentID).draw(Vec2(1070, 100 + (i - 1) * 18), Palette::White);
				if (actions[i].type == U"move")
					font(U"move").draw(Vec2(1190, 100 + (i - 1) * 18), Palette::Cyan);
				else if (actions[i].type == U"remove")
					font(U"remove").draw(Vec2(1190, 100 + (i - 1) * 18), Palette::Yellow);
				else if (actions[i].type == U"stay")
					font(U"stay").draw(Vec2(1190, 100 + (i - 1) * 18), Palette::Red);
			}
			else {
				font(U"agent:", agentID[i]).draw(Vec2(850, 100 + i * 18), Palette::White);
				if (actions[i].type == U"move")
					font(U"move").draw(Vec2(970, 100 + i * 18), Palette::Cyan);
				else if (actions[i].type == U"remove")
					font(U"remove").draw(Vec2(970, 100 + i * 18), Palette::Yellow);
				else if (actions[i].type == U"stay")
					font(U"stay").draw(Vec2(970, 100 + i * 18), Palette::Red);
			}
		}
		//更新ボタン//
		if (SimpleGUI::Button(U"更新", Vec2(850, 400), 400, FileSystem::Exists(textbox.getText()))) {
			json.setJsonName(textbox.getText());
			x = json.width();
			y = json.height();
			turn = json.turn();

			point.assign(x, vector<int>(y, 0));
			point = json.points(x, y);

			tile.assign(x, vector<int>(y, 0));
			tile = json.tiled(x, y);

			teams = json.teams();
			teamID = teams[0].teamID;
			enemyID = teams[1].teamID;

			mytile = teams[0].tilePoint;
			myarea = teams[0].areaPoint;
			enemytile = teams[1].tilePoint;
			enemyarea = teams[1].areaPoint;

			actions.resize(turn);
			actions = json.actions();
		}
		//ソルバーメッセージ//
		font(U"solver status").draw(Vec2(850, 450), Palette::White);
		Rect(850, 490, 420, 200).draw(Palette::Black);
		
		//---------------------------
		textbox.update();
		textbox.draw();
		textbox.drawOverlay();
		//---------------------------
	}
}