
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/mm_types.h>
#include <linux/sched/task.h>

int display_task_details(struct task_struct *pptr)
{



	task_lock(pptr);

	pr_info("task struct ptr = 0x%lx\n",pptr);
	pr_info(" [ %s ] , pid = %d and tgid = %d\n",pptr->comm, pptr->pid, pptr->tgid);

	task_unlock(pptr);
}