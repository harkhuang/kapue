#include <stdlib.h>
#include <pthread.h>


// 多线程的消费队列
struct job {
	struct job *j_next;
	struct job *j_prev;
	pthread_t   j_id;   /* tells which thread handles this job */
	/* ... more stuff here ... */
};
// 这是一个队列  里面有头尾指针  和读写锁
struct queue {
	struct job      *q_head;
	struct job      *q_tail;

	// 读写锁  可以对读加密  可以对写加密
	// 单独的读加密是共享的  但是不能获取写锁  写锁是不可以被和读锁共享的
	pthread_rwlock_t q_lock;
};

/*
 * Initialize a queue.
 */
int
queue_init(struct queue *qp)
{
	int err;

	qp->q_head = NULL;
	qp->q_tail = NULL;

	// 初始化了一个互斥锁
	err = pthread_rwlock_init(&qp->q_lock, NULL);
	if (err != 0)
		return(err);

	/* ... continue initialization ... */

	return(0);
}

/*
 * Insert a job at the head of the queue.
 * 向队列头插入一个job
 */
void
job_insert(struct queue *qp, struct job *jp)
{
	// 获取写锁
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->j_next = qp->q_head;
	jp->j_prev = NULL;
	if (qp->q_head != NULL)
		qp->q_head->j_prev = jp;
	else
		qp->q_tail = jp;	/* list was empty */
	qp->q_head = jp;
	pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Append a job on the tail of the queue.
 * 向队列尾部添加一个job
 */
void
job_append(struct queue *qp, struct job *jp)
{
	// 获取写锁
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->j_next = NULL;
	jp->j_prev = qp->q_tail;
	if (qp->q_tail != NULL)
		qp->q_tail->j_next = jp;
	else
		qp->q_head = jp;	/* list was empty */
	qp->q_tail = jp;
	pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Remove the given job from a queue.
 * 从队列中移除一个job
 */
void
job_remove(struct queue *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	if (jp == qp->q_head) {
		qp->q_head = jp->j_next;
		if (qp->q_tail == jp)
			qp->q_tail = NULL;
	} else if (jp == qp->q_tail) {
		qp->q_tail = jp->j_prev;
		if (qp->q_head == jp)
			qp->q_head = NULL;
	} else {
		jp->j_prev->j_next = jp->j_next;
		jp->j_next->j_prev = jp->j_prev;
	}
	pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Find a job for the given thread ID.
 */
struct job *
job_find(struct queue *qp, pthread_t id)
{
	struct job *jp;
	// 获取读锁  此锁可以被多个读锁共享
	if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
		return(NULL);

	for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
		if (pthread_equal(jp->j_id, id))
			break;

	pthread_rwlock_unlock(&qp->q_lock);
	return(jp);
}
