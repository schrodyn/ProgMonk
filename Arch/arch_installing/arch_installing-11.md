##Step \#12 Create users

It’s never a good idea to run your system as root, so we are going to create a user
for our system. Before we do that lets’ create a password for the root user. Run the following
command and then provide it with the desired password:

```
# passwd
```

Now we are going to create a user for our system. This command will also create the home
directory for the user and add it to some groups:

```
# useradd -m -g users -G wheel,storage,power -s /bin/bash swapnil
```

You can read more about users and groups [here](https://wiki.archlinux.org/index.php/Users_and_groups).

Exchange ‘swapnil’ with your username. Then provide the user with its password; how good is a Linux user without any password:

```
# passwd swapnil
```

Let’s give this user some ‘sudo’ superpowers so it can perform administrative jobs without bothering the root. First install sudo:

```
# pacman -S sudo
```

You can read more about Arch Linux Package Management [here](https://www.digitalocean.com/community/tutorials/how-to-use-arch-linux-package-management).

Now we have to edit the sudoers file to give this user the much needed sudo powers. Don’t open this file with a regular editor; it must be edited with `visudo` command.

Un-comment this line in this file:

```
%wheel ALL=(ALL) ALL
```

I also suggest installing the bash-completion package which makes it easier with auto-complete of commands and package names.

```
# pacman -S bash-completion
```

It’s now time to configure grub.
