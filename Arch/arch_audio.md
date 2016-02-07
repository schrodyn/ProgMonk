#Audio Setup

Configuring audio on Arch Linux is surprisingly easy compared to configuration of other components. The only thing that should be required to get audio working is to unmute the desired channels and set the audio levels with ALSA.

##Install ALSA

Install `alsa-lib`,`alsa-utils` and `alsa-firmware`(?) 

##Run ALSA Mixer

```
sudo alsamixer
```

Hit `F6` and select the sound card you plan on using. 
Press `m` to toggle between mute or unmute of the channel. 

```
amixer set Master 85% unmute
```
