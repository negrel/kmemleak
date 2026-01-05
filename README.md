# `kmemleak` - Exceeding cgroup memory limits by allocating kernel memory.

Small `io_uring` program, that allocates kernel memory and never frees it.
This can be used to exceed control group memory limits. See my
[bugzilla report](https://bugzilla.kernel.org/show_bug.cgi?id=220794).

[A fix has been accepted](https://lkml.org/lkml/2025/12/30/1017) and will be
part of Linux 6.19 release.

## :scroll: License

MIT © [Alexandre Negrel](https://www.negrel.dev/)
