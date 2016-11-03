// main.c ... driver program for Graph type
// Finds and displays "well-connected" vertices

#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"

int main(int argc, char *argv[])
{
   Graph G;
   Connects *conns;
   int n;

   G = readGraph(stdin);
   if (G == NULL) {
      printf("Invalid graph\n");
      return EXIT_FAILURE;
   }
   else {
      printf("Graph:\n"); showGraph(G); printf("\n");
      conns = wellConnected(G, &n);
      if (n == 0)
         printf("No well-connected vertices\n");
      else {
         printf("Well-connected vertices\n");
         int i;
         for (i = 0; i < n; i++)
            printf("%d has %d connections\n", conns[i].vertx, conns[i].nconn);
      }
   }
   return EXIT_SUCCESS;
}
