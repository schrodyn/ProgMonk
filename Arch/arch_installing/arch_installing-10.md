##Step \#11 Configure repositories

If you are running a 64bit system then you need to enable the ‘multilib repository. Open the pacman.conf file using vim:

```
# vim /etc/pacman.conf
```

Scroll down and un-comment the ‘multilib’ repo:

```
[multilib]
Include = /etc/pacman.d/mirrorlist
```

Enabling the *multilib* repository allows the user to run and build 32-bit applications on 64-bit installations of Arch Linux. *multilib* creates a directory containing 32-bit instruction set libraries inside `/usr/lib32/`, which 32-bit binary applications may need when executed.

A 64-bit installation of Arch Linux with *multilib* enabled follows a directory structure similar to Debian. The 32-bit compatible libraries are located under `/usr/lib32/`, and the native 64-bit libraries under `/usr/lib/`.

While we are still inside ‘pacman.conf’ file, let’s also add the AUR repo so we can easily install
packages from AUR. Add these lines at the bottom of the file:

For 32bit system
```
[archlinuxfr]
SigLevel = Never
Server = http://repo.archlinux.fr/i686
```

For 64bit system
```
[archlinuxfr]
SigLevel = Never
Server = http://repo.archlinux.fr/x86_64
```

Then save and close the config file. Now it’s time to update the repositories by running this command:

```
# pacman -Sy
```

You must always update repos before installing any packages.
