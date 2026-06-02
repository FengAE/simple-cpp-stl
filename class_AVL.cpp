#include<iostream>
using namespace std;

template<class K, class V>
struct AVLNode
{
    AVLNode<K, V>* _left;
    AVLNode<K, V>* _right;
    AVLNode<K, V>* _parent;

    int _bf;    // 平衡因子
    pair<K, V> _kv;

    AVLNode(const pair<K, V>& kv)
    :_left(nullptr), _right(nullptr)
    ,_parent(nullptr), _bf(0), _kv(kv) {}
};

template<class K, class V>
class AVLTree
{
    typedef AVLNode<K, V> Node;
public:
    bool Insert(const pair<K, V>& kv)   // 有插入返回true
    {
        // 1. 按搜索树规则插入
        if(_root == nullptr)
        {
            _root = new AVLNode(kv);
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
            else    return false;
        }
        cur = new Node(kv);
        cur->_parent = parent;
        if(parent->_kv.first < cur->_kv.first)
            parent->_right = cur;
        else 
            parent->_left = cur;

        // 2. 更新平衡因子
        GetInsert_bf(parent, cur);
        return true;
    }

    void GetInsert_bf(Node* parent, Node* cur)  // 在cur处插入
    {
        //  插入更新平衡因子
        // （1）cur为parent的左，parent->_bf--; cur为parent的右，parent->_bf++;
        // （2）更新完parent后，如果parent->_bf==0，结束更新
        // （3）如果更新后parent->_bf == ±1，继续（1）
        // （4）如果更新后parent->_bf == ±2，不平衡，旋转处理
        while(parent)
        {
            if(cur == parent->_left)
                parent->_bf--;
            else
                parent->_bf++;
            
            if(parent->_bf == 0)
                break;
            else if(parent->_bf == 1 || parent->_bf == -1)
            {
                parent = parent->_parent;
                cur = cur->_parent;
            }
            else    // ±2，旋转处理，保持搜索树
            {
                if(parent->_bf == 2)
                {
                    if(cur->_bf == 1)   // 单旋
                        RotateL(parent);
                    else    // 双旋
                        RotateRL(parent);

                }
                else
                {
                    if(cur->_bf == -1)
                        RotateR(parent);
                    else    // 双旋
                        RotateLR(parent);
                }

                // 旋转完后，parent所在树已经恢复插入前的高度，
                break;
            }
        }
    }

    bool Erase(const K& key)    // 有删除返回true
    {
        Node* parent = nullptr;
        Node* cur = _root;
        while(cur != nullptr)
        {
            if(cur->_kv.first > key)
            {
                parent = cur;
                cur = cur->_left;
            }
            else if(cur->_kv.first < key)
            {
                parent = cur;
                cur = cur->_right;
            }
            else // 找到，开始删除
            {
                if(cur->_right == nullptr)
                {
                    if(cur == _root)    // parent为nullptr
                    {
                        _root = cur->_left;
                        if(_root)
                            _root->_parent = nullptr;
                        return true;
                    }
                    else
                    {
                        GetErase_bf(parent, cur);
                        if(parent->_left == cur)
                            parent->_left = cur->_left;
                        else
                            parent->_right = cur->_left;  

                        if(cur->_left)
                            cur->_left->_parent = parent;                        
                    }
                } 
                else if(cur->_left == nullptr)
                {
                    if(cur == _root)
                    {
                        _root = cur->_right;
                        if(_root)
                            _root->_parent = nullptr;
                        return true;
                    }
                    else
                    {
                        GetErase_bf(parent, cur);
                        if(parent->_left == cur)
                            parent->_left = cur->_right;
                        else
                            parent->_right = cur->_right;
                        
                        if(cur->_right)
                            cur->_right->_parent = parent;
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

                    // 替换并更新平衡因子
                    cur->_kv = rightMin->_kv;
                    GetErase_bf(rightMinParent, rightMin);

                    // 转换成删除rightMin
                    if(rightMinParent == cur)   
                        cur->_right = rightMin->_right;
                    else
                        rightMinParent->_left = rightMin->_right;

                    if(rightMin->_right)
                        rightMin->_right->_parent = rightMinParent;
                
                    delete rightMin;
                }
                return true;
            }
        }
        return false;
    }

    void GetErase_bf(Node* parent, Node* cur)  // 在cur处删除
    {
        //  删除更新平衡因子
        // （1）cur为parent的左，parent->_bf++; cur为parent的右，parent->_bf--;
        // （2）更新完parent后，如果parent->_bf==±1，结束更新
        // （3）如果更新后parent->_bf == 0，继续（1）
        // （4）如果更新后parent->_bf == ±2，不平衡，旋转处理
        while(parent)
        {
            if(cur == parent->_left)
                parent->_bf++;
            else
                parent->_bf--;
            
            if(parent->_bf == 1 || parent->_bf == -1)
                break;
            else if(parent->_bf == 0)
            {
                parent = parent->_parent;
                cur = cur->_parent;
            }
            else    // ±2，旋转处理，保持搜索树
            {
                if(parent->_bf == 2)
                {
                    if(cur->_bf == 1)   // 单旋
                        RotateL(parent);
                    else    // 双旋
                        RotateRL(parent);

                }
                else
                {
                    if(cur->_bf == -1)
                        RotateR(parent);
                    else    // 双旋
                        RotateLR(parent);
                }

                // 旋转完后，parent所在树已经恢复插入前的高度，
                break;
            }
        }
    }

    void RotateL(Node* parent)  // 左单旋
    {
        Node* subR = parent->_right; 
        parent->_right = subR->_left;
        if(subR->_left)
            subR->_left->_parent = parent;

        Node* ppNode = parent->_parent;
        subR->_left = parent;
        parent->_parent = subR;

        // 串联subR和ppNode
        if(parent == _root)
        {
            _root = subR;
        }
        else
        {
            if(ppNode->_left == parent)
                ppNode->_left = subR;
            else
                ppNode->_right = subR;
        }
        subR->_parent = ppNode;   

        parent->_bf = subR->_bf = 0;    // 更新_bf
    }

    void RotateR(Node* parent)  // 右单旋
    {
        Node* subL = parent->_left;
        parent->_left = subL->_right;
        if(subL->_right)
            subL->_right->_parent = parent;
        Node* ppNode = parent->_parent;
        subL->_right = parent;
        parent->_parent = subL;
        
        if(parent == _root)
        {
            _root = subL;
        }
        else
        {
            if(ppNode->_left == parent)
                ppNode->_left = subL;
            else
                ppNode->_right = subL;
        }
        subL->_parent = ppNode;

        subL->_bf = parent->_bf = 0;
    }

    void RotateRL(Node* parent)
    {
        Node* subR = parent->_right;
        Node* subRL = subR->_left;
        int bf = subRL->_bf;
        RotateR(subR);
        RotateL(parent);

        if(bf == 1) // subRL右边插入
        {
            parent->_bf = -1;
            subR->_bf = 0;
            subRL->_bf = 0;
        }
        else if(bf == -1)
        {
            parent->_bf = 0;
            subR->_bf = 1;
            subRL->_bf = 0;
        }
        else    // bf == 0
        {
            parent->_bf = subR->_bf = subRL->_bf = 0;
        }
    }

    void RotateLR(Node* parent)
    {
        Node* subL = parent->_left;
        Node* subLR = subL->_right;
        int bf = subLR->_bf;
        RotateL(subL);
        RotateR(parent);

        if(bf == 1)
        {
            parent->_bf = 0;
            subL->_bf = -1;
            subLR->_bf = 0;
        }
        else if(bf == -1)
        {
            parent->_bf = 1;
            subL->_bf = 0;
            subLR->_bf = 0;
        }
        else    // bf == 0
        {
            parent->_bf = subL->_bf = subLR->_bf = 0;
        }
    }

    void _InOrder(Node* root)
    {
    if(root == nullptr)
        return;
    _InOrder(root->_left);
        cout<<root->_kv.first<<':'<<root->_kv.second<<' ';
        _InOrder(root->_right);
    }
    void InOrder()
    {
        _InOrder(_root);
        cout<<endl;
    }

    bool _IsBalance(Node* root)
    {
        if(root == nullptr)
            return true;
        int leftheight = Height(root->_left);
        int rightheight = Height(root->_right);
        return abs(leftheight-rightheight)<2 && 
                _IsBalance(root->_left) && _IsBalance(root->_right);
    }
    bool IsBalance()
    {
        return _IsBalance(_root);
    }

    int Height(Node* root)
    {
        if(root == nullptr)
            return 0;
        int leftheight = Height(root->_left);
        int rightheight = Height(root->_right);
        return leftheight>rightheight? 1+leftheight: 1+rightheight;
    }

private:
    Node* _root = nullptr;
};

int main()
{
    int a[] = {1,5,0,63};
    AVLTree<int, int> t;
    for(auto e:a)
    {
        t.Insert(make_pair(e, e));
    }
    t.InOrder();
    cout<<t.IsBalance()<<endl;  
    t.Erase(5);
    t.Erase(1);
    t.InOrder();
    cout<<t.IsBalance()<<endl;

    t.Erase(63);
    t.Erase(0);
    t.InOrder();
    cout<<t.IsBalance();

    return 0;
}