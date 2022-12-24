# 21-12-2022

- Overall plan: Create a search space of codeblocks (a few assembly instructions long). Build a superoptimiser to search the space of codeblocks and come up with an implementation for a given function (like strlen). Goal is to reach as close to the handwritten optimised implementation as possible.
- Initially, we can assume all input addresses are aligned for vector loads/stores. We can handle alignment later.
- Special logic for individual functions may be replaced by more reusable generic codeblocks, but we must ensure that the performance doesn't degrade by *too much*.
- Make the common case fast:
    - Focus on fast code paths.
    - Focus on glibc functions which are the most frequently used.

# 22-12-2022

- Codeblock idea: Check the next N*VEC_SIZE bytes for a set of characters. Here VEC_SIZE is 32 and N can be 1,4. The set of characters may be {0}, {c} or {0,c}.

- The superoptimiser will have the choice of connecting two codeblocks, or connecting a codeblock to some point in the original implementation.
    - The idea is that our codeblocks should be able to implement the fast path, and for the slow path we can defer to the original implementation.
    - Some codeblocks should have exit points from where the original implementation can take over. We may also need to write thunk code to execute between a codeblock and the original implementation for transferring data.