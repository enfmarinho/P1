[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/y34ghiqj)
﻿# Programming Assignment: Cowsay

### Table of Contents
1. [Introduction](#introduction)
2. [Input](#input)
3. [Interface](#interface)
4. [Execution](#execution)
5. [System Modeling](#system-modeling)
6. [Output](#output)
7. [Assignment Grading](#assignment-grading)
8. [Authorship and Collaboration Policy](#authorship-and-collaboration-policy)
9. [Work Submission](#work-submission)


#  Introduction

In this programming assignment you should design and develop a clone of an Unix program called **cowsay**. This program is a terminal application that displays a text-based representation of a cow "_speaking_'' a text the client provides. Several aspects of the application should be configurable via a basic Command Line Interface (CLI). For instance, it should be possible to set  the cow's mood (appearance) or even to control the format/alignment of the text the cow is saying. Below you will find our friendly cow in action, when she receives this text [here](data/basic_text.txt) as input.
```
$ ./cowsay < data/basic_text.txt
 __________________________________________
/ This is a basic example of what I can    \
| do once you feed me with interesting     |
| stuff to say.                            |
|                                          |
| You may also control my appearance by    |
| letting me graze freely in a field as    |
| much as I want!                          |
| (hummmm, that's not entirely accurate,   |
| I must say, but I wouldn't mind)         |
|                                          |
| Sometimes I may feel **tired**, other    |
| times **wired**, but if you begin to     |
| stare at me too intensely, I may become  |
| **paranoid**!!!                          |
|                                          |
|                                          |
| Other times I just want to say           |
\ "muuuuuuuu".                             /
 ------------------------------------------
       \   ^__^
        \  (oo)\_______
           (__)\       )\/\
               ||----w |
               ||     ||
```
This project will help you develop your object-oriented programming skills since you need to model this application with classes. This assignment also relies heavily on [strings](https://en.cppreference.com/w/cpp/string/basic_string) and [stream](https://en.cppreference.com/w/cpp/io/basic_ostringstream) manipulation, as well as the processing and parsing of command line arguments to support a CLI. Of course, we will also have the chance to choose a good data structure to hold the data and create the desired output.

# Input

The only real data input the `cowsay` receives is the message you want to display. If you run `cowsay` with no arguments, the program should read the text from the standard input (i.e. read from `std::cin`) and wait for a `Ctrl+D` to end the input reading.

Alternatively, the client may provide a message straight through the CLI, as the last argument. See how to do this in the next section.

Another type of input is the configuration parameters that control the way the cow behaves and how the message should be formatted. These configuration parameters are presented next.


# Interface

If you run `cowsay -h` you program should display its basic interface, as follows

```
$ ./cowsay -h
Welcome to C++ cowsay clone, version 1.0, (c) 2021-2023 your_name, DIMAp, UFRN.
Usage: cowsay [-h] [-bdgpstwy] [-E eyes] [-T tongue] [-F cowfile] [-J alignment] [-W wrapcolumn] [-n] [message]
       where - eyes       is a 2-character string. Ex.: '@@'.
             - tongue     is a 2-character string. Ex.: ' U'.
             - cowfile    is a text file that contains a cow description.
             - alignment  `c` means center aligned, `r` means right aligned.
             - wrapcolunm is an integer value in [1,100].
```

Note that all the arguments enclosed in square brackets, `[`_parameter_`]`, are optional and may not be provided. All options in upper case letter requires an argument: `E`, `T`, `F`, `J`, and `W`. Other options, called **flags**, work like a boolean value, turning on some internal feature. For instance, if you provide `-g` the cow will be displayed with a *greedy* face.

There are several pre-defined **modes** that change the appearance of the cow depending on its particular emotional/physical state. These modes basically affects the cow's eyes and tongue, while the body remains unchanged. Alternatively, you may want to manually define how the cow's eyes and tongue should be displayed by providing a suitable string to the proper option. See below all the modes you will need to support.

```
 ^__^               ^__^               ^__^               ^__^               ^__^
 (oo)\_______       (==)\_______       (xx)\_______       ($$)\_______       (@@)\_______
 (__)\       )\/\   (__)\       )\/\   (__)\       )\/\   (__)\       )\/\   (__)\       )\/\
     ||----w |          ||----w |        U ||----w |          ||----w |          ||----w |
     ||     ||          ||     ||          ||     ||          ||     ||          ||     ||
      Default             Borg               Dead               Greedy            Paranoia
 ^__^               ^__^               ^__^               ^__^
 (**)\_______       (--)\_______       (OO)\_______       (..)\_______
 (__)\       )\/\   (__)\       )\/\   (__)\       )\/\   (__)\       )\/\
   U ||----w |          ||----w |          ||----w |          ||----w |
     ||     ||          ||     ||          ||     ||          ||     ||
      Stoned              Tired              Wired              Young
```
Now that you've learn how the cow's appearance can be changed, it is time to explain each option available.

The flags (boolean-like options):

+ `-b`: initiates the cow into the **Borg** mode. Only the eyes (`==`) are affected.
+ `-d`: causes the cow to appear **dead**. Both the eyes (`xx`) and tongue (`U `) are affected.
+ `-g`: invokes **greedy** mode. Only the eyes (`$$`) are affected.
+ `-p`: causes a state of **paranoia** to come over the cow. Only the eyes (`@@`) are affected.
+ `-s`: makes the cow appear thoroughly **stoned**. Both the eyes (`**`) and tongue (`U `) are affected.
+ `-t`: yields a **tired** cow. Only the eyes (`--`) are affected.
+ `-w`: initiates **wired** mode. Only the eyes (`OO`) are affected.
+ `-y`: brings on the cow's **youthful** appearance. Only the eyes (`..`) are affected.
+ `-f`: flips the cow horizontally.
+ `-n`: this option preserves the original message "as is", which means all the excessive white spaces are preserved and the  message will not be word-wrapped.
+ `-h`: display the basic help.

Argument-dependent options:

+ `-E eyes`: selects the appearance of the cow's eyes, in which case the first two characters of the argument string will be used. The default eyes are `oo`. The eyes-string must be two characters long.
+ `-T tongue`: selects the appearance of the cow's tongue. The tongue-string also must be two characters long and does not appear by default.
+ `-W wrapcolumn`: specifies roughly where the message should be wrapped. The default value is `-W 40` i.e. wrap words at or before the 40th column.
+ `-F cowfile`: specifies a particular cow picture file (`cowfile`) to use. _The implementation of this feature is optional and yields extra credits (see Section [Assignment Grading](#assignment-grading) for more details)._
+ `-J alignment`: specifies how the message should be aligned inside the text window (billboard). Possible alignment values are `-J c` (centered), and `-J r` (right justified). It is not necessary an option for the left-alignment, since this is the default behavior.

See below the same message presented earlier in default mode (left aligned) now is presented with **center alignment** (40 columns), 
```
 __________________________________________
/  This is a basic example of what I can   \
|  do once you feed me with interesting    |
|              stuff to say.               |
|                                          |
|  You may also control my appearance by   |
|  letting me graze freely in a field as   |
|             much as I want!              |
| (hummmm, that's not entirely accurate,   |
|     I must say, but I wouldn't mind)     |
|                                          |
|  Sometimes I may feel **tired**, other   |
|  times **wired**, but if you begin to    |
| stare at me too intensely, I may become  |
|             **paranoid**!!!              |
|                                          |
|                                          |
|     Other times I just want to say       |
\               "muuuuuuuu".               /
 ------------------------------------------
       \   ^__^
        \  (oo)\_______
           (__)\       )\/\
               ||----w |
               ||     ||
```
and **right alignment** (40 columns).
```
 __________________________________________
/   This is a basic example of what I can  \
|    do once you feed me with interesting  |
|                            stuff to say. |
|                                          |
|   You may also control my appearance by  |
|   letting me graze freely in a field as  |
|                          much as I want! |
|  (hummmm, that's not entirely accurate,  |
|         I must say, but I wouldn't mind) |
|                                          |
|   Sometimes I may feel **tired**, other  |
|    times **wired**, but if you begin to  |
| stare at me too intensely, I may become  |
|                          **paranoid**!!! |
|                                          |
|                                          |
|          Other times I just want to say  |
\                             "muuuuuuuu". /
 ------------------------------------------
       \   ^__^
        \  (oo)\_______
           (__)\       )\/\
               ||----w |
               ||     ||
```
Finally, note that the order of the options when running `cowsay` matters. If the client adds an option after the message begins, the option will be treated as part of the message.
For instance, the command `cowsay -w beginning of the message -E 'qq'` would produce the following output:

```
 ________________________________
< beginning of the message -E qq >
 --------------------------------
        \   ^__^
         \  (OO)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
```
Therefore, the lesson here is that your CLI parser should look for all the possible running options first; if we get to something that does not begin with `-` it means the message has begun.

Remember that some options take one argument, thus they must be treated as a pair of information when parsing. This means, for instance, that when you find `-E`, say, in `argv[i]` you will assume that the next argument `argv[i+1]` _is_ the expected argument for the eyes setting option. 

# Execution

After receiving and parsing the command-line arguments and acquiring the message (either via CLI or reading the _stdin_), your program should modify the cow appearance accordingly (the preset modes have priority over a manual specification of eyes/tongue), and format the message so that it is displayed as requested by the user.

To make it easy to control how the message is displayed I recommend to break each individual string into its components or **tokens**, i.e. the individual words (no spaces). After creating this list of tokens it becomes easier to create new strings with the alignment and length we wish. For that purpose I recommend the use of [`std::ostringstream`](https://en.cppreference.com/w/cpp/io/basic_ostringstream).

Once you managed to create a list of tokens (words) the wrapping algorithms is simple:

1. Create an empty output string (`std::ostringstream`) and set to zero a length-count variable.
2. Traverse the current list (a `std::vector`?) of tokens corresponding to a single original input line.
3.  Check if it's possible to add the current token to the current output string by doing this:
	+ Add the current token length + 1 (white space) to the length-count variable.
	+  If the result would go beyond the wrap value (say, 40) it means the current token *does not fit* in the output string; so you flush the output string (because it's full),  and start a new output string (same as in step 1).
	+ Add the current token to the output string and update the length-count variable.

# System Modeling

Because you must follow the Object-Oriented Design (OOD) paradigm the first step is to identify which **entities** might help you model the problem. Because I assuming you have little or no experience in OOD I recommend that you create two classes, and one structure.

## The `Cow` class

The first entity is the `Cow` class which controls how the cow should appear. For that purpose, this class contains a string representation of the cow, as displayed below.

```c++
body = 
     std::string{"       $thoughts   ^__^            \n"} + 
     std::string{"        $thoughts  ($eyes)\\_______\n"} +
     std::string{"           (__)\\       )\\/\\     \n"} +
     std::string{"            $tongue ||----w |      \n"} +
     std::string{"               ||     ||           \n"} ;
```

The variables `$eyes`, `$tongue`, and `$thoughts` represent, respectively, the exact location where the eyes-string, tong-string, and thought-string should be placed. Note that to print the special `\` symbol it is necessary precede it with another `\`.

The `Cow` class also keeps track of the cow's mood the user request via CLI, and a flag that indicates whether the cow should be _flipped_ or not. A (horizontally) flipped cow looks like this

```
                   __________________________________________
                  / This is a basic example of what I can    \
                  | do once you feed me with interesting     |
                  \ stuff to say. BTW: "muuuuuuuu".          /
                   ------------------------------------------
             ^__^   /
     _______/(oo)  /
 /\/(       /(__)
    | w---- |
    ||     ||
```

## The `BillBoard` class

The second entity is the `BillBoard` class that represents a frame where the user's message should be displayed on. The text is always surrounded by a frame. The frame of a single line message is different from a multi-line message, as depicted below.

```
 _______________________________
< This is a single line message >
 -------------------------------

 _______________________________
/ This is a message with two    \
\ lines.                        /   
 -------------------------------

 _______________________________
/ This is a long multiline      \
| message that requires a       | 
\ different frame.              /
 -------------------------------
```

Additionally, this class keeps track of the message alignment, which may be either left-, center-, or right-justified. Another information stored in this class is whether or not it needs to remove all excessive white spaces and wrap the message at the specified column. Lastly, this class needs to know if the cow has been flipped; for this situation we need to add some padding to the left of the billboard frame so that the frame is connected to the `$thoughts` character (see the flipped cow example above).

## The `CLIOption` struct

This struct stores all the information that comes from the command-line arguments. Because this is a regular object it might be passed around to the other objects as an argument. This would enable both the `Cow` and `BillBoard` classes to have access to the information they need to work.

Because all the command-line arguments of the `cowsay` are optional, I recommend that you create the fields of this struct as [`std::optional<>`](https://en.cppreference.com/w/cpp/utility/optional) objects. This template class was added to the C++17 standard and it "_manages an optional contained value, i.e. a value that may or may not be present._" This make this object ideal to represent the CLI options.



# Output

## Regular output

As discussed previously, the output of this program is simple: the billboard content, followed by the cow representation.

The default behavior while presenting the message on the standard output is to remove all preceding white spaces and all the excessive white spaces that may come between words, leaving only a single white space between any two words. You saw this behavior in all the `cowsay` output presented so far in this document. Bellow you find what happens if the user runs `cowwsay` with the `-n` option (i.e. no wrapping, no excessive white space removal) 

```
____________________________________________________________________________________________________________________________________
/ This is a basic   example of what I can do once     you feed me with interesting stuff to say.                                     \
|                                                                                                                                    |
| You may also     control my appearance by letting me graze freely in a field as much as I want!                                    |
|     (hummmm, that's not entirely accurate,       I must say, but I wouldn't mind)                                                  |
|                                                                                                                                    |
| Sometimes I may feel **tired**, other times **wired**, but if you begin to stare at me too intensely, I may become **paranoid**!!! |
|                                                                                                                                    |
|                                                                                                                                    |
\        Other times I just want     to    say     "muuuuuuuu".                                                                      /
 ------------------------------------------------------------------------------------------------------------------------------------
       \   ^__^
        \  (oo)\_______
           (__)\       )\/\
               ||----w |
               ||     ||

```
## Error output

Whenever the user provides an incorrect argument your program should print an instructive error message. For instance, is the user runs `cowsay -x algumas palavras` i. e. an invalid argument, your program would print
```
$ ./cowsay -x algumas palavras
Unknow option: x
 __________________
< algumas palavras >
 ------------------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||

```
ignore the unknown option, and carry on.

If the user provides a wrap column value outside the valid range, just explain what went wrong and assume the default value of 40.
```
$ ./cowsay -W 120 algumas palavras
Wrap column value must be in range [1,100]. Assuming -W 40 (default behavior)
 __________________
< algumas palavras >
 ------------------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||

```
Other possible errors are a missing cowfile (if you are implementing this extra feature), invalid arguments for argument-based options, etc.

# Assignment Grading

This assignment has a total of **110 points**. Bellow is a table with the description of the item under consideration and the assigned grade _if the item is totally correct_. Partial grade will be considered if the item is not fully correct.

Item     | Value (points)
-------- | :-----:
Read, validate, and process command-line arguments | 20
Show help as requested `-h` | 2
Support all cow moods `-bdgpstwy` (8 in total, 2 pts each)  | 16
Support cow flip `-f`    | 5
Suspend excessive white space elimination and text wrapping `-n` | 5
Allow manual definition of eyes with `-E eye_string` | 5
Allow manual definition of tongue with `-T tongue_string` | 5
Support definition of wrap column with `-W column_value` | 5
Read message from standard input correctly | 5
Remove all excessive white spaces and wrap text at or close to column 40 (default behavior) | 5
Support message with left-alignment (default behavior, no option needed) | 5
Support message with center-alignment `-J c` | 5
Support message with right-alignment `-J r` | 5
Print billboard decoration (frame) correctly, for single- and multi-line messages | 8
The project contains at least two classes | 10
Program handles errors properly | 4

The following situations may take credits out of your assignment, if they happen during the evaluation process:

Item     | Value (points)
-------- | :-----:
Compiling and/or runtime errors  | -5
Missing code documentation in Doxygen style | -10
Memory leak | -10
[`author.md`](docs/author.md) file not filled out | -10
Program crashes while testing | -5

The [`author.md`](docs/author.md) file should contain a brief description of the project, and how to run it. It also should describe possible errors, limitations, or issues found. Do not forget to include the author name!

## Extra credits
You may earn extra credits if:

 - **[15 points]** Your program support the option `-F cow_file` that read a `cow_file` which contains a new appearance for the cow. The cow file description should follow the traditional `.cow` file, the same way it is support by the original `cowsay`program.
 - **[10 points]** You add the option to justify the text with `-J j`. This option introduces some white spaces among some words so that all lines would occupy the entire line space and the last word touches the right border. In case we are interested in this option and has any doubts on this concept, ask the instructor for clarification.

# Authorship and Collaboration Policy

Originally this is an *individual assignment* but if you choose, you may pair up with someone. You may be called for an interview. The purpose of the interview is to confirm the authorship of the assignment and to ask for explanations on specific points of the code. During the interview, you should be capable of explaining any piece of code. After the interview, the assignment’s credits may be reviewed.

The cooperation among students is strongly encouraged. It is accepted the open discussion of ideas or development strategies. Notice, however, that this type of interaction should not be understood as a free permission to copy and use somebody else’s code. This is may be interpreted as plagiarism.

Any two (or more) programs deemed as plagiarism will automatically receive no credits, regardless of the real authorship of the programs involved in the case. If your project uses a (small) piece of code from someone else’s, please provide proper acknowledgment in the [`author.md`](author.md) file.

# Work Submission

You may submit your work in two possible ways: via GitHub Classroom (GHC), or, via Sigaa submission task. In case you decide to send your work via GHC you _must_ also send a text file via Sigaa submission task with the github link to your repository. In case you choose to send your work via Sigaa only, send a zip file containing all the code necessary to compile and run the project.

I any of these two ways, remember to remove all the executable files (i.e. the `build` folder) from your project before handing in your work.

--------
&copy; DIMAp/UFRN 2021.
