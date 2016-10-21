# init.d and systemd cheatsheet

## Useful links

  * http://www.tldp.org/HOWTO/HighQuality-Apps-HOWTO/boot.html
  * https://www.digitalocean.com/community/tutorials/how-to-configure-a-linux-service-to-start-automatically-after-a-crash-or-reboot-part-2-reference
  * https://www.digitalocean.com/community/tutorials/understanding-systemd-units-and-unit-files
  * https://www.digitalocean.com/community/tutorials/how-to-use-systemctl-to-manage-systemd-services-and-units
  * https://www.certdepot.net/rhel7-get-started-systemd/
  * https://coreos.com/docs/launching-containers/launching/getting-started-with-systemd/
  * https://learn.adafruit.com/running-programs-automatically-on-your-tiny-computer/systemd-writing-and-enabling-a-service
  * https://patrakov.blogspot.ru/2011/01/writing-systemd-service-files.html
  * https://doc.opensuse.org/documentation/leap/reference/html/book.opensuse.reference/cha.systemd.html
  * http://0pointer.de/blog/projects/systemd-for-admins-1.html
  * https://doc.opensuse.org/documentation/leap/reference/html/book.opensuse.reference/cha.journalctl.html
  * https://access.redhat.com/documentation/en-US/Red\_Hat\_Enterprise\_Linux/7/html/System\_Administrators\_Guide/chap-Managing\_Services\_with\_systemd.html

Finally

  * https://www.freedesktop.org/wiki/Software/systemd/

## Notes init.d

The most important line sits at the top of the file, as unassuming as 
can be. It's the one that sources the file `/etc/init.d/functions`. 
This file is filled with simple but useful bash shell functions that 
are ready for use in your init script.




