#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "mmu.h" 

/*
 * This is where you'll need to implement the user-level functions
 */
int ustack; 
	
void lock_init(lock_t *lock) {
	lock->flag = 0;
}

void lock_acquire(lock_t *lock) {
	
}

void lock_release(lock_t *lock) {
}

int thread_join(int pid) {
	int new_pid = join(pid);
	free((void*)ustack); 
	return new_pid; 
}

int thread_create(void (*start_routine)(void *), void *arg) {
	ustack = (int)malloc(sizeof(int) * (PGSIZE));
    int pid = clone(*start_routine, arg, (void*)ustack);
	return pid; 
}
