#ifndef _OG_SCHEDULE_H_
#define _OG_SCHEDULE_H_

#include <stdint.h>
#include "dbi.h"
#include "list.h"
#include <ev.h>

struct og_schedule_time {
	unsigned int	years;
	unsigned int	months;
	unsigned int	days;
	unsigned int	hours;
	unsigned int	am_pm;
	unsigned int	minutes;
};

struct og_schedule {
	struct list_head	list;
	struct ev_timer		timer;
	time_t			seconds;
	unsigned int		task_id;
};

void og_expand_schedule(unsigned int task_id, struct og_schedule_time *time);
void og_next_schedule(struct ev_loop *loop);
void og_set_schedule_timer(struct ev_loop *loop);
void og_schedule_task(unsigned int task_id);

#endif
