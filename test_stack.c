/*
 * test_stack.c
 *
 * Copyright (c) 2013, Jeffrey Picard
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies, 
 * either expressed or implied, of the FreeBSD Project.
 *
 * -----------------------------------------------------------------------------
 *
 * C program to test my lock free concurrent stack implementation in C.
 *
 * Author: Jeffrey Picard
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "stack.h"

/* macros */
#define EXIT_WITH_ERROR( ... ) do {   \
  fprintf( stderr, __VA_ARGS__ );     \
  exit(-1);                           \
} while(0)

#define MALLOC_CHECK( p, func ) do {    \
  if( !p )                              \
    EXIT_WITH_ERROR("Error: malloc failed in "#func );  \
} while(0)

/* structs */
/*struct node {
  int data;
  int thread;
  struct node *link;
} typedef node;*/

struct pt_args {
  int num_push, num_pop;
  int n;
} typedef pt_args;

/* function declarations */
void *work( void* );
void print_list( node *root );
void count_list( node *root, int n );
void usage( void );

/* global variables */
pthread_barrier_t bar;

NODE_TYPE *root;

int main( int argc, char **argv )
{
  int num_threads, pops_per_thread, pushes_per_thread;
  int i;
  pthread_t *threads;
  pt_args *args;

  if( argc != 4 )
    usage();

  num_threads = atoi( argv[1] );
  pops_per_thread = atoi( argv[2] );
  pushes_per_thread = atoi( argv[3] );

  if( 0 >= num_threads )
    EXIT_WITH_ERROR("Error: num_threads must be positive.\n");
  if( pops_per_thread > pushes_per_thread )
    EXIT_WITH_ERROR("Error: pushes_per_thread must be greater than or equal to pops_per_thread.\n");

  threads = calloc( num_threads, sizeof(pthread_t) );
  args    = calloc( num_threads, sizeof(pt_args) );
  MALLOC_CHECK( threads, main );
  MALLOC_CHECK( args, main );

  for( i = 0; i < num_threads; i++ )
  {
    args[i].n = i;
    args[i].num_push = pushes_per_thread;
    args[i].num_pop = pops_per_thread;
  }

  root = NULL;

  pthread_barrier_init( &bar, NULL, 1 );

  for( i = 0; i < num_threads; i++ )
    pthread_create( &threads[i], NULL, work, (void*) &args[i] );
  for( i = 0; i < num_threads; i++ )
    pthread_join( threads[i], NULL );

  //print_list( root );
  count_list( root, 0 );

  return 0;
}

void * work( void *arg )
{
  pt_args *args = (pt_args*) arg;
  int i = 0;
  long data;

  for( i = 0; i < args->num_push; i++ )
  {
    /*n = malloc( sizeof(node) );
    MALLOC_CHECK( n, work );*/
    data = rand();

    /*push_asm( &root, &n->link, &n );*/
    push( &root, (void*)data );
  }

  pthread_barrier_wait( &bar );

  for( i = 0; i < args->num_pop; i++ )
  {
    pop( &root, (void**)&data );
  }
  
  return 0;
}

void count_list( node *root, int n )
{
  node *walk = root;
  int count = 0;
  while( walk )
  {
    count++;
    walk = walk->link;
  }
  fprintf( stderr, "count: %d\n", count );
}

void print_list( node *root )
{
  if( !root )
  {
    fprintf( stderr, "NULL\n");
    return;
  }
  fprintf( stderr, "%ld\n", (long)root->data );
  print_list( root->link );
}

void usage( void )
{
  fprintf( stderr, "usage: test_stack num_threads pops_per_thread"
                   " pushes_per_thread\n");
  exit(-1);
}
