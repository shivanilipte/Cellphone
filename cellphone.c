#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 1000
#define MIN 2
#define maxchildren 4

  typedef struct user {
      long long int num;
      int stnid;
  }user;
  typedef struct btreeNode {
        user* val[MAX + 1]; 
        int count;
        struct btreeNode *link[MAX + 1];
  }btn;









typedef struct station 
{ 
    int id; 
    struct station *left, *right;
    char* name; 
}station; 
   
// A utility function to create a new BST station 
struct station *newstation(int item, char *name) 
{ 
    struct station *temp =  (struct station *)malloc(sizeof(struct station)); 
    temp->id = item; 
    temp->name = name; 
    temp->left = temp->right = NULL; 
    return temp; 
} 
   
// A utility function to do inorder traversal of BST 
void inorder(struct station *root) 
{ 
    if (root != NULL) 
    { 
        inorder(root->left); 
        printf("%d %s\n", root->id, root->name); 
        inorder(root->right); 
    } 
} 
   
/* A utility function to insert a new station with given id in BST */
struct station* insert(struct station* station, int id, char* name) 
{ 
    /* If the tree is empty, return a new station */
    if (station == NULL) return newstation(id,name); 
  
    /* Otherwise, recur down the tree */
    if (id < station->id) 
        station->left  = insert(station->left, id, name); 
    else if (id > station->id) 
        station->right = insert(station->right, id, name);    
  
    /* return the (unchanged) station pointer */
    return station; 
} 


// C function to search a given id in a given BST 
void search(station *root, int id) 
{ 
    // Base Cases: root is null or id is present at root 
    if (root == NULL){ 
       printf("no such station exists"); 
       return;
    }
    else if(root->id == id){
        printf("station name: %s found with given id %d",root->name,root->id);
        return;
    }
    // id is greater than root's id 
    if (root->id < id) 
       return search(root->right, id); 
  
    // id is smaller than root's id 
    return search(root->left, id); 
} 













struct btreeNode *numbroot;

/* creating new node */
struct btreeNode * createNode(long long int val, struct btreeNode *child, int id) {
struct btreeNode *newNode;
newNode = (struct btreeNode *)malloc(sizeof(struct btreeNode));
user* temp;
temp = (user*)malloc(sizeof(user));
temp->num=val;
temp->stnid = id;
newNode->val[1] = temp;
newNode->count = 1;
newNode->link[0] = numbroot;
newNode->link[1] = child;
return newNode;
}

/* Places the value in appropriate position */
void addValToNode(long long int val, int pos, struct btreeNode *node,
                struct btreeNode *child, int id) {
int j = node->count;
while (j > pos) {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
}
user* temp;
temp = (user*)malloc(sizeof(user));
temp->num=val;
//stnid
temp->stnid = id;
node->val[j + 1] = temp;
node->link[j + 1] = child;
node->count++;
}

/* split the node */
void splitNode (long long int val, long long int *pval, int pos, struct btreeNode *node,
struct btreeNode *child, struct btreeNode **newNode, int id) {
int median, j;

if (pos > MIN)
        median = MIN + 1;
else
        median = MIN;

*newNode = (struct btreeNode *)malloc(sizeof(struct btreeNode));
j = median + 1;
while (j <= MAX) {
        (*newNode)->val[j - median] = node->val[j];
        (*newNode)->link[j - median] = node->link[j];
        j++;
}
node->count = median;
(*newNode)->count = MAX - median;

if (pos <= MIN) {
        addValToNode(val, pos, node, child, id);
} else {
        addValToNode(val, pos - median, *newNode, child, id);
}
*pval = (node->val[node->count])->num;
(*newNode)->link[0] = node->link[node->count];
node->count--;
}

/* sets the value val in the node */
long long int setValueInNode(long long int val, long long int *pval,
struct btreeNode *node, struct btreeNode **child, int id) {

int pos;
if (!node) {
        *pval = val;
        *child = NULL;
        return 1;
}

if (val < (node->val[1])->num) {
        pos = 0;
} else {
        for (pos = node->count;
                (val < (node->val[pos]->num) && pos > 1); pos--);
        if (val == (node->val[pos])->num) {
                printf("Duplicates not allowed\n");
                return 0;
        }
}
if (setValueInNode(val, pval, node->link[pos], child, id)) {
        if (node->count < MAX) {
                addValToNode(*pval, pos, node, *child, id);
        } else {
                splitNode(*pval, pval, pos, node, *child, child, id);
                return 1;
        }
}
return 0;
}

/* insert val in B-Tree */
void insertion(long long int val, int id) {
int flag;
long long int i;
struct btreeNode *child;

flag = setValueInNode(val, &i, numbroot, &child, id);
if (flag)
        numbroot = createNode(i, child, id);
}

/* copy successor for the value to be deleted */
void copySuccessor(struct btreeNode *myNode, int pos) {
struct btreeNode *dummy;
dummy = myNode->link[pos];

for (;dummy->link[0] != NULL;)
        dummy = dummy->link[0];
myNode->val[pos] = dummy->val[1];

}

/* removes the value from the given node and rearrange values */
void removeVal(struct btreeNode *myNode, int pos) {
int i = pos + 1;
while (i <= myNode->count) {
        myNode->val[i - 1] = myNode->val[i];
        myNode->link[i - 1] = myNode->link[i];
        i++;
}
myNode->count--;
}

/* shifts value from parent to right child */
void doRightShift(struct btreeNode *myNode, int pos) {
struct btreeNode *x = myNode->link[pos];
int j = x->count;

while (j > 0) {
        x->val[j + 1] = x->val[j];
        x->link[j + 1] = x->link[j];
}
x->val[1] = myNode->val[pos];
x->link[1] = x->link[0];
x->count++;

x = myNode->link[pos - 1];
myNode->val[pos] = x->val[x->count];
myNode->link[pos] = x->link[x->count];
x->count--;
return;
}

/* shifts value from parent to left child */
void doLeftShift(struct btreeNode *myNode, int pos) {
int j = 1;
struct btreeNode *x = myNode->link[pos - 1];

x->count++;
x->val[x->count] = myNode->val[pos];
x->link[x->count] = myNode->link[pos]->link[0];

x = myNode->link[pos];
myNode->val[pos] = x->val[1];
x->link[0] = x->link[1];
x->count--;

while (j <= x->count) {
        x->val[j] = x->val[j + 1];
        x->link[j] = x->link[j + 1];
        j++;
}
return;
}

/* merge nodes */
void mergeNodes(struct btreeNode *myNode, int pos) {
int j = 1;
struct btreeNode *x1 = myNode->link[pos], *x2 = myNode->link[pos - 1];

x2->count++;
x2->val[x2->count] = myNode->val[pos];
x2->link[x2->count] = myNode->link[0];

while (j <= x1->count) {
        x2->count++;
        x2->val[x2->count] = x1->val[j];
        x2->link[x2->count] = x1->link[j];
        j++;
}

j = pos;
while (j < myNode->count) {
        myNode->val[j] = myNode->val[j + 1];
        myNode->link[j] = myNode->link[j + 1];
        j++;
}
myNode->count--;
free(x1);
}

/* adjusts the given node */
void adjustNode(struct btreeNode *myNode, int pos) {
if (!pos) {
        if (myNode->link[1]->count > MIN) {
                doLeftShift(myNode, 1);
        } else {
                mergeNodes(myNode, 1);
        }
} else {
        if (myNode->count != pos) {
                if(myNode->link[pos - 1]->count > MIN) {
                        doRightShift(myNode, pos);
                } else {
                        if (myNode->link[pos + 1]->count > MIN) {
                                doLeftShift(myNode, pos + 1);
                        } else {
                                mergeNodes(myNode, pos);
                        }
                }
        } else {
                if (myNode->link[pos - 1]->count > MIN)
                        doRightShift(myNode, pos);
                else
                        mergeNodes(myNode, pos);
        }
}
}

/* delete val from the node */
int delValFromNode(long long int val, struct btreeNode *myNode) {
int pos, flag = 0;
if (myNode) {
        if (val < (myNode->val[1])->num) {
                pos = 0;
                flag = 0;
        } else {
                for (pos = myNode->count;
                        (val < (myNode->val[pos])->num && pos > 1); pos--);
                        if (val == (myNode->val[pos])->num) {
                        flag = 1;
                } else {
                        flag = 0;
                }
        }
        if (flag) {
                if (myNode->link[pos - 1]) {
                        copySuccessor(myNode, pos);
                        flag = delValFromNode((myNode->val[pos])->num, myNode->link[pos]);
                        if (flag == 0) {
                                printf("Given data is not present in B-Tree\n");
                        }
                } else {
                        removeVal(myNode, pos);
                }
        } else {
                flag = delValFromNode(val, myNode->link[pos]);
        }
        if (myNode->link[pos]) {
                if (myNode->link[pos]->count < MIN)
                        adjustNode(myNode, pos);
        }
}
return flag;
}

/* delete val from B-tree */
void deletion(long long int val, struct btreeNode *myNode) {
struct btreeNode *tmp;
if (!delValFromNode(val, myNode)) {
        printf("Given value is not present in B-Tree\n");
        return;
} else {
        if (myNode->count == 0) {
                tmp = myNode;
                myNode = myNode->link[0];
                free(tmp);
        }
}
numbroot = myNode;
return;
}

/* search val in B-Tree */
void searching(long long int val, int *pos, struct btreeNode *myNode, station *root) {
if (!myNode) {
        printf("no such number exists");
        return;
}

if (val < (myNode->val[1])->num) {
        *pos = 0;
} else {
        for (*pos = myNode->count;
                (val < (myNode->val[*pos])->num && *pos > 1); (*pos)--);
        if (val == (myNode->val[*pos])->num) {
                printf("Given data %lld is present in B-Tree with ", val,(myNode->val[*pos])->stnid);
                search(root, (myNode->val[*pos])->stnid);
                return;
        }
}
searching(val, pos, myNode->link[*pos], root);
return;
}

/* B-Tree Traversal */
void traversal(struct btreeNode *myNode) {
int i;
if (myNode) {
        for (i = 0; i < myNode->count; i++) {
                traversal(myNode->link[i]);
                printf("%lld at station %d\n", (myNode->val[i + 1])->num,(myNode->val[i + 1])->stnid);
        }
        printf("\n");
        traversal(myNode->link[i]);
}
}

int isempty(btn *mynode, int id){
        int i;
        if (mynode) {
                for (i = 0; i < mynode->count; i++) {
                        isempty(mynode->link[i], id);
                        if((mynode->val[i + 1])->stnid==id){
                                
                                return 1;
                        }
                        
                }
                
                isempty(mynode->link[i], id);
        }
        return 0;
}

int ismember(btn *myNode,int *pos,long long int val, int tid){
        if (!myNode) {
                return 0;
        }

        if (val < (myNode->val[1])->num) {
                *pos = 0;
        } else {
                for (*pos = myNode->count;
                        (val < (myNode->val[*pos])->num && *pos > 1); (*pos)--);
                if (val == (myNode->val[*pos])->num && tid == (myNode->val[*pos])->stnid) {
                        return 1;
                }
        }
        ismember(myNode->link[*pos], pos, val, tid);
        
}


















































int main() {
long long int val;
int ch,id,tid;



struct station *root = NULL; 
root = insert(root, 4, "mains"); 
insert(root, 2, "subleft"); 
insert(root, 1, "base ll"); 
insert(root, 3, "base lr"); 
insert(root, 6, "subright"); 
insert(root, 5, "base rl"); 
insert(root, 7, "base rr");
        



while (1) {
        printf("1. Insertion\t2. Deletion\n");
        printf("3. Searching\t4. Traversal\n");
        printf("5. isempty\t 6. isMember\n7.\t8. Exit\nEnter your choice:");
        scanf("%lld", &ch);
        switch (ch) {
                case 1:
                        printf("Enter your input:");
                        scanf("%lld %d", &val,&id);
                        insertion(val,id);
                        break;
                case 2:
                        printf("Enter the element to delete:");
                        scanf("%lld", &val);
                        deletion(val, numbroot);
                        break;
                case 3:
                        printf("Enter the element to search:");
                        scanf("%lld", &val);
                        searching(val, &ch, numbroot, root);
                        break;
                case 4:
                        traversal(numbroot);
                        break;
                case 5:
                        printf("enter id\n");
                        scanf("%d",&tid);
                        if(isempty(numbroot, tid)==0){
                                printf("False");
                        }
                        else{
                                printf("True");
                        }
                        break;
                case 6:
                        printf("enter number and id");
                        scanf("%d %d",&val,&tid);
                        if(ismember(numbroot,&ch, val, tid)==0){
                                printf("False");
                        }
                        else{
                                printf("True");
                        }
                        break;
                case 7:

                case 8:
                        exit(0);
                default:
                        printf("U have entered wrong option!!\n");
                        break;
        }
        printf("\n");
}
}
