# Problem 2: `leader.c` 🌶️

In this problem, we have two types of threads: a leader and a set of followers.
There is exactly 1 leader and `NUM_FOLLOWERS` follower threads. The follower
threads work for some time (defined by the `FOLLOWER_SHIFT` constant) and then
go into the playground to rest. They stay there as long as the leader is not in
the playground.  The leader, on the other hand, works for some time (defined by
`LEADER_SHIFT`) and then goes into the playground and spends some time there
(defined by `LEADER_REST`). After that, the leader will go back to work until
they complete `NUM_LEADER_SHIFTS` total shifts.

After each of their work shifts, each follower will try to enter the playground.
If the leader is not around (i.e., the leader is working), the follower will
enter the playground and stay there (i.e., wait) until the leader arrives.  On
the other hand, if the leader is in the playground, then the follower will not
stay in the playground, they will instead leave directly.

After each of their work shifts, the leader will enter the playground. If there
are followers in the playground lingering, then the leader will kick them all
out. The leader will then spend `LEADER_REST` time units in the playground.
During this time, followers will not linger in the playground, they will enter,
see the leader in there and then directly exit and go back to work.

Use concurrency mechanisms (condition variables and mutex locks) to implement
the above requirements. You can compile your code using `make leader.bin` and
then run it. Here is a sample output from my solution:

```shell
$ ./leader.bin
Follower (1) doing work...
Follower (2) doing work...
Follower (5) doing work...
Follower (6) doing work...
Leader doing shift 1 out of 5...
Follower (3) doing work...
Follower (4) doing work...
Follower (1) entering the playground...
Follower (1) lingering in the playground with 0 others...
Follower (6) entering the playground...
Follower (6) lingering in the playground with 1 others...
Follower (5) entering the playground...
Follower (5) lingering in the playground with 2 others...
Follower (2) entering the playground...
Follower (2) lingering in the playground with 3 others...
Follower (3) entering the playground...
Follower (3) lingering in the playground with 4 others...
Follower (4) entering the playground...
Follower (4) lingering in the playground with 5 others...
Leader entered the playground...
Follower (3) left the playground...
Follower (3) doing work...
Follower (2) left the playground...
Follower (2) doing work...
Follower (1) left the playground...
Follower (1) doing work...
Follower (4) left the playground...
Follower (4) doing work...
Follower (6) left the playground...
Follower (6) doing work...
Follower (5) left the playground...
Follower (5) doing work...
Leader has left the playground...
Leader doing shift 2 out of 5...
Leader entered the playground...
Follower (3) entering the playground...
Follower (3) left the playground...
Follower (3) doing work...
Follower (2) entering the playground...
Follower (2) left the playground...
Follower (2) doing work...
Follower (1) entering the playground...
Follower (1) left the playground...
Follower (1) doing work...
Leader has left the playground...
Leader doing shift 3 out of 5...
Follower (4) entering the playground...
Follower (4) lingering in the playground with 0 others...
Follower (6) entering the playground...
Follower (6) lingering in the playground with 1 others...
Follower (5) entering the playground...
Follower (5) lingering in the playground with 2 others...
Leader entered the playground...
Follower (4) left the playground...
Follower (4) doing work...
Follower (6) left the playground...
Follower (6) doing work...
Follower (5) left the playground...
Follower (5) doing work...
Follower (3) entering the playground...
Follower (3) left the playground...
Follower (3) doing work...
Follower (2) entering the playground...
Follower (2) left the playground...
Follower (2) doing work...
Follower (1) entering the playground...
Follower (1) left the playground...
Follower (1) doing work...
Leader has left the playground...
Leader doing shift 4 out of 5...
Follower (4) entering the playground...
Follower (4) lingering in the playground with 0 others...
Follower (6) entering the playground...
Follower (6) lingering in the playground with 1 others...
Follower (5) entering the playground...
Follower (5) lingering in the playground with 2 others...
Follower (3) entering the playground...
Follower (3) lingering in the playground with 3 others...
Follower (2) entering the playground...
Follower (2) lingering in the playground with 4 others...
Follower (1) entering the playground...
Follower (1) lingering in the playground with 5 others...
Leader entered the playground...
Follower (6) left the playground...
Follower (6) doing work...
Follower (1) left the playground...
Follower (1) doing work...
Follower (2) left the playground...
Follower (2) doing work...
Follower (5) left the playground...
Follower (5) doing work...
Follower (3) left the playground...
Follower (3) doing work...
Follower (4) left the playground...
Follower (4) doing work...
Leader has left the playground...
Leader doing shift 5 out of 5...
Follower (6) entering the playground...
Follower (6) lingering in the playground with 0 others...
Follower (1) entering the playground...
Follower (1) lingering in the playground with 1 others...
Follower (5) entering the playground...
Follower (3) entering the playground...
Follower (5) lingering in the playground with 2 others...
Follower (2) entering the playground...
Follower (2) lingering in the playground with 3 others...
Follower (4) entering the playground...
Follower (3) lingering in the playground with 4 others...
Follower (4) lingering in the playground with 5 others...
Leader entered the playground...
Follower (4) left the playground...
Follower (4) doing work...
Follower (6) left the playground...
Follower (6) doing work...
Follower (1) left the playground...
Follower (1) doing work...
Follower (2) left the playground...
Follower (2) doing work...
Follower (5) left the playground...
Follower (5) doing work...
Follower (3) left the playground...
Follower (3) doing work...
Leader has left the playground...
Leader finished, exiting even if followers are still there...
```

Note that you will not get the exact same output, that is okay. Here are the
requirements for your output:

1. Once the leader has entered the playground, all followers that are in there
   must leave the playground. This is shown in the following output snippet:
   ```
   Leader entered the playground...
   Follower (3) left the playground...
   Follower (3) doing work...
   Follower (2) left the playground...
   Follower (2) doing work...
   Follower (1) left the playground...
   Follower (1) doing work...
   Follower (4) left the playground...
   Follower (4) doing work...
   Follower (6) left the playground...
   Follower (6) doing work...
   Follower (5) left the playground...
   Follower (5) doing work...
   Leader has left the playground...
   ```

2. If the leader is not in the playground, then the followers can enter the
   playground and linger in there. This is shown in the following snippet:
   ```
   Leader doing shift 1 out of 5...
   Follower (3) doing work...
   Follower (4) doing work...
   Follower (1) entering the playground...
   Follower (1) lingering in the playground with 0 others...
   Follower (6) entering the playground...
   Follower (6) lingering in the playground with 1 others...
   Follower (5) entering the playground...
   Follower (5) lingering in the playground with 2 others...
   Follower (2) entering the playground...
   Follower (2) lingering in the playground with 3 others...
   Follower (3) entering the playground...
   Follower (3) lingering in the playground with 4 others...
   Follower (4) entering the playground...
   Follower (4) lingering in the playground with 5 others...
   Leader entered the playground...
   ```

3. If the leader is in the playground, then any follower that enters the
   playground directly leaves and goes back to work. This is shown in the
   following output snippet:
   ```
   Leader entered the playground...
   Follower (4) left the playground...
   Follower (4) doing work...
   Follower (6) left the playground...
   Follower (6) doing work...
   Follower (5) left the playground...
   Follower (5) doing work...
   Follower (3) entering the playground... <<<<< This follower tries to enter
   Follower (3) left the playground...     <<<<< They will leave directly
   Follower (3) doing work...              <<<<< And go back to work!
   Follower (2) entering the playground...
   Follower (2) left the playground...
   Follower (2) doing work...
   Follower (1) entering the playground...
   Follower (1) left the playground...
   Follower (1) doing work...
   Leader has left the playground...
   ```

Feel free to reduce the number of followers to hunt bugs and get a clearer idea
of what is going on, but please put it back to 6 before you submit.
