// q3.c ... find vertices close to a given vertex

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "Graph.h"

#define  MAX 100

int main(int argc, char *argv[])
{
   Graph g;         // the graph
   char *edgeFile;  // file containing graph data
   Vertex startV;   // source vertex
   int dist;        // "radius"
   int N;           // # vertices
   char line[MAX];  // input line buffer
   Vertex v1, v2;

   // collect command-line params
   if (argc < 4) {
      fprintf(stderr, "Usage: ./q3 EdgeFile StartVertex Distance\n");
      exit(1);
   }
   edgeFile = argv[1];
   startV = atoi(argv[2]);
   dist = atoi(argv[3]);

   // read edges from file and insert into g
   FILE *in;
   if ((in = fopen(edgeFile,"r")) == NULL) {
      fprintf(stderr,"Can't open edge file\n");
   }
   // first line contains #vertices
   fgets(line,MAX,in);
   sscanf(line,"%d",&N);
   g = newGraph(N);
   // rest of lines contain edges
   while (fgets(line,MAX,in) != NULL) {
      if (sscanf(line,"%d %d",&v1,&v2) != 2)
         fprintf(stderr,"Bad edge (%d,%d)\n",v1,v2);
      else
         insertE(g, mkEdge(g,v1,v2));
   }
   fclose(in);

#if 0
   // show graph ... use for debugging if needed
   show(g);
#endif

   // find vertices within distance "dist" of "startV"
   Vertex *vertices = malloc(N*sizeof(Vertex));
   int nvertices;
   nvertices = within(g, startV, dist, vertices);
   int i;
   for (i = 0; i < nvertices; i++)
      printf("%d\n", vertices[i]);

   return 0;
}
