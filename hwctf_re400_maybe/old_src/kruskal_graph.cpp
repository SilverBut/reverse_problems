#include <iostream>  
#include <algorithm>  
  
using namespace std;  
  
struct CEdge  
{  
    int u;  
    int v;  
    int weight;  
  
    CEdge(){}  
    CEdge(int u,int v,int w):u(u),v(v),weight(w){}  
};  
  
  
int *root;  
  
  
bool compare(CEdge a,CEdge b)  
{  
    return  a.weight < b.weight;//ÉýÐò..  
}  
  
int Find(int x)  
{  
    return root[x];  
}  
  
void Union(int a,int b,int V)  
{  
    int root_a = Find(a);  
    int root_b = Find(b);  
    if(root_a != root_b)  
    {  
        root[b] = root_a;  
        //如果a，b不在同一棵树，要把b所在树的所有顶点都移植过去给a...  
        for(int i = 1 ; i <= V;i++)  
            if(root[i] == root_b)  
                root[i] = root_a;  
    }  
}  
  
void Kruskal(int V,int E,CEdge *e)  
{  
    for(int i = 1 ; i <= V;i++)  
        root[i] = i;  
  
    //order by weight in edge  
    sort(e,e+E,compare);  
  
    for(int i = 0 ; i < E;i++)  
        if(Find(e[i].u) != Find(e[i].v))  
        {  
            cout<<e[i].u<<"---"<<e[i].v<<" ";  
            Union(e[i].u,e[i].v,V);  
        }  
    cout<<endl;  
}  
  
int main()  
{  
    int V = 9,E = 14;  
  
    // make a graph  
    CEdge edges[E];  
    edges[0].v = 1;edges[0].u=2;edges[0].weight=4;  
    edges[1].v = 2;edges[1].u=3;edges[1].weight=8;  
    edges[2].v = 3;edges[2].u=4;edges[2].weight=7;  
    edges[3].v = 4;edges[3].u=5;edges[3].weight=9;  
    edges[4].v = 5;edges[4].u=6;edges[4].weight=10;  
    edges[5].v = 6;edges[5].u=7;edges[5].weight=2;  
    edges[6].v = 7;edges[6].u=8;edges[6].weight=1;  
    edges[7].v = 1;edges[7].u=8;edges[7].weight=8;  
    edges[8].v = 2;edges[8].u=8;edges[8].weight=11;  
    edges[9].v = 3;edges[9].u=9;edges[9].weight=2;  
    edges[10].v = 9;edges[10].u=8;edges[10].weight=7;  
    edges[11].v = 9;edges[11].u=7;edges[11].weight=6;  
    edges[12].v = 3;edges[12].u=6;edges[12].weight=4;  
    edges[13].v = 4;edges[13].u=6;edges[13].weight=14;  
  
    //init  
    root = new int[V+1];  
  
    Kruskal(V,E,edges);  
  
    return 0;  
}  
