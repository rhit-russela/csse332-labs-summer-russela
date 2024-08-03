# Problem 4: `big_little_car.c` 🌶️🌶️🌶️

In this problem, imagine a bridge that can only hold 2 units of weight. There
are two types of cars:

- Big cars weigh 2 units.

- Small cars weigh 1 unit.

Therefore, only one big car or two little cars can be crossing the bridge at
once.

When a car first reaches the side of the bridge, it prints that it is
`approaching` the bridge. When a car starts crossing the bridge, it will print
`starting bridge`; this is when that car will contribute to the weight limit on
the bridge.

Here are the rules that you must satisfy:

1. When a car arrives at the bridge, if it can go, then **it should**! (i.e., a
   little car should not wait because there's a big car that has been waiting
   for a long time).

   In other words, whenever a car arrives at the bridge and sees that the
   bridge can take on its weight without going over the limit, then the car
   will go on the bridge, regardless of whether there are other cars waiting
   there or not. You do not have to worry about starvation, that is a problem
   for another day.

2. When a car leaves the bridge, if there is a car waiting that can go on the
   bridge then it **should go**. If there are more then one car waiting on the
   bridge, then at least one must go, and the order in which they go **does not
   matter**.

   In other words, cars do not necessarily need to go on the bridge in the order
   they arrive, it is more of whoever can run will run regardless of the others.

3. When a big car leaves the bridge and a little car goes on the bridge, then if
   there is another little car waiting, that waiting car should go as well.

Here is a sample output from running my solution. note that the order of
printout is not going to exactly match mine,  but your output should not violate
any of the above rules.

```shell
$ ./big_little_car.bin
Big car approaching bridge
	 -----> Big car starting bridge
Little car approaching bridge
Big car approaching bridge
Little car approaching bridge
Big car approaching bridge
	 -----> Big car finishing bridge
	 -----> Little car starting bridge
	 -----> Little car starting bridge
	 -----> Little car finishing bridge
	 -----> Little car finishing bridge
	 -----> Big car starting bridge
	 -----> Big car finishing bridge
	 -----> Big car starting bridge
	 -----> Big car finishing bridge
Little car approaching bridge
	 -----> Little car starting bridge
Big car approaching bridge
Little car approaching bridge
	 -----> Little car starting bridge
	 -----> Little car finishing bridge
	 -----> Little car finishing bridge
	 -----> Big car starting bridge
	 -----> Big car finishing bridge
Everything finished.
```

## Efficiency Constraint

This problem has an efficiency component that you will need to consider,
though for a smaller amount of points. The majority of this problem's points
are for the correctness of you solution.

To prevent any brain pollution, I have hidden some hints about the efficiency
constraint in a file calledn `.efficiency.md` in this directory. **I strongly
suggest that you solve this problem for correctness first, without looking at
this file**, otherwise you might pollute your brain and prevent yourself from
solving the problem. Remember that _premature optimization is the root of all
evil_.

Once you are ready to check out the efficiency constraint, you can read the
file using `cat .efficiency.md` in this directory. There a few hints about what
the problem is. Again, **please do not address efficiency until you have solved
for correctness**.

