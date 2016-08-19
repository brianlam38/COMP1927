typedef struct _Student {
    int  id;       // unique identifying number
    char name[20]; // student name
    int  program;  // enrolled degree program
} Student;

// sort by program/name only

int compareProgramThenName(Student *x, Student *y)
{
	int diff = x->program - y->program;

	if (diff < 0) return -1;
	if (diff > 0) return 1;
	return compareName(x,y);
}



// sort by id only

int compareIds(Student *a, Student *b)
{
	return (a->id - b->id);
}



// sort by name only

int compareNames(Student *c, Student *d)
{
	return strcmp(c->name,d->name);
}


// discussion on usage of compare functions
// with the generic sort() function

void sort(Student a[], int lo, int hi,
          int (*compare)(Student *, Student *));

sort(a,0,N-1,compareIds);
sort(a,0,N-1,compareNames);
sort(a,0,N-1,compareProgramThenName);

- function address vs function call

compareIds  ... gives address of function
compareIds(a,b) ...
   calls function with specified parameters

- slight analogy with arrays

defined as e.g. int a[1000]

a == &a[0] ... gives address of array

a[i] ...
   indexes into array, gives value of one elem