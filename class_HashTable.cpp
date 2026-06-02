#include<iostream>
#include<vector>
using namespace std;

// 位图，便于大数据查找，节省空间
// 缺陷：只能处理整型
class bitset
{
public:
    bitset(size_t N)
    {
        _bits.resize(N/32+1, 0);
        _num = 0;
    }

    void set(size_t x)
    {
        size_t index = x/32;    // x映射的位在第几个整型
        size_t pos = x%32;  // x在这个整型的第几个位

        _bits[index] |= (1<<pos);   // pos位置1
    }

    void reset(size_t x)
    {
        size_t index = x/32;
        size_t pos = x%32;

        _bits[index] &= ~(1<<pos);  // pos位置0
    }

    bool test(size_t x) // 判断x映射的是0还是1
    {
        size_t index = x/32;
        size_t pos = x%32;

        return (_bits[index] & (1<<pos));
    }


private:
    vector<int> _bits;
    size_t _num;    // 映射存储数据数
};


//布隆过滤器
struct HashStr1
{
    size_t operator() (const string& str)
    {
        size_t hash = 0;
        for(size_t i=0; i<str.size(); i++)
        {
            hash *= 131;
            hash += str[i];
        }
        return hash;
    }
};

template<class K, class Hash1 = HashStr1, class Hash2, class Hash3>
class bloomfilter
{
public:
    bloomfilter(size_t num)
    :_bs(5 * num)
    ,_N(5 * num) {}

    void set(const K& key)
    {
        size_t index1 = Hash1()(key) % _N;
        size_t index2 = Hash2()(key) % _N;
        size_t index3 = Hash3()(key) % _N;

        _bs.set(index1);
        _bs.set(index2);
        _bs.set(index3);
    }

    bool test(const K& key)
    {
        size_t index1 = Hash1()(key) % _N;
        if(_bs.test(index1))    return false;

        size_t index2 = Hash2()(key) % _N;
        if(_bs.test(index2))    return false;

        size_t index3 = Hash3()(key) % _N;
        if(_bs.test(index3))    return false;

        return true;    // 不一定真的有
    }
private:
    bitset _bs; // 位图
    size_t _N;
};

int main()
{
    bitset bs(100);
    bs.set(99);
    bs.set(7);
    bs.reset(99);
    for(int i=0; i<100; i++)
    {
        cout<<i<<":"<<bs.test(i)<<endl;
    }
}