
# gfx2col graphic converter for Colecovision PVcollib development

## Usage
```
gfx2col [options] png/bmp/pcx/tga filename ...  
```
where filename is a 256 color PNG, BMP, PCX or TGA file  with only 1st palette used with default 16 colors of Coleco

## Options
### General options 
- `-c[no|rle|ple|dan] Compression method [no] 
  
### Graphique options 
- `-s` Sprite mode 
- `-b` Bitmap mode (no more 256 tiles limits)

### Map options
- `-m!` Exclude map from output  
- `-m` Convert the whole picture  
- `-mR!` No tile reduction (not advised)  
  
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
V1.1.0 : add bitmap mode

V1.0.0 : initial release
