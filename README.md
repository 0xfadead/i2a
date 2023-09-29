# i2a

## About

**I**mage**2A**scii is, as the name implies, an image to text converter.

Its goals are to be lightweight, simple and fast.

## Features

- Can convert images to a variety of sizes in ascii
  
- Supports **many file formats** thanks to `stb_image`
  

## Roadmap

- Color support
  
- Fixing some errors in the conversion
  
- Support contrast changes
  

## Usage

### Building

- `git clone https://github.com/0xfadead/i2a.git --recursive` to clone i2a and its dependency
  
- `cd ia2 && make` to build the application (**NOTE: There are other make targets for debugging**)
  
- `./build/i2a` to run the build executable
  

### CLI

To get a simple help, just type `i2a --help`.

#### Converting an image

`i2a [Options] <input image> <scale factor in percent>`

##### Example

`i2a -v all input.png 23.5`
