# Link Shortener
Basic link shortener program in C.

## Usage
- To make a browser or curl request: `http://[address]:8080/create/?url=[url]`
- To request a shortened link: `http://[address]:8080/go/[shortened_url]`

## Current Issues
- leaving the last `/` on a url can sometimes also add trailing characters
	- [x] https://google.com
	- [ ] https://google.com/
