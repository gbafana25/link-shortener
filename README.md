# Link Shortener
Basic link shortener program in C.

## Usage
Make a browser or curl request: `http://[address]:8080/create/[url]`

## Current Issues
- leaving the last `/` on a url can sometimes also add trailing characters
	- [x] https://google.com
	- [ ] https://google.com/
