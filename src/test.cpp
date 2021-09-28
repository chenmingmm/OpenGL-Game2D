#include<iostream>
#include<time.h>
#include<map>
#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>
#include <queue>
#include<unordered_set>
#include<unordered_map>
#include<set>
#include<list>
#include<algorithm>
#include<windows.h>
using namespace std;

class Solution {

    unordered_map<int, set<int>> site2Routes;
    vector<bool> visitedRoute {false};
    void CreateView(const vector<vector<int>> &routes)
    {     
        for (int i = 0; i < routes.size(); i++)
        {
            for (auto &iter : routes[i])
            {
                site2Routes[iter].insert(i);
            }
        }
    }

    int update(queue<int> &cur, unordered_map<int,int> &myvisited, unordered_map<int,int>&other,
                const vector<vector<int>> &routes)
    {
        auto front = cur.front();
        cur.pop();

        for (auto &route : site2Routes[front])
        {
            if (visitedRoute[route])
                continue;
            visitedRoute[route] = true;
            for (auto &site : routes[route]){
                if (!myvisited.count(site))
                {
                    if (other.count(site)) {
                        return other[site] + myvisited[front] - 1; 
                    }
                    myvisited[site] = myvisited[front] + 1;
                    cur.push(site);
                }   
            }
        }
        return -1;
    }

public:
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        if (source == target) {
            return 0;   
        }
        CreateView(routes);  
        queue<int> q;
        queue<int> q1;
        q.emplace(source);
        q1.emplace(target);

        unordered_map<int,int> visited_head;
        unordered_map<int,int> visited_end;
        visited_head.emplace(source,1);
        visited_end.emplace(target,1);

        while (!q.empty() && !q1.empty())
        {
            int ret = 0;
            if (q.size() < q1.size())
            {
                ret = update(q, visited_head, visited_end, routes);
            } else
            {
                ret = update(q1,visited_end, visited_head, routes);
            }
            if (ret != -1) {
                return ret;
            }
        }
        return -1;
    }
};


string GetString(int i)
{
    string ret;
    while (i > 0)
    {
        i--;
        ret = char('A'+ (i % 26)) + ret;
        i = i / 26;
    }
    return ret;
       
}

class Solution2 {
public:
    int maxIceCream(vector<int> costs, int coins) {

        sort(costs.begin(), costs.end(), less<int>());

        int sum = 0;
        for (int i = 0; i < costs.size(); i++) {
            sum+=costs[i];
            if (coins < sum) {
                return i;
            }
        }
        return costs.size();
    }
};

class Solution3 {
    unordered_set<string> wordList_set;

    void GetTransFromOnce(vector<string> &reslut, std::string &world)
    {
        reslut.clear();
        int length = world.size();
        std::string temp;
        for (int i = 0; i < length; i++)
        {
            temp = world;
            for (int j = 0; j < 26; j++)
            {
                temp[i] = 'a' + j;
                if (temp != world && wordList_set.find(temp) != wordList_set.end()) {
                    reslut.push_back(temp);
                }
            }
        }
    }

    int update(queue<string> &q, unordered_map<string,int> &cur, unordered_map<string,int> &other)
    {
        auto front = q.front();
        q.pop();
        vector<string> worldlst;
        GetTransFromOnce(worldlst, front);
        for (auto iter :worldlst) {

            if (cur.count(iter)) {
                continue;
            }
            if (other.count(iter)) {
                return other[iter] + cur[front];
            }
            q.push(iter);
            cur[iter] = cur[front] + 1;
        }
        return -1;
    }
public:
    int ladderLength(string beginWord, string endWord, vector<string> wordList) {
        wordList_set.insert(wordList.begin(), wordList.end());
        if (wordList_set.find(endWord) == wordList_set.end()) {
            return 0;
        }

        queue<string> qs;
        queue<string> qe;
        qs.push(beginWord);
        qe.push(endWord);

        string startWorld;
        unordered_map<string,int> visitedWorldS;
        visitedWorldS.emplace(beginWord, 1);
        unordered_map<string,int> visitedWorldE;
        visitedWorldE.emplace(endWord, 1);
        while (!qs.empty() && !qe.empty())
        {
            int nums = 0;
            if (qs.size() <= qe.size()) {
                nums = update(qs, visitedWorldS, visitedWorldE);
            } else {
                nums = update(qe, visitedWorldE, visitedWorldS);
            }
            if (nums != -1) {
                return nums;
            }
        }
        return 0;
    }
};

unordered_set<int> err {4,5};
bool isBadVersion(int n)
{
    return err.count(n);
}
class Solution4 {
public:
    int searchInsert(vector<int> nums, int target) {
        bool up = nums[0] > nums[1];
        int left = 0;
        int right = nums.size() - 1;
        int mid = 0;
        while (left <= right)
        {
            mid = left + (right - left) /2 ;
            if (nums[mid] == target){
                return mid;
            } else if (nums[mid] > target ){
                up ? (left = mid + 1) : (right = mid - 1 );
            } else {
                up ? (right = mid - 1) : (left = mid + 1); 
            }
        }
        return left;
    }
};

class Solution5 {
public:
    int eliminateMaximum(vector<int> dist, vector<int> speed) {


        priority_queue<int, vector<int>,greater<int> > q;
        for (int i = 0; i < dist.size(); i++)
        {
            if (dist[i] % speed[i])
            {
                q.push(dist[i] / speed[i] + 1);
            } else {
                 q.push(dist[i] / speed[i]);
            } 
        }
        int ans = 0;
        int time = 0;
        while (!q.empty())
        {
            auto front = q.top();
            if (time >= front ){
                break;
            }
            q.pop();
            ans++;
            time++;
        }
        return ans;
    }
};

class Solution6 {
public:
    vector<vector<string>> displayTable(vector<vector<string>> orders) {
       
        map<int, map<string,int>> hasmap;
        set<string> foods;
        for (auto &order : orders) {
            string num = order[1];
            string food = order[2];
            foods.insert(food);
            hasmap[atoi(num.c_str())][food]++;
        }
    
        for (auto keyValue : hasmap) {

            cout << keyValue.first << ":";
            for (auto iter : keyValue.second) {

                cout << iter.first <<":" << iter.second;
            }
            cout << endl;
        }


        vector<vector<string>> ret(hasmap.size() + 1);
        ret[0].push_back("table");
        for (auto &food : foods) {
            ret[0].push_back(food);
        }
        int index = 1;
        for (auto &keyValue : hasmap) 
        {
            ret[index].push_back(to_string(keyValue.first));
            for (auto &food : foods) {
                auto iter = keyValue.second.find(food);
                int value = iter == keyValue.second.end() ? 0 : iter->second;
                ret[index].push_back(to_string(value));
            }
            ++index;
        }
        ofstream of;
        of.open("reslut.txt");
        for (auto &iter : ret) {           
            for (auto &n : iter) {
                of << n << "    ";
            }
            of <<endl;
        }
        of.close();
        return ret;
    }
};
class Solution7 {


    void bfs(vector<vector<int>> &grid, int &area, int row, int col)
    {
        if (row < 0 || row > grid.size() - 1 || col < 0 || col > grid[row].size() -1 ) 
        {
            return;
        }
        if (grid[row][col] == 0) {
            return;
        }
        grid[row][col] = 0;
        ++area;

        bfs(grid, area,row + 1, col);
        bfs(grid, area,row - 1, col);
        bfs(grid, area,row, col + 1);
        bfs(grid, area,row, col - 1);
    }
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {

        int row = grid.size();
        int maxArea = 0;
        int temp = 0;
        for (int i = 0; i < row; i++) {

            for (int j = 0; j < grid[i].size(); j++) {

                if (grid[i][j] == 1) {
                    temp = 0;
                    bfs(grid, temp, i, j);
                    if (temp > maxArea) {
                        maxArea = temp;
                    }
                   
                }
            }
        }
        return maxArea;
    }
};


template<class T, class... Args>
void get_params(int params[], int idx, T head, Args... rest)
{
	params[idx] = head;
	if (sizeof...(rest) == 0) return;
	get_params(params, idx + 1, rest...);//报错，“应输入4个参数，却提供了2个”
}

int* changeArrary(int a[], int n)
{
    int b[5];
    for (int i = 0; i < n; i++) {
        a[i] = i;
        b[i] = i;
    }

    return b;
}

struct A
{
    char * ch;
    int a;
};

int getA(A &temp)
{
    if (temp.ch){
        free(temp.ch);
    }
}

void findStr(std::string str)
{
    static set<string> target {"LINKING_FAILED"};

    str.find(target.begin()->c_str());
}

int main(int argn, char* args[])
{
    // cout << args[1] << endl;
    // if (argn < 2){
    //     cout << "need a param"<< endl;
    // }
    
    // ifstream of;
    // of.open(args[1]);
    // if (!of.is_open()) {
    //     cout << "open file failed" << endl;
    // }
    // //读取文件
    // stringstream buf;
    // buf << of.rdbuf();
    // cout << buf.str();
    fstream of;   
    of.open("erro.txt",ios::in|ios::out);
    of.seekg(0,ios::beg);
    stringstream buf;
    buf << of.rdbuf();
    string buffstr = buf.str();

    auto curpos = string::npos;
    while ((curpos = buffstr.find("LINKING_FAILED", curpos + 1)) != string::npos)
    {
        //of.seekp(curpos + sizeof("LINKING_FAILED") - 1, ios::beg);
        //of << " /data/valgrind/bin/valgrind bin";
        buffstr.insert(curpos + sizeof("LINKING_FAILED") - 1," /data/valgrind/bin/valgrind bin");
    }
    of.seekp(0,ios::beg);
    of << buffstr;
    of.close();

    return 1;
}



 