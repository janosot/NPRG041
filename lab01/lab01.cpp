#include <iostream> 
#include <string>
#include <vector>

using namespace std;

class Multiply
{
public:
	void BasicMultiplier(vector<string>& arg);
	void ImprovedMultiplier(vector<string>& arg);
};

void Multiply::BasicMultiplier(vector<string>& arg)
{
	int to = 10;
	for (int i = 1; i < arg.size(); i++)
	{
		for (int j = 1; j <= to; j++) {
			cout << j << " * " << arg[i] << " = " << j * stoi(arg[i]) << endl;
		}
		cout << endl;
	}
}

void Multiply::ImprovedMultiplier(vector<string>& arg)
{
	int from = 0;
	int to = 0;
	int using_parameters = 5;
	if (arg.size() == 1)
	{
		exit(1);
	}
	if (arg[1] == "-f" && arg[3] == "-t")
	{
		from = stoi(arg[2]);
		to = stoi(arg[4]);
	}
	if (from == 0 && to == 0)
	{
		BasicMultiplier(arg);
	}
	else
	{
		for (int i = using_parameters; i < arg.size(); i++)
		{
			for (int j = from; j <= to; j++)
			{
				if (j * stoi(arg[i]) >= from)
				{
					cout << j << " * " << arg[i] << " = " << j * stoi(arg[i]) << endl;
				}
			}
			cout << endl;
		}
	}
}

int main(int argc, char** argv)
{
	vector<string> arg(argv, argv + argc);
	Multiply m;
	//m.BasicMultiplier(arg);
	m.ImprovedMultiplier(arg);
	return 0;
}
