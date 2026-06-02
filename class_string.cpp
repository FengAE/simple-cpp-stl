#include<iostream>
using namespace std;

#include<cstring>
#include<assert.h>
#include<string>

class my_string
{
private:
    char* _str;
    size_t _size; // 为增删查改服务
    size_t _capacity; // 能存多少有效字符

    typedef char* iterator;

public:
    my_string(const char* str = "") // const确保权限缩小
    {
        _size = strlen(str);
        _capacity = _size;
        _str = new char[_capacity+1];
        strcpy(_str, str);
    }

    ~my_string()
    {
        delete[] _str;
        _str = nullptr;
        _size = _capacity = 0;
    }

    // my_string s2(s1)，采用深拷贝
    my_string(const my_string& s)
    :_str(new char [strlen(s._str)+1])
    ,_size(s._size),_capacity(s._capacity)
    {
        strcpy(_str, s._str);
    }
    /*   现代深拷贝构造写法：
     my_string(const my_string& s)
        :_str(nullptr)
        {
            string tmp(s);
            swap(_str, tmp._str);
        } */

    char& operator[] (size_t i) // 可修改访问
    {
        assert(i < _size);
        return _str[i];
    }

    const char& operator[](size_t i) const // 只读访问
    {
        assert(i<_size);
        return _str[i];
    }

    my_string& operator+=(const char ch)
    {
        push_back(ch);
        return *this;
    }
    my_string& operator+=(const char* ch)
    {
        append(ch);
        return *this;
    }

    // 赋值'='重载注意返回类型，满足连等需求
    my_string& operator= (const my_string& s) 
    {
        if(this != &s) // 排除自我赋值情况
        {
            char* tmp = new char[strlen(s._str)+1];
            strcpy(tmp, s._str);
            delete[] _str;  // 记得要释放原来空间
            _str = tmp;

            _size = s._size;
            _capacity = s._capacity;
            return *this;
        }
    }
    /* 现代赋值写法
    my_string& operator=(const my_string& s)
    {
        if(this != &s)
        {
            string tmp(s);
            swap(_str, tmp._str);  // 出作用域，tmp帮助析构原来的_str空间
        }
        return *this;
    }*/

    bool operator<(my_string s)
    {
        return strcmp(_str, s._str)<0;
    }
    bool operator>(my_string s)
    {
        return strcmp(_str, s._str)>0;
    }
    bool operator==(my_string s)
    {
        return strcmp(_str, s._str) == 0;
    }
    bool operator>=(my_string s)
    {
        return !(*this<s);
    }
    bool operator<=(my_string s)
    {
        return !(*this>s);
    }

    size_t size() const
    {
        return _size;
    }
    size_t capacity() const
    {
        return _capacity;
    }

    iterator begin()
    {
        return _str;
    }
    iterator end()
    {
        return _str+_size;
    }

    void reserve(size_t n)
    {
        if(n>_capacity)
        {
            char* tmp = new char[n];
            strcpy(tmp, _str);
            delete[] _str;
            _str = tmp;
            _capacity = n;
        }
    }

    void push_back(const char ch) // 添加字符
    {
        if(_size == _capacity)
        {
            _capacity==0? reserve(2): reserve(2*_capacity);
        }
        _str[_size] = ch;
        _str[++_size] = '\0';
    }

    void append(const char* str) // 添加字符串
    {
        size_t len = strlen(str);
        if(len+_size > _capacity)
        {
            reserve(len+_size);
        }
        strcpy(_str+_size, str);
        _size += len;
    }

    void insert(size_t pos, char ch)
    {
        if(_size == _capacity)
        {
            size_t newcapacity = _capacity==0? 2: 2*_capacity;
            reserve(newcapacity);
        }
        _size++;
        size_t end = _size-1;
        while(end>pos)
        {
            _str[end] = _str[end-1];
            end--;
        }
        _str[pos] = ch;
        _str[_size] = '\0';
    }

    void insert(size_t pos, const char* str)
    {
        int len = strlen(str);
        reserve(len+_size);
        _size += len;
        size_t end = _size-1;
        while(end > pos+len)
        {
            _str[end] = _str[end-len];
            end--;
        }
        strncpy(_str+pos, str, len*sizeof(char));
        _str[_size] = '\0';
    }

    void resize(size_t n, char ch = '\0')
    {
        if(n < _size)
        {
            _size = n;
            _str[_size] = '\0';
        }
        else
        {
            if(n > _capacity)
                reserve(n);
            for(size_t i=_size; i<n; i++)
                _str[i] = ch;
            _size = n;
            _str[_size] = '\0';
        }
    }

    void erase(size_t pos = 0, size_t len = string::npos)
    {
        assert(pos < _size);
        if(len > _size-pos)
        {
            _str[pos] = '\0';
            _size = pos;
        }
        else
        {
            for(size_t i=pos; i+len<_size; i++)
                _str[i] = _str[i+len];
            _size -= len;
            _str[_size] = '\0';
        }
    }

    size_t find(char ch, size_t pos = 0)
    {
        for(size_t i=pos; i<_size; i++)
        {
            if(_str[i] == ch)
                return i;
        }
        return string::npos;
    }

    size_t find(const char* str, size_t pos = 0)
    {
        char* p = strstr(_str+pos, str);
        if(p == nullptr)
            return string::npos;
        else
            return p-_str;
    }
};

ostream& operator<<(ostream& out, const my_string& s)
{
    for(size_t i=0; i<s.size(); i++)
    {
        cout<<s[i]<<' ';
    }
    return out;
}

istream& operator>>(istream& in, my_string& s)
{
    while(1)
    {
        char ch;
        ch = in.get();
        if(ch == ' ' || ch == '\n')
            break;
        else
            s+=ch;
    }
}

int main()
{
    my_string s;
    my_string s1 = s;
    cout<<s1<<endl;
    s1+='a';
    cout<<s1<<endl;
    s1+="aialhfj";
    cout<<s1<<endl;
    s1.insert(0, "xxx");
    cout<<s1<<endl;
    return 0;
}