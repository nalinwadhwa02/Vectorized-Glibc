# 21-12-2022

- Overall plan: Create a search space of codeblocks (a few assembly instructions long). Build a superoptimiser to search the space of codeblocks and come up with an implementation for a given function (like strlen). Goal is to reach as close to the handwritten optimised implementation as possible.
- Initially, we can assume all input addresses are aligned for vector loads/stores. We can handle alignment later.
- Special logic for individual functions may be replaced by more reusable generic codeblocks, but we must ensure that the performance doesn't degrade by *too much*.
- Make the common case fast:
    - Focus on fast code paths.
    - Focus on glibc functions which are the most frequently used.