#How to clear bash history completely?

`~/.bash_history` holds the history.

To clear the bash history completely on the server, open terminal and 
type `cat /dev/null > ~/.bash_history`

Other alternate way is to link `~/.bash_history` to `/dev/null`

The history comes back when you login back. The history entries has a 
copy in the memory and it will flush back to the file when you log out.
The following command worked for me.

```
cat /dev/null > ~/.bash_history && history -c && exit
```
