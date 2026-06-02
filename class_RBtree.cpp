#include<iostream>
#include <chrono>
#include <vector>
#include <random>
using namespace std;

enum Color
{
    BLACK,
    RED
};

template<class K, class V>
struct RBtreeNode
{
    Color _color;
    struct RBtreeNode<K, V>* _left;
    struct RBtreeNode<K, V>* _right;
    struct RBtreeNode<K, V>* _parent;

    pair<K, V> _kv;

    RBtreeNode(const pair<K, V>& kv)
    :_color(RED),     // 默认插入红的
    _left(nullptr),_right(nullptr),_parent(nullptr),
    _kv(kv) {}
};

template<class K, class V>
class RBtree
{
    typedef struct RBtreeNode<K, V> Node;
public:
    RBtree()
        :_root(nullptr) {}
    RBtree(const pair<K, V> kv)
        :_root(new RBtreeNode(kv)) {}

    bool Insert(const pair<K, V> kv)
    {
        if(_root == nullptr)
        {
            _root = new Node(kv);
            _root->_color = BLACK;
            return true;
        }

        Node* cur = _root;
        Node* parent = nullptr;
        while(cur)
        {           
            if(cur->_kv.first > kv.first)
            {
                parent = cur;
                cur = cur->_left;
            }
            else if(cur->_kv.first < kv.first)
            {
                parent = cur;
                cur = cur->_right;
            }
            else
                return false;
        }

        cur = new Node(kv);
        cur->_parent = parent;
        if(parent->_kv.first > kv.first)
            parent->_left = cur;
        else
            parent->_right = cur;
             
        while(parent && parent->_color == RED)  
        {
            Node* grandfather = parent->_parent;
            if(!grandfather)    break;
            Node* uncle;
            if(parent == grandfather->_left)
                uncle = grandfather->_right;
            else
                uncle = grandfather->_left;
            
            // 情况一
            if(uncle && uncle->_color == RED)   
            {
                parent->_color = uncle->_color = BLACK;
                grandfather->_color = RED;
                if(grandfather == _root)
                {
                    grandfather->_color = BLACK;
                    break;
                }
                // 更新cur=grandfather, parent=ancestor
                Node* ancestor = grandfather->_parent;  
                parent = ancestor;
                cur = grandfather;
            }
            else 
            {
                // 情况二，cur在外侧 
                if(cur == parent->_left && parent == grandfather->_left)
                {
                    RotateR(grandfather);
                    parent->_color = BLACK;
                    grandfather->_color = RED;
                }
                else if(cur == parent->_right && parent == grandfather->_right)
                {
                    RotateL(grandfather);
                    parent->_color = BLACK;
                    grandfather->_color = RED;
                }
                // 情况三，cur在内侧
                else if(cur == parent->_left && parent == grandfather->_right)
                {
                    RotateRL(grandfather);
                    cur->_color = BLACK;
                    grandfather->_color = RED;
                }
                else
                {
                    RotateLR(grandfather);
                    cur->_color = BLACK;
                    grandfather->_color = RED;        
                }
            }
        }
        _root->_color = BLACK;  // 统一设置root颜色，反正不影响路径黑节点
        return true;
    }

    void RotateR(Node* parent)
    {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        Node* ppNode = parent->_parent;
        parent->_left = subLR;
        if(subLR)
            subLR->_parent = parent;

        subL->_right = parent;
        parent->_parent = subL;
        if(parent == _root)
            _root = subL;
        else
        {
            if(ppNode->_left == parent)
                ppNode->_left = subL;
            else
                ppNode->_right = subL;
        }
        subL->_parent = ppNode;
    }

    void RotateL(Node* parent)
    {
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        Node* ppNode = parent->_parent;
        parent->_right = subRL;
        if(subRL)
            subRL->_parent = parent;
        
        subR->_left = parent;
        parent->_parent = subR;
        if(parent == _root)
            subR = _root;
        else
        {
            if(ppNode->_left == parent)
                ppNode->_left = subR;
            else
                ppNode->_right = subR;
        }
        subR->_parent = ppNode;
    }
    
    void RotateLR(Node* parent)
    {
        Node* subL = parent->_left;
        RotateL(subL);
        RotateR(parent);
    }

    void RotateRL(Node* parent)
    {
        Node* subR = parent->_right;
        RotateR(subR); 
        RotateL(parent);
    }

    void InOrder()
    {
        _InOrder(_root);
        cout<<endl;
    }
    void _InOrder(Node* root)
    {
        if(!root)   return;
        _InOrder(root->_left);
        cout<<root->_kv.first<<":"<<root->_kv.second<<" ";
        _InOrder(root->_right);
    }

    void Check()
    {
        if(Check_Color(_root) & Check_Length())
            cout<<"yes"<<endl;
    }

    bool Check_Color(Node* root)  // 检查没有相邻的红色
    {
        if(!root)   return true;
        if(root->_color == RED && root->_parent->_color == RED)
        {
            cout<<"Consistent red node"<<endl;
            return false;
        }
        return Check_Color(root->_left) & Check_Color(root->_right);
    }

    bool Check_Length()   // 检查叶子路径黑节点个数相等
    {
        int num = GetBlackNum();
        return _Check_Length(_root, num, 0);
    }
    bool _Check_Length(Node* root, int num, int cur_cnt)
    {
        if(!root)   // 叶子
        {
            if(cur_cnt+1 != num)
            {
                cout<<"Black node num not equal"<<endl;
                return false;
            }
            else    return true;
        }
        if(root->_color == BLACK)
            cur_cnt++;
        return _Check_Length(root->_left, num, cur_cnt) 
                & _Check_Length(root->_right, num, cur_cnt);
    }

    int GetBlackNum()
    {
        int cnt = 0;
        Node* cur;
        for(cur = _root; cur; cur = cur->_left)
        {
            if(cur->_color ==  BLACK)   cnt++;
        }
        return cnt+1;   // 包括nullptr的黑色
    }

private:
    Node* _root;
};

int main()
{
    RBtree<int, int> t;
    const int N = 100000;
    vector<int> keys(N);
    random_device rd;
    mt19937 gen(rd());
    for (int i = 0; i < N; ++i) {
        keys[i] = gen();
    }
    // 计时开始
    auto start = chrono::high_resolution_clock::now();
    // 构建红黑树
    for (int key : keys) {
        t.Insert({key, key});
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    // 验证红黑树性质
    t.Check();
    // 输出结果
    cout << duration << " ms" << endl;
    return 0;
}