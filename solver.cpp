#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<cmath>
#include<stack>
#include<iterator>
#include<thread>
#include<mutex>
#include<chrono>
using namespace std;


mutex Q_mutex;
struct xtoy {
	int x;
	int y;
};

class Node {
public:
	stack<vector<xtoy>> st;
	int max = 0;
	int deep = 0;
	//優先度設定オーバーロード
	bool operator < (const Node& container) const {
		return (max < container.max);
	}

	void rinsetsu(priority_queue<Node>& qu, const vector<vector<int>>& val) const {
		Node tempclass;
		int d = deep + 1;
		int x = -1;
		int y = -1;
		int total = 0;
		int	valsize_y = val.size() - 1;
		int	valsize_x = val[0].size() - 1;
		bool dainyu = true;
		vector<xtoy> tempbox(st.top().size());
		vector<vector<xtoy>>stcvec;
		stack<vector<xtoy>> tempst;
		tempst = st;
		//vectorにstackコピー
		for (int i = 0; (tempst.empty() == false); i++, tempst.pop()) {
			stcvec.push_back(tempst.top());
		}

		tempst = st;
		//判定
		for (; x <= 1; x++) {
			for (y = -1; y <= 1; y++) {
				dainyu = true;
				for (int i = 0; i < st.top().size(); i++) {
					if (x + st.top()[i].x<0 || y + st.top()[i].y<0 || x + st.top()[i].x>valsize_x || y + st.top()[i].y>valsize_y) {
						dainyu = false;
						break;
					}
				}
				for (int i = 0; i < st.top().size(); i++) {
					for (int k = 0; k < stcvec.size(); k++) {
						for (int j = 0; j < stcvec[k].size(); j++) {
							if ((stcvec[k][j].x == x + st.top()[i].x) && (stcvec[k][j].y == y + st.top()[i].y)) {
								dainyu = false;
								break;
							}
						}
					}
				}
				//判定ここまで

				if (dainyu == true) {
					if (x == 0 && y == 0) {
						continue;
					}
					for (int i = 0; i < st.top().size(); i++) {
						tempbox[i].x = x + st.top()[i].x;
						tempbox[i].y = y + st.top()[i].y;
						total += val[tempbox[i].y][tempbox[i].x];
					}
					tempst.push(tempbox);
					tempclass.st = tempst;
					tempclass.deep = d;
					tempclass.max = max + total;
					total = 0;
					//lock
					{
						lock_guard<mutex>lock(Q_mutex);
						qu.push(Node{ tempclass });
					}
					tempst.pop();
				}

			}
		}
	}
};


int main() {
	auto time_before = chrono::system_clock::now();
	cout << "=========================" << endl;
	const vector<vector<int>>value{
	{0,3,-1,1,2,2,1,-1,3,0},
	{3,-3,1,2,3,3,2,1,-3,3},
	{1,2,3,2,3,3,2,3,2,1},
	{-2,1,0,2,2,2,2,0,1,-2},
	{-2,1,0,2,-2,-2,2,0,1,-2},
	{-2,-1,2,3,2,2,3,2,-1,-2},
	{-3,2,-2,3,-3,-3,3,-2,2,-3},
	{-3,0,3,3,3,3,3,3,0,-3},
	{-1,-3,2,1,-1,-1,1,2,-3,-1},
	{0,3,3,2,0,0,2,3,3,0}
	};

	vector<xtoy> agent{ {7,7},{2,2},{7,2},{2,7} };
	priority_queue<Node>PQ;
	priority_queue<Node>temppq;
	vector<thread>thr;
	stack<Node>tempstack;
	//しょきか
	Node first;
	first.st.push(agent);
	first.max = 0;
	first.deep = 0;
	//深さ1
	first.rinsetsu(PQ, value);
	//しょきかここまで

	//深さ2から
	for (int j = 0; j < 6; j++) {
		
		//リセット
		thr.clear();
		while (!tempstack.empty())tempstack.pop();

		temppq = PQ;
		//リセット
		while (!PQ.empty()) {  PQ.pop(); }
		//評価
		for (; temppq.empty() == false; temppq.pop()) {//評価ノード調節(ここにビームサーチ)
			tempstack.push(temppq.top());
			{
				lock_guard<mutex>lock_a(Q_mutex);
				thr.push_back(thread(&Node::rinsetsu, cref(tempstack.top()), ref(PQ), ref(value)));
			}
		}
		for (auto iter = thr.begin(); iter != thr.end(); ++iter) {
			iter->join();
		}

		cout << "探索の深さ:" << PQ.top().deep << " " << "ノードの数:" << PQ.size() << endl;
	}


	auto time_after = chrono::system_clock::now();
	auto time_all = time_after - time_before;
	cout << "=========================" << endl;
	Node result;
	result = PQ.top();
	cout << "所要時間:" << chrono::duration_cast<chrono::milliseconds>(time_all).count() << "msec"<< endl;
	cout << "探索の深さ:" << result.deep << " " << "ノードの数:" << PQ.size() << endl;
	for (int depth = result.st.size() - 1; result.st.empty() == false; result.st.pop(), depth--) {
		cout << "深さ:" << depth << endl;
		for (int i = 0; i < agent.size(); i++) {
			cout << "座標" << i << ":" << "(" << result.st.top()[i].x << "," << result.st.top()[i].y << ")" << endl;
		}

	}
}
