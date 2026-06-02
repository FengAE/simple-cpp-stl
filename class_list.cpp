#include<iostream>
#include<assert.h>
using namespace std;

template<typename T>
class Listnode
{
    public:
        T _data;
        Listnode* _next;
        Listnode* _prev;

        Listnode(const T& data = 0)
        :_data(data),_next(nullptr), _prev(nullptr){}
};

template<class T, class Ref, class Ptr>
struct Listiterator    // 默认public
{
    typedef Listiterator<T, Ref, Ptr> Self; // 将Ref，Ptr作为参数，以区分T和const T
    Listnode<T>* curnode;

    Listiterator(Listnode<T>* node)
    : curnode(node) {}

    // 拷贝构造
    Listiterator(const Self& other) : curnode(other.curnode) {}

    Ref operator*()  // 使用Ref，可以区分const型和非const型的迭代器
    {
        return curnode->_data;
    }

    Ptr operator->()    // 注意该处省略了一个->，实际应该是 iterator->->data
    {
        return &curnode->_data;
    }

    // 重置前增运算符
    Self& operator++()
    {
        curnode = curnode->_next;
        return *this;
    }

    Self& operator--()
    {
        curnode = curnode->_prev;
        return *this;
    }

    // 重置后增运算符
    Self operator++(int)
    {
        Self tmp(*this);
        ++(*this);
        return tmp;
    }

    Self operator--(int)
    {
        Self tmp(*this);
        --(*this);
        return tmp;
    }

    Self& operator=(const Self& other)
    {
        this->curnode = other.curnode;
        return *this;
    }

    bool operator==(const Self& newiterator) const
    {
        return curnode == newiterator.curnode;
    }
    bool operator!=(const Self& newiterator) const
    {
        return !(*this==newiterator);
    }
};

template<typename T>
class list  // 带头结点的双向列表
{
    typedef Listnode<T> Listnode;
    public:
        typedef Listiterator<T, T&, T*> iterator;
        typedef Listiterator<T, const T&, const T*> const_iterator;

        list()
        {
            _head = new Listnode(0);
            _head->_next = _head->_prev = _head;
        }

        ~list()
        {
            clear();
            delete _head;
            _head = nullptr;
        }

        void clear()    // 链表内容清空
        {
            Listnode* cur = _head->_next;
            while(cur != _head)
            {
                Listnode* tmp = cur;
                cur = cur->_next;
                delete tmp;
            }
        }

        list(const list<T>& lt) // 拷贝构造
        {
            _head = new Listnode();
            _head->_next = _head;
            _head->_prev = _head;

            for(auto e : lt)
            {
                push_back(e);
            }
        }

        list<T>& operator=(const list<T>& lt)
        {
            if(this != &lt)
            {
                clear();
                list<T> tmp(lt);
                swap(_head, lt._head);
            }
            return *this;
        }
        
        iterator begin()
        {
            return iterator(_head->_next);  // _head为哨兵位
        }

        iterator end()
        {
            return iterator(_head);
        }
        
        const_iterator begin() const
        {
            return const_iterator(_head->_next);  // _head为哨兵位
        }

        const_iterator end() const
        {
            return const_iterator(_head);
        }

        void push_back(const T& x)
        {
            Insert(end(), x);
        }

        void push_front(const T& x)
        {
            Insert(begin(), x);
        }

        void pop_front()
        {
            erase(begin());
        }

        void pop_back()
        {
            Listnode* tail = _head->_prev;
            iterator lt(tail);
            erase(lt);
        }

        void Insert(iterator pos, const T& x)
        {
            Listnode* cur = pos.curnode;
            Listnode* prev = cur->_prev;
            Listnode* newnode = new Listnode(x);
            cur->_prev = newnode;
            newnode->_next = cur;
            prev->_next = newnode;
            newnode->_prev = prev; 
        }

        void erase(iterator pos)
        {
            assert(pos != end());   // 不能把_head删除
            Listnode* cur = pos.curnode;
            Listnode* prev = cur->_prev;
            prev->_next = cur->_next;
            cur->_next->_prev = prev;
            delete cur;
        }

    private:
        Listnode* _head;
};

int main()
{
    list<int> head;
    head.push_back(1);
    head.push_back(2);
    head.push_back(3);
    head.push_back(4);
    head.pop_front();
    head.pop_back();
    list<int> head1(head);
    for(list<int>::iterator lt = head.begin(); lt!=head.end(); lt++)
    {
        (*lt)++;
        cout<<*lt<<endl;
    }  
    return 0;
}