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

void *ustack; 
void *start_routine; 
	
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

int thread_join(int pid, void (*start_routine)(void *)) {
	int new_pid = join(pid);
    printf(1, "start_routine in thread_join is: %d\n", start_routine); 
	free(start_routine); 
	return new_pid; 
}

int thread_create(void (*start_routine)(void *), void *arg) {
	ustack = malloc(2*PGSIZE);
	start_routine = ustack;
    printf(1, "start_routine in thread_create is: %d\n", start_routine); 
	if((uint)ustack % PGSIZE)
     ustack = ustack + (PGSIZE - (uint)ustack % PGSIZE);
    printf(1, "ustack in thread_create is: %d\n", (int)ustack); 
    int pid = clone(*start_routine, arg, ustack);
    printf(1, "pid in thread_create is: %d\n", pid); 
	return pid; 
}
