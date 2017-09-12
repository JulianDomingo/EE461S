/*
 * Name: Julian Domingo
 * UT EID: jad5348
 */

#include "process_group.h"
#include "bg_jobs.h"

/*
 * Moves yash's foreground job to the background after stopping it, 
 * then clears the old, duplicate entry.
 */
void move_fg_job_to_bg(yash_t *yash) {
    yash->fg_job.status = STOPPED; 
   
    // TODO: Implement stack push operation

    yash->fg_job = NULL;
}
