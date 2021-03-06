#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

int MAX_WIDTH = 10;
int MAX_BLOCKS = 100;
int compute = 0;

struct polyomino** plist;
int np = 0;

// Based on code from https://stackoverflow.com/a/3219471
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define fori(n, f) for (int i=0; i<n, i++) { f }

// char COLOR_ORDER = { RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA };
// TODO: implement arrayview
// TODO: object representing all combinations/configurations of some objects?
// TODO: add memoization
// TODO: define mappings from one polyomino to another (and transformations, etc.)
// TODO: Polyomino naming
// TODO: bounding box sizes
// TODO: perimeter/edge shapes of polyominoes (convext)
// TODO: polyomino sets
// TODO: add equivalence pre-checks
// TODO: quicker ways of counting than enumerating all polyominoes?
// TODO: database indices
// TODO: polyomino grammars
//
// TODO: polyomino perimeter
// TODO: optimization
// TODO: track updates/cache propagated information
// TODO: add semi-mutable polyomino data structures
// TODO: use pointers to make my life easier
// TODO: count holes
// TODO: use linked lists

// TODO: use bit arrays
// TODO: possible naming schemes?
// TODO: polyomino signatures for memoization of testing translational/rotational equivalence? (maybe break down into sub-polyominoes)
// TODO: more robust coordinate system (what is a polyomino?)
//
// TODO: use preprocessor
// TODO: add mechanism for describing and testing invariants

struct vector {
//	int x, y, z;
	int x, y;
};

struct vector vec(int x, int y) {
	return (struct vector) { x, y };
}

struct array {
	int rank;
	int* shape;
	int size;
	int* data;
};


struct array fill_array(struct array a, int value) {
	//fori(a.size,
	//	a.data[i] = value;
	//	compute ++;
	//)
	for (int i=0; i<a.size; i++) {
		a.data[i] = value;
		compute ++;
	}
	return a;
}

struct array new_array(int rank, int* shape) {
	// int rank = sizeof(shape);
	// int* size = malloc(sizeof(int));
	int size = 1;
	for(int i=0; i<rank; i++) {
		size *= shape[i];
		compute ++;
	}
	printf("Initalizing array with size %i \n", size);
	//int data[size] = {0};
	//int data[size];
	int* data = calloc(size, sizeof(int));
	struct array a = { rank, shape, size, data };
	fill_array(a, 0);
	return a;
};

//struct array array_and(struct array a1, struct array a2) {

// struct array map_array(struct array a

struct polyomino {
	int n;
	// int* indices;
	struct vector** indices;
	struct array matrix;
	char* name;
//	struct vector* bounds;
};


struct polyomino new_polyomino(char* name, int x, int y) {
	// use array of pointers?
	//int idx[MAX_BLOCKS * 2] = {0};
	//int idx[MAX_BLOCKS * 2];

	// do we need to typecast this?
	// int* idx = calloc(MAX_BLOCKS * 2, sizeof(int));
	//int shape[2] = { MAX_WIDTH, MAX_WIDTH };
	
	struct vector** indices = calloc(MAX_BLOCKS, sizeof(struct vector));
	int* shape = calloc(2, sizeof(int));
	//shape = { MAX_WIDTH, MAX_WIDTH };
	shape[0] = x;
	shape[1] = y;

	printf("Creating polyomino (max width: %i, max height: %i) \n", shape[0], shape[1]);
	// malloc?
	struct array matrix = new_array(2, shape);
	struct polyomino p = { 0, indices, matrix, name };

	plist[np] = &p;
	np ++;

	return p;
}


int bound(int* x, int a, int b) {
	if (*x < a) { *x = a; }
	if (*x > b) { *x = b; }
}

int* get_cell(struct polyomino p, struct vector w) {
	//bound(&w.x, 0, p.matrix.shape[0]);
	//bound(&w.y, 0, p.matrix.shape[1]);
	if (w.x < 0 || w.x > p.matrix.shape[0]-1) { return NULL; }
	if (w.y < 0 || w.y > p.matrix.shape[1]-1) { return NULL; }
	compute ++;
	return &p.matrix.data[w.x * p.matrix.shape[1] + w.y];
}

int get_cell_value(struct polyomino p, struct vector w) {
	compute ++;
	int* cell_ptr = get_cell(p, w);
	return cell_ptr == NULL ? 0 : *cell_ptr;
}

void pinfo(struct polyomino p) {
	printf("n: %i, shape: %i by %i \n", p.n, p.matrix.shape[0], p.matrix.shape[1]);
}

void pprint(struct polyomino p, char color[]) {
	for (int x=0; x<p.matrix.shape[0]; x++) {
		for (int y=0; y<p.matrix.shape[1]; y++) {
			if (color == "n") {
				//char C[] = COLOR_ORDER[(int) 
				printf((*get_cell(p, vec(x, y))) == 1 ? ("**") : "  ");
				compute ++;
			}
		}
		printf("\n");
	}
}

void afree(struct array a) {
	free(a.shape);
	free(a.data);
}

void pfree(struct polyomino p) {
	afree(p.matrix);
	free(p.indices);
}

int intersect(struct polyomino p1, struct polyomino p2, int dx, int dy) {
	// TODO: revise bounds
	for (int x=0; x<p1.matrix.shape[0]; x++) {
		for (int y=0; y<p1.matrix.shape[1]; y++) {
			compute ++;
			if (
				get_cell_value(p1, vec(x, y)) &&
				get_cell_value(p2, vec(x+dx, y+dy))
			) {
				return 1;
			}
		}
	}
	return 0;
}

// double adjacent(struct polyomino p) {
// 	for (int x=0; x<p.matrix.shape[0]; x++) {
// 		for (int y=0; y<p.matrix.shape[1]; y++) {
// 		
// 		}
// 	}
// }
//



void* find_space(struct vector** source, int n) {
	for (int i=0; i<n; i++) {
		compute ++;
		if (source[n] == NULL) {
			return &source[n];
		}
	}
	printf("No sections available (%i checked) \n", n);
	return NULL;
}

void add_block(struct polyomino p, struct vector w) {
	compute ++;
	int* cell_ptr = get_cell(p, w);
	*cell_ptr = 1;
//	p.idx[p.n] = w;
	// TODO***
	struct vector* w_ptr = (struct vector*) find_space(p.indices, MAX_BLOCKS);
	if (w_ptr == NULL) {
		printf("Memory limit reached \n");
		exit(1);
	}
	*w_ptr = w;
	p.n ++;
}

void remove_block(struct polyomino p, struct vector w) {
	int* cell_ptr = get_cell(p, w);
	*cell_ptr = 0;
	for (int i=0; i<p.n; i++) {
		compute ++;
		if (p.indices[i] -> x == w.x && p.indices[i] -> y == w.y) {
			p.indices[i] = NULL;
		}
		p.n --;
		break;
	}
}

void add_center(struct polyomino p) {
	add_block(
		p,
		vec(
			(int) (p.matrix.shape[0]/2),
			(int) (p.matrix.shape[1]/2)
		)
	);
}

struct node {
	void* value;
	struct node** adjacent;
};

struct graph {
	struct node** nodes;
};

struct edges {
	int** edges;
	int num_edges;
};

int count_adj(struct polyomino p, int x, int y) {
	int adj = (
		get_cell_value(p, vec(x-1, y))+
		get_cell_value(p, vec(x+1, y))+
		get_cell_value(p, vec(x, y-1))+
		get_cell_value(p, vec(x, y+1))
	);
	compute ++;
	return adj;
}

int perimeter(struct polyomino p) {
	int P = 0;
//	for (int i=0; i<p.n; i++) {
	for (int x=0; x<p.matrix.shape[0]; x++) {
		for (int y=0; y<p.matrix.shape[1]; y++) {
			compute ++;
			int v = get_cell_value(p, vec(x, y));
			if (v) {
				P += 4 - count_adj(p, x, y);
			}
		}
	}
	return P;
}

struct edges get_edges(struct polyomino p) {
	int** edges = calloc(p.matrix.size, sizeof(int));
	int num_edges = 0;
	// more elegant way to do this?
	for (int x=0; x<p.matrix.shape[0]; x++) {
		for (int y=0; y<p.matrix.shape[1]; y++) {
			struct vector v = { x, y };
			int* cell_ptr = get_cell(p, v);
			int adj = (
				get_cell_value(p, vec(x-1, y))+
				get_cell_value(p, vec(x+1, y))+
				get_cell_value(p, vec(x, y-1))+
				get_cell_value(p, vec(x, y+1))
			);
			// printf("%i adj; ", adj);
			compute ++;
			if (*cell_ptr == 0 && adj > 0) {
				edges[num_edges] = cell_ptr;
				num_edges ++;
			}
		}
	}
	struct edges e = { edges, num_edges };
	return e;

}

// TODO: add "base polyominoes" and choice indices for combinatorial descriptions
char* global_name(struct polyomino p) {
}

char namechars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int enumerate(struct polyomino p, int n, int i, int limit, int* prev, int filter) {
//	printf("Enumerating extensions on p of size %i (running total is %i, limit %i) \n", n, i, limit);
	struct edges e = get_edges(p);
	for (int j=0; j<e.num_edges; j++) {
		if (i > limit) {
			printf("Limit (%i) reached; halting execution \n", limit);
			break;
		}
		if (n == 1) {
			return 1;
			//i = 1;
			//break;
		}
		//if (prev != NULL) {
		//	*prev = 0;
		//}

		*(e.edges[j]) = 1;
//		pprint(p, "n");
//		TODO: check if polyomino already exists before making recursive calls
		compute ++;
		i += enumerate(p, n-1, 0, limit, e.edges[j], 1);
		*(e.edges[j]) = 0;
	}
	return i;
}


//void build(struct polyomino p) {
//	struct edges = get_edges(p);


// Modifies polyomino in place
void grow_polyomino(struct polyomino p) {
	//int* edges[p.matrix.size] = {0};
	//int* edges[p.matrix.size];
	
	// ???
	compute ++;
	struct edges e = get_edges(p);	
	if (e.num_edges > 0) {
	// if (edges != NULL) {
		int z = rand() % e.num_edges;
		*(e.edges[z]) = 1;
	}
	free(e.edges);
}

void shrink_polyomino(struct polyomino p) {
	compute ++;
	if (p.n) {
		int z = rand() % p.n;
//		remove_block(p, (struct vector) {
//		TODO: allow passing pointer to vector in remove_block
		remove_block(p, *p.indices[z]);
	}
}

int equivalent(struct polyomino p1, struct polyomino p2, int dx, int dy) {
	for (int x=0; x<p1.matrix.shape[0]; x++) {
		for (int y=0; y<p1.matrix.shape[1]; y++) {
			compute ++;
			if (
				get_cell_value(p1, vec(x, y)) !=
				get_cell_value(p2, vec(x, y))
			) {
				return 0;
			}
		}
	}
	return 1;
}

int connected(struct polyomino p) {

}

void optimize(struct polyomino p, int iterations, char* goal) {
	printf("Maximizing %s... \n", goal);
	double l1, l2;
	for (int i=0; i<iterations; i++) {
		struct edges e = get_edges(p);
		compute ++;
		if (e.num_edges > 0) {
			int z = rand() % e.num_edges;
			*(e.edges[z]) = 1;
			l2 = perimeter(p);
			printf("Loss value is %f \n", l2);
			if (l2 <= l1) {
				*(e.edges[z]) = 0;
			}
		}
		free(e.edges);
	}
}

// Other methods of comparison (more efficient)?
int translation_equivalent(struct polyomino p1, struct polyomino p2) {
	for (int x=-p1.matrix.shape[0]; x<p1.matrix.shape[0]*2; x++) {
		for (int y=-p1.matrix.shape[1]; y<p1.matrix.shape[1]*2; y++) {
			compute ++;
			if (!equivalent(p1, p2, x, y)) {
				return 0;
			}
		}
	}
	return 1;
}

int rotation_equivalent(struct polyomino p1, struct polyomino p2) {

}

// struct table {
// 	char headers[][];
// 	int data[][];
// };

struct attempt {
	int success, level;
	char message[];
};


void endpoint() {
	for (int i=0; i<np; i++) {
		if (plist[i] != NULL) {
			printf(RED "Warning: " RESET "object was not cleared from memory after use");
		}
	}
}

void init() {
	plist = calloc(100, sizeof(struct polyomino));
	np = 0;
}

int main() {
	srand(time(NULL));
	printf("packings.c loaded successfully\n");

	init();

	for (int i=1; i<=7; i++) {
		printf("Enumerating polyominos of size %i... \n", i);
		struct polyomino p = new_polyomino(NULL, 10, 10);
		add_center(p);
		int n = enumerate(p, i, 0, 100000, NULL, 1) / i;
		//pprint(p, "n");
		printf(" found %i \n", n);
		printf("Total compute used: %i \n", compute);
		printf("\n");
	}
	printf("\n");

	for (int i=0; i<10; i++) {
		printf("Creating new polyomino\n");
		int q = (int) (sqrt(i+1))*10;
		struct polyomino p = new_polyomino(NULL, q, q);
		add_center(p);
		pinfo(p);
		printf("Adding blocks to polyomino\n");
		for (int j=0; j<(i+1)*10; j++) {
			grow_polyomino(p);
		}
		pprint(p, "n");
		printf("Perimeter: %i \n", perimeter(p));
		pfree(p);
		printf("Total compute used: %i \n", compute);
		printf("\n");
	}
	printf("\n");
	
	struct polyomino p = new_polyomino(NULL, 20, 20);
	add_center(p);
	optimize(p, 20, "perimeter");
	pprint(p, "n");
	printf("Total compute used: %i \n", compute);
	printf("\n");

	// endpoint();
}
