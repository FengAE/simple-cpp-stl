#include<iostream>
using namespace std;

template<class K, class V>
struct BSnode
{
    K _key;
    V _val;
    BSnode<K, V>* _left;
    BSnode<K, V>* _right;

    BSnode(const K& key, const V& val)
    :_key(key), _val(val), _left(nullptr), _right(nullptr) {}
};

template<class K, class V>
class BStree
{
    typedef BSnode<K, V> Node;

public:
    BStree(Node* root = nullptr)
    :_root(root) {}

    bool Insert(const K& key, const V& val)   // 若有插入则返回true
    {
        if(_root == nullptr)
        {
            _root = new Node(key, val);
            return true;
        }
        Node* parent = nullptr;
        Node* cur = _root;
        while(cur != nullptr)
        {
            if(cur->_key == key)
                return false;
            else if(cur->_key > key)
            {
                parent = cur;
                cur = cur->_left;
            }
            else
            {
                parent = cur;
                cur = cur->_right;
            }
        }
        cur = new Node(key, val);
        if(parent->_key < key)
            parent->_right = cur;
        else
            parent->_left = cur;
        return true;
    }

    Node* find(const K& key)
    {
        Node* cur = _root;
        while(cur)
        {
            if(cur->_key < key)
                cur = cur->_right;
            else if(cur->_key > key)
                cur = cur->_left;
            else
                return cur;
        }
        return nullptr;
    }

    bool Erase(const K& key)    // 有删除返回true
    {
        Node* parent = nullptr;
        Node* cur = _root;
        while(cur != nullptr)
        {
            if(cur->_key > key)
            {
                parent = cur;
                cur = cur->_left;
            }
            else if(cur->_key < key)
            {
                parent = cur;
                cur = cur->_right;
            }
            else // 找到，开始删除
            {
                if(cur->_right == nullptr)
                {
                    if(cur == _root)    // parent为nullptr
                        _root = cur->_left;
                    else
                    {
                        if(parent->_left == cur)
                            parent->_left = cur->_left;
                        else
                            parent->_right = cur->_left;
                    }
                } 
                else if(cur->_left == nullptr)
                {
                    if(cur == _root)
                        _root = cur->_right;
                    else
                    {
                        if(parent->_left == cur)
                            parent->_left = cur->_right;
                        else
                            parent->_right = cur->_right;
                    }
                }
                else
                {
                    // 左右均不为空，需要替换
                    // 可以找左子树最大or右子树最小替换
                    Node* rightMin = cur->_right;
                    Node* rightMinParent = cur;
                    while(rightMin->_left)
                    {
                        rightMinParent = rightMin;
                        rightMin = rightMin->_left;
                    }

                    // 替换
                    cur->_key = rightMin->_key;
                    // 转换成删除rightMin
                    if(rightMinParent == cur)   
                        cur->_right = rightMin->_right;
                    else
                        rightMinParent->_left = rightMin->_right;
                    
                    delete rightMin;
                }
                return true;
            }
        }
        return false;
    }

    void _InOrder(const Node* root)  // 因为递归要传私有的root，故而定义_InOrder便于访问
    {
        if(root == nullptr)
            return;
        _InOrder(root->_left);
        cout<<root->_key<<':'<<root->_val<<' ';
        _InOrder(root->_right);
    }

    void InOrder()  // 中序遍历输出
    {
        _InOrder(_root);
        cout<<endl;
    }

private:
    Node* _root;
};

int main()
{
    BStree<int, string> bs;
    bs.Insert(2, "1");
    bs.Insert(3, "2");
    bs.Insert(5, "2");
    bs.Insert(1, "2");
    bs.Insert(3, "2");
    bs.Insert(11, "2");
    bs.Insert(29, "2");
    bs.Insert(9, "2");
    bs.Insert(8, "2");
    bs.InOrder();
    cout<<endl;
    bs.Erase(3);
    bs.Erase(9);
    bs.InOrder();
    return 0;
}