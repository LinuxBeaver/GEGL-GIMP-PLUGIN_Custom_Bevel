## Custom Bevel - A Gimp Plugin to make any bevel/bump effect that your heart desires.
BECAUSE OF YOUR CARE AND SUPPORT A BRANCH OF CUSTOM BEVEL WAS ACCEPTED IN GIMP 2.99 officially.  It will be named "Chamfer" and will have a different GUI and lack Gimp only blend modes. This plugin build is seperate from the version that will be in Gimp and can co-exist with Gimp's version.

![image preview](custombevelpreview.png )


![image preview](customB_images/1.png )


## OS specific location to put GEGL Filter binaries 

Windows
 C:\\Users\<YOUR NAME>\AppData\Local\gegl-0.4\plug-ins
 
 Linux 
 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
 Linux (Flatpak)
 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins





## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

If you have an older version of gegl you may need to copy to `~/.local/share/gegl-0.3/plug-ins`
instead (on Ubuntu 18.04 for example).



### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```

## More image previews to show off how based this plugin is. 

![image](https://github.com/LinuxBeaver/GEGL-Custom-Bevel/assets/78667207/2134ef33-a523-4e3d-9b31-a3cb75b51c3e)

![image](https://github.com/LinuxBeaver/GEGL-Custom-Bevel/assets/78667207/6064a396-9bc2-4919-b444-dc7a8d3be383)

July 2023 update makes new styles possible.

![image](https://github.com/LinuxBeaver/GEGL-Custom-Bevel/assets/78667207/590687bd-9126-48c4-a9bb-62c3d21fbcc9)

![image](https://github.com/LinuxBeaver/GEGL-Custom-Bevel/assets/78667207/7785a44a-19bb-4aaf-a268-06a5f40ab4cf)

![image](https://github.com/LinuxBeaver/GEGL-Custom-Bevel/assets/78667207/a80b0e6f-d4ec-4b63-9800-1ea4cfeeea72)


Renders Custom Bevel Generated.
![image](https://github.com/LinuxBeaver/GEGL-Custom-Bevel/assets/78667207/14f0a74a-3eb3-467c-af31-8264a5df66e6)


Enjoy Custom Bevel. 
