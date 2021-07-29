# Super_Shell
Implementaion of a shell in linux environment with some additional functions
In this code i will write my own shell which will include the existing functions of original shell including additional functions i will write.

encryptFile:
This command gets 3 parameters: file name, number 100-1 and new file name. The command will encrypt
Contents of a file as follows: Add the value of the number to each character in the modulo 256 file,
The result will be written to the new file.

decryptFile:
This command gets 3 parameters: file name, number 100-1 and new file name. The command will be decoded
The contents of the file as follows: Subtract the value of the number for each character in the modulo file
256, the result will be written to the new file.

lockCmdForTime:
This command gets 2 parameters: command name X and time T in seconds, the command will lock the command
X for use for T seconds. If someone tries to use the command while locking it will be accepted
Appropriate notification and the command will be blocked. After T seconds the command will be released and will be possible
Use it as usual.

letterFreq:
This command gets as a parameter the name of a txt file, the command will print the three characters with
the highest percentage case (sensitive case not (z-a contained in the file) signal
Uppercase = lowercase letter (do not calculate in the distribution of characters that are not in the range of the letters z-a.
Only the letter distribution that exists in the text should be printed in the output.
If the character with the highest distribution is the character with the next largest distribution
(In second place) it is a and the character with the next largest distribution( in third place) it is o or i
Then make a line drop and print Frequency Letter Good.
These characters are the most common characters so we will perform this test.
Example output:
e - 13.7%
a - 8.4%
o - 7.5%
Good Letter Frequency

 uppercaseByIndex:
This command has 3 parameters: name of txt file to read, name of file to be printed to
Output and Index I. The command turns every lowercase letter in position I in each word into a capital letter.
If the file does not exist, create it.
If the file exists, it should be overwritten with the new contents.


 lowercaseByIndex:
This command gets 3 parameters: name of txt file, file name to which the output will be written and index
The command turns each uppercase letter in the I position in each word into a lowercase letter.
If the file does not exist, create it.
If the file exists, it must be overwritten with the new contents.

 randomFile:
This command takes 2 parameters: number X (number of characters (and name of output file, command)
You will create a file and write into it X random characters, the characters should be letters z-a
Big and small.
If the file exists, it should be overwritten with the new contents.

 compressFile: 
This command gets 2 parameters, txt file location and location to save a new compressed file,
The command will compress the file as follows: The command will replace any sequence greater than 4 that contains
The same character of the same character - his daughter and immediately followed by the length of the sequence.
example:
For a file containing the text !!!!! Worldddddd hellllllllo:
You will receive a file containing the text: 5! World6 H

 byebye:
This command exits the program, closes the shell, and returns to the original shell.
