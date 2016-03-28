#lookup support

Now we will add `lookup` support so `ls` will work fine without errors.

When we do `ls` it calls `iterate()` which gives VFS filename and inode
of all the children in that directory. Also when we do `ls`, it search
in dcache for dentry of inode, when it doesn't find it calls `lookup()`
for that inode.

Right now we have dummy function that does nothing

```c
struct dentry *dedupfs_lookup(struct inode *parent_inode,
							  struct dentry *child_dentry, 
							  unsigned int flags)
{
	return NULL;
}
```

Let's fill it up

```c
	struct dedupfs_inode *parent = DEDUPFS_INODE(parent_inode);
	struct super_block *sb = parent_inode->i_sb;
	struct buffer_head *bh;
	struct dedupfs_file_record *record;
```

we start from retrieving our custom filesystem inode associated with 
`parent_inode` and also super block associated with `parent_inode`.

after that we go ahead and start reading parent directory 

```c
bh = (struct buffer_head *)sb_bread(sb, parent->data_block_number);
```

and iterate over its records

```c
	record = (struct dedupfs_file_record *) bh->b_data;
	for(int i = 0; i < parent->dir_children_count; i++) {
		if(!strcmp(record->filename, child_dentry->d_name.name)) {
			
			struct inode *inode;
			struct dedupfs_inode *dfs_inode;

			dfs_inode = dedupfs_get_inode(sb, record->inode_no);

			inode = new_inode(sb);
			inode->i_ino = record->inode_no;
			inode_init_owner(inode, parent_inode, dfs_inode->mode);
			inode->i_sb = sb;
			inode->i_op = &dedupfs_inode_ops;
			inode->i_fop = &dedupfs_dir_operations;

			/* FIXME: we should store these times to disk and 
			retrieve them */
			inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;

			inode->i_private = dfs_inode;

			d_add(child_dentry, inode);
			return NULL;
		}
	}
```

here we iterate over records and find the record with name of 
`child_dentry`, after that we retrieve `dedupfs_inode` structure
from disk with appropriate inode number `inode_no`

```c
dfs_inode = dedupfs_get_inode(sb, record->inode_no);
```

create in memory linux kernel structure `inode` and associate it with
our on-disk `dfs_inode`.

```c
			inode = new_inode(sb);
			inode->i_ino = record->inode_no;
			inode_init_owner(inode, parent_inode, dfs_inode->mode);
			inode->i_sb = sb;
			inode->i_op = &dedupfs_inode_ops;
			inode->i_fop = &dedupfs_dir_operations;

			/* FIXME: we should store these times to disk and 
			retrieve them */
			inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;

			inode->i_private = dfs_inode;
```

Finally, we associate `child_dentry` with newly created `inode` 
structure.

```c
d_add(child_dentry, inode);
```	

`dentry` structure is defined [here](http://lxr.free-electrons.com/source/include/linux/dcache.h?v=3.13#L108).

Ok, we have completed `lookup` function. The last thing we should
cover is what does it return. Why we return NULL?

returning NULL means you are fairly welcoming the supplied dentry and
attach it to your inode.

returning a dentry means it is a dentry which filesystem figured out 
was already associated with its inode. When can this happen? lookup 
process builds up a chain from parent to child dentry. In case of NFS 
export, lookup is done from file handle which can result in dentries 
which are disconnected from their parent, but are associated with 
inode. So, when you return such dentries from this operation, VFS will
change their disconnected status.



