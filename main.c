#include <stdio.h>
#include <stdlib.h>

// declare structure to store block information (id, starting address, ending address, link to next block)
struct node {
    int id, start, end;
    struct node *link;
} *block_list = NULL;

typedef struct node block_type;

// declare global variables
	int pm_size;
	int hole_algo;
	int remaining;


/********************************************************************/
void parameter() {
	
	// declare local variables (if any)
	// prompt for size of physical memory and choice of hole-fitting 
	//algorithm (0=first-fit, 1=best-fit), initialize remaining memory 
	
	printf("\nEnter size of physical memory: ");
	scanf("%d", &pm_size);
	remaining = pm_size;

		printf("\nEnter hole-fitting algorithm (0=first fit, 1=best_fit): ");
	scanf("%d", &hole_algo);
	printf("\n");
	
	// initilize linked list with "dummy" block of size 0 and i = -1
	block_list = (block_type *)malloc(sizeof(block_type));
	block_list->id = -1;
	block_list->start = 0;
	block_list->end = 0;
	block_list->link = NULL;
	
    return;

}


/********************************************************************/
void print_blocks() {
	
	// declare local variables 
	block_type * next_block = block_list->link;
	
	//print the header row of the field names
	printf("\nID    Start   End\n");
	printf("-------------------\n");
	
	while (next_block != NULL){
	    // print table containing block id, starting address, ending address
	    printf("%d\t%d\t%d\n", next_block->id, next_block->start, next_block->end);
	    next_block = next_block->link;
	}//end while loop
	
	return;
}


/********************************************************************/
void Allocate() {
	
	// declare local variables
int block_id;
int block_size;
block_type *next_block, *new_block, *best_block;
int hole_start, hole_end;
int best_start, best_end;

	// initialize best hole so far to size of physical memory
	int best_hole = pm_size;
	
	// prompt for block id & block size
	printf("Enter block id: ");
	scanf("%d", &block_id);
	printf("Enter block size: ");
	scanf("%d", &block_size);
	
	// check if size of block is larger than remaining unallocated space, if not, return	
	if (block_size > remaining){
	    printf("Im out of memory, nothing I can do. Block is too big, I cannot fit it, I give up.");
	    return;
	}// end if
	
	// allocate space for new block and set id
	new_block = (block_type *)malloc(sizeof(block_type));
	new_block->id = block_id;
		

	
	// if only "dummy" block exists, insert block at end of linked list, set fields, return
	if(block_list == NULL){
	    printf("block list is nULL");
	    return;
	}
		if( block_list->link == NULL){
		    new_block->start = block_list->end;
		    new_block->end = new_block->start + block_size;
		    new_block->link = NULL;
		    block_list->link = new_block;
		    print_blocks();
		    return;
		}//end if
		
		// else traverse list until either appropriate hole is found or the end of the list is reached
		else {
		    next_block = block_list->link;
		    
		    while (next_block != NULL){
		        if (next_block->id == block_id){
		            printf("ID duplucate\n");
		            return;
		    }//end if
		    next_block = next_block->link;
		}//end while
		
			// if id already exists, reject request and return set values for start and end of currently found hol
		next_block = block_list->link;
		while (next_block != NULL){
			
			hole_start = next_block->end;
			if (next_block->link == NULL){
			    hole_end = pm_size;
			}else
        			hole_end = next_block->link->start;
        			// if hole is large enough for block
                    if (block_size <= (hole_end - hole_start)) {
    			    		// if first-fit algorithm
    					if (hole_algo == 0){
    					   // set start & end fields of new block & add block into linked list
    					    new_block->start = hole_start;
    					    new_block->end = hole_start + block_size;
    					    new_block->link = next_block->link;
                            next_block->link = new_block;
                            // reduce remaining available memory and return
    					    remaining -= block_size;
    					    return;
    					    
    					} // end if statement for the first fit
				

					// else--best-fit algorithm
    					else{
                       
                        // if hole is smaller than best so far
                        if((hole_end - hole_start) < best_hole){
                            // set values of best start & best end & best hole size so far
                            best_hole = hole_end - hole_start;
                            best_start = hole_start;
                            best_end = hole_end;
                            best_block = next_block;
                        }//end of the if best so far
					}//end else 
			}// if the block fit
			next_block = next_block->link;
		} //end while loop
		new_block->start = best_start;
    	new_block->end = new_block->start + block_size;
    	new_block->link = best_block->link;
    	best_block->link = new_block;
	    remaining -= block_size;
    } // end else of traversal
    print_blocks();
	return;	
}
	

/********************************************************************/
void Deallocate() {
	// declare local variables
	int block_id;
	block_type *next_block = block_list->link;
	block_type *prev_block;
	
	// prompt for block id
printf("Enter block id: ");
	scanf("%d", &block_id);
	
	// until end of linked list is reached or block id is found traverse list
	while ((next_block != NULL) && (next_block->id != block_id)){
    	prev_block = next_block;
    	next_block = next_block->link;
	} // end while loop of trasversal
	
	// if end of linked list reached, print block id not found
	if (next_block == NULL){
	    printf("I didnt find the block");
	    return;
	}
	// else remove block and deallocate memory
	else {
	    prev_block->link = next_block->link;
	    remaining += (next_block->end - next_block->start);
	    free (next_block);
	} //end else
	return;
}


/********************************************************************/
void Defragment() {
	// declare local variables 	   
block_type* next_block;
next_block = block_list->link;
int next_size;
int prev_end = 0;
	// until end of list is reached
	while (next_block != NULL){
			// calculate current hole size
			next_size = next_block->end - next_block->start;
			next_block->start = prev_end;
			next_block->end = next_block->start + next_size;
			prev_end = next_block->end;
    	    next_block = next_block->link;
    } // end while 
    print_blocks();
	return;
}


/********************************************************************/
void quits(block_type *node) {
	
	if (node == NULL)
	return;
	
	else
	    quits(node->link);
		free (node);
    return;
}


/***************************************************************/
int main() {

/* Declare local variables */
	int choice;
	
	/* Until the user quits, print the menu, prompt for the menu choice, call the appropriate procedure */
while ( choice != 5)
{
    printf("Memory allocation\n");
	printf("-----------------\n");
	printf("1) Enter parameters\n");
	printf("2) Allocate memory for block\n");
	printf("3) Deallocate memory for block \n");
	printf("4) Defragment memory \n");
	printf("5) Quit program\n\n");

	printf("Enter selection: ");
	scanf("%d", &choice);

	
	if (choice == 1)
	{
	   parameter();
	}
	
	else if (choice == 2)
	{
	   Allocate();
	}
	
	else if (choice == 3)
	{
	   Deallocate();
	}
		else if (choice == 4)
	{
	   Defragment();
	}
		else if (choice == 5)
	{
	   quits(block_list);
	}

} //end while loop

  return 1;
}
