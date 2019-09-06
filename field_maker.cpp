#include <Siv3D.hpp>
#include<ctime>
#include<cstdlib>
#include<fstream>
using namespace std;

void Main()
{
	/*int vy=14, vt=17;*/
	const Font font(25);
	//画面の大きさを取得
	const int32 w = Window::Width(), h = Window::Height();
	//出力しやすいように動的配列に格納
	/*vector<vector<int>>point(vt,vector<int>(vy));*/
	//画面サイズを指定
	Window::Resize(1280,720);
	//名前の変更
	Window::SetTitle(U"Procon30_fieldmaker");
	//P-->プラス,M-->マイナス r_p　負の出る確率
	int vy = 10, vx = 10, max = 4, r_p = 3,ans=0,min=2;
	//出力しやすいように動的配列に格納
	vector<vector<int>>point(vx, vector<int>(vy));
	//ボタン

	//縦線対称((デフォルト
	while (true){
		srand(time(NULL));
		for (int i = 0; i < vx; i++) {
			for (int j = 0; j < vy / 2; j++) {
				if (rand() % 10 + 1 > r_p) {
					point[i][j] = rand() % max;
					point[i][(vy - 1) - j] = point[i][j];
				}
				else {
					point[i][j] = -(rand() % min+1);
					point[i][(vy - 1) - j] = point[i][j];
				}
			}

		}
		if (vy % 2 != 0) {
			for (int i = 0; i < vx; i++) {
				if (rand() % 10 + 1 > r_p) {
					point[i][vy / 2] = rand() % max;
				}
				else {
					point[i][vy / 2] = -(rand() % min);
				}
			}
		}
		for (int i = 0; i < vx; i++) {
			for (int j = 0; j < vy; j++) {
				ans += point[i][j];
			}
		}
		if (ans >= 0) {
			break;
		}
		
	}
	while (System::Update())
	{
		for (int i = 0; i < vx; i++) {
			for (int j = 0; j < vy; j++) {
				// 矩形を描く.最大で20*20最小で10*10
				Rect(15 + (i * 41), 15 + (j * 31), 40,30).draw(Palette::White);
			}
		}
		//デフォルトで縦線対称を表示
		//フィールドを生成
		for (int i = 0; i < vx; i++) {
			for (int j = 0; j < vy; j++) {
				font(point[i][j]).draw(Arg::bottomLeft = Vec2(23 + (j * 41), 45+ (i * 31)),Palette::Black);
			}
		}
		//スイッチfield
		font(U"フィールドのポイント").draw(Arg::bottomRight = Vec2(1100, 30), Palette::White);
		//縦線対称作成
		if (SimpleGUI::Button(U"縦線対称", Vec2(880, 50), 150)) {
			while (true) {
				srand(time(NULL));
				for (int i = 0; i < vx; i++) {
					for (int j = 0; j < vy / 2; j++) {
						if (rand() % 10 + 1 > r_p) {
							point[i][j] = rand() % max;
							point[i][(vy - 1) - j] = point[i][j];
						}
						else {
							point[i][j] = -(rand() % min+1);
							point[i][(vy - 1) - j] = point[i][j];
						}
					}

				}
				if (vy % 2 != 0) {
					for (int i = 0; i < vx; i++) {
						if (rand() % 10 + 1 > r_p) {
							point[i][vy / 2] = rand() % max;
						}
						else {
							point[i][vy / 2] = -(rand() % min+1);
						}
					}
				}
				for (int i = 0; i < vx; i++) {
					for (int j = 0; j < vy; j++) {
						ans += point[i][j];
					}
				}
				if (ans >= 0) {
					break;
				}

			}
		}
		//横線対称
		if (SimpleGUI::Button(U"横線対称", Vec2(1050, 50), 150)) {
			while (true) {
				srand(time(NULL));
				for (int i = 0; i < vx/2; i++) {
					for (int j = 0; j < vy; j++) {
						if (rand() % 10 + 1 > r_p) {
							point[i][j] = rand() % max;
							point[(vx - 1) - i][j] = point[i][j];
						}
						else {
							point[i][j] = -(rand() % min);
							point[(vx - 1) - i][j] = point[i][j];
						}
					}

				}
				if (vx % 2 != 0) {
					for (int i = 0; i < vy; i++) {
						if (rand() % 10 + 1 > r_p) {
							point[vx / 2][i] = rand() % max;
						}
						else {
							point[vx / 2][i] = -(rand() % min);
						}
					}
				}
				for (int i = 0; i < vx; i++) {
					for (int j = 0; j < vy; j++) {
						ans += point[i][j];
					}
				}
				if (ans >= 0) {
					break;
				}

			}
		}
		//点対称
		if (SimpleGUI::Button(U"点対称", Vec2(880, 100), 150)) {
			while (true) {
				for (int i = 0; i < vx / 2; i++) {
					for (int j = 0; j < vy / 2; j++) {
						if (rand() % 10 + 1 > r_p) {
							point[i][j] = rand() % max;
							point[(vx - 1) - i][(vy - 1) - j] = point[i][j];
							point[i][(vy - 1) - j] = point[i][j];
							point[(vx - 1) - i][j] = point[i][j];
						}
						else {
							point[i][j] = -(rand() % min);
							point[(vx - 1) - i][(vy - 1) - j] = point[i][j];
							point[i][(vy - 1) - j] = point[i][j];
							point[(vx - 1) - i][j] = point[i][j];
						}
					}
				}
				if (vx % 2 != 0) {
					for (int i = 0; i < vx / 2; i++) {
						if (rand() % 10 + 1 > r_p) {
							point[vx / 2][i] = rand() % max;
							point[vx / 2][(vx - 1) - i] = point[vx / 2][i];
						}
						else {
							point[vx / 2][i] = -(rand() % min);
							point[vx / 2][(vx - 1) - i] = point[vx / 2][i];
						}
					}
				}
				if (vy % 2 != 0) {
					for (int i = 0; i < vy / 2; i++) {
						if (rand() % 10 + 1 > r_p) {
							point[i][vy / 2] = rand() % max;
							point[(vy - 1) - i][vy / 2] = point[i][vy / 2];
						}
						else {
							point[i][vy / 2] = -(rand() % min);
							point[(vy - 1) - i][vy / 2] = point[i][vy / 2];
						}
					}
					
				}
				for (int i = 0; i < vx; i++) {
					for (int j = 0; j < vy; j++) {
						ans += point[i][j];
					}
				}
				if (ans >= 0) {
					break;
				}
			}
		}
		//csv出力
		if(KeyF11.down()) {
			ofstream ofs("test.csv");
			for (int i = 0; i < vx; i++) {
				for (int j = 0; j < vy; j++) {
					ofs << (int)point[i][j] << ",";
				}
				ofs << endl;
			}
			ofs.close();
		}

	}
}
