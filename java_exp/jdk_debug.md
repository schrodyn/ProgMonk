# JDK debugging

Generally speaking, to be able to watch the variables while stepping 
through JDK source code, you need the class files to be compiled with 
debug information i.e. compile using `javac -g`.

So your best bet is to either find an already compiled version with 
debug information (I couldn't find anything for JDK 8) or you can try 
compiling the source for yourself.

All the commands are invoked from command line:

1. Create your working folder. I chose `~/tools` root folder
2. Inside your working folder create the source folder i.e. `jdk8_src` 
   and output folder `jdk_debug`

3. Get the JDK 8 source code archive file **src.zip** and extract it 
inside `jdk8_src`

you can get it with packet manager:

for example on Arch Linux

```
pacman -S openjdk8-src
```

This command will fetch **src.zip** and put it inside **JDK_HOME** 
folder.

To locate **JDK_HOME** folder:

```
$ whereis java
java: /usr/bin/java /usr/share/java /usr/lib/jvm/java-8-openjdk/bin/java /usr/share/man/man1/java.1.gz
```

We see that on my Arch Linux system **java** binary is located in 

```
/usr/lib/jvm/java-8-openjdk/bin/java
```

and so JDK is located in

```
/usr/lib/jvm/java-8-openjdk
```

4. Select what you will compile and delete the rest. If you select 
only the following folders:

  * java
  * javax
  * org

Then source code in **src.zip** will be sufficient to compile selected
JDK classes with debug option enabled.

For all of them you might need additional steps.

5. From your **JDK_HOME/jre/lib** get the file `rt.jar` and put in the
work folder (this is only for convenience to not specify too large 
file names in the command line). 

**Note:**

**rt.jar** contains all of the compiled class files for the
base Java Runtime ("rt") Environment. Normally, **javac** should know 
the path to this file. If you don't have it on your classpath you will
not have access to any of those classes you need to use like 
**java.lang.String** or **java.io.File**.


6. Execute the command

```
find jdk8_src/ -name *.java > filelist
```

7. Execute `javac` using the command:

```
javac -J-Xms512m -J-Xmx1024m -cp rt.jar -sourcepath ~/tools/jdk8_src -d ~/tools/jdk8_debug @filelist -g > log 2> log.err
```

This will compile all the files in the **jdk8_debug** folder and will 
generate a log files in your working folder. Check the log contents. 
You should get a bunch of warnings but no error.

From **docs.oracle.com**:

```
The default value of the class path is ".", meaning that only the 
current directory is searched. Specifying either the CLASSPATH 
variable or the -cp command line switch overrides this value.
```

8. Go inside the **jdk8_debug** folder and run the command: 

```
jar cf0 rt_debug.jar * 
```

This will generate your new runtime library with debug information.

9. Copy that new jar to the folder **JDK_HOME/jre/lib/endorsed**. If 
the endorsed folder does not exist, create it.

Happy debugging :)

## More folders

I have chosen the following folders:

  * com
  * java
  * javax
  * org

**Note:** If you include **com** folder then you also need to include
parts of the Java codebase that aren't included in **src.zip**

Since **src.zip** doesn't contain all JDK source code I've decided
to download full source code.

First of all, you should determine your JDK version:

```
$ java -version
Picked up _JAVA_OPTIONS: -Dawt.useSystemAAFontSettings=on
openjdk version "1.8.0_112"
OpenJDK Runtime Environment (build 1.8.0_112-b15)
OpenJDK 64-Bit Server VM (build 25.112-b15, mixed mode)
```

After that go to

```
http://hg.openjdk.java.net/jdk8u/jdk8u/jdk/tags
```

select corresponding version tag and note **node** version on the 
right side of the list; for example, **jdk8u112-b15** has node
**60767ec3909b**

Now clone mercurial jdk8 repository:

```
hg clone http://hg.openjdk.java.net/jdk8u/jdk8u/jdk jdk8u
```

move into `jdk8u` folder and revert repository back to the specified 
node 

```
hg revert --all -r 60767ec3909b
```

copy JDK classes source code 

```
cp -r jdk8u/src/share/classes/* jdk8_src/
```

into **jdk8_src** folder 

While compiling I've encountered errors with the following packages:

  * **com.sun.jmx.snmp** 
  * **sun.management.snmp** 
  * **sun.dc** 

rename `jdk8_src/sun/tools/jconsole/Version.java.template` into
`jdk8_src/sun/tools/jconsole/Version.java`

Copy `tools.jar` from **JDK_HOME/lib** folder into your working 
directory

In **tools.jar** are all tools that are needed by a JDK but not a JRE.
Included in **tools.jar** is e.g. the compiler `javac` and the tool 
`javadoc`. All tools needed to develop in Java are included in this 
**.jar** file.

Execute `javac` using the command:

```
javac -J-Xms512m -J-Xmx1024m -cp rt.jar:tools.jar -sourcepath ~/tools/jdk8_src -d ~/tools/jdk8_debug @filelist -g > log 2> log.err
```

Go inside **jdk8_debug** folder and run the command:

```
jar cf0 rt_debug.jar *
```

This will generate your new runtime library with debug information.

Copy that new jar to the folder **JDK_HOME/jre/lib/endorsed**. If the 
endorsed folder does not exist, create it.

Happy debugging :)



