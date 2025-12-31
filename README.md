# FreeBSD 6.2 R4R Experiments 🛠️

This repository is based on the original FreeBSD 6.2 source code, with added R4R (Rusted 4 Rings) experiments. Please note that these experiments are conducted on Intel architecture, both 32-bit (i386) and 64-bit (AMD64) processors.

## What is R4R? ✨

R4R experiments are an attempt to utilize all CPU rings as additional helper layers for the kernel or user space. For a more detailed philosophy and idea behind this, please check out the main repository here: [[R4R link](https://github.com/isoux/R4R)].

## Our Approach 🧩

We are not modifying the core FreeBSD itself. Instead, we are exploring how these ideas can be gradually integrated as additional experiments on Intel processors, without altering the existing system structure. The R4R option in the code indicates that this is our experimental addition and not a fundamental FreeBSD change.

## Initial Idea 💡

The initial idea started [from here](https://forums.freebsd.org/threads/humble-hobby-os-project.99600/page-2#post-734141), and you can read more details and the reactions on the forum.

---