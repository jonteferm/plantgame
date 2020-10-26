#include "Config.h";
#include <iostream>;
#include <sstream>;
#include <vector>;
#include <array>;
#include <stack>;

using namespace std;

class AStar
{
private:
	friend bool operator < (const Node& lhs, const Node& rhs)
	{//We need to overload "<" to put our struct into a set
		return lhs.fCost < rhs.fCost;
	}

	static bool isValid(int x, int y, vector<vector<char>> obstacles);
	static bool isDestination(int x, int y, Node dest);
	static double calculateH(int x, int y, Node dest);
public:
	AStar();
	~AStar();

	static vector<Node> aStar(Node player, Node dest, vector<vector<char>> obstacles);
	static std::vector<Node> makePath(array<array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map, Node dest);
};

