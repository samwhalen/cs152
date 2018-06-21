#include<stdio.h>
#include<stdlib.h>
#include "bst.h"
#include "llist.h"
#include "memory.h"


/* Will create a new node and return a
 * pointer to its data field
 */
node* node_new(void* data){

	node* nn = (node*)malloc(sizeof(node));
	nn->data = data;
	nn->left = NULL;
	nn->right = NULL;
	return nn;
}

/* create a new bst, initialize its root to be NULL
 */
bst* bst_new(int (*cmp)(const void* x, const void* y)){
	
	bst* nbst = (bst*)malloc(sizeof(bst));
	nbst->root = NULL;
	nbst->cmp = cmp;
	return nbst;
}

/* Insert a node to to a subtree with a root node as parameter
 * Insertion is in sorted order. 
 * Return the new root of the modified subtree.  
 */
node* node_insert(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){

	node* current = root;

	if(current == NULL){
		root = node_new(data);
		return root;
	}

	int signal = cmp(data, current->data);

	switch(signal){
		
		case 1:
			if(current->right == NULL){
				current->right = node_new(data);
				return root;
			}
			else {
				node_insert(current->right, data, cmp);
			}
			break;

		case 0:
		case -1:
			if(current->left == NULL){
				current->left = node_new(data);
				return root;
			}
			else {
				node_insert(current->left, data, cmp);
			}
			break;
	}
	return root;
}

/* Insert a new node to the bst
 */
void bst_insert(bst* b, void* data){

	b->root = node_insert(b->root, data, b->cmp);
}

//find_max helper for node_delete
node* find_max(node* root)
{
	node* current = root;

	if(current == NULL){
		return NULL;
	}
	if(current->right == NULL){
		return current;
	}
	else {
		return find_max(current->right);
	}
}

//find_min helper for node_delete
node* find_min(node* root)
{
	node* current = root;

	if(current == NULL){
		return NULL;
	}
	if(current->left == NULL){
		return current;
	}
	else {
		return find_min(current->left);
	}
}

/* delete a node from a subtree with a given root node
 * use the comparison function to search the node and delete 
 * it when a matching node is found. This function only
 * deletes the first occurrence of the node, i.e, if multiple 
 * nodes contain the data we are looking for, only the first node 
 * we found is deleted. 
 * Return the new root node after deletion.
 */
node* node_delete(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){
	
	if(root == NULL){
		return NULL;
	}
	int signal = cmp(data, root->data);
	if(signal == -1){
		root->left = node_delete(root->left, data, cmp);
		return root;
	}
	else if(signal == 1){
		root->right = node_delete(root->right, data, cmp);
		return root;
	}
	else if(signal == 0){
		if((root->right == NULL) && (root->left == NULL)){
			free(root);
			return NULL;
		}
		else if((root->left != NULL) && (root->right == NULL)){
			node* replacement = find_max(root->left);
			root->data = replacement->data;
			root->left = node_delete(root->left,
				       	replacement->data, cmp);
			return root;
		}
		else if((root->left == NULL) && (root->right != NULL)){
			node* replacement = find_min(root->right);
			root->data = replacement->data;
			root->right = node_delete(root->right,
				       	replacement->data, cmp);
			return root;
		}
		else if((root->left != NULL) && (root->right != NULL)){
			node* replacement = find_min(root->right);
			root->data = replacement->data;
			root->right = node_delete(root->right,
				       	replacement->data, cmp);
			return root;
		}
	}
	return NULL;
}


void bst_delete(bst* b, void* data){
	
	int comp = b->cmp(data, b->root->data);
	switch(comp){
		case 0:
			if((b->root->left == NULL) &&
					(b->root->right == NULL)){
				b->root = NULL;
				return;
			}
			else if(b->root->left != NULL){
				b->root = b->root->left;
				return;
			}
			else if(b->root->right != NULL){
				b->root = b->root->right;
				return;
			}
	}
	b->root = node_delete(b->root, data, b->cmp);
}

/* Search for a node containing data in a subtree with
 * a given root node. Use recursion to search that node. 
 * Return the first occurrence of node. 
 */
void* node_search(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){

	node* current = root;
	if(current != NULL){
		int signal = cmp(data, current->data);
		switch(signal){
			
			case 0:
				return current;

			case -1:
				return node_search(current->left, data, cmp);

			case 1:
				return node_search(current->right, data, cmp);

			default:
				return NULL;
		}
	
	}
	else {
		printf("ERROR: node_search: Did not find the specified ");
		printf("node.\n");
		return NULL;
	}	
}

/* Search a node with data in a bst. 
 */
void* bst_search(bst* b, void* data)
{	
	node* res = node_search(b->root, data, b->cmp);
	return res;
}

/* traverse a subtree with root in ascending order. 
 * Apply func to the data of each node. 
 */
void inorder_traversal(node* root, void(*func)(void* data))
{
	if(root == NULL){
		return;
	}
	else {
		inorder_traversal(root->left, func);
		func(root->data);
		inorder_traversal(root->right, func);
	}
}

/* traverse a bst in ascending order. 
 * Apply func to the data of each node. 
 */
void bst_inorder_traversal(bst* b, void(*func)(void* data))
{
	inorder_traversal(b->root, func);
}

// a helper function to create linked list from bst by in order traversal
void inorder_traversal_insert_llist(node* root, llist* l){

	if(root == NULL){
		return;
	}
	inorder_traversal_insert_llist(root->left, l);
	insert_tail(l, root->data);
	inorder_traversal_insert_llist(root->right, l);
}

/* an iterator to iterate through the bst in ascending order
 */
void* bst_iterate(bst* b){

	if(b != NULL){
		llist* list = create_llist();
		inorder_traversal_insert_llist(b->root, list);
		return iterate(list);
	}
	if(b == NULL){
		return iterate(NULL);
	}
	else {
		return NULL;
	}
}

//node_free
void node_free(node* n)
{
	if(n == NULL){
		return;
	}
	else {
		node_free(n->left);
		node_free(n->right);
		free(n);
	}
}

/* free the bst with 
 */
void bst_free(bst* b)
{
	if(b != NULL){
		node_free(b->root);
	}
}
