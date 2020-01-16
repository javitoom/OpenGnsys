#include "schedule.h"
#include "list.h"
#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <ev.h>

struct og_schedule *current_schedule = NULL;
LIST_HEAD(schedule_list);

static void og_schedule_add_sorted(struct og_schedule *new)
{
	if (new->seconds < time(NULL))
		return;
	struct og_schedule *schedule, *next;
	list_for_each_entry_safe(schedule, next, &schedule_list, list) {
		if (new->seconds < schedule->seconds) {
			list_add(&new->list, &schedule->list);
			return;
		}
	}
	list_add_tail(&new->list, &schedule_list);
}

static void og_parse_years(uint16_t years_mask, int years[])
{
	int i, j = 0;

	for (i = 0; i < 16; i++) {
		if ((1 << i) & years_mask)
			years[j++] = 2009 + i - 1900;
	}
}

static void og_parse_months(uint16_t months_mask, int months[])
{
	int i, j = 0;

	for (i = 0; i < 12; i++) {
		if ((1 << i) & months_mask)
			months[j++] = i;
	}
}

//static void og_parse_weeks(uint16_t weeks_mask, int weeks[])
//{
//	int i, j = 0;
//
//	for (i = 0; i < 5; i++) {
//		if ((1 << i) & weeks_mask)
//			weeks[j++] = i + 1;
//	}
//}

static void og_parse_days(uint16_t days_mask, int days[])
{
	int i, j = 0;

	for (i = 0; i < 31; i++) {
		if ((1 << i) & days_mask)
			days[j++] = i + 1;
	}
}

//static void og_parse_week_days(uint16_t week_days_mask, int week_days[])
//{
//	int i, j = 0;
//
//	for (i = 0; i < 7; i++) {
//		if ((1 << i) & week_days_mask)
//			week_days[j++] = i;
//	}
//}

static void og_parse_hours(uint16_t hours_mask, uint8_t am_pm, int hours[])
{
	int pm = 12 * am_pm;
	int i, j = 0;

	for (i = 0; i < 12; i++) {
		if ((1 << i) & hours_mask)
			hours[j++] = i + pm;
	}
}

void og_expand_schedule(unsigned int task_id, struct og_schedule_time *time)
{
	int years[12] = {};
	int months[12] = {};
//	int weeks[6] = {};
	int days[31] = {};
//	int week_days[7] = {};
	int hours[12] = {};
	int minutes;
	int i, j, k = 0;

	og_parse_years(time->years, years);
	og_parse_months(time->months, months);
	og_parse_days(time->days, days);
	og_parse_hours(time->hours, time->am_pm, hours);
	minutes = time->minutes;

	for (i = 0; years[i] != 0; i++) {
		for (j = 0; months[j] != 0; j++) {
			for (k = 0; days[k] != 0; k++) {
				struct og_schedule *schedule;
				schedule = (struct og_schedule *)
					calloc(1, sizeof(struct og_schedule));
				struct tm tm = {};

				tm.tm_year = years[i];
				tm.tm_mon = months[j];
				tm.tm_mday = days[k];
				tm.tm_hour = hours[k];
				tm.tm_min = minutes;

				schedule->seconds = mktime(&tm);
				schedule->task_id = task_id;
				og_schedule_add_sorted(schedule);
			}
//			for (k = 0; weeks[k] != 0; k++) {
//				for (l = 0; week_days[l] != 0; l++) {
//					date[n].tm_year = years[i];
//					date[n].tm_mon = months[j];
//					//date[n].week = weeks[k];
//					date[n].tm_wday = week_days[l];
//					date[n].tm_hour = hours[k];
//					date[n].tm_min = minutes;
//				}
//			}
		}
	}
}

static void og_agent_timer_cb(struct ev_loop *loop, ev_timer *timer, int events)
{
	struct og_schedule *current;

	current = container_of(timer, struct og_schedule, timer);
	og_schedule_task(current->task_id);

	ev_timer_stop(loop, timer);
	list_del(&current->list);
	free(current);

	og_next_schedule(loop);
}

void og_next_schedule(struct ev_loop *loop)
{
	struct og_schedule *next;
	unsigned int seconds;

	next = list_first_entry(&schedule_list, struct og_schedule, list);
	seconds = next->seconds - time(NULL);
	ev_timer_init(&next->timer, og_agent_timer_cb, seconds, 0.);
	ev_timer_start(loop, &next->timer);

	current_schedule = next;
}

void og_set_schedule_timer(struct ev_loop *loop)
{
	ev_timer_stop(loop, &current_schedule->timer);
	og_next_schedule(loop);
}
