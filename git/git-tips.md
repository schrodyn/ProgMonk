#Undoing Things

 When you commit too early and possibly forget to add some files, or you mess up 
your commit message. If you want to try that commit again, you can run commit 
with the `--amend` option:

```
$ git commit --amend
```


This command takes your staging area and uses it for the commit. If you’ve made no 
changes since your last commit (for instance, you run this command immediately 
after your previous commit), then your snapshot will look exactly the same, and 
all you’ll change is your commit message.


#Adding Patch

```
git add -p file.c
```

or

```
git add --patch file.c
```

This command makes partial staging.

For every hunk you have a lot of options:

```
Stage this hunk [y,n,a,d,/,j,J,g,e,?]?
```

Typing `?` shows a list of what you can do:

```
Stage this hunk [y,n,a,d,/,j,J,g,e,?]? ?
y - stage this hunk
n - do not stage this hunk
a - stage this and all the remaining hunks in the file
d - do not stage this hunk nor any of the remaining hunks in the file
g - select a hunk to go to
/ - search for a hunk matching the given regex
j - leave this hunk undecided, see next undecided hunk
J - leave this hunk undecided, see next hunk
k - leave this hunk undecided, see previous undecided hunk
K - leave this hunk undecided, see previous hunk
s - split the current hunk into smaller hunks
e - manually edit the current hunk
? - print help
```

Generally, you’ll type `y` or `n` if you want to stage each hunk, but staging all 
of them in certain files or skipping a hunk decision until later can be helpful 
too. If you stage one part of the file and leave another part unstaged, your 
status output will look like this:

In my opinion, very helpful option is `J` which allows you not to add
current hunk in staging area and skip it.

#Unstage Files

The `reset` command overwrites these three trees in a specific order, stopping 
when you tell it to:

1. Move the branch HEAD points to (stop here if --soft)

2. Make the Index look like HEAD (stop here unless --hard)

3. Make the Working Directory look like the Index

If you specify a path, `reset` will skip step 1, and limit the remainder of its 
actions to a specific file or set of files.


```
git reset --mixed HEAD -- file.txt
```

which will:

1. Move the branch HEAD points to (**skipped**)
2. Make the Index look like HEAD (stop here)

So it essentially just copies `file.txt` from HEAD to the Index.

However, you might be wondering about the strange `--` that shows up in the 
command. Those are referred to as the 'bare double dashes', and they prevent 
ambiguity by ensuring that we're resetting a file and not specifying a branch.

**Modification:** in last versions of git `reset --mixed` is deprecated
with file path, so we should simply use `git reset -- <path>`:

```
git reset HEAD -- file.txt
```

Good explanation of `reset` command you can read 
[here](https://git-scm.com/book/en/v2/Git-Tools-Reset-Demystified).

We could just as easily not let Git assume we meant “pull the data from HEAD” by 
specifying a specific commit to pull that file version from. We would just run 
something like `git reset eb43bf  -- file.txt`.

#Restore Published Commit

The `git revert` command undoes a committed snapshot. But, instead of removing 
the commit from the project history, it figures out how to undo the changes 
introduced by the commit and appends a new commit with the resulting content.

This prevents Git from losing history, which is important for the integrity of 
your revision history and for reliable collaboration.

```
git revert <commit>
```

Generate a new commit that undoes all of the changes introduced in `<commit>`, 
then apply it to the current branch.

You can read more [here](https://www.atlassian.com/pt/git/tutorial/undoing-changes#!revert).

#Stashing

Stashing takes the dirty state of your working directory —- that is, your modified
tracked files and staged changes —- and saves it on a stack of unfinished changes 
that you can reapply at any time.

To push a new stash onto your stack, run `git stash`.

At this point, you can easily switch branches and do work elsewhere; your changes 
are stored on your stack. To see which stashes you’ve stored, you can use 
`git stash list`:

```
$ git stash list
stash@{0}: WIP on master: 049d078 added the index file
stash@{1}: WIP on master: c264051 Revert "added file_size"
stash@{2}: WIP on master: 21d80a5 added number to log
```

You can reapply the one you just stashed by using the command `git stash apply`. 
If you want to apply one of the older stashes, you can specify it by naming it, 
like this: 

```
git stash apply stash@{2}
```

If you don’t specify a stash, Git assumes the most recent stash and tries to 
apply it.

The apply option only tries to apply the stashed work —- you continue to have it 
on your stack. To remove it, you can run `git stash drop` with the name of the 
stash to remove:

```
$ git stash list
stash@{0}: WIP on master: 049d078 added the index file
stash@{1}: WIP on master: c264051 Revert "added file_size"
stash@{2}: WIP on master: 21d80a5 added number to log
$ git stash drop stash@{0}
Dropped stash@{0} (364e91f3f268f0900bc3ee613f9f733e82aaed43)
```

You can also run `git stash pop` to apply the stash and then immediately drop it 
from your stack.




