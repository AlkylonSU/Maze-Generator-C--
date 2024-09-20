#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <chrono>
#include <set>
#include <map>

using namespace std;

template <class T>
class maze;

template <class T>
class LinkedListStack {
public:
	LinkedListStack();

	bool isEmpty();

	void pop();
	T topx();
	T topy();
	void push(T x, T y);
	friend class maze<T>;

private:
	struct nodeStack
	{
		T x;
		T y;
		nodeStack* _next;

		nodeStack(const T& coordX, const T& coordY, nodeStack* n) : x(coordX), y(coordY), _next(n) {}
	};
	nodeStack* _top;
};

template<class T>
class maze {
public:
	maze();
	void createMaze(const int& height, const int& width);
	ofstream mazegen(const string& file);
	ofstream pathFinder(T startX, T startY, T endX, T endY, int ID);

private:
	set<string> XandY;
	map<string, int> LeftWall;
	map<string, int> RightWall;
	map<string, int> DownWall;
	map<string, int> UpWall;
	vector<string> CleanResWalls;
	vector<string> CleanResCoords;
	int mazeHeight;
	int mazeWidth;
	int max_cells;
	LinkedListStack<T> _stack;
	LinkedListStack<T> pathStack;
};


//Other Functions
template <class T>
T inVector(const vector<T>& v1, const vector<T>& v2, T elem1, T elem2) {
	for (int i = 0; i < v1.size(); i++) {
		if (elem1 == v1[i] && elem2 == v2[i]) {
			return i;
		}
	}
	return -1;
}

template <class T>
T Wall(const vector<string>& v1, T e1, T e2, T x, T y) {
	string res;
	for (int i = 0; i < v1.size(); i += (y + 1)) {
		string cc = v1[i];
		int cidx = cc.find(' ');
		cc = cc.substr(0, cidx);
		if (to_string(e1) == cc) {
			return (i + e2);
		}
	}
	return -1;
}

// STACK IMPLEMENTATIONS

template<class T>
LinkedListStack<T>::LinkedListStack()
{
	_top = new nodeStack(0, 0, nullptr);
}


template<class T>
bool LinkedListStack<T>::isEmpty()
{
	return (_top == nullptr);
}

template<class T>
void LinkedListStack<T>::pop()
{
	nodeStack* temp = _top;
	if (_top != nullptr) {
		_top = _top->_next;

	}
	delete temp;
}

template <class T>
T LinkedListStack<T>::topx()
{
	return _top->x;
}

template <class T>
T LinkedListStack<T>::topy()
{
	return _top->y;
}

template <class T>
void LinkedListStack<T>::push(T Xcoord, T Ycoord)
{
	_top = new nodeStack(Xcoord, Ycoord, _top);
}


//MAZE IMPLEMENTATIONS
template <class T>
maze<T>::maze()
{
	XandY.insert("0 0");
	mazeHeight = 0;
	mazeWidth = 0;
}



template <class T>
void maze<T>::createMaze(const int& height, const int& width)
{
	//auto begin = std::chrono::high_resolution_clock::now();

	string forSet;
	string ourKey;
	string ourIdx;
	vector<int> blocked;
	blocked.push_back(0);
	blocked.push_back(0);
	blocked.push_back(0);
	blocked.push_back(0);
	max_cells = (height) * (width);
	int pos_next_x, pos_next_y;
	int visited_cells = 1;
	mazeHeight = height - 1;
	mazeWidth = width - 1;
	random_device rd;
	for (int i = 0; i <= mazeWidth; i++) {
		for (int j = 0; j <= mazeHeight; j++) {
			ourKey = to_string(i) + " " + to_string(j);
			LeftWall[ourKey] = 1;
			DownWall[ourKey] = 1;
			UpWall[ourKey] = 1;
			RightWall[ourKey] = 1;
		}
	}

	int direction = 0;
	while (visited_cells != max_cells) {

		if (blocked[0] + blocked[1] + blocked[2] + blocked[3] >= 4) {
			_stack.pop();
			blocked[0] = 0;
			blocked[1] = 0;
			blocked[2] = 0;
			blocked[3] = 0;
		}
		uniform_int_distribution<int> dist(0, 3); 	 //0 is down, 1 is up, 2 is right, 3 is left
		direction = dist(rd);
		if (direction == 0 && blocked[0] == 0) { //DOWN
			if (_stack.topy() > 0) {
				pos_next_x = _stack.topx();
				pos_next_y = _stack.topy() - 1;
				forSet = to_string(pos_next_x) + " " + to_string(pos_next_y);
				if (XandY.count(forSet) == false) {
					ourIdx = to_string(_stack.topx()) + " " + to_string(_stack.topy());
					DownWall[ourIdx] = 0;
					_stack.push(pos_next_x, pos_next_y);
					blocked[0] = blocked[1] = blocked[2] = blocked[3] = 0;
					XandY.insert(forSet);
					ourIdx = to_string(_stack.topx()) + " " + to_string(_stack.topy());
					UpWall[ourIdx] = 0;
					visited_cells++;
				}
				else {
					blocked[0] = 1;
				}
			}
			else {
				blocked[0] = 1;
			}
		}
		else if (direction == 1 && blocked[1] == 0) { //UP
			if (_stack.topy() < mazeHeight) {
				pos_next_x = _stack.topx();
				pos_next_y = _stack.topy() + 1;
				forSet = to_string(pos_next_x) + " " + to_string(pos_next_y);
				if (XandY.count(forSet) == false) {
					ourIdx = to_string(_stack.topx()) + " " + to_string(_stack.topy());
					UpWall[ourIdx] = 0;
					_stack.push(pos_next_x, pos_next_y);
					blocked[0] = blocked[1] = blocked[2] = blocked[3] = 0;
					XandY.insert(forSet);
					ourIdx = to_string(_stack.topx()) + " " + to_string(_stack.topy());
					DownWall[ourIdx] = 0;
					visited_cells++;
				}
				else {
					blocked[1] = 1;
				}
			}
			else {
				blocked[1] = 1;
			}
		}
		else if (direction == 2 && blocked[2] == 0) { //RIGHT
			if (_stack.topx() < mazeWidth) {
				pos_next_x = _stack.topx() + 1;
				pos_next_y = _stack.topy();
				forSet = to_string(pos_next_x) + " " + to_string(pos_next_y);
				if (XandY.count(forSet) == false) {
					ourIdx = to_string(_stack.topx()) + " " + to_string(_stack.topy());
					RightWall[ourIdx] = 0;
					_stack.push(pos_next_x, pos_next_y);
					blocked[0] = blocked[1] = blocked[2] = blocked[3] = 0;
					XandY.insert(forSet);
					ourIdx = to_string(_stack.topx()) + " " + to_string(_stack.topy());
					LeftWall[ourIdx] = 0;
					visited_cells++;
				}
				else {
					blocked[2] = 1;
				}
			}
			else {
				blocked[2] = 1;
			}
		}
		else if (direction == 3 && blocked[3] == 0) { //LEFT
			if (_stack.topx() > 0) {
				pos_next_x = _stack.topx() - 1;
				pos_next_y = _stack.topy();
				forSet = to_string(pos_next_x) + " " + to_string(pos_next_y);
				if (XandY.count(forSet) == false) {
					ourIdx = to_string(_stack.topx()) + " " + to_string(_stack.topy());
					LeftWall[ourIdx] = 0;
					_stack.push(pos_next_x, pos_next_y);
					blocked[0] = blocked[1] = blocked[2] = blocked[3] = 0;
					XandY.insert(forSet);
					ourIdx = to_string(_stack.topx()) + " " + to_string(_stack.topy());
					RightWall[ourIdx] = 0;
					visited_cells++;
				}
				else {
					blocked[3] = 1;
				}
			}
			else {
				blocked[3] = 1;
			}
		}
	}
	//auto end = std::chrono::high_resolution_clock::now();
	//auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
	//cout << elapsed.count() << "mazegen one" << endl;
}

template<class T>
ofstream maze<T>::mazegen(const string& file)
{
	//auto begin = std::chrono::high_resolution_clock::now();
	ofstream output;
	output.open(file);
	output << mazeHeight + 1 << " " << mazeWidth + 1 << endl;
	for (int i = 0; i <= mazeWidth; i++) {
		for (int j = 0; j <= mazeHeight; j++) {
			string key = to_string(i) + " " + to_string(j);

			output << "x=" << i << " y=" << j << " l=" << LeftWall.at(key) << " r=" << RightWall.at(key) << " u=" << UpWall.at(key) << " d=" << DownWall.at(key) << endl;
		}
	}
	//auto end = std::chrono::high_resolution_clock::now();
	//auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
	//cout << elapsed.count() << "mazefile one" << endl;
	return output;
}



template<class T>
ofstream maze<T>::pathFinder(T startX, T startY, T endX, T endY, int ID)
{
	//auto begin = std::chrono::high_resolution_clock::now();
	vector<int> blocked;
	set<string> _VisitedXandY;
	blocked.push_back(0);
	blocked.push_back(0);
	blocked.push_back(0);
	blocked.push_back(0);
	bool found = false;
	int dir, idx, p1, p2;
	int w1, w2, w3, w4;
	string key, forSet;
	//vector<int> prange = myRandNumGen(0, 3);
	random_device rd;
	pathStack.pop();
	pathStack.push(startX, startY);
	forSet = to_string(startX) + " " + to_string(startY);
	_VisitedXandY.insert(forSet);
	while (!found) {
		//dir = RandGenSelect(prange);
		if (blocked[0] + blocked[1] + blocked[2] + blocked[3] >= 4) {
			pathStack.pop();
			//prange = myRandNumGen(0, 3);
			//cout << "POP" << endl;
			blocked[0] = 0;
			blocked[1] = 0;
			blocked[2] = 0;
			blocked[3] = 0;
		}

		uniform_int_distribution<int> dist(0, 3); 	 //0 is down, 1 is up, 2 is right, 3 is left
		dir = dist(rd);

		if (dir == 0 && blocked[0] == 0) { //down
			if (pathStack.topy() > 0) {
				p1 = pathStack.topx();
				p2 = pathStack.topy() - 1;
				key = to_string(p1) + " " + to_string(p2 + 1);
				forSet = to_string(p1) + " " + to_string(p2);
				w1 = DownWall.at(key);
				if (w1 == 0 && _VisitedXandY.count(forSet) == false) {
					_VisitedXandY.insert(forSet);
					pathStack.push(p1, p2);
					blocked[0] = blocked[1] = blocked[2] = blocked[3] = 0;
				}
				else {
					blocked[0] = 1;
				}
			}
			else {
				blocked[0] = 1;
			}
		}
		else if (dir == 1 && blocked[1] == 0) { //up
			if (pathStack.topy() < mazeHeight) {
				p1 = pathStack.topx();
				p2 = pathStack.topy() + 1;
				key = to_string(p1) + " " + to_string(p2 - 1);
				forSet = to_string(p1) + " " + to_string(p2);
				w2 = UpWall.at(key);
				if (w2 == 0 && _VisitedXandY.count(forSet) == false) {
					pathStack.push(p1, p2);
					_VisitedXandY.insert(forSet);
					blocked[0] = blocked[1] = blocked[2] = blocked[3] = 0;
				}
				else {
					blocked[1] = 1;
				}
			}
			else {
				blocked[1] = 1;
			}
		}
		else if (dir == 2 && blocked[2] == 0) { //right
			if (pathStack.topx() < mazeWidth) {
				p1 = pathStack.topx() + 1;
				p2 = pathStack.topy();
				key = to_string(p1 -1) + " " + to_string(p2);
				forSet = to_string(p1) + " " + to_string(p2);
				w3 = RightWall.at(key);
				if (w3 == 0 && _VisitedXandY.count(forSet) == false) {
					pathStack.push(p1, p2);
					_VisitedXandY.insert(forSet);
					blocked[0] = blocked[1] = blocked[2] = blocked[3] = 0;
				}
				else {
					blocked[2] = 1;
				}
			}
			else {
				blocked[2] = 1;
			}
		}
		else if (dir == 3 && blocked[3] == 0) { //left
			if (pathStack.topx() > 0) {
				p1 = pathStack.topx() - 1;
				p2 = pathStack.topy();
				key = to_string(p1 + 1) + " " + to_string(p2);
				forSet = to_string(p1) + " " + to_string(p2);
				w4 = LeftWall.at(key);
				if (w4 == 0 && _VisitedXandY.count(forSet) == false) {
					pathStack.push(p1, p2);
					_VisitedXandY.insert(forSet);
					blocked[0] = blocked[1] = blocked[2] = blocked[3] = 0;
				}
				else {
					blocked[3] = 1;
				}
			}
			else {
				blocked[3] = 1;
			}
		}
		if (pathStack.topx() == endX && pathStack.topy() == endY) {
			found = true;
		}
	}
	vector<string> path;
	while (pathStack.isEmpty() == false) {

		string res = to_string(pathStack.topx()) + " " + to_string(pathStack.topy());
		path.push_back(res);
		pathStack.pop();
	}
	ofstream cx;
	string fname = "maze_" + to_string(ID) + "_path_" + to_string(startX) + "_" + to_string(startY) + "_" + to_string(endX) + "_" + to_string(endY) + ".txt";
	cx.open(fname);
	for (int i = path.size() - 1; i >= 0; i--) {
		cx << path[i] << endl;
	}
	//auto end = std::chrono::high_resolution_clock::now();
	//auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
	//cout << elapsed.count() << "path finder one" << endl;
	return cx;
}