# Language specification

This document is intended to capture the language specification once decisions are made.

## Example of syntax

The syntax is roughly C-like.

'''Language
dynamic my_dynamic_var = "42";

int my_int = 1;

my_int += my_dynamic_var;   // RuntimeError: cannot add a string to an integer.

function my_funct(int my_int, dynamic my_unknown) -> int {
    if IsDigits(my_unknown) {
        return my_int + my_unknown;
    }
    else {
        return my_int;
    }
}

'''

## Lexical structure

- Comment syntax: // for single line, """for docstring""".  

## Literals and identifiers

- Numbers are not accepted at the start of identifiers (e.g. function 5_loops) but are allowed in the middle or end.  

## Types and values

Basic types are be:
[string, int_32, float_32, bool, none]
The language is strongly and hybridly typed.  
This means:

- Type conversion will not be implicit (instead require someting like int()).  
- It will allow static typing but use dynamic as a fallback.  
- Variables will be initialised as so:
'''
dynamic my_var = ...
int my_var = ...
'''
    This is formally called inferred strong typing via a dynamic initialiser - it functions similar to TypeVar in python:
    when you write dynamic my_var, you tell the parser to replace 'dynamic' with whatever type it infers from the value.

## Errors

Errors cause the program running to end, with a stack traceback.  
