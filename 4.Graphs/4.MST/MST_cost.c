typedef struct GraphRep {
	int nV;
	int nE;
	float **edges;			// * to an array, each elt in array is ptr to another array
} GraphRep;					// floating pt values rather than ints

typedef GraphRep *Graph;

float