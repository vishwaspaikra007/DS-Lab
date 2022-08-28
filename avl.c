#include<stdio.h> 
#include<stdlib.h> // for malloc
#include<limits.h> // for MIN_ELE
#include<stdbool.h>

// structure for threaded BST node
struct Node {
	int val;
	int balanceFactor;
	struct Node *left;
	struct Node *right;
};

// creates nodes
int createNode(struct Node **head, int value) {
	
	(*head) = (struct Node *)malloc(sizeof(struct Node));
	(*head)->val = value;
	(*head)->balanceFactor = 0;
	(*head)->left = NULL;
	(*head)->right = NULL;
		
	return 1;
}

// left rotate
struct Node * rotateLeft(struct Node *head) {
	struct Node *node = head->right;
	struct Node *temp = node->left;

	node->left = head;
	head->right = temp;

	return node;
}

// right rotate
struct Node * rotateRight(struct Node *head) {
	struct Node *node = head->left;
	struct Node *temp = node->right;

	node->right = head;
	head->left = temp;

	return node;
}

// insert new nodes
int insert(struct Node *root, int value) {

    struct Node *prev = root;
	struct Node *head = root->right;
	struct Node *stk[1000];
	int top = 0;

	while(head) {
		prev = head;
		stk[top++] = head;
		if(head->val == value) {
			printf("%d already exist\n", value);
			return 0;
		}
		if(head->val < value) {
			head->balanceFactor += -1;
			head = head->right;
		} else {
			head->balanceFactor += 1;
			head = head->left;
		}
	}
	
	// inserting new Node in the end
	if(prev->val < value)
		createNode(&prev->right, value);
	else
		createNode(&prev->left, value);

	// Balacing the tree
	struct Node *temp = NULL;
	while(top-- > 0) {
		if(stk[top]->balanceFactor > 1) {
			if(stk[top]->left->val > value) 
				temp = rotateRight(stk[top]);
			else {
				stk[top]->left = rotateLeft(stk[top]->left);
				temp = rotateRight(stk[top]);
			}
			break;
		} else if(stk[top]->balanceFactor < -1) {
			if(stk[top]->right->val < value) 
				temp = rotateLeft(stk[top]);
			else {
				stk[top]->right = rotateRight(stk[top]->right);
				temp = rotateLeft(stk[top]);
			}
			break;
		}
	}

	if(top-- > 0) {
		if(stk[top]->val > value) {
			stk[top]->left = temp;
			stk[top]->balanceFactor += -1;
		} else {
			stk[top]->right = temp;
			stk[top]->balanceFactor += 1;
		}
	} else if(temp) root->right = temp;

	// changing balance factor of the parent nodes;
	while(top-- > 0) {
		stk[top]->balanceFactor += stk[top]->val > value ? -1 : 1;
	}

}

int dfs(struct Node * head, FILE *f) {
	if(!head) return 1;
	
	// for graphviz
	if(head->left)
		fprintf(f, "%d -> %d\n", head->val, head->left->val);
	if(head->right)
		fprintf(f, "%d -> %d\n", head->val, head->right->val);

	dfs(head->left, f);
	printf("%d ", head->val);
	dfs(head->right, f);
	return 1;
}

int display(struct Node *head) {
	FILE *f;
	f = fopen("graph1.gv", "w");
	fprintf(f, "digraph G {\n");
	dfs(head, f); // dfs call for inorder
	fprintf(f, "}\n");
	fclose(f);
	//system("dot -Tps graph1.gv -o graph1.ps");
}

int bstdelete(struct Node *root, int value) {
	struct Node *prev = root, *prev2;
	
	while(root && root->val != value) {
		prev = root;
		if(root->val < value)
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
		
		prev2->left = NULL;
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

struct Node * KtheElement(struct Node *head, int value) {}
int bstSearch(struct Node *head, int *value) {}
int printNode(struct Node *head) {}

int main() {
	struct Node *head;

	createNode(&head, INT_MIN);

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

	bstdelete(head, 12);

	display(head->right);
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
			// case 3 : printf("enter K for Kthe largest element = "); 
			// 	 scanf("%d", &value);
			// 	 printNode(KtheElement(head->right, &value));
			// 	 break;
			// case 4 : printf("enter element to search = "); 
			// 	 scanf("%d", &value);
			// 	 printNode(bstSearch(head, value));
			// 	 break;
			case 5 : printf("\ninorder representation of BST \n"); 
				 display(head->right);
				 break;
			case 0 : choice = 0;
		}
	}
}