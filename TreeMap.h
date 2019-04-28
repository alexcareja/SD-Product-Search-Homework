#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <stdlib.h>

#define MAX(a, b) (((a) >= (b))?(a):(b))

//-------------------------------------------------------------------------

typedef struct node{
	void* elem; // Node key
	void* info; // Node satellite information
	struct node *pt; // Parent link
	struct node *lt; // left child link
	struct node *rt; // right child link
	struct node* next; // next link in the list of duplicates
	struct node* prev; // prev link in the list of duplicates
	struct node* end; // pointer to the end of the duplicates list
	long height;
}TreeNode;

typedef struct TTree{
	TreeNode *root; // pointer to the root of the tree
	void* (*createElement)(void*); // create element method
	void (*destroyElement)(void*); // destroy element method
	void* (*createInfo)(void*); // create info method
	void (*destroyInfo)(void*); // destroy info method
	int (*compare)(void*, void*); // comparison method between elements
	long size;
}TTree;


TTree* createTree(void* (*createElement)(void*), void (*destroyElement)(void*),
		void* (*createInfo)(void*), void (*destroyInfo)(void*),
		int compare(void*, void*)){
	TTree *tree = (TTree *) malloc(sizeof(TTree));
	if(tree == NULL){
		return NULL;
	}
	tree->size = 0;
	tree->createElement = createElement;
	tree->createInfo = createInfo;
	tree->destroyElement = destroyElement;
	tree->destroyInfo = destroyInfo;
	tree->compare = compare;
	tree->root = NULL;
	return tree;
}

int isEmpty(TTree* tree){

	if(tree->size == 0){
		return 1;
	}
	return 0;
}

TreeNode* search(TTree *tree, TreeNode* x, void* elem){
	if(x == NULL){
		return NULL;
	}
	if(tree->compare(x->elem, elem) == 0){
		return x;
	}
	if(tree->compare(x->elem, elem) > 0){
		if(x->lt == NULL){
			return NULL;
		}
		return search(tree, x->lt, elem);
	}
	else{
		if(x->rt == NULL){
			return NULL;
		}
		return search(tree, x->rt, elem);
	}
	return NULL;
}

TreeNode* search_node(TreeNode* x, void* elem){
	if(x == NULL){
		return NULL;
	}
	if(*((long *)x->elem) == *(long *)elem){
		return x;
	}
	if(*((long *)x->elem) > *(long *)elem){
		if(x->lt == NULL){
			return NULL;
		}
		search_node(x->lt, elem);
	}
	else{
		if(x->rt == NULL){
			return NULL;
		}
		search_node(x->rt, elem);
	}
}


TreeNode* minimum(TreeNode* x){
	if(x == NULL){
		return NULL;
	}
	if(x->lt == NULL){
		return x;
	}
	return minimum(x->lt);
}

TreeNode* maximum(TreeNode* x){
	if(x == NULL){
		return NULL;
	}
	if(x->rt == NULL){
		return x;
	}
	return maximum(x->rt);
}

TreeNode* successor(TreeNode* x){
	TreeNode *aux = x;
	if(x == NULL){
		return NULL;
	}
	if(x->rt != NULL){	// Merg pe dreapta
		return minimum(x->rt);
	}
	else{	// Merg in sus
		while(aux != NULL){
			if(aux->pt == NULL){ // Daca nu are parinte
				return NULL;
			}
			if(aux->pt->lt == aux){
				return aux->pt;
			}
			aux = aux->pt;
		} 
		if(aux == NULL){	// Daca nu are succesor
			return NULL;
		}
		return NULL;
	}
}

TreeNode* predecessor(TreeNode* x){
	TreeNode *aux = x;
	if(x == NULL){
		return NULL;
	}
	if(x->lt != NULL){	// Merg pe stanga
		return maximum(x->lt);
	}
	else{	// Merg in sus
		while(aux != NULL){
			if(aux->pt == NULL){ // Daca nu are parinte
				return NULL;
			}
			if(aux->pt->rt == aux){
				return aux->pt;
			}
			aux = aux->pt;
		} 
		if(aux == NULL){	// Daca nu are succesor
			return NULL;
		}
		return NULL;
	}
}

void updateHeight(TreeNode* x){
	int leftHeight = 0;
	int rightHeight = 0;
	if(x != NULL){
		if(x->lt != NULL) leftHeight = x->lt->height;
		if(x->rt != NULL) rightHeight = x->rt->height;
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}

void avlRotateLeft(TTree* tree, TreeNode* x){
	TreeNode *aux; 
	if(x->rt == NULL){
		return;
	}
	aux = x->rt->lt;
	if(x == tree->root){
		tree->root = x->rt;
		x->rt->pt = NULL;
		tree->root->lt = x;
		x->pt = tree->root;
		x->rt = aux;
		if(aux != NULL){
			aux->pt = x->rt;
		}

	}
	else{
		if(x->pt->rt ==  x){
			x->pt->rt = x->rt;
			x->rt->pt = x->pt;
			x->pt->rt->lt = x;
			x->pt = x->pt->rt;
		}
		else{
			x->pt->lt = x->rt;
			x->rt->pt = x->pt;
			x->pt->lt->lt = x;
			x->pt = x->pt->lt;
		}	
		x->rt = aux;
		if(aux != NULL){
			aux->pt = x;
		}
		return;
	}

}

void avlRotateRight(TTree* tree, TreeNode* y){
	TreeNode *aux;
	if(y->lt == NULL){
		return;
	}
	aux= y->lt->rt;
	if(y == tree->root){
		tree->root = y->lt;
		y->lt->pt = NULL;
		tree->root->rt = y;
		y->pt = tree->root;
		y->lt = aux;
		if(aux != NULL){
			aux->pt = y->lt;
		}
	}
	else{
		if(y->pt->lt ==  y){
			y->pt->lt = y->lt;
			y->lt->pt = y->pt;
			y->pt->lt->rt = y;
			y->pt = y->pt->lt;
		}
		else{
			y->pt->rt = y->lt;
			y->lt->pt = y->pt;
			y->pt->rt->rt = y;
			y->pt = y->pt->rt;
		}	
		y->lt = aux;
		if(aux != NULL){
			aux->pt = y;
		}
		
	}

}

/* Get AVL balance factor for node x */
int avlGetBalance(TreeNode *x){
	if(x == NULL){
		return 0;
	}
	int left = 0, right = 0;
	if(x->lt != NULL){
		left = x->lt->height;
	}
	if(x->rt != NULL){
		right = x->rt->height;
	}
	return left - right;
}


void avlFixUp(TTree* tree, TreeNode* y){

	// TODO: Cerinta 1
}


TreeNode* createTreeNode(TTree *tree, void* value, void* info){
	// Allocate node
	TreeNode* newNode = (TreeNode *) malloc(sizeof(TreeNode));

	// Set its element field
	newNode->elem = tree->createElement(value);
	//newNode->elem = value;

	//Set its info
	newNode->info = tree->createInfo(info);
	//newNode->info = info;

	// Set its tree links
	newNode->pt = newNode->rt = newNode->lt = NULL;

	// Set its list links
	newNode->next = newNode->prev = newNode->end = NULL;

	/*
	 *  The height of a new node is 1,
	 *  while the height of NULL is 0
	 */
	newNode->height = 1;

	return newNode;
}

void destroyTreeNode(TTree *tree, TreeNode* node){
	// Check arguments
	if(tree == NULL || node == NULL) return;

	// Use object methods to de-alocate node fields
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// De-allocate the node
	free(node);
}

int update_height(TreeNode *node){
	if(node == NULL){
		return 0;
	}
	int left, right;
	left = update_height(node->lt) + 1;
	right = update_height(node->rt) + 1;
	if(left >= right){
		node->height = left;
		return node->height;
	}
	else{
		node->height = right;
		return node->height;
	}
	
}

int height_diff(TreeNode *node){
	int left, right;
	if(node->lt == NULL){
		left = 0;
	}
	else{
		left = node->lt->height;
	}
	if(node->rt == NULL){
		right = 0;
	}
	else{
		right = node->rt->height;
	}
	return left-right;
}

void insert(TTree* tree, void* elem, void* info) {
	if(tree == NULL){
		return;
	}
	TreeNode *node = createTreeNode(tree, elem, info);
	TreeNode *aux = search_node(tree->root, elem);
	TreeNode *succ, *pred;
	if(aux != NULL){	// Daca exista deja nodul in arbore
		aux->end->next = node;
		node->prev = aux->end;
		succ = successor(aux);
		node->next = succ;
		if(succ != NULL){
			succ->prev = node;
		}
		aux->end = node;
		return;
	}
	node->end = node;
	if(tree->size == 0){	// Daca arborele e gol, adaug doar elementul
		tree->root = node;
		node->pt = NULL; 
		tree->size = 1;
		return;
	}
	aux = tree->root;
	while( 1 ){
		if(tree->compare(aux->elem, elem) > 0){	// Daca elem e mai mic, ne ducem pe stanga
			if(aux->lt != NULL){
				aux = aux->lt;
				continue;
			}
			else{	// Trebuie adaugat
				tree->size++;
				pred = predecessor(aux);
				aux->lt = node;
				node->pt = aux;
				node->next = aux;
				aux->prev = node;
				node->prev = pred;
				if(pred != NULL){
					pred->end->next = node;
				}
				break;
			}
		}
		else{	// Daca e mai mare, ne ducem pe dreapta
			if(aux->rt != NULL){
				aux = aux->rt;
				continue;
			}
			else{	// Trebuie adaugat
				tree->size++;
				succ = successor(aux);
				aux->rt = node;
				node->pt = aux;
				node->prev = aux;
				aux->end->next = node;
				node->next = succ;
				if(succ != NULL){
					succ->prev = node;
				}
				break;
			}
		}
		break;
	}
	// Recalculez intaltimile in arbore
	tree->root->height = update_height(tree->root);
	// Caut un nod dezechilibrat
	char poz[100];
	int i;
	aux = node;
	if(aux->pt->rt == aux){
		strcpy(poz, "r");
		i = 0;
	}
	else{
		strcpy(poz, "l");
		i = 0;
	}
	aux = aux->pt;
	while(aux != NULL){
		if(height_diff(aux) == 2){
			// cazul stanga stanga
			if(poz[i-1] == 'l'){
				//printf("cazul stanga stanga \n");
				avlRotateRight(tree, aux);
			}
			// cazul stanga dreapta
			if(poz[i-1] == 'r'){
				//printf("cazul stanga dreapta \n");
				avlRotateLeft(tree,aux->lt);
				avlRotateRight(tree,aux);
				
			}
			break;
		}
		if(height_diff(aux) == -2){
			// cazul dreapta dreapta
			if(poz[i-1] == 'r'){
				//printf("cazul dreapta dreapta \n");
				avlRotateLeft(tree, aux);
			}
			// cazul dreapta stanga
			if(poz[i-1] == 'l'){
				//printf("cazul dreapta stanga \n");
				avlRotateRight(tree, aux->rt);
				avlRotateLeft(tree, aux);
			}
			break;
		}
		if(aux->pt != NULL){
			if(aux->pt->rt == aux){
				strcat(poz, "r");
				i++;
			}
			else{
				strcat(poz, "l");
				i++;
			}
		}
		aux = aux->pt;
	}
	tree->root->height = update_height(tree->root);

}

void delete(TTree* tree, void* elem){
	TreeNode *aux = search(tree, tree->root, elem), *aux2, *succ, *pred;
	if(aux == NULL){
		return;
	}
	if(aux->end != aux){
		aux->end->prev->next = aux->end->next;
		if(aux->end->next != NULL){
			aux->end->next->prev = aux->end->prev;
		}
		aux2 = aux->end;
		aux->end = aux->end->prev;
		tree->destroyElement(aux2->elem);
		tree->destroyElement(aux2->info);
		free(aux2);
		return;
	}
	if(tree->size == 1){	// Cazul in care exista numai un nod
		tree->size = 0;
		tree->destroyElement(aux->elem);
		tree->destroyElement(aux->info);
		free(aux);
		tree->root = NULL;
		return;
	}
	tree->size--;
	// Elimin nodul din arbore
	if( (aux->lt == NULL) || (aux->rt == NULL) )
	{
		// Elimin nodul din lista
		succ = successor(aux);
		pred = predecessor(aux);
		if(pred != NULL){
			pred->end->next = succ;
			if(succ != NULL){
				succ->prev = pred->end;
			}
		}
		else{
			if(succ != NULL){
				succ->prev = NULL;
			}	
		}
		if((aux->lt == NULL) && (aux->rt == NULL)){ // Cazul frunza
			if(aux->pt->lt == aux){
				aux->pt->lt = NULL;
			}
			else{
				aux->pt->rt = NULL;
			}
			tree->destroyElement(aux->elem);
			tree->destroyElement(aux->info);
			free(aux);
		}
		else{	// Cazul doar un copil
			aux2 = aux;
			if(aux->lt != NULL){
				aux->lt->pt = aux->pt;
				if(aux == tree->root){ // Cazul radacina
					tree->root = aux->lt;
				}
				else{
					if(aux->pt->lt == aux){
						aux->pt->lt = aux->lt;
					}
					else{
						aux->pt->rt = aux->lt;
					}
				}
			}
			else{
				aux->rt->pt = aux->pt;
				if(aux == tree->root){ // Cazul radacina
					tree->root = aux->rt;
				}
				else{
					if(aux->pt->lt == aux){
						aux->pt->lt = aux->rt;
					}
					else{
						aux->pt->rt = aux->rt;
					}
				}
			}
			tree->destroyElement(aux2->elem);
			tree->destroyElement(aux2->info);
			free(aux2);
		}
	}
	else{
		// Cazul cu 2 copii
		// Folosesc succesorul nodului de sters
		aux2 = minimum(aux->rt);
		// Elimin succesorul din arbore
		if(aux2->pt->rt == aux2){
			aux2->pt->rt = aux2->rt;
			if(aux2->rt != NULL){
				aux2->rt->pt = aux2->pt;
			}
		}
		else{
			aux2->pt->lt = aux2->rt;
			if(aux2->rt != NULL){
				aux2->rt->pt = aux2->pt;
			}	
		}
		// Copiez datele din succesor
		aux->next = aux2->next;
		tree->destroyElement(aux->elem);
		tree->destroyElement(aux->info);
		aux->elem = tree->createElement(aux2->elem);
		aux->info = tree->createInfo(aux2->info);

		// Sterg succesorul
		tree->destroyElement(aux2->elem);
		tree->destroyElement(aux2->info);
		free(aux2);
	}

	// Recalculez inaltimile
	tree->root->height = update_height(tree->root);
	// Caut un nod dezechilibrat
	if((avlGetBalance(aux) < 2) && (avlGetBalance(aux) > -2) && (aux != tree->root)){
		do{
			aux = aux->pt;
			if((avlGetBalance(aux) > 1) || (avlGetBalance(aux) < - 1)){
				// Am gasit nod dezechilibrat
				break;
			}
		}while(aux != tree->root);
	}
	int balance = avlGetBalance(aux);

	if (balance > 1 && avlGetBalance(aux->lt) >= 0){
		// Cazul Stanga Stanga
		avlRotateRight(tree, aux);
		tree->root->height = update_height(tree->root);
		return;
	}
	if (balance > 1 && avlGetBalance(aux->lt) < 0) {
		// Cazul Stanga Dreapta
		avlRotateLeft(tree, aux->lt);
		avlRotateRight(tree, aux);
		tree->root->height = update_height(tree->root);
		return;
	}
	if (balance < -1 && avlGetBalance(aux->rt) <= 0){
		// Cazul Dreapta Dreapta
		avlRotateLeft(tree, aux);
		tree->root->height = update_height(tree->root);
		return;
	}
	if (balance < -1 && avlGetBalance(aux->rt) > 0){
		// Cazul Dreapta Stanga
		avlRotateRight(tree, aux->rt);
		avlRotateLeft(tree, aux);
		tree->root->height = update_height(tree->root);
		return;
	} 

}

void destroyTree(TTree* tree){
	TreeNode *aux = minimum(tree->root), *aux2;
	while(aux != NULL){
		tree->destroyElement(aux->elem);
		tree->destroyElement(aux->info);
		aux2 = aux->next;
		free(aux);
		aux = aux2;
	}
	free(tree);
}


#endif /* TREEMAP_H_ */
