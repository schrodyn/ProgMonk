##Step \#14 Booting into your new Arch system

When you boot into your new Arch system, it will present you with a login prompt. You can either
log in with your newly created user (though you will need sudo to perform tasks) or just login as root.

Let’s install X server (window system)

```
# pacman -S xorg-server xorg-xinit xorg-server-utils
```

If you want 3D support, you'll also want to install `mesa`:

```
# pacman -S mesa
```

It will ask you to choose the libgl file, that’s tricky. If you are installing Nvidia drivers then
use the latest libgl package from nvidia, otherwise choose the default option.

Now we will install video drivers. I assume you know which GPU you are using. Arch wiki has done a great job at documenting [which drivers you need](https://wiki.archlinux.org/index.php/Xorg#Driver_installation) to install for your hardware.

Brand |	Type |	Driver |	Multilib |	Documentation
------|------|---------|-------------|------------------
AMD/ATI | Open source | xf86-video-amdgpu | lib32-mesa-libgl | ATI
        |             | xf86-video-ati |               |
        | Proprietary | catalyst   | lib32-catalyst-utils | AMD Catalyst
Intel | Open source | xf86-video-intel | lib32-mesa-libgl | Intel graphics
Nvidia | Open source | xf86-video-nouveau | lib32-mesa-libgl | Nouveau
       | Proprietary | nvidia          | lib32-nvidia-libgl       | NVIDIA
       |             | nvidia-340xx    | lib32-nvidia-340xx-libgl |
       |             | nvidia-304xx    | lib32-nvidia-304xx-libgl |

I have AMD/ATI on my systems so I installed AMD drivers.

```
# sudo pacman -S xf86-video-ati
```

Most desktop computers should be able to use their mouse and keyboard out of the box. If you're on a laptop, though, you should take this time to install some drivers for your trackpad:

```
sudo pacman -S xf86-input-synaptics
```

##Step \#15 Install Desktop Environment

 it's time to see if you can get a graphical environment up and running. We'll install now the default environment:

```
 pacman -S xorg-twm xorg-xclock xterm
```

Then, to test X, just run:

```
startx
```

If it works, you should be able to interact with a very basic windowed environment
and run commands in xterm. You can exit by typing `exit` into xterm and hitting Enter.

![](/Arch/arch_installing/images/arch-linux-install-005.jpg)

It is here where I learned I used the wrong video drivers my first time around, so I had to uninstall the old ones and reinstall a different package. To remove a package in Pacman, just type:

```
pacman -Rns package_name
```

The `n` flag removes all the configuration files related to a package, while the `s` flag removes any dependencies that are now unnecessary. I generally always use `n` and `s` when removing a package, because I like to keep my system clean of cruft.
