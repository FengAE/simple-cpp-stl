#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

#define MAX_INT 10000

template<class V, class W, W MAX_W = MAX_INT, bool Direction = false>
class Graph
{
public:
    Graph(const V* a, size_t n)
    {
        _vertex.reserve(n);
        for(size_t i=0; i<n; i++)
        {
            _indexMap[a[i]] = i;
            _vertex.push_back(a[i]);
        }
        _matrix.resize(n, vector<W>(n, MAX_W));
        _vexnum = n;
    }

    size_t Getvertexindex(const V& v)
    {
        auto pos = _indexMap.find(v);
        if(pos != _indexMap.end())
        {
            return pos->second;
        }
        else
        {
            throw invalid_argument("Vertex not exist");
            return -1;
        }
    }

    void AddEdge(const V& src, const V& dest, const W& w)
    {
        size_t src_i = Getvertexindex(src);
        size_t dest_i = Getvertexindex(dest);
        if(!Direction)   // 无向
            _matrix[dest_i][src_i] = w;
        _matrix[src_i][dest_i] = w;
    }

    void Print()
    {
        for(size_t i=0; i<_vexnum; i++)
        {
            cout<<"["<<i<<"]"<<"->"<<_vertex[i]<<endl;
        }
        for(size_t i=0; i<_vexnum; i++)
        {
            for(size_t j=0; j<_vexnum; j++)
            {
                if(_matrix[i][j] == MAX_W)
                    cout<<"*";
                else
                    cout<<_matrix[i][j];

                cout<<" ";
            }
            cout<<endl;
        }
    }

private:
    vector<V> _vertex;
    unordered_map<V, int> _indexMap;    // 顶点到下标的映射
    vector<vector<W>> _matrix;    // 邻接矩阵
    size_t _vexnum;
};

int main()
{
    Graph<char, int, true> g("0123", 4);
    g.AddEdge('0', '1', 1);
    g.AddEdge('1', '3', 1);
    g.Print();
    return 0;
}