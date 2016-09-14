// Map.c ... implementation of Map type
// (a specialised version of the Map ADT)
// You can change this as much as you want

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Map.h"
#include "Places.h"

typedef struct vNode *VList;

struct vNode {
   LocationID  v;    // ALICANTE, etc
   TransportID type; // ROAD, RAIL, BOAT
   VList       next; // link to next node
};

struct MapRep {
   int   nV;         // #vertices
   int   nE;         // #edges
   VList connections[NUM_MAP_LOCATIONS]; // array of lists
};

static void addConnections(Map);

// Create a new empty graph (for a map)
// #Vertices always same as NUM_PLACES
Map newMap()
{
   int i;
   Map g = malloc(sizeof(struct MapRep));
   assert(g != NULL);
   g->nV = NUM_MAP_LOCATIONS;
   for (i = 0; i < g->nV; i++){
      g->connections[i] = NULL;
   }
   g->nE = 0;
   addConnections(g);
   return g;
}

// Remove an existing graph
void disposeMap(Map g)
{
   int i;
   VList curr;
   VList next;
   assert(g != NULL);
   assert(g->connections != NULL);

   for (i = 0; i < g->nV; i++){
       curr = g->connections[i];
       while(curr != NULL){
          next = curr->next;
          free(curr);
          curr=next;
       }
   }
   free(g);
}

static VList insertVList(VList L, LocationID v, TransportID type)
{
   VList newV = malloc(sizeof(struct vNode));
   newV->v = v;
   newV->type = type;
   newV->next = L;
   return newV;
}

static int inVList(VList L, LocationID v, TransportID type)
{
	VList cur;
	for (cur = L; cur != NULL; cur = cur->next) {
		if (cur->v == v && cur->type == type) return 1;
	}
	return 0;
}

// Add a new edge to the Map/Graph
void addLink(Map g, LocationID start, LocationID end, TransportID type)
{
	assert(g != NULL);
	// don't add edges twice
	if (!inVList(g->connections[start],end,type)) {
   	g->connections[start] = insertVList(g->connections[start],end,type);
   	g->connections[end] = insertVList(g->connections[end],start,type);
   	g->nE++;
	}
}

// Display content of Map/Graph
void showMap(Map g)
{
   assert(g != NULL);
   printf("V=%d, E=%d\n", g->nV, g->nE);
   int i;
   for (i = 0; i < g->nV; i++) {
      VList n = g->connections[i];
      while (n != NULL) {
         printf("%s connects to %s ",idToName(i),idToName(n->v));
         switch (n->type) {
         case ROAD: printf("by road\n"); break;
         case RAIL: printf("by rail\n"); break;
         case BOAT: printf("by boat\n"); break;
         default:   printf("by ????\n"); break;
         }
         n = n->next;
      }
   }
}

// Return count of nodes
int numV(Map g)
{
   assert(g != NULL);
   return g->nV;
}

// Return count of edges of a particular type
int numE(Map g, TransportID type)
{
   int i, nE=0;
   assert(g != NULL);
   assert(type >= 0 && type <= ANY);
   for (i = 0; i < g->nV; i++) {
      VList n = g->connections[i];
      while (n != NULL) {
         if (n->type == type || type == ANY) nE++;
         n = n->next;
      }
    }
    return nE;
}

// Add edges to Graph representing map of Europe
static void addConnections(Map g)
{
   //### ROAD Connections ###

   addLink(g, ALICANTE, GRANADA, ROAD);
   addLink(g, ALICANTE, MADRID, ROAD);
   addLink(g, ALICANTE, SARAGOSSA, ROAD);
   addLink(g, AMSTERDAM, BRUSSELS, ROAD);
   addLink(g, AMSTERDAM, COLOGNE, ROAD);
   addLink(g, ATHENS, VALONA, ROAD);
   addLink(g, BARCELONA, SARAGOSSA, ROAD);
   addLink(g, BARCELONA, TOULOUSE, ROAD);
   addLink(g, BARI, NAPLES, ROAD);
   addLink(g, BARI, ROME, ROAD);
   addLink(g, BELGRADE, BUCHAREST, ROAD);
   addLink(g, BELGRADE, KLAUSENBURG, ROAD);
   addLink(g, BELGRADE, SARAJEVO, ROAD);
   addLink(g, BELGRADE, SOFIA, ROAD);
   addLink(g, BELGRADE, ST_JOSEPH_AND_ST_MARYS, ROAD);
   addLink(g, BELGRADE, SZEGED, ROAD);
   addLink(g, BERLIN, HAMBURG, ROAD);
   addLink(g, BERLIN, LEIPZIG, ROAD);
   addLink(g, BERLIN, PRAGUE, ROAD);
   addLink(g, BORDEAUX, CLERMONT_FERRAND, ROAD);
   addLink(g, BORDEAUX, NANTES, ROAD);
   addLink(g, BORDEAUX, SARAGOSSA, ROAD);
   addLink(g, BORDEAUX, TOULOUSE, ROAD);
   addLink(g, BRUSSELS, COLOGNE, ROAD);
   addLink(g, BRUSSELS, LE_HAVRE, ROAD);
   addLink(g, BRUSSELS, PARIS, ROAD);
   addLink(g, BRUSSELS, STRASBOURG, ROAD);
   addLink(g, BUCHAREST, CONSTANTA, ROAD);
   addLink(g, BUCHAREST, GALATZ, ROAD);
   addLink(g, BUCHAREST, KLAUSENBURG, ROAD);
   addLink(g, BUCHAREST, SOFIA, ROAD);
   addLink(g, BUDAPEST, KLAUSENBURG, ROAD);
   addLink(g, BUDAPEST, SZEGED, ROAD);
   addLink(g, BUDAPEST, VIENNA, ROAD);
   addLink(g, BUDAPEST, ZAGREB, ROAD);
   addLink(g, CADIZ, GRANADA, ROAD);
   addLink(g, CADIZ, LISBON, ROAD);
   addLink(g, CADIZ, MADRID, ROAD);
   addLink(g, CASTLE_DRACULA, GALATZ, ROAD);
   addLink(g, CASTLE_DRACULA, KLAUSENBURG, ROAD);
   addLink(g, CLERMONT_FERRAND, GENEVA, ROAD);
   addLink(g, CLERMONT_FERRAND, MARSEILLES, ROAD);
   addLink(g, CLERMONT_FERRAND, NANTES, ROAD);
   addLink(g, CLERMONT_FERRAND, PARIS, ROAD);
   addLink(g, CLERMONT_FERRAND, TOULOUSE, ROAD);
   addLink(g, COLOGNE, FRANKFURT, ROAD);
   addLink(g, COLOGNE, HAMBURG, ROAD);
   addLink(g, COLOGNE, LEIPZIG, ROAD);
   addLink(g, COLOGNE, STRASBOURG, ROAD);
   addLink(g, CONSTANTA, GALATZ, ROAD);
   addLink(g, CONSTANTA, VARNA, ROAD);
   addLink(g, DUBLIN, GALWAY, ROAD);
   addLink(g, EDINBURGH, MANCHESTER, ROAD);
   addLink(g, FLORENCE, GENOA, ROAD);
   addLink(g, FLORENCE, ROME, ROAD);
   addLink(g, FLORENCE, VENICE, ROAD);
   addLink(g, FRANKFURT, LEIPZIG, ROAD);
   addLink(g, FRANKFURT, NUREMBURG, ROAD);
   addLink(g, FRANKFURT, STRASBOURG, ROAD);
   addLink(g, GALATZ, KLAUSENBURG, ROAD);
   addLink(g, GENEVA, MARSEILLES, ROAD);
   addLink(g, GENEVA, PARIS, ROAD);
   addLink(g, GENEVA, STRASBOURG, ROAD);
   addLink(g, GENEVA, ZURICH, ROAD);
   addLink(g, GENOA, MARSEILLES, ROAD);
   addLink(g, GENOA, MILAN, ROAD);
   addLink(g, GENOA, VENICE, ROAD);
   addLink(g, GRANADA, MADRID, ROAD);
   addLink(g, HAMBURG, LEIPZIG, ROAD);
   addLink(g, KLAUSENBURG, SZEGED, ROAD);
   addLink(g, LEIPZIG, NUREMBURG, ROAD);
   addLink(g, LE_HAVRE, NANTES, ROAD);
   addLink(g, LE_HAVRE, PARIS, ROAD);
   addLink(g, LISBON, MADRID, ROAD);
   addLink(g, LISBON, SANTANDER, ROAD);
   addLink(g, LIVERPOOL, MANCHESTER, ROAD);
   addLink(g, LIVERPOOL, SWANSEA, ROAD);
   addLink(g, LONDON, MANCHESTER, ROAD);
   addLink(g, LONDON, PLYMOUTH, ROAD);
   addLink(g, LONDON, SWANSEA, ROAD);
   addLink(g, MADRID, SANTANDER, ROAD);
   addLink(g, MADRID, SARAGOSSA, ROAD);
   addLink(g, MARSEILLES, MILAN, ROAD);
   addLink(g, MARSEILLES, TOULOUSE, ROAD);
   addLink(g, MARSEILLES, ZURICH, ROAD);
   addLink(g, MILAN, MUNICH, ROAD);
   addLink(g, MILAN, VENICE, ROAD);
   addLink(g, MILAN, ZURICH, ROAD);
   addLink(g, MUNICH, NUREMBURG, ROAD);
   addLink(g, MUNICH, STRASBOURG, ROAD);
   addLink(g, MUNICH, VENICE, ROAD);
   addLink(g, MUNICH, VIENNA, ROAD);
   addLink(g, MUNICH, ZAGREB, ROAD);
   addLink(g, MUNICH, ZURICH, ROAD);
   addLink(g, NANTES, PARIS, ROAD);
   addLink(g, NAPLES, ROME, ROAD);
   addLink(g, NUREMBURG, PRAGUE, ROAD);
   addLink(g, NUREMBURG, STRASBOURG, ROAD);
   addLink(g, PARIS, STRASBOURG, ROAD);
   addLink(g, PRAGUE, VIENNA, ROAD);
   addLink(g, SALONICA, SOFIA, ROAD);
   addLink(g, SALONICA, VALONA, ROAD);
   addLink(g, SANTANDER, SARAGOSSA, ROAD);
   addLink(g, SARAGOSSA, TOULOUSE, ROAD);
   addLink(g, SARAJEVO, SOFIA, ROAD);
   addLink(g, SARAJEVO, ST_JOSEPH_AND_ST_MARYS, ROAD);
   addLink(g, SARAJEVO, VALONA, ROAD);
   addLink(g, SARAJEVO, ZAGREB, ROAD);
   addLink(g, SOFIA, VALONA, ROAD);
   addLink(g, SOFIA, VARNA, ROAD);
   addLink(g, STRASBOURG, ZURICH, ROAD);
   addLink(g, ST_JOSEPH_AND_ST_MARYS, SZEGED, ROAD);
   addLink(g, ST_JOSEPH_AND_ST_MARYS, ZAGREB, ROAD);
   addLink(g, SZEGED, ZAGREB, ROAD);
   addLink(g, VIENNA, ZAGREB, ROAD);

   //### RAIL Connections ###

   addLink(g, ALICANTE, BARCELONA, RAIL);
   addLink(g, ALICANTE, MADRID, RAIL);
   addLink(g, BARCELONA, SARAGOSSA, RAIL);
   addLink(g, BARI, NAPLES, RAIL);
   addLink(g, BELGRADE, SOFIA, RAIL);
   addLink(g, BELGRADE, SZEGED, RAIL);
   addLink(g, BERLIN, HAMBURG, RAIL);
   addLink(g, BERLIN, LEIPZIG, RAIL);
   addLink(g, BERLIN, PRAGUE, RAIL);
   addLink(g, BORDEAUX, PARIS, RAIL);
   addLink(g, BORDEAUX, SARAGOSSA, RAIL);
   addLink(g, BRUSSELS, COLOGNE, RAIL);
   addLink(g, BRUSSELS, PARIS, RAIL);
   addLink(g, BUCHAREST, CONSTANTA, RAIL);
   addLink(g, BUCHAREST, GALATZ, RAIL);
   addLink(g, BUCHAREST, SZEGED, RAIL);
   addLink(g, BUDAPEST, SZEGED, RAIL);
   addLink(g, BUDAPEST, VIENNA, RAIL);
   addLink(g, COLOGNE, FRANKFURT, RAIL);
   addLink(g, EDINBURGH, MANCHESTER, RAIL);
   addLink(g, FLORENCE, MILAN, RAIL);
   addLink(g, FLORENCE, ROME, RAIL);
   addLink(g, FRANKFURT, LEIPZIG, RAIL);
   addLink(g, FRANKFURT, STRASBOURG, RAIL);
   addLink(g, GENEVA, MILAN, RAIL);
   addLink(g, GENOA, MILAN, RAIL);
   addLink(g, LEIPZIG, NUREMBURG, RAIL);
   addLink(g, LE_HAVRE, PARIS, RAIL);
   addLink(g, LISBON, MADRID, RAIL);
   addLink(g, LIVERPOOL, MANCHESTER, RAIL);
   addLink(g, LONDON, MANCHESTER, RAIL);
   addLink(g, LONDON, SWANSEA, RAIL);
   addLink(g, MADRID, SANTANDER, RAIL);
   addLink(g, MADRID, SARAGOSSA, RAIL);
   addLink(g, MARSEILLES, PARIS, RAIL);
   addLink(g, MILAN, ZURICH, RAIL);
   addLink(g, MUNICH, NUREMBURG, RAIL);
   addLink(g, NAPLES, ROME, RAIL);
   addLink(g, PRAGUE, VIENNA, RAIL);
   addLink(g, SALONICA, SOFIA, RAIL);
   addLink(g, SOFIA, VARNA, RAIL);
   addLink(g, STRASBOURG, ZURICH, RAIL);
   addLink(g, VENICE, VIENNA, RAIL);

   //### BOAT Connections ###

   addLink(g, ADRIATIC_SEA, BARI, BOAT);
   addLink(g, ADRIATIC_SEA, IONIAN_SEA, BOAT);
   addLink(g, ADRIATIC_SEA, VENICE, BOAT);
   addLink(g, ALICANTE, MEDITERRANEAN_SEA, BOAT);
   addLink(g, AMSTERDAM, NORTH_SEA, BOAT);
   addLink(g, ATHENS, IONIAN_SEA, BOAT);
   addLink(g, ATLANTIC_OCEAN, BAY_OF_BISCAY, BOAT);
   addLink(g, ATLANTIC_OCEAN, CADIZ, BOAT);
   addLink(g, ATLANTIC_OCEAN, ENGLISH_CHANNEL, BOAT);
   addLink(g, ATLANTIC_OCEAN, GALWAY, BOAT);
   addLink(g, ATLANTIC_OCEAN, IRISH_SEA, BOAT);
   addLink(g, ATLANTIC_OCEAN, LISBON, BOAT);
   addLink(g, ATLANTIC_OCEAN, MEDITERRANEAN_SEA, BOAT);
   addLink(g, ATLANTIC_OCEAN, NORTH_SEA, BOAT);
   addLink(g, BARCELONA, MEDITERRANEAN_SEA, BOAT);
   addLink(g, BAY_OF_BISCAY, BORDEAUX, BOAT);
   addLink(g, BAY_OF_BISCAY, NANTES, BOAT);
   addLink(g, BAY_OF_BISCAY, SANTANDER, BOAT);
   addLink(g, BLACK_SEA, CONSTANTA, BOAT);
   addLink(g, BLACK_SEA, IONIAN_SEA, BOAT);
   addLink(g, BLACK_SEA, VARNA, BOAT);
   addLink(g, CAGLIARI, MEDITERRANEAN_SEA, BOAT);
   addLink(g, CAGLIARI, TYRRHENIAN_SEA, BOAT);
   addLink(g, DUBLIN, IRISH_SEA, BOAT);
   addLink(g, EDINBURGH, NORTH_SEA, BOAT);
   addLink(g, ENGLISH_CHANNEL, LE_HAVRE, BOAT);
   addLink(g, ENGLISH_CHANNEL, LONDON, BOAT);
   addLink(g, ENGLISH_CHANNEL, NORTH_SEA, BOAT);
   addLink(g, ENGLISH_CHANNEL, PLYMOUTH, BOAT);
   addLink(g, GENOA, TYRRHENIAN_SEA, BOAT);
   addLink(g, HAMBURG, NORTH_SEA, BOAT);
   addLink(g, IONIAN_SEA, SALONICA, BOAT);
   addLink(g, IONIAN_SEA, TYRRHENIAN_SEA, BOAT);
   addLink(g, IONIAN_SEA, VALONA, BOAT);
   addLink(g, IRISH_SEA, LIVERPOOL, BOAT);
   addLink(g, IRISH_SEA, SWANSEA, BOAT);
   addLink(g, MARSEILLES, MEDITERRANEAN_SEA, BOAT);
   addLink(g, MEDITERRANEAN_SEA, TYRRHENIAN_SEA, BOAT);
   addLink(g, NAPLES, TYRRHENIAN_SEA, BOAT);
   addLink(g, ROME, TYRRHENIAN_SEA, BOAT);
}
