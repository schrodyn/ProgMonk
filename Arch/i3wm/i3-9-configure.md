##Tree

i3 stores all information about the X11 outputs, workspaces and layout of the windows on them in a tree.
The root node is the X11 root window, followed by the X11 outputs, then dock areas and a content
container, then workspaces and finally the windows themselves.

###The tree consists of Containers

The building blocks of the tree are so called `Containers`. A `Container` can host a window
(meaning an X11 window, one that you can actually see and use, like a browser). Alternatively,
it could contain one or more `Containers`. A simple example is the workspace: When you start i3 with
a single monitor, a single workspace and you open two terminal windows, you will end up with a tree like this:

![](/Arch/i3wm/images/i3wm-tree-1.png)

![](/Arch/i3wm/images/i3wm-tree-2.png)

*Two terminals on standard workspace*

###Orientation and Split Containers

It is only natural to use so-called `Split Containers` in order to build a layout when using a tree
as data structure. In i3, every `Container` has an orientation (horizontal, vertical or unspecified)
and the orientation depends on the layout the container is in (vertical for splitv and stacking, horizontal for splith and tabbed). So, in our example with the workspace, the default layout of the workspace
`Container` is splith (most monitors are widescreen nowadays). If you change the layout to splitv (`$mod+r` in my config) and then open two terminals, i3 will configure your windows like this:

![](/Arch/i3wm/images/i3wm-tree-3.png)

###Focus parent

Let’s say we have a terminal on the left and two vertically split terminals on the right, focus is on the bottom right one.

![](/Arch/i3wm/images/i3wm-tree-4.png)

![](/Arch/i3wm/images/i3wm-tree-5.png)

When you open a new terminal, it will open below the current one.

So, how can you open a new terminal window to the **right** of the current one? The solution is to use
`focus parent`, which will focus the `Parent Container` of the current `Container`. In this case,
you would focus the `Vertical Split Container` which is **inside** the horizontally oriented workspace. Thus, now new windows will be opened to the right of the `Vertical Split Container`:

![](/Arch/i3wm/images/i3wm-tree-6.png)

*Focus parent, then open new terminal*

I've configured it as `$mod+p` (mnemonic rule is "parent").
