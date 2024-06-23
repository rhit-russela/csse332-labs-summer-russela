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
 * For testing purposes, we need to record where our memory starts. Generally
 * this is hidden from the users of the library but we're just using it here to
 * make our tests more meaningful.
 */
static void *heap_mem_start = 0;

/**
 * Head of the free list. It is actually a pointer to the header of the first
 * free block.
 *
 * @warning In this assignment, "freelist" is somewhat of a misnomer, because
 * this list contains both free and unfree nodes.
 */
static metadata_t *freelist = 0;

/**
 * Return a pointer to the metadata of the head of the free list.
 *
 * @return The freelist pointer.
 */
metadata_t *freelist_head(void) { return freelist; }

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
  heap_mem_start = sbrk(0);

  /* grow the memory area by MAX_HEAP_SIZE bytes */
  p = sbrk(MAX_HEAP_SIZE);
  if(p == (char *)-1) {
    fprintf(2, "sbrk failed:exiting....\n");
    exit(1);
  }

  /* TODO: Add code here to initialize freelist and its content. */

  freelist = (struct metadata*)heap_mem_start;
  freelist->in_use = 0;
  freelist->size = MAX_HEAP_SIZE - sizeof(metadata_t);
  freelist->prev = 0;
  freelist->next = 0;

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

  freelist = 0;
  heap_mem_start = 0;
}

/**
 * Allocate size bytes and return a pointer to start of the region. 
 * 
 * @return A valid void ptr if there is enough room, 0 on error. 
 */
void *rhmalloc(uint32 size)
{
  /* Check if we need to call rhmalloc_init and call it if needed. */
  if(!freelist)
    if(rhmalloc_init()) return 0;

  /* TODO: Add you malloc code here. */
  metadata_t *temp = 0;

  unsigned bytes_for_user = ALIGN(size);


  //find the first block that is free that is also the size or greater
  while(freelist != 0){
  if((freelist->in_use == 0) && ((freelist->size + sizeof(metadata_t)) >= (bytes_for_user + sizeof(metadata_t)))){
      break;
    }
  if(freelist->next != 0){
    freelist = freelist->next;
  }else{
    //no more memory
    return 0;
  }
  }

    if(freelist == (freelist + MAX_HEAP_SIZE) || freelist->in_use == 1){
    temp = freelist;
    freelist = (struct metadata*)heap_mem_start;
    return (void*)temp;
  }

  //check the size of freelist block. If block is greater than 2*sizeof(metadata_t*) + ALGIN(size)
  //then split [since there is enough memory to fit another metadata + usable memory]
  if((int)(freelist->size - (bytes_for_user)) >= (int)(sizeof(metadata_t) + ALIGN(8))){
    //split
    //where new pointer ends;
    metadata_t* splitMemNode = (metadata_t*)((void*)(freelist) +
					     bytes_for_user + sizeof(metadata_t));
    
    splitMemNode->in_use = 0;
    splitMemNode->size = freelist->size - (sizeof(metadata_t) + bytes_for_user);
    splitMemNode->prev = freelist;
    if(freelist->next != 0){
    splitMemNode->next = freelist->next;
    }

    //make the last block in use and readjust
    freelist->size = bytes_for_user;
    freelist->in_use = 1;
    if(freelist->next != 0){
      freelist->next->prev = splitMemNode;
    }
    freelist->next = splitMemNode;

    
  }else{
    //don't split and use
    freelist->in_use = 1;
    
    temp = freelist;
    freelist = (struct metadata*)heap_mem_start;
    return (void*)(temp) + sizeof(metadata_t);
  }
  temp = freelist;
  freelist = (struct metadata*)heap_mem_start;
  return (void*)(temp) + sizeof(metadata_t);
}

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
  /* TODO: Add your free code here. */
  metadata_t* freeNode = (struct metadata*)(ptr - sizeof(metadata_t));

  //find node in freelist
  freelist = freeNode;
  
  freelist->in_use = 0;
   
  //free + merge
  if(freelist->next->in_use == 0 && freelist->next != 0){
     //change the size (merge)
     freelist->size = freelist->size + freelist->next->size + sizeof(metadata_t);

     freelist->next = freelist->next->next;

     if(freelist->next->in_use != 0){
       freelist->next->prev = freelist;
      }
   }

  if(freelist->prev != 0 && freelist->prev->in_use == 0){
     //change size
     freelist->prev->size = freelist->prev->size + freelist->size + sizeof(metadata_t);

     freelist->prev->next = freelist->next;
     
     //change address
      if(freelist->next != 0){
	freelist->next->prev = freelist->prev;
      }
      
       freelist = freelist->prev;
       
     }
   
}
