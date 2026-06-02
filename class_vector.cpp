#include<iostream>
#include<assert.h>
using namespace std;

template<typename T>
class vector 
{
public:
    typedef T* iterator;
    vector() 
    : _start(nullptr)
    , _finish(nullptr)
    , _endofstorage(nullptr) {}

    ~vector()
    {
        delete[] _start;
        _start = _finish = _endofstorage = nullptr;
    }

    vector(const vector& v)
    {
        _start = new T[v.capacity()];
        _finish = _start+v.size();
        _endofstorage = _start+v.capacity();
        memcpy(_start, v.begin(), v.size()*sizeof(T));
    }

    iterator begin() const
    {
        return _start;
    }
    iterator end() const
    {
        return _finish;
    }

    size_t size() const
    {
        return _finish - _start;
    }
    size_t capacity() const
    {
        return _endofstorage-_start;
    }

    void reserve(size_t n)  // 将容量扩充到n
    {
        if(n > capacity())
        {
            size_t sz = size();
            T* tmp = new T[n];
            if(_start)
            {
                // 若使用memcpy(tmp, _start, sizeof(T)*sz); 存在浅拷贝风险，在拷贝string时，拷下地址
                for(size_t i=0; i<sz; i++)
                    tmp[i] = _start[i];  // 调用operator赋值，深拷贝
                delete[] _start;
            }
            _start = tmp;
            _finish = tmp+sz;
            _endofstorage = tmp+n;
        }
    }

    void resize(size_t n, const T& x = T()) // 扩容+初始化
    {
        if(n < size())
        {
            _finish = _start+n;
        }
        if(n > capacity())
        {
            reserve(n);
        }
        _endofstorage = _start+n;
        iterator end = _finish;
        while(end<_endofstorage)
        {
            *(end++) = x;
        }
    }

    T& operator[] (size_t i)  // 返回引用，允许被修改
    {
        assert(i<size());
        return _start[i];
    }

    const T& operator[] (size_t i)  const// 只读访问
    {
        assert(i<size());
        return _start[i];
    }

    vector<T> operator=(vector<T> v) // 传入已经做了一次拷贝构造，故可以直接交换
    {
        if(&v != this)
            swap(v);
        return *this;
    }

    void swap(vector<T>& v)
    {
        ::swap(_start, v._start); // 使用全局swap函数
        ::swap(_finish, v._finish);
        ::swap(_endofstorage, v._endofstorage);
    }

    void printvector()
    {
        for(size_t i=0; i<size(); i++)
            cout<<this->_start[i]<<' ';
    }

    void pushback(const T& x)
    {
        if(capacity() <= size())
        {
            size_t newcapacity = size()>0? 2*size(): 4;
            reserve(newcapacity);
        }
        *_finish = x;
        _finish++;
    }

    void popback()
    {
        erase(_finish-1);
    }

    void insert(iterator pos, const T& x)
    {
        assert(pos <= _finish);
        if(size() >= capacity()) // 扩容
        {
            size_t n = _finish-_start;
            size_t newcapacity = size()>0? 2*size(): 4;
            reserve(newcapacity);
            pos = _start+n;  // 保证pos位置，排除迭代器失效
        }
        iterator end = _finish-1; // _finish为末尾后一位
        while(end >= pos)
        {
            *(end+1) = *end;
            end--;
        }
        *pos = x;
        _finish++;
    }

    iterator erase(iterator pos)
    {
        assert(pos < _finish);
        iterator end = pos;
        while(end < _finish-1)
        {
            *end = *(end+1);
            end++;
        }
        _finish--;
        return pos;
    }

private: 
    iterator _start;
    iterator _finish;
    iterator _endofstorage;
};

int main() {
    vector<int> arr;
    arr.pushback(1);
    arr.pushback(2);
    for(size_t i=0; i<arr.capacity(); i++)
    {
        cout<<*(arr.begin()+i)<<" ";
    }
    cout<<endl;

    arr.insert(arr.begin(), 2);
    for(size_t i=0; i<arr.capacity(); i++)
    {
        cout<<*(arr.begin()+i)<<" ";
    }
    cout<<endl;

    arr.resize(20, 0);
    for(size_t i=0; i<arr.capacity(); i++)
    {
        cout<<*(arr.begin()+i)<<" ";
    }
    cout<<endl;

    vector<int> v1=arr;
    v1.printvector();
    cout<<endl;

    vector<int> v2;
    v2.pushback(3);
    v2.swap(v1);
    v1.printvector();
    cout<<endl;
    v2.printvector();
    return 0;
}
