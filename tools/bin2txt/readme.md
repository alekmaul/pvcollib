
# bin2txt binary to text converter for Colecovision PVcollib development

## Usage
```
bin2txt [options] filename ...  
```
where filename is a binary file

## Options  
### General options 
- `-c[no|rle|ple|dan]` Compression method [no]  
  
### Convert options  
- `-cc` Output in c style format
- `-ca` Output in assembly style format
  
### Misc options 
- `-q` Quiet mode  
  
## Example 
```
bin2txt -cc myimage.bin
```
 This will will convert a myimage bin file to a h file.

## History  
V1.0.0 : initial release  
