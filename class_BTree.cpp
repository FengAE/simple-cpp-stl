#include<iostream>
using namespace std;

template<class K, size_t M> // key, M
struct BTreeNode
{
    K _keys[M]; // original: M-1
    BTreeNode* _subs[M+1];
    BTreeNode* _parent;
    size_t _n;  // 实际存储的关键字个数

    BTreeNode(size_t n = 0)
    :_n(n)
    {
        for(size_t i=0; i<M; i++)
        {
            _keys[i] = K(); // 缺省值
            _subs[i] = nullptr;
        }
        _subs[M] = _parent = nullptr;
    }
};

template<class K, size_t M>
class BTree
{
    typedef BTreeNode<K, M> Node;
public:
    pair<Node*, int> Find(const K& key)
    {
        Node* cur = _root;
        Node* prev = nullptr;
        while(cur)
        {
            size_t i=0;
            for(; i<cur->_n; i++)
            {
                if(cur->_keys[i] == key)
                    return make_pair(cur, i);
                else if(cur->_keys[i] > key)
                    break;
            }
            prev = cur;
            cur = cur->_subs[i];
        }
        return make_pair(prev, -1); // 未找到
    }

    void InsertKey(Node* cur, const K& key, Node* child)  
    {
        int i;
        for(i=cur->_n-1; i>=0 && cur->_keys[i]>key; i--)
        {
            cur->_keys[i+1] = cur->_keys[i];
            cur->_subs[i+2] = cur->_subs[i+1];
        }
        cur->_keys[i+1] = key;
        cur->_subs[i+2] = child;
        if(child)
            child->_parent = cur;
        cur->_n++;
    }

    bool Insert(const K& key)
    {
        pair<Node*, int> ret = Find(key);
        if(ret.second >= 0) 
            return false;
        // 未找到，需要插入
        Node* cur = ret.first;
        Node* child = nullptr;
        size_t newkey = key;

        // 初始：cur == _root为空
        if(cur == nullptr)
        {
            _root = new Node(1);
            _root->_keys[0] = newkey;
            return true;
        }

        while(1)
        {
            InsertKey(cur, newkey, child);
            // 1. 未满，直接结束
            if(cur->_n < M) 
                return true;
            // 2. 已满，可能连续分裂！
            else    
            {
                size_t mid = M/2;
                // 分裂[mid+1, M-1]给兄弟[0, M-mid-2]
                Node* brother = new Node(M-mid-1);
                size_t i, j;
                for(i=0, j=mid+1; j<M; j++)
                {
                    brother->_keys[i] = cur->_keys[j];
                    // 记得孩子也要拷贝
                    brother->_subs[i] = cur->_subs[j];
                    if(cur->_subs[j])
                        cur->_subs[j]->_parent = brother;
                    cur->_subs[j] = nullptr;    // 清理原来孩子
                }
                // 拷最后一个孩子
                brother->_subs[i] = cur->_subs[j];
                if(cur->_subs[j])
                    cur->_subs[j]->_parent = brother;
                cur->_subs[j] = nullptr;

                cur->_n = mid;

                // 将mid给parent
                newkey = cur->_keys[mid];
                Node* parent = cur->_parent;
                if(!parent)     // cur已经到第一层
                {
                    _root = new Node(1);
                    _root->_keys[0] = newkey;
                    _root->_subs[0] = cur, _root->_subs[1] = brother;
                    cur->_parent = brother->_parent = _root;
                    break;
                }
                // 需要插入parent，直接再次循环处理
                else
                {
                    child = brother;
                    cur = parent;
                }
            }
        }
        return true;
    }

    void InOrder()
    {
        _InOrder(_root);
    }
    void _InOrder(Node* root)
    {
        if(!root)   return;
        size_t i;
        for(i=0; i<root->_n; i++)
        {
            _InOrder(root->_subs[i]);
            cout<<root->_keys[i]<<" ";
        }
        _InOrder(root->_subs[i]);
    }

private:
    Node* _root = nullptr;
};

int main()
{
    int a[] = {53, 139, 75, 49, 82,12, 34,50};
    BTree<int, 3> t;
    for(auto e:a)
    {
        t.Insert(e);
    }
    t.InOrder();
    return 0;
}