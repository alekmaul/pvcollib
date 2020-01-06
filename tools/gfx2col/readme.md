
# gfx2col graphic converter for Colecovision PVcollib development

## Usage
```
gfx2col [options] png/bmp/pcx/tga filename ...  
```
where filename is a 256 color PNG, BMP, PCX or TGA file  with only 1st palette used with default 16 colors of Coleco  

## Options  
### General options 
- `-c[no|rle|ple|dan]` Compression method [no]  
  
### Graphique options  
- `-s` Sprite mode   
- `-b` Bitmap mode (no more 256 tiles limits)  
- `-g[m1|m2]` TMS9918 Graphic mode (mode 2 or mode 1) [m2]  
- `-e[0|1|2|3]` Enhanced Color Mode (F18A only) [0]  
- `-t` Enable transparent tiles (color 0)  

### Map options  
- `-m!` Exclude map from output  
- `-m` Convert the whole picture  
- `-mR!` No tile reduction (not advised)  
- `-mn#` Generate the whole picture with an offset for tile number where # is the offset in decimal (0 to 2047)  

### Palette options  
- `-p!` Exclude palette from output  
- `-po` Export palette (16 colors (ecm0) or 64 colors(ecm1-3)  
- `-pR` Palette rounding  

### File options  
- `-f[bmp|pcx|tga|png]` Convert a bmp or pcx or tga or png file [bmp]  
  
### Misc options 
- `-q` Quiet mode  
  
## Example 
```
gfx2col -crle -fpng -m myimage.png
```
 This will will convert a myimage png file to a inc file with 8x8 tiles, rle compressed.

## History  
V1.2.0 : add f18a features  

V1.1.0 : add bitmap mode  

V1.0.0 : initial release  
