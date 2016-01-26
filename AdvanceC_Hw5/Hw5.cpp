#include "homework5.h"

/*	==================== insert ====================
	This function uses recursion to insert the new data 
	into a leaf node in the BST tree.
	   Pre:   Application has called insert, which  
	          passes root by two level and data pointer
			  comp is a pointer to a function inorder to compare data
	   Post:   Data have been inserted
	   Return: 1 data inserted or 0 duplicate*/
int insert( NODE **root, char *data, int line, int (*comp)(void*, void*)){
   if(!(*root)){ // parent found: insert data
      // allocate the new node
      *root = (NODE *) malloc (sizeof(NODE));
      strcpy((*root)->identifier, data);
      (*root)->left  = (*root)->right = NULL;
	  (*root)->queue.queue = NULL;
	  enqueue(&(*root)->queue.queue, &(*root)->queue.rear, line);
      return 1; // data inserted
   }

   if (comp(data, (*root)->identifier) > 0)
      return insert(&(*root)->right, data, line, comp);
   else if (comp(data, (*root)->identifier) < 0)
      return insert(&(*root)->left, data, line, comp);
   else
   {
	   if((*root)->queue.rear->line != line) 
			enqueue(&(*root)->queue.queue, &(*root)->queue.rear, line);
       return 2;
   }
}
/*	==================== find ===================  
	Retrieve node searches tree for the node containing 9
	the requested key and returns pointer to its data.
	   Pre:    A pointer to the tree has been created (may be null) 
	           target is the key to find 
			   comp is a pointer to a function 
	   Post:    Tree searched and data pointer returned
	   Return:  Address of matching node returned 
	           If not found, NULL returned */
NODE *find(NODE *root, char* target, int (*comp)(void*, void*))
{
   if (root)
   {
       if (comp(target , root->identifier) < 0)
           return find(root->left, target, comp);
       else
           if (comp(target , root->identifier) > 0)
               return find(root->right, target, comp);
           else
               return root;
   }
   return NULL;
}
/*	=============== destroy ============== 
	Deletes all data in the tree and recycles memory.
	It also recycles memory for the name and account structure.
	   Pre:      proot is a pointer to the valid tree/subtree 
	   Post:     All data and structures are deleted 
	   Return:   null pointer */
NODE *destroy(NODE *root){
	if( root ){
		  destroy( root->left);
		  QUEUELIST *f;
	      while(f = root->queue.queue){			  
		      root->queue.queue =  root->queue.queue->next;
			  free(f);
		  }
		  root->queue.queue = root->queue.rear = NULL;
		  destroy( root->right);
		  free(root);
	   }
	return NULL;
}

/***************************************************
This function prints the tree and the queue
Pre:  pointer to file and pointer to node
Post: prints the tree and the queue*/
void print(FILE* output, NODE* node)
{	
	if( node ){
		QUEUELIST *ptr = node->queue.queue;
		int counter = 0;

		print(output, node->left);
		fprintf(output, " %-25s", node->identifier);
		while(ptr)
		{
			fprintf(output, "\t%3d", ptr->line);
			ptr = ptr->next;
			counter++;
			if(!(counter % 6) && ptr)
				fprintf(output, "\n\t\t\t");
		}
		fprintf(output, "\n");
		print(output, node->right);
	}
	return;
}

/***************************************************
This function inserts a node to the queue
Pre:  Two level pointers to the front and rear of the queue
      The structure of the node to insert
Post: Node inserted to the queue*/
void enqueue(QUEUELIST **queue, QUEUELIST **rear, int data)
{
    QUEUELIST *pnew;

    pnew = (QUEUELIST*) malloc(sizeof (QUEUELIST));
    if (!pnew)
    {
        printf("... error in enqueue!\n");
        exit(1);
    }
    pnew->line = data;
    pnew->next = NULL;
    if (*queue == NULL) 
		*queue = pnew; 
    else 
		(*rear)->next = pnew;   
    *rear = pnew;
    return;
}
