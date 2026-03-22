<!--
    Copyright © 2026 Gaël Fortier <gael.fortier.1@ens.etsmtl.ca>
-->

Copyright © 2026 Gaël Fortier 
<gael.fortier.1@ens.etsmtl.ca>

## Disclaimer

This project is publicly released as part of my portfolio but should be regarded as a personal project intended for my own personal use. 
Should you use this project, you are fully responsible and accountable for any loss, damage or injury arising from the use of this project.

- You may not profit financially in any possible way from this project without prior agreement with me
- You may not train any artificial intelligence models or computer algorithms on this project
- You may not keep a copy of this project for online redistribution without prior agreement with me
- You may not publicly release a modified version of this project without prior agreement with me
- Legal issues are to be resolved within Canadian and Québécois legal frameworks only. 

## Clapi

Clapi, or Command Line Application Programming Interface, is a framework that binds a chain of identifiers and wildcards to a function pointer, 
similar to REST APIs used in web development frameworks like Express. 
I made Clapi because I was wasting my energy and my time parsing user inputs in a ugly, highly coupled way, every time I was doing a terminal-based app. 


### Definitions

- Route: a path of execution
- Identifier: a list of predefined word that uniquely identify a route. 
- Wildcard: a route can have wildcard identifier to have variable tokens of a specific type.

### Features

- User input parsing and sanitization
- Function pattern matching for routing
- 5 supported types of wildcards: 
  - signed long integer
  - double precision float (ex: `3.45`)
  - double-quote delimited string literal (ex: `"hello world"`)
  - a space delimited alphanumerical word
  - a space delimited hexadecimal sequence (ex: `89504E47`)
- All tests have passed.

### Structure
- /auto: automation scripts (module management, compiling and executing tests)
- /src: source code
- /test: test code
  
### Usage

See [/test/cli/cli.c](/test/cli/cli.c). 

#### Route buffer

When the program starts, a pre-allocated buffer must be set to register routes

### Objects
#### cli_param_t
- A polymorphic object (union) that can hold a signed long integer, a double-precision float, or a pointer and the length in bytes of pointer region. 

#### cli_toklist_t
- A list of tokens. Holds an array of pointers and the number of tokens.

#### cli_route_t
- An object holding characteristics of a route: 
  - the list of identifiers (or the pattern)
  - the number of identifiers
  - the number of wildcards found in the list
  - a pointer to a `cli_func_t` function. 
  
#### cli_error_t 
- An alias for `const char*` that points to an error message string. 

### Functions
#### cli_setbuf_s
- Description: Sets the buffer used to save added routes. 
- Returns: pointer of previous buffer used
- Parameter `buffer`: an array of `cli_route_t` objects
- Parameter `number`: the number of objects allocated in array.

#### cli_clearrte
- Description: Clear all routes held in buffer. 
- Returns: nothing
- No parameters.

#### cli_rtecount
- Description: Gets the number of routes in buffer.
- Returns: unsigned long integer.
- No parameters. 

#### cli_addrte_s
- Description: Adds a route to route buffer. 
- Returns: NULL on success, otherwise points to an internal error message string.
- Parameter `id_list`: List of identifiers unique to added route.
- Parameter `id_num`: Number of identifiers in list.
- Parameter `func`: a pointer to a `cli_func_t` function. 

#### cli_parseint
- Description: Internal function used to parse integer strings to integer values

#### cli_parsernb
- Description: Internal function used to parse **R**eal **N**um**B**er strings to float values.

#### cli_parsestr
- Description: Internal function used to parse string literals.

#### cli_parseswrd
- Description: Internal function used to parse words. 

#### cli_parseshex
- Description: Internal function used to parse hex strings.

#### cli_cmpidlist
- Description: Internal function that compares the tokenized user input to a route id list.

#### cli_hasrte_s
- Description: Tries to find a route id list that matches the tokenized user input.
- Returns: NULL on success, otherwise points to an internal error message string.
- Parameter `tokens`: tokenized user input.
- Parameter `out`: reference to a pointer of `cli_route_t`, used to save the matching route. 

#### cli_usrinput
- Description: Prompts user and tokenize input. Provides a `cli_toklst_t` used by `cli_hasrte_s`. 
- Returns: NULL on success, otherwise points to an internal error message string.
- Parameter `stream`: A standard stream with read access. 
- Parameter `out`: reference to `cli_toklst_t` used to save result of tokenization. 

#### cli_getparam
- Description: Gets a parsed wildcard value (AKA a parameter, variable, argument, etc.)
- Returns: NULL on success, otherwise points to an internal error message string.
- Parameter `index`: index of desired parameter.
- Parameter `param`: reference to `cli_param_t` used to save desired parameter value.

#### cli_paramcnt
- Description: Gets the number of parameters.
- Returns: an unsigned long integer.
- No parameters.

- 
