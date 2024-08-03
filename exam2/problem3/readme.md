# Problem 3: `two_person_jobs.c` (20 points) 🌶️🌶️🌶️

In this system, worker threads work on jobs that require 2 people. There are
`JOBNUM` jobs and `JOBNUM*2` workers.

When a worker is ready to work on a job, they print `Worker X ready to work on
job Y`, where `Y` is the next unstarted job. However, they cannot start the job
yet; first, they must wait for a second worker to join them on the same job
`Y`.

The second worker arriving will print `Worker Z helping on job Y`, and then
**both** workers will print `Starting job Y` (see the sample output below for an
example).

Both workers will then spend time working on the job. The first worker to finish
working will print `Worker A partially finished on job Y`. After that, when the
second worker finishes, they will print `Worker B completely finished on job Y.`

Note that the workers who start the job are always the same workers who finish
that job, **but they don't need to finish in order**. Example, worker 0 might
be `ready to work` and worker 1 might `help`. However, it is okay if worker 1
finishes first (i.e., prints the `partially finished` message) while worker 0
finishes second (and thus prints the `completely finished` message).

Here is a sample output from running my solution. note that the order of
printout is not going to exactly match mine,  but your output should not
violate any of the above rules.

```shell
$ ./two_person_jobs.bin
Worker 0 ready to work on job 0
Worker 4 helping on job 0
[Worker 4]: Starting job 0
[Worker 0]: Starting job 0
Worker 2 ready to work on job 1
Worker 1 helping on job 1
[Worker 1]: Starting job 1
[Worker 2]: Starting job 1
Worker 3 ready to work on job 2
Worker 1 partially finished on job 1
Worker 2 completely finished on job 1
Worker 4 partially finished on job 0
Worker 0 completely finished on job 0
Worker 5 helping on job 2
[Worker 5]: Starting job 2
Worker 6 ready to work on job 3
Worker 7 helping on job 3
[Worker 7]: Starting job 3
[Worker 3]: Starting job 2
[Worker 6]: Starting job 3
Worker 5 partially finished on job 2
Worker 7 partially finished on job 3
Worker 3 completely finished on job 2
Worker 6 completely finished on job 3
Everyting finished...
```

Here is a quick summary of the rules:

1. Exactly two workers need to be working on the same job.

2. A worker cannot start a job until their fellow helper arrives.

3. The first worker to finish on job Y will partially finish the job.

4. The second worker to finish on job Y will completely finish the job.

5. The two workers who start job Y must also be the same two workers that
   finish that job Y.

> Hint: The array `jobs[JOBNUM]` can be used to represent the state of each job.
        Initially (in the `main` function), we set all the jobs to the
        `UNSTARTED` state.

> Hint: You might find it helpful to define a couple of additional states for
        each job.

> Hint: Line 54 contains a conditional statement that you can use to debug your
        code. If your solution is correct, you should never hit this if
        statement and thus you can safely delete it. If you ever execute the
        branch inside the statement, then something is wrong with your approach
        and you will need to rethink it.

