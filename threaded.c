#include<stdio.h> 
#include<stdlib.h> // for malloc
#include<limits.h> // for MIN_ELE
#include<stdbool.h>
// structure for threaded BST node
struct Node {
	int val;
	bool isThreaded;
	struct Node *left;
	struct Node *right;
};

int swap(int *x, int *y) {
	int temp = *x;
	*x = *y;
	*y = temp;
	return 1;
}

// creates nodes
int createNode(struct Node **head, int value, bool threaded, struct Node *successor) {
	
	(*head) = (struct Node *)malloc(sizeof(struct Node));
	(*head)->val = value;
	(*head)->isThreaded = successor ? true : false;
	(*head)->left = NULL;
	(*head)->right = threaded ? successor : NULL;
		
	return 1;
}

// insert new nodes
int insertWithThread(struct Node *head, int value, bool threaded, struct Node *successor) {

	struct Node *prev = head;
	while(head && (!prev->isThreaded || (prev->isThreaded && prev->val > value))) {
		prev = head;
		if(head->val == value) {
			printf("%d already exist\n", value);
			return 0;
		}
		if(head->val < value) {
			head = head->right;
		} else {
			successor = head;
			head = head->left;
		}
	}
	
	if(prev->val < value) {
		prev->isThreaded = false;
		createNode(&prev->right, value, threaded, successor);
	} else
		createNode(&prev->left, value, threaded, successor);

}

int insert(struct Node *head, int value) {
	insertWithThread(head, value, true, NULL);
}

int display(struct Node *head, bool isPrevThreaded) {
	int i = 20;
	while(head && i) {
		while(head->left) {
			head = head->left;
		}
		printf("%d -> ", head->val);
		while(head->isThreaded) {
			head = head->right;
			printf("%d -> ", head->val);
		}
		head = head->right;
		--i;
	}
	return 1;
}

// deletes a node
int bstdelete(struct Node *root, int data) {
	struct Node *prev = root, *prev2;
	
	while(root && root->val != data) {
		prev = root;
		if(root->val < data)
			root = root->right;
		else
			root = root->left;
	}
	if(!root) return 0;
	
	struct Node *target = root;
	if(root->right && root->right->left) { 
	// if both left and right subtree exist then 
	//finds left most element from right subtree of target element	
		
		root = root->right;
		prev2 = root;
		while(root->left ) {
			prev2 = root;
			root = root->left;
		}
		
		prev2->left = root->isThreaded ? NULL : root->right;
		target->val = root->val;
		
		root = NULL;
		free(root);
		
	} else if(root->right) {
		if(prev->val < root->val)
			prev->right = root->right;
		else {
			prev->left = root->right;
			root->right->left = root->left;
		}

		root = NULL;
		free(root);
		
	} else if(root->left){
		if(prev->val < root->val)
			prev->right = root->left;
		else
			prev->left = root->left;
		
		root = NULL;
		free(root);
	}
	
}

// finds kthe largest element
struct Node * KtheElement(struct Node *head, int *K) {
	if(!head) return NULL;
	if(K == 0) return head;
	
	struct Node * temp = KtheElement(head->right, K);
	if(temp) return temp;
	
	if(--(*K) == 0)
		return head;
		
	temp = KtheElement(head->left, K);
	if(temp) return temp;
	
	return NULL;
}
// prints a particular node value
int printNode(struct Node *head) {
	if(head)
		printf("%d\n", head->val);
	else
		printf("Element doesn't exist in BST\n");
	return 1;
}
// searches an element
struct Node *bstSearch(struct Node *root, int data) {
	while(root && root->val != data) {

		if(root->val < data)
			root = root->right;
		else
			root = root->left;
	}
	return root;
} 
int main() {
	struct Node *head;

	createNode(&head, INT_MIN, false, NULL);

	insert(head, 10);
	insert(head, 5);
	insert(head, 15);
	insert(head, 3);
	insert(head, 7);
	insert(head, 12);
	insert(head, 20);
	insert(head, 10);
	insert(head, 14);
	insert(head, 17);
	insert(head, 13);
	insert(head, 19);

	int choice = -1;
	while(choice != 0) {
		printf("\n\nenter 1 to insert element\n");
		printf("enter 2 to delete element\n");
		printf("enter 3 to find Kth largest element\n");
		printf("enter 4 to search an element\n");
		printf("enter 5 to display element\n");
		printf("enter 0 to exit\n\n");
		
		printf("enter your choice = ");
		scanf("%d", &choice);
		
		int value;
		switch(choice) {
			case 1 : printf("enter value you want to insert = "); 
				 scanf("%d", &value);
				 insert(head, value);
				 break;
			case 2 : printf("enter value you want to delete = "); 
				 scanf("%d", &value);
				 bstdelete(head, value);
				 break;
			case 3 : printf("enter K for Kthe largest element = "); 
				 scanf("%d", &value);
				 printNode(KtheElement(head->right, &value));
				 break;
			case 4 : printf("enter element to search = "); 
				 scanf("%d", &value);
				 printNode(bstSearch(head, value));
				 break;
			case 5 : printf("\ninorder representation of BST \n"); 
				 display(head->right, false);
				 break;
			case 0 : choice = 0;
		}
	}
}