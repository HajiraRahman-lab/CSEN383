#include "scheduler.h"
#include <limits.h>

void run_HPF_NonPreemptive(Process *p, int count)
{
    int time = 0;
    int completed = 0;
    Process *current = NULL;

    /* ---- Initialize dynamic fields ---- */
    for (int i = 0; i < count; i++) {
        p[i].remaining_time = p[i].run_time;
        p[i].start_time = -1;
        p[i].finish_time = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].response_time = 0;

        for (int t = 0; t < TOTAL_QUANTA; t++) {
            p[i].history[t] = false;
        }
    }

    /* ---- Main simulation loop ---- */
    while (completed < count) {

        /* Select next job if CPU is idle */
        if (current == NULL) {
            int best = -1;
            int best_priority = INT_MAX;
            int best_arrival = INT_MAX;
            int best_id = INT_MAX;

            for (int i = 0; i < count; i++) {
                if (p[i].arrival_time <= time &&
                    p[i].remaining_time > 0) {

                    if (p[i].priority < best_priority ||
                       (p[i].priority == best_priority &&
                        p[i].arrival_time < best_arrival) ||
                       (p[i].priority == best_priority &&
                        p[i].arrival_time == best_arrival &&
                        p[i].id < best_id)) {

                        best = i;
                        best_priority = p[i].priority;
                        best_arrival = p[i].arrival_time;
                        best_id = p[i].id;
                    }
                }
            }

            if (best != -1) {
                current = &p[best];

                if (current->start_time == -1) {
                    current->start_time = time;
                    current->response_time =
                        time - current->arrival_time;
                }
            }
        }

        /* Execute one quantum */
        if (current != NULL) {
            if (time < TOTAL_QUANTA) {
                current->history[time] = true;
            }

            current->remaining_time--;

            if (current->remaining_time == 0) {
                current->finish_time = time + 1;
                completed++;
                current = NULL;
            }
        }

        time++;
    }
}
