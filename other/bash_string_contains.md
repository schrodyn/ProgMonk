#String contains in Bash

Using Bash, I have a string:

```
string = "My string"
```

How can I test if it contains another string?

Answer:

```bash
string='My long string';

if [[ $string == *"My long"* ]]
then
	echo "It's there!";
fi
```

Note that the string need to be placed between double quotes, and the 
`*` wildcards should be outside the double quotes.
