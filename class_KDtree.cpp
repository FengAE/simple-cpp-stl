#include<iostream>
#include<vector>
#include<stack>
#include<algorithm>
#include<math.h>
#include<assert.h>
#include<queue>

using namespace std;

struct Point {
    vector<double> coords;
    int id;
};

struct KDNode {
    Point point;          // 当前节点存储的那个多维点（作为切分点）
    int axis;             // 切分轴/维度。例如 0代表X轴，1代表Y轴，2代表Z轴
    KDNode* left = nullptr;  
    KDNode* right = nullptr; 

    KDNode(Point p, int a) : point(p), axis(a) {}
    ~KDNode() { delete left; delete right; }
};

struct BBFCandidate {
    KDNode* node;
    double dist_to_plane;

    // 距离超平面距离作为优先级
    bool operator>(const BBFCandidate& other) const {
        return dist_to_plane > other.dist_to_plane;
    }
};

struct KNNItem {
    double dist;
    Point point;
};

struct KNNItemLess {
    bool operator()(const KNNItem& a, const KNNItem& b) const {
        return a.dist < b.dist;
    }
};

class KDTree {
private:
    KDNode* root = nullptr;
    int k;

    KDNode* buildRecursive(vector<Point>& points, int axis, int left, int right)
    {
        if(left > right)  return nullptr;
        int mid_idx = left + (right-left)/2;
        // 执行后，nums[mid_idx] 处就是正确的中位数
        nth_element(points.begin()+left, points.begin() + mid_idx, 
                    points.begin()+right+1,
                [axis](const Point& a, const Point& b) {
                    return a.coords[axis] < b.coords[axis];
                });

        // parent应该使用new创建，否则在栈创建的话，出函数就销毁了
        KDNode* parent = new KDNode(points[mid_idx], axis);
        parent->left = buildRecursive(points, (axis+1)%k, left, mid_idx-1);
        parent->right = buildRecursive(points, (axis+1)%k, mid_idx+1, right);
        return parent;
    }

    void Lookdown(Point& target, KDNode* cur, stack<KDNode*>& st)
    {
        if(cur == nullptr)  return;
        st.push(cur);
        int axis = cur->axis;

        if(cur->point.coords[axis] > target.coords[axis])
            Lookdown(target, cur->left, st);
        else
            Lookdown(target, cur->right, st);
    }

    double GetDist(Point& a, Point& b)
    {
        double ret = 0;
        for(int i=0; i<k; i++)
        {
            ret += (a.coords[i]-b.coords[i])*(a.coords[i]-b.coords[i]);
        }
        return sqrt(ret);
    }

    KDNode* _FindMinDist(Point& p, KDNode* _root)
    {
        if(_root == nullptr)    return nullptr;
        // 1. 向下查找
        stack<KDNode*> st;
        Lookdown(p, _root, st);

        // 2. 回溯
        KDNode* cur_ret = st.top();
        double cur_dist = GetDist(p, cur_ret->point);
        st.pop();

        while(!st.empty())
        {
            KDNode* pivort = st.top();
            st.pop();
            int axis = pivort->axis;
            // 比较当前节点自身
            double piv_dist = GetDist(p, pivort->point);
            if(piv_dist < cur_dist){ cur_dist = piv_dist; cur_ret = pivort; }
            double dist = abs(pivort->point.coords[axis] - p.coords[axis]);
            if(dist < cur_dist) // 圆和超平面有交点，则需要查找另外一个孩子
            {
                KDNode* tmp = nullptr;
                if(p.coords[axis] < pivort->point.coords[axis])
                    tmp = _FindMinDist(p, pivort->right);
                else
                    tmp = _FindMinDist(p, pivort->left);

                if(tmp != nullptr)
                {
                    double tmp_dist = GetDist(p, tmp->point);
                    if(tmp_dist < cur_dist)
                    {
                        cur_dist = tmp_dist;
                        cur_ret = tmp;
                    }
                }
            }
        }
        return cur_ret;
    }

public:
    KDTree(int dimensions) : k(dimensions) {}

    void build(vector<Point>& points) {
        for(auto& e:points)
            assert(e.coords.size() == (size_t)k);
        root = buildRecursive(points, 0, 0, points.size()-1);
    }

    Point FindMinDist(vector<double>& coords)
    {
        Point p = {coords, 0};
        return _FindMinDist(p, root)->point;
    }

    vector<Point> FindKNN_BBF(vector<double>& coords, int max_search_steps=100, int m=1)
    {
        Point target = {coords, -1};
        if (root == nullptr) return {};

        priority_queue<BBFCandidate, vector<BBFCandidate>, greater<BBFCandidate>> pq;
        priority_queue<KNNItem, vector<KNNItem>, KNNItemLess> knn; // 大顶堆，维护当前最好的m个邻居

        vector<Point> ret;
        KDNode* cur = root;
        int steps = 0;
        while ((cur != nullptr || !pq.empty()) && steps < max_search_steps)
        {
            // 1. 一路向下探底，等价于上面的Lookdown
            while (cur != nullptr)
            {
                steps++;
                double d = GetDist(target, cur->point);
                if (knn.size() < static_cast<size_t>(m)) {
                    knn.push({d, cur->point});
                } else if (d < knn.top().dist) {
                    knn.pop();
                    knn.push({d, cur->point});
                }

                int axis = cur->axis;
                double plane_dist = target.coords[axis] - cur->point.coords[axis];

                if (plane_dist < 0) {
                    if (cur->right)
                        pq.push({cur->right, abs(plane_dist)});
                    cur = cur->left;
                } else {
                    if (cur->left)
                        pq.push({cur->left, abs(plane_dist)});
                    cur = cur->right;
                }
            }

            // 2. 探底结束，从队列中挑出离超平面最近的兄弟子树
            if (!pq.empty())
            {
                BBFCandidate candidate = pq.top();
                pq.pop();

                double worst_dist = knn.size() < static_cast<size_t>(m) ? INFINITY : knn.top().dist;
                if (candidate.dist_to_plane >= worst_dist)
                    break; 

                cur = candidate.node;
            }
        }
        size_t idx = 0;
        while(!knn.empty())
        {
            cout<<"KNN candidate "<<idx<<": id="<<knn.top().point.id<<", dist="<<knn.top().dist<<endl;
            ret.push_back(knn.top().point);
            knn.pop();
            idx++;
        }
        return ret;
    }

};

int main()
{
    KDTree tree(2);
    vector<Point> arr = {{{2,3}, 0},
                        {{5,4}, 1},
                        {{4,7}, 2},
                        {{7,2}, 3},
                        {{9,6}, 4},
                        {{8,1}, 5}};
    tree.build(arr);
    vector<double> coords = {2,4.5};

    int min_id = -1;
    double min_dist = 1e9;
    for(size_t i=0; i<arr.size(); i++)
    {
        double dist = 0;
        for(int j=0; j<2; j++)
            dist += (arr[i].coords[j]-coords[j])*(arr[i].coords[j]-coords[j]);
        dist = sqrt(dist);
        if(dist < min_dist)
        {
            min_dist = dist;
            min_id = arr[i].id;
        }
    }
    cout<<"Brute-force result: "<<min_id<<endl;

    Point p = tree.FindMinDist(coords);
    cout<<"KD Tree result: "<<p.id<<endl;
    for(auto e:tree.FindKNN_BBF(coords, 1000, 4))
        cout<<"BBF result: "<<e.id<<endl;
    return 0;
}