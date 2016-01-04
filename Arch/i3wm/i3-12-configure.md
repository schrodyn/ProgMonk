##The floating modifier

To move floating windows with your mouse, you can either grab their titlebar or configure the so called floating modifier which you can then press and click anywhere in the window itself to move it. The most common setup is to use the same key you use for managing windows (`Mod1` for example). Then you can press `Mod1`,
click into a window using your left mouse button, and drag it to the position you want.

When holding the floating modifier, you can resize a floating window by pressing the right
mouse button on it and moving around while holding it. If you hold the shift button as well,
the resize will be proportional (the aspect ratio will be preserved).

Syntax:

```
floating_modifier <Modifier>
```

Example:

```
floating_modifier Mod1
```

##Constraining floating window size

The maximum and minimum dimensions of floating windows can be specified. If either dimension of `floating_maximum_size` is specified as -1, that dimension will be unconstrained with respect to its
maximum value. If either dimension of `floating_maximum_size` is undefined, or specified as 0,
i3 will use a default value to constrain the maximum size. `floating_minimum_size` is treated in a manner analogous to `floating_maximum_size`.

Syntax:

```
floating_minimum_size <width> x <height>
floating_maximum_size <width> x <height>
```

Example:

```
floating_minimum_size 75 x 50
floating_maximum_size -1 x -1
```

##Orientation for new workspaces

New workspaces get a reasonable default orientation: Wide-screen monitors (anything wider than high) get horizontal orientation, rotated monitors (anything higher than wide) get vertical orientation.

With the `default_orientation` configuration directive, you can override that behavior.

Syntax:

```
default_orientation horizontal|vertical|auto
```

Example:

```
default_orientation vertical
```
