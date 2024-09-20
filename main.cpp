#include "maze3.cpp"

using namespace std;

int main() {

	maze<int> m2;
	vector<maze<int>> mazes;
	int cols, rows, K, sx, sy, ex, ey, find;
	string filename;
	cout << "Enter the number of mazes: " << endl;
	cin >> K;
	cout << "Enter the number of rows and columns (M and N): " << endl;
	cin >> rows >> cols;
	for (int i = 1; i <= K; i++) {
		maze<int> m1;
		m1.createMaze(cols, rows);
		mazes.push_back(m1);
		filename = "maze_" + to_string(i) + ".txt";
		m1.mazegen(filename);
	}
	cout << "All mazes are generated." << endl << endl;
	cout << "Enter a maze ID between 1 to "  + to_string(K) + " inclusive to find a path: " << endl;
	cin >> find;
	m2 = mazes[find - 1];
	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): " << endl;
	cin >> sy >> sx;
	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): " << endl;
	cin >> ey >> ex;
	m2.pathFinder(sx, sy, ex, ey, find);
	return 0;
}