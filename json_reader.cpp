# include <Siv3D.hpp>
# include <vector>
using namespace std;

struct action {
	int agentID;
	String type;
	int dx;
	int dy;
	int turn;
	int apply;
};

struct agent {
	int agentID;
	int x;
	int y;
};

struct team {
	int teamID;
	vector<agent> agents;
	int tilePoint;
	int areaPoint;
};

void Main()
{
	JSONReader json(U"test.json");

	//簡単にとれるやつ
	int width = json[U"width"].get<int>();
	int height = json[U"height"].get<int>();
	int startedAtUnixTime = json[U"startedAtUnixTime"].get<int>();
	int turn = json[U"turn"].get<int>();

	//points取得
	vector<int>tmp;
	vector<vector<int>> points(height, vector<int>(width, 0));

	for (const auto& i : json[U"points"].arrayView())
		for (const auto& j : i.arrayView())
			tmp.push_back(j.get<int>());

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			points[i][j] = tmp[i * width + j];

	//tiled取得
	tmp.clear();
	vector<vector<int>> tiled(height, vector<int>(width, 0));

	for (const auto& i : json[U"tiled"].arrayView())
		for (const auto& j : i.arrayView())
			tmp.push_back(j.get<int>());

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			tiled[i][j] = tmp[i * width + j];


	//teams取得
	vector<team>teams;
	vector<agent>agent_tmp;
	for (const auto& i : json[U"teams"].arrayView()) {
		for (const auto& j : i[U"agents"].arrayView()) {
			int agentID = j[U"agentID"].get<int>();
			int x = j[U"x"].get<int>();
			int y = j[U"y"].get<int>();
			Print(agentID, U" ", x, U" ", y);
			agent_tmp.push_back({ agentID,x,y });
		}
		int teamID = i[U"teamID"].get<int>();
		int tilePoint = i[U"tilePoint"].get<int>();
		int areaPoint = i[U"areaPoint"].get<int>();

		teams.push_back({ teamID,agent_tmp,tilePoint,areaPoint });
		Print(teamID, U" ", tilePoint, U" ", areaPoint);
	}

	//actions取得
	vector<action>actions;
	for (const auto& i : json[U"actions"].arrayView()) {
		int agentID = i[U"agentID"].get<int>();
		String type = i[U"type"].get<String>();
		int dx = i[U"dx"].get<int>();
		int dy = i[U"dy"].get<int>();
		int turn = i[U"turn"].get<int>();
		int apply = i[U"apply"].get<int>();

		Print(agentID, U" ", type, U" ", dx, U" ", dy, U" ", turn, U" ", apply);

		actions.push_back({ agentID,type,dx,dy,turn,apply });
	}

	Print << U"width " << width;
	Print << U"height " << height;
	Print << U"points " << points;
	Print << U"tiled " << tiled;
	Print << U"startedAtUnixTime " << startedAtUnixTime;
	Print << U"turn " << turn;


	json.close();

	while (System::Update())
	{

	}
}
