#pragma once
#include"json_get.h"

void Json::setJsonName(String s) {
	jsonName = s;
}

int Json::width() {
	json.open(jsonName);
	return json[U"width"].get<int>();
}

int Json::height() {
	json.open(jsonName);
	return json[U"height"].get<int>();
}

vector<vector<int>> Json::points(int height,int width) {
	json.open(jsonName);
	vector<int>tmp;
	vector<vector<int>> points(height, vector<int>(width, 0));

	for (const auto& i : json[U"points"].arrayView())
		for (const auto& j : i.arrayView())
			tmp.push_back(j.get<int>());

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			points[i][j] = tmp[i * width + j];

	return points;
}

int Json::startedAtUnixTime() {
	json.open(jsonName);
	return json[U"startAtUnixTime"].get<int>();
}

int Json::turn() {
	json.open(jsonName);
	return json[U"turn"].get<int>();
}

vector<vector<int>> Json::tiled(int height, int width) {
	vector<int>tmp;
	vector<vector<int>> tiled(height, vector<int>(width, 0));

	for (const auto& i : json[U"tiled"].arrayView())
		for (const auto& j : i.arrayView())
			tmp.push_back(j.get<int>());

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			tiled[i][j] = tmp[i * width + j];

	return tiled;
}

vector<team> Json::teams() {
	vector<team>teams;
	vector<agent>agent_tmp;
	for (const auto& i : json[U"teams"].arrayView()) {
		for (const auto& j : i[U"agents"].arrayView()) {
			int agentID = j[U"agentID"].get<int>();
			int x = j[U"x"].get<int>();
			int y = j[U"y"].get<int>();
			agent_tmp.push_back({ agentID,x,y });
		}
		int teamID = i[U"teamID"].get<int>();
		int tilePoint = i[U"tilePoint"].get<int>();
		int areaPoint = i[U"areaPoint"].get<int>();

		teams.push_back({ teamID,agent_tmp,tilePoint,areaPoint });
	}

	return teams;
}

vector<action> Json::actions() {
	vector<action>actions;
	for (const auto& i : json[U"actions"].arrayView()) {
		int agentID = i[U"agentID"].get<int>();
		String type = i[U"type"].get<String>();
		int dx = i[U"dx"].get<int>();
		int dy = i[U"dy"].get<int>();
		int turn = i[U"turn"].get<int>();
		int apply = i[U"apply"].get<int>();

		actions.push_back({ agentID,type,dx,dy,turn,apply });
	}
	
	return actions;
}