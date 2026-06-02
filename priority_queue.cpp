#include<iostream>
#include<vector>
using namespace std;

// 仿函数
template<class T>
struct my_less
{
    bool operator() (const T& x1, const T& x2) const
    {
        return x1 < x2;
    }
};

template<class T>
struct my_greater
{
    bool operator() (const T& x1, const T& x2) const
    {
        return x1 > x2;
    }
};

template<class T, class Container = vector<T>, class Compare = my_less<T>>
class priority_queue
{
    public:
        void push(const T& x)
        {
            _con.push_back(x);
            AdjustUp(_con.size()-1);
        }
        
        size_t size()
        {
            return _con.size();
        }

        T& top()
        {
            return _con[0];
        }

        void pop()
        {
            swap(_con[0], _con[_con.size()-1]);
            _con.pop_back();
            AdjustDown(0);
        }

        void AdjustUp(int child)
        {
            while(child > 0)
            {
                int parent = (child-1)/2;
                if(compare(_con[parent], _con[child]))
                {
                    swap(_con[parent], _con[child]);
                    child = parent;
                }
                else
                    break;
            }
        }

        void AdjustDown(int root)
        {
            int n = _con.size();
            int parent = root;
            int child = parent*2+1;
            while(child < n)
            {
                // 找左右孩子大的
                if(child+1<n && compare(_con[child], _con[child+1]))
                    child++;
                // 调整
                if(compare(_con[parent], _con[child]))
                {
                    swap(_con[parent], _con[child]);
                    parent = child;
                    child = parent*2+1;
                }
                else    
                    break;
            }
        }

    private:
        Container _con;
        Compare compare;
};




int main()
{
    priority_queue<int, vector<int>, greater<int>> pq;
    pq.push(1);
    pq.push(2);
    pq.push(5);
    pq.push(6);
    pq.push(2);
    pq.push(4);
    pq.push(9);
    pq.push(7);
    pq.push(5);
    size_t size = pq.size();
    for(size_t i=0; i<size; i++)
    {
        cout<<pq.top()<<' ';
        pq.pop();
    }


    
    return 0;
}