# TLS - Thread List Client

tls is a program that uses threads to recursively traverse a directory tree, generating a report with the most important information for each directory in the tree. 

## SYNOPSIS

```bash
tls [-h] | [-n i] [-d directory] [output]
```

## OPTIONS

`-h` Displays a help message (syntax, parameter description, etc.)


`-n` Requested concurrency level. It must be an integer. Example: -n 5 


`-d` Specifies the directory in which the scan begins. Example: -d ~/david/photos/
