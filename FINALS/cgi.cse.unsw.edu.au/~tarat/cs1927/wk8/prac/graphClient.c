#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "GRAPH.h"
#define MAXV 5
#define MAXE 10


Graph graphScan(){
    int v,w;
    int V;
    printf("Enter num vertices : ");
    scanf("%d",&V);
    printf("Enter pairs of vertices between 0 and %d\n",V-1);
    Graph g = GRAPHinit(V);
    while(scanf("%d %d",&v,&w) == 2){
        GRAPHinsertE(g,EDGE(v,w));
    }
    return g;
}

int main(int argc, char * argv[]){
    //No euler path
    printf("Testing graph 1\n");
    Graph g = GRAPHinit(4);
    GRAPHinsertE(g, EDGE(0,1));
    GRAPHinsertE(g, EDGE(0,1)); 
    GRAPHinsertE(g, EDGE(0,3));
    GRAPHinsertE(g, EDGE(1,2));
    GRAPHinsertE(g, EDGE(1,2));
    GRAPHinsertE(g, EDGE(1,3));
    GRAPHinsertE(g, EDGE(2,3));
    GRAPHshow(g);
    assert(hasEuler(g) == 0);
    GRAPHdestroy(g);
    printf("Testing graph 2\n"); 
    g = GRAPHinit(4);
    GRAPHinsertE(g, EDGE(0,1));
    GRAPHinsertE(g, EDGE(0,3));
    GRAPHinsertE(g, EDGE(1,2));
    GRAPHinsertE(g, EDGE(1,3));
    GRAPHinsertE(g, EDGE(2,3));
    GRAPHshow(g);
    assert(hasEuler(g) == 1);
    GRAPHdestroy(g); 
    printf("Testing graph 3\n"); 
    g = GRAPHinit(4);
    GRAPHinsertE(g, EDGE(0,1));
    GRAPHinsertE(g, EDGE(1,2));
    GRAPHinsertE(g, EDGE(2,3));
    GRAPHinsertE(g, EDGE(3,0));
    GRAPHshow(g);
    assert(hasEuler(g) == 1);
    GRAPHdestroy(g);
    printf("All tests passed! Congratulations, comrade, you will not be shipped to Siberia!\n"); 
    return 0;
}
