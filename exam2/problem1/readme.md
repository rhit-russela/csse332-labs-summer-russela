# Problem 1: `count_chars.c` 🌶️

In this problem, we were given five files (under the `files/` directory) that
contain hexadecimal representations of ASCII characters. We would like to count
the number of `e` characters in all of those files and report that back to the
user. The code given to you in `count_chars.c` performs that operation
sequentially. Your job in this problem is to parrallelize the code to use 5
threads instead of a single one.

For obvious reasons, we would like to do this counting operation in two steps:

1. First, we will transform all the hex strings in the all the files into their
   corresponding ASCII characters, and save those characters in temporary files.
   That can be achieved using the function `transform_file`.

2. Second, we will use the `read_file_to_buffer` function to read the
   transformed files into a character buffer, and then we can easily count the
   number of `e` characters in that buffer using a simple loop.

## The Sequential Code

Here's a quick description of the sequential code that we have provided you
with. The array `file_names` contains the names of the files that we would like
to process. The array `ofnames` contains the names of the temporary files that
we shall generate during our transformation phase.

The function `transform_file(int, const char*, const char*)` accepts three
arguments: the thread's id (for the thread calling the function), the name of
the input file to transform, and the name of the output file to write to.

The function `read_file_to_buffer` accepts two arguments: a large buffer that
will eventually contain all the characters in the file, and the name of the
file that we should read into the buffer.

In the sequential phase of this implementation, we first have a simple loop that
iterates over all input `file_names` and then calls the `transform_file`
function on those to generate temporary files containing the transformed ASCII
characters.

After that, we go into a second loop that iterates over all the temporary files
(generated in the previous step) and first calls `read_file_to_buffer` to read
the content of the file into a larger buffer. Then, we iterate over the buffer
and count the number of `e` characters that show up in the buffer.

### Running the Sequential Code

To run the sequential code, first compile the program using:

```shell
$ make
```

And then run it using:

```shell
$ ./count_chars.bin
Sequential sum approach computed 818 'e' chars
Sequential approach finished in 5753 usecs.
Parallel sum approach computed 0 'e' chars
Parallel approach finished in 6 usecs.
```

You can see that the sequential approach counted 818 `e` characters while the
parallel approach does not count anything yet.

## Parallelizing the Code

### Step 1: Parallelize the Transformation

What we would like to do is to first parallelize the transformation of the input
files. In this case, we will create 5 threads and ask each thread to transform
one of the available files to process. __You do not need to actually change the
`transform_file` function__, you only need to call it from different threads,
each time on a different file.

Therefore, modify your code to create 5 threads and implement the threading
function for those threads. Your threading function simply needs to call
`transform_file` with the right arguments. You will need to pass three values to
each thread:

1. A custom assigned thread ID.

2. The name of the input file that the thread must transform. This will be one
   element of the `file_names` array.

3. The name of the output file that the thread must generate. This will be one
   element of the `ofnames` array.

**Note that you are not allowed to make the `file_names` and the `ofnames`
arrays global variables, they are to remain local variables in the main
thread.** The implication of this constraint is that your thread function
**must** accept three arguments and not just an index.

> _Hint_: You will find it useful to use a `struct` in this case.

To help in debugging, I made each thread print `[Thread <tid>] Working on file
<filename>` where `<tid>` is the passed thread ID and `<filename>` is the passed
input file that we must transform.

Once you implement this step, you can compile your code using `make` and run
it. Your output should look like the following:

```shell
$ ./count_chars.bin
Sequential sum approach computed 818 'e' chars
Sequential approach finished in 6148 usecs.
[Thread 1] Working on file files/file1.dat
[Thread 3] Working on file files/file3.dat
[Thread 2] Working on file files/file2.dat
[Thread 4] Working on file files/file4.dat
[Thread 5] Working on file files/file5.dat
Parallel sum approach computed 0 'e' chars
Parallel approach finished in 3450 usecs.
```

Note that since we are not yet doing the counting, the count still shows up as
0.

Note that your threads might print in a different order than mine. That is OKAY.

### Step 2: Doing the Counting

Next, we must spawn threads to do the counting on the transformed files. **We
will implement this by asking each transformation thread to spawn a counting
thread right after it is done transforming a file**. In other words, it is the
job of the threads created in the previous step (and thus not main) to spawn
counting threads to take care of the counting implementation.

Each counting thread will accept the same set of parameters as its corresponding
transformation thread. You may reuse the same `<tid>` between the transformation
thread and its created counting thread. In other words, it is okay (and desired)
if transformation thread 1 creates counting thread 1, transformation thread 2
creates counting thread 2, and so on.

Each counting thread will first call `read_file_to_buffer` on the passed
`ofname` and a temporary buffer. It will then count the number of `e`
characters in that buffer and update the global `sum` variable with the number
of `e` characters that it has found.

Note that accessing the global variable `sum` will create a possible race
condition between the counting threads, therefore you must use appropriate
locking mechanisms to avoid any concurrency bugs that might arise.
**Your code will be judged by the efficiency in which it access the `sum`
variable**.

In order to help me debug, I made each counting thread print something like:
```
[Counting Thread <tid>] Counting 'e's in file <ofilename>
```
where `<tid>` is the counting thread's ID and `<ofilename>` is the name of the
generated output file that the counting thread will read.

After you finish your implementation, your output should look something like the
following:

```shell
$ make count_chars.bin
$ ./count_chars.bin
Sequential sum approach computed 818 'e' chars
Sequential approach finished in 6052 usecs.
[Thread 1] Working on file files/file1.dat
[Thread 3] Working on file files/file3.dat
[Thread 2] Working on file files/file2.dat
[Counting Thread 1] Counting 'e's in file files/output_file1.txt
[Thread 4] Working on file files/file4.dat
[Counting Thread 3] Counting 'e's in file files/output_file3.txt
[Thread 5] Working on file files/file5.dat
[Counting Thread 4] Counting 'e's in file files/output_file4.txt
[Counting Thread 2] Counting 'e's in file files/output_file2.txt
[Counting Thread 5] Counting 'e's in file files/output_file5.txt
Parallel sum approach computed 818 'e' chars
Parallel approach finished in 3900 usecs.
```

Another possible interleaving could be as follows:

```shell
$ ./count_chars.bin
Sequential sum approach computed 818 'e' chars
Sequential approach finished in 6036 usecs.
[Thread 1] Working on file files/file1.dat
[Thread 2] Working on file files/file2.dat
[Thread 3] Working on file files/file3.dat
[Thread 5] Working on file files/file5.dat
[Thread 4] Working on file files/file4.dat
[Counting Thread 5] Counting 'e's in file files/output_file5.txt
[Counting Thread 3] Counting 'e's in file files/output_file3.txt
[Counting Thread 2] Counting 'e's in file files/output_file2.txt
[Counting Thread 1] Counting 'e's in file files/output_file1.txt
[Counting Thread 4] Counting 'e's in file files/output_file4.txt
Parallel sum approach computed 818 'e' chars
Parallel approach finished in 3872 usecs.
```

It is okay if your threads interleave and print in a different order than the one
I have, as long as the following requirements are met:

1. The parallel sum at the end is equal to the sequential sum, which is 818.

2. Each counting thread prints **after** its creating transformation thread. In
   other words, `[Counting Thread 1]` should show up after `[Thread 1]`,
   `[Counting Thread 2]` should show up after `[Thread 2]`, and so on.

3. The line `Parallel sum approach computed 818 'e' chars` should print **after
   all** threads (including both transformation and counting threads) have
   completed.

4. The main thread only creates and joins 5 threads and no more. In other words,
   the main thread only creates and waits for transformation threads.

