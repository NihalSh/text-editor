# Text Editor written in C
Basic functions of a text editor implemented in C with standard library.

## Compile
gcc --std=c89 -Werror code.c -o binary

## Execute
$ ./binary

## Sample Output
<pre>
  Text Editor
  
  Options:

    1 New

    2 Open

    3 Save As

    4 Save and Close

    5 Cut

    6 Copy

    7 Paste

    8 Delete Text

    9 Display Content

    0 Add Text

    q Quit
  

Option: <strong>1</strong>

	Filename: <strong>foobar</strong>
  
	File successfully created
  
Option: <strong>0</strong>

	Position: <strong>0</strong>
  
	String: <strong>hello world</strong>
  
	Text Added
  
Option: <strong>9</strong>

"hello world"

Option: <strong>8</strong>

	Position: <strong>0</strong>
  
	Length: <strong>6</strong>
  
	Text Deleted
  
Option: <strong>9</strong>

"world"

Option: <strong>6</strong>

	Position: <strong>0</strong>
  
	Length: <strong>5</strong>
  
	Text Copied: world
  
Option: <strong>7</strong>

	Position: <strong>5</strong>
  
	Text Pasted
  
Option: <strong>9</strong>

"worldworld"

Option: <strong>5</strong>

	Position: <strong>0</strong>
  
	Length: <strong>3</strong>
  
	Text Cut: wor
  
Option: <strong>9</strong>

"ldworld"

Option: <strong>7</strong>

	Position: <strong>7</strong>
  
	Text Pasted
  
Option: <strong>9</strong>

"ldworldwor"

Option: <strong>q</strong>
</pre>
