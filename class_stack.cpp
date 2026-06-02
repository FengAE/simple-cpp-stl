#include<iostream>
#include<vector>
#include<list>
using namespace std;

template<class T, class Container>
class stack
{
    public:
        void push_back(const T& x)
        {
            _con.push_back(x);
        }

        void pop_back()
        {
            _con.pop_back();
        }

        size_t size()
        {
            return _con.size();
        }

        T& back()
        {
            return _con.back();
        }

        bool empty()
        {
            return _con.empty();
        }

    private:
        Container _con;
};

int main()
{
    stack<int, vector<int>> st;
    st.push_back(1);
    st.push_back(2);
    st.push_back(3);
    st.push_back(4);
    while(!st.empty())
    {
        cout<<st.back()<<' ';
        st.pop_back();
    }
    cout<<endl;
}