```
==> Verifying source file signatures with gpg...
cower-7.tar.gz ... FAILED (unknown public key 1EB2638FF56C0C53)
==> WARNING: Warnings have occurred while verifying the signatures.
    Please make sure you really trust them.
```

EDIT: The warning is shown due to signature files attached to the 
package. It can be solved for packages made by archlinux devs:

  - run as user
  ```
  # gpg --list-keys"
  ```
  in order to create a gpg database for your current user, if it is not
  present already.

  - add `keyring /etc/pacman.d/gnupg/pubring.gpg` 
    to the end of `~/.gnupg/gpg.conf`.
