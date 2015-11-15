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

//void *ustack;  //only good for a single thread; need to make it accomodate multiple threads 
	
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
	int new_pid = join(pid);
	free((void*)ustack); 
	return new_pid; 
}

int thread_create(void (*start_routine)(void *), void *arg) {
	ustack = (int*)malloc(2*PGSIZE);
    printf(1, "ustack in thread_create is: %d\n", (int)ustack); 
    int pid = clone(start_routine, arg, (void*)ustack);
    printf(1, "pid in thread_create is: %d\n", pid); 
	return pid; 
}
