##Step \#9 Language and location settings

We are going to configure the language of the new system. Run the following command:

```
# vim /etc/locale.gen
```

It will open a long list of locals, un-comment your language. Since I am using English I un-commented ‘en_US.UTF-8’. You can choose the language that you use.

Once uncommitted the language and saved the file, run the following commands, one by one:

```
# locale-gen
# echo LANG=en_US.UTF-8 > /etc/locale.conf
# export LANG=en_US.UTF-8
```

Let’s now configure time and date.
