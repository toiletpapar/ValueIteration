#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

pair<string, double> max(vector<pair<string,double> > vector_to_maximize) {
	if (vector_to_maximize.empty()) {
		return pair<string, double>("empty", 0.0);
	}
	else {
		pair<string,double> max = vector_to_maximize.at(0);

		for (unsigned int i = 1; i < vector_to_maximize.size(); ++i) {
			if (max.second < vector_to_maximize.at(i).second) {
				max = vector_to_maximize.at(i);
			}
		}

		return max;
	}
}

int main() {
	double r = 0;
	double discount_factor = 0.9;
	map<string, double> Table;
	map<string, double> Updated_Table;
	map<string, string> Policy;

	//Initialize the tables
	Table.insert(pair<string, double>("top-left", r));
	Table.insert(pair<string, double>("top", -1));
	Table.insert(pair<string, double>("top-right", 10));
	Table.insert(pair<string, double>("centre-left", -1));
	Table.insert(pair<string, double>("centre", -1));
	Table.insert(pair<string, double>("centre-right", -1));
	Table.insert(pair<string, double>("bottom-left", -1));
	Table.insert(pair<string, double>("bottom", -1));
	Table.insert(pair<string, double>("bottom-right", -1));

	//Do value iteration 25 times
	for (int i = 1; i < 25; ++i) {
		//The grid
		//top-left    |  top   | top-right
		//centre-left | centre | centre-right
		//bottom-left | bottom | bottom-right

		//Actions: up, down, left, right

		//S1: TOP-LEFT
		//Top-left: right
		pair<string, double> tl_right = pair<string, double>("right", 0.1 * Table.at("top-left") + 0.1 * Table.at("centre-left") + 0.8 * Table.at("top"));

		//Top-left: down
		pair<string, double> tl_down = pair<string, double>("down", 0.1 * Table.at("top-left") + 0.1 * Table.at("top") + 0.8 * Table.at("centre-left"));

		//Calculate the expected discounted sum of rewards and policy
		pair<string, double> tl_policy = max(vector<pair<string, double> >({ tl_right, tl_down }));
		
		Policy["top-left"] = tl_policy.first;									//Policy
		Updated_Table["top-left"] = r + discount_factor * tl_policy.second;		//Expected Discounted Sum of Rewards

		//S2: TOP
		//Top: left
		pair<string, double> t_left = pair<string, double>("left", 0.8 * Table.at("top-left") + 0.1 * Table.at("centre") + 0.1 * Table.at("top"));
		//Top: right
		pair<string, double> t_right = pair<string, double>("right", 0.8 * Table.at("top-right") + 0.1 * Table.at("centre") + 0.1 * Table.at("top"));
		//Top: down
		pair<string, double> t_down = pair<string, double>("down", 0.8 * Table.at("centre") + 0.1 * Table.at("top-left") + 0.1 * Table.at("top-right"));

		pair<string, double> t_policy = max(vector<pair<string, double> >({ t_left, t_right, t_down }));
		Policy["top"] = t_policy.first;
		Updated_Table["top"] = -1 + discount_factor * t_policy.second;

		//S3: TOP-RIGHT - Terminating state
		Updated_Table["top-right"] = 10;

		//S4: CENTRE-LEFT
		//Centre-left: up
		pair<string, double> cl_up = pair<string, double>("up", 0.8 * Table.at("top-left") + 0.1 * Table.at("centre") + 0.1 * Table.at("centre-left"));
		//Centre-left: right
		pair<string, double> cl_right = pair<string, double>("right", 0.8 * Table.at("centre") + 0.1 * Table.at("top-left") + 0.1 * Table.at("bottom-left"));
		//Centre-left: down
		pair<string, double> cl_down = pair<string, double>("down", 0.8 * Table.at("bottom-left") + 0.1 * Table.at("centre") + 0.1 * Table.at("centre-left"));

		pair<string, double> cl_policy = max(vector<pair<string, double> >({ cl_up, cl_right, cl_down }));
		Policy["centre-left"] = cl_policy.first;
		Updated_Table["centre-left"] = -1 + discount_factor * cl_policy.second;

		//S5: CENTRE
		pair<string, double> c_up = pair<string, double>("up", 0.8 * Table.at("top") + 0.1 * Table.at("centre-left") + 0.1 * Table.at("centre-right"));
		pair<string, double> c_right = pair<string, double>("right", 0.8 * Table.at("centre-right") + 0.1 * Table.at("top") + 0.1 * Table.at("bottom"));
		pair<string, double> c_down = pair<string, double>("down", 0.8 * Table.at("bottom") + 0.1 * Table.at("centre-left") + 0.1 * Table.at("centre-right"));
		pair<string, double> c_left = pair<string, double>("left", 0.8 * Table.at("centre-left") + 0.1 * Table.at("top") + 0.1 * Table.at("bottom"));

		pair<string, double> c_policy = max(vector<pair<string, double> >({ c_up, c_right, c_down, c_left }));
		Policy["centre"] = c_policy.first;
		Updated_Table["centre"] = -1 + discount_factor * c_policy.second;

		//S6: CENTRE-RIGHT
		pair<string, double> cr_up = pair<string, double>("up", 0.8 * Table.at("top-right") + 0.1 * Table.at("centre") + 0.1 * Table.at("centre-right"));
		pair<string, double> cr_left = pair<string, double>("left", 0.8 * Table.at("centre") + 0.1 * Table.at("top-right") + 0.1 * Table.at("bottom-right"));
		pair<string, double> cr_down = pair<string, double>("down", 0.8 * Table.at("bottom-right") + 0.1 * Table.at("centre") + 0.1 * Table.at("centre-right"));

		pair<string, double> cr_policy = max(vector<pair<string, double> >({ cr_up, cr_left, cr_down }));
		Policy["centre-right"] = cr_policy.first;
		Updated_Table["centre-right"] = -1 + discount_factor * cr_policy.second;

		//S7: BOTTOM-LEFT
		pair<string, double> bl_up = pair<string, double>("up", 0.8 * Table.at("centre-left") + 0.1 * Table.at("bottom-left") + 0.1 * Table.at("bottom"));
		pair<string, double> bl_right = pair<string, double>("right", 0.8 * Table.at("bottom") + 0.1 * Table.at("bottom-left") + 0.1 * Table.at("centre-left"));

		pair<string, double> bl_policy = max(vector<pair<string, double> >({ bl_up, bl_right }));
		Policy["bottom-left"] = bl_policy.first;
		Updated_Table["bottom-left"] = -1 + discount_factor * bl_policy.second;

		//S8: BOTTOM
		pair<string, double> b_up = pair<string, double>("up", 0.8 * Table.at("centre") + 0.1 * Table.at("bottom-left") + 0.1 * Table.at("bottom-right"));
		pair<string, double> b_right = pair<string, double>("right", 0.8 * Table.at("bottom-right") + 0.1 * Table.at("centre") + 0.1 * Table.at("bottom"));
		pair<string, double> b_left = pair<string, double>("left", 0.8 * Table.at("bottom-left") + 0.1 * Table.at("centre") + 0.1 * Table.at("bottom"));

		pair<string, double> b_policy = max(vector<pair<string, double> >({ b_up, b_right, b_left }));
		Policy["bottom"] = b_policy.first;
		Updated_Table["bottom"] = -1 + discount_factor * b_policy.second;

		//S9: BOTTOM-RIGHT
		pair<string, double> br_up = pair<string, double>("up", 0.8 * Table.at("centre-right") + 0.1 * Table.at("bottom-right") + 0.1 * Table.at("bottom"));
		pair<string, double> br_left = pair<string, double>("left", 0.8 * Table.at("bottom") + 0.1 * Table.at("centre-right") + 0.1 * Table.at("bottom-right"));

		pair<string, double> br_policy = max(vector<pair<string, double> >({ br_up, br_left }));
		Policy["bottom-right"] = br_policy.first;
		Updated_Table["bottom-right"] = -1 + discount_factor * br_policy.second;

		//Copy the updated table to the original table
		Table = Updated_Table;
	}

	//Print the policy and expected value
	cout << "Policy(top-left): " << Policy.at("top-left") << " Expected Value(top-left): " << Table.at("top-left") << "\n";
	cout << "Policy(top): " << Policy.at("top") << " Expected Value(top): " << Table.at("top") << "\n";
	cout << "Policy(top-right): " << "N/A" << " Expected Value(top-right): " << Table.at("top-right") << "\n";
	cout << "Policy(centre-left): " << Policy.at("centre-left") << " Expected Value(centre-left): " << Table.at("centre-left") << "\n";
	cout << "Policy(centre): " << Policy.at("centre") << " Expected Value(centre): " << Table.at("centre") << "\n";
	cout << "Policy(centre-right): " << Policy.at("centre-right") << " Expected Value(centre-right): " << Table.at("centre-right") << "\n";
	cout << "Policy(bottom-left): " << Policy.at("bottom-left") << " Expected Value(bottom-left): " << Table.at("bottom-left") << "\n";
	cout << "Policy(bottom): " << Policy.at("bottom") << " Expected Value(bottom): " << Table.at("bottom") << "\n";
	cout << "Policy(bottom-right): " << Policy.at("bottom-right") << " Expected Value(bottom-right): " << Table.at("bottom-right") << endl;

	return 0;
}