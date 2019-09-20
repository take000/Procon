#pragma once
#include<Siv3D.hpp>
#include<vector>
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

class Json {
	String jsonName;
	JSONReader json;
public:
	void setJsonName(String s);
	int width();
	int height();
	vector<vector<int>> points(int height, int width);
	int startedAtUnixTime();
	int turn();
	vector<vector<int>> tiled(int height, int width);
	vector<team> teams();
	vector<action> actions();
};