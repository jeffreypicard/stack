;
; push_pop.asm
;
; Assembly functions to push and pop for a lock-free
; concurrent stack.
;
; x86_64 Version
;
; Author: Jeffrey Picard
;

extern get_link

section .text
  global push_asm 
  global pop_asm

push_asm:
  push rbp                  ; Save the old frame pointer.
  mov  rbp,rsp              ; Establish new frame pointer.
;  
  push r12                  ; Save callee saved registers
  push r13
  push r14
  push r15
;
  mov  r12,rdi              ; &oldhead
  mov  r13,rsi              ; &newhead->link
  mov  r14,[rdx]            ; &newhead
  ;mov  r15,[rdi]
;
spin:
;
  mov  r15,[r12]            ; r15 <- oldhead
  mov  rax,r15              ; rax <- oldhead
  mov  [r13],r15            ; newhead->link <- oldhead
  lock cmpxchg [r12],r14    ; oldhead = newhead
  jnz   spin
;
  pop  r15                  ; Return callee saved registers
  pop  r14
  pop  r13
  pop  r12
;
  xor rax,rax               ; return 0
  mov rsp,rbp               ; Re-establish stack pointer
  pop rbp                   ; Re-establish old frame pointer
  ret                       ; Return

pop_asm:
  push rbp                  ; Save the old frame pointer.
  mov  rbp,rsp              ; Establish new frame pointer.
;  
  push r12                  ; Save callee saved registers
  push r13
  push r14
  push r15
;
  mov  r12,rdi              ; &oldhead
;
spin2:
;
  mov  r15,[r12]            ; oldhead
  mov  rdi,r15              
  call get_link             ; oldhead->link
  mov  r13,rax              ; r13 <- oldhead->link
  mov  rax,r15              ; rax <- oldhead (return value)
  lock cmpxchg [r12],r13    ; oldhead <- newhead
  jnz   spin2
;
  pop  r15                  ; return callee saved registers
  pop  r14
  pop  r13
  pop  r12
;
  mov rsp,rbp               ; Re-establish stack pointer
  pop rbp                   ; Re-establish old frame pointer
  ret                       ; Return
