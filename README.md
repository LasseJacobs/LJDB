# STL-Style Open Address Hashmap
## Usage
Build and run the project, a command input field '>' shoud appear on the terminal. Input a command and see what happens!

Example:
```
$./LJDB
$ > create database myDb
$ > select database myDb
$ [myDb] > create table example
$ [myDb] > put example "hello" "world"
$ [myDb] > get example "hello"
$ == world
$ [myDb] > exit

```

## Description
This is a very basic implementation of a log-based key-value store. Many aspects of this codebase still need a lot of work!

## Commands
For a more updated list of commands, you can look at the source. Specifically everything in the *eval* package.

```
create
    - database <name>
    - [database] [create] table <name>

delete
    - database <name>
    - [database] [delete] table <name>

select
    - database <name>

[database] put <table> "<key>" "<value>"

[database] get <table> "<key>"

[database] remove <table> "<key>"

exit

```

NOTE: " can be escaped (\\")


## Contributing
Any feedback, on anything, is always appreciated. 

## Building

Building:

    $ cd LJBD/
    $ make
	
	
## The TODO list:
Planned updates:
 - overall improvements to the IO operations
 - socket API
 - rollbacks
 - snapshots
 - log compaction 
 - presistent indeces (maybe SStables)
	

