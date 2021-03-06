/*
 * stack.h
 *
 * Copyright (c) 2012-2013, Jeffrey Picard
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
 * A simple implementation of a stack.
 *
 * Author: Jeffrey Picard
 */
#ifndef _STACK_H_
#define _STACK_H_

/* Be flexible */
#define NODE_TYPE node

struct NODE_TYPE {
  void *data;
  struct NODE_TYPE *link;
} typedef NODE_TYPE;

/*
 * push
 * 
 * Takes a pointer the top of the stack by reference and a pointer
 * to the new data as a void *.
 * Returns 0 on success, non-zero otherwise.
 */
int push( NODE_TYPE **, void * );

/*
 * pop
 *
 * Takes a pointer to the top of the stack by reference and pops it.
 * Result is returned through the second paramater.
 * Returns 0 on success, non-zero otherwise.
 */
int pop( NODE_TYPE **, void ** );

/*
 * get_link
 *
 * Takes a node and returns the node it links to.
 * This function is used in the assembly pop function
 * for implementing a lock free concurrent stack.
 */
NODE_TYPE *get_link( NODE_TYPE *);
#endif
