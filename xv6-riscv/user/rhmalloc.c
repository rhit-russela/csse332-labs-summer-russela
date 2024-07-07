/****************************************************************************
 * Copyright © 2022 Rose-Hulman Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ****************************************************************************/
#include "kernel/types.h"
#include "user/user.h"
#include "user/rhmalloc.h"

/**
 * Record the original start of our memory area in case we needed to allocate
 * more stuff to align our target start address.
*/
static void *original_start = 0;

/**
 * For testing purposes, we need to record where our memory starts. Generally
 * this is hidden from the users of the library but we're just using it here to
 * make our tests more meaningful.
 */
static void *heap_mem_start = 0;

// Add a new freelist to use
static metadata_t* freelist;

//Keep track of current not in_use chunk
static metadata_t* currentFree;

//static metadata_t** pointerList;
//int arraySize;
/**
 * Check if the freelist has been initialized, if not, call rhmalloc_init()
*/
static int initialized = 0;

/**
 * For testing purposes, exposed the initialization bit.
*/
int is_initialized(void) { return initialized; }

/**
 * Return the pointer to the start of the heap memory.
 * 
 * @return The heam_mem_start ptr.
 */
void *heap_start(void) { return heap_mem_start; }

/**
 * Initialize the rh memroy allocator system.
 *
 * @return 0 on success, non-zero number on failure.
 */
uint8 rhmalloc_init(void)
{
  char *p;

  /* Grab the start of the memory where we are allocating. */
  original_start = sbrk(0);

  /* keep allocating useless crap until we hit our target starting address */
  p = sbrk(TARGET_START - original_start);

  /* grow the memory area by MAX_HEAP_SIZE bytes */
  p = sbrk(MAX_HEAP_SIZE);
  if(p == (char *)-1) {
    fprintf(2, "sbrk failed:exiting....\n");
    exit(1);
  }

  /* check if we matched the target start */
  if(p != TARGET_START) {
    fprintf(2, "sbrk failed: cannot get to target start\n");
    exit(1);
  }

  /* check if lower 21 bits are all zeros */
  if((uint64)p & (uint64)0x1FFFFF) {
    fprintf(2, "sbrk failed: cannot get good start of memory...\n");
    exit(1);
  }

  /* update the heap start */
  heap_mem_start = p;

  // mark it as initialized
  initialized = 1;

  // TODO: Add your initialization code here, but do not change anything above
  // this line.
  freelist = heap_mem_start;
  freelist->next = 0;
  freelist->prev = 0;
  freelist->size = ALIGN(MAX_HEAP_SIZE) - sizeof(metadata_t);
  freelist->in_use = 0;
  currentFree = freelist;

  /*  int size = MAX_HEAP_SIZE;
  arraySize = 0;
  while(size >= 32){
    size = size/2;
    arraySize++;
  }

  metadata_t* initializedList[arraySize];
  pointerList = initializedList;
  int j = 0;
  while(j < arraySize){
    pointerList[j] = 0;
    
    printf("LOOP pointerList[%d] = %p\n", j, pointerList[j]);
    j++;
  }

  //initialize pointerlist
  pointerList[17] = freelist;
  printf("INITIALIZE pointerList[17] = %p\n", pointerList[17]);*/
  return 0;
}

/**
 * Deallocates everything and frees back all the memory to the operating system.
 *
 * This routine is useful to do between tests so that we can reset everything.
 * You should not need to modify this routine though if you use global
 * variables, it might be useful to reset their values here.
 */
void rhfree_all(void)
{
  /* Imagine what would happen on a double free, yikes! */
  sbrk(-MAX_HEAP_SIZE);

  /* move back if we did have to allocate more stuff */
  sbrk(-(TARGET_START - original_start));

  heap_mem_start = 0;
  original_start = 0;
  initialized = 0;

  // TODO: Add your destruction code here, but do not change anything above this
  // line.

}

/**
 * Grab the pointer to the buddy region, given a pointer to a memory chunk of a
 * given size.
 * 
 * @param ptr               A pointer to a given memory chunk of a given size.
 * @param exponent          The exponent representing the size. 
 * 
 * @return A pointer to the buddy region, i.e., the adjacent region of the same
 * size with only 1 bit difference.
 * 
*/
void *get_buddy(void *ptr, int exponent)
{
  // TODO: Add your code here.
  
  return (void*)((unsigned long)ptr ^ (1 << exponent));
}

/*int index_offset(int size){
  int offset = 0;
  int count = 32;
  while(size > count){
    count = count * 2;
    offset++;
  }
  return offset;  
  }*/

/**
 * Allocate size bytes and return a pointer to start of the region. 
 * 
 * @return A valid void ptr if there is enough room, 0 on error. 
 */
void *rhmalloc(uint32 size)
{
  /* Check if we need to call rhmalloc_init and call it if needed. */
  if(!initialized)
    if(rhmalloc_init()) return 0;

  // TODO: Add your malloc code here.
   int allocate_size = ALIGN(size);
  //check if the size is minimum and make sure the allocated size is power of 2
  if(ALIGN(size) + sizeof(metadata_t) <= 32){
    allocate_size = 32;
  }else if(size % 2 != 0){
    int i = 64;
    int target = size;
    while(i < target){
      i = i * 2;
    }
    allocate_size = i;
  }
  

  //check if in use
  freelist = currentFree;

  while(freelist->size > 32){
   if(freelist->in_use == 0){
    if(freelist->size >= (allocate_size + 32)){
      //split
      metadata_t* newNode;
      newNode = (metadata_t*)((void*)freelist + allocate_size);
      newNode->size = ((freelist->size) - sizeof(metadata_t) - allocate_size);
      newNode->in_use = 0;
      newNode->prev = freelist;

      freelist->size = allocate_size;
      metadata_t* prevNext = freelist->next;
      freelist->next = newNode;
      newNode->next = prevNext;      
    }
    // no split + aftermath of splitting
    freelist->in_use = 1;
    currentFree = freelist->next;
    return (void*)freelist + sizeof(metadata_t);
   }
   //keep searching
    freelist = freelist->next;
    currentFree = freelist;
  }
  
  return (void*)0;
}

  //what does pointerList look like right now?
  /* int j = 0;
  while(j < arraySize){
    pointerList[j] = 0;
    printf("LOOP START MALLOC pointerList[%d] = %p\n", j, pointerList[j]);
    j++;
  }
  
  int allocate_size = ALIGN(size);
  printf("This is the size they're looking for: %d\n", size);
  //check if the size is minimum and make sure the allocated size is power of 2
  if(ALIGN(size) + sizeof(metadata_t) <= 32){
    allocate_size = index_offset(ALIGN(32 + sizeof(metadata_t)));
  }else{
    allocate_size = index_offset(ALIGN(size + sizeof(metadata_t)));
  }
  printf("This is the offset I'm making them look for: %d\n", allocate_size);

  //check if in use
  int index = allocate_size;
  while(pointerList[index] == 0 && index < arraySize){
    printf("LOOP pointerList[%d] = %p\n", index, pointerList[index]);
    index++;
  }

  printf("AT ZERO? pointerList[0] = %p\n", pointerList[0]);
  printf("ALLOCATE? pointerList[allocate_size] = %p\n", pointerList[allocate_size]);
  printf("Where is index? : %d\n", index);

  while(index > allocate_size){
   printf("LOOP Where is index? : %d\n", index);
    //make head nodes and chained nodes
    //split in 2 constantly
    metadata_t* newNode = (metadata_t*)((void*)freelist + (freelist->size + sizeof(metadata_t))/2);
    newNode->size = (freelist->size + sizeof(metadata_t))/2 - sizeof(metadata_t);
    printf("LOOP Hello from the other side of *math*\n");
    newNode->in_use = 0;
    newNode->next = 0;
    newNode->prev = freelist;

    printf("LOOP Hello from the other side of *newNode*\n");
    freelist->size = newNode->size;
    metadata_t* next = freelist->next;
    freelist->next = newNode;

    
    printf("LOOP Hello from the other side of *freelist*\n");
    
    pointerList[index-1] = freelist;
    pointerList[index] = next;

    printf("LOOP pointerList[index-1] = %p\n", pointerList[index-1]);
    printf("LOOP pointerList[index] = %p\n", pointerList[index]);
    
    
    index--;     
  }
  // in the correct index
  pointerList[index]->in_use = 1;
  metadata_t* returnPointer = pointerList[index];
  printf("returnPointer [no metadata] = %p\n", pointerList[index]);

  pointerList[index] = pointerList[index]->next;
  printf("pointerList[index] = %p\n", pointerList[index]);
  
  return (void*)returnPointer + sizeof(metadata_t);
}*/

/**
 * Free a memory region and return it to the memory allocator.
 *
 * @param ptr The pointer to free.
 *
 * @warning This routine is not responsible for making sure that the free
 * operation will not result in an error. If freeing a pointer that has already
 * been freed, undefined behavior may occur.
 */
void rhfree(void *ptr)
{
  // TODO: Add your free code here.
}
