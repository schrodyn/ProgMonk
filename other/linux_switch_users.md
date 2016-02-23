#How to switch between users on one terminal?

```
$ whoami 
```

This command prints the current user. To change users, we will have to use this command (followed by the user's password):

```
$ su secondUser
Password:
```

After entering the correct password, you will be logged in as the 
specified user (which you can check by rerunning `whoami`).

```
$ users
```

This command prints the user names currently logged in to the current 
host

```
$ who
```

prints information about users who are logged in

For example

```
$ who
mind     tty1         2016-02-18 22:03
mind     pts/0        2016-02-18 22:04 (:0)
mind     pts/1        2016-02-18 22:04 (:0)
```

pts means pseudoterminal
