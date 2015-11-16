#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "mmu.h" 
#include "spinlock.h"
#include "param.h"
#include "memlayout.h"
#include "proc.h"


/*
 * This is where you'll need to implement the user-level functions
 */

typedef struct __stackandpid {
	void *stack;
	int pid; 
} stackandpid;

stackandpid allstacks[64]; 

int num_stacks = 0; 
	
void lock_init(lock_t *lock) {
	lock->flag = 0;
}

void lock_acquire(lock_t *lock) {
	/*if (holding(lock)) {
		panic("acquire");
	} */

	while (xchg(&lock->flag, 1) != 0){}		//spin wait 
		
}

void lock_release(lock_t *lock) {
	/*if(!holding(lock)) {
		panic("release");
	}*/
	xchg(&lock->flag, 0);
}

int thread_join(int pid) {
	int new_pid; 
	int i;
    if (pid == -1) {
		new_pid = join(pid); 
		for (i = 0; i < num_stacks; i++) {
			if (allstacks[i].pid == pid) {
				free(allstacks[i].stack);
			}
		}
	}
	else {
		for (i = 0; i < num_stacks; i++) {
			if (allstacks[i].pid == pid) {
				new_pid = join(pid);
				free(allstacks[i].stack); 
			}
		}
	}
	//printf(1, "new_pid is: %d\n", new_pid); 
	return new_pid; 
}

int thread_create(void (*start_routine)(void *), void *arg) {
	void *ustack = malloc(2*PGSIZE);
	if((uint)ustack % PGSIZE)
     ustack = ustack + (PGSIZE - (uint)ustack % PGSIZE);
    printf(1, "ustack in thread_create is: %d\n", (int)ustack); 

	stackandpid *new_entry = &allstacks[num_stacks];
	new_entry->stack = ustack;
    int pid = clone(start_routine, arg, ustack);
	new_entry->pid = pid; 
	num_stacks++;
 
    printf(1, "pid in thread_create is: %d\n", pid); 
	return pid; 
}
