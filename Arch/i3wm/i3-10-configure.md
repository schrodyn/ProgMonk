##Configuring i3

To change the configuration of i3, copy `/etc/i3/config` to `~/.i3/config` (or `~/.config/i3/config`
if you like the XDG directory scheme) and edit it with a text editor.

##Comments

It is possible and recommended to use comments in your configuration file to properly document
your setup for later reference. Comments are started with a `#` and can only be used
at the beginning of a line:

Examples:

```
# This is a comment
```

##Fonts

i3 has support for both X core fonts and FreeType fonts (through Pango) to render window titles.

A FreeType font description is composed by a font family, a style, a weight, a variant, a stretch
and a size. FreeType fonts support right-to-left rendering and contain often more Unicode glyphs
than X core fonts.

If i3 cannot open the configured font, it will output an error in the logfile and fall back to a working font.

Syntax:

```
font <X core font description>
font pango:<family list> [<style options>] <size>
```

Examples:

```
font pango:DejaVu Sans Mono 10
font pango:DejaVu Sans Mono, Terminus Bold Semi-Condensed 11
font pango:Terminus 11px
```

##Infanality

The `infinality` patchset aims to greatly improve font rendering. It adds multiple new capabilities.

###Installation

`Infinality-bundle` is a collection of software aiming to improve text rendering in Arch Linux.

To install it add to `pacman.conf`:

```
[infinality-bundle]
Server = http://bohoomil.com/repo/$arch

[infinality-bundle-multilib]
Server = http://bohoomil.com/repo/multilib/$arch
```

Then add repository key id to pacman keyring:

```
# pacman-key -r 962DDE58
```

verify the fingerprint

```
# pacman-key -f 962DDE58
```

Finally, you need to locally sign the imported key:

```
# pacman-key --lsign-key 962DDE58
```

You now trust this key to sign packages.

Synchronising package databases:

```
# pacman -Syy
```

Then install packages

```
# pacman -S freetype2-infinality-ultimate
# pacman -S lib32-freetype2-infinality-ultimate
```
